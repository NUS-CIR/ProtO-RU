// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/du/du_high/du_manager/converters/asn1_rrc_config_helpers.h"
#include "lib/du/du_high/du_manager/ran_resource_management/du_meas_config_manager.h"
#include "tests/ocudu_test_requirements.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/sys_info.h"
#include "ocudu/asn1/rrc_nr/ul_dcch_msg_ies.h"
#include "ocudu/du/du_cell_config_helpers.h"
#include "ocudu/ran/ssb/ssb_properties.h"
#include "ocudu/scheduler/rrm/ue_capability_summary.h"
#include "fmt/format.h"
#include "fmt/ranges.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace odu;
using namespace asn1::rrc_nr;

namespace {

using smtc_duration = ssb_mtc_s::dur_opts::options;
using offset_range  = std::pair<uint8_t, uint8_t>;

// Builds a supported_meas_gap_patterns marking the given Gap Pattern Ids as supported (0 and 1 are always added).
supported_meas_gap_patterns make_supported_gap_patterns(std::initializer_list<unsigned> pattern_ids)
{
  supported_meas_gap_patterns patterns;
  for (unsigned pattern_id : pattern_ids) {
    patterns.mark_supported(pattern_id);
  }
  return patterns;
}

struct meas_gap_test_params {
  subcarrier_spacing         pcell_scs;
  ssb_periodicity            smtc_period;
  offset_range               smtc_offsets; // half-open offset range [first, second)
  smtc_duration              smtc_dur;
  meas_gap_length            expected_mgl;
  meas_gap_repetition_period expected_mgrp;
};

void PrintTo(const meas_gap_test_params& p, std::ostream* os)
{
  *os << "scs=" << scs_to_khz(p.pcell_scs) << "kHz period=" << static_cast<unsigned>(p.smtc_period)
      << "ms off=" << +p.smtc_offsets.first << "_" << +p.smtc_offsets.second
      << " dur=" << (static_cast<unsigned>(p.smtc_dur) + 1) << "sf"
      << " mgl=" << meas_gap_length_to_msec(p.expected_mgl) << "ms mgrp=" << static_cast<unsigned>(p.expected_mgrp)
      << "ms";
}

ssb_mtc_s make_smtc(ssb_periodicity period, uint8_t offset, smtc_duration dur)
{
  ssb_mtc_s smtc;
  smtc.dur.value = dur;
  switch (period) {
    case ssb_periodicity::ms5:
      smtc.periodicity_and_offset.set_sf5() = offset;
      break;
    case ssb_periodicity::ms10:
      smtc.periodicity_and_offset.set_sf10() = offset;
      break;
    case ssb_periodicity::ms20:
      smtc.periodicity_and_offset.set_sf20() = offset;
      break;
    case ssb_periodicity::ms40:
      smtc.periodicity_and_offset.set_sf40() = offset;
      break;
    case ssb_periodicity::ms80:
      smtc.periodicity_and_offset.set_sf80() = offset;
      break;
    case ssb_periodicity::ms160:
      smtc.periodicity_and_offset.set_sf160() = offset;
      break;
  }
  return smtc;
}

std::string param_name(const ::testing::TestParamInfo<meas_gap_test_params>& info)
{
  const auto& p = info.param;
  return fmt::format("scs_{:_>3}kHz_period_{:_>3}ms_offsets_{:_>3}_to_{:_>3}_duration_{:01}sf",
                     scs_to_khz(p.pcell_scs),
                     fmt::underlying(p.smtc_period),
                     p.smtc_offsets.first,
                     p.smtc_offsets.second,
                     static_cast<unsigned>(p.smtc_dur) + 1);
}

class du_meas_config_manager_create_meas_gap_test : public ::testing::TestWithParam<meas_gap_test_params>
{};

TEST_P(du_meas_config_manager_create_meas_gap_test, gap_matches_expected_mgl_mgrp_and_offset)
{
  OCUDU_TEST_REQUIREMENTS("DU-GEN-9");

  const meas_gap_test_params& p = GetParam();
  for (uint8_t off = p.smtc_offsets.first; off < p.smtc_offsets.second; ++off) {
    SCOPED_TRACE(fmt::format("smtc_offset={}", off));
    const ssb_mtc_s       smtc = make_smtc(p.smtc_period, off, p.smtc_dur);
    const meas_gap_config gap =
        create_meas_gap(p.pcell_scs, smtc, {}, std::nullopt, supported_meas_gap_patterns::all());

    EXPECT_EQ(gap.offset, off);
    EXPECT_EQ(gap.mgl, p.expected_mgl);
    EXPECT_EQ(gap.mgrp, p.expected_mgrp);
  }
}

// SMTC Duration to Measurement Gap Length test
INSTANTIATE_TEST_SUITE_P(duration_to_mgl,
                         du_meas_config_manager_create_meas_gap_test,
                         ::testing::Values(
                             // sf1 + 15 kHz -> ms3.
                             meas_gap_test_params{subcarrier_spacing::kHz15,
                                                  ssb_periodicity::ms20,
                                                  {0, 20},
                                                  smtc_duration::sf1,
                                                  meas_gap_length::ms3,
                                                  meas_gap_repetition_period::ms20},
                             // sf1 + 30 kHz -> ms1.5.
                             meas_gap_test_params{subcarrier_spacing::kHz30,
                                                  ssb_periodicity::ms20,
                                                  {0, 20},
                                                  smtc_duration::sf1,
                                                  meas_gap_length::ms1dot5,
                                                  meas_gap_repetition_period::ms20},
                             // sf1 + 60 kHz -> ms1.5.
                             meas_gap_test_params{subcarrier_spacing::kHz60,
                                                  ssb_periodicity::ms20,
                                                  {0, 20},
                                                  smtc_duration::sf1,
                                                  meas_gap_length::ms1dot5,
                                                  meas_gap_repetition_period::ms20},
                             // sf2 -> ms3.
                             meas_gap_test_params{subcarrier_spacing::kHz30,
                                                  ssb_periodicity::ms20,
                                                  {0, 20},
                                                  smtc_duration::sf2,
                                                  meas_gap_length::ms3,
                                                  meas_gap_repetition_period::ms20},
                             // sf3 -> ms4.
                             meas_gap_test_params{subcarrier_spacing::kHz30,
                                                  ssb_periodicity::ms20,
                                                  {0, 20},
                                                  smtc_duration::sf3,
                                                  meas_gap_length::ms4,
                                                  meas_gap_repetition_period::ms20},
                             // sf4 -> ms6.
                             meas_gap_test_params{subcarrier_spacing::kHz30,
                                                  ssb_periodicity::ms20,
                                                  {0, 20},
                                                  smtc_duration::sf4,
                                                  meas_gap_length::ms6,
                                                  meas_gap_repetition_period::ms20},
                             // sf5 -> ms6.
                             meas_gap_test_params{subcarrier_spacing::kHz30,
                                                  ssb_periodicity::ms20,
                                                  {0, 20},
                                                  smtc_duration::sf5,
                                                  meas_gap_length::ms6,
                                                  meas_gap_repetition_period::ms20}),
                         param_name);

// SMTC periodicity to Measurement Gap Repetition Period test
INSTANTIATE_TEST_SUITE_P(periodicity_to_mgrp,
                         du_meas_config_manager_create_meas_gap_test,
                         ::testing::Values(meas_gap_test_params{subcarrier_spacing::kHz30,
                                                                ssb_periodicity::ms5,
                                                                {0, 5},
                                                                smtc_duration::sf5,
                                                                meas_gap_length::ms6,
                                                                meas_gap_repetition_period::ms20},
                                           meas_gap_test_params{subcarrier_spacing::kHz30,
                                                                ssb_periodicity::ms10,
                                                                {0, 10},
                                                                smtc_duration::sf5,
                                                                meas_gap_length::ms6,
                                                                meas_gap_repetition_period::ms20},
                                           meas_gap_test_params{subcarrier_spacing::kHz30,
                                                                ssb_periodicity::ms20,
                                                                {0, 20},
                                                                smtc_duration::sf5,
                                                                meas_gap_length::ms6,
                                                                meas_gap_repetition_period::ms20},
                                           meas_gap_test_params{subcarrier_spacing::kHz30,
                                                                ssb_periodicity::ms40,
                                                                {0, 40},
                                                                smtc_duration::sf5,
                                                                meas_gap_length::ms6,
                                                                meas_gap_repetition_period::ms40},
                                           meas_gap_test_params{subcarrier_spacing::kHz30,
                                                                ssb_periodicity::ms80,
                                                                {0, 80},
                                                                smtc_duration::sf5,
                                                                meas_gap_length::ms6,
                                                                meas_gap_repetition_period::ms80},
                                           meas_gap_test_params{subcarrier_spacing::kHz30,
                                                                ssb_periodicity::ms160,
                                                                {0, 160},
                                                                smtc_duration::sf5,
                                                                meas_gap_length::ms6,
                                                                meas_gap_repetition_period::ms160}),
                         param_name);

// ---------- Collision avoidance scenarios ----------

struct collision_params {
  const char*                      tag;
  subcarrier_spacing               pcell_scs;
  ssb_periodicity                  smtc_period;
  uint8_t                          smtc_offset;
  smtc_duration                    smtc_dur;
  std::vector<periodic_uci_config> ul_occasions;
  meas_gap_config                  expected;
};

void PrintTo(const collision_params& p, std::ostream* os)
{
  *os << p.tag << " scs=" << scs_to_khz(p.pcell_scs) << "kHz period=" << static_cast<unsigned>(p.smtc_period)
      << "ms offset=" << +p.smtc_offset << " dur=" << (static_cast<unsigned>(p.smtc_dur) + 1)
      << "sf expected={offset=" << p.expected.offset << " mgl=" << meas_gap_length_to_msec(p.expected.mgl)
      << "ms mgrp=" << static_cast<unsigned>(p.expected.mgrp) << "ms}";
}

class du_meas_config_manager_collision_test : public ::testing::TestWithParam<collision_params>
{};

TEST_P(du_meas_config_manager_collision_test, gap_avoids_or_minimises_collisions)
{
  OCUDU_TEST_REQUIREMENTS("DU-GEN-9");

  const auto&           p    = GetParam();
  const ssb_mtc_s       smtc = make_smtc(p.smtc_period, p.smtc_offset, p.smtc_dur);
  const meas_gap_config gap =
      create_meas_gap(p.pcell_scs, smtc, p.ul_occasions, std::nullopt, supported_meas_gap_patterns::all());

  EXPECT_EQ(gap.offset, p.expected.offset);
  EXPECT_EQ(gap.mgl, p.expected.mgl);
  EXPECT_EQ(gap.mgrp, p.expected.mgrp);
}

// At 30 kHz SCS: 2 slots per ms. SR/CSI period and offset are in PCell slots.
INSTANTIATE_TEST_SUITE_P(
    collision_avoidance,
    du_meas_config_manager_collision_test,
    ::testing::Values(
        // No SR/CSI configured. Gap is placed straight at the SMTC offset.
        //
        //      ms | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|
        //   SSB   | S| S| S| S| S|  |  |  |  |  | S| S| S| S| S|  |  |  |  |  |
        //   gap@0 | G| G| G| G| G| G|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  OK
        collision_params{"no_ul_occasion_collisions",
                         subcarrier_spacing::kHz30,
                         ssb_periodicity::ms10,
                         0,
                         smtc_duration::sf5,
                         {},
                         meas_gap_config{0, meas_gap_length::ms6, meas_gap_repetition_period::ms20}},
        // SR present but its only instance per MGRP lives outside the gap window.
        // SR period 40 slots (20 ms), offset 20 slots (10 ms).
        //
        //      ms | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|
        //   SSB   | S| S| S| S| S|  |  |  |  |  | S| S| S| S| S|  |  |  |  |  |
        //   SR    |  |  |  |  |  |  |  |  |  |  | R|  |  |  |  |  |  |  |  |  |
        //   gap@0 | G| G| G| G| G| G|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  OK
        collision_params{"sr_no_collision",
                         subcarrier_spacing::kHz30,
                         ssb_periodicity::ms10,
                         0,
                         smtc_duration::sf5,
                         {{40, 20}},
                         meas_gap_config{0, meas_gap_length::ms6, meas_gap_repetition_period::ms20}},
        // SR collides at gap_offset=0; algorithm shifts to the next SMTC instance at ms=10.
        // SR period 20 ms, offset 0.
        //
        //      ms  | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|
        //   SSB    | S| S| S| S| S|  |  |  |  |  | S| S| S| S| S|  |  |  |  |  |
        //   SR     | R|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
        //   gap@0  | G| G| G| G| G| G|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  SR in gap
        //   gap@10 |  |  |  |  |  |  |  |  |  |  | G| G| G| G| G| G|  |  |  |  |  OK
        collision_params{"sr_collision_avoided_by_offset_shift",
                         subcarrier_spacing::kHz30,
                         ssb_periodicity::ms10,
                         0,
                         smtc_duration::sf5,
                         {{40, 0}},
                         meas_gap_config{10, meas_gap_length::ms6, meas_gap_repetition_period::ms20}},
        // CSI collides at gap_offset=0; algorithm shifts to gap_offset=10.
        // CSI period 20 ms, offset 4 slots (= 2 ms).
        //
        //      ms  | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|
        //   SSB    | S| S| S| S| S|  |  |  |  |  | S| S| S| S| S|  |  |  |  |  |
        //   CSI    |  |  | C|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
        //   gap@0  | G| G| G| G| G| G|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  CSI in gap
        //   gap@10 |  |  |  |  |  |  |  |  |  |  | G| G| G| G| G| G|  |  |  |  |  OK
        collision_params{"csi_collision_avoided_by_offset_shift",
                         subcarrier_spacing::kHz30,
                         ssb_periodicity::ms10,
                         0,
                         smtc_duration::sf5,
                         {{40, 4}},
                         meas_gap_config{10, meas_gap_length::ms6, meas_gap_repetition_period::ms20}},
        // Combined SR + CSI: SR at ms 0 forces shift; CSI at slot 34 (= 17 ms) doesn't overlap gap@10.
        //
        //      ms  | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|
        //   SSB    | S| S| S| S| S|  |  |  |  |  | S| S| S| S| S|  |  |  |  |  |
        //   SR     | R|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
        //   CSI    |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |  | C|  |  |
        //   gap@0  | G| G| G| G| G| G|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  SR in gap
        //   gap@10 |  |  |  |  |  |  |  |  |  |  | G| G| G| G| G| G|  |  |  |  |  OK
        collision_params{"sr_and_csi_avoided_together",
                         subcarrier_spacing::kHz30,
                         ssb_periodicity::ms10,
                         0,
                         smtc_duration::sf5,
                         {{40, 0}, {40, 34}},
                         meas_gap_config{10, meas_gap_length::ms6, meas_gap_repetition_period::ms20}},
        // Strict at MGRP=20 fails: SMTC period = MGRP gives only one candidate gap offset (0 ms),
        // and the only SR instance per cycle lands right inside it. Doubling MGRP to 40 yields two
        // SR instances per cycle — gap@0 still catches the first, but the second at ms 20 stays
        // outside the gap, so loose accepts (every-other-SR works).
        //
        // MGRP=20 (rejected: only SR fully blocked):
        //      ms  | 0| 1| 2| 3| 4| 5| 6|...                            |19|
        //   SSB    | S| S| S| S| S|  |  |...                            |  |
        //   SR     | R|  |  |  |  |  |  |...                            |  |
        //   gap@0  | G| G| G| G| G| G|  |...                            |  |  only SR fully in gap
        //
        // MGRP=40 (accepted by loose: one in, one out):
        //      ms  | 0| 1| 2| 3| 4| 5| 6|...|19|20|21|...               |39|
        //   SSB    | S| S| S| S| S|  |  |...|  | S| S|...               |  |
        //   SR     | R|  |  |  |  |  |  |...|  | R|  |...               |  |
        //   gap@0  | G| G| G| G| G| G|  |...|  |  |  |...               |  |  OK, SR@20 stays out
        collision_params{"loose_check_with_doubling_required_for_sr",
                         subcarrier_spacing::kHz30,
                         ssb_periodicity::ms20,
                         0,
                         smtc_duration::sf5,
                         {{40, 0}},
                         meas_gap_config{0, meas_gap_length::ms6, meas_gap_repetition_period::ms40}},
        // SR period 40 ms forces min MGRP >= 40, even though SMTC period 10 ms would allow MGRP=20.
        // At MGRP=40, gap@0 already avoids SR which lives at ms 15.
        //
        //      ms  | 0| 1| 2| 3| 4| 5| 6|...|14|15|16|...|29|30|...     |39|
        //   SSB    | S| S| S| S| S|  |  |...|  |  |  |...|  | S|...     |  |   (SMTC at 0, 10, 20, 30)
        //   SR     |  |  |  |  |  |  |  |...|  | R|  |...|  |  |...     |  |   (single SR, period 40)
        //   gap@0  | G| G| G| G| G| G|  |...|  |  |  |...|  |  |...     |  |   OK
        collision_params{"min_mgrp_raised_by_sr_period",
                         subcarrier_spacing::kHz30,
                         ssb_periodicity::ms10,
                         0,
                         smtc_duration::sf5,
                         {{80, 30}},
                         meas_gap_config{0, meas_gap_length::ms6, meas_gap_repetition_period::ms40}},
        // SMTC period 20 = MGRP_min, only one SMTC alignment in MGRP.
        // SR at slot 11 sits at ms 5.5 — right at the trailing edge of gap[0, 6). MGL=6 vs SMTC duration=5 leaves
        // 1 ms of left-shift slack: gap_offset=19 wraps the gap to [19, 20) ∪ [0, 5) which still
        // encloses SMTC[0, 5) ms but excludes SR at ms 5.5.
        //
        //      ms   | 0| 1| 2| 3| 4| 5| 6| 7| 8| 9|10|11|12|13|14|15|16|17|18|19|
        //   SSB     | S| S| S| S| S|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  |
        //   SR      |  |  |  |  |  |.R|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  ← SR at ms 5.5
        //   gap@0   | G| G| G| G| G| G|  |  |  |  |  |  |  |  |  |  |  |  |  |  |  SR in gap
        //   gap@19  | G| G| G| G| G|  |  |  |  |  |  |  |  |  |  |  |  |  |  | G|  wraparound; SR out
        collision_params{"offset_shift_by_slack_between_mgl_and_ssb_duration",
                         subcarrier_spacing::kHz30,
                         ssb_periodicity::ms20,
                         0,
                         smtc_duration::sf5,
                         {{40, 11}},
                         meas_gap_config{19, meas_gap_length::ms6, meas_gap_repetition_period::ms20}}),
    [](const ::testing::TestParamInfo<collision_params>& test_info) { return std::string{test_info.param.tag}; });

// ---------- UE supported gap pattern restriction ----------

struct gap_pattern_params {
  const char*                 tag;
  subcarrier_spacing          pcell_scs;
  ssb_periodicity             smtc_period;
  uint8_t                     smtc_offset;
  smtc_duration               smtc_dur;
  supported_meas_gap_patterns supported_patterns;
  meas_gap_config             expected;
};

void PrintTo(const gap_pattern_params& p, std::ostream* os)
{
  std::vector<unsigned> pattern_ids;
  for (unsigned pattern_id = 0; pattern_id != nof_meas_gap_patterns; ++pattern_id) {
    if (p.supported_patterns.is_supported(pattern_id)) {
      pattern_ids.push_back(pattern_id);
    }
  }
  *os << fmt::format("{} scs={}kHz period={}ms offset={} dur={}sf patterns=[{}] expected={{offset={} mgl={}ms "
                     "mgrp={}ms}}",
                     p.tag,
                     scs_to_khz(p.pcell_scs),
                     fmt::underlying(p.smtc_period),
                     p.smtc_offset,
                     static_cast<unsigned>(p.smtc_dur) + 1,
                     fmt::join(pattern_ids, ","),
                     p.expected.offset,
                     meas_gap_length_to_msec(p.expected.mgl),
                     static_cast<unsigned>(p.expected.mgrp));
}

class du_meas_config_manager_gap_pattern_test : public ::testing::TestWithParam<gap_pattern_params>
{};

TEST_P(du_meas_config_manager_gap_pattern_test, gap_respects_supported_patterns)
{
  OCUDU_TEST_REQUIREMENTS("DU-GEN-9");

  const auto&           p    = GetParam();
  const ssb_mtc_s       smtc = make_smtc(p.smtc_period, p.smtc_offset, p.smtc_dur);
  const meas_gap_config gap  = create_meas_gap(p.pcell_scs, smtc, {}, std::nullopt, p.supported_patterns);

  EXPECT_EQ(gap.offset, p.expected.offset);
  EXPECT_EQ(gap.mgl, p.expected.mgl);
  EXPECT_EQ(gap.mgrp, p.expected.mgrp);
}

INSTANTIATE_TEST_SUITE_P(
    supported_gap_patterns,
    du_meas_config_manager_gap_pattern_test,
    ::testing::Values(
        // UE supports only the mandatory gap patterns 0 (MGL=6, MGRP=40) and 1 (MGL=6, MGRP=80). Although MGRP=20 would
        // suffice for the SMTC period, pattern 4 (MGL=6, MGRP=20) is not supported, so the gap uses MGRP=40 instead.
        gap_pattern_params{"unsupported_mgrp_is_skipped",
                           subcarrier_spacing::kHz30,
                           ssb_periodicity::ms20,
                           0,
                           smtc_duration::sf5,
                           supported_meas_gap_patterns{}, // only the mandatory patterns 0 and 1
                           meas_gap_config{0, meas_gap_length::ms6, meas_gap_repetition_period::ms40}},
        // UE does not support the short MGL=1.5ms patterns (20, 21) that the SMTC duration would default to; only gap
        // pattern 10 (MGL=3ms, MGRP=20ms) is supported, so the MGL is escalated from 1.5ms to 3ms.
        gap_pattern_params{"mgl_escalated_when_default_unsupported",
                           subcarrier_spacing::kHz30,
                           ssb_periodicity::ms20,
                           0,
                           smtc_duration::sf1,
                           make_supported_gap_patterns({10}), // pattern 10
                           meas_gap_config{0, meas_gap_length::ms3, meas_gap_repetition_period::ms20}},
        // Best-effort fallback: the SMTC period (160ms) requires MGRP>=160, but the UE supports no MGRP=160 pattern
        // (only the mandatory patterns 0 and 1). The fallback returns the mandatory gap pattern 1 (MGL=6, MGRP=80),
        // which is always supported and encloses the SMTC, even though its MGRP is shorter than the SMTC period.
        gap_pattern_params{"fallback_to_mandatory_pattern",
                           subcarrier_spacing::kHz30,
                           ssb_periodicity::ms160,
                           0,
                           smtc_duration::sf5,
                           supported_meas_gap_patterns{}, // only the mandatory patterns 0 and 1
                           meas_gap_config{0, meas_gap_length::ms6, meas_gap_repetition_period::ms80}}),
    [](const ::testing::TestParamInfo<gap_pattern_params>& test_info) { return std::string{test_info.param.tag}; });

// ---------- NTN uplink timing advance ----------

// 15kHz SCS, so one slot per ms. SMTC every 20ms, but a UL occasion every 80 slots forces MGRP=80, which leaves several
// candidate gap offsets to choose from: 0, 79, 20, 19, 40, 39, 60, 59.
constexpr subcarrier_spacing ntn_scs          = subcarrier_spacing::kHz15;
constexpr unsigned           ul_occasion_slot = 8;

meas_gap_config create_ntn_meas_gap(std::optional<std::chrono::microseconds> ul_ta)
{
  const std::vector<periodic_uci_config> ul_occasions = {periodic_uci_config{80, ul_occasion_slot}};
  return create_meas_gap(ntn_scs,
                         make_smtc(ssb_periodicity::ms20, 0, smtc_duration::sf5),
                         ul_occasions,
                         ul_ta,
                         supported_meas_gap_patterns::all());
}

// In a terrestrial cell the uplink window sits at the gap offset, so the first candidate offset is already free.
TEST(du_meas_config_manager_ntn_test, without_timing_advance_the_gap_is_placed_at_the_smtc_offset)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-MOB-1", "DU-GEN-9");

