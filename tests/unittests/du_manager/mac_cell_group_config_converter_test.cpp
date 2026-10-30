// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "lib/du/du_high/du_manager/converters/asn1_rrc_config_helpers.h"
#include "ocudu/adt/byte_buffer.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/rrc_nr/cell_group_config.h"
#include "ocudu/mac/config/mac_cell_group_config_factory.h"
#include <gtest/gtest.h>

using namespace ocudu;

static odu::du_ue_resource_config make_initial_du_ue_resource_config()
{
  odu::du_ue_resource_config dest_cfg{};
  dest_cfg.cell_group.mcg_cfg = config_helpers::make_initial_mac_cell_group_config();
  return dest_cfg;
}

TEST(mac_cell_group_config_converter_test, test_default_initial_sr_cfg_conversion)
{
  auto                           dest_cfg = make_initial_du_ue_resource_config();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.mac_cell_group_cfg_present);

  const auto& rrc_mcg_cfg  = rrc_cell_grp_cfg.mac_cell_group_cfg;
  const auto& dest_mcg_cfg = dest_cfg.cell_group.mcg_cfg;

  if (not dest_mcg_cfg.scheduling_request_config.empty()) {
    ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_present);

    ASSERT_EQ(rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list.size(),
              dest_mcg_cfg.scheduling_request_config.size());
    ASSERT_EQ(rrc_mcg_cfg.sched_request_cfg.sched_request_to_release_list.size(), 0);
  }
}

TEST(mac_cell_group_config_converter_test, test_custom_sr_cfg_conversion)
{
  const auto&                src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  // Add new configuration to be setup.
  auto& dest_mcg_cfg = dest_cfg.cell_group.mcg_cfg;
  dest_mcg_cfg.scheduling_request_config.push_back(scheduling_request_to_addmod{
      .sr_id = static_cast<scheduling_request_id>(1), .prohibit_timer = sr_prohib_timer::ms1, .max_tx = sr_max_tx::n8});
  dest_mcg_cfg.scheduling_request_config.push_back(
      scheduling_request_to_addmod{.sr_id          = static_cast<scheduling_request_id>(2),
                                   .prohibit_timer = sr_prohib_timer::ms128,
                                   .max_tx         = sr_max_tx::n16});
  dest_mcg_cfg.scheduling_request_config.erase(dest_mcg_cfg.scheduling_request_config.begin());

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.mac_cell_group_cfg_present);

  auto rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;

  if (not dest_mcg_cfg.scheduling_request_config.empty()) {
    ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_present);

    ASSERT_EQ(rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list.size(), 2);
    ASSERT_EQ(rrc_mcg_cfg.sched_request_cfg.sched_request_to_release_list.size(), 1);
  }
}

TEST(mac_cell_group_config_converter_test, test_legacy_sr_prohibit_timer_is_not_signalled_in_the_r17_extension)
{
  auto dest_cfg                                                                = make_initial_du_ue_resource_config();
  dest_cfg.cell_group.mcg_cfg.scheduling_request_config.front().prohibit_timer = sr_prohib_timer::ms128;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  const auto& rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_present);
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list[0].sr_prohibit_timer_present);
  ASSERT_EQ(rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list[0].sr_prohibit_timer,
            asn1::rrc_nr::sched_request_to_add_mod_s::sr_prohibit_timer_opts::ms128);
  ASSERT_FALSE(rrc_mcg_cfg.sched_request_cfg_v1700.is_present())
      << "a value within the legacy range needs no extension";
}

TEST(mac_cell_group_config_converter_test, test_extended_sr_prohibit_timer_conversion)
{
  auto dest_cfg = make_initial_du_ue_resource_config();
  // Only representable in sr-ProhibitTimer-v1700.
  dest_cfg.cell_group.mcg_cfg.scheduling_request_config.front().prohibit_timer = sr_prohib_timer::ms320;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  const auto& rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_present);

  // The legacy field is capped at ms128.
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list[0].sr_prohibit_timer_present);
  ASSERT_EQ(rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list[0].sr_prohibit_timer,
            asn1::rrc_nr::sched_request_to_add_mod_s::sr_prohibit_timer_opts::ms128);
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_v1700.is_present());
  const auto& ext_list = rrc_mcg_cfg.sched_request_cfg_v1700->sched_request_to_add_mod_list_ext_v1700;
  // One entry per schedulingRequestToAddModList entry, in the same order.
  ASSERT_EQ(ext_list.size(), rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list.size());
  ASSERT_TRUE(ext_list[0].sr_prohibit_timer_v1700_present);
  ASSERT_EQ(ext_list[0].sr_prohibit_timer_v1700,
            asn1::rrc_nr::sched_request_to_add_mod_ext_v1700_s::sr_prohibit_timer_v1700_opts::ms320);
}

