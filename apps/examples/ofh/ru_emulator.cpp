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

#include "helpers.h"
#include "ru_emulator_appconfig.h"
#include "ru_emulator_cli11_schema.h"
#include "ru_emulator_rx_window_checker.h"
#include "ru_emulator_seq_id_checker.h"
#include "ru_emulator_timing_notifier.h"
#include "ru_emulator_transceiver.h"
#include "lower_phy_factory.h"
#include "upper_phy_fake.h"
#include "ru_ofh_transmitter.h"
#include "../radio/radio_notifier_sample.h"
#include "ru_rx_symbol_handler.h"
#include "srsran/adt/circular_map.h"
#include "srsran/adt/expected.h"
#include "srsran/adt/to_array.h"
#include "srsran/adt/gps_clock.h"
#include "srsran/du/du_cell_config.h"
#include "./helpers/ru_config_translator.h"
#include "./helpers/ru_config.h"
#include "./helpers/worker_manager.h"
#include "./helpers/data_flow_uplane_uplink_factory.h"
#include "./helpers/timing_window_params.h"
#include "./support/uplink_context_repository.h"
#include "./support/prach_context_repository.h"
#include "srsran/radio/radio_configuration.h"
#include "srsran/radio/radio_factory.h"
#include "srsran/support/executors/task_worker.h"
#include "srsran/ofh/compression/compression_params.h"
#include "srsran/ofh/compression/compression_factory.h"
#include "srsran/phy/lower/lower_phy_controller.h"
#include "srsran/phy/adapters/phy_error_adapter.h"
#include "srsran/phy/adapters/phy_metrics_adapter.h"
#include "srsran/phy/adapters/phy_rg_gateway_adapter.h"
#include "srsran/phy/adapters/phy_rx_symbol_adapter.h"
#include "srsran/phy/adapters/phy_rx_symbol_request_adapter.h"
#include "srsran/phy/adapters/phy_timing_adapter.h"
#include "./decoders/ofh_uplane_rx_symbol_data_flow_writer.h"

#include "srsran/ofh/ecpri/ecpri_constants.h"
#include "srsran/ofh/ecpri/ecpri_packet_properties.h"
#include "srsran/ofh/ethernet/dpdk/dpdk_ethernet_factories.h"
#include "srsran/ofh/ofh_constants.h"
#include "srsran/ofh/ofh_factories.h"
#include "srsran/ofh/serdes/ofh_message_properties.h"
#include "srsran/ofh/serdes/ofh_serdes_factories.h"
#include "srsran/ran/cyclic_prefix.h"
#include "srsran/ran/resource_block.h"
#include "srsran/ran/slot_point.h"
#include "srsran/srslog/logger.h"
#include "srsran/support/config_parsers.h"
#include "srsran/support/executors/task_execution_manager.h"
#include "srsran/support/executors/task_executor.h"
#include "srsran/support/format/fmt_to_c_str.h"
#include "srsran/support/signal_handling.h"
#include "fmt/chrono.h"
#include <arpa/inet.h>
#include <random>
#include <uhd/types/time_spec.hpp>
#ifdef DPDK_FOUND
#include "srsran/hal/dpdk/dpdk_eal_factory.h"
#endif

using namespace srsran;
using namespace ofh;
using namespace ether;

/// Ethernet packet size.
static constexpr unsigned ETHERNET_FRAME_SIZE = 9000;

/// Depending on configured compression parameters one UL U-Plane message may occupy up to 2 Ethernet packets.
static constexpr size_t MAX_NOF_PACKETS_PER_UPLANE_MESSAGE = 2;

/// Supported values of udCmpHdr field used for UL U-Plane.
static constexpr auto SUPPORTED_UL_CMPR_HDR = to_array<uint8_t>({0x00, 0x91});

/// Flag that indicates if the application is running or being shutdown.
static std::atomic<bool> is_app_running = {true};

//static unsigned duration_slots = 60000;

namespace {

/// RU emulator configuration structure.
struct ru_emulator_config {
  /// Sector id.
  unsigned sector;
  /// Static uplink compression parameters.
  ru_compression_params ul_compr_params;
  /// Static downlink compression parameters.
  ru_compression_params dl_compr_params;
  /// Flag that indicates if the uplink static compression header is enabled.
  bool is_downlink_static_comp_hdr_enabled = true;
  /// RU emulator operating bandwidth.
  bs_channel_bandwidth bandwidth;
  /// Subcarrier spacing.
  subcarrier_spacing scs;
  /// Cell bandwidth in number of PRBs.
  unsigned nof_prb;
  /// RU emulator Ethernet MAC address.
  mac_address ru_mac;
  /// DU Ethernet MAC address.
  mac_address du_mac;
  /// VLAN tag.
  unsigned vlan_tag;
  /// Timing parameters.
  ru_window_timing_parameters timing_params;
  /// Sdr radio unit configs
  ru_sdr_unit_config sdr_unit_config;
  /// TDD configuration
  // std::optional<tdd_ul_dl_config_common> tdd_config;
  /// DL, UL and PRACH ports.
  std::vector<unsigned> dl_eaxc;
  std::vector<unsigned> ul_eaxc;
  std::vector<unsigned> prach_eaxc;
  /// Size of UL/DL context repository.
  unsigned repo_size;
  /// timeing notifier and logger are deleted from struct `ru_generic_configuration`
  /// Maximum number of PRACH concurrent requests.
  unsigned max_nof_prach_concurrent_requests = 11;

  /// Radio configuration.
  radio_configuration::radio radio_cfg;
  /// Lower PHY configurations. Was originally `std::vector<lower_phy_configuration>`
  lower_phy_configuration lower_phy_config;
  /// Fake Upper PHY configurations.
  upper_phy_fake::configuration upper_fake_config;
  /// Configurations to build uplink data flow.
  uplink_data_flow_config ul_data_flow_config;
  /// max DL processing delay in slots.
  unsigned max_processing_delay_slot = 5;
  /// Phy logger level. Default level: `warning`.
  srslog::basic_levels phy_log_level = srslog::basic_levels::info;
};

/// RU emulator dependencies.
struct ru_emulator_dependencies {
  /// Logger.
  srslog::basic_logger* logger = nullptr;
  /// RU emulators executor.
  task_executor* executor = nullptr;
  /// Ethernet transmitter.
  ru_emulator_transceiver* transceiver = nullptr;
  /// Sequence identifier checkers.
  std::unique_ptr<ru_emulator_seq_id_checker> dl_up_seq_id_checker;
  std::unique_ptr<ru_emulator_seq_id_checker> dl_cp_seq_id_checker;
  std::unique_ptr<ru_emulator_seq_id_checker> ul_cp_seq_id_checker;
  std::unique_ptr<ru_emulator_seq_id_checker> prach_seq_id_checker;
};

/// Helper structure used to group OFH header parameters.
struct header_parameters {
  uint8_t  port;
  unsigned payload_size;
  unsigned start_prb;
  unsigned nof_prbs;
};

/// Aggregates information received in a message from DU.
struct rx_message_info {
  unsigned            eaxc;
  data_direction      direction;
  filter_index_type   filter_index;
  message_type        type;
  slot_symbol_point   symbol_point{{}, 0, MAX_NOF_SYMBOLS};
  unsigned            nof_symbols;
  unsigned            nof_prbs;
  uint16_t            prb_start;
  subcarrier_spacing  scs;
  uint8_t             compr_header;
  uint8_t             seq_id;
  uint16_t            section_id;
  int                 freq_offset;
};

/// One symbol may require up to two byte buffers depending on configured compression parameters.
using symbol_buffer = static_vector<std::vector<uint8_t>, MAX_NOF_PACKETS_PER_UPLANE_MESSAGE>;

/// Array of symbol buffers, representing symbols of one eAxC.
using eaxc_buffers = static_vector<symbol_buffer, MAX_NOF_SYMBOLS>;

} // namespace

