// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "../../support/resource_grid_test_doubles.h"
#include "ocudu/phy/lower/modulation/modulation_factories.h"
#include <functional>
#include <gtest/gtest.h>

using namespace ocudu;

namespace {

constexpr unsigned nof_ports   = 2;
constexpr unsigned nof_symbols = MAX_NSYMB_PER_SLOT;
constexpr unsigned bw_rb       = 4;

// Fills all the resource elements of the grid with a non-zero value.
void fill_grid(resource_grid_writer_spy& grid)
{
  for (unsigned i_port = 0; i_port != nof_ports; ++i_port) {
    for (unsigned i_symbol = 0; i_symbol != nof_symbols; ++i_symbol) {
      for (cbf16_t& re : grid.get_view(i_port, i_symbol)) {
        re = cbf16_t(1.0F, 1.0F);
      }
    }
  }
}

// Returns true if all the resource elements of the given port and symbol are zero.
bool is_zero(resource_grid_writer_spy& grid, unsigned i_port, unsigned i_symbol)
{
  span<cbf16_t> view = grid.get_view(i_port, i_symbol);
  return std::all_of(view.begin(), view.end(), [](cbf16_t re) { return re == cbf16_t(0.0F, 0.0F); });
}

// OFDM demodulator that runs a hook while it is in use, so that the test can access the pool with all its instances
// busy.
class ofdm_symbol_demodulator_hook : public ofdm_symbol_demodulator
{
public:
  explicit ofdm_symbol_demodulator_hook(std::function<void()>& hook_) : hook(hook_) {}

  unsigned get_symbol_size(unsigned symbol_index) const override { return 1; }

  void set_center_frequency(double center_frequency_Hz) override {}

  void
  demodulate(resource_grid_writer& grid, span<const ci16_t> input, unsigned port_index, unsigned symbol_index) override
  {
    if (hook) {
      hook();
    }
  }

private:
  std::function<void()>& hook;
};

class ofdm_demodulator_factory_hook : public ofdm_demodulator_factory
{
public:
  std::unique_ptr<ofdm_symbol_demodulator>
  create_ofdm_symbol_demodulator(const ofdm_demodulator_configuration&) override
  {
    return std::make_unique<ofdm_symbol_demodulator_hook>(hook);
  }

  std::unique_ptr<ofdm_slot_demodulator> create_ofdm_slot_demodulator(const ofdm_demodulator_configuration&) override
  {
    return nullptr;
  }

  std::function<void()> hook;
};

} // namespace

class OfdmDemodulatorPoolFixture : public ::testing::TestWithParam<cyclic_prefix>
{};

// Without available demodulator instances, the pool zeroes the destination symbol of the slot. The symbol index is
// relative to the subframe, so symbols beyond the first slot of the subframe map to the corresponding slot symbol. The
// grid always holds MAX_NSYMB_PER_SLOT symbols, so with extended CP it holds more symbols than the slot.
TEST_P(OfdmDemodulatorPoolFixture, NoInstanceZeroesSlotSymbol)
{
  cyclic_prefix cp                   = GetParam();
  unsigned      nof_symbols_per_slot = get_nsymb_per_slot(cp);

  // The pool holds a single instance, which is busy while its hook runs.
  auto base_factory = std::make_shared<ofdm_demodulator_factory_hook>();
  auto pool_factory = create_ofdm_demodulator_pool_factory(base_factory, 1);
  ASSERT_NE(pool_factory, nullptr);

  ofdm_demodulator_configuration config = {
      .numerology = 2, .bw_rb = bw_rb, .dft_size = 128, .cp = cp, .nof_samples_window_offset = 0, .scale = 1.0F};
  std::unique_ptr<ofdm_symbol_demodulator> demodulator = pool_factory->create_ofdm_symbol_demodulator(config);
  ASSERT_NE(demodulator, nullptr);

  resource_grid_writer_spy grid(nof_ports, nof_symbols, bw_rb);

  // Symbol indices of the first two slots of the subframe.
  for (unsigned symbol_index_sf = 0; symbol_index_sf != 2 * nof_symbols_per_slot; ++symbol_index_sf) {
    fill_grid(grid);

    unsigned            i_port = symbol_index_sf % nof_ports;
    std::vector<ci16_t> input(demodulator->get_symbol_size(symbol_index_sf));

    // Demodulate while the only instance is busy.
    base_factory->hook = [&]() { demodulator->demodulate(grid, input, i_port, symbol_index_sf); };
    demodulator->demodulate(grid, input, (i_port + 1) % nof_ports, 0);
    base_factory->hook = nullptr;

    unsigned expected_symbol = symbol_index_sf % nof_symbols_per_slot;
    for (unsigned p = 0; p != nof_ports; ++p) {
      for (unsigned l = 0; l != nof_symbols; ++l) {
        bool expect_zero = (p == i_port) && (l == expected_symbol);
        ASSERT_EQ(is_zero(grid, p, l), expect_zero)
            << "symbol_index_sf=" << symbol_index_sf << " port=" << p << " symbol=" << l;
      }
    }
  }
}

INSTANTIATE_TEST_SUITE_P(OfdmDemodulatorPool,
                         OfdmDemodulatorPoolFixture,
                         ::testing::Values(cyclic_prefix::NORMAL, cyclic_prefix::EXTENDED),
                         [](const ::testing::TestParamInfo<cyclic_prefix>& param_info) {
                           return param_info.param.to_string();
                         });
