// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../lib/ofh/ru/ofh_ru_downlink_rx_window_handler.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/phy/support/prach_buffer.h"
#include "ocudu/phy/support/support_factories.h"
#include <gtest/gtest.h>

using namespace ocudu;
using namespace ofh;

namespace {

class upper_phy_spy : public ru_upper_phy
{
  prach_buffer* dummy = nullptr;

public:
  unsigned             nof_completed = 0;
  shared_resource_grid get_downlink_rx_grid(slot_point) override { return {}; }
  void on_downlink_rx_grid_completed(slot_point, const shared_resource_grid&) override { ++nof_completed; }
  shared_resource_grid get_uplink_tx_grid(slot_point) override { return {}; }
  const prach_buffer&  get_prach_tx_buffer(slot_point) override { return *dummy; }
};

constexpr unsigned NOF_SYMBOLS_PER_SLOT = 14;

ru_downlink_rx_window_handler_config make_config(int finalize_offset_symbols = NOF_SYMBOLS_PER_SLOT)
{
  ru_downlink_rx_window_handler_config cfg;
  cfg.sector                  = 0;
  cfg.finalize_offset_symbols = finalize_offset_symbols;
  return cfg;
}

slot_symbol_point make_ota(unsigned slot_count, unsigned symbol)
{
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), slot_count);
  return {slot, symbol, NOF_SYMBOLS_PER_SLOT};
}

} // namespace

TEST(ru_downlink_rx_window_handler_test, registered_grid_is_finalised_one_slot_later)
{
  auto rg_factory = create_resource_grid_factory();
  ASSERT_TRUE(rg_factory);

  std::vector<std::unique_ptr<resource_grid>> grids;
  grids.push_back(rg_factory->create(1, NOF_SYMBOLS_PER_SLOT, NOF_SUBCARRIERS_PER_RB));
  auto                 pool = create_generic_resource_grid_pool(std::move(grids));
  slot_point           slot(to_numerology_value(subcarrier_spacing::kHz30), 10);
  shared_resource_grid grid = pool->allocate_resource_grid(slot);
  ASSERT_TRUE(grid.is_valid());

  auto                          grid_repo = std::make_shared<rx_grid_context_repository>(40);
  ocudulog::basic_logger&       logger    = ocudulog::fetch_basic_logger("TEST");
  upper_phy_spy                 upper_phy;
  ru_downlink_rx_window_handler handler(make_config(), upper_phy, grid_repo, logger);

  // Register a downlink grid for slot 10, symbols [0, 14).
  resource_grid_context ctx;
  ctx.slot   = slot;
  ctx.sector = 0;
  grid_repo->add(ctx, grid, ofdm_symbol_range(0, NOF_SYMBOLS_PER_SLOT), logger);

  // A tick at slot 10 (before a full slot elapses) must not finalise slot 10 yet.
  handler.on_new_symbol({make_ota(10, 0), {}});
  ASSERT_EQ(upper_phy.nof_completed, 0);

  // A tick one slot later closes slot 10, symbol 0: the grid is popped and the upper PHY notified.
  handler.on_new_symbol({make_ota(11, 0), {}});
  ASSERT_EQ(upper_phy.nof_completed, 1);
}

TEST(ru_downlink_rx_window_handler_test, negative_offset_finalises_ahead_of_air_time)
{
  auto rg_factory = create_resource_grid_factory();
  ASSERT_TRUE(rg_factory);

  std::vector<std::unique_ptr<resource_grid>> grids;
  grids.push_back(rg_factory->create(1, NOF_SYMBOLS_PER_SLOT, NOF_SUBCARRIERS_PER_RB));
  auto                 pool = create_generic_resource_grid_pool(std::move(grids));
  slot_point           slot(to_numerology_value(subcarrier_spacing::kHz30), 10);
  shared_resource_grid grid = pool->allocate_resource_grid(slot);
  ASSERT_TRUE(grid.is_valid());

  auto                    grid_repo = std::make_shared<rx_grid_context_repository>(40);
  ocudulog::basic_logger& logger    = ocudulog::fetch_basic_logger("TEST");
  upper_phy_spy           upper_phy;
  // SDR-style: finalize two slots (plus one symbol) ahead of air time, so the radio pipeline gets the grid in time.
  ru_downlink_rx_window_handler handler(
      make_config(-static_cast<int>(2 * NOF_SYMBOLS_PER_SLOT + 1)), upper_phy, grid_repo, logger);

  resource_grid_context ctx;
  ctx.slot   = slot;
  ctx.sector = 0;
  grid_repo->add(ctx, grid, ofdm_symbol_range(0, NOF_SYMBOLS_PER_SLOT), logger);

  // A tick three slots before air time is still too early to finalise slot 10, symbol 0.
  handler.on_new_symbol({make_ota(7, 12), {}});
  ASSERT_EQ(upper_phy.nof_completed, 0);

  // The tick whose look-ahead reaches slot 10, symbol 0 (OTA = slot 10 air time minus two slots and one symbol)
  // finalises it: the grid is popped ahead of air time, ready for the radio.
  handler.on_new_symbol({make_ota(7, 13), {}});
  ASSERT_EQ(upper_phy.nof_completed, 1);
}

TEST(ru_downlink_rx_window_handler_test, no_grid_no_notification)
{
  auto                          grid_repo = std::make_shared<rx_grid_context_repository>(40);
  ocudulog::basic_logger&       logger    = ocudulog::fetch_basic_logger("TEST");
  upper_phy_spy                 upper_phy;
  ru_downlink_rx_window_handler handler(make_config(), upper_phy, grid_repo, logger);

  // No grid registered: ticking does not notify completion.
  handler.on_new_symbol({make_ota(5, 0), {}});
  handler.on_new_symbol({make_ota(6, 0), {}});
  ASSERT_EQ(upper_phy.nof_completed, 0);
}
