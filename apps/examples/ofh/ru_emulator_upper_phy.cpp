// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ru_emulator_upper_phy.h"
#include "ocudu/adt/complex.h"
#include "ocudu/phy/support/resource_grid.h"
#include "ocudu/phy/support/resource_grid_writer.h"
#include "ocudu/phy/support/support_factories.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/support/error_handling.h"
#include <vector>

using namespace ocudu;

namespace {

/// Number of grids held by each resource grid pool. The pools are round-robin; this only needs to cover the few grids
/// outstanding at a time (a downlink grid registered in the receive repository, an uplink grid being read by the
/// transmit data flow), with margin.
constexpr unsigned NOF_POOL_GRIDS = 16;

/// Number of OFDM symbols per slot assumed by the emulator (normal cyclic prefix).
constexpr unsigned NOF_SYMBOLS_PER_SLOT = get_nsymb_per_slot(cyclic_prefix::NORMAL);

/// Returns a deterministic test IQ sample for the given resource element coordinates.
cf_t test_iq_sample(unsigned symbol, unsigned re)
{
  // A gentle, reproducible ramp that stays within [-0.5, 0.5) and is non-zero, so a downstream observer can verify the
  // emulator transmits real IQ rather than zeros.
  float real = static_cast<float>(re % NOF_SUBCARRIERS_PER_RB) / static_cast<float>(NOF_SUBCARRIERS_PER_RB) - 0.5F;
  float imag = static_cast<float>(symbol) / static_cast<float>(NOF_SYMBOLS_PER_SLOT) - 0.5F;
  return {real, imag};
}

} // namespace

ru_emulator_upper_phy::ru_emulator_upper_phy(std::unique_ptr<resource_grid_pool> dl_rx_grid_pool_,
                                             std::unique_ptr<resource_grid_pool> ul_tx_grid_pool_,
                                             std::unique_ptr<prach_buffer>       prach_buffer_) :
  dl_rx_grid_pool(std::move(dl_rx_grid_pool_)),
  ul_tx_grid_pool(std::move(ul_tx_grid_pool_)),
  prach(std::move(prach_buffer_))
{
  ocudu_assert(dl_rx_grid_pool, "Invalid downlink reception grid pool");
  ocudu_assert(ul_tx_grid_pool, "Invalid uplink transmission grid pool");
  ocudu_assert(prach, "Invalid PRACH buffer");
}

shared_resource_grid ru_emulator_upper_phy::get_downlink_rx_grid(slot_point slot)
{
  return dl_rx_grid_pool->allocate_resource_grid(slot);
}

void ru_emulator_upper_phy::on_downlink_rx_grid_completed(slot_point /* slot */, const shared_resource_grid& /* grid */)
{
  // The loopback emulator does not consume received downlink grids.
}

shared_resource_grid ru_emulator_upper_phy::get_uplink_tx_grid(slot_point slot)
{
  return ul_tx_grid_pool->allocate_resource_grid(slot);
}

const prach_buffer& ru_emulator_upper_phy::get_prach_tx_buffer(slot_point /* slot */)
{
  return *prach;
}

std::unique_ptr<ru_emulator_upper_phy> ocudu::create_ru_emulator_upper_phy(const ru_emulator_upper_phy_config& config)
{
  std::shared_ptr<resource_grid_factory> rg_factory = create_resource_grid_factory();
  report_error_if_not(rg_factory, "Failed to create resource grid factory for the RU emulator upper PHY");

  const unsigned nof_subc = config.nof_prb * NOF_SUBCARRIERS_PER_RB;

  // Uplink transmission grids: single port, pre-filled with deterministic test IQ. The transmit data flow only reads
  // them, so filling once and reusing is correct.
  std::vector<cf_t>                           symbol_iq(nof_subc);
  std::vector<std::unique_ptr<resource_grid>> ul_grids;
  for (unsigned i = 0; i != NOF_POOL_GRIDS; ++i) {
    std::unique_ptr<resource_grid> grid   = rg_factory->create(1, NOF_SYMBOLS_PER_SLOT, nof_subc);
    resource_grid_writer&          writer = grid->get_writer();
    for (unsigned symbol = 0; symbol != NOF_SYMBOLS_PER_SLOT; ++symbol) {
      for (unsigned re = 0; re != nof_subc; ++re) {
        symbol_iq[re] = test_iq_sample(symbol, re);
      }
      writer.put(0, symbol, 0, symbol_iq);
    }
    ul_grids.push_back(std::move(grid));
  }
  std::unique_ptr<resource_grid_pool> ul_tx_grid_pool = create_generic_resource_grid_pool(std::move(ul_grids));

  // Downlink reception grids: one port per downlink eAxC. Written by the receive path and recycled.
  std::vector<std::unique_ptr<resource_grid>> dl_grids;
  for (unsigned i = 0; i != NOF_POOL_GRIDS; ++i) {
    dl_grids.push_back(rg_factory->create(config.nof_dl_ports, NOF_SYMBOLS_PER_SLOT, nof_subc));
  }
  std::unique_ptr<resource_grid_pool> dl_rx_grid_pool = create_generic_resource_grid_pool(std::move(dl_grids));

  // PRACH buffer, pre-filled with deterministic test IQ (single antenna, single occasion).
  std::unique_ptr<prach_buffer> prach =
      config.prach_long_format ? create_prach_buffer_long(1, 1) : create_prach_buffer_short(1, 1, 1);
  report_error_if_not(prach, "Failed to create PRACH buffer for the RU emulator upper PHY");

  unsigned nof_prach_symbols = std::min(config.nof_prach_symbols, prach->get_max_nof_symbols());
  for (unsigned symbol = 0; symbol != nof_prach_symbols; ++symbol) {
    span<cbf16_t> prach_symbol = prach->get_symbol(0, 0, 0, symbol);
    for (unsigned re = 0, nof_re = prach_symbol.size(); re != nof_re; ++re) {
      prach_symbol[re] = to_cbf16(test_iq_sample(symbol, re));
    }
  }

  return std::make_unique<ru_emulator_upper_phy>(
      std::move(dl_rx_grid_pool), std::move(ul_tx_grid_pool), std::move(prach));
}
