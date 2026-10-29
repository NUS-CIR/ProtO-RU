// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/du/du_high/du_manager/converters/asn1_ntn_config_helpers.h"
#include "lib/du/du_high/du_manager/converters/asn1_rrc_config_helpers.h"
#include "tests/ocudu_test_requirements.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/asn1_utils.h"
#include "ocudu/asn1/rrc_nr/cell_group_config.h"
#include "ocudu/asn1/rrc_nr/rrc_nr.h"
#include "ocudu/du/du_cell_config_helpers.h"
#include "ocudu/mac/config/mac_cell_group_config_factory.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/rlm_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include <gtest/gtest.h>

using namespace ocudu;

static odu::du_ue_resource_config make_initial_du_ue_resource_config()
{
  odu::du_ue_resource_config dest_cfg{};
  dest_cfg.cell_group.cells.emplace(
      SERVING_PCELL_IDX, config_helpers::make_default_ue_cell_config(config_helpers::make_default_ran_cell_config()));
  dest_cfg.cell_group.mcg_cfg = config_helpers::make_initial_mac_cell_group_config();
  return dest_cfg;
}

static pusch_config make_initial_pusch_config()
{
  pusch_config cfg{};
  cfg.tx_cfg = tx_scheme_codebook{.max_rank        = 1,
                                  .codebook_subset = tx_scheme_codebook_subset::fully_and_partial_and_non_coherent};
  cfg.pusch_mapping_type_b_dmrs.emplace();
  cfg.pusch_mapping_type_b_dmrs.value().trans_precoder_disabled.emplace();
  cfg.pusch_mapping_type_b_dmrs.value().ptrs = ptrs_uplink_config{
      .trans_precoder_disabled = {ptrs_uplink_config::transform_precoder_disabled{.f_density = {2, 4},
                                                                                  .t_density = {7, 8}}},
      .trans_precoder_enabled  = {ptrs_uplink_config::transform_precoder_enabled{.sampl_density = {2, 5}}}};

  cfg.pusch_pwr_ctrl = pusch_config::pusch_power_control{.msg3_alpha               = alpha::alpha1,
                                                         .p0_nominal_without_grant = -76,
                                                         .p0_alphasets             = {},
                                                         .pathloss_ref_rs          = {},
                                                         .sri_pusch_mapping        = {}};
  cfg.pusch_pwr_ctrl.value().p0_alphasets.push_back(pusch_config::pusch_power_control::p0_pusch_alphaset{
      .id = static_cast<p0_pusch_alphaset_id>(0), .p0 = 0, .p0_pusch_alpha = alpha::alpha1});
  cfg.pusch_pwr_ctrl.value().pathloss_ref_rs.push_back(pusch_config::pusch_power_control::pusch_pathloss_ref_rs{
      .id = static_cast<pusch_config::pusch_power_control::pusch_pathloss_ref_rs_id>(0), .rs = 0});
  cfg.pusch_pwr_ctrl.value().sri_pusch_mapping.push_back(pusch_config::pusch_power_control::sri_pusch_pwr_ctrl{
      .id                           = static_cast<pusch_config::pusch_power_control::sri_pusch_pwr_ctrl_id>(0),
      .sri_pusch_pathloss_ref_rs_id = static_cast<pusch_config::pusch_power_control::pusch_pathloss_ref_rs_id>(0),
      .sri_p0_pusch_alphaset_id     = static_cast<p0_pusch_alphaset_id>(0),
      .closed_loop_idx = pusch_config::pusch_power_control::sri_pusch_pwr_ctrl::sri_pusch_closed_loop_index::i0});
  cfg.res_alloc = pusch_config::resource_allocation::resource_allocation_type_1;
  cfg.pusch_td_alloc_list.resize(1);
  cfg.pusch_td_alloc_list[0].k2       = 4;
  cfg.pusch_td_alloc_list[0].map_type = sch_mapping_type::typeA;
  cfg.pusch_td_alloc_list[0].symbols  = {0, 14};
  cfg.trans_precoder                  = pusch_config::transform_precoder::disabled;

  cfg.uci_cfg.emplace();
  auto& uci_cfg   = cfg.uci_cfg.value();
  uci_cfg.scaling = alpha_scaling_opt::f1;
  beta_offsets b_offset{};
  b_offset.beta_offset_ack_idx_1    = 9;
  b_offset.beta_offset_ack_idx_2    = 9;
  b_offset.beta_offset_ack_idx_3    = 9;
  b_offset.beta_offset_csi_p1_idx_1 = 6;
  b_offset.beta_offset_csi_p1_idx_2 = 6;
  b_offset.beta_offset_csi_p2_idx_1 = 6;
  b_offset.beta_offset_csi_p2_idx_2 = 6;
  uci_cfg.beta_offsets_cfg          = uci_on_pusch::beta_offsets_semi_static{b_offset};

  return cfg;
}

static srs_config make_initial_srs_config()
{
  srs_config cfg{};

  cfg.srs_res_set_list.push_back(srs_config::srs_resource_set{
      .id              = static_cast<srs_config::srs_res_set_id>(0),
      .srs_res_id_list = {static_cast<srs_config::srs_res_id>(0)},
      .res_type =
          srs_config::srs_resource_set::aperiodic_resource_type{.aperiodic_srs_res_trigger = 1, .slot_offset = 2},
      .srs_res_set_usage  = srs_usage::codebook,
      .srs_pwr_ctrl_alpha = alpha::alpha1,
      .p0                 = -80});
  srs_config::srs_resource::tx_comb_params tx_comb{
      .size = tx_comb_size::n2, .tx_comb_offset = 0, .tx_comb_cyclic_shift = 0};
  cfg.srs_res_list.push_back(srs_config::srs_resource{
      .id                    = srs_config::srs_res_id_t{0U, static_cast<srs_config::srs_res_id>(0)},
      .nof_ports             = srs_config::srs_resource::nof_srs_ports::port1,
      .tx_comb               = tx_comb,
      .res_mapping           = srs_config::resource_mapping{.start_pos   = 2,
                                                            .nof_symb    = srs_nof_symbols::n1,
                                                            .rept_factor = srs_nof_symbols::n1},
      .freq_domain_pos       = 0,
      .freq_domain_shift     = 0,
      .freq_hop              = srs_config::srs_resource::frequency_hopping{.c_srs = 0, .b_srs = 0, .b_hop = 0},
      .grp_or_seq_hop        = srs_group_or_sequence_hopping::neither,
      .res_type              = srs_resource_type::aperiodic,
      .sequence_id           = 40,
      .spatial_relation_info = srs_config::srs_resource::srs_spatial_relation_info{
          .reference_signal = static_cast<nzp_csi_rs_res_id_t>(0)}});

  return cfg;
}

TEST(serving_cell_config_converter_test, test_default_initial_ue_pdcch_cfg_conversion)
{
  auto                           dest_cfg = make_initial_du_ue_resource_config();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg_present, dest_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.has_value());
  // Since its initial setup and no source cell group config was provided pdcch must be of setup type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.has_value()) {
    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.setup().coreset_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.value().coresets.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.setup().coreset_to_release_list.size(), 0);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.setup().search_spaces_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.value().search_spaces.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.setup().search_spaces_to_release_list.size(), 0);
  }
}

TEST(serving_cell_config_converter_test, test_ue_pdcch_cfg_release_conversion)
{
  auto                       src_cfg  = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg = make_initial_du_ue_resource_config();
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.init_dl_bwp.pdcch_cfg.reset();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg_present,
            not dest_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.has_value());
  // PDCCH Config is released due to absence in dest cell group config.
  ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdcch_cfg.type(), asn1::setup_release_opts::release);
}

TEST(serving_cell_config_converter_test, test_default_initial_ue_pdsch_cfg_conversion)
{
  auto                           dest_cfg = make_initial_du_ue_resource_config();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg_present, dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.has_value());
  // Since its initial setup and no source cell group config was provided pdsch must be of setup type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.has_value()) {
    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().data_scrambling_id_pdsch_present,
              dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.value().data_scrambling_id_pdsch.has_value());
    if (dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.value().data_scrambling_id_pdsch.has_value()) {
      ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().data_scrambling_id_pdsch,
                dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.value().data_scrambling_id_pdsch.value());
    }

    if (dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.value().pdsch_mapping_type_a_dmrs.has_value()) {
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().dmrs_dl_for_pdsch_map_type_a_present);
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().dmrs_dl_for_pdsch_map_type_a.is_setup());
    }

    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().tci_states_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.value().tci_states.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().tci_states_to_release_list.size(), 0);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().rate_match_pattern_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.value().rate_match_pattrn.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().rate_match_pattern_to_release_list.size(), 0);
  }
}

TEST(serving_cell_config_converter_test, test_ue_pdsch_cfg_release_conversion)
{
  odu::du_ue_resource_config dest_cfg = make_initial_du_ue_resource_config();
  auto                       src_cfg  = dest_cfg;
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.init_dl_bwp.pdsch_cfg.reset();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg_present,
            not dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.has_value());
  // PDSCH Config is released due to absence in dest cell group config.
  ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.type(), asn1::setup_release_opts::release);
}

