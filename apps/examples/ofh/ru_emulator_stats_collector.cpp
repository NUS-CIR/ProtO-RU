// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ru_emulator_stats_collector.h"
#include "ocudu/ofh/ecpri/ecpri_packet_properties.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include "ocudu/ofh/ofh_factories.h"
#include "ocudu/support/format/fmt_to_c_str.h"
#include "fmt/chrono.h"

using namespace ocudu;
using namespace ofh;

/// Maximum number of symbols in a slot, considering normal cyclic prefix.
static constexpr size_t   MAX_NOF_SYMBOLS   = get_nsymb_per_slot(cyclic_prefix::NORMAL);
static constexpr unsigned ETH_HEADER_SIZE   = 14;
static constexpr unsigned ETH_VLAN_TAG_SIZE = 4;
static constexpr uint16_t VLAN_TPID         = 0x8100;

ru_emulator_stats_collector::ru_emulator_stats_collector(const ru_emulator_stats_collector_config& cfg_,
                                                         ocudulog::basic_logger&                   logger_) :
  logger(logger_),
  cfg(cfg_),
  dl_cp_window_checker(cfg_.dl_cp_window),
  dl_up_window_checker(cfg_.dl_up_window),
  ul_cp_window_checker(cfg_.ul_cp_window),
  dl_up_seq_id_checker("DL UP", logger_, create_sequence_id_checker()),
  dl_cp_seq_id_checker("DL CP", logger_, create_sequence_id_checker()),
  ul_cp_seq_id_checker("UL CP", logger_, create_sequence_id_checker()),
  prach_seq_id_checker("PRACH", logger_, create_sequence_id_checker())
{
}

/// \brief Checks whether a received OFH packet should be dropped or processed by the RU emulator.
///
/// A packet must be dropped if it is not destined for this RU (MAC addresses do not match) or if it is not an eCPRI OFH
/// packet.
static bool should_packet_be_dropped(span<const uint8_t>                       packet,
                                     const ru_emulator_stats_collector_config& cfg,
                                     ocudulog::basic_logger&                   logger,
                                     unsigned&                                 ethernet_header_adjustment)
{
  if (packet.size() < ETH_HEADER_SIZE) {
    logger.debug("Dropping packet smaller than an Ethernet header");
    return true;
  }

  // Drop frames not destined for this RU or not originating from the configured O-DU, so the KPIs only count traffic
  // the O-RU sector would actually accept. Bytes 0-5 carry the destination MAC and bytes 6-11 the source MAC. This
  // mirrors the sector's Ethernet receive filter (\ref ru_message_receiver::should_ethernet_frame_be_filtered).
  if (!std::equal(cfg.ru_mac_address.begin(), cfg.ru_mac_address.end(), packet.begin())) {
    logger.debug("Dropping packet whose destination MAC is not this RU");
    return true;
  }
  if (!std::equal(cfg.du_mac_address.begin(), cfg.du_mac_address.end(), packet.begin() + ether::ETH_ADDR_LEN)) {
    logger.debug("Dropping packet whose source MAC is not the configured O-DU");
    return true;
  }

  ethernet_header_adjustment                     = 0;
  uint16_t                              eth_type = (uint16_t(packet[12]) << 8u) | packet[13];
  std::optional<ether::vlan_parameters> received_vlan;
  if (eth_type == VLAN_TPID) {
    if (packet.size() < ETH_HEADER_SIZE + ETH_VLAN_TAG_SIZE) {
      logger.debug("Dropping packet smaller than a VLAN Ethernet header");
      return true;
    }

    const uint16_t tci         = (uint16_t(packet[14]) << 8U) | packet[15];
    received_vlan              = ether::vlan_parameters{.tci_vid = static_cast<uint16_t>(tci & 0x0fffU),
                                                        .tci_pcp = static_cast<uint8_t>((tci >> 13U) & 0x07U)};
    ethernet_header_adjustment = ETH_VLAN_TAG_SIZE;
    eth_type                   = (uint16_t(packet[16]) << 8U) | packet[17];
  }

  // Match the sector receiver: PCP is a transmit QoS marking, not a receive filter, and an absent tag can mean that
  // the NIC or a VLAN sub-interface stripped it before delivery. Enforce only a VLAN identifier that was observed.
  if (received_vlan.has_value() && cfg.vlan_config.has_value() && received_vlan->tci_vid != cfg.vlan_config->tci_vid) {
    logger.debug("Dropping packet whose VLAN identifier does not match this RU");
    return true;
  }

  if (packet.size() < 26 + ethernet_header_adjustment) {
    logger.debug("Dropping packet too small to contain an Open Fronthaul header");
    return true;
  }

  // Verify the encapsulated Ethernet type is eCPRI.
  if (eth_type != ether::ECPRI_ETH_TYPE) {
    logger.debug("Dropping packet as it is not of eCPRI type");
    return true;
  }

  return false;
}