/*
static unsigned get_statistics_time_interval_in_slots(unsigned interval_seconds, subcarrier_spacing scs)
{
  return get_nof_slots_per_subframe(scs) * SUBFRAME_DURATION_MSEC * 1000 * interval_seconds;
}
*/

static std::unique_ptr<radio_session> build_radio(task_executor&              executor,
                                                  radio_notification_handler& radio_handler,
                                                  radio_configuration::radio& config,
                                                  const std::string&          device_driver)
{
  print_available_radio_factories();

  std::unique_ptr<radio_factory> factory = create_radio_factory(device_driver);
  if (!factory) {
  return nullptr;
  }
  /*
  if (!factory->get_configuration_validator().is_configuration_valid(config)) {
  report_error("Invalid radio configuration.\n");
  }*/

  return factory->create(config, executor, radio_handler);
}

/// Returns structure with RU emulators dependencies.
static ru_emulator_dependencies resolve_ru_emulator_dependencies(srslog::basic_logger&    logger,
                                                                 task_executor&           executor,
                                                                 ru_emulator_transceiver& transceiver
                                                                )
{
  ru_emulator_dependencies dependencies;

  dependencies.logger      = &logger;
  dependencies.executor    = &executor;
  dependencies.transceiver = &transceiver;
  //dependencies.rx_symbol_writer = &rx_symbol_writer;
  dependencies.dl_cp_seq_id_checker =
      std::make_unique<ru_emulator_seq_id_checker>("DL CP", logger, ofh::create_sequence_id_checker());
  dependencies.dl_up_seq_id_checker =
      std::make_unique<ru_emulator_seq_id_checker>("DL UP", logger, ofh::create_sequence_id_checker());
  dependencies.ul_cp_seq_id_checker =
      std::make_unique<ru_emulator_seq_id_checker>("UL CP", logger, ofh::create_sequence_id_checker());
  dependencies.prach_seq_id_checker =
      std::make_unique<ru_emulator_seq_id_checker>("PRACH", logger, ofh::create_sequence_id_checker());

  return dependencies;
}

/// @brief Generate uplink data flow configurations from ru configurations.
/// @return struct 'uplink_data_flow_config'.
uplink_data_flow_config generate_uplink_data_config(ru_emulator_config emu_cfg, ru_emulator_ofh_appconfig ru_cfg){
  uplink_data_flow_config config;
  config.bw                                 = emu_cfg.bandwidth;
  config.sector                             = emu_cfg.sector;
  config.scs                                = emu_cfg.lower_phy_config.scs;
  config.cp                                 = emu_cfg.lower_phy_config.cp;
  config.mac_dst_address                    = emu_cfg.du_mac;
  config.mac_src_address                    = emu_cfg.ru_mac;
  config.mtu_size                           = units::bytes{ETHERNET_FRAME_SIZE};
  config.tci_cp                             = emu_cfg.vlan_tag;
  config.tci_up                             = emu_cfg.vlan_tag;
  config.ru_working_bw                      = emu_cfg.bandwidth;
  config.ul_compr_params                    = emu_cfg.ul_compr_params;
  config.is_uplink_static_compr_hdr_enabled = ru_cfg.is_uplink_static_comp_hdr_enabled;
  config.iq_scaling                         = ru_cfg.iq_scaling;
  auto n = emu_cfg.ul_eaxc.size();
  auto m = emu_cfg.prach_eaxc.size();
  srsran_assert(n <= MAX_NOF_SUPPORTED_EAXC, "Number of exac ({}) exceeds threshold of supported exac.", n);
  srsran_assert(m <= MAX_NOF_SUPPORTED_EAXC, "Number of prach exac ({}) exceeds threshold of supported exac.", m);
  config.ul_eaxc.assign(emu_cfg.ul_eaxc.begin(), emu_cfg.ul_eaxc.end());
  config.prach_eaxc.assign(emu_cfg.prach_eaxc.begin(), emu_cfg.prach_eaxc.end());
  return config;
}

/// @brief Checks if the dst MAC address of the received packet matches the RU's MAC address.
/// @param packet The packet received on Ethernet port.
/// @param logger RU emulator logger.
/// @param ru_mac RU emulator's MAC address.
/// @return True if matches. False otherwise.
static bool valid_mac_addr(span<const uint8_t> packet, srslog::basic_logger& logger, const mac_address& ru_mac){
  mac_address mac{};
  std::memcpy(mac.data(), packet.data(), mac.size());
  // Drop packets that are not destinated to this RU.
  if (!compare_mac_addresses(mac, ru_mac)){
    logger.debug("Dropping packet as it is not destinated to the RU");
    return false;
  }

  return true;
}

/// \brief Checks whether received OFH packet should be dropped or processed by the RU emulator.
///
/// Packet must be dropped if it is not an eCPRI OFH packet.
///
/// \param packet A packet received on Ethernet port.
/// \param logger RU emulator's logger instance.
/// \param ru_mac RU emulator's MAC address.
///
/// \return true if packet should be dropped, false otherwise.
static bool should_packet_be_dropped(span<const uint8_t> packet, srslog::basic_logger& logger, const mac_address& ru_mac)
{
  // Drop non OFH packet.
  if (packet.size() < 26) {
    logger.debug("Dropping packet of size smaller than 26 bytes");
    return true;
  }

  // Verify the Ethernet type is eCPRI.
  uint16_t eth_type = (uint16_t(packet[12]) << 8u) | packet[13];
  if (eth_type != ECPRI_ETH_TYPE) {
    logger.debug("Dropping packet as it is not of eCPRI type");
    return true;
  }

  return false;
}

