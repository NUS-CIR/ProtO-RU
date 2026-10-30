// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "mac_controller.h"
#include "../rnti_manager.h"
#include "mac_ue_removal_procedure.h"
#include "proc_logger.h"
#include "ue_creation_procedure.h"
#include "ue_reconfiguration_procedure.h"
#include "ocudu/adt/format.h"
#include "ocudu/mac/mac_clock_controller.h"
#include "ocudu/ran/tdd/tdd_ul_dl_config.h"

using namespace ocudu;

mac_controller::mac_controller(const mac_control_config&   cfg_,
                               mac_ul_configurator&        ul_unit_,
                               mac_dl_configurator&        dl_unit_,
                               rnti_manager&               rnti_table_,
                               mac_scheduler_configurator& sched_cfg_) :
  cfg(cfg_),
  logger(cfg.logger),
  ul_unit(ul_unit_),
  dl_unit(dl_unit_),
  rnti_table(rnti_table_),
  sched_cfg(sched_cfg_),
  time_ctrl(cfg.time_source),
  metrics(cfg.metrics, cfg.ctrl_exec, cfg.time_source.get_timer_manager(), logger)
{
}

mac_cell_controller& mac_controller::add_cell(const mac_cell_creation_request& cell_add_req)
{
  ocudu_assert(not cells.contains(cell_add_req.cell_index),
               "cell={}: Overwriting an existing cell is invalid",
               cell_add_req.cell_index);

  // Add new cell to track timing.
  auto cell_time_source = time_ctrl.add_cell(cell_add_req.cell_index);

  // Add cell to metrics reports.
  unsigned tdd_period_slots = cell_add_req.sched_req.ran.tdd_cfg.has_value()
                                  ? nof_slots_per_tdd_period(*cell_add_req.sched_req.ran.tdd_cfg)
                                  : 0U;

  auto cell_metrics_cfg =
      metrics.add_cell(cell_add_req.cell_index, cell_add_req.scs_common, tdd_period_slots, *cell_time_source);

  // > Fill sched cell configuration message and pass it to the scheduler.
  sched_cfg.add_cell(
      mac_scheduler_cell_creation_request{cell_add_req, cell_metrics_cfg.sched_notifier, cfg.metrics.sched_ue_enabled});

  // > Create MAC Cell DL Handler.
  mac_dl_cell_controller& dl_cell =
      dl_unit.add_cell(cell_add_req,
                       mac_cell_config_dependencies{
                           std::move(cell_time_source), cell_metrics_cfg.report_period, cell_metrics_cfg.mac_notifier});

  // > Create the cell controller, which starts the broadcast of the cell System Information. The cell must already
  // exist in the scheduler at this point, as an SI-message configured for test mode auto-broadcast is activated
  // right away.
  return cells.emplace(cell_add_req.cell_index,
                       cell_add_req,
                       timer_factory{cfg.time_source.get_timer_manager(), cfg.ctrl_exec},
                       cfg.ctrl_exec,
                       dl_cell);
}

void mac_controller::remove_cell(du_cell_index_t cell_index)
{
  ocudu_assert(cells.contains(cell_index), "cell={}: Accessing non-existent cell", cell_index);

  // > Remove cell from MAC Cell Handler.
  dl_unit.remove_cell(cell_index);

  // > Remove cell from scheduler.
  sched_cfg.remove_cell(cell_index);

  // > Remove cell from metrics reports.
  metrics.rem_cell(cell_index);

  cells.erase(cell_index);
}

mac_cell_controller& mac_controller::get_cell_controller(du_cell_index_t cell_index)
{
  ocudu_assert(cells.contains(cell_index), "cell={}: Accessing non-existent cell", cell_index);
  return cells[cell_index];
}

mac_subframe_time_mapper& mac_controller::get_subframe_time_mapper()
{
  return dl_unit.get_subframe_time_mapper();
}

async_task<mac_ue_create_response> mac_controller::handle_ue_create_request(const mac_ue_create_request& msg)
{
  return launch_async<mac_ue_create_request_procedure>(msg, cfg, *this, ul_unit, dl_unit, sched_cfg);
}

async_task<mac_ue_delete_response> mac_controller::handle_ue_delete_request(const mac_ue_delete_request& msg)
{
  return launch_async<mac_ue_removal_procedure>(msg, cfg, *this, ul_unit, dl_unit, sched_cfg);
}

