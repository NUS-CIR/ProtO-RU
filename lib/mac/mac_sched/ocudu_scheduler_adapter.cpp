// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu_scheduler_adapter.h"
#include "ocudu/scheduler/result/sched_result_grid.h"
#include "ocudu/scheduler/scheduler_factory.h"
#include "ocudu/support/async/async_timer.h"
#include "ocudu/support/executors/execute_until_success.h"

using namespace ocudu;

/// Convert a UE creation request for the MAC into a UE creation request of the scheduler.
static sched_ue_creation_request_message make_scheduler_ue_creation_request(const mac_ue_create_request& request)
{
  sched_ue_creation_request_message ret{};
  ret.ue_index           = request.ue_index;
  ret.crnti              = request.crnti;
  ret.starts_in_fallback = request.initial_fallback;
  ret.ul_ccch_slot_rx    = request.ul_ccch_slot_rx;
  ret.cfra_enabled       = request.cfra_preamble_index.has_value();
  ret.cfg                = request.sched_cfg;
  return ret;
}

/// Convert a UE reconfiguration request for the MAC into a UE reconfiguration request of the scheduler.
static sched_ue_reconfiguration_message
make_scheduler_ue_reconfiguration_request(const mac_ue_reconfiguration_request& request)
{
  sched_ue_reconfiguration_message ret{};
  ret.ue_index = request.ue_index;
  ret.crnti    = request.crnti;
  ret.cfg      = request.sched_cfg;
  ret.cs_rnti  = request.assigned_cs_rnti;
  return ret;
}

ocudu_scheduler_adapter::ocudu_scheduler_adapter(const ocudu_mac_sched_config& params, rnti_manager& rnti_mng_) :
  rnti_mng(rnti_mng_),
  rlf_handler(params.mac_cfg, params.ctrl_exec),
  ctrl_exec(params.ctrl_exec),
  timers(params.timers),
  logger(ocudulog::fetch_basic_logger("MAC")),
  notifier(*this),
  sched_impl(create_scheduler(scheduler_config{params.sched_cfg, notifier})),
  rach_handler(*sched_impl, rnti_mng, logger),
  pos_handler(create_positioning_handler(*sched_impl, params.ctrl_exec, params.timers, logger))
{
  ocudu_assert(last_slot_point.is_lock_free(), "slot point is not lock free");
  ocudu_assert(last_slot_tp.is_lock_free(), "slot time_point is not lock free");
}

void ocudu_scheduler_adapter::add_cell(const mac_scheduler_cell_creation_request& msg)
{
  // Setup UCI decoder for new cell.
  cell_handlers.emplace(msg.cell_params.cell_index, *this, msg.cell_params.sched_req);

  // Forward cell configuration to scheduler.
  auto sched_req                      = msg.cell_params.sched_req;
  sched_req.metrics                   = {msg.metric_notifier};
  sched_req.metrics.report_ue_metrics = msg.report_ue_metrics;
  sched_impl->handle_cell_configuration_request(sched_req);
}

void ocudu_scheduler_adapter::remove_cell(du_cell_index_t cell_index)
{
  if (not cell_handlers.contains(cell_index)) {
    return;
  }

  // Request cell removal from scheduler.
  sched_impl->handle_cell_removal_request(cell_index);

  // Remove cell from cell handlers.
  cell_handlers.erase(cell_index);
}

async_task<bool> ocudu_scheduler_adapter::handle_ue_creation_request(const mac_ue_create_request& msg)
{
  return launch_async([this, msg](coro_context<async_task<bool>>& ctx) {
    CORO_BEGIN(ctx);

    // Add UE to RLF handler.
    rlf_handler.add_ue(msg.ue_index, *msg.rlf_notifier);

    // In case of CFRA, store the RA preamble.
    if (msg.cfra_preamble_index.has_value()) {
      if (not cell_handlers[msg.cell_index].get_rach_handler().handle_cfra_allocation(
              msg.cfra_preamble_index.value(), msg.ue_index, msg.crnti)) {
        CORO_EARLY_RETURN(false);
      }
    }

    // Create UE in the Scheduler.
    sched_impl->handle_ue_creation_request(make_scheduler_ue_creation_request(msg));

    // Await Scheduler notification that UE was added.
    CORO_AWAIT_VALUE(bool res, sched_cfg_notif_map[msg.ue_index].ue_config_ready);
    sched_cfg_notif_map[msg.ue_index].ue_config_ready.reset();

    CORO_RETURN(res);
  });
}

