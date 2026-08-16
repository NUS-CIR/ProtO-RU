// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_cli11_schema.h"
#include "ocudu/support/config_parsers.h"
#include <array>
#include <gtest/gtest.h>
#include <sstream>
#include <string_view>

using namespace ocudu;

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
