// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ofh_ru_message_receiver.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include "ocudu/ofh/serdes/ofh_uplane_message_decoder.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <variant>

using namespace ocudu;
using namespace ofh;

/// Minimum Ethernet frame length expected by the Ethernet frame decoder.
static constexpr unsigned MIN_ETH_FRAME_LEN = 64;

ru_message_receiver::ru_message_receiver(const ru_message_receiver_config&  config,
                                         ru_message_receiver_dependencies&& dependencies) :
  logger(*dependencies.logger),
  eth_decoder(std::move(dependencies.eth_decoder)),
  ecpri_decoder(std::move(dependencies.ecpri_decoder)),
  uplane_handler(*dependencies.uplane_handler),
  cplane_handler(*dependencies.cplane_handler),
  uplane_seq_id_checker(dependencies.uplane_seq_id_checker),
  cplane_dl_seq_id_checker(dependencies.cplane_dl_seq_id_checker),
  cplane_ul_seq_id_checker(dependencies.cplane_ul_seq_id_checker),
  window_checker(dependencies.window_checker),
  vlan_params(config.vlan_params),
  dl_eaxc(config.dl_eaxc),
  scs(config.scs),
  nof_symbols_per_slot(config.nof_symbols_per_slot),
  sector_id(config.sector)
{
  ocudu_assert(eth_decoder, "Invalid Ethernet decoder");
  ocudu_assert(ecpri_decoder, "Invalid eCPRI decoder");
  ocudu_assert(dependencies.uplane_handler, "Invalid User-Plane handler");
  ocudu_assert(dependencies.cplane_handler, "Invalid Control-Plane handler");
}

void ru_message_receiver::process_frame(span<const uint8_t> payload)
{
  // The Ethernet decoder drops frames below the 64-byte wire minimum, but received frames may legitimately be shorter:
  // the frame check sequence (and possibly the VLAN tag) is stripped before reception. The O-DU only ever receives
  // large User-Plane frames, while the O-RU receives small Control-Plane requests that fall below the limit, so pad
  // them back up (the eCPRI decoder validates the real payload sizes).
  std::array<uint8_t, MIN_ETH_FRAME_LEN> padded_frame;
  if (payload.size() < MIN_ETH_FRAME_LEN) {
    std::memcpy(padded_frame.data(), payload.data(), payload.size());
    std::memset(padded_frame.data() + payload.size(), 0, MIN_ETH_FRAME_LEN - payload.size());
    payload = padded_frame;
  }

  ether::vlan_frame_params eth_params;
  span<const uint8_t>      ecpri_pdu = eth_decoder->decode(payload, eth_params);
  if (OCUDU_UNLIKELY(ecpri_pdu.empty() || should_ethernet_frame_be_filtered(eth_params))) {
    return;
  }

  ecpri::packet_parameters ecpri_params;
  span<const uint8_t>      ofh_pdu = ecpri_decoder->decode(ecpri_pdu, ecpri_params);
  if (OCUDU_UNLIKELY(ofh_pdu.empty())) {
    return;
  }

  // Reception-window check. The slot is encoded identically in the User-Plane and Control-Plane radio application
  // headers, so the U-plane peeker works for both.
  if (window_checker != nullptr) {
    if (auto slot = uplane_peeker::peek_slot_symbol_point(ofh_pdu, nof_symbols_per_slot, scs)) {
      window_checker->update_rx_window_statistics(*slot);
    }
  }

  switch (ecpri_params.header.msg_type) {
    case ecpri::message_type::iq_data: {
      const auto& params = std::get<ecpri::iq_data_parameters>(ecpri_params.type_params);
      if (OCUDU_UNLIKELY(std::find(dl_eaxc.begin(), dl_eaxc.end(), params.pc_id) == dl_eaxc.end())) {
        logger.info("Sector#{}: dropped received Open Fronthaul User-Plane packet as decoded eAxC value '{}' is not "
                    "configured in reception",
                    sector_id,
                    params.pc_id);
        return;
      }
      const uint8_t seq_id = params.seq_id >> 8;
      if (uplane_seq_id_checker != nullptr &&
          uplane_seq_id_checker->update_and_compare_seq_id(params.pc_id, seq_id) < 0) {
        // Sequence identifiers are assigned while messages are encoded. Parallel encoding can therefore make their
        // wire order differ from their identifier order. Keep the diagnostic, but do not discard otherwise valid IQ.
        logger.info("Sector#{}: received User-Plane message for eAxC '{}' with sequence identifier '{}' from the past; "
                    "processing it",
                    sector_id,
                    params.pc_id,
                    seq_id);
      }
      uplane_handler.decode_message(params.pc_id, ofh_pdu);
      break;
    }
    case ecpri::message_type::rt_control_data: {
      const auto& params = std::get<ecpri::realtime_control_parameters>(ecpri_params.type_params);
      // Downlink and uplink Control-Plane carry independent sequence counters (the data direction is the MSB of the
      // first radio-application-header byte), so pick the checker by direction.
      const bool           is_downlink = (ofh_pdu[0] & 0x80U) != 0;
      sequence_id_checker* seq_checker = is_downlink ? cplane_dl_seq_id_checker : cplane_ul_seq_id_checker;
      const uint8_t        seq_id      = params.seq_id >> 8;
      if (seq_checker != nullptr && seq_checker->update_and_compare_seq_id(params.rtc_id, seq_id) < 0) {
        logger.info("Sector#{}: received {} Control-Plane message for eAxC '{}' with sequence identifier '{}' from the "
                    "past; processing it",
                    sector_id,
                    is_downlink ? "downlink" : "uplink",
                    params.rtc_id,
                    seq_id);
      }
      cplane_handler.decode_message(params.rtc_id, ofh_pdu);
      break;
    }
    default:
      logger.info("Sector#{}: dropped received frame with unsupported eCPRI message type", sector_id);
      break;
  }
}