async_task<bool> ocudu_scheduler_adapter::handle_ue_reconfiguration_request(const mac_ue_reconfiguration_request& msg)
{
  return launch_async([this, msg](coro_context<async_task<bool>>& ctx) {
    CORO_BEGIN(ctx);

    // Reconfigure UE in the scheduler.
    sched_impl->handle_ue_reconfiguration_request(make_scheduler_ue_reconfiguration_request(msg));

    // Await Scheduler notification, which reports whether the scheduler accepted the new configuration.
    CORO_AWAIT_VALUE(bool res, sched_cfg_notif_map[msg.ue_index].ue_config_ready);
    sched_cfg_notif_map[msg.ue_index].ue_config_ready.reset();

    CORO_RETURN(res);
  });
}

async_task<void> ocudu_scheduler_adapter::handle_ue_removal_request(const mac_ue_delete_request& msg)
{
  return launch_async([this, msg](coro_context<async_task<void>>& ctx) {
    CORO_BEGIN(ctx);

    if (msg.min_removal_delay.count() > 0) {
      // > Wait for the minimum removal delay before starting the actual UE removal.

      // Deactivate UE so it doesn't get new PDSCH/PUSCH grants during the wait period.
      sched_impl->handle_ue_deactivation_request(msg.ue_index);

      // Wait for the specified delay.
      CORO_AWAIT(async_wait_for(timers.create_unique_timer(ctrl_exec), msg.min_removal_delay));
    }

    // Remove UE from the scheduler.
    sched_impl->handle_ue_removal_request(msg.ue_index);

    // Await Scheduler notification.
    CORO_AWAIT(sched_cfg_notif_map[msg.ue_index].ue_config_ready);
    sched_cfg_notif_map[msg.ue_index].ue_config_ready.reset();

    // In case the UE was created via CFRA, remove the RA preamble.
    cell_handlers[msg.cell_index].get_rach_handler().handle_cfra_deallocation(msg.ue_index);

    // Remove UE from RLF handler.
    rlf_handler.rem_ue(msg.ue_index, msg.cell_index);

    CORO_RETURN();
  });
}

void ocudu_scheduler_adapter::handle_ue_config_applied(du_ue_index_t ue_index)
{
  // Free any CFRA preamble slot still held by this UE — the config being applied means
  // the random access procedure is complete and the slot can be reused.
  rach_handler.handle_cfra_deallocation(ue_index);

  // Notify scheduler that the UE confirmed the configuration.
  sched_impl->handle_ue_config_applied(ue_index);
}

/// Converts BSR MAC CE (See 38.321, 6.1.3.1) to Scheduler UL BSR event type.
static ul_bsr_lcg_report make_sched_lcg_report(const lcg_bsr_report& ul_mac_bsr, bsr_format bsr_format)
{
  return ul_bsr_lcg_report{ul_mac_bsr.lcg_id, buff_size_field_to_bytes(ul_mac_bsr.buffer_size, bsr_format)};
}

void ocudu_scheduler_adapter::handle_ul_bsr_indication(const mac_bsr_ce_info& bsr)
{
  // Prepare UL BSR indication for Scheduler
  ul_bsr_indication_message ul_bsr_ind{};
  ul_bsr_ind.cell_index = bsr.cell_index;
  ul_bsr_ind.ue_index   = bsr.ue_index;
  ul_bsr_ind.crnti      = bsr.rnti;
  ul_bsr_ind.type       = bsr.bsr_fmt;
  if (bsr.bsr_fmt == bsr_format::SHORT_BSR or bsr.bsr_fmt == bsr_format::SHORT_TRUNC_BSR) {
    const ul_bsr_lcg_report sched_bsr = make_sched_lcg_report(bsr.lcg_reports[0], bsr.bsr_fmt);

    if (sched_bsr.nof_bytes == 0) {
      // In case SBSR=0, assume all LCGs are 0.
      for (unsigned j = 0; j <= MAX_LCG_ID; ++j) {
        ul_bsr_ind.reported_lcgs.push_back(ul_bsr_lcg_report{uint_to_lcg_id(j), 0U});
      }
    } else {
      ul_bsr_ind.reported_lcgs.push_back(sched_bsr);
    }
  } else {
    // Long BSR or Long truncated BSR.
    for (const lcg_bsr_report& lb : bsr.lcg_reports) {
      ul_bsr_ind.reported_lcgs.push_back(make_sched_lcg_report(lb, bsr.bsr_fmt));
    }
  }

  // Send UL BSR indication to Scheduler.
  sched_impl->handle_ul_bsr_indication(ul_bsr_ind);
}

