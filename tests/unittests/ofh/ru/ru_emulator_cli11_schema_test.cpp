// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_appconfig.h"
#include "../../../../apps/examples/ofh/ru_emulator_cli11_schema.h"
#include "ocudu/support/config_parsers.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace ocudu;

namespace {

void parse_config_with_vlan_id(unsigned vlan_id)
{
  CLI::App app("RU emulator configuration test");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::error);

  ru_emulator_appconfig config;
  configure_cli11_with_ru_emulator_appconfig_schema(app, config);

  std::istringstream yaml("ru_emu:\n  cells:\n    - vlan_tag: " + std::to_string(vlan_id) + "\n");
  app.parse_from_stream(yaml);
}

void parse_config_with_eaxc_list(const std::string& option, const std::string& values)
{
  CLI::App app("RU emulator configuration test");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::error);

  ru_emulator_appconfig config;
  configure_cli11_with_ru_emulator_appconfig_schema(app, config);

  std::istringstream yaml("ru_emu:\n  cells:\n    - " + option + ": " + values + "\n");
  app.parse_from_stream(yaml);
}

} // namespace

TEST(ru_emulator_cli11_schema_test, accepts_usable_vlan_id_boundaries)
{
  ASSERT_NO_THROW(parse_config_with_vlan_id(1));
  ASSERT_NO_THROW(parse_config_with_vlan_id(4094));
}

TEST(ru_emulator_cli11_schema_test, rejects_reserved_vlan_ids)
{
  ASSERT_THROW(parse_config_with_vlan_id(0), CLI::ParseError);
  ASSERT_THROW(parse_config_with_vlan_id(4095), CLI::ParseError);
}

TEST(ru_emulator_cli11_schema_test, vlan_id_is_optional)
{
  CLI::App app("RU emulator configuration test");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::error);

  ru_emulator_appconfig config;
  configure_cli11_with_ru_emulator_appconfig_schema(app, config);

  // An omitted VLAN identifier leaves the frames untagged, for interfaces that insert the tag themselves.
  std::istringstream yaml("ru_emu:\n  cells:\n    - du_mac_addr: aa:bb:cc:dd:ee:ff\n");
  ASSERT_NO_THROW(app.parse_from_stream(yaml));
  ASSERT_EQ(config.ru_cfg.size(), 1);
  ASSERT_FALSE(config.ru_cfg.front().vlan_tag.has_value());
}

TEST(ru_emulator_cli11_schema_test, accepts_eaxc_identifier_boundaries)
{
  ASSERT_NO_THROW(parse_config_with_eaxc_list("ul_port_id", "[0, 31]"));
}

TEST(ru_emulator_cli11_schema_test, rejects_too_many_eaxc_identifiers)
{
  ASSERT_THROW(parse_config_with_eaxc_list("dl_port_id", "[0, 1, 2, 3, 4]"), CLI::ParseError);
}

TEST(ru_emulator_cli11_schema_test, rejects_out_of_range_eaxc_identifier)
{
  ASSERT_THROW(parse_config_with_eaxc_list("prach_port_id", "[32]"), CLI::ParseError);
}

TEST(ru_emulator_cli11_schema_test, rejects_duplicate_eaxc_identifiers)
{
  ASSERT_THROW(parse_config_with_eaxc_list("ul_port_id", "[1, 1]"), CLI::ParseError);
}
