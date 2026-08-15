// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/ru/ofh_ru_prach_window_responder.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/ofh_controller.h"
#include "ocudu/phy/support/prach_buffer.h"
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

/// Data flow spy that records the PRACH contexts it is asked to enqueue.
class data_flow_uplane_data_spy : public data_flow_uplane_data
{
  operation_controller_dummy controller;

public:
  std::vector<data_flow_uplane_prach_context> enqueued;

  operation_controller& get_operation_controller() override { return controller; }
  void                  enqueue_section_type_1_message(const data_flow_uplane_resource_grid_context&,
                                                       const shared_resource_grid&) override
  {
  }
  void enqueue_prach_message(const data_flow_uplane_prach_context& context, const prach_buffer&) override
  {
    enqueued.push_back(context);
  }
  data_flow_message_encoding_metrics_collector* get_metrics_collector() override { return nullptr; }
};

class prach_buffer_double : public prach_buffer
{
  std::vector<cbf16_t> data{1};

public:
  unsigned            get_max_nof_ports() const override { return 1; }
  unsigned            get_max_nof_td_occasions() const override { return 1; }
  unsigned            get_max_nof_fd_occasions() const override { return 1; }
  unsigned            get_max_nof_symbols() const override { return MAX_NSYMB_PER_SLOT; }
  unsigned            get_sequence_length() const override { return data.size(); }
  span<cbf16_t>       get_symbol(unsigned, unsigned, unsigned, unsigned) override { return data; }
  span<const cbf16_t> get_symbol(unsigned, unsigned, unsigned, unsigned) const override { return data; }
};

ru_prach_window_responder_config make_config()
{
  ru_prach_window_responder_config config;
  config.sector     = 0;
  config.prach_eaxc = {4};
  return config;
}

/// PRACH request for eAxC 4: 12 symbols of 12 PRBs from the given start symbol, section ID 1.
ru_uplink_request make_request(slot_point slot, uint8_t start_symbol = 0)
{
  ru_uplink_request request;
  request.slot           = slot;
  request.filter_index   = filter_index_type::ul_prach_preamble_short;
  request.start_symbol   = start_symbol;
  request.prb_start      = 0;
  request.nof_prb        = 12;
  request.nof_symbols    = 12;
  request.section_id     = 1;
  request.prach_scs      = prach_subcarrier_spacing::kHz30;
  request.prach_start_rb = 6;
  return request;
}

} // namespace

TEST(ru_prach_window_responder_test, recorded_request_enqueues_prach_reply)
{
  auto       prach_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 7);
  prach_repo->add(4, make_request(slot));

  data_flow_uplane_data_spy data_flow;
  ru_prach_window_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), prach_repo, data_flow);

  prach_buffer_double buffer;
  responder.handle_prach_window(slot, buffer);

  ASSERT_EQ(data_flow.enqueued.size(), 1);
  ASSERT_EQ(data_flow.enqueued[0].eaxc, 4);
  ASSERT_EQ(data_flow.enqueued[0].nof_symbols, 12);
  ASSERT_EQ(data_flow.enqueued[0].nof_prb, 12);
  ASSERT_EQ(data_flow.enqueued[0].filter_index, filter_index_type::ul_prach_preamble_short);
  ASSERT_EQ(data_flow.enqueued[0].prach_scs, prach_subcarrier_spacing::kHz30);
  // The reply echoes the section ID of the recorded request.
  ASSERT_EQ(data_flow.enqueued[0].section_id, 1);
}

TEST(ru_prach_window_responder_test, no_recorded_request_no_reply)
{
  auto       prach_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 7);

  data_flow_uplane_data_spy data_flow;
  ru_prach_window_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), prach_repo, data_flow);

  prach_buffer_double buffer;
  responder.handle_prach_window(slot, buffer);

  ASSERT_TRUE(data_flow.enqueued.empty());
}

TEST(ru_prach_window_responder_test, request_is_cleared_after_the_reply)
{
  auto       prach_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 7);
  prach_repo->add(4, make_request(slot));

  data_flow_uplane_data_spy data_flow;
  ru_prach_window_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), prach_repo, data_flow);

  prach_buffer_double buffer;
  responder.handle_prach_window(slot, buffer);
  ASSERT_EQ(data_flow.enqueued.size(), 1);

  // The request was consumed: the same slot (e.g. one wire numbering period later, with the O-DU stopped) produces no
  // further replies and no occasion to peek.
  responder.handle_prach_window(slot, buffer);
  ASSERT_EQ(data_flow.enqueued.size(), 1);
  ASSERT_FALSE(responder.peek_prach_occasion(slot).has_value());
}

TEST(ru_prach_window_responder_test, request_for_a_different_slot_sharing_the_repository_index_is_not_answered)
{
  auto       prach_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 7);
  prach_repo->add(4, make_request(slot));

  data_flow_uplane_data_spy data_flow;
  ru_prach_window_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), prach_repo, data_flow);

  // A later slot mapping onto the same repository entry (repository size 40).
  slot_point          wrapped_slot(to_numerology_value(subcarrier_spacing::kHz30), 47);
  prach_buffer_double buffer;
  responder.handle_prach_window(wrapped_slot, buffer);

  ASSERT_TRUE(data_flow.enqueued.empty());
  ASSERT_FALSE(responder.peek_prach_occasion(wrapped_slot).has_value());
}

TEST(ru_prach_window_responder_test, peek_returns_recorded_occasion)
{
  auto       prach_repo = std::make_shared<ru_uplink_request_repository>(40);
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 7);
  prach_repo->add(4, make_request(slot, /*start_symbol=*/2));

  data_flow_uplane_data_spy data_flow;
  ru_prach_window_responder responder(make_config(), ocudulog::fetch_basic_logger("TEST"), prach_repo, data_flow);

  // The application's radio driver peeks the recorded occasion (slot + start symbol) to issue the capture request.
  std::optional<ru_prach_occasion> occasion = responder.peek_prach_occasion(slot);
  ASSERT_TRUE(occasion.has_value());
  ASSERT_EQ(occasion->slot, slot);
  ASSERT_EQ(occasion->eaxc, 4);
  ASSERT_EQ(occasion->start_symbol, 2);
  ASSERT_EQ(occasion->rb_offset, 6);
  ASSERT_EQ(occasion->prach_scs, prach_subcarrier_spacing::kHz30);

  // A slot with nothing recorded yields no occasion.
  slot_point empty_slot(to_numerology_value(subcarrier_spacing::kHz30), 8);
  ASSERT_FALSE(responder.peek_prach_occasion(empty_slot).has_value());
}
