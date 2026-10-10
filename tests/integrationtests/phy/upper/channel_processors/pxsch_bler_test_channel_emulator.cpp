// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "pxsch_bler_test_channel_emulator.h"
#include "ocudu/adt/format.h"
#include "ocudu/adt/span.h"
#include "ocudu/ocuduvec/add.h"
#include "ocudu/ocuduvec/prod.h"
#include "ocudu/ocuduvec/sc_prod.h"
#include "ocudu/ocuduvec/zero.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include "ocudu/phy/support/resource_grid_writer.h"
#include "ocudu/support/executors/task_executor.h"
#include "ocudu/support/synchronization/sync_event.h"
#include <algorithm>
#include <cmath>
#include <set>
#include <thread>

using namespace ocudu;

unsigned channel_emulator::concurrent_channel_emulator::seed = 0;

/// \brief TDP channel tap.
///
/// The first element is the tap delay in nanoseconds, and the second element is the tap amplitude in dB.
using tdl_tap = std::pair<unsigned, float>;

/// Single-tap profile.
static constexpr std::array<tdl_tap, 1> taps_single = {{{0, 0.0}}};

/// TDLA fading profile.
static constexpr std::array<tdl_tap, 12> taps_tdla = {{{0, -15.5},
                                                       {10, 0.0},
                                                       {15, -5.1},
                                                       {20, -5.1},
                                                       {25, -9.6},
                                                       {50, -8.2},
                                                       {65, -13.1},
                                                       {75, -11.5},
                                                       {105, -11.0},
                                                       {135, -16.2},
                                                       {150, -16.6},
                                                       {290, -26.2}}};

/// TDLB fading profile.
static constexpr std::array<tdl_tap, 12> taps_tdlb = {{{0, 0},
                                                       {10, -2.2},
                                                       {20, -0.6},
                                                       {30, -0.6},
                                                       {35, -0.3},
                                                       {45, -1.2},
                                                       {55, -5.9},
                                                       {120, -2.2},
                                                       {170, -0.8},
                                                       {245, -6.3},
                                                       {330, -7.5},
                                                       {480, -7.1}}};

/// TDLC fading profile.
static constexpr std::array<tdl_tap, 12> taps_tdlc = {{{0, -6.9},
                                                       {65, 0},
                                                       {70, -7.7},
                                                       {190, -2.5},
                                                       {195, -2.4},
                                                       {200, -9.9},
                                                       {240, -8.0},
                                                       {325, -6.6},
                                                       {520, -7.1},
                                                       {1045, -13.0},
                                                       {1510, -14.2},
                                                       {2595, -16.0}}};

/// \brief Gets the MIMO correlation parameters as per TS38.104 Table G.2.3.1.2-1.
///
/// Alpha is the correlation between the receive antennas at the gNB, and beta is the correlation between the transmit
/// antennas at the UE. Each parameter is the correlation between the two outermost antennas of its side. The
/// correlation between antennas \c i and \c j of an \f$N\f$-antenna side is \f$\rho^{((i-j)/(N-1))^2}\f$, with
/// \f$\rho\f$ equal to alpha or beta, as per TS38.104 Tables G.2.3.1.1-1 and G.2.3.1.1-2. Hence, zero means
/// uncorrelated antennas and one means fully correlated antennas.
///
/// \return A pair with the parameters alpha and beta.
static std::pair<float, float> get_correlation_parameters(const std::string& mimo_correlation)
{
  if (mimo_correlation == "low") {
    return {0.0F, 0.0F};
  }
  if (mimo_correlation == "medium") {
    return {0.9F, 0.3F};
  }
  if (mimo_correlation == "high") {
    return {0.9F, 0.9F};
  }
  report_fatal_error("Invalid MIMO correlation '{}'. Valid levels are low, medium, and high.", mimo_correlation);
}

/// \brief Returns the scaling factor that keeps the spatial correlation matrix positive semi-definite.
///
/// The values are given in TS38.104 Tables G.2.3.1.2-2 and G.2.3.1.2-3. Configurations without a listed value use
/// zero.
static float get_correlation_scaling(const std::string& mimo_correlation, unsigned nof_ue_ports, unsigned nof_gnb_ports)
{
  if ((mimo_correlation == "high") && (nof_ue_ports == 2) && (nof_gnb_ports == 4)) {
    return 0.00010F;
  }
  if ((mimo_correlation != "low") && (nof_ue_ports == 4) && (nof_gnb_ports == 4)) {
    return 0.00012F;
  }
  return 0.0F;
}

