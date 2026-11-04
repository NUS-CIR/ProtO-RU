// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../cell/cell_harq_manager.h"
#include "../cell/resource_grid.h"
#include "../pdcch_scheduling/pdcch_resource_allocator.h"
#include "../support/prbs_calculator.h"
#include "ra_ue_repository.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/prach/ssb_to_ro_mapping.h"
#include "ocudu/ran/resource_allocation/rb_bitmap.h"
#include "ocudu/scheduler/config/scheduler_expert_config.h"
#include "ocudu/scheduler/scheduler_feedback_handler.h"
#include "ocudu/scheduler/scheduler_rach_handler.h"

namespace ocudu {

class scheduler_event_logger;
class cell_metrics_handler;
class pucch_allocator;
class uci_allocator;
class ue_cell;
class ue_cell_repository;
class ue_cell_configuration;
struct ul_crc_indication;

/// Scheduler for RAR PDSCHs and Msg3 PUSCH grants and handler of RACH indications.
class ra_scheduler
{
public:
  explicit ra_scheduler(const cell_configuration& cfg_,
                        pdcch_resource_allocator& pdcch_sched_,
                        pucch_allocator&          pucch_alloc_,
                        uci_allocator&            uci_alloc_,
                        ra_ue_repository&         ra_ue_repo_,
                        ue_cell_repository&       ue_cell_db_,
                        scheduler_event_logger&   ev_logger_,
                        cell_metrics_handler&     metrics_handler_);
  ~ra_scheduler();

  /// Handle a RACH indication coming from lower layers.
  void handle_rach_indication(const rach_indication_message& msg);

  /// \brief Store the UL CRCs of the indication that ACK/NACK a Msg3 HARQ process, discarding the remaining ones.
  void handle_crc_indication(const ul_crc_indication& crc_ind);

  /// Allocate pending RARs + Msg3s
  void run_slot(cell_resource_allocator& res_alloc);

  /// Halt any pending allocations and stop RA scheduler activity.
  void stop();

private:
  class cached_bwp_info;

  struct pending_rar_failed_attempts_t {
    unsigned pdcch = 0;
    unsigned pdsch = 0;
    unsigned pusch = 0;
  };
  /// RAR grant pending to be scheduled.
  struct pending_rar_alloc {
    pending_rar_alloc(rnti_t ra_rnti_, slot_point prach_slot_rx_, ssb_id_t ssb_index_) :
      ra_rnti(ra_rnti_), prach_slot_rx(prach_slot_rx_), ssb_index(ssb_index_)
    {
    }

    /// RA-RNTI generated for a given group of detected RACH preambles.
    rnti_t ra_rnti;
    /// Slot at which PRACH preambles were detected.
    slot_point prach_slot_rx;
    /// Index of the SS/PBCH block associated with the PRACH occasion, as per TS 38.213, Section 8.1.
    ssb_id_t ssb_index;
    /// Last slot at which the scheduler attempted to allocated this RAR grant.
    slot_point last_sched_try_slot;
    /// Range of slots valid for RAR transmission.
    slot_interval rar_window;
    /// List of generated TC-RNTIs for each of the detected PRACH preambles.
    static_vector<rnti_t, MAX_PREAMBLES_PER_PRACH_OCCASION> tc_rntis;
    /// Attempts at scheduling a RAR and associated Msg3 grants.
    pending_rar_failed_attempts_t failed_attempts;
    /// \brief Set when at least one detected preamble in this occasion was excluded due to congestion control (SNR
    /// below threshold or occasion preamble count above threshold). Causes a Backoff Indicator subheader to be
    /// included in the RAR.
    bool send_backoff_indicator = false;
  };

  /// Pending MsgB whose registered MsgA preambles still have to be allocated a MsgA PUSCH.
  struct pending_msga_occasion {
    rnti_t     msgb_rnti;
    slot_point prach_slot_rx;
  };

  struct msg3_alloc_candidate {
    uint8_t      pusch_td_res_index;
    rnti_t       rnti_to_alloc;
    crb_interval crbs;
  };

  /// State for a pending MsgB PDSCH (pending to be scheduled or waiting for a positive HARQ-ACK).
  struct pending_msgb_alloc {
    /// Per-preamble state carried into MsgB scheduling.
    struct preamble_ctx {
      rach_indication_message::preamble info;
      /// CRC outcome for the MsgA PUSCH.
      /// nullopt = indication not yet received; true = CRC OK (SuccessRAR); false = CRC KO (FallbackRAR).
      std::optional<bool> crc_result;
      /// Set to true once the MsgA PUSCH of this preamble has been allocated in the grid.
      bool msga_pusch_scheduled = false;
      /// Set to true once the MsgB grant for this preamble has been scheduled.
      bool msgb_scheduled = false;

