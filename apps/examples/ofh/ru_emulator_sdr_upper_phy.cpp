// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ru_emulator_sdr_upper_phy.h"
#include "ru_emulator_gps_slot_alignment.h"
#include "ocudu/phy/support/resource_grid.h"
#include "ocudu/phy/support/resource_grid_context.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include "ocudu/phy/support/support_factories.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/ru/ru_downlink_plane.h"
#include "ocudu/support/error_handling.h"
#include <vector>

using namespace ocudu;

namespace {

/// Number of grids held by the downlink reception pool.
constexpr unsigned NOF_DL_POOL_GRIDS = 16;

} // namespace

ru_emulator_sdr_upper_phy::ru_emulator_sdr_upper_phy(unsigned                            sector,
                                                     std::unique_ptr<resource_grid_pool> dl_rx_grid_pool_) :
  dl_rx_grid_pool(std::move(dl_rx_grid_pool_)), sector_id(sector)
{
  ocudu_assert(dl_rx_grid_pool, "Invalid downlink reception grid pool");
}

shared_resource_grid ru_emulator_sdr_upper_phy::get_downlink_rx_grid(slot_point slot)
{
  shared_resource_grid grid = dl_rx_grid_pool->allocate_resource_grid(slot);
  if (grid && !grid.get_reader().is_empty()) {
    grid.get().set_all_zero();
  }
  return grid;
}

void ru_emulator_sdr_upper_phy::on_downlink_rx_grid_completed(slot_point slot, const shared_resource_grid& grid)
{
  // Completion is notified once per closing symbol but the grid spans the whole slot: push it on the first.
  {
    std::lock_guard<std::mutex> lock(mutex);
    if (last_pushed_slot == slot) {
      return;
    }
    last_pushed_slot = slot;
  }

  // Until the radio is connected and the initial GPS slot alignment is available the grid cannot be transmitted: drop
  // it, so it returns to the reception pool.
  ru_downlink_plane_handler* handler = dl_handler.load(std::memory_order_relaxed);
  const int64_t              offset  = gps_slot_offset.load(std::memory_order_relaxed);
  if ((handler == nullptr) || (offset < 0)) {
    return;
  }

  // Translate the wire (GPS) slot back into the radio slot numbering and hand the grid to the Radio Unit. This runs at
  // the reception-window finalize point, ahead of the lower PHY's baseband pull (one millisecond before air time).
  // The offset only determines the radio slot modulo the wire numbering period (256 frames), while the radio numbers
  // its slots over 1024 frames: pick the candidate nearest the radio's current slot (the slot being pushed airs within
  // a couple of slots of it).
  const subcarrier_spacing scs          = to_subcarrier_spacing(slot.numerology());
  const unsigned           period       = gps_slot_alignment::nof_slots_per_ofh_period(scs);
  const unsigned           radio_period = NOF_SFNS * NOF_SUBFRAMES_PER_FRAME * get_nof_slots_per_subframe(scs);
  const unsigned           base    = (slot.system_slot() + period - static_cast<unsigned>(offset) % period) % period;
  const uint32_t           current = current_radio_slot_count.load(std::memory_order_relaxed);
  const unsigned           past_wire_periods = (((current + radio_period - base) % radio_period) + period / 2) / period;
  const unsigned           radio_slot_count  = (base + past_wire_periods * period) % radio_period;

  resource_grid_context context;
  context.slot   = slot_point(slot.numerology(), radio_slot_count);
  context.sector = sector_id;
  handler->handle_dl_data(context, grid);
  nof_dl_tx.fetch_add(1, std::memory_order_relaxed);
}

const prach_buffer& ru_emulator_sdr_upper_phy::get_prach_tx_buffer(slot_point /* slot */)
{
  report_fatal_error("get_prach_tx_buffer is not used in SDR mode; the radio supplies the PRACH IQ");
}

std::unique_ptr<ru_emulator_sdr_upper_phy>
ocudu::create_ru_emulator_sdr_upper_phy(const ru_emulator_sdr_upper_phy_config& config)
{
  std::shared_ptr<resource_grid_factory> rg_factory = create_resource_grid_factory();
  report_error_if_not(rg_factory, "Failed to create resource grid factory for the RU emulator SDR upper PHY");

  const unsigned nof_subc    = config.nof_prb * NOF_SUBCARRIERS_PER_RB;
  const unsigned nof_symbols = get_nsymb_per_slot(cyclic_prefix::NORMAL);

  std::vector<std::unique_ptr<resource_grid>> dl_grids;
  for (unsigned i = 0; i != NOF_DL_POOL_GRIDS; ++i) {
    dl_grids.push_back(rg_factory->create(config.nof_dl_ports, nof_symbols, nof_subc));
  }
  auto dl_rx_grid_pool = create_generic_resource_grid_pool(std::move(dl_grids));

  return std::make_unique<ru_emulator_sdr_upper_phy>(config.sector, std::move(dl_rx_grid_pool));
}
