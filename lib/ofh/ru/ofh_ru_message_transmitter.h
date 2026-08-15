// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ethernet/ethernet_frame_pool.h"
#include "ocudu/ofh/ethernet/ethernet_transmitter.h"
#include "ocudu/ofh/timing/ofh_ota_symbol_boundary_notifier.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// O-RU message transmitter configuration.
struct ru_message_transmitter_config {
  /// Radio sector identifier.
  unsigned sector;
  /// \brief Transmit window (Ta3) in symbols after the captured symbol's air time.
  ///
  /// At OTA symbol T the transmitter drains the frames of past symbols [T - start, T - end]: a frame for symbol X is
  /// sent while the OTA time is within X + [end, start] - the Ta3 window, which for an O-RU opens AFTER the air time
  /// (the O-DU's downlink is the opposite, transmitted ahead of air).
  unsigned tx_window_start_symbols;
  unsigned tx_window_end_symbols;
};

/// O-RU message transmitter dependencies.
struct ru_message_transmitter_dependencies {
  /// Logger.
  ocudulog::basic_logger* logger = nullptr;
  /// Ethernet transmitter.
  std::unique_ptr<ether::transmitter> eth_transmitter;
  /// Uplink User-Plane frame pool drained by this transmitter.
  std::shared_ptr<ether::eth_frame_pool> uplink_uplane_pool;
  /// PRACH frame pool drained by this transmitter.
  std::shared_ptr<ether::eth_frame_pool> prach_pool;
};

/// \brief O-RU message transmitter.
///
/// Driven by the OFH timing manager: on each OTA symbol boundary it drains the uplink User-Plane and PRACH frame pools
/// for the frames due within the transmit window and sends them through the Ethernet transmitter. It is the O-RU
/// counterpart of the O-DU message transmitter, which instead drains the downlink Control-Plane, uplink Control-Plane
/// and downlink User-Plane pools.
class ru_message_transmitter : public ota_symbol_boundary_notifier
{
public:
  ru_message_transmitter(const ru_message_transmitter_config&  config,
                         ru_message_transmitter_dependencies&& dependencies);

  // See interface for documentation.
  void on_new_symbol(const slot_symbol_point_context& symbol_point_context) override;

  /// Returns the Ethernet transmitter of this message transmitter.
  ether::transmitter& get_ethernet_transmitter() { return *eth_transmitter; }

private:
  ocudulog::basic_logger&                logger;
  std::unique_ptr<ether::transmitter>    eth_transmitter;
  std::shared_ptr<ether::eth_frame_pool> uplink_uplane_pool;
  std::shared_ptr<ether::eth_frame_pool> prach_pool;
  const int                              tx_window_start;
  const int                              tx_window_end;
  const unsigned                         sector_id;
};

} // namespace ofh
} // namespace ocudu
