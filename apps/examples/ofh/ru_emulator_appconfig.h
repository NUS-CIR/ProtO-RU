// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/bs_channel_bandwidth.h"
#include "ocudu/ran/nr_band.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include <optional>
#include <string>
#include <vector>

namespace ocudu {

/// PRACH formats supported by the RU emulator are: long format 0 and short format B4.
enum class ru_emulator_prach_format : uint8_t { LONG_F0, SHORT_B4, NONE };

/// \brief RU emulator SDR (radio) configuration.
///
/// When present in an RU configuration, the emulator runs that RU in SDR mode: it drives a real radio (UHD or ZMQ) plus
/// the lower PHY, instead of replying with pre-generated test IQ. When absent, the RU runs in the original Ethernet
/// packet loopback (test-IQ) mode.
struct ru_emulator_sdr_appconfig {
  /// Radio device driver ("uhd" or "zmq").
  std::string device_driver = "zmq";
  /// Radio device arguments (e.g. ZMQ "tx_port=tcp://...,rx_port=tcp://..." ports).
  std::string device_arguments;
  /// Sampling rate in MHz.
  double srate_MHz = 23.04;
  /// \brief Downlink centre frequency override in Hz. If set, overrides the value derived from the cell's DL ARFCN and
  /// NR band; otherwise the radio uses the band-derived frequency (the O-DU way).
  std::optional<double> dl_freq_override_Hz;
  /// Uplink centre frequency override in Hz. If set, overrides the value derived from the cell's UL ARFCN and NR band.
  std::optional<double> ul_freq_override_Hz;
  /// Transmit gain in dB.
  double tx_gain_dB = 60.0;
  /// Receive gain in dB.
  double rx_gain_dB = 60.0;
  /// Centre frequency offset in Hz applied to all radio channels (RF calibration).
  double center_freq_offset_Hz = 0.0;
  /// Clock calibration in PPM, applied to the carrier frequency (corrects the radio oscillator offset; useful on a
  /// non-GPSDO internal clock).
  double calibrate_clock_ppm = 0.0;
  /// LO offset in MHz: shifts the LO away from the centre frequency to move LO leakage out of the channel.
  double lo_offset_MHz = 0.0;
  /// Rx-to-Tx radio time-alignment calibration in samples (overrides the RF driver default when set).
  std::optional<int> time_alignment_calibration;
  /// Radio transmission mode ("continuous", "discontinuous", "same-port").
  std::string transmission_mode = "continuous";
  /// Transmit power ramping time in microseconds (discontinuous transmission).
  float power_ramping_us = 0.0F;
  /// Amplitude gain back-off in dB (headroom for the signal PAPR + DFT power normalisation).
  float gain_backoff_dB = 12.0F;
  /// Amplitude ceiling in dBFS (the level at which the optional clipping acts).
  float power_ceiling_dBFS = -0.1F;
  /// Enable amplitude clipping at the ceiling.
  bool enable_clipping = false;
  // Note: the number of radio TX/RX antenna ports is not configured here — it is derived from the cell's eAxC lists
  // (TX antennas = number of downlink eAxCs, RX antennas = number of uplink eAxCs) so the radio and fronthaul cannot
  // disagree.
  /// Over-the-wire sample format ("default", "sc16", "sc12", "sc8").
  std::string otw_format = "default";
  /// Clock source ("default", "internal", "external", "gpsdo").
  std::string clock_source = "default";
  /// Synchronisation source ("default", "internal", "external", "gpsdo").
  std::string sync_source = "default";
  /// \brief Maximum lower PHY processing delay in slots (how far ahead the TTI boundary is raised).
  ///
  /// The lower PHY advances the notified slot and timestamp by this many slots. ProtO-RU also uses this value to look
  /// back from an advanced TTI when selecting the PRACH occasion approaching its radio capture point. The downlink
  /// fronthaul deadline is controlled separately by the reception-window finalizer. Default 2.
  unsigned max_proc_delay = 2;
  /// \brief Lower-PHY baseband execution profile: "auto", "sequential", "single", "dual" or "triple".
  ///
  /// Selects how many baseband worker threads run the lower-PHY chain. "sequential" runs all roles on one thread (the
  /// only valid choice for ZMQ); "single"/"dual"/"triple" split the chain (high-priority DL/UL + 1 / tx+rx / tx+rx+ul
  /// threads) for real-radio throughput. "auto" picks by host CPU count (single <4, dual <8, triple otherwise), and is
  /// forced to "sequential" for the ZMQ driver.
  std::string execution_profile = "auto";
  /// CPUs the SDR radio and baseband workers are pinned to (empty = no explicit affinity).
  std::vector<unsigned> ru_cpus;
  /// \brief Thread pinning policy within \c ru_cpus: "mask" (every worker shares the whole CPU set) or "round-robin"
  /// (each worker is pinned to one CPU from the set, cycling).
  std::string pinning_policy = "mask";
};

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
  /// Ta3 maximum parameter for the uplink User-Plane (the RU transmit window) in microseconds.
  std::chrono::microseconds Ta3_max_up{300};
  /// Ta3 minimum parameter for the uplink User-Plane (the RU transmit window) in microseconds.
  std::chrono::microseconds Ta3_min_up{85};
  /// Ethernet network interface name or PCI bus identifier.
  std::string network_interface;
  /// RU emulator MAC address.
  std::string ru_mac_address;
  /// Distributed Unit MAC address.
  std::string du_mac_address;
  /// \brief VLAN Tag control information field. When unset, frames are transmitted untagged.
  ///
  /// Leave it unset when the fronthaul interface tags the traffic itself, as an SR-IOV VF with a port VLAN does
  /// (\c "ip link set <PF> vf <N> vlan <ID>"): the PF inserts the tag on egress and strips it on ingress, so a
  /// software tag on top of it puts a second 802.1Q header on the wire.
  std::optional<unsigned> vlan_tag;
  /// Promiscuous mode flag.
  bool enable_promiscuous = false;
  /// RU Uplink ports.
  std::vector<unsigned> ru_ul_port_id = {0, 1};
  /// RU Downlink ports.
  std::vector<unsigned> ru_dl_port_id = {0, 1, 2, 3};
  /// RU PRACH ports.
  std::vector<unsigned> ru_prach_port_id = {4, 5};
  /// RU emulator operating bandwidth.
  bs_channel_bandwidth bandwidth = ocudu::bs_channel_bandwidth::MHz100;
  /// \brief Cell common subcarrier spacing (15 or 30 kHz).
  ///
  /// All configured cells must share the same SCS: the GPS timing worker (ru_timing) ticks a single numerology for
  /// every sector it drives.
  subcarrier_spacing common_scs = subcarrier_spacing::kHz30;
  /// \brief Downlink ARFCN — the cell's DL centre-frequency reference.
  ///
  /// DL and UL centre frequencies are derived from this and the NR band (the O-DU way, via the band tables), unless
  /// overridden by the SDR configuration's dl_freq_override_Hz / ul_freq_override_Hz.
  unsigned dl_arfcn = 632628;
  /// NR band. If unset, it is derived from the DL ARFCN.
  std::optional<nr_band> band;
  /// Uplink compression method (the U-plane the RU transmits).
  std::string ul_compr_method = "bfp";
  /// Uplink compression bitwidth.
  unsigned ul_compr_bitwidth = 9;
  /// Downlink compression method (the U-plane the RU receives).
  std::string dl_compr_method = "bfp";
  /// Downlink compression bitwidth.
  unsigned dl_compr_bitwidth = 9;
  /// PRACH compression method (the PRACH U-plane the RU transmits).
  std::string prach_compr_method = "bfp";
  /// PRACH compression bitwidth.
  unsigned prach_compr_bitwidth = 9;
  /// IQ scaling applied before compression (1.0 = no scaling; lower it for real captured signals to avoid clipping).
  float iq_scaling = 1.0f;
  /// Uplink static (out-of-band) compression header flag.
  bool is_ul_static_compr_hdr = true;
  /// Downlink static (out-of-band) compression header flag.
  bool is_dl_static_compr_hdr = true;
  /// PRACH format used when sending dummy PRACH U-Plane packets.
  ru_emulator_prach_format prach_format = ru_emulator_prach_format::LONG_F0;
  /// \brief SDR (radio) configuration. When present, this RU runs in SDR mode (real radio + lower PHY); when absent it
  /// runs in the original Ethernet packet loopback (test-IQ) mode.
  std::optional<ru_emulator_sdr_appconfig> sdr_config;
  /// CPUs the Open Fronthaul receive + decode workers (ru_rx, ru_emu — and the fake upper PHY in loopback) are pinned
  /// to (empty = no explicit affinity).
  std::vector<unsigned> ofh_cpus;
};

/// RU emulator logging parameters.
struct ru_emulator_log_appconfig {
  /// Log level
  ocudulog::basic_levels level = ocudulog::basic_levels::info;
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
  /// CPUs the shared GPS timing worker (ru_timing) is pinned to (empty = no explicit affinity).
  std::vector<unsigned> timing_cpus;
  /// DPDK configuration.
  std::optional<ru_emulator_dpdk_appconfig> dpdk_config;
};

} // namespace ocudu
