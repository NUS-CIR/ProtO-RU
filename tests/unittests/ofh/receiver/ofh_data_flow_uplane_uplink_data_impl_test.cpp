// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/receiver/ofh_data_flow_uplane_uplink_data_impl.h"
#include "../ofh_uplane_rx_symbol_notifier_test_doubles.h"
#include "helpers.h"
#include "ocudu/ocudulog/ocudulog.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;
using namespace ofh::testing;

namespace {
class uplane_message_decoder_spy : public uplane_message_decoder
{
  uplane_message_decoder_results spy_results;

public:
  bool decode(uplane_message_decoder_results& results, span<const uint8_t> message) override
  {
    results = spy_results;

    return true;
  }

  void set_results(const uplane_message_decoder_results& results) { spy_results = results; }
};

} // namespace

class data_flow_uplane_uplink_data_impl_fixture : public ::testing::Test
{
protected:
  const static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc = {5, 1, 2, 3};
  slot_point                                            slot;
  unsigned                                              symbol   = 0;
  unsigned                                              nof_prbs = 51;
  unsigned                                              sector   = 0;
  unsigned                                              eaxc     = 5;
  resource_grid_writer_bool_spy                         rg_writer;
  resource_grid_reader_spy                              rg_reader;
  resource_grid_spy                                     grid;
  shared_resource_grid_spy                              shared_grid;
  uplane_rx_symbol_notifier_spy*                        notifier;
  std::shared_ptr<uplink_cplane_context_repository>     ul_cplane_context_repo_ptr =
      std::make_shared<uplink_cplane_context_repository>(1);
  std::shared_ptr<rx_grid_context_repository> ul_context_repo = std::make_shared<rx_grid_context_repository>(1);
  uplane_message_decoder_spy*                 uplane_decoder;
  data_flow_uplane_uplink_data_impl           data_flow;

public:
  data_flow_uplane_uplink_data_impl_fixture() :
    slot(0, 0, 1),
    rg_writer(nof_prbs),
    rg_reader(rg_writer.get_nof_ports(), rg_writer.get_nof_symbols(), nof_prbs),
    grid(rg_reader, rg_writer),
    shared_grid(grid),
    data_flow(get_config(), get_dependencies())
  {
    ul_cplane_context context = {.filter_index = ocudu::ofh::filter_index_type::standard_channel_filter,
                                 .start_symbol = 0,
                                 .prb_start    = 0,
                                 .nof_prb      = static_cast<uint16_t>(nof_prbs),
                                 .nof_symbols  = 14};

    // Fill the contexts
    ul_cplane_context_repo_ptr->add(slot, eaxc, context);
    ul_context_repo->add(resource_grid_context{.slot = slot, .sector = sector},
                         shared_grid.get_grid(),
                         ofdm_symbol_range{context.start_symbol, context.nof_symbols},
                         ocudulog::fetch_basic_logger("TEST"));
    ul_context_repo->process_pending_contexts();
  }

  data_flow_uplane_uplink_data_impl_config get_config()
  {
    return data_flow_uplane_uplink_data_impl_config{.sector = 0, .are_metrics_enabled = false, .ul_eaxc = ul_eaxc};
  }

  data_flow_uplane_uplink_data_impl_dependencies get_dependencies()
  {
    data_flow_uplane_uplink_data_impl_dependencies dependencies;

    dependencies.logger                 = &ocudulog::fetch_basic_logger("TEST");
    dependencies.ul_cplane_context_repo = ul_cplane_context_repo_ptr;
    dependencies.ul_context_repo        = ul_context_repo;

    {
      auto temp             = std::make_shared<uplane_rx_symbol_notifier_spy>();
      notifier              = temp.get();
      dependencies.notifier = std::move(temp);
    }
    {
      auto temp                   = std::make_unique<uplane_message_decoder_spy>();
      uplane_decoder              = temp.get();
      dependencies.uplane_decoder = std::move(temp);
    }

    return dependencies;
  }

