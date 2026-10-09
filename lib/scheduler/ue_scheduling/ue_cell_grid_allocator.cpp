// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ue_cell_grid_allocator.h"
#include "../support/csi_rs_helpers.h"
#include "../support/dci_builder.h"
#include "../support/dmrs_helpers.h"
#include "../support/repetition_helpers.h"
#include "../ue_context/ue_drx_controller.h"
#include "grant_params_selector.h"
#include "ocudu/scheduler/result/dci_info.h"
#include "ocudu/support/error_handling.h"

using namespace ocudu;

/// Number of possible Downlink Assignment Indexes {0, ..., 3} as per TS 38.213 Section 9.1.3.
static constexpr unsigned DAI_MOD = 4U;

/// Returns the value of the Differential Koffset MAC CE the PDSCH carries, if any.
static std::optional<std::chrono::milliseconds> find_differential_koffset_ce(const dl_msg_alloc& msg)
{
  for (const dl_msg_tb_info& tb : msg.tb_list) {
    for (const dl_msg_lc_info& lc_info : tb.lc_chs_to_sched) {
      if (lc_info.lcid == lcid_dl_sch_t::from_elcid(elcid_dl_sch_t::DIFFERENTIAL_KOFFSET)) {
        return std::get<differential_koffset_ce_payload>(lc_info.ce_payload).koffset;
      }
    }
  }
  return std::nullopt;
}

ue_cell_grid_allocator::ue_cell_grid_allocator(const scheduler_ue_expert_config& expert_cfg_,
                                               ue_repository&                    ues_,
                                               pdcch_resource_allocator&         pdcch_sched_,
                                               uci_allocator&                    uci_alloc_,
                                               srs_allocator&                    srs_alloc_,
                                               cell_resource_allocator&          cell_alloc_,
                                               ocudulog::basic_logger&           logger_) :
  expert_cfg(expert_cfg_),
  ues(ues_),
  pdcch_sched(pdcch_sched_),
  uci_alloc(uci_alloc_),
  srs_alloc(srs_alloc_),
  cell_alloc(cell_alloc_),
  logger(logger_)
{
  dl_grants.reserve(MAX_UE_PDUS_PER_SLOT);
  ul_grants.reserve(MAX_PUSCH_PDUS_PER_SLOT);
}

/// Computes the PRBs of a PDSCH allocation, relative to the start of the RB limits of the searchSpace.
static prb_bitmap compute_pdsch_prbs(const search_space_info& ss_info, const vrb_alloc& vrbs, bool enable_interleaving)
{
  if (vrbs.is_type0()) {
    // Resource allocation type 0 always uses the non-interleaved VRB-to-PRB mapping. It requires DCI format 1_1, whose
    // RB limits are the BWP CRBs, as per TS 38.214, Section 5.1.2.2.
    ocudu_sanity_check(ss_info.dl_crb_lims == ss_info.bwp->dl.cfg().crbs, "Type 0 RB limits must match the BWP CRBs");
    return vrbs.type0().convert_to<prb_bitmap>();
  }
  prb_bitmap prbs(ss_info.dl_crb_lims.length());
  if (enable_interleaving) {
    const auto prb_intervals = ss_info.interleaved_mapping.value().vrb_to_prb(vrbs.type1());
    prbs |= prb_intervals.first;
    prbs |= prb_intervals.second;
  } else {
    prbs |= vrbs.type1().convert_to<prb_interval>();
  }
  return prbs;
}

/// Calls \c func with each CRB interval of the PDSCH PRBs.
template <typename Func>
static void for_each_pdsch_crb_interval(const search_space_info& ss_info, const prb_bitmap& prbs, Func&& func)
{
  for_each_interval(
      prbs, [&](size_t start, size_t stop) { func(prb_to_crb(ss_info.dl_crb_lims, prb_interval(start, stop))); });
}

/// Checks whether any PRB of the PDSCH collides with other allocations of the resource grid.
static bool pdsch_collides(const cell_slot_resource_grid& grid,
                           const search_space_info&       ss_info,
                           ofdm_symbol_range              symbols,
                           const prb_bitmap&              prbs)
{
  const subcarrier_spacing scs      = ss_info.bwp->dl.cfg().scs;
  bool                     collides = false;
  for_each_pdsch_crb_interval(
      ss_info, prbs, [&](crb_interval crbs) { collides = collides or grid.collides(scs, symbols, crbs); });
  return collides;
}

/// Marks the PRBs of the PDSCH as used in the resource grid.
static void fill_pdsch_prbs(cell_slot_resource_grid& grid,
                            const search_space_info& ss_info,
                            ofdm_symbol_range        symbols,
                            const prb_bitmap&        prbs)
{
  const subcarrier_spacing scs = ss_info.bwp->dl.cfg().scs;
  for_each_pdsch_crb_interval(ss_info, prbs, [&](crb_interval crbs) { grid.fill(grant_info{scs, symbols, crbs}); });
}

std::optional<sch_mcs_tbs> ue_cell_grid_allocator::calculate_dl_mcs_tbs(const cell_slot_resource_allocator& pdsch_alloc,
                                                                        const search_space_info&            ss_info,
                                                                        uint8_t           pdsch_td_res_index,
                                                                        const prb_bitmap& prbs,
                                                                        sch_mcs_index     mcs,
                                                                        unsigned          nof_layers) const
{
  const auto& pdsch_cfg = ss_info.get_pdsch_config(pdsch_td_res_index, nof_layers);

  // The PDSCH is rate-matched around the CSI-RS of its slot, which the TBS does not account for.
  const unsigned nof_csi_rs_re = csi_helper::get_nof_csi_rs_re(
      pdsch_alloc.result_reader().dl().csi_rs(), pdsch_cfg.symbols, prb_to_crb(ss_info.dl_crb_lims, prbs));

  // [Implementation-defined] Lower the MCS of the link adaptation to the highest one whose spectral efficiency, as per
  // TS 38.214, Section 5.1.3.1, does not exceed its own, scaled by the fraction of resource elements that the CSI-RS
  // leave. The PDSCH then carries no more bits per resource element than intended. Unlike the code rate, the spectral
  // efficiency is comparable across modulation orders.
  sch_mcs_index adjusted_mcs{mcs};
  if (nof_csi_rs_re != 0) {
    const unsigned nof_re                  = prbs.count() * (pdsch_cfg.symbols.length() * NOF_SUBCARRIERS_PER_RB -
                                            calculate_nof_dmrs_per_rb(pdsch_cfg.dmrs));
    const float    max_spectral_efficiency = pdsch_mcs_get_config(pdsch_cfg.mcs_table, mcs).get_spectral_efficiency() *
                                          (1.0F - static_cast<float>(nof_csi_rs_re) / static_cast<float>(nof_re));
    while (adjusted_mcs != 0 and pdsch_mcs_get_config(pdsch_cfg.mcs_table, adjusted_mcs).get_spectral_efficiency() >
                                     max_spectral_efficiency) {
      adjusted_mcs = adjusted_mcs - 1;
    }
  }

  // If it's a new Tx, compute the MCS and TBS.
  // As \c txDirectCurrentLocation, in \c SCS-SpecificCarrier, TS 38.331, "If this field (\c
  // txDirectCurrentLocation) is absent for downlink within ServingCellConfigCommon and ServingCellConfigCommonSIB,
  // the UE assumes the default value of 3300 (i.e. "Outside the carrier")".
  bool                           contains_dc = false;
  const std::optional<unsigned>& tx_dc =
      cell_alloc.cfg.params.dl_cfg_common.freq_info_dl.scs_carrier_list.back().tx_direct_current_location;
  if (tx_dc.has_value()) {
    for_each_pdsch_crb_interval(ss_info, prbs, [&](crb_interval crbs) {
      contains_dc = contains_dc or dc_offset_helper::is_contained(tx_dc.value(), crbs);
    });
  }
  // The effective code rate of the resulting MCS and TBS accounts for the CSI-RS resource elements, as per TS 38.214,
  // Section 5.1.3.1.
  auto mcs_tbs_info = compute_dl_mcs_tbs(pdsch_cfg, adjusted_mcs, prbs.count(), contains_dc, nof_csi_rs_re);

  return mcs_tbs_info;
}

expected<pdcch_dl_information*, alloc_status>
ue_cell_grid_allocator::alloc_dl_pdcch(const ue_cell& ue_cc, const search_space_info& ss_info) const
{
  const rnti_t crnti = ue_cc.rnti();

  const aggregation_level aggr_lvl =
      ue_cc.get_aggregation_level(ue_cc.link_adaptation_controller().get_wideband_cqi(), ss_info, true);

  cell_slot_resource_allocator& pdcch_alloc = cell_alloc[0];
  pdcch_dl_information*         pdcch =
      pdcch_sched.alloc_dl_pdcch_ue(pdcch_alloc, crnti, ue_cc.cfg(), ss_info.cfg->get_id(), aggr_lvl);
  if (pdcch == nullptr) {
    logger.info("ue={} rnti={}: Failed to allocate PDSCH. Cause: No space in PDCCH.", ue_cc.ue_index, crnti);
    // Note: (Implementation-defined) Assuming all UEs share the same CORESET, if there are no more CCEs left in the
    // CORESET, stop attempting to allocate new PDCCHs in the slot.
    unsigned nof_cces_left = ss_info.coreset->cfg().get_nof_cces();
    for (const auto& dl_pdcch : pdcch_alloc.result_builder().dl().dl_pdcchs()) {
      nof_cces_left -= std::min(nof_cces_left, to_nof_cces(dl_pdcch.ctx.cces.aggr_lvl));
    }
    for (const auto& ul_pdcch : pdcch_alloc.result_builder().dl().ul_pdcchs()) {
      nof_cces_left -= std::min(nof_cces_left, to_nof_cces(ul_pdcch.ctx.cces.aggr_lvl));
    }
    return make_unexpected(nof_cces_left == 0 ? alloc_status::skip_slot : alloc_status::skip_ue);
  }

  return pdcch;
}

