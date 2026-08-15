// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ofh/ru_upper_phy.h"
#include "ocudu/phy/support/prach_buffer.h"
#include "ocudu/phy/support/resource_grid_pool.h"
#include <memory>

namespace ocudu {

/// RU emulator fake upper PHY configuration.
struct ru_emulator_upper_phy_config {
  /// RU operating bandwidth in PRBs (determines the resource grid size).
  unsigned nof_prb;
  /// Number of downlink eAxCs, i.e. the number of ports of the downlink reception grid.
  unsigned nof_dl_ports;
  /// Number of OFDM symbols comprising a PRACH U-Plane transmission.
  unsigned nof_prach_symbols;
  /// Set to true to source long-format (format 0) PRACH, false for short-format (B4) PRACH.
  bool prach_long_format;
};

/// \brief RU emulator fake upper PHY.
///
/// Implements the O-RU upper PHY boundary the library O-RU sector expects, backed by resource grids and a PRACH buffer
/// pre-filled with deterministic test IQ. It replaces the upstream emulator's hand-rolled \c generate_test_data and
/// \c generate_test_prach: the uplink and PRACH IQ are now read from a grid / PRACH buffer by the library transmit data
/// flow, instead of being baked into raw Ethernet frames.
///
/// Received downlink is written into a recycled grid and discarded (the emulator does not consume downlink content).
class ru_emulator_upper_phy : public ofh::ru_upper_phy
{
public:
  ru_emulator_upper_phy(std::unique_ptr<resource_grid_pool> dl_rx_grid_pool_,
                        std::unique_ptr<resource_grid_pool> ul_tx_grid_pool_,
                        std::unique_ptr<prach_buffer>       prach_buffer_);

  // See interface for documentation.
  shared_resource_grid get_downlink_rx_grid(slot_point slot) override;

  // See interface for documentation.
  void on_downlink_rx_grid_completed(slot_point slot, const shared_resource_grid& grid) override;

  // See interface for documentation.
  shared_resource_grid get_uplink_tx_grid(slot_point slot) override;

  // See interface for documentation.
  const prach_buffer& get_prach_tx_buffer(slot_point slot) override;

private:
  std::unique_ptr<resource_grid_pool> dl_rx_grid_pool;
  std::unique_ptr<resource_grid_pool> ul_tx_grid_pool;
  std::unique_ptr<prach_buffer>       prach;
};

/// Creates an RU emulator fake upper PHY pre-filled with deterministic test IQ from the given configuration.
std::unique_ptr<ru_emulator_upper_phy> create_ru_emulator_upper_phy(const ru_emulator_upper_phy_config& config);

} // namespace ocudu