TEST(serving_cell_config_converter_test, test_ue_custom_pdsch_cfg_conversion)
{
  auto                       src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  // Add new configuration to be setup.
  auto& dest_pdsch_cfg = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.init_dl_bwp.pdsch_cfg.value();
  dest_pdsch_cfg.tci_states.push_back(tci_state{
      .state_id  = static_cast<tci_state_id_t>(1),
      .qcl_type1 = {.ref_sig  = {.type   = qcl_info::reference_signal::reference_signal_type::csi_rs,
                                 .csi_rs = static_cast<nzp_csi_rs_res_id_t>(0)},
                    .qcl_type = qcl_info::qcl_type::type_a},
  });
  dest_pdsch_cfg.tci_states.erase(dest_pdsch_cfg.tci_states.begin());

  dest_pdsch_cfg.pdsch_mapping_type_a_dmrs.value().additional_positions = dmrs_additional_positions::pos0;
  dest_pdsch_cfg.pdsch_mapping_type_a_dmrs.value().scrambling_id0       = 10;
  dest_pdsch_cfg.pdsch_mapping_type_a_dmrs.value().scrambling_id1       = 20;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg_present, dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.has_value()) {
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().dmrs_dl_for_pdsch_map_type_a_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().dmrs_dl_for_pdsch_map_type_a.is_setup());

    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().tci_states_to_add_mod_list.size(), 1);
    ASSERT_EQ(rrc_sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup().tci_states_to_release_list.size(), 1);
  }
}

TEST(serving_cell_config_converter_test, test_ue_pdsch_td_alloc_list_r16_conversion)
{
  auto                       src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto& dest_pdsch_cfg = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.init_dl_bwp.pdsch_cfg.value();
  dest_pdsch_cfg.pdsch_td_alloc_list.push_back({.k0 = 0, .map_type = sch_mapping_type::typeA, .symbols = {2, 14}});
  dest_pdsch_cfg.pdsch_td_alloc_list.push_back(
      {.k0 = 0, .map_type = sch_mapping_type::typeA, .symbols = {2, 14}, .rep_number = 8});
  dest_pdsch_cfg.slot_based_repetition_enabled = true;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  auto& rrc_pdsch_cfg = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup();
  ASSERT_TRUE(rrc_pdsch_cfg.ext);
  ASSERT_TRUE(rrc_pdsch_cfg.pdsch_time_domain_alloc_list_r16.is_present());
  ASSERT_TRUE(rrc_pdsch_cfg.pdsch_time_domain_alloc_list_r16->is_setup());

  // The slot-based repetition scheme must be configured along with the Rel-16 TDRA list.
  ASSERT_TRUE(rrc_pdsch_cfg.repeat_scheme_cfg_r16.is_present());
  ASSERT_TRUE(rrc_pdsch_cfg.repeat_scheme_cfg_r16->is_setup());
  ASSERT_EQ(rrc_pdsch_cfg.repeat_scheme_cfg_r16->setup().type(),
            asn1::rrc_nr::repeat_scheme_cfg_r16_c::types_opts::slot_based_r16);
  ASSERT_TRUE(rrc_pdsch_cfg.repeat_scheme_cfg_r16->setup().slot_based_r16().is_setup());
  const auto& rrc_list = rrc_pdsch_cfg.pdsch_time_domain_alloc_list_r16->setup();
  ASSERT_EQ(rrc_list.size(), 2);
  ASSERT_FALSE(rrc_list[0].repeat_num_r16_present);
  ASSERT_TRUE(rrc_list[1].repeat_num_r16_present);
  ASSERT_EQ(rrc_list[1].repeat_num_r16.to_number(), 8);
  ASSERT_EQ(rrc_list[0].start_symbol_and_len_r16, rrc_list[1].start_symbol_and_len_r16);

  // The generated cell group config must be packable.
  byte_buffer   packed;
  asn1::bit_ref bref{packed};
  ASSERT_EQ(rrc_cell_grp_cfg.pack(bref), asn1::OCUDUASN_SUCCESS);
}

TEST(serving_cell_config_converter_test, test_ue_pdsch_td_alloc_list_r16_release_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();
  src_cfg.cell_group.cells.at(SERVING_PCELL_IDX)
      .serv_cell_cfg.init_dl_bwp.pdsch_cfg->pdsch_td_alloc_list.push_back(
          {.k0 = 0, .map_type = sch_mapping_type::typeA, .symbols = {2, 14}, .rep_number = 2});
  odu::du_ue_resource_config dest_cfg{src_cfg};
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.init_dl_bwp.pdsch_cfg->pdsch_td_alloc_list.clear();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  auto& rrc_pdsch_cfg = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded.init_dl_bwp.pdsch_cfg.setup();
  ASSERT_TRUE(rrc_pdsch_cfg.pdsch_time_domain_alloc_list_r16.is_present());
  ASSERT_EQ(rrc_pdsch_cfg.pdsch_time_domain_alloc_list_r16->type(), asn1::setup_release_opts::release);
}

TEST(serving_cell_config_converter_test, test_ue_pusch_td_alloc_list_dci_0_1_r16_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();
  src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config->init_ul_bwp.pusch_cfg =
      make_initial_pusch_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto&                      dest_pusch_cfg =
      dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config->init_ul_bwp.pusch_cfg.value();
  dest_pusch_cfg.pusch_td_alloc_list.clear();
  dest_pusch_cfg.pusch_td_alloc_list.push_back({.k2 = 4, .map_type = sch_mapping_type::typeA, .symbols = {0, 14}});
  dest_pusch_cfg.pusch_td_alloc_list.push_back(
      {.k2 = 4, .map_type = sch_mapping_type::typeA, .symbols = {0, 14}, .nof_repetitions = 4});

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  auto& rrc_pusch_cfg = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup();
  ASSERT_TRUE(rrc_pusch_cfg.ext);
  ASSERT_TRUE(rrc_pusch_cfg.pusch_time_domain_alloc_list_dci_0_1_r16.is_present());
  ASSERT_TRUE(rrc_pusch_cfg.pusch_time_domain_alloc_list_dci_0_1_r16->is_setup());
  // pusch-RepTypeIndicatorDCI-0-1-r16 is mandatory whenever pusch-TimeDomainAllocationListDCI-0-1-r16 is configured.
  ASSERT_TRUE(rrc_pusch_cfg.pusch_rep_type_ind_dci_0_1_r16_present);
  ASSERT_EQ(rrc_pusch_cfg.pusch_rep_type_ind_dci_0_1_r16.value,
            asn1::rrc_nr::pusch_cfg_s::pusch_rep_type_ind_dci_0_1_r16_opts::pusch_rep_type_a);
  // availableSlotCounting-r17 is Cond PUSCH-RepTypeAIndicated: mandatory here since Type A is always indicated.
  ASSERT_TRUE(rrc_pusch_cfg.available_slot_count_r17_present);
  const auto& rrc_list = rrc_pusch_cfg.pusch_time_domain_alloc_list_dci_0_1_r16->setup();
  ASSERT_EQ(rrc_list.size(), 2);
  ASSERT_TRUE(rrc_list[0].k2_r16_present);
  ASSERT_EQ(rrc_list[0].k2_r16, 4);
  ASSERT_EQ(rrc_list[0].pusch_alloc_list_r16.size(), 1);
  // numberOfRepetitions-r16 is mandatory present in the DCI 0_1 list; a single transmission is encoded as n1.
  ASSERT_TRUE(rrc_list[0].pusch_alloc_list_r16[0].nof_repeats_r16_present);
  ASSERT_EQ(rrc_list[0].pusch_alloc_list_r16[0].nof_repeats_r16.to_number(), 1);
  ASSERT_EQ(rrc_list[1].pusch_alloc_list_r16.size(), 1);
  ASSERT_TRUE(rrc_list[1].pusch_alloc_list_r16[0].nof_repeats_r16_present);
  ASSERT_EQ(rrc_list[1].pusch_alloc_list_r16[0].nof_repeats_r16.to_number(), 4);

  // The generated cell group config must be packable.
  byte_buffer   packed;
  asn1::bit_ref bref{packed};
  ASSERT_EQ(rrc_cell_grp_cfg.pack(bref), asn1::OCUDUASN_SUCCESS);
}

TEST(serving_cell_config_converter_test, test_default_initial_ue_uplink_cfg_conversion)
{
  auto                           dest_cfg = make_initial_du_ue_resource_config();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg_present, dest_sp_cell_cfg_ded.ul_config.has_value());

  if (dest_sp_cell_cfg_ded.ul_config.has_value()) {
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg_present,
              dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.has_value());
    if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.has_value()) {
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.is_setup());

      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_set_to_add_mod_list.size(),
                dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().pucch_res_set.size());
      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_set_to_release_list.size(), 0);

      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_to_add_mod_list.size(),
                dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().pucch_res_list.size());
      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_to_release_list.size(), 0);

      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format1_present,
                dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_1_common_param.has_value());
      if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_1_common_param.has_value()) {
        ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format1.is_setup());
      }

      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format2_present,
                dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_2_common_param.has_value());
      if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_2_common_param.has_value()) {
        ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format2.is_setup());
      }

      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format3_present,
                dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_3_common_param.has_value());
      if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_3_common_param.has_value()) {
        ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format3.is_setup());
      }

      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format4_present,
                dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_4_common_param.has_value());
      if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_4_common_param.has_value()) {
        ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format4.is_setup());
      }

      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().sched_request_res_to_add_mod_list.size(),
                dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().sr_res_list.size());
      ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().sched_request_res_to_release_list.size(), 0);
    }
  }
}

