// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "fapi_dummy_o_du_factory.h"
#include "apps/helpers/e2/e2_metric_connector_manager.h"
#include "apps/services/worker_manager/worker_manager.h"
#include "apps/units/flexible_o_du/flexible_o_du_commands.h"
#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_config.h"
#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_factory.h"
#include "apps/units/flexible_o_du/split_helpers/commands/ntn_config_update_remote_command.h"
#include "fapi_dummy_o_du_impl.h"
#include "ocudu/du/du_high/du_high.h"
#include "ocudu/e2/e2_du_metrics_connector.h"
#include "ocudu/fapi_adaptor/phy/p5/phy_fapi_p5_sector_adaptor.h"
#include "ocudu/fapi_adaptor/phy/p7/phy_fapi_p7_sector_adaptor.h"
#include "ocudu/fapi_adaptor/phy_dummy/phy_dummy_fapi_adaptor_factory.h"
#include "ocudu/mac/mac_clock_controller.h"
#include "ocudu/ntn/ntn_configuration_manager.h"

using namespace ocudu;

o_du_unit ocudu::create_fapi_dummy_o_du(const o_du_high_unit_config&  odu_high_cfg,
                                        const fapi_dummy_unit_config& fapi_dummy_cfg,
                                        const o_du_unit_dependencies& dependencies)
{
  const du_high_unit_config& du_hi     = odu_high_cfg.du_high_cfg.config;
  const unsigned             nof_cells = du_hi.cells_cfg.size();

  o_du_unit odu_unit;
  odu_unit.e2_metric_connectors = std::make_unique<
      e2_metric_connector_manager<e2_du_metrics_connector, e2_du_metrics_notifier, e2_du_metrics_interface>>(nof_cells);

  std::unique_ptr<fapi_adaptor::phy_fapi_adaptor> fapi_dummy = fapi_adaptor::create_phy_dummy_fapi_adaptor(
      {.scs                        = du_hi.cells_cfg.front().cell.common_scs,
       .nof_sectors                = nof_cells,
       .clock_mode                 = fapi_dummy_cfg.clock_mode,
       .ul_indication_delay_slots  = fapi_dummy_cfg.ul_indication_delay_slots,
       .max_processing_delay_slots = fapi_dummy_cfg.max_processing_delay_slots},
      {.logger           = *dependencies.fapi_logger,
       .clock_executor   = dependencies.workers->get_fapi_dummy_clock_executor(),
       .sector_executors = dependencies.workers->get_fapi_dummy_sector_executors()});

  o_du_high_unit_dependencies odu_hi_unit_dependencies = {dependencies.workers->get_du_high_executor_mapper(),
                                                          *dependencies.f1c_client_handler,
                                                          *dependencies.f1u_teid_allocator,
                                                          *dependencies.f1u_gw,
                                                          *dependencies.timer_ctrl,
                                                          *dependencies.mac_p,
                                                          *dependencies.rlc_p,
                                                          *dependencies.e2_client_handler,
                                                          *(odu_unit.e2_metric_connectors),
                                                          *dependencies.metrics_notifier,
                                                          dependencies.remote_metrics_gateway,
                                                          {}};

  for (unsigned i = 0; i != nof_cells; ++i) {
    auto&                              p5_sector_adaptor   = fapi_dummy->get_sector_adaptor(i).get_p5_sector_adaptor();
    auto&                              p7_sector_adaptor   = fapi_dummy->get_sector_adaptor(i).get_p7_sector_adaptor();
    odu::o_du_high_sector_dependencies sector_dependencies = {
        .p5_gateway           = p5_sector_adaptor.get_p5_requests_gateway(),
        .p7_gateway           = p7_sector_adaptor.get_p7_requests_gateway(),
        .p7_last_req_notifier = p7_sector_adaptor.get_p7_last_request_notifier(),
        .timer_mng            = dependencies.timer_ctrl->get_timer_manager(),
        .fapi_ctrl_executor   = dependencies.workers->get_cmd_line_executor(),
        .mac_ctrl_executor    = dependencies.workers->get_du_high_executor_mapper().du_control_executor(),
        .fapi_logger          = *dependencies.fapi_logger,
    };
    odu_hi_unit_dependencies.o_du_hi_dependencies.sectors.push_back(sector_dependencies);
  }

  o_du_high_unit odu_hi_unit = make_o_du_high_unit(odu_high_cfg, std::move(odu_hi_unit_dependencies));

  odu_unit.commands = std::move(odu_hi_unit.commands);
  odu_unit.metrics  = std::move(odu_hi_unit.metrics);
  odu_unit.commands.cmdline.commands.push_back(std::make_unique<change_log_level_app_command>());

  // The DU-high creates the NTN configuration manager only when at least one NTN cell is configured.
  if (auto* ntn_manager = odu_hi_unit.o_du_hi->get_du_high().get_ntn_configuration_manager(); ntn_manager != nullptr) {
    odu_unit.commands.remote.push_back(std::make_unique<ocudu_ntn::ntn_config_update_remote_command>(*ntn_manager));
  }

  odu_unit.unit =
      std::make_unique<fapi_dummy_o_du_impl>(nof_cells, std::move(fapi_dummy), std::move(odu_hi_unit.o_du_hi));

  announce_du_high_cells(du_hi);

  return odu_unit;
}
