// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cell_event_manager.h"
#include "cell/resource_grid.h"
#include "common_scheduling/paging_scheduler.h"
#include "common_scheduling/ra_scheduler.h"
#include "common_scheduling/ra_ue_repository.h"
#include "common_scheduling/si_scheduler.h"
#include "config/cell_configuration.h"
#include "logging/cell_metrics_handler.h"
#include "logging/scheduler_event_logger.h"
#include "srs/srs_scheduler.h"
#include "uci_scheduling/uci_indication_selector.h"
#include "ue_context/ue_cell_repository.h"
#include "ocudu/adt/mpmc_queue.h"
#include "ocudu/adt/unique_function.h"
#include "ocudu/support/math/math_utils.h"
#include "ocudu/support/memory_pool/bounded_object_pool.h"

using namespace ocudu;

/// \brief Queue of the events pending to be processed by a cell, and pools of their payloads.
///
/// Events are pushed from any executor and dispatched in the cell scheduler executor.
class ocudu::cell_event_dispatcher
{
  // [Implementation defined] Number of paging requests that can be in flight at any moment.
  static constexpr size_t PAGING_POOL_SIZE = 128;
  // [Implementation defined] System information updates are rare, so only a few can be in flight at any moment.
  static constexpr size_t SI_POOL_SIZE = 4;
  // [Implementation defined] Lower layers report at most one RACH indication per slot, so the pool only needs to
  // cover the slots that can elapse before the events are dispatched.
  static constexpr size_t RACH_POOL_SIZE = 8;
  // [Implementation defined] Lower layers report one CRC indication per PUSCH PDU, so the pool has to cover a full
  // slot of PUSCH PDUs times the slots that can elapse before the events are dispatched.
  static constexpr size_t CRC_POOL_SIZE = MAX_PUSCH_PDUS_PER_SLOT * 4;
  // [Implementation defined] Every concurrent positioning measurement of the DU can target this cell.
  static constexpr size_t POSITIONING_POOL_SIZE = MAX_NOF_CONCURRENT_POSITIONING_MEASUREMENTS;
  // [Implementation defined] Number of UCI PDUs that can be in flight at any moment.
  static constexpr size_t UCI_POOL_SIZE = MAX_PUCCH_PDUS_PER_SLOT * 4;
  // [Implementation defined] Number of SRS PDUs that can be in flight at any moment.
  static constexpr size_t SRS_POOL_SIZE = MAX_SRS_PDUS_PER_SLOT * 4;
  // [Implementation defined] The UE lifecycle events carry their payload in the callback, so they have no pool of
  // their own. They are dispatched by the UE PCell, so one event per UE of the cell can be in flight at any moment.
  static constexpr size_t UE_CONFIG_EVENT_SIZE = MAX_NOF_DU_UES_PER_CELL;
  // [Implementation defined] BSR, PHR and TA reports are reported in the same quantity as CRC indications, but they
  // follow a slower path to reach the scheduler: a thread hop to a per-UE MAC UL PDU executor, plus the MAC PDU
  // decoding, so more of them accumulate in flight.
  static constexpr size_t MAC_REPORT_POOL_SIZE = MAX_PUSCH_PDUS_PER_SLOT * 8;
  // [Implementation defined] Slice reconfigurations are rare, so only a few can be in flight at any moment.
  static constexpr size_t SLICE_RECONF_POOL_SIZE = 4;
  // [Implementation defined] CG Confirmation MAC CEs are reported like the BSR, PHR and TA reports: at most one per
  // PUSCH PDU, over the same path, so the pool is sized like theirs.
  // NOTE: the cap on the CG Type 2 activations per slot does not bound this, as it limits the rate at which the
  // confirmations are generated, not the rate at which they arrive: each UE sends its CE on its own next UL
  // opportunity, so in TDD the confirmations triggered over a whole DL run can land in the same few UL slots.
  static constexpr size_t CG_CONFIRMATIONS_POOL_SIZE = MAX_PUSCH_PDUS_PER_SLOT * 8;

  /// \brief Capacity of the queue of pending events.
  ///
  /// It holds every payload the pools can hand out, so that an event type is only ever limited by its own pool and
  /// never by another type having filled the queue.
  static constexpr size_t EVENT_QUEUE_SIZE = PAGING_POOL_SIZE +          // paging
                                             2 * SI_POOL_SIZE +          // SI and ETWS/CMAS SI updates
                                             RACH_POOL_SIZE +            // RACH indications
                                             CRC_POOL_SIZE +             // CRC indications
                                             2 * POSITIONING_POOL_SIZE + // positioning requests and stops
                                             UCI_POOL_SIZE +             // UCI PDUs
                                             SRS_POOL_SIZE +             // SRS PDUs
                                             UE_CONFIG_EVENT_SIZE +      // UE creation/reconfiguration/deletion
                                             3 * MAC_REPORT_POOL_SIZE +  // BSR, PHR and TA reports
                                             SLICE_RECONF_POOL_SIZE +    // slice reconfigurations
                                             CG_CONFIRMATIONS_POOL_SIZE; // CG confirmations

  using paging_pool          = bounded_object_pool<sched_paging_information>;
  using si_pool              = bounded_object_pool<si_scheduling_update_request>;
  using pws_si_pool          = bounded_object_pool<pws_si_scheduling_update_request>;
  using rach_pool            = bounded_object_pool<rach_indication_message>;
  using crc_pool             = bounded_object_pool<ul_crc_indication>;
  using pos_req_pool         = bounded_object_pool<positioning_measurement_request::cell_info>;
  using srs_pool             = bounded_object_pool<srs_indication::srs_indication_pdu>;
  using uci_pool             = bounded_object_pool<uci_indication::uci_pdu>;
  using bsr_pool             = bounded_object_pool<ul_bsr_indication_message>;
  using phr_pool             = bounded_object_pool<ul_phr_indication_message>;
  using ta_report_pool       = bounded_object_pool<ul_ta_report_indication_message>;
  using slice_reconf_pool    = bounded_object_pool<du_cell_slice_reconfig_request>;
  using cg_confirmation_pool = bounded_object_pool<cg_confirmation_indication_message>;

