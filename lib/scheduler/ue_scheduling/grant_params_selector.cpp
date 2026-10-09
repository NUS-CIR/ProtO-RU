// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "grant_params_selector.h"
#include "../config/time_domain_mapper.h"
#include "../slicing/slice_ue_repository.h"
#include "../support/csi_report_helpers.h"
#include "../support/dmrs_helpers.h"
#include "../support/prbs_calculator.h"
#include "../ue_context/ue_cell.h"
#include "ocudu/ran/csi_rs/csi_report_config.h"
#include "ocudu/ran/sch/tbs_calculator.h"
#include "ocudu/ran/transform_precoding/transform_precoding_helpers.h"
#include "ocudu/scheduler/support/rb_helper.h"
#include <utility>
#include <variant>

using namespace ocudu;
using namespace sched_helper;

namespace {

/// Estimation of the number of PRBs and MCS to use for a given number of pending bytes and channel state.
struct mcs_prbs_selection {
  /// Recommended MCS to use.
  sch_mcs_index mcs;
  /// Number of recommended PRBs for the PDSCH grant given the number of pending bytes and chosen MCS.
  unsigned nof_prbs;
};

} // namespace

static std::optional<mcs_prbs_selection> compute_newtx_required_mcs_and_prbs(const pdsch_config_params& pdsch_cfg,
                                                                             const ue_cell&             ue_cc,
                                                                             units::bytes               pending_bytes,
                                                                             interval<unsigned>         nof_rb_lims)
{
  // Note: At this point, CQI must be higher than 0, so MCS is valid.
  const sch_mcs_index       mcs = ue_cc.link_adaptation_controller().calculate_dl_mcs(pdsch_cfg.mcs_table).value();
  const sch_mcs_description mcs_config = pdsch_mcs_get_config(pdsch_cfg.mcs_table, mcs);

  sch_prbs_tbs prbs_tbs = get_nof_prbs(prbs_calculator_sch_config{pending_bytes.value(),
                                                                  pdsch_cfg.symbols.length(),
                                                                  calculate_nof_dmrs_per_rb(pdsch_cfg.dmrs),
                                                                  pdsch_cfg.nof_oh_prb,
                                                                  mcs_config,
                                                                  pdsch_cfg.nof_layers},
                                       nof_rb_lims.stop());
  ocudu_sanity_check(prbs_tbs.nof_prbs <= nof_rb_lims.stop(), "Error in RB computation");
  if (prbs_tbs.nof_prbs == 0) {
    return std::nullopt;
  }

  // Apply min RB grant size limits (max was applied before).
  prbs_tbs.nof_prbs = std::max(prbs_tbs.nof_prbs, nof_rb_lims.start());

  // [Implementation-defined] In case of partial slots and nof. PRBs allocated equals to 1 probability of KO is
  // high due to code not being able to cope with interference. So the solution is to increase the PRB allocation
  // to greater than 1 PRB.
  if (prbs_tbs.nof_prbs == 1 and pdsch_cfg.symbols.length() < NOF_OFDM_SYM_PER_SLOT_NORMAL_CP) {
    prbs_tbs.nof_prbs = 2;
  }

  return mcs_prbs_selection{mcs.value(), prbs_tbs.nof_prbs};
}

/// Compute PUSCH grant parameters for a newTx/reTx given the UE state, DCI type and PUSCH time-domain resource.
static pusch_config_params compute_pusch_config_params(const ue_cell&                               ue_cc,
                                                       dci_ul_rnti_config_type                      dci_type,
                                                       unsigned                                     nof_layers,
                                                       const pusch_time_domain_resource_allocation& pusch_td_cfg,
                                                       unsigned                                     uci_bits,
                                                       bool                                         is_csi_report_slot)
{
  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();
  const cell_configuration&    cell_cfg    = ue_cc.cfg().cell_cfg_common;
  const bwp_uplink_common&     bwp_ul_cmn  = ue_cell_cfg.bwp(ue_cc.active_bwp_id()).ul.common();

  pusch_config_params pusch_cfg;
  switch (dci_type) {
    case dci_ul_rnti_config_type::tc_rnti_f0_0:
      pusch_cfg = get_pusch_config_f0_0_tc_rnti(cell_cfg, pusch_td_cfg);
      break;
    case dci_ul_rnti_config_type::c_rnti_f0_0:
      pusch_cfg =
          get_pusch_config_f0_0_c_rnti(cell_cfg, &ue_cell_cfg, bwp_ul_cmn, pusch_td_cfg, uci_bits, is_csi_report_slot);
      break;
    case dci_ul_rnti_config_type::c_rnti_f0_1:
      pusch_cfg = get_pusch_config_f0_1_c_rnti(ue_cell_cfg, pusch_td_cfg, nof_layers, uci_bits, is_csi_report_slot);
      break;
    default:
      report_fatal_error("Unsupported PDCCH DCI UL format");
  }

  return pusch_cfg;
}

/// Compute PUSCH grant parameters for a newTx given the UE state, DCI type and PUSCH time-domain resource.
static pusch_config_params compute_newtx_pusch_config_params(const ue_cell&                               ue_cc,
                                                             dci_ul_rnti_config_type                      dci_type,
                                                             const pusch_time_domain_resource_allocation& pusch_td_cfg,
                                                             unsigned                                     uci_bits,
                                                             bool is_csi_report_slot)
{
  return compute_pusch_config_params(
      ue_cc, dci_type, ue_cc.channel_state_manager().get_nof_ul_layers(), pusch_td_cfg, uci_bits, is_csi_report_slot);
}

