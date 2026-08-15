// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ofh/ru_upper_phy.h"
#include "ocudu/phy/support/resource_grid_pool.h"
#include "ocudu/ran/slot_point.h"
#include <atomic>
#include <cstdint>
#include <memory>
#include <mutex>

namespace ocudu {

class ru_downlink_plane_handler;

/// RU emulator SDR upper PHY configuration.
struct ru_emulator_sdr_upper_phy_config {
  /// Radio sector identifier.
  unsigned sector;
  /// RU operating bandwidth in PRBs.
  unsigned nof_prb;
  /// Number of downlink eAxCs, i.e. the number of ports of the downlink reception grid.
  unsigned nof_dl_ports;
};

/// \brief RU emulator SDR upper PHY.
///
/// The upper PHY the O-RU sector drives when the emulator runs over a real radio (SDR mode). It implements only the
/// downlink reception side: it hands the sector a grid to assemble the received downlink into and, once the grid is
/// finalized by the sector's reception window handler, **pushes** it straight to the Radio Unit's downlink plane,
/// translating the wire slot back into the radio slot numbering with the GPS slot offset. Pushing at finalize time
/// (instead of waiting for the radio's TTI) means the only deadline is the lower PHY's baseband pull, one millisecond
/// before the slot's air time. Until the downlink handler is connected and the initial GPS slot offset is available,
/// completed grids are discarded so they return to the reception pool. The uplink/PRACH transmit getters are unused in
/// SDR mode (the radio supplies that IQ, which is pushed into the sector's uplink IQ sink instead).
class ru_emulator_sdr_upper_phy : public ofh::ru_upper_phy
{
public:
  ru_emulator_sdr_upper_phy(unsigned sector, std::unique_ptr<resource_grid_pool> dl_rx_grid_pool_);

  // See interface for documentation.
  shared_resource_grid get_downlink_rx_grid(slot_point slot) override;

  // See interface for documentation.
  void on_downlink_rx_grid_completed(slot_point slot, const shared_resource_grid& grid) override;

  // See interface for documentation. Unused in SDR mode (the radio supplies the uplink IQ).
  shared_resource_grid get_uplink_tx_grid(slot_point slot) override { return {}; }

  // See interface for documentation. Unused in SDR mode (the radio supplies the PRACH IQ).
  const prach_buffer& get_prach_tx_buffer(slot_point slot) override;

  /// Connects the Radio Unit's downlink plane, the destination of the completed downlink grids.
  void connect_downlink(ru_downlink_plane_handler& handler) { dl_handler = &handler; }

  /// Sets the offset translating the wire (GPS) slot numbering back to the radio slot numbering.
  void set_gps_slot_offset(unsigned offset) { gps_slot_offset.store(offset, std::memory_order_relaxed); }

  /// \brief Tracks the radio's current slot (called by the TTI orchestrator every TTI).
  ///
  /// The wire numbering wraps every 256 frames while the radio slot numbering spans 1024, so the wire-to-radio
  /// translation is ambiguous by whole wire periods; the current radio slot disambiguates it.
  void set_current_radio_slot(slot_point slot)
  {
    current_radio_slot_count.store(slot.system_slot(), std::memory_order_relaxed);
  }

  /// Number of completed downlink grids handed to the Radio Unit for transmission.
  uint64_t get_nof_transmitted_dl_grids() const { return nof_dl_tx.load(std::memory_order_relaxed); }

private:
  std::unique_ptr<resource_grid_pool> dl_rx_grid_pool;
  const unsigned                      sector_id;
  /// Radio Unit downlink plane (null until connected).
  std::atomic<ru_downlink_plane_handler*> dl_handler{nullptr};
  /// Wire-to-radio slot offset; negative until the GPS slot alignment is established.
  std::atomic<int64_t> gps_slot_offset{-1};
  /// Current radio slot count, written by the TTI orchestrator (see \ref set_current_radio_slot).
  std::atomic<uint32_t> current_radio_slot_count{0};
  std::atomic<uint64_t> nof_dl_tx{0};
  /// Completion is notified once per closing symbol of the slot: remember the last pushed slot to push once.
  slot_point last_pushed_slot;
  std::mutex mutex;
};

/// Creates an RU emulator SDR upper PHY.
std::unique_ptr<ru_emulator_sdr_upper_phy>
create_ru_emulator_sdr_upper_phy(const ru_emulator_sdr_upper_phy_config& config);

} // namespace ocudu
