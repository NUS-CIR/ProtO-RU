// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "helpers.h"
#include "ru_emulator_appconfig.h"
#include "ru_emulator_appconfig_yaml_writer.h"
#include "ru_emulator_cli11_schema.h"
#include "ru_emulator_gps_slot_aligner.h"
#include "ru_emulator_rx_symbol_adapter.h"
#include "ru_emulator_sdr.h"
#include "ru_emulator_sdr_upper_phy.h"
#include "ru_emulator_stats_collector.h"
#include "ru_emulator_timing_notifier.h"
#include "ru_emulator_tti_orchestrator.h"
#include "ru_emulator_upper_phy.h"
#include "ocudu/ocudulog/logger.h"
#include "ocudu/ofh/compression/compression_params.h"
#include "ocudu/ofh/ethernet/dpdk/dpdk_ethernet_factories.h"
#include "ocudu/ofh/ethernet/ethernet_controller.h"
#include "ocudu/ofh/ethernet/ethernet_factories.h"
#include "ocudu/ofh/ethernet/ethernet_frame_notifier.h"
#include "ocudu/ofh/ethernet/ethernet_properties.h"
#include "ocudu/ofh/ethernet/ethernet_receiver.h"
#include "ocudu/ofh/ethernet/ethernet_transmitter.h"
#include "ocudu/ofh/ethernet/ethernet_unique_buffer.h"
#include "ocudu/ofh/ru_sector.h"
#include "ocudu/ofh/timing/ofh_ota_symbol_boundary_notifier.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/prach/prach_preamble_information.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/ran/slot_point.h"
#include "ocudu/ru/ru.h"
#include "ocudu/ru/ru_controller.h"
#include "ocudu/ru/ru_error_notifier.h"
#include "ocudu/ru/sdr/ru_sdr_executor_mapper.h"
#include "ocudu/ru/sdr/ru_sdr_factory.h"
#include "ocudu/support/config_parsers.h"
#include "ocudu/support/cpu_architecture_info.h"
#include "ocudu/support/executors/task_execution_manager.h"
#include "ocudu/support/executors/task_executor.h"
#include "ocudu/support/executors/unique_thread.h"
#include "ocudu/support/format/fmt_to_c_str.h"
#include "ocudu/support/math/math_utils.h"
#include "ocudu/support/signal_handling.h"
#include "fmt/chrono.h"
#include "fmt/color.h"
#include <algorithm>
#include <atomic>
#include <chrono>
#include <thread>
#ifdef DPDK_FOUND
#include "ocudu/hal/dpdk/dpdk_eal_factory.h"
#endif

using namespace ocudu;
using namespace ofh;

/// Ethernet frame (MTU) size used by the RU emulator.
static constexpr unsigned ETHERNET_FRAME_SIZE = 9000;

/// Maximum number of symbols in a slot, considering normal cyclic prefix.
static constexpr size_t MAX_NOF_SYMBOLS = get_nsymb_per_slot(cyclic_prefix::NORMAL);

namespace {

/// Reception window timing parameters expressed in a number of symbols, derived from the T2a parameters.
struct ru_em_rx_window_timing_parameters {
  unsigned sym_cp_dl_start;
  unsigned sym_cp_dl_end;
  unsigned sym_cp_ul_start;
  unsigned sym_cp_ul_end;
  unsigned sym_up_dl_start;
  unsigned sym_up_dl_end;
  /// Uplink User-Plane transmit window (Ta3), the RU's transmit window.
  unsigned sym_up_ul_start;
  unsigned sym_up_ul_end;
};

} // namespace

/// Converts timing parameters expressed in microseconds into the ones expressed in number of OFDM symbols.
static ru_em_rx_window_timing_parameters rx_timing_window_params_us_to_symbols(subcarrier_spacing        scs,
                                                                               std::chrono::microseconds T2a_max_cp_dl,
                                                                               std::chrono::microseconds T2a_min_cp_dl,
                                                                               std::chrono::microseconds T2a_max_cp_ul,
                                                                               std::chrono::microseconds T2a_min_cp_ul,
                                                                               std::chrono::microseconds T2a_max_up,
                                                                               std::chrono::microseconds T2a_min_up,
                                                                               std::chrono::microseconds Ta3_max_up,
                                                                               std::chrono::microseconds Ta3_min_up)
{
  std::chrono::duration<double, std::nano> symbol_duration((1e6 / (MAX_NOF_SYMBOLS * get_nof_slots_per_subframe(scs))));

  ru_em_rx_window_timing_parameters params;
  params.sym_cp_dl_start = std::floor(T2a_max_cp_dl / symbol_duration);
  params.sym_cp_dl_end   = std::ceil(T2a_min_cp_dl / symbol_duration);
  params.sym_cp_ul_start = std::floor(T2a_max_cp_ul / symbol_duration);
  params.sym_cp_ul_end   = std::ceil(T2a_min_cp_ul / symbol_duration);
  params.sym_up_dl_start = std::floor(T2a_max_up / symbol_duration);
  params.sym_up_dl_end   = std::ceil(T2a_min_up / symbol_duration);
  params.sym_up_ul_start = std::floor(Ta3_max_up / symbol_duration);
  params.sym_up_ul_end   = std::ceil(Ta3_min_up / symbol_duration);

  return params;
}

