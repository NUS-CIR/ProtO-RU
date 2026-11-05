// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ocuduvec/zero.h"
#include "ocudu/phy/lower/modulation/ofdm_demodulator.h"
#include "ocudu/phy/support/resource_grid_writer.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/support/memory_pool/bounded_object_pool.h"

namespace ocudu {

/// Implements an OFDM demodulator concurrent pool.
class ofdm_symbol_demodulator_pool : public ofdm_symbol_demodulator
{
public:
  /// Internal pool of demodulators.
  using demodulator_pool = bounded_unique_object_pool<ofdm_symbol_demodulator>;

  /// \brief Constructs an OFDM symbol demodulator concurrent pool.
  /// \param[in] base_         Base instance for getting the symbol size.
  /// \param[in] demodulators_ Shared pool of OFDM demodulators.
  /// \param[in] cp            Cyclic prefix type.
  ofdm_symbol_demodulator_pool(std::unique_ptr<ofdm_symbol_demodulator> base_,
                               std::shared_ptr<demodulator_pool>        demodulators_,
                               cyclic_prefix                            cp) :
    logger(ocudulog::fetch_basic_logger("PHY")),
    base(std::move(base_)),
    demodulators(std::move(demodulators_)),
    nof_symbols_per_slot(get_nsymb_per_slot(cp))
  {
  }

  // See the interface for documentation.
  unsigned get_symbol_size(unsigned symbol_index) const override { return base->get_symbol_size(symbol_index); }

  // See the interface for documentation.
  void set_center_frequency(double center_frequency_Hz_) override
  {
    center_frequency_Hz.store(center_frequency_Hz_, std::memory_order_relaxed);
  }

  // See the interface for documentation.
  void
  demodulate(resource_grid_writer& grid, span<const ci16_t> input, unsigned port_index, unsigned symbol_index) override
  {
    auto demodulator = demodulators->get();
    if (!demodulator) {
      // The symbol index is relative to the subframe, while the grid only holds the symbols of one slot.
      ocuduvec::zero(grid.get_view(port_index, symbol_index % nof_symbols_per_slot));
      logger.error("Insufficient number of OFDM demodulator instances.");
      return;
    }

    double current_center_frequency_Hz = center_frequency_Hz.load(std::memory_order_relaxed);
    if (std::isnormal(current_center_frequency_Hz)) {
      demodulator->set_center_frequency(current_center_frequency_Hz);
    }

    demodulator->demodulate(grid, input, port_index, symbol_index);
  }

private:
  std::atomic<double>                      center_frequency_Hz = {};
  ocudulog::basic_logger&                  logger;
  std::unique_ptr<ofdm_symbol_demodulator> base;
  std::shared_ptr<demodulator_pool>        demodulators;
  /// Number of OFDM symbols per slot.
  unsigned nof_symbols_per_slot;
};

} // namespace ocudu
