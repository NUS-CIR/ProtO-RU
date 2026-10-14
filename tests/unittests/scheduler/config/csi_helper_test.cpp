// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "tests/ocudu_test_requirements.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/csi_report/csi_report_config_helpers.h"
#include "ocudu/ran/csi_report/csi_report_on_pucch_helpers.h"
#include "ocudu/ran/csi_report/csi_report_on_pusch_helpers.h"
#include "ocudu/ran/tdd/tdd_ul_dl_config_formatters.h"
#include "ocudu/scheduler/config/bwp_configuration.h"
#include "ocudu/scheduler/config/cell_bwp_res_config.h"
#include "ocudu/scheduler/config/csi_helper.h"
#include "ocudu/scheduler/config/ran_cell_config_helper.h"
#include "ocudu/scheduler/config/serving_cell_config_factory.h"
#include "ocudu/scheduler/config/serving_cell_config_validator.h"
#include "ocudu/support/enum_utils.h"
#include <gtest/gtest.h>

using namespace ocudu;

namespace ocudu {

void PrintTo(const tdd_ul_dl_config_common& cfg, std::ostream* os)
{
  *os << fmt::format("{}", cfg);
}

} // namespace ocudu

class csi_rs_slot_derivation_test : public ::testing::TestWithParam<tdd_ul_dl_config_common>
{
protected:
  csi_rs_slot_derivation_test()
  {
    static constexpr std::array<unsigned, 4> def_track_csi_ofdm_symbol_idx = {6, 10, 6, 10};
    const unsigned                           max_csi_symbol =
        *std::max_element(def_track_csi_ofdm_symbol_idx.begin(), def_track_csi_ofdm_symbol_idx.end());
    static constexpr ssb_periodicity         default_ssb_period = ssb_periodicity::ms10;
    static constexpr std::array<unsigned, 1> default_ssb_slots  = {0U};
    const bool                               ret = csi_helper::derive_valid_csi_rs_slot_offsets(result.csi_params,
                                                                  std::nullopt,
                                                                  std::nullopt,
                                                                                                {},
                                                                  tdd_cfg,
                                                                  max_csi_symbol,
                                                                  default_ssb_period,
                                                                  default_ssb_slots,
                                                                  1U,
                                                                                                {});
    report_error_if_not(ret, "Derivation failed");
  }

  tdd_ul_dl_config_common                    tdd_cfg = GetParam();
  csi_helper::csi_meas_config_builder_params result{};
};

TEST_P(csi_rs_slot_derivation_test, csi_rs_slot_offset_fall_in_dl_slots)
{
  static const unsigned ZP_SYMBOL_IDX = 8, MEAS_SYMBOL_IDX = 4, TRACKING_MAX_SYMBOL_IDX = 8;

  ASSERT_GE(get_active_tdd_dl_symbols(tdd_cfg, result.csi_params.zp_csi_slot_offsets[0], cyclic_prefix::NORMAL).stop(),
            ZP_SYMBOL_IDX);
  ASSERT_GE(
      get_active_tdd_dl_symbols(tdd_cfg, result.csi_params.meas_csi_slot_offsets[0], cyclic_prefix::NORMAL).stop(),
      MEAS_SYMBOL_IDX);
  // Note: Tracking occupies two consecutive slots.
  ASSERT_GE(
      get_active_tdd_dl_symbols(tdd_cfg, result.csi_params.tracking_csi_slot_offset, cyclic_prefix::NORMAL).stop(),
      TRACKING_MAX_SYMBOL_IDX);
  ASSERT_GE(
      get_active_tdd_dl_symbols(tdd_cfg, result.csi_params.tracking_csi_slot_offset + 1, cyclic_prefix::NORMAL).stop(),
      TRACKING_MAX_SYMBOL_IDX);
}

TEST_P(csi_rs_slot_derivation_test, csi_rs_slot_offsets_do_not_collide)
{
  // Note: ZP and NZP-CSI-RS slots are always in different symbols.
  ASSERT_NE(result.csi_params.zp_csi_slot_offsets[0], result.csi_params.tracking_csi_slot_offset);
  ASSERT_NE(result.csi_params.zp_csi_slot_offsets[0], result.csi_params.tracking_csi_slot_offset + 1);
  ASSERT_NE(result.csi_params.meas_csi_slot_offsets[0], result.csi_params.tracking_csi_slot_offset);
  ASSERT_NE(result.csi_params.meas_csi_slot_offsets[0], result.csi_params.tracking_csi_slot_offset + 1);
}