  const meas_gap_config gap = create_ntn_meas_gap(std::nullopt);

  EXPECT_EQ(0, gap.offset);
  EXPECT_EQ(meas_gap_length::ms6, gap.mgl);
  EXPECT_EQ(meas_gap_repetition_period::ms80, gap.mgrp);
}

// With a 7ms T_TA the uplink window moves 7 slots past the gap offset, onto the UL occasion, so the offset that was
// good enough for a terrestrial cell now collides and another one must be picked.
TEST(du_meas_config_manager_ntn_test, timing_advance_moves_the_gap_off_the_uplink_occasion)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-MOB-1", "DU-GEN-9");

  constexpr std::chrono::microseconds ul_ta{7000};

  const slot_point occasion_slot{ntn_scs, ul_occasion_slot};

  // The offset chosen while ignoring T_TA would in fact have the UE drop the occasion.
  const meas_gap_config unaware_gap = create_ntn_meas_gap(std::nullopt);
  EXPECT_TRUE(is_inside_ul_meas_gap(unaware_gap, occasion_slot, ul_ta));

  // Accounting for T_TA, a different offset is selected and the occasion survives.
  const meas_gap_config gap = create_ntn_meas_gap(ul_ta);
  EXPECT_NE(unaware_gap.offset, gap.offset);
  EXPECT_FALSE(is_inside_ul_meas_gap(gap, occasion_slot, ul_ta));
  // The gap must still enclose an SMTC window, which repeats every 20ms.
  EXPECT_EQ(0, gap.offset % 20);
  EXPECT_EQ(meas_gap_length::ms6, gap.mgl);
  EXPECT_EQ(meas_gap_repetition_period::ms80, gap.mgrp);
}

