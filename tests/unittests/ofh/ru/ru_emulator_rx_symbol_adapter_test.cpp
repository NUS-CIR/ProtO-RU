// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_rx_symbol_adapter.h"
#include "ocudu/phy/support/support_factories.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/ran/slot_point.h"
#include <gtest/gtest.h>

using namespace ocudu;

namespace {

/// Sink spy recording the captured uplink symbols pushed into it.
class uplink_iq_sink_spy : public ofh::ru_uplink_iq_sink
{
public:
  unsigned   nof_uplink_symbols = 0;
  slot_point last_slot;
  unsigned   last_symbol = 0;

  void handle_uplink_symbol(slot_point slot, unsigned symbol, const shared_resource_grid&) override
  {
    ++nof_uplink_symbols;
    last_slot   = slot;
    last_symbol = symbol;
  }
  void handle_prach_window(slot_point, const prach_buffer&) override {}
};

shared_resource_grid make_grid(std::unique_ptr<resource_grid_pool>& pool_out, slot_point slot)
{
  auto                                        rg_factory = create_resource_grid_factory();
  std::vector<std::unique_ptr<resource_grid>> grids;
  grids.push_back(rg_factory->create(1, MAX_NSYMB_PER_SLOT, NOF_SUBCARRIERS_PER_RB));
  pool_out = create_generic_resource_grid_pool(std::move(grids));
  return pool_out->allocate_resource_grid(slot);
}

} // namespace

TEST(ru_emulator_rx_symbol_adapter_test, valid_symbol_is_forwarded_to_sink)
{
  uplink_iq_sink_spy            sink;
  ru_emulator_rx_symbol_adapter adapter(sink);

  slot_point                          slot(to_numerology_value(subcarrier_spacing::kHz30), 9);
  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, slot);

  adapter.on_new_uplink_symbol({slot, /*sector=*/0, /*symbol_id=*/3}, grid, /*is_valid=*/true);

  ASSERT_EQ(sink.nof_uplink_symbols, 1);
  ASSERT_EQ(sink.last_slot, slot);
  ASSERT_EQ(sink.last_symbol, 3);
}

TEST(ru_emulator_rx_symbol_adapter_test, invalid_symbol_is_dropped)
{
  uplink_iq_sink_spy            sink;
  ru_emulator_rx_symbol_adapter adapter(sink);

  slot_point                          slot(to_numerology_value(subcarrier_spacing::kHz30), 9);
  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, slot);

  adapter.on_new_uplink_symbol({slot, /*sector=*/0, /*symbol_id=*/3}, grid, /*is_valid=*/false);

  ASSERT_EQ(sink.nof_uplink_symbols, 0);
}

TEST(ru_emulator_rx_symbol_adapter_test, gps_slot_offset_translates_radio_slot_to_wire_numbering)
{
  uplink_iq_sink_spy            sink;
  ru_emulator_rx_symbol_adapter adapter(sink);
  adapter.set_gps_slot_offset(100);

  slot_point                          radio_slot(to_numerology_value(subcarrier_spacing::kHz30), 9);
  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, radio_slot);

  adapter.on_new_uplink_symbol({radio_slot, /*sector=*/0, /*symbol_id=*/3}, grid, /*is_valid=*/true);

  ASSERT_EQ(sink.nof_uplink_symbols, 1);
  ASSERT_EQ(sink.last_slot, slot_point(to_numerology_value(subcarrier_spacing::kHz30), 109));
}

TEST(ru_emulator_rx_symbol_adapter_test, gps_slot_offset_wraps_at_the_wire_numbering_period)
{
  uplink_iq_sink_spy            sink;
  ru_emulator_rx_symbol_adapter adapter(sink);
  adapter.set_gps_slot_offset(10);

  // 30 kHz wire numbering period: 256 frames * 20 slots = 5120 slots.
  slot_point                          radio_slot(to_numerology_value(subcarrier_spacing::kHz30), 5115);
  std::unique_ptr<resource_grid_pool> pool;
  shared_resource_grid                grid = make_grid(pool, radio_slot);

  adapter.on_new_uplink_symbol({radio_slot, /*sector=*/0, /*symbol_id=*/0}, grid, /*is_valid=*/true);

  ASSERT_EQ(sink.last_slot, slot_point(to_numerology_value(subcarrier_spacing::kHz30), 5));
}
