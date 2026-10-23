// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/scheduler/config/du_cell_group_config_pool.h"
#include "tests/test_doubles/scheduler/scheduler_config_helper.h"
#include "ocudu/scheduler/config/scheduler_expert_config_factory.h"
#include "ocudu/scheduler/config/scheduler_ue_config_validator.h"
#include <gtest/gtest.h>

using namespace ocudu;

/// Checks the validation of a UE that carries a Configured Grant, which the scheduler only supports on a CP-OFDM
/// PUSCH: both build_cg_pusch_cfg_params() and build_dci_f0_1_cg_activation() hardcode transform precoding to
/// disabled, so validate_cg_cfg() has to refuse a UE whose configuration would turn it on.
class cg_transform_precoding_validator_test : public ::testing::Test
{
protected:
  scheduler_expert_config sched_cfg = config_helpers::make_default_scheduler_expert_config();

  /// Validates a CG UE on a cell whose msg3-transformPrecoder is \c msg3_tp. The UE's configuredGrantConfig
  /// transformPrecoder is set to \c cg_tp (absent if empty) and its dedicated pusch-Config transformPrecoder to
  /// \c ded_tp.
  error_type<std::string> validate(bool                             msg3_tp,
                                   std::optional<bool>              cg_tp,
                                   pusch_config::transform_precoder ded_tp,
                                   cg_builder_params::cg_type       type = cg_builder_params::cg_type::type1)
  {
    // A new cell per call, so that a test can validate more than one combination.
    du_cell_group_config_pool                cfg_pool;
    sched_cell_configuration_request_message cell_req =
        sched_config_helper::make_default_sched_cell_configuration_request();
    cell_req.ran.init_bwp.cg_cfg                                                    = cg_builder_params{.type = type};
    cell_req.ran.ul_cfg_common.init_ul_bwp.rach_cfg_common->msg3_transform_precoder = msg3_tp;

    const cell_configuration&         cell_cfg = cfg_pool.add_cell(sched_cfg, cell_req);
    sched_ue_creation_request_message ue_req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);

    ocudu_assert(ue_req.cfg.cells->front().ded.has_value(), "Dedicated configuration is required for this test");
    auto& ul_cfg                                 = ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config.value();
    ul_cfg.init_ul_bwp.pusch_cfg->trans_precoder = ded_tp;
    ul_cfg.init_ul_bwp.cg_cfg->trans_precoder    = cg_tp;

    return config_validators::validate_sched_ue_creation_request_message(ue_req, cell_cfg);
  }

  /// Checks that the validation failed because of transform precoding, so that a test cannot pass on an unrelated
  /// validation failure.
  static void expect_tp_failure(const error_type<std::string>& result)
  {
    ASSERT_FALSE(result.has_value());
    ASSERT_NE(result.error().find("transform precoding"), std::string::npos) << result.error();
  }
};

TEST_F(cg_transform_precoding_validator_test, cg_without_transform_precoding_is_valid)
{
  ASSERT_TRUE(validate(false, false, pusch_config::transform_precoder::disabled).has_value());
}

TEST_F(cg_transform_precoding_validator_test, cg_with_cg_transform_precoding_is_invalid)
{
  expect_tp_failure(validate(false, true, pusch_config::transform_precoder::disabled));
}

TEST_F(cg_transform_precoding_validator_test, cg_falls_back_to_msg3_transform_precoder_when_cg_field_absent)
{
  // With no transformPrecoder in configuredGrantConfig, msg3-transformPrecoder decides, even if the dedicated
  // pusch-Config disables transform precoding.
  expect_tp_failure(validate(true, std::nullopt, pusch_config::transform_precoder::disabled));
  ASSERT_TRUE(validate(false, std::nullopt, pusch_config::transform_precoder::disabled).has_value());
}

TEST_F(cg_transform_precoding_validator_test, cg_transform_precoder_overrides_msg3_transform_precoder)
{
  // transformPrecoder in configuredGrantConfig set to disabled takes precedence over msg3-transformPrecoder.
  ASSERT_TRUE(validate(true, false, pusch_config::transform_precoder::disabled).has_value());
}

TEST_F(cg_transform_precoding_validator_test, cg_type1_ignores_dedicated_transform_precoder)
{
  // The dedicated pusch-Config transformPrecoder applies to dynamic grants only (TS 38.214, Section 6.1.3), so a
  // CP-OFDM Type 1 CG is valid while the dynamic PUSCH uses DFT-s-OFDM.
  ASSERT_TRUE(validate(false, false, pusch_config::transform_precoder::enabled).has_value());
  ASSERT_TRUE(validate(true, false, pusch_config::transform_precoder::not_set).has_value());
}

TEST_F(cg_transform_precoding_validator_test, cg_type2_requires_cp_ofdm_dynamic_pusch)
{
  // The Type 2 activation DCI is sized after the dedicated PUSCH waveform, which must then match the CG one.
  constexpr auto type2 = cg_builder_params::cg_type::type2;
  ASSERT_TRUE(validate(false, false, pusch_config::transform_precoder::disabled, type2).has_value());
  expect_tp_failure(validate(false, false, pusch_config::transform_precoder::enabled, type2));
  expect_tp_failure(validate(true, false, pusch_config::transform_precoder::not_set, type2));
}

/// Checks the Type 2 requirement for a UE-specific SearchSpace monitored with DCI format 0_1, which is the only format
/// the activation DCI is built in.
class cg_type2_search_space_validator_test : public ::testing::Test
{
protected:
  scheduler_expert_config                  sched_cfg = config_helpers::make_default_scheduler_expert_config();
  sched_cell_configuration_request_message cell_req =
      sched_config_helper::make_default_sched_cell_configuration_request();
  du_cell_group_config_pool cfg_pool;