namespace {

/// \brief Creates the Ethernet transmitter and receiver pair the RU emulator uses.
///
/// Built exactly the way the O-DU builds its own (see lib/ofh/ofh_factories.cpp): from the library socket or DPDK
/// Ethernet factories. The transmitter is handed to the O-RU sector's message transmitter; the receiver feeds the
/// emulator's frame notifier.
std::pair<std::unique_ptr<ether::transmitter>, std::unique_ptr<ether::receiver>>
create_ru_emulator_txrx(const ru_emulator_ofh_appconfig& ru_cfg,
                        bool                             uses_dpdk,
                        task_executor&                   rx_executor,
                        ocudulog::basic_logger&          logger)
{
  ether::transmitter_config tx_cfg;
  tx_cfg.interface                   = ru_cfg.network_interface;
  tx_cfg.is_promiscuous_mode_enabled = ru_cfg.enable_promiscuous;
  tx_cfg.are_metrics_enabled         = false;
  tx_cfg.mtu_size                    = units::bytes{ETHERNET_FRAME_SIZE};
  if (!parse_mac_address(ru_cfg.du_mac_address, tx_cfg.mac_dst_address)) {
    report_error("Invalid MAC address provided: '{}'", ru_cfg.du_mac_address);
  }

#ifdef DPDK_FOUND
  if (uses_dpdk) {
    return ether::create_dpdk_txrx(tx_cfg, rx_executor, logger);
  }
#else
  (void)uses_dpdk;
#endif

  auto rx = ether::create_receiver(
      {tx_cfg.interface, tx_cfg.is_promiscuous_mode_enabled, tx_cfg.are_metrics_enabled}, rx_executor, logger);
  auto tx = ether::create_transmitter(tx_cfg, logger);
  return {std::move(tx), std::move(rx)};
}

/// \brief RU emulator: a thin driver over the library O-RU sector.
///
/// Feeds received Ethernet frames to the sector (which decodes the O-DU's Control-Plane and responds with uplink/PRACH
/// User-Plane carrying the fake upper PHY's test IQ) and exposes the sector's OTA notifiers for the timing source.
class ru_emulator : public ether::frame_notifier
{
  ocudulog::basic_logger&            logger;
  task_executor&                     executor;
  std::unique_ptr<ether::receiver>   receiver;
  std::unique_ptr<ofh::ru_upper_phy> upper_phy;
  // Declared before the sector: the sector's Ethernet transmitter is decorated to report sent frames to the collector.
  std::unique_ptr<ru_emulator_stats_collector> stats;
  std::unique_ptr<ofh::ru_sector>              sector;
  // SDR-only components (nullptr in loopback mode). Declared after the sector and base upper PHY they reference so that
  // they are destroyed first.
  std::unique_ptr<ru_emulator_rx_symbol_adapter> rx_symbol_adapter;
  std::unique_ptr<ru_emulator_tti_orchestrator>  orchestrator;
  // GPS slot aligner (SDR mode): the radio's timing notifier, measuring the radio-to-GPS slot offset before forwarding
  // the TTIs to the orchestrator. Declared after the orchestrator it forwards to and before the radio that drives it.
  std::unique_ptr<ru_emulator_gps_slot_aligner> gps_aligner;
  std::unique_ptr<radio_unit>                   radio;
  // OTA symbol boundary notifiers of the sector plus the statistics reception-window checkers.
  std::vector<ota_symbol_boundary_notifier*> ota_notifiers;

public:
  ru_emulator(ocudulog::basic_logger&                        logger_,
              task_executor&                                 executor_,
              std::unique_ptr<ether::receiver>               receiver_,
              std::unique_ptr<ofh::ru_upper_phy>             upper_phy_,
              std::unique_ptr<ru_emulator_stats_collector>   stats_,
              std::unique_ptr<ofh::ru_sector>                sector_,
              std::unique_ptr<ru_emulator_rx_symbol_adapter> rx_symbol_adapter_,
              std::unique_ptr<ru_emulator_tti_orchestrator>  orchestrator_,
              std::unique_ptr<ru_emulator_gps_slot_aligner>  gps_aligner_,
              std::unique_ptr<radio_unit>                    radio_) :
    logger(logger_),
    executor(executor_),
    receiver(std::move(receiver_)),
    upper_phy(std::move(upper_phy_)),
    stats(std::move(stats_)),
    sector(std::move(sector_)),
    rx_symbol_adapter(std::move(rx_symbol_adapter_)),
    orchestrator(std::move(orchestrator_)),
    gps_aligner(std::move(gps_aligner_)),
    radio(std::move(radio_))
  {
    auto sector_notifiers = sector->get_ota_symbol_boundary_notifiers();
    ota_notifiers.assign(sector_notifiers.begin(), sector_notifiers.end());
    for (auto* notifier : stats->get_ota_notifiers()) {
      ota_notifiers.push_back(notifier);
    }
  }

  // See interface for documentation.
  void on_new_frame(ether::unique_rx_buffer buffer) override
  {
    if (!executor.execute([this, b = std::move(buffer)]() mutable {
          stats->on_frame(b.data());
          sector->on_new_frame(b.data());
        })) {
      logger.warning("Failed to dispatch receiver task");
    }
  }

  void start()
  {
    // In SDR mode, start the radio first so it drives the lower PHY timing (and, through the GPS slot aligner, the TTI
    // orchestrator). The aligner measures the radio-to-GPS slot offset from the first TTI notifications.
    if (radio) {
      radio->get_controller().get_operation_controller().start();
    }
    receiver->get_operation_controller().start(*this);
  }

  void stop()
  {
    receiver->get_operation_controller().stop();
    if (radio) {
      radio->get_controller().get_operation_controller().stop();
    }
  }

  /// Returns the OTA symbol boundary notifiers (the sector's and the statistics reception-window checkers'), to be
  /// subscribed to the timing source.
  span<ota_symbol_boundary_notifier* const> get_ota_notifiers() { return ota_notifiers; }

  void print_statistics(unsigned emu_id) { stats->print_statistics(emu_id); }
};

/// Ethernet transmitter decorator that reports the number of sent frames to the statistics collector.
class counting_eth_transmitter : public ether::transmitter
{
  std::unique_ptr<ether::transmitter> inner;
  ru_emulator_stats_collector&        stats;

public:
  counting_eth_transmitter(std::unique_ptr<ether::transmitter> inner_, ru_emulator_stats_collector& stats_) :
    inner(std::move(inner_)), stats(stats_)
  {
  }

  // See interface for documentation.
  void send(span<span<const uint8_t>> frames) override
  {
    stats.on_frames_sent(frames.size());
    inner->send(frames);
  }