TEST_P(csi_rs_slot_derivation_test, generated_csi_meas_config_validation)
{
  serving_cell_config cell_cfg =
      config_helpers::make_default_ue_cell_config(config_helpers::make_default_ran_cell_config()).serv_cell_cfg;
  result.nof_rbs   = 52;
  result.mcs_table = pdsch_mcs_table::qam64;
  // Note: Since by default we use periodic CSI, we don't care about pusch_td_alloc_list or ul_config_common.
  cell_cfg.csi_meas_cfg = make_csi_meas_config(result, {});
  ul_config_common ul_cfg_cmn{};
  config_validators::validate_csi_meas_cfg(cell_cfg, tdd_cfg, ul_cfg_cmn);
}

INSTANTIATE_TEST_SUITE_P(
    csi_helper_test,
    csi_rs_slot_derivation_test,
    // clang-format off
    ::testing::Values(tdd_ul_dl_config_common{subcarrier_spacing::kHz30, {4,  2, 9, 1, 0}, std::nullopt},
                      tdd_ul_dl_config_common{subcarrier_spacing::kHz30, {10, 6, 9, 3, 0}, std::nullopt},
                      tdd_ul_dl_config_common{subcarrier_spacing::kHz30, {10, 7, 9, 2, 0}, std::nullopt}));
// clang-format on

static csi_helper::csi_meas_config_builder_params make_csi_params_for_ports(unsigned nof_ports)
{
  static const tdd_ul_dl_config_common     tdd_cfg{subcarrier_spacing::kHz30, {10, 6, 9, 3, 0}, std::nullopt};
  static constexpr ssb_periodicity         ssb_period     = ssb_periodicity::ms10;
  static constexpr std::array<unsigned, 1> ssb_slots      = {0U};
  static constexpr std::array<unsigned, 4> track_sym      = {6, 10, 6, 10};
  const unsigned                           max_csi_symbol = *std::max_element(track_sym.begin(), track_sym.end());

  csi_helper::csi_meas_config_builder_params params{};
  params.nof_ports                = nof_ports;
  params.max_nof_layers           = std::min(nof_ports, 4U);
  params.nof_rbs                  = 52;
  params.mcs_table                = pdsch_mcs_table::qam64;
  params.csi_params.csi_rs_period = csi_helper::get_max_csi_rs_period(tdd_cfg.ref_scs);
  report_error_if_not(
      csi_helper::derive_valid_csi_rs_slot_offsets(
          params.csi_params, std::nullopt, std::nullopt, {}, tdd_cfg, max_csi_symbol, ssb_period, ssb_slots, 1U, {}),
      "Derivation failed");
  return params;
}

TEST(csi_helper_test, zp_csi_rs_resource_with_eight_ports_uses_row6)
{
  const csi_helper::csi_meas_config_builder_params params = make_csi_params_for_ports(8);

  const std::vector<zp_csi_rs_resource> zp_list = csi_helper::make_periodic_zp_csi_rs_resource_list(params);
  ASSERT_FALSE(zp_list.empty());
  const csi_rs_resource_mapping& mapping = zp_list.front().res_mapping;
  // Row 6 of TS 38.211 Table 7.4.1.5.3-1: 8 ports, 6 possible FD locations with 4 occupied, CDM group of 2.
  EXPECT_EQ(mapping.nof_ports, 8);
  EXPECT_EQ(mapping.fd_alloc.size(), 6);
  EXPECT_EQ(mapping.cdm, csi_rs_cdm_type::fd_CDM2);
  // The lowest four FD locations {0, 1, 2, 3} are occupied (fill(0, 4)).
  EXPECT_EQ(mapping.fd_alloc.count(), 4);
  for (unsigned pos = 0; pos != mapping.fd_alloc.size(); ++pos) {
    EXPECT_EQ(mapping.fd_alloc.test(pos), pos < 4) << "unexpected ZP-CSI-RS FD allocation at position " << pos;
  }
}