  /// Validates a Type 2 CG UE whose UE-specific SearchSpace monitors \c uss_fmt.
  error_type<std::string> validate(search_space_configuration::ue_specific_dci_format uss_fmt)
  {
    cell_req.ran.init_bwp.cg_cfg = cg_builder_params{.type = cg_builder_params::cg_type::type2};
    cell_req.ran.init_bwp.pdcch_cfg->search_spaces[0].set_non_ss0_monitored_dci_formats(uss_fmt);

    const cell_configuration&         cell_cfg = cfg_pool.add_cell(sched_cfg, cell_req);
    sched_ue_creation_request_message ue_req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    // A Type 2 CG carries no RRC-configured grant.
    ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config->init_ul_bwp.cg_cfg->rrc_configured_ul_grant_cfg.reset();

    return config_validators::validate_sched_ue_creation_request_message(ue_req, cell_cfg);
  }
};

TEST_F(cg_type2_search_space_validator_test, cg_type2_with_a_dci_f0_1_search_space_is_valid)
{
  ASSERT_TRUE(validate(search_space_configuration::ue_specific_dci_format::f0_1_and_1_1).has_value());
}

TEST_F(cg_type2_search_space_validator_test, cg_type2_without_a_dci_f0_1_search_space_is_invalid)
{
  const auto result = validate(search_space_configuration::ue_specific_dci_format::f0_0_and_f1_0);
  ASSERT_FALSE(result.has_value());
  ASSERT_NE(result.error().find("DCI format 0_1"), std::string::npos) << result.error();
}

/// Checks the Type 2 requirement for a PUSCH time domain resource that activate_ue() can actually pick, i.e. one that
/// neither overlaps the SRS region nor asks for slot repetitions.
class cg_type2_activation_td_res_validator_test : public ::testing::Test
{
protected:
  scheduler_expert_config                  sched_cfg = config_helpers::make_default_scheduler_expert_config();
  sched_cell_configuration_request_message cell_req =
      sched_config_helper::make_default_sched_cell_configuration_request();
  du_cell_group_config_pool cfg_pool;

  /// \brief Validates a Type 2 CG UE whose dedicated TDRA list mirrors the cell's common list.
  /// \param nof_repetitions Repetitions every row asks for. A row asking for more than one is unusable, as the CG
  /// scheduler emits a single PUSCH per occasion.
  /// \param prepend_row Whether to put a row absent from the common list ahead of the mirrored ones, so that the
  /// dedicated list no longer starts with the common one.
  error_type<std::string> validate(uint8_t nof_repetitions, bool prepend_row = false)
  {
    cell_req.ran.init_bwp.cg_cfg = cg_builder_params{.type = cg_builder_params::cg_type::type2};

    const cell_configuration&         cell_cfg = cfg_pool.add_cell(sched_cfg, cell_req);
    sched_ue_creation_request_message ue_req =
        sched_config_helper::create_default_sched_ue_creation_request(cell_req.ran);
    auto& ul_cfg = ue_req.cfg.cells->front().ded->serv_cell_cfg.ul_config.value();
    // A Type 2 CG carries no RRC-configured grant.
    ul_cfg.init_ul_bwp.cg_cfg->rrc_configured_ul_grant_cfg.reset();

    // Give the UE a dedicated list that copies the cell's common list, as the DU does, and have every row ask for the
    // repetitions under test. The activation resolves against the dedicated list whenever one is configured, so these
    // rows are the only candidates the scheduler would have.
    const auto& common_list = cell_req.ran.ul_cfg_common.init_ul_bwp.pusch_cfg_common->pusch_td_alloc_list;
    auto&       ded_list    = ul_cfg.init_ul_bwp.pusch_cfg->pusch_td_alloc_list;
    ded_list.assign(common_list.begin(), common_list.end());
    if (nof_repetitions > 1) {
      for (pusch_time_domain_resource_allocation& td_res : ded_list) {
        td_res.nof_repetitions = nof_repetitions;
      }
    }
    if (prepend_row) {
      pusch_time_domain_resource_allocation extra_row = common_list.front();
      ++extra_row.k2;
      ded_list.insert(ded_list.begin(), extra_row);
    }

    return config_validators::validate_sched_ue_creation_request_message(ue_req, cell_cfg);
  }
};

TEST_F(cg_type2_activation_td_res_validator_test, cg_type2_with_a_td_res_without_repetitions_is_valid)
{
  ASSERT_TRUE(validate(/*nof_repetitions=*/1).has_value());
}

TEST_F(cg_type2_activation_td_res_validator_test, cg_type2_whose_only_td_res_asks_for_repetitions_is_invalid)
{
  const auto result = validate(/*nof_repetitions=*/4);
  ASSERT_FALSE(result.has_value());
  // Assert on the cause, so that the test cannot pass on an unrelated validation failure.
  ASSERT_NE(result.error().find("slot repetitions"), std::string::npos) << result.error();
}

TEST_F(cg_type2_activation_td_res_validator_test,
       cg_type2_whose_ded_td_list_does_not_start_with_the_common_list_is_invalid)
{
  const auto result = validate(/*nof_repetitions=*/1, /*prepend_row=*/true);
  ASSERT_FALSE(result.has_value());
  // Assert on the cause, so that the test cannot pass on an unrelated validation failure.
  ASSERT_NE(result.error().find("start with"), std::string::npos) << result.error();
}
