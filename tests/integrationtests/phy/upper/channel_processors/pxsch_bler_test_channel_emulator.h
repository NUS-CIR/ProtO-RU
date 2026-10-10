// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/complex.h"
#include "ocudu/adt/span.h"
#include "ocudu/adt/tensor.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include "ocudu/support/math/complex_normal_random.h"
#include "ocudu/support/memory_pool/bounded_object_pool.h"
#include <random>
#include <string>

namespace ocudu {
class resource_grid_reader;
class resource_grid_writer;
class task_executor;

/// \brief Implements a frequency domain channel emulator.
///
/// The emulator applies the tapped delay line (TDL) models of TS38.104 Annex G.2. Each tap applies an exact phase ramp
/// in the frequency domain, so the delays are not quantized to the sampling rate.
///
/// The tap coefficients depend on the fading distribution and the maximum Doppler frequency:
/// - Rayleigh fading with a non-zero maximum Doppler frequency follows the classical Doppler spectrum of TS38.104
/// Annex
///   G.2.2. The coefficients evolve continuously across OFDM symbols and slots. A sum of sinusoids generates the fading
///   process, and each OFDM symbol samples it at the middle of its useful part.
/// - Rayleigh fading with a zero maximum Doppler frequency draws uncorrelated coefficients on every call to run(). The
///   coefficients stay constant within the slot.
/// - Uniform-phase and Butler distributions do not support Doppler nor spatial correlation.
///
/// Rayleigh coefficients follow the MIMO spatial correlation of TS38.104 Annex G.2.3.1, with the transmit ports at the
/// UE and the receive ports at the gNB.
///
/// The slot index given to run() sets the channel time, so the caller chooses the time between transmissions.
///
/// The channel coefficients stay constant within each OFDM symbol. The emulator ignores the inter-carrier interference
/// caused by Doppler, which is negligible when the Doppler frequency is much smaller than the subcarrier spacing.
///
/// run() is not thread safe. It deterministically selects the tap coefficients. However, the noise addition is
/// performed asynchronously, which can lead to different results across executions.
class channel_emulator
{
public:
  /// \brief Creates a channel emulator.
  ///
  /// \param[in] delay_profile               Delay profile - The valid profiles are TDLA, TDLB, TDLC, and single-tap.
  /// \param[in] fading_distribution         Fading distribution - The valid distributions are rayleigh and
  ///                                        uniform-phase.
  /// \param[in] sinr_dB                     Signal-to-Interference-plus-Noise Ratio.
  /// \param[in] cfo_Hz                      Carrier frequency offset in Herz.
  /// \param[in] doppler_Hz                  Maximum Doppler frequency in Herz.
  /// \param[in] mimo_correlation            MIMO correlation level - The valid levels are low, medium, and high.
  /// \param[in] nof_corrupted_re_per_symbol Number of corrupted RE per OFDM symbol. Set to zero for no corrupted RE.
  /// \param[in] nof_tx_ports                Number of transmit ports.
  /// \param[in] nof_rx_ports                Number of receive ports.
  /// \param[in] nof_subc                    Number of resource grid subcarriers.
  /// \param[in] nof_symbols                 Number of OFDM symbols per slot.
  /// \param[in] max_nof_threads             Maximum number of threads used by the asynchronous executor.
  /// \param[in] scs                         Resource grid subcarrier spacing.
  /// \param[in] executor_                   Asynchronous task execution.
  channel_emulator(std::string        delay_profile,
                   std::string        fading_distribution,
                   float              sinr_dB,
                   float              cfo_Hz,
                   float              doppler_Hz,
                   std::string        mimo_correlation,
                   unsigned           nof_corrupted_re_per_symbol,
                   unsigned           nof_tx_ports,
                   unsigned           nof_rx_ports,
                   unsigned           nof_subc,
                   unsigned           nof_symbols,
                   unsigned           max_nof_threads,
                   subcarrier_spacing scs,
                   task_executor&     executor_);

  /// \brief Runs the channel emulator.
  ///
  /// \param[out] rx_grid Receive resource grid.
  /// \param[in]  tx_grid Transmit resource grid.
  /// \param[in]  i_slot  Slot index, which sets the channel time.
  void run(resource_grid_writer& rx_grid, const resource_grid_reader& tx_grid, uint64_t i_slot);

private:
  /// Internal concurrent channel emulator.
  class concurrent_channel_emulator
  {
  public:
    concurrent_channel_emulator(float sinr_dB, unsigned nof_corrupted_re_, unsigned nof_subc) :
      rgen(seed++),
      dist_awgn(cf_t(), convert_dB_to_amplitude(-sinr_dB)),
      dist_corrupted_re(0, nof_subc - 1),
      nof_corrupted_re(nof_corrupted_re_),
      temp_ofdm_symbol(nof_subc),
      temp_single_ofdm_symbol(nof_subc),
      temp_channel(nof_subc),
      temp_tap_channel(nof_subc)
    {
    }

