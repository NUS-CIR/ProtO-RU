// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/adt/static_vector.h"
#include "ocudu/ofh/compression/iq_compressor.h"
#include "ocudu/ofh/ecpri/ecpri_packet_builder.h"
#include "ocudu/ofh/ethernet/ethernet_mac_address.h"
#include "ocudu/ofh/ethernet/ethernet_vlan_params.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/serdes/ofh_cplane_message_properties.h"
#include "ocudu/ofh/transmitter/ofh_transmitter_timing_parameters.h"
#include "ocudu/ran/antenna_topology.h"
#include "ocudu/ran/bs_channel_bandwidth.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/tdd/tdd_ul_dl_config.h"
#include <optional>

namespace ocudu {
namespace ofh {

/// Open Fronthaul transmitter downlink beamforming (Category B) configuration.
struct transmitter_beamforming_config {
  /// Antenna topology used for beamforming weights codebook generation.
  antenna_topology topology;
  /// Compression parameters applied to the beamforming weights.
  ru_compression_params bfw_compr_params;
};

/// Open Fronthaul transmitter configuration.
struct transmitter_config {
  /// Radio sector identifier.
  unsigned sector;
  /// Channel bandwidth.
  bs_channel_bandwidth bw;
  /// Subcarrier spacing.
  subcarrier_spacing scs;
  /// Cyclic prefix.
  cyclic_prefix cp;
  /// Transmission window timing parameters.
  tx_window_timing_parameters tx_timing_params;
  /// Downlink eAxC.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  /// Uplink eAxC.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  /// PRACH eAxC.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc;
  /// PRACH Contol-Plane enabled flag.
  bool is_prach_cp_enabled;
  /// Destination MAC address.
  ether::mac_address mac_dst_address;
  /// Source MAC address.
  ether::mac_address mac_src_address;
  /// VLAN configuration for C-Plane.
  std::optional<ether::vlan_parameters> vlan_cfg_cp;
  /// VLAN configuration for U-Plane.
  std::optional<ether::vlan_parameters> vlan_cfg_up;
  /// Ethernet interface name or identifier.
  std::string interface;
  /// Promiscuous mode flag.
  bool is_promiscuous_mode_enabled;
  /// MTU size.
  units::bytes mtu_size;
  /// RU working bandwidth.
  bs_channel_bandwidth ru_working_bw;
  /// Downlink compression parameters.
  ru_compression_params dl_compr_params;
  /// Uplink compression parameters.
  ru_compression_params ul_compr_params;
  /// PRACH compression parameters.
  ru_compression_params prach_compr_params;
  /// Downlink static compression header flag.
  bool is_downlink_static_compr_hdr_enabled;
  /// Uplink static compression header flag.
  bool is_uplink_static_compr_hdr_enabled;
  /// IQ samples scaling factor.
  float iq_scaling;
  /// O-RAN FS_Offset of the downlink IQ full scale (O-RAN.WG4.CUS, clause 8.1.3.1).
  std::optional<unsigned> dl_fs_offset;
  /// C-Plane PRACH FFT size (to be used in Type 3 messages).
  cplane_fft_size c_plane_prach_fft_len;
  /// Optional TDD configuration.
  std::optional<tdd_ul_dl_config_common> tdd_config;
  /// Indicates if DPDK should be used by the underlying implementation.
  bool uses_dpdk;
  /// If set to true, metrics are enabled in the transmitter.
  bool are_metrics_enabled = false;
  /// If set to true, logs late events as warnings, otherwise as info.
  bool enable_log_warnings_for_lates;
  /// Downlink beamforming configuration. Unset value means Category A.
  std::optional<transmitter_beamforming_config> beamforming;
};

} // namespace ofh
} // namespace ocudu