/// \brief Analyzes content of received OFH packets.
///
/// \param message_info  Decoded message parameters.
/// \param packet        A packet received on Ethernet port.
/// \param logger        RU emulator's logger instance.
///
/// \return true if passed message was decoded successfully, false otherwise.
static bool decode_rx_message(rx_message_info& message_info, span<const uint8_t> packet, ru_emulator_config& cfg, srslog::basic_logger& logger)
{
  // Decode and check the filter index in the byte 26, bits 0-3.
  auto filter_index = static_cast<filter_index_type>(packet[22] & 0x0f);
  if (filter_index != filter_index_type::standard_channel_filter && !is_a_prach_message(filter_index)) {
    logger.warning("Packet is corrupt: unknown filter index = {} decoded", fmt::underlying(filter_index));
    return false;
  }
  message_info.filter_index = filter_index;

  // Decode and check message type.
  uint8_t type = packet[15];
  if (type != uint8_t(ecpri::message_type::rt_control_data) && type != uint8_t(ecpri::message_type::iq_data)) {
    logger.warning("Packet is corrupt: unknown eCPRI message type = {} decoded", type);
    return false;
  }
  message_info.type = static_cast<ecpri::message_type>(type) == ecpri::message_type::rt_control_data
                          ? message_type::control_plane
                          : message_type::user_plane;

  // Decode direction, which is codified in the byte 26, bit 7.
  message_info.direction = static_cast<data_direction>((packet[22] & 0x80) >> 7u);
  logger.debug("Packet direction is {}", message_info.direction == data_direction::uplink ? "uplink" : "downlink");

  // Peek the eAxC.
  message_info.eaxc = packet[19];
  // Peek sequence identifier.
  message_info.seq_id = packet[20];
  // Peek the timestamp. 'symbol_id' also serves as 'start_symbol' in cp packets.
  unsigned slot_id           = 0;
  unsigned symbol_id         = 0;
  uint8_t  frame             = packet[23];
  uint8_t  subframe_and_slot = packet[24];
  uint8_t  slot_and_symbol   = packet[25];
  uint8_t  subframe          = subframe_and_slot >> 4;

  slot_id |= (subframe_and_slot & 0x0f) << 2;
  slot_id |= slot_and_symbol >> 6;
  symbol_id = slot_and_symbol & 0x3f;

  auto slot                 = slot_point(to_numerology_value(cfg.scs), frame, subframe, slot_id);
  message_info.symbol_point = {slot, symbol_id, MAX_NOF_SYMBOLS};

  if( message_info.type == message_type::control_plane) {
    /// Section 1 Uplink C-Plane packet.
    if(!is_a_prach_message(message_info.filter_index)){
      // Peek number of PRBs.
      message_info.nof_prbs = packet[33];
      // Peek start_prb.
      message_info.prb_start = ((uint16_t)packet[31]<<8 | (uint16_t)packet[32]) & 0x03ff;
      // Peek number of symbols.
      message_info.nof_symbols = (packet[35] & 0xf);
      // Peek compression header.
      message_info.compr_header = packet[28];
      // Peek section id.
      message_info.section_id = (uint16_t(packet[30])<<4) | (uint16_t(packet[31])>>4);
    }
    /// Section 3 Uplink PRACH packet.
    else{
      message_info.nof_prbs = packet[37];
      message_info.prb_start = ((uint16_t)packet[35]<<8 | (uint16_t)packet[36]) & 0x03ff;
      message_info.nof_symbols = packet[39] & 0xf;
      // Peek subcarrier spacing.
      message_info.scs = static_cast<subcarrier_spacing>(packet[30] & 0x0f);
      message_info.compr_header = packet[33];
      message_info.section_id = (uint16_t(packet[34])<<4) | (uint16_t(packet[35])>>4);
      // Peek frequency offset.
      uint32_t u_freq = uint32_t(packet[42])<<16 | uint32_t(packet[43])<<8 | uint32_t(packet[44]);
      if(u_freq & 0x00800000) u_freq |= 0xff000000;
      message_info.freq_offset = static_cast<int>(u_freq);
    }
  }
  // if(message_info.type==message_type::control_plane) logger.warning("received {} C-Plane packet for Slot {}", 
  //                                                                   message_info.direction==data_direction::uplink? "uplink":"downlink",
  //                                                                   slot);  
  // else{
  //   logger.warning("received U-Plane packet for Slot {}, symbol {}", slot, symbol_id);
  // }
  return true;
}

namespace {
  using ::is_app_running;
/// RU emulator receives OFH traffic and replies with UL packets to a DU.
class ru_emulator : public frame_notifier
{
  using kpi_counter = ru_emu_stats::kpi_counter;

  srslog::basic_logger&    logger;
  task_executor&           executor;
  ru_emulator_transceiver& transceiver;
  // RU emulator configuration.
  ru_emulator_config cfg;

  std::shared_ptr<ofh_transmitter_impl>  ofh_transmitter;
  std::unique_ptr<lower_phy> low_phy;
  std::unique_ptr<radio_session> radio;
  std::unique_ptr<upper_phy_fake> upper_fake;
  /// A repo to store downlink resource grids.
  std::shared_ptr<downlink_context_repository> dl_context_repo;
  /// A repo to store uplink contexts.
  std::shared_ptr<uplink_cplane_context_repository> ul_context_repo;
  /// A repo to store uplink PRACH contexts.
  std::shared_ptr<uplink_cplane_context_repository> prach_cp_repo;
  /// A repo to store PRACH buffer contexts.
  std::shared_ptr<prach_context_repository> prach_context_repo;
  /// Downlink resource grid pool.
  std::unique_ptr<resource_grid_pool> dl_rg_pool;
  /// Uplink PRACH buffer pool.
  std::unique_ptr<prach_buffer_pool> prach_pool;
  /// Uplink ethernet frame pool.
  std::shared_ptr<ether::eth_frame_pool> ul_frame_pool;

  // Timing window checkers, store statistics of early/late/on-time packets.
  ru_emulator_rx_window_checker dl_cp_window_checker;
  ru_emulator_rx_window_checker dl_up_window_checker;
  ru_emulator_rx_window_checker ul_cp_window_checker;
  
  // Keeps track of last used seq_id for each eAxC.
  static_circular_map<unsigned, uint8_t, MAX_SUPPORTED_EAXC_ID_VALUE> seq_counters;
  // Stores the list of configured eAxC for uplink, downlink and PRACH.
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> ul_eaxc;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> dl_eaxc;
  static_vector<unsigned, MAX_NOF_SUPPORTED_EAXC> prach_eaxc;

  // Other KPI counters.
  kpi_counter rx_total_counter;
  kpi_counter tx_total_counter;
  kpi_counter corrupt_counter;
  kpi_counter dropped_counter;


  // Sequence identifier checkers.
  std::unique_ptr<ru_emulator_seq_id_checker> dl_up_seq_id_checker;
  std::unique_ptr<ru_emulator_seq_id_checker> dl_cp_seq_id_checker;
  std::unique_ptr<ru_emulator_seq_id_checker> ul_cp_seq_id_checker;
  std::unique_ptr<ru_emulator_seq_id_checker> prach_seq_id_checker;