TEST(csi_helper_test, nzp_channel_measurement_resource_supports_eight_ports)
{
  const csi_helper::csi_meas_config_builder_params params = make_csi_params_for_ports(8);

  const std::vector<nzp_csi_rs_resource> nzp_list = csi_helper::make_nzp_csi_rs_resource_list(params);
  const auto                             cm_res   = std::find_if(
      nzp_list.begin(), nzp_list.end(), [](const nzp_csi_rs_resource& res) { return res.res_mapping.nof_ports == 8; });
  ASSERT_NE(cm_res, nzp_list.end()) << "No 8-port channel-measurement NZP-CSI-RS resource was generated";
  // 8 ports use FD-CDM2 over 6 possible FD locations, with 4 (k_hat) locations occupied.
  const csi_rs_resource_mapping& mapping = cm_res->res_mapping;
  EXPECT_EQ(mapping.fd_alloc.size(), 6);
  EXPECT_EQ(mapping.cdm, csi_rs_cdm_type::fd_CDM2);
  // The occupied locations are the four contiguous positions (pci + k_hat) mod 6, k_hat = 0..3.
  std::array<bool, 6> expected_occupied = {};
  for (unsigned i_k = 0; i_k != 4; ++i_k) {
    expected_occupied[(static_cast<unsigned>(params.pci) + i_k) % 6] = true;
  }
  EXPECT_EQ(mapping.fd_alloc.count(), 4);
  for (unsigned pos = 0; pos != mapping.fd_alloc.size(); ++pos) {
    EXPECT_EQ(mapping.fd_alloc.test(pos), expected_occupied[pos])
        << "unexpected NZP-CSI-RS FD allocation at position " << pos;
  }
}

TEST(csi_helper_test, csi_report_config_with_eight_ports_uses_four_one_codebook)
{
  const csi_helper::csi_meas_config_builder_params params = make_csi_params_for_ports(8);

  const csi_meas_config cfg = csi_helper::make_csi_meas_config(params, {});
  const auto            rep = std::find_if(cfg.csi_report_cfg_list.begin(),
                                cfg.csi_report_cfg_list.end(),
                                [](const csi_report_config& r) { return r.codebook_cfg.has_value(); });
  ASSERT_NE(rep, cfg.csi_report_cfg_list.end()) << "No CSI report with a codebook config was generated";

  ASSERT_TRUE(std::holds_alternative<codebook_config::type1>(rep->codebook_cfg->codebook_type));
  const auto& type1 = std::get<codebook_config::type1>(rep->codebook_cfg->codebook_type);
  ASSERT_TRUE(std::holds_alternative<codebook_config::type1::single_panel>(type1.sub_type));
  const auto& single_panel = std::get<codebook_config::type1::single_panel>(type1.sub_type);
  ASSERT_TRUE(std::holds_alternative<codebook_config::type1::single_panel::more_than_two_antenna_ports>(
      single_panel.nof_antenna_ports));
  const auto& port_cfg =
      std::get<codebook_config::type1::single_panel::more_than_two_antenna_ports>(single_panel.nof_antenna_ports);
  EXPECT_EQ(port_cfg.n1_n2_restriction_type, pmi_codebook_single_panel_config::four_one);
}

TEST(csi_helper_test, ssb_slot_offsets_are_all_avoided)
{
  // With L_max=4 and all beams active, SSBs occupy slots 0 and 1 (for 30 kHz SCS, case C).
  // Verify that no derived CSI-RS offset lands on either of those slots within the SSB period.
  static const tdd_ul_dl_config_common     tdd_cfg{subcarrier_spacing::kHz30, {10, 6, 9, 3, 0}, std::nullopt};
  static constexpr ssb_periodicity         ssb_period = ssb_periodicity::ms10;
  static constexpr std::array<unsigned, 2> ssb_slots  = {0U, 1U};

  static constexpr std::array<unsigned, 4> track_sym      = {6, 10, 6, 10};
  const unsigned                           max_csi_symbol = *std::max_element(track_sym.begin(), track_sym.end());

  csi_helper::csi_meas_config_builder_params params{};
  params.csi_params.csi_rs_period = csi_helper::get_max_csi_rs_period(tdd_cfg.ref_scs);

  ASSERT_TRUE(csi_helper::derive_valid_csi_rs_slot_offsets(
      params.csi_params, std::nullopt, std::nullopt, {}, tdd_cfg, max_csi_symbol, ssb_period, ssb_slots, 1U, {}));

  const unsigned ssb_period_slots = to_underlying(ssb_period) * get_nof_slots_per_subframe(tdd_cfg.ref_scs);
  for (unsigned ssb_slot : ssb_slots) {
    EXPECT_NE(params.csi_params.meas_csi_slot_offsets[0] % ssb_period_slots, ssb_slot)
        << "meas_csi_slot_offsets[0] collides with SSB slot " << ssb_slot;
    EXPECT_NE(params.csi_params.zp_csi_slot_offsets[0] % ssb_period_slots, ssb_slot)
        << "zp_csi_slot_offsets[0] collides with SSB slot " << ssb_slot;
    EXPECT_NE(params.csi_params.tracking_csi_slot_offset % ssb_period_slots, ssb_slot)
        << "tracking_csi_slot_offset collides with SSB slot " << ssb_slot;
    EXPECT_NE((params.csi_params.tracking_csi_slot_offset + 1) % ssb_period_slots, ssb_slot)
        << "tracking_csi_slot_offset+1 collides with SSB slot " << ssb_slot;
  }
}