TEST(serving_cell_config_converter_test, test_default_initial_ue_dl_bwp_conversion)
{
  auto                           dest_cfg = make_initial_du_ue_resource_config();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_EQ(rrc_sp_cell_cfg_ded.dl_bwp_to_add_mod_list.size(), dest_sp_cell_cfg_ded.dl_bwps.size());
  ASSERT_EQ(rrc_sp_cell_cfg_ded.dl_bwp_to_release_list.size(), 0);
}

TEST(serving_cell_config_converter_test, test_default_initial_ue_pucch_cfg_conversion)
{
  auto                           dest_cfg = make_initial_du_ue_resource_config();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg_present, dest_sp_cell_cfg_ded.ul_config.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg_present);
  // Since its initial setup and no source cell group config was provided pucch must be of setup type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.has_value()) {
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_set_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().pucch_res_set.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_set_to_release_list.size(), 0);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().pucch_res_list.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_to_release_list.size(), 0);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().sched_request_res_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().sr_res_list.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().sched_request_res_to_release_list.size(), 0);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().dl_data_to_ul_ack.size(),
              dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().dl_data_to_ul_ack.size());

    if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_1_common_param.has_value()) {
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format1_present);
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format1.is_setup());
    }

    if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_2_common_param.has_value()) {
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format2_present);
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format2.is_setup());
    }

    if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_3_common_param.has_value()) {
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format3_present);
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format3.is_setup());
    }

    if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().format_4_common_param.has_value()) {
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format4_present);
      ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format4.is_setup());
    }
  }
}

TEST(serving_cell_config_converter_test, test_ue_custom_pucch_cfg_conversion)
{
  auto                       src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  // Add new configuration to be setup. Assume PUCCH Config is present in initial cell group config.
  auto& dest_pucch_cfg =
      dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pucch_cfg.value();

  // >> PUCCH Resource Set 1.
  dest_pucch_cfg.pucch_res_set.emplace_back();
  dest_pucch_cfg.pucch_res_set.back().id = ocudu::pucch_res_set_id::set_1;
  dest_pucch_cfg.pucch_res_set.back().resources.emplace_back(pucch_res_id_t::make_ded(1, 1));
  // Remove first element.
  dest_pucch_cfg.pucch_res_set.erase(dest_pucch_cfg.pucch_res_set.begin());

  // >>> PUCCH resource 2.
  pucch_resource res_basic;
  res_basic.res_id         = pucch_res_id_t::make_ded(14, 14);
  res_basic.starting_prb   = 40;
  res_basic.second_hop_prb = 50;
  res_basic.syms           = ofdm_symbol_range::start_and_len(13, 1);
  res_basic.format_params.emplace<pucch_resource::f2_config>(pucch_resource::f2_config{.nof_prbs = 1});
  dest_pucch_cfg.pucch_res_list.push_back(res_basic);

  // Remove first element.
  dest_pucch_cfg.pucch_res_list.erase(dest_pucch_cfg.pucch_res_list.begin());

  dest_pucch_cfg.dl_data_to_ul_ack.push_back(2);
  dest_pucch_cfg.dl_data_to_ul_ack.push_back(3);

  // Release default one.
  dest_pucch_cfg.format_1_common_param.reset();

  // Note: Set a configuration different from the one already existing the starting config; this will trigger the
  // calculate_cell_group_config_diff() to add an element in format2 in the generated ASN1 message.
  dest_pucch_cfg.format_2_common_param.emplace();
  dest_pucch_cfg.format_2_common_param.value().max_c_rate                = max_pucch_code_rate::dot_08;
  dest_pucch_cfg.format_2_common_param.value().simultaneous_harq_ack_csi = true;

  dest_pucch_cfg.format_3_common_param.emplace();
  dest_pucch_cfg.format_3_common_param.value().max_c_rate = max_pucch_code_rate::dot_08;
  dest_pucch_cfg.format_3_common_param.value().nof_slots  = pucch_common_all_formats::num_of_slots::n2;

  dest_pucch_cfg.format_4_common_param.emplace();
  dest_pucch_cfg.format_4_common_param.value().max_c_rate      = max_pucch_code_rate::dot_60;
  dest_pucch_cfg.format_4_common_param.value().additional_dmrs = true;

  // >>> SR Resource.
  dest_pucch_cfg.sr_res_list.push_back(
      scheduling_request_resource_config{.sr_res_id    = 2,
                                         .sr_id        = uint_to_sched_req_id(1),
                                         .period       = sr_periodicity::sl_80,
                                         .offset       = 0,
                                         .pucch_res_id = pucch_res_id_t::make_ded(1, 1)});

  // Remove first element.
  dest_pucch_cfg.sr_res_list.erase(dest_pucch_cfg.sr_res_list.begin());

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg_present, dest_sp_cell_cfg_ded.ul_config.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg_present);
  // Since its initial setup and no source cell group config was provided pucch must be of setup type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.has_value()) {
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_set_to_add_mod_list.size(), 1);
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_set_to_release_list.size(), 1);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_to_add_mod_list.size(), 1);
    auto& pucch_res1 = rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_to_add_mod_list[0];
    ASSERT_EQ(pucch_res1.pucch_res_id, 14);
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().res_to_release_list.size(), 1);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().sched_request_res_to_add_mod_list.size(), 1);
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().sched_request_res_to_release_list.size(), 1);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().dl_data_to_ul_ack.size(),
              dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.value().dl_data_to_ul_ack.size());

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format1.type(),
              asn1::setup_release_opts::release);

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format2_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format2.is_setup());

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format3_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format3.is_setup());

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format4_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup().format4.is_setup());
  }
}

TEST(serving_cell_config_converter_test, test_pucch_res_repetition_factor_conversion)
{
  auto                       src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto&                      dest_pucch_cfg =
      dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pucch_cfg.value();

  // Add a PUCCH resource repeated over four slots and a PUCCH resource without repetition.
  pucch_resource res_rep;
  res_rep.res_id         = pucch_res_id_t::make_ded(14, 14);
  res_rep.starting_prb   = 40;
  res_rep.second_hop_prb = 50;
  res_rep.syms           = ofdm_symbol_range::start_and_len(0, 14);
  res_rep.format_params.emplace<pucch_resource::f1_config>(pucch_resource::f1_config{});
  res_rep.rep_factor = pucch_repetition_factor::n4;
  dest_pucch_cfg.pucch_res_list.push_back(res_rep);

  pucch_resource res_no_rep = res_rep;
  res_no_rep.res_id         = pucch_res_id_t::make_ded(15, 15);
  res_no_rep.rep_factor     = pucch_repetition_factor::n1;
  dest_pucch_cfg.pucch_res_list.push_back(res_no_rep);

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  // pucch-RepetitionNrofSlots-r17 is in an extension addition group of PUCCH-ResourceExt-v1610, whose list
  // resourceToAddModListExt-v1610 is in an extension addition group of PUCCH-Config. Both are only encoded with the
  // extension bit of their parent set.
  byte_buffer   packed;
  asn1::bit_ref bref{packed};
  ASSERT_EQ(rrc_cell_grp_cfg.pack(bref), asn1::OCUDUASN_SUCCESS);
  asn1::rrc_nr::cell_group_cfg_s unpacked;
  asn1::cbit_ref                 cbref{packed};
  ASSERT_EQ(unpacked.unpack(cbref), asn1::OCUDUASN_SUCCESS);

  // resourceToAddModListExt-v1610 has the same number of entries, listed in the same order, as resourceToAddModList
  // (TS 38.331, PUCCH-Config).
  const auto& pucch_cfg = unpacked.sp_cell_cfg.sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup();
  ASSERT_EQ(pucch_cfg.res_to_add_mod_list.size(), 2);
  ASSERT_TRUE(pucch_cfg.res_to_add_mod_list_ext_v1610.is_present());
  ASSERT_EQ(pucch_cfg.res_to_add_mod_list_ext_v1610->size(), pucch_cfg.res_to_add_mod_list.size());
  for (unsigned i = 0, e = pucch_cfg.res_to_add_mod_list.size(); i != e; ++i) {
    const auto& res_ext = (*pucch_cfg.res_to_add_mod_list_ext_v1610)[i];
    if (pucch_cfg.res_to_add_mod_list[i].pucch_res_id == 14) {
      ASSERT_TRUE(res_ext.pucch_repeat_nrof_slots_r17_present);
      ASSERT_EQ(res_ext.pucch_repeat_nrof_slots_r17.to_number(), 4);
    } else {
      ASSERT_EQ(pucch_cfg.res_to_add_mod_list[i].pucch_res_id, 15);
      ASSERT_FALSE(res_ext.pucch_repeat_nrof_slots_r17_present);
    }
  }
}

