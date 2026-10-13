// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ru_sdr_config_validator.h"
#include "ru_sdr_config_helpers.h"
#include "ocudu/adt/format.h"
#include "ocudu/adt/interval.h"
#include <sstream>
#include <string_view>

using namespace ocudu;

/// Returns \p text without its leading and trailing whitespace.
static std::string trim_whitespace(const std::string& text)
{
  static constexpr std::string_view whitespace = " \t\n";

  const std::size_t first = text.find_first_not_of(whitespace);
  if (first == std::string::npos) {
    return {};
  }
  return text.substr(first, text.find_last_not_of(whitespace) - first + 1);
}

/// Returns true if \p key starts with \p prefix.
static bool starts_with(const std::string& key, std::string_view prefix)
{
  return key.compare(0, prefix.size(), prefix) == 0;
}

/// Validates the DIFI device arguments against the antennas of \p cell_config. Returns true on success, otherwise
/// false.
static bool validate_difi_device_arguments(const std::string&                        device_arguments,
                                           span<const ru_sdr_cell_validation_config> cell_config)
{
  static constexpr std::string_view iq_byte_order_key = "iq_byte_order";
  static constexpr std::string_view tx_addr_key       = "tx_addr";
  static constexpr std::string_view rx_addr_key       = "rx_addr";
  static constexpr std::string_view tx_port_key       = "tx_port";
  static constexpr std::string_view rx_port_key       = "rx_port";

  unsigned nof_tx_addr = 0;
  unsigned nof_rx_addr = 0;
  unsigned nof_tx_port = 0;
  unsigned nof_rx_port = 0;

  std::istringstream stream(device_arguments);
  std::string        argument;
  while (std::getline(stream, argument, ',')) {
    const std::size_t equal = argument.find('=');
    if (equal == std::string::npos) {
      continue;
    }

    // Device arguments may be folded across lines in the configuration file, so surrounding whitespace is ignored.
    const std::string key = trim_whitespace(argument.substr(0, equal));

    // Keys match by prefix, since each port takes an indexed key, as in tx_port0 and tx_port1, and the addresses
    // accept the same spelling.
    if (starts_with(key, tx_addr_key)) {
      ++nof_tx_addr;
      continue;
    }
    if (starts_with(key, rx_addr_key)) {
      ++nof_rx_addr;
      continue;
    }
    if (starts_with(key, tx_port_key)) {
      ++nof_tx_port;
      continue;
    }
    if (starts_with(key, rx_port_key)) {
      ++nof_rx_port;
      continue;
    }

    if (key != iq_byte_order_key) {
      continue;
    }

    const std::string value = trim_whitespace(argument.substr(equal + 1));
    if ((value != "big") && (value != "little")) {
      fmt::print("Invalid DIFI {} '{}'. Valid values are 'big' and 'little'.\n", iq_byte_order_key, value);
      return false;
    }
  }

  // Every port of the radio shares one address per direction and differs only by UDP port, so a second address
  // would be silently ignored.
  if (nof_tx_addr > 1) {
    fmt::print("DIFI takes at most one {} for all ports, but {} were given.\n", tx_addr_key, nof_tx_addr);
    return false;
  }
  if (nof_rx_addr > 1) {
    fmt::print("DIFI takes at most one {} for all ports, but {} were given.\n", rx_addr_key, nof_rx_addr);
    return false;
  }

  // DIFI carries every antenna as a stream of its own, numbered by cell and then by antenna. Without a port list,
  // a single antenna per direction takes the driver default port.
  unsigned nof_tx_antennas = 0;
  unsigned nof_rx_antennas = 0;
  for (const ru_sdr_cell_validation_config& cell : cell_config) {
    nof_tx_antennas += cell.nof_antennas_dl;
    nof_rx_antennas += cell.nof_antennas_ul;
  }
  if (((nof_tx_port != 0) || (nof_tx_antennas > 1)) && (nof_tx_port != nof_tx_antennas)) {
    fmt::print("DIFI takes one {} per transmit antenna, i.e., {}, but {} were given.\n",
               tx_port_key,
               nof_tx_antennas,
               nof_tx_port);
    return false;
  }
  if (((nof_rx_port != 0) || (nof_rx_antennas > 1)) && (nof_rx_port != nof_rx_antennas)) {
    fmt::print("DIFI takes one {} per receive antenna, i.e., {}, but {} were given.\n",
               rx_port_key,
               nof_rx_antennas,
               nof_rx_port);
    return false;
  }

  return true;
}

/// Validates the given amplitude control application configuration. Returns true on success, otherwise false.
static bool validate_amplitude_control_unit_config(const amplitude_control_unit_config& config)
{
  bool valid = true;

  if (config.gain_backoff_dB < 0.0F) {
    fmt::print("Invalid gain backoff (i.e., {:.1f} dB). Must be positive or zero.\n", config.gain_backoff_dB);
    valid = false;
  }

  return valid;
}