/// Builds CSI parameters that request a Type-II codebook for the given number of ports.
static csi_helper::csi_meas_config_builder_params make_type2_csi_params(unsigned nof_ports, unsigned nof_beams)
{
  csi_helper::csi_meas_config_builder_params params = make_csi_params_for_ports(nof_ports);

  // The Type-II PMI is carried in CSI Part 2, which is only multiplexed in PUSCH.
  params.csi_params.csi_report_slot_offset.reset();
  params.csi_params.enable_aperiodic_report = true;

  du_type2_codebook_params& type2 = params.csi_params.type2_codebook.emplace();
  type2.nof_beams                 = nof_beams;
  type2.phase_alphabet_size       = pmi_codebook_typeII_phase_size::psk8;

  return params;
}

/// Returns the codebook configuration of the first CSI report that carries one.
static codebook_config get_codebook_config(const csi_meas_config& cfg)
{
  const auto rep = std::find_if(cfg.csi_report_cfg_list.begin(),
                                cfg.csi_report_cfg_list.end(),
                                [](const csi_report_config& r) { return r.codebook_cfg.has_value(); });
  report_error_if_not(rep != cfg.csi_report_cfg_list.end(), "No CSI report with a codebook config was generated");
  return rep->codebook_cfg.value();
}

TEST(csi_helper_test, csi_report_config_with_eight_ports_uses_type2_codebook)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-2");

  const csi_meas_config cfg      = csi_helper::make_csi_meas_config(make_type2_csi_params(8, 4), {});
  const codebook_config codebook = get_codebook_config(cfg);

  ASSERT_TRUE(std::holds_alternative<codebook_config::type2>(codebook.codebook_type));
  const auto& type2 = std::get<codebook_config::type2>(codebook.codebook_type);

  EXPECT_EQ(type2.nof_beams, 4);
  EXPECT_EQ(type2.phase_alphabet_size, pmi_codebook_typeII_phase_size::psk8);
  EXPECT_FALSE(type2.subband_amplitude);

  ASSERT_TRUE(std::holds_alternative<codebook_config::type2::typeii>(type2.sub_type));
  const auto& typeii = std::get<codebook_config::type2::typeii>(type2.sub_type);

  EXPECT_EQ(typeii.n1_n2_codebook_subset_restriction_type,
            codebook_config::type2::typeii::n1_n2_codebook_subset_restriction_type_t::four_one);
  // The eight port restriction bitmap is 32 bits wide and enables every beam combination.
  EXPECT_EQ(typeii.n1_n2_codebook_subset_restriction_value.size(), 32);
  EXPECT_EQ(typeii.n1_n2_codebook_subset_restriction_value.count(), 32);

  // As per TS38.214 Section 5.2.2.2.3, the Type-II codebook supports at most two layers.
  EXPECT_EQ(typeii.typeii_ri_restriction.size(), 2);
  EXPECT_EQ(typeii.typeii_ri_restriction.count(), 2);
}

TEST(csi_helper_test, csi_report_config_with_four_ports_uses_two_one_type2_codebook)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-2");

  const csi_meas_config cfg      = csi_helper::make_csi_meas_config(make_type2_csi_params(4, 2), {});
  const codebook_config codebook = get_codebook_config(cfg);

  ASSERT_TRUE(std::holds_alternative<codebook_config::type2>(codebook.codebook_type));
  const auto& typeii =
      std::get<codebook_config::type2::typeii>(std::get<codebook_config::type2>(codebook.codebook_type).sub_type);

  EXPECT_EQ(typeii.n1_n2_codebook_subset_restriction_type,
            codebook_config::type2::typeii::n1_n2_codebook_subset_restriction_type_t::two_one);
  // The four port restriction bitmap is 16 bits wide.
  EXPECT_EQ(typeii.n1_n2_codebook_subset_restriction_value.size(), 16);
  EXPECT_EQ(typeii.n1_n2_codebook_subset_restriction_value.count(), 16);
}

TEST(csi_helper_test, generated_type2_codebook_derives_a_valid_report_configuration)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-2");

  const csi_meas_config cfg = csi_helper::make_csi_meas_config(make_type2_csi_params(8, 4), {});

  // The generated configuration must be usable by the CSI report unpacking.
  const csi_report_configuration report_cfg = create_csi_report_configuration(cfg);

  ASSERT_TRUE(std::holds_alternative<pmi_codebook_typeII>(report_cfg.pmi_codebook));
  EXPECT_EQ(std::get<pmi_codebook_typeII>(report_cfg.pmi_codebook).nof_beams, 4);
  EXPECT_EQ(get_precoding_codebook_antenna_ports(report_cfg.pmi_codebook), 8);
  EXPECT_TRUE(is_valid(report_cfg));
}