/// \brief Returns an element of a single-side correlation matrix as per TS38.104 Tables G.2.3.1.1-1 and G.2.3.1.1-2.
///
/// The element of row \c i and column \c j of an \f$N\f$-port matrix is \f$\rho^{((i-j)/(N-1))^2}\f$.
static float get_correlation_element(float rho, unsigned i, unsigned j, unsigned nof_ports)
{
  if (i == j) {
    return 1.0F;
  }
  float distance = static_cast<float>(std::max(i, j) - std::min(i, j)) / static_cast<float>(nof_ports - 1);
  return std::pow(rho, distance * distance);
}

/// \brief Returns the lower triangular Cholesky factor of a symmetric positive semi-definite matrix.
///
/// Both matrices are in row-major order. A column with a non-positive pivot stays zero, which handles singular
/// matrices. The helper applies Cholesky-Crout algorithm.
static std::vector<float> cholesky(span<const float> matrix, unsigned size)
{
  std::vector<float> lower(size * size, 0.0F);
  for (unsigned j = 0; j != size; ++j) {
    double pivot = matrix[j * size + j];
    for (unsigned k = 0; k != j; ++k) {
      pivot -= static_cast<double>(lower[j * size + k]) * lower[j * size + k];
    }
    if (pivot <= 1e-9) {
      continue;
    }
    float diag          = static_cast<float>(std::sqrt(pivot));
    lower[j * size + j] = diag;
    for (unsigned i = j + 1; i != size; ++i) {
      double sum = matrix[i * size + j];
      for (unsigned k = 0; k != j; ++k) {
        sum -= static_cast<double>(lower[i * size + k]) * lower[j * size + k];
      }
      lower[i * size + j] = static_cast<float>(sum) / diag;
    }
  }
  return lower;
}

channel_emulator::doppler_process::doppler_process(double doppler_Hz, std::mt19937& rgen)
{
  std::uniform_real_distribution<double> dist_phase(-M_PI, M_PI);
  for (unsigned i = 0; i != nof_sinusoids; ++i) {
    // Spreading the arrival angles uniformly over the circle with a random offset makes the frequencies follow the
    // classical Doppler spectrum.
    double angle = (2.0 * M_PI * i + dist_phase(rgen)) / nof_sinusoids;
    freq_Hz[i]   = doppler_Hz * std::cos(angle);
    phase_rad[i] = dist_phase(rgen);
  }
}

cf_t channel_emulator::doppler_process::evaluate(double time_s) const
{
  // Wrap the phase in double precision to keep the accuracy over long simulation times.
  cf_t sum = 0;
  for (unsigned i = 0; i != nof_sinusoids; ++i) {
    double phase = std::fmod(2.0 * M_PI * freq_Hz[i] * time_s + phase_rad[i], 2.0 * M_PI);
    sum += std::polar(1.0F, static_cast<float>(phase));
  }
  return sum / std::sqrt(static_cast<float>(nof_sinusoids));
}