async_task<mac_ue_reconfiguration_response>
mac_controller::handle_ue_reconfiguration_request(const mac_ue_reconfiguration_request& msg)
{
  // Detect whether configured grant (CG) is present in the new configuration.
  bool has_cg_in_request = msg.cs_rnti_requested;

  mac_ue_context* ue_ctx = find_ue(msg.ue_index);
  ocudu_assert(ue_ctx != nullptr, "Requested MAC UE reconfiguration for a UE that hasn't been found");

  const bool            had_cs_rnti = ue_ctx->cs_rnti != rnti_t::INVALID_RNTI;
  std::optional<rnti_t> cs_rnti     = {};

  if (has_cg_in_request and not had_cs_rnti) {
    // CG activation: allocate a new CS-RNTI.
    rnti_t new_cs_rnti = rnti_table.allocate(/* is_cs_rnti */ true);
    if (new_cs_rnti == rnti_t::INVALID_RNTI) {
      logger.warning("{}: Failed to allocate CS-RNTI for configured grant",
                     mac_log_prefix(msg.ue_index, msg.crnti, "MAC UE Reconfiguration"));
    } else {
      if (not rnti_table.add_ue(new_cs_rnti, msg.ue_index, /* is_cs_rnti */ true)) {
        logger.warning("{}: CS-RNTI={} collided in RNTI table",
                       mac_log_prefix(msg.ue_index, msg.crnti, "MAC UE Reconfiguration"),
                       new_cs_rnti);
      } else {
        cs_rnti         = new_cs_rnti;
        ue_ctx->cs_rnti = new_cs_rnti;
        logger.debug("{}: Allocated CS-RNTI={} for configured grant",
                     mac_log_prefix(msg.ue_index, msg.crnti, "MAC UE Reconfiguration"),
                     new_cs_rnti);
      }
    }
  } else if (has_cg_in_request and had_cs_rnti) {
    // CG kept: report the CS-RNTI the UE already holds, so that the DU keeps it in the UE configuration.
    cs_rnti = ue_ctx->cs_rnti;
  } else if (not has_cg_in_request and had_cs_rnti) {
    // CG deactivation: free the CS-RNTI.
    logger.debug("{}: Deallocating CS-RNTI={} (configured grant deactivated)",
                 mac_log_prefix(msg.ue_index, msg.crnti, "MAC UE Reconfiguration"),
                 ue_ctx->cs_rnti);
    rnti_table.rem_ue(ue_ctx->cs_rnti, /* is_cs_rnti */ true);
    ue_ctx->cs_rnti = rnti_t::INVALID_RNTI;
  }

  if (cs_rnti.has_value()) {
    // Patch the CS-RNTI into the request copy before forwarding to the scheduler.
    mac_ue_reconfiguration_request patched_msg = msg;
    patched_msg.assigned_cs_rnti               = cs_rnti;
    if (patched_msg.phy_cell_group_cfg.has_value()) {
      patched_msg.phy_cell_group_cfg->cs_rnti = cs_rnti.value();
    }
    return launch_async<mac_ue_reconfiguration_procedure>(patched_msg, cfg, ul_unit, dl_unit, sched_cfg, cs_rnti);
  }

  return launch_async<mac_ue_reconfiguration_procedure>(msg, cfg, ul_unit, dl_unit, sched_cfg);
}

void mac_controller::handle_ue_config_applied(du_ue_index_t ue_index)
{
  ocudu_assert(ue_db.contains(ue_index), "Invalid ue_index={}", ue_index);

  ul_unit.handle_ue_config_applied(ue_index);

  sched_cfg.handle_ue_config_applied(ue_index);
}

rnti_t mac_controller::add_ue(du_ue_index_t ue_index, du_cell_index_t cell_index, rnti_t tc_rnti)
{
  ocudu_assert(is_du_ue_index_valid(ue_index), "Invalid ue_index={}", ue_index);

  if (ue_db.contains(ue_index)) {
    // UE already existed with same ue_index.
    return rnti_t::INVALID_RNTI;
  }

  // Create UE object.
  ue_db.emplace(ue_index);
  mac_ue_context& u = ue_db[ue_index];
  u.du_ue_index     = ue_index;
  u.pcell_idx       = cell_index;

  if (tc_rnti != rnti_t::INVALID_RNTI) {
    // We reuse the TC-RNTI as the C-RNTI, if it was already chosen.
    u.rnti = tc_rnti;
  } else {
    // Otherwise, we generate a new C-RNTI.
    u.rnti = rnti_table.allocate();
  }

  // Update RNTI -> UE index map.
  if (not rnti_table.add_ue(u.rnti, ue_index)) {
    logger.error("ue={} rnti={}: The update of the RNTI table overwrote a previous entry", ue_index, u.rnti);
    rnti_table.rem_ue(u.rnti);
    return rnti_t::INVALID_RNTI;
  }
  return u.rnti;
}

void mac_controller::remove_ue(du_ue_index_t ue_index)
{
  if (not ue_db.contains(ue_index)) {
    logger.warning("Failed to find ue_index={:#x}", fmt::underlying(ue_index));
    return;
  }

  // Remove C-RNTI from the RNTI table.
  rnti_table.rem_ue(ue_db[ue_index].rnti);

  // Remove CS-RNTI from the RNTI table, if allocated for configured grants.
  if (ue_db[ue_index].cs_rnti != rnti_t::INVALID_RNTI) {
    rnti_table.rem_ue(ue_db[ue_index].cs_rnti, /* is_cs_rnti */ true);
  }

  ue_db.erase(ue_index);
}

mac_ue_context* mac_controller::find_ue(du_ue_index_t ue_index)
{
  return ue_db.contains(ue_index) ? &ue_db[ue_index] : nullptr;
}

mac_ue_context* mac_controller::find_by_rnti(rnti_t rnti)
{
  return find_ue(rnti_table[rnti]);
}
