// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "tests/ocudu_test_requirements.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "tests/unittests/scheduler/test_utils/scheduler_test_suite.h"
#include "uci_test_utils.h"
#include "ocudu/ran/pucch/pucch_configuration.h"
#include <gtest/gtest.h>

using namespace ocudu;

/////////////  Test SR scheduling   /////////////

class uci_sched_sr_test : public ::testing::TestWithParam<sr_periodicity>
{
public:
  uci_sched_sr_test() :
    sr_period(GetParam()),
    sr_offset(test_rng::uniform_int<unsigned>(0, sr_periodicity_to_slot(GetParam()) - 1)),
    t_bench{test_bench_params{.pucch_res_common = pucch_res_common,
                              .n_cces           = n_cces,
                              .sr_period        = sr_period,
                              .sr_offset        = sr_offset}}
  {
    csi_offset = t_bench.get_main_ue().get_pcell().cfg().init_bwp().ul.ue_cfg()->csi.periodic_report->offset;
    csi_period = t_bench.cell_cfg.params.init_bwp.csi->csi_rs_period;

    // Set the expected SR grant to the SR resource.
    pucch_sr_only_test = test_helpers::make_pucch_info(t_bench.cell_cfg,
                                                       t_bench.cell_cfg.bwp_res[to_bwp_id(0)].ul().pucch.dedicated[6],
                                                       {.sr_bits = sr_nof_bits::one});

    // Set the expected HARQ CSI grant to the CSI resource.
    pucch_sr_csi_test =
        test_helpers::make_pucch_info(t_bench.cell_cfg,
                                      t_bench.cell_cfg.bwp_res[to_bwp_id(0)].ul().pucch.dedicated[14],
                                      {.sr_bits = sr_nof_bits::one, .csi_part1_nof_bits = default_csi_part1_bits});
  }

protected:
  // Parametrized variables.
  sr_periodicity sr_period;
  unsigned       sr_offset;
  // Helper variables.
  csi_resource_periodicity csi_period;
  unsigned                 csi_offset;
  unsigned                 pucch_res_common{11};
  unsigned                 n_cces{0};
  test_bench               t_bench;
  // Expected PUCCH grants to be verified in the test.
  pucch_info pucch_sr_only_test;
  pucch_info pucch_sr_csi_test;

  static constexpr unsigned default_csi_part1_bits = 4;
};

TEST_P(uci_sched_sr_test, test_different_periods)
{
  // Check at the allocation for at least 2 the size of the resource grid.
  const unsigned nof_slots_to_test = std::max(sr_periodicity_to_slot(sr_period) * 4, t_bench.res_grid.ring_size() * 2);

  // Randomize initial slot, as the UCI scheduler will be called only after the UE is added.
  const auto starting_slot = test_rng::uniform_int<unsigned>(0, 1000U);
  for (unsigned sl_cnt = starting_slot; sl_cnt < starting_slot + nof_slots_to_test; ++sl_cnt) {
    t_bench.uci_sched.run_slot(t_bench.res_grid);
    if ((t_bench.sl_tx - sr_offset).count() % sr_periodicity_to_slot(sr_period) == 0) {
      ASSERT_EQ(1, t_bench.res_grid[0].result_reader().ul().pucchs().size());
      // The scheduler allocates:
      // - SR only on slots that are for SR only.
      // - CSI + SR on slots that are for CSI + SR.
      if ((t_bench.sl_tx - csi_offset).count() % to_underlying(csi_period) == 0) {
        ASSERT_TRUE(find_pucch_pdu(
            t_bench.res_grid[0].result_reader().ul().pucchs().unsorted(),
            [&expected = pucch_sr_csi_test](const auto& pdu) { return pucch_info_match(expected, pdu); }));
      } else {
        ASSERT_TRUE(find_pucch_pdu(
            t_bench.res_grid[0].result_reader().ul().pucchs().unsorted(),
            [&expected = pucch_sr_only_test](const auto& pdu) { return pucch_info_match(expected, pdu); }));
      }
    }
    // Update the slot indicator.
    t_bench.slot_indication(++t_bench.sl_tx);
  }
}

INSTANTIATE_TEST_SUITE_P(test_sr_sched_different_periods_offsets,
                         uci_sched_sr_test,
                         testing::Values(sr_periodicity::sl_1,
                                         sr_periodicity::sl_2,
                                         sr_periodicity::sl_4,
                                         sr_periodicity::sl_5,
                                         sr_periodicity::sl_8,
                                         sr_periodicity::sl_16,
                                         sr_periodicity::sl_20,
                                         sr_periodicity::sl_40,
                                         sr_periodicity::sl_80,
                                         sr_periodicity::sl_160,
                                         sr_periodicity::sl_320,
                                         sr_periodicity::sl_640));