std::optional<uci_allocation> ue_cell_grid_allocator::alloc_uci(const ue_cell&           ue_cc,
                                                                const search_space_info& ss_info,
                                                                uint8_t                  pdsch_td_res_index,
                                                                unsigned                 last_occasion_offset) const
{
  const pdsch_time_domain_resource_allocation& pdsch_td_cfg =
      ss_info.bwp->dl.td_mapper().pdsch_td_resources(ss_info.get_dl_dci_format())[pdsch_td_res_index];

  // Allocate UCI. UCI destination (i.e., PUCCH or PUSCH) depends on whether there exist a PUSCH grant for the UE.
  // NOTE: With PDSCH repetitions, the UE counts k1 from the last repetition occasion, so the UCI is booked relative
  // to that slot.
  const slot_point    last_pdsch_slot = cell_alloc[pdsch_td_cfg.k0 + last_occasion_offset].slot;
  span<const uint8_t> k1_list =
      cell_alloc.cfg.init_bwp.ul.td_mapper().k1_candidates(ss_info.get_dl_dci_format(), last_pdsch_slot.count());

  const pucch_repetition_factor max_rep_factor = ue_cc.link_adaptation_controller().get_recommended_pucch_rep_factor();
  std::optional<uci_allocation> uci =
      uci_alloc.alloc_harq_ack(cell_alloc, ue_cc, pdsch_td_cfg.k0 + last_occasion_offset, k1_list, max_rep_factor);
  if (not uci.has_value()) {
    logger.debug(
        "ue={} rnti={}: Failed to allocate PDSCH. Cause: UCI allocation failed.", ue_cc.ue_index, ue_cc.rnti());
  }
  return uci;
}

expected<ue_cell_grid_allocator::dl_newtx_grant_builder, dl_alloc_failure_cause>
ue_cell_grid_allocator::allocate_dl_grant(const ue_newtx_dl_grant_request& request)
{
  static constexpr search_space_id ue_ded_ss_id = to_search_space_id(2);
  const ue_cell&                   ue_cc        = *ues[request.user.ue_index()].find_cell(cell_alloc.cell_index());
  const search_space_info&         ss_info      = ue_cc.cfg().search_space(ue_ded_ss_id);

  // Select PDCCH searchSpace and PDSCH time-domain resource config.
  auto sched_ctxt = sched_helper::get_newtx_dl_sched_context(request.user,
                                                             cell_alloc[0].slot,
                                                             request.pdsch_slot,
                                                             request.interleaving_enabled,
                                                             request.pending_bytes,
                                                             request.restrictions);
  if (not sched_ctxt.has_value()) {
    // No valid parameters were found for this UE. When repetitions were requested but no repetition row fits this slot
    // (e.g. a special slot), the grant is deferred to a later one.
    return make_unexpected(dl_alloc_failure_cause::other);
  }

  // Build the repetition bundle when a repetition row was selected. The allocation is deferred when the bundle cannot
  // start in this slot.
  std::optional<dl_repetition_info> reps;
  if (sched_ctxt->nof_repetitions.has_value()) {
    reps = select_pdsch_repetitions(ue_cc, ss_info, sched_ctxt->pdsch_td_res_index);
    if (not reps.has_value()) {
      return make_unexpected(dl_alloc_failure_cause::other);
    }
  }

  // Set up a DL grant.
  auto result = setup_dl_grant_builder(request.user, sched_ctxt.value(), std::nullopt, std::move(reps));
  if (not result.has_value()) {
    return make_unexpected(result.error());
  }

  // Add DL grant to list of pending grants and create a newTx DL grant builder.
  dl_grants.push_back(*result);
  return dl_newtx_grant_builder{*this, static_cast<unsigned>(dl_grants.size()) - 1};
}

std::optional<ue_cell_grid_allocator::dl_repetition_info>
ue_cell_grid_allocator::select_pdsch_repetitions(const ue_cell&           ue_cc,
                                                 const search_space_info& ss_info,
                                                 uint8_t                  pdsch_td_res_index) const
{
  // The DCI signals the selected TDRA row, repetitionNumber-r16 included, so the grant is bound to the whole bundle.
  // A bundle that cannot start in this slot therefore defers the allocation to a later one.
  const dl_time_domain_mapper&                 dl_td_mapper = ss_info.bwp->dl.td_mapper();
  const pdsch_time_domain_resource_allocation& td_res = dl_td_mapper.dedicated_pdsch_td_resources()[pdsch_td_res_index];
  const uint8_t                                nof_repetitions = *td_res.rep_number;

  // All occasions must fit the DL allocation window of the resource grid. This can only fail for pathological
  // configurations (k0 close to the ring limit).
  if (static_cast<unsigned>(td_res.k0 + nof_repetitions - 1) > cell_alloc.max_dl_slot_alloc_delay) {
    return std::nullopt;
  }

  const cell_configuration& cell_cfg   = cell_alloc.cfg;
  const slot_point          pdsch_slot = cell_alloc[td_res.k0].slot;

  dl_repetition_info reps{nof_repetitions, {}};
  for (uint8_t i = 1; i != nof_repetitions; ++i) {
    // Occasions whose slot cannot carry the PDSCH symbols (UL/special slots) are dropped, losing the respective RV.
    if (td_res.symbols.stop() > cell_cfg.get_nof_dl_symbol_per_slot(pdsch_slot + i)) {
      continue;
    }
    // If the transmission of an occasion cannot be guaranteed (no space for more PDSCHs), the bundle cannot start in
    // this slot, as the UE would combine noise for the missing occasion. Note: an occasion slot cannot already carry
    // a PDSCH for this same UE.
    if (cell_alloc[td_res.k0 + i].result_builder().dl().ue_grants().full()) {
      if (logger.debug.enabled()) {
        logger.debug("ue={} rnti={}: PDSCH repetition deferred at slot={}. Cause: occasion slot={} is full.",
                     ue_cc.ue_index,
                     ue_cc.rnti(),
                     pdsch_slot,
                     pdsch_slot + i);
      }
      return std::nullopt;
    }
    reps.tx_offsets.push_back(i);
  }

  // Defer the allocation when the special/UL slot is less than 2 slots away, i.e. the bundle would have a single
  // actual transmission while still blocking the RVs of the dropped occasions.
  if (reps.tx_offsets.empty() or reps.tx_offsets.front() != 1) {
    if (logger.debug.enabled()) {
      logger.debug("ue={} rnti={}: PDSCH allocation deferred at slot={}. Cause: less than 2 slots to the special slot.",
                   ue_cc.ue_index,
                   ue_cc.rnti(),
                   pdsch_slot);
    }
    return std::nullopt;
  }

  return reps;
}

expected<ue_cell_grid_allocator::dl_grant_info, dl_alloc_failure_cause>
ue_cell_grid_allocator::setup_dl_grant_builder(const slice_ue&                       user,
                                               const sched_helper::dl_sched_context& params,
                                               std::optional<dl_harq_process_handle> h_dl,
                                               std::optional<dl_repetition_info>     reps) const
{
  const bool            is_retx              = h_dl.has_value();
  const search_space_id ss_id                = params.ss_id;
  const uint8_t         pdsch_td_res_index   = params.pdsch_td_res_index;
  const uint8_t         nof_repetitions      = reps.has_value() ? reps->nof_occasions : uint8_t{1};
  const unsigned        last_occasion_offset = nof_repetitions - 1U;

  // Derive remaining parameters from \c dl_grant_params.
  ue&                                          u           = ues[user.ue_index()];
  ue_cell&                                     ue_cc       = *u.find_cell(cell_alloc.cell_index());
  const ue_cell_configuration&                 ue_cell_cfg = ue_cc.cfg();
  const search_space_info&                     ss_info     = ue_cell_cfg.search_space(ss_id);
  const pdsch_time_domain_resource_allocation& pdsch_td_cfg =
      ss_info.bwp->dl.td_mapper().pdsch_td_resources(ss_info.get_dl_dci_format())[pdsch_td_res_index];

  // Fetch PDCCH and PDSCH resource grid allocators.
  cell_slot_resource_allocator& pdcch_alloc = cell_alloc[0];
  cell_slot_resource_allocator& pdsch_alloc = cell_alloc[pdsch_td_cfg.k0];

  ocudu_sanity_check(not pdcch_alloc.result_builder().dl().dl_pdcchs().full(), "No space available for PDCCH");
  ocudu_sanity_check(pdsch_alloc.result_reader().dl().sibs().size() +
                             pdsch_alloc.result_reader().dl().paging_grants().size() +
                             pdsch_alloc.result_reader().dl().rar_grants().size() +
                             pdsch_alloc.result_builder().dl().ue_grants().size() <
                         expert_cfg.max_pdschs_per_slot,
                     "Max number of PDSCHs per slot was reached");
  ocudu_sanity_check(not pdsch_alloc.result_builder().dl().ue_grants().full(),
                     "No space available in scheduler PDSCH outputs");

  // Both the PUCCH below and the HARQ process after it follow this value, so they cannot disagree. Only a new tx still
  // picks a HARQ process; a retx is bound to the mode of the one it already holds.
  harq_utils::harq_mode_t harq_mode = harq_utils::harq_mode_t::normal;
  if (ue_cc.harqs.is_dl_harq_mode_selective()) {
    if (is_retx) {
      harq_mode = h_dl->mode();
    } else {
      harq_mode = harq_utils::to_harq_mode(params.restrictions.harq_feedback);
      // Checked before the PUCCH is booked, as it cannot be given back.
      if (not ue_cc.harqs.has_empty_dl_harqs(harq_mode)) {
        logger.debug("ue={} rnti={}: Failed to allocate PDSCH in slot={}. Cause: no free DL HARQ process",
                     user.ue_index(),
                     user.crnti(),
                     pdsch_alloc.slot);
        return make_unexpected(dl_alloc_failure_cause::other);
      }
    }
  }

  // Allocate PDCCH.
  auto pdcch_result = alloc_dl_pdcch(ue_cc, ss_info);
  if (not pdcch_result.has_value()) {
    return make_unexpected(dl_alloc_failure_cause::pdcch_alloc_failed);
  }
  pdcch_dl_information* pdcch = pdcch_result.value();

  std::optional<uci_allocation> uci_result;

  if (harq_mode == harq_utils::harq_mode_t::normal) {
    // Allocate UCI.
    uci_result = alloc_uci(ue_cc, ss_info, pdsch_td_res_index, last_occasion_offset);
    if (not uci_result.has_value()) {
      ++pdcch_alloc.result_builder().failed_attempts().uci;
      pdcch_sched.cancel_last_pdcch(pdcch_alloc);
      return make_unexpected(dl_alloc_failure_cause::uci_alloc_failed);
    }
  } else {
    // Note: k1 needed for PDSCH-to-HARQ timing-indicator, but UE sends no feedback.
    // The values (4, 0) are arbitrary since they will not be used by the UE.
    uci_result = uci_allocation{.k1 = 4U, .k1_last_rep = 4U, .harq_bit_idx = 0};
  }
  uci_allocation& uci                     = uci_result.value();
  unsigned        k1                      = uci.k1;
  pdcch->ctx.context.harq_feedback_timing = k1;

  // Both delays are counted from \c pdsch_alloc.slot, the first PDSCH occasion. With PDSCH repetitions, the UE counts
  // k1 from the last repetition occasion, so \c last_occasion_offset must be added on top.
  // In the case of a multi-slot PUCCH repetition burst, the HARQ-ACK feedback can only be considered lost once the
  // last repetition has been transmitted.
  const cell_configuration& cell_cfg  = ue_cell_cfg.cell_cfg_common;
  const unsigned            ack_delay = cell_cfg.ul_delay(last_occasion_offset + k1 + ue_cc.ntn_koffset());
  const unsigned last_ack_delay       = cell_cfg.ul_delay(last_occasion_offset + uci.k1_last_rep + ue_cc.ntn_koffset());

  // Allocate UE DL HARQ.
  // NOTE: With PDSCH repetitions, the HARQ-ACK is expected k1 slots after the last repetition occasion.
  if (not is_retx) {
    // It is a new tx.
    h_dl = ue_cc.harqs.alloc_dl_harq(pdcch_alloc.slot,
                                     pdsch_alloc.slot,
                                     ack_delay,
                                     expert_cfg.max_nof_dl_harq_retxs,
                                     uci.harq_bit_idx,
                                     harq_mode,
                                     nof_repetitions,
                                     last_ack_delay);
    ocudu_assert(h_dl.has_value(), "Failed to allocate DL HARQ");
  } else {
    // It is a retx.
    bool result = h_dl->new_retx(pdsch_alloc.slot, ack_delay, uci.harq_bit_idx, nof_repetitions, last_ack_delay);
    ocudu_assert(result, "Harq is in invalid state");
  }

  // Create PDSCH PDU. Room was checked before the PDCCH, UCI and HARQ were allocated.
  dl_msg_alloc* msg = pdsch_alloc.result_builder().dl().ue_grants().emplace_back();
  ocudu_assert(msg != nullptr, "ue={} rnti={}: No space available in scheduler PDSCH outputs", u.ue_index, u.crnti);

  // Create a DL grant builder.
  return dl_grant_info{&user, params, h_dl.value(), pdcch, msg, uci, reps};
}

