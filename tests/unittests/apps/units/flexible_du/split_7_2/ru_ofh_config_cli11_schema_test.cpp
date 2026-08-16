// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_cli11_schema.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_yaml_writer.h"
#include "ocudu/support/config_parsers.h"
#include "CLI/CLI11.hpp"
#include <array>
#include <gtest/gtest.h>
#include <sstream>
#include <string_view>

using namespace ocudu;

namespace {

/// Two cells; the \c beamforming blocks are inserted at the given places.
std::string make_ru_ofh_yaml(const std::string& base_bf, const std::string& cell0_bf, const std::string& cell1_bf)
{
  return "ru_ofh:\n" + base_bf +
         "  cells:\n"
         "    - network_interface: eth0\n" +
         cell0_bf + "    - network_interface: eth1\n" + cell1_bf;
}

ru_ofh_unit_config parse_ru_ofh_config(const std::string& yaml_body)
{
  ru_ofh_unit_parsed_config cfg;

  CLI::App app("ru_ofh_config_cli11_schema_test");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::capture);
  configure_cli11_with_ru_ofh_config_schema(app, cfg);

  std::istringstream ss(yaml_body);
  app.parse_from_stream(ss);

  return cfg.config;
}

const std::string base_bf_bfp9  = "  beamforming:\n    bfw_compr_method: bfp\n    bfw_compr_bitwidth: 9\n";
const std::string cell_bf_bfp12 = "      beamforming:\n        bfw_compr_method: bfp\n        bfw_compr_bitwidth: 12\n";

} // namespace

TEST(ru_ofh_config_cli11_schema_test, beamforming_is_disabled_by_default)
{
  ru_ofh_unit_config cfg = parse_ru_ofh_config(make_ru_ofh_yaml("", "", ""));

  ASSERT_EQ(cfg.cells.size(), 2);
  EXPECT_FALSE(cfg.cells[0].cell.dl_beamforming.has_value());
  EXPECT_FALSE(cfg.cells[1].cell.dl_beamforming.has_value());
}

TEST(ru_ofh_config_cli11_schema_test, base_cell_beamforming_applies_to_every_cell)
{
  ru_ofh_unit_config cfg = parse_ru_ofh_config(make_ru_ofh_yaml(base_bf_bfp9, "", ""));

  ASSERT_EQ(cfg.cells.size(), 2);
  for (const auto& cell : cfg.cells) {
    ASSERT_TRUE(cell.cell.dl_beamforming.has_value());
    EXPECT_EQ(cell.cell.dl_beamforming->compression_method, "bfp");
    EXPECT_EQ(cell.cell.dl_beamforming->compression_bitwidth, 9);
  }
}

TEST(ru_ofh_config_cli11_schema_test, cell_beamforming_overrides_base_cell)
{
  ru_ofh_unit_config cfg = parse_ru_ofh_config(make_ru_ofh_yaml(base_bf_bfp9, "", cell_bf_bfp12));

  ASSERT_EQ(cfg.cells.size(), 2);
  ASSERT_TRUE(cfg.cells[0].cell.dl_beamforming.has_value());
  EXPECT_EQ(cfg.cells[0].cell.dl_beamforming->compression_bitwidth, 9);
  ASSERT_TRUE(cfg.cells[1].cell.dl_beamforming.has_value());
  EXPECT_EQ(cfg.cells[1].cell.dl_beamforming->compression_bitwidth, 12);
}

TEST(ru_ofh_config_cli11_schema_test, beamforming_can_be_enabled_in_a_single_cell)
{
  ru_ofh_unit_config cfg = parse_ru_ofh_config(make_ru_ofh_yaml("", cell_bf_bfp12, ""));

  ASSERT_EQ(cfg.cells.size(), 2);
  ASSERT_TRUE(cfg.cells[0].cell.dl_beamforming.has_value());
  EXPECT_EQ(cfg.cells[0].cell.dl_beamforming->compression_method, "bfp");
  EXPECT_EQ(cfg.cells[0].cell.dl_beamforming->compression_bitwidth, 12);
  EXPECT_FALSE(cfg.cells[1].cell.dl_beamforming.has_value());
}

TEST(ru_ofh_config_cli11_schema_test, unsupported_beamforming_compression_method_is_rejected)
{
  EXPECT_THROW(parse_ru_ofh_config(make_ru_ofh_yaml(
                   "  beamforming:\n    bfw_compr_method: mu law\n    bfw_compr_bitwidth: 8\n", "", "")),
               CLI::ParseError);
}

TEST(ru_ofh_config_cli11_schema_test, beamforming_yaml_output_round_trips)
{
  ru_ofh_unit_config cfg = parse_ru_ofh_config(make_ru_ofh_yaml(base_bf_bfp9, "", ""));

  YAML::Node node;
  fill_ru_ofh_config_in_yaml_schema(node, cfg);
  ru_ofh_unit_config reparsed = parse_ru_ofh_config(YAML::Dump(node));

  ASSERT_EQ(reparsed.cells.size(), 2);
  for (const auto& cell : reparsed.cells) {
    ASSERT_TRUE(cell.cell.dl_beamforming.has_value());
    EXPECT_EQ(cell.cell.dl_beamforming->compression_method, "bfp");
    EXPECT_EQ(cell.cell.dl_beamforming->compression_bitwidth, 9);
  }
}

namespace {

void parse_timing_parameter(std::string_view parameter, unsigned value)
{
  CLI::App app("OFH timing range test");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::error);

  ru_ofh_unit_parsed_config config;
  configure_cli11_with_ru_ofh_config_schema(app, config);

  std::istringstream input("ru_ofh:\n  " + std::string(parameter) + ": " + std::to_string(value) + "\n");
  app.parse_from_stream(input);
}

constexpr std::array<std::string_view, 8> timing_parameters = {"t1a_max_cp_dl",
                                                               "t1a_min_cp_dl",
                                                               "t1a_max_cp_ul",
                                                               "t1a_min_cp_ul",
                                                               "t1a_max_up",
                                                               "t1a_min_up",
                                                               "ta4_max",
                                                               "ta4_min"};

} // namespace

TEST(ru_ofh_config_cli11_schema_test, accepts_extended_timing_upper_bound)
{
  for (std::string_view parameter : timing_parameters) {
    SCOPED_TRACE(parameter);
    EXPECT_NO_THROW(parse_timing_parameter(parameter, 5000));
  }
}

TEST(ru_ofh_config_cli11_schema_test, rejects_timing_value_above_upper_bound)
{
  for (std::string_view parameter : timing_parameters) {
    SCOPED_TRACE(parameter);
    EXPECT_THROW(parse_timing_parameter(parameter, 5001), CLI::ParseError);
  }
}
