// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_appconfig.h"
#include "../../../../apps/examples/ofh/ru_emulator_cli11_schema.h"
#include "ocudu/support/config_parsers.h"
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

using namespace ocudu;

TEST(ru_emulator_example_configs_test, parses_all_protoru_example_configs)
{
  const std::vector<const char*> config_paths = {"proto-ru/conf-files/protoru-B210-FDD-n3-10MHz-1x1-15kHz.yml",
                                                 "proto-ru/conf-files/protoru-B210-FDD-n3-10MHz-2x2-15kHz.yml",
                                                 "proto-ru/conf-files/protoru-B210-TDD-n78-20MHz-2x2-30kHz.yml",
                                                 "proto-ru/conf-files/protoru-B210-TDD-n78-40MHz-1x1-30kHz.yml",
                                                 "proto-ru/conf-files/protoru-B210-TDD-n78-40MHz-1x1-30kHz-dpdk.yml"};

  for (const char* relative_path : config_paths) {
    const std::string path = std::string(OCUDU_SOURCE_DIR) + "/" + relative_path;
    std::ifstream     input(path);
    ASSERT_TRUE(input.is_open()) << path;

    CLI::App app("RU emulator example configuration test");
    app.config_formatter(create_yaml_config_parser());
    app.allow_config_extras(CLI::config_extras_mode::error);

    ru_emulator_appconfig config;
    configure_cli11_with_ru_emulator_appconfig_schema(app, config);

    ASSERT_NO_THROW(app.parse_from_stream(input)) << path;
    ASSERT_EQ(config.ru_cfg.size(), 1U) << path;
    ASSERT_TRUE(config.ru_cfg.front().sdr_config.has_value()) << path;
    EXPECT_EQ(config.ru_cfg.front().sdr_config->device_driver, "uhd") << path;
  }
}
