// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_sdr_upper_phy.h"
#include "ocudu/phy/support/resource_grid_context.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include "ocudu/phy/support/resource_grid_writer.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/ran/slot_point.h"
#include "ocudu/ru/ru_downlink_plane.h"
#include <gtest/gtest.h>

using namespace ocudu;

namespace {

ru_emulator_sdr_upper_phy_config make_config()
{
  ru_emulator_sdr_upper_phy_config config;
  config.sector       = 0;
  config.nof_prb      = 51;
  config.nof_dl_ports = 2;
  return config;
}

/// Radio downlink plane spy recording the grids handed over for transmission.
class downlink_plane_handler_spy : public ru_downlink_plane_handler
{
public:
  unsigned   nof_grids = 0;
  slot_point last_slot;

  void handle_dl_data(const resource_grid_context& context, const shared_resource_grid&) override
  {
    ++nof_grids;
    last_slot = context.slot;
  }
};

} // namespace

TEST(ru_emulator_sdr_upper_phy_test, downlink_rx_grid_has_one_port_per_eaxc)
{
  auto       upper_phy = create_ru_emulator_sdr_upper_phy(make_config());
  slot_point slot(to_numerology_value(subcarrier_spacing::kHz30), 0);

  shared_resource_grid grid = upper_phy->get_downlink_rx_grid(slot);
  ASSERT_TRUE(grid.is_valid());
  ASSERT_EQ(grid.get_reader().get_nof_ports(), 2);
  ASSERT_EQ(grid.get_reader().get_nof_subc(), 51 * NOF_SUBCARRIERS_PER_RB);
}

TEST(ru_emulator_sdr_upper_phy_test, recycled_downlink_rx_grid_is_cleared_before_reuse)
{
  auto           upper_phy  = create_ru_emulator_sdr_upper_phy(make_config());
  const unsigned numerology = to_numerology_value(subcarrier_spacing::kHz30);

  shared_resource_grid first_grid = upper_phy->get_downlink_rx_grid(slot_point(numerology, 0));
  ASSERT_TRUE(first_grid.is_valid());
  first_grid.get_writer().get_view(0, 0)[0] = to_cbf16(cf_t(1.0F, -1.0F));
  ASSERT_FALSE(first_grid.get_reader().is_empty());
  first_grid.release();

  // Advance through the remaining 15 pool entries so the next allocation reuses the written grid.
  for (unsigned i = 1; i != 16; ++i) {
    shared_resource_grid grid = upper_phy->get_downlink_rx_grid(slot_point(numerology, i));
    ASSERT_TRUE(grid.is_valid());
  }

  shared_resource_grid recycled_grid = upper_phy->get_downlink_rx_grid(slot_point(numerology, 16));
  ASSERT_TRUE(recycled_grid.is_valid());
  ASSERT_TRUE(recycled_grid.get_reader().is_empty());
  ASSERT_EQ(recycled_grid.get_reader().get_view(0, 0)[0], cbf16_t{});
}

TEST(ru_emulator_sdr_upper_phy_test, completed_grid_is_pushed_to_the_radio_once_in_radio_numbering)
{
  auto upper_phy = create_ru_emulator_sdr_upper_phy(make_config());

  downlink_plane_handler_spy radio;
  upper_phy->connect_downlink(radio);
  // Radio slot 5 maps to wire slot 8.
  upper_phy->set_gps_slot_offset(3);
  upper_phy->set_current_radio_slot(slot_point(to_numerology_value(subcarrier_spacing::kHz30), 4));

  slot_point           wire_slot(to_numerology_value(subcarrier_spacing::kHz30), 8);
  shared_resource_grid grid = upper_phy->get_downlink_rx_grid(wire_slot);

  // Completion is notified once per closing symbol of the slot: only the first pushes.
  upper_phy->on_downlink_rx_grid_completed(wire_slot, grid);
  upper_phy->on_downlink_rx_grid_completed(wire_slot, grid);

  ASSERT_EQ(radio.nof_grids, 1);
  ASSERT_EQ(radio.last_slot, slot_point(to_numerology_value(subcarrier_spacing::kHz30), 5));
  ASSERT_EQ(upper_phy->get_nof_transmitted_dl_grids(), 1);
}

TEST(ru_emulator_sdr_upper_phy_test, gps_slot_offset_translation_wraps_at_the_wire_numbering_period)
{
  auto upper_phy = create_ru_emulator_sdr_upper_phy(make_config());

  downlink_plane_handler_spy radio;
  upper_phy->connect_downlink(radio);
  upper_phy->set_gps_slot_offset(10);
  upper_phy->set_current_radio_slot(slot_point(to_numerology_value(subcarrier_spacing::kHz30), 5113));

  // 30 kHz wire numbering period: 256 frames * 20 slots = 5120 slots. Wire slot 5 maps back to radio slot 5115.
  slot_point           wire_slot(to_numerology_value(subcarrier_spacing::kHz30), 5);
  shared_resource_grid grid = upper_phy->get_downlink_rx_grid(wire_slot);
  upper_phy->on_downlink_rx_grid_completed(wire_slot, grid);

  ASSERT_EQ(radio.nof_grids, 1);
  ASSERT_EQ(radio.last_slot, slot_point(to_numerology_value(subcarrier_spacing::kHz30), 5115));
}

TEST(ru_emulator_sdr_upper_phy_test, translation_targets_the_wire_period_of_the_current_radio_slot)
{
  auto upper_phy = create_ru_emulator_sdr_upper_phy(make_config());

  downlink_plane_handler_spy radio;
  upper_phy->connect_downlink(radio);
  upper_phy->set_gps_slot_offset(3);

  // The wire numbering wraps every 5120 slots while the radio counts over 20480: a radio in its second wire period
  // (current slot 5120 + 3) must receive the grid labeled with its own numbering, not the first period's.
  upper_phy->set_current_radio_slot(slot_point(to_numerology_value(subcarrier_spacing::kHz30), 5123));

  slot_point           wire_slot(to_numerology_value(subcarrier_spacing::kHz30), 8);
  shared_resource_grid grid = upper_phy->get_downlink_rx_grid(wire_slot);
  upper_phy->on_downlink_rx_grid_completed(wire_slot, grid);

  ASSERT_EQ(radio.nof_grids, 1);
  ASSERT_EQ(radio.last_slot, slot_point(to_numerology_value(subcarrier_spacing::kHz30), 5125));
}

TEST(ru_emulator_sdr_upper_phy_test, completions_before_alignment_are_dropped_and_do_not_starve_the_pool)
{
  auto upper_phy = create_ru_emulator_sdr_upper_phy(make_config());

  downlink_plane_handler_spy radio;
  upper_phy->connect_downlink(radio);
  // No GPS slot offset set yet: grids completed during the alignment settle period cannot be transmitted.

  const unsigned numerology = to_numerology_value(subcarrier_spacing::kHz30);
  // Complete more slots than the pool holds (16 grids): every completion must release its grid back to the pool.
  for (unsigned i = 0; i != 32; ++i) {
    slot_point           slot(numerology, i);
    shared_resource_grid grid = upper_phy->get_downlink_rx_grid(slot);
    ASSERT_TRUE(grid.is_valid());
    upper_phy->on_downlink_rx_grid_completed(slot, grid);
  }

  ASSERT_EQ(radio.nof_grids, 0);
  // The pool did not starve.
  ASSERT_TRUE(upper_phy->get_downlink_rx_grid(slot_point(numerology, 40)).is_valid());
}