/// Validates the given SDR Radio Unit application configuration. Returns true on success, otherwise false.
static bool validate_ru_sdr_appconfig(const ru_sdr_unit_config&                 config,
                                      span<const ru_sdr_cell_validation_config> cell_config)
{
  if (config.expert_execution_cfg.cell_affinities.size() != cell_config.size()) {
    fmt::println("Using different number of cells for SDR expert execution '{}' than the number of defined cells '{}'",
                 config.expert_execution_cfg.cell_affinities.size(),
                 cell_config.size());
    return false;
  }

  if (!is_srate_sample_accurate(config.srate_MHz)) {
    fmt::print("The sampling rate must be multiple of {:.2f} MHz.\n", 1e-6 / ru_sdr_srate_reference_time.to_seconds());
    return false;
  }

  const bool discontinuous_transmission = (config.expert_cfg.transmission_mode != "continuous");
  for (const auto& cell : cell_config) {
    if ((config.expert_cfg.transmission_mode == "same-port") && (cell.dplx_mode == duplex_mode::FDD)) {
      fmt::print("same-port transmission mode cannot be used with FDD cell configurations.\n");
      return false;
    }

    // Validates the sampling rate is compatible with the PRACH sequence.
    if (!is_srate_prach_sample_accurate(config.srate_MHz, cell)) {
      std::vector<double> valid_sampling_rates;
      for (double sampling_rate : ru_sdr_standard_srates_MHz) {
        // A valid alternative sampling rate must cover the cell bandwidth and be sample accurate with the preamble
        // cyclic prefix.
        if (does_srate_cover_bandwidth(sampling_rate, cell) && is_srate_prach_sample_accurate(sampling_rate, cell)) {
          valid_sampling_rates.push_back(sampling_rate);
        }
      }

      fmt::print("PRACH Format {} with subcarrier spacing of {} is not compatible with {:.2f}MHz sampling rate. "
                 "Valid sampling rates are {} MHz.\n",
                 to_string(cell.prach_format),
                 to_string(cell.common_scs),
                 config.srate_MHz,
                 span<const double>(valid_sampling_rates));
      return false;
    }

    static constexpr float subframe_duration_us = 1e3;
    float slot_duration_us = subframe_duration_us / static_cast<float>(get_nof_slots_per_subframe(cell.common_scs));
    if (discontinuous_transmission && (config.expert_cfg.power_ramping_time_us > 2 * slot_duration_us)) {
      fmt::print("Power ramping time, i.e., {:.1f} us, cannot exceed the duration of two NR slots, i.e., {:.1f} us.\n",
                 config.expert_cfg.power_ramping_time_us,
                 2 * slot_duration_us);
      return false;
    }
  }

  static constexpr interval<float> lphy_dl_throttling_range(0.0F, 20.0F);
  if (!lphy_dl_throttling_range.contains(config.expert_cfg.lphy_dl_throttling)) {
    fmt::print("Lower PHY throttling (i.e., {}) must be in range {}.\n",
               config.expert_cfg.lphy_dl_throttling,
               lphy_dl_throttling_range);
    return false;
  }

  if (discontinuous_transmission && (config.device_driver == "zmq")) {
    fmt::print("Discontinuous transmission modes cannot be used with ZMQ.\n");
    return false;
  }

  if (discontinuous_transmission && (config.device_driver == "difi")) {
    fmt::print("Discontinuous transmission modes cannot be used with DIFI.\n");
    return false;
  }

  if ((config.device_driver == "difi") && !validate_difi_device_arguments(config.device_arguments, cell_config)) {
    return false;
  }

  if (config.expert_cfg.power_ramping_time_us < 0) {
    fmt::print("Power ramping time, i.e., {:.1f} us, must be positive or zero.\n",
               config.expert_cfg.power_ramping_time_us);
    return false;
  }

  if (!validate_amplitude_control_unit_config(config.amplitude_cfg)) {
    return false;
  }

  return true;
}

bool ocudu::validate_ru_sdr_config(const ru_sdr_unit_config&                 config,
                                   span<const ru_sdr_cell_validation_config> cell_config)
{
  if (!validate_ru_sdr_appconfig(config, cell_config)) {
    return false;
  }
  if (!does_srate_cover_bandwidth(config.srate_MHz, cell_config.front())) {
    fmt::print("Sampling rate (i.e. {} MHz) is too low for the requested channel bandwidth (i.e. {} MHz).\n",
               config.srate_MHz,
               bs_channel_bandwidth_to_MHz(cell_config.front().channel_bw_mhz));
    return false;
  }

  return true;
}