TEST(serving_cell_config_converter_test, test_pucch_res_repetition_factor_update_conversion)
{
  auto                       src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto&                      dest_pucch_cfg =
      dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pucch_cfg.value();

  // Configure the repetition of an existing PUCCH resource over two slots.
  ASSERT_FALSE(dest_pucch_cfg.pucch_res_list.empty());
  pucch_resource& res = dest_pucch_cfg.pucch_res_list.front();
  ASSERT_EQ(res.rep_factor, pucch_repetition_factor::n1);
  res.rep_factor = pucch_repetition_factor::n2;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  byte_buffer   packed;
  asn1::bit_ref bref{packed};
  ASSERT_EQ(rrc_cell_grp_cfg.pack(bref), asn1::OCUDUASN_SUCCESS);
  asn1::rrc_nr::cell_group_cfg_s unpacked;
  asn1::cbit_ref                 cbref{packed};
  ASSERT_EQ(unpacked.unpack(cbref), asn1::OCUDUASN_SUCCESS);

  // The modified PUCCH resource is signalled again, with its repetition factor.
  const auto& pucch_cfg = unpacked.sp_cell_cfg.sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup();
  ASSERT_EQ(pucch_cfg.res_to_add_mod_list.size(), 1);
  ASSERT_EQ(pucch_cfg.res_to_add_mod_list[0].pucch_res_id, res.res_id.ded().ue_res_id);
  ASSERT_TRUE(pucch_cfg.res_to_add_mod_list_ext_v1610.is_present());
  ASSERT_EQ(pucch_cfg.res_to_add_mod_list_ext_v1610->size(), 1);
  ASSERT_TRUE((*pucch_cfg.res_to_add_mod_list_ext_v1610)[0].pucch_repeat_nrof_slots_r17_present);
  ASSERT_EQ((*pucch_cfg.res_to_add_mod_list_ext_v1610)[0].pucch_repeat_nrof_slots_r17.to_number(), 2);
}

TEST(serving_cell_config_converter_test, test_pucch_res_repetition_factor_release_conversion)
{
  auto  src_cfg = make_initial_du_ue_resource_config();
  auto& src_pucch_cfg =
      src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pucch_cfg.value();
  ASSERT_FALSE(src_pucch_cfg.pucch_res_list.empty());
  src_pucch_cfg.pucch_res_list.front().rep_factor = pucch_repetition_factor::n2;

  // Disable the repetition of the only repeated PUCCH resource.
  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto&                      dest_pucch_cfg =
      dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pucch_cfg.value();
  pucch_resource& res = dest_pucch_cfg.pucch_res_list.front();
  res.rep_factor      = pucch_repetition_factor::n1;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  byte_buffer   packed;
  asn1::bit_ref bref{packed};
  ASSERT_EQ(rrc_cell_grp_cfg.pack(bref), asn1::OCUDUASN_SUCCESS);
  asn1::rrc_nr::cell_group_cfg_s unpacked;
  asn1::cbit_ref                 cbref{packed};
  ASSERT_EQ(unpacked.unpack(cbref), asn1::OCUDUASN_SUCCESS);

  // resourceToAddModListExt-v1610 is Need N, so it must be present for pucch-RepetitionNrofSlots-r17 to be released.
  const auto& pucch_cfg = unpacked.sp_cell_cfg.sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.setup();
  ASSERT_EQ(pucch_cfg.res_to_add_mod_list.size(), 1);
  ASSERT_EQ(pucch_cfg.res_to_add_mod_list[0].pucch_res_id, res.res_id.ded().ue_res_id);
  ASSERT_TRUE(pucch_cfg.res_to_add_mod_list_ext_v1610.is_present());
  ASSERT_EQ(pucch_cfg.res_to_add_mod_list_ext_v1610->size(), 1);
  ASSERT_FALSE((*pucch_cfg.res_to_add_mod_list_ext_v1610)[0].pucch_repeat_nrof_slots_r17_present);
}

TEST(serving_cell_config_converter_test, test_ue_pucch_cfg_release_conversion)
{
  auto                       src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pucch_cfg.reset();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg_present,
            not dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pucch_cfg.has_value());
  // PUCCH Config is released due to absence in dest cell group config.
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pucch_cfg.type(), asn1::setup_release_opts::release);
}

TEST(serving_cell_config_converter_test, test_initial_pusch_cfg_conversion)
{
  auto dest_cfg = make_initial_du_ue_resource_config();
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pusch_cfg =
      make_initial_pusch_config();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg_present, dest_sp_cell_cfg_ded.ul_config.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg_present);
  // Since its initial setup and no source cell group config was provided pusch must be of setup type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pusch_cfg.has_value()) {
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().dmrs_ul_for_pusch_map_type_b_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().dmrs_ul_for_pusch_map_type_b.is_setup());

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup()
                    .dmrs_ul_for_pusch_map_type_b.setup()
                    .phase_tracking_rs_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup()
                    .dmrs_ul_for_pusch_map_type_b.setup()
                    .phase_tracking_rs.is_setup());

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl_present);
    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.pathloss_ref_rs_to_add_mod_list.size(),
        dest_sp_cell_cfg_ded.ul_config.value()
            .init_ul_bwp.pusch_cfg.value()
            .pusch_pwr_ctrl.value()
            .pathloss_ref_rs.size());
    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.pathloss_ref_rs_to_release_list.size(),
        0);

    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.sri_pusch_map_to_add_mod_list.size(),
        dest_sp_cell_cfg_ded.ul_config.value()
            .init_ul_bwp.pusch_cfg.value()
            .pusch_pwr_ctrl.value()
            .sri_pusch_mapping.size());
    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.sri_pusch_map_to_release_list.size(),
        0);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.p0_alpha_sets.size(),
              dest_sp_cell_cfg_ded.ul_config.value()
                  .init_ul_bwp.pusch_cfg.value()
                  .pusch_pwr_ctrl.value()
                  .p0_alphasets.size());

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_time_domain_alloc_list_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_time_domain_alloc_list.is_setup());
  }
}

// Verifies a non-default p0_nominal_without_grant is carried into the packed dedicated PUSCH power control.
TEST(serving_cell_config_converter_test, test_pusch_p0_nominal_without_grant_override_conversion)
{
  auto dest_cfg                                             = make_initial_du_ue_resource_config();
  auto pusch_cfg                                            = make_initial_pusch_config();
  pusch_cfg.pusch_pwr_ctrl.value().p0_nominal_without_grant = -90;
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pusch_cfg = pusch_cfg;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.p0_nominal_without_grant_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.p0_nominal_without_grant, -90);
}

TEST(serving_cell_config_converter_test, test_ue_custom_pusch_cfg_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();
  src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pusch_cfg =
      make_initial_pusch_config();

  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto&                      dest_pusch_cfg =
      dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pusch_cfg.value();
  // Add new/remove configurations.
  dest_pusch_cfg.pusch_mapping_type_a_dmrs.emplace();
  dest_pusch_cfg.pusch_mapping_type_a_dmrs.value().trans_precoder_enabled.emplace(
      dmrs_uplink_config::transform_precoder_enabled());
  dest_pusch_cfg.pusch_mapping_type_a_dmrs.value().trans_precoder_enabled.value().is_seq_hopping_enabled = true;
  // Release.
  dest_pusch_cfg.pusch_mapping_type_b_dmrs.reset();

  dest_pusch_cfg.pusch_pwr_ctrl.value().p0_alphasets.push_back(pusch_config::pusch_power_control::p0_pusch_alphaset{
      .id = static_cast<p0_pusch_alphaset_id>(1), .p0 = 2, .p0_pusch_alpha = alpha::alpha05});
  // Erase/Release.
  dest_pusch_cfg.pusch_pwr_ctrl.value().p0_alphasets.erase(dest_pusch_cfg.pusch_pwr_ctrl.value().p0_alphasets.begin());

  dest_pusch_cfg.pusch_pwr_ctrl.value().pathloss_ref_rs.push_back(
      pusch_config::pusch_power_control::pusch_pathloss_ref_rs{
          .id = static_cast<pusch_config::pusch_power_control::pusch_pathloss_ref_rs_id>(1),
          .rs = static_cast<nzp_csi_rs_res_id_t>(0)});
  // Erase/Release.
  dest_pusch_cfg.pusch_pwr_ctrl.value().pathloss_ref_rs.erase(
      dest_pusch_cfg.pusch_pwr_ctrl.value().pathloss_ref_rs.begin());

  dest_pusch_cfg.pusch_pwr_ctrl.value().sri_pusch_mapping.push_back(
      pusch_config::pusch_power_control::sri_pusch_pwr_ctrl{
          .id                           = static_cast<pusch_config::pusch_power_control::sri_pusch_pwr_ctrl_id>(1),
          .sri_pusch_pathloss_ref_rs_id = static_cast<pusch_config::pusch_power_control::pusch_pathloss_ref_rs_id>(1),
          .sri_p0_pusch_alphaset_id     = static_cast<p0_pusch_alphaset_id>(1),
          .closed_loop_idx = pusch_config::pusch_power_control::sri_pusch_pwr_ctrl::sri_pusch_closed_loop_index::i0});
  // Erase/Release.
  dest_pusch_cfg.pusch_pwr_ctrl.value().sri_pusch_mapping.erase(
      dest_pusch_cfg.pusch_pwr_ctrl.value().sri_pusch_mapping.begin());

  dest_pusch_cfg.pusch_td_alloc_list.push_back(pusch_time_domain_resource_allocation{
      .k2 = 4, .map_type = ocudu::sch_mapping_type::typeB, .symbols = ofdm_symbol_range{2, 12}});

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg_present, dest_sp_cell_cfg_ded.ul_config.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pusch_cfg.has_value()) {
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().dmrs_ul_for_pusch_map_type_a_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().dmrs_ul_for_pusch_map_type_a.is_setup());

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().dmrs_ul_for_pusch_map_type_b_present);
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().dmrs_ul_for_pusch_map_type_b.type(),
              asn1::setup_release_opts::release);

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl_present);
    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.pathloss_ref_rs_to_add_mod_list.size(),
        1);
    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.pathloss_ref_rs_to_release_list.size(),
        1);

    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.sri_pusch_map_to_add_mod_list.size(),
        1);
    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.sri_pusch_map_to_release_list.size(),
        1);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_pwr_ctrl.p0_alpha_sets.size(),
              dest_sp_cell_cfg_ded.ul_config.value()
                  .init_ul_bwp.pusch_cfg.value()
                  .pusch_pwr_ctrl.value()
                  .p0_alphasets.size());

    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_time_domain_alloc_list_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.setup().pusch_time_domain_alloc_list.is_setup());
  }
}