  // Section data decoder.
  std::unique_ptr<uplane_message_decoder> uplane_section_decoder;
  // Writes IQ data received in an Open Fronthaul message to the corresponding resource grid.
  uplane_rx_symbol_data_flow_writer rx_symbol_writer;
  // Radio notification handler.
  radio_notifier_spy notification_handler;
  /// UL symbol handler that extracts iq samples from UL rg and then builds UL messages.
  ru_rx_symbol_handler rx_symbol_handler;
  // Create lower_phy adapters.
  phy_error_adapter             error_adapter;
  phy_metrics_adapter           metrics_adapter;
  phy_rx_symbol_adapter         rx_symbol_adapter;
  phy_rg_gateway_adapter        rg_gateway_adapter;
  phy_timing_adapter            timing_adapter;
  // This may be for uplink rx symbols.
  phy_rx_symbol_request_adapter phy_rx_symbol_req_adapter;
  unsigned tmp_file_counter = 0;

public:
  ru_emulator(ru_emulator_dependencies&& dependencies, ru_emulator_config cfg_, worker_manager& workers_) :
    logger(*dependencies.logger),
    executor(*dependencies.executor),
    transceiver(*dependencies.transceiver),
    cfg(cfg_),
    dl_context_repo(std::make_shared<downlink_context_repository>(cfg.repo_size)),
    ul_context_repo(std::make_shared<uplink_cplane_context_repository>(cfg.repo_size)),
    prach_cp_repo(std::make_shared<uplink_cplane_context_repository>(cfg.repo_size)),
    prach_context_repo(std::make_shared<prach_context_repository>(cfg.repo_size)),
    dl_rg_pool(create_rg_pool(cfg.upper_fake_config, logger)),
    prach_pool(create_prach_pool(cfg.upper_fake_config)),
    ul_frame_pool(create_eth_frame_pool(cfg_.ul_data_flow_config, logger)),
    dl_cp_window_checker({cfg_.timing_params.sym_cp_dl_end, cfg_.timing_params.sym_cp_dl_start}),
    dl_up_window_checker({cfg_.timing_params.sym_up_dl_end, cfg_.timing_params.sym_up_dl_start}),
    ul_cp_window_checker({cfg_.timing_params.sym_cp_ul_end, cfg_.timing_params.sym_cp_ul_start}),

    dl_up_seq_id_checker(std::move(dependencies.dl_up_seq_id_checker)),
    dl_cp_seq_id_checker(std::move(dependencies.dl_cp_seq_id_checker)),
    ul_cp_seq_id_checker(std::move(dependencies.ul_cp_seq_id_checker)),
    prach_seq_id_checker(std::move(dependencies.prach_seq_id_checker)),

    rx_symbol_writer(cfg.dl_eaxc, cfg_.sector, *dependencies.logger, dl_context_repo),
    notification_handler(cfg.radio_cfg.log_level),
    rx_symbol_handler(cfg.radio_cfg.log_level,  ul_context_repo, prach_cp_repo, cfg.ul_data_flow_config, ul_frame_pool, *workers_.lower_phy_ul_exec[cfg.sector]),
    // rx_symbol_handler(cfg.radio_cfg.log_level),
    error_adapter(*cfg.lower_phy_config.logger)
  {
    for (auto eaxc : cfg.dl_eaxc) {
      srsran_assert(eaxc <= MAX_SUPPORTED_EAXC_ID_VALUE, "Unsupported DL eAxC value requested");
      dl_eaxc.push_back(eaxc);
    }

    for (auto eaxc : cfg.ul_eaxc) {
      srsran_assert(eaxc <= MAX_SUPPORTED_EAXC_ID_VALUE, "Unsupported UL eAxC value requested");
      ul_eaxc.push_back(eaxc);
      seq_counters.insert(eaxc, 0);
    }

    for (auto eaxc : cfg.prach_eaxc) {
      srsran_assert(eaxc <= MAX_SUPPORTED_EAXC_ID_VALUE, "Unsupported DL eAxC value requested");
      prach_eaxc.push_back(eaxc);
    }

    // Create radio.
    radio = build_radio(*workers_.radio_exec, notification_handler, cfg.radio_cfg, cfg.sdr_unit_config.device_driver);
    report_error_if_not(radio, "Unable to create radio session.");
  
    
    // low_cfg is struct `lower_phy_configuration`.
    lower_phy_configuration& low_cfg = cfg.lower_phy_config;
    // Move the executors to `lower_phy_configuration`.
    low_cfg.tx_task_executor         = workers_.lower_phy_tx_exec[cfg.sector];
    low_cfg.rx_task_executor         = workers_.lower_phy_rx_exec[cfg.sector];
    low_cfg.dl_task_executor         = workers_.lower_phy_dl_exec[cfg.sector];
    low_cfg.ul_task_executor         = workers_.lower_phy_ul_exec[cfg.sector];
    low_cfg.prach_async_executor     = workers_.lower_prach_exec[cfg.sector];

    low_cfg.logger->set_level(cfg.phy_log_level);

    ofh_transmitter = std::make_shared<ofh_transmitter_impl>(logger, cfg.timing_params, transceiver.get_transmitter(), ul_frame_pool, ul_context_repo, prach_cp_repo, &tx_total_counter);

    low_cfg.error_notifier = &error_adapter;
    low_cfg.metric_notifier = &metrics_adapter;
    low_cfg.rx_symbol_notifier = &rx_symbol_adapter;
    low_cfg.timing_notifier = &timing_adapter;

    // Create lower_phy factory.
    auto lphy_factory = create_lower_phy_factory(low_cfg, cfg.max_nof_prach_concurrent_requests);
    report_error_if_not(lphy_factory, "Failed to create lower PHY factory.");

    // Connect radio to lower_phy baseband gateway.
    low_cfg.bb_gateway         = &radio->get_baseband_gateway(cfg.sector);
    
    // Create lower_phy.
    low_phy = lphy_factory->create(low_cfg);
    report_error_if_not(low_phy, "Unable to create lower PHY.");

    // Create upper_fake.
    cfg.upper_fake_config.gateway = &rg_gateway_adapter;
    cfg.upper_fake_config.rx_symb_req_notifier = &phy_rx_symbol_req_adapter;
    cfg.upper_fake_config.dl_slot_repo = dl_context_repo;
    cfg.upper_fake_config.prach_context_repo = prach_context_repo;
    
    upper_fake = upper_phy_fake::create(cfg.upper_fake_config);

    // See `radio_ssb.cpp`.
    rx_symbol_adapter.connect(&rx_symbol_handler);
    // To call `on_tti_boundary()`.
    timing_adapter.connect(upper_fake.get());
    // Connect the adaptors.
    rg_gateway_adapter.connect(&low_phy->get_rg_handler());
    // For uplink(?).
    phy_rx_symbol_req_adapter.connect(&low_phy->get_request_handler());

    std::array<std::unique_ptr<ofh::iq_decompressor>, ofh::NOF_COMPRESSION_TYPES_SUPPORTED> decompr;
    for (unsigned i = 0; i != ofh::NOF_COMPRESSION_TYPES_SUPPORTED; ++i) {
      decompr[i] = create_iq_decompressor(static_cast<ofh::compression_type>(i), logger);
    }

    uplane_section_decoder = (cfg.is_downlink_static_comp_hdr_enabled)
                              ? ofh::create_static_compr_method_ofh_user_plane_packet_decoder(
                                logger,
                                cfg.scs,
                                cyclic_prefix::NORMAL,
                                cfg.nof_prb,
                                cfg.sector,
                                create_iq_decompressor_selector(std::move(decompr)),
                                cfg.dl_compr_params)
                              : ofh::create_dynamic_compr_method_ofh_user_plane_packet_decoder(
                                logger,
                                cfg.scs,
                                cyclic_prefix::NORMAL,
                                cfg.nof_prb,
                                cfg.sector,
                                create_iq_decompressor_selector(std::move(decompr)));
  }