// ---------- Location measurements ----------

using prs_len = nr_prs_meas_info_r16_s::nr_meas_prs_len_r16_opts::options;

// PRS measurement window of one frequency layer.
struct prs_window {
  unsigned period_ms;
  uint8_t  offset_ms;
  prs_len  len;
};

byte_buffer make_location_meas_info(std::initializer_list<prs_window> windows)
{
  location_meas_info_c info;
  auto&                prs_list = info.set_nr_prs_meas_r16();
  for (const prs_window& w : windows) {
    nr_prs_meas_info_r16_s prs_info;
    prs_info.nr_meas_prs_len_r16.value = w.len;
    auto& repeat_and_offset            = prs_info.nr_meas_prs_repeat_and_offset_r16;
    switch (w.period_ms) {
      case 20:
        repeat_and_offset.set_ms20_r16() = w.offset_ms;
        break;
      case 40:
        repeat_and_offset.set_ms40_r16() = w.offset_ms;
        break;
      case 80:
        repeat_and_offset.set_ms80_r16() = w.offset_ms;
        break;
      default:
        repeat_and_offset.set_ms160_r16() = w.offset_ms;
        break;
    }
    prs_list.push_back(prs_info);
  }
  byte_buffer   buf;
  asn1::bit_ref bref{buf};
  report_fatal_error_if_not(info.pack(bref) == asn1::OCUDUASN_SUCCESS, "Failed to pack LocationMeasurementInfo");
  return buf;
}