bool ru_emulator_stats_collector::decode_rx_message(rx_message_info&    message_info,
                                                    span<const uint8_t> packet,
                                                    unsigned            ethernet_header_adjustment) const
{
  auto filter_index = static_cast<filter_index_type>(packet[22 + ethernet_header_adjustment] & 0x0f);
  if (filter_index != filter_index_type::standard_channel_filter && !is_a_prach_message(filter_index)) {
    logger.warning("Packet is corrupt: unknown filter index = {} decoded", fmt::underlying(filter_index));
    return false;
  }
  message_info.filter_index = filter_index;

  // Decode and check message type.
  uint8_t type = packet[15 + ethernet_header_adjustment];
  if (type != uint8_t(ecpri::message_type::rt_control_data) && type != uint8_t(ecpri::message_type::iq_data)) {
    logger.warning("Packet is corrupt: unknown eCPRI message type = {} decoded", type);
    return false;
  }
  message_info.type = static_cast<ecpri::message_type>(type) == ecpri::message_type::rt_control_data
                          ? message_type::control_plane
                          : message_type::user_plane;

  message_info.direction = static_cast<data_direction>((packet[22 + ethernet_header_adjustment] & 0x80) >> 7u);
  logger.debug("Packet direction is {}", message_info.direction == data_direction::uplink ? "uplink" : "downlink");

  // Peek the timestamp.
  unsigned slot_id           = 0;
  unsigned symbol_id         = 0;
  uint8_t  frame             = packet[23 + ethernet_header_adjustment];
  uint8_t  subframe_and_slot = packet[24 + ethernet_header_adjustment];
  uint8_t  slot_and_symbol   = packet[25 + ethernet_header_adjustment];
  uint8_t  subframe          = subframe_and_slot >> 4;

  slot_id |= (subframe_and_slot & 0x0f) << 2;
  slot_id |= slot_and_symbol >> 6;
  symbol_id = slot_and_symbol & 0x3f;

  auto slot                 = slot_point(to_numerology_value(cfg.scs), frame, subframe, slot_id);
  message_info.symbol_point = {slot, symbol_id, MAX_NOF_SYMBOLS};

  // Peek the eAxC.
  message_info.eaxc = packet[19 + ethernet_header_adjustment];

  // Peek sequence identifier.
  message_info.seq_id = packet[20 + ethernet_header_adjustment];

  if (is_a_prach_message(message_info.filter_index)) {
    if (packet.size() <= 39 + ethernet_header_adjustment) {
      logger.warning("PRACH packet is corrupt: packet is too short");
      return false;
    }
    // Peek number of symbols.
    message_info.nof_symbols = (packet[39 + ethernet_header_adjustment] & 0x0f);
    // Peek compression header.
    message_info.compr_header = packet[33 + ethernet_header_adjustment];
    return true;
  }

  if (packet.size() <= 35 + ethernet_header_adjustment) {
    logger.warning("Open Fronthaul packet is corrupt: packet is too short");
    return false;
  }
  // Peek number of symbols.
  message_info.nof_symbols = (packet[35 + ethernet_header_adjustment] & 0x0f);
  // Peek compression header.
  message_info.compr_header = packet[28 + ethernet_header_adjustment];
  return true;
}

bool ru_emulator_stats_collector::validate_rx_ofh_params(const rx_message_info& message_info)
{
  if (!message_info.symbol_point.get_slot().valid() || !message_info.symbol_point.is_valid()) {
    logger.warning("Packet is corrupt: incorrect timestamp = {}:{}",
                   message_info.symbol_point.get_slot(),
                   message_info.symbol_point.get_symbol_index());
    return false;
  }

  if (message_info.direction == data_direction::downlink) {
    if (std::find(cfg.dl_eaxc.begin(), cfg.dl_eaxc.end(), message_info.eaxc) == cfg.dl_eaxc.end()) {
      logger.warning("Packet is corrupt: received eAxC = '{}' is not configured in the RU emulator DL ports list",
                     message_info.eaxc);
      return false;
    }
  }

  // Following parameters are only checked for UL C-Plane messages.
  if (message_info.direction != data_direction::uplink || message_info.type != message_type::control_plane) {
    return true;
  }

  const auto& eaxc = is_a_prach_message(message_info.filter_index) ? cfg.prach_eaxc : cfg.ul_eaxc;
  if (std::find(eaxc.begin(), eaxc.end(), message_info.eaxc) == eaxc.end()) {
    logger.warning("Packet is corrupt: received eAxC = '{}' is not configured in the RU emulator UL/PRACH ports list",
                   message_info.eaxc);
    return false;
  }

  if (!is_a_prach_message(message_info.filter_index) && (message_info.nof_symbols > MAX_NOF_SYMBOLS)) {
    logger.warning("Packet is corrupt: incorrect number of symbols = {}", message_info.nof_symbols);
    return false;
  }

  if (is_a_prach_message(message_info.filter_index)) {
    if (message_info.filter_index != cfg.prach_filter_index) {
      logger.warning("Packet is corrupt: incorrect PRACH filter index = {}, expected {}",
                     to_value(message_info.filter_index),
                     to_value(cfg.prach_filter_index));
      return false;
    }
    if (message_info.nof_symbols > cfg.nof_prach_symbols) {
      logger.warning("Packet is corrupt: incorrect number of PRACH symbols = {}, expected {} symbols",
                     message_info.nof_symbols,
                     cfg.nof_prach_symbols);
      return false;
    }
  }

  // For UL C-Plane message check also compression parameters.
  if (std::find(cfg.supported_ul_compr_hdr.begin(), cfg.supported_ul_compr_hdr.end(), message_info.compr_header) ==
      cfg.supported_ul_compr_hdr.end()) {
    logger.warning("Packet is corrupt: unsupported UL compression parameters = {}", message_info.compr_header);
    return false;
  }

  return true;
}