TEST(serving_cell_config_converter_test, test_ue_pusch_cfg_release_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();
  src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pusch_cfg =
      make_initial_pusch_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.pusch_cfg.reset();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg_present,
            not dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.pusch_cfg.has_value());
  // PUSCH Config is released due to absence in dest cell group config.
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.pusch_cfg.type(), asn1::setup_release_opts::release);
}

TEST(serving_cell_config_converter_test, test_initial_srs_cfg_conversion)
{
  auto dest_cfg = make_initial_du_ue_resource_config();
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.srs_cfg =
      make_initial_srs_config();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg_present, dest_sp_cell_cfg_ded.ul_config.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg_present);
  // Since its initial setup and no source cell group config was provided srs config must be of setup type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.srs_cfg.has_value()) {
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.setup().srs_res_set_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.srs_cfg.value().srs_res_set_list.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.setup().srs_res_set_to_release_list.size(), 0);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.setup().srs_res_to_add_mod_list.size(),
              dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.srs_cfg.value().srs_res_set_list.size());
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.setup().srs_res_to_release_list.size(), 0);
  }
}

TEST(serving_cell_config_converter_test, test_ue_custom_srs_cfg_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();
  src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.srs_cfg =
      make_initial_srs_config();

  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto&                      dest_pusch_cfg =
      dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.srs_cfg.value();
  // Add new/remove configurations.
  srs_config::srs_resource_set::semi_persistent_resource_type semi_persistent_resource;
  semi_persistent_resource.associated_csi_rs = static_cast<nzp_csi_rs_res_id_t>(1);
  dest_pusch_cfg.srs_res_set_list.push_back(
      srs_config::srs_resource_set{.id                 = static_cast<srs_config::srs_res_set_id>(1),
                                   .srs_res_id_list    = {static_cast<srs_config::srs_res_id>(1)},
                                   .res_type           = semi_persistent_resource,
                                   .srs_res_set_usage  = srs_usage::non_codebook,
                                   .srs_pwr_ctrl_alpha = alpha::alpha07,
                                   .p0                 = -70});
  // Release.
  dest_pusch_cfg.srs_res_set_list.erase(dest_pusch_cfg.srs_res_set_list.begin());

  srs_config::srs_resource::tx_comb_params tx_comb = {
      .size = ocudu::tx_comb_size::n4, .tx_comb_offset = 0, .tx_comb_cyclic_shift = 0};
  dest_pusch_cfg.srs_res_list.push_back(srs_config::srs_resource{
      .id                    = srs_config::srs_res_id_t{0U, static_cast<srs_config::srs_res_id>(1)},
      .nof_ports             = srs_config::srs_resource::nof_srs_ports::port1,
      .tx_comb               = tx_comb,
      .res_mapping           = srs_config::resource_mapping{.start_pos   = 50,
                                                            .nof_symb    = srs_nof_symbols::n1,
                                                            .rept_factor = srs_nof_symbols::n1},
      .freq_domain_pos       = 0,
      .freq_domain_shift     = 0,
      .freq_hop              = srs_config::srs_resource::frequency_hopping{.c_srs = 0, .b_srs = 0, .b_hop = 0},
      .grp_or_seq_hop        = srs_group_or_sequence_hopping::group_hopping,
      .res_type              = srs_resource_type::semi_persistent,
      .sequence_id           = 41,
      .spatial_relation_info = srs_config::srs_resource::srs_spatial_relation_info{
          .reference_signal = srs_config::srs_resource::srs_spatial_relation_info::srs_ref_signal{
              .res_id = static_cast<srs_config::srs_res_id>(1), .ul_bwp = static_cast<bwp_id_t>(1)}}});

  dest_pusch_cfg.srs_res_list.back().periodicity_and_offset.emplace(
      srs_config::srs_periodicity_and_offset{.period = ocudu::srs_periodicity::sl10, .offset = 5});

  // Release.
  dest_pusch_cfg.srs_res_list.erase(dest_pusch_cfg.srs_res_list.begin());

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg_present, dest_sp_cell_cfg_ded.ul_config.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg_present);
  // Since its initial setup and no source cell group config was provided srs config must be of setup type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.is_setup());

  if (dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.srs_cfg.has_value()) {
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.setup().srs_res_set_to_add_mod_list.size(), 1);
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.setup().srs_res_set_to_release_list.size(), 1);

    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.setup().srs_res_to_add_mod_list.size(), 1);
    ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.setup().srs_res_to_release_list.size(), 1);
  }
}

TEST(serving_cell_config_converter_test, test_ue_srs_cfg_release_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();
  src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.srs_cfg =
      make_initial_srs_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.ul_config.value().init_ul_bwp.srs_cfg.reset();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg_present,
            not dest_sp_cell_cfg_ded.ul_config.value().init_ul_bwp.srs_cfg.has_value());
  // SRS Config is released due to absence in dest cell group config.
  ASSERT_EQ(rrc_sp_cell_cfg_ded.ul_cfg.init_ul_bwp.srs_cfg.type(), asn1::setup_release_opts::release);
}

TEST(serving_cell_config_converter_test, test_initial_pdsch_serving_cell_cfg_conversion)
{
  auto dest_cfg = make_initial_du_ue_resource_config();

  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.pdsch_serv_cell_cfg->nof_harq_proc =
      pdsch_serving_cell_config::nof_harq_proc_for_pdsch::n8;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg_present, dest_sp_cell_cfg_ded.pdsch_serv_cell_cfg.has_value());
  // Since its initial setup and no source cell group config was provided PDSCH serving cell config must be of setup
  // type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.is_setup());

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().max_mimo_layers_present);
  ASSERT_FALSE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().nrof_harq_processes_for_pdsch_present)
      << "For 8 HARQs, this flag can be avoided";
}

TEST(serving_cell_config_converter_test, test_custom_pdsch_serving_cell_cfg_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();

  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto&                      dest_pdsch_serving_cell_cfg =
      dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.pdsch_serv_cell_cfg.value();
  // Add new/remove configurations. Need not be valid configuration.
  dest_pdsch_serving_cell_cfg.code_block_group_tx.emplace(pdsch_code_block_group_transmission{
      .max_cbg_per_tb                   = pdsch_code_block_group_transmission::max_code_block_groups_per_tb::n8,
      .code_block_group_flush_indicator = true});

  dest_pdsch_serving_cell_cfg.x_ov_head     = ocudu::x_overhead::xoh6;
  dest_pdsch_serving_cell_cfg.nof_harq_proc = ocudu::pdsch_serving_cell_config::nof_harq_proc_for_pdsch::n12;
  dest_pdsch_serving_cell_cfg.pucch_cell    = to_serv_cell_index(1);
  dest_pdsch_serving_cell_cfg.processing_type_2_enabled = false;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg_present, dest_sp_cell_cfg_ded.pdsch_serv_cell_cfg.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.is_setup());

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().code_block_group_tx_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().code_block_group_tx.is_setup());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().max_mimo_layers_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().nrof_harq_processes_for_pdsch_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().xoverhead_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().pucch_cell_present);
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.setup().processing_type2_enabled_present);
}

TEST(serving_cell_config_converter_test, test_pdsch_serving_cell_cfg_release_conversion)
{
  auto                       src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.pdsch_serv_cell_cfg.reset();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg_present,
            not dest_sp_cell_cfg_ded.pdsch_serv_cell_cfg.has_value());
  // PDSCH Serving Cell Config is released due to absence in dest cell group config.
  ASSERT_EQ(rrc_sp_cell_cfg_ded.pdsch_serving_cell_cfg.type(), asn1::setup_release_opts::release);
}

static csi_meas_config make_test_csi_meas_cfg()
{
  csi_helper::csi_meas_config_builder_params csi_params{};
  csi_params.pci                      = (pci_t)1;
  csi_params.nof_rbs                  = 52;
  csi_params.csi_params.csi_rs_period = csi_helper::get_max_csi_rs_period(subcarrier_spacing::kHz15);
  csi_params.mcs_table                = pdsch_mcs_table::qam64;
  // Note: Since by default we use periodic CSI, we pass an empty PUSCH TD allocation list.
  return csi_helper::make_csi_meas_config(csi_params, {});
}

