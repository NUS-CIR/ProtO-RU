// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "apps/helpers/metrics/metrics_config.h"
#include "apps/services/worker_manager/os_sched_affinity_manager.h"
#include "ocudu/ofh/receiver/ofh_receiver_configuration.h"
#include "ocudu/ofh/serdes/ofh_cplane_message_properties.h"
#include "ocudu/ran/bs_channel_bandwidth.h"
#include "ocudu/support/units.h"
#include <chrono>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace ocudu {

/// Configuration parameters related to the scaling of the IQ symbols in the DL OFH resource grid.
struct ru_ofh_scaling_config {
  /// \brief RU reference level in dBFS.
  ///
  /// IQ power level that, carried over a single subcarrier, the RU maps to its maximum RMS output power. It
  /// corresponds to the O-RAN Reference_Level (O-RAN.WG4.CUS, clauses 8.1.3.2.2 and 8.1.3.3), which is 0 dBFS if the
  /// RU does not support or configure it. The default of -12 dBFS is a conservative legacy value.
  ///
  /// The dBFS scale follows O-RAN.WG4.CUS, clause 8.1.3.1: 0 dBFS is an IQ power \f$I^2+Q^2 = FS_0 2^{-FS\_Offset}\f$,
  /// where \f$FS_0 = \max(I^2) = \max(Q^2)\f$ is the largest IQ power that can be represented with arbitrary phase with
  /// the given IQ bit width and compression. The FS_Offset is:
  /// - \ref ru_ofh_unit_base_cell_config::fs_offset_dl, if set.
  /// - Otherwise, the one of the OCUDU native full scale:
  ///   - 0 without compression, i.e., the full scale of the configured IQ bit width;
  ///   - \f$2(m-1)\f$ for BFP with an \f$m\f$-bit mantissa, because the IQ data are quantized to 16 bits before
  ///     compression and the exponents above \f$16-m\f$ are never used. For BFP9, this is 16, i.e., the OCUDU full
  ///     scale is \f$6.02(m-1) = 48.2\f$ dB below \f$FS_0\f$.
  ///
  /// This value equals the RU Reference_Level only if the effective FS_Offset matches the one configured in the RU.
  /// Otherwise, a level of \f$L\f$ dBFS here is \f$L - 3.01(FS\_Offset_{OCUDU} - FS\_Offset_{RU})\f$ dBFS at the RU.
  ///
  /// \warning Check the RU documentation before setting this parameter, as incorrect values may damage the RU.
  float ru_reference_level_dBFS = -12.0f;
  /// \brief Back-off of the subcarrier RMS power with respect to \ref ru_reference_level_dBFS.
  ///
  /// The O-RAN TX power budget (O-RAN.WG4.CUS, clause 8.1.3.3) requires the power summed over all simultaneously
  /// transmitted subcarriers not to exceed the reference level. If not set, the back-off is
  /// \f$10\log_{10}(12 N_{RB})\f$, which meets this budget for a fully allocated carrier. Smaller values exceed the
  /// O-RAN power budget and should only be used if the RU documentation specifies the expected subcarrier level.
  ///
  /// RUs that apply bandwidth normalization internally (not O-RAN compliant) typically require a fixed back-off
  /// independent of the bandwidth. For RUs that do not normalize their gain to the channel bandwidth, a back-off
  /// calibrated for one bandwidth converts to another as \f$B_{BW2} = B_{BW1} + 10\log_{10}(BW2/BW1)\f$.
  ///
  /// \warning Check the RU documentation before setting this parameter, as incorrect values may damage the RU.
  std::optional<float> subcarrier_rms_backoff_dB;
};

/// Configuration parameters related to the scaling of the IQ symbols in the DL OFH resource grid (legacy version).
struct ru_ofh_legacy_scaling_config {
  /// \brief IQ data scaling to be applied prior to Downlink data compression.
  ///
  /// If configured, it overrides the other power configuration parameters \ref ru_reference_level_dBFS and \ref
  /// subcarrier_rms_backoff_dB. To preserve backwards-compatibility with older configs, the IQ scaling factor applied
  /// to the OFH symbols will include \f$ G_{iq}=\frac{\text{iq\_scaling}}{\sqrt{12 N_{RB}}} \f$.
  ///
  /// \warning Check the RU documentation before setting this parameter, as incorrect values may damage the RU.
  float iq_scaling = 0.1f;
};

/// Open Fronthaul downlink beamforming (Category B) configuration.
struct ru_ofh_beamforming_config {
  /// Beamforming weights compression method.
  std::string compression_method = "none";
  /// Beamforming weights compression bitwidth.
  unsigned compression_bitwidth = 16;
};