  // See interface for documentation.
  ether::transmitter_metrics_collector* get_metrics_collector() override { return inner->get_metrics_collector(); }
};

/// No-op SDR Radio Unit error notifier.
class ru_error_notifier_dummy : public ru_error_notifier
{
public:
  void on_late_downlink_message(const ru_error_context&) override {}
  void on_late_uplink_message(const ru_error_context&) override {}
  void on_late_prach_message(const ru_error_context&) override {}
};

/// Builds an RU emulator for the given OFH configuration. In loopback mode it uses a fake test-IQ upper PHY; in SDR
/// mode (when ru_cfg.sdr_config is set) it drives a real radio + lower PHY through the in-tree SDR radio_unit.
std::unique_ptr<ru_emulator> create_ru_emulator(unsigned                         emu_id,
                                                ocudulog::basic_logger&          logger,
                                                task_executor&                   proc_executor,
                                                task_executor&                   rx_executor,
                                                task_executor&                   ofh_tx_executor,
                                                bool                             uses_dpdk,
                                                ru_sdr_executor_mapper*          sdr_exec_mapper,
                                                ru_error_notifier&               error_notifier,
                                                const ru_emulator_ofh_appconfig& ru_cfg)
{
  const bool               sdr_mode     = ru_cfg.sdr_config.has_value();
  const subcarrier_spacing scs          = ru_cfg.common_scs;
  const unsigned           nof_prb      = get_max_Nprb(ru_cfg.bandwidth, scs, frequency_range::FR1);
  const auto               nof_dl_ports = static_cast<unsigned>(ru_cfg.ru_dl_port_id.size());
  const auto               nof_ul_ports = static_cast<unsigned>(ru_cfg.ru_ul_port_id.size());

  ru_em_rx_window_timing_parameters timing = rx_timing_window_params_us_to_symbols(scs,
                                                                                   ru_cfg.T2a_max_cp_dl,
                                                                                   ru_cfg.T2a_min_cp_dl,
                                                                                   ru_cfg.T2a_max_cp_ul,
                                                                                   ru_cfg.T2a_min_cp_ul,
                                                                                   ru_cfg.T2a_max_up,
                                                                                   ru_cfg.T2a_min_up,
                                                                                   ru_cfg.Ta3_max_up,
                                                                                   ru_cfg.Ta3_min_up);

  // SDR timing consistency. The received downlink is finalized and pushed to the radio one millisecond (the lower
  // PHY's reception-to-transmission lead, when its baseband pulls the modulated slot) plus one slot of margin ahead
  // of air time; the margin absorbs the asynchronous modulation latency and the sub-slot residual between the GPS
  // tick and the radio clock (the GPS slot alignment is exact to the slot, not within it). The PRACH capture request
  // is issued one millisecond plus half a slot ahead of air. The O-DU must have delivered everything by those points:
  // each message leaves the O-DU when its T2a_max window opens, relative to its own air time, and the receive/decode
  // path adds latency on top - give the windows headroom beyond these hard minimums.
  if (sdr_mode) {
    const unsigned nsym               = get_nsymb_per_slot(cyclic_prefix::NORMAL);
    const unsigned slot_duration_us   = 1000U / get_nof_slots_per_subframe(scs);
    const unsigned symbol_duration_us = divide_ceil(slot_duration_us, nsym);
    const unsigned dl_lead_us         = 1000U + slot_duration_us;
    const unsigned prach_lead_us      = 1000U + slot_duration_us / 2;
    // The downlink Control-Plane (sent at slot granularity) must arrive, and open the reception grid, before its
    // slot's finalize point; the PRACH Control-Plane before its slot's capture-request point.
    if (static_cast<unsigned>(ru_cfg.T2a_max_cp_dl.count()) <= dl_lead_us ||
        static_cast<unsigned>(ru_cfg.T2a_max_cp_ul.count()) <= prach_lead_us) {
      report_error("SDR timing inconsistent: t2a_max_cp_dl={} us must exceed the downlink hand-over lead of 1 ms + 1 "
                   "slot = {} us (the downlink grid is pushed to the radio that far ahead of air time) and "
                   "t2a_max_cp_ul={} us must exceed the PRACH capture lead of 1 ms + half a slot = {} us. Increase "
                   "them (cap 5000 us) on both the O-DU and here.\n",
                   ru_cfg.T2a_max_cp_dl.count(),
                   dl_lead_us,
                   ru_cfg.T2a_max_cp_ul.count(),
                   prach_lead_us);
    }
    // The last downlink User-Plane symbol of a slot (air time 13 symbols after the slot start) must also arrive
    // before the slot's finalize point.
    if (static_cast<unsigned>(ru_cfg.T2a_max_up.count()) <= dl_lead_us + (nsym - 1) * symbol_duration_us) {
      report_error("SDR timing inconsistent: T2a_max_up={} us must exceed the downlink hand-over lead plus the last "
                   "symbol's offset within the slot = {} + {} = {} us, so the whole downlink slot has arrived when "
                   "its grid is pushed to the radio. Increase t2a_max_up (cap 5000 us) on both the O-DU and here, "
                   "with headroom for the receive/decode latency.\n",
                   ru_cfg.T2a_max_up.count(),
                   dl_lead_us,
                   (nsym - 1) * symbol_duration_us,
                   dl_lead_us + (nsym - 1) * symbol_duration_us);
    }
  }

  const bool prach_long = ru_cfg.prach_format == ru_emulator_prach_format::LONG_F0;
  // PRACH-over-radio (the TTI orchestrator's handle_prach_occasion path) assumes a short format B4 capture context, so
  // long-format PRACH is not supported over a real radio yet.
  if (sdr_mode && prach_long) {
    report_error(
        "PRACH-over-radio in SDR mode currently supports only short PRACH formats (set prach_format=short).\n");
  }
  // For short preambles the PRACH SCS equals the cell common SCS.
  const unsigned nof_prach_symbols =
      prach_long
          ? get_prach_preamble_long_info(prach_format_type::zero).nof_symbols
          : get_prach_preamble_short_info(prach_format_type::B4, to_ra_subcarrier_spacing(scs), true).nof_symbols;

  // O-RU sector configuration (shared by both modes).
  ru_sector_config sec_cfg;
  sec_cfg.sector                 = emu_id;
  sec_cfg.scs                    = scs;
  sec_cfg.cp                     = cyclic_prefix::NORMAL;
  sec_cfg.ru_nof_prbs            = nof_prb;
  sec_cfg.ul_compr_params        = {to_compression_type(ru_cfg.ul_compr_method), ru_cfg.ul_compr_bitwidth};
  sec_cfg.dl_compr_params        = {to_compression_type(ru_cfg.dl_compr_method), ru_cfg.dl_compr_bitwidth};
  sec_cfg.prach_compr_params     = {to_compression_type(ru_cfg.prach_compr_method), ru_cfg.prach_compr_bitwidth};
  sec_cfg.iq_scaling             = ru_cfg.iq_scaling;
  sec_cfg.is_ul_static_compr_hdr = ru_cfg.is_ul_static_compr_hdr;
  sec_cfg.is_dl_static_compr_hdr = ru_cfg.is_dl_static_compr_hdr;

  ether::mac_address ru_mac;
  ether::mac_address du_mac;
  if (!parse_mac_address(ru_cfg.ru_mac_address, ru_mac)) {
    report_error("Invalid RU MAC address provided: '{}'", ru_cfg.ru_mac_address);
  }
  if (!parse_mac_address(ru_cfg.du_mac_address, du_mac)) {
    report_error("Invalid DU MAC address provided: '{}'", ru_cfg.du_mac_address);
  }
  // The emulator transmits towards the O-DU, so the destination is the O-DU and the source is the O-RU. An unset VLAN
  // tag leaves the frames untagged, for interfaces that insert the tag themselves (e.g. an SR-IOV VF port VLAN).
  std::optional<ether::vlan_parameters> vlan_config;
  if (ru_cfg.vlan_tag) {
    vlan_config = ether::vlan_parameters{.tci_vid = static_cast<uint16_t>(*ru_cfg.vlan_tag)};
  }
  sec_cfg.vlan_params = {du_mac, ru_mac, vlan_config, ether::ECPRI_ETH_TYPE};

  for (auto eaxc : ru_cfg.ru_dl_port_id) {
    sec_cfg.dl_eaxc.push_back(eaxc);
  }
  for (auto eaxc : ru_cfg.ru_ul_port_id) {
    sec_cfg.ul_eaxc.push_back(eaxc);
  }
  for (auto eaxc : ru_cfg.ru_prach_port_id) {
    sec_cfg.prach_eaxc.push_back(eaxc);
  }

  sec_cfg.mtu_size              = units::bytes(ETHERNET_FRAME_SIZE);
  sec_cfg.nof_frames_per_symbol = 2;
  // Uplink User-Plane transmit window is the RU's Ta3 window (not the C-plane receive window T2a_cp_ul).
  sec_cfg.tx_window_start_symbols = timing.sym_up_ul_start;
  sec_cfg.tx_window_end_symbols   = timing.sym_up_ul_end;
  sec_cfg.rx_window               = {timing.sym_up_dl_end, timing.sym_up_dl_start};
  // Loopback replies to the Control-Plane immediately with test IQ; SDR records the request and replies with the IQ the
  // radio captures, pushed through the uplink IQ sink.
  sec_cfg.uplink_response_mode =
      sdr_mode ? ru_uplink_response_mode::store_and_respond : ru_uplink_response_mode::immediate;
  // In SDR mode the received downlink grid must be finalized ahead of its air time: the finalized grid is pushed
  // straight to the radio, modulated asynchronously, and its baseband pulls the result one millisecond (the lower
  // PHY's reception-to-transmission lead) before air. One subframe of lead plus a full slot of margin covers the
  // modulation latency and the sub-slot GPS-to-radio residual; the radio accepts requests well ahead (its request
  // ring holds 16 slots), so the early hand-over is free. In loopback mode the grid is only consumed for statistics,
  // so it is finalized one slot after air.
  const unsigned nsym_per_slot            = get_nsymb_per_slot(cyclic_prefix::NORMAL);
  const unsigned dl_finalize_lead_symbols = (get_nof_slots_per_subframe(scs) + 1) * nsym_per_slot;
  sec_cfg.dl_grid_finalize_offset_symbols =
      sdr_mode ? -static_cast<int>(dl_finalize_lead_symbols) : static_cast<int>(nsym_per_slot);

  // Statistics collector, classifying received frames and counting transmitted ones the upstream emulator way.
  ru_emulator_stats_collector_config stats_cfg;
  stats_cfg.scs = scs;
  stats_cfg.prach_filter_index =
      prach_long ? filter_index_type::ul_prach_preamble_1p25khz : filter_index_type::ul_prach_preamble_short;
  stats_cfg.nof_prach_symbols = nof_prach_symbols;
  // Accepted udCompHdr values of received UL Control-Plane requests. With a static UL compression header the O-DU
  // signals the compression out of band and writes 0 into the C-Plane udCompHdr; with a dynamic header it writes the
  // in-band udIqWidth (4 MSB, 16 encoded as 0) | udCompMeth (4 LSB). Accept both (the upstream emulator hard-coded
  // {0x00, 0x91} for 9-bit BFP), so the UL C-Plane is not misclassified as corrupt in static-header mode.
  stats_cfg.supported_ul_compr_hdr = {0x00U,
                                      static_cast<uint8_t>(((ru_cfg.ul_compr_bitwidth & 0xFU) << 4U) |
                                                           to_value(to_compression_type(ru_cfg.ul_compr_method)))};
  stats_cfg.dl_cp_window           = {timing.sym_cp_dl_end, timing.sym_cp_dl_start};
  stats_cfg.dl_up_window           = {timing.sym_up_dl_end, timing.sym_up_dl_start};
  stats_cfg.ul_cp_window           = {timing.sym_cp_ul_end, timing.sym_cp_ul_start};
  for (auto eaxc : ru_cfg.ru_dl_port_id) {
    stats_cfg.dl_eaxc.push_back(eaxc);
  }
  for (auto eaxc : ru_cfg.ru_ul_port_id) {
    stats_cfg.ul_eaxc.push_back(eaxc);
  }
  for (auto eaxc : ru_cfg.ru_prach_port_id) {
    stats_cfg.prach_eaxc.push_back(eaxc);
  }
  // Count only frames destined for this RU and coming from the configured O-DU (received dst = RU, src = DU).
  stats_cfg.ru_mac_address = ru_mac;
  stats_cfg.du_mac_address = du_mac;
  stats_cfg.vlan_config    = sec_cfg.vlan_params.vlan_config;
  auto stats               = std::make_unique<ru_emulator_stats_collector>(stats_cfg, logger);

  // Ethernet fronthaul transmitter/receiver pair (both modes), built the O-DU way.
  auto txrx = create_ru_emulator_txrx(ru_cfg, uses_dpdk, rx_executor, logger);
  // Count the frames the sector transmits (TX_TOTAL).
  txrx.first = std::make_unique<counting_eth_transmitter>(std::move(txrx.first), *stats);

  // Upper PHY: loopback test-IQ source, or the SDR downlink-reception upper PHY.
  std::unique_ptr<ofh::ru_upper_phy> upper_phy;
  ru_emulator_sdr_upper_phy*         sdr_upper_phy = nullptr;
  if (sdr_mode) {
    ru_emulator_sdr_upper_phy_config sdr_phy_cfg{emu_id, nof_prb, nof_dl_ports};
    auto                             sdr_up = create_ru_emulator_sdr_upper_phy(sdr_phy_cfg);
    sdr_upper_phy                           = sdr_up.get();
    upper_phy                               = std::move(sdr_up);
  } else {
    ru_emulator_upper_phy_config phy_cfg{nof_prb, nof_dl_ports, nof_prach_symbols, prach_long};
    upper_phy = create_ru_emulator_upper_phy(phy_cfg);
  }

  ru_sector_dependencies sec_deps;
  sec_deps.logger                 = &logger;
  sec_deps.upper_phy              = upper_phy.get();
  sec_deps.eth_transmitter        = std::move(txrx.first);
  sec_deps.uplink_encode_executor = &ofh_tx_executor;
  auto sector                     = create_ru_sector(sec_cfg, std::move(sec_deps));

  // SDR-only: the radio_unit, the TTI orchestrator, the GPS slot aligner and the received-symbol adapter.
  std::unique_ptr<ru_emulator_rx_symbol_adapter> rx_symbol_adapter;
  std::unique_ptr<ru_emulator_tti_orchestrator>  orchestrator;
  std::unique_ptr<ru_emulator_gps_slot_aligner>  gps_aligner;
  std::unique_ptr<radio_unit>                    radio;
  if (sdr_mode) {
    report_error_if_not(sdr_exec_mapper, "Missing SDR executor mapper for the SDR-mode RU emulator\n");
    ofh::ru_uplink_iq_sink* sink = sector->get_uplink_iq_sink();
    report_error_if_not(sink, "SDR-mode O-RU sector must expose an uplink IQ sink\n");

    rx_symbol_adapter = std::make_unique<ru_emulator_rx_symbol_adapter>(*sink);
    ru_emulator_tti_orchestrator_config orch_cfg{emu_id, nof_prb, nof_ul_ports, scs};
    // The PRACH peek lookback makes the capture request fire one millisecond ahead of the occasion's air time (after
    // its Control-Plane has been recorded) instead of at the occasion's own, too-early TTI.
    orch_cfg.prach_peek_lookback_slots = ru_cfg.sdr_config->max_proc_delay;
    orchestrator                       = create_ru_emulator_tti_orchestrator(orch_cfg, logger, *sdr_upper_phy, *sector);

    // The aligner measures the offset between the radio slot numbering and the O-DU's GPS wire numbering from the TTI
    // notifications and, once established, applies it to the OFH-facing slot translations while continuing to track
    // drift.
    gps_aligner = std::make_unique<ru_emulator_gps_slot_aligner>(
        *orchestrator,
        logger,
        [orch = orchestrator.get(), adapter = rx_symbol_adapter.get(), sdr_phy = sdr_upper_phy](unsigned offset) {
          orch->set_gps_slot_offset(offset);
          adapter->set_gps_slot_offset(offset);
          sdr_phy->set_gps_slot_offset(offset);
        });

    ru_sdr_configuration sdr_config =
        generate_ru_emulator_sdr_config(*ru_cfg.sdr_config,
                                        scs,
                                        ru_cfg.bandwidth,
                                        ru_cfg.dl_arfcn,
                                        ru_cfg.band,
                                        /* nof_tx_antennas = downlink eAxCs */ nof_dl_ports,
                                        /* nof_rx_antennas = uplink eAxCs */ nof_ul_ports);

    ru_sdr_dependencies sdr_deps = {.radio_exec      = sdr_exec_mapper->asynchronous_radio_executor(),
                                    .rf_logger       = ocudulog::fetch_basic_logger("RF", false),
                                    .symbol_notifier = *rx_symbol_adapter,
                                    .timing_notifier = *gps_aligner,
                                    .error_notifier  = error_notifier};

    ru_sdr_sector_executor_mapper& sector_map = sdr_exec_mapper->get_sector_mapper(0);
    sdr_deps.sector_dependencies.push_back(
        ru_sdr_sector_dependencies{.logger               = ocudulog::fetch_basic_logger("PHY", false),
                                   .rx_task_executor     = sector_map.receiver_executor(),
                                   .tx_task_executor     = sector_map.transmitter_executor(),
                                   .dl_task_executor     = sector_map.downlink_executor(),
                                   .ul_task_executor     = sector_map.uplink_executor(),
                                   .prach_async_executor = sector_map.prach_executor()});

    radio = create_sdr_ru(sdr_config, sdr_deps);
    report_error_if_not(radio, "Unable to create the SDR radio unit for the RU emulator\n");

    orchestrator->connect(radio->get_uplink_plane_handler());
    // Completed downlink grids are pushed to the radio at the reception-window finalize point (on the OFH timing),
    // ahead of the lower PHY's baseband pull.
    sdr_upper_phy->connect_downlink(radio->get_downlink_plane_handler());
  }

  return std::make_unique<ru_emulator>(logger,
                                       proc_executor,
                                       std::move(txrx.second),
                                       std::move(upper_phy),
                                       std::move(stats),
                                       std::move(sector),
                                       std::move(rx_symbol_adapter),
                                       std::move(orchestrator),
                                       std::move(gps_aligner),
                                       std::move(radio));
}

/// Manages the workers of the RU emulators.
struct worker_manager {
  static constexpr uint32_t task_worker_queue_size = 1024;