TEST(serving_cell_config_converter_test, test_initial_csi_meas_cfg_conversion)
{
  auto dest_cfg = make_initial_du_ue_resource_config();

  if (not dest_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg.has_value()) {
    dest_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg = make_test_csi_meas_cfg();
  }

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.csi_meas_cfg_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg_present, dest_sp_cell_cfg_ded.csi_meas_cfg.has_value());
  // Since its initial setup and no source cell group config was provided csi meas config must be of setup type.
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.csi_meas_cfg.is_setup());

  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().nzp_csi_rs_res_to_add_mod_list.size(),
            dest_sp_cell_cfg_ded.csi_meas_cfg.value().nzp_csi_rs_res_list.size());
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().nzp_csi_rs_res_to_release_list.size(), 0);

  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().nzp_csi_rs_res_set_to_add_mod_list.size(),
            dest_sp_cell_cfg_ded.csi_meas_cfg.value().nzp_csi_rs_res_set_list.size());
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().nzp_csi_rs_res_set_to_release_list.size(), 0);

  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_im_res_to_add_mod_list.size(),
            dest_sp_cell_cfg_ded.csi_meas_cfg.value().csi_im_res_list.size());
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_im_res_to_release_list.size(), 0);

  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_im_res_set_to_add_mod_list.size(),
            dest_sp_cell_cfg_ded.csi_meas_cfg.value().csi_im_res_set_list.size());
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_im_res_set_to_release_list.size(), 0);

  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_ssb_res_set_to_add_mod_list.size(),
            dest_sp_cell_cfg_ded.csi_meas_cfg.value().csi_ssb_res_set_list.size());
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_ssb_res_set_to_release_list.size(), 0);

  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_res_cfg_to_add_mod_list.size(),
            dest_sp_cell_cfg_ded.csi_meas_cfg.value().csi_res_cfg_list.size());
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_res_cfg_to_release_list.size(), 0);

  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_report_cfg_to_add_mod_list.size(),
            dest_sp_cell_cfg_ded.csi_meas_cfg.value().csi_report_cfg_list.size());
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.setup().csi_report_cfg_to_release_list.size(), 0);
}

TEST(serving_cell_config_converter_test, test_custom_csi_meas_cfg_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();
  if (not src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.csi_meas_cfg.has_value()) {
    src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.csi_meas_cfg = make_test_csi_meas_cfg();
  }
  auto& src_meas = src_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.csi_meas_cfg.value();
  src_meas.nzp_csi_rs_res_list.resize(1);
  src_meas.nzp_csi_rs_res_set_list.resize(1);
  src_meas.csi_res_cfg_list.resize(1);

  odu::du_ue_resource_config dest_cfg{src_cfg};
  auto& dest_csi_meas_cfg = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.csi_meas_cfg.value();
  // Add new/remove configurations. Configuration need not be valid.
  // Resource 1.
  dest_csi_meas_cfg.nzp_csi_rs_res_list.push_back(dest_csi_meas_cfg.nzp_csi_rs_res_list[0]);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_id = static_cast<nzp_csi_rs_res_id_t>(1);
  // Resource 2.
  dest_csi_meas_cfg.nzp_csi_rs_res_list.push_back(dest_csi_meas_cfg.nzp_csi_rs_res_list[0]);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_id = static_cast<nzp_csi_rs_res_id_t>(2);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_mapping.first_ofdm_symbol_in_td = 8;
  // Resource 3.
  dest_csi_meas_cfg.nzp_csi_rs_res_list.push_back(dest_csi_meas_cfg.nzp_csi_rs_res_list[0]);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_id         = static_cast<nzp_csi_rs_res_id_t>(3);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().csi_res_offset = 12;
  // Resource 4.
  dest_csi_meas_cfg.nzp_csi_rs_res_list.push_back(dest_csi_meas_cfg.nzp_csi_rs_res_list[0]);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_id = static_cast<nzp_csi_rs_res_id_t>(4);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_mapping.first_ofdm_symbol_in_td = 8;
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().csi_res_offset                      = 12;
  // Resource 5.
  dest_csi_meas_cfg.nzp_csi_rs_res_list.push_back(dest_csi_meas_cfg.nzp_csi_rs_res_list[0]);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_id = static_cast<nzp_csi_rs_res_id_t>(5);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_mapping.fd_alloc.resize(12);
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_mapping.nof_ports    = 2;
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_mapping.cdm          = csi_rs_cdm_type::cdm4_FD2_TD2;
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().res_mapping.freq_density = csi_rs_freq_density_type::dot5_odd_RB;
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().csi_res_period           = csi_resource_periodicity::slots320;
  dest_csi_meas_cfg.nzp_csi_rs_res_list.back().csi_res_offset           = 4;
  // Remove.
  dest_csi_meas_cfg.nzp_csi_rs_res_list.erase(dest_csi_meas_cfg.nzp_csi_rs_res_list.begin());

  // Add new/remove configurations. Configuration need not be valid.
  // Resource Set 1.
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.push_back(dest_csi_meas_cfg.nzp_csi_rs_res_set_list[0]);
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.back().res_set_id          = static_cast<nzp_csi_rs_res_set_id_t>(1);
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.back().nzp_csi_rs_res      = {static_cast<nzp_csi_rs_res_id_t>(1),
                                                                          static_cast<nzp_csi_rs_res_id_t>(2),
                                                                          static_cast<nzp_csi_rs_res_id_t>(3),
                                                                          static_cast<nzp_csi_rs_res_id_t>(4)};
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.back().is_trs_info_present = true;
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.push_back(dest_csi_meas_cfg.nzp_csi_rs_res_set_list[0]);
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.back().res_set_id               = static_cast<nzp_csi_rs_res_set_id_t>(2);
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.back().nzp_csi_rs_res           = {static_cast<nzp_csi_rs_res_id_t>(5)};
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.back().is_repetition_on         = true;
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.back().aperiodic_trigger_offset = 4;
  // Remove.
  dest_csi_meas_cfg.nzp_csi_rs_res_set_list.erase(dest_csi_meas_cfg.nzp_csi_rs_res_set_list.begin());

  // Add new/remove configurations. Configuration need not be valid.
  dest_csi_meas_cfg.csi_im_res_list.push_back(dest_csi_meas_cfg.csi_im_res_list[0]);
  dest_csi_meas_cfg.csi_im_res_list.push_back(dest_csi_meas_cfg.csi_im_res_list[1]);
  dest_csi_meas_cfg.csi_im_res_list.back().res_id = static_cast<csi_im_res_id_t>(1);
  dest_csi_meas_cfg.csi_im_res_list.back().csi_im_res_element_pattern.emplace(
      csi_im_resource::csi_im_resource_element_pattern{
          .pattern_type        = csi_im_resource::csi_im_resource_element_pattern_type::pattern0,
          .subcarrier_location = 2,
          .symbol_location     = 2});
  dest_csi_meas_cfg.csi_im_res_list.back().freq_band_rbs = {4, 4 + 48};
  dest_csi_meas_cfg.csi_im_res_list.back().csi_res_period.emplace(csi_resource_periodicity::slots64);
  dest_csi_meas_cfg.csi_im_res_list.back().csi_res_offset.emplace(11);

  // Add new/remove configurations.
  dest_csi_meas_cfg.csi_im_res_set_list.push_back(dest_csi_meas_cfg.csi_im_res_set_list[0]);
  dest_csi_meas_cfg.csi_im_res_set_list.push_back(dest_csi_meas_cfg.csi_im_res_set_list[0]);
  dest_csi_meas_cfg.csi_im_res_set_list.back().res_set_id        = static_cast<csi_im_res_set_id_t>(1);
  dest_csi_meas_cfg.csi_im_res_set_list.back().csi_ims_resources = {static_cast<csi_im_res_id_t>(1)};

  // Add new configurations. Configuration need not be valid.
  dest_csi_meas_cfg.csi_ssb_res_set_list.push_back(
      csi_ssb_resource_set{.res_set_id = static_cast<csi_ssb_res_set_id_t>(0), .csi_ssb_res_list = {0}});

  // Add new/remove configurations. Configuration need not be valid.
  dest_csi_meas_cfg.csi_res_cfg_list.push_back(dest_csi_meas_cfg.csi_res_cfg_list[0]);
  dest_csi_meas_cfg.csi_res_cfg_list.back().res_cfg_id = static_cast<csi_res_config_id_t>(1);
  csi_resource_config::csi_im_resource_set_list csi_im_res_set{};
  csi_im_res_set.push_back(static_cast<const csi_im_res_set_id_t>(0));
  dest_csi_meas_cfg.csi_res_cfg_list.back().csi_rs_res_set_list = csi_im_res_set;
  dest_csi_meas_cfg.csi_res_cfg_list.push_back(dest_csi_meas_cfg.csi_res_cfg_list[0]);
  dest_csi_meas_cfg.csi_res_cfg_list.back().res_cfg_id = static_cast<csi_res_config_id_t>(2);
  csi_resource_config::nzp_csi_rs_ssb nzp_csi_rs_res_set{};
  nzp_csi_rs_res_set.nzp_csi_rs_res_set_list.push_back(static_cast<const nzp_csi_rs_res_set_id_t>(1));
  dest_csi_meas_cfg.csi_res_cfg_list.back().csi_rs_res_set_list = nzp_csi_rs_res_set;
  dest_csi_meas_cfg.csi_res_cfg_list.push_back(dest_csi_meas_cfg.csi_res_cfg_list[0]);
  dest_csi_meas_cfg.csi_res_cfg_list.back().res_cfg_id = static_cast<csi_res_config_id_t>(3);
  csi_resource_config::nzp_csi_rs_ssb nzp_csi_res_set{};
  nzp_csi_res_set.nzp_csi_rs_res_set_list.push_back(static_cast<const nzp_csi_rs_res_set_id_t>(1));
  nzp_csi_res_set.csi_ssb_res_set_list.push_back(static_cast<const csi_ssb_res_set_id_t>(0));
  dest_csi_meas_cfg.csi_res_cfg_list.back().csi_rs_res_set_list = nzp_csi_res_set;
  dest_csi_meas_cfg.csi_res_cfg_list.back().res_type            = csi_resource_config::resource_type::semiPersistent;
  // Remove.
  dest_csi_meas_cfg.csi_res_cfg_list.erase(dest_csi_meas_cfg.csi_res_cfg_list.begin());

  // Add new/remove configurations. Configuration need not be valid.
  dest_csi_meas_cfg.csi_report_cfg_list.push_back(dest_csi_meas_cfg.csi_report_cfg_list[0]);
  dest_csi_meas_cfg.csi_report_cfg_list.back().report_cfg_id               = static_cast<csi_report_config_id_t>(1);
  dest_csi_meas_cfg.csi_report_cfg_list.back().res_for_channel_meas        = static_cast<csi_res_config_id_t>(2);
  dest_csi_meas_cfg.csi_report_cfg_list.back().csi_im_res_for_interference = static_cast<csi_res_config_id_t>(3);

  csi_report_config::semi_persistent_report_on_pusch report_cfg_type{};
  report_cfg_type.slot_cfg = csi_report_periodicity::slots5;
  report_cfg_type.report_slot_offset_list.push_back(9);
  report_cfg_type.p0_alpha                                     = static_cast<p0_pusch_alphaset_id>(10);
  dest_csi_meas_cfg.csi_report_cfg_list.back().report_cfg_type = report_cfg_type;

  dest_csi_meas_cfg.csi_report_cfg_list.back().report_qty_type =
      csi_report_config::report_quantity_type_t::cri_ri_i1_cqi;
  dest_csi_meas_cfg.csi_report_cfg_list.back().pdsch_bundle_size_for_csi =
      csi_report_config::csi_ri_i1_cqi_pdsch_bundle_size_for_csi::n4;

  dest_csi_meas_cfg.csi_report_cfg_list.back().report_freq_cfg.emplace();
  dest_csi_meas_cfg.csi_report_cfg_list.back().report_freq_cfg.value().cqi_format_ind =
      csi_report_config::cqi_format_indicator::subband_cqi;
  dest_csi_meas_cfg.csi_report_cfg_list.back().report_freq_cfg.value().pmi_format_ind =
      csi_report_config::pmi_format_indicator::subband_pmi;

  dest_csi_meas_cfg.csi_report_cfg_list.back().is_time_restrictions_for_channel_meas_configured      = true;
  dest_csi_meas_cfg.csi_report_cfg_list.back().is_time_restrictions_for_interference_meas_configured = true;

  dest_csi_meas_cfg.csi_report_cfg_list.back().codebook_cfg.emplace();
  auto ant_restriction = codebook_config::type2::typeii::n1_n2_codebook_subset_restriction_value_t(139);
  // '111111'B.
  ant_restriction.all();
  codebook_config::type2::typeii sub_type{};
  sub_type.n1_n2_codebook_subset_restriction_type =
      ocudu::codebook_config::type2::typeii::n1_n2_codebook_subset_restriction_type_t::four_four;
  sub_type.n1_n2_codebook_subset_restriction_value = ant_restriction;
  sub_type.typeii_ri_restriction                   = bounded_bitset<2>(2);
  // '03'H.
  sub_type.typeii_ri_restriction.from_uint64(0x03);
  codebook_config::type2 type2{};
  type2.sub_type            = sub_type;
  type2.phase_alphabet_size = pmi_codebook_typeII_phase_size::qpsk;
  type2.subband_amplitude   = true;
  type2.nof_beams           = 2;
  dest_csi_meas_cfg.csi_report_cfg_list.back().codebook_cfg.value().codebook_type = type2;

  dest_csi_meas_cfg.csi_report_cfg_list.back().is_group_based_beam_reporting_enabled = true;
  dest_csi_meas_cfg.csi_report_cfg_list.back().cqi_table.emplace(cqi_table_t::table1);
  dest_csi_meas_cfg.csi_report_cfg_list.back().subband_size = csi_report_config::subband_size_t::value2;
  dest_csi_meas_cfg.csi_report_cfg_list.back().non_pmi_port_indication.push_back(
      csi_report_config::port_index_for_8_ranks{
          .port_index_type = ocudu::csi_report_config::port_index_for_8_ranks::port_index_type_t::port_index_4,
          .rank1_x         = 1,
          .rank2_x         = {2, 4},
          .rank3_x         = {3},
          .rank4_x         = {5, 6, 7}});
  // Remove.
  dest_csi_meas_cfg.csi_report_cfg_list.erase(dest_csi_meas_cfg.csi_report_cfg_list.begin());

  dest_csi_meas_cfg.report_trigger_size = 2;

  auto associated_report_cfg_info_list =
      csi_associated_report_config_info{.report_cfg_id   = static_cast<csi_report_config_id_t>(1),
                                        .res_for_channel = csi_associated_report_config_info::csi_ssb_resource_set{1}};
  dest_csi_meas_cfg.aperiodic_trigger_state_list.push_back(
      csi_aperiodic_trigger_state{.associated_report_cfg_info_list = {associated_report_cfg_info_list}});

  dest_csi_meas_cfg.semi_persistent_on_pusch_trigger_state_list.emplace();
  dest_csi_meas_cfg.semi_persistent_on_pusch_trigger_state_list.value().push_back(
      csi_semi_persistent_on_pusch_trigger_state{.associated_report_cfg_info = static_cast<csi_report_config_id_t>(1)});

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.csi_meas_cfg_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg_present, dest_sp_cell_cfg_ded.csi_meas_cfg.has_value());
  ASSERT_TRUE(rrc_sp_cell_cfg_ded.csi_meas_cfg.is_setup());
  const auto& setup = rrc_sp_cell_cfg_ded.csi_meas_cfg.setup();

  ASSERT_EQ(setup.nzp_csi_rs_res_to_add_mod_list.size(), 5);
  ASSERT_EQ(setup.nzp_csi_rs_res_to_release_list.size(), 1);

  ASSERT_EQ(setup.nzp_csi_rs_res_set_to_add_mod_list.size(), 2);
  ASSERT_EQ(setup.nzp_csi_rs_res_set_to_release_list.size(), 1);

  ASSERT_EQ(setup.csi_im_res_to_add_mod_list.size(), 2);
  ASSERT_EQ(setup.csi_im_res_to_release_list.size(), 0);

  ASSERT_EQ(setup.csi_im_res_set_to_add_mod_list.size(), 2);
  ASSERT_EQ(setup.csi_im_res_set_to_release_list.size(), 0);

  ASSERT_EQ(setup.csi_ssb_res_set_to_add_mod_list.size(), 1);
  ASSERT_EQ(setup.csi_ssb_res_set_to_release_list.size(), 0);

  ASSERT_EQ(setup.csi_res_cfg_to_add_mod_list.size(), 3);
  ASSERT_EQ(setup.csi_res_cfg_to_release_list.size(), 1);

  ASSERT_EQ(setup.csi_report_cfg_to_add_mod_list.size(), 1);
  ASSERT_EQ(setup.csi_report_cfg_to_release_list.size(), 1);

  ASSERT_TRUE(setup.aperiodic_trigger_state_list_present);
  ASSERT_TRUE(setup.aperiodic_trigger_state_list.is_setup());

  ASSERT_TRUE(setup.semi_persistent_on_pusch_trigger_state_list_present);
  ASSERT_TRUE(setup.semi_persistent_on_pusch_trigger_state_list.is_setup());
}