      preamble_ctx(const rach_indication_message::preamble& info_) : info(info_) {}
    };

    rnti_t     msgb_rnti = rnti_t::INVALID_RNTI;
    rnti_t     ra_rnti   = rnti_t::INVALID_RNTI;
    slot_point prach_slot_rx;
    /// Frequency domain index of the PRACH occasion that carried the MsgA preambles.
    uint8_t       frequency_index = 0;
    slot_interval msgb_window;
    /// Last slot at which the scheduler attempted to allocate this MsgB grant.
    slot_point last_sched_try_slot;
    /// List of detected MsgA preambles multiplexed into this MsgB response.
    static_vector<preamble_ctx, MAX_PREAMBLES_PER_PRACH_OCCASION> preambles;
    /// DL HARQ entity used for MsgB PDSCH retransmissions. Allocated when MsgB is first scheduled.
    unique_ue_harq_entity msgb_harq_ent;
  };

  /// Pre-compute invariant fields of RAR PDUs (PDSCH, DCI, etc.) for faster scheduling.
  void precompute_rar_fields();

  /// Pre-compute invariant fields of Msg3 PDUs (PUSCH, DCI, etc.) for faster scheduling.
  void precompute_msg3_pdus();

  /// \brief SS/PBCH block index associated with a preamble detected in the PRACH occasion
  /// \c (prach_slot_rx, fd_occasion_idx), as per TS 38.213, Section 8.1.
  /// \return Nullopt if the PRACH occasion is associated with no SS/PBCH block index.
  std::optional<ssb_id_t>
  get_preamble_ssb_index(slot_point prach_slot_rx, unsigned fd_occasion_idx, unsigned preamble_id) const;

  /// Handle a PRACH occasion carrying Msg1 (4-step RACH) preambles.
  void handle_msg1_occasion(const rach_indication_message::occasion&      occ,
                            span<const rach_indication_message::preamble> preambles,
                            slot_point                                    prach_slot_rx);

  /// Handle a PRACH occasion carrying MsgA (2-step RACH) preambles and allocate their PUSCH receptions.
  void handle_msga_occasion(const rach_indication_message::occasion&      occ,
                            span<const rach_indication_message::preamble> preambles,
                            slot_point                                    prach_slot_rx);

  /// Allocate in the grid the MsgA PUSCHs of the PRACH occasions handled so far.
  void schedule_pending_msgas(cell_resource_allocator& res_alloc);

  /// \brief Allocate in the grid the MsgA PUSCHs of the preambles registered in a pending MsgB.
  /// \note The preambles that could not be allocated one are removed from the MsgB.
  void schedule_msga_puschs(rnti_t msgb_rnti, slot_point prach_slot_rx, cell_resource_allocator& res_alloc);

  /// Apply a UL CRC that ACKs/NACKs a Msg3 HARQ process.
  void handle_ra_crc(const ul_crc_pdu_indication& crc, slot_point sl_rx);

  /// Allocate the Msg3 retransmissions left pending by the CRCs handled so far.
  void schedule_pending_msg3_retxs(cell_resource_allocator& res_alloc);

  /// Marks the MsgA PUSCH CRC outcome for preamble \c rapid under \c ra_rnti, and creates the ra_ue_repository
  /// entry (successRAR placeholder or Msg3 fallback entry).
  /// \return false if the preamble wasn't found, or the entry couldn't be created (TC-RNTI ring slot collision).
  bool handle_msga_crc(rnti_t ra_rnti, uint8_t rapid, bool success);

  void log_postponed_rar(const pending_rar_alloc&  rar,
                         const char*               cause_str,
                         std::optional<slot_point> sl = std::nullopt) const;

  /// Delete RARs that are out of the RAR window.
  void update_pending_rars(slot_point pdcch_slot);

  /// Determines whether the resource grid for the provided slot has the conditions for RAR scheduling.
  bool is_slot_candidate_for_rar(cell_slot_resource_allocator& slot_res_alloc);

  /// Schedule pending RARs in the cell resource grid.
  void schedule_pending_rars(cell_resource_allocator& res_alloc);

