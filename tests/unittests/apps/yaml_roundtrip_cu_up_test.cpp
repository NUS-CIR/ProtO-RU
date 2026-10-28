// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "apps/cu_up/cu_up_appconfig.h"
#include "apps/cu_up/cu_up_appconfig_cli11_schema.h"
#include "apps/cu_up/cu_up_appconfig_yaml_writer.h"
#include "apps/units/o_cu_up/o_cu_up_application_unit.h"
#include "yaml_roundtrip_test_helpers.h"
#include "ocudu/support/config_parsers.h"
#include "CLI/CLI11.hpp"

#ifndef CONFIGS_DIR
#error "CONFIGS_DIR must be defined"
#endif

using namespace ocudu;
using namespace ocudu::yaml_roundtrip_test;

namespace {

YAML::Node load_and_emit(const std::string& yaml_text)
{
  temp_yaml_file tmp(yaml_text);

  CLI::App app("cu_up yaml-roundtrip-test");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::error);
  std::string cfg_path;
  app.set_config("-c,", cfg_path, "Read config from file", false);

  cu_up_appconfig cu_up_cfg;
  configure_cli11_with_cu_appconfig_schema(app, cu_up_cfg);

  auto o_cu_up = create_o_cu_up_application_unit("cu-up");
  o_cu_up->on_parsing_configuration_registration(app);

  app.callback([&]() { o_cu_up->on_configuration_parameters_autoderivation(app); });

  std::vector<const char*> argv = {"cu_up", "-c", tmp.path().c_str()};
  app.parse(static_cast<int>(argv.size()), argv.data());

  YAML::Node node;
  fill_cu_up_appconfig_in_yaml_schema(node, cu_up_cfg);
  o_cu_up->dump_config(node);
  return node;
}

YAML::Node emit_defaults()
{
  cu_up_appconfig cu_up_cfg;
  auto            o_cu_up = create_o_cu_up_application_unit("cu-up");
  YAML::Node      node;
  fill_cu_up_appconfig_in_yaml_schema(node, cu_up_cfg);
  o_cu_up->dump_config(node);
  return node;
}

const std::string CONFIGS = CONFIGS_DIR;

class cu_up_example_config_test : public ::testing::TestWithParam<std::string>
{};

TEST_P(cu_up_example_config_test, roundtrip)
{
  const std::string& name = GetParam();
  assert_roundtrip(read_file(CONFIGS + "/" + name), &load_and_emit, name);
}

INSTANTIATE_TEST_SUITE_P(,
                         cu_up_example_config_test,
                         ::testing::Values("cu_up.yml", "cu_up_f1u_multiple_sockets.yml", "ngu_multiple_sockets.yml"));

TEST(cu_up_default_config_test, roundtrip)
{
  YAML::Node a = emit_defaults();
  assert_roundtrip(YAML::Dump(a), &load_and_emit, "cu_up defaults");
}

} // namespace