  /// Event enqueued and dispatched by this class.
  struct event_t {
    static constexpr size_t callback_capacity = 64;
    using callback_type                       = unique_function<void(), callback_capacity, true>;

    callback_type callback;
    const char*   ev_name = "invalid";

    event_t() = default;
    template <typename Callable>
    event_t(const char* ev_name_, Callable&& callable) : callback(std::forward<Callable>(callable)), ev_name(ev_name_)
    {
    }
  };

  using event_queue = concurrent_queue<event_t, concurrent_queue_policy::lockfree_mpmc>;

public:
  cell_event_dispatcher(const cell_configuration& cell_cfg_, ocudulog::basic_logger& logger_) :
    cell_cfg(cell_cfg_),
    logger(logger_),
    pending_pagings(PAGING_POOL_SIZE),
    pending_si_reqs(SI_POOL_SIZE),
    pending_pws_si_reqs(SI_POOL_SIZE),
    pending_rachs(RACH_POOL_SIZE),
    pending_crcs(CRC_POOL_SIZE),
    pending_pos_reqs(POSITIONING_POOL_SIZE),
    pending_srss(SRS_POOL_SIZE),
    pending_ucis(UCI_POOL_SIZE),
    pending_bsrs(MAC_REPORT_POOL_SIZE),
    pending_phrs(MAC_REPORT_POOL_SIZE),
    pending_ta_reports(MAC_REPORT_POOL_SIZE),
    pending_slice_reconfs(SLICE_RECONF_POOL_SIZE),
    pending_cg_confirmations(CG_CONFIRMATIONS_POOL_SIZE),
    pending_events(EVENT_QUEUE_SIZE)
  {
  }

  /// Allow events to be pushed.
  void start() { active.store(true, std::memory_order_release); }

  /// Stop accepting events and discard the pending ones.
  void stop()
  {
    active.store(false, std::memory_order_release);

    event_t ev;
    while (pending_events.try_pop(ev)) {
    }
  }

  /// Dispatch the pending events.
  void run()
  {
    event_t ev;
    while (pending_events.try_pop(ev)) {
      ev.callback();
    }
  }

  /// \brief Create an event payload managed by an object pool.
  /// \return \c nullptr if the respective pool is exhausted.
  template <typename PDUType>
  auto create_pdu(const PDUType& pdu)
  {
    auto* pool = std::get<bounded_object_pool<std::decay_t<PDUType>>*>(pools);
    auto  ret  = pool->get();
    if (ret != nullptr) {
      *ret = pdu;
    } else {
      logger.warning("cell={}: Discarding {} event. Cause: Payload pool is empty",
                     cell_cfg.cell_index,
                     pdu_type_name<std::decay_t<PDUType>>());
    }
    return ret;
  }

  /// Enqueue an event to be dispatched in the next slot indication of this cell.
  template <typename Callable>
  void push(const char* ev_name, Callable&& callable)
  {
    if (OCUDU_UNLIKELY(not active.load(std::memory_order_acquire))) {
      logger.warning("cell={}: Discarding {} event. Cause: Cell is not active", cell_cfg.cell_index, ev_name);
      return;
    }

    if (not pending_events.try_push(event_t{ev_name, std::forward<Callable>(callable)})) {
      logger.warning("cell={}: Discarding {} event. Cause: Event queue is full", cell_cfg.cell_index, ev_name);
    }
  }

private:
  /// Returns a human-readable name for an event payload type.
  template <typename PDUType>
  static constexpr const char* pdu_type_name()
  {
    if constexpr (std::is_same_v<PDUType, sched_paging_information>) {
      return "paging";
    } else if constexpr (std::is_same_v<PDUType, si_scheduling_update_request>) {
      return "SI update";
    } else if constexpr (std::is_same_v<PDUType, pws_si_scheduling_update_request>) {
      return "ETWS/CMAS SI update";
    } else if constexpr (std::is_same_v<PDUType, rach_indication_message>) {
      return "RACH";
    } else if constexpr (std::is_same_v<PDUType, ul_crc_indication>) {
      return "CRC";
    } else if constexpr (std::is_same_v<PDUType, positioning_measurement_request::cell_info>) {
      return "positioning request";
    } else if constexpr (std::is_same_v<PDUType, srs_indication::srs_indication_pdu>) {
      return "SRS";
    } else if constexpr (std::is_same_v<PDUType, uci_indication::uci_pdu>) {
      return "UCI";
    } else if constexpr (std::is_same_v<PDUType, ul_bsr_indication_message>) {
      return "BSR";
    } else if constexpr (std::is_same_v<PDUType, ul_phr_indication_message>) {
      return "PHR";
    } else if constexpr (std::is_same_v<PDUType, ul_ta_report_indication_message>) {
      return "TA report";
    } else if constexpr (std::is_same_v<PDUType, du_cell_slice_reconfig_request>) {
      return "slice reconfiguration";
    } else if constexpr (std::is_same_v<PDUType, cg_confirmation_indication_message>) {
      return "CG confirmation";
    } else {
      return "unknown";
    }
  }

  const cell_configuration& cell_cfg;
  ocudulog::basic_logger&   logger;

  paging_pool          pending_pagings;
  si_pool              pending_si_reqs;
  pws_si_pool          pending_pws_si_reqs;
  rach_pool            pending_rachs;
  crc_pool             pending_crcs;
  pos_req_pool         pending_pos_reqs;
  srs_pool             pending_srss;
  uci_pool             pending_ucis;
  bsr_pool             pending_bsrs;
  phr_pool             pending_phrs;
  ta_report_pool       pending_ta_reports;
  slice_reconf_pool    pending_slice_reconfs;
  cg_confirmation_pool pending_cg_confirmations;