// UE capabilities supporting all the gap patterns.
const ue_capability_summary* all_gap_patterns_ue_caps()
{
  static const ue_capability_summary ue_caps = [] {
    ue_capability_summary caps;
    caps.supported_meas_gaps = supported_meas_gap_patterns::all();
    return caps;
  }();
  return &ue_caps;
}

TEST(du_meas_config_manager_location_meas_test, prs_window_is_added_to_the_meas_gap_when_they_fit)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;
  ue_cfg.ssb_meas_gap = meas_gap_config{0, meas_gap_length::ms3, meas_gap_repetition_period::ms40};
  ue_cfg.meas_gap     = ue_cfg.ssb_meas_gap;

  // The gap [0, 3) every 40ms and the PRS window [3, 4.5) every 80ms fit in a 5.5ms gap at offset 0 every 40ms.
  ASSERT_TRUE(mng.update(ue_cfg, {}, make_location_meas_info({{80, 3, prs_len::ms1dot5}}), all_gap_patterns_ue_caps()));

  EXPECT_EQ(ue_cfg.meas_gap, (meas_gap_config{0, meas_gap_length::ms5dot5, meas_gap_repetition_period::ms40}));
}

TEST(du_meas_config_manager_location_meas_test, prs_window_that_does_not_fit_with_the_meas_gap_is_rejected)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;
  const meas_gap_config  ssb_gap{0, meas_gap_length::ms6, meas_gap_repetition_period::ms40};
  ue_cfg.ssb_meas_gap = ssb_gap;
  ue_cfg.meas_gap     = ssb_gap;

  ASSERT_FALSE(mng.update(ue_cfg, {}, make_location_meas_info({{80, 30, prs_len::ms3}}), all_gap_patterns_ue_caps()));

  EXPECT_EQ(ue_cfg.meas_gap, ssb_gap);
  EXPECT_TRUE(ue_cfg.prs_meas_gaps.empty());
}