TEST(mac_cell_group_config_converter_test, test_extended_sr_prohibit_timer_fills_every_extension_list_entry)
{
  auto  dest_cfg     = make_initial_du_ue_resource_config();
  auto& dest_mcg_cfg = dest_cfg.cell_group.mcg_cfg;
  // Only the second SR needs the extension, but the list still needs an entry for the first.
  dest_mcg_cfg.scheduling_request_config.front().prohibit_timer = sr_prohib_timer::ms64;
  dest_mcg_cfg.scheduling_request_config.push_back(
      scheduling_request_to_addmod{.sr_id          = static_cast<scheduling_request_id>(1),
                                   .prohibit_timer = sr_prohib_timer::ms1082,
                                   .max_tx         = sr_max_tx::n8});

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  const auto& rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_v1700.is_present());
  const auto& ext_list = rrc_mcg_cfg.sched_request_cfg_v1700->sched_request_to_add_mod_list_ext_v1700;
  ASSERT_EQ(ext_list.size(), 2);

  const auto& add_mod_list = rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list;
  for (unsigned i = 0; i != ext_list.size(); ++i) {
    const bool needs_ext = add_mod_list[i].sched_request_id == 1;
    ASSERT_EQ(ext_list[i].sr_prohibit_timer_v1700_present, needs_ext);
    if (needs_ext) {
      ASSERT_EQ(ext_list[i].sr_prohibit_timer_v1700,
                asn1::rrc_nr::sched_request_to_add_mod_ext_v1700_s::sr_prohibit_timer_v1700_opts::ms1082);
    }
  }
}

TEST(mac_cell_group_config_converter_test, test_extended_sr_prohibit_timer_survives_encoding)
{
  auto dest_cfg                                                                = make_initial_du_ue_resource_config();
  dest_cfg.cell_group.mcg_cfg.scheduling_request_config.front().prohibit_timer = sr_prohib_timer::ms320;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  byte_buffer   buf;
  asn1::bit_ref bref{buf};
  ASSERT_EQ(rrc_cell_grp_cfg.pack(bref), asn1::OCUDUASN_SUCCESS);

  asn1::rrc_nr::cell_group_cfg_s decoded;
  asn1::cbit_ref                 cbref{buf};
  ASSERT_EQ(decoded.unpack(cbref), asn1::OCUDUASN_SUCCESS);

  const auto& rrc_mcg_cfg = decoded.mac_cell_group_cfg;
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_v1700.is_present());
  const auto& ext_list = rrc_mcg_cfg.sched_request_cfg_v1700->sched_request_to_add_mod_list_ext_v1700;
  ASSERT_EQ(ext_list.size(), 1);
  ASSERT_TRUE(ext_list[0].sr_prohibit_timer_v1700_present);
  ASSERT_EQ(ext_list[0].sr_prohibit_timer_v1700,
            asn1::rrc_nr::sched_request_to_add_mod_ext_v1700_s::sr_prohibit_timer_v1700_opts::ms320);
}

TEST(mac_cell_group_config_converter_test, test_unchanged_extended_sr_prohibit_timer_is_not_resignalled)
{
  auto src_cfg                                                                = make_initial_du_ue_resource_config();
  src_cfg.cell_group.mcg_cfg.scheduling_request_config.front().prohibit_timer = sr_prohib_timer::ms320;
  const auto dest_cfg                                                         = src_cfg;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  // The UE keeps the stored value (Need M).
  const auto& rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;
  ASSERT_FALSE(rrc_mcg_cfg.sched_request_cfg_present);
  ASSERT_FALSE(rrc_mcg_cfg.sched_request_cfg_v1700.is_present());
}