  // See interface for documentation.
  void on_new_frame(unique_rx_buffer buffer) override
  {
    if (!executor.execute([this, b = std::move(buffer)]() mutable { process_new_frame(std::move(b)); })) {
      logger.warning("Failed to dispatch receiver task");
    }
  }

  void start() {
    transceiver.start(*this); 
    
    // double                     delay_s      = 0.1;
    baseband_gateway_timestamp radio_current_time = radio->read_current_time();
    
    fmt::print("radio_current_time = {}\n", radio_current_time);
    baseband_gateway_timestamp radio_start_time = radio_current_time;//+ static_cast<uint64_t>(delay_s * cfg.radio_cfg.sampling_rate_hz);

    
    // Round start time to the next subframe.
    uint64_t sf_duration = static_cast<uint64_t>(cfg.radio_cfg.sampling_rate_hz / 1e3);
    radio_start_time           = divide_ceil(radio_start_time, sf_duration) * sf_duration;
    
    fmt::print("radio_start_time(real) : {}, {}(ticks)\n", uhd::time_spec_t::from_ticks(radio_start_time, radio->get_actual_srate()).get_real_secs(), radio_start_time);
    // Start Processing.
    radio->start(radio_start_time);
    low_phy->get_controller().start(radio_start_time);
    // Receive and transmit per block basis.
    for (;is_app_running.load();) {
      // Wait for PHY to detect a TTI boundary.
      upper_fake->wait_tti_boundary();
    }
  }

  void stop(){
    transceiver.stop();

    if (radio != nullptr) {
      radio->stop();
    }

    upper_fake->stop();

    if (low_phy != nullptr) {
      low_phy->get_controller().stop();
    }

    low_phy.reset();
    upper_fake.reset();
    radio.reset();

  }

  void print_statistics(unsigned emu_id)
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
                   print_seq_id_err(dl_eaxc, *dl_up_seq_id_checker),
                   dl_cp_kpi.rx_on_time,
                   dl_cp_kpi.rx_early,
                   dl_cp_kpi.rx_late,
                   print_seq_id_err(dl_eaxc, *dl_cp_seq_id_checker),
                   ul_cp_kpi.rx_on_time,
                   ul_cp_kpi.rx_early,
                   ul_cp_kpi.rx_late,
                   print_seq_id_err(ul_eaxc, *ul_cp_seq_id_checker),
                   print_seq_id_err(prach_eaxc, *prach_seq_id_checker),
                   malformed,
                   dropped,
                   tx_total);

    fmt::print(to_c_str(buffer));
  }

  std::vector<ofh::ota_symbol_boundary_notifier*> get_ota_notifiers()
  {
    std::vector<ofh::ota_symbol_boundary_notifier*> notifiers;
    notifiers.push_back(&dl_up_window_checker);
    notifiers.push_back(&dl_cp_window_checker);
    notifiers.push_back(&ul_cp_window_checker);
    notifiers.push_back(ofh_transmitter.get());

    return notifiers;
  }