void ocudu_scheduler_adapter::handle_ul_sched_command(const mac_ul_scheduling_command& cmd)
{
  // Convert UL scheduling command into a small BSR indication. The SR indication is not used for this purpose
  // because the Scheduler relies on UCI signalling to know the current UE state.
  ul_bsr_indication_message bsr{};
  bsr.cell_index    = cmd.cell_index;
  bsr.ue_index      = cmd.ue_index;
  bsr.crnti         = cmd.rnti;
  bsr.type          = bsr_format::SHORT_BSR;
  bsr.reported_lcgs = {ul_bsr_lcg_report{uint_to_lcg_id(0), 8U}};

  // Send BSR indication to Scheduler.
  sched_impl->handle_ul_bsr_indication(bsr);
}

void ocudu_scheduler_adapter::handle_dl_mac_ce_indication(const mac_ce_scheduling_command& mac_ce)
{
  sched_impl->handle_dl_mac_ce_indication(dl_mac_ce_indication{mac_ce.ue_index, mac_ce.ce_lcid});
}

static slot_point chrono_to_slot_point(std::chrono::steady_clock::time_point hol_toa,
                                       std::chrono::steady_clock::time_point last_slot_tp,
                                       slot_point                            last_slot_p)
{
  using namespace std::chrono;
  static constexpr microseconds half_system_frame_dur = milliseconds{10240 / 2};

  // Get delay between last slot indication time point and HOL ToA.
  microseconds hol_delay = duration_cast<microseconds>(last_slot_tp - hol_toa);

  // Bound delay to avoid negative values and slot wrap around ambiguity.
  hol_delay = std::min(std::max(hol_delay, microseconds{0}), half_system_frame_dur);

  // Convert usec to slots
  const int hol_delay_slots = hol_delay.count() * last_slot_p.nof_slots_per_subframe() / 1000;

  // Subtract the delay to the current slot.
  return last_slot_p - hol_delay_slots;
}

void ocudu_scheduler_adapter::handle_dl_buffer_state_update(const mac_dl_buffer_state_indication_message& mac_dl_bs_ind)
{
  using namespace std::chrono;

  // Forward DL buffer state indication to the scheduler.
  dl_buffer_state_indication_message bs{};
  bs.ue_index = mac_dl_bs_ind.ue_index;
  bs.lcid     = mac_dl_bs_ind.lcid;
  bs.bs       = mac_dl_bs_ind.bs;
  if (mac_dl_bs_ind.hol_toa.has_value()) {
    // Check if at least one slot indication has been processed.
    const steady_clock::time_point sl_tp = last_slot_tp.load(std::memory_order_relaxed);
    if (sl_tp != steady_clock::time_point{}) {
      // Convert HOL TOA from chrono time point to slots.
      bs.hol_toa = chrono_to_slot_point(
          mac_dl_bs_ind.hol_toa.value(), sl_tp, last_slot_point.load(std::memory_order_relaxed).without_hyper_sfn());
    }
  }

  // Forward DL buffer status.
  sched_impl->handle_dl_buffer_state_indication(bs);
}

void ocudu_scheduler_adapter::handle_ul_phr_indication(const mac_phr_ce_info& phr)
{
  // Forward UL PHR indication to the scheduler.
  ul_phr_indication_message ind{};
  ind.cell_index = phr.cell_index;
  ind.ue_index   = phr.ue_index;
  ind.rnti       = phr.rnti;
  ind.slot_rx    = phr.slot_rx;
  ind.phr        = phr.phr;
  sched_impl->handle_ul_phr_indication(ind);
}

void ocudu_scheduler_adapter::handle_ul_ta_report_indication(const mac_ta_report_ce_info& ta_report)
{
  // Forward the Timing Advance Report to the scheduler.
  ul_ta_report_indication_message ind{};
  ind.cell_index = ta_report.cell_index;
  ind.ue_index   = ta_report.ue_index;
  ind.rnti       = ta_report.rnti;
  ind.slot_rx    = ta_report.slot_rx;
  ind.ul_ta      = ta_report.ul_ta;
  sched_impl->handle_ul_ta_report_indication(ind);
}

