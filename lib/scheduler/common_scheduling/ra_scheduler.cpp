// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ra_scheduler.h"
#include "../config/time_domain_mapper.h"
#include "../logging/cell_metrics_handler.h"
#include "../logging/scheduler_event_logger.h"
#include "../pdcch_scheduling/pdcch_resource_allocator_impl.h"
#include "../pucch_scheduling/pucch_allocator.h"
#include "../support/csi_rs_helpers.h"
#include "../support/dci_builder.h"
#include "../support/dmrs_helpers.h"
#include "../support/pdsch/pdsch_default_time_allocation.h"
#include "../support/pdsch/pdsch_resource_allocation.h"
#include "../support/sch_pdu_builder.h"
#include "../uci_scheduling/uci_allocator.h"
#include "../ue_context/ue_cell_repository.h"
#include "ocudu/adt/scope_exit.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/pdcch/dci_format.h"
#include "ocudu/ran/prach/prach_preamble_information.h"
#include "ocudu/ran/prach/prach_time_mapping.h"
#include "ocudu/ran/prach/ra_helper.h"
#include "ocudu/ran/resource_allocation/resource_allocation_frequency.h"
#include "ocudu/ran/sch/tbs_calculator.h"
#include "ocudu/scheduler/config/pucch_default_resource.h"
#include "ocudu/scheduler/config/pucch_guardbands.h"
#include "ocudu/scheduler/support/rb_helper.h"
#include "ocudu/support/compiler.h"
#include <limits>

using namespace ocudu;

/// Builds the association between SS/PBCH block indexes and PRACH occasions of the cell.
static prach_helper::ssb_to_ro_mapping make_ssb_to_ro_mapping(const cell_configuration& cell_cfg)
{
  const auto& ul_bwp = cell_cfg.params.ul_cfg_common.init_ul_bwp;
  return prach_helper::ssb_to_ro_mapping{prach_helper::prach_occasion_mapping_config{cell_cfg.params.dl_carrier.band,
                                                                                     ul_bwp.generic_params.scs,
                                                                                     ul_bwp.generic_params.cp,
                                                                                     *ul_bwp.rach_cfg_common,
                                                                                     cell_cfg.params.ssb_cfg,
                                                                                     cell_cfg.params.tdd_cfg}};
}

/// Convert CRBs to VRBs.
static vrb_interval ul_crb_to_vrb(const cell_configuration& cell_cfg, crb_interval grant_crbs)
{
  return rb_helper::crb_to_vrb_ul_non_interleaved(
      grant_crbs, cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.start());
}

static crb_interval msg3_vrb_to_crb(const cell_configuration& cell_cfg, vrb_interval grant_vrbs)
{
  return rb_helper::vrb_to_crb_ul_non_interleaved(
      grant_vrbs, cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs.start());
}

/// Maps a requested Backoff Indicator duration to its index in TS 38.321, Table 7.2-1.
static uint8_t backoff_ms_to_indicator(std::chrono::milliseconds backoff_duration)
{
  const span<const std::chrono::milliseconds> table = ra_helper::get_backoff_indicator_values();
  const auto                                  it    = std::find(table.begin(), table.end(), backoff_duration);
  ocudu_assert(it != table.end(), "Invalid Backoff Indicator duration={}ms", backoff_duration.count());
  return static_cast<uint8_t>(std::distance(table.begin(), it));
}

// (Implementation-defined) limit for maximum number of MsgA PRACH occasions pending to be allocated.
static constexpr size_t MAX_PENDING_MSGA_OCCASIONS = MAX_PRACH_OCCASIONS_PER_SLOT * 2;

/// \brief Compute the PRACH occasion duration in slots from cell configuration.
///
/// Returns the number of slots from the PRACH start slot to its end slot (inclusive), using the PUSCH SCS as
/// reference. Accounts for the starting symbol offset within the slot.
static unsigned compute_prach_occasion_duration_slots(const cell_configuration& cell_cfg)
{
  const prach_configuration prach_cfg = prach_configuration_get(
      band_helper::get_freq_range(cell_cfg.band()),
      band_helper::get_duplex_mode(cell_cfg.band()),
      cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.prach_config_index);
  return get_prach_duration_info(prach_cfg, cell_cfg.scs_common()).prach_length_slots;
}

/// Helper to fetch list of PDSCH time-domain resources for RA.
static span<const pdsch_time_domain_resource_allocation> get_ra_pdsch_td_list(const cell_configuration& cell_cfg)
{
  return get_ra_rnti_pdsch_time_domain_list(cell_cfg.params.dl_cfg_common.init_dl_bwp.pdsch_common,
                                            cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.cp,
                                            cell_cfg.params.dmrs_typeA_pos);
}

/// Helper to select ra-SearchSpace config.
/// [3GPP TS 38.213, clause 10.1] a UE monitors PDCCH candidates in one or more of the following search spaces sets
///  - a Type1-PDCCH CSS set configured by ra-SearchSpace in PDCCH-ConfigCommon for a DCI format with
///    CRC scrambled by a RA-RNTI, a MsgB-RNTI, or a TC-RNTI on the primary cell.
static const search_space_configuration& get_ra_ss_cfg(const cell_configuration& cell_cfg)
{
  return cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common
      .search_spaces[cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.ra_search_space_id];
}

/// Helper to fetch PUSCH time-domain resources for Msg3.
static span<const pusch_time_domain_resource_allocation> get_pusch_td_list(const cell_configuration& cell_cfg)
{
  return get_pusch_time_domain_resource_table(*cell_cfg.params.ul_cfg_common.init_ul_bwp.pusch_cfg_common);
}

/// \brief First OFDM symbol used by an SRS already allocated in the given UL slot.
/// \return The start symbol of the earliest SRS grant in \c ul_alloc, or the number of symbols per slot if no SRS is
/// allocated, meaning the whole slot is available for PUSCH.
static unsigned get_min_srs_symbol(const cell_slot_resource_allocator& ul_alloc)
{
  unsigned min_srs_symbol = NOF_OFDM_SYM_PER_SLOT_NORMAL_CP;
  for (const auto& srs : ul_alloc.result_reader().ul().srss()) {
    min_srs_symbol = std::min(static_cast<unsigned>(srs.symbols.start()), min_srs_symbol);
  }
  return min_srs_symbol;
}

class ra_scheduler::cached_bwp_info
{
public:
  cached_bwp_info(const cell_configuration& cfg, const prach_helper::preamble_slot_mapping& preamble_td_mapper_) :
    preamble_td_mapper(preamble_td_mapper_), cell_cfg(cfg)
  {
    fill_msga_pusch_info(cfg);
  }

  /// Whether a MsgA PUSCH can be scheduled in this slot.
  /// \note \c sl is on the gNB DL clock, where the MsgA PUSCH lands at prach_slot_rx + td_offset; subtract k_mac.
  bool is_msga_pusch_slot(slot_point sl) const
  {
    return preamble_td_mapper.has_prach_occasion(cell_cfg.dl_to_ue_ul_slot(sl) - msga_td_offset);
  }

  /// Pre-reserved space in the resource grid for MsgA PUSCH.
  /// \note This space needs to pre-reserved in advance so that other PUSCH/PUCCH grants occupy it.
  grant_info reserved_msga_pusch_space;

private:
  const prach_helper::preamble_slot_mapping& preamble_td_mapper;
  unsigned                                   msga_td_offset = 0;
  const cell_configuration&                  cell_cfg;