TEST(du_meas_config_manager_location_meas_test, prs_windows_of_several_layers_share_one_gap)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;

  // At MGRP=20 the windows are [18, 19.5) and [1, 2.5), as 21 mod 20 = 1. The gap wraps around the period: it starts
  // at 18 and spans 4.5ms, which rounds up to MGL=5.5ms.
  ASSERT_TRUE(mng.update(ue_cfg,
                         {},
                         make_location_meas_info({{20, 18, prs_len::ms1dot5}, {40, 21, prs_len::ms1dot5}}),
                         all_gap_patterns_ue_caps()));

  EXPECT_EQ(ue_cfg.meas_gap, (meas_gap_config{18, meas_gap_length::ms5dot5, meas_gap_repetition_period::ms20}));
}

TEST(du_meas_config_manager_location_meas_test, prs_windows_too_far_apart_are_rejected)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;

  // The windows start 15ms apart every 80ms, so they lie at least 5ms apart for any shorter gap period too.
  ASSERT_FALSE(mng.update(ue_cfg,
                          {},
                          make_location_meas_info({{80, 30, prs_len::ms3}, {80, 45, prs_len::ms3}}),
                          all_gap_patterns_ue_caps()));

  EXPECT_FALSE(ue_cfg.meas_gap.has_value());
}