/// gNB app Open Fronthaul base cell configuration.
struct ru_ofh_unit_base_cell_config {
  /// \brief RU operating bandwidth.
  ///
  /// Set this option when the operating bandwidth of the RU is larger than the configured bandwidth of the cell.
  std::optional<bs_channel_bandwidth> ru_operating_bw;
  /// T1a maximum parameter for downlink Control-Plane in microseconds.
  std::chrono::microseconds T1a_max_cp_dl{500};
  /// T1a minimum parameter for downlink Control-Plane in microseconds.
  std::chrono::microseconds T1a_min_cp_dl{258};
  /// T1a maximum parameter for uplink Control-Plane in microseconds.
  std::chrono::microseconds T1a_max_cp_ul{500};
  /// T1a minimum parameter for uplink Control-Plane in microseconds.
  std::chrono::microseconds T1a_min_cp_ul{285};
  /// T1a maximum parameter for downlink User-Plane in microseconds.
  std::chrono::microseconds T1a_max_up{300};
  /// T1a minimum parameter for downlink User-Plane in microseconds.
  std::chrono::microseconds T1a_min_up{85};
  /// Ta4 maximum parameter for uplink User-Plane in microseconds.
  std::chrono::microseconds Ta4_max{500};
  /// Ta4 minimum parameter for uplink User-Plane in microseconds.
  std::chrono::microseconds Ta4_min{85};
  /// Enables the Control-Plane PRACH message signalling.
  bool is_prach_control_plane_enabled = true;
  /// Ignore the start symbol value received in the PRACH U-Plane packets.
  bool ignore_prach_start_symbol = false;
  /// If set to true, the payload size encoded in a eCPRI header is ignored.
  bool ignore_ecpri_payload_size_field = false;
  /// If set to true, the sequence id encoded in a eCPRI packet is ignored.
  bool ignore_ecpri_seq_id_field = false;
  /// If set to true, logs late events as warnings, otherwise as info.
  bool enable_log_warnings_for_lates = true;
  /// Warn of unreceived Radio Unit frames status.
  ofh::warn_unreceived_ru_frames log_unreceived_ru_frames = ofh::warn_unreceived_ru_frames::after_traffic_detection;
  /// Uplink compression method.
  std::string compression_method_ul = "bfp";
  /// Uplink compression bitwidth.
  unsigned compression_bitwidth_ul = 9;
  /// Downlink compression method.
  std::string compression_method_dl = "bfp";
  /// Downlink compression bitwidth.
  unsigned compression_bitwidth_dl = 9;
  /// PRACH compression method.
  std::string compression_method_prach = "bfp";
  /// PRACH compression bitwidth.
  unsigned compression_bitwidth_prach = 9;
  /// Downlink static compression header flag.
  bool is_downlink_static_comp_hdr_enabled = true;
  /// Uplink static compression header flag.
  bool is_uplink_static_comp_hdr_enabled = true;
  /// \brief O-RAN FS_Offset of the downlink IQ full scale (O-RAN.WG4.CUS, clause 8.1.3.1).
  ///
  /// Sets the full scale that the downlink IQ data refer to as \f$FS_0 2^{-\text{FS\_Offset}}\f$, where \f$FS_0\f$ is
  /// the largest IQ power of the compression format and each step is 3.01 dB. It must match the downlink FS_Offset
  /// configured in the RU.
  ///
  /// If not set, no full scale scaling is applied and the IQ data keep the OCUDU native full scale, as in previous
  /// OCUDU versions. This corresponds to an FS_Offset of 0 without compression and \f$2(m-1)\f$ for BFP compression
  /// with an \f$m\f$-bit mantissa, e.g., 16 for BFP9, which is the FS_Offset used by FlexRAN. Setting this value to
  /// the native FS_Offset is therefore equivalent to leaving it unset.
  ///
  /// \note Leaving it unset is not the same as setting it to 0, even though 0 is the O-RAN default. RUs that follow
  /// the O-RAN default with BFP compression need this value to be set to 0 explicitly.
  ///
  /// \warning Check the RU documentation before setting this parameter, as incorrect values may damage the RU.
  std::optional<unsigned> fs_offset_dl;
  /// \brief O-RAN FS_Offset of the uplink IQ full scale (O-RAN.WG4.CUS, clause 8.1.3.1).
  ///
  /// Same as \ref fs_offset_dl, but applies to the uplink IQ data, including PRACH. It must match the uplink
  /// FS_Offset configured in the RU. If not set, the native FS_Offset is derived separately from the uplink and the
  /// PRACH compression parameters.
  ///
  /// \warning Check the RU documentation before setting this parameter, as incorrect values may damage the RU.
  std::optional<unsigned> fs_offset_ul;
  /// Scaling configuration parameters.
  std::variant<std::monostate, ru_ofh_scaling_config, ru_ofh_legacy_scaling_config> iq_scaling_config;
  /// PRACH FFT size (to be used in the C-plane Type 3 messages).
  ofh::cplane_fft_size c_plane_prach_fft_len = ofh::cplane_fft_size::fft_4096;
  /// Downlink beamforming (Category B) configuration. Unset means Category A.
  std::optional<ru_ofh_beamforming_config> dl_beamforming;
};