  std::tuple<paging_pool*,
             si_pool*,
             pws_si_pool*,
             rach_pool*,
             crc_pool*,
             pos_req_pool*,
             srs_pool*,
             uci_pool*,
             bsr_pool*,
             phr_pool*,
             ta_report_pool*,
             slice_reconf_pool*,
             cg_confirmation_pool*>
      pools{&pending_pagings,
            &pending_si_reqs,
            &pending_pws_si_reqs,
            &pending_rachs,
            &pending_crcs,
            &pending_pos_reqs,
            &pending_srss,
            &pending_ucis,
            &pending_bsrs,
            &pending_phrs,
            &pending_ta_reports,
            &pending_slice_reconfs,
            &pending_cg_confirmations};

  event_queue pending_events;

  // Whether the cell is currently accepting events.
  std::atomic<bool> active{true};
};

/// \brief More than one DL buffer occupancy update may be received per slot for the same UE and bearer. This class
/// ensures that the UE DL buffer occupancy is updated only once per bearer per slot for efficiency reasons.
class cell_event_manager::ue_dl_buffer_occupancy_manager final : public scheduler_dl_buffer_state_indication_handler
{
  using bearer_key                        = uint32_t;
  static constexpr size_t NOF_BEARER_KEYS = MAX_NOF_DU_UES * MAX_NOF_RB_LCIDS;

  static bearer_key    get_bearer_key(du_ue_index_t ue_index, lcid_t lcid) { return lcid * MAX_NOF_DU_UES + ue_index; }
  static du_ue_index_t get_ue_index(bearer_key key) { return to_du_ue_index(key % MAX_NOF_DU_UES); }
  static lcid_t        get_lcid(bearer_key key) { return uint_to_lcid(key / MAX_NOF_DU_UES); }

public:
  ue_dl_buffer_occupancy_manager(cell_event_manager& parent_) : parent(parent_), pending_evs(NOF_BEARER_KEYS)
  {
    std::fill(ue_dl_bo_table.begin(), ue_dl_bo_table.end(), std::make_pair(-1, 0));
  }

  void handle_dl_buffer_state_indication(const dl_buffer_state_indication_message& rlc_dl_bo) override
  {
    // Update DL Buffer Occupancy for the given UE and bearer.
    unsigned key          = get_bearer_key(rlc_dl_bo.ue_index, rlc_dl_bo.lcid);
    bool     first_rlc_bo = ue_dl_bo_table[key].first.exchange(rlc_dl_bo.bs, std::memory_order_acquire) < 0;
    ue_dl_bo_table[key].second.store(rlc_dl_bo.hol_toa.valid() ? rlc_dl_bo.hol_toa.count_val : -1,
                                     std::memory_order_relaxed);

    if (not first_rlc_bo) {
      // If another DL BO update has been received before for this same bearer, we do not need to enqueue a new event.
      return;
    }

    // Signal that this bearer needs its BO state updated.
    if (not pending_evs.try_push(key)) {
      parent.logger.warning("ue={} lcid={}: Discarding DL buffer occupancy update. Cause: Event queue is full",
                            rlc_dl_bo.ue_index,
                            rlc_dl_bo.lcid);
    }
  }

  void slot_indication(slot_point sl)
  {
    // Process RLC buffer updates of pending UEs.
    bearer_key key;
    while (pending_evs.try_pop(key)) {
      // Recreate latest DL BO update.
      dl_buffer_state_indication_message dl_bo;
      // > Extract UE index and LCID.
      dl_bo.ue_index = get_ue_index(key);
      dl_bo.lcid     = get_lcid(key);
      int hol_toa    = ue_dl_bo_table[key].second.load(std::memory_order_relaxed);
      if (hol_toa >= 0) {
        dl_bo.hol_toa = std::min(sl, slot_point{sl.numerology(), static_cast<unsigned>(hol_toa)});
      }
      // > Extract last DL BO value for the respective bearer and reset BO table position.
      dl_bo.bs = ue_dl_bo_table[key].first.exchange(-1, std::memory_order_release);
      if (dl_bo.bs < 0) {
        parent.logger.warning(
            "ue={} lcid={}: Invalid DL buffer occupancy value: {}", dl_bo.ue_index, dl_bo.lcid, dl_bo.bs);
        continue;
      }

      // Apply the update to the UE, which the cell group owns.
      if (parent.ue_ev_handler.handle_dl_buffer_state_indication(dl_bo)) {
        parent.ev_logger.enqueue(dl_bo);
        parent.metrics.handle_dl_buffer_state_indication(dl_bo);
      }
    }
  }

private:
  using bearer_key_queue =
      concurrent_queue<bearer_key, concurrent_queue_policy::lockfree_mpmc, concurrent_queue_wait_policy::non_blocking>;

  cell_event_manager& parent;

  // Table of pending DL Buffer Occupancy values and HOL TOAs. DL Buffer Occupancy=-1 means that it is not set. HOL
  // ToA of 0 means it is not set.
  std::array<std::pair<std::atomic<int>, std::atomic<int>>, NOF_BEARER_KEYS> ue_dl_bo_table;

  // Queue of {UE Id, LCID} pairs with pending DL Buffer Occupancy updates.
  bearer_key_queue pending_evs;
};

cell_event_manager::cell_event_manager(const cell_configuration&      cell_cfg_,
                                       cell_resource_allocator&       res_grid_,
                                       ue_cell_repository&            ue_cell_db_,
                                       si_scheduler&                  si_sch_,
                                       paging_scheduler&              pg_sch_,
                                       ra_scheduler&                  ra_sch_,
                                       srs_scheduler&                 srs_sch_,
                                       cell_group_event_handler&      ue_ev_handler_,
                                       ra_ue_repository&              ra_ue_repo_,
                                       uci_indication_selector&       uci_sel_,
                                       cell_metrics_handler&          metrics_,
                                       scheduler_event_logger&        ev_logger_,
                                       schedtrace::cell_event_tracer& ev_tracer_,
                                       ocudulog::basic_logger&        logger_) :
  cell_cfg(cell_cfg_),
  res_grid(res_grid_),
  ue_cell_db(ue_cell_db_),
  si_sch(si_sch_),
  pg_sch(pg_sch_),
  ra_sch(ra_sch_),
  srs_sch(srs_sch_),
  ue_ev_handler(ue_ev_handler_),
  ra_ue_repo(ra_ue_repo_),
  uci_sel(uci_sel_),
  metrics(metrics_),
  ev_logger(ev_logger_),
  ev_tracer(ev_tracer_),
  logger(logger_),
  dispatcher(std::make_unique<cell_event_dispatcher>(cell_cfg_, logger_)),
  dl_bo_mng(std::make_unique<ue_dl_buffer_occupancy_manager>(*this))
{
}