ue_cell_grid_allocator::dl_repetition_occasion_list
ue_cell_grid_allocator::set_pdsch_params(dl_grant_info& grant, const vrb_alloc& vrbs, bool enable_interleaving) const
{
  // Derive remaining parameters from \c dl_grant_params.
  ue&                                          u                  = ues[grant.user->ue_index()];
  ue_cell&                                     ue_cc              = *u.find_cell(cell_alloc.cell_index());
  const ue_cell_configuration&                 ue_cell_cfg        = ue_cc.cfg();
  const search_space_info&                     ss_info            = ue_cell_cfg.search_space(grant.cfg.ss_id);
  uint8_t                                      pdsch_td_res_index = grant.cfg.pdsch_td_res_index;
  const pdsch_time_domain_resource_allocation& pdsch_td_cfg =
      ss_info.bwp->dl.td_mapper().pdsch_td_resources(ss_info.get_dl_dci_format())[pdsch_td_res_index];
  const subcarrier_spacing  scs        = ss_info.bwp->dl.cfg().scs;
  const cell_configuration& cell_cfg   = ue_cell_cfg.cell_cfg_common;
  const bool                is_retx    = grant.h_dl.nof_retxs() != 0;
  const unsigned            nof_layers = grant.cfg.recommended_ri;
  const sch_mcs_index       mcs        = grant.cfg.recommended_mcs;
  const auto&               pdsch_cfg  = ss_info.get_pdsch_config(pdsch_td_res_index, nof_layers);
  const unsigned            k1         = grant.uci_alloc.k1;

  // Fetch PDCCH and PDSCH resource grid allocators.

  cell_slot_resource_allocator& pdcch_alloc = cell_alloc[0];
  cell_slot_resource_allocator& pdsch_alloc = cell_alloc[pdsch_td_cfg.k0];

  if (vrbs.empty()) {
    // RBs could not be allocated. Cancel associated grants.
    grant.pdcch->ctx.rnti       = rnti_t::INVALID_RNTI;
    grant.pdsch->pdsch_cfg.rnti = rnti_t::INVALID_RNTI;
    // TODO: Cancel UCI allocation.
    grant.h_dl.cancel_unsent_tx();
    return {};
  }

  const prb_bitmap prbs = compute_pdsch_prbs(ss_info, vrbs, enable_interleaving);
  ocudu_sanity_check(not pdsch_collides(pdsch_alloc.dl_res_grid, ss_info, pdsch_td_cfg.symbols, prbs),
                     "Invalid calculation of PDSCH RBs. Used CRBs={:i}. Allocated CRBs={:i}.",
                     pdsch_alloc.dl_res_grid.used_crbs(scs, {0, cell_cfg.nof_dl_prbs}, pdsch_td_cfg.symbols),
                     prb_to_crb(ss_info.dl_crb_lims, prbs));

  sch_mcs_tbs mcs_tbs_info;
  if (is_retx) {
    // It is a reTx.
    mcs_tbs_info = {grant.h_dl.get_grant_params().mcs, grant.h_dl.get_grant_params().tbs};
  } else {
    // It is a newTx.
    auto mcs_or_error = calculate_dl_mcs_tbs(pdsch_alloc, ss_info, pdsch_td_res_index, prbs, mcs, nof_layers);
    if (not mcs_or_error.has_value()) {
      // We give up on more reTxs of this HARQ.
      grant.h_dl.cancel_retxs();

      logger.warning("ue={} rnti={}: Failed to derive MCS for PDSCH. Cause: no MCS such that code rate <= 0.95 with "
                     "provided configuration",
                     u.ue_index,
                     u.crnti);
    }
    mcs_tbs_info = mcs_or_error.value_or(sch_mcs_tbs{sch_mcs_index{0}, units::bytes{0}});
  }

  // Mark resources as occupied in the ResourceGrid.
  fill_pdsch_prbs(pdsch_alloc.dl_res_grid, ss_info, pdsch_td_cfg.symbols, prbs);

  // Compute TPC for PUCCH. With PDSCH repetitions, the PUCCH takes place k1 slots after the last occasion.
  const unsigned last_occasion_offset = grant.reps.has_value() ? grant.reps->nof_occasions - 1 : 0;
  const uint8_t  tpc                  = ue_cc.get_pucch_power_controller().compute_tpc_command(
      pdsch_alloc.slot + ue_cell_cfg.cell_cfg_common.ul_delay(last_occasion_offset + k1 + ue_cc.ntn_koffset()));

  // Check if is possible to allocate an aperiodic SRS.
  // NOTE: if the prohibit time is not set, the scheduler doesn't allocate any aperiodic SRS.
  unsigned aperiodic_srs_res_trigger = 0U;
  if (expert_cfg.srs_prohibit_time.has_value() and ss_info.get_dl_dci_format() == dci_dl_format::f1_1) {
    const auto [srs_res_trigger, slot_offset] = srs_alloc.allocate_aperiodic_srs(
        cell_alloc, ue_cc.channel_state_manager().last_aperiodic_srs_slot, ue_cell_cfg, ue_cc.ntn_koffset());
    if (srs_res_trigger != 0) {
      ue_cc.channel_state_manager().on_scheduled_aperiodic_srs(pdcch_alloc.slot +
                                                               cell_cfg.ul_delay(slot_offset + ue_cc.ntn_koffset()));
      aperiodic_srs_res_trigger = srs_res_trigger;
    }
  }

  // Fill DL PDCCH DCI PDU.
  const uint8_t rv = ue_cc.get_pdsch_rv(grant.h_dl.nof_retxs());
  // For allocation on PUSCH, we use a PUCCH resource indicator set to 0, as it will get ignored by the UE.
  const unsigned pucch_res_indicator = grant.uci_alloc.pucch_res_indicator.value_or(0);
  switch (ss_info.get_dl_dci_format()) {
    case dci_dl_format::f1_0: {
      build_dci_f1_0_c_rnti(grant.pdcch->dci,
                            ss_info,
                            cell_cfg.params.dl_cfg_common.init_dl_bwp,
                            vrbs.type1(),
                            pdsch_td_res_index,
                            k1,
                            pucch_res_indicator,
                            grant.uci_alloc.harq_bit_idx % DAI_MOD,
                            mcs_tbs_info.mcs,
                            rv,
                            grant.h_dl);
    } break;
    case dci_dl_format::f1_1:
      // The DCI Time domain resource assignment field indexes the PDSCH TDRA list in use directly; for a repetition
      // grant this row is the selected repetitionNumber-r16 entry.
      build_dci_f1_1_c_rnti(grant.pdcch->dci,
                            ue_cell_cfg,
                            ss_info.cfg->get_id(),
                            vrbs,
                            pdsch_td_res_index,
                            k1,
                            pucch_res_indicator,
                            grant.uci_alloc.harq_bit_idx % DAI_MOD,
                            mcs_tbs_info.mcs,
                            rv,
                            grant.h_dl,
                            nof_layers,
                            tpc,
                            enable_interleaving,
                            aperiodic_srs_res_trigger);
      break;
    default:
      report_fatal_error("Unsupported RNTI type for PDSCH allocation");
  }

  // Fill PDSCH PDU.
  dl_msg_alloc& msg                     = *grant.pdsch;
  msg.context.ue_index                  = u.ue_index;
  msg.context.k1                        = k1;
  msg.context.ss_id                     = grant.cfg.ss_id;
  msg.context.nof_retxs                 = grant.h_dl.nof_retxs();
  msg.context.nof_repetitions           = grant.reps.has_value() ? grant.reps->nof_occasions : uint8_t{1};
  msg.context.nof_remaining_repetitions = grant.reps.has_value() ? grant.reps->tx_offsets.size() : uint8_t{0};
  msg.context.buffer_occupancy          = 0; // We fill this value later, after the TB is built.
  if (not is_retx and ue_cc.link_adaptation_controller().is_dl_olla_enabled()) {
    msg.context.olla_offset = ue_cc.link_adaptation_controller().dl_cqi_offset();
  }
  switch (grant.pdcch->dci.type()) {
    case dci_dl_rnti_config_type::c_rnti_f1_0:
      build_pdsch_f1_0_c_rnti(msg.pdsch_cfg,
                              pdsch_cfg,
                              mcs_tbs_info.tbs,
                              u.crnti,
                              cell_cfg,
                              ss_info,
                              grant.pdcch->dci.as_c_rnti_f1_0(),
                              vrbs.type1(),
                              not is_retx);
      break;
    case dci_dl_rnti_config_type::c_rnti_f1_1:
      build_pdsch_f1_1_c_rnti(msg.pdsch_cfg,
                              pdsch_cfg,
                              mcs_tbs_info,
                              u.crnti,
                              ue_cell_cfg,
                              grant.cfg.ss_id,
                              grant.pdcch->dci.as_c_rnti_f1_1(),
                              vrbs,
                              not is_retx,
                              ue_cc.channel_state_manager());
      break;
    default:
      report_fatal_error("Unsupported PDCCH DL DCI format");
  }

  // Save set PDCCH and PDSCH PDU parameters in HARQ process.
  dl_harq_alloc_context pdsch_sched_ctx;
  pdsch_sched_ctx.dci_cfg_type    = grant.pdcch->dci.type();
  pdsch_sched_ctx.nof_repetitions = grant.reps.has_value() ? grant.reps->nof_occasions : 1;
  if (not is_retx) {
    pdsch_sched_ctx.olla_mcs = ue_cc.link_adaptation_controller().calculate_dl_mcs(msg.pdsch_cfg.mcs_table);
    pdsch_sched_ctx.slice_id = grant.user->ran_slice_id();
  }
  pdsch_sched_ctx.cqi = ue_cc.channel_state_manager().get_wideband_cqi();

  // Set MAC logical channels to schedule in this PDU for each new-TX codeword.
  for (const auto& cw : msg.pdsch_cfg.codewords) {
    if (cw.new_data) {
      build_dl_transport_block_info(
          msg.tb_list.emplace_back(),
          u.logical_channels(),
          cw.tb_size_bytes,
          grant.user->ran_slice_id(),
          dl_transport_block_restrictions{harq_utils::to_dl_harq_feedback(grant.h_dl.mode())});
    }
  }
  if (not is_retx) {
    // Update context with buffer occupancy of the slice after the TB is built.
    msg.context.buffer_occupancy = grant.user->pending_dl_newtx_bytes();
  }

  // Only a cell deriving UE-specific Koffsets sends the MAC CE.
  if (get_ue_koffset_cfg(cell_cfg.params.ntn_params).has_value()) {
    // A Differential Koffset carried by this PDSCH applies from the first slot three subframes past its HARQ-ACK, as
    // per TS 38.213, Section 9. The HARQ-ACK uses the current Koffset, so the slot is known now. It is a frame number
    // the UE and the gNB DL clock share, not an instant at the gNB antenna, so it carries no k_mac.
    const slot_point koffset_apply_slot =
        pdsch_alloc.slot + last_occasion_offset + k1 + ue_cc.ntn_koffset() +
        DIFFERENTIAL_KOFFSET_APPLY_DELAY_SUBFRAMES * get_nof_slots_per_subframe(cell_cfg.scs_common()) + 1;
    if (is_retx) {
      // The UE acts on the first transmission it decodes, as per TS 38.321, Section 5.3.2.2, so a retransmission moves
      // the apply slot.
      if (u.koffset_tracker().has_pending_ce() and carries_differential_koffset_ce(grant.h_dl)) {
        u.koffset_tracker().handle_ce_retx(koffset_apply_slot);
      }
    } else if (const std::optional<std::chrono::milliseconds> koffset = find_differential_koffset_ce(msg);
               koffset.has_value()) {
      u.koffset_tracker().handle_ce_sent(*koffset, koffset_apply_slot);
    }
  }

  // Save PDSCH parameters in DL HARQ.
  grant.h_dl.save_grant_params(pdsch_sched_ctx, msg);

  // Allocate the PDCCH-less PDSCH repetition occasions. Each occasion re-transmits the TB stored in the MAC HARQ
  // buffer (new_data=false) in the same PRBs/symbols, with the RV following the repetition cycle.
  dl_repetition_occasion_list committed_repetition_slots;
  if (grant.reps.has_value()) {
    for (unsigned i = 0; i < grant.reps->tx_offsets.size(); ++i) {
      cell_slot_resource_allocator& rep_alloc = cell_alloc[pdsch_td_cfg.k0 + grant.reps->tx_offsets[i]];
      const unsigned                nof_other_grants =
          rep_alloc.result_reader().dl().sibs().size() + rep_alloc.result_reader().dl().paging_grants().size() +
          rep_alloc.result_reader().dl().rar_grants().size() + rep_alloc.result_builder().dl().ue_grants().size();
      if (rep_alloc.result_builder().dl().ue_grants().full() or nof_other_grants >= expert_cfg.max_pdschs_per_slot or
          pdsch_collides(rep_alloc.dl_res_grid, ss_info, pdsch_td_cfg.symbols, prbs)) {
        logger.debug("ue={} rnti={}: Skipping PDSCH repetition occasion at slot={}. Cause: No space in the grid.",
                     u.ue_index,
                     u.crnti,
                     rep_alloc.slot);
        continue;
      }
      dl_msg_alloc* rep_pdu = rep_alloc.result_builder().dl().ue_grants().emplace_back();
      ocudu_assert(rep_pdu != nullptr,
                   "rnti={}: no room left for the PDSCH repetition occasion at slot={}",
                   u.crnti,
                   rep_alloc.slot);
      fill_pdsch_prbs(rep_alloc.dl_res_grid, ss_info, pdsch_td_cfg.symbols, prbs);
      dl_msg_alloc& rep_msg                     = *rep_pdu;
      rep_msg.context                           = msg.context;
      rep_msg.context.buffer_occupancy          = 0;
      rep_msg.pdsch_cfg                         = msg.pdsch_cfg;
      rep_msg.pdsch_cfg.codewords[0].new_data   = false;
      rep_msg.pdsch_cfg.codewords[0].rv_index   = get_repetition_rv(rv, grant.reps->tx_offsets[i]);
      rep_msg.context.nof_remaining_repetitions = grant.reps->tx_offsets.size() - 1 - i;
      committed_repetition_slots.push_back(rep_alloc.slot);
    }
  }

  // Update DRX state given the new allocation.
  u.drx_controller().on_new_dl_pdcch_alloc(pdcch_alloc.slot);
  return committed_repetition_slots;
}