/// Compute PUSCH grant parameters for a reTx given the UE state, DCI type and PUSCH time-domain resource.
pusch_config_params static compute_retx_pusch_config_params(const ue_cell&                               ue_cc,
                                                            const ul_harq_process_handle&                h_ul,
                                                            const pusch_time_domain_resource_allocation& pusch_td_cfg,
                                                            unsigned                                     uci_bits,
                                                            bool is_csi_report_slot)
{
  return compute_pusch_config_params(ue_cc,
                                     h_ul.get_grant_params().dci_cfg_type,
                                     h_ul.get_grant_params().nof_layers,
                                     pusch_td_cfg,
                                     uci_bits,
                                     is_csi_report_slot);
}

/// Derive recommended MCS and number of PRBs for a newTx PUSCH grant.
static std::optional<mcs_prbs_selection> compute_newtx_required_mcs_and_prbs(const pusch_config_params& pusch_cfg,
                                                                             const ue_cell&             ue_cc,
                                                                             units::bytes               pending_bytes,
                                                                             interval<unsigned>         nof_rb_lims)
{
  sch_mcs_index mcs =
      ue_cc.link_adaptation_controller().calculate_ul_mcs(pusch_cfg.mcs_table, pusch_cfg.use_transform_precoder);
  sch_mcs_description mcs_config =
      pusch_mcs_get_config(pusch_cfg.mcs_table, mcs, pusch_cfg.use_transform_precoder, pusch_cfg.tp_pi2bpsk_present);

  const auto nof_symbols = pusch_cfg.symbols.length();

  sch_prbs_tbs prbs_tbs = get_nof_prbs(prbs_calculator_sch_config{pending_bytes.value(),
                                                                  nof_symbols,
                                                                  calculate_nof_dmrs_per_rb(pusch_cfg.dmrs),
                                                                  pusch_cfg.nof_oh_prb,
                                                                  mcs_config,
                                                                  pusch_cfg.nof_layers},
                                       nof_rb_lims.stop());

  // Apply minimum grant size.
  unsigned nof_prbs = std::max(prbs_tbs.nof_prbs, nof_rb_lims.start());

  // We need to adjust the number of PRBs to the PHR, to prevent the UE from reducing the nominal TX power to meet the
  // max TX power.
  nof_prbs = ue_cc.get_pusch_power_controller().adapt_pusch_prbs_to_phr(nof_prbs);

  // Ensure code rate and UCI is valid. If not, increase the number of PRBs.
  // Note: We make the pessimistic assumption that the DC intersects the newTx grant.
  static constexpr bool contains_dc = true;
  while (nof_prbs <= nof_rb_lims.stop() and
         not is_pusch_effective_rate_valid(pusch_cfg, ue_cc.active_bwp(), mcs, nof_prbs, contains_dc)) {
    ++nof_prbs;
  }
  if (nof_prbs > nof_rb_lims.stop()) {
    // No valid MCS-PRB allocation.
    return std::nullopt;
  }

  if (mcs == 5U and nof_prbs == 1U and nof_prbs < nof_rb_lims.stop()) {
    // [Implementation-defined] In our tests, we have seen that MCS 5 with 1 PRB can lead (depending on the
    // configuration) to a non-valid MCS-PRB allocation; therefore, we increase the number of 1 PRBs.
    // TODO: Remove this part and handle the problem with a loop that is general for any configuration.
    ++nof_prbs;
  }

  // Ensure the number of PRB is valid if the transform precoder is used. The condition the PUSCH bandwidth with
  // transform precoder is defined in TS 38.211 Section 6.1.3.
  // The number of PRB must be lower than or equal to current number of PRB.
  if (pusch_cfg.use_transform_precoder) {
    nof_prbs = transform_precoding::get_nof_prbs_lower_bound(nof_prbs).value_or(nof_prbs);
  }

  if (nof_prbs == 0) {
    // No valid MCS-PRB allocation.
    return std::nullopt;
  }
  return mcs_prbs_selection{mcs, nof_prbs};
}

static std::optional<mcs_prbs_selection>
compute_retx_nof_rbs_mcs(const pdsch_config_params&                  pdsch_cfg,
                         const dl_harq_process_handle::grant_params& prev_h_params,
                         const interval<unsigned>&                   rb_lims)
{
  if (pdsch_cfg.symbols.length() == prev_h_params.nof_symbols) {
    // Number of symbols did not change. Reuse the same MCS and TBS of previous HARQ transmission.
    const unsigned nof_rbs = prev_h_params.rbs.nof_vrbs();
    if (nof_rbs > rb_lims.stop()) {
      // If we are exceeding imposed RB limits, postpone reTx.
      return std::nullopt;
    }
    return mcs_prbs_selection{prev_h_params.mcs, nof_rbs};
  }
  return std::nullopt;
}

