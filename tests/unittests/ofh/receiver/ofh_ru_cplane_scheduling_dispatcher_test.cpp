// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/receiver/ofh_ru_cplane_scheduling_dispatcher.h"
#include "ocudu/ocudulog/ocudulog.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

namespace {

class downlink_scheduling_handler_spy : public ru_downlink_scheduling_handler
{
public:
  unsigned count = 0;
  unsigned eaxc  = 0;
  void     handle_downlink_scheduling(unsigned eaxc_, const cplane_message_decoder_results&) override
  {
    ++count;
    eaxc = eaxc_;
  }
};

class uplink_scheduling_handler_spy : public ru_uplink_scheduling_handler
{
public:
  unsigned ul_count    = 0;
  unsigned prach_count = 0;
  void     handle_uplink_scheduling(unsigned, const cplane_message_decoder_results&) override { ++ul_count; }
  void     handle_prach_scheduling(unsigned, const cplane_message_decoder_results&) override { ++prach_count; }
};

cplane_message_decoder_results make_results(uint8_t section_type, data_direction direction)
{
  cplane_message_decoder_results results;
  results.section_type        = section_type;
  results.radio_hdr.direction = direction;
  return results;
}

/// Builds a dispatcher configured with DL eAxC 7, UL eAxC 7 and PRACH eAxC 4.
ru_cplane_scheduling_dispatcher make_dispatcher(ru_downlink_scheduling_handler& dl, ru_uplink_scheduling_handler& ul)
{
  ru_cplane_scheduling_dispatcher_config config;
  config.sector     = 0;
  config.dl_eaxc    = {7};
  config.ul_eaxc    = {7};
  config.prach_eaxc = {4};
  return ru_cplane_scheduling_dispatcher(config, ocudulog::fetch_basic_logger("TEST"), dl, ul);
}

} // namespace

TEST(ru_cplane_scheduling_dispatcher_test, downlink_section_routed_to_downlink_handler)
{
  downlink_scheduling_handler_spy dl;
  uplink_scheduling_handler_spy   ul;
  ru_cplane_scheduling_dispatcher dispatcher = make_dispatcher(dl, ul);

  dispatcher.on_cplane_message_received(7, make_results(1, data_direction::downlink));

  ASSERT_EQ(1, dl.count);
  ASSERT_EQ(7, dl.eaxc);
  ASSERT_EQ(0, ul.ul_count);
  ASSERT_EQ(0, ul.prach_count);
}

TEST(ru_cplane_scheduling_dispatcher_test, uplink_section_routed_to_uplink_handler)
{
  downlink_scheduling_handler_spy dl;
  uplink_scheduling_handler_spy   ul;
  ru_cplane_scheduling_dispatcher dispatcher = make_dispatcher(dl, ul);

  dispatcher.on_cplane_message_received(7, make_results(1, data_direction::uplink));

  ASSERT_EQ(0, dl.count);
  ASSERT_EQ(1, ul.ul_count);
  ASSERT_EQ(0, ul.prach_count);
}

TEST(ru_cplane_scheduling_dispatcher_test, prach_section_routed_to_prach_handler)
{
  downlink_scheduling_handler_spy dl;
  uplink_scheduling_handler_spy   ul;
  ru_cplane_scheduling_dispatcher dispatcher = make_dispatcher(dl, ul);

  // Section type 3 is PRACH, routed to the PRACH path regardless of direction. eAxC 4 is the configured PRACH eAxC.
  dispatcher.on_cplane_message_received(4, make_results(3, data_direction::uplink));

  ASSERT_EQ(0, dl.count);
  ASSERT_EQ(0, ul.ul_count);
  ASSERT_EQ(1, ul.prach_count);
}

TEST(ru_cplane_scheduling_dispatcher_test, command_for_unconfigured_eaxc_is_dropped)
{
  downlink_scheduling_handler_spy dl;
  uplink_scheduling_handler_spy   ul;
  ru_cplane_scheduling_dispatcher dispatcher = make_dispatcher(dl, ul);

  // Uplink command for eAxC 9, which is not a configured uplink eAxC: dropped.
  dispatcher.on_cplane_message_received(9, make_results(1, data_direction::uplink));
  // PRACH command for eAxC 7, which is not a configured PRACH eAxC: dropped.
  dispatcher.on_cplane_message_received(7, make_results(3, data_direction::uplink));

  ASSERT_EQ(0, dl.count);
  ASSERT_EQ(0, ul.ul_count);
  ASSERT_EQ(0, ul.prach_count);
}