  explicit worker_manager(const ru_emulator_appconfig& cfg) { create_executors(cfg); }

  void create_executors(const ru_emulator_appconfig& cfg)
  {
    using namespace execution_config_helper;

    auto build_mask = [](const std::vector<unsigned>& cpus) {
      os_sched_affinity_bitmask m;
      for (unsigned c : cpus) {
        m.set(c);
      }
      return m;
    };

    unsigned nof_emulators = cfg.ru_cfg.size();
    sdr_exec_mappers.resize(nof_emulators);

    for (unsigned i = 0; i != nof_emulators; ++i) {
      // Executors for Open Fronthaul messages reception.
      {
        const std::string name      = "ru_rx_#" + std::to_string(i);
        const std::string exec_name = "ru_rx_exec_#" + std::to_string(i);

        const single_worker ru_worker{name,
                                      {exec_name, concurrent_queue_policy::lockfree_spsc, 2},
                                      std::chrono::microseconds{1},
                                      os_thread_realtime_priority::max() - 1,
                                      build_mask(cfg.ru_cfg[i].ofh_cpus)};
        if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
          report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
        }
        ru_rx_exec.push_back(exec_mng.executors().at(exec_name));
      }

      // Executors for the RU emulators.
      {
        const std::string   name      = "ru_emu_#" + std::to_string(i);
        const std::string   exec_name = "ru_emu_exec_#" + std::to_string(i);
        const single_worker ru_worker{name,
                                      {exec_name, concurrent_queue_policy::lockfree_spsc, task_worker_queue_size},
                                      std::chrono::microseconds{1},
                                      os_thread_realtime_priority::max() - 1,
                                      build_mask(cfg.ru_cfg[i].ofh_cpus)};
        if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
          report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
        }
        ru_emulators_exec.push_back(exec_mng.executors().at(exec_name));
      }

