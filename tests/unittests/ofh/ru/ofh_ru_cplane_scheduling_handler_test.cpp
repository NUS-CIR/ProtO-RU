// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/operation_controller_dummy.h"
#include "../../../../lib/ofh/ru/ofh_ru_cplane_scheduling_handler.h"
#include "../../phy/support/resource_grid_test_doubles.h"
#include "ocudu/phy/support/prach_buffer.h"
#include <array>
#include <gtest/gtest.h>
#include <vector>

using namespace ocudu;
using namespace ofh;

namespace {

/// Spy transmit data flow recording the enqueued contexts.
class data_flow_uplane_data_spy : public data_flow_uplane_data
{
  operation_controller_dummy ctrl;

public:
  bool                                   ul_called    = false;
  bool                                   prach_called = false;
  data_flow_uplane_resource_grid_context ul_ctx{};
  data_flow_uplane_prach_context         prach_ctx{};

  operation_controller& get_operation_controller() override { return ctrl; }

  void enqueue_section_type_1_message(const data_flow_uplane_resource_grid_context& ctx,
                                      const shared_resource_grid&) override
  {
    ul_called = true;
    ul_ctx    = ctx;
  }

  void enqueue_prach_message(const data_flow_uplane_prach_context& ctx, const prach_buffer&) override
  {
    prach_called = true;
    prach_ctx    = ctx;
  }

  data_flow_message_encoding_metrics_collector* get_metrics_collector() override { return nullptr; }
};

/// Minimal PRACH buffer double.
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

/// Fake upper PHY returning the test grid / PRACH buffer.
class fake_upper_phy : public ru_upper_phy
{
  std::array<shared_resource_grid_spy*, 2> dl_grids;
  shared_resource_grid_spy&                ul_grid;
  prach_buffer_double&                     prach;
  unsigned                                 next_dl_grid = 0;

public:
  unsigned nof_downlink_grid_requests = 0;

  fake_upper_phy(shared_resource_grid_spy& dl_grid0,
                 shared_resource_grid_spy& dl_grid1,
                 shared_resource_grid_spy& ul_grid_,
                 prach_buffer_double&      prach_) :
    dl_grids{&dl_grid0, &dl_grid1}, ul_grid(ul_grid_), prach(prach_)
  {
  }

  shared_resource_grid get_downlink_rx_grid(slot_point) override
  {
    ++nof_downlink_grid_requests;
    ocudu_assert(next_dl_grid < dl_grids.size(), "No downlink test grid available");
    return dl_grids[next_dl_grid++]->get_grid();
  }
  void                 on_downlink_rx_grid_completed(slot_point, const shared_resource_grid&) override {}
  shared_resource_grid get_uplink_tx_grid(slot_point) override { return ul_grid.get_grid(); }
  const prach_buffer&  get_prach_tx_buffer(slot_point) override { return prach; }
};

} // namespace

class ru_cplane_scheduling_handler_fixture : public ::testing::Test
{
protected:
  static constexpr unsigned                   sector   = 0;
  static constexpr unsigned                   nof_prbs = 51;
  slot_point                                  slot     = slot_point(0, 0, 1);
  resource_grid_writer_spy                    rg_writer0{2, 14, nof_prbs};
  resource_grid_reader_spy                    rg_reader0{2, 14, nof_prbs};
  resource_grid_spy                           grid0{rg_reader0, rg_writer0};
  shared_resource_grid_spy                    shared_grid0{grid0};
  resource_grid_writer_spy                    rg_writer1{2, 14, nof_prbs};
  resource_grid_reader_spy                    rg_reader1{2, 14, nof_prbs};
  resource_grid_spy                           grid1{rg_reader1, rg_writer1};
  shared_resource_grid_spy                    shared_grid1{grid1};
  resource_grid_writer_spy                    ul_rg_writer{1, 14, nof_prbs};
  resource_grid_reader_spy                    ul_rg_reader{1, 14, nof_prbs};
  resource_grid_spy                           ul_grid{ul_rg_reader, ul_rg_writer};
  shared_resource_grid_spy                    shared_ul_grid{ul_grid};
  prach_buffer_double                         prach;
  fake_upper_phy                              upper_phy{shared_grid0, shared_grid1, shared_ul_grid, prach};
  std::shared_ptr<rx_grid_context_repository> grid_repo = std::make_shared<rx_grid_context_repository>(1);
  data_flow_uplane_data_spy                   ul_data_flow;
  data_flow_uplane_data_spy                   prach_data_flow;
  ru_cplane_scheduling_handler                handler;

  ru_cplane_scheduling_handler_fixture() : handler(get_config(), get_dependencies()) {}

  ru_cplane_scheduling_handler_config get_config()
  {
    ru_cplane_scheduling_handler_config config;
    config.sector = sector;
    return config;
  }

