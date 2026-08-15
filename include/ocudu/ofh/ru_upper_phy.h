// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/phy/support/shared_resource_grid.h"
#include "ocudu/ran/slot_point.h"

namespace ocudu {

class prach_buffer;

namespace ofh {

/// \brief O-RU upper PHY boundary.
///
/// Provided by the O-RU's PHY (a fake one in the RU emulator). The O-RU sector uses it as a sink for received downlink
/// resource grids and as a source of the uplink and PRACH IQ that the O-RU transmits.
class ru_upper_phy
{
public:
  /// Default destructor.
  virtual ~ru_upper_phy() = default;

  /// Returns a resource grid into which the scheduled downlink will be received for the given slot.
  virtual shared_resource_grid get_downlink_rx_grid(slot_point slot) = 0;

  /// Notifies that the downlink resource grid for the given slot has been fully received.
  virtual void on_downlink_rx_grid_completed(slot_point slot, const shared_resource_grid& grid) = 0;

  /// Returns the resource grid holding the uplink IQ to transmit for the given slot.
  virtual shared_resource_grid get_uplink_tx_grid(slot_point slot) = 0;

  /// Returns the PRACH buffer holding the PRACH IQ to transmit for the given slot.
  virtual const prach_buffer& get_prach_tx_buffer(slot_point slot) = 0;
};

} // namespace ofh
} // namespace ocudu
