// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cell_group_event_manager.h"
#include "../cell/resource_grid.h"
#include "../common_scheduling/ra_ue_repository.h"
#include "../config/sched_config_manager.h"
#include "../srs/srs_scheduler.h"
#include "../uci_scheduling/uci_indication_selector.h"
#include "../uci_scheduling/uci_scheduler_impl.h"
#include "../ue_context/ue_cell_repository.h"
#include "ocudu/scheduler/scheduler_feedback_handler.h"
#include "fmt/chrono.h"
#include <memory>

using namespace ocudu;

cell_group_event_handler_impl::cell_group_event_handler_impl(cell_group_event_manager&  parent_,
                                                             const cell_creation_event& cell_ev,
                                                             ue_repository&             ue_db_,
                                                             ocudulog::basic_logger&    logger_) :
  parent(parent_),
  ue_db(ue_db_),
  logger(logger_),
  cfg(cell_ev.cell_res_grid.cfg),
  fallback_sched(cell_ev.fallback_sched),
  uci_sched(cell_ev.uci_sched),
  slice_sched(cell_ev.slice_sched),
  srs_sched(cell_ev.srs_sched),
  cg_sched(cell_ev.cg_sched),
  ra_ue_repo(cell_ev.ra_ue_repo)
{
}

cell_group_event_handler_impl::~cell_group_event_handler_impl()
{
  // Deregister cell from cell_group_event_manager.
  parent.cells[cfg.cell_index] = nullptr;
}

bool cell_group_event_handler_impl::handle_ue_creation(ue_config_update_event ev)
{
  const du_ue_index_t ue_index = ev.get_ue_index();
  const rnti_t        crnti    = ev.next_config().crnti;
  if (ue_db.contains(ue_index)) {
    logger.error(
        "ue={} rnti={}: Discarding UE creation. Cause: A UE with the same index already exists", ue_index, crnti);
    return false;
  }

  // Check if this UE was created via RACH and is still tracked by the RA scheduler, so its PRACH reception slot
  // can be carried over into the UE's PCell context. A 2-step RACH successRAR completion already resolved
  // contention (TS38.321 6.2.3a), so no MAC ConRes CE is needed.
  auto                      ra_it = ra_ue_repo.find(crnti);
  std::optional<slot_point> prach_slot_rx =
      ra_it != ra_ue_repo.end() ? std::optional<slot_point>(ra_it->prach_slot_rx) : std::nullopt;

  bool             is_in_fallback = ev.get_fallback_command().has_value() and ev.get_fallback_command().value();
  ue_creation_mode creation_mode  = ue_creation_mode::skip_fallback;
  if (is_in_fallback) {
    if (ev.get_ul_ccch_slot_rx().has_value()) {
      // RACH-created UE. A 2-step RACH successRAR completion already resolved contention (TS38.321 6.2.3a), so
      // no MAC ConRes CE is needed; any other RACH path (native 4-step, or 2-step fallback) still needs one.
      creation_mode = ra_it != ra_ue_repo.end() and ra_it->is_msgb_success_rar()
                          ? ue_creation_mode::two_step_success_rar
                          : ue_creation_mode::msg3_rach;
    } else if (ev.get_cfra_enabled()) {
      creation_mode = ue_creation_mode::cfra;
    } else {
      creation_mode = ue_creation_mode::high_layers;
    }
  }

  // Insert UE in UE repository.
  ue_db.add_ue(ev.next_config(), {creation_mode, ev.get_ul_ccch_slot_rx(), prach_slot_rx});

  auto& u     = ue_db[ue_index];
  auto& ue_cc = u.get_pcell();
  if (ue_cc.get_pcell_state().conres_st != ue_conres_state::pending_conres_crnti_ce) {
    // Defer UCI/SR scheduling only for UEs awaiting a C-RNTI MAC CE.
    uci_sched.add_ue(ue_cc.cfg());
    srs_sched.add_ue(ue_cc.cfg());
  }

  // Add UE to slice scheduler.
  // Note: This action only has effect when UE is created in non-fallback mode.
  slice_sched.add_ue(ue_index);

  if (ue_cc.get_pcell_state().conres_st == ue_conres_state::pending_conres_ce) {
    // Note: In case of RACH-created UE, auto-inject MAC ConRes CE.

    // Forward CE to ue instance.
    u.handle_dl_mac_ce_indication(dl_mac_ce_indication{ue_index, lcid_dl_sch_t::UE_CON_RES_ID});

    // Notify fallback scheduler of a pending ConRes CE.
    fallback_sched.handle_conres_indication(ue_index);
  }

  // Notify config manager that creation is complete with success.
  ev.notify_completion();

  return true;
}

