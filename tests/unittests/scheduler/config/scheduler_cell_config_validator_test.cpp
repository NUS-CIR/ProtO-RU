// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "ocudu/scheduler/config/scheduler_cell_config_validator.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include <gtest/gtest.h>

using namespace ocudu;

static error_type<std::string> validate(unsigned max_nof_ue_contexts)
{
  sched_cell_configuration_request_message msg = sched_config_helper::make_default_sched_cell_configuration_request();
  msg.max_nof_ue_contexts                      = max_nof_ue_contexts;
  return config_validators::validate_sched_cell_configuration_request_message(
      msg, config_helpers::make_default_scheduler_expert_config());
}

TEST(scheduler_cell_config_validator_test, default_nof_ue_contexts_is_valid)
{
  ASSERT_TRUE(
      validate(sched_config_helper::make_default_sched_cell_configuration_request().max_nof_ue_contexts).has_value());
}

TEST(scheduler_cell_config_validator_test, nof_ue_contexts_at_cell_limit_is_valid)
{
  ASSERT_TRUE(validate(MAX_NOF_DU_UES_PER_CELL).has_value());
}

TEST(scheduler_cell_config_validator_test, zero_nof_ue_contexts_is_invalid)
{
  ASSERT_FALSE(validate(0).has_value());
}

TEST(scheduler_cell_config_validator_test, nof_ue_contexts_above_cell_limit_is_invalid)
{
  ASSERT_FALSE(validate(MAX_NOF_DU_UES_PER_CELL + 1).has_value());
}

TEST(scheduler_cell_config_validator_test, periodic_csi_report_that_does_not_fit_in_the_pucch_is_invalid)
{
  sched_cell_configuration_request_message msg = sched_config_helper::make_default_sched_cell_configuration_request();
  ASSERT_TRUE(msg.ran.init_bwp.csi->csi_report_slot_offset.has_value()) << "This test assumes periodic CSI reports";

  // A PUCCH Format 2 resource of 1 RB, 1 symbol and the lowest code rate carries a single bit.
  msg.ran.init_bwp.pucch.resources.f2_or_f3_or_f4_params =
      pucch_f2_params{.nof_syms      = pucch_f2_params::nof_symbols{1},
                      .max_nof_rbs   = pucch_f2_params::nof_rbs{1},
                      .max_code_rate = max_pucch_code_rate::dot_08};

  auto result = config_validators::validate_sched_cell_configuration_request_message(
      msg, config_helpers::make_default_scheduler_expert_config());
  ASSERT_FALSE(result.has_value());
  ASSERT_NE(result.error().find("CSI report"), std::string::npos) << result.error();
}

/// Validates a cell with a Type 2 CG, after \c modify has been applied to its dedicated PDCCH configuration.
template <typename Modifier>
static error_type<std::string> validate_cg_type2(const Modifier& modify)
{
  sched_cell_configuration_request_message msg = sched_config_helper::make_default_sched_cell_configuration_request();
  msg.ran.init_bwp.cg_cfg                      = cg_builder_params{.type = cg_builder_params::cg_type::type2};
  modify(*msg.ran.init_bwp.pdcch_cfg);
  return config_validators::validate_sched_cell_configuration_request_message(
      msg, config_helpers::make_default_scheduler_expert_config());
}

TEST(scheduler_cell_config_validator_test, cg_type2_with_a_ue_specific_dci_f0_1_search_space_is_valid)
{
  const auto result = validate_cg_type2([](pdcch_config&) {});
  ASSERT_TRUE(result.has_value()) << result.error();
}

TEST(scheduler_cell_config_validator_test, cg_type2_without_a_dci_f0_1_search_space_is_invalid)
{
  // The activation DCI is a DCI format 0_1: with only fallback formats monitored, no UE could ever be activated.
  const auto result = validate_cg_type2([](pdcch_config& pdcch_cfg) {
    for (search_space_configuration& ss : pdcch_cfg.search_spaces) {
      if (not ss.is_common_search_space()) {
        ss.set_non_ss0_monitored_dci_formats(search_space_configuration::ue_specific_dci_format::f0_0_and_f1_0);
      }
    }
  });
  ASSERT_FALSE(result.has_value());
  ASSERT_NE(result.error().find("DCI format 0_1"), std::string::npos) << result.error();
}