static std::optional<dl_sched_context> get_dl_sched_context(const slice_ue&                 u,
                                                            slot_point                      pdcch_slot,
                                                            slot_point                      pdsch_slot,
                                                            bool                            interleaving_enabled,
                                                            const dl_harq_process_handle*   h_dl,
                                                            units::bytes                    pending_bytes,
                                                            dl_transport_block_restrictions restrictions,
                                                            unsigned                        max_rbs = MAX_NOF_PRBS)
{
  const ue_cell& ue_cc = u.get_cc();

  if (not ue_cc.is_pdsch_enabled(pdcch_slot, pdsch_slot) or ue_cc.is_in_fallback_mode()) {
    // The UE cannot be scheduled in the provided slots.
    return std::nullopt;
  }

  const ue_cell_configuration& ue_cell_cfg      = ue_cc.cfg();
  const cell_configuration&    cell_cfg         = ue_cell_cfg.cell_cfg_common;
  unsigned                     slot_nof_symbols = cell_cfg.get_nof_dl_symbol_per_slot(pdsch_slot);

  // TODO: Support more search spaces.
  static constexpr search_space_id ue_ded_ss_id = to_search_space_id(2);
  const search_space_info&         ss           = ue_cc.cfg().search_space(ue_ded_ss_id);

  if (h_dl != nullptr) {
    // ReTx case.
    ocudu_assert(ss.get_dl_dci_format() == get_dci_format(h_dl->get_grant_params().dci_cfg_type),
                 "DCI type cannot change across reTxs");
  }

  // The UE configuration validator rejects resource allocation type 0 in cells with interleaved VRB-to-PRB mapping.
  ocudu_sanity_check(not interleaving_enabled or ss.get_dl_dci_format() != dci_dl_format::f1_1 or
                         ss.bwp->dl.pdsch().ded() == nullptr or
                         ss.bwp->dl.pdsch().ded()->res_alloc ==
                             pdsch_config::resource_allocation::resource_allocation_type_1,
                     "PDSCH resource allocation type 0 is not supported with interleaved VRB-to-PRB mapping");

  // For a newTx, link adaptation decides how many Rel-16 PDSCH repetitions to request, or nullopt for a single
  // transmission. A reTx reuses the scheme of the original transmission, so its count comes from the HARQ grant
  // params (like the number of layers), not from the current link quality.
  std::optional<uint8_t> nof_repetitions;
  if (h_dl == nullptr) {
    nof_repetitions = ue_cc.link_adaptation_controller().select_pdsch_repetition_count(ss);
  } else if (h_dl->get_grant_params().nof_repetitions > 1) {
    nof_repetitions = h_dl->get_grant_params().nof_repetitions;
  }

  // Determine RB allocation limits.
  const auto crb_lims = (cell_cfg.expert_cfg.ue.pdsch_crb_limits & ss.dl_crb_lims).convert_to<crb_interval>();
  // For reTx, additionally cap by the slice's max RB budget to prevent overflowing the slice allocation.
  const interval<unsigned> slice_max_lims =
      (h_dl != nullptr) ? interval<unsigned>{0, max_rbs} : interval<unsigned>{0, MAX_NOF_PRBS};
  const interval<unsigned> nof_rb_lims =
      cell_cfg.expert_cfg.ue.pdsch_nof_rbs &
      ue_cell_cfg.rrm_cfg().pdsch_grant_size_limits.convert_to<interval<unsigned>>() &
      interval<unsigned>{0, crb_lims.length()} & slice_max_lims;
  if (nof_rb_lims.empty()) {
    // Invalid RB allocation range.
    return std::nullopt;
  }

  // Find the best PDSCH time-domain resource candidate matching the requested transmission regime: when
  // \c nof_repetitions is set, only the TDRA row carrying that repetitionNumber-r16 is eligible; when unset, only
  // single-transmission rows. Among eligible rows the longest allocation wins.
  const dl_time_domain_mapper& dl_td_mapper = ss.bwp->dl.td_mapper();

  std::optional<unsigned> selected_pdsch_td_index;
  // NOTE: pdcch_slot is the slot at which candidates are generated by inter_slice_scheduler, which already filters
  // PDSCH TD resource indices via the mapper's precomputed per-slot index set. Restricting the search to that same
  // set avoids scanning candidates known not to apply to pdcch_slot.
  const dci_dl_format dci_format = ss.get_dl_dci_format();
  for (const uint8_t pdsch_td_index : dl_td_mapper.pdsch_td_res_indices(dci_format, pdcch_slot.count())) {
    const pdsch_time_domain_resource_allocation& pdsch_td_res =
        dl_td_mapper.pdsch_td_resources(dci_format)[pdsch_td_index];

    // Consider only rows matching the requested repetition count (nullopt = single transmission).
    if (pdsch_td_res.rep_number != nof_repetitions) {
      continue;
    }

    // Check whether k0 matches the chosen PDSCH slot
    // - PDSCH time domain resource last symbol is lower than the total number of DL symbols of the slot.
    // - PDSCH time domain resource does not overlap with CORESET.
    if (pdcch_slot + pdsch_td_res.k0 != pdsch_slot) {
      continue;
    }

    // In case of HARQ retxs, skip (i) candidates where the number of symbols differs from the original grant and (ii)
    // candidates where the total number of symbols (PDCCH + PDSCH) doesn't match the available number of DL symbols.
    // Without this second condition, it would still be possible to allocate a retx (whose original TX was in a special
    // slot) in a full DL slot, as long there is a candidate for it.
    if (h_dl != nullptr and (h_dl->get_grant_params().nof_symbols != pdsch_td_res.symbols.length() or
                             ss.coreset->cfg().duration() + pdsch_td_res.symbols.length() != slot_nof_symbols)) {
      continue;
    }

    if (not selected_pdsch_td_index.has_value() or
        dl_td_mapper.pdsch_td_resources(dci_format)[*selected_pdsch_td_index].symbols.length() <
            pdsch_td_res.symbols.length()) {
      // A better PDSCH time-domain resource candidate was found.
      selected_pdsch_td_index = pdsch_td_index;
    }
  }
  if (not selected_pdsch_td_index.has_value()) {
    // No adequate PDSCH time-domain resource candidate was found.
    return std::nullopt;
  }

  // Compute recommended number of layers, MCS and PRBs.
  unsigned                          nof_layers;
  std::optional<mcs_prbs_selection> mcs_prbs_sel;
  if (h_dl == nullptr) {
    // NewTx Case.
    nof_layers = ue_cc.channel_state_manager().get_nof_dl_layers();
    if (nof_repetitions.has_value()) {
      // TS 38.214 limits PDSCH repetition Type A to at most 2 layers.
      nof_layers = std::min(nof_layers, 2U);
    }
    const pdsch_config_params& pdsch_cfg = ss.get_pdsch_config(*selected_pdsch_td_index, nof_layers);
    mcs_prbs_sel = compute_newtx_required_mcs_and_prbs(pdsch_cfg, ue_cc, pending_bytes, nof_rb_lims);
  } else {
    // ReTx Case.
    const auto& prev_params              = h_dl->get_grant_params();
    nof_layers                           = prev_params.nof_layers;
    const pdsch_config_params& pdsch_cfg = ss.get_pdsch_config(*selected_pdsch_td_index, nof_layers);
    mcs_prbs_sel                         = compute_retx_nof_rbs_mcs(pdsch_cfg, prev_params, nof_rb_lims);
  }
  if (not mcs_prbs_sel.has_value()) {
    return std::nullopt;
  }

  dl_sched_context ctxt;
  ctxt.ss_id              = ss.cfg->get_id();
  ctxt.pdsch_td_res_index = *selected_pdsch_td_index;
  ctxt.recommended_mcs    = mcs_prbs_sel->mcs;
  ctxt.recommended_ri     = nof_layers;
  ctxt.nof_rb_lims        = nof_rb_lims;
  ctxt.expected_nof_rbs   = mcs_prbs_sel->nof_prbs;
  ctxt.pending_bytes      = units::bytes{pending_bytes};
  ctxt.restrictions       = restrictions;
  ctxt.nof_repetitions    = dl_td_mapper.pdsch_td_resources(dci_format)[*selected_pdsch_td_index].rep_number;
  return ctxt;
}

