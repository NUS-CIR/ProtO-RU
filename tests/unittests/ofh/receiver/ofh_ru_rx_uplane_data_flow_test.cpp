// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/receiver/ofh_ru_rx_uplane_data_flow.h"
#include "helpers.h"
#include "ocudu/ofh/serdes/ofh_uplane_message_decoder_properties.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;
using namespace ofh::testing;

namespace {

/// Decoder spy returning preset results.
class uplane_message_decoder_spy : public uplane_message_decoder
{
  uplane_message_decoder_results spy_results;

public:
  bool decode(uplane_message_decoder_results& results, span<const uint8_t> /*message*/) override
  {
    results = spy_results;
    return true;
  }

  void set_results(const uplane_message_decoder_results& results) { spy_results = results; }
};

} // namespace

class ru_rx_uplane_data_flow_fixture : public ::testing::Test
{
protected:
  const static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> eaxc_list = {5, 1, 2, 3};
  slot_point                                            slot;
  unsigned                                              nof_prbs = 51;
  unsigned                                              sector   = 0;
  unsigned                                              eaxc     = 5;
  resource_grid_writer_bool_spy                         rg_writer;
  resource_grid_reader_spy                              rg_reader;
  resource_grid_spy                                     grid;
  shared_resource_grid_spy                              shared_grid;
  std::shared_ptr<rx_grid_context_repository>           grid_repo = std::make_shared<rx_grid_context_repository>(1);
  uplane_message_decoder_spy*                           decoder;
  ru_rx_uplane_data_flow                                data_flow;

public:
  ru_rx_uplane_data_flow_fixture() :
    slot(0, 0, 1),
    rg_writer(nof_prbs),
    rg_reader(rg_writer.get_nof_ports(), rg_writer.get_nof_symbols(), nof_prbs),
    grid(rg_reader, rg_writer),
    shared_grid(grid),
    data_flow(get_config(), get_dependencies())
  {
    grid_repo->add({slot, sector}, shared_grid.get_grid(), {0, 14}, ocudulog::fetch_basic_logger("TEST"));
    grid_repo->process_pending_contexts();
  }

  ru_rx_uplane_data_flow_config get_config()
  {
    ru_rx_uplane_data_flow_config config;
    config.sector = sector;
    config.eaxc   = eaxc_list;

    return config;
  }

  ru_rx_uplane_data_flow_dependencies get_dependencies()
  {
    ru_rx_uplane_data_flow_dependencies dependencies;
    dependencies.logger    = &ocudulog::fetch_basic_logger("TEST");
    dependencies.grid_repo = grid_repo;

    auto temp                   = std::make_unique<uplane_message_decoder_spy>();
    decoder                     = temp.get();
    dependencies.uplane_decoder = std::move(temp);

    return dependencies;
  }

  uplane_message_decoder_results build_downlink_results()
  {
    uplane_message_decoder_results results;
    results.params.slot               = slot;
    results.params.direction          = data_direction::downlink;
    results.params.filter_index       = filter_index_type::standard_channel_filter;
    results.params.symbol_id          = 0;
    auto& section                     = results.sections.emplace_back();
    section.start_prb                 = 0;
    section.nof_prbs                  = nof_prbs;
    section.use_current_symbol_number = true;
    section.is_every_rb_used          = true;
    section.iq_samples.resize(MAX_NOF_SUBCARRIERS);

    return results;
  }
};

TEST_F(ru_rx_uplane_data_flow_fixture, decodes_downlink_uplane_and_writes_to_grid)
{
  decoder->set_results(build_downlink_results());
  data_flow.decode_message(eaxc, {});

  ASSERT_EQ(nof_prbs, rg_writer.get_nof_prbs_written());
}

TEST_F(ru_rx_uplane_data_flow_fixture, without_a_grid_context_nothing_is_written)
{
  grid_repo->clear();
  decoder->set_results(build_downlink_results());
  data_flow.decode_message(eaxc, {});

  ASSERT_FALSE(rg_writer.has_grid_been_written());
}