TEST(csi_helper_test, type1_codebook_is_generated_when_type2_is_disabled)
{
  OCUDU_TEST_REQUIREMENTS("MVP-FUNC-MIMO-16-2");

  const csi_meas_config cfg      = csi_helper::make_csi_meas_config(make_csi_params_for_ports(8), {});
  const codebook_config codebook = get_codebook_config(cfg);

  ASSERT_TRUE(std::holds_alternative<codebook_config::type1>(codebook.codebook_type));
}

TEST(cell_csi_config_test, cell_without_csi_has_no_csi_config)
{
  cell_config_builder_params builder;
  builder.csi_rs_enabled = false;

  ASSERT_FALSE(make_cell_bwp_res_config(config_helpers::make_default_ran_cell_config(builder)).dl.csi.has_value());
}

struct cell_csi_test_params {
  unsigned        nof_ports;
  bool            aperiodic;
  pdsch_mcs_table mcs_table;
  bool            type2 = false;
};

class cell_csi_config_test : public ::testing::TestWithParam<cell_csi_test_params>
{};

TEST_P(cell_csi_config_test, cell_csi_config_matches_the_ue_csi_meas_config)
{
  cell_config_builder_params builder;
  builder.dl_carrier.nof_ant        = GetParam().nof_ports;
  ran_cell_config cell_cfg          = config_helpers::make_default_ran_cell_config(builder);
  cell_cfg.init_bwp.pdsch.mcs_table = GetParam().mcs_table;
  if (GetParam().aperiodic) {
    cell_cfg.init_bwp.csi->csi_report_slot_offset.reset();
    cell_cfg.init_bwp.csi->enable_aperiodic_report = true;
  }
  if (GetParam().type2) {
    cell_cfg.init_bwp.csi->type2_codebook.emplace();
  }

  const cell_bwp_csi_config csi = make_cell_bwp_res_config(cell_cfg).dl.csi.value();
  const csi_meas_config     ue_csi =
      config_helpers::make_default_ue_cell_config(cell_cfg).serv_cell_cfg.csi_meas_cfg.value();

  EXPECT_EQ(csi.cqi_table, ue_csi.csi_report_cfg_list.front().cqi_table);
  EXPECT_EQ(csi_helper::get_csi_report_trigger_size(*cell_cfg.init_bwp.csi), ue_csi.report_trigger_size.value_or(0));
  EXPECT_EQ(csi.report_type, is_pusch_configured(ue_csi) ? csi_report_type::aperiodic : csi_report_type::periodic);
  EXPECT_EQ(csi.pucch_report() != nullptr, csi.report_type == csi_report_type::periodic);

  // The default UE configuration uses the codebook that the cell offers.
  const csi_report_configuration ue_report = create_csi_report_configuration(ue_csi);
  ASSERT_EQ(csi.type2_report.has_value(), GetParam().type2);
  const csi_report_configuration& cell_report = GetParam().type2 ? *csi.type2_report : csi.type1_report;
  EXPECT_EQ(cell_report.pmi_codebook.index(), ue_report.pmi_codebook.index());
  EXPECT_EQ(get_csi_report_pusch_size(cell_report).part1_size, get_csi_report_pusch_size(ue_report).part1_size);
  EXPECT_EQ(get_csi_report_pusch_size(cell_report).part2_max_size, get_csi_report_pusch_size(ue_report).part2_max_size);
  EXPECT_EQ(cell_report.ri_restriction, ue_report.ri_restriction);
  EXPECT_EQ(cell_report.nof_csi_rs_resources, ue_report.nof_csi_rs_resources);
  EXPECT_EQ(cell_report.quantities, ue_report.quantities);
  if (GetParam().type2) {
    EXPECT_TRUE(std::holds_alternative<pmi_codebook_typeI_single_panel>(csi.type1_report.pmi_codebook));
  }
}

INSTANTIATE_TEST_SUITE_P(cell_csi_config_test,
                         cell_csi_config_test,
                         ::testing::Values(cell_csi_test_params{1, false, pdsch_mcs_table::qam64},
                                           cell_csi_test_params{2, false, pdsch_mcs_table::qam256},
                                           cell_csi_test_params{4, false, pdsch_mcs_table::qam64},
                                           cell_csi_test_params{4, true, pdsch_mcs_table::qam256},
                                           cell_csi_test_params{4, true, pdsch_mcs_table::qam64, true}));