std::optional<dl_sched_context> sched_helper::get_newtx_dl_sched_context(const slice_ue& u,
                                                                         slot_point      pdcch_slot,
                                                                         slot_point      pdsch_slot,
                                                                         bool            interleaving_enabled,
                                                                         units::bytes    pending_bytes,
                                                                         dl_transport_block_restrictions restrictions)
{
  return get_dl_sched_context(u, pdcch_slot, pdsch_slot, interleaving_enabled, nullptr, pending_bytes, restrictions);
}

std::optional<dl_sched_context> sched_helper::get_retx_dl_sched_context(const slice_ue& u,
                                                                        slot_point      pdcch_slot,
                                                                        slot_point      pdsch_slot,
                                                                        bool            interleaving_enabled,
                                                                        const dl_harq_process_handle& h_dl,
                                                                        unsigned                      max_rbs)
{
  return get_dl_sched_context(u, pdcch_slot, pdsch_slot, interleaving_enabled, &h_dl, units::bytes{0}, {}, max_rbs);
}

/// Gets the PDSCH resource allocation type that can be used in a DL grant.
static pdsch_config::resource_allocation get_dl_resource_allocation(const search_space_info& ss_info,
                                                                    bool                     interleaving_enabled)
{
  const pdsch_config* pdsch_ded = ss_info.bwp->dl.pdsch().ded();
  // Resource allocation type 0 is only available in DCI format 1_1 and with non-interleaved VRB-to-PRB mapping, as per
  // TS 38.212, Section 7.3.1.2.2, and TS 38.214, Section 5.1.2.3.
  if (pdsch_ded == nullptr or ss_info.get_dl_dci_format() != dci_dl_format::f1_1 or interleaving_enabled) {
    return pdsch_config::resource_allocation::resource_allocation_type_1;
  }
  return pdsch_ded->res_alloc;
}

/// \brief Selects free RBGs of the BWP, from lowest to highest, until \c target_nof_rbs VRBs are reached.
///
/// RBGs with any used VRB are skipped, and so are RBGs that would make the allocation exceed \c max_nof_rbs VRBs.
static vrb_bitmap find_available_rbgs(const search_space_info& ss_info,
                                      const vrb_bitmap&        used_vrbs,
                                      unsigned                 target_nof_rbs,
                                      unsigned                 max_nof_rbs)
{
  const crb_interval     bwp_crbs  = ss_info.bwp->dl.cfg().crbs;
  const nominal_rbg_size P         = get_nominal_rbg_size(bwp_crbs.length(), ss_info.bwp->dl.pdsch().ded()->rbg_sz);
  const rbg_bitmap       used_rbgs = convert_vrbs_to_rbgs(used_vrbs, bwp_crbs, P);

  rbg_bitmap selected_rbgs(used_rbgs.size());
  unsigned   nof_rbs = 0;
  for (unsigned rbg_idx = 0, nof_rbgs = used_rbgs.size(); rbg_idx != nof_rbgs and nof_rbs < target_nof_rbs; ++rbg_idx) {
    const unsigned rbg_size = get_rbg_size(bwp_crbs, P, rbg_idx);
    if (used_rbgs.test(rbg_idx) or nof_rbs + rbg_size > max_nof_rbs) {
      continue;
    }
    selected_rbgs.set(rbg_idx);
    nof_rbs += rbg_size;
  }
  return convert_rbgs_to_vrbs(selected_rbgs, bwp_crbs, P);
}

/// \brief Selects the VRBs of a DL grant.
///
/// \param[in] target_nof_rbs Number of VRBs to allocate.
/// \param[in] nof_rbs_remaining Hard limit on the VRBs of a type 0 grant, which may exceed \c target_nof_rbs to fit
/// whole RBGs.
static vrb_alloc find_available_vrbs(const search_space_info& ss_info,
                                     const vrb_bitmap&        used_vrbs,
                                     bool                     interleaving_enabled,
                                     unsigned                 target_nof_rbs,
                                     unsigned                 nof_rbs_remaining)
{
  const pdsch_config::resource_allocation res_alloc = get_dl_resource_allocation(ss_info, interleaving_enabled);
  if (res_alloc == pdsch_config::resource_allocation::resource_allocation_type_0) {
    return find_available_rbgs(ss_info, used_vrbs, target_nof_rbs, nof_rbs_remaining);
  }

  const vrb_interval vrbs = rb_helper::find_empty_interval_of_length(used_vrbs, target_nof_rbs);
  if (res_alloc == pdsch_config::resource_allocation::resource_allocation_type_1 or vrbs.length() >= target_nof_rbs) {
    return vrbs;
  }

  // [Implementation-defined] With dynamic switch, type 0 is only used when it fits more VRBs than type 1.
  const vrb_bitmap rbg_vrbs = find_available_rbgs(ss_info, used_vrbs, target_nof_rbs, nof_rbs_remaining);
  if (rbg_vrbs.count() > vrbs.length()) {
    return rbg_vrbs;
  }
  return vrbs;
}