private:
  /// Decodes and processes received OFH message.
  void process_new_frame(unique_rx_buffer buffer)
  {
    span<const uint8_t> payload = buffer.data();

    // Filtering out packets that are not destinated to this RU.
    if(!valid_mac_addr(payload, logger, cfg.ru_mac)) {
      return;
    }

    if (should_packet_be_dropped(payload, logger, cfg.ru_mac)) {
      return dropped_counter.increment();
    }

    rx_message_info message_info;
    if (!decode_rx_message(message_info, payload, cfg, logger)) {
      return corrupt_counter.increment();
    }

    if (!validate_rx_ofh_params(message_info)) {
      return corrupt_counter.increment();
    }

    rx_total_counter.increment();

    // Check SeqId field and update statistics for the messages received on time.
    get_window_checker(message_info).update_rx_window_statistics(message_info.symbol_point);
    update_seq_id_statistics(message_info);

    // decode message received from the DU.
    decode_section_data(payload, message_info);

    // Send uplink packets.
    if (is_ul_request(message_info)) {
      enqueue_ul_context(message_info);
    }
  }

  /// @brief Enqueue uplink context info into 'ul_context_repo'.
  /// @param message_info Packet header.
  void enqueue_ul_context(const rx_message_info& message_info){
    cplane_radio_application_header radio_hdr;
    radio_hdr.direction         = message_info.direction;
    radio_hdr.filter_index      = message_info.filter_index;
    radio_hdr.slot              = message_info.symbol_point.get_slot();
    radio_hdr.start_symbol      = message_info.symbol_point.get_symbol_index();

    ul_cplane_context context;
    context.radio_hdr         = radio_hdr;
    context.nof_prb           = message_info.nof_prbs;
    context.nof_symbols       = (uint8_t)message_info.nof_symbols;
    context.prb_start         = message_info.prb_start;
    context.section_id        = message_info.section_id;
    
    if(!is_a_prach_message(message_info.filter_index)){
      ul_context_repo->add(radio_hdr.slot, message_info.eaxc, context);
    }
    else{
      prach_cp_repo->add(radio_hdr.slot, message_info.eaxc, context);
    }
  }

  /// @brief Decompress IQ data for further use.
  /// @param packet OFH packet received from the DU
  /// @param message_info Decoded packet headers, including compression header.
  void decode_section_data(span<const uint8_t> packet, const rx_message_info& message_info){
    if (message_info.direction == data_direction::downlink && message_info.type == message_type::control_plane){
      // A new slot point for downlink.
      slot_point slot = message_info.symbol_point.get_slot();
      // Allocate a resource grid from the pool for writing.
      shared_resource_grid rg = dl_rg_pool->allocate_resource_grid(slot);

      // Send to dl_slot_repo.
      resource_grid_context ctx = {slot, cfg.sector};
      ofdm_symbol_range symbol_range = ofdm_symbol_range(message_info.symbol_point.get_symbol_index(), message_info.nof_symbols);
      // logger.warning("DL slot {}: symbol [{}, {}]\n", slot, symbol_range.start(), symbol_range.stop());
      const downlink_context& dl_ctx = dl_context_repo->get(slot, 0);
      if(dl_ctx.empty()){
        dl_context_repo->add(ctx, rg, symbol_range);
      }
    }
    else if(message_info.direction == data_direction::downlink && message_info.type == message_type::user_plane){
      uplane_message_decoder_results results;
      if (!uplane_section_decoder->decode(results, packet.subspan(22, packet.size()-22))) {
        logger.warning("Failure of decoding packets");
      }
      // Write to dl_slot_repo.
      rx_symbol_writer.write_to_resource_grid(message_info.eaxc, results);
      // if(message_info.symbol_point.get_slot().sfn()==90 && message_info.symbol_point.get_slot().subframe_slot_index()==0 &&
      //    message_info.symbol_point.get_slot().subframe_index()==0 && message_info.symbol_point.get_symbol_index()==5){
      //   // Print the 1st downlink uplane packet for varification.
      //   print_1st_rx_packet(results);
      //   dump_hex(packet);
      // }
    }
    else if(is_a_prach_message(message_info.filter_index)){
      // A new slot for uplink PRACH.
      slot_point slot = message_info.symbol_point.get_slot();
      // Calculate the PRACH frequency start in RBs.
      double total_bw_Hz        
              = 1000 * scs_to_khz(cfg.scs) * cfg.upper_fake_config.max_nof_prb * NOF_SUBCARRIERS_PER_RB;
      double freq_offset_Hz     
              = (ra_scs_to_Hz(to_ra_subcarrier_spacing(message_info.scs))/2) * static_cast<double>(message_info.freq_offset);
      double offset_to_prach_Hz = total_bw_Hz/2 + freq_offset_Hz;
      int prach_start_re 
              = static_cast<unsigned>(offset_to_prach_Hz / ra_scs_to_Hz(to_ra_subcarrier_spacing(message_info.scs)));
      unsigned K = (1000 * scs_to_khz(cfg.scs)) / ra_scs_to_Hz(to_ra_subcarrier_spacing(message_info.scs));
      // Construct an empty PRACH buffer.
      prach_buffer& buffer = prach_pool->get_prach_buffer();
      // Construct a PRACH buffer context and add to the repo.
      prach_buffer_context prach_ctx;
      prach_ctx.sector                = cfg.sector;
      prach_ctx.slot                  = slot;
      unsigned n                      = cfg.prach_eaxc.size();
      for(unsigned i=0; i<n; i++) prach_ctx.ports.push_back(i);
      prach_ctx.start_symbol          = 0;
      prach_ctx.format                = prach_format_type::B4;
      prach_ctx.rb_offset             = uint16_t((prach_start_re / K) / NOF_SUBCARRIERS_PER_RB);   // prach_frequency_start
      prach_ctx.nof_td_occasions      = 1;
      prach_ctx.nof_fd_occasions      = 1;   // max_nof_fd_occasions = 1?
      prach_ctx.nof_prb_ul_grid       = cfg.upper_fake_config.max_nof_prb;
      prach_ctx.pusch_scs             = to_subcarrier_spacing(slot.numerology());
      prach_ctx.root_sequence_index   = 1;
      prach_ctx.restricted_set        = restricted_set_config::UNRESTRICTED;
      prach_ctx.zero_correlation_zone = 0;
      prach_ctx.start_preamble_index  = 0;
      prach_ctx.nof_preamble_indices  = 64; 

      prach_context ctx = prach_context_repo->get(slot);
      if(ctx.empty()){
        prach_context_repo->add(prach_ctx, buffer, message_info.symbol_point.get_symbol_index(), slot);
      }
    }
    else return;
  }

  /// Print the 1st downlink uplane packet for varification.
  void print_1st_rx_packet(srsran::ofh::uplane_message_decoder_results results) {
    if (results.sections.empty()) {
      std::cout << "No sections found in the received packet." << std::endl;
      return;
    }
    // std::string tmp_file = "/tmp/RU_frame" + std::to_string(tmp_file_counter) + ".txt";
    // std::ofstream ofs(tmp_file, std::ios::out);
    // Sections
    for (size_t sec_idx = 0; sec_idx < results.sections.size(); ++sec_idx) {
      const auto& section = results.sections[sec_idx];
      
      // IQ samples in a section
      for (size_t i = 0; i < section.iq_samples.size(); ++i) {
        const auto& sample = section.iq_samples[i];
        
        float real_val = to_float(sample.real);
        float imag_val = to_float(sample.imag);
        // ofs << fmt::format("Sample {} : real= {}, imag = {}\n", i, real_val, imag_val);
        fmt::print("Sample {} : real= {}, imag = {}\n", i, real_val, imag_val);
      }
    }
    fmt::print("New packet:\n");
  }

  /// Dumps the content of the packet in hex format to a file.
  void dump_hex(span<const uint8_t> packet)
  {
    std::string tmp_file = "/tmp/RU_frame" + std::to_string(tmp_file_counter++) + ".txt";
    std::ofstream ofs(tmp_file, std::ios::out);
    for (size_t i = 0; i < packet.size(); ++i) {
        ofs << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(packet[i]) << " ";
        if ((i + 1) % 16 == 0) {
            ofs << "\n";
        }
    }
    ofs << std::dec;
    ofs.close();
  }

  /// Returns window checker for OFH messages of corresponding type given by \c message_info parameter.
  ru_emulator_rx_window_checker& get_window_checker(const rx_message_info& message_info)
  {
    return (message_info.direction == data_direction::uplink)   ? ul_cp_window_checker
           : (message_info.type == message_type::control_plane) ? dl_cp_window_checker
                                                                : dl_up_window_checker;
  }

  /// Return sequence identifier checker depending on the given \c message_info parameter.
  ru_emulator_seq_id_checker& get_sequence_id_checker(const rx_message_info& message_info)
  {
    return (message_info.direction == data_direction::uplink)
               ? is_a_prach_message(message_info.filter_index) ? *prach_seq_id_checker : *ul_cp_seq_id_checker
           : (message_info.type == message_type::control_plane) ? *dl_cp_seq_id_checker
                                                                : *dl_up_seq_id_checker;
  }

  bool is_ul_request(const rx_message_info& message_info)
  {
    return (message_info.direction == data_direction::uplink) && (message_info.type == message_type::control_plane);
  }

  /// Returns string containing sequence identifier errors for the given list of eAxCs collected by the specified
  /// sequence identifier checker object.
  std::string print_seq_id_err(span<const unsigned> eaxc, ru_emulator_seq_id_checker& seq_id_checker)
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

  void update_seq_id_statistics(const rx_message_info& message_info)
  {
    get_sequence_id_checker(message_info)
        .update_statistics(message_info.eaxc, message_info.seq_id, message_info.symbol_point);
  }

  /// \brief Validates decoded message parameters.
  ///
  /// \param message_info Decoded message parameters.
  /// \return false if decoded parameters are invalid (a packet is considered corrupt then), true otherwise.
  ///
  /// \note packet is considered corrupt if any of the decoded parameters has undefined in the ORAN specification
  /// value, unsupported value (e.g. compression parameters) or unconfigured value (e.g. eAxC value).
  bool validate_rx_ofh_params(rx_message_info& message_info)
  {
    if (!message_info.symbol_point.get_slot().valid() || !message_info.symbol_point.is_valid()) {
      logger.warning("Packet is corrupt: incorrect timestamp = {}:{}",
                     message_info.symbol_point.get_slot(),
                     message_info.symbol_point.get_symbol_index());
      return false;
    }

    if (message_info.direction == data_direction::downlink) {
      if (std::find(dl_eaxc.begin(), dl_eaxc.end(), message_info.eaxc) == dl_eaxc.end()) {
        logger.warning("Packet is corrupt: received eAxC = '{}' is not configured in the RU emulator DL ports list",
                       message_info.eaxc);
        return false;
      }
    }

    // Following parameters are only checked for UL C-Plane messages.
    if (message_info.direction != data_direction::uplink || message_info.type != message_type::control_plane) {
      return true;
    }

    const auto& eaxc = is_a_prach_message(message_info.filter_index) ? prach_eaxc : ul_eaxc;
    if (std::find(eaxc.begin(), eaxc.end(), message_info.eaxc) == eaxc.end()) {
      logger.warning("Packet is corrupt: received eAxC = '{}' is not configured in the RU emulator UL ports list",
                     message_info.eaxc);
      return false;
    }

    if (message_info.nof_symbols > MAX_NOF_SYMBOLS) {
      logger.warning("Packet is corrupt: incorrect number of symbols = {}", message_info.nof_symbols);
      return false;
    }

    // For UL C-Plane message check also compression parameters.
    if (!is_a_prach_message(message_info.filter_index) &&
        std::find(SUPPORTED_UL_CMPR_HDR.begin(), SUPPORTED_UL_CMPR_HDR.end(), message_info.compr_header) ==
            SUPPORTED_UL_CMPR_HDR.end()) {
      logger.warning("Packet is corrupt: unsupported UL compression parameters = {}", message_info.compr_header);
      return false;
    }

    return true;
  }

  void set_runtime_header_params(span<uint8_t> frame, slot_point slot, unsigned symbol, unsigned eaxc)
  {
    // Set timestamp.
    uint8_t octet = 0;
    frame[27]     = uint8_t(slot.sfn());
    // Subframe index; offset: 4, 4 bits long.
    octet |= uint8_t(slot.subframe_index()) << 4u;
    // Four MSBs of the slot index within 1ms subframe; offset: 4, 6 bits long.
    octet |= uint8_t(slot.subframe_slot_index() >> 2u);
    frame[28] = octet;

    octet = 0;
    octet |= uint8_t(slot.subframe_slot_index() & 0x3) << 6u;
    octet |= uint8_t(symbol);
    frame[29] = octet;

    // Set sequence index.
    uint8_t& seq_id = seq_counters[eaxc];
    frame[24]       = seq_id++;
  }
};
 
} // namespace