/////////////  Test SR scheduling in an NTN cell   /////////////

/// \brief Base fixture for the periodic SR occasion of an NTN cell. The UE derives the occasion from its SR
/// configuration (TS 38.331, Information Element \e SchedulingRequestResourceConfig), so only k_mac separates the slot
/// the UE transmits in from the slot the gNB receives in. CSI reporting is disabled, so every PUCCH in the grid belongs
/// to an SR occasion.
class uci_sched_ntn_test_base : public ::testing::Test
{
public:
  static constexpr sr_periodicity sr_period = sr_periodicity::sl_40;
  static constexpr unsigned       sr_offset = 7;

  uci_sched_ntn_test_base(unsigned k_mac_ms, unsigned cs_koffset_ms) :
    t_bench{test_bench_params{.sr_period      = sr_period,
                              .sr_offset      = sr_offset,
                              .csi_period     = std::nullopt,
                              .ntn_cs_koffset = std::chrono::milliseconds{cs_koffset_ms},
                              .ntn_k_mac      = std::chrono::milliseconds{k_mac_ms}}}
  {
  }

protected:
  /// Runs the scheduler over several SR periods and returns whether an SR occasion was seen, failing the test on any
  /// PUCCH that falls outside \c expected_offset.
  void run_and_check_sr_offset(unsigned expected_offset)
  {
    const unsigned period_slots      = sr_periodicity_to_slot(sr_period);
    const unsigned nof_slots_to_test = std::max(period_slots * 4, t_bench.res_grid.ring_size() * 2);
    bool           observed_sr       = false;

    for (unsigned sl_cnt = 0; sl_cnt != nof_slots_to_test; ++sl_cnt) {
      t_bench.uci_sched.run_slot(t_bench.res_grid);
      const bool is_sr_slot = t_bench.sl_tx.count() % period_slots == expected_offset;
      if (is_sr_slot) {
        ASSERT_EQ(t_bench.res_grid[0].result_reader().ul().pucchs().size(), 1)
            << fmt::format("No SR at slot {}, the expected occasion", t_bench.sl_tx);
        observed_sr = true;
      } else {
        ASSERT_TRUE(t_bench.res_grid[0].result_reader().ul().pucchs().empty())
            << fmt::format("SR at slot {}, away from the expected occasion", t_bench.sl_tx);
      }
      t_bench.slot_indication(++t_bench.sl_tx);
    }
    ASSERT_TRUE(observed_sr);
  }

  test_bench t_bench;
};

/// Fixture for an NTN cell whose k_mac is not a multiple of the SR periodicity, so an unshifted occasion cannot pass by
/// coincidence.
class uci_sched_ntn_kmac_test : public uci_sched_ntn_test_base
{
public:
  static constexpr unsigned K_MAC_MS = 19;

  uci_sched_ntn_kmac_test() : uci_sched_ntn_test_base(K_MAC_MS, /*cs_koffset_ms=*/0) {}
};

/// Verifies that k_mac shifts the SR occasion onto the slot the gNB receives it in.
TEST_F(uci_sched_ntn_kmac_test, periodic_sr_is_shifted_by_k_mac)
{
  const unsigned k_mac_slots = t_bench.cell_cfg.ntn_k_mac_slots;
  ASSERT_GT(k_mac_slots, 0U);
  ASSERT_EQ(t_bench.cell_cfg.ntn_cs_koffset, 0U);

  const unsigned period_slots = sr_periodicity_to_slot(sr_period);
  ASSERT_NE(k_mac_slots % period_slots, 0U) << "k_mac must shift the occasion, or the test cannot tell the two apart";

  run_and_check_sr_offset((sr_offset + k_mac_slots) % period_slots);
}

/// Fixture for an NTN cell carrying a cell-specific K-offset and no k_mac.
class uci_sched_ntn_koffset_test : public uci_sched_ntn_test_base
{
public:
  static constexpr unsigned KOFFSET_MS = 5;

  uci_sched_ntn_koffset_test() : uci_sched_ntn_test_base(/*k_mac_ms=*/0, KOFFSET_MS) {}
};