TEST(serving_cell_config_converter_test, test_csi_meas_cfg_release_conversion)
{
  auto src_cfg = make_initial_du_ue_resource_config();

  if (not src_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg.has_value()) {
    src_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg = make_test_csi_meas_cfg();
  }

  odu::du_ue_resource_config dest_cfg{src_cfg};
  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg.csi_meas_cfg.reset();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg_present);
  ASSERT_TRUE(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

  auto& rrc_sp_cell_cfg_ded  = rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded;
  auto& dest_sp_cell_cfg_ded = dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX).serv_cell_cfg;

  ASSERT_TRUE(rrc_sp_cell_cfg_ded.csi_meas_cfg_present);
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg_present, not dest_sp_cell_cfg_ded.csi_meas_cfg.has_value());
  // CSI-Meas Config is released due to absence in dest cell group config.
  ASSERT_EQ(rrc_sp_cell_cfg_ded.csi_meas_cfg.type(), asn1::setup_release_opts::release);
}

TEST(serving_cell_config_converter_test, test_rlm_cfg_conversion)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-RLM-16-1");

  using namespace asn1::rrc_nr;

  // Some lambda to check recurrent configs.
  auto validate_basic_sp_cell_cfg = [](const cell_group_cfg_s& cell_gr_cfg, size_t sz) {
    ASSERT_TRUE(cell_gr_cfg.sp_cell_cfg_present);
    ASSERT_TRUE(cell_gr_cfg.sp_cell_cfg.sp_cell_cfg_ded_present);

    const auto& rrc_sp_cell_cfg_ded = cell_gr_cfg.sp_cell_cfg.sp_cell_cfg_ded;
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.radio_link_monitoring_cfg_present);
    ASSERT_TRUE(rrc_sp_cell_cfg_ded.init_dl_bwp.radio_link_monitoring_cfg.is_setup());
    ASSERT_EQ(
        rrc_sp_cell_cfg_ded.init_dl_bwp.radio_link_monitoring_cfg.setup().fail_detection_res_to_add_mod_list.size(),
        sz);
  };

  auto validate_rlm_ssb_resource = [](const serving_cell_cfg_s& sp_cell_cfg) {
    ASSERT_EQ(
        sp_cell_cfg.init_dl_bwp.radio_link_monitoring_cfg.setup().fail_detection_res_to_add_mod_list[0U].purpose.value,
        radio_link_monitoring_rs_s::purpose_opts::rlf);
    ASSERT_EQ(sp_cell_cfg.init_dl_bwp.radio_link_monitoring_cfg.setup()
                  .fail_detection_res_to_add_mod_list[0U]
                  .detection_res.type(),
              radio_link_monitoring_rs_s::detection_res_c_::types_opts::ssb_idx);
    ASSERT_EQ(sp_cell_cfg.init_dl_bwp.radio_link_monitoring_cfg.setup()
                  .fail_detection_res_to_add_mod_list[0U]
                  .detection_res.ssb_idx(),
              static_cast<uint8_t>(0U));
  };

  auto validate_rlm_csi_rs_resources = [](const serving_cell_cfg_s& sp_cell_cfg,
                                          span<uint8_t>             rlm_res_indices,
                                          span<uint8_t>             csi_rs_res_indices) {
    ASSERT_EQ(rlm_res_indices.size(), csi_rs_res_indices.size());

    for (auto rlm_res_idx = rlm_res_indices.begin(), csi_rs_res_idx = csi_rs_res_indices.begin();
         rlm_res_idx != rlm_res_indices.end();
         rlm_res_idx++, csi_rs_res_idx++) {
      ASSERT_EQ(sp_cell_cfg.init_dl_bwp.radio_link_monitoring_cfg.setup()
                    .fail_detection_res_to_add_mod_list[*rlm_res_idx]
                    .purpose.value,
                radio_link_monitoring_rs_s::purpose_opts::rlf)
          << fmt::format("Invalid purpose for RLM resource idx={}", *rlm_res_idx);
      ASSERT_EQ(sp_cell_cfg.init_dl_bwp.radio_link_monitoring_cfg.setup()
                    .fail_detection_res_to_add_mod_list[*rlm_res_idx]
                    .detection_res.type(),
                radio_link_monitoring_rs_s::detection_res_c_::types_opts::csi_rs_idx)
          << fmt::format("Invalid detection resource type for RLM resource idx={}", *rlm_res_idx);
      ASSERT_EQ(sp_cell_cfg.init_dl_bwp.radio_link_monitoring_cfg.setup()
                    .fail_detection_res_to_add_mod_list[*rlm_res_idx]
                    .detection_res.csi_rs_idx(),
                *csi_rs_res_idx)
          << fmt::format("Invalid detection CSI-res idx ({}) for RLM resource idx={}", *csi_rs_res_idx, *rlm_res_idx);
    }
  };

  auto src_cfg = make_initial_du_ue_resource_config();

  if (not src_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg.has_value()) {
    src_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg = make_test_csi_meas_cfg();
  }

  odu::du_ue_resource_config dest_cfg{src_cfg};

  // 1. Make a RLM config that uses SSB resources.
  uint8_t                        L_max = 4U;
  rlm_helper::rlm_builder_params rlm_params(rlm_resource_type::ssb, L_max, ssb_bitmap_t(0b1000, L_max));

  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX)
      .serv_cell_cfg.init_dl_bwp.rlm_cfg.emplace(rlm_helper::make_radio_link_monitoring_config(
          rlm_params,
          dest_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg.value().nzp_csi_rs_res_list));

  cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  unsigned nof_rlm_resources = 1U;
  validate_basic_sp_cell_cfg(rrc_cell_grp_cfg, nof_rlm_resources);
  validate_rlm_ssb_resource(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded);

  // 2. Make a RLM config that uses CSI-RS resources.
  L_max      = 8U;
  rlm_params = rlm_helper::rlm_builder_params(rlm_resource_type::csi_rs, L_max);

  // Set the DEST configuration as the new SRC.
  src_cfg = dest_cfg;

  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX)
      .serv_cell_cfg.init_dl_bwp.rlm_cfg.emplace(rlm_helper::make_radio_link_monitoring_config(
          rlm_params,
          dest_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg.value().nzp_csi_rs_res_list));

  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  // Nof RLM expected resources (this depends on L_max).
  nof_rlm_resources = 4U;
  validate_basic_sp_cell_cfg(rrc_cell_grp_cfg, nof_rlm_resources);

  // The CSI-RS resources are those for tracking, which are indexed starting from 1.
  std::array<uint8_t, 4U> rlm_res_indices    = {0, 1, 2, 3};
  std::array<uint8_t, 4U> csi_rs_res_indices = {1, 2, 3, 4};
  validate_rlm_csi_rs_resources(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded, rlm_res_indices, csi_rs_res_indices);

  // 3. Make a RLM config that uses both SSB and CSI-RS resources.
  rlm_params =
      rlm_helper::rlm_builder_params(rlm_resource_type::ssb_and_csi_rs, L_max, ssb_bitmap_t(0b10000000, L_max));

  // Set the DEST (with SSB config) configuration as the new SRC.
  src_cfg = dest_cfg;

  dest_cfg.cell_group.cells.at(SERVING_PCELL_IDX)
      .serv_cell_cfg.init_dl_bwp.rlm_cfg.emplace(rlm_helper::make_radio_link_monitoring_config(
          rlm_params,
          dest_cfg.cell_group.cells.begin()->second.serv_cell_cfg.csi_meas_cfg.value().nzp_csi_rs_res_list));

  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  // Nof RLM expected resources (this depends on L_max).
  validate_basic_sp_cell_cfg(rrc_cell_grp_cfg, nof_rlm_resources);
  validate_rlm_ssb_resource(rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded);
  // With both SSB and CSI-RS resources used for RLM, the first RLM resource (idx 0) uses SSB, the second RLM resource
  // (idx 1) uses CSI-RS resource idx 1 (the first CSI-RS for tracking), etc., until max 4 RLM resources are reached.
  std::array<uint8_t, 3U> rlm_res_new_indices    = {1, 2, 3};
  std::array<uint8_t, 3U> csi_rs_res_new_indices = {1, 2, 3};
  validate_rlm_csi_rs_resources(
      rrc_cell_grp_cfg.sp_cell_cfg.sp_cell_cfg_ded, rlm_res_new_indices, csi_rs_res_new_indices);
}