TEST(du_meas_config_manager_location_meas_test, prs_windows_apart_do_not_use_a_10ms_gap)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;

  // The windows [30, 33) and [36, 39) every 80ms span 9ms. A 10ms gap encloses them, but no PRS window needs 10ms.
  ASSERT_FALSE(mng.update(ue_cfg,
                          {},
                          make_location_meas_info({{80, 30, prs_len::ms3}, {80, 36, prs_len::ms3}}),
                          all_gap_patterns_ue_caps()));

  EXPECT_FALSE(ue_cfg.meas_gap.has_value());
}

TEST(du_meas_config_manager_location_meas_test, longer_gap_is_used_when_the_ue_does_not_support_the_shortest_one)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;

  // Without UE capabilities, only the mandatory patterns 0 (MGL=6ms, MGRP=40ms) and 1 (MGL=6ms, MGRP=80ms) apply.
  ASSERT_TRUE(mng.update(ue_cfg, {}, make_location_meas_info({{40, 5, prs_len::ms3}}), nullptr));

  EXPECT_EQ(ue_cfg.meas_gap, (meas_gap_config{5, meas_gap_length::ms6, meas_gap_repetition_period::ms40}));
}

TEST(du_meas_config_manager_location_meas_test, shorter_gap_period_is_used_when_the_ue_does_not_support_the_prs_one)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;

  // No mandatory gap pattern repeats every 160ms, but the one repeating every 80ms still meets every PRS occasion.
  ASSERT_TRUE(mng.update(ue_cfg, {}, make_location_meas_info({{160, 100, prs_len::ms3}}), nullptr));

  EXPECT_EQ(ue_cfg.meas_gap, (meas_gap_config{20, meas_gap_length::ms6, meas_gap_repetition_period::ms80}));
}