  uplane_message_decoder_results build_valid_decoder_results()
  {
    uplane_message_decoder_results deco_results = {
        .params   = {.direction    = data_direction::uplink,
                     .slot         = slot,
                     .filter_index = filter_index_type::standard_channel_filter,
                     .symbol_id    = 0},
        .sections = {
            {.is_every_rb_used = true, .use_current_symbol_number = true, .start_prb = 0, .nof_prbs = nof_prbs}}};
    deco_results.sections.back().iq_samples.resize(MAX_NOF_SUBCARRIERS);

    return deco_results;
  }
};

TEST_F(data_flow_uplane_uplink_data_impl_fixture, valid_message_containing_all_symbols_must_be_notified)
{
  uplane_message_decoder_results deco_results = build_valid_decoder_results();
  uplane_decoder->set_results(deco_results);
  data_flow.decode_type1_message(eaxc, {}, true);

  ASSERT_TRUE(notifier->has_new_uplink_symbol_function_been_called());
  ASSERT_FALSE(notifier->has_new_prach_function_been_called());
  ASSERT_EQ(nof_prbs, rg_writer.get_nof_prbs_written());
}

TEST_F(data_flow_uplane_uplink_data_impl_fixture, invalid_filter_index_does_not_write_grid)
{
  uplane_message_decoder_results deco_results = build_valid_decoder_results();
  deco_results.params.filter_index            = ocudu::ofh::filter_index_type::ul_prach_preamble_1p25khz;
  uplane_decoder->set_results(deco_results);
  data_flow.decode_type1_message(eaxc, {}, true);

  ASSERT_FALSE(notifier->has_new_uplink_symbol_function_been_called());
  ASSERT_FALSE(notifier->has_new_prach_function_been_called());
  ASSERT_FALSE(rg_writer.has_grid_been_written());
}

TEST_F(data_flow_uplane_uplink_data_impl_fixture, invalid_prb_range_does_not_write_grid)
{
  uplane_message_decoder_results deco_results = build_valid_decoder_results();
  deco_results.sections.front().start_prb     = 51;
  uplane_decoder->set_results(deco_results);
  data_flow.decode_type1_message(eaxc, {}, true);

  ASSERT_FALSE(notifier->has_new_uplink_symbol_function_been_called());
  ASSERT_FALSE(notifier->has_new_prach_function_been_called());
  ASSERT_FALSE(rg_writer.has_grid_been_written());
}

TEST_F(data_flow_uplane_uplink_data_impl_fixture, partial_prb_falling_outside_cplane_prbs_does_not_write_grid)
{
  uplane_message_decoder_results deco_results = build_valid_decoder_results();
  deco_results.sections.front().start_prb     = 41;
  deco_results.sections.front().nof_prbs      = 51;
  uplane_decoder->set_results(deco_results);
  data_flow.decode_type1_message(eaxc, {}, true);

  ASSERT_FALSE(notifier->has_new_uplink_symbol_function_been_called());
  ASSERT_FALSE(notifier->has_new_prach_function_been_called());
  ASSERT_FALSE(rg_writer.has_grid_been_written());
}

TEST_F(data_flow_uplane_uplink_data_impl_fixture, partial_prb_inside_cplane_prbs_write_grid)
{
  uplane_message_decoder_results deco_results = build_valid_decoder_results();
  unsigned                       num_prbs     = 23;
  deco_results.sections.front().start_prb     = 10;
  deco_results.sections.front().nof_prbs      = num_prbs;
  uplane_decoder->set_results(deco_results);
  data_flow.decode_type1_message(eaxc, {}, true);

  ASSERT_FALSE(notifier->has_new_uplink_symbol_function_been_called());
  ASSERT_FALSE(notifier->has_new_prach_function_been_called());
  ASSERT_TRUE(rg_writer.has_grid_been_written());
  ASSERT_EQ(num_prbs, rg_writer.get_nof_prbs_written());
}