  /// Try scheduling pending RARs for the provided slot.
  void schedule_pending_rars(cell_resource_allocator& res_alloc, slot_point pdcch_slot);

  /// Find and allocate DL and UL resources for the pending RAR pointed to by \c rar_it and its Msg3 grants. The
  /// allocated UEs are removed from the RAR's tc_rntis.
  /// \return Iterator to the next pending RAR to process: the RAR is erased (so the returned iterator points to the
  /// following RAR) when all its Msg3 grants were allocated, otherwise the iterator advances past the kept RAR.
  std::vector<pending_rar_alloc>::iterator schedule_rar(std::vector<pending_rar_alloc>::iterator rar_it,
                                                        cell_resource_allocator&                 res_alloc,
                                                        slot_point                               pdcch_slot);

  /// \brief Schedule a RAR PDU containing only a Backoff Indicator subheader (no RAPID subPDUs), used to signal
  /// congestion to UEs whose preambles were all excluded by congestion control.
  std::vector<pending_rar_alloc>::iterator schedule_backoff_only_rar(std::vector<pending_rar_alloc>::iterator rar_it,
                                                                     cell_resource_allocator&                 res_alloc,
                                                                     slot_point pdcch_slot);

  /// Returns true if an RAR UL grant can be scheduled for the given UE in the given slot.
  bool can_allocate_rar_ul_grant(rnti_t crnti, const cell_slot_resource_allocator& slot_alloc) const;

  /// \brief Returns the UE cell associated with a C-RNTI that is still undergoing a CFRA.
  /// \return \c nullptr if the C-RNTI does not belong to a UE of this cell undergoing a CFRA.
  const ue_cell* find_cfra_ue(rnti_t crnti) const;

  /// \brief Returns the dedicated config of a CFRA UE whose pending UCI may be multiplexed into its Msg3 PUSCH.
  /// \return \c nullptr if UCI-on-Msg3 is disabled, the RNTI is not a CFRA UE, or the UE has no dedicated config.
  const ue_cell_configuration* find_uci_on_msg3_ue_cfg(rnti_t crnti) const;

  /// Move the UCI the UE has pending in the Msg3 slot from its PUCCH onto the Msg3 PUSCH, if enabled for this UE.
  void try_multiplex_uci_on_msg3(ul_sched_info& msg3, cell_slot_resource_allocator& msg3_alloc, rnti_t crnti) const;

  /// Schedule RAR grant and associated Msg3 grants in the provided scheduling resources.
  /// \param res_alloc Cell Resource Allocator.
  /// \param pdcch_slot Slot where the PDCCH is going to be scheduled.
  /// \param rar_crbs CRBs of the RAR to be scheduled.
  /// \param pdsch_time_res_index Index of PDSCH time domain resource.
  /// \param msg3_candidates List of Msg3s with respective resource information (e.g. RBs and symbols, and the RNTI to
  /// allocate) to schedule.
  void fill_rar_grant(cell_resource_allocator&         res_alloc,
                      slot_point                       pdcch_slot,
                      crb_interval                     rar_crbs,
                      unsigned                         pdsch_time_res_index,
                      span<const msg3_alloc_candidate> msg3_candidates,
                      bool                             send_backoff_indicator,
                      beam_identifier                  beam);

  /// Schedule retransmission of Msg3.
  void schedule_msg3_retx(cell_resource_allocator& res_alloc, ra_ue_context& msg3_ctx) const;

  /// Schedule pending MsgB grants in the cell resource grid.
  void schedule_pending_msgbs(cell_resource_allocator& res_alloc);

  /// Try scheduling pending MsgBs for the provided slot.
  void schedule_pending_msgbs(cell_resource_allocator& res_alloc, slot_point pdcch_slot);

  sch_prbs_tbs get_nof_pdsch_prbs_required(unsigned time_res_idx, unsigned nof_ul_grants, bool has_bi = false) const;

  /// \brief Computes the number of PRBs and TBS required for a MsgB PDSCH carrying a mix of fallbackRAR and
  /// successRAR subPDUs. Unlike \c get_nof_pdsch_prbs_required, this is not backed by a precomputed table, since a
  /// successRAR subPDU (TS38.321, 6.1.5a/6.2.3a) is larger than a fallbackRAR subPDU, so the required size depends
  /// on the split between the two, not just the total grant count.
  sch_prbs_tbs get_nof_msgb_pdsch_prbs_required(unsigned time_res_idx,
                                                unsigned nof_fallback_grants,
                                                unsigned nof_success_grants) const;