bool cell_group_event_handler_impl::handle_ue_reconfiguration(ue_config_update_event ev)
{
  const du_ue_index_t ue_idx = ev.get_ue_index();
  if (not ue_db.contains(ue_idx)) {
    log_invalid_ue_index(ue_idx, "ue_reconf");
    return false;
  }
  auto& u = ue_db[ue_idx];

  // Reconfigure PCell
  // Note: Carrier aggregation not yet supported.
  auto& ue_cc = u.get_cell(SERVING_PCELL_IDX);

  if (ue_cc.get_pcell_state().conres_st != ue_conres_state::pending_conres_crnti_ce) {
    uci_sched.reconf_ue(ev.next_config().ue_cell_cfg(ue_cc.cell_index), ue_cc.cfg());
    srs_sched.reconf_ue(ev.next_config().ue_cell_cfg(ue_cc.cell_index), ue_cc.cfg());
    if (cg_sched != nullptr) {
      cg_sched->add_reconf_ue(ev.next_config().ue_cell_cfg(ue_cc.cell_index), &ue_cc.cfg());
    }
  }

  // Configure existing UE.
  ue_db.reconfigure_ue(ev.next_config(), ev.get_cause());

  // Update slice scheduler.
  slice_sched.reconf_ue(u.ue_index);

  // Notify config manager that reconfiguration is complete with success.
  ev.notify_completion();

  return true;
}

bool cell_group_event_handler_impl::handle_ue_deletion(ue_config_delete_event ev)
{
  const du_ue_index_t ue_idx = ev.ue_index();
  if (not ue_db.contains(ue_idx)) {
    log_invalid_ue_index(ue_idx, "ue_rem");
    return false;
  }
  const auto& u     = ue_db[ue_idx];
  const auto& ue_cc = u.get_pcell();
  if (ue_cc.get_pcell_state().conres_st != ue_conres_state::pending_conres_crnti_ce) {
    // A UE awaiting a C-RNTI CE was not added to UCI/SRS scheduling yet (deferred until the CE is received), so
    // it must not be removed either. All other UEs (including CFRA) were added at creation.
    uci_sched.rem_ue(u.get_pcell().cfg());
    srs_sched.rem_ue(u.get_pcell().cfg());
    if (cg_sched != nullptr) {
      cg_sched->rem_ue(u.get_pcell().cfg());
    }
  }
  // Schedule removal of UE from slice scheduler.
  slice_sched.rem_ue(ue_idx);

  // Schedule UE removal from repository.
  ue_db.schedule_ue_rem(std::move(ev));

  return true;
}

/// \brief Signals a UE the Differential Koffset derived from its reported T_TA, as per TS 38.321, Section 5.18.24.
///
/// Runs on each T_TA report, and on the configuration that declares the UE accepts the MAC CE.
///
/// Nothing is signalled while a value is in flight, since the scheduler cannot tell which one the UE applies.
static void update_differential_koffset(ue& u, ocudulog::basic_logger& logger)
{
  const cell_configuration&              cell_cfg       = u.get_pcell().cfg().cell_cfg_common;
  const std::optional<ue_koffset_config> ue_koffset_cfg = get_ue_koffset_cfg(cell_cfg.params.ntn_params);
  if (not ue_koffset_cfg.has_value()) {
    return;
  }

  const ue_configuration* ue_cfg = u.ue_cfg_dedicated();
  if (ue_cfg == nullptr or not ue_cfg->supports_ue_specific_koffset()) {
    logger.debug(
        "ue={} rnti={}: No Differential Koffset derived. Cause: UE does not accept the MAC CE", u.ue_index, u.crnti);
    return;
  }

  const std::optional<std::chrono::microseconds> reported_ul_ta = u.ta_report_tracker().last_reported_ul_ta();
  if (not reported_ul_ta.has_value()) {
    logger.debug(
        "ue={} rnti={}: No Differential Koffset derived. Cause: UE has not reported its T_TA", u.ue_index, u.crnti);
    return;
  }

  ue_koffset_tracker& tracker = u.koffset_tracker();
  if (tracker.has_pending_ce()) {
    logger.debug("ue={} rnti={}: No Differential Koffset derived. Cause: an earlier value has not reached the UE",
                 u.ue_index,
                 u.crnti);
    return;
  }

  const std::chrono::milliseconds cell_koffset{cell_cfg.ntn_cs_koffset /
                                               get_nof_slots_per_subframe(cell_cfg.scs_common())};
  const std::chrono::milliseconds derived =
      derive_differential_koffset(cell_koffset, *reported_ul_ta, ue_koffset_cfg->margin);
  if (derived == tracker.applied_koffset()) {
    logger.debug("ue={} rnti={}: Keeping Differential Koffset={}ms. Cause: the derived value has not changed",
                 u.ue_index,
                 u.crnti,
                 tracker.applied_koffset().count());
    return;
  }

  logger.info("ue={} rnti={}: Signalling Differential Koffset={}ms, derived from T_TA={}us",
              u.ue_index,
              u.crnti,
              derived.count(),
              reported_ul_ta->count());
  u.logical_channels().handle_mac_ce_indication(
      {lcid_dl_sch_t::from_elcid(elcid_dl_sch_t::DIFFERENTIAL_KOFFSET), differential_koffset_ce_payload{derived}});
}

