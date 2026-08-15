// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "../receiver/ofh_ru_rx_message_handler.h"
#include "../receiver/ofh_rx_window_checker.h"
#include "ocudu/adt/static_vector.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/ecpri/ecpri_packet_decoder.h"
#include "ocudu/ofh/ethernet/vlan_ethernet_frame_decoder.h"
#include "ocudu/ofh/ethernet/vlan_ethernet_frame_params.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/ofh/receiver/ofh_sequence_id_checker.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include <memory>

namespace ocudu {
namespace ofh {

/// O-RU message receiver configuration.
struct ru_message_receiver_config {
  /// Radio sector identifier.
  unsigned sector;
  /// Subcarrier spacing (used to peek the slot for reception-window checking).
  subcarrier_spacing scs = subcarrier_spacing::kHz30;
  /// Number of OFDM symbols per slot.
  unsigned nof_symbols_per_slot = 14;
  /// \brief Expected Ethernet (VLAN) parameters of received frames.
  ///
  /// The source is the O-DU's MAC address and the destination this O-RU's, i.e. the reverse of the sector's transmit
  /// parameters. Received frames that do not match are filtered out, like in the O-DU message receiver (this also
  /// discards the O-RU's own transmissions when they are observed on a shared interface).
  ether::vlan_frame_params vlan_params;
  /// Downlink eAxCs: received User-Plane messages for any other eAxC are filtered out.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
};

/// O-RU message receiver dependencies.
struct ru_message_receiver_dependencies {
  /// Logger.
  ocudulog::basic_logger* logger = nullptr;
  /// Ethernet (VLAN) frame decoder.
  std::unique_ptr<ether::vlan_frame_decoder> eth_decoder;
  /// eCPRI packet decoder.
  std::unique_ptr<ecpri::packet_decoder> ecpri_decoder;
  /// Handler for received User-Plane messages.
  ru_rx_message_handler* uplane_handler = nullptr;
  /// Handler for received Control-Plane messages.
  ru_rx_message_handler* cplane_handler = nullptr;
  /// Optional sequence-identifier checker for received User-Plane messages.
  sequence_id_checker* uplane_seq_id_checker = nullptr;
  /// \brief Optional sequence-identifier checkers for received Control-Plane messages, one per data direction.
  ///
  /// The O-DU generates the downlink and uplink Control-Plane sequence identifiers from independent per-eAxC counters
  /// (they are separate message streams sharing an eAxC), so they must be checked independently.
  sequence_id_checker* cplane_dl_seq_id_checker = nullptr;
  sequence_id_checker* cplane_ul_seq_id_checker = nullptr;
  /// Optional reception-window checker.
  rx_window_checker* window_checker = nullptr;
};

/// \brief O-RU message receiver.
///
/// Decodes a received Ethernet frame into its eCPRI payload and dispatches the Open Fronthaul message to the O-RU
/// User-Plane or Control-Plane receive data flow according to the eCPRI message type (IQ data -> User-Plane,
/// real-time control -> Control-Plane). The eAxC is taken from the eCPRI payload identifier.
class ru_message_receiver
{
public:
  ru_message_receiver(const ru_message_receiver_config& config, ru_message_receiver_dependencies&& dependencies);

  /// Processes a received Ethernet frame.
  void process_frame(span<const uint8_t> payload);

private:
  /// Returns true when the given decoded Ethernet parameters do not match the expected ones.
  bool should_ethernet_frame_be_filtered(const ether::vlan_frame_params& eth_params) const;

  ocudulog::basic_logger&                               logger;
  std::unique_ptr<ether::vlan_frame_decoder>            eth_decoder;
  std::unique_ptr<ecpri::packet_decoder>                ecpri_decoder;
  ru_rx_message_handler&                                uplane_handler;
  ru_rx_message_handler&                                cplane_handler;
  sequence_id_checker*                                  uplane_seq_id_checker;
  sequence_id_checker*                                  cplane_dl_seq_id_checker;
  sequence_id_checker*                                  cplane_ul_seq_id_checker;
  rx_window_checker*                                    window_checker;
  const ether::vlan_frame_params                        vlan_params;
  const static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  const subcarrier_spacing                              scs;
  const unsigned                                        nof_symbols_per_slot;
  const unsigned                                        sector_id;
};

} // namespace ofh
} // namespace ocudu
