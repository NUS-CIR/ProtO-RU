// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "apps/units/o_cu_up/cu_up/cu_up_unit_config.h"
#include "apps/units/o_cu_up/cu_up/cu_up_unit_config_cli11_schema.h"
#include "apps/units/o_cu_up/cu_up/cu_up_unit_config_translators.h"
#include "apps/units/o_cu_up/cu_up/cu_up_unit_config_validator.h"
#include "ocudu/support/config_parsers.h"
#include "CLI/CLI11.hpp"
#include <gtest/gtest.h>
#include <sstream>

using namespace ocudu;

/// Parses the given YAML text into a CU-UP unit configuration.
/// Throws CLI::ParseError if any of the registered ->check() validators reject the input.
static cu_up_unit_config parse_cu_up_config(const std::string& yaml_text)
{
  cu_up_unit_config cfg;

  CLI::App app("cu_up_unit_ngu_socket_config_test");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::capture);
  configure_cli11_with_cu_up_unit_config_schema(app, cfg);

  std::istringstream ss(yaml_text);
  app.parse_from_stream(ss);

  return cfg;
}

/// Returns a CU-UP unit configuration with the given NG-U sockets.
static cu_up_unit_config make_config_with_sockets(std::vector<cu_up_unit_ngu_socket_config> sockets)
{
  cu_up_unit_config cfg;
  cfg.plmn_list              = {"00101"};
  cfg.ngu_cfg.ngu_socket_cfg = std::move(sockets);
  return cfg;
}

/// Returns an NG-U socket bound to the given address and dedicated to the given S-NSSAI, if any.
static cu_up_unit_ngu_socket_config
make_socket(const std::string& bind_addr, std::optional<uint8_t> sst = {}, std::optional<uint32_t> sd = {})
{
  cu_up_unit_ngu_socket_config sock;
  sock.bind_addr = bind_addr;
  sock.sst       = sst;
  sock.sd        = sd;
  return sock;
}

TEST(cu_up_unit_ngu_socket_config_test, socket_slice_is_parsed)
{
  cu_up_unit_config cfg = parse_cu_up_config("cu_up:\n"
                                             "  ngu:\n"
                                             "    socket:\n"
                                             "      - bind_addr: 127.0.3.1\n"
                                             "      - bind_addr: 127.0.3.2\n"
                                             "        sst: 1\n"
                                             "        sd: 2\n"
                                             "      - bind_addr: 127.0.3.3\n"
                                             "        sst: 2\n");

  ASSERT_EQ(cfg.ngu_cfg.ngu_socket_cfg.size(), 3);
  EXPECT_FALSE(cfg.ngu_cfg.ngu_socket_cfg[0].sst.has_value());
  EXPECT_FALSE(cfg.ngu_cfg.ngu_socket_cfg[0].sd.has_value());
  EXPECT_EQ(cfg.ngu_cfg.ngu_socket_cfg[1].sst, 1);
  EXPECT_EQ(cfg.ngu_cfg.ngu_socket_cfg[1].sd, 2U);
  EXPECT_EQ(cfg.ngu_cfg.ngu_socket_cfg[2].sst, 2);
  EXPECT_FALSE(cfg.ngu_cfg.ngu_socket_cfg[2].sd.has_value());
}

TEST(cu_up_unit_ngu_socket_config_test, sd_out_of_range_is_rejected)
{
  // 0xffffff is the "no SD" value of TS 23.003, Section 28.4.2, so it cannot be configured explicitly.
  EXPECT_THROW(parse_cu_up_config("cu_up:\n"
                                  "  ngu:\n"
                                  "    socket:\n"
                                  "      - bind_addr: 127.0.3.1\n"
                                  "        sst: 1\n"
                                  "        sd: 16777215\n"),
               CLI::ParseError);
}

TEST(cu_up_unit_ngu_socket_config_test, socket_with_sd_and_no_sst_is_invalid)
{
  cu_up_unit_config cfg = make_config_with_sockets({make_socket("127.0.3.1", std::nullopt, 2)});

  EXPECT_FALSE(validate_cu_up_unit_config(cfg, false));
}

TEST(cu_up_unit_ngu_socket_config_test, sockets_with_and_without_slice_are_valid)
{
  cu_up_unit_config cfg =
      make_config_with_sockets({make_socket("127.0.3.1"), make_socket("127.0.3.2", 1), make_socket("127.0.3.3", 1, 2)});

  EXPECT_TRUE(validate_cu_up_unit_config(cfg, false));
}

TEST(cu_up_unit_ngu_socket_config_test, socket_slices_are_translated_in_socket_order)
{
  cu_up_unit_config cfg =
      make_config_with_sockets({make_socket("127.0.3.1", 1, 2), make_socket("127.0.3.2"), make_socket("127.0.3.3", 2)});

  ocuup::cu_up_config cu_up_cfg = generate_cu_up_config(cfg);

  const auto& gw_snssais = cu_up_cfg.ngu_cfg.gw_snssais;
  ASSERT_EQ(gw_snssais.size(), 3);
  EXPECT_EQ(gw_snssais[0], (s_nssai_t{slice_service_type{1}, slice_differentiator::create(2).value()}));
  EXPECT_FALSE(gw_snssais[1].has_value());
  EXPECT_EQ(gw_snssais[2], s_nssai_t{slice_service_type{2}});
  EXPECT_FALSE(gw_snssais[2]->sd.is_set());
}

TEST(cu_up_unit_ngu_socket_config_test, socket_slices_are_ignored_without_a_core)
{
  cu_up_unit_config cfg      = make_config_with_sockets({make_socket("127.0.3.1", 1), make_socket("127.0.3.2", 2)});
  cfg.ngu_cfg.no_core        = true;
  ocuup::cu_up_config cu_cfg = generate_cu_up_config(cfg);

  EXPECT_TRUE(cu_cfg.ngu_cfg.gw_snssais.empty());
}
