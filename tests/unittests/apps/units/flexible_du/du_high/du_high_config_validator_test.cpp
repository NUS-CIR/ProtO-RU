// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "apps/units/flexible_o_du/o_du_high/du_high/du_high_config_cli11_schema.h"
#include "apps/units/flexible_o_du/o_du_high/du_high/du_high_config_validator.h"
#include "tests/ocudu_test_requirements.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/configured_grant/cg_configuration.h"
#include "ocudu/ran/ntn.h"
#include "ocudu/ran/pusch/pusch_constants.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace std::chrono_literals;

// The receiver soft-combines every reception of a transport block into one buffer -- the occasions of a repetition
// bundle and the HARQ retransmissions of that bundle alike -- and admits a bounded number of them. A configuration
// asking for more is rejected at startup rather than left to discard occasions silently in the PHY.
class du_high_pusch_repetition_config_test : public ::testing::Test
{
protected:
  du_high_pusch_repetition_config_test() : app("du_high config validator test")
  {
    // Build the configuration the way the application does: several fields are filled in by the CLI11 schema and the
    // auto-derivation step rather than defaulted in the struct, and the validator expects them to be set.
    configure_cli11_with_du_high_config_schema(app, parsed_cfg);
    app.parse("");
    autoderive_du_high_parameters_after_parsing(parsed_cfg.config);
  }

  du_high_unit_pusch_config& pusch_cfg() { return parsed_cfg.config.cells_cfg.front().cell.pusch_cfg; }

  bool validate() const { return validate_du_high_config(parsed_cfg.config); }

  CLI::App              app;
  du_high_parsed_config parsed_cfg;
};

TEST_F(du_high_pusch_repetition_config_test, default_configuration_is_valid)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  ASSERT_TRUE(validate());
}

TEST_F(du_high_pusch_repetition_config_test, repetitions_within_the_soft_combining_budget_are_valid)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  // (1 + 3) * 8 == 32, exactly the budget.
  pusch_cfg().max_nof_harq_retxs = 3;
  pusch_cfg().max_nof_rep        = 8;

  ASSERT_EQ((1U + pusch_cfg().max_nof_harq_retxs) * pusch_cfg().max_nof_rep,
            pusch_constants::MAX_NOF_SOFT_COMBINED_PUSCH_TXS);
  ASSERT_TRUE(validate());
}

TEST_F(du_high_pusch_repetition_config_test, repetitions_beyond_the_soft_combining_budget_are_rejected)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  // (1 + 4) * 8 == 40, over the budget by one retransmission.
  pusch_cfg().max_nof_harq_retxs = 4;
  pusch_cfg().max_nof_rep        = 8;

  ASSERT_FALSE(validate());
}

TEST_F(du_high_pusch_repetition_config_test, the_initial_transmission_counts_towards_the_budget)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-SVCS-16-8-d");

  // (1 + 1) * 16 == 32 fits, while (1 + 2) * 16 == 48 does not: the budget covers the initial transmission too, not
  // just the retransmissions.
  pusch_cfg().max_nof_rep        = 16;
  pusch_cfg().max_nof_harq_retxs = 1;
  ASSERT_TRUE(validate());

  pusch_cfg().max_nof_harq_retxs = 2;
  ASSERT_FALSE(validate());
}

class du_high_pdsch_res_alloc_config_test : public ::testing::Test
{
protected:
  du_high_pdsch_res_alloc_config_test() : app("du_high config validator test")
  {
    configure_cli11_with_du_high_config_schema(app, parsed_cfg);
    app.parse("");
    autoderive_du_high_parameters_after_parsing(parsed_cfg.config);
  }

  du_high_unit_pdsch_config& pdsch_cfg() { return parsed_cfg.config.cells_cfg.front().cell.pdsch_cfg; }

  bool validate() const { return validate_du_high_config(parsed_cfg.config); }

  CLI::App              app;
  du_high_parsed_config parsed_cfg;
};

TEST_F(du_high_pdsch_res_alloc_config_test, resource_allocation_type0_without_interleaving_is_valid)
{
  pdsch_cfg().res_alloc = pdsch_config::resource_allocation::resource_allocation_type_0;
  ASSERT_TRUE(validate());

  pdsch_cfg().res_alloc = pdsch_config::resource_allocation::dynamic_switch;
  ASSERT_TRUE(validate());
}

TEST_F(du_high_pdsch_res_alloc_config_test, resource_allocation_type0_with_interleaving_is_rejected)
{
  pdsch_cfg().interleaving_bundle_size = vrb_to_prb::mapping_type::interleaved_n2;
  pdsch_cfg().res_alloc                = pdsch_config::resource_allocation::resource_allocation_type_1;
  ASSERT_TRUE(validate());

  pdsch_cfg().res_alloc = pdsch_config::resource_allocation::resource_allocation_type_0;
  ASSERT_FALSE(validate());

  pdsch_cfg().res_alloc = pdsch_config::resource_allocation::dynamic_switch;
  ASSERT_FALSE(validate());
}

