// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ran/slot_point.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include <chrono>
#include <ctime>

namespace ocudu {
namespace ofh {

/// \brief Difference in seconds between the Unix epoch and the GPS epoch.
///
/// The Unix epoch is 1970-01-01 00:00:00 UTC and the GPS epoch is 1980-01-06 00:00:00 UTC, which are 315964800 seconds
/// apart. GPS time additionally runs ahead of UTC by the 18 leap seconds inserted since the GPS epoch, hence the
/// subtraction.
inline constexpr uint64_t UNIX_TO_GPS_SECONDS_OFFSET = 315964800ULL - 18ULL;

/// \brief GPS clock.
///
/// std::chrono-compatible clock whose epoch is the GPS epoch. It is derived from the system real-time clock by
/// subtracting the Unix-to-GPS offset, and provides helpers to map GPS time onto an Open Fronthaul slot point.
struct gps_clock {
  using duration   = std::chrono::nanoseconds;
  using rep        = duration::rep;
  using period     = duration::period;
  using time_point = std::chrono::time_point<gps_clock>;

  /// Returns the current GPS time.
  static time_point now()
  {
    ::timespec ts = {};
    ::clock_gettime(CLOCK_REALTIME, &ts);

    time_point current(std::chrono::seconds(ts.tv_sec) + std::chrono::nanoseconds(ts.tv_nsec));

    return current - std::chrono::seconds(UNIX_TO_GPS_SECONDS_OFFSET);
  }

  /// Returns the sub-second (fractional) part of the given time point.
  static std::chrono::nanoseconds calculate_ns_fraction_from(time_point tp)
  {
    auto tp_sec = std::chrono::time_point_cast<std::chrono::seconds>(tp);
    return tp - tp_sec;
  }

  /// \brief Calculates the slot point corresponding to the given GPS time.
  ///
  /// \param[in] scs              Subcarrier spacing.
  /// \param[in] gps_seconds      Whole GPS seconds.
  /// \param[in] fractional_us    Sub-second part of the GPS time, in microseconds.
  /// \param[in] slot_duration_us Slot duration in microseconds for the given subcarrier spacing.
  /// \return The slot point that contains the given GPS time instant.
  static slot_point
  calculate_slot_point(subcarrier_spacing scs, uint64_t gps_seconds, uint32_t fractional_us, uint32_t slot_duration_us)
  {
    unsigned subframe_index      = (fractional_us / (SUBFRAME_DURATION_MSEC * 1000)) % NOF_SUBFRAMES_PER_FRAME;
    unsigned slot_subframe_index = (fractional_us / slot_duration_us) % get_nof_slots_per_subframe(scs);

    static constexpr unsigned NUM_FRAMES_PER_SEC = 100;
    static constexpr unsigned FRAME_DURATION_US  = SUBFRAME_DURATION_MSEC * NOF_SUBFRAMES_PER_FRAME * 1000;
    unsigned                  sfn = (gps_seconds * NUM_FRAMES_PER_SEC + (fractional_us / FRAME_DURATION_US)) % NOF_SFNS;

    return slot_point(to_numerology_value(scs), sfn, subframe_index, slot_subframe_index);
  }
};

} // namespace ofh
} // namespace ocudu
