/*
 *
 * Copyright 2021-2025 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#pragma once

#include "srsran/ran/bs_channel_bandwidth.h"
#include "srsran/ran/subcarrier_spacing.h"
#include "srsran/ran/nr_band.h"
#include "srsran/srslog/srslog.h"
#include "./helpers/ru_config.h"
#include <string>
#include <vector>

namespace srsran {

/// RU emulator OFH configuration parameters.
struct ru_emulator_ofh_appconfig {
  /// T2a maximum parameter for downlink Control-Plane in microseconds.
  std::chrono::microseconds T2a_max_cp_dl{500};
  /// T2a minimum parameter for downlink Control-Plane in microseconds.
  std::chrono::microseconds T2a_min_cp_dl{258};
  /// T2a maximum parameter for uplink Control-Plane in microseconds.
  std::chrono::microseconds T2a_max_cp_ul{500};
  /// T2a minimum parameter for uplink Control-Plane in microseconds.
  std::chrono::microseconds T2a_min_cp_ul{285};
  /// T2a maximum parameter for downlink User-Plane in microseconds.
  std::chrono::microseconds T2a_max_up{300};
  /// T2a minimum parameter for downlink User-Plane in microseconds.
  std::chrono::microseconds T2a_min_up{85};
  /// Ta3 maximum parameter for uplink User-Plane in microseconds.
  std::chrono::microseconds Ta3_max_up{300};
  /// Ta3 minimum parameter for uplink User-Plane in microseconds.
  std::chrono::microseconds Ta3_min_up{85};
  /// Ethernet network interface name or PCI bus identifier.
  std::string network_interface;
  /// RU emulator MAC address.
  std::string ru_mac_address;
  /// Distributed Unit MAC address.
  std::string du_mac_address;
  /// V-LAN Tag control information field.
  unsigned vlan_tag;
  /// Promiscuous mode flag.
  bool enable_promiscuous = false;
  /// RU Uplink ports.
  std::vector<unsigned> ru_ul_port_id = {0};
  /// RU Downlink ports.
  std::vector<unsigned> ru_dl_port_id = {0};
  /// RU PRACH ports.
  std::vector<unsigned> ru_prach_port_id = {4};
  /// RU emulator operating bandwidth.
  bs_channel_bandwidth bandwidth = srsran::bs_channel_bandwidth::MHz100;
  /// Uplink compression method.
  std::string ul_compr_method = "bfp";
  /// Uplink compression bitwidth.
  unsigned ul_compr_bitwidth = 9;
  /// Downlink compression method.
  std::string dl_compr_method = "bfp";
  /// Downlink compression bitwidth.
  unsigned dl_compr_bitwidth = 9;
  /// Downlink static compression header flag.
  bool is_downlink_static_comp_hdr_enabled = true;
  /// Uplink static compression header flag.
  bool is_uplink_static_comp_hdr_enabled = true;
  /// IQ data scaling to be applied prior to Downlink data compression.
  float iq_scaling = 1.0F;
  /// max DL processing delay in slots
  unsigned max_proc_delay = 5;
  /// DL ARFCN of "F_REF", which is the RF reference frequency, as per TS 38.104, Section 5.4.2.1.
  unsigned dl_arfcn;
  /// Common subcarrier spacing for the entire resource grid. It must be supported by the band SS raster.
  subcarrier_spacing common_scs = subcarrier_spacing::kHz15;
  /// NR band.
  std::optional<nr_band> band;
  /// Number of antennas in downlink.
  unsigned nof_antennas_dl = 1;
  /// Number of antennas in uplink.
  unsigned nof_antennas_ul = 1;
};

/// RU emulator logging parameters.
struct ru_emulator_log_appconfig {
  /// Log level
  srslog::basic_levels level = srslog::basic_levels::info;
  /// Path to log file or "stdout" to print to console.
  std::string filename = "stdout";
};

/// DPDK configuration.
struct ru_emulator_dpdk_appconfig {
  /// EAL configuration arguments.
  std::string eal_args;
};

/// RU emulator application configuration.
struct ru_emulator_appconfig {
  /// Logging configuration.
  ru_emulator_log_appconfig log_cfg;
  /// Individual RU emulators configurations.
  std::vector<ru_emulator_ofh_appconfig> ru_cfg = {{}};
  /// sdr RU Configs.
  ru_sdr_unit_config sdr_unit_config;
  /// DPDK configuration.
  std::optional<ru_emulator_dpdk_appconfig> dpdk_config;
};

} // namespace srsran
