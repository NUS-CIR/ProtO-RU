// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ru_emulator_gps_slot_alignment.h"
#include "ocudu/ofh/ru_sector.h"
#include "ocudu/phy/support/prach_buffer.h"
#include "ocudu/phy/support/prach_buffer_context.h"
#include "ocudu/ru/ru_uplink_plane.h"
#include <atomic>

namespace ocudu {

/// \brief Adapts the SDR Radio Unit's received-symbol notifications to the O-RU sector's uplink IQ sink (SDR mode).
///
/// When the emulator runs over a real radio, the SDR \ref radio_unit captures uplink and notifies this handler with the
/// received resource grid symbols and PRACH windows. They are pushed straight into the O-RU sector's
/// \ref ofh::ru_uplink_iq_sink, which matches them against the recorded Control-Plane requests and builds the
/// User-Plane replies. The radio reports its own slot numbering, while the sector keys the recorded requests by the
/// O-DU's GPS-derived wire slots; the adapter translates between the two using the configured GPS slot offset. It is
/// the thin SDR-mode equivalent of ProtO-RU's \c ru_rx_symbol_handler (the matching and encoding now live in the O-RU
/// library).
class ru_emulator_rx_symbol_adapter : public ru_uplink_plane_rx_symbol_notifier
{
public:
  explicit ru_emulator_rx_symbol_adapter(ofh::ru_uplink_iq_sink& sink_) : sink(sink_) {}

  /// \brief Sets the offset, in slots, from the radio slot numbering to the GPS/OFH slot numbering.
  ///
  /// See \ref gps_slot_alignment::calculate_gps_slot_offset. Must be set before the radio starts capturing.
  void set_gps_slot_offset(unsigned offset) { gps_slot_offset.store(offset, std::memory_order_relaxed); }

  // See interface for documentation.
  void on_new_uplink_symbol(const ru_uplink_rx_symbol_context& context,
                            const shared_resource_grid&        grid,
                            bool                               is_valid) override
  {
    if (is_valid) {
      sink.handle_uplink_symbol(to_ofh_slot(context.slot), context.symbol_id, grid);
    }
  }

  // See interface for documentation.
  void on_new_prach_window_data(const prach_buffer_context& context, shared_prach_buffer buffer) override
  {
    if (buffer) {
      sink.handle_prach_window(to_ofh_slot(context.slot), *buffer);
    }
  }

private:
  /// Translates a radio-numbered slot into the GPS/OFH wire numbering.
  slot_point to_ofh_slot(slot_point radio_slot) const
  {
    return gps_slot_alignment::to_ofh_slot(radio_slot, gps_slot_offset.load(std::memory_order_relaxed));
  }

  ofh::ru_uplink_iq_sink& sink;
  /// Offset from the radio slot numbering to the GPS/OFH slot numbering, in slots.
  std::atomic<unsigned> gps_slot_offset{0};
};

} // namespace ocudu