expected<ue_cell_grid_allocator::dl_retx_grant_result, dl_alloc_failure_cause>
ue_cell_grid_allocator::allocate_dl_grant(const ue_retx_dl_grant_request& request) const
{
  static constexpr search_space_id ue_ded_ss_id = to_search_space_id(2);
  const ue_cell&                   ue_cc        = request.user.get_cc();
  const search_space_info&         ss_info      = ue_cc.cfg().search_space(ue_ded_ss_id);

  // Select PDCCH searchSpace and PDSCH time-domain resource config.
  auto sched_ctxt = sched_helper::get_retx_dl_sched_context(request.user,
                                                            cell_alloc[0].slot,
                                                            request.pdsch_slot,
                                                            request.interleaving_enabled,
                                                            request.h_dl,
                                                            request.max_rbs);
  if (not sched_ctxt) {
    // No valid parameters were found. When repetitions were requested but no repetition row fits this slot, the reTx
    // is deferred to a later one.
    return make_unexpected(dl_alloc_failure_cause::other);
  }

  // Build the repetition bundle when a repetition row was selected.
  std::optional<dl_repetition_info> reps;
  if (sched_ctxt->nof_repetitions.has_value()) {
    reps = select_pdsch_repetitions(ue_cc, ss_info, sched_ctxt->pdsch_td_res_index);
    if (not reps.has_value()) {
      // The bundle cannot start in this slot. Defer the reTx to a later one.
      return make_unexpected(dl_alloc_failure_cause::other);
    }
  }

  // Select DL CRBs.
  const vrb_alloc vrbs = sched_helper::compute_retx_dl_vrbs(
      sched_ctxt.value(), ss_info, request.used_dl_vrbs, request.interleaving_enabled);
  if (vrbs.empty()) {
    return make_unexpected(dl_alloc_failure_cause::pdcch_alloc_failed);
  }

  // Allocate PDCCH, PDSCH and UCI PDUs.
  auto grant = setup_dl_grant_builder(request.user, sched_ctxt.value(), request.h_dl, std::move(reps));
  if (not grant.has_value()) {
    return make_unexpected(grant.error());
  }

  // Set PDSCH parameters.
  dl_repetition_occasion_list committed_repetition_slots =
      set_pdsch_params(grant.value(), vrbs, request.interleaving_enabled);
  return dl_retx_grant_result{vrbs, committed_repetition_slots};
}

expected<ue_cell_grid_allocator::ul_newtx_grant_builder, alloc_status>
ue_cell_grid_allocator::allocate_ul_grant(const ue_newtx_ul_grant_request& request)
{
  static constexpr search_space_id ue_ded_ss_id = to_search_space_id(2);
  const ue_cell&                   ue_cc        = *ues[request.user.ue_index()].find_cell(cell_alloc.cell_index());
  const search_space_info&         ss_info      = ue_cc.cfg().search_space(ue_ded_ss_id);

  unsigned pending_uci_harq_bits =
      uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(request.pusch_slot, request.user.crnti());

  // Select PDCCH searchSpace and PUSCH time-domain resource config.
  auto sched_ctxt = sched_helper::get_newtx_ul_sched_context(request.user,
                                                             cell_alloc[0].slot,
                                                             request.pusch_slot,
                                                             pending_uci_harq_bits,
                                                             request.pending_bytes,
                                                             request.allowed_symbols);
  if (not sched_ctxt.has_value()) {
    return make_unexpected(alloc_status::skip_ue);
  }

  // Build the repetition bundle when a repetition row was selected. A bundle that cannot be scheduled in this slot
  // does not defer the grant: it is downgraded to the single-transmission row the selector picked alongside.
  std::optional<ul_repetition_info> reps;
  if (sched_ctxt->nof_repetitions.has_value()) {
    reps = select_pusch_repetitions(ue_cc, ss_info, sched_ctxt->pusch_td_res_index);
    if (reps.has_value() and
        // pending_uci_harq_bits covers the base slot only, but the bundle carries the HARQ-ACKs booked in any of its
        // slots, known only now. Re-size for that payload; the TDRA row is kept, so reps stays valid.
        not sched_helper::resize_newtx_ul_grant_for_uci(
            *sched_ctxt,
            request.user,
            request.pusch_slot,
            bundle_uci_harq_bits(request.user.crnti(), request.pusch_slot, *reps),
            reps->tx_offsets)) {
      reps.reset();
    }
    if (not reps.has_value() and not sched_helper::downgrade_newtx_ul_grant_to_single_tx(
                                     *sched_ctxt, request.user, request.pusch_slot, pending_uci_harq_bits)) {
      return make_unexpected(alloc_status::skip_ue);
    }
  }

  // Set up a UL grant.
  auto result = setup_ul_grant_builder(request.user, *sched_ctxt, std::nullopt, reps);
  if (not result.has_value()) {
    return make_unexpected(result.error());
  }

  // Add UL grant to list of pending grants and create a newTx DL grant builder.
  ul_grants.push_back(*result);
  return ul_newtx_grant_builder{*this, static_cast<unsigned>(ul_grants.size()) - 1};
}