cell_event_manager::~cell_event_manager() = default;

void cell_event_manager::start()
{
  dispatcher->start();
}

void cell_event_manager::stop()
{
  dispatcher->stop();
}

void cell_event_manager::run_slot(slot_point sl_tx)
{
  dispatcher->run();

  // Process pending DL buffer occupancy updates.
  dl_bo_mng->slot_indication(sl_tx);
}

void cell_event_manager::handle_dl_buffer_state_indication(const dl_buffer_state_indication_message& dl_bo)
{
  dl_bo_mng->handle_dl_buffer_state_indication(dl_bo);
}

void cell_event_manager::handle_paging_information(const sched_paging_information& pi)
{
  auto pi_ptr = dispatcher->create_pdu(pi);
  if (pi_ptr == nullptr) {
    return;
  }

  dispatcher->push("paging", [this, pi_ptr = std::move(pi_ptr)]() { pg_sch.handle_paging_information(*pi_ptr); });
}

void cell_event_manager::handle_si_update_request(const si_scheduling_update_request& req)
{
  auto req_ptr = dispatcher->create_pdu(req);
  if (req_ptr == nullptr) {
    return;
  }

  dispatcher->push("SI update", [this, req_ptr = std::move(req_ptr)]() { si_sch.handle_si_update_request(*req_ptr); });
}

void cell_event_manager::handle_pws_si_update_request(const pws_si_scheduling_update_request& req)
{
  auto req_ptr = dispatcher->create_pdu(req);
  if (req_ptr == nullptr) {
    return;
  }

  dispatcher->push("PWS SI update",
                   [this, req_ptr = std::move(req_ptr)]() { si_sch.handle_pws_si_update_request(*req_ptr); });
}

void cell_event_manager::handle_rach_indication(const rach_indication_message& msg)
{
  auto msg_ptr = dispatcher->create_pdu(msg);
  if (msg_ptr == nullptr) {
    return;
  }

  dispatcher->push("RACH", [this, msg_ptr = std::move(msg_ptr)]() { ra_sch.handle_rach_indication(*msg_ptr); });
}

void cell_event_manager::handle_crc_indication(const ul_crc_indication& crc_ind)
{
  auto crc_ptr = dispatcher->create_pdu(crc_ind);
  if (crc_ptr == nullptr) {
    return;
  }

  dispatcher->push("CRC", [this, crc_ptr = std::move(crc_ptr)]() {
    // The RA scheduler selects the CRCs that belong to the RA procedure.
    ra_sch.handle_crc_indication(*crc_ptr);

    for (const ul_crc_pdu_indication& crc : crc_ptr->crcs) {
      if (crc.ue_index == INVALID_DU_UE_INDEX) {
        // The CRC of a UE that has not been created yet, and is therefore only of interest to the RA scheduler.
        continue;
      }
      handle_ue_crc(crc_ptr->sl_rx, crc);
    }
  });
}

void cell_event_manager::handle_ue_crc(slot_point sl_rx, const ul_crc_pdu_indication& crc)
{
  ue_cell* ue_cc = ue_cell_db.find(crc.ue_index);
  if (ue_cc == nullptr) {
    return;
  }

  // Update HARQ.
  const bool was_pending_cfra = ue_cc->get_pcell_state().conres_st == ue_conres_state::pending_cfra;
  const auto crc_process_res  = ue_cc->handle_crc_pdu(sl_rx, crc);
  if (not crc_process_res.has_value()) {
    if (crc_process_res.error() == ue_cell::crc_not_concluded::held_for_bundle) {
      // An occasion of a PUSCH repetition bundle that is still being received. Its result was folded into the
      // transmission's accumulated CRC; there is nothing to report until the bundle's last occasion.
      return;
    }
    // The Msg3 HARQ of a contention-free access is owned by the RA scheduler. Completing the contention resolution
    // needs the state shared by the UEs of the cell group, so hand it over to the UE scheduler.
    if (was_pending_cfra and crc.tb_crc_success) {
      ue_ev_handler.handle_cfra_msg3_acked(crc.ue_index);
    }
    return;
  }

  const ue_cell::crc_outcome& outcome = crc_process_res.value();

  const ul_crc_status crc_status = not outcome.pusch_transmitted ? ul_crc_status::dtx
                                   : crc.tb_crc_success          ? ul_crc_status::ok
                                                                 : ul_crc_status::ko;

  // Log event.
  const ul_crc_event event{crc.ue_index,
                           crc.rnti,
                           cell_cfg.cell_index,
                           sl_rx,
                           crc.harq_id,
                           crc_status,
                           outcome.tbs,
                           crc.ul_sinr_dB,
                           crc.ul_rsrp_dBFS,
                           crc.time_advance_offset};
  ev_tracer.on_event(event);
  ev_logger.enqueue(event);

  if (not outcome.pusch_transmitted) {
    return;
  }

  // Notify the CG scheduler that a CRC PDU (that wasn't DTX) has been received for a CG PUSCH of this UE. Only a CG
  // PUSCH confirms a CG activation; a CRC for a dynamic grant says nothing about whether the UE received the activation
  // DCI. Type 1 CG has no activation, so its CRCs are not forwarded.
  // NOTE: a CG PUSCH implies that the cell has a CG configured.
  if (outcome.is_cg and cell_cfg.params.init_bwp.cg_cfg->is_type2()) {
    ue_ev_handler.handle_cg_activation_complete_indication(crc.ue_index, sl_rx);
  }

  // Process Timing Advance Offset.
  if (crc.tb_crc_success and crc.time_advance_offset.has_value() and crc.ul_sinr_dB.has_value()) {
    ue_ev_handler.handle_ul_n_ta_update(
        ue_cc->ue_index, ue_cc->cfg().tag_id(), crc.time_advance_offset.value(), crc.ul_sinr_dB.value());
  }

  // Notify metrics handler.
  metrics.handle_crc_indication(sl_rx, crc, outcome.tbs);
}