ru_emulator_rx_window_checker& ru_emulator_stats_collector::get_window_checker(const rx_message_info& message_info)
{
  return (message_info.direction == data_direction::uplink)   ? ul_cp_window_checker
         : (message_info.type == message_type::control_plane) ? dl_cp_window_checker
                                                              : dl_up_window_checker;
}

ru_emulator_seq_id_checker& ru_emulator_stats_collector::get_sequence_id_checker(const rx_message_info& message_info)
{
  return (message_info.direction == data_direction::uplink)
             ? is_a_prach_message(message_info.filter_index) ? prach_seq_id_checker : ul_cp_seq_id_checker
         : (message_info.type == message_type::control_plane) ? dl_cp_seq_id_checker
                                                              : dl_up_seq_id_checker;
}

void ru_emulator_stats_collector::on_frame(span<const uint8_t> payload)
{
  unsigned ethernet_header_adjustment = 0;
  if (should_packet_be_dropped(payload, cfg, logger, ethernet_header_adjustment)) {
    return dropped_counter.increment();
  }

  rx_message_info message_info;
  if (!decode_rx_message(message_info, payload, ethernet_header_adjustment)) {
    return corrupt_counter.increment();
  }

  if (!validate_rx_ofh_params(message_info)) {
    return corrupt_counter.increment();
  }

  rx_total_counter.increment();

  // Check SeqId field and update statistics for the messages received on time.
  get_window_checker(message_info).update_rx_window_statistics(message_info.symbol_point);
  get_sequence_id_checker(message_info)
      .update_statistics(message_info.eaxc, message_info.seq_id, message_info.symbol_point);
}

std::vector<ota_symbol_boundary_notifier*> ru_emulator_stats_collector::get_ota_notifiers()
{
  return {&dl_up_window_checker, &dl_cp_window_checker, &ul_cp_window_checker};
}

std::string ru_emulator_stats_collector::print_seq_id_err(span<const unsigned>        eaxc,
                                                          ru_emulator_seq_id_checker& seq_id_checker)
{
  fmt::memory_buffer stats_format_buf;
  for (unsigned i = 0, e = eaxc.size(); i != e; ++i) {
    fmt::format_to(std::back_inserter(stats_format_buf),
                   "{}{}",
                   seq_id_checker.calculate_statistics(eaxc[i]),
                   (i == e - 1) ? "" : "/");
  }
  return to_string(stats_format_buf);
}

void ru_emulator_stats_collector::print_statistics(unsigned emu_id)
{
  fmt::memory_buffer buffer;

  auto    now          = std::chrono::system_clock::now();
  std::tm current_time = fmt::gmtime(std::chrono::system_clock::to_time_t(now));

  // Fetch KPIs from window checkers.
  auto dl_up_kpi = dl_up_window_checker.get_statistics();
  auto dl_cp_kpi = dl_cp_window_checker.get_statistics();
  auto ul_cp_kpi = ul_cp_window_checker.get_statistics();

  uint64_t rx_total  = rx_total_counter.calculate_acc_value();
  uint64_t tx_total  = tx_total_counter.calculate_acc_value();
  uint64_t malformed = corrupt_counter.calculate_acc_value();
  uint64_t dropped   = dropped_counter.calculate_acc_value();

  fmt::format_to(std::back_inserter(buffer),
                 "| {:%H:%M:%S} | {:^3} | {:^11} | {:^11} | {:^11} | {:^11} | {:^15} | {:^13} | {:^13} | {:^13} | "
                 "{:^15} | {:^14} | {:^14} | {:^14} | {:^15} | {:^15} | {:^11} | {:^11} | {:^11} |\n",
                 current_time,
                 emu_id,
                 rx_total,
                 dl_up_kpi.rx_on_time,
                 dl_up_kpi.rx_early,
                 dl_up_kpi.rx_late,
                 print_seq_id_err(cfg.dl_eaxc, dl_up_seq_id_checker),
                 dl_cp_kpi.rx_on_time,
                 dl_cp_kpi.rx_early,
                 dl_cp_kpi.rx_late,
                 print_seq_id_err(cfg.dl_eaxc, dl_cp_seq_id_checker),
                 ul_cp_kpi.rx_on_time,
                 ul_cp_kpi.rx_early,
                 ul_cp_kpi.rx_late,
                 print_seq_id_err(cfg.ul_eaxc, ul_cp_seq_id_checker),
                 print_seq_id_err(cfg.prach_eaxc, prach_seq_id_checker),
                 malformed,
                 dropped,
                 tx_total);

  fmt::print(to_c_str(buffer));
}