/// \brief Verifies that the cell-specific K-offset leaves the SR occasion at its configured offset.
///
/// The occasion follows from an SFN and slot modulo condition on SR_PERIODICITY and SR_OFFSET (TS 38.213,
/// Section 9.2.4), so there is no "slot n+k" relation for a K-offset to enter.
TEST_F(uci_sched_ntn_koffset_test, periodic_sr_is_not_shifted_by_the_cell_specific_koffset)
{
  ASSERT_GT(t_bench.cell_cfg.ntn_cs_koffset, 0U);
  ASSERT_EQ(t_bench.cell_cfg.ntn_k_mac_slots, 0U);

  const unsigned period_slots = sr_periodicity_to_slot(sr_period);
  ASSERT_NE(t_bench.cell_cfg.ntn_cs_koffset % period_slots, 0U)
      << "the K-offset must be able to move the occasion, or the test cannot tell the two apart";

  run_and_check_sr_offset(sr_offset % period_slots);
}

/// \brief Fixture for the periodic CSI report occasion of an NTN cell. The UE derives the occasion from its own
/// report configuration (TS 38.214, Section 5.2.1.4), so, as for the periodic SR, only k_mac separates the slot the UE
/// transmits in from the slot the gNB receives in. The k_mac is not a multiple of the CSI periodicity, so an unshifted
/// occasion cannot pass by coincidence.
class uci_sched_ntn_kmac_csi_test : public ::testing::Test
{
public:
  static constexpr unsigned                 K_MAC_MS   = 19;
  static constexpr sr_periodicity           sr_period  = sr_periodicity::sl_40;
  static constexpr unsigned                 sr_offset  = 7;
  static constexpr csi_resource_periodicity csi_period = csi_resource_periodicity::slots40;
  static constexpr unsigned                 csi_offset = 11;

  uci_sched_ntn_kmac_csi_test() :
    t_bench{test_bench_params{.sr_period  = sr_period,
                              .sr_offset  = sr_offset,
                              .csi_period = csi_period,
                              .csi_offset = csi_offset,
                              .ntn_k_mac  = std::chrono::milliseconds{K_MAC_MS}}}
  {
  }

protected:
  test_bench t_bench;
};

/// Verifies that k_mac shifts the CSI report occasion onto the slot the gNB receives it in.
TEST_F(uci_sched_ntn_kmac_csi_test, periodic_csi_report_is_shifted_by_k_mac)
{
  const unsigned k_mac_slots = t_bench.cell_cfg.ntn_k_mac_slots;
  ASSERT_GT(k_mac_slots, 0U);
  ASSERT_EQ(t_bench.cell_cfg.ntn_cs_koffset, 0U);

  const unsigned csi_period_slots = to_underlying(csi_period);
  const unsigned sr_period_slots  = sr_periodicity_to_slot(sr_period);
  ASSERT_NE(k_mac_slots % csi_period_slots, 0U) << "k_mac must shift the occasion, or the test cannot tell the two "
                                                   "apart";

  // The SR occasion is shifted the same way, and has an offset of its own, so its slots are left out of the check.
  const unsigned expected_csi_offset = (csi_offset + k_mac_slots) % csi_period_slots;
  const unsigned expected_sr_offset  = (sr_offset + k_mac_slots) % sr_period_slots;
  ASSERT_NE(expected_csi_offset, expected_sr_offset) << "the two occasions share a slot, so neither can be told apart";

  const unsigned nof_slots_to_test = std::max(csi_period_slots * 4, t_bench.res_grid.ring_size() * 2);
  bool           observed_csi      = false;
  for (unsigned sl_cnt = 0; sl_cnt != nof_slots_to_test; ++sl_cnt) {
    t_bench.uci_sched.run_slot(t_bench.res_grid);
    const bool is_csi_slot = t_bench.sl_tx.count() % csi_period_slots == expected_csi_offset;
    const bool is_sr_slot  = t_bench.sl_tx.count() % sr_period_slots == expected_sr_offset;
    if (is_csi_slot) {
      ASSERT_EQ(t_bench.res_grid[0].result_reader().ul().pucchs().size(), 1)
          << fmt::format("No CSI report at slot {}, the expected occasion", t_bench.sl_tx);
      observed_csi = true;
    } else if (not is_sr_slot) {
      ASSERT_TRUE(t_bench.res_grid[0].result_reader().ul().pucchs().empty())
          << fmt::format("PUCCH at slot {}, away from the expected occasions", t_bench.sl_tx);
    }
    t_bench.slot_indication(++t_bench.sl_tx);
  }
  ASSERT_TRUE(observed_csi);
}

/////////////  Test SR scheduling with a UE measurement gap   /////////////

