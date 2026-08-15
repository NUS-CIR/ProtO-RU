// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ocudu/ofh/timing/gps_clock.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

TEST(ofh_gps_clock_test, calculate_slot_point_15khz)
{
  // 15 kHz: 1 slot per subframe, 1000 us slot duration. fractional time 7300 us, GPS second 100.
  //   subframe index      = (7300 / 1000) % 10 = 7
  //   slot in subframe     = (7300 / 1000) % 1  = 0
  //   sfn                  = (100 * 100 + 7300 / 10000) % 1024 = 10000 % 1024 = 784
  slot_point slot = gps_clock::calculate_slot_point(subcarrier_spacing::kHz15, 100, 7300, 1000);

  ASSERT_EQ(slot, slot_point(to_numerology_value(subcarrier_spacing::kHz15), 784, 7, 0));
}

TEST(ofh_gps_clock_test, calculate_slot_point_30khz)
{
  // 30 kHz: 2 slots per subframe, 500 us slot duration. fractional time 3700 us, GPS second 0.
  //   subframe index      = (3700 / 1000) % 10 = 3
  //   slot in subframe     = (3700 / 500) % 2   = 1
  //   sfn                  = (0 + 3700 / 10000) % 1024 = 0
  slot_point slot = gps_clock::calculate_slot_point(subcarrier_spacing::kHz30, 0, 3700, 500);

  ASSERT_EQ(slot, slot_point(to_numerology_value(subcarrier_spacing::kHz30), 0, 3, 1));
}

TEST(ofh_gps_clock_test, now_fraction_is_below_one_second)
{
  gps_clock::time_point    tp       = gps_clock::now();
  std::chrono::nanoseconds fraction = gps_clock::calculate_ns_fraction_from(tp);

  ASSERT_GE(fraction.count(), 0);
  ASSERT_LT(fraction, std::chrono::seconds(1));
}