void cell_event_manager::handle_positioning_measurement_request(const positioning_measurement_request::cell_info& req)
{
  ocudu_assert(req.cell_index == cell_cfg.cell_index, "Received positioning request for wrong cell");

  auto req_ptr = dispatcher->create_pdu(req);
  if (req_ptr == nullptr) {
    return;
  }

  dispatcher->push("positioning request", [this, req_ptr = std::move(req_ptr)]() {
    srs_sch.handle_positioning_measurement_request(*req_ptr);
  });
}

void cell_event_manager::handle_ue_creation(ue_config_update_event ev)
{
  dispatcher->push("UE creation", [this, ev = std::move(ev)]() mutable {
    const du_ue_index_t ue_index = ev.get_ue_index();
    const rnti_t        crnti    = ev.next_config().crnti;
    const pci_t         pci      = ev.next_config().pcell_common_cfg().params.pci;
    if (ue_ev_handler.handle_ue_creation(std::move(ev))) {
      // Trace/log event.
      ev_logger.enqueue(scheduler_event_logger::ue_creation_event{ue_index, crnti, cell_cfg.cell_index});
      metrics.handle_ue_creation(ue_index, crnti, pci);
    }
  });
}

void cell_event_manager::handle_ue_reconfiguration(ue_config_update_event ev)
{
  dispatcher->push("UE reconfiguration", [this, ev = std::move(ev)]() mutable {
    const du_ue_index_t ue_index = ev.get_ue_index();
    const rnti_t        crnti    = ev.next_config().crnti;
    if (ue_ev_handler.handle_ue_reconfiguration(std::move(ev))) {
      // Trace/log event.
      ev_logger.enqueue(scheduler_event_logger::ue_reconf_event{ue_index, crnti});
      metrics.handle_ue_reconfiguration(ue_index);
    }
  });
}

void cell_event_manager::handle_ue_deletion(ue_config_delete_event ev)
{
  dispatcher->push("UE deletion", [this, ev = std::move(ev)]() mutable {
    const du_ue_index_t ue_index = ev.ue_index();
    const ue_cell*      ue_cc    = ue_cell_db.find(ue_index);
    const rnti_t        crnti    = ue_cc != nullptr ? ue_cc->rnti() : rnti_t::INVALID_RNTI;
    if (ue_ev_handler.handle_ue_deletion(std::move(ev))) {
      // Trace/log event.
      // Note: The UE deletion is not yet complete, so we don't update the metrics yet.
      ev_logger.enqueue(sched_ue_delete_message{ue_index, crnti});
    }
  });
}

void cell_event_manager::handle_ue_config_applied(du_ue_index_t ue_idx)
{
  dispatcher->push("UE config applied", [this, ue_idx]() {
    const ue_cell* ue_cc = ue_cell_db.find(ue_idx);
    const rnti_t   crnti = ue_cc != nullptr ? ue_cc->rnti() : rnti_t::INVALID_RNTI;
    if (ue_ev_handler.handle_ue_config_applied(ue_idx)) {
      ev_logger.enqueue(scheduler_event_logger::ue_cfg_applied_event{ue_idx, crnti});
    }
  });
}

void cell_event_manager::handle_ue_deactivation_request(du_ue_index_t ue_idx)
{
  dispatcher->push("UE deactivation", [this, ue_idx]() {
    const ue_cell* ue_cc = ue_cell_db.find(ue_idx);
    const rnti_t   crnti = ue_cc != nullptr ? ue_cc->rnti() : rnti_t::INVALID_RNTI;
    if (ue_ev_handler.handle_ue_deactivation_request(ue_idx)) {
      ev_logger.enqueue(scheduler_event_logger::ue_deactivation_event{ue_idx, crnti});
    }
  });
}

void cell_event_manager::handle_ul_bsr_indication(const ul_bsr_indication_message& bsr)
{
  auto bsr_ptr = dispatcher->create_pdu(bsr);
  if (bsr_ptr == nullptr) {
    return;
  }
  dispatcher->push("BSR", [this, bsr_ptr = std::move(bsr_ptr)]() {
    if (ue_ev_handler.handle_ul_bsr_indication(*bsr_ptr)) {
      ev_logger.enqueue(*bsr_ptr);
      metrics.handle_ul_bsr_indication(*bsr_ptr);
    }
  });
}

void cell_event_manager::handle_ul_phr_indication(const ul_phr_indication_message& phr)
{
  auto phr_ptr = dispatcher->create_pdu(phr);
  if (phr_ptr == nullptr) {
    return;
  }
  dispatcher->push("PHR", [this, phr_ptr = std::move(phr_ptr)]() {
    if (ue_ev_handler.handle_ul_phr_indication(*phr_ptr)) {
      ev_logger.enqueue(*phr_ptr);
      metrics.handle_ul_phr_indication(*phr_ptr);
    }
  });
}