// The Differential Koffset of a UE is derived from the T_TA it last reported, so the T_TA grows past what the UE was
// told between two reports. The margin is what keeps the Koffset it applies covering its T_TA meanwhile.
class du_high_ue_koffset_config_test : public ::testing::Test
{
protected:
  du_high_ue_koffset_config_test() : app("du_high ue koffset config test")
  {
    configure_cli11_with_du_high_config_schema(app, parsed_cfg);
    app.parse("");
    parsed_cfg.config.cells_cfg.resize(1);

    du_high_unit_base_cell_config& c = parsed_cfg.config.cells_cfg[0].cell;
    c.dl_f_ref_arfcn                 = 437000;
    c.band                           = nr_band::n256;
    // An NTN cell must broadcast SIB19, which in turn needs an SI window position and a SIB below SIB15 before it.
    c.sib_cfg.si_window_len_slots = 5;
    c.sib_cfg.si_sched_info       = {{.sib_mapping_info = {2}, .si_period_rf = 16},
                                     {.sib_mapping_info = {19}, .si_period_rf = 16, .si_window_position = 2}};
    c.sib_cfg.sib2_cfg            = du_high_unit_sib_config::sib2_config{};

    auto& serving                       = c.ntn_cfg.emplace().serving.emplace();
    serving.cell_specific_koffset       = 40ms;
    serving.sat_ref.epoch_timestamp     = std::chrono::system_clock::time_point{};
    serving.sat_ref.ephemeris_info      = ecef_coordinates_t{.position_x  = 20922195,
                                                             .position_y  = 1967783,
                                                             .position_z  = 19770302,
                                                             .velocity_vx = 0,
                                                             .velocity_vy = 0,
                                                             .velocity_vz = 0};
    serving.ue_specific_koffset_enabled = true;
  }

  void set_koffset_params(float threshold, std::chrono::milliseconds margin)
  {
    auto& serving                      = *parsed_cfg.config.cells_cfg[0].cell.ntn_cfg->serving;
    serving.ta_report_offset_threshold = threshold;
    serving.ue_specific_koffset_margin = margin;
  }

  bool validate()
  {
    unit_cfg = parsed_cfg.config;
    autoderive_du_high_parameters_after_parsing(unit_cfg);
    return validate_du_high_config(unit_cfg);
  }

  CLI::App              app;
  du_high_parsed_config parsed_cfg;
  du_high_unit_config   unit_cfg;
};

TEST_F(du_high_ue_koffset_config_test, a_margin_covering_the_threshold_is_valid)
{
  // 1ms of drift between reports, and a margin that covers it.
  set_koffset_params(1.0F, 2ms);

  ASSERT_TRUE(validate());
}

TEST_F(du_high_ue_koffset_config_test, a_margin_short_of_the_threshold_is_rejected)
{
  set_koffset_params(4.0F, 2ms);

  ASSERT_FALSE(validate());
}

TEST_F(du_high_ue_koffset_config_test, a_disabled_dl_harq_feedback_is_rejected)
{
  set_koffset_params(1.0F, 2ms);
  parsed_cfg.config.cells_cfg[0].cell.pdsch_cfg.harq_feedback_disabled.set(0, true);

  ASSERT_FALSE(validate());
}

TEST_F(du_high_ue_koffset_config_test, an_uplink_harq_process_in_mode_b_is_rejected)
{
  set_koffset_params(1.0F, 2ms);
  parsed_cfg.config.cells_cfg[0].cell.pusch_cfg.harq_mode_b.set(0, true);

  ASSERT_FALSE(validate());
}

TEST_F(du_high_ue_koffset_config_test, a_margin_matching_the_threshold_is_valid)
{
  // The T_TA report rounds up when the Koffset is derived from it, so a margin equal to the threshold still covers
  // the drift until the UE reports again.
  set_koffset_params(2.0F, 2ms);

  ASSERT_TRUE(validate());
}

TEST_F(du_high_ue_koffset_config_test, a_wider_threshold_asks_for_a_wider_margin)
{
  set_koffset_params(2.0F, 4ms);
  ASSERT_TRUE(validate());

  // The margin has not moved, and the T_TA now drifts a millisecond more between reports than the margin covers.
  set_koffset_params(5.0F, 4ms);
  ASSERT_FALSE(validate());
}

// A Type 2 CG is activated by a DCI format 0_1 scrambled with the CS-RNTI, which only a UE-specific SearchSpace
// monitored with that format can carry. A configuration that leaves SearchSpace#2 without it is rejected at startup,
// rather than every UE failing the reconfiguration that adds its CG.
class du_high_cg_type2_config_test : public ::testing::Test
{
protected:
  du_high_cg_type2_config_test() : app("du_high config validator test")
  {
    configure_cli11_with_du_high_config_schema(app, parsed_cfg);
    app.parse("");
    autoderive_du_high_parameters_after_parsing(parsed_cfg.config);
    cell().cg_cfg.periodicity_slots = 40;
    cell().cg_cfg.type2             = true;
  }