vrb_bitmap ue_cell_grid_allocator::bundle_used_vrbs(const vrb_bitmap&                     base_used_vrbs,
                                                    const slice_ue&                       user,
                                                    const sched_helper::ul_sched_context& ctxt,
                                                    const ul_repetition_info&             reps) const
{
  vrb_bitmap used = base_used_vrbs;

  const ue_cell&                               ue_cc   = *ues[user.ue_index()].find_cell(cell_alloc.cell_index());
  const search_space_info&                     ss_info = ue_cc.cfg().search_space(ctxt.ss_id);
  const pusch_time_domain_resource_allocation& td_res =
      ss_info.bwp->ul.td_mapper().pusch_td_resources(ss_info.get_ul_dci_format())[ctxt.pusch_td_res_index];
  const subcarrier_spacing scs      = ss_info.bwp->ul.common().generic_params.scs;
  const unsigned           final_k2 = cell_alloc.cfg.ul_delay(td_res.k2 + ue_cc.ntn_koffset());

  for (const uint8_t offset : reps.tx_offsets) {
    // An occasion spans the same symbols and CRB limits as the base grant, so the two bitmaps line up VRB by VRB.
    used |= cell_alloc[final_k2 + offset]
                .ul_res_grid.used_prbs(scs, ss_info.ul_crb_lims, td_res.symbols)
                .convert_to<vrb_bitmap>();
  }
  return used;
}

unsigned
ue_cell_grid_allocator::bundle_uci_harq_bits(rnti_t crnti, slot_point base_slot, const ul_repetition_info& reps) const
{
  unsigned nof_bits = uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(base_slot, crnti);
  for (const uint8_t offset : reps.tx_offsets) {
    nof_bits += uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(base_slot + offset, crnti);
  }
  return nof_bits;
}

/// UL DAI carried by DCI format 0_1 for a HARQ-ACK codebook of the given number of bits, as per TS 38.213,
/// Table 9.1.3-2 (leftmost column).
static unsigned ul_dai_value(unsigned nof_harq_ack_bits)
{
  ocudu_assert(nof_harq_ack_bits != 0, "A HARQ-ACK codebook of no bits has no DAI to carry");
  return (nof_harq_ack_bits - 1) % DAI_MOD;
}

std::optional<ue_cell_grid_allocator::ul_repetition_info>
ue_cell_grid_allocator::select_pusch_repetitions(const ue_cell&           ue_cc,
                                                 const search_space_info& ss_info,
                                                 uint8_t                  pusch_td_res_index) const
{
  // Per Rel-17 "available slot counting" (puschTypeA-RepetitionsAvailSlot-r17), the UE repeats the PUSCH over the
  // next nof_repetitions-1 UL slots, skipping DL/special ones. It transmits every occasion once it has the DCI, so
  // an unusable slot rejects the whole bundle rather than costing an occasion.
  const ul_time_domain_mapper&                 ul_td_mapper = ss_info.bwp->ul.td_mapper();
  const pusch_time_domain_resource_allocation& td_res = ul_td_mapper.dedicated_pusch_td_resources()[pusch_td_res_index];
  const uint8_t                                nof_repetitions = *td_res.nof_repetitions;

  const cell_configuration& cell_cfg   = cell_alloc.cfg;
  const unsigned            final_k2   = cell_cfg.ul_delay(td_res.k2 + ue_cc.ntn_koffset());
  const slot_point          pusch_slot = cell_alloc[final_k2].slot;

  // As per TS 38.213 clause 9, the UE multiplexes in each slot of the bundle the HARQ-ACKs it would have reported on
  // a PUCCH there, and the single DAI of the grant's only DCI applies to every one of those slots alike. One value
  // can only describe them all if they agree on it, so a bundle whose slots disagree is rejected here.
  const unsigned base_harq_ack_bits = uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(pusch_slot, ue_cc.rnti());

  ul_repetition_info reps{nof_repetitions, {}, base_harq_ack_bits};
  unsigned           offset = final_k2;
  for (uint8_t count = 1; count != nof_repetitions; ++count) {
    // Advance to the next full UL slot.
    do {
      ++offset;
      if (offset > cell_alloc.max_ul_slot_alloc_delay) {
        // Not enough UL slots left in the resource grid window; defer.
        if (logger.debug.enabled()) {
          logger.debug("ue={} rnti={}: PUSCH repetition bundle rejected at slot={}. Cause: not enough UL slots in "
                       "the resource grid window to fit the bundle.",
                       ue_cc.ue_index,
                       ue_cc.rnti(),
                       pusch_slot);
        }
        return std::nullopt;
      }
    } while (not cell_cfg.is_fully_ul_enabled(cell_alloc[offset].slot));

    const slot_point occasion_slot = cell_alloc[offset].slot;

    // No room for another PUSCH in the occasion's slot: the bundle cannot start here.
    if (cell_alloc[offset].result_builder().ul().puschs().full()) {
      if (logger.debug.enabled()) {
        logger.debug("ue={} rnti={}: PUSCH repetition bundle rejected at slot={}. Cause: occasion slot={} is full.",
                     ue_cc.ue_index,
                     ue_cc.rnti(),
                     pusch_slot,
                     occasion_slot);
      }
      return std::nullopt;
    }

    // Per-UE restrictions on an occasion's slot, which the base slot gets when its UL scheduling context is built.
    // The UE counts a slot as available for repetition from the semi-static UL/DL configuration alone, so it keeps
    // the remaining occasions where they are: a slot it must not transmit in costs the bundle, it cannot be skipped
    // over. First, an uplink measurement gap, which the UE leaves to measure (TS 38.133, Section 9.1C.2).
    if (not ue_cc.is_ul_enabled(occasion_slot)) {
      if (logger.debug.enabled()) {
        logger.debug("ue={} rnti={}: PUSCH repetition bundle rejected at slot={}. Cause: the UE uplink is disabled "
                     "in the occasion slot={}.",
                     ue_cc.ue_index,
                     ue_cc.rnti(),
                     pusch_slot,
                     occasion_slot);
      }
      return std::nullopt;
    }

    // Then a Configured Grant occasion of this UE, which the dynamic grant would have it transmit on top of. The CG
    // period and offset are in the UE uplink slot numbering, the occasion slot is on the gNB DL clock.
    if (ue_cc.get_conf_grant_state_manager().is_cg_slot(cell_cfg.dl_to_ue_ul_slot(occasion_slot))) {
      if (logger.debug.enabled()) {
        logger.debug("ue={} rnti={}: PUSCH repetition bundle rejected at slot={}. Cause: occasion slot={} is a "
                     "Configured Grant occasion of this UE.",
                     ue_cc.ue_index,
                     ue_cc.rnti(),
                     pusch_slot,
                     occasion_slot);
      }
      return std::nullopt;
    }

    // Likewise for a PUCCH whose UCI cannot be moved onto a PUSCH: as per TS 38.213 Sections 9.2.1 and 9.2.6 the UE
    // transmits the PUCCH and drops the overlapping PUSCH, so the occasion could never be received.
    if (uci_alloc.has_harq_ack_on_common_pucch_res(ue_cc.rnti(), occasion_slot) or
        uci_alloc.has_pucch_repetition(ue_cc.rnti(), occasion_slot)) {
      if (logger.debug.enabled()) {
        logger.debug("ue={} rnti={}: PUSCH repetition bundle rejected at slot={}. Cause: occasion slot={} carries a "
                     "PUCCH of this UE whose UCI cannot be multiplexed onto a PUSCH.",
                     ue_cc.ue_index,
                     ue_cc.rnti(),
                     pusch_slot,
                     occasion_slot);
      }
      return std::nullopt;
    }

    // A slot carrying no HARQ-ACK constrains nothing: the UE reports none there, whatever the DAI says.
    const unsigned occasion_harq_ack_bits =
        uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(occasion_slot, ue_cc.rnti());
    if (occasion_harq_ack_bits != 0) {
      if (reps.harq_ack_bits_per_slot == 0) {
        reps.harq_ack_bits_per_slot = occasion_harq_ack_bits;
      } else if (ul_dai_value(occasion_harq_ack_bits) != ul_dai_value(reps.harq_ack_bits_per_slot)) {
        if (logger.debug.enabled()) {
          logger.debug("ue={} rnti={}: PUSCH repetition bundle rejected at slot={}. Cause: occasion slot={} needs a "
                       "UL DAI of {} HARQ-ACK bits, incompatible with the {} bits of an earlier slot.",
                       ue_cc.ue_index,
                       ue_cc.rnti(),
                       pusch_slot,
                       occasion_slot,
                       occasion_harq_ack_bits,
                       reps.harq_ack_bits_per_slot);
        }
        return std::nullopt;
      }
    }
    reps.tx_offsets.push_back(static_cast<uint8_t>(offset - final_k2));
  }

  if (logger.debug.enabled()) {
    logger.debug("ue={} rnti={}: PUSCH repetition bundle selected at slot={}: nof_repetitions={} dci_row={}.",
                 ue_cc.ue_index,
                 ue_cc.rnti(),
                 pusch_slot,
                 reps.nof_occasions,
                 pusch_td_res_index);
  }

  return reps;
}