unsigned sched_helper::get_min_newtx_dl_nof_vrbs(const search_space_info& ss_info, bool interleaving_enabled)
{
  if (get_dl_resource_allocation(ss_info, interleaving_enabled) !=
      pdsch_config::resource_allocation::resource_allocation_type_0) {
    return 1;
  }
  const crb_interval bwp_crbs = ss_info.bwp->dl.cfg().crbs;
  return to_underlying(get_nominal_rbg_size(bwp_crbs.length(), ss_info.bwp->dl.pdsch().ded()->rbg_sz));
}

vrb_alloc sched_helper::compute_newtx_dl_vrbs(const dl_sched_context&  decision_ctxt,
                                              const search_space_info& ss_info,
                                              const vrb_bitmap&        used_vrbs,
                                              bool                     interleaving_enabled,
                                              unsigned                 max_nof_rbs,
                                              unsigned                 nof_rbs_remaining)
{
  // A reTx keeps the VRB count of the newTx, so rounding up to whole RBGs must not exceed the UE RB limits.
  const unsigned max_type0_nof_rbs =
      std::min(std::max(max_nof_rbs, nof_rbs_remaining), decision_ctxt.nof_rb_lims.stop());
  return find_available_vrbs(ss_info,
                             used_vrbs,
                             interleaving_enabled,
                             std::min(decision_ctxt.expected_nof_rbs, max_nof_rbs),
                             max_type0_nof_rbs);
}

vrb_alloc sched_helper::compute_retx_dl_vrbs(const dl_sched_context&  decision_ctxt,
                                             const search_space_info& ss_info,
                                             const vrb_bitmap&        used_vrbs,
                                             bool                     interleaving_enabled)
{
  // In case of Retx, the #VRBs need to stay the same.
  const unsigned                          nof_rbs   = decision_ctxt.expected_nof_rbs;
  const pdsch_config::resource_allocation res_alloc = get_dl_resource_allocation(ss_info, interleaving_enabled);
  if (res_alloc != pdsch_config::resource_allocation::resource_allocation_type_0) {
    const vrb_interval vrbs = rb_helper::find_empty_interval_of_length(used_vrbs, nof_rbs);
    if (vrbs.length() == nof_rbs) {
      return vrbs;
    }
    if (res_alloc == pdsch_config::resource_allocation::resource_allocation_type_1) {
      return vrb_interval{};
    }
  }
  const crb_interval     bwp_crbs = ss_info.bwp->dl.cfg().crbs;
  const nominal_rbg_size P        = get_nominal_rbg_size(bwp_crbs.length(), ss_info.bwp->dl.pdsch().ded()->rbg_sz);
  const rbg_bitmap       rbgs =
      find_free_rbgs_with_nof_vrbs(convert_vrbs_to_rbgs(used_vrbs, bwp_crbs, P), bwp_crbs, P, nof_rbs);
  if (rbgs.none()) {
    return vrb_interval{};
  }
  return convert_rbgs_to_vrbs(rbgs, bwp_crbs, P);
}

static std::optional<mcs_prbs_selection>
compute_retx_nof_rbs_mcs(const pusch_config_params&                  pusch_cfg,
                         const ul_harq_process_handle::grant_params& prev_h_params,
                         const ue_cell&                              ue_cc,
                         const interval<unsigned>&                   rb_lims)
{
  if (pusch_cfg.symbols.length() == prev_h_params.nof_symbols) {
    // Number of symbols did not change. Reuse the same MCS and TBS of previous HARQ transmission.
    const unsigned nof_rbs = prev_h_params.rbs.nof_vrbs();
    if (nof_rbs > rb_lims.stop()) {
      // ReTx would exceed the assigned RB limits. Postpone it.
      return std::nullopt;
    }
    // Re-validate TBS: UCI overhead can change across retransmissions.
    // Note: Assume worst-case that the grant would collide with DC.
    static constexpr bool contains_dc = true;
    const auto            tbs = compute_ul_tbs(pusch_cfg, ue_cc.active_bwp(), prev_h_params.mcs, nof_rbs, contains_dc);
    if (not tbs.has_value() or tbs.value() != prev_h_params.tbs) {
      // UCI would lead to an invalid code rate.
      return std::nullopt;
    }
    return mcs_prbs_selection{prev_h_params.mcs, nof_rbs};
  }
  return std::nullopt;
}