TEST(mac_cell_group_config_converter_test, test_extended_sr_prohibit_timer_is_released_when_reconfigured_to_legacy)
{
  auto src_cfg                                                                 = make_initial_du_ue_resource_config();
  src_cfg.cell_group.mcg_cfg.scheduling_request_config.front().prohibit_timer  = sr_prohib_timer::ms320;
  auto dest_cfg                                                                = src_cfg;
  dest_cfg.cell_group.mcg_cfg.scheduling_request_config.front().prohibit_timer = sr_prohib_timer::ms64;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  const auto& rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_present);
  ASSERT_EQ(rrc_mcg_cfg.sched_request_cfg.sched_request_to_add_mod_list[0].sr_prohibit_timer,
            asn1::rrc_nr::sched_request_to_add_mod_s::sr_prohibit_timer_opts::ms64);

  // Without the extension, the UE would keep the stored ms320 (Need M), which overrides the legacy field.
  ASSERT_TRUE(rrc_mcg_cfg.sched_request_cfg_v1700.is_present());
  const auto& ext_list = rrc_mcg_cfg.sched_request_cfg_v1700->sched_request_to_add_mod_list_ext_v1700;
  ASSERT_EQ(ext_list.size(), 1);
  ASSERT_FALSE(ext_list[0].sr_prohibit_timer_v1700_present) << "an absent field releases the stored value (Need R)";
}

TEST(mac_cell_group_config_converter_test, test_custom_bsr_cfg_conversion)
{
  const auto                 src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  // Add new configuration to be setup. Assume BSR Config is already set.
  auto& dest_mcg_cfg                             = dest_cfg.cell_group.mcg_cfg;
  dest_mcg_cfg.bsr_cfg.value().periodic_timer    = ocudu::periodic_bsr_timer::sf2560;
  dest_mcg_cfg.bsr_cfg.value().lc_sr_delay_timer = logical_channel_sr_delay_timer::sf1024;
  dest_mcg_cfg.bsr_cfg.value().retx_timer        = ocudu::retx_bsr_timer::sf5120;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.mac_cell_group_cfg_present);

  auto rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;

  ASSERT_TRUE(rrc_mcg_cfg.bsr_cfg_present);
  ASSERT_EQ(rrc_mcg_cfg.bsr_cfg.retx_bsr_timer, asn1::rrc_nr::bsr_cfg_s::retx_bsr_timer_opts::sf5120);
  ASSERT_EQ(rrc_mcg_cfg.bsr_cfg.periodic_bsr_timer, asn1::rrc_nr::bsr_cfg_s::periodic_bsr_timer_opts::sf2560);
  ASSERT_TRUE(rrc_mcg_cfg.bsr_cfg.lc_ch_sr_delay_timer_present);
  ASSERT_EQ(rrc_mcg_cfg.bsr_cfg.lc_ch_sr_delay_timer, asn1::rrc_nr::bsr_cfg_s::lc_ch_sr_delay_timer_opts::sf1024);
}

TEST(mac_cell_group_config_converter_test, test_default_initial_tag_cfg_conversion)
{
  auto                           dest_cfg = make_initial_du_ue_resource_config();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.mac_cell_group_cfg_present);

  const auto& rrc_mcg_cfg  = rrc_cell_grp_cfg.mac_cell_group_cfg;
  const auto& dest_mcg_cfg = dest_cfg.cell_group.mcg_cfg;

  if (not dest_mcg_cfg.tag_config.empty()) {
    ASSERT_TRUE(rrc_mcg_cfg.tag_cfg_present);

    ASSERT_EQ(rrc_mcg_cfg.tag_cfg.tag_to_add_mod_list.size(), dest_mcg_cfg.tag_config.size());
    ASSERT_EQ(rrc_mcg_cfg.tag_cfg.tag_to_release_list.size(), 0);
  }
}

TEST(mac_cell_group_config_converter_test, test_custom_tag_cfg_conversion)
{
  const auto                 src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  // Add new configuration to be setup.
  auto& dest_mcg_cfg = dest_cfg.cell_group.mcg_cfg;
  dest_mcg_cfg.tag_config.push_back(
      time_alignment_group{.tag_id = time_alignment_group::id_t{1}, .ta_timer = time_alignment_timer::ms2560});
  dest_mcg_cfg.tag_config.push_back(
      time_alignment_group{.tag_id = time_alignment_group::id_t{2}, .ta_timer = time_alignment_timer::ms1280});
  dest_mcg_cfg.tag_config.erase(dest_mcg_cfg.tag_config.begin());

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.mac_cell_group_cfg_present);

  auto rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;

  if (not dest_mcg_cfg.tag_config.empty()) {
    ASSERT_TRUE(rrc_mcg_cfg.tag_cfg_present);

    ASSERT_EQ(rrc_mcg_cfg.tag_cfg.tag_to_add_mod_list.size(), 2);
    ASSERT_EQ(rrc_mcg_cfg.tag_cfg.tag_to_release_list.size(), 1);
  }
}