/// \brief A UE does not transmit on an SR occasion that overlaps its measurement gap (TS 38.321, Section 5.4.4), so the
/// scheduler must not place an SR opportunity there.
class uci_sched_meas_gap_test : public ::testing::Test
{
public:
  uci_sched_meas_gap_test() :
    t_bench{test_bench_params{.sr_period = sr_period, .sr_offset = 0, .csi_period = std::nullopt, .meas_gap = test_gap}}
  {
    OCUDU_TEST_REQUIREMENTS("DU-GEN-9");
  }

protected:
  /// 6ms gap every 40ms, starting at subframe 0. The cell is FDD at 15kHz, so there is one slot per subframe.
  static constexpr meas_gap_config test_gap{0, meas_gap_length::ms6, meas_gap_repetition_period::ms40};
  /// Short enough that a single gap period holds SR occasions both inside and outside the gap.
  static constexpr sr_periodicity sr_period = sr_periodicity::sl_4;

  /// \brief Whether the UE uplink is disabled in the given slot.
  ///
  /// The gap spans its 6 slots, plus the trailing slot that the uplink window adds for the gap edge reaching into the
  /// next slot. The cell tracks no T_TA here, so the window is not shifted.
  static bool is_ue_ul_disabled(slot_point sl) { return sl.count() % 40 < 7; }

  test_bench t_bench;
};

TEST_F(uci_sched_meas_gap_test, sr_opportunities_overlapping_the_ue_meas_gap_are_not_allocated)
{
  // Two gap periods, so that the slots pre-scheduled when the UE was added and those scheduled slot by slot are both
  // covered.
  const unsigned nof_slots_to_test = 2 * 40;

  bool tested_inside_gap = false, tested_outside_gap = false;
  for (unsigned sl_cnt = 0; sl_cnt != nof_slots_to_test; ++sl_cnt) {
    t_bench.uci_sched.run_slot(t_bench.res_grid);

    if (t_bench.sl_tx.count() % sr_periodicity_to_slot(sr_period) == 0) {
      // SR occasion.
      if (is_ue_ul_disabled(t_bench.sl_tx)) {
        ASSERT_EQ(0, t_bench.res_grid[0].result_reader().ul().pucchs().size())
            << "no SR must be scheduled in slot " << t_bench.sl_tx.count() << ", inside the UE measurement gap";
        tested_inside_gap = true;
      } else {
        ASSERT_EQ(1, t_bench.res_grid[0].result_reader().ul().pucchs().size())
            << "the SR must be scheduled in slot " << t_bench.sl_tx.count() << ", outside the UE measurement gap";
        tested_outside_gap = true;
      }
    }

    t_bench.slot_indication(++t_bench.sl_tx);
  }

  ASSERT_TRUE(tested_inside_gap and tested_outside_gap) << "the test covered only one side of the gap";
}

/////////////  Test CSI scheduling   /////////////

class uci_sched_csi_test : public ::testing::TestWithParam<csi_resource_periodicity>
{
public:
  uci_sched_csi_test() :
    csi_period(GetParam()),
    csi_offset(test_rng::uniform_int<unsigned>(0, to_underlying(GetParam()) - 1)),
    t_bench{test_bench_params{.csi_period = csi_period, .csi_offset = csi_offset}}
  {
    OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-1");

    sr_period = sr_periodicity_to_slot(
        t_bench.get_main_ue().get_pcell().cfg().init_bwp().ul.ded()->pucch_cfg.value().sr_res_list[0].period);

    sr_offset = t_bench.get_main_ue().get_pcell().cfg().init_bwp().ul.ded()->pucch_cfg.value().sr_res_list[0].offset;

    // In the slots with SR + CSI, the expected format is Format 2.
    pucch_csi_and_sr_test =
        test_helpers::make_pucch_info(t_bench.cell_cfg,
                                      t_bench.cell_cfg.bwp_res[to_bwp_id(0)].ul().pucch.dedicated[14],
                                      {.sr_bits = sr_nof_bits::one, .csi_part1_nof_bits = default_csi_part1_bits});

    pucch_csi_only_test                  = pucch_csi_and_sr_test;
    pucch_csi_only_test.uci_bits.sr_bits = sr_nof_bits::no_sr;
  }

protected:
  // Parametrized variables.
  csi_resource_periodicity csi_period;
  unsigned                 csi_offset;
  // Helper variables.
  unsigned   sr_period;
  unsigned   sr_offset{0};
  test_bench t_bench;
  // Expected PUCCH grants to be verified in the test.
  pucch_info pucch_csi_and_sr_test;
  pucch_info pucch_csi_only_test;