/// Determine whether a CSI report has to be accounted for in the UL grant of \c pusch_slot.
/// \param[in] bundle_tx_offsets When the grant is a PUSCH repetition bundle, the slot offsets of its occasions
/// beyond the base one.
static bool is_csi_included(const ue_cell& ue_cc, slot_point pusch_slot, span<const uint8_t> bundle_tx_offsets)
{
  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();
  const cell_configuration&    cell_cfg    = ue_cell_cfg.cell_cfg_common;

  bool include_csi = false;
  if (ue_cell_cfg.csi_meas_cfg() != nullptr) {
    // TODO: pass this through the scheduler config instead.
    auto aperiodic_csi_prohibit_time_slots =
        static_cast<unsigned>(ue_cell_cfg.csi_meas_cfg()->nzp_csi_rs_res_list[0].csi_res_period.value());
    if (std::holds_alternative<csi_report_config::aperiodic_report>(
            ue_cell_cfg.csi_meas_cfg()->csi_report_cfg_list[0].report_cfg_type) and
        ue_cc.channel_state_manager().is_aperiodic_csi_allowed(pusch_slot, aperiodic_csi_prohibit_time_slots)) {
      include_csi = true;
    } else if (std::holds_alternative<csi_report_config::periodic_or_semi_persistent_report_on_pucch>(
                   ue_cell_cfg.csi_meas_cfg()->csi_report_cfg_list[0].report_cfg_type)) {
      // A bundle carries the UCI of any of its slots, so a periodic CSI report due in any of them has to be sized
      // for here.
      // CSI period and offset are in the UE uplink slot numbering, the slots checked here are on the gNB DL clock.
      const auto is_csi_slot = [&](slot_point sl) {
        return csi_helper::is_csi_reporting_slot(*ue_cell_cfg.init_bwp().ul.ue_cfg()->periodic_csi_report,
                                                 cell_cfg.params.init_bwp.csi->csi_rs_period,
                                                 cell_cfg.dl_to_ue_ul_slot(sl));
      };
      include_csi = is_csi_slot(pusch_slot) or
                    std::any_of(bundle_tx_offsets.begin(), bundle_tx_offsets.end(), [&](uint8_t offset) {
                      return is_csi_slot(pusch_slot + offset);
                    });
    }
  }
  return include_csi;
}

/// Fill in the UL grant parameters that depend on the UCI payload: the PUSCH config params and the resulting MCS
/// and RB count. Everything else in \c ctxt is UCI-independent.
/// \return false if no valid MCS/RB combination exists, in which case \c ctxt is left untouched.
static bool size_ul_grant(ul_sched_context&                            ctxt,
                          const ue_cell&                               ue_cc,
                          const search_space_info&                     ss,
                          const pusch_time_domain_resource_allocation& pusch_td_res,
                          unsigned                                     uci_nof_harq_bits,
                          const ul_harq_process_handle*                h_ul,
                          bool                                         include_csi)
{
  pusch_config_params               pusch_cfg;
  std::optional<mcs_prbs_selection> mcs_prbs_sel;
  if (h_ul == nullptr) {
    // NewTx Case.
    dci_ul_rnti_config_type dci_type = ss.get_ul_dci_format() == dci_ul_format::f0_0
                                           ? dci_ul_rnti_config_type::c_rnti_f0_0
                                           : dci_ul_rnti_config_type::c_rnti_f0_1;
    // Note: We assume k2 <= k1, which means that all the HARQ bits are set at this point for this UL slot and UE.
    pusch_cfg    = compute_newtx_pusch_config_params(ue_cc, dci_type, pusch_td_res, uci_nof_harq_bits, include_csi);
    mcs_prbs_sel = compute_newtx_required_mcs_and_prbs(pusch_cfg, ue_cc, ctxt.pending_bytes, ctxt.nof_rb_lims);
  } else {
    // ReTx Case.
    // Compute if effective code rate does not go over the limit for this reTx, for instance, due to presence of UCI.
    pusch_cfg    = compute_retx_pusch_config_params(ue_cc, *h_ul, pusch_td_res, uci_nof_harq_bits, include_csi);
    mcs_prbs_sel = compute_retx_nof_rbs_mcs(pusch_cfg, h_ul->get_grant_params(), ue_cc, ctxt.nof_rb_lims);
  }
  if (not mcs_prbs_sel.has_value()) {
    return false;
  }

  ctxt.recommended_mcs  = mcs_prbs_sel->mcs;
  ctxt.expected_nof_rbs = mcs_prbs_sel->nof_prbs;
  ctxt.pusch_cfg        = pusch_cfg;
  return true;
}

