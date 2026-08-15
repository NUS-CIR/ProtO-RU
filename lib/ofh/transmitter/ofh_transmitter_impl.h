// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_downlink_handler_impl.h"
#include "ofh_message_transmitter_impl.h"
#include "ofh_transmitter_metrics_collector_impl.h"
#include "ofh_transmitter_ota_symbol_task_dispatcher.h"
#include "ofh_uplink_request_handler_impl.h"
#include "ofh_uplink_request_handler_task_dispatcher.h"
#include "ocudu/ofh/ofh_controller.h"
#include "ocudu/ofh/transmitter/ofh_transmitter.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_configuration.h"

namespace ocudu {
namespace ofh {

/// Open Fronthaul transmitter implementation dependencies.
struct transmitter_impl_dependencies {
  /// Log.
  ocudulog::basic_logger* logger = nullptr;
  /// Error notifier.
  error_notifier* err_notifier = nullptr;
  /// Transmitter task executor.
  task_executor* executor = nullptr;
  /// Downlink task executor.
  task_executor* dl_executor = nullptr;
  /// Data flow for downlink Control-Plane.
  std::unique_ptr<data_flow_cplane_scheduling_commands> dl_df_cplane;
  /// Data flow for downlink User-Plane.
  std::unique_ptr<data_flow_uplane_data> dl_df_uplane;
  /// Data flow for uplink Control-Plane scheduling commands.
  std::unique_ptr<data_flow_cplane_scheduling_commands> ul_df_cplane;
  /// Uplink slot context repository.
  std::shared_ptr<rx_grid_context_repository> ul_slot_repo;
  /// Uplink PRACH context repository.
  std::shared_ptr<prach_context_repository> ul_prach_repo;
  /// Notified uplink grid symbol repository.
  std::shared_ptr<uplink_notified_grid_symbol_repository> notifier_symbol_repo;
  /// Ethernet transmitter.
  std::unique_ptr<ether::transmitter> eth_transmitter;
  /// Ethernet frame pool downlink Control-Plane.
  std::shared_ptr<ether::eth_frame_pool> frame_pool_dl_cp;
  /// Ethernet frame pool uplink Control-Plane.
  std::shared_ptr<ether::eth_frame_pool> frame_pool_ul_cp;
  /// Ethernet frame pool downlink User-Plane.
  std::shared_ptr<ether::eth_frame_pool> frame_pool_dl_up;
};

class transmitter_impl : public transmitter, public operation_controller
{
public:
  transmitter_impl(const transmitter_config& config, transmitter_impl_dependencies&& dependencies);

  ~transmitter_impl() override { stop(); }

  // See interface for documentation.
  void start() override;

  // See interface for documentation.
  void stop() override;

  // See interface for documentation.
  operation_controller& get_operation_controller() override { return *this; }

  // See interface for documentation.
  uplink_request_handler& get_uplink_request_handler() override;

  // See interface for documentation.
  downlink_handler& get_downlink_handler() override;

  // See interface for documentation.
  ota_symbol_boundary_notifier& get_ota_symbol_boundary_notifier() override;

  transmitter_metrics_collector* get_metrics_collector() override;

private:
  downlink_handler_impl                  dl_handler;
  uplink_request_handler_impl            ul_request_handler;
  uplink_request_handler_task_dispatcher ul_task_dispatcher;
  message_transmitter_impl               msg_transmitter;
  transmitter_ota_symbol_task_dispatcher ota_dispatcher;
  transmitter_metrics_collector_impl     metrics_collector;
};

} // namespace ofh
} // namespace ocudu
