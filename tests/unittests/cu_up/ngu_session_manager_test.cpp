// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ngu_session_manager_test.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ocuup;

/// Returns the bind address of the given gateway, or an empty string if there is none.
static std::string bind_address_of(const gtpu_tnl_pdu_session* gw)
{
  std::string ip_addr;
  if (gw == nullptr or not gw->get_bind_address(ip_addr)) {
    return {};
  }
  return ip_addr;
}

static const s_nssai_t slice_a{slice_service_type{1}, slice_differentiator::create(1).value()};
static const s_nssai_t slice_b{slice_service_type{1}, slice_differentiator::create(2).value()};
static const s_nssai_t slice_c{slice_service_type{2}};

TEST_F(ngu_session_manager_test, mngr_creation)
{
  ASSERT_NE(ngu_session_mngr, nullptr);
}

TEST_F(ngu_session_manager_test, rr_session_selection)
{
  EXPECT_EQ(bind_address_of(ngu_session_mngr->get_next_ngu_gateway(slice_a)), "127.0.0.1");
  EXPECT_EQ(bind_address_of(ngu_session_mngr->get_next_ngu_gateway(slice_a)), "127.0.0.2");
  EXPECT_EQ(bind_address_of(ngu_session_mngr->get_next_ngu_gateway(slice_a)), "127.0.0.1");
}

TEST_F(ngu_session_manager_test, default_gateways_serve_every_slice)
{
  EXPECT_EQ(bind_address_of(ngu_session_mngr->get_next_ngu_gateway(slice_a)), "127.0.0.1");
  EXPECT_EQ(bind_address_of(ngu_session_mngr->get_next_ngu_gateway(slice_b)), "127.0.0.2");
  EXPECT_EQ(bind_address_of(ngu_session_mngr->get_next_ngu_gateway(slice_c)), "127.0.0.1");
}

TEST_F(ngu_session_manager_test, dedicated_gateways_serve_only_their_slice)
{
  // 127.0.0.1 is a default gateway, 127.0.0.2 is dedicated to slice A.
  const std::vector<std::optional<s_nssai_t>> gw_snssais = {std::nullopt, slice_a};
  ngu_session_manager_impl                    mngr(ngu_gws, gw_snssais);

  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_a)), "127.0.0.2");
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_a)), "127.0.0.2");
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_b)), "127.0.0.1");
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_c)), "127.0.0.1");
}

TEST_F(ngu_session_manager_test, gateways_of_one_slice_take_turns)
{
  add_gateways(2);
  // 127.0.0.1 and 127.0.0.3 serve slice A, 127.0.0.2 and 127.0.0.4 serve slice B.
  const std::vector<std::optional<s_nssai_t>> gw_snssais = {slice_a, slice_b, slice_a, slice_b};
  ngu_session_manager_impl                    mngr(ngu_gws, gw_snssais);

  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_a)), "127.0.0.1");
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_b)), "127.0.0.2");
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_a)), "127.0.0.3");
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_a)), "127.0.0.1");
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_b)), "127.0.0.4");
}

TEST_F(ngu_session_manager_test, slice_without_dedicated_or_default_gateway_is_not_served)
{
  const std::vector<std::optional<s_nssai_t>> gw_snssais = {slice_a, slice_b};
  ngu_session_manager_impl                    mngr(ngu_gws, gw_snssais);

  EXPECT_EQ(mngr.get_next_ngu_gateway(slice_c), nullptr);
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_a)), "127.0.0.1");
}

TEST_F(ngu_session_manager_test, sd_is_part_of_the_slice_match)
{
  // Slice C has no SD, so SST 2 with an SD is a different slice.
  const std::vector<std::optional<s_nssai_t>> gw_snssais = {slice_c, slice_a};
  ngu_session_manager_impl                    mngr(ngu_gws, gw_snssais);

  const s_nssai_t sst2_with_sd{slice_service_type{2}, slice_differentiator::create(5).value()};
  EXPECT_EQ(mngr.get_next_ngu_gateway(sst2_with_sd), nullptr);
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_c)), "127.0.0.1");
}

TEST_F(ngu_session_manager_test, gateways_past_the_snssai_list_are_default_gateways)
{
  // Only the first gateway has an entry, so 127.0.0.2 is a default gateway.
  const std::vector<std::optional<s_nssai_t>> gw_snssais = {slice_a};
  ngu_session_manager_impl                    mngr(ngu_gws, gw_snssais);

  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_a)), "127.0.0.1");
  EXPECT_EQ(bind_address_of(mngr.get_next_ngu_gateway(slice_b)), "127.0.0.2");
}

int main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
