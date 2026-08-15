// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/slot_point.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include <chrono>
#include <cstdint>

namespace ocudu {

/// \brief Helpers to align the SDR Radio Unit's slot numbering with the O-DU's GPS slot numbering.
///
/// The SDR Radio Unit numbers its slots from the moment it starts streaming, while the O-DU numbers slots on the GPS
/// timescale: the slot count since the GPS epoch, with the SFN sent modulo 256 on the Open Fronthaul wire. Both
/// timelines tick at the same rate, so a constant offset maps one onto the other: the GPS slot count at the host time
/// a radio slot goes over the air, minus that radio slot. The original ProtO-RU measured this difference at radio
/// start inside its patched lower PHY, pairing the radio timestamp with the host wall clock (so it works with any
/// radio clock source, as long as the host clock is GPS/NTP-accurate); here the same measurement stays in the
/// application, taken from the TTI boundary notifications, and is applied when translating between the radio TTI
/// slots and the O-RU sector's wire slots.
namespace gps_slot_alignment {

/// Difference between Unix seconds and GPS seconds (Unix epoch 1970.1.1, GPS epoch 1980.1.6, 18 leap seconds since).
constexpr uint64_t UNIX_TO_GPS_SECONDS_OFFSET = 315964800ULL - 18ULL;

/// Number of slots in the Open Fronthaul wire numbering period (the radio-application header carries an 8-bit SFN).
constexpr unsigned nof_slots_per_ofh_period(subcarrier_spacing scs)
{
  return 256U * NOF_SUBFRAMES_PER_FRAME * get_nof_slots_per_subframe(scs);
}

/// \brief Measures the offset, in slots, from the radio slot numbering to the GPS slot numbering.
///
/// \param air_time Host time at which the radio slot goes over the air.
/// \param radio_slot The radio slot.
/// \return The GPS slot count at the air time (rounded to the nearest slot boundary) minus the radio slot, modulo the
/// OFH wire numbering period.
inline unsigned measure_gps_slot_offset(std::chrono::system_clock::time_point air_time, slot_point radio_slot)
{
  const subcarrier_spacing scs = to_subcarrier_spacing(radio_slot.numerology());
  // Slot duration in nanoseconds (1000 * slots-per-subframe slots tick per second).
  const uint64_t slot_ns = 1000000000ULL / (uint64_t(1000) * get_nof_slots_per_subframe(scs));
  const uint64_t unix_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(air_time.time_since_epoch()).count();
  const uint64_t gps_ns  = unix_ns - UNIX_TO_GPS_SECONDS_OFFSET * 1000000000ULL;

  // GPS slot count at the air time, rounded to the nearest slot boundary.
  const uint64_t gps_slots = (gps_ns + slot_ns / 2) / slot_ns;
  const unsigned period    = nof_slots_per_ofh_period(scs);

  return static_cast<unsigned>((gps_slots + period - (radio_slot.system_slot() % period)) % period);
}

/// Translates a radio-numbered slot into the GPS/OFH wire slot numbering using the given offset.
inline slot_point to_ofh_slot(slot_point radio_slot, unsigned gps_slot_offset)
{
  const subcarrier_spacing scs = to_subcarrier_spacing(radio_slot.numerology());
  return slot_point(radio_slot.numerology(),
                    (radio_slot.system_slot() + gps_slot_offset) % nof_slots_per_ofh_period(scs));
}

} // namespace gps_slot_alignment
} // namespace ocudu