  static constexpr unsigned default_csi_part1_bits = 4;
};

TEST_P(uci_sched_csi_test, test_different_periods)
{
  // Check at the allocation for at least 2 the size of the resource grid.
  const unsigned nof_slots_to_test =
      std::max(to_underlying(csi_period) * 8, static_cast<unsigned>(t_bench.res_grid.max_ul_slot_alloc_delay) * 2);

  // Randomize initial slot, as the UCI scheduler will be called only after the UE is added.
  const auto starting_slot = test_rng::uniform_int<unsigned>(0, 1000U);
  for (unsigned sl_cnt = starting_slot; sl_cnt < starting_slot + nof_slots_to_test; ++sl_cnt) {
    t_bench.uci_sched.run_slot(t_bench.res_grid);
    if ((t_bench.sl_tx - csi_offset).count() % to_underlying(csi_period) == 0) {
      ASSERT_EQ(1, t_bench.res_grid[0].result_reader().ul().pucchs().size());
      // The scheduler allocates:
      // - CSI only on slots that are for CSI only.
      // - CSI + SR on slots that are for CSI + SR.
      if ((t_bench.sl_tx - sr_offset).count() % sr_period == 0) {
        ASSERT_TRUE(find_pucch_pdu(
            t_bench.res_grid[0].result_reader().ul().pucchs().unsorted(),
            [&expected = pucch_csi_and_sr_test](const auto& pdu) { return pucch_info_match(expected, pdu); }));
      } else {
        ASSERT_TRUE(find_pucch_pdu(
            t_bench.res_grid[0].result_reader().ul().pucchs().unsorted(),
            [&expected = pucch_csi_only_test](const auto& pdu) { return pucch_info_match(expected, pdu); }));
      }
    }
    // Update the slot indicator.
    t_bench.slot_indication(++t_bench.sl_tx);
  }
}

INSTANTIATE_TEST_SUITE_P(test_csi_sched_different_periods_offsets,
                         uci_sched_csi_test,
                         testing::Values(csi_report_periodicity::slots16,
                                         csi_report_periodicity::slots20,
                                         csi_report_periodicity::slots40,
                                         csi_report_periodicity::slots80,
                                         csi_report_periodicity::slots160,
                                         csi_report_periodicity::slots320));

/////////////  Test UCI with UE reconfiguration   /////////////

class uci_sched_reconf_test : public ::testing::TestWithParam<std::optional<csi_resource_periodicity>>
{
public:
  uci_sched_reconf_test() : t_bench{test_bench_params{.csi_period = GetParam()}} {}

protected:
  // Helper variables.
  unsigned   sr_period_slots{sr_periodicity_to_slot(test_bench_params().sr_period)};
  unsigned   sr_offset{test_bench_params().sr_offset};
  test_bench t_bench;
};

TEST_P(uci_sched_reconf_test, after_ue_reconf_uci_doesnt_stopped_being_scheduled)
{
  const unsigned nof_slots_to_test = sr_period_slots * 4;

  for (unsigned sl_cnt = 0; sl_cnt < nof_slots_to_test; ++sl_cnt) {
    t_bench.uci_sched.run_slot(t_bench.res_grid);
    if ((t_bench.sl_tx - sr_offset).count() % sr_period_slots == 0) {
      ASSERT_EQ(1, t_bench.res_grid[0].result_reader().ul().pucchs().size());
    }
    // Update the slot indicator.
    t_bench.slot_indication(++t_bench.sl_tx);
  }

  // This is the reconfiguration of the UE; we are interested in checking that the UCI scheduler does not crash
  // after the reconfiguration, more than the testing that the configuration has been changed.
  t_bench.uci_sched.reconf_ue(t_bench.get_main_ue().get_pcell().cfg(), t_bench.get_main_ue().get_pcell().cfg());

  // After reconfiguration, the UCI scheduler should still be able to schedule the UCI.
  for (unsigned sl_cnt = 0; sl_cnt < nof_slots_to_test; ++sl_cnt) {
    t_bench.uci_sched.run_slot(t_bench.res_grid);
    if ((t_bench.sl_tx - sr_offset).count() % sr_period_slots == 0) {
      ASSERT_EQ(1, t_bench.res_grid[0].result_reader().ul().pucchs().size());
    }
    // Update the slot indicator.
    t_bench.slot_indication(++t_bench.sl_tx);
  }
}

INSTANTIATE_TEST_SUITE_P(test_with_and_without_csi,
                         uci_sched_reconf_test,
                         testing::Values(std::nullopt, csi_resource_periodicity::slots320));