/// gNB app Open Fronthaul cell configuration.
struct ru_ofh_unit_cell_config {
  /// Base cell configuration.
  ru_ofh_unit_base_cell_config cell;
  /// Ethernet network interface name or identifier.
  std::string network_interface = "enp1s0f0";
  /// Promiscuous mode flag.
  bool enable_promiscuous_mode = false;
  /// Ethernet link status checking flag.
  bool check_link_status = false;
  /// MTU size.
  units::bytes mtu_size{9000};
  /// Radio Unit MAC address.
  std::string ru_mac_address = "70:b3:d5:e1:5b:06";
  /// Distributed Unit MAC address.
  std::string du_mac_address = "00:11:22:33:00:77";
  /// VLAN Tag control information VLAN identifier field for C-Plane.
  std::optional<uint16_t> vlan_tag_cp;
  /// VLAN Tag control information VLAN identifier field for U-Plane.
  std::optional<uint16_t> vlan_tag_up;
  /// VLAN Tag control information Priority code point (PCP) field for C-Plane.
  std::optional<uint8_t> vlan_pcp_cp;
  /// VLAN Tag control information Priority code point (PCP) field for U-Plane.
  std::optional<uint8_t> vlan_pcp_up;

  /// RU PRACH port.
  std::vector<unsigned> ru_prach_port_id = {4};
  /// RU Downlink port.
  std::vector<unsigned> ru_dl_port_id = {0};
  /// RU Uplink port.
  std::vector<unsigned> ru_ul_port_id = {0};
};

/// RU OFH logging functionalities.
struct ru_ofh_unit_logger_config {
  ocudulog::basic_levels ofh_level = ocudulog::basic_levels::warning;
};

/// CPU affinities configuration for the cell.
struct ru_ofh_unit_cpu_affinities_cell_config {
  /// Radio Unit workers CPU affinity mask.
  os_sched_affinity_config ru_cpu_cfg = {sched_affinity_mask_types::ru, {}, sched_affinity_mask_policy::mask};
};

/// Expert configuration.
struct ru_ofh_unit_expert_execution_config {
  /// RU timing thread.
  os_sched_affinity_bitmask ru_timing_cpu;
  /// Busy waiting enabled flag.
  bool enable_busy_waiting = false;
  /// CPU affinities per RU txrx thread.
  std::vector<os_sched_affinity_bitmask> txrx_affinities;
  /// CPU affinities per cell.
  std::vector<ru_ofh_unit_cpu_affinities_cell_config> cell_affinities = {{}};
};

/// Metrics configuration.
struct ru_ofh_unit_metrics_config {
  /// Metrics configuration.
  app_helpers::metrics_config metrics_cfg;
  /// Flag that control RU metrics.
  bool enable_ru_metrics = false;
};

/// gNB app Open Fronthaul Radio Unit configuration.
struct ru_ofh_unit_config {
  /// GPS Alpha - Valid value range: [0, 1.2288e7].
  unsigned gps_Alpha = 0;
  /// GPS Beta - Valid value range: [-32768, 32767].
  int gps_Beta = 0;
  /// Loggers.
  ru_ofh_unit_logger_config loggers;
  /// \brief Individual Open Fronthaul cells configurations.
  ///
  /// \note Add one cell by default.
  std::vector<ru_ofh_unit_cell_config> cells = {{}};
  /// \brief Expert execution parameters for the Open Fronthaul Radio Unit.
  ///
  /// \note Add one cell by default.
  ru_ofh_unit_expert_execution_config expert_execution_cfg;
  /// Metrics configuration.
  ru_ofh_unit_metrics_config metrics_cfg;
};

/// gNB app Open Fronthaul Radio Unit configuration.
struct ru_ofh_unit_parsed_config {
  /// Open Fronthaul Radio Unit configuration.
  ru_ofh_unit_config config;
  /// Base cell configuration for the Radio Unit.
  ru_ofh_unit_base_cell_config base_cell_cfg;
};

} // namespace ocudu
