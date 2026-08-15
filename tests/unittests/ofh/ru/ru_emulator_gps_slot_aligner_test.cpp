// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_gps_slot_aligner.h"
#include "ocudu/ocudulog/ocudulog.h"
#include <gtest/gtest.h>

using namespace ocudu;

namespace {

/// Timing notifier spy counting the forwarded notifications.
class timing_notifier_spy : public ru_timing_notifier
{
public:
  unsigned nof_ttis       = 0;
  unsigned nof_half_slots = 0;
  unsigned nof_full_slots = 0;

  void on_tti_boundary(const tti_boundary_context&) override { ++nof_ttis; }
  void on_ul_half_slot_boundary(slot_point) override { ++nof_half_slots; }
  void on_ul_full_slot_boundary(slot_point) override { ++nof_full_slots; }
};

/// 30 kHz slot duration in nanoseconds.
constexpr uint64_t SLOT_NS = 500000ULL;

/// \brief Builds a TTI context for the given radio slot whose (synthetic) host clock runs with the given GPS slot
/// offset: radio slot \c radio_slot airs when the GPS slot count is \c radio_slot + \c gps_slot_offset.
///
/// The time point carries air time minus the downlink processing lead the lower PHY notification exhibits (the aligner
/// adds it back), plus the given jitter.
tti_boundary_context make_tti(uint64_t radio_slot, unsigned gps_slot_offset, int64_t jitter_ns = 0)
{
  const uint64_t gps_air_ns  = (radio_slot + gps_slot_offset) * SLOT_NS;
  const uint64_t unix_air_ns = gps_air_ns + gps_slot_alignment::UNIX_TO_GPS_SECONDS_OFFSET * 1000000000ULL;

  tti_boundary_context ctx;
  ctx.slot = slot_point_extended(subcarrier_spacing::kHz30, radio_slot);
  ctx.time_point =
      std::chrono::system_clock::time_point(std::chrono::duration_cast<std::chrono::system_clock::duration>(
                                                std::chrono::nanoseconds(unix_air_ns + jitter_ns)) -
                                            std::chrono::milliseconds(1));
  return ctx;
}

} // namespace

TEST(ru_emulator_gps_slot_aligner_test, offset_is_measured_after_the_settle_period_and_reported_once)
{
  timing_notifier_spy          next;
  std::vector<unsigned>        reported;
  ru_emulator_gps_slot_aligner aligner(
      next, ocudulog::fetch_basic_logger("TEST"), [&reported](unsigned offset) { reported.push_back(offset); });

  const unsigned gps_slot_offset = 1234;

  // One second of settle TTIs (2000 at 30 kHz) plus the measurement window.
  uint64_t radio_slot = 0;
  for (; radio_slot != 2100; ++radio_slot) {
    aligner.on_tti_boundary(make_tti(radio_slot, gps_slot_offset));
  }

  ASSERT_EQ(reported.size(), 1);
  ASSERT_EQ(reported.front(), gps_slot_offset);
  // Every notification was forwarded, including the ones consumed for measurement.
  ASSERT_EQ(next.nof_ttis, 2100);
}

TEST(ru_emulator_gps_slot_aligner_test, sub_half_slot_jitter_does_not_change_the_measured_offset)
{
  timing_notifier_spy          next;
  std::vector<unsigned>        reported;
  ru_emulator_gps_slot_aligner aligner(
      next, ocudulog::fetch_basic_logger("TEST"), [&reported](unsigned offset) { reported.push_back(offset); });

  const unsigned gps_slot_offset = 77;

  // Notification times jitter by up to 200 us (less than half a 30 kHz slot) around the ideal instant.
  uint64_t radio_slot = 0;
  for (; radio_slot != 2100; ++radio_slot) {
    const int64_t jitter_ns = (radio_slot % 5) * 50000 - 100000;
    aligner.on_tti_boundary(make_tti(radio_slot, gps_slot_offset, jitter_ns));
  }

  ASSERT_EQ(reported.size(), 1);
  ASSERT_EQ(reported.front(), gps_slot_offset);
}

TEST(ru_emulator_gps_slot_aligner_test, sustained_drift_re_aligns_the_offset)
{
  timing_notifier_spy          next;
  std::vector<unsigned>        reported;
  ru_emulator_gps_slot_aligner aligner(
      next, ocudulog::fetch_basic_logger("TEST"), [&reported](unsigned offset) { reported.push_back(offset); });

  const unsigned base       = 500;
  uint64_t       radio_slot = 0;

  // Settle (2000 TTIs) plus one full measurement window (64) at the base offset: the offset is established once.
  for (unsigned i = 0; i != 2000 + 64; ++i, ++radio_slot) {
    aligner.on_tti_boundary(make_tti(radio_slot, base));
  }
  ASSERT_EQ(reported.size(), 1);
  ASSERT_EQ(reported.front(), base);

  // The radio clock drifted by a slot (offset base+1). The first confirming window only arms the candidate.
  for (unsigned i = 0; i != 64; ++i, ++radio_slot) {
    aligner.on_tti_boundary(make_tti(radio_slot, base + 1));
  }
  ASSERT_EQ(reported.size(), 1);

  // The second consecutive window adopts the new offset.
  for (unsigned i = 0; i != 64; ++i, ++radio_slot) {
    aligner.on_tti_boundary(make_tti(radio_slot, base + 1));
  }
  ASSERT_EQ(reported.size(), 2);
  ASSERT_EQ(reported.back(), base + 1);
}

TEST(ru_emulator_gps_slot_aligner_test, single_window_excursion_does_not_re_align)
{
  timing_notifier_spy          next;
  std::vector<unsigned>        reported;
  ru_emulator_gps_slot_aligner aligner(
      next, ocudulog::fetch_basic_logger("TEST"), [&reported](unsigned offset) { reported.push_back(offset); });

  const unsigned base       = 500;
  uint64_t       radio_slot = 0;

  for (unsigned i = 0; i != 2000 + 64; ++i, ++radio_slot) {
    aligner.on_tti_boundary(make_tti(radio_slot, base));
  }
  ASSERT_EQ(reported.size(), 1);

  // A single window straddles the next slot boundary (one-off excursion), then it returns: no re-alignment, the
  // two-window hysteresis rejects it.
  for (unsigned i = 0; i != 64; ++i, ++radio_slot) {
    aligner.on_tti_boundary(make_tti(radio_slot, base + 1));
  }
  for (unsigned i = 0; i != 64; ++i, ++radio_slot) {
    aligner.on_tti_boundary(make_tti(radio_slot, base));
  }
  ASSERT_EQ(reported.size(), 1);
  ASSERT_EQ(reported.front(), base);
}

TEST(ru_emulator_gps_slot_aligner_test, other_boundaries_are_forwarded)
{
  timing_notifier_spy          next;
  ru_emulator_gps_slot_aligner aligner(next, ocudulog::fetch_basic_logger("TEST"), [](unsigned) {});

  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 5);
  aligner.on_ul_half_slot_boundary(slot);
  aligner.on_ul_full_slot_boundary(slot);

  ASSERT_EQ(next.nof_half_slots, 1);
  ASSERT_EQ(next.nof_full_slots, 1);
}