static std::optional<ul_sched_context> get_ul_sched_context(const slice_ue&               u,
                                                            slot_point                    pdcch_slot,
                                                            slot_point                    pusch_slot,
                                                            unsigned                      uci_nof_harq_bits,
                                                            const ul_harq_process_handle* h_ul,
                                                            units::bytes                  pending_bytes,
                                                            ofdm_symbol_range             allowed_symbols,
                                                            unsigned                      max_rbs = MAX_NOF_PRBS)
{
  const ue_cell& ue_cc = u.get_cc();

  if (not ue_cc.is_pusch_enabled(pdcch_slot, pusch_slot) or ue_cc.is_in_fallback_mode()) {
    // The UE cannot be scheduled in the provided slots.
    return std::nullopt;
  }

  // If there is a CG configured and this is a CG slot, then we skip the PUSCH dynamic grant allocation.
  // CG period/offset are in the UE's UL slot numbering, pusch_slot is on the gNB DL clock.
  if (ue_cc.get_conf_grant_state_manager().is_cg_slot(ue_cc.cfg().cell_cfg_common.dl_to_ue_ul_slot(pusch_slot))) {
    return std::nullopt;
  }

  const ue_cell_configuration& ue_cell_cfg = ue_cc.cfg();
  const cell_configuration&    cell_cfg    = ue_cell_cfg.cell_cfg_common;
  const sched_bwp_config&      bwp_cfg     = ue_cc.active_bwp();

  // TODO: Support more search spaces.
  static constexpr search_space_id ue_ded_ss_id = to_search_space_id(2);
  const search_space_info&         ss           = ue_cc.cfg().search_space(ue_ded_ss_id);

  if (h_ul != nullptr) {
    // ReTx case.
    ocudu_assert(ss.get_ul_dci_format() == get_dci_format(h_ul->get_grant_params().dci_cfg_type),
                 "DCI type cannot change across reTxs");
  }

  // For a newTx, link adaptation decides how many Rel-16 PUSCH repetitions to request, or nullopt for a single
  // transmission. A reTx reuses the scheme of the original transmission, so its count comes from the HARQ grant
  // params (like the number of layers), not from the current link quality.
  std::optional<uint8_t> nof_repetitions;
  if (h_ul == nullptr) {
    nof_repetitions = ue_cc.link_adaptation_controller().select_pusch_repetition_count(ss);
  } else if (h_ul->get_grant_params().nof_repetitions > 1) {
    nof_repetitions = h_ul->get_grant_params().nof_repetitions;
  }

  // Determine RB allocation limits.
  // For reTx, additionally cap by the slice's max RB budget to prevent overflowing the slice allocation.
  const interval<unsigned> slice_max_lims =
      (h_ul != nullptr) ? interval<unsigned>{0, max_rbs} : interval<unsigned>{0, MAX_NOF_PRBS};
  interval<unsigned> nof_rb_lims = cell_cfg.expert_cfg.ue.pusch_nof_rbs &
                                   ue_cell_cfg.rrm_cfg().pusch_grant_size_limits.convert_to<interval<unsigned>>() &
                                   slice_max_lims;
  const auto crb_lims = cell_cfg.expert_cfg.ue.pusch_crb_limits & ss.ul_crb_lims;
  const auto prb_lims = crb_to_prb(ss.ul_crb_lims, crb_lims);
  const auto vrb_lims = prb_lims.convert_to<vrb_interval>();
  nof_rb_lims         = nof_rb_lims & interval<unsigned>{0, vrb_lims.length()};
  if (nof_rb_lims.empty()) {
    // Invalid RB allocation range.
    return std::nullopt;
  }

  // Make sure the PUSCH time resource symbols fit within the UL symbols available in this slot; this is to avoid
  // allocating the PUSCH over SRS resource symbols, if any. For a reTx, the time resource must also match the
  // number of symbols used by the original transmission.
  const std::optional<uint8_t> retx_symbols =
      h_ul != nullptr ? std::optional<uint8_t>{h_ul->get_grant_params().nof_symbols} : std::nullopt;
  // The search also reports the single-transmission row qualifying for this slot, so that a bundle found later not
  // to be schedulable can be downgraded to it without searching again.
  const ul_time_domain_mapper::pusch_td_res_selection td_res_sel =
      bwp_cfg.ul.td_mapper().find_pusch_td_res_indices(ss.get_ul_dci_format(),
                                                       pdcch_slot,
                                                       pusch_slot,
                                                       allowed_symbols,
                                                       cell_cfg.ul_delay(u.get_cc().ntn_koffset()),
                                                       nof_repetitions,
                                                       retx_symbols);
  std::optional<uint8_t> pusch_td_index  = td_res_sel.selected;
  std::optional<uint8_t> single_tx_index = td_res_sel.single_tx;
  if (not pusch_td_index.has_value()) {
    // No row carries the requested repetition count in this slot, so the single-transmission row found alongside it
    // takes over right away -- leaving nothing to fall back to later.
    pusch_td_index = std::exchange(single_tx_index, std::nullopt);
  }
  if (not pusch_td_index.has_value()) {
    return std::nullopt;
  }
  const pusch_time_domain_resource_allocation& pusch_td_res =
      bwp_cfg.ul.td_mapper().pusch_td_resources(ss.get_ul_dci_format())[*pusch_td_index];

  // Fill in the grant parameters that do not depend on the UCI payload.
  ul_sched_context ctxt;
  ctxt.ss_id                        = ss.cfg->get_id();
  ctxt.pusch_td_res_index           = *pusch_td_index;
  ctxt.vrb_lims                     = vrb_lims;
  ctxt.nof_rb_lims                  = nof_rb_lims;
  ctxt.pending_bytes                = pending_bytes;
  ctxt.nof_repetitions              = pusch_td_res.nof_repetitions;
  ctxt.single_tx_pusch_td_res_index = single_tx_index;

  // Compute recommended number of layers, MCS and PRBs.
  if (not size_ul_grant(
          ctxt, ue_cc, ss, pusch_td_res, uci_nof_harq_bits, h_ul, is_csi_included(ue_cc, pusch_slot, {}))) {
    return std::nullopt;
  }

  // Successful selection of grant parameters.
  return ctxt;
}

std::optional<ul_sched_context> sched_helper::get_newtx_ul_sched_context(const slice_ue&   u,
                                                                         slot_point        pdcch_slot,
                                                                         slot_point        pusch_slot,
                                                                         unsigned          uci_nof_harq_bits,
                                                                         units::bytes      pending_bytes,
                                                                         ofdm_symbol_range allowed_symbols)
{
  return get_ul_sched_context(u, pdcch_slot, pusch_slot, uci_nof_harq_bits, nullptr, pending_bytes, allowed_symbols);
}

std::optional<ul_sched_context> sched_helper::get_retx_ul_sched_context(const slice_ue&               u,
                                                                        slot_point                    pdcch_slot,
                                                                        slot_point                    pusch_slot,
                                                                        unsigned                      uci_nof_harq_bits,
                                                                        const ul_harq_process_handle& h_ul,
                                                                        ofdm_symbol_range             allowed_symbols,
                                                                        unsigned                      max_rbs)
{
  return get_ul_sched_context(
      u, pdcch_slot, pusch_slot, uci_nof_harq_bits, &h_ul, units::bytes{0}, allowed_symbols, max_rbs);
}

static bool resize_ul_grant_for_uci(ul_sched_context&             ctxt,
                                    const slice_ue&               u,
                                    slot_point                    pusch_slot,
                                    unsigned                      uci_nof_harq_bits,
                                    const ul_harq_process_handle* h_ul,
                                    span<const uint8_t>           bundle_tx_offsets)
{
  const ue_cell&                               ue_cc = u.get_cc();
  const search_space_info&                     ss    = ue_cc.cfg().search_space(ctxt.ss_id);
  const pusch_time_domain_resource_allocation& pusch_td_res =
      ue_cc.active_bwp().ul.td_mapper().pusch_td_resources(ss.get_ul_dci_format())[ctxt.pusch_td_res_index];

  return size_ul_grant(
      ctxt, ue_cc, ss, pusch_td_res, uci_nof_harq_bits, h_ul, is_csi_included(ue_cc, pusch_slot, bundle_tx_offsets));
}