static std::string config_file;


/// Maximum number of configuration files allowed to be concatenated in the command line.
static constexpr unsigned MAX_CONFIG_FILES = 6;

/// Function to call when the application is interrupted.
static void interrupt_signal_handler(int signal)
{
  is_app_running = false;
}

/// Function to call when the application is going to be forcefully shutdown.
static void cleanup_signal_handler(int signal)
{
  srslog::flush();
}

int main(int argc, char** argv)
{
  // Set interrupt and cleanup signal handlers.
  register_interrupt_signal_handler(interrupt_signal_handler);
  register_cleanup_signal_handler(cleanup_signal_handler);

  // Setup and configure config parsing.
  CLI::App app("RU emulator application");
  app.config_formatter(create_yaml_config_parser());
  app.allow_config_extras(CLI::config_extras_mode::error);
  app.set_config("-c,", config_file, "Read config from file", false)->expected(1, MAX_CONFIG_FILES);

  ru_emulator_appconfig ru_emulator_parsed_cfg;
  // Configure CLI11 with the RU emulator application configuration schema.
  configure_cli11_with_ru_emulator_appconfig_schema(app, ru_emulator_parsed_cfg);

  // Parse arguments.
  CLI11_PARSE(app, argc, argv);

  if (ru_emulator_parsed_cfg.ru_cfg.empty()) {
    report_error("Invalid configuration detected, at least one RU configuration must be provided\n");
  }

  // Set up logging.
  srslog::sink* log_sink = (ru_emulator_parsed_cfg.log_cfg.filename == "stdout")
                               ? srslog::create_stdout_sink()
                               : srslog::create_file_sink(ru_emulator_parsed_cfg.log_cfg.filename);
  if (log_sink == nullptr) {
    report_error("Could not create application main log sink.\n");
  }
  srslog::set_default_sink(*log_sink);
  srslog::init();

  srslog::basic_logger& logger = srslog::fetch_basic_logger("RU_EMU", false);
  logger.set_level(ru_emulator_parsed_cfg.log_cfg.level);

#ifdef DPDK_FOUND
  bool uses_dpdk = ru_emulator_parsed_cfg.dpdk_config.has_value();

  // Initialize DPDK EAL.
  std::unique_ptr<dpdk::dpdk_eal> eal;
  if (uses_dpdk) {
    // Prepend the application name in argv[0] as it is expected by EAL.
    eal = dpdk::create_dpdk_eal(std::string(argv[0]) + " " + ru_emulator_parsed_cfg.dpdk_config->eal_args,
                                srslog::fetch_basic_logger("EAL", false));
    if (!eal) {
      report_error("Failed to initialize DPDK EAL\n");
    }
  }
#endif
  // Create workers and executors.
  worker_manager_config worker_manager_cfg;
  worker_manager_cfg.nof_emulators = ru_emulator_parsed_cfg.ru_cfg.size();
  fill_ru_worker_manager_config(worker_manager_cfg, ru_emulator_parsed_cfg.sdr_unit_config);
  worker_manager workers{worker_manager_cfg};

  // Set up DPDK transceivers and create RU emulators.
  std::vector<std::unique_ptr<ru_emulator_transceiver>> transceivers;
  std::vector<std::unique_ptr<ru_emulator>>             ru_emulators;

  // Create cells configs.
  std::vector<srs_du::du_cell_config>                   du_cells;
  du_cells.resize(ru_emulator_parsed_cfg.ru_cfg.size());
  

  for (unsigned i = 0, e = ru_emulator_parsed_cfg.ru_cfg.size(); i != e; ++i) {
    ru_emulator_ofh_appconfig ru_cfg = ru_emulator_parsed_cfg.ru_cfg[i];

    gw_config cfg;
    cfg.interface                   = ru_cfg.network_interface;
    cfg.mtu_size                    = units::bytes{ETHERNET_FRAME_SIZE};
    cfg.is_promiscuous_mode_enabled = ru_cfg.enable_promiscuous;
#ifdef DPDK_FOUND
    if (uses_dpdk) {
      auto ctx = create_dpdk_port_context(cfg);
      transceivers.push_back(std::make_unique<dpdk_transceiver>(logger, *workers.ru_rx_exec[i], ctx));
    } else
#endif
    {
      if (!parse_mac_address(ru_cfg.du_mac_address, cfg.mac_dst_address)) {
        report_error("Invalid MAC address provided: '{}'", ru_cfg.du_mac_address);
      }
      transceivers.push_back(std::make_unique<socket_transceiver>(logger, *workers.ru_rx_exec[i], cfg));
    }

    ru_emulator_config emu_cfg;
    emu_cfg.bandwidth = ru_cfg.bandwidth;
    emu_cfg.nof_prb =
        get_max_Nprb(bs_channel_bandwidth_to_MHz(ru_cfg.bandwidth), ru_cfg.common_scs, frequency_range::FR1);
    // Currently UL and DL use the same compression/decompression parameters.
    emu_cfg.dl_compr_params = {to_compression_type(ru_cfg.dl_compr_method), ru_cfg.dl_compr_bitwidth};
    emu_cfg.ul_compr_params = {to_compression_type(ru_cfg.ul_compr_method), ru_cfg.ul_compr_bitwidth};
    emu_cfg.vlan_tag     = ru_cfg.vlan_tag;
    if (!parse_mac_address(ru_cfg.ru_mac_address, emu_cfg.ru_mac)) {
      report_error("Invalid MAC address provided: '{}'", ru_cfg.ru_mac_address);
    }
    if (!parse_mac_address(ru_cfg.du_mac_address, emu_cfg.du_mac)) {
      report_error("Invalid MAC address provided: '{}'", ru_cfg.du_mac_address);
    }
    emu_cfg.timing_params = rx_timing_window_params_us_to_symbols(ru_cfg.T2a_max_cp_dl,
                                                                  ru_cfg.T2a_min_cp_dl,
                                                                  ru_cfg.T2a_max_cp_ul,
                                                                  ru_cfg.T2a_min_cp_ul,
                                                                  ru_cfg.T2a_max_up,
                                                                  ru_cfg.T2a_min_up,
                                                                  ru_cfg.Ta3_max_up,
                                                                  ru_cfg.Ta3_min_up,
                                                                  ru_cfg.common_scs);
    emu_cfg.dl_eaxc       = ru_cfg.ru_dl_port_id;
    emu_cfg.ul_eaxc       = ru_cfg.ru_ul_port_id;
    emu_cfg.prach_eaxc    = ru_cfg.ru_prach_port_id;
    emu_cfg.max_processing_delay_slot = ru_cfg.max_proc_delay;
    emu_cfg.scs           = ru_cfg.common_scs;
    // emu_cfg.tdd_config    = {ru_cfg.common_scs, {5, 3, 10, 1, 2}};

    // unsigned repo_size = calculate_repository_size(ru_cfg.common_scs, emu_cfg.max_processing_delay_slot * 1024);
    unsigned repo_size = calculate_repository_size(ru_cfg.common_scs, emu_cfg.max_processing_delay_slot * 512);
    emu_cfg.repo_size = repo_size;
    
    emu_cfg.sdr_unit_config = ru_emulator_parsed_cfg.sdr_unit_config;

    // Doing hard-coding right now.
    srs_du::du_cell_config& du_cell = du_cells[i];
    uint16_t bw = static_cast<uint16_t>(bs_channel_bandwidth_to_MHz(emu_cfg.bandwidth));
    nr_band  band = ru_cfg.band ? ru_cfg.band.value() : band_helper::get_band_from_dl_arfcn(ru_cfg.dl_arfcn);

    du_cell.scs_common = ru_cfg.common_scs;
    du_cell.dl_carrier = {bw, ru_cfg.dl_arfcn, band, static_cast<uint16_t>(ru_cfg.nof_antennas_dl)};
    du_cell.ul_carrier.arfcn_f_ref = band_helper::get_ul_arfcn_from_dl_arfcn(du_cell.dl_carrier.arfcn_f_ref, band);
    du_cell.ul_carrier.nof_ant = static_cast<uint16_t>(ru_cfg.nof_antennas_ul);

    generate_radio_config(emu_cfg.radio_cfg, emu_cfg.sdr_unit_config, {du_cells});

    emu_cfg.upper_fake_config = generate_upper_part_configuraion(ru_cfg);

    emu_cfg.lower_phy_config = generate_low_phy_config(du_cell, ru_emulator_parsed_cfg.sdr_unit_config, ru_cfg.max_proc_delay);
    emu_cfg.ul_data_flow_config = generate_uplink_data_config(emu_cfg, ru_cfg);
    emu_cfg.is_downlink_static_comp_hdr_enabled = ru_cfg.is_downlink_static_comp_hdr_enabled;
    
    // Here `i` serves as `sector_id`.
    emu_cfg.sector = i;
    ru_emulators.push_back(std::make_unique<ru_emulator>(
        resolve_ru_emulator_dependencies(logger, *workers.ru_emulators_exec[i], *transceivers[i]), emu_cfg, workers));
  }

  // Create timing worker.
  ru_emulator_timing_notifier timing_notifier(logger, *workers.ru_timing_exec, ru_emulator_parsed_cfg.ru_cfg[0].common_scs);

  // Subscribe RU emulator window checkers to the 'OTA symbol start' notifications.
  std::vector<ofh::ota_symbol_boundary_notifier*> ota_symbol_notifiers;
  for (auto& ru : ru_emulators) {
    auto ru_em_ota_notifiers = ru->get_ota_notifiers();
    ota_symbol_notifiers.insert(ota_symbol_notifiers.end(), ru_em_ota_notifiers.begin(), ru_em_ota_notifiers.end());
  }
  timing_notifier.subscribe(ota_symbol_notifiers);

  // Start RU emulators.
  timing_notifier.start();

  std::vector<std::thread> ru_threads;
  ru_threads.reserve(ru_emulators.size());

  for (auto& ru : ru_emulators) {
    ru_threads.emplace_back([ptr = ru.get()]{
      ptr->start();
    });
    // ru->start();
  }
  fmt::print("Running. Waiting for incoming packets...\n");

  fmt::print("| {:^8} | {:^3} | {:^11} | {:^11} | {:^11} | {:^11} | {:^15} | {:^13} | {:^13} | {:^13} | {:^15} | "
             "{:^14} | {:^14} | {:^14} | {:^15} | {:^15} | {:^11} | {:^11} | {:^11} |\n",
             "TIME",
             "ID",
             "RX_TOTAL",
             "RX_ON_TIME",
             "RX_EARLY",
             "RX_LATE",
             "RX_SEQ_ERR",
             "RX_ON_TIME_C",
             "RX_EARLY_C",
             "RX_LATE_C",
             "RX_SEQ_ERR_C",
             "RX_ON_TIME_C_U",
             "RX_EARLY_C_U",
             "RX_LATE_C_U",
             "RX_SEQ_ERR_C_U",
             "RX_SEQ_ERR_PRACH",
             "RX_CORRUPT",
             "RX_ERR_DROP",
             "TX_TOTAL");

  while (is_app_running) {
    std::this_thread::sleep_for(std::chrono::seconds(1));

    for (unsigned i = 0, e = ru_emulators.size(); i != e; ++i) {
      ru_emulators[i]->print_statistics(i);
    }
  }

  timing_notifier.stop();
  // for (auto& txrx : transceivers) {
  //   txrx->stop();
  // }

  for (auto& ru : ru_emulators) {
    ru->stop();
  }

  for(auto& th : ru_threads){
    if(th.joinable()) th.join();
  }

  workers.stop();
  srslog::flush();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  fmt::print("\nRU emulator app stopped\n");

  return 0;
}