  /// Allocates a common PUCCH resource for a successRAR's MsgB HARQ-ACK, trying each k1 candidate in turn.
  /// \return The HARQ feedback fields for the RAR grant; \c std::nullopt if no PUCCH resource is available.
  std::optional<rar_ul_grant::two_step_success_info> alloc_msgb_harq_ack_pucch(cell_resource_allocator& res_alloc,
                                                                               rnti_t                   tc_rnti,
                                                                               unsigned                 pdsch_delay,
                                                                               span<const uint8_t>      k1_candidates,
                                                                               const pdcch_dl_information& pdcch) const;

  /// Reserve space in the resource grid for the MsgA PUSCH so it is not taken by other UL grants.
  void reserve_msga_pusch_rbs(cell_resource_allocator& res_alloc);

  // Set the max number of slots the scheduler can look ahead in the resource grid (with respect to the current slot) to
  // find PDSCH space for RAR.
  static constexpr unsigned max_dl_slots_ahead_sched = 8U;

  // -- args.

  const scheduler_ra_expert_config& sched_cfg;
  const cell_configuration&         cell_cfg;
  pdcch_resource_allocator&         pdcch_sch;
  pucch_allocator&                  pucch_alloc;
  uci_allocator&                    uci_alloc;
  // Shared repository of in-flight RA attempts, keyed by TC-RNTI. It also holds the 2-step RACH
  // contention-resolution outcome.
  ra_ue_repository& ra_ue_repo;
  // UEs configured in this cell.
  ue_cell_repository&     ue_cell_db;
  scheduler_event_logger& ev_logger;
  cell_metrics_handler&   metrics_hdlr;
  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("SCHED");

  // -- Derived from args.

  /// RA window size in number of slots.
  const unsigned     ra_win_nof_slots;
  const crb_interval ra_crb_lims;
  /// Duration of a single PRACH occasion in slots.
  const unsigned prach_occasion_duration_slots;
  /// Backoff Indicator value included in the RAR, as per TS38.321 Table 7.2-1, mapped from
  /// \c scheduler_ra_expert_config::backoff_indicator_duration.
  const uint8_t backoff_indicator_value;
  /// Bitmap of CRBs that might be used for PUCCH transmissions, to avoid scheduling MSG3-PUSCH over them.
  crb_bitmap pucch_crbs;
  /// Association between SS/PBCH block indexes and PRACH occasions.
  const prach_helper::ssb_to_ro_mapping ssb_ro_map;

  /// Pre-cached information related to RAR for a given PDSCH time resource.
  struct rar_param_cached_data {
    dmrs_information dmrs_info;
    /// Number of PRBs and TBS required for different numbers of grants. Index=0 corresponds to 1 grant.
    std::vector<sch_prbs_tbs> prbs_tbs_per_nof_grants;
    /// \brief Number of PRBs and TBS required for different numbers of grants, when a Backoff Indicator subheader is
    /// also included in the RAR. Index=0 corresponds to 0 grants (Backoff Indicator only, no RAPID subPDUs).
    std::vector<sch_prbs_tbs> prbs_tbs_per_nof_grants_with_bi;
  };
  std::vector<rar_param_cached_data> rar_data;

  /// Modulation and coding scheme.
  sch_mcs_description rar_mcs_config;

  /// Pre-cached information related to Msg3 for different PUSCH time resources.
  struct msg3_param_cached_data {
    dci_ul_info       dci;
    pusch_information pusch;
  };
  std::vector<msg3_param_cached_data> msg3_data;
  sch_mcs_description                 msg3_mcs_config;

  /// Pre-cached information for the initial BWP.
  std::unique_ptr<cached_bwp_info> cached_init_bwp_info;

  // -- State.

  // MsgBs whose MsgA PUSCHs are pending to be allocated. Never reallocates, as it is filled up to its reserved
  // capacity.
  std::vector<pending_msga_occasion> pending_msgas;

  // List of pending RARs to be scheduled.
  std::vector<pending_rar_alloc> pending_rars;

  // List of pending MsgBs (2-step RACH responses) to be scheduled.
  std::vector<pending_msgb_alloc> pending_msgbs;

  // Marks whether the next slot indication is the first.
  bool first_slot_flag = true;
};

} // namespace ocudu