  void fill_msga_pusch_info(const cell_configuration& cfg)
  {
    const auto& ul_bwp = cfg.init_bwp.ul;
    if (not ul_bwp.rach_common()->two_step_rach_cfg.has_value()) {
      return;
    }
    const auto&                                  msga_pusch_cfg = ul_bwp.rach_common()->two_step_rach_cfg->pusch;
    const pusch_time_domain_resource_allocation& td_alloc = get_pusch_td_list(cfg)[msga_pusch_cfg.pusch_td_res_index];
    // For MsgA, the PUSCH slot is at prach_slot + td_offset; k2 of PUSCH TD resource is irrelevant as per spec.
    msga_td_offset = msga_pusch_cfg.td_offset;

    const unsigned crb_start = ul_bwp.cfg().crbs.start() + msga_pusch_cfg.prb_start;
    reserved_msga_pusch_space =
        grant_info{ul_bwp.cfg().scs,
                   td_alloc.symbols,
                   crb_interval{crb_start, crb_start + msga_pusch_cfg.po_fdm * msga_pusch_cfg.nof_prbs_per_msgA_po}};
  }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ra_scheduler::ra_scheduler(const cell_configuration& cellcfg_,
                           pdcch_resource_allocator& pdcch_sch_,
                           pucch_allocator&          pucch_alloc_,
                           uci_allocator&            uci_alloc_,
                           ra_ue_repository&         ra_ue_repo_,
                           ue_cell_repository&       ue_cell_db_,
                           scheduler_event_logger&   ev_logger_,
                           cell_metrics_handler&     metrics_hdlr_) :
  sched_cfg(cellcfg_.expert_cfg.ra),
  cell_cfg(cellcfg_),
  pdcch_sch(pdcch_sch_),
  pucch_alloc(pucch_alloc_),
  uci_alloc(uci_alloc_),
  ra_ue_repo(ra_ue_repo_),
  ue_cell_db(ue_cell_db_),
  ev_logger(ev_logger_),
  metrics_hdlr(metrics_hdlr_),
  ra_win_nof_slots(cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common->rach_cfg_generic.ra_resp_window),
  ra_crb_lims(pdsch_helper::get_ra_crb_limits_common(
      cell_cfg.params.dl_cfg_common.init_dl_bwp,
      cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.ra_search_space_id)),
  prach_occasion_duration_slots(compute_prach_occasion_duration_slots(cell_cfg)),
  backoff_indicator_value(backoff_ms_to_indicator(sched_cfg.backoff_indicator_duration)),
  pucch_crbs(compute_pucch_crbs(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.crbs,
                                cell_cfg.params.ul_cfg_common.init_ul_bwp.pucch_cfg_common->pucch_resource_common,
                                cell_cfg.bwp_res[to_bwp_id(0)].ul().pucch.dedicated)),
  ssb_ro_map(make_ssb_to_ro_mapping(cell_cfg)),
  cached_init_bwp_info(std::make_unique<cached_bwp_info>(cell_cfg, ssb_ro_map.occasions().td_slot_mapping()))
{
  pending_msgas.reserve(MAX_PENDING_MSGA_OCCASIONS);

  // The maximum number of pending RARs is given by the maximum number of PRACH occasions that can accumulate from a
  // given UL slot (at which the PRACH is received) until the expiration of the RAR window. The worst case is when:
  // (i) the PRACH is received instantaneously by the scheduler, and the PRACH slot is the farthest possible from the
  //     beginning of the start of the RAR window.
  // (ii) RAR window is min(80 slots, 10 ms).
  // (iii) there are PRACHs occasions in every UL slot.
  // (iv) there are no suitable DL slots for scheduling the RARs (pending RARs will be in the vector until the RAR
  //      window expires).
  // [Implementation-defined] Assume 80 slots RAR window + TDD 2D1S7D with 30kHz SCS slots and
  // MAX_PRACH_OCCASIONS_PER_SLOT (the actual number would depend on the PRACH configuration index).
  static constexpr size_t MAX_PENDING_RARS_SLOTS = 90U;
  pending_rars.reserve(MAX_PRACH_OCCASIONS_PER_SLOT * MAX_PENDING_RARS_SLOTS);
  // MsgB window can be up to 320 slots (msgB-ResponseWindow-r16), so use the same bound.
  pending_msgbs.reserve(MAX_PRACH_OCCASIONS_PER_SLOT * MAX_PENDING_RARS_SLOTS);

  // Precompute RAR PDSCH and DCI PDUs.
  precompute_rar_fields();

  // Precompute Msg3 PUSCH and DCI PDUs.
  precompute_msg3_pdus();
}

ra_scheduler::~ra_scheduler() = default;

void ra_scheduler::precompute_rar_fields()
{
  // RAR payload size in bytes as per TS 38.321, 6.1.5 and 6.2.3.
  static constexpr unsigned rar_payload_size_bytes   = 7;
  static constexpr unsigned rar_subheader_size_bytes = 1;
  // Backoff Indicator subPDU has a subheader only, no payload, as per TS 38.321, 6.1.5 and 6.2.2.
  static constexpr unsigned bi_subheader_size_bytes = 1;
  // As per TS 38.214, Section 5.1.3.2, nof_oh_prb = 0 if PDSCH is scheduled by PDCCH with a CRC scrambled by RA-RNTI.
  static constexpr unsigned nof_oh_prb = 0;
  static constexpr unsigned nof_layers = 1;

  const span<const pdsch_time_domain_resource_allocation> pdsch_td_list = get_ra_pdsch_td_list(cell_cfg);

  // Cache PDSCH DM-RS information and RAR required TBS and number of PRBs.
  rar_data.resize(pdsch_td_list.size());
  rar_mcs_config = pdsch_mcs_get_config(pdsch_mcs_table::qam64, sched_cfg.rar_mcs_index);
  for (unsigned td_idx = 0; td_idx != rar_data.size(); ++td_idx) {
    // > Compute and cache DM-RS information for different PDSCH TD indexes.
    rar_data[td_idx].dmrs_info =
        make_dmrs_info_common(pdsch_td_list, td_idx, cell_cfg.params.pci, cell_cfg.params.dmrs_typeA_pos);

    // > Compute and cache #PRBs and TBS information for different PDSCH TD indexes.
    const unsigned             nof_symb_sh = pdsch_td_list[td_idx].symbols.length();
    prbs_calculator_sch_config prbs_cfg{rar_payload_size_bytes + rar_subheader_size_bytes,
                                        nof_symb_sh,
                                        calculate_nof_dmrs_per_rb(rar_data[td_idx].dmrs_info),
                                        nof_oh_prb,
                                        rar_mcs_config,
                                        nof_layers};
    for (unsigned ngrant = 0; ngrant != MAX_RAR_PDUS_PER_SLOT; ++ngrant) {
      prbs_cfg.payload_size_bytes = (rar_payload_size_bytes + rar_subheader_size_bytes) * (ngrant + 1);
      rar_data[td_idx].prbs_tbs_per_nof_grants.push_back(get_nof_prbs(prbs_cfg));
    }

    // > Compute and cache #PRBs and TBS information for RARs that also carry a Backoff Indicator subPDU. Index=0
    // corresponds to 0 grants (Backoff Indicator only).
    for (unsigned ngrant = 0; ngrant != MAX_RAR_PDUS_PER_SLOT + 1; ++ngrant) {
      prbs_cfg.payload_size_bytes =
          bi_subheader_size_bytes + (rar_payload_size_bytes + rar_subheader_size_bytes) * ngrant;
      rar_data[td_idx].prbs_tbs_per_nof_grants_with_bi.push_back(get_nof_prbs(prbs_cfg));
    }
  }
}

void ra_scheduler::precompute_msg3_pdus()
{
  // Msg3 UL CCCH message size is up to 64bits (8 octets), and its subheader has 1 octet as per TS 38.321.
  static constexpr unsigned max_msg3_sdu_payload_size_bytes = 8;
  static constexpr unsigned msg3_subheader_size_bytes       = 1;
  // As per TS 38.214, Section 5.1.3.2, nof_oh_prb = 0 if PDSCH is scheduled by PDCCH with a CRC scrambled by RA-RNTI.
  static constexpr unsigned nof_oh_prb = 0;
  static constexpr unsigned nof_layers = 1;

  msg3_mcs_config = pusch_mcs_get_config(
      pusch_mcs_table::qam64, sched_cfg.msg3_mcs_index, cell_cfg.use_msg3_transform_precoder(), false);

  const auto pusch_td_alloc_list = get_pusch_td_list(cell_cfg);

  // One position per time resource.
  msg3_data.resize(pusch_td_alloc_list.size());

  for (unsigned i = 0; i != msg3_data.size(); ++i) {
    // Compute the required PRBs and TBS for Msg3.
    const pusch_config_params pusch_cfg = get_pusch_config_f0_0_tc_rnti(cell_cfg, pusch_td_alloc_list[i]);
    const sch_prbs_tbs        prbs_tbs =
        get_nof_prbs(prbs_calculator_sch_config{max_msg3_sdu_payload_size_bytes + msg3_subheader_size_bytes,
                                                pusch_td_alloc_list[i].symbols.length(),
                                                calculate_nof_dmrs_per_rb(pusch_cfg.dmrs),
                                                nof_oh_prb,
                                                msg3_mcs_config,
                                                nof_layers});

    // Generate DCI and PUSCH PDUs.
    static constexpr uint8_t msg3_rv = 0;
    build_dci_f0_0_tc_rnti(msg3_data[i].dci,
                           cell_cfg.params.dl_cfg_common.init_dl_bwp,
                           cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params,
                           crb_interval{0, prbs_tbs.nof_prbs},
                           i,
                           sched_cfg.msg3_mcs_index,
                           msg3_rv);

    // Note: RNTI will be overwritten later.
    const vrb_interval vrbs = ul_crb_to_vrb(cell_cfg, crb_interval{0, prbs_tbs.nof_prbs});
    build_pusch_f0_0_tc_rnti(msg3_data[i].pusch,
                             pusch_cfg,
                             prbs_tbs.tbs_bytes,
                             to_rnti(0x4601),
                             cell_cfg,
                             msg3_data[i].dci.as_tc_rnti_f0_0(),
                             vrbs,
                             true);
  }
}

void ra_scheduler::handle_rach_indication(const rach_indication_message& msg)
{
  const rach_config_common& rach_cfg = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;

  for (const auto& prach_occ : msg.occasions) {
    if (prach_occ.preambles.empty()) {
      // As per FAPI, this should not occur.
      logger.warning(
          "pci={}: Discarding PRACH occasion detected at slot={}. Cause: There are no preambles detected for this "
          "PRACH occasion",
          cell_cfg.params.pci,
          msg.slot_rx);
      continue;
    }

    span<const rach_indication_message::preamble> msg1_preambles;
    span<const rach_indication_message::preamble> msga_preambles;

    if (rach_cfg.two_step_rach_cfg.has_value()) {
      // Split detected preambles into Msg1 (4-step) and MsgA (2-step) spans. Preambles are assumed to be ordered by
      // preamble_id, so the two groups form contiguous ranges.
      const auto msga_begin = std::partition_point(
          prach_occ.preambles.begin(), prach_occ.preambles.end(), [&](const rach_indication_message::preamble& p) {
            return not ra_helper::is_msga_cb_preamble(rach_cfg, p.preamble_id);
          });
      msg1_preambles = {prach_occ.preambles.begin(), msga_begin};
      msga_preambles = {msga_begin, prach_occ.preambles.end()};
    } else {
      // All preambles are 4-step RACH preambles.
      msg1_preambles = prach_occ.preambles;
    }

    if (not msg1_preambles.empty()) {
      handle_msg1_occasion(prach_occ, msg1_preambles, msg.slot_rx);
    }
    if (not msga_preambles.empty()) {
      handle_msga_occasion(prach_occ, msga_preambles, msg.slot_rx);
    }
  }

  // Forward RACH indication to metrics handler.
  metrics_hdlr.handle_rach_indication(msg);
}

std::optional<ssb_id_t>
ra_scheduler::get_preamble_ssb_index(slot_point prach_slot_rx, unsigned fd_occasion_idx, unsigned preamble_id) const
{
  // The lower layers do not report the time-domain occasion index, so only the first occasion of the PRACH slot is
  // resolved. The configuration validator rejects time multiplexed occasions when more than one SSB beam is active.
  // PRACH occasions are located by the tables of TS 38.211 Section 6.3.3.2, keyed by frame, subframe and slot number
  // in the uplink frame the UE transmits in (TS 38.211 Section 4.3.1); the SS/PBCH-to-RO association period built on
  // them is anchored at frame 0 (TS 38.213 Section 8.1). prach_slot_rx is the slot the preamble was captured in, which
  // this scheduler runs k_mac ahead of, so undo k_mac before indexing the association table.
  return ssb_ro_map.get_ssb_index(cell_cfg.dl_to_ue_ul_slot(prach_slot_rx), 0, fd_occasion_idx, preamble_id);
}

void ra_scheduler::handle_msg1_occasion(const rach_indication_message::occasion&      occ,
                                        span<const rach_indication_message::preamble> preambles,
                                        slot_point                                    prach_slot_rx)
{
  const rnti_t ra_rnti = ra_helper::get_ra_rnti(occ.slot_index, occ.start_symbol, occ.frequency_index);

  // The RAR is a single PDSCH, so it can only be carried by the beam of one SS/PBCH block. The configuration validator
  // rejects the RACH configurations that would multiplex several of them into one occasion, so any preamble resolves
  // the SS/PBCH block of the whole occasion. Resolved before the RAR is registered, so that a pending RAR always
  // carries the beam it has to be transmitted on, even when every preamble is later dropped.
  const std::optional<ssb_id_t> ssb_index =
      get_preamble_ssb_index(prach_slot_rx, occ.frequency_index, preambles.front().preamble_id);
  if (not ssb_index.has_value()) {
    logger.info("pci={} ra-rnti={}: Discarding PRACH occasion. Cause: It is associated with no SS/PBCH block",
                cell_cfg.params.pci,
                ra_rnti);
    return;
  }

  // Search for pending RAR with matching RA-RNTI and Rx Slot.
  auto rar_it = std::find_if(pending_rars.begin(), pending_rars.end(), [&](const pending_rar_alloc& rar) {
    return rar.ra_rnti == ra_rnti and rar.prach_slot_rx == prach_slot_rx;
  });
  if (rar_it == pending_rars.end()) {
    // No match was found. Create new pending RAR.
    if (pending_rars.capacity() == pending_rars.size()) {
      logger.warning("pci={} ra-rnti={}: Discarding PRACH occasion. Cause: Pending RARs queue is full",
                     cell_cfg.params.pci,
                     ra_rnti);
      return;
    }
    pending_rars.emplace_back(ra_rnti, prach_slot_rx, *ssb_index);
    rar_it = std::prev(pending_rars.end());
  }
  pending_rar_alloc* rar_req = &*rar_it;
  ocudu_sanity_check(rar_req->ssb_index == *ssb_index, "PRACH occasion maps to several SS/PBCH blocks");

  // Set RAR window. First slot after PRACH with active DL slot represents the start of the RAR window.
  if (cell_cfg.is_tdd()) {
    // TDD case.
    const unsigned period = nof_slots_per_tdd_period(*cell_cfg.params.tdd_cfg);
    for (unsigned sl_idx = 0; sl_idx != period; ++sl_idx) {
      const slot_point sl_start = rar_req->prach_slot_rx + prach_occasion_duration_slots + sl_idx;
      if (cell_cfg.is_dl_enabled(sl_start)) {
        rar_req->rar_window = {sl_start, sl_start + ra_win_nof_slots};
        break;
      }
    }
    ocudu_sanity_check(rar_req->rar_window.length() != 0, "Invalid configuration");
  } else {
    // FDD case.
    rar_req->rar_window = {rar_req->prach_slot_rx + prach_occasion_duration_slots,
                           rar_req->prach_slot_rx + prach_occasion_duration_slots + ra_win_nof_slots};
  }

  // Determine which preambles are excluded due to congestion control (SNR below threshold, or occasion preamble
  // count above threshold), and whether the RAR must carry a Backoff Indicator subheader as a result.
  static_vector<bool, MAX_PREAMBLES_PER_PRACH_OCCASION> keep_preamble(preambles.size(), true);
  bool                                                  congestion_detected = false;

  if (sched_cfg.backoff_indicator_snr_threshold_dB.has_value()) {
    for (unsigned i = 0; i != preambles.size(); ++i) {
      if (preambles[i].snr_dB.has_value() and *preambles[i].snr_dB < *sched_cfg.backoff_indicator_snr_threshold_dB) {
        keep_preamble[i]    = false;
        congestion_detected = true;
      }
    }
  }

  static_vector<unsigned, MAX_PREAMBLES_PER_PRACH_OCCASION> kept_indices;
  for (unsigned i = 0; i != preambles.size(); ++i) {
    if (keep_preamble[i]) {
      kept_indices.push_back(i);
    }
  }
  const unsigned max_preambles = sched_cfg.backoff_indicator_max_preambles;
  if (kept_indices.size() > max_preambles) {
    congestion_detected = true;
    // Keep the preambles with the strongest SNR. Preambles with unknown SNR are treated as lowest priority.
    std::sort(kept_indices.begin(), kept_indices.end(), [&preambles](unsigned lhs, unsigned rhs) {
      const float lhs_snr = preambles[lhs].snr_dB.value_or(-std::numeric_limits<float>::infinity());
      const float rhs_snr = preambles[rhs].snr_dB.value_or(-std::numeric_limits<float>::infinity());
      return lhs_snr > rhs_snr;
    });
    for (unsigned k = max_preambles; k != kept_indices.size(); ++k) {
      keep_preamble[kept_indices[k]] = false;
    }
  }

  if (congestion_detected) {
    rar_req->send_backoff_indicator = true;
  }

  for (unsigned idx = 0; idx != preambles.size(); ++idx) {
    const auto& preamble = preambles[idx];

    // Log event.
    ev_logger.enqueue(scheduler_event_logger::prach_event{
        prach_slot_rx,
        cell_cfg.cell_index,
        preamble.preamble_id,
        ra_rnti,
        preamble.tc_rnti,
        *ssb_index,
        preamble.time_advance.to_Ta(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs),
        false});

    if (not keep_preamble[idx]) {
      // Preamble excluded due to congestion control. The UE will get no RAPID match in the RAR and will retry.
      logger.debug("pci={} ra-rnti={}: Discarding PRACH preamble. Cause: Congestion control excluded the preamble",
                   cell_cfg.params.pci,
                   ra_rnti);
      continue;
    }

    // Note: Checked before the RA UE context is created, so that no context is left without a RAR to be listed in.
    if (rar_req->tc_rntis.full()) {
      logger.warning("pci={} ra-rnti={}: Discarding PRACH preamble. Cause: The RAR already carries the maximum number "
                     "of TC-RNTIs",
                     cell_cfg.params.pci,
                     ra_rnti);
      break;
    }

    // Create a new UE RA context.
    ra_ue_context* msg3_entry = ra_ue_repo.add(preamble, prach_slot_rx, *ssb_index);
    if (msg3_entry == nullptr and
        ra_helper::is_msg1_cf_preamble(*cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common,
                                       preamble.preamble_id)) {
      // A CFRA UE that misses its RAR retransmits the dedicated preamble (TS 38.321, 5.1.4), which carries the same
      // C-RNTI as the stale attempt, so the attempt is restarted to send a new RAR.
      msg3_entry = ra_ue_repo.restart(preamble, prach_slot_rx, *ssb_index);
      if (msg3_entry != nullptr) {
        logger.info("pci={} rnti={}: Restarting the contention-free RA. Cause: Dedicated preamble retransmitted",
                    cell_cfg.params.pci,
                    preamble.tc_rnti);
      }
    }
    if (msg3_entry == nullptr) {
      logger.warning("pci={}: PRACH ignored, as the allocated TC-RNTI={} is already under use",
                     cell_cfg.params.pci,
                     preamble.tc_rnti);
      continue;
    }

    // Store TC-RNTI of the preamble.
    rar_req->tc_rntis.emplace_back(preamble.tc_rnti);
  }

  // No preamble was registered and no Backoff Indicator has to be sent, so the RAR would carry nothing.
  if (rar_req->tc_rntis.empty() and not rar_req->send_backoff_indicator) {
    pending_rars.erase(rar_it);
  }
}

void ra_scheduler::handle_msga_occasion(const rach_indication_message::occasion&      occ,
                                        span<const rach_indication_message::preamble> preambles,
                                        slot_point                                    prach_slot_rx)
{
  const rach_config_common& rach_cfg = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  ocudu_sanity_check(rach_cfg.two_step_rach_cfg.has_value(), "MsgA received but 2-step RACH is not configured");
  const rach_config_common_two_step&                    two_step_cfg   = *rach_cfg.two_step_rach_cfg;
  const rach_config_common_two_step::msgA_pusch_config& msga_pusch_cfg = two_step_cfg.pusch;

  // Derive MsgB-RNTI.
  const rnti_t msgb_rnti = ra_helper::get_msgb_rnti(occ.slot_index, occ.start_symbol, occ.frequency_index);
  const rnti_t ra_rnti   = ra_helper::get_ra_rnti(occ.slot_index, occ.start_symbol, occ.frequency_index);

  // Determine the MsgA PUSCH slot and whether its OFDM symbols fall in valid UL symbols.
  // Note: k2 from the TD allocation is ignored; the actual PUSCH slot offset is given by td_offset as per
  // TS 38.331 "msgA-PUSCH-TimeDomainAllocation".
  const slot_point                             pusch_slot = prach_slot_rx + msga_pusch_cfg.td_offset;
  const pusch_time_domain_resource_allocation& td_alloc =
      get_pusch_td_list(cell_cfg)[msga_pusch_cfg.pusch_td_res_index];
  const unsigned start_ul_symbols = NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - cell_cfg.get_nof_ul_symbol_per_slot(pusch_slot);
  if (not cell_cfg.is_ul_enabled(pusch_slot) or td_alloc.symbols.start() < start_ul_symbols) {
    logger.warning("pci={} msgb-rnti={}: Discarding MsgA PUSCH. Cause: PUSCH would fall in an invalid slot={}",
                   cell_cfg.params.pci,
                   msgb_rnti,
                   pusch_slot);
    return;
  }

  // One pending entry per MsgB suffices, as the preambles of every occasion sharing it are registered in it. Checked
  // before any preamble is registered: a registration that no pending entry covers would leave the MsgB responding
  // to a preamble whose MsgA PUSCH was never allocated.
  const bool already_pending =
      std::any_of(pending_msgas.begin(), pending_msgas.end(), [&](const pending_msga_occasion& pending) {
        return pending.msgb_rnti == msgb_rnti and pending.prach_slot_rx == prach_slot_rx;
      });
  if (not already_pending and pending_msgas.size() == pending_msgas.capacity()) {
    logger.warning("pci={} msgb-rnti={}: Discarding MsgA occasion. Cause: Too many occasions pending to be scheduled",
                   cell_cfg.params.pci,
                   msgb_rnti);
    return;
  }

  // Search for a pending MsgB entry matching in MsgB-RNTI and PRACH slot.
  auto msgb_it = std::find_if(pending_msgbs.begin(), pending_msgbs.end(), [&](const pending_msgb_alloc& msgb) {
    return msgb.msgb_rnti == msgb_rnti and msgb.prach_slot_rx == prach_slot_rx;
  });
  if (msgb_it == pending_msgbs.end()) {
    // No MsgB with matching MsgB-RNTI and PRACH slot exists. Create one.
    if (pending_msgbs.capacity() == pending_msgbs.size()) {
      logger.warning("pci={} msgb-rnti={}: Discarding MsgA occasion. Cause: Pending MsgBs queue is full",
                     cell_cfg.params.pci,
                     msgb_rnti);
      return;
    }
    pending_msgb_alloc& new_msgb = pending_msgbs.emplace_back();
    msgb_it                      = pending_msgbs.end() - 1;
    new_msgb.msgb_rnti           = msgb_rnti;
    new_msgb.ra_rnti             = ra_rnti;
    new_msgb.prach_slot_rx       = prach_slot_rx;
    new_msgb.frequency_index     = occ.frequency_index;

    // Set MsgB response window. First slot after PRACH with active DL slot is the window start.
    if (cell_cfg.is_tdd()) {
      const unsigned period = nof_slots_per_tdd_period(*cell_cfg.params.tdd_cfg);
      for (unsigned sl_idx = 0; sl_idx < period; ++sl_idx) {
        const slot_point sl_start = prach_slot_rx + prach_occasion_duration_slots + sl_idx;
        if (cell_cfg.is_dl_enabled(sl_start)) {
          new_msgb.msgb_window = {sl_start, sl_start + two_step_cfg.msgB_response_window_slots};
          break;
        }
      }
      ocudu_sanity_check(new_msgb.msgb_window.length() != 0, "Invalid TDD configuration for MsgB window");
    } else {
      new_msgb.msgb_window = {prach_slot_rx + prach_occasion_duration_slots,
                              prach_slot_rx + prach_occasion_duration_slots + two_step_cfg.msgB_response_window_slots};
    }
  }

  // Register the detected preambles in the MsgB.
  pending_msgb_alloc& msgb_req = *msgb_it;
  for (const auto& preamble : preambles) {
    ocudu_sanity_check(ra_helper::is_msga_cb_preamble(rach_cfg, preamble.preamble_id),
                       "Handling preamble that is not for MsgA. Are preamble IDs sorted in the RACH indication?");
    const std::optional<ssb_id_t> ssb_index =
        get_preamble_ssb_index(prach_slot_rx, occ.frequency_index, preamble.preamble_id);
    if (not ssb_index.has_value()) {
      logger.info("pci={} msgb-rnti={}: Discarding MsgA preamble. Cause: Its PRACH occasion is associated with no "
                  "SS/PBCH block",
                  cell_cfg.params.pci,
                  msgb_rnti);
      continue;
    }

    ev_logger.enqueue(scheduler_event_logger::prach_event{
        prach_slot_rx,
        cell_cfg.cell_index,
        preamble.preamble_id,
        msgb_rnti,
        preamble.tc_rnti,
        *ssb_index,
        preamble.time_advance.to_Ta(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs),
        true});

    if (msgb_req.preambles.full()) {
      logger.warning("pci={} msgb-rnti={}: Discarding MsgA preamble id={}. Cause: MsgB preamble list is full",
                     cell_cfg.params.pci,
                     msgb_rnti,
                     preamble.preamble_id);
      continue;
    }
    msgb_req.preambles.emplace_back(preamble);
  }

  // The MsgA PUSCHs can only be allocated once their resources have been reserved in the grid, which happens in
  // run_slot.
  if (not already_pending) {
    pending_msgas.push_back(pending_msga_occasion{msgb_rnti, prach_slot_rx});
  }
}

void ra_scheduler::schedule_pending_msgas(cell_resource_allocator& res_alloc)
{
  for (const pending_msga_occasion& pending : pending_msgas) {
    schedule_msga_puschs(pending.msgb_rnti, pending.prach_slot_rx, res_alloc);
  }
  pending_msgas.clear();
}

void ra_scheduler::schedule_msga_puschs(rnti_t msgb_rnti, slot_point prach_slot_rx, cell_resource_allocator& res_alloc)
{
  const rach_config_common&                             rach_cfg       = *cell_cfg.init_bwp.ul.rach_common();
  const rach_config_common_two_step&                    two_step_cfg   = *rach_cfg.two_step_rach_cfg;
  const bwp_uplink_common&                              ul_bwp         = cell_cfg.params.ul_cfg_common.init_ul_bwp;
  const rach_config_common_two_step::msgA_pusch_config& msga_pusch_cfg = two_step_cfg.pusch;

  // The MsgB entry, and the preambles registered in it, were set up when the occasion was handled.
  auto msgb_it = std::find_if(pending_msgbs.begin(), pending_msgbs.end(), [&](const pending_msgb_alloc& msgb) {
    return msgb.msgb_rnti == msgb_rnti and msgb.prach_slot_rx == prach_slot_rx;
  });
  if (msgb_it == pending_msgbs.end()) {
    logger.warning("pci={} msgb-rnti={}: Discarding MsgA PUSCHs. Cause: The pending MsgB no longer exists",
                   cell_cfg.params.pci,
                   msgb_rnti);
    return;
  }
  pending_msgb_alloc& msgb_req = *msgb_it;
  const rnti_t        ra_rnti  = msgb_req.ra_rnti;

  // If by the end of this function, no MsgA PUSCHs were allocated, we remove the respective MsgB entry.
  auto erase_msgb_if_empty = make_scope_exit([this, &msgb_it]() {
    if (msgb_it->preambles.empty()) {
      // No MsgA PUSCHs were successfully allocated. Drop the MsgB altogether.
      pending_msgbs.erase(msgb_it);
    }
  });

  const slot_point pusch_slot = prach_slot_rx + msga_pusch_cfg.td_offset;
  if (pusch_slot < res_alloc.slot_tx()) {
    logger.warning("pci={} msgb-rnti={}: Discarding MsgA PUSCH. Cause: The PUSCH slot={} has already passed",
                   cell_cfg.params.pci,
                   msgb_rnti,
                   pusch_slot);
    // Drop only the preambles that never got a PUSCH. The ones already allocated one keep the MsgB alive.
    msgb_req.preambles.erase(std::remove_if(msgb_req.preambles.begin(),
                                            msgb_req.preambles.end(),
                                            [](const auto& p) { return not p.msga_pusch_scheduled; }),
                             msgb_req.preambles.end());
    return;
  }
  cell_slot_resource_allocator& pusch_alloc = res_alloc[pusch_slot];

  // Look up the PUSCH time-domain allocation to derive symbol range and mapping type for DMRS computation.
  const pusch_time_domain_resource_allocation& td_alloc =
      get_pusch_td_list(cell_cfg)[msga_pusch_cfg.pusch_td_res_index];

  // Compute DMRS and TBS — shared by all preambles since nof_prbs_per_msgA_po is constant across FDM occasions.
  const dmrs_information    dmrs = make_dmrs_info_common(td_alloc, cell_cfg.params.pci, cell_cfg.params.dmrs_typeA_pos);
  const sch_mcs_description mcs_cfg = pusch_mcs_get_config(pusch_mcs_table::qam64, msga_pusch_cfg.mcs, false, false);
  const units::bytes        tbs     = tbs_calculator_calculate(tbs_calculator_configuration{
                 .nof_symb_sh      = td_alloc.symbols.length(),
                 .nof_dmrs_prb     = calculate_nof_dmrs_per_rb(dmrs),
                 .nof_oh_prb       = 0,
                 .mcs_descr        = mcs_cfg,
                 .nof_layers       = 1,
                 .tb_scaling_field = 0,
                 .n_prb            = msga_pusch_cfg.nof_prbs_per_msgA_po,
  });

  // Precompute the preamble-to-PUSCH-occasion index mapping. When po_fdm > 1, the CB preambles are divided evenly
  // across po_fdm FDM occasions. Preamble i (within the MsgA range) maps to occasion floor(i / preambles_per_po).
  const unsigned preambles_per_ssb = ra_helper::get_preambles_per_ssb(rach_cfg);
  const unsigned preambles_per_po  = two_step_cfg.cb_preambles_per_ssb_per_shared_ro / msga_pusch_cfg.po_fdm;

  for (auto it = msgb_req.preambles.begin(); it != msgb_req.preambles.end();) {
    if (it->msga_pusch_scheduled) {
      // The MsgA PUSCH of this preamble was already allocated in a previous slot indication.
      ++it;
      continue;
    }
    const rach_indication_message::preamble& preamble = it->info;

    // Determine this preamble's PUSCH occasion index and its CRB allocation.
    const unsigned msga_local_id = (preamble.preamble_id % preambles_per_ssb) - rach_cfg.nof_cb_preambles_per_ssb;
    const unsigned po_idx        = msga_local_id / preambles_per_po;
    const unsigned crb_start =
        ul_bwp.generic_params.crbs.start() + msga_pusch_cfg.prb_start + po_idx * msga_pusch_cfg.nof_prbs_per_msgA_po;
    const crb_interval preamble_crbs{crb_start, crb_start + msga_pusch_cfg.nof_prbs_per_msgA_po};

    // Note: PUSCH resources have been pre-reserved.
    ocudu_sanity_check(
        pusch_alloc.ul_res_grid.all_set(grant_info{ul_bwp.generic_params.scs, td_alloc.symbols, preamble_crbs}),
        "Invalid prereservation of MsgA PUSCH resources");

    ul_sched_info* ul_pdu = pusch_alloc.result_builder().ul().puschs().emplace_back();
    if (ul_pdu == nullptr) {
      logger.warning(
          "pci={} msgb-rnti={} tc-rnti={}: Discarding MsgA PUSCH. Cause: PUSCH grant list is full for slot={}",
          cell_cfg.params.pci,
          msgb_rnti,
          preamble.tc_rnti,
          pusch_slot);
      it = msgb_req.preambles.erase(it);
      continue;
    }

    // Fill scheduling result.
    ul_sched_info& ul_info     = *ul_pdu;
    ul_info.context.ue_index   = INVALID_DU_UE_INDEX;
    ul_info.context.ss_id      = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.ra_search_space_id;
    ul_info.context.k2         = msga_pusch_cfg.td_offset;
    ul_info.context.nof_retxs  = 0;
    ul_info.context.nof_oh_prb = 0;
    ul_info.context.rapid      = preamble.preamble_id;

    pusch_information& pusch      = ul_info.pusch_cfg;
    pusch.rnti                    = ra_rnti;
    pusch.bwp_cfg                 = &ul_bwp.generic_params;
    pusch.rbs                     = ul_crb_to_vrb(cell_cfg, preamble_crbs);
    pusch.symbols                 = td_alloc.symbols;
    pusch.mcs_table               = pusch_mcs_table::qam64;
    pusch.mcs_index               = msga_pusch_cfg.mcs;
    pusch.mcs_descr               = mcs_cfg;
    pusch.nof_layers              = 1;
    pusch.dmrs                    = dmrs;
    pusch.n_id                    = cell_cfg.params.pci;
    pusch.pusch_dmrs_id           = cell_cfg.params.pci;
    pusch.rv_index                = 0;
    pusch.new_data                = true;
    pusch.harq_id                 = to_harq_id(0);
    pusch.tb_size_bytes           = tbs;
    pusch.nof_cb                  = 0;
    pusch.transform_precoding     = cell_cfg.use_msg3_transform_precoder();
    pusch.intra_slot_freq_hopping = false;
    pusch.tx_direct_current_location =
        dc_offset_helper::pack(cell_cfg.expert_cfg.ue.initial_ul_dc_offset, cell_cfg.nof_ul_prbs);
    pusch.ul_freq_shift_7p5khz = false;
    pusch.dmrs_hopping_mode    = pusch_information::dmrs_hopping_mode::no_hopping;

    it->msga_pusch_scheduled = true;
    ++it;
  }
}

void ra_scheduler::handle_crc_indication(const ul_crc_indication& crc_ind)
{
  // Filter out the CRCs that do not belong to the RA procedure, so that only the relevant ones are buffered.
  // Note: Only HARQ-ID=0 is relevant for the RA procedure, as per TS 38.321, 5.4.2.1.
  // Note: A UE on CBRA has no ue_index yet, so a CRC that carries one only belongs to the RA procedure while the UE is
  // still undergoing a CFRA. Otherwise, a UE that already completed RA could free the Msg3 HARQ that its
  // TC-RNTI-turned-C-RNTI still matches in ra_ue_repo.
  auto is_ra_crc = [this](const ul_crc_pdu_indication& pdu) {
    return pdu.harq_id == to_harq_id(0) and (pdu.ue_index == INVALID_DU_UE_INDEX or find_cfra_ue(pdu.rnti) != nullptr);
  };
  for (const ul_crc_pdu_indication& crc : crc_ind.crcs) {
    if (is_ra_crc(crc)) {
      handle_ra_crc(crc, crc_ind.sl_rx);
    }
  }
}

void ra_scheduler::handle_ra_crc(const ul_crc_pdu_indication& crc, slot_point sl_rx)
{
  auto crc_it = ra_ue_repo.find(crc.rnti);
  if (crc_it == ra_ue_repo.end()) {
    // RNTI of the CRC is not associated with any existing TC-RNTI.
    if (not crc.rapid.has_value()) {
      // It is a UE which has finished RA. Ignore it.
      return;
    }

    // It is a 2-step RA UE. Handle its MsgA PUSCH CRC.
    handle_msga_crc(crc.rnti, *crc.rapid, crc.tb_crc_success);
    return;
  }
  // It is a 4-step RA.
  auto& pending_msg3 = *crc_it;

  // See TS 38.321, 5.4.2.1 - "For UL transmission with UL grant in RA Response, HARQ process identifier 0 is used."
  harq_id_t                             h_id = to_harq_id(0);
  std::optional<ul_harq_process_handle> h_ul = pending_msg3.harq_ent.ul_harq(h_id, sl_rx);
  if (not h_ul.has_value() or crc.harq_id != h_id) {
    logger.warning("pci={} tc-rnti={}: Invalid UL CRC for HARQ h_id={}. Cause: HARQ-Id 0 must be used in Msg3",
                   cell_cfg.params.pci,
                   crc.rnti,
                   crc.harq_id);
    return;
  }

  // Handle CRC info.
  // Note: The ra_ue_repository entry is intentionally left in place once its Msg3 HARQ empties (slot_indication's
  // ConRes-timer sweep reclaims the ring slot in due course): the async UE-creation event still needs to read
  // prach_slot_rx from it.
  const unsigned nof_retxs = h_ul->nof_retxs();
  h_ul->ul_crc_info(crc.tb_crc_success);

  // Forward MSG3 CRC indication to metrics handler.
  metrics_hdlr.handle_msg3_crc_indication(crc);

  // A CBRA Msg3 KO with very low SINR suggests that no UE is listening (e.g. a preamble collision or false detection),
  // so its reTxs are dropped and the TC-RNTI released. CFRA UEs are exempt, as dedicated preambles cannot collide.
  const bool is_cbra = crc.ue_index == INVALID_DU_UE_INDEX;
  if (is_cbra and not crc.tb_crc_success and crc.ul_sinr_dB.has_value() and
      *crc.ul_sinr_dB < sched_cfg.msg3_dtx_sinr_threshold_dB) {
    logger.debug("pci={} tc-rnti={}: Dropping Msg3 reTxs after nof_retxs={}. Cause: CRC KO with SINR={:.1f}dB below "
                 "the DTX threshold",
                 cell_cfg.params.pci,
                 crc.rnti,
                 nof_retxs,
                 *crc.ul_sinr_dB);
    metrics_hdlr.handle_msg3_dtx();
    ra_ue_repo.erase(crc_it);
  }
}

const ue_cell* ra_scheduler::find_cfra_ue(rnti_t crnti) const
{
  // For CFRA the RA scheduler is given the UE's real C-RNTI, so the UE cell lookup is keyed by it.
  const ue_cell* ue_cc = ue_cell_db.find_by_rnti(crnti);
  if (ue_cc == nullptr or not ue_cc->is_pcell() or
      ue_cc->get_pcell_state().conres_st != ue_conres_state::pending_cfra) {
    return nullptr;
  }
  return ue_cc;
}

bool ra_scheduler::can_allocate_rar_ul_grant(rnti_t crnti, const cell_slot_resource_allocator& slot_alloc) const
{
  auto msg3_it = ra_ue_repo.find(crnti);
  if (msg3_it == ra_ue_repo.end()) {
    return false;
  }
  // A UE cannot be granted two PUSCHs in the same slot, so a new Msg3 must land after the last one allocated.
  const slot_point last_pusch_slot = msg3_it->harq_ent.last_pusch_slot();
  if (last_pusch_slot.valid() and slot_alloc.slot <= last_pusch_slot) {
    return false;
  }

  const rach_config_common& rach_cfg = *cell_cfg.params.ul_cfg_common.init_ul_bwp.rach_cfg_common;
  if (not ra_helper::is_msg1_cf_preamble(rach_cfg, msg3_it->preamble.preamble_id)) {
    // If it is a CBRA, RAR UL grant can be allocated.
    return true;
  }
  if (find_uci_on_msg3_ue_cfg(crnti) != nullptr) {
    // The UE's UCI will be moved from the PUCCH onto the Msg3 PUSCH, so a PUCCH in this slot is not an obstacle.
    return true;
  }

  // Multiplexing of UCI into RAR UL grant has been disabled or the beta offset config could not be fetched.
  // Check if there aren't any other PUCCHs falling in the same slot as the RAR UL grant.
  span<const pucch_info> pucchs = slot_alloc.result_reader().ul().pucchs().unsorted();
  return std::none_of(pucchs.begin(), pucchs.end(), [crnti](const pucch_info& pucch) { return pucch.crnti == crnti; });
}

const ue_cell_configuration* ra_scheduler::find_uci_on_msg3_ue_cfg(rnti_t crnti) const
{
  if (not sched_cfg.multiplex_uci_on_cf_rar_ul_grant) {
    return nullptr;
  }
  const ue_cell* ue_cc = find_cfra_ue(crnti);
  if (ue_cc == nullptr) {
    return nullptr;
  }
  // Semi-static beta offsets are required to size the UCI on the PUSCH, and only come with the dedicated config.
  const ue_cell_configuration& ue_cfg = ue_cc->cfg();
  const auto*                  ul_ded = ue_cfg.init_bwp().ul.ded();
  if (ul_ded == nullptr or not ul_ded->pusch_cfg.has_value() or not ul_ded->pusch_cfg->uci_cfg.has_value()) {
    return nullptr;
  }
  const auto& beta_offsets_cfg = ul_ded->pusch_cfg->uci_cfg->beta_offsets_cfg;
  if (not beta_offsets_cfg.has_value() or
      not std::holds_alternative<uci_on_pusch::beta_offsets_semi_static>(beta_offsets_cfg.value())) {
    return nullptr;
  }
  return &ue_cfg;
}

void ra_scheduler::try_multiplex_uci_on_msg3(ul_sched_info&                msg3,
                                             cell_slot_resource_allocator& msg3_alloc,
                                             rnti_t                        crnti) const
{
  const ue_cell_configuration* ue_cfg = find_uci_on_msg3_ue_cfg(crnti);
  if (ue_cfg == nullptr) {
    return;
  }
  static constexpr bool include_aperiodic_csi = false;
  uci_alloc.multiplex_uci_on_pusch(msg3, msg3_alloc, *ue_cfg, include_aperiodic_csi);
}

bool ra_scheduler::handle_msga_crc(rnti_t ra_rnti, uint8_t rapid, bool success)
{
  for (auto& msgb : pending_msgbs) {
    if (msgb.ra_rnti != ra_rnti) {
      continue;
    }
    for (auto& p : msgb.preambles) {
      if (p.info.preamble_id != rapid) {
        continue;
      }
      p.crc_result = success;

      // Track the outcome now: schedule_pending_msgbs can commit the grant several slots later, and the fallback
      // gate needs an entry to check against before then.
      // The association is immutable, so the SS/PBCH block resolved when the preamble was detected is recovered
      // here rather than carried on every pending preamble.
      const std::optional<ssb_id_t> ssb_index =
          get_preamble_ssb_index(msgb.prach_slot_rx, msgb.frequency_index, p.info.preamble_id);
      ocudu_sanity_check(ssb_index.has_value(), "MsgA preamble accepted in an occasion with no SS/PBCH block");

      ra_ue_context* ctx = success ? ra_ue_repo.add_msgb_pending(p.info, msgb.prach_slot_rx, *ssb_index)
                                   : ra_ue_repo.add(p.info, msgb.prach_slot_rx, *ssb_index);
      if (ctx != nullptr) {
        return true;
      }
      logger.warning("pci={} tc-rnti={}: Could not track pending {}. Cause: TC-RNTI ring slot already in use",
                     cell_cfg.params.pci,
                     p.info.tc_rnti,
                     success ? "successRAR completion" : "Msg3 fallback");
      return false;
    }
  }

  logger.warning(
      "pci={} rnti={}: Invalid UL CRC PDU indication. Cause: Nonexistent 2-step RA UE", cell_cfg.params.pci, ra_rnti);
  return false;
}

void ra_scheduler::schedule_pending_msg3_retxs(cell_resource_allocator& res_alloc)
{
  // Allocate pending Msg3 retransmissions.
  // Note: pending_ul_retxs size will change in this iteration, so we prefetch the next iterator.
  auto pending_ul_retxs = ra_ue_repo.harqs().pending_ul_retxs();
  for (auto it = pending_ul_retxs.begin(); it != pending_ul_retxs.end();) {
    ul_harq_process_handle h_ul = *it;
    ++it;
    auto retx_it = ra_ue_repo.find(h_ul.rnti());
    ocudu_sanity_check(retx_it != ra_ue_repo.end(), "Msg3 retx HARQ has no matching pending_msg3 entry");
    schedule_msg3_retx(res_alloc, *retx_it);
  }
}

void ra_scheduler::run_slot(cell_resource_allocator& res_alloc)
{
  // Reserve MsgA PUSCH space.
  reserve_msga_pusch_rbs(res_alloc);

  // Update Msg3 HARQ state and erase RA UE entries whose ra-ContentionResolutionTimer has expired.
  ra_ue_repo.slot_indication(res_alloc.slot_tx());

  // Allocate the MsgA PUSCHs of the PRACH occasions handled so far.
  schedule_pending_msgas(res_alloc);

  // Schedule pending RARs.
  schedule_pending_rars(res_alloc);

  // Schedule pending MsgBs.
  schedule_pending_msgbs(res_alloc);

  // Msg3 reTxs go last, as RARs and MsgBs are bounded by their response window, while a reTx can be postponed. Under
  // PRACH load, many reTxs also target preambles from collisions or false detections.
  schedule_pending_msg3_retxs(res_alloc);
}

void ra_scheduler::stop()
{
  pending_msgas.clear();
  pending_rars.clear();
  ra_ue_repo.clear();
  pending_msgbs.clear();
}

void ra_scheduler::update_pending_rars(slot_point pdcch_slot)
{
  for (auto it = pending_rars.begin(); it != pending_rars.end();) {
    pending_rar_alloc& rar_req = *it;

    // In case of RAR being outside RAR window:
    // - if window has passed, discard RAR
    // - if window hasn't started, stop loop, as RARs are ordered by slot
    if (not rar_req.rar_window.contains(pdcch_slot)) {
      if (pdcch_slot >= rar_req.rar_window.stop()) {
        logger.warning("ra-rnti={}: Could not transmit RAR within the window={}, prach_slot={}, slot_tx={}, "
                       "failed_attempts={{pdcch={}, pdsch={}, pusch={}}}",
                       rar_req.ra_rnti,
                       rar_req.rar_window,
                       rar_req.prach_slot_rx,
                       pdcch_slot,
                       rar_req.failed_attempts.pdcch,
                       rar_req.failed_attempts.pdsch,
                       rar_req.failed_attempts.pusch);
        // Clear associated Msg3 grants that were not yet scheduled.
        for (rnti_t tcrnti : rar_req.tc_rntis) {
          ra_ue_repo.erase(tcrnti);
        }
        it = pending_rars.erase(it);
        continue;
      }
      break;
    }
    ++it;
  }
}

bool ra_scheduler::is_slot_candidate_for_rar(cell_slot_resource_allocator& slot_res_alloc)
{
  slot_point pdcch_slot = slot_res_alloc.slot;

  // Check there are any RARs to schedule.
  if (pending_rars.empty() or not pending_rars.front().rar_window.contains(pdcch_slot)) {
    // There are no RARs to schedule with RAR window containing this slot.
    // Note: The pending RARs are ordered by slot, and this function should be called after outdated RARs have already
    // been removed.
    return false;
  }

  // Ensure slot for RAR PDCCH has DL enabled.
  if (not cell_cfg.is_dl_enabled(pdcch_slot)) {
    log_postponed_rar(pending_rars.front(), "PDCCH slot is not DL enabled", pdcch_slot);
    ++pending_rars.front().failed_attempts.pdcch;
    return false;
  }

  // Check for space in PDCCH result list. We check for space in PDSCH later, once the k0 is known.
  if (slot_res_alloc.result_builder().dl().dl_pdcchs().full()) {
    log_postponed_rar(pending_rars.front(), "PDCCH grants limit reached", pdcch_slot);
    ++pending_rars.front().failed_attempts.pdcch;
    return false;
  }

  // Ensure (i) RA SearchSpace PDCCH monitoring is active for this slot and (ii) there are enough DL symbols to allocate
  // the PDCCH.
  const search_space_configuration& ss_cfg = get_ra_ss_cfg(cell_cfg);
  const coreset_configuration&      cs_cfg = cell_cfg.get_common_coreset(ss_cfg.get_coreset_id());
  if (not pdcch_helper::is_pdcch_monitoring_active(pdcch_slot, ss_cfg) or
      ss_cfg.get_first_symbol_index() + cs_cfg.duration() > cell_cfg.get_nof_dl_symbol_per_slot(pdcch_slot)) {
    // RAR scheduling only possible when PDCCH monitoring is active.
    log_postponed_rar(pending_rars.front(), "PDCCH monitoring is inactive or not enough DL symbols", pdcch_slot);
    ++pending_rars.front().failed_attempts.pdcch;
    return false;
  }

  // Ensure there are UL slots where Msg3s can be allocated.
  bool       pusch_slots_available = false;
  const auto pusch_td_list         = get_pusch_td_list(cell_cfg);
  for (const auto& pusch_td_alloc : pusch_td_list) {
    const unsigned msg3_delay = cell_cfg.ul_delay(
        ra_helper::get_msg3_delay(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs, pusch_td_alloc.k2) +
        cell_cfg.ntn_cs_koffset);
    const unsigned start_ul_symbols =
        NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - cell_cfg.get_nof_ul_symbol_per_slot(pdcch_slot + msg3_delay);
    if (cell_cfg.is_ul_enabled(pdcch_slot + msg3_delay) and pusch_td_alloc.symbols.start() >= start_ul_symbols) {
      pusch_slots_available = true;
      break;
    }
  }
  if (not pusch_slots_available) {
    // Msg3 scheduling only possible when PUSCH is available.
    log_postponed_rar(pending_rars.front(), "No PUSCH slot that is UL enabled.", pdcch_slot);
    ++pending_rars.front().failed_attempts.pusch;
    return false;
  }

  return true;
}

void ra_scheduler::schedule_pending_rars(cell_resource_allocator& res_alloc)
{
  if (pending_rars.empty()) {
    // No RARs to allocate.
    return;
  }

  // In case there were attempts to schedule a pending RAR in an earlier slot, we resume the scheduling of the same
  // RAR from where we left off to avoid unnecessary work.
  // In case it is the first attempt at scheduling a pending RAR, we start from the current PDCCH slot.
  unsigned sched_start_delay = pending_rars.front().last_sched_try_slot.valid()
                                   ? pending_rars.front().last_sched_try_slot + 1 - res_alloc.slot_tx()
                                   : 0;

  for (unsigned n = sched_start_delay; n <= max_dl_slots_ahead_sched and not pending_rars.empty(); ++n) {
    // Schedule RARs for the given PDCCH slot.
    schedule_pending_rars(res_alloc, res_alloc.slot_tx() + n);
  }

  // For the RARs that were not scheduled, save the last slot when an allocation was attempted. This avoids redundant
  // scheduling attempts.
  for (pending_rar_alloc& rar : pending_rars) {
    rar.last_sched_try_slot = res_alloc.slot_tx() + max_dl_slots_ahead_sched;
  }
}

void ra_scheduler::schedule_pending_rars(cell_resource_allocator& res_alloc, slot_point pdcch_slot)
{
  // Remove outdated RARs.
  update_pending_rars(pdcch_slot);

  // Check if slot is a valid candidate for RAR scheduling.
  if (not is_slot_candidate_for_rar(res_alloc[pdcch_slot])) {
    return;
  }

  for (auto it = pending_rars.begin(); it != pending_rars.end();) {
    pending_rar_alloc& rar_req = *it;
    if (not rar_req.rar_window.contains(pdcch_slot)) {
      // RAR window hasn't started yet for this RAR. Given that the RARs are in order of slot, we can stop here.
      break;
    }
    if (rar_req.tc_rntis.empty()) {
      if (not rar_req.send_backoff_indicator) {
        // This should never happen, unless there was some corruption of the queue.
        logger.warning(
            "ra-rnti={}: Discarding RAR scheduling request. Cause: There are no TC-RNTIs associated with this RAR",
            rar_req.ra_rnti);
        it = pending_rars.erase(it);
        continue;
      }
      // All detected preambles were excluded by congestion control (e.g. SNR below threshold). Schedule a RAR
      // containing only a Backoff Indicator subheader.
      it = schedule_backoff_only_rar(it, res_alloc, pdcch_slot);
      continue;
    }

    // Try to schedule DCIs + Msg3 grants for this RAR. schedule_rar removes the allocated UEs from the RAR and
    // returns the iterator to continue from.
    it = schedule_rar(it, res_alloc, pdcch_slot);
  }
}

auto ra_scheduler::schedule_rar(std::vector<pending_rar_alloc>::iterator rar_it,
                                cell_resource_allocator&                 res_alloc,
                                slot_point pdcch_slot) -> std::vector<pending_rar_alloc>::iterator
{
  pending_rar_alloc&            rar         = *rar_it;
  cell_slot_resource_allocator& pdcch_alloc = res_alloc[pdcch_slot];

  const subcarrier_spacing dl_scs = cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.scs;
  span<const pdsch_time_domain_resource_allocation> pdsch_td_res_alloc_list = get_ra_pdsch_td_list(cell_cfg);
  const search_space_configuration&                 ss_cfg                  = get_ra_ss_cfg(cell_cfg);
  const unsigned coreset_duration = cell_cfg.get_common_coreset(ss_cfg.get_coreset_id()).duration();

  unsigned     pdsch_time_res_index = pdsch_td_res_alloc_list.size();
  unsigned     max_alloc_preambles  = 0;
  crb_interval rar_crbs{};
  for (const auto& pdsch_td_res : pdsch_td_res_alloc_list) {
    const unsigned                time_resource = std::distance(pdsch_td_res_alloc_list.begin(), &pdsch_td_res);
    cell_slot_resource_allocator& pdsch_alloc   = res_alloc[pdcch_slot + pdsch_td_res.k0];

    // > Check space in DL sched result for RAR.
    if (pdsch_alloc.result_builder().dl().rar_grants().full()) {
      // early exit.
      log_postponed_rar(rar, "RAR grants limit reached", pdcch_slot);
      ++rar.failed_attempts.pdsch;
      return std::next(rar_it);
    }

    // > Ensure slot for RAR PDSCH has DL enabled.
    if (not cell_cfg.is_dl_enabled(pdsch_alloc.slot)) {
      // Early exit.
      log_postponed_rar(rar, "PDSCH slot is not DL enabled", pdcch_slot);
      ++rar.failed_attempts.pdsch;
      return std::next(rar_it);
    }

    // > Check whether PDSCH time domain resource fits in DL symbols of the slot.
    if (pdsch_td_res.symbols.stop() > cell_cfg.get_nof_dl_symbol_per_slot(pdsch_alloc.slot)) {
      continue;
    }

    // > Check whether PDSCH time domain resource does not overlap with CORESET.
    if (pdsch_td_res.symbols.start() < ss_cfg.get_first_symbol_index() + coreset_duration) {
      continue;
    }

    if (csi_helper::is_csi_rs_slot(cell_cfg, pdsch_alloc.slot)) {
      // We cannot multiplex RAR PDSCH and CSI-RS, because at this point the UE has no access to the CSI-RS config.
      log_postponed_rar(rar, "RAR can not be in CSI-RS slot", pdcch_slot);
      ++rar.failed_attempts.pdsch;
      return std::next(rar_it);
    }

    // > Find available RBs in PDSCH for RAR grant.
    const unsigned nof_rar_rbs =
        get_nof_pdsch_prbs_required(time_resource, rar.tc_rntis.size(), rar.send_backoff_indicator).nof_prbs;
    const ofdm_symbol_range symbols        = pdsch_td_res.symbols;
    const crb_bitmap        used_crbs      = pdsch_alloc.dl_res_grid.used_crbs(dl_scs, ra_crb_lims, symbols);
    const auto              available_crbs = rb_helper::find_empty_interval_of_length(used_crbs, nof_rar_rbs);
    // Check how many allocations can we fit in the available interval.
    // Note: we have to call \c get_nof_pdsch_prbs_required for every max_preamble_allocs because the number of PRBs is
    // not linear w.r.t. the payload size (all RARs are sent in the same PDSCH grant). See \ref ocudu::get_nof_prbs.
    unsigned max_preamble_allocs = rar.tc_rntis.size();
    while (max_preamble_allocs != 0 and
           get_nof_pdsch_prbs_required(time_resource, max_preamble_allocs, rar.send_backoff_indicator).nof_prbs >
               available_crbs.length()) {
      --max_preamble_allocs;
    }

    // > Pick the TD resource that leads to the maximum allocations possible.
    if (max_preamble_allocs > max_alloc_preambles) {
      max_alloc_preambles  = max_preamble_allocs;
      rar_crbs             = available_crbs;
      pdsch_time_res_index = time_resource;
    }
  }

  if (max_alloc_preambles == 0) {
    // Early exit.
    log_postponed_rar(rar, "Not enough PRBs available for RAR PDSCH", pdcch_slot);
    ++rar.failed_attempts.pdsch;
    return std::next(rar_it);
  }

  if (pdsch_time_res_index == pdsch_td_res_alloc_list.size()) {
    // Early exit.
    log_postponed_rar(rar, "No PDSCH time domain resource found for RAR", pdcch_slot);
    ++rar.failed_attempts.pdsch;
    return std::next(rar_it);
  }

  // > Find available RBs in PUSCH for Msg3 grants. This process requires searching for a valid K2 value in
  // the list of PUSCH-TimeDomainResourceAllocation in PUSCHConfigCommon.
  static_vector<msg3_alloc_candidate, MAX_GRANTS_PER_RAR> msg3_candidates;
  const auto& ul_bwp_cfg = cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params;
  const auto  pusch_list = get_pusch_td_list(cell_cfg);
  // UEs still pending a Msg3 grant, stored in reverse order so the next UE to serve is at the back (cheap to
  // pop).
  static_vector<rnti_t, MAX_PREAMBLES_PER_PRACH_OCCASION> remaining_ues(
      std::make_reverse_iterator(rar.tc_rntis.end()), std::make_reverse_iterator(rar.tc_rntis.begin()));
  for (unsigned pusch_idx = 0, sz = pusch_list.size(); pusch_idx != sz; ++pusch_idx) {
    // >> Verify if Msg3 delay provided by current PUSCH-TimeDomainResourceAllocation corresponds to an UL slot.
    const unsigned msg3_delay = cell_cfg.ul_delay(ra_helper::get_msg3_delay(ul_bwp_cfg.scs, pusch_list[pusch_idx].k2) +
                                                  cell_cfg.ntn_cs_koffset);
    cell_slot_resource_allocator& msg3_alloc = res_alloc[pdcch_slot + msg3_delay];
    const unsigned                start_ul_symbols =
        NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - cell_cfg.get_nof_ul_symbol_per_slot(msg3_alloc.slot);
    if (not cell_cfg.is_ul_enabled(msg3_alloc.slot) or pusch_list[pusch_idx].symbols.start() < start_ul_symbols) {
      continue;
    }

    // >> Avoid allocating Msg3 PUSCH over symbols already used by SRS in this slot.
    if (pusch_list[pusch_idx].symbols.stop() > get_min_srs_symbol(msg3_alloc)) {
      continue;
    }

    unsigned pusch_res_max_grants =
        std::min<unsigned>(msg3_candidates.capacity(), max_alloc_preambles) - msg3_candidates.size();

    // >> Check space in UL sched result for remaining Msg3s.
    const unsigned list_space = msg3_alloc.result_builder().ul().puschs().space_left();
    pusch_res_max_grants      = std::min(pusch_res_max_grants, list_space);
    if (pusch_res_max_grants == 0) {
      continue;
    }

    // >> Check CRBs available in PUSCH for Msg3.
    const unsigned nof_rbs_per_msg3 = msg3_data[pusch_idx].pusch.rbs.type1().length();
    unsigned       nof_msg3_rbs     = nof_rbs_per_msg3 * pusch_res_max_grants;
    crb_bitmap     used_ul_crbs     = msg3_alloc.ul_res_grid.used_crbs(ul_bwp_cfg, pusch_list[pusch_idx].symbols);
    // Mark the CRBs used by PUCCH as occupied.
    used_ul_crbs |= pucch_crbs;
    crb_interval   msg3_crbs              = rb_helper::find_empty_interval_of_length(used_ul_crbs, nof_msg3_rbs);
    const unsigned max_allocs_on_free_rbs = msg3_crbs.length() / nof_rbs_per_msg3;
    if (max_allocs_on_free_rbs == 0) {
      continue;
    }
    if (max_allocs_on_free_rbs > msg3_alloc.result_builder().ul().puschs().space_left()) {
      // NOTE: this should not happen, but we added as an extra safety step.
      logger.warning("Overestimated number of MSG3 grants that can be allocated ({} > {}). This number will be capped",
                     max_allocs_on_free_rbs,
                     list_space);
    }
    pusch_res_max_grants = std::min(pusch_res_max_grants, max_allocs_on_free_rbs);

    // A restarted CFRA attempt keeps the Msg3 HARQ of the previous attempt until that HARQ gets its CRC.
    auto has_free_msg3_harq = [this](rnti_t rnti) {
      auto msg3_it = ra_ue_repo.find(rnti);
      return msg3_it != ra_ue_repo.end() and msg3_it->harq_ent.has_empty_ul_harqs();
    };

    // >> Register Msg3 grants for this PUSCH resource as successful.
    unsigned last_crb = msg3_crbs.start();
    for (unsigned i = 0; i != pusch_res_max_grants; ++i) {
      // Pick the next pending UE (from the back = earliest in order) that fits this slot and remove it. A UE that
      // does not fit stays in the list to be retried in a later slot.
      rnti_t rnti_to_alloc = rnti_t::INVALID_RNTI;
      for (int k = static_cast<int>(remaining_ues.size()) - 1; k >= 0; --k) {
        if (has_free_msg3_harq(remaining_ues[k]) and can_allocate_rar_ul_grant(remaining_ues[k], msg3_alloc)) {
          rnti_to_alloc = remaining_ues[k];
          remaining_ues.erase(remaining_ues.begin() + k);
          break;
        }
      }
      if (rnti_to_alloc == rnti_t::INVALID_RNTI) {
        break;
      }
      msg3_alloc_candidate& candidate = msg3_candidates.emplace_back();
      candidate.crbs                  = {last_crb, last_crb + nof_rbs_per_msg3};
      candidate.pusch_td_res_index    = pusch_idx;
      candidate.rnti_to_alloc         = rnti_to_alloc;
      last_crb += nof_rbs_per_msg3;
    }
  }
  max_alloc_preambles = msg3_candidates.size();
  if (max_alloc_preambles == 0) {
    log_postponed_rar(rar, "No PUSCH time domain resource found for Msg3");
    ++rar.failed_attempts.pusch;
    return std::next(rar_it);
  }
  rar_crbs.resize(
      get_nof_pdsch_prbs_required(pdsch_time_res_index, max_alloc_preambles, rar.send_backoff_indicator).nof_prbs);

  // > Find space in PDCCH for RAR.
  static constexpr aggregation_level aggr_lvl = aggregation_level::n4;
  const search_space_id              ss_id = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.ra_search_space_id;
  const beam_identifier              beam  = cell_cfg.params.ssb_cfg.ssb_beams.get_beam(rar.ssb_index.value());
  pdcch_dl_information* pdcch = pdcch_sch.alloc_dl_pdcch_common(pdcch_alloc, rar.ra_rnti, ss_id, aggr_lvl, beam);
  if (pdcch == nullptr) {
    log_postponed_rar(rar, "No PDCCH space for RAR", pdcch_slot);
    ++rar.failed_attempts.pdcch;
    return std::next(rar_it);
  }

  // Status: RAR allocation is successful.

  // > Fill RAR and Msg3 PDSCH, PUSCH and DCI.
  fill_rar_grant(
      res_alloc, pdcch_slot, rar_crbs, pdsch_time_res_index, msg3_candidates, rar.send_backoff_indicator, beam);

  // The UEs left in remaining_ues are those whose Msg3 was not allocated; they stay pending in the RAR. Reverse
  // back so tc_rntis keeps its original order (remaining_ues is held in reverse order).
  rar.tc_rntis.assign(std::make_reverse_iterator(remaining_ues.end()),
                      std::make_reverse_iterator(remaining_ues.begin()));

  // If all Msg3 grants were allocated, drop the RAR and advance to the next one; otherwise keep it pending and
  // advance past it.
  return rar.tc_rntis.empty() ? pending_rars.erase(rar_it) : std::next(rar_it);
}

auto ra_scheduler::schedule_backoff_only_rar(std::vector<pending_rar_alloc>::iterator rar_it,
                                             cell_resource_allocator&                 res_alloc,
                                             slot_point pdcch_slot) -> std::vector<pending_rar_alloc>::iterator
{
  pending_rar_alloc&            rar         = *rar_it;
  cell_slot_resource_allocator& pdcch_alloc = res_alloc[pdcch_slot];

  const subcarrier_spacing dl_scs = cell_cfg.params.dl_cfg_common.init_dl_bwp.generic_params.scs;
  span<const pdsch_time_domain_resource_allocation> pdsch_td_res_alloc_list = get_ra_pdsch_td_list(cell_cfg);
  const search_space_configuration&                 ss_cfg                  = get_ra_ss_cfg(cell_cfg);
  const unsigned coreset_duration = cell_cfg.get_common_coreset(ss_cfg.get_coreset_id()).duration();

  // Find PDSCH time-domain resource and CRBs that fit a Backoff Indicator-only RAR (1-byte payload, no RAPID
  // subPDUs).
  unsigned     pdsch_time_res_index = pdsch_td_res_alloc_list.size();
  crb_interval rar_crbs{};
  for (const auto& pdsch_td_res : pdsch_td_res_alloc_list) {
    const unsigned                time_resource = std::distance(pdsch_td_res_alloc_list.begin(), &pdsch_td_res);
    cell_slot_resource_allocator& pdsch_alloc   = res_alloc[pdcch_slot + pdsch_td_res.k0];

    if (pdsch_alloc.result_builder().dl().rar_grants().full() or not cell_cfg.is_dl_enabled(pdsch_alloc.slot)) {
      continue;
    }
    if (pdsch_td_res.symbols.stop() > cell_cfg.get_nof_dl_symbol_per_slot(pdsch_alloc.slot)) {
      continue;
    }
    if (pdsch_td_res.symbols.start() < ss_cfg.get_first_symbol_index() + coreset_duration) {
      continue;
    }
    if (csi_helper::is_csi_rs_slot(cell_cfg, pdsch_alloc.slot)) {
      continue;
    }

    const unsigned          nof_rar_rbs    = get_nof_pdsch_prbs_required(time_resource, 0, true).nof_prbs;
    const ofdm_symbol_range symbols        = pdsch_td_res.symbols;
    const crb_bitmap        used_crbs      = pdsch_alloc.dl_res_grid.used_crbs(dl_scs, ra_crb_lims, symbols);
    const auto              available_crbs = rb_helper::find_empty_interval_of_length(used_crbs, nof_rar_rbs);
    if (available_crbs.length() >= nof_rar_rbs) {
      rar_crbs             = crb_interval{available_crbs.start(), available_crbs.start() + nof_rar_rbs};
      pdsch_time_res_index = time_resource;
      break;
    }
  }

  if (pdsch_time_res_index == pdsch_td_res_alloc_list.size()) {
    log_postponed_rar(rar, "No PDSCH resources available for Backoff Indicator-only RAR", pdcch_slot);
    ++rar.failed_attempts.pdsch;
    return std::next(rar_it);
  }

  // > Find space in PDCCH for RAR.
  static constexpr aggregation_level aggr_lvl = aggregation_level::n4;
  const search_space_id              ss_id = cell_cfg.params.dl_cfg_common.init_dl_bwp.pdcch_common.ra_search_space_id;
  const beam_identifier              beam  = cell_cfg.params.ssb_cfg.ssb_beams.get_beam(rar.ssb_index.value());
  pdcch_dl_information* pdcch = pdcch_sch.alloc_dl_pdcch_common(pdcch_alloc, rar.ra_rnti, ss_id, aggr_lvl, beam);
  if (pdcch == nullptr) {
    log_postponed_rar(rar, "No PDCCH space for Backoff Indicator-only RAR", pdcch_slot);
    ++rar.failed_attempts.pdcch;
    return std::next(rar_it);
  }

  // Status: Backoff Indicator-only RAR allocation is successful.
  static_vector<msg3_alloc_candidate, MAX_GRANTS_PER_RAR> no_msg3_candidates;
  fill_rar_grant(res_alloc, pdcch_slot, rar_crbs, pdsch_time_res_index, no_msg3_candidates, true, beam);

  return pending_rars.erase(rar_it);
}

void ra_scheduler::fill_rar_grant(cell_resource_allocator&         res_alloc,
                                  slot_point                       pdcch_slot,
                                  crb_interval                     rar_crbs,
                                  unsigned                         pdsch_time_res_index,
                                  span<const msg3_alloc_candidate> msg3_candidates,
                                  bool                             send_backoff_indicator,
                                  beam_identifier                  beam)
{
  const auto& init_dl_bwp             = cell_cfg.params.dl_cfg_common.init_dl_bwp;
  const auto  pdsch_td_res_alloc_list = get_ra_pdsch_td_list(cell_cfg);

  cell_slot_resource_allocator& pdcch_alloc = res_alloc[pdcch_slot];
  cell_slot_resource_allocator& rar_alloc   = res_alloc[pdcch_slot + pdsch_td_res_alloc_list[pdsch_time_res_index].k0];

  // Fill RAR DCI.
  pdcch_dl_information& pdcch = pdcch_alloc.result_builder().dl().dl_pdcchs().back();
  build_dci_f1_0_ra_rnti(pdcch.dci, init_dl_bwp, rar_crbs, pdsch_time_res_index, sched_cfg.rar_mcs_index);

  // Allocate RBs and space for RAR.
  rar_alloc.dl_res_grid.fill(
      grant_info{init_dl_bwp.generic_params.scs, pdsch_td_res_alloc_list[pdsch_time_res_index].symbols, rar_crbs});

  // Fill RAR PDSCH. Room was checked before the PDCCH was allocated.
  rar_information* rar_pdu = rar_alloc.result_builder().dl().rar_grants().emplace_back();
  ocudu_assert(rar_pdu != nullptr, "No space available in scheduler RAR outputs for slot={}", rar_alloc.slot);
  rar_information& rar  = *rar_pdu;
  rar.backoff_indicator = send_backoff_indicator ? std::optional<uint8_t>{backoff_indicator_value} : std::nullopt;
  build_pdsch_f1_0_ra_rnti(
      rar.pdsch_cfg,
      get_nof_pdsch_prbs_required(pdsch_time_res_index, msg3_candidates.size(), send_backoff_indicator).tbs_bytes,
      pdcch.ctx.rnti,
      cell_cfg,
      pdcch.dci.as_ra_rnti_f1_0(),
      rar_crbs,
      rar_data[pdsch_time_res_index].dmrs_info);
  rar.pdsch_cfg.precoding_and_beamforming = make_single_beam_precoding(beam);

  const auto& init_ul_bwp         = cell_cfg.params.ul_cfg_common.init_ul_bwp;
  const auto  pusch_td_alloc_list = get_pusch_td_list(cell_cfg);
  for (unsigned i = 0; i < msg3_candidates.size(); ++i) {
    const auto&    msg3_candidate = msg3_candidates[i];
    const auto&    pusch_res      = pusch_td_alloc_list[msg3_candidate.pusch_td_res_index];
    const unsigned msg3_delay     = cell_cfg.ul_delay(
        ra_helper::get_msg3_delay(init_ul_bwp.generic_params.scs, pusch_res.k2) + cell_cfg.ntn_cs_koffset);
    cell_slot_resource_allocator& msg3_alloc = res_alloc[pdcch_slot + msg3_delay];
    const vrb_interval            vrbs       = ul_crb_to_vrb(cell_cfg, msg3_candidate.crbs);

    auto msg3_it = ra_ue_repo.find(msg3_candidate.rnti_to_alloc);
    ocudu_sanity_check(msg3_it != ra_ue_repo.end(),
                       "Pending Msg3 entry should have been reserved when RACH was received");
    auto& pending_msg3 = *msg3_it;

    // Take the Msg3 PUSCH PDU before the HARQ, so that a full list or pool leaves no HARQ allocated behind.
    ul_sched_info* msg3_pdu = rar.grants.full() ? nullptr : msg3_alloc.result_builder().ul().puschs().emplace_back();
    if (msg3_pdu == nullptr) {
      // Note: This should never happen.
      logger.error("RAR grant for tc-rnti={} will be dropped. Cause: no available free RAR or UL grants",
                   pending_msg3.preamble.tc_rnti);
      return;
    }

    // Allocate Msg3 UL HARQ.
    std::optional<ul_harq_process_handle> h_ul =
        pending_msg3.harq_ent.alloc_ul_harq(pdcch_slot, msg3_alloc.slot, sched_cfg.max_nof_msg3_harq_retxs);
    ocudu_sanity_check(h_ul.has_value(), "Pending Msg3 HARQ must be available when RAR is allocated");

    // Add MAC SDU with UL grant (Msg3) in RAR PDU.
    rar_ul_grant& msg3_info            = rar.grants.emplace_back();
    msg3_info.rapid                    = pending_msg3.preamble.preamble_id;
    msg3_info.ta                       = pending_msg3.preamble.time_advance.to_Ta(init_ul_bwp.generic_params.scs);
    msg3_info.temp_crnti               = pending_msg3.preamble.tc_rnti;
    msg3_info.time_resource_assignment = msg3_candidate.pusch_td_res_index;
    msg3_info.freq_resource_assignment = ra_frequency_type1_get_riv(
        ra_frequency_type1_configuration{init_ul_bwp.generic_params.crbs.length(), vrbs.start(), vrbs.length()});
    msg3_info.mcs = sched_cfg.msg3_mcs_index;
    // Determine TPC command based on Table 8.2-2, TS 38.213.
    msg3_info.tpc     = (init_ul_bwp.pusch_cfg_common->msg3_delta_power.value() + 6) / 2;
    msg3_info.csi_req = false;

    // Allocate Msg3 RBs.
    const ofdm_symbol_range& symbols = pusch_td_alloc_list[msg3_candidate.pusch_td_res_index].symbols;
    msg3_alloc.ul_res_grid.fill(grant_info{init_ul_bwp.generic_params.scs, symbols, msg3_candidate.crbs});

    // Fill PUSCH for Msg3.
    ul_sched_info& pusch     = *msg3_pdu;
    pusch.context.ue_index   = INVALID_DU_UE_INDEX;
    pusch.context.ss_id      = init_dl_bwp.pdcch_common.ra_search_space_id;
    pusch.context.nof_retxs  = 0;
    pusch.context.msg3_delay = msg3_delay;
    pusch.pusch_cfg          = msg3_data[msg3_candidate.pusch_td_res_index].pusch;
    pusch.pusch_cfg.rnti     = pending_msg3.preamble.tc_rnti;
    pusch.pusch_cfg.rbs      = vrbs;
    pusch.pusch_cfg.rv_index = 0;
    pusch.pusch_cfg.new_data = true;

    try_multiplex_uci_on_msg3(pusch, msg3_alloc, pending_msg3.preamble.tc_rnti);

    // Store parameters used in HARQ.
    h_ul->save_grant_params(ul_harq_alloc_context{dci_ul_rnti_config_type::tc_rnti_f0_0}, pusch.pusch_cfg);
  }
}

/// Helper function to log a failure to allocate a Msg3 grant.
/// Note: Debug log level is used. We only warn once the HARQ process retransmission period times out.
static void log_failed_msg3_retx(ocudulog::basic_logger& logger,
                                 pci_t                   pci,
                                 rnti_t                  tcrnti,
                                 slot_point              pusch_slot,
                                 const char*             cause_str)
{
  if (not pusch_slot.valid()) {
    logger.debug("pci={} tc-rnti={}: Failed to allocate PUSCH Msg3 reTx grant. Retrying it in a later slot. Cause: {}",
                 pci,
                 tcrnti,
                 cause_str);
  } else {
    logger.debug("pci={} tc-rnti={}: Failed to allocate PUSCH Msg3 reTx grant in slot {}. Retrying it in a later slot. "
                 "Cause: {}",
                 pci,
                 tcrnti,
                 pusch_slot,
                 cause_str);
  }
}

void ra_scheduler::schedule_msg3_retx(cell_resource_allocator& res_alloc, ra_ue_context& msg3_ctx) const
{
  cell_slot_resource_allocator& pdcch_alloc = res_alloc[0];
  const bwp_configuration&      bwp_ul_cmn  = cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params;

  if (not cell_cfg.is_dl_enabled(pdcch_alloc.slot)) {
    // Not possible to schedule Msg3s in this TDD slot.
    return;
  }

  // Verify there is space in PDCCH result lists for new allocations.
  if (pdcch_alloc.result_builder().dl().ul_pdcchs().full()) {
    // Early exit. No space for new PDCCHs.
    log_failed_msg3_retx(logger,
                         cell_cfg.params.pci,
                         msg3_ctx.preamble.tc_rnti,
                         {},
                         "No space available in scheduler PDCCH output list");
    return;
  }

  // Check if there are enough DL symbols to allocate the UL PDCCH
  const search_space_configuration& ss_cfg = get_ra_ss_cfg(cell_cfg);
  const coreset_configuration&      cs_cfg = cell_cfg.get_common_coreset(ss_cfg.get_coreset_id());
  if (ss_cfg.get_first_symbol_index() + cs_cfg.duration() > cell_cfg.get_nof_dl_symbol_per_slot(pdcch_alloc.slot)) {
    // Early exit. RAR scheduling only possible when PDCCH monitoring is active.
    log_failed_msg3_retx(
        logger, cell_cfg.params.pci, msg3_ctx.preamble.tc_rnti, {}, "Not enough DL symbols to fit UL PDCCH");
    return;
  }

  // Fetch UL HARQ.
  ul_harq_process_handle h_ul = msg3_ctx.harq_ent.ul_harq(to_harq_id(0)).value();
  ocudu_sanity_check(h_ul.has_pending_retx(), "schedule_msg3_retx called when HARQ has no pending reTx");
  const ul_harq_process_handle::grant_params& last_harq_params = h_ul.get_grant_params();

  const span<const pusch_time_domain_resource_allocation> pusch_td_alloc_list = get_pusch_td_list(cell_cfg);
  for (unsigned pusch_td_res_index = 0; pusch_td_res_index != pusch_td_alloc_list.size(); ++pusch_td_res_index) {
    const auto&                   pusch_td_cfg = pusch_td_alloc_list[pusch_td_res_index];
    const unsigned                k2           = pusch_td_cfg.k2;
    cell_slot_resource_allocator& pusch_alloc  = res_alloc[cell_cfg.ul_delay(k2 + cell_cfg.ntn_cs_koffset)];
    const unsigned                start_ul_symbols =
        NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - cell_cfg.get_nof_ul_symbol_per_slot(pusch_alloc.slot);
    // If it is a retx, we need to ensure we use a time_domain_resource with the same number of symbols as used for
    // the first transmission.
    const bool sym_length_match_prev_grant_for_retx = pusch_td_cfg.symbols.length() == last_harq_params.nof_symbols;
    if (not cell_cfg.is_ul_enabled(pusch_alloc.slot) or pusch_td_cfg.symbols.start() < start_ul_symbols or
        !sym_length_match_prev_grant_for_retx) {
      // Not possible to schedule Msg3s in this TDD slot due to lack of PUSCH symbols.
      continue;
    }

    // Avoid allocating Msg3 reTx PUSCH over symbols already used by SRS in this slot.
    if (pusch_td_cfg.symbols.stop() > get_min_srs_symbol(pusch_alloc)) {
      continue;
    }

    if (not can_allocate_rar_ul_grant(msg3_ctx.preamble.tc_rnti, pusch_alloc)) {
      continue;
    }

    // Verify there is space in PUSCH and PDCCH result lists for new allocations.
    auto           puschs        = pusch_alloc.result_builder().ul().puschs();
    const unsigned max_ul_grants = res_alloc.cfg.expert_cfg.ue.max_ul_grants_per_slot -
                                   static_cast<unsigned>(pusch_alloc.result_reader().ul().pucchs().size());
    if (puschs.full() or puschs.size() >= max_ul_grants) {
      // Early continue. Maximum limit of PUSCH grants was reached for this slot.
      log_failed_msg3_retx(logger,
                           cell_cfg.params.pci,
                           msg3_ctx.preamble.tc_rnti,
                           pusch_alloc.slot,
                           "No space available in scheduler PUSCH output list");
      continue;
    }

    // Try to reuse previous HARQ PRBs.
    const vrb_interval msg3_vrbs = last_harq_params.rbs.type1();
    grant_info         grant;
    grant.scs     = bwp_ul_cmn.scs;
    grant.symbols = pusch_td_cfg.symbols;
    grant.crbs    = msg3_vrb_to_crb(cell_cfg, msg3_vrbs);
    if (pusch_alloc.ul_res_grid.collides(grant)) {
      // Find available symbol x RB resources.
      log_failed_msg3_retx(
          logger, cell_cfg.params.pci, msg3_ctx.preamble.tc_rnti, pusch_alloc.slot, "Not enough available RBs");
      continue;
    }

    // > Find space in PDCCH for Msg3 DCI.
    // [3GPP TS 38.213, clause 10.1] a UE monitors PDCCH candidates in one or more of the following search spaces sets
    //  - a Type1-PDCCH CSS set configured by ra-SearchSpace in PDCCH-ConfigCommon for a DCI format with
    //    CRC scrambled by a RA-RNTI, a MsgB-RNTI, or a TC-RNTI on the primary cell.
    pdcch_ul_information* pdcch =
        pdcch_sch.alloc_ul_pdcch_common(pdcch_alloc,
                                        msg3_ctx.preamble.tc_rnti,
                                        ss_cfg.get_id(),
                                        aggregation_level::n4,
                                        cell_cfg.params.ssb_cfg.ssb_beams.get_beam(msg3_ctx.ssb_index.value()));
    if (pdcch == nullptr) {
      // Early exit. No point in continuing iteration if PDCCH fails to allocate.
      log_failed_msg3_retx(
          logger, cell_cfg.params.pci, msg3_ctx.preamble.tc_rnti, pusch_alloc.slot, "Failed to allocate UL PDCCH");
      break;
    }

    // Mark resources as occupied in the ResourceGrid.
    pusch_alloc.ul_res_grid.fill(grant);

    // Allocate new retx in the HARQ.
    if (not h_ul.new_retx(pusch_alloc.slot)) {
      logger.error(
          "pci={}: tc-rnti={}: Failed to allocate reTx for Msg3", cell_cfg.params.pci, msg3_ctx.preamble.tc_rnti);
      pdcch_sch.cancel_last_pdcch(pdcch_alloc);
      continue;
    }

    // Fill DCI.
    static constexpr uint8_t msg3_rv = 0;
    build_dci_f0_0_tc_rnti(pdcch->dci,
                           cell_cfg.params.dl_cfg_common.init_dl_bwp,
                           cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params,
                           grant.crbs,
                           pusch_td_res_index,
                           last_harq_params.mcs,
                           msg3_rv);

    // Fill PUSCH. Room was checked before the PDCCH and HARQ were allocated.
    ul_sched_info* ul_pdu = pusch_alloc.result_builder().ul().puschs().emplace_back();
    ocudu_assert(ul_pdu != nullptr,
                 "tc-rnti={}: No space available in scheduler PUSCH outputs for Msg3 retx",
                 msg3_ctx.preamble.tc_rnti);
    ul_sched_info& ul_info     = *ul_pdu;
    ul_info.context.ue_index   = INVALID_DU_UE_INDEX;
    ul_info.context.ss_id      = ss_cfg.get_id();
    ul_info.context.k2         = k2;
    ul_info.context.nof_retxs  = h_ul.nof_retxs();
    ul_info.pusch_cfg          = msg3_data[pusch_td_res_index].pusch;
    ul_info.pusch_cfg.rnti     = msg3_ctx.preamble.tc_rnti;
    ul_info.pusch_cfg.rbs      = msg3_vrbs;
    ul_info.pusch_cfg.rv_index = pdcch->dci.as_tc_rnti_f0_0().redundancy_version;
    ul_info.pusch_cfg.new_data = false;

    try_multiplex_uci_on_msg3(ul_info, pusch_alloc, msg3_ctx.preamble.tc_rnti);

    // Store parameters used in HARQ.
    h_ul.save_grant_params(ul_harq_alloc_context{dci_ul_rnti_config_type::tc_rnti_f0_0}, ul_info.pusch_cfg);

    // successful allocation. Exit loop.
    break;
  }
}

void ra_scheduler::schedule_pending_msgbs(cell_resource_allocator& res_alloc)
{
  if (pending_msgbs.empty()) {
    // Early exit: No pending MsgBs to schedule.
    return;
  }

  const unsigned sched_start_delay = pending_msgbs.front().last_sched_try_slot.valid()
                                         ? pending_msgbs.front().last_sched_try_slot + 1 - res_alloc.slot_tx()
                                         : 0U;

  for (unsigned n = sched_start_delay; n <= max_dl_slots_ahead_sched and not pending_msgbs.empty(); ++n) {
    schedule_pending_msgbs(res_alloc, res_alloc.slot_tx() + n);
  }

  for (pending_msgb_alloc& msgb : pending_msgbs) {
    msgb.last_sched_try_slot = res_alloc.slot_tx() + max_dl_slots_ahead_sched;
  }
}

void ra_scheduler::schedule_pending_msgbs(cell_resource_allocator& res_alloc, slot_point pdcch_slot)
{
  // -- Expire MsgB entries whose window has passed.
  for (auto it = pending_msgbs.begin(); it != pending_msgbs.end();) {
    if (pdcch_slot >= it->msgb_window.stop()) {
      logger.warning("msgb-rnti={}: Could not transmit MsgB within the window={}, prach_slot={}",
                     it->msgb_rnti,
                     it->msgb_window,
                     it->prach_slot_rx);
      it = pending_msgbs.erase(it);
    } else {
      ++it;
    }
  }
  if (pending_msgbs.empty()) {
    return;
  }

  // -- Check common PDCCH slot conditions.
  if (not cell_cfg.is_dl_enabled(pdcch_slot)) {
    return;
  }
  cell_slot_resource_allocator&     pdcch_slot_alloc = res_alloc[pdcch_slot];
  const search_space_configuration& ss_cfg           = get_ra_ss_cfg(cell_cfg);
  const coreset_configuration&      cs_cfg           = cell_cfg.get_common_coreset(ss_cfg.get_coreset_id());
  const unsigned                    coreset_duration = cs_cfg.duration();
  if (not pdcch_helper::is_pdcch_monitoring_active(pdcch_slot, ss_cfg) or
      ss_cfg.get_first_symbol_index() + coreset_duration > cell_cfg.get_nof_dl_symbol_per_slot(pdcch_slot)) {
    return;
  }

  const auto&                                             init_dl_bwp   = cell_cfg.params.dl_cfg_common.init_dl_bwp;
  const auto&                                             init_ul_bwp   = cell_cfg.params.ul_cfg_common.init_ul_bwp;
  const subcarrier_spacing                                dl_scs        = init_dl_bwp.generic_params.scs;
  const span<const pdsch_time_domain_resource_allocation> pdsch_td_list = get_ra_pdsch_td_list(cell_cfg);
  const span<const pusch_time_domain_resource_allocation> pusch_td_list = get_pusch_td_list(cell_cfg);
  static constexpr aggregation_level                      aggr_lvl      = aggregation_level::n4;
  const search_space_id                                   ss_id         = init_dl_bwp.pdcch_common.ra_search_space_id;

  for (auto msgb_it = pending_msgbs.begin(); msgb_it != pending_msgbs.end();) {
    if (pdcch_slot_alloc.result_builder().dl().dl_pdcchs().full()) {
      // Early exit if slot is already at max PDCCH capacity.
      return;
    }
    pending_msgb_alloc& msgb = *msgb_it;

    if (not msgb.msgb_window.contains(pdcch_slot)) {
      // MsgB window has not yet started.
      ++msgb_it;
      continue;
    }

    // If any preamble's CRC hasn't arrived yet, postpone it, unless this is the last slot in the MsgB window, in which
    // case, we proceed and treat pending CRCs as FallbackRAR.
    const bool any_crc_pending = std::any_of(
        msgb.preambles.begin(), msgb.preambles.end(), [](const auto& p) { return not p.crc_result.has_value(); });
    const bool is_last_slot_in_window = pdcch_slot + 1 >= msgb.msgb_window.stop();
    if (any_crc_pending and not is_last_slot_in_window) {
      // Postpone MsgB.
      ++msgb_it;
      continue;
    }

    // -- Find PDSCH time domain resource and available CRBs that maximizes the number of MsgB allocations --
    unsigned     pdsch_time_res_index = pdsch_td_list.size();
    unsigned     max_alloc_preambles  = 0;
    crb_interval msgb_crbs{};
    for (unsigned time_resource = 0; time_resource != pdsch_td_list.size(); ++time_resource) {
      const auto&                   pdsch_td_res = pdsch_td_list[time_resource];
      cell_slot_resource_allocator& pdsch_alloc  = res_alloc[pdcch_slot + pdsch_td_res.k0];

      if (pdsch_alloc.result_builder().dl().rar_grants().full()) {
        break;
      }
      if (not cell_cfg.is_dl_enabled(pdsch_alloc.slot) or
          pdsch_td_res.symbols.stop() > cell_cfg.get_nof_dl_symbol_per_slot(pdsch_alloc.slot) or
          pdsch_td_res.symbols.start() < ss_cfg.get_first_symbol_index() + coreset_duration) {
        // Not enough PDSCH symbols available.
        continue;
      }
      if (csi_helper::is_csi_rs_slot(cell_cfg, pdsch_alloc.slot)) {
        // CSI-RS and MsgB cannot be multiplexed.
        continue;
      }

      const crb_bitmap used_crbs      = pdsch_alloc.dl_res_grid.used_crbs(dl_scs, ra_crb_lims, pdsch_td_res.symbols);
      const auto       available_crbs = rb_helper::find_empty_interval_of_length(
          used_crbs, get_nof_pdsch_prbs_required(time_resource, msgb.preambles.size()).nof_prbs);

      unsigned max_preamble_allocs = msgb.preambles.size();
      while (max_preamble_allocs != 0 and
             get_nof_pdsch_prbs_required(time_resource, max_preamble_allocs).nof_prbs > available_crbs.length()) {
        --max_preamble_allocs;
      }

      if (max_preamble_allocs > max_alloc_preambles) {
        max_alloc_preambles  = max_preamble_allocs;
        msgb_crbs            = available_crbs;
        pdsch_time_res_index = time_resource;
      }
    }
    if (max_alloc_preambles == 0 or pdsch_time_res_index == pdsch_td_list.size()) {
      logger.debug("msgb-rnti={}: MsgB postponed. Cause: No PDSCH TDRA candidates available", msgb.msgb_rnti);
      ++msgb_it;
      continue;
    }

    // Pending CRCs (nullopt) are treated as FallbackRAR at this point.
    unsigned nof_success  = 0;
    unsigned nof_fallback = 0;
    for (const auto& p : msgb.preambles) {
      if (p.crc_result.has_value() and *p.crc_result) {
        ++nof_success;
      } else {
        ++nof_fallback;
      }
    }
    nof_success  = std::min(max_alloc_preambles, nof_success);
    nof_fallback = std::min(max_alloc_preambles, nof_fallback);

    // -- Find Msg3 PUSCH candidates for FallbackRAR preambles.
    static_vector<msg3_alloc_candidate, MAX_GRANTS_PER_RAR> msg3_candidates;
    for (unsigned pusch_idx = 0, sz = pusch_td_list.size(); pusch_idx != sz and msg3_candidates.size() < nof_fallback;
         ++pusch_idx) {
      unsigned pusch_res_max_allocs =
          std::min(msg3_candidates.capacity() - msg3_candidates.size(), nof_fallback - msg3_candidates.size());

      const unsigned msg3_delay =
          cell_cfg.ul_delay(ra_helper::get_msg3_delay(init_ul_bwp.generic_params.scs, pusch_td_list[pusch_idx].k2) +
                            cell_cfg.ntn_cs_koffset);
      cell_slot_resource_allocator& msg3_alloc = res_alloc[pdcch_slot + msg3_delay];
      const unsigned                start_ul_sym =
          NOF_OFDM_SYM_PER_SLOT_NORMAL_CP - cell_cfg.get_nof_ul_symbol_per_slot(msg3_alloc.slot);
      if (not cell_cfg.is_ul_enabled(msg3_alloc.slot) or pusch_td_list[pusch_idx].symbols.start() < start_ul_sym) {
        continue;
      }

      // Avoid allocating Msg3 PUSCH over symbols already used by SRS in this slot.
      if (pusch_td_list[pusch_idx].symbols.stop() > get_min_srs_symbol(msg3_alloc)) {
        continue;
      }

      const unsigned list_space = msg3_alloc.result_builder().ul().puschs().space_left();
      pusch_res_max_allocs      = std::min(pusch_res_max_allocs, list_space);
      if (pusch_res_max_allocs == 0) {
        continue;
      }

      const unsigned nof_rbs_per_msg3 = msg3_data[pusch_idx].pusch.rbs.type1().length();
      crb_bitmap     used_ul_crbs =
          msg3_alloc.ul_res_grid.used_crbs(init_ul_bwp.generic_params, pusch_td_list[pusch_idx].symbols);
      used_ul_crbs |= pucch_crbs;
      const crb_interval msg3_crbs =
          rb_helper::find_empty_interval_of_length(used_ul_crbs, nof_rbs_per_msg3 * pusch_res_max_allocs);
      const unsigned max_allocs_on_free_rbs = msg3_crbs.length() / nof_rbs_per_msg3;
      if (max_allocs_on_free_rbs == 0) {
        continue;
      }

      unsigned last_crb = msg3_crbs.start();
      for (unsigned i = 0; i != std::min(pusch_res_max_allocs, max_allocs_on_free_rbs); ++i) {
        msg3_alloc_candidate& candidate = msg3_candidates.emplace_back();
        candidate.crbs                  = {last_crb, last_crb + nof_rbs_per_msg3};
        candidate.pusch_td_res_index    = pusch_idx;
        last_crb += nof_rbs_per_msg3;
      }
    }

    // Determine the effective number of grants we can schedule.
    nof_fallback        = msg3_candidates.size();
    nof_success         = std::min(nof_success, max_alloc_preambles - nof_fallback);
    max_alloc_preambles = nof_success + nof_fallback;
    if (max_alloc_preambles == 0) {
      logger.debug("msgb-rnti={}: MsgB postponed. Cause: No PUSCH resources available for Msg3", msgb.msgb_rnti);
      ++msgb_it;
      continue;
    }

    // The association is immutable, so the SS/PBCH block resolved when the preambles were detected is recovered
    // here rather than carried on the pending MsgB. They all share one occasion, hence one SS/PBCH block.
    const std::optional<ssb_id_t> msgb_ssb_index =
        get_preamble_ssb_index(msgb.prach_slot_rx, msgb.frequency_index, msgb.preambles.front().info.preamble_id);
    ocudu_sanity_check(msgb_ssb_index.has_value(), "MsgA preamble accepted in an occasion with no SS/PBCH block");
    const beam_identifier msgb_beam = cell_cfg.params.ssb_cfg.ssb_beams.get_beam(msgb_ssb_index->value());

    // -- Allocate PDCCH for MsgB-RNTI --
    cell_slot_resource_allocator& pdcch_alloc = res_alloc[pdcch_slot];
    pdcch_dl_information*         pdcch =
        pdcch_sch.alloc_dl_pdcch_common(pdcch_alloc, msgb.msgb_rnti, ss_id, aggr_lvl, msgb_beam);
    if (pdcch == nullptr) {
      logger.debug("msgb-rnti={}: MsgB postponed. Cause: No PDCCH space available", msgb.msgb_rnti);
      ++msgb_it;
      continue;
    }

    cell_slot_resource_allocator& pdsch_alloc = res_alloc[pdcch_slot + pdsch_td_list[pdsch_time_res_index].k0];

    // Candidate k1 values for the MsgB HARQ-ACK feedback (successRAR only), and the resulting PDSCH-to-PUCCH delay
    // reference slot. All successRAR grants in this MsgB share the same PDSCH, so these are computed once.
    const span<const uint8_t> msgb_k1_candidates =
        cell_cfg.init_bwp.ul.td_mapper().k1_candidates(dci_dl_format::f1_0, pdsch_alloc.slot.count());
    const unsigned msgb_pdsch_delay = pdsch_alloc.slot - res_alloc.slot_tx();
    ocudu_sanity_check(not msgb_k1_candidates.empty(),
                       "No valid k1 candidate exists for the MsgB HARQ-ACK feedback of successRAR");

    // -- Attempt the successRAR HARQ-ACK PUCCH allocations up front --
    // The PUCCH resource index depends on this PDCCH's CCE position (TS 38.213, 9.2.3), so availability can only be
    // known once the PDCCH above is allocated. Doing these attempts before sizing/filling the PDSCH and pushing the
    // RAR grant means the PDSCH is sized to, and the RAR grant list filled with, only preambles that actually
    // secured a PUCCH resource, so the RAR PDU can never end up empty with leftover unschedulable preambles.
    static_vector<std::pair<pending_msgb_alloc::preamble_ctx*, rar_ul_grant::two_step_success_info>, MAX_GRANTS_PER_RAR>
        success_allocs;
    for (auto& pctx : msgb.preambles) {
      if (success_allocs.size() >= nof_success) {
        break;
      }
      if (success_allocs.full()) {
        logger.error("msgb-rnti={}: SuccessRAR grants dropped. Cause: No space in grant list", msgb.msgb_rnti);
        break;
      }
      if (not pctx.crc_result.has_value() or not *pctx.crc_result) {
        continue;
      }

      // Allocate a common PUCCH resource for the MsgB HARQ-ACK feedback, trying each k1 candidate in turn.
      std::optional<rar_ul_grant::two_step_success_info> msgb_harq_ack =
          alloc_msgb_harq_ack_pucch(res_alloc, pctx.info.tc_rnti, msgb_pdsch_delay, msgb_k1_candidates, *pdcch);
      if (msgb_harq_ack.has_value()) {
        success_allocs.emplace_back(&pctx, *msgb_harq_ack);
      } else {
        // No common PUCCH resource available for any k1 candidate this attempt. Leave this preamble pending, so a
        // later slot within the MsgB window can retry, instead of granting a successRAR without HARQ-ACK feedback.
        logger.debug("msgb-rnti={} tc-rnti={}: SuccessRAR postponed. Cause: No PUCCH resources available for "
                     "the MsgB HARQ-ACK feedback",
                     msgb.msgb_rnti,
                     pctx.info.tc_rnti);
      }
    }
    nof_success = success_allocs.size();

    if (nof_success + nof_fallback == 0) {
      // Nothing could actually be scheduled this attempt (e.g. every successRAR PUCCH allocation above failed, and
      // there were no FallbackRAR candidates). The PDSCH grid and RAR grant haven't been touched yet, and no PUCCH
      // was committed either (success_allocs is empty), so only the PDCCH needs undoing.
      pdcch_sch.cancel_last_pdcch(pdcch_alloc);
      logger.debug("msgb-rnti={}: MsgB postponed. Cause: No grant could be scheduled", msgb.msgb_rnti);
      ++msgb_it;
      continue;
    }

    // -- Fill DCI and PDSCH --
    // Note: a successRAR subPDU (TS 38.321, 6.1.5a/6.2.3a) is larger than a fallbackRAR subPDU, so the two counts are
    // sized separately rather than through get_nof_pdsch_prbs_required, which assumes uniformly-sized fallbackRAR
    // subPDUs.
    const auto msgb_prbs_tbs = get_nof_msgb_pdsch_prbs_required(pdsch_time_res_index, nof_fallback, nof_success);
    msgb_crbs.resize(msgb_prbs_tbs.nof_prbs);

    const unsigned msgb_resp_window_ms =
        cell_cfg.init_bwp.ul.rach_common()->two_step_rach_cfg->msgB_response_window_slots >>
        to_numerology_value(dl_scs);
    build_dci_f1_0_msgb_rnti(pdcch->dci,
                             init_dl_bwp,
                             msgb_crbs,
                             pdsch_time_res_index,
                             sched_cfg.rar_mcs_index,
                             msgb_resp_window_ms,
                             cell_cfg.dl_to_ue_ul_slot(msgb.prach_slot_rx).sfn());
    pdsch_alloc.dl_res_grid.fill(grant_info{dl_scs, pdsch_td_list[pdsch_time_res_index].symbols, msgb_crbs});

    // Room was checked before the PDCCH and the MsgB HARQ-ACK PUCCHs were allocated.
    rar_information* msgb_pdu = pdsch_alloc.result_builder().dl().rar_grants().emplace_back();
    ocudu_assert(msgb_pdu != nullptr, "msgb-rnti={}: No space available in scheduler RAR outputs", msgb.msgb_rnti);
    rar_information& msgb_rar = *msgb_pdu;
    build_pdsch_f1_0_ra_rnti(msgb_rar.pdsch_cfg,
                             msgb_prbs_tbs.tbs_bytes,
                             msgb.msgb_rnti,
                             cell_cfg,
                             pdcch->dci.as_ra_rnti_f1_0(),
                             msgb_crbs,
                             rar_data[pdsch_time_res_index].dmrs_info);
    msgb_rar.pdsch_cfg.precoding_and_beamforming = make_single_beam_precoding(msgb_beam);

    // -- Fill per-preamble grants --
    // SuccessRAR: UE's MsgA PUSCH decoded — 2-step RACH completes, no Msg3 needed. Already secured a PUCCH
    // resource above.
    for (auto& [pctx, msgb_harq_ack] : success_allocs) {
      rar_ul_grant& g = msgb_rar.grants.emplace_back();
      g.rapid         = pctx->info.preamble_id;
      g.ta            = pctx->info.time_advance.to_Ta(init_ul_bwp.generic_params.scs);
      g.temp_crnti    = pctx->info.tc_rnti;
      g.freq_hop_flag = false;
      g.mcs           = sched_cfg.msg3_mcs_index;
      g.tpc           = 0;
      g.csi_req       = false;
      g.type          = msgb_harq_ack;

      pctx->msgb_scheduled = true;
    }

    // FallbackRAR up to nof_fallback (limited by msg3_candidates).
    for (unsigned i = 0, fallback_count = 0, sz = msgb.preambles.size(); i != sz and fallback_count < nof_fallback;
         ++i) {
      pending_msgb_alloc::preamble_ctx& pctx = msgb.preambles[i];

      if (not pctx.crc_result.has_value() or *pctx.crc_result) {
        continue;
      }
      // FallbackRAR: MsgA PUSCH not decoded (or CRC pending at window boundary) — UE falls back to Msg3.
      if (msgb_rar.grants.full()) {
        logger.error("msgb-rnti={}: FallbackRAR grant for tc-rnti={} dropped. Cause: No space in grant list",
                     msgb.msgb_rnti,
                     pctx.info.tc_rnti);
        break;
      }

      const msg3_alloc_candidate& msg3_candidate = msg3_candidates[fallback_count];
      const auto&                 pusch_res      = pusch_td_list[msg3_candidate.pusch_td_res_index];
      const unsigned              msg3_delay     = cell_cfg.ul_delay(
          ra_helper::get_msg3_delay(init_ul_bwp.generic_params.scs, pusch_res.k2) + cell_cfg.ntn_cs_koffset);
      cell_slot_resource_allocator& msg3_alloc = res_alloc[pdcch_slot + msg3_delay];
      const vrb_interval            vrbs       = ul_crb_to_vrb(cell_cfg, msg3_candidate.crbs);

      // Fetch the pending Msg3 entry handle_msga_crc already created for this CRC=KO.
      auto pending_msg3_it = ra_ue_repo.find(pctx.info.tc_rnti);
      if (pending_msg3_it == ra_ue_repo.end()) {
        logger.warning("pci={} tc-rnti={}: Cannot schedule Msg3 for FallbackRAR. Cause: No pending entry found "
                       "for this TC-RNTI",
                       cell_cfg.params.pci,
                       pctx.info.tc_rnti);
        pctx.msgb_scheduled = true;
        ++fallback_count;
        continue;
      }
      ra_ue_context*                        pending_msg3 = &*pending_msg3_it;
      std::optional<ul_harq_process_handle> h_ul =
          pending_msg3->harq_ent.alloc_ul_harq(pdcch_slot, msg3_alloc.slot, sched_cfg.max_nof_msg3_harq_retxs);
      ocudu_sanity_check(h_ul.has_value(), "Pending Msg3 HARQ must be available for FallbackRAR");

      // Fill rar_ul_grant.
      rar_ul_grant& g            = msgb_rar.grants.emplace_back();
      g.rapid                    = pctx.info.preamble_id;
      g.ta                       = pctx.info.time_advance.to_Ta(init_ul_bwp.generic_params.scs);
      g.temp_crnti               = pctx.info.tc_rnti;
      g.freq_hop_flag            = false;
      g.time_resource_assignment = msg3_candidate.pusch_td_res_index;
      g.freq_resource_assignment = ra_frequency_type1_get_riv(
          ra_frequency_type1_configuration{init_ul_bwp.generic_params.crbs.length(), vrbs.start(), vrbs.length()});
      g.mcs = sched_cfg.msg3_mcs_index;
      // Determine TPC command based on Table 8.2-2, TS 38.213.
      g.tpc     = (init_ul_bwp.pusch_cfg_common->msg3_delta_power.value() + 6) / 2;
      g.csi_req = false;
      g.type    = rar_ul_grant::two_step_fallback_info{};

      // Allocate Msg3 RBs.
      msg3_alloc.ul_res_grid.fill(grant_info{init_ul_bwp.generic_params.scs,
                                             pusch_td_list[msg3_candidate.pusch_td_res_index].symbols,
                                             msg3_candidate.crbs});

      // Fill Msg3 PUSCH. Room was checked when the Msg3 candidates were selected.
      ul_sched_info* ul_pdu = msg3_alloc.result_builder().ul().puschs().emplace_back();
      ocudu_assert(
          ul_pdu != nullptr, "tc-rnti={}: No space available in scheduler PUSCH outputs for Msg3", pctx.info.tc_rnti);
      ul_sched_info& ul_info     = *ul_pdu;
      ul_info.context.ue_index   = INVALID_DU_UE_INDEX;
      ul_info.context.ss_id      = init_dl_bwp.pdcch_common.ra_search_space_id;
      ul_info.context.nof_retxs  = 0;
      ul_info.context.msg3_delay = msg3_delay;
      ul_info.pusch_cfg          = msg3_data[msg3_candidate.pusch_td_res_index].pusch;
      ul_info.pusch_cfg.rnti     = pctx.info.tc_rnti;
      ul_info.pusch_cfg.rbs      = vrbs;
      ul_info.pusch_cfg.rv_index = 0;
      ul_info.pusch_cfg.new_data = true;
      h_ul->save_grant_params(ul_harq_alloc_context{dci_ul_rnti_config_type::tc_rnti_f0_0}, ul_info.pusch_cfg);

      pctx.msgb_scheduled = true;
      ++fallback_count;
    }

    // Erase scheduled preambles.
    auto* new_end =
        std::remove_if(msgb.preambles.begin(), msgb.preambles.end(), [](const auto& p) { return p.msgb_scheduled; });
    msgb.preambles.erase(new_end, msgb.preambles.end());
    if (msgb.preambles.empty()) {
      msgb_it = pending_msgbs.erase(msgb_it);
    } else {
      ++msgb_it;
    }
  }
}

void ra_scheduler::reserve_msga_pusch_rbs(cell_resource_allocator& res_alloc)
{
  if (not cell_cfg.init_bwp.ul.rach_common()->two_step_rach_cfg.has_value()) {
    // Two-step RACH is not active.
    return;
  }

  // Helper function to fill grid resources.
  auto fill_grid_in_slot = [this, &res_alloc](unsigned ul_lookahead_slots) {
    auto& pusch_alloc = res_alloc[ul_lookahead_slots];

    if (not cached_init_bwp_info->is_msga_pusch_slot(pusch_alloc.slot)) {
      // MsgA PUSCH does not fall in this slot.
      return;
    }

    // Fill resource grid.
    pusch_alloc.ul_res_grid.fill(cached_init_bwp_info->reserved_msga_pusch_space);
  };

  if (OCUDU_UNLIKELY(first_slot_flag)) {
    first_slot_flag = false;
    for (unsigned lookahead_slots = 0; lookahead_slots != res_alloc.max_ul_slot_alloc_delay; ++lookahead_slots) {
      fill_grid_in_slot(lookahead_slots);
    }
  }

  fill_grid_in_slot(res_alloc.max_ul_slot_alloc_delay);
}

sch_prbs_tbs ra_scheduler::get_nof_pdsch_prbs_required(unsigned time_res_idx, unsigned nof_ul_grants, bool has_bi) const
{
  ocudu_assert(nof_ul_grants > 0 or has_bi, "Invalid number of UL grants");

  if (has_bi) {
    const auto& tbl = rar_data[time_res_idx].prbs_tbs_per_nof_grants_with_bi;
    return tbl[std::min(nof_ul_grants, static_cast<unsigned>(tbl.size()) - 1)];
  }

  return rar_data[time_res_idx].prbs_tbs_per_nof_grants
      [std::min(nof_ul_grants, static_cast<unsigned>(rar_data[time_res_idx].prbs_tbs_per_nof_grants.size())) - 1];
}

sch_prbs_tbs ra_scheduler::get_nof_msgb_pdsch_prbs_required(unsigned time_res_idx,
                                                            unsigned nof_fallback_grants,
                                                            unsigned nof_success_grants) const
{
  // subPDU sizes (subheader + payload) as per TS 38.321, 6.1.5a and 6.2.3a.
  static constexpr unsigned fallback_rar_subpdu_size_bytes = 8;
  static constexpr unsigned success_rar_subpdu_size_bytes  = 12;
  static constexpr unsigned nof_oh_prb                     = 0;
  static constexpr unsigned nof_layers                     = 1;

  ocudu_assert(nof_fallback_grants + nof_success_grants > 0, "Invalid number of MsgB UL grants");

  const span<const pdsch_time_domain_resource_allocation> pdsch_td_list = get_ra_pdsch_td_list(cell_cfg);
  const prbs_calculator_sch_config prbs_cfg{nof_fallback_grants * fallback_rar_subpdu_size_bytes +
                                                nof_success_grants * success_rar_subpdu_size_bytes,
                                            pdsch_td_list[time_res_idx].symbols.length(),
                                            calculate_nof_dmrs_per_rb(rar_data[time_res_idx].dmrs_info),
                                            nof_oh_prb,
                                            rar_mcs_config,
                                            nof_layers};
  return get_nof_prbs(prbs_cfg);
}

std::optional<rar_ul_grant::two_step_success_info>
ra_scheduler::alloc_msgb_harq_ack_pucch(cell_resource_allocator&    res_alloc,
                                        rnti_t                      tc_rnti,
                                        unsigned                    pdsch_delay,
                                        span<const uint8_t>         k1_candidates,
                                        const pdcch_dl_information& pdcch) const
{
  // TS 38.213, Section 8.2A: the successRAR's PUCCH is in the UE's uplink slot n + k + Delta + K_cell,offset, with
  // the cell-specific Koffset and the Delta of Msg3 PUSCH timing, as per TS 38.214, Table 6.1.2.1.1-5.
  const unsigned delta = ra_helper::get_pusch_delay_delta(cell_cfg.params.ul_cfg_common.init_ul_bwp.generic_params.scs);
  for (unsigned k1_idx = 0, sz = k1_candidates.size(); k1_idx != sz; ++k1_idx) {
    const std::optional<unsigned> delta_pri = pucch_alloc.alloc_common_harq_ack(
        res_alloc, tc_rnti, pdsch_delay, k1_candidates[k1_idx] + delta, pdcch, cell_cfg.ntn_cs_koffset);
    if (delta_pri.has_value()) {
      // TS 38.213, Section 8.2A: unlike the regular DCI 1_0/1_1 HARQ-ACK PUCCH resource indicator (which carries
      // Delta_PRI), the successRAR's HARQ Feedback Timing Indicator field value V maps to k1 = V + 1, and its
      // PUCCH Resource Indicator field directly carries r_PUCCH, not Delta_PRI. r_PUCCH is derived here with the
      // same (CCE position, Delta_PRI) inputs pucch_alloc used internally to pick the actual reserved resource, so
      // this always matches what was allocated.
      const unsigned r_pucch =
          get_pucch_default_resource_index(pdcch.ctx.cces.ncce, pdcch.ctx.coreset_cfg->get_nof_cces(), *delta_pri);

      const slot_point msgb_pdsch_slot = res_alloc.slot_tx() + pdsch_delay;
      // The PUCCH is received where alloc_common_harq_ack reserved it, on the gNB DL clock.
      const slot_point harq_ack_slot =
          cell_cfg.ue_ul_to_dl_slot(msgb_pdsch_slot + k1_candidates[k1_idx] + delta + cell_cfg.ntn_cs_koffset);
      if (not ra_ue_repo.set_msgb_scheduled(tc_rnti, msgb_pdsch_slot, harq_ack_slot)) {
        logger.warning("pci={} tc-rnti={}: Could not track successRAR completion. Cause: No pending entry found "
                       "for this TC-RNTI",
                       cell_cfg.params.pci,
                       tc_rnti);
      }

      return rar_ul_grant::two_step_success_info{.harq_feedback_timing_indicator =
                                                     static_cast<uint8_t>(k1_candidates[k1_idx] - 1),
                                                 .pucch_resource_indicator = static_cast<uint8_t>(r_pucch)};
    }
  }
  return std::nullopt;
}

void ra_scheduler::log_postponed_rar(const pending_rar_alloc&  rar,
                                     const char*               cause_str,
                                     std::optional<slot_point> sl) const
{
  if (sl.has_value()) {
    logger.debug("RAR allocation for ra-rnti={} was postponed. Cause: {} at slot={}", rar.ra_rnti, cause_str, *sl);
  } else {
    logger.debug("RAR allocation for ra-rnti={} was postponed. Cause: {}", rar.ra_rnti, cause_str);
  }
}