bool ru_message_receiver::should_ethernet_frame_be_filtered(const ether::vlan_frame_params& eth_params) const
{
  if (OCUDU_UNLIKELY(eth_params.mac_src_address != vlan_params.mac_src_address)) {
    logger.debug("Sector#{}: dropped received Ethernet frame as source MAC addresses do not match (detected={:02X}, "
                 "expected={:02X})",
                 sector_id,
                 span<const uint8_t>(eth_params.mac_src_address),
                 span<const uint8_t>(vlan_params.mac_src_address));

    return true;
  }

  if (OCUDU_UNLIKELY(eth_params.mac_dst_address != vlan_params.mac_dst_address)) {
    logger.debug(
        "Sector#{}: dropped received Ethernet frame as destination MAC addresses do not match (detected={:02X}, "
        "expected={:02X})",
        sector_id,
        span<const uint8_t>(eth_params.mac_dst_address),
        span<const uint8_t>(vlan_params.mac_dst_address));

    return true;
  }

  if (OCUDU_UNLIKELY(eth_params.vlan_config.has_value() && vlan_params.vlan_config.has_value() &&
                     eth_params.vlan_config->tci_vid != vlan_params.vlan_config->tci_vid)) {
    logger.info("Sector#{}: dropped received Ethernet frame as its VLAN identifier '{}' does not match the configured "
                "VLAN identifier '{}'",
                sector_id,
                eth_params.vlan_config->tci_vid,
                vlan_params.vlan_config->tci_vid);

    return true;
  }

  if (OCUDU_UNLIKELY(eth_params.eth_type != vlan_params.eth_type)) {
    logger.debug("Sector#{}: dropped received Ethernet frame as decoded Ethernet type is '{}' but expected '{}'",
                 sector_id,
                 eth_params.eth_type,
                 vlan_params.eth_type);

    return true;
  }

  return false;
}