bool cell_group_event_handler_impl::handle_ue_config_applied(du_ue_index_t ue_idx)
{
  if (not ue_db.contains(ue_idx)) {
    log_invalid_ue_index(ue_idx, "ue_cfg_applied");
    return false;
  }

  // Confirm that UE applied new config.
  ue_db.ue_config_applied(ue_idx);

  // The applied configuration declares whether the UE accepts a Differential Koffset.
  update_differential_koffset(ue_db[ue_idx], logger);

  // Notify slice scheduler only when the UE fully exits fallback (conres also done).
  if (not ue_db[ue_idx].get_pcell().is_in_fallback_mode()) {
    slice_sched.config_applied(ue_idx);
    if (cg_sched != nullptr) {
      cg_sched->start_ue_cg_activation(ue_idx);
    }
  }

  return true;
}

bool cell_group_event_handler_impl::handle_ue_deactivation_request(du_ue_index_t ue_idx)
{
  if (not ue_db.contains(ue_idx)) {
    log_invalid_ue_index(ue_idx, "ue_deactivation");
    return false;
  }
  auto& u = ue_db[ue_idx];

  // Deactivate the UE (no more grants after this point).
  u.deactivate();

  // Schedule removal of UE from slice scheduler so it doesn't get scheduled PDSCH/PUSCH.
  slice_sched.rem_ue(ue_idx);

  return true;
}

bool cell_group_event_handler_impl::handle_ul_bsr_indication(const ul_bsr_indication_message& bsr_ind)
{
  if (not ue_db.contains(bsr_ind.ue_index)) {
    log_invalid_ue_index(bsr_ind.ue_index, "BSR");
    return false;
  }
  auto& u = ue_db[bsr_ind.ue_index];

  // Handle event.
  u.handle_bsr_indication(bsr_ind);

  if (u.get_pcell().is_in_fallback_mode()) {
    // Signal SRB fallback scheduler with the new SRB0/SRB1 buffer state.
    fallback_sched.handle_ul_bsr_indication(bsr_ind.ue_index, bsr_ind);
  }

  return true;
}

void cell_group_event_handler_impl::handle_conres_ce_acked(du_ue_index_t ue_index)
{
  if (not ue_db.contains(ue_index)) {
    log_invalid_ue_index(ue_index, "ConRes CE ACKed");
    return;
  }
  const bool was_in_fallback = ue_db[ue_index].get_pcell().is_in_fallback_mode();
  ue_db.handle_conres_ce_outcome(ue_index, true);

  // The UE exits fallback once both the ConRes CE is ACKed and its config is applied, in any order.
  if (was_in_fallback and not ue_db[ue_index].get_pcell().is_in_fallback_mode()) {
    slice_sched.config_applied(ue_index);
  }
}

void cell_group_event_handler_impl::handle_sr_detected(du_ue_index_t ue_index, slot_point uci_slot)
{
  // Note: Not warned about when the UE is gone, for the same reason as the UCI indication it comes from.
  if (not ue_db.contains(ue_index)) {
    log_invalid_ue_index(ue_index, "SR", false);
    return;
  }
  ue&            u     = ue_db[ue_index];
  const ue_cell* ue_cc = u.find_cell(cfg.cell_index);
  if (ue_cc == nullptr) {
    log_invalid_cc(ue_index, "SR", false);
    return;
  }

  u.handle_sr_indication(uci_slot);
  if (ue_cc->is_in_fallback_mode()) {
    fallback_sched.handle_sr_indication(ue_index);
  }
}