expected<ue_cell_grid_allocator::ul_retx_grant_result, alloc_status>
ue_cell_grid_allocator::allocate_ul_grant(const ue_retx_ul_grant_request& request) const
{
  static constexpr search_space_id ue_ded_ss_id = to_search_space_id(2);
  const ue_cell&                   ue_cc        = request.user.get_cc();
  const search_space_info&         ss_info      = ue_cc.cfg().search_space(ue_ded_ss_id);

  unsigned pending_uci_harq_bits =
      uci_alloc.get_scheduled_pdsch_counter_in_ue_uci(request.pusch_slot, request.user.crnti());

  // Select PDCCH searchSpace and PUSCH time-domain resource config.
  auto sched_ctxt = sched_helper::get_retx_ul_sched_context(request.user,
                                                            cell_alloc[0].slot,
                                                            request.pusch_slot,
                                                            pending_uci_harq_bits,
                                                            request.h_ul,
                                                            request.allowed_symbols,
                                                            request.max_rbs);
  if (not sched_ctxt) {
    return make_unexpected(alloc_status::skip_ue);
  }

  // Build the repetition bundle when a repetition row was selected, downgrading to a single transmission as for a
  // newTx when it cannot be scheduled. A reTx only ever downgrades: it asks for the scheme of the original
  // transmission, never for more.
  std::optional<ul_repetition_info> reps;
  if (sched_ctxt->nof_repetitions.has_value()) {
    reps = select_pusch_repetitions(ue_cc, ss_info, sched_ctxt->pusch_td_res_index);
    if (reps.has_value() and
        // Re-size for the HARQ-ACKs booked in the occasion slots too, now that those slots are known. The TDRA row
        // does not depend on the UCI payload, so it stays as selected.
        not sched_helper::resize_retx_ul_grant_for_uci(
            *sched_ctxt,
            request.user,
            request.pusch_slot,
            bundle_uci_harq_bits(request.user.crnti(), request.pusch_slot, *reps),
            request.h_ul,
            reps->tx_offsets)) {
      reps.reset();
    }
    if (not reps.has_value() and
        not sched_helper::downgrade_retx_ul_grant_to_single_tx(
            *sched_ctxt, request.user, request.pusch_slot, pending_uci_harq_bits, request.h_ul)) {
      return make_unexpected(alloc_status::skip_ue);
    }
  }

  // Select UL CRBs. For a bundle, the VRBs are repeated on every occasion, so they must be free in all of its slots.
  vrb_interval vrbs =
      reps.has_value()
          ? sched_helper::compute_retx_ul_vrbs(
                sched_ctxt.value(), bundle_used_vrbs(request.used_ul_vrbs, request.user, sched_ctxt.value(), *reps))
          : sched_helper::compute_retx_ul_vrbs(sched_ctxt.value(), request.used_ul_vrbs);
  if (vrbs.empty()) {
    return make_unexpected(alloc_status::skip_ue);
  }

  // Allocate PDCCH, PUSCH PDUs.
  auto grant = setup_ul_grant_builder(request.user, sched_ctxt.value(), request.h_ul, reps);
  if (not grant.has_value()) {
    return make_unexpected(grant.error());
  }

  // Set PUSCH parameters.
  ul_repetition_occasion_list committed_repetition_slots = set_pusch_params(grant.value(), vrbs);

  return ul_retx_grant_result{vrbs, committed_repetition_slots};
}

expected<ue_cell_grid_allocator::ul_grant_info, alloc_status>
ue_cell_grid_allocator::setup_ul_grant_builder(const slice_ue&                       user,
                                               const sched_helper::ul_sched_context& params,
                                               std::optional<ul_harq_process_handle> h_ul,
                                               std::optional<ul_repetition_info>     reps) const
{
  // Derive remaining parameters from \c ul_grant_params.
  ue&                                          u                  = ues[user.ue_index()];
  ue_cell&                                     ue_cc              = *u.find_cell(cell_alloc.cell_index());
  const ue_cell_configuration&                 ue_cell_cfg        = ue_cc.cfg();
  const search_space_info&                     ss_info            = ue_cell_cfg.search_space(params.ss_id);
  const search_space_configuration&            ss_cfg             = *ss_info.cfg;
  const uint8_t                                pusch_td_res_index = params.pusch_td_res_index;
  const pusch_time_domain_resource_allocation& pusch_td_cfg =
      ss_info.bwp->ul.td_mapper().pusch_td_resources(ss_info.get_ul_dci_format())[pusch_td_res_index];
  const bool is_retx = h_ul.has_value();
  // Slot span used to extend the UE's last known Tx slot. Available slot counting may skip slots, so this is
  // 1 + the last occasion's offset, not the repetition count.
  const uint8_t harq_slot_span =
      reps.has_value() ? static_cast<uint8_t>((reps->tx_offsets.empty() ? 0 : reps->tx_offsets.back()) + 1) : 1;

  // Fetch PDCCH and PUSCH resource grid allocators.
  cell_slot_resource_allocator& pdcch_alloc = cell_alloc[0];
  const unsigned                final_k2    = cell_alloc.cfg.ul_delay(pusch_td_cfg.k2 + ue_cc.ntn_koffset());
  cell_slot_resource_allocator& pusch_alloc = cell_alloc[final_k2];
  ocudu_sanity_check(not pdcch_alloc.result_builder().dl().ul_pdcchs().full(),
                     "Maximum number of UL PDCCH grants per slot reached");
  ocudu_sanity_check(not pusch_alloc.result_builder().ul().puschs().full(),
                     "No PUSCH space available in scheduler output list");

  // [Implementation-defined] We skip allocation of PUSCH if there is already a PUCCH grant scheduled using common
  // PUCCH resources.
  if (uci_alloc.has_harq_ack_on_common_pucch_res(u.crnti, pusch_alloc.slot)) {
    logger.debug("ue={} rnti={}: Failed to allocate PUSCH in slot={}. Cause: UE has PUCCH grant using common PUCCH "
                 "resources scheduled",
                 u.ue_index,
                 u.crnti,
                 pusch_alloc.slot);
    return make_unexpected(alloc_status::skip_ue);
  }

  // As per TS 38.213, Section 9.2.6, if a PUCCH transmission with repetitions overlaps a PUSCH, the UE transmits the
  // PUCCH and does not transmit the PUSCH in the overlapping slots. The UCI cannot be moved to the PUSCH either, as
  // Section 9.2.5 scopes UCI multiplexing on PUSCH to PUCCHs "over a single slot without repetitions". So the PUSCH
  // must not be scheduled in any slot of an in-flight repetition burst, or the UE would drop it.
  if (uci_alloc.has_pucch_repetition(u.crnti, pusch_alloc.slot)) {
    logger.debug("ue={} rnti={}: Failed to allocate PUSCH in slot={}. Cause: slot is part of a PUCCH repetition burst "
                 "of this UE",
                 u.ue_index,
                 u.crnti,
                 pusch_alloc.slot);
    return make_unexpected(alloc_status::skip_ue);
  }

  // Only a new tx picks a HARQ process; a retx keeps the one it already holds.
  harq_utils::harq_mode_t harq_mode = harq_utils::harq_mode_t::normal;
  if (not is_retx and ue_cc.harqs.is_ul_harq_mode_selective()) {
    harq_mode = harq_utils::to_harq_mode(user.select_ul_grant_restrictions().harq_mode);
    // Checked before the PDCCH is booked, as cancelling one still costs the UL attempt of the slot.
    if (not ue_cc.harqs.has_empty_ul_harqs(harq_mode)) {
      logger.debug("ue={} rnti={}: Failed to allocate PUSCH in slot={}. Cause: no free UL HARQ process",
                   u.ue_index,
                   u.crnti,
                   pusch_alloc.slot);
      return make_unexpected(alloc_status::skip_ue);
    }
  }

  // Allocate PDCCH position.
  const aggregation_level aggr_lvl =
      ue_cc.get_aggregation_level(ue_cc.link_adaptation_controller().get_effective_cqi(), ss_info, false);
  pdcch_ul_information* pdcch =
      pdcch_sched.alloc_ul_pdcch_ue(pdcch_alloc, u.crnti, ue_cell_cfg, ss_cfg.get_id(), aggr_lvl);
  if (pdcch == nullptr) {
    logger.info("ue={} rnti={}: Failed to allocate PUSCH. Cause: No space in PDCCH.", u.ue_index, u.crnti);
    // Note: (Implementation-defined) Assuming all UEs share the same CORESET, if there are no more CCEs left in the
    // CORESET, stop attempting to allocate new PDCCHs in the slot.
    unsigned nof_cces_left = ss_info.coreset->cfg().get_nof_cces();
    for (const auto& dl_pdcch : pdcch_alloc.result_builder().dl().dl_pdcchs()) {
      nof_cces_left -= std::min(nof_cces_left, to_nof_cces(dl_pdcch.ctx.cces.aggr_lvl));
    }
    for (const auto& ul_pdcch : pdcch_alloc.result_builder().dl().ul_pdcchs()) {
      nof_cces_left -= std::min(nof_cces_left, to_nof_cces(ul_pdcch.ctx.cces.aggr_lvl));
    }
    return make_unexpected(nof_cces_left == 0 ? alloc_status::skip_slot : alloc_status::skip_ue);
  }

  // Allocate UE UL HARQ.
  if (not is_retx) {
    // It is a new tx.
    // NOTE: in this scheduler, we do not request a specific HARQ-ID process.
    h_ul = ue_cc.harqs.alloc_ul_harq(pdcch_alloc.slot,
                                     pusch_alloc.slot,
                                     expert_cfg.max_nof_ul_harq_retxs,
                                     /* cg_params */ std::nullopt,
                                     harq_mode,
                                     harq_slot_span);
    ocudu_assert(h_ul.has_value(), "Failed to allocate UL HARQ");
  } else {
    // It is a retx.
    bool result = h_ul->new_retx(pusch_alloc.slot, harq_slot_span);
    ocudu_assert(result, "Failed to allocate HARQ retx");
  }

  // Create PUSCH PDU. Room was checked before the PDCCH and HARQ were allocated.
  ul_sched_info* msg = pusch_alloc.result_builder().ul().puschs().emplace_back();
  ocudu_assert(msg != nullptr, "ue={} rnti={}: No space available in scheduler PUSCH outputs", u.ue_index, u.crnti);

  // Create a UL grant builder.
  return ul_grant_info{&user, params, h_ul.value(), pdcch, msg, reps};
}