TEST(du_meas_config_manager_location_meas_test, prs_length_of_10ms_is_rejected_without_gap_pattern_24)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;

  ASSERT_FALSE(mng.update(ue_cfg, {}, make_location_meas_info({{80, 30, prs_len::ms10}}), nullptr));

  EXPECT_FALSE(ue_cfg.meas_gap.has_value());
}

TEST(du_meas_config_manager_location_meas_test, prs_length_of_10ms_is_signalled_with_mgl_r16)
{
  du_meas_config_manager mng{{}};
  du_ue_resource_config  ue_cfg;

  ASSERT_TRUE(mng.update(ue_cfg, {}, make_location_meas_info({{80, 30, prs_len::ms10}}), all_gap_patterns_ue_caps()));
  ASSERT_EQ(ue_cfg.meas_gap, (meas_gap_config{30, meas_gap_length::ms10, meas_gap_repetition_period::ms80}));

  meas_gap_cfg_s asn1_gap;
  calculate_meas_gap_config_diff(asn1_gap, std::nullopt, ue_cfg.meas_gap);
  ASSERT_TRUE(asn1_gap.gap_ue.is_present());
  const gap_cfg_s& gap_ue = asn1_gap.gap_ue->setup();
  ASSERT_TRUE(gap_ue.mgl_r16_present);
  EXPECT_EQ(gap_ue.mgl_r16.value, gap_cfg_s::mgl_r16_opts::ms10);
  EXPECT_EQ(gap_ue.mgrp.value, gap_cfg_s::mgrp_opts::ms80);
  EXPECT_EQ(gap_ue.gap_offset, 30);
}