void cell_group_event_handler_impl::handle_ul_n_ta_update(du_ue_index_t              ue_index,
                                                          time_alignment_group::id_t tag_id,
                                                          phy_time_unit              n_ta_diff,
                                                          float                      ul_sinr)
{
  if (not ue_db.contains(ue_index)) {
    log_invalid_ue_index(ue_index, "N_TA update");
    return;
  }
  ue_db[ue_index].handle_ul_n_ta_update_indication(tag_id, n_ta_diff, ul_sinr);
}

void cell_group_event_handler_impl::handle_cfra_msg3_acked(du_ue_index_t ue_index)
{
  if (not ue_db.contains(ue_index)) {
    log_invalid_ue_index(ue_index, "CFRA Msg3 ACKed");
    return;
  }
  const ue_cell* ue_cc = ue_db[ue_index].find_cell(cfg.cell_index);
  if (ue_cc == nullptr) {
    log_invalid_cc(ue_index, "CFRA Msg3 ACKed");
    return;
  }

  if (ue_db.cfra_msg3_acked(ue_index) and not ue_cc->is_in_fallback_mode()) {
    // CFRA Msg3 ACKed. UE is directly added to slice scheduling. It doesn't need to be in fallback mode.
    slice_sched.config_applied(ue_index);
  }
}

bool cell_group_event_handler_impl::handle_ul_phr_indication(const ul_phr_indication_message& phr_ind)
{
  // Fetch UE objects.
  if (not ue_db.contains(phr_ind.ue_index)) {
    log_invalid_ue_index(phr_ind.ue_index, "PHR");
    return false;
  }
  auto& u = ue_db[phr_ind.ue_index];

  for (const cell_ph_report& cell_phr : phr_ind.phr.get_phr()) {
    ocudu_sanity_check(cell_phr.serv_cell_id < u.nof_cells(), "Invalid serving cell index={}", cell_phr.serv_cell_id);
    auto& ue_cc = u.get_cell(cell_phr.serv_cell_id);

    ue_cc.get_pusch_power_controller().handle_phr(cell_phr, phr_ind.slot_rx, phr_ind.rnti);
  }

  return true;
}

bool cell_group_event_handler_impl::handle_ul_ta_report_indication(const ul_ta_report_indication_message& ta_report)
{
  if (not ue_db.contains(ta_report.ue_index)) {
    log_invalid_ue_index(ta_report.ue_index, "TA report");
    return false;
  }

  ue& u = ue_db[ta_report.ue_index];
  u.ta_report_tracker().handle_ta_report(ta_report.ul_ta);
  update_differential_koffset(u, logger);

  return true;
}

bool cell_group_event_handler_impl::handle_dl_mac_ce_indication(const dl_mac_ce_indication& ce)
{
  if (not ue_db.contains(ce.ue_index)) {
    log_invalid_ue_index(ce.ue_index, "DL MAC CE");
    return false;
  }
  // Forward CE to UE instance.
  ue_db[ce.ue_index].handle_dl_mac_ce_indication(ce);

  return true;
}

bool cell_group_event_handler_impl::handle_crnti_ce_received(du_ue_index_t ue_index)
{
  if (not ue_db.contains(ue_index)) {
    log_invalid_ue_index(ue_index, "C-RNTI CE received");
    return false;
  }
  auto& u     = ue_db[ue_index];
  auto& ue_cc = u.get_pcell();
  if (ue_cc.cell_index != cfg.cell_index) {
    logger.warning("cell={} ue={} rnti={}: Discarding C-RNTI CE. It was received in cell that is not PCell ({})",
                   cfg.cell_index,
                   ue_index,
                   u.crnti,
                   ue_cc.cell_index);
    return false;
  }

  const bool was_pending_cfra = ue_cc.get_pcell_state().conres_st == ue_conres_state::pending_cfra;
  if (ue_db.crnti_ce_received(ue_index)) {
    // C-RNTI CE received: contention resolution completed for this F1AP-created UE.

    // Initiate UCI and SRS schedulers with the confirmed UE resources. CFRA UEs already have them from creation.
    if (not was_pending_cfra) {
      uci_sched.add_ue(ue_cc.cfg());
      srs_sched.add_ue(ue_cc.cfg());
    }

    if (cg_sched != nullptr and not was_pending_cfra) {
      cg_sched->add_reconf_ue(ue_cc.cfg(), nullptr);
    }

    // Notify slice scheduler only when the UE fully exits fallback (config also applied).
    if (not ue_cc.is_in_fallback_mode()) {
      slice_sched.config_applied(ue_index);
      if (cg_sched != nullptr) {
        // A CG Type 2 UE is queued for activation by the configuration-applied indication, which is skipped while the
        // UE is still in fallback. When the configuration was applied before the C-RNTI CE arrived, this is the only
        // chance to catch that up: nothing would send the indication a second time, and the UE would be left with a
        // CG that is never activated. No-op for CG Type 1, which needs no activation.
        cg_sched->start_ue_cg_activation(ue_index);
      }
    }
  }

  return true;
}

