// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_appconfig.h"
#include "../../../../apps/examples/ofh/ru_emulator_appconfig_yaml_writer.h"
#include "../../../../apps/examples/ofh/ru_emulator_cli11_schema.h"
#include "ocudu/support/config_parsers.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace ocudu;

namespace {

/// Builds a configuration exercising the optional members the writer has to decide about.
ru_emulator_appconfig make_config()
{
  ru_emulator_appconfig config;
  config.log_cfg.filename = "stdout";
  config.timing_cpus      = {1};

  auto& cell             = config.ru_cfg.front();
  cell.network_interface = "ens1f0";
  cell.ru_mac_address    = "00:11:22:33:44:55";
  cell.du_mac_address    = "00:11:22:33:44:66";
  cell.vlan_tag          = 5;
  cell.bandwidth         = bs_channel_bandwidth::MHz40;
  cell.common_scs        = subcarrier_spacing::kHz30;
  cell.dl_arfcn          = 628032;
  cell.band              = nr_band::n78;
  cell.ru_dl_port_id     = {0};
  cell.ru_ul_port_id     = {0};
  cell.ru_prach_port_id  = {4};
  cell.prach_format      = ru_emulator_prach_format::SHORT_B4;
  cell.T2a_max_cp_dl     = std::chrono::microseconds{2435};
  cell.T2a_min_cp_dl     = std::chrono::microseconds{2235};
  cell.ofh_cpus          = {2, 3};

  return config;
}

/// Dumps the configuration and parses the result back through the application schema.
ru_emulator_appconfig round_trip(const ru_emulator_appconfig& config)
{
  YAML::Node node;
  fill_ru_emulator_appconfig_in_yaml_schema(node, config);

  CLI::App app("RU emulator configuration test");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::error);

  ru_emulator_appconfig parsed;
  configure_cli11_with_ru_emulator_appconfig_schema(app, parsed);

  std::istringstream yaml(YAML::Dump(node));
  app.parse_from_stream(yaml);

  return parsed;
}

} // namespace

TEST(ru_emulator_appconfig_yaml_writer_test, dumped_configuration_is_accepted_by_the_parser)
{
  const ru_emulator_appconfig config = make_config();

  ru_emulator_appconfig parsed;
  ASSERT_NO_THROW(parsed = round_trip(config));

  ASSERT_EQ(parsed.timing_cpus, config.timing_cpus);
  ASSERT_EQ(parsed.ru_cfg.size(), 1);

  const auto& in  = config.ru_cfg.front();
  const auto& out = parsed.ru_cfg.front();
  ASSERT_EQ(out.network_interface, in.network_interface);
  ASSERT_EQ(out.ru_mac_address, in.ru_mac_address);
  ASSERT_EQ(out.du_mac_address, in.du_mac_address);
  ASSERT_EQ(out.vlan_tag, in.vlan_tag);
  ASSERT_EQ(out.bandwidth, in.bandwidth);
  ASSERT_EQ(out.common_scs, in.common_scs);
  ASSERT_EQ(out.dl_arfcn, in.dl_arfcn);
  ASSERT_EQ(out.band, in.band);
  ASSERT_EQ(out.ru_dl_port_id, in.ru_dl_port_id);
  ASSERT_EQ(out.ru_prach_port_id, in.ru_prach_port_id);
  ASSERT_EQ(out.prach_format, in.prach_format);
  ASSERT_EQ(out.T2a_max_cp_dl, in.T2a_max_cp_dl);
  ASSERT_EQ(out.T2a_min_cp_dl, in.T2a_min_cp_dl);
  ASSERT_EQ(out.ofh_cpus, in.ofh_cpus);
}

TEST(ru_emulator_appconfig_yaml_writer_test, untagged_cell_omits_the_vlan_key)
{
  ru_emulator_appconfig config = make_config();
  config.ru_cfg.front().vlan_tag.reset();

  YAML::Node node;
  fill_ru_emulator_appconfig_in_yaml_schema(node, config);

  // An absent VLAN identifier must not be written as a null value: the parser would reject it.
  ASSERT_FALSE(node["ru_emu"]["cells"][0]["vlan_tag"]);
  ASSERT_FALSE(round_trip(config).ru_cfg.front().vlan_tag.has_value());
}

TEST(ru_emulator_appconfig_yaml_writer_test, sdr_section_follows_the_configuration)
{
  ru_emulator_appconfig config = make_config();

  // A loopback cell has no radio, so the section must be absent rather than emitted with defaults.
  YAML::Node loopback_node;
  fill_ru_emulator_appconfig_in_yaml_schema(loopback_node, config);
  ASSERT_FALSE(loopback_node["ru_emu"]["cells"][0]["sdr"]);
  ASSERT_FALSE(round_trip(config).ru_cfg.front().sdr_config.has_value());

  ru_emulator_sdr_appconfig sdr;
  sdr.device_driver = "uhd";
  sdr.srate_MHz     = 46.08;
  sdr.ru_cpus       = {4, 5};
  config.ru_cfg.front().sdr_config = sdr;

  const ru_emulator_appconfig parsed = round_trip(config);
  ASSERT_TRUE(parsed.ru_cfg.front().sdr_config.has_value());
  ASSERT_EQ(parsed.ru_cfg.front().sdr_config->device_driver, "uhd");
  ASSERT_EQ(parsed.ru_cfg.front().sdr_config->srate_MHz, 46.08);
  ASSERT_EQ(parsed.ru_cfg.front().sdr_config->ru_cpus, sdr.ru_cpus);
}

TEST(ru_emulator_appconfig_yaml_writer_test, dpdk_section_follows_the_configuration)
{
  ru_emulator_appconfig config = make_config();

  YAML::Node without_dpdk;
  fill_ru_emulator_appconfig_in_yaml_schema(without_dpdk, config);
  ASSERT_FALSE(without_dpdk["dpdk"]);

  config.dpdk_config = ru_emulator_dpdk_appconfig{.eal_args = "-l 0 -a 0000:31:01.0"};

  YAML::Node with_dpdk;
  fill_ru_emulator_appconfig_in_yaml_schema(with_dpdk, config);
  ASSERT_EQ(with_dpdk["dpdk"]["eal_args"].as<std::string>(), "-l 0 -a 0000:31:01.0");
}
