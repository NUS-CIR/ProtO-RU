// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <optional>

namespace ocudu {

/// \brief Distribution of how early received DIFI datagrams arrive, ahead of the time their samples are due.
///
/// A datagram's margin is the wall-clock time from its arrival to when the last of its samples is due on the anchored
/// receive timeline. Recording never allocates.
class radio_difi_rx_timing_stats
{
public:
  /// [Implementation-defined] Width of one histogram bin.
  static constexpr std::chrono::microseconds BIN_WIDTH{25};
  /// [Implementation-defined] Smallest margin the histogram resolves; anything below falls in the first bin.
  static constexpr std::chrono::microseconds MIN_MARGIN{-2000};
  /// [Implementation-defined] Number of bins, covering [MIN_MARGIN, MIN_MARGIN + NOF_BINS * BIN_WIDTH); anything above
  /// falls in the last bin.
  static constexpr unsigned NOF_BINS = 480;

  /// Summary of the margins recorded since the last reset.
  struct summary {
    /// Data datagrams recorded.
    uint64_t nof_datagrams;
    /// Datagrams that arrived behind the samples already handed over, and were dropped.
    uint64_t nof_late;
    /// Smallest margin.
    std::chrono::microseconds min;
    /// Margin that 0.1% of the datagrams fall below, to the histogram resolution.
    std::chrono::microseconds p0_1;
    /// Margin that 1% of the datagrams fall below, to the histogram resolution.
    std::chrono::microseconds p1;
    /// Median margin, to the histogram resolution.
    std::chrono::microseconds p50;
  };

  /// Records the margin of one datagram.
  void record(std::chrono::nanoseconds margin);

  /// Records a datagram that arrived too late for any of its samples to be used.
  void record_late() { ++nof_late; }

  /// Returns the summary since the last reset, or nothing if no datagram was recorded.
  std::optional<summary> get_summary() const;

  /// Clears every count.
  void reset();

private:
  /// Returns the margin at the lower edge of the bin below which a fraction \p quantile of the datagrams fall.
  std::chrono::microseconds get_quantile(double quantile) const;

  /// Histogram of the margins, one count per bin.
  std::array<uint32_t, NOF_BINS> bins = {};
  /// Number of data datagrams recorded.
  uint64_t nof_datagrams = 0;
  /// Number of datagrams that arrived too late for any of their samples to be used.
  uint64_t nof_late = 0;
  /// Smallest margin recorded, kept exactly even outside the histogram range.
  std::chrono::nanoseconds min_margin = std::chrono::nanoseconds::max();
};

} // namespace ocudu