channel_emulator::channel_emulator(std::string        delay_profile,
                                   std::string        fading_distribution_,
                                   float              sinr_dB,
                                   float              cfo_Hz,
                                   float              doppler_Hz,
                                   std::string        mimo_correlation,
                                   unsigned           nof_corrupted_re_per_symbol,
                                   unsigned           nof_tx_ports_,
                                   unsigned           nof_rx_ports_,
                                   unsigned           nof_subc,
                                   unsigned           nof_symbols,
                                   unsigned           max_nof_threads,
                                   subcarrier_spacing scs,
                                   task_executor&     executor_) :
  nof_ofdm_symbols(nof_symbols),
  nof_tx_ports(nof_tx_ports_),
  nof_rx_ports(nof_rx_ports_),
  temp_uncorrelated(nof_tx_ports_ * nof_rx_ports_),
  emulators(max_nof_threads, sinr_dB, nof_corrupted_re_per_symbol, nof_subc),
  executor(executor_)
{
  // Select fading channel taps.
  span<const tdl_tap> taps;
  if (delay_profile == "single-tap") {
    taps = taps_single;
  } else if (delay_profile == "TDLA") {
    taps = taps_tdla;
  } else if (delay_profile == "TDLB") {
    taps = taps_tdlb;
  } else if (delay_profile == "TDLC") {
    taps = taps_tdlc;
  }
  report_fatal_error_if_not(!taps.empty(),
                            "Invalid delay profile '{}'. Valid delay profiles are single-tap, TDLA, TDLB, and TDLC.",
                            delay_profile);

  if (fading_distribution_ == "rayleigh") {
    fading_distribution = rayleigh;
  } else if (fading_distribution_ == "uniform-phase") {
    fading_distribution = uniform_phase;
  } else if (fading_distribution_ == "butler") {
    fading_distribution = butler;
  }
  report_fatal_error_if_not(
      fading_distribution != invalid_distribution,
      "Invalid fading distribution '{}'. Valid distributions are rayleigh, uniform-phase, and butler.",
      fading_distribution_);

  auto [alpha, beta] = get_correlation_parameters(mimo_correlation);
  report_fatal_error_if_not(doppler_Hz >= 0, "Invalid Doppler frequency {} Hz.", doppler_Hz);
  report_fatal_error_if_not((fading_distribution == rayleigh) || ((doppler_Hz == 0) && (mimo_correlation == "low")),
                            "Doppler and MIMO correlation require the rayleigh fading distribution.");

  // Estimate taps power.
  float taps_power = std::accumulate(taps.begin(), taps.end(), 0.0F, [](float acc, const tdl_tap& tap) {
    return acc + convert_dB_to_power(tap.second);
  });

  // Calculate power normalization coefficient.
  float norm_coefficient = 1.0F / std::sqrt(nof_rx_ports * taps_power);

  // Generate frequency response of each tap.
  taps_channel_response.resize({nof_subc, static_cast<unsigned>(taps.size())});
  for (unsigned i_tap = 0; i_tap != taps.size(); ++i_tap) {
    // Extract tap delay in seconds.
    float delay_s = taps[i_tap].first * 1e-9;

    // Extract tap linear average amplitude.
    float amplitude = norm_coefficient * convert_dB_to_amplitude(taps[i_tap].second);

    // Calculate the subcarrier phase shift.
    float subcarrier_phase_shift = delay_s * static_cast<float>(scs_to_khz(scs) * 1000);

    // Generate frequency response for the tap.
    span<cf_t> freq_response_tap = taps_channel_response.get_view({i_tap});
    std::generate(
        freq_response_tap.begin(), freq_response_tap.end(), [n = 0, amplitude, subcarrier_phase_shift]() mutable {
          return std::polar(amplitude, -TWOPI * static_cast<float>(n++) * subcarrier_phase_shift);
        });
  }

  tap_channel_coeffs.resize({static_cast<unsigned>(taps.size()), nof_tx_ports, nof_rx_ports, nof_ofdm_symbols});

  // Generate CFO coefficients and symbol times.
  double symbol_duration_s   = 1.0 / (1000.0 * scs_to_khz(scs));
  double symbol_start_time_s = 0.0;
  for (unsigned i_symb = 0; i_symb != MAX_NSYMB_PER_SLOT; ++i_symb) {
    // Calculate cyclic prefix duration and add it to the start time.
    symbol_start_time_s += cyclic_prefix(cyclic_prefix::NORMAL).get_length(i_symb, scs).to_seconds();

    // Calculate CFO coefficient for the OFDM symbol.
    cfo_coeffs[i_symb] = std::polar(1.0, TWOPI * symbol_start_time_s * cfo_Hz);

    symbol_times_s[i_symb] = symbol_start_time_s + symbol_duration_s / 2.0;

    // Advance symbol duration.
    symbol_start_time_s += symbol_duration_s;
  }
  slot_duration_s = symbol_start_time_s;

  // Create a fading process for each tap and link.
  if (doppler_Hz > 0) {
    unsigned nof_processes = taps.size() * nof_tx_ports * nof_rx_ports;
    doppler_processes.reserve(nof_processes);
    for (unsigned i = 0; i != nof_processes; ++i) {
      doppler_processes.emplace_back(doppler_Hz, rgen);
    }
  }

  // Compute the square root of the spatial correlation matrix R_UE ⊗ R_gNB as per TS38.104 Section G.2.3.1.1. The
  // receive port index runs fastest.
  if (mimo_correlation != "low") {
    unsigned           nof_links = nof_tx_ports * nof_rx_ports;
    float              a         = get_correlation_scaling(mimo_correlation, nof_tx_ports, nof_rx_ports);
    std::vector<float> correlation(nof_links * nof_links);
    for (unsigned i_row = 0; i_row != nof_links; ++i_row) {
      for (unsigned i_col = 0; i_col != nof_links; ++i_col) {
        float r_ue  = get_correlation_element(beta, i_row / nof_rx_ports, i_col / nof_rx_ports, nof_tx_ports);
        float r_gnb = get_correlation_element(alpha, i_row % nof_rx_ports, i_col % nof_rx_ports, nof_rx_ports);
        correlation[i_row * nof_links + i_col] = (r_ue * r_gnb + ((i_row == i_col) ? a : 0.0F)) / (1.0F + a);
      }
    }
    correlation_sqrt = cholesky(correlation, nof_links);
  }
}