bool cell_group_event_handler_impl::handle_dl_buffer_state_indication(const dl_buffer_state_indication_message& dl_bo)
{
  if (not ue_db.contains(dl_bo.ue_index)) {
    log_invalid_ue_index(dl_bo.ue_index, "DL buffer occupancy update");
    return false;
  }
  ue& u = ue_db[dl_bo.ue_index];

  // Forward DL BO update to UE.
  u.handle_dl_buffer_state_indication(dl_bo.lcid, dl_bo.bs, dl_bo.hol_toa);
  if (u.get_pcell().is_in_fallback_mode()) {
    // Signal SRB fallback scheduler with the new SRB0/SRB1 buffer state.
    fallback_sched.handle_dl_buffer_state_indication(dl_bo.ue_index);
  }

  return true;
}

void cell_group_event_handler_impl::handle_slice_reconfiguration(const du_cell_slice_reconfig_request& req)
{
  slice_sched.handle_slice_reconfiguration_request(req);
}

void cell_group_event_handler_impl::handle_cg_activation_complete_indication(du_ue_index_t ue_index)
{
  // This notification can come either from a MAC-CE Configured Grant Confirmation, or from the reception of a non-DTX
  // CRC.

  if (cg_sched == nullptr) {
    // CG Confirmation received in a cell with no configured grants configured.
    logger.warning("ue={}: Discarding CG confirmation. Cause: No CG configured in this cell", ue_index);
    return;
  }

  // The CG confirmation is decoded in the MAC and only reaches the scheduler at the next slot boundary, by which time
  // the UE may already have been removed. The MAC drains the pending UL PDUs of a UE before removing it, so the index
  // cannot have been reassigned to another UE by now, but it can be stale.
  if (not ue_db.contains(ue_index)) {
    log_invalid_ue_index(ue_index, "CG confirmation");
    return;
  }

  cg_sched->handle_cg_activation_complete_indication(ue_index);
}

void cell_group_event_handler_impl::handle_error_indication(slot_point                            sl_tx,
                                                            scheduler_slot_handler::error_outcome event)
{
  if (cg_sched == nullptr) {
    // No configured grants in this cell. Unlike the CG confirmation, error indications are a normal occurrence in any
    // cell, so this is not worth logging.
    return;
  }
  cg_sched->handle_error_indication(sl_tx, event);
}

void cell_group_event_handler_impl::log_invalid_ue_index(du_ue_index_t ue_index,
                                                         const char*   event_name,
                                                         bool          warn_if_ignored) const
{
  ocudulog::log_channel& log_channel = warn_if_ignored ? logger.warning : logger.info;
  log_channel("cell={} ue={}: Discarding {} event. Cause: UE with provided Id does not exist",
              cfg.cell_index,
              ue_index,
              event_name);
}

void cell_group_event_handler_impl::log_invalid_cc(du_ue_index_t ue_idx,
                                                   const char*   event_name,
                                                   bool          warn_if_ignored) const
{
  ocudulog::log_channel& log_channel = warn_if_ignored ? logger.warning : logger.info;
  log_channel("cell={} ue={}: Discarding {} event. Cause: UE is not configured in this cell",
              cfg.cell_index,
              ue_idx,
              event_name);
}

cell_group_event_manager::cell_group_event_manager(ue_repository& ue_db_) :
  ue_db(ue_db_), logger(ocudulog::fetch_basic_logger("SCHED"))
{
  std::fill(cells.begin(), cells.end(), nullptr);
}

std::unique_ptr<cell_group_event_handler_impl> cell_group_event_manager::add_cell(const cell_creation_event& cell_ev)
{
  const du_cell_index_t cell_index = cell_ev.cell_res_grid.cell_index();
  ocudu_assert(not cell_exists(cell_index), "Overwriting cell configurations not supported");

  // Create cell_group_event_handler_impl.
  auto cell = std::make_unique<cell_group_event_handler_impl>(*this, cell_ev, ue_db, logger);

  // Register cell.
  cells[cell_index] = cell.get();

  return cell;
}

bool cell_group_event_manager::cell_exists(du_cell_index_t cell_index) const
{
  return cell_index < MAX_NOF_DU_CELLS and cells[cell_index] != nullptr;
}