TEST(mac_cell_group_config_converter_test, test_default_initial_phr_cfg_conversion)
{
  auto                           dest_cfg = make_initial_du_ue_resource_config();
  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, {}, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.mac_cell_group_cfg_present);

  const auto& rrc_mcg_cfg  = rrc_cell_grp_cfg.mac_cell_group_cfg;
  const auto& dest_mcg_cfg = dest_cfg.cell_group.mcg_cfg;

  if (dest_mcg_cfg.phr_cfg.has_value()) {
    ASSERT_TRUE(rrc_mcg_cfg.phr_cfg_present);
    // Since its initial setup and no source mac cell group config was provided PHR Config must be of setup type.
    ASSERT_TRUE(rrc_mcg_cfg.phr_cfg.is_setup());
  }
}

TEST(mac_cell_group_config_converter_test, test_custom_phr_cfg_conversion)
{
  const auto                 src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  // Add new configuration to be setup.
  auto& dest_mcg_cfg                                  = dest_cfg.cell_group.mcg_cfg;
  dest_mcg_cfg.phr_cfg.value().periodic_timer         = phr_periodic_timer::sf100;
  dest_mcg_cfg.phr_cfg.value().prohibit_timer         = phr_prohibit_timer::sf20;
  dest_mcg_cfg.phr_cfg.value().power_factor_change    = phr_tx_power_factor_change::db3;
  dest_mcg_cfg.phr_cfg.value().multiple_phr           = true;
  dest_mcg_cfg.phr_cfg.value().dummy                  = true;
  dest_mcg_cfg.phr_cfg.value().phr_type_to_other_cell = true;
  dest_mcg_cfg.phr_cfg.value().phr_mode               = phr_mode_other_cg::virtual_;

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.mac_cell_group_cfg_present);

  auto rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;

  if (dest_mcg_cfg.phr_cfg.has_value()) {
    ASSERT_TRUE(rrc_mcg_cfg.phr_cfg_present);
    ASSERT_TRUE(rrc_mcg_cfg.phr_cfg.is_setup());

    ASSERT_EQ(rrc_mcg_cfg.phr_cfg.setup().phr_periodic_timer, asn1::rrc_nr::phr_cfg_s::phr_periodic_timer_opts::sf100);
    ASSERT_EQ(rrc_mcg_cfg.phr_cfg.setup().phr_prohibit_timer, asn1::rrc_nr::phr_cfg_s::phr_prohibit_timer_opts::sf20);
    ASSERT_EQ(rrc_mcg_cfg.phr_cfg.setup().phr_tx_pwr_factor_change,
              asn1::rrc_nr::phr_cfg_s::phr_tx_pwr_factor_change_opts::db3);
    ASSERT_TRUE(rrc_mcg_cfg.phr_cfg.setup().multiple_phr);
    ASSERT_TRUE(rrc_mcg_cfg.phr_cfg.setup().dummy);
    ASSERT_TRUE(rrc_mcg_cfg.phr_cfg.setup().phr_type2_other_cell);
    ASSERT_EQ(rrc_mcg_cfg.phr_cfg.setup().phr_mode_other_cg,
              asn1::rrc_nr::phr_cfg_s::phr_mode_other_cg_opts::virtual_value);
  }
}

TEST(serving_cell_config_converter_test, test_phr_cfg_release_conversion)
{
  const auto&                src_cfg = make_initial_du_ue_resource_config();
  odu::du_ue_resource_config dest_cfg{src_cfg};
  // Remove PHR configuration to be setup.
  auto& dest_mcg_cfg = dest_cfg.cell_group.mcg_cfg;
  dest_mcg_cfg.phr_cfg.reset();

  asn1::rrc_nr::cell_group_cfg_s rrc_cell_grp_cfg;
  odu::calculate_cell_group_config_diff(rrc_cell_grp_cfg, src_cfg, dest_cfg);

  ASSERT_TRUE(rrc_cell_grp_cfg.mac_cell_group_cfg_present);

  auto rrc_mcg_cfg = rrc_cell_grp_cfg.mac_cell_group_cfg;

  ASSERT_TRUE(rrc_mcg_cfg.phr_cfg_present);
  // PHR Config is released due to absence in dest mac cell group config.
  ASSERT_EQ(rrc_mcg_cfg.phr_cfg.type(), asn1::setup_release_opts::release);
}