class du_meas_config_manager_meas_cfg_with_prs_test : public ::testing::Test
{
protected:
  du_meas_config_manager_meas_cfg_with_prs_test()
  {
    ue_cfg.cell_group.cells.emplace(SERVING_PCELL_IDX, ue_cell_config{});
    ue_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.cell_index = to_du_cell_index(0);
    ue_cfg.cell_group.cells.at(SERVING_PCELL_IDX).bwps.emplace_back();
    ue_caps.supported_meas_gaps = supported_meas_gap_patterns::all();
  }

  // Applies a measConfig with an SSB whose SMTC lasts 1ms every 40ms, at offset 0, together with the given
  // LocationMeasurementInfo. For an inter-frequency SSB, the SSB gap starts at 0 and lasts at most 3ms.
  bool update_meas_cfg(const byte_buffer& location_meas_info, bool inter_freq = true)
  {
    meas_cfg_s meas_cfg;
    meas_cfg.meas_obj_to_add_mod_list.resize(1);
    auto& meas_obj_nr            = meas_cfg.meas_obj_to_add_mod_list[0].meas_obj.set_meas_obj_nr();
    meas_obj_nr.ssb_freq_present = true;
    meas_obj_nr.ssb_freq         = cell_cfgs[0].ran.dl_cfg_common.freq_info_dl.absolute_frequency_ssb.value();
    if (inter_freq) {
      meas_obj_nr.ssb_freq += 100;
    }
    meas_obj_nr.smtc1_present = true;
    meas_obj_nr.smtc1         = make_smtc(ssb_periodicity::ms40, 0, smtc_duration::sf1);

    byte_buffer   buf;
    asn1::bit_ref bref{buf};
    report_fatal_error_if_not(meas_cfg.pack(bref) == asn1::OCUDUASN_SUCCESS, "Failed to pack measConfig");
    return mng.update(ue_cfg, buf, location_meas_info, &ue_caps);
  }

  std::vector<du_cell_config> cell_cfgs{config_helpers::make_default_du_cell_config()};
  du_meas_config_manager      mng{cell_cfgs};
  du_ue_resource_config       ue_cfg;
  ue_capability_summary       ue_caps;
};

TEST_F(du_meas_config_manager_meas_cfg_with_prs_test, gap_encloses_the_ssb_and_the_prs)
{
  ASSERT_TRUE(update_meas_cfg(make_location_meas_info({{80, 3, prs_len::ms1dot5}})));

  ASSERT_TRUE(ue_cfg.ssb_meas_gap.has_value());
  EXPECT_EQ(ue_cfg.meas_gap, (meas_gap_config{0, meas_gap_length::ms5dot5, meas_gap_repetition_period::ms40}));
}

TEST_F(du_meas_config_manager_meas_cfg_with_prs_test, prs_that_do_not_fit_with_the_ssb_are_rejected)
{
  // The PRS starts 10ms after the SSB gap for every gap period, too far apart to share a gap.
  EXPECT_FALSE(update_meas_cfg(make_location_meas_info({{80, 10, prs_len::ms3}})));
}

TEST_F(du_meas_config_manager_meas_cfg_with_prs_test, meas_cfg_without_location_meas_info_removes_the_prs)
{
  ASSERT_TRUE(update_meas_cfg(make_location_meas_info({{80, 3, prs_len::ms1dot5}})));

  ASSERT_TRUE(update_meas_cfg({}));

  ASSERT_TRUE(ue_cfg.ssb_meas_gap.has_value());
  EXPECT_EQ(ue_cfg.meas_gap, ue_cfg.ssb_meas_gap);
  EXPECT_TRUE(ue_cfg.prs_meas_gaps.empty());
}

TEST_F(du_meas_config_manager_meas_cfg_with_prs_test, intra_freq_meas_cfg_without_location_meas_info_releases_the_gap)
{
  ASSERT_TRUE(update_meas_cfg(make_location_meas_info({{80, 3, prs_len::ms1dot5}}), /* inter_freq */ false));
  ASSERT_TRUE(ue_cfg.meas_gap.has_value());

  ASSERT_TRUE(update_meas_cfg({}, /* inter_freq */ false));

  EXPECT_FALSE(ue_cfg.meas_gap.has_value());
}

} // namespace
