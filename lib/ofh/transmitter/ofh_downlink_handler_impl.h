// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ofh_data_flow_cplane_scheduling_commands.h"
#include "ofh_data_flow_uplane_data.h"
#include "ofh_downlink_handler_metrics_collector.h"
#include "ofh_tx_window_checker.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ofh/ethernet/ethernet_frame_pool.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/ofh_controller.h"
#include "ocudu/ofh/transmitter/ofh_downlink_handler.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_configuration.h"
#include "ocudu/ran/tdd/tdd_ul_dl_config.h"
#include "ocudu/support/synchronization/stop_event.h"

namespace ocudu {
namespace ofh {

/// Downlink handler implementation configuration.
struct downlink_handler_impl_config {
  /// Radio sector identifier.
  unsigned sector;
  /// Downlink eAxCs.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  /// Optional TDD configuration.
  std::optional<tdd_ul_dl_config_common> tdd_config;
  /// Cyclic prefix.
  cyclic_prefix cp;
  /// Subcarrier spacing.
  subcarrier_spacing scs;
  /// Downlink processing time in microseconds.
  std::chrono::microseconds dl_processing_time;
  /// Transmission window timing parameters for delay management.
  tx_window_timing_parameters tx_timing_params;
  /// If set to true, logs late events as warnings, otherwise as info.
  bool enable_log_warnings_for_lates;
};

/// Downlink handler implementation dependencies.
struct downlink_handler_impl_dependencies {
  /// Logger
  ocudulog::basic_logger& logger;
  /// Error notifier.
  error_notifier& err_notifier;
  /// Data flow for Control-Plane.
  std::unique_ptr<data_flow_cplane_scheduling_commands> data_flow_cplane;
  /// Data flow for User-Plane.
  std::unique_ptr<data_flow_uplane_data> data_flow_uplane;
  /// Ethernet frame pool downlink Control-Plane.
  std::shared_ptr<ether::eth_frame_pool> frame_pool_dl_cp;
  /// Ethernet frame pool downlink User-Plane.
  std::shared_ptr<ether::eth_frame_pool> frame_pool_dl_up;
};

/// Open Fronthaul downlink handler implementation.
class downlink_handler_impl : public downlink_handler, public operation_controller
{
public:
  downlink_handler_impl(const downlink_handler_impl_config& config, downlink_handler_impl_dependencies&& dependencies);

  ~downlink_handler_impl() override { stop(); }

  // See interface for documentation.
  void start() override;

  // See interface for documentation.
  void stop() override;

  // See interface for documentation.
  void handle_dl_data(const resource_grid_context& context, const shared_resource_grid& grid) override;

  /// Returns the OTA symbol boundary notifier of this downlink handler implementation.
  ota_symbol_boundary_notifier& get_ota_symbol_boundary_notifier() { return window_checker; }

  /// Returns the metrics collector of this downlink handler implementation.
  downlink_handler_metrics_collector& get_metrics_collector() { return metrics_collector; }

private:
  const unsigned                                        sector_id;
  ocudulog::basic_logger&                               logger;
  const cyclic_prefix                                   cp;
  const std::optional<tdd_ul_dl_config_common>          tdd_config;
  const static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  tx_window_checker                                     window_checker;
  std::unique_ptr<data_flow_cplane_scheduling_commands> data_flow_cplane;
  std::unique_ptr<data_flow_uplane_data>                data_flow_uplane;
  std::shared_ptr<ether::eth_frame_pool>                frame_pool_dl_cp;
  std::shared_ptr<ether::eth_frame_pool>                frame_pool_dl_up;
  error_notifier&                                       err_notifier;
  downlink_handler_metrics_collector                    metrics_collector;
  bool                                                  enable_log_warnings_for_lates;
  rt_stop_event_source                                  stop_control;
};

} // namespace ofh
} // namespace ocudu