void ocudu_scheduler_adapter::handle_cg_confirmation_indication(const mac_cg_confirmation_ce_info& cg_conf)
{
  // Forward the CG Confirmation to the scheduler.
  cg_confirmation_indication_message ind{};
  ind.cell_index = cg_conf.cell_index;
  ind.ue_index   = cg_conf.ue_index;
  ind.rnti       = cg_conf.rnti;
  ind.slot_rx    = cg_conf.slot_rx;
  sched_impl->handle_cg_confirmation_message(ind);
}

void ocudu_scheduler_adapter::handle_crnti_ce_indication(du_ue_index_t old_ue_index, du_cell_index_t cell_index)
{
  rlf_handler.handle_crnti_ce(old_ue_index);
  sched_impl->handle_crnti_ce_received(old_ue_index);
}

sched_slot_result_reader ocudu_scheduler_adapter::slot_indication(slot_point_extended slot_tx, du_cell_index_t cell_idx)
{
  using namespace std::chrono;

  // Mark start of the slot in time (estimate).
  if (last_slot_point.exchange(slot_tx, std::memory_order_relaxed) != slot_tx) {
    auto slot_tp = steady_clock::now();
    last_slot_tp.store(slot_tp, std::memory_order_relaxed);
  }

  const auto res = sched_impl->slot_indication(slot_tx, cell_idx);

  if (res.success()) {
    // Store UCI PDUs for later decoding.
    const auto ul_res = res.ul();
    cell_handlers[cell_idx].uci_decoder.store_uci(
        slot_tx.without_hyper_sfn(), ul_res.pucchs().unsorted(), ul_res.puschs());
  }

  return res;
}

void ocudu_scheduler_adapter::handle_error_indication(slot_point                         slot_tx,
                                                      du_cell_index_t                    cell_idx,
                                                      mac_cell_slot_handler::error_event event)
{
  scheduler_slot_handler::error_outcome sched_err;
  sched_err.pdcch_discarded           = event.pdcch_discarded;
  sched_err.pdsch_discarded           = event.pdsch_discarded;
  sched_err.pusch_and_pucch_discarded = event.pusch_and_pucch_discarded;
  sched_impl->handle_error_indication(slot_tx, cell_idx, sched_err);
}

void ocudu_scheduler_adapter::handle_si_change_indication(const si_scheduling_update_request& request)
{
  sched_impl->handle_si_update_request(request);
}

void ocudu_scheduler_adapter::handle_pws_si_change_indication(const pws_si_scheduling_update_request& request)
{
  sched_impl->handle_pws_si_update_request(request);
}

void ocudu_scheduler_adapter::handle_slice_reconfiguration_request(const du_cell_slice_reconfig_request& req)
{
  // Update RRM policies in the scheduler.
  sched_impl->handle_slice_reconfiguration_request(req);
}

void ocudu_scheduler_adapter::handle_ntn_ul_ta_update(const sched_cell_ntn_ul_ta_update& req)
{
  // Update the reference location uplink timing advance in the scheduler cell configuration.
  sched_impl->handle_ntn_ul_ta_update(req);
}

void ocudu_scheduler_adapter::sched_config_notif_adapter::on_ue_config_complete(du_ue_index_t ue_index,
                                                                                bool          ue_creation_result)
{
  ocudu_sanity_check(is_du_ue_index_valid(ue_index), "Invalid ue index={}", ue_index);

  // Remove continuation of task in ctrl executor.
  defer_until_success(parent.ctrl_exec, parent.timers, [this, ue_index, ue_creation_result]() {
    parent.sched_cfg_notif_map[ue_index].ue_config_ready.set(ue_creation_result);
  });
}

void ocudu_scheduler_adapter::sched_config_notif_adapter::on_ue_deletion_completed(du_ue_index_t ue_index)
{
  ocudu_sanity_check(is_du_ue_index_valid(ue_index), "Invalid ue index={}", ue_index);

  // Continuation of ue remove task dispatched to the ctrl executor.
  defer_until_success(parent.ctrl_exec, parent.timers, [this, ue_index]() {
    parent.sched_cfg_notif_map[ue_index].ue_config_ready.set(true);
  });
}

