// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../receiver/ofh_ru_cplane_scheduling_dispatcher.h"
#include "../support/rx_grid_context_repository.h"
#include "../transmitter/ofh_data_flow_uplane_data.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ru_upper_phy.h"
#include <memory>
#include <mutex>

namespace ocudu {
namespace ofh {

/// O-RU Control-Plane scheduling handler configuration.
struct ru_cplane_scheduling_handler_config {
  /// Radio sector identifier.
  unsigned sector;
};

/// O-RU Control-Plane scheduling handler dependencies.
struct ru_cplane_scheduling_handler_dependencies {
  /// Logger.
  ocudulog::basic_logger* logger = nullptr;
  /// Upper PHY boundary (sink for received downlink grids, source of uplink/PRACH IQ).
  ru_upper_phy* upper_phy = nullptr;
  /// Repository where downlink grids are prepared and assembled.
  std::shared_ptr<rx_grid_context_repository> grid_repo;
  /// Data flow used to transmit uplink User-Plane (its frame pool is of type user_plane).
  data_flow_uplane_data* uplink_data_flow = nullptr;
  /// Data flow used to transmit PRACH User-Plane (its frame pool is of type uplane_prach).
  data_flow_uplane_data* prach_data_flow = nullptr;
};

/// \brief O-RU Control-Plane scheduling handler.
///
/// Reacts to decoded Control-Plane scheduling commands received from the O-DU: a downlink command prepares a resource
/// grid (obtained from the upper PHY) to receive the downlink User-Plane into; an uplink command transmits the uplink
/// IQ obtained from the upper PHY; a PRACH command transmits the PRACH IQ obtained from the upper PHY.
class ru_cplane_scheduling_handler : public ru_downlink_scheduling_handler, public ru_uplink_scheduling_handler
{
public:
  ru_cplane_scheduling_handler(const ru_cplane_scheduling_handler_config&       config,
                               const ru_cplane_scheduling_handler_dependencies& dependencies);

  // See interface for documentation.
  void handle_downlink_scheduling(unsigned eaxc, const cplane_message_decoder_results& results) override;

  // See interface for documentation.
  void handle_uplink_scheduling(unsigned eaxc, const cplane_message_decoder_results& results) override;

  // See interface for documentation.
  void handle_prach_scheduling(unsigned eaxc, const cplane_message_decoder_results& results) override;

private:
  ocudulog::basic_logger&                     logger;
  ru_upper_phy&                               upper_phy;
  std::shared_ptr<rx_grid_context_repository> grid_repo;
  data_flow_uplane_data&                      uplink_data_flow;
  data_flow_uplane_data&                      prach_data_flow;
  const unsigned                              sector_id;
  /// Serializes per-slot downlink grid lookup and allocation.
  std::mutex downlink_mutex;
};

} // namespace ofh
} // namespace ocudu