  du_high_unit_base_cell_config& cell() { return parsed_cfg.config.cells_cfg.front().cell; }

  bool validate() const { return validate_du_high_config(parsed_cfg.config); }

  CLI::App              app;
  du_high_parsed_config parsed_cfg;
};

TEST_F(du_high_cg_type2_config_test, ue_specific_search_space_with_dci_f0_1_is_valid)
{
  ASSERT_TRUE(validate());
}

TEST_F(du_high_cg_type2_config_test, common_search_space_is_rejected)
{
  cell().pdcch_cfg.dedicated.ss2_type               = search_space_configuration::type_t::common;
  cell().pdcch_cfg.dedicated.dci_format_0_1_and_1_1 = false;
  ASSERT_FALSE(validate());
}

TEST_F(du_high_cg_type2_config_test, fallback_dci_formats_are_rejected)
{
  cell().pdcch_cfg.dedicated.dci_format_0_1_and_1_1 = false;
  ASSERT_FALSE(validate());
}

// The scheduler checks the CG configuration of each UE when the reconfiguration that adds its CG is applied. A cell
// configuration that every UE CG would fail is rejected at startup instead.
class du_high_cg_config_test : public ::testing::TestWithParam<bool>
{
protected:
  du_high_cg_config_test() : app("du_high config validator test")
  {
    configure_cli11_with_du_high_config_schema(app, parsed_cfg);
    app.parse("");
    autoderive_du_high_parameters_after_parsing(parsed_cfg.config);
    cell().cg_cfg.periodicity_slots = 40;
    cell().cg_cfg.type2             = GetParam();
  }

  du_high_unit_base_cell_config& cell() { return parsed_cfg.config.cells_cfg.front().cell; }

  bool validate() const { return validate_du_high_config(parsed_cfg.config); }

  CLI::App              app;
  du_high_parsed_config parsed_cfg;
};

TEST_P(du_high_cg_config_test, default_cg_config_is_valid)
{
  ASSERT_TRUE(validate());
}

TEST_P(du_high_cg_config_test, fewer_cg_harq_processes_than_the_configured_grant_timer_are_rejected)
{
  cell().cg_cfg.nof_harq_processes = cg_configuration::configured_grant_timer;
  ASSERT_TRUE(validate());

  cell().cg_cfg.nof_harq_processes = cg_configuration::configured_grant_timer - 1;
  ASSERT_FALSE(validate());
}

TEST_P(du_high_cg_config_test, pusch_transform_precoding_is_rejected)
{
  cell().pusch_cfg.enable_transform_precoding = true;
  ASSERT_FALSE(validate());
}

TEST_P(du_high_cg_config_test, cg_period_without_dynamic_ul_slots_in_the_tdd_period_is_rejected)
{
  // Move the cell to a TDD band and derive again the band-dependent parameters, e.g., the TDD pattern.
  cell().dl_f_ref_arfcn = 520002;
  cell().band.reset();
  cell().prach_cfg.prach_config_index.reset();
  autoderive_du_high_parameters_after_parsing(parsed_cfg.config);
  ASSERT_TRUE(cell().band.has_value());
  ASSERT_EQ(band_helper::get_duplex_mode(cell().band.value()), duplex_mode::TDD);
  ASSERT_TRUE(cell().tdd_ul_dl_cfg.has_value());
  tdd_ul_dl_pattern_unit_config& pattern = cell().tdd_ul_dl_cfg->pattern1;
  ASSERT_FALSE(cell().tdd_ul_dl_cfg->pattern2.has_value());

  // CG period longer than the TDD period.
  cell().cg_cfg.periodicity_slots = 2 * pattern.dl_ul_period_slots;
  ASSERT_TRUE(validate());

  // CG period equal to the TDD period, with more than one full UL slot left for the dynamic PUSCHs.
  pattern.nof_ul_slots            = 2;
  cell().cg_cfg.periodicity_slots = pattern.dl_ul_period_slots;
  ASSERT_TRUE(validate());

  // CG period equal to the TDD period, with a single full UL slot.
  pattern.nof_ul_slots = 1;
  ASSERT_FALSE(validate());

  // CG period shorter than the TDD period.
  pattern.nof_ul_slots            = 2;
  cell().cg_cfg.periodicity_slots = pattern.dl_ul_period_slots / 2;
  ASSERT_FALSE(validate());
}

INSTANTIATE_TEST_SUITE_P(cg_type,
                         du_high_cg_config_test,
                         ::testing::Bool(),
                         [](const ::testing::TestParamInfo<bool>& param_info) {
                           return param_info.param ? "type2" : "type1";
                         });