void channel_emulator::generate_uncorrelated(span<cf_t> coeffs, unsigned i_tap, unsigned i_symbol)
{
  // Sample the fading processes at the middle of the OFDM symbol.
  if (!doppler_processes.empty()) {
    double time_s = slot_start_time_s + symbol_times_s[i_symbol];
    for (unsigned i_link = 0, nof_links = coeffs.size(); i_link != nof_links; ++i_link) {
      coeffs[i_link] = doppler_processes[i_tap * nof_links + i_link].evaluate(time_s);
    }
    return;
  }

  for (unsigned i_tx_port = 0; i_tx_port != nof_tx_ports; ++i_tx_port) {
    for (unsigned i_rx_port = 0; i_rx_port != nof_rx_ports; ++i_rx_port) {
      cf_t& coeff = coeffs[i_tx_port * nof_rx_ports + i_rx_port];
      switch (fading_distribution) {
        case rayleigh:
          coeff = dist_rayleigh(rgen);
          break;
        case uniform_phase:
          coeff = std::polar(1.0F, dist_uniform_phase(rgen));
          break;
        case butler:
          coeff = std::polar(1.0F, static_cast<float>(2.0F * M_PI * i_rx_port * i_tx_port / nof_tx_ports));
          break;
        case invalid_distribution:
          coeff = std::numeric_limits<float>::quiet_NaN();
          break;
      }
    }
  }
}

void channel_emulator::generate_tap_coefficients()
{
  unsigned nof_taps  = tap_channel_coeffs.get_dimension_size(0);
  unsigned nof_links = nof_tx_ports * nof_rx_ports;

  for (unsigned i_symbol = 0; i_symbol != nof_ofdm_symbols; ++i_symbol) {
    for (unsigned i_tap = 0; i_tap != nof_taps; ++i_tap) {
      // Without Doppler, the channel is constant within the slot.
      if (doppler_processes.empty() && (i_symbol != 0)) {
        for (unsigned i_tx = 0; i_tx != nof_tx_ports; ++i_tx) {
          for (unsigned i_rx = 0; i_rx != nof_rx_ports; ++i_rx) {
            tap_channel_coeffs[{i_tap, i_tx, i_rx, i_symbol}] = tap_channel_coeffs[{i_tap, i_tx, i_rx, 0}];
          }
        }
        continue;
      }

      generate_uncorrelated(temp_uncorrelated, i_tap, i_symbol);

      for (unsigned i_link = 0; i_link != nof_links; ++i_link) {
        cf_t coeff = temp_uncorrelated[i_link];

        // Apply the spatial correlation.
        if (!correlation_sqrt.empty()) {
          coeff = 0;
          for (unsigned k = 0; k <= i_link; ++k) {
            coeff += correlation_sqrt[i_link * nof_links + k] * temp_uncorrelated[k];
          }
        }

        tap_channel_coeffs[{i_tap, i_link / nof_rx_ports, i_link % nof_rx_ports, i_symbol}] = coeff;
      }
    }
  }
}

