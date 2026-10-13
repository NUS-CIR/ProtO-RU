// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "radio_difi_rx_timing_stats.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace std::chrono_literals;

TEST(RxTimingStats, NothingRecordedHasNoSummary)
{
  radio_difi_rx_timing_stats stats;
  EXPECT_FALSE(stats.get_summary().has_value());
}

TEST(RxTimingStats, QuantilesFollowTheLowTail)
{
  static constexpr unsigned                  NOF_DATAGRAMS   = 1000;
  static constexpr unsigned                  NOF_EARLY       = 990;
  static constexpr unsigned                  NOF_LESS_EARLY  = 9;
  static constexpr std::chrono::microseconds EARLY_MARGIN    = 1500us;
  static constexpr std::chrono::microseconds LESS_EARLY      = 300us;
  static constexpr std::chrono::microseconds LATEST_MARGIN   = 40us;
  static constexpr std::chrono::microseconds LATEST_BIN_EDGE = 25us;

  // Most datagrams arrive well ahead, a few less so, and one barely in time.
  radio_difi_rx_timing_stats stats;
  for (unsigned i = 0; i != NOF_EARLY; ++i) {
    stats.record(EARLY_MARGIN);
  }
  for (unsigned i = 0; i != NOF_LESS_EARLY; ++i) {
    stats.record(LESS_EARLY);
  }
  stats.record(LATEST_MARGIN);

  const auto summary = stats.get_summary();
  ASSERT_TRUE(summary.has_value());
  EXPECT_EQ(summary->nof_datagrams, NOF_DATAGRAMS);
  EXPECT_EQ(summary->nof_late, 0U);
  EXPECT_EQ(summary->min, LATEST_MARGIN);
  // The quantiles are lower bin edges, at the bin width of 25 us.
  EXPECT_EQ(summary->p0_1, LATEST_BIN_EDGE);
  EXPECT_EQ(summary->p1, LESS_EARLY);
  EXPECT_EQ(summary->p50, EARLY_MARGIN);
}

TEST(RxTimingStats, LateDatagramsAreCountedAndMarginsOutOfRangeClamp)
{
  static constexpr std::chrono::milliseconds BELOW_RANGE = -5ms;
  static constexpr std::chrono::milliseconds ABOVE_RANGE = 50ms;

  radio_difi_rx_timing_stats stats;
  stats.record(BELOW_RANGE);
  stats.record_late();
  stats.record(ABOVE_RANGE);

  const auto summary = stats.get_summary();
  ASSERT_TRUE(summary.has_value());
  EXPECT_EQ(summary->nof_late, 1U);
  // The exact minimum is kept even below the histogram range, while the quantile stops at the first bin.
  EXPECT_EQ(summary->min, BELOW_RANGE);
  EXPECT_EQ(summary->p0_1, radio_difi_rx_timing_stats::MIN_MARGIN);
  EXPECT_EQ(summary->p50, radio_difi_rx_timing_stats::MIN_MARGIN);
}

TEST(RxTimingStats, ResetClearsEverything)
{
  static constexpr std::chrono::milliseconds BEFORE_RESET = 1ms;
  static constexpr std::chrono::milliseconds AFTER_RESET  = 2ms;

  radio_difi_rx_timing_stats stats;
  stats.record(BEFORE_RESET);
  stats.record_late();
  stats.reset();
  EXPECT_FALSE(stats.get_summary().has_value());

  stats.record(AFTER_RESET);
  const auto summary = stats.get_summary();
  ASSERT_TRUE(summary.has_value());
  EXPECT_EQ(summary->nof_datagrams, 1U);
  EXPECT_EQ(summary->nof_late, 0U);
  EXPECT_EQ(summary->min, AFTER_RESET);
}