/// Returns the JSON representation of an ASN.1 object, to compare two objects in full.
template <typename T>
static std::string to_json_string(const T& obj)
{
  asn1::json_writer js;
  obj.to_json(js);
  return js.to_string();
}

TEST(serving_cell_config_converter_test, handover_to_an_ntn_cell_carries_the_ntn_config_of_the_target_cell)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-MOB-2");

  odu::du_cell_config target_cell = config_helpers::make_default_du_cell_config();
  target_cell.ran.ntn_params.emplace();
  target_cell.ran.ntn_params->ntn_cfg.cell_specific_koffset = std::chrono::milliseconds{20};
  target_cell.ran.ntn_params->ntn_cfg.k_mac                 = std::chrono::milliseconds{4};

  asn1::rrc_nr::recfg_with_sync_s recfg_with_sync;
  ASSERT_TRUE(odu::calculate_reconfig_with_sync_diff(recfg_with_sync,
                                                     target_cell,
                                                     make_initial_du_ue_resource_config(),
                                                     asn1::rrc_nr::ho_prep_info_s{},
                                                     to_rnti(0x4601)));

  // TS 38.331, ServingCellConfigCommon: the UE learns the NTN-Config of the target cell from the handover command,
  // since it cannot read SIB19 of the target cell before accessing it.
  ASSERT_TRUE(recfg_with_sync.sp_cell_cfg_common_present);
  ASSERT_TRUE(recfg_with_sync.sp_cell_cfg_common.ntn_cfg_r17.is_present());
  ASSERT_EQ(to_json_string(*recfg_with_sync.sp_cell_cfg_common.ntn_cfg_r17),
            to_json_string(odu::make_asn1_rrc_cell_ntn_cfg(target_cell.ran.ntn_params->ntn_cfg)));
  ASSERT_TRUE(recfg_with_sync.sp_cell_cfg_common.ntn_cfg_r17->cell_specific_koffset_r17_present);
  ASSERT_EQ(recfg_with_sync.sp_cell_cfg_common.ntn_cfg_r17->cell_specific_koffset_r17, 20);
}

TEST(serving_cell_config_converter_test, handover_to_a_terrestrial_cell_carries_no_ntn_config)
{
  OCUDU_TEST_REQUIREMENTS("DU-NTN-MOB-2");

  const odu::du_cell_config target_cell = config_helpers::make_default_du_cell_config();

  asn1::rrc_nr::recfg_with_sync_s recfg_with_sync;
  ASSERT_TRUE(odu::calculate_reconfig_with_sync_diff(recfg_with_sync,
                                                     target_cell,
                                                     make_initial_du_ue_resource_config(),
                                                     asn1::rrc_nr::ho_prep_info_s{},
                                                     to_rnti(0x4601)));

  ASSERT_TRUE(recfg_with_sync.sp_cell_cfg_common_present);
  ASSERT_FALSE(recfg_with_sync.sp_cell_cfg_common.ntn_cfg_r17.is_present());
}