void cell_event_manager::handle_ul_ta_report_indication(const ul_ta_report_indication_message& ta_report)
{
  auto ta_ptr = dispatcher->create_pdu(ta_report);
  if (ta_ptr == nullptr) {
    return;
  }
  dispatcher->push("TA report", [this, ta_ptr = std::move(ta_ptr)]() {
    if (not ue_ev_handler.handle_ul_ta_report_indication(*ta_ptr)) {
      return;
    }

    // Cross-check of the cell reference-location estimate against the UE's own report. The scheduler maps the
    // measurement gap onto the uplink grid with the estimate: the gap sits on the downlink frame timing, the UE
    // transmits T_TA earlier (TS 38.211, Section 4.3.1) and drops whatever lands in it (TS 38.321, Section 5.14). A
    // mismatch beyond the report's one-slot quantization (TS 38.321, Section 6.1.3.56) - e.g. wrong estimate inputs
    // or a UE far from the reference location - means the mapping is off and the UE drops the affected grants.
    constexpr std::chrono::milliseconds            max_ul_ta_deviation{1};
    const std::optional<std::chrono::microseconds> estimate = cell_cfg.ntn_ref_location_ul_ta;
    if (estimate.has_value() and std::chrono::abs(ta_ptr->ul_ta - *estimate) > max_ul_ta_deviation) {
      logger.warning("ue={} rnti={}: Reported T_TA={}us differs from the cell estimate={}us by more than a slot",
                     ta_ptr->ue_index,
                     ta_ptr->rnti,
                     ta_ptr->ul_ta.count(),
                     estimate->count());
    } else {
      logger.debug("ue={} rnti={}: Reported T_TA={}us (cell estimate={}us)",
                   ta_ptr->ue_index,
                   ta_ptr->rnti,
                   ta_ptr->ul_ta.count(),
                   estimate.has_value() ? estimate->count() : 0);
    }
  });
}

void cell_event_manager::handle_dl_mac_ce_indication(const dl_mac_ce_indication& ce)
{
  dispatcher->push("DL MAC CE", [this, ce]() {
    if (ce.ce_lcid == lcid_dl_sch_t::UE_CON_RES_ID) {
      const ue_cell* ue_cc = ue_cell_db.find(ce.ue_index);
      logger.warning("cell={} rnti={} ue={}: Discarding ConRes CE indication. Cause: The scheduler automatically "
                     "triggers this type of CE",
                     cell_cfg.cell_index,
                     ue_cc != nullptr ? ue_cc->rnti() : rnti_t::INVALID_RNTI,
                     ce.ue_index);
      return;
    }
    if (ue_ev_handler.handle_dl_mac_ce_indication(ce)) {
      ev_logger.enqueue(ce);
    }
  });
}

void cell_event_manager::handle_crnti_ce_received(du_ue_index_t ue_index)
{
  dispatcher->push("C-RNTI CE received", [this, ue_index]() { ue_ev_handler.handle_crnti_ce_received(ue_index); });
}

void cell_event_manager::handle_slice_reconfiguration_request(const du_cell_slice_reconfig_request& req)
{
  auto req_ptr = dispatcher->create_pdu(req);
  if (req_ptr == nullptr) {
    return;
  }
  dispatcher->push("slice reconfiguration", [this, req_ptr = std::move(req_ptr)]() {
    ue_ev_handler.handle_slice_reconfiguration(*req_ptr);
    ev_logger.enqueue(scheduler_event_logger::slice_reconfiguration_event{req_ptr->cell_index});
  });
}

void cell_event_manager::handle_positioning_measurement_stop(rnti_t pos_rnti)
{
  dispatcher->push("positioning stop", [this, pos_rnti]() { srs_sch.handle_positioning_measurement_stop(pos_rnti); });
}

void cell_event_manager::handle_srs_indication(const srs_indication& srs)
{
  for (const srs_indication::srs_indication_pdu& srs_pdu : srs.srss) {
    auto srs_ptr = dispatcher->create_pdu(srs_pdu);
    if (srs_ptr == nullptr) {
      return;
    }

    dispatcher->push("SRS", [this, srs_ptr = std::move(srs_ptr)]() {
      ue_cell* ue_cc = ue_cell_db.find(srs_ptr->ue_index);
      if (ue_cc == nullptr) {
        return;
      }

      // Indicate the channel matrix.
      ue_cc->handle_srs_channel_matrix(srs_ptr->channel_matrix);

      // Log event.
      ev_logger.enqueue(scheduler_event_logger::srs_indication_event{
          srs_ptr->ue_index, srs_ptr->rnti, ue_cc->channel_state_manager().get_latest_tpmi_select_info()});

      // Handle time aligment measurement if present.
      if (srs_ptr->time_advance_offset.has_value()) {
        // Assume some SINR for the TA feedback using the channel matrix topology and near zero noise variance.
        const float frobenius_norm = srs_ptr->channel_matrix.frobenius_norm();
        const float noise_var      = near_zero;
        const float sinr_dB        = convert_power_to_dB(frobenius_norm * frobenius_norm / noise_var);

        // Notify UL TA update.
        ue_ev_handler.handle_ul_n_ta_update(
            ue_cc->ue_index, ue_cc->cfg().tag_id(), srs_ptr->time_advance_offset.value(), sinr_dB);

        // Report the SRS PDU to the metrics handler.
        metrics.handle_srs_indication(*srs_ptr, ue_cc->channel_state_manager().get_nof_ul_layers());
      }
    });
  }
}

void cell_event_manager::handle_uci_indication(const uci_indication& uci)
{
  for (const uci_indication::uci_pdu& pdu : uci.ucis) {
    auto uci_ptr = dispatcher->create_pdu(pdu);
    if (uci_ptr == nullptr) {
      return;
    }

    // Note: The removal of a UE is deferred until it has no HARQ awaiting feedback, so a UCI of a grant that was
    // scheduled for it is not lost. Its CSI and SR carry none, and it keeps sending them until it processes the RRC
    // Release, so a UCI of an unknown UE is expected and not worth reporting.
    dispatcher->push(
        "UCI", [this, uci_sl = uci.slot_rx, uci_ptr = std::move(uci_ptr)]() { handle_uci_pdu(uci_sl, *uci_ptr); });
  }
}

