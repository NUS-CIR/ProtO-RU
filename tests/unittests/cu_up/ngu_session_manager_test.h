// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "cu_up_test_helpers.h"
#include "lib/cu_up/ngu_session_manager_impl.h"
#include "ocudu/support/ocudu_assert.h"
#include <gtest/gtest.h>

namespace ocudu::ocuup {

/// Fixture base class for PDU session manager tests
class ngu_session_manager_test : public ::testing::Test
{
  void SetUp() override
  {
    ocudulog::fetch_basic_logger("TEST").set_level(ocudulog::basic_levels::debug);
    ocudulog::init();

    add_gateways(2);

    // All gateways are default gateways.
    ngu_session_mngr = std::make_unique<ngu_session_manager_impl>(ngu_gws);
  }

  void TearDown() override
  {
    ngu_gws.clear();
    ngu_session_mngr.reset();

    // flush logger after each test
    ocudulog::flush();
  }

protected:
  /// Appends gateways bound to 127.0.0.N, where N continues from the last gateway.
  void add_gateways(unsigned nof_gws)
  {
    ocudu_assert(ngu_gws.size() + nof_gws <= 255,
                 "Cannot have more than 255 gateways bound to 127.0.0.N. nof_gws={} new_gws={}",
                 ngu_gws.size(),
                 nof_gws);
    for (unsigned i = 0; i != nof_gws; ++i) {
      auto        ngu_gw = std::make_unique<dummy_gtpu_gateway>();
      std::string addr   = fmt::format("127.0.0.{}", 1 + ngu_gws.size());
      ngu_gw->set_bind_address(addr);
      ngu_gws.push_back(std::move(ngu_gw));
    }
  }

  std::unique_ptr<ngu_session_manager>               ngu_session_mngr;
  std::vector<std::unique_ptr<gtpu_tnl_pdu_session>> ngu_gws;
};

} // namespace ocudu::ocuup
