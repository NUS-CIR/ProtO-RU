// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/ru/ofh_ru_uplink_symbol_responder.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/ofh_controller.h"
#include "ocudu/phy/support/support_factories.h"
#include "ocudu/ran/resource_block.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

namespace {

class operation_controller_dummy : public operation_controller
{
public:
  void start() override {}
  void stop() override {}
};

/// Data flow spy that records the contexts it is asked to enqueue.
class data_flow_uplane_data_spy : public data_flow_uplane_data
{
  operation_controller_dummy controller;

public:
  std::vector<data_flow_uplane_resource_grid_context> enqueued;

  operation_controller& get_operation_controller() override { return controller; }
  void                  enqueue_section_type_1_message(const data_flow_uplane_resource_grid_context& context,
                                                       const shared_resource_grid&) override
  {
    enqueued.push_back(context);
  }
  data_flow_message_encoding_metrics_collector* get_metrics_collector() override { return nullptr; }
};

shared_resource_grid make_grid(std::unique_ptr<resource_grid_pool>& pool_out, slot_point slot)
{
  auto                                        rg_factory = create_resource_grid_factory();
  std::vector<std::unique_ptr<resource_grid>> grids;
  grids.push_back(rg_factory->create(1, MAX_NSYMB_PER_SLOT, NOF_SUBCARRIERS_PER_RB));
  pool_out = create_generic_resource_grid_pool(std::move(grids));
  return pool_out->allocate_resource_grid(slot);
}

ru_uplink_symbol_responder_config make_config()
{
  ru_uplink_symbol_responder_config config;
  config.sector  = 0;
  config.ul_eaxc = {2};
  return config;
}

/// Request for eAxC 2: symbols [4, 14), full bandwidth, section ID 3.
ru_uplink_request make_request(slot_point slot)
{
  return {slot, filter_index_type::standard_channel_filter, 4, 0, 51, 10, 3};
}

} // namespace

TEST(ru_uplink_symbol_responder_test, symbol_in_window_enqueues_uplane_reply)
{
  auto       ul_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 5);
  ul_repo->add(2, make_request(slot));

  data_flow_uplane_data_spy  data_flow;
  ru_uplink_symbol_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), ul_repo, data_flow);

  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, slot);

  responder.handle_uplink_symbol(slot, /*symbol=*/5, grid);

  ASSERT_EQ(data_flow.enqueued.size(), 1);
  ASSERT_EQ(data_flow.enqueued[0].eaxc, 2);
  ASSERT_EQ(data_flow.enqueued[0].port, 0);
  ASSERT_EQ(data_flow.enqueued[0].symbol_range.start(), 5);
  ASSERT_EQ(data_flow.enqueued[0].symbol_range.stop(), 6);
  // The reply echoes the section ID of the recorded request.
  ASSERT_EQ(data_flow.enqueued[0].section_id, 3);
}

TEST(ru_uplink_symbol_responder_test, symbol_outside_window_is_ignored)
{
  auto       ul_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 5);
  ul_repo->add(2, make_request(slot));

  data_flow_uplane_data_spy  data_flow;
  ru_uplink_symbol_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), ul_repo, data_flow);

  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, slot);

  // Symbol 0 is before the requested window start (4).
  responder.handle_uplink_symbol(slot, /*symbol=*/0, grid);

  ASSERT_TRUE(data_flow.enqueued.empty());
}

TEST(ru_uplink_symbol_responder_test, no_recorded_request_no_reply)
{
  auto       ul_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 5);

  data_flow_uplane_data_spy  data_flow;
  ru_uplink_symbol_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), ul_repo, data_flow);

  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, slot);

  responder.handle_uplink_symbol(slot, /*symbol=*/5, grid);

  ASSERT_TRUE(data_flow.enqueued.empty());
}

TEST(ru_uplink_symbol_responder_test, request_for_a_different_slot_sharing_the_repository_index_is_not_answered)
{
  auto       ul_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 5);
  ul_repo->add(2, make_request(slot));

  data_flow_uplane_data_spy  data_flow;
  ru_uplink_symbol_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), ul_repo, data_flow);

  // A later slot mapping onto the same repository entry (repository size 40): a stale request recorded before the
  // O-DU stopped must not generate uplink traffic when the slot numbering wraps around.
  slot_point                          wrapped_slot(to_numerology_value(subcarrier_spacing::kHz30), 45);
  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, wrapped_slot);

  responder.handle_uplink_symbol(wrapped_slot, /*symbol=*/5, grid);

  ASSERT_TRUE(data_flow.enqueued.empty());
}

TEST(ru_uplink_symbol_responder_test, request_is_cleared_after_its_last_symbol_is_answered)
{
  auto       ul_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 5);
  ul_repo->add(2, make_request(slot));

  data_flow_uplane_data_spy  data_flow;
  ru_uplink_symbol_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), ul_repo, data_flow);

  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, slot);

  // Answer every symbol of the requested window [4, 14).
  for (unsigned symbol = 4; symbol != 14; ++symbol) {
    responder.handle_uplink_symbol(slot, symbol, grid);
  }
  ASSERT_EQ(data_flow.enqueued.size(), 10);

  // The request was consumed: capturing the same slot again (e.g. one wire numbering period later, with the O-DU
  // stopped) produces no further replies.
  responder.handle_uplink_symbol(slot, /*symbol=*/5, grid);
  ASSERT_EQ(data_flow.enqueued.size(), 10);
}