      // OFH transmit/encode executor: runs the decoupled uplink User-Plane build + compression (handed off the
      // baseband / immediate-response thread, like the O-DU downlink). Shares the OFH CPUs (--ofh_cpus).
      {
        const std::string   name      = "ru_ofh_tx_#" + std::to_string(i);
        const std::string   exec_name = "ru_ofh_tx_exec_#" + std::to_string(i);
        const single_worker ru_worker{name,
                                      {exec_name, concurrent_queue_policy::lockfree_mpmc, task_worker_queue_size},
                                      std::chrono::microseconds{10},
                                      os_thread_realtime_priority::max() - 1,
                                      build_mask(cfg.ru_cfg[i].ofh_cpus)};
        if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
          report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
        }
        ru_ofh_tx_exec.push_back(exec_mng.executors().at(exec_name));
      }

      // SDR radio and baseband workers (sequential baseband profile, upstream style) for SDR-mode RUs.
      if (cfg.ru_cfg[i].sdr_config) {
        create_sdr_executors(i, *cfg.ru_cfg[i].sdr_config);
      }
    }

    // Timing executor.
    {
      const std::string name      = "ru_timing";
      const std::string exec_name = "ru_timing_exec";

      const single_worker ru_worker{name,
                                    {exec_name, concurrent_queue_policy::lockfree_spsc, 4},
                                    std::chrono::microseconds{1},
                                    os_thread_realtime_priority::max() - 0,
                                    build_mask(cfg.timing_cpus)};
      if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
      }
      ru_timing_exec = exec_mng.executors().at(exec_name);
    }
  }

  /// Resolves the configured lower-PHY execution profile. ZMQ is forced to sequential (its blocking baseband); "auto"
  /// picks by host CPU count, mirroring the split-8 RU (single < 4, dual < 8, triple otherwise).
  static std::string resolve_execution_profile(const ru_emulator_sdr_appconfig& sdr_cfg)
  {
    if (sdr_cfg.device_driver == "zmq") {
      return "sequential";
    }
    if (sdr_cfg.execution_profile != "auto") {
      return sdr_cfg.execution_profile;
    }
    const unsigned ncpu = cpu_architecture_info::get().get_host_nof_available_cpus();
    return (ncpu < 4) ? "single" : (ncpu < 8) ? "dual" : "triple";
  }

  /// Creates the SDR radio house-keeping + lower-PHY baseband workers for RU \c i and builds its executor mapper for
  /// the configured execution profile (sequential / single / dual / triple, see the upstream worker_manager), with the
  /// worker CPU affinity taken from the RU configuration.
  void create_sdr_executors(unsigned i, const ru_emulator_sdr_appconfig& sdr_cfg)
  {
    using namespace execution_config_helper;

    const std::string suffix = "_#" + std::to_string(i);

    // Builds the affinity mask for the next worker according to the pinning policy: "round-robin" pins each worker to a
    // single CPU from ru_cpus (cycling), "mask" shares the whole set across all workers.
    unsigned rr_index  = 0;
    auto     next_mask = [&]() -> os_sched_affinity_bitmask {
      os_sched_affinity_bitmask m;
      if (sdr_cfg.ru_cpus.empty()) {
        return m;
      }
      if (sdr_cfg.pinning_policy == "round-robin") {
        m.set(sdr_cfg.ru_cpus[rr_index++ % sdr_cfg.ru_cpus.size()]);
      } else {
        for (unsigned cpu : sdr_cfg.ru_cpus) {
          m.set(cpu);
        }
      }
      return m;
    };

    // Helper: create one realtime worker pinned per the affinity policy and return its executor.
    auto make_worker = [&](const std::string&          name,
                           const std::string&          exec,
                           unsigned                    queue_size,
                           std::chrono::microseconds   sleep,
                           os_thread_realtime_priority prio) -> task_executor* {
      const single_worker worker{
          name, {exec, concurrent_queue_policy::lockfree_mpmc, queue_size}, sleep, prio, next_mask()};
      if (!exec_mng.add_execution_context(create_execution_context(worker))) {
        report_fatal_error("Failed to instantiate {} execution context", worker.name);
      }
      return exec_mng.executors().at(exec);
    };

    // Radio house-keeping worker (always present). Like the upstream split-8 worker manager, this runs at non-realtime
    // priority: it only services the UHD async-message receive loop (TX metadata / under-overflow notifications) and
    // must not outrank the baseband or the radio's own UHD threads - a realtime poller here starves UHD's transport /
    // control threads (manifesting as RFNoC control-ACK timeouts and baseband jitter).
    task_executor* radio = make_worker("ru_radio" + suffix,
                                       "ru_radio_exec" + suffix,
                                       task_worker_queue_size,
                                       std::chrono::microseconds{50},
                                       os_thread_realtime_priority::no_realtime());

    const std::string profile = resolve_execution_profile(sdr_cfg);

    if (profile == "sequential") {
      task_executor*                                  bb = make_worker("ru_phy" + suffix,
                                      "ru_phy_exec" + suffix,
                                      task_worker_queue_size,
                                      std::chrono::microseconds{10},
                                      os_thread_realtime_priority::max());
      ru_sdr_executor_mapper_sequential_configuration map_cfg;
      map_cfg.asynchronous_exec = radio;
      map_cfg.common_exec       = bb;
      map_cfg.nof_sectors       = 1;
      sdr_exec_mappers[i]       = create_ru_sdr_executor_mapper(map_cfg);
      return;
    }

    // single / dual / triple share a high-priority executor for downlink modulation + uplink demodulation. It runs just
    // below the baseband transport threads (max()-2, matching the upstream split-8 rt_prio_exec), so the sample-feeding
    // tx/rx/ul workers - which must never underflow - keep priority over the heavier modulation/demodulation compute.
    task_executor* high_prio = make_worker("ru_phy_hp" + suffix,
                                           "ru_phy_hp_exec" + suffix,
                                           task_worker_queue_size,
                                           std::chrono::microseconds{10},
                                           os_thread_realtime_priority::max() - 2);

    if (profile == "single") {
      task_executor*                              bb = make_worker("ru_phy" + suffix,
                                      "ru_phy_exec" + suffix,
                                      128,
                                      std::chrono::microseconds{10},
                                      os_thread_realtime_priority::max());
      ru_sdr_executor_mapper_single_configuration map_cfg;
      map_cfg.radio_exec         = radio;
      map_cfg.high_prio_executor = high_prio;
      map_cfg.baseband_exec      = {bb};
      sdr_exec_mappers[i]        = create_ru_sdr_executor_mapper(map_cfg);
    } else if (profile == "dual") {
      task_executor*                            tx = make_worker("ru_phy_tx" + suffix,
                                      "ru_phy_tx_exec" + suffix,
                                      128,
                                      std::chrono::microseconds{10},
                                      os_thread_realtime_priority::max());
      task_executor*                            rx = make_worker("ru_phy_rx" + suffix,
                                      "ru_phy_rx_exec" + suffix,
                                      2,
                                      std::chrono::microseconds{10},
                                      os_thread_realtime_priority::max() - 1);
      ru_sdr_executor_mapper_dual_configuration map_cfg;
      map_cfg.radio_exec         = radio;
      map_cfg.high_prio_executor = high_prio;
      map_cfg.baseband_exec = {ru_sdr_executor_mapper_dual_configuration::cell_executors{.tx_exec = tx, .rx_exec = rx}};
      sdr_exec_mappers[i]   = create_ru_sdr_executor_mapper(map_cfg);
    } else { // triple
      task_executor*                              tx = make_worker("ru_phy_tx" + suffix,
                                      "ru_phy_tx_exec" + suffix,
                                      128,
                                      std::chrono::microseconds{10},
                                      os_thread_realtime_priority::max());
      task_executor*                              rx = make_worker("ru_phy_rx" + suffix,
                                      "ru_phy_rx_exec" + suffix,
                                      1,
                                      std::chrono::microseconds{10},
                                      os_thread_realtime_priority::max() - 2);
      task_executor*                              ul = make_worker("ru_phy_ul" + suffix,
                                      "ru_phy_ul_exec" + suffix,
                                      128,
                                      std::chrono::microseconds{10},
                                      os_thread_realtime_priority::max() - 1);
      ru_sdr_executor_mapper_triple_configuration map_cfg;
      map_cfg.radio_exec         = radio;
      map_cfg.high_prio_executor = high_prio;
      map_cfg.baseband_exec      = {
          ru_sdr_executor_mapper_triple_configuration::cell_executors{.tx_exec = tx, .rx_exec = rx, .ul_exec = ul}};
      sdr_exec_mappers[i] = create_ru_sdr_executor_mapper(map_cfg);
    }
  }

  void stop() { exec_mng.stop(); }

  task_execution_manager exec_mng;
  task_executor*         ru_timing_exec = nullptr;

  std::vector<task_executor*>                          ru_rx_exec;
  std::vector<task_executor*>                          ru_emulators_exec;
  std::vector<task_executor*>                          ru_ofh_tx_exec;
  std::vector<std::unique_ptr<ru_sdr_executor_mapper>> sdr_exec_mappers;
};

} // namespace