void cell_event_manager::handle_uci_pdu(slot_point uci_sl, const uci_indication::uci_pdu& uci_pdu)
{
  // The UE context may not exist yet if this is the successRAR's own HARQ-ACK PUCCH (2-step RACH), whose common PUCCH
  // resource is allocated by the RA scheduler before UE creation completes.
  ue_cell* ue_cc = ue_cell_db.find(uci_pdu.ue_index);
  if (ue_cc == nullptr) {
    return;
  }

  // Process the PDU and determine resulting action.
  auto action = uci_sel.handle_uci_ind_pdu(uci_sl, uci_pdu);
  if (not action.has_value()) {
    // No action came out of this UCI PDU (likely needs to combine more UCI PDUs).
    return;
  }

  // Process DL HARQ-ACK bits.
  // Note: the slot of the UCI grant is used, rather than the slot in which the PDU was received, as they differ in the
  // case of a multi-slot PUCCH repetition burst, and it is the former that the DL HARQ processes are keyed on.
  if (not action->harq_ack_bits.empty()) {
    handle_harq_ind(*ue_cc, action->uci_slot, action->uci_valid, action->harq_ack_bits, action->ul_sinr_dB);
  }

  // Process SRs.
  if (action->sr_detected) {
    // Serving the SR needs the logical channels of the UE, so hand it over to the UE scheduler.
    ue_ev_handler.handle_sr_detected(ue_cc->ue_index, uci_sl);

    // Log SR event.
    sr_event event{ue_cc->ue_index, ue_cc->rnti()};
    ev_tracer.on_event(event);
    ev_logger.enqueue(event);

    // Report SR to metrics.
    metrics.handle_sr_indication(ue_cc->ue_index, uci_sl);
  }

  // Process CSI, if present.
  if (action->csi.has_value()) {
    handle_csi(*ue_cc, uci_sl, *action->csi);
  }

  // Process SINR and Timing Advance Offset.
  if (action->uci_valid and action->ul_sinr_dB.has_value()) {
    if (action->type == uci_action::pdu_type::pucch_f2f3f4) {
      ue_cc->get_pucch_power_controller().update_pucch_sinr_f2_f3_f4(
          uci_sl, action->ul_sinr_dB.value(), not action->harq_ack_bits.empty(), action->csi.has_value());
    } else {
      ue_cc->get_pucch_power_controller().update_pucch_sinr_f0_f1(uci_sl, *action->ul_sinr_dB);
    }

    if (action->time_advance_offset.has_value()) {
      ue_ev_handler.handle_ul_n_ta_update(
          ue_cc->ue_index, ue_cc->cfg().tag_id(), *action->time_advance_offset, action->ul_sinr_dB.value());
    }
  }

  // Report the UCI PDU to the metrics handler.
  metrics.handle_uci_pdu_indication(uci_pdu.ue_index, *action);
}

bool cell_event_manager::is_msgb_harq_ack_slot(rnti_t rnti, slot_point uci_sl) const
{
  auto ra_it = ra_ue_repo.find(rnti);
  return ra_it != ra_ue_repo.end() and ra_it->msgb_ack_slot_tx.has_value() and *ra_it->msgb_ack_slot_tx == uci_sl;
}

void cell_event_manager::handle_harq_ind(ue_cell&                             ue_cc,
                                         slot_point                           uci_sl,
                                         bool                                 uci_valid,
                                         const bounded_bitset<MAX_NOF_HARQS>& harq_bits,
                                         std::optional<float>                 pucch_snr)
{
  metrics.handle_uci_with_harq_ack(ue_cc.ue_index, uci_sl, pucch_snr.has_value());

  mac_harq_ack_report_status status = mac_harq_ack_report_status::dtx;
  for (unsigned harq_idx = 0, harq_end_idx = harq_bits.size(); harq_idx != harq_end_idx; ++harq_idx) {
    // Possible report scenarios: (i) ACK, (ii) NACK, (iii) UCI invalid, (iv) UCI timeout.
    // The (iii) and (iv) are treated as HARQ report status "DTX" to not affect the DL OLLA.
    if (uci_valid) {
      status = harq_bits.test(harq_idx) ? mac_harq_ack_report_status::ack : mac_harq_ack_report_status::nack;
    }

    // Update UE HARQ state with received HARQ-ACK.
    std::optional<dl_harq_process_handle> h_dl = ue_cc.handle_dl_ack_info(uci_sl, status, harq_idx, pucch_snr);
    if (not h_dl.has_value()) {
      // HARQ process was not found or in invalid state. This is expected for the successRAR's own HARQ-ACK PUCCH
      // (2-step RACH), allocated by the RA scheduler against a common PUCCH resource rather than tracked here; only
      // warn when that fallback explanation doesn't hold.
      if (not is_msgb_harq_ack_slot(ue_cc.rnti(), uci_sl)) {
        logger.warning("rnti={}: Discarding ACK info. Cause: DL HARQ for uci slot={} and HARQ-ACK bit={} not found.",
                       ue_cc.rnti(),
                       uci_sl,
                       harq_idx);
      }
      continue;
    }
    const units::bytes tbs{h_dl->get_grant_params().tbs};

    // Log Event.
    harq_ack_event event{ue_cc.ue_index, ue_cc.rnti(), ue_cc.cell_index, uci_sl, h_dl->id(), status, tbs};
    ev_tracer.on_event(event);
    ev_logger.enqueue(event);

    // NOTE: this is for the first attachment only. In this case, the first ACK is the one that acks the ConRes or the
    // ConRes + MSG4; there is only 1 HARQ process waiting for ACKs, which acks the ConRes.
    if (h_dl->empty() and ue_cc.is_pcell() and
        ue_cc.get_pcell_state().conres_st == ue_conres_state::pending_conres_ce) {
      // Completing the contention resolution needs the state shared by the UEs of the cell group.
      ue_ev_handler.handle_conres_ce_acked(ue_cc.ue_index);
    }

    // Notify metrics handler with HARQ outcome.
    metrics.handle_dl_harq_ack(ue_cc.ue_index, status == mac_harq_ack_report_status::ack, tbs);
  }
}

void cell_event_manager::handle_csi(ue_cell& ue_cc, slot_point sl_rx, const csi_report_data& csi_rep)
{
  // Forward CSI bits to UE.
  ue_cc.handle_csi_report(csi_rep);

  // Log event.
  csi_report_event event{ue_cc.ue_index, ue_cc.rnti(), sl_rx, csi_rep};
  ev_tracer.on_event(event);
  ev_logger.enqueue(event);
}