void channel_emulator::run(resource_grid_writer& rx_grid, const resource_grid_reader& tx_grid, uint64_t i_slot)
{
  slot_start_time_s = static_cast<double>(i_slot) * slot_duration_s;
  generate_tap_coefficients();

  // Channel emulator.
  sync_event sync;
  for (unsigned i_rx_port = 0; i_rx_port != nof_rx_ports; ++i_rx_port) {
    for (unsigned i_symbol = 0; i_symbol != nof_ofdm_symbols; ++i_symbol) {
      bool success = executor.execute([this, &rx_grid, &tx_grid, i_rx_port, i_symbol, token = sync.get_token()]() {
        auto emulator = emulators.get();
        report_fatal_error_if_not(emulator, "Failed to retrieve channel emulator.");
        emulator->run(
            rx_grid, tx_grid, taps_channel_response, tap_channel_coeffs, cfo_coeffs[i_symbol], i_rx_port, i_symbol);
      });
      report_fatal_error_if_not(success, "Failed to enqueue concurrent channel emulate.");
    }
  }

  // Wait for channel emulator to finish.
  sync.wait();
}

void channel_emulator::concurrent_channel_emulator::run(resource_grid_writer&       rx_grid,
                                                        const resource_grid_reader& tx_grid,
                                                        const tensor<2, cf_t>&      taps_response,
                                                        const tensor<4, cf_t>&      tap_coeffs,
                                                        cf_t                        time_coeff,
                                                        unsigned                    i_rx_port,
                                                        unsigned                    i_symbol)
{
  using namespace std::complex_literals;
  unsigned nof_taps = tap_coeffs.get_dimension_size(0);
  unsigned nof_tx   = tap_coeffs.get_dimension_size(1);

  // For each transmit port.
  for (unsigned i_tx = 0; i_tx != nof_tx; ++i_tx) {
    // Generate the frequency domain channel response of the link for the OFDM symbol.
    for (unsigned i_tap = 0; i_tap != nof_taps; ++i_tap) {
      // Write the first tap directly into the channel response and skip the accumulation.
      span<cf_t> tap_channel = temp_tap_channel;
      if (i_tap == 0) {
        tap_channel = temp_channel;
      }

      ocuduvec::sc_prod(tap_channel, taps_response.get_view({i_tap}), tap_coeffs[{i_tap, i_tx, i_rx_port, i_symbol}]);

      if (i_tap != 0) {
        ocuduvec::add(temp_channel, tap_channel, temp_channel);
      }
    }

    // Select temporary buffer.
    span<cf_t> single_ofdm_symbol = temp_single_ofdm_symbol;
    if (i_tx == 0) {
      single_ofdm_symbol = temp_ofdm_symbol;
    }

    // Get OFDM symbol.
    tx_grid.get(single_ofdm_symbol, i_tx, i_symbol, 0);

    // Apply frequency domain fading channel.
    ocuduvec::prod(single_ofdm_symbol, temp_channel, single_ofdm_symbol);

    // Skip accumulating for the first transmit port.
    if (i_tx != 0) {
      ocuduvec::add(temp_ofdm_symbol, single_ofdm_symbol, temp_ofdm_symbol);
    }
  }

  // Apply time-domain coefficient.
  ocuduvec::sc_prod(temp_ofdm_symbol, temp_ofdm_symbol, time_coeff);

  // Apply AWGN.
  std::generate(temp_single_ofdm_symbol.begin(), temp_single_ofdm_symbol.end(), [this]() { return dist_awgn(rgen); });
  ocuduvec::add(temp_ofdm_symbol, temp_single_ofdm_symbol, temp_ofdm_symbol);

  // Corrupt REs.
  std::set<unsigned> corrupted_i_subc;
  for (unsigned i = 0; i != nof_corrupted_re; ++i) {
    // Select a subcarrier that has not been corruped before.
    unsigned i_subc;
    do {
      i_subc = dist_corrupted_re(rgen);
    } while (corrupted_i_subc.count(i_subc) != 0);
    corrupted_i_subc.emplace(i_subc);

    temp_ofdm_symbol[i_subc] *= 1i;
  }

  // Write the OFDM symbol back to the grid.
  rx_grid.put(i_rx_port, i_symbol, 0, temp_ofdm_symbol);
}
