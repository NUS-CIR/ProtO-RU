// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_difi_rx_timing_stats.h"
#include <algorithm>
#include <cmath>

using namespace ocudu;

void radio_difi_rx_timing_stats::record(std::chrono::nanoseconds margin)
{
  const int64_t bin = (margin - MIN_MARGIN) / BIN_WIDTH;
  ++bins[std::clamp<int64_t>(bin, 0, NOF_BINS - 1)];
  ++nof_datagrams;
  min_margin = std::min(min_margin, margin);
}

std::chrono::microseconds radio_difi_rx_timing_stats::get_quantile(double quantile) const
{
  // The first bin whose cumulative count reaches the quantile.
  const auto target     = static_cast<uint64_t>(std::ceil(quantile * static_cast<double>(nof_datagrams)));
  uint64_t   cumulative = 0;
  for (unsigned bin = 0; bin != NOF_BINS; ++bin) {
    cumulative += bins[bin];
    if (cumulative >= std::max<uint64_t>(target, 1)) {
      return MIN_MARGIN + bin * BIN_WIDTH;
    }
  }
  return MIN_MARGIN + (NOF_BINS - 1) * BIN_WIDTH;
}

std::optional<radio_difi_rx_timing_stats::summary> radio_difi_rx_timing_stats::get_summary() const
{
  if (nof_datagrams == 0) {
    return std::nullopt;
  }

  static constexpr double P0_1 = 0.001;
  static constexpr double P1   = 0.01;
  static constexpr double P50  = 0.5;

  return summary{.nof_datagrams = nof_datagrams,
                 .nof_late      = nof_late,
                 .min           = std::chrono::duration_cast<std::chrono::microseconds>(min_margin),
                 .p0_1          = get_quantile(P0_1),
                 .p1            = get_quantile(P1),
                 .p50           = get_quantile(P50)};
}

void radio_difi_rx_timing_stats::reset()
{
  bins.fill(0);
  nof_datagrams = 0;
  nof_late      = 0;
  min_margin    = std::chrono::nanoseconds::max();
}