void cell_event_manager::handle_cg_confirmation_message(const cg_confirmation_indication_message& cg_conf_ind)
{
  auto cg_ptr = dispatcher->create_pdu(cg_conf_ind);
  if (cg_ptr == nullptr) {
    return;
  }
  dispatcher->push("CG confirmation",
                   [this, ue_idx = cg_conf_ind.ue_index, slot_rx = cg_conf_ind.slot_rx, cg_ptr = std::move(cg_ptr)]() {
                     ue_ev_handler.handle_cg_activation_complete_indication(ue_idx, slot_rx);
                     ev_logger.enqueue(*cg_ptr);
                   });
}

void cell_event_manager::handle_uci_indication_timeout(slot_point uci_slot, rnti_t crnti, const uci_action& action)
{
  // Notify respective DL HARQ that the UCI went missing.
  ue_cell* ue_cc = ue_cell_db.find_by_rnti(crnti);
  if (ue_cc == nullptr) {
    // The UE context may not exist yet if this is the successRAR's own HARQ-ACK PUCCH (2-step RACH), whose common
    // PUCCH resource is allocated by the RA scheduler before UE creation completes.
    if (not is_msgb_harq_ack_slot(crnti, uci_slot)) {
      logger.warning("rnti={}: UCI timeout detected for unknown UE at UCI slot={}", crnti, uci_slot);
    }
    return;
  }

  // Forward HARQ-ACK bits.
  if (not action.harq_ack_bits.empty()) {
    handle_harq_ind(*ue_cc, uci_slot, action.uci_valid, action.harq_ack_bits, action.ul_sinr_dB);
  }

  // Forward SR indication, if pending.
  if (action.sr_detected) {
    ue_ev_handler.handle_sr_detected(ue_cc->ue_index, uci_slot);

    // Log SR event.
    sr_event event{ue_cc->ue_index, ue_cc->rnti()};
    ev_tracer.on_event(event);
    ev_logger.enqueue(event);

    // Report SR to metrics.
    metrics.handle_sr_indication(ue_cc->ue_index, uci_slot);
  }
}

static void handle_discarded_pusch(const cell_slot_resource_allocator& prev_slot_result, ue_cell_repository& ue_cell_db)
{
  for (const ul_sched_info& grant : prev_slot_result.result_reader().ul().puschs()) {
    ue_cell* ue_cc = ue_cell_db.find_by_rnti(grant.pusch_cfg.rnti);
    if (ue_cc == nullptr) {
      // UE has been removed.
      continue;
    }

    // - The lower layers will not attempt to decode the PUSCH and will not send any CRC indication.
    std::optional<ul_harq_process_handle> h_ul = ue_cc->harqs.ul_harq(to_harq_id(grant.pusch_cfg.harq_id));
    if (h_ul.has_value()) {
      // Note: We don't use this cancellation to update the UL OLLA, as we shouldn't take lates into account in link
      // adaptation.
      if (h_ul->nof_retxs() == 0) {
        // Given that the PUSCH grant was discarded before it reached the PHY, the "new_data" flag was not handled
        // and the UL softbuffer was not reset. To avoid mixing different TBs in the softbuffer, it is important to
        // release the UL HARQ process. The UE received the UL DCI, so the NDI is kept and the next newTx toggles it.
        h_ul->discard_sent_tx();
      } else {
        // To avoid a long UL HARQ timeout window (due to lack of CRC indication), it is important to force a NACK
        // in the UL HARQ process.
        h_ul->ul_crc_info(false);
      }
    }
  }
}

void cell_event_manager::handle_error_indication(slot_point sl_tx, scheduler_slot_handler::error_outcome event)
{
  auto handle_error_impl = [this, sl_tx, event]() {
    // Handle Error Indication.

    // Forward to the cell group (to the CG scheduler): this is done before the slot history lookup below, as the
    // configured grant scheduler does not need the slot result.
    ue_ev_handler.handle_error_indication(sl_tx, event);

    const cell_slot_resource_allocator* prev_slot_result = res_grid.get_history(sl_tx);
    if (prev_slot_result == nullptr) {
      logger.warning("cell={}, slot={}: Discarding error indication. Cause: Scheduler results associated with the slot "
                     "of the error indication have already been erased (current slot={})",
                     cell_cfg.cell_index,
                     sl_tx,
                     res_grid.slot_tx());
      return;
    }

    // In case DL PDCCHs were skipped, there will be the following consequences:
    // - The UE will not decode the PDSCH and will not send the respective UCI.
    // - The UE won't update the HARQ NDI, if new HARQ TB.
    // - The UCI indication coming later from the lower layers will likely contain a HARQ-ACK=DTX.
    // In case UL PDCCHs were skipped, there will be the following consequences:
    // - The UE will not decode the PUSCH.
    // - The UE won't update the HARQ NDI, if new HARQ TB.
    // - The CRC indication coming from the lower layers will likely be CRC=KO.
    // - Any UCI in the respective PUSCH will be likely reported as HARQ-ACK=DTX.
    // In neither of the cases, the HARQs will timeout, because we did not lose the UCI/CRC indications in the
    // lower layers. We do not need to cancel associated PUSCH grant (in UL PDCCH case) because it is important
    // that the PUSCH "new_data" flag reaches the lower layers, telling them whether the UL HARQ buffer needs to
    // be reset or not. Cancelling HARQ retransmissions is dangerous as it increases the chances of NDI
    // ambiguity.

    // In case of PDSCH grants being discarded, there will be the following consequences:
    // - If the PDCCH was not discarded,the UE will fail to decode the PDSCH and will send an HARQ-ACK=NACK. The
    // scheduler will retransmit the respective DL HARQ. No actions required.

    // In case of PUCCH and PUSCH grants being discarded.
    if (event.pusch_and_pucch_discarded) {
      handle_discarded_pusch(*prev_slot_result, ue_cell_db);

      uci_sel.handle_discarded_ucis(sl_tx);
    }

    // Log event.
    ev_logger.enqueue(scheduler_event_logger::error_indication_event{sl_tx, event});

    // Report metrics.
    metrics.handle_error_indication();
  };

  dispatcher->push("error indication", std::move(handle_error_impl));
}