  ru_cplane_scheduling_handler_dependencies get_dependencies()
  {
    ru_cplane_scheduling_handler_dependencies dependencies;
    dependencies.logger           = &ocudulog::fetch_basic_logger("TEST");
    dependencies.upper_phy        = &upper_phy;
    dependencies.grid_repo        = grid_repo;
    dependencies.uplink_data_flow = &ul_data_flow;
    dependencies.prach_data_flow  = &prach_data_flow;
    return dependencies;
  }

  cplane_message_decoder_results make_results(uint8_t section_type, data_direction direction)
  {
    cplane_message_decoder_results results;
    results.section_type           = section_type;
    results.radio_hdr.direction    = direction;
    results.radio_hdr.slot         = slot;
    results.radio_hdr.start_symbol = 2;
    results.radio_hdr.filter_index = filter_index_type::ul_prach_preamble_short;
    results.section.prb_start      = 0;
    results.section.nof_prb        = nof_prbs;
    results.section.nof_symbols    = 4;
    results.frame_structure_scs    = cplane_scs::kHz15;
    return results;
  }
};

TEST_F(ru_cplane_scheduling_handler_fixture, downlink_command_prepares_receive_grid)
{
  handler.handle_downlink_scheduling(5, make_results(1, data_direction::downlink));
  grid_repo->process_pending_contexts();

  // The grid was registered for the scheduled symbols.
  ASSERT_FALSE(grid_repo->get(slot, 2).empty());
  ASSERT_FALSE(grid_repo->get(slot, 5).empty());
  ASSERT_FALSE(ul_data_flow.ul_called);
}

TEST_F(ru_cplane_scheduling_handler_fixture, downlink_commands_for_multiple_eaxcs_reuse_the_slot_grid)
{
  cplane_message_decoder_results results = make_results(1, data_direction::downlink);

  // Schedule the first antenna and receive some of its User-Plane before the second antenna is scheduled.
  handler.handle_downlink_scheduling(5, results);
  grid_repo->process_pending_contexts();
  std::vector<cbf16_t> iq_data(nof_prbs * NOF_SUBCARRIERS_PER_RB, cbf16_t(1));
  grid_repo->write_grid(slot, 0, results.radio_hdr.start_symbol, 0, iq_data);

  // A second eAxC for the same slot must reuse the first grid and must not reset the first port's received-RE mask.
  handler.handle_downlink_scheduling(6, results);
  grid_repo->process_pending_contexts();
  grid_repo->write_grid(slot, 1, results.radio_hdr.start_symbol, 0, iq_data);

  ASSERT_EQ(upper_phy.nof_downlink_grid_requests, 1U);
  ASSERT_EQ(rg_writer0.get_count(), 2U);
  ASSERT_EQ(rg_writer1.get_count(), 0U);
  ASSERT_TRUE(grid_repo->try_popping_complete_resource_grid_symbol(slot, results.radio_hdr.start_symbol).has_value());
}

TEST_F(ru_cplane_scheduling_handler_fixture, uplink_command_transmits_uplink_uplane)
{
  handler.handle_uplink_scheduling(5, make_results(1, data_direction::uplink));

  ASSERT_TRUE(ul_data_flow.ul_called);
  ASSERT_EQ(slot, ul_data_flow.ul_ctx.slot);
  ASSERT_EQ(5, ul_data_flow.ul_ctx.eaxc);
  ASSERT_EQ(2, ul_data_flow.ul_ctx.symbol_range.start());
  ASSERT_EQ(6, ul_data_flow.ul_ctx.symbol_range.stop());
  ASSERT_FALSE(prach_data_flow.prach_called);
}

TEST_F(ru_cplane_scheduling_handler_fixture, prach_command_transmits_prach_uplane)
{
  handler.handle_prach_scheduling(5, make_results(3, data_direction::uplink));

  ASSERT_TRUE(prach_data_flow.prach_called);
  ASSERT_EQ(slot, prach_data_flow.prach_ctx.slot);
  ASSERT_EQ(5, prach_data_flow.prach_ctx.eaxc);
  ASSERT_EQ(0, prach_data_flow.prach_ctx.prb_start);
  ASSERT_EQ(nof_prbs, prach_data_flow.prach_ctx.nof_prb);
  ASSERT_EQ(2, prach_data_flow.prach_ctx.start_symbol);
  ASSERT_EQ(4, prach_data_flow.prach_ctx.nof_symbols);
  ASSERT_EQ(filter_index_type::ul_prach_preamble_short, prach_data_flow.prach_ctx.filter_index);
  ASSERT_EQ(prach_subcarrier_spacing::kHz15, prach_data_flow.prach_ctx.prach_scs);
  ASSERT_FALSE(ul_data_flow.ul_called);
}