void ocudu_scheduler_adapter::handle_paging_information(const paging_information& msg)
{
  // Convert MAC paging information to scheduler notification.
  sched_paging_information pg_info{};
  pg_info.paging_drx            = msg.paging_drx;
  pg_info.edrx                  = msg.edrx;
  pg_info.paging_identity       = msg.paging_identity;
  pg_info.paging_type_indicator = msg.paging_type_indicator;
  pg_info.ue_identity           = msg.ue_identity;
  pg_info.paging_cells.resize(msg.paging_cells.size());
  pg_info.paging_cells.assign(msg.paging_cells.begin(), msg.paging_cells.end());

  sched_impl->handle_paging_information(pg_info);
}

ocudu_scheduler_adapter::cell_handler::cell_handler(ocudu_scheduler_adapter&                        parent_,
                                                    const sched_cell_configuration_request_message& sched_cfg) :
  uci_decoder(sched_cfg, parent_.rnti_mng, parent_.rlf_handler),
  pos_handler(parent_.pos_handler->add_cell(sched_cfg.cell_index)),
  cell_idx(sched_cfg.cell_index),
  parent(parent_),
  rach_handler(parent.rach_handler.add_cell(sched_cfg))
{
}

ocudu_scheduler_adapter::cell_handler::~cell_handler()
{
  parent.rach_handler.rem_cell(cell_idx);
}

void ocudu_scheduler_adapter::cell_handler::handle_crc(const mac_crc_indication_message& msg)
{
  // Convert MAC CRC indication to OCUDU scheduler UL CRC indication.
  ul_crc_indication ind{};
  ind.cell_index = cell_idx;
  ind.sl_rx      = msg.sl_rx;
  ind.crcs.resize(msg.crcs.size());
  for (unsigned i = 0, size = msg.crcs.size(); i != size; ++i) {
    const mac_crc_pdu&     mac_pdu = msg.crcs[i];
    ul_crc_pdu_indication& pdu     = ind.crcs[i];
    pdu.rnti                       = mac_pdu.rnti;
    pdu.ue_index                   = parent.rnti_mng[mac_pdu.rnti];
    pdu.harq_id                    = to_harq_id(mac_pdu.harq_id);
    pdu.rapid                      = mac_pdu.rapid;
    pdu.tb_crc_success             = mac_pdu.tb_crc_success;
    pdu.ul_sinr_dB                 = mac_pdu.ul_sinr_dB;
    pdu.ul_rsrp_dBFS               = mac_pdu.ul_rsrp_dBFS;
    pdu.time_advance_offset        = mac_pdu.time_advance_offset;
  }

  // Forward CRC indication to the scheduler.
  parent.sched_impl->handle_crc_indication(ind);

  // Report to RLF handler the CRC result.
  for (const auto& crc : ind.crcs) {
    // If Msg3, ignore the CRC result.
    // Note: UE index is invalid for Msg3 CRCs because no UE has been allocated yet.
    if (crc.ue_index != INVALID_DU_UE_INDEX) {
      parent.rlf_handler.handle_crc(crc.ue_index, cell_idx, crc.tb_crc_success);
    }
  }
}

void ocudu_scheduler_adapter::cell_handler::handle_uci(const mac_uci_indication_message& msg)
{
  // Forward UCI indication to the scheduler.
  parent.sched_impl->handle_uci_indication(uci_decoder.decode_uci(msg));
}

void ocudu_scheduler_adapter::cell_handler::handle_srs(const mac_srs_indication_message& msg)
{
  srs_indication ind;
  ind.cell_index = cell_idx;
  ind.slot_rx    = msg.sl_rx;
  for (const auto& mac_pdu : msg.srss) {
    // Only add PDUs with normalized channel IQ matrix.
    if (const auto* matrix = std::get_if<mac_srs_pdu::normalized_channel_iq_matrix>(&mac_pdu.report)) {
      ind.srss.emplace_back(
          parent.rnti_mng[mac_pdu.rnti], mac_pdu.rnti, mac_pdu.time_advance_offset, matrix->channel_matrix);
    }
  }
  // Forward SRS indication to the scheduler.
  parent.sched_impl->handle_srs_indication(ind);

  // Forward SRS into positioning handler.
  pos_handler->handle_srs_indication(msg);
}