    /// \brief Runs the channel emulator on a receive port and OFDM symbol index for the given resource grids.
    /// \param[out] rx_grid       Received resource grid.
    /// \param[in]  tx_grid       Transmit resource grid.
    /// \param[in]  taps_response Frequency domain response of each tap.
    /// \param[in]  tap_coeffs    Tap coefficients for each transmit port, receive port and OFDM symbol.
    /// \param[in]  time_coeff    Time domain coefficient.
    /// \param[in]  i_port        Receive port index.
    /// \param[in]  i_symbol      OFDM symbol index within the slot.
    void run(resource_grid_writer&       rx_grid,
             const resource_grid_reader& tx_grid,
             const tensor<2, cf_t>&      taps_response,
             const tensor<4, cf_t>&      tap_coeffs,
             cf_t                        time_coeff,
             unsigned                    i_port,
             unsigned                    i_symbol);

  private:
    /// Seed for the random generator (each instance gets a different initialization).
    static unsigned seed;
    /// Random generator.
    std::mt19937 rgen;
    /// Additive white gaussian noise distribution.
    complex_normal_distribution<cf_t> dist_awgn;
    /// Distribution for simulating corrupted RE.
    std::uniform_int_distribution<unsigned> dist_corrupted_re;
    /// Number of corrupted RE per OFDM symbol.
    unsigned nof_corrupted_re;
    /// Temporary OFDM frequency domain symbol.
    std::vector<cf_t> temp_ofdm_symbol;
    /// Temporary single OFDM frequency domain symbol.
    std::vector<cf_t> temp_single_ofdm_symbol;
    /// Temporary frequency domain channel response.
    std::vector<cf_t> temp_channel;
    /// Temporary frequency domain response of a single tap.
    std::vector<cf_t> temp_tap_channel;
  };

  /// Rayleigh fading process with the classical Doppler spectrum, generated as a sum of sinusoids.
  class doppler_process
  {
  public:
    /// Creates a fading process with random arrival angles and phases.
    doppler_process(double doppler_Hz, std::mt19937& rgen);

    /// Returns the fading coefficient at the given time in seconds.
    cf_t evaluate(double time_s) const;

  private:
    /// [Implementation-defined] Number of sinusoids.
    static constexpr unsigned nof_sinusoids = 32;
    /// Frequency of each sinusoid in Herz.
    std::array<double, nof_sinusoids> freq_Hz;
    /// Initial phase of each sinusoid in radians.
    std::array<double, nof_sinusoids> phase_rad;
  };

  /// Generates the uncorrelated Rayleigh coefficients of all links for one tap and OFDM symbol.
  void generate_uncorrelated(span<cf_t> coeffs, unsigned i_tap, unsigned i_symbol);

  /// Generates the tap coefficients of all links, taps and OFDM symbols for the current slot.
  void generate_tap_coefficients();

  enum { invalid_distribution, rayleigh, uniform_phase, butler } fading_distribution = invalid_distribution;
  /// Number of OFDM symbols per slot.
  unsigned nof_ofdm_symbols;
  /// Random generator for taps phase and power.
  std::mt19937 rgen;
  /// Random distribution for uncorrelated tap weights.
  complex_normal_distribution<cf_t> dist_rayleigh;
  /// Uniform real distribution for phase.
  std::uniform_real_distribution<float> dist_uniform_phase = std::uniform_real_distribution<float>(-M_PI, M_PI);
  /// CFO coefficients.
  std::array<cf_t, MAX_NSYMB_PER_SLOT> cfo_coeffs;
  /// Time of the middle of each OFDM symbol useful part, relative to the slot start.
  std::array<double, MAX_NSYMB_PER_SLOT> symbol_times_s;
  /// Slot duration in seconds.
  double slot_duration_s = 0.0;
  /// Start time of the emulated slot in seconds.
  double slot_start_time_s = 0.0;
  /// Number of transmit ports.
  unsigned nof_tx_ports;
  /// Number of receive ports.
  unsigned nof_rx_ports;
  /// Fading processes, indexed by tap, transmit port and receive port. Empty if the Doppler is disabled.
  std::vector<doppler_process> doppler_processes;
  /// Lower triangular square root of the spatial correlation matrix in row-major order. Empty if uncorrelated.
  std::vector<float> correlation_sqrt;
  /// Temporary uncorrelated coefficients of all links.
  std::vector<cf_t> temp_uncorrelated;
  /// Tap coefficients, indexed by tap, transmit port, receive port and OFDM symbol.
  dynamic_tensor<4, cf_t> tap_channel_coeffs;
  /// Frequency response of each of the channel taps.
  dynamic_tensor<2, cf_t> taps_channel_response;
  /// Concurrent channel emulators.
  bounded_object_pool<concurrent_channel_emulator> emulators;
  /// Asynchronous executor.
  task_executor& executor;
};

} // namespace ocudu