ue_cell_grid_allocator::ul_repetition_occasion_list
ue_cell_grid_allocator::set_pusch_params(ul_grant_info& grant, const vrb_interval& vrbs) const
{
  ue&      u     = ues[grant.user->ue_index()];
  ue_cell& ue_cc = *u.find_cell(cell_alloc.cell_index());

  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();
  const cell_configuration&    cell_cfg    = ue_cell_cfg.cell_cfg_common;

  // Derive remaining parameters from \c ul_grant_params.
  const search_space_info&                     ss_info            = ue_cell_cfg.search_space(grant.cfg.ss_id);
  const search_space_configuration&            ss_cfg             = *ss_info.cfg;
  const bwp_uplink_common&                     bwp_ul_cmn         = ss_info.bwp->ul.common();
  const subcarrier_spacing                     scs                = bwp_ul_cmn.generic_params.scs;
  const dci_ul_rnti_config_type                dci_type           = ss_info.get_ul_dci_format() == dci_ul_format::f0_0
                                                                        ? dci_ul_rnti_config_type::c_rnti_f0_0
                                                                        : dci_ul_rnti_config_type::c_rnti_f0_1;
  uint8_t                                      pusch_td_res_index = grant.cfg.pusch_td_res_index;
  const pusch_time_domain_resource_allocation& pusch_td_cfg =
      ss_info.bwp->ul.td_mapper().pusch_td_resources(ss_info.get_ul_dci_format())[pusch_td_res_index];
  const pusch_config_params& pusch_cfg = grant.cfg.pusch_cfg;
  const bool                 is_retx   = grant.h_ul.nof_retxs() != 0;

  // Fetch PDCCH and PUSCH resource grid allocators.
  cell_slot_resource_allocator& pdcch_alloc = cell_alloc[0];
  const unsigned                final_k2    = cell_cfg.ul_delay(pusch_td_cfg.k2 + ue_cc.ntn_koffset());
  cell_slot_resource_allocator& pusch_alloc = cell_alloc[final_k2];

  if (vrbs.empty()) {
    // RBs could not be allocated. Cancel associated grants.
    grant.pdcch->ctx.rnti       = rnti_t::INVALID_RNTI;
    grant.pusch->pusch_cfg.rnti = rnti_t::INVALID_RNTI;
    grant.h_ul.cancel_unsent_tx();
    return {};
  }

  // The UE transmits every occasion once it has the DCI, and the occasion slots were checked a stage earlier: a
  // bundle for another UE may since have taken the last room in one. Cancel the whole grant here, before any UCI is
  // multiplexed on below -- past that point a PUCCH is gone in favour of this PUSCH and its UCI would be stranded.
  if (grant.reps.has_value()) {
    for (const uint8_t offset : grant.reps->tx_offsets) {
      cell_slot_resource_allocator& rep_alloc = cell_alloc[final_k2 + offset];
      if (rep_alloc.result_builder().ul().puschs().full()) {
        logger.info("ue={} rnti={}: Cancelling PUSCH repetition bundle at slot={}. Cause: no room left for the "
                    "occasion at slot={}.",
                    u.ue_index,
                    u.crnti,
                    pusch_alloc.slot,
                    rep_alloc.slot);
        grant.pdcch->ctx.rnti       = rnti_t::INVALID_RNTI;
        grant.pusch->pusch_cfg.rnti = rnti_t::INVALID_RNTI;
        grant.h_ul.cancel_unsent_tx();
        return {};
      }
    }
  }

  // Compute exact MCS and TBS for this transmission.
  expected<sch_mcs_tbs, compute_ul_mcs_tbs_error> mcs_tbs_info;
  // TODO: find TS reference for -> Since, PUSCH always uses non interleaved mapping, prbs = vrbs.
  const auto crbs = prb_to_crb(ss_info.ul_crb_lims, vrbs.convert_to<prb_interval>());
  if (not is_retx) {
    // If it's a new Tx, compute the MCS and TBS from SNR, payload size, and available RBs.

    // Note: Even if at this point we can determine if the RBs intersect the DC location (via
    // dc_offset_helper::is_contained), we take the conservative approach and assume that the DC location is always
    // contained in the RBs. This may slightly reduce the MCS beyond what's needed to satisfy the effective code rate
    // limits. We do this to simplify the search for available RBs during HARQ retxs.
    constexpr bool contains_dc = true;
    mcs_tbs_info =
        compute_ul_mcs_tbs(pusch_cfg, ue_cc.active_bwp(), grant.cfg.recommended_mcs, vrbs.length(), contains_dc);

    // If there is not MCS-TBS info, it means no MCS exists such that the effective code rate is <= 0.95.
    if (not mcs_tbs_info.has_value()) {
      // We cannot cancel this allocation at this point. So, we will proceed as normal, but set an invalid coderate.
      mcs_tbs_info.emplace();
      mcs_tbs_info->mcs = 0;
      mcs_tbs_info->tbs = compute_ul_tbs_unsafe(pusch_cfg, grant.cfg.recommended_mcs, vrbs.length());

      logger.warning(
          "ue={} rnti={}: Failed to allocate PUSCH. Cause: {} with this "
          "configuration: mcs={} vrbs={} symbols={} nof_oh={} tb-sc-field={} layers={} pi2bpsk={} "
          "harq_bits={} csi1_bits={} csi2_bits={} mcs_table_idx={} dmrs_A_pos={} is_dmrs_type2={} dmrs_add_pos_idx={}",
          u.ue_index,
          u.crnti,
          to_string(mcs_tbs_info.error()),
          grant.cfg.recommended_mcs,
          vrbs,
          pusch_cfg.symbols,
          pusch_cfg.nof_oh_prb,
          pusch_cfg.tb_scaling_field,
          pusch_cfg.nof_layers,
          pusch_cfg.tp_pi2bpsk_present ? "yes" : "no",
          pusch_cfg.nof_harq_ack_bits,
          pusch_cfg.nof_csi_part1_bits,
          pusch_cfg.max_nof_csi_part2_bits,
          static_cast<unsigned>(pusch_cfg.mcs_table),
          ue_cell_cfg.cell_cfg_common.params.dmrs_typeA_pos == dmrs_typeA_position::pos2 ? "pos2" : "pos3",
          ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg->pusch_mapping_type_a_dmrs.value().is_dmrs_type2 ? "yes" : "no",
          static_cast<unsigned>(
              ue_cell_cfg.init_bwp().ul.ded()->pusch_cfg->pusch_mapping_type_a_dmrs.value().additional_positions));
    }

  } else {
    // If it's a reTx, fetch the MCS, TBS and number of layers from the previous transmission.
    const auto& prev_params = grant.h_ul.get_grant_params();
    mcs_tbs_info.emplace(sch_mcs_tbs{.mcs = prev_params.mcs, .tbs = prev_params.tbs});
    ocudu_sanity_check(prev_params.mcs_table == pusch_cfg.mcs_table, "MCS table cannot change across HARQ reTxs");
  }

  ocudu_sanity_check(not pusch_alloc.ul_res_grid.collides(scs, pusch_td_cfg.symbols, crbs),
                     "Invalid calculation of PUSCH RBs. Used CRBs={:i}. Allocated CRBs={}.",
                     pusch_alloc.ul_res_grid.used_crbs(scs, {0, cell_cfg.nof_ul_prbs}, pusch_td_cfg.symbols),
                     crbs);

  // Mark resources as occupied in the ResourceGrid.
  pusch_alloc.ul_res_grid.fill(grant_info{scs, pusch_td_cfg.symbols, crbs});

  // Compute total DAI. See TS 38.213, 9.1.3.2.
  // Note: We assume k2 <= k1, which means that all the HARQ bits are set at this point for this UL slot and UE.
  // Total DAI provides total number of transmissions at the end of each interval (slot in a cell). Values range from
  // 1 to 4. If a UE is not provided PDSCH-CodeBlockGroupTransmission and the UE is scheduled for a PUSCH transmission
  // by DCI format 0_1 with DAI field value V_T_DAI_UL = 4 and the UE has not received any PDCCH within the monitoring
  // occasions for PDCCH with DCI format 1_0 or DCI format 1_1 for scheduling PDSCH receptions or SPS PDSCH
  // release on any serving cell c and the UE does not have HARQ-ACK information in response to a SPS PDSCH
  // reception to multiplex in the PUSCH, the UE does not multiplex HARQ-ACK information in the PUSCH transmission.
  // NOTE: DAI is encoded as per left most column in Table 9.1.3-2 of TS 38.213.
  unsigned dai = 3;
  if (dci_type == dci_ul_rnti_config_type::c_rnti_f0_1) {
    // A bundle reports in each of its slots only the HARQ-ACKs booked for that slot, and the DAI applies to each of
    // them alike, so it carries that per-slot count -- not the total across slots the grant was sized for. The
    // bundle was only accepted because every slot carrying HARQ-ACK agrees on the count. Getting the DAI wrong is
    // silent: the UE would size its HARQ-ACK codebook differently than the gNB demaps it.
    unsigned total_harq_ack_in_uci =
        grant.reps.has_value() ? grant.reps->harq_ack_bits_per_slot : pusch_cfg.nof_harq_ack_bits;
    if (total_harq_ack_in_uci != 0) {
      // See TS 38.213, Table 9.1.3-2. dai value below maps to the leftmost column in the table.
      dai = ul_dai_value(total_harq_ack_in_uci);
    }
  }

  // Compute TPC command before computing the nof_prbs adaptation based on PHR; this is because, when the TPC gets
  // computed, the channel state manager will update close-loop power control adjustment.
  const uint8_t tpc_command = ue_cc.get_pusch_power_controller().compute_tpc_command(
      pdcch_alloc.slot + cell_cfg.ul_delay(pusch_td_cfg.k2 + ue_cc.ntn_koffset()));

  // Check if is possible to allocate an aperiodic SRS.
  // NOTE: if the prohibit time is not set, the scheduler doesn't allocate any aperiodic SRS.
  unsigned aperiodic_srs_res_trigger = 0U;
  if (expert_cfg.srs_prohibit_time.has_value() and dci_type == dci_ul_rnti_config_type::c_rnti_f0_1) {
    const auto [srs_res_trigger, slot_offset] = srs_alloc.allocate_aperiodic_srs(
        cell_alloc, ue_cc.channel_state_manager().last_aperiodic_srs_slot, ue_cell_cfg, ue_cc.ntn_koffset());
    if (srs_res_trigger != 0) {
      ue_cc.channel_state_manager().on_scheduled_aperiodic_srs(pdcch_alloc.slot +
                                                               cell_cfg.ul_delay(slot_offset + ue_cc.ntn_koffset()));
      aperiodic_srs_res_trigger = srs_res_trigger;
    }
  }

  // Fill UL PDCCH DCI.
  const uint8_t rv = ue_cc.get_pusch_rv(grant.h_ul.nof_retxs());
  switch (dci_type) {
    case dci_ul_rnti_config_type::c_rnti_f0_0:
      build_dci_f0_0_c_rnti(grant.pdcch->dci,
                            ss_info,
                            cell_cfg.params.ul_cfg_common.init_ul_bwp,
                            vrbs,
                            grant.cfg.pusch_td_res_index,
                            mcs_tbs_info.value().mcs,
                            rv,
                            grant.h_ul,
                            tpc_command);
      break;
    case dci_ul_rnti_config_type::c_rnti_f0_1:
      build_dci_f0_1_c_rnti(grant.pdcch->dci,
                            ue_cell_cfg,
                            grant.cfg.ss_id,
                            vrbs,
                            grant.cfg.pusch_td_res_index,
                            mcs_tbs_info.value().mcs,
                            rv,
                            grant.h_ul,
                            dai,
                            pusch_cfg.nof_layers,
                            ue_cc.channel_state_manager().get_recommended_pusch_tpmi(pusch_cfg.nof_layers),
                            tpc_command,
                            grant.cfg.pusch_cfg.aperiodic_csi
                                ? std::optional<bool>(grant.cfg.pusch_cfg.nof_csi_part1_bits != 0)
                                : std::nullopt,
                            aperiodic_srs_res_trigger);
      break;
    default:
      report_fatal_error("Unsupported PDCCH UL DCI format");
  }

  // Fill PUSCH PDU.
  ul_sched_info& msg     = *grant.pusch;
  msg.context.ue_index   = u.ue_index;
  msg.context.ss_id      = grant.cfg.ss_id;
  msg.context.k2         = final_k2;
  msg.context.nof_retxs  = grant.h_ul.nof_retxs();
  msg.context.nof_oh_prb = pusch_cfg.nof_oh_prb;
  if (grant.reps.has_value()) {
    msg.pusch_cfg.repetitions = pusch_information::repetition_info{grant.reps->nof_occasions,
                                                                   static_cast<uint8_t>(grant.reps->nof_occasions - 1)};
  }
  if (not is_retx and ue_cc.link_adaptation_controller().is_ul_olla_enabled()) {
    msg.context.olla_offset = ue_cc.link_adaptation_controller().ul_snr_offset_db();
  }
  switch (grant.pdcch->dci.type()) {
    case dci_ul_rnti_config_type::tc_rnti_f0_0:
      build_pusch_f0_0_tc_rnti(msg.pusch_cfg,
                               pusch_cfg,
                               mcs_tbs_info.value().tbs,
                               u.crnti,
                               cell_cfg,
                               grant.pdcch->dci.as_tc_rnti_f0_0(),
                               vrbs,
                               not is_retx);
      break;
    case dci_ul_rnti_config_type::c_rnti_f0_0:
      build_pusch_f0_0_c_rnti(msg.pusch_cfg,
                              u.crnti,
                              pusch_cfg,
                              mcs_tbs_info.value().tbs,
                              cell_cfg,
                              bwp_ul_cmn,
                              grant.pdcch->dci.as_c_rnti_f0_0(),
                              vrbs,
                              not is_retx);
      break;
    case dci_ul_rnti_config_type::c_rnti_f0_1:
      build_pusch_f0_1_c_rnti(msg.pusch_cfg,
                              u.crnti,
                              pusch_cfg,
                              mcs_tbs_info.value(),
                              ue_cell_cfg,
                              ss_cfg.get_id(),
                              grant.pdcch->dci.as_c_rnti_f0_1(),
                              vrbs,
                              not is_retx);
      break;
    default:
      report_fatal_error("Unsupported PDCCH UL DCI format");
  }

  // Check if there is any UCI grant allocated on the PUCCH that can be moved to the PUSCH.
  uci_alloc.multiplex_uci_on_pusch(
      msg, pusch_alloc, ue_cell_cfg, pusch_cfg.aperiodic_csi and pusch_cfg.nof_csi_part1_bits != 0);

  // Save set PDCCH and PUSCH PDU parameters in HARQ process.
  ul_harq_alloc_context pusch_sched_ctx;
  pusch_sched_ctx.dci_cfg_type    = grant.pdcch->dci.type();
  pusch_sched_ctx.nof_repetitions = grant.reps.has_value() ? grant.reps->nof_occasions : 1;
  if (not is_retx) {
    pusch_sched_ctx.olla_mcs =
        ue_cc.link_adaptation_controller().calculate_ul_mcs(pusch_cfg.mcs_table, pusch_cfg.use_transform_precoder);
    pusch_sched_ctx.slice_id = grant.user->ran_slice_id();
  }

  grant.h_ul.save_grant_params(pusch_sched_ctx, msg.pusch_cfg);

  // Register UL allocations for this slot.
  u.logical_channels().handle_ul_grant(grant.h_ul.get_grant_params().tbs);

  // Update DRX state given the new allocation.
  u.drx_controller().on_new_ul_pdcch_alloc(pdcch_alloc.slot, pusch_alloc.slot);

  // Update the number of PRBs used in the PUSCH allocation.
  ue_cc.get_pusch_power_controller().update_pusch_pw_ctrl_state(pusch_alloc.slot, vrbs.length());

  if (pusch_cfg.aperiodic_csi and pusch_cfg.nof_csi_part1_bits != 0) {
    // Notify the channel state manager about the scheduled PUSCH for aperiodic CSI reporting.
    ue_cc.channel_state_manager().on_scheduled_aperiodic_csi_pusch(pusch_alloc.slot);
  }

  // Allocate the PDCCH-less PUSCH repetition occasions: same TB, same HARQ process, same PRBs/symbols, with the RV
  // cycled and the FAPI countdown decremented. new_data=false drives the soft combining -- only the base occasion
  // opens the PHY receive buffer, the later ones accumulate into it.
  ul_repetition_occasion_list committed_repetition_slots;
  if (grant.reps.has_value()) {
    // Note: an occasion's nominal position in the bundle (RV cycle, FAPI countdown) is its index in tx_offsets, not
    // its slot offset; with Rel-17 available slot counting the two diverge when a slot was skipped.
    for (unsigned occasion_idx = 1; occasion_idx <= grant.reps->tx_offsets.size(); ++occasion_idx) {
      const uint8_t                 offset    = grant.reps->tx_offsets[occasion_idx - 1];
      cell_slot_resource_allocator& rep_alloc = cell_alloc[final_k2 + offset];
      // Such slots were refused when the bundle was selected, and PUCCHs are allocated before the UL grants.
      ocudu_assert(not uci_alloc.has_harq_ack_on_common_pucch_res(u.crnti, rep_alloc.slot) and
                       not uci_alloc.has_pucch_repetition(u.crnti, rep_alloc.slot),
                   "rnti={}: PUSCH repetition occasion at slot={} carries a PUCCH whose UCI cannot be multiplexed",
                   u.crnti,
                   rep_alloc.slot);
      // An occasion must reuse the base grant's exact PRBs; they were picked against every slot of the bundle.
      ocudu_assert(not rep_alloc.ul_res_grid.collides(scs, pusch_td_cfg.symbols, crbs),
                   "rnti={}: PUSCH repetition occasion at slot={} lands on PRBs taken by another grant",
                   u.crnti,
                   rep_alloc.slot);
      ul_sched_info* rep_pdu = rep_alloc.result_builder().ul().puschs().emplace_back();
      ocudu_assert(rep_pdu != nullptr,
                   "rnti={}: no room left for the PUSCH repetition occasion at slot={}",
                   u.crnti,
                   rep_alloc.slot);
      rep_alloc.ul_res_grid.fill(grant_info{scs, pusch_td_cfg.symbols, crbs});
      ul_sched_info& rep_msg     = *rep_pdu;
      rep_msg                    = msg;
      rep_msg.uci                = std::nullopt;
      rep_msg.context.k2         = final_k2 + offset;
      rep_msg.pusch_cfg.new_data = false;
      rep_msg.pusch_cfg.rv_index = get_repetition_rv(rv, occasion_idx);
      rep_msg.pusch_cfg.repetitions->nof_remaining_repetitions =
          static_cast<uint8_t>(grant.reps->nof_occasions - 1U - occasion_idx);
      // As per TS 38.213 Section 9.2.5.2, a PUCCH overlapping a PUSCH with repetitions is not transmitted: its UCI
      // rides on the overlapped occasion. No-op without a PUCCH here; the DCI's aperiodic CSI stays on the base one.
      uci_alloc.multiplex_uci_on_pusch(rep_msg, rep_alloc, ue_cell_cfg, /* include_aperiodic_csi */ false);
      committed_repetition_slots.push_back(rep_alloc.slot);
    }
  }

  return committed_repetition_slots;
}

