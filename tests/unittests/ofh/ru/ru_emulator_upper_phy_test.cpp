// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_upper_phy.h"
#include "ocudu/adt/complex.h"
#include "ocudu/phy/support/prach_buffer.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/ran/slot_point.h"
#include <gtest/gtest.h>

using namespace ocudu;

static ru_emulator_upper_phy_config make_config()
{
  ru_emulator_upper_phy_config config;
  config.nof_prb           = 51;
  config.nof_dl_ports      = 4;
  config.nof_prach_symbols = 12;
  config.prach_long_format = false;
  return config;
}

TEST(ru_emulator_upper_phy_test, uplink_tx_grid_is_single_port_and_prefilled)
{
  auto       upper_phy = create_ru_emulator_upper_phy(make_config());
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 0);

  shared_resource_grid grid = upper_phy->get_uplink_tx_grid(slot);
  ASSERT_TRUE(grid.is_valid());

  const resource_grid_reader& reader = grid.get_reader();
  ASSERT_EQ(reader.get_nof_ports(), 1);
  ASSERT_EQ(reader.get_nof_subc(), 51 * NOF_SUBCARRIERS_PER_RB);
  ASSERT_FALSE(reader.is_empty(0));

  // The first resource element of the first symbol carries the deterministic test pattern (-0.5, -0.5).
  span<const cbf16_t> symbol0 = reader.get_view(0, 0);
  ASSERT_FALSE(symbol0.empty());
  cf_t sample = to_cf(symbol0[0]);
  ASSERT_NEAR(sample.real(), -0.5F, 0.01F);
  ASSERT_NEAR(sample.imag(), -0.5F, 0.01F);
}

TEST(ru_emulator_upper_phy_test, downlink_rx_grid_has_one_port_per_eaxc)
{
  auto       upper_phy = create_ru_emulator_upper_phy(make_config());
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 0);

  shared_resource_grid grid = upper_phy->get_downlink_rx_grid(slot);
  ASSERT_TRUE(grid.is_valid());
  ASSERT_EQ(grid.get_reader().get_nof_ports(), 4);
  ASSERT_EQ(grid.get_reader().get_nof_subc(), 51 * NOF_SUBCARRIERS_PER_RB);
}

TEST(ru_emulator_upper_phy_test, prach_buffer_is_prefilled)
{
  auto       upper_phy = create_ru_emulator_upper_phy(make_config());
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 0);

  const prach_buffer& prach = upper_phy->get_prach_tx_buffer(slot);
  span<const cbf16_t> sym0  = prach.get_symbol(0, 0, 0, 0);
  ASSERT_FALSE(sym0.empty());
  // First sample of the first symbol is non-zero test IQ.
  cf_t sample = to_cf(sym0[0]);
  ASSERT_NE(sample.real(), 0.0F);
}