static std::string config_file;

/// Flag that indicates if the application is running or being shutdown.
static std::atomic<bool> is_app_running = {true};
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
  ocudulog::flush();
}

static void print_header()
{
  fmt::print(fmt::emphasis::bold,
             "| {:^8} | {:^3} | {:^11} | {:^11} | {:^11} | {:^11} | {:^15} | {:^13} | {:^13} | {:^13} | {:^15} | "
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
}

int main(int argc, char** argv)
{
  // Number of seconds between printing the statistics header.
  static constexpr unsigned SECONDS_BETWEEN_HEADERS = 20;

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

  // The GPS timing worker ticks a single numerology for every sector it drives, so all cells must share the same SCS.
  const subcarrier_spacing common_scs = ru_emulator_parsed_cfg.ru_cfg.front().common_scs;
  for (const auto& cell_cfg : ru_emulator_parsed_cfg.ru_cfg) {
    if (cell_cfg.common_scs != common_scs) {
      report_error("Invalid configuration detected, all cells must use the same common subcarrier spacing\n");
    }
  }

  // Set up logging.
  ocudulog::sink* log_sink = (ru_emulator_parsed_cfg.log_cfg.filename == "stdout")
                                 ? ocudulog::create_stdout_sink()
                                 : ocudulog::create_file_sink(ru_emulator_parsed_cfg.log_cfg.filename);
  if (log_sink == nullptr) {
    report_error("Could not create application main log sink.\n");
  }
  ocudulog::set_default_sink(*log_sink);
  ocudulog::init();

  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("RU_EMU", false);
  logger.set_level(ru_emulator_parsed_cfg.log_cfg.level);

  // Log the whole input configuration, defaults included, whatever the application log level is: a log that carries the
  // configuration that produced it is enough on its own to support someone else's deployment. The dump is built from
  // the parsed structures rather than from the parser, which keeps each cell as one opaque block and cannot expand the
  // per-cell defaults. Its keys are the ones the parser accepts, so it can be saved and replayed with -c.
  ocudulog::basic_logger& config_logger = ocudulog::fetch_basic_logger("CONFIG", false);
  config_logger.set_level(ocudulog::basic_levels::info);
  YAML::Node config_node;
  fill_ru_emulator_appconfig_in_yaml_schema(config_node, ru_emulator_parsed_cfg);
  config_logger.info("Input configuration (all values): \n{}", YAML::Dump(config_node));

  // Snapshot the host CPU topology before DPDK EAL pins the calling thread to its main lcore. The worker affinity
  // masks are built after EAL initialization and must retain the CPU set available when the application started.
  (void)cpu_architecture_info::get();

  bool uses_dpdk = false;
#ifdef DPDK_FOUND
  uses_dpdk = ru_emulator_parsed_cfg.dpdk_config.has_value();

  // Initialize DPDK EAL.
  std::unique_ptr<dpdk::dpdk_eal> eal;
  if (uses_dpdk) {
    // Prepend the application name in argv[0] as it is expected by EAL.
    eal = dpdk::create_dpdk_eal(std::string(argv[0]) + " " + ru_emulator_parsed_cfg.dpdk_config->eal_args,
                                ocudulog::fetch_basic_logger("EAL", false));
    if (!eal) {
      report_error("Failed to initialize DPDK EAL\n");
    }
  }
#endif
  // Create workers and executors.
  worker_manager workers(ru_emulator_parsed_cfg);

  // SDR Radio Unit error notifier (shared by all SDR RUs).
  ru_error_notifier_dummy error_notifier;

  // Create RU emulators. Each one owns its Ethernet receiver; its O-RU sector owns the Ethernet transmitter.
  std::vector<std::unique_ptr<ru_emulator>> ru_emulators;
  for (unsigned i = 0, e = ru_emulator_parsed_cfg.ru_cfg.size(); i != e; ++i) {
    ru_emulators.push_back(create_ru_emulator(i,
                                              logger,
                                              *workers.ru_emulators_exec[i],
                                              *workers.ru_rx_exec[i],
                                              *workers.ru_ofh_tx_exec[i],
                                              uses_dpdk,
                                              workers.sdr_exec_mappers[i].get(),
                                              error_notifier,
                                              ru_emulator_parsed_cfg.ru_cfg[i]));
  }

  // Create timing worker.
  ru_emulator_timing_notifier timing_notifier(logger, *workers.ru_timing_exec, common_scs);

  // Aggregate the OTA symbol boundary notifiers of all sectors and subscribe them once (subscribe overwrites).
  std::vector<ofh::ota_symbol_boundary_notifier*> ota_symbol_notifiers;
  for (auto& ru : ru_emulators) {
    auto notifiers = ru->get_ota_notifiers();
    ota_symbol_notifiers.insert(ota_symbol_notifiers.end(), notifiers.begin(), notifiers.end());
  }
  timing_notifier.subscribe(ota_symbol_notifiers);

  // Start RU emulators.
  timing_notifier.start();
  for (auto& ru : ru_emulators) {
    ru->start();
  }
  fmt::print("Running. Waiting for incoming packets...\n");

  unsigned wait_to_print_hdr = 0;
  while (is_app_running) {
    if (wait_to_print_hdr == 0) {
      wait_to_print_hdr = SECONDS_BETWEEN_HEADERS;
      print_header();
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
    --wait_to_print_hdr;

    for (unsigned i = 0, e = ru_emulators.size(); i != e; ++i) {
      ru_emulators[i]->print_statistics(i);
    }
  }

  timing_notifier.stop();
  for (auto& ru : ru_emulators) {
    ru->stop();
  }
  workers.stop();
  ocudulog::flush();

  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  fmt::print("\nRU emulator app stopped\n");

  return 0;
}