void ue_cell_grid_allocator::post_process_results()
{
  auto& slot_alloc = cell_alloc[0];

  // Remove cancelled allocations.
  for (auto it = slot_alloc.result_builder().dl().dl_pdcchs().begin();
       it != slot_alloc.result_builder().dl().dl_pdcchs().end();) {
    if (it->ctx.rnti == rnti_t::INVALID_RNTI) {
      it = slot_alloc.result_builder().dl().dl_pdcchs().erase(it);
    } else {
      ++it;
    }
  }
  for (auto it = slot_alloc.result_builder().dl().ul_pdcchs().begin();
       it != slot_alloc.result_builder().dl().ul_pdcchs().end();) {
    if (it->ctx.rnti == rnti_t::INVALID_RNTI) {
      it = slot_alloc.result_builder().dl().ul_pdcchs().erase(it);
    } else {
      ++it;
    }
  }
  for (auto it = slot_alloc.result_builder().dl().ue_grants().begin();
       it != slot_alloc.result_builder().dl().ue_grants().end();) {
    if (it->pdsch_cfg.rnti == rnti_t::INVALID_RNTI) {
      it = slot_alloc.result_builder().dl().ue_grants().erase(it);
    } else {
      ++it;
    }
  }
  for (auto it = slot_alloc.result_builder().ul().puschs().begin();
       it != slot_alloc.result_builder().ul().puschs().end();) {
    if (it->pusch_cfg.rnti == rnti_t::INVALID_RNTI) {
      it = slot_alloc.result_builder().ul().puschs().erase(it);
    } else {
      ++it;
    }
  }

  dl_grants.clear();
  ul_grants.clear();
}

ue_cell_grid_allocator::dl_repetition_occasion_list
ue_cell_grid_allocator::dl_newtx_grant_builder::set_pdsch_params(const vrb_alloc& alloc_vrbs, bool enable_interleaving)
{
  // Transfer the PDSCH parameters to the parent DL grant.
  dl_repetition_occasion_list committed_repetition_slots =
      parent->set_pdsch_params(parent->dl_grants[grant_index], alloc_vrbs, enable_interleaving);

  // Set PDSCH parameters and set parent as nullptr to avoid further modifications.
  parent = nullptr;
  return committed_repetition_slots;
}

ue_cell_grid_allocator::ul_repetition_occasion_list
ue_cell_grid_allocator::ul_newtx_grant_builder::set_pusch_params(const vrb_interval& alloc_vrbs)
{
  // Transfer the PUSCH parameters to the parent UL grant.
  ul_repetition_occasion_list committed_repetition_slots =
      parent->set_pusch_params(parent->ul_grants[grant_index], alloc_vrbs);

  // Set PUSCH parameters and set parent as nullptr to avoid further modifications.
  parent = nullptr;
  return committed_repetition_slots;
}
