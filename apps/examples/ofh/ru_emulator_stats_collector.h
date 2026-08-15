// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ru_emulator_rx_window_checker.h"
#include "ru_emulator_seq_id_checker.h"
#include "ocudu/ofh/ethernet/ethernet_mac_address.h"
#include "ocudu/ofh/ethernet/ethernet_vlan_params.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/serdes/ofh_message_properties.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include <optional>

namespace ocudu {

/// RU emulator statistics collector configuration.
struct ru_emulator_stats_collector_config {
  /// Cell common subcarrier spacing (used to decode the timestamp of received messages).
  subcarrier_spacing scs;
  /// Filter index expected on PRACH Control-Plane requests, given the configured PRACH format.
  ofh::filter_index_type prach_filter_index;
  /// Number of OFDM symbols comprising a PRACH U-Plane transmission.
  unsigned nof_prach_symbols;
  /// Supported values of the udCompHdr field of UL Control-Plane requests.
  static_vector<uint8_t, 2> supported_ul_compr_hdr;
  /// Reception windows (in symbols) for DL Control-Plane, DL User-Plane and UL Control-Plane messages.
  ofh::rx_window_timing_parameters dl_cp_window;
  ofh::rx_window_timing_parameters dl_up_window;
  ofh::rx_window_timing_parameters ul_cp_window;
  /// Configured eAxCs for downlink, uplink and PRACH.
  static_vector<unsigned, ofh::MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  static_vector<unsigned, ofh::MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  static_vector<unsigned, ofh::MAX_NOF_SUPPORTED_EAXC> prach_eaxc;
  /// This RU's own MAC address (the expected destination of received frames) and the peer O-DU's MAC address (their
  /// expected source). Frames not matching both are not destined for this RU and are not counted, mirroring the O-RU
  /// sector's Ethernet receive filter.
  ether::mac_address ru_mac_address;
  ether::mac_address du_mac_address;
  /// Expected VLAN of received frames. When a tag is observed, its VID must match; PCP is not filtered and untagged
  /// frames are accepted because the tag may have been stripped before delivery.
  std::optional<ether::vlan_parameters> vlan_config;
};

/// \brief RU emulator statistics collector.
///
/// Restores the upstream RU emulator KPI table: it peeks every received Ethernet frame (independently of the O-RU
/// sector that processes it) and classifies it exactly the way the upstream emulator did - dropped (non-eCPRI),
/// corrupt (undefined/unsupported/unconfigured O-RAN parameters) or valid; valid messages update the per-plane
/// reception-window statistics and the per-eAxC sequence-identifier error counters. Transmitted frames are counted
/// through \ref on_frames_sent by a decorator around the sector's Ethernet transmitter.
class ru_emulator_stats_collector
{
  using kpi_counter = ru_emu_stats::kpi_counter;

  /// Aggregates information peeked from a message received from the DU.
  struct rx_message_info {
    unsigned               eaxc;
    ofh::data_direction    direction;
    ofh::filter_index_type filter_index;
    ofh::message_type      type;
    ofh::slot_symbol_point symbol_point{{}, 0, get_nsymb_per_slot(cyclic_prefix::NORMAL)};
    unsigned               nof_symbols;
    uint8_t                compr_header;
    uint8_t                seq_id;
  };

  ocudulog::basic_logger&                  logger;
  const ru_emulator_stats_collector_config cfg;

  // Timing window checkers, store statistics of early/late/on-time packets.
  ru_emulator_rx_window_checker dl_cp_window_checker;
  ru_emulator_rx_window_checker dl_up_window_checker;
  ru_emulator_rx_window_checker ul_cp_window_checker;

  // Sequence identifier checkers.
  ru_emulator_seq_id_checker dl_up_seq_id_checker;
  ru_emulator_seq_id_checker dl_cp_seq_id_checker;
  ru_emulator_seq_id_checker ul_cp_seq_id_checker;
  ru_emulator_seq_id_checker prach_seq_id_checker;

  // Other KPI counters.
  kpi_counter rx_total_counter;
  kpi_counter tx_total_counter;
  kpi_counter corrupt_counter;
  kpi_counter dropped_counter;

public:
  ru_emulator_stats_collector(const ru_emulator_stats_collector_config& cfg_, ocudulog::basic_logger& logger_);

  /// Classifies a received Ethernet frame and updates the reception statistics.
  void on_frame(span<const uint8_t> payload);

  /// Counts frames transmitted by the O-RU sector.
  void on_frames_sent(unsigned nof_frames) { tx_total_counter.increment(nof_frames); }

  /// Returns the OTA symbol boundary notifiers of the reception window checkers.
  std::vector<ofh::ota_symbol_boundary_notifier*> get_ota_notifiers();

  /// Prints the KPIs accumulated since the last call, one table row per call.
  void print_statistics(unsigned emu_id);

private:
  /// Analyzes the content of a received OFH packet. Returns true on success, false if the packet is corrupt.
  bool decode_rx_message(rx_message_info&    message_info,
                         span<const uint8_t> packet,
                         unsigned            ethernet_header_adjustment) const;

  /// \brief Validates decoded message parameters.
  ///
  /// \note A packet is considered corrupt if any of the decoded parameters has a value undefined in the O-RAN
  /// specification, an unsupported value (e.g. compression parameters) or an unconfigured value (e.g. eAxC value).
  bool validate_rx_ofh_params(const rx_message_info& message_info);

  /// Returns the window checker for OFH messages of the type given by \c message_info.
  ru_emulator_rx_window_checker& get_window_checker(const rx_message_info& message_info);

  /// Returns the sequence identifier checker for OFH messages of the type given by \c message_info.
  ru_emulator_seq_id_checker& get_sequence_id_checker(const rx_message_info& message_info);

  /// Returns a string containing the sequence identifier errors for the given list of eAxCs collected by the specified
  /// sequence identifier checker object.
  static std::string print_seq_id_err(span<const unsigned> eaxc, ru_emulator_seq_id_checker& seq_id_checker);
};

} // namespace ocudu