static bool downgrade_ul_grant_to_single_tx(ul_sched_context&             ctxt,
                                            const slice_ue&               u,
                                            slot_point                    pusch_slot,
                                            unsigned                      uci_nof_harq_bits,
                                            const ul_harq_process_handle* h_ul)
{
  if (not ctxt.single_tx_pusch_td_res_index.has_value()) {
    return false;
  }
  const ue_cell&                               ue_cc = u.get_cc();
  const search_space_info&                     ss    = ue_cc.cfg().search_space(ctxt.ss_id);
  const pusch_time_domain_resource_allocation& pusch_td_res =
      ue_cc.active_bwp().ul.td_mapper().pusch_td_resources(ss.get_ul_dci_format())[*ctxt.single_tx_pusch_td_res_index];

  // The row taking over spans its own number of symbols, so the sizing of the repetition row does not carry over.
  // Size a copy, leaving the caller's context untouched if nothing fits.
  ul_sched_context single_tx_ctxt   = ctxt;
  single_tx_ctxt.pusch_td_res_index = *ctxt.single_tx_pusch_td_res_index;
  single_tx_ctxt.nof_repetitions    = pusch_td_res.nof_repetitions;
  single_tx_ctxt.single_tx_pusch_td_res_index.reset();
  if (not size_ul_grant(
          single_tx_ctxt, ue_cc, ss, pusch_td_res, uci_nof_harq_bits, h_ul, is_csi_included(ue_cc, pusch_slot, {}))) {
    return false;
  }

  ctxt = single_tx_ctxt;
  return true;
}

bool sched_helper::downgrade_newtx_ul_grant_to_single_tx(ul_sched_context& ctxt,
                                                         const slice_ue&   u,
                                                         slot_point        pusch_slot,
                                                         unsigned          uci_nof_harq_bits)
{
  return downgrade_ul_grant_to_single_tx(ctxt, u, pusch_slot, uci_nof_harq_bits, nullptr);
}

bool sched_helper::downgrade_retx_ul_grant_to_single_tx(ul_sched_context&             ctxt,
                                                        const slice_ue&               u,
                                                        slot_point                    pusch_slot,
                                                        unsigned                      uci_nof_harq_bits,
                                                        const ul_harq_process_handle& h_ul)
{
  return downgrade_ul_grant_to_single_tx(ctxt, u, pusch_slot, uci_nof_harq_bits, &h_ul);
}

bool sched_helper::resize_newtx_ul_grant_for_uci(ul_sched_context&   ctxt,
                                                 const slice_ue&     u,
                                                 slot_point          pusch_slot,
                                                 unsigned            uci_nof_harq_bits,
                                                 span<const uint8_t> bundle_tx_offsets)
{
  return resize_ul_grant_for_uci(ctxt, u, pusch_slot, uci_nof_harq_bits, nullptr, bundle_tx_offsets);
}

bool sched_helper::resize_retx_ul_grant_for_uci(ul_sched_context&             ctxt,
                                                const slice_ue&               u,
                                                slot_point                    pusch_slot,
                                                unsigned                      uci_nof_harq_bits,
                                                const ul_harq_process_handle& h_ul,
                                                span<const uint8_t>           bundle_tx_offsets)
{
  return resize_ul_grant_for_uci(ctxt, u, pusch_slot, uci_nof_harq_bits, &h_ul, bundle_tx_offsets);
}

static vrb_interval
find_available_vrbs(const ul_sched_context& sched_ctxt, const vrb_bitmap& used_vrbs, unsigned max_rbs = MAX_NOF_PRBS)
{
  // Compute recommended number of layers, MCS and VRBs.
  unsigned nof_rbs = std::min(sched_ctxt.expected_nof_rbs, max_rbs);
  nof_rbs          = sched_ctxt.nof_rb_lims.clamp(nof_rbs);

  // Compute VRB allocation interval.
  vrb_interval vrbs = rb_helper::find_empty_interval_of_length(used_vrbs, nof_rbs, sched_ctxt.vrb_lims);
  if (vrbs.empty()) {
    return vrb_interval{};
  }

  if (sched_ctxt.pusch_cfg.use_transform_precoder) {
    // At this point we need to ensure a valid number of RBs is selected to be used with transform precoding.
    auto valid_nof_rbs = transform_precoding::get_nof_prbs_lower_bound(vrbs.length());
    if (not valid_nof_rbs.has_value()) {
      return vrb_interval{};
    }
    vrbs.resize(valid_nof_rbs.value());
  }

  // Successful VRB interval derivation.
  return vrbs;
}

vrb_interval sched_helper::compute_newtx_ul_vrbs(const ul_sched_context& decision_ctxt,
                                                 const vrb_bitmap&       used_vrbs,
                                                 unsigned                max_nof_rbs)
{
  return find_available_vrbs(decision_ctxt, used_vrbs, max_nof_rbs);
}

vrb_interval sched_helper::compute_retx_ul_vrbs(const ul_sched_context& decision_ctxt, const vrb_bitmap& used_vrbs)
{
  vrb_interval vrbs = find_available_vrbs(decision_ctxt, used_vrbs, decision_ctxt.expected_nof_rbs);
  if (vrbs.length() != decision_ctxt.expected_nof_rbs) {
    // In case of Retx, the #VRBs need to stay the same.
    return vrb_interval{};
  }
  return vrbs;
}
