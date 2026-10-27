// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "split6_o_du_factory.h"
#include "apps/helpers/e2/e2_metric_connector_manager.h"
#include "apps/services/worker_manager/worker_manager.h"
#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_config.h"
#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_factory.h"
#include "split6_o_du_impl.h"
#include "split6_o_du_unit_config.h"
#include "ocudu/e2/e2_du_metrics_connector.h"
#include "ocudu/fapi_adaptor/phy/p5/phy_fapi_p5_sector_adaptor.h"
#include "ocudu/fapi_adaptor/phy/p7/phy_fapi_p7_sector_adaptor.h"
#include "ocudu/mac/mac_clock_controller.h"
#include "ocudu/support/ocudu_assert.h"

using namespace ocudu;

o_du_unit ocudu::create_o_du_split6(const split6_o_du_unit_config&                  du_unit_cfg,
                                    const o_du_unit_dependencies&                   du_dependencies,
                                    std::unique_ptr<fapi_adaptor::phy_fapi_adaptor> fapi_adaptor)
{
  ocudu_assert(du_dependencies.workers != nullptr, "Invalid workers");

  o_du_unit odu_unit;
  odu_unit.e2_metric_connectors = std::make_unique<
      e2_metric_connector_manager<e2_du_metrics_connector, e2_du_metrics_notifier, e2_du_metrics_interface>>(
      du_unit_cfg.odu_high_cfg.du_high_cfg.config.cells_cfg.size());

  o_du_high_unit_dependencies odu_hi_unit_dependencies = {du_dependencies.workers->get_du_high_executor_mapper(),
                                                          *du_dependencies.f1c_client_handler,
                                                          *du_dependencies.f1u_teid_allocator,
                                                          *du_dependencies.f1u_gw,
                                                          *du_dependencies.timer_ctrl,
                                                          *du_dependencies.mac_p,
                                                          *du_dependencies.rlc_p,
                                                          *du_dependencies.e2_client_handler,
                                                          *(odu_unit.e2_metric_connectors),
                                                          *du_dependencies.metrics_notifier,
                                                          du_dependencies.remote_metrics_gateway,
                                                          {}};

  // Adjust the dependencies.
  const auto& du_hi_unit_cfg = du_unit_cfg.odu_high_cfg.du_high_cfg.config;
  for (unsigned i = 0, e = du_hi_unit_cfg.cells_cfg.size(); i != e; ++i) {
    auto&                              p5_sector_adaptor = fapi_adaptor->get_sector_adaptor(i).get_p5_sector_adaptor();
    auto&                              p7_sector_adaptor = fapi_adaptor->get_sector_adaptor(i).get_p7_sector_adaptor();
    odu::o_du_high_sector_dependencies sector_dependencies = {
        .p5_gateway           = p5_sector_adaptor.get_p5_requests_gateway(),
        .p7_gateway           = p7_sector_adaptor.get_p7_requests_gateway(),
        .p7_last_req_notifier = p7_sector_adaptor.get_p7_last_request_notifier(),
        .timer_mng            = du_dependencies.timer_ctrl->get_timer_manager(),
        .fapi_ctrl_executor   = du_dependencies.workers->get_cmd_line_executor(),
        // :TODO: create executor mapper for split 6 and pass correct executor here
        .mac_ctrl_executor = du_dependencies.workers->get_du_high_executor_mapper().du_control_executor(),
        .fapi_logger       = *du_dependencies.fapi_logger,
    };

    odu_hi_unit_dependencies.o_du_hi_dependencies.sectors.push_back(sector_dependencies);
  }

  o_du_high_unit odu_hi_unit = make_o_du_high_unit(du_unit_cfg.odu_high_cfg, std::move(odu_hi_unit_dependencies));

  odu_unit.commands = std::move(odu_hi_unit.commands);
  odu_unit.metrics  = std::move(odu_hi_unit.metrics);

  // Create the DU.
  odu_unit.unit = std::make_unique<split6_o_du_impl>(
      du_hi_unit_cfg.cells_cfg.size(), std::move(fapi_adaptor), std::move(odu_hi_unit.o_du_hi));
  report_error_if_not(odu_unit.unit, "Invalid Distributed Unit");

  announce_du_high_cells(du_hi_unit_cfg);

  return odu_unit;
}
