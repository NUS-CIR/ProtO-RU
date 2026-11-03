// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ethernet/ethernet_rx_buffer_pool.h"
#include "ofh_integration_test_config.h"
#include "ofh_integration_test_helpers.h"
#include "ofh_integration_test_helpers_cat_b.h"
#include "ofh_integration_test_non_rt_ru_factory.h"
#include "ocudu/adt/bounded_bitset.h"
#include "ocudu/adt/circular_map.h"
#include "ocudu/adt/format.h"
#include "ocudu/ofh/ecpri/ecpri_constants.h"
#include "ocudu/ofh/ethernet/ethernet_controller.h"
#include "ocudu/ofh/ethernet/ethernet_frame_notifier.h"
#include "ocudu/ofh/ethernet/ethernet_frame_pool.h"
#include "ocudu/ofh/ethernet/ethernet_receiver.h"
#include "ocudu/ofh/ethernet/ethernet_receiver_metrics_collector.h"
#include "ocudu/ofh/ethernet/ethernet_transmitter.h"
#include "ocudu/ofh/ethernet/ethernet_transmitter_metrics_collector.h"
#include "ocudu/ofh/ofh_metrics.h"
#include "ocudu/phy/support/resource_grid_context.h"
#include "ocudu/phy/support/resource_grid_reader.h"
#include "ocudu/phy/support/resource_grid_writer.h"
#include "ocudu/phy/support/shared_resource_grid.h"
#include "ocudu/phy/support/support_factories.h"
#include "ocudu/ran/antenna_topology.h"
#include "ocudu/ru/ofh/ru_ofh_configuration.h"
#include "ocudu/ru/ofh/ru_ofh_executor_mapper_factory.h"
#include "ocudu/ru/ofh/ru_ofh_factory.h"
#include "ocudu/ru/ru_controller.h"
#include "ocudu/ru/ru_downlink_plane.h"
#include "ocudu/ru/ru_error_notifier.h"
#include "ocudu/ru/ru_metrics.h"
#include "ocudu/ru/ru_metrics_collector.h"
#include "ocudu/ru/ru_timing_notifier.h"
#include "ocudu/ru/ru_uplink_plane.h"
#include "ocudu/support/executors/task_execution_manager.h"
#include "ocudu/support/executors/task_executor.h"
#include "fmt/std.h"
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <mutex>
#include <net/if.h>
#include <netinet/ether.h>
#include <numeric>
#include <random>
#include <sys/ioctl.h>

using namespace ocudu;
using namespace ofh;
using namespace std::chrono_literals;

/// Random generator.
static std::mt19937 rgen(0);

/// Transmission window parameters expressed in symbols, given the 30kHz scs.
unsigned T1a_max_cp_dl = 13; // 470us.
unsigned T1a_min_cp_dl = 8;  // 258us.
unsigned T1a_max_cp_ul = 8;  // 300us.
unsigned T1a_min_cp_ul = 8;  // 285us.
unsigned T1a_max_up    = 9;  // 350us.
unsigned T1a_min_up    = 2;  // 50us.
/// Reception window parameters expressed in symbols, given the 30kHz scs.
unsigned Ta4_min = 1;  // 35us.
unsigned Ta4_max = 28; // 1ms.

static const tdd_ul_dl_pattern tdd_pattern_7d2u{10, 7, 0, 2, 0};
static const tdd_ul_dl_pattern tdd_pattern_6d3u{10, 6, 0, 3, 0};
static tdd_ul_dl_pattern       tdd_pattern = tdd_pattern_7d2u;

static const unsigned vlan_tag               = 9;
static const unsigned processing_delay_slots = 6;
static unsigned       nof_antennas_dl        = 4;
static unsigned       nof_antennas_ul        = 2;

static std::atomic<unsigned> nof_malformed_packets{0};
static std::atomic<unsigned> nof_missing_dl_packets{0};

/// Message counters indexed by eAxC, updated concurrently during the test.
using eaxc_counters = std::array<std::atomic<unsigned>, MAX_SUPPORTED_EAXC_ID_VALUE>;
/// Number of messages expected per eAxC, indexed by eAxC.
using eaxc_expected_counters = std::array<unsigned, MAX_SUPPORTED_EAXC_ID_VALUE>;

namespace {

/// Dummy Radio Unit error notifier.
class dummy_ru_error_notifier : public ru_error_notifier
{
public:
  void on_late_downlink_message(const ru_error_context& context) override
  {
    nof_late_dl_grids.fetch_add(1, std::memory_order_relaxed);
  }
  void on_late_uplink_message(const ru_error_context& context) override {}
  void on_late_prach_message(const ru_error_context& context) override {}

  /// Number of late downlink resource grids dropped by the OFH RU.
  std::atomic<unsigned> nof_late_dl_grids{0};
};

} // namespace

static test::test_parameters test_params;

/// Returns true if the test runs with downlink beamforming, i.e., against a Category B O-RU.
static bool is_cat_b_enabled()
{
  return test_params.beamforming_cfg.enable;
}

namespace {

class dummy_frame_notifier : public ether::frame_notifier
{
  // See interface for documentation.
  void on_new_frame(ether::unique_rx_buffer buffer) override {}
};
dummy_frame_notifier dummy_notifier;

/// Test Ethernet receiver interface.
class test_ether_receiver : public ether::receiver,
                            public ether::receiver_operation_controller,
                            private ether::receiver_metrics_collector
{
public:
  test_ether_receiver(ocudulog::basic_logger& logger_) : logger(logger_), notifier(dummy_notifier) {}
  virtual ~test_ether_receiver() = default;

  receiver_operation_controller& get_operation_controller() override { return *this; }

  void start(ether::frame_notifier& notifier_) override
  {
    notifier = std::ref(notifier_);
    logger.debug("Test Ethernet receiver started");
  }

  void stop() override
  {
    stop_requested.store(true, std::memory_order_relaxed);

    while (is_running.load(std::memory_order_relaxed)) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  // See interface for documentation.
  ether::receiver_metrics_collector* get_metrics_collector() override { return this; }

  virtual void push_new_data(span<const uint8_t> frame) = 0;

protected:
  /// Accounts a frame delivered to the OFH receiver.
  void update_rx_metrics(span<const uint8_t> frame) { nof_rx_bytes.fetch_add(frame.size(), std::memory_order_relaxed); }

  std::atomic<uint64_t>                         nof_rx_bytes{0};
  ocudulog::basic_logger&                       logger;
  std::reference_wrapper<ether::frame_notifier> notifier;
  std::atomic<bool>                             is_running{false};
  std::atomic<bool>                             stop_requested{false};

private:
  // See interface for documentation.
  void collect_metrics(ether::receiver_metrics& metric) override
  {
    metric                 = {};
    metric.total_nof_bytes = nof_rx_bytes.exchange(0, std::memory_order_relaxed);
  }
};

/// Dummy Ethernet receiver that receives data from RU emulator and pushes them to the OFH receiver without using real
/// Ethernet interface.
class dummy_eth_receiver : public test_ether_receiver
{
public:
  dummy_eth_receiver(ocudulog::basic_logger& logger_, ether::ethernet_rx_buffer_pool& pool) :
    test_ether_receiver(logger_), buffer_pool(pool)
  {
  }

  void push_new_data(span<const uint8_t> frame) override
  {
    if (stop_requested.load(std::memory_order_relaxed)) {
      return;
    }
    is_running.store(true, std::memory_order::memory_order_relaxed);

    auto exp_buffer = buffer_pool.reserve();
    while (!exp_buffer.has_value()) {
      if (stop_requested.load(std::memory_order_relaxed)) {
        is_running.store(false, std::memory_order::memory_order_relaxed);
        return;
      }
      std::this_thread::sleep_for(std::chrono::microseconds(10));
      exp_buffer = buffer_pool.reserve();
    }
    ether::ethernet_rx_buffer_impl buffer = std::move(exp_buffer.value());
    std::memcpy(buffer.storage().data(), frame.data(), frame.size());
    buffer.resize(frame.size());

    update_rx_metrics(frame);
    notifier.get().on_new_frame(ether::unique_rx_buffer(std::move(buffer)));
    is_running.store(false, std::memory_order::memory_order_relaxed);
  }

private:
  ether::ethernet_rx_buffer_pool& buffer_pool;
};

/// Ethernet receiver using loopback ('lo') interface.
class lo_eth_receiver : public test_ether_receiver
{
public:
  lo_eth_receiver(ocudulog::basic_logger& logger_) : test_ether_receiver(logger_) { init_loopback_connection(); }

  // See interface for documentation.
  void push_new_data(span<const uint8_t> frame) override
  {
    if (::sendto(socket_fd,
                 frame.data(),
                 frame.size(),
                 0,
                 reinterpret_cast<::sockaddr*>(&socket_address),
                 sizeof(socket_address)) < 0) {
      fmt::print("sendto failed to transmit {} bytes", frame.size());
    }
  }

private:
  void init_loopback_connection()
  {
    socket_fd = ::socket(AF_PACKET, SOCK_RAW | SOCK_NONBLOCK, IPPROTO_RAW);
    if (socket_fd < 0) {
      report_error("Unable to open raw socket for Ethernet gateway: {}", ::strerror(errno));
    }

    // Get the index of loopback interface.
    ::ifreq if_idx = {};
    ::strncpy(if_idx.ifr_name, "lo", IFNAMSIZ - 1);
    if (::ioctl(socket_fd, SIOCGIFINDEX, &if_idx) < 0) {
      report_error("Unable to get index for loopback interface");
    }

    // Prepare the socket address used by sendto.
    socket_address             = {};
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    socket_address.sll_halen   = ETH_ALEN;
  }

  /// Ethernet structures.
  int         socket_fd = -1;
  sockaddr_ll socket_address;
};

/// Dummy RU notifier class for symbol events.
class dummy_rx_symbol_notifier : public ru_uplink_plane_rx_symbol_notifier
{
public:
  // See interface for documentation.
  void on_new_uplink_symbol(const ru_uplink_rx_symbol_context& context,
                            const shared_resource_grid&        grid,
                            bool                               is_valid) override
  {
    ocudu_assert(grid, "Invalid grid.");
    (is_valid ? nof_valid_symbols : nof_invalid_symbols).fetch_add(1, std::memory_order_relaxed);
  }

  // See interface for documentation.
  void on_new_prach_window_data(const prach_buffer_context& context, shared_prach_buffer buffer) override
  {
    nof_prach_windows.fetch_add(1, std::memory_order_relaxed);
  }

  /// Number of valid uplink symbols notified.
  std::atomic<unsigned> nof_valid_symbols{0};
  /// Number of invalid uplink symbols notified, i.e., not fully received within the reception window.
  std::atomic<unsigned> nof_invalid_symbols{0};
  /// Number of PRACH windows notified.
  std::atomic<unsigned> nof_prach_windows{0};
};

/// RU emulator class responsible for generating uplink packets with random IQ data.
class test_ru_emulator
{
  /// Ethernet packet size, set to the value used by OFH implementation.
  static constexpr unsigned ethernet_frame_size = 9000;

  /// Helper structure used internally to group OFH header parameters.
  struct header_parameters {
    uint8_t  port;
    unsigned payload_size;
    unsigned start_prb;
    unsigned nof_prbs;
  };

public:
  /// Constructor.
  test_ru_emulator(ocudulog::basic_logger& logger_,
                   task_executor&          executor_,
                   test_ether_receiver&    receiver_,
                   ru_compression_params   compr_params_,
                   unsigned                nof_prb_) :
    logger(logger_), executor(executor_), receiver(receiver_), compr_params(compr_params_), nof_prb(nof_prb_)
  {
    ul_eaxc.assign(test_params.ul_port_id.begin(), test_params.ul_port_id.end());
    prepare_test_data();

    for (unsigned K = 0; K != MAX_SUPPORTED_EAXC_ID_VALUE; ++K) {
      seq_counters.emplace(K, 0);
    }
  }

  /// Generates UL packets with random IQ data for the specified slot and sends to an ethernet receiver.
  void send_uplink_data(slot_point slot)
  {
    if (!executor.execute([this, slot]() { send_uplink(slot); })) {
      logger.warning("Failed to dispatch uplink task");
    }
  }

private:
  void send_uplink(slot_point slot)
  {
    for (unsigned symbol = 0; symbol != get_nsymb_per_slot(cyclic_prefix::NORMAL); ++symbol) {
      // Prepare symbol for all eAxC.
      for (unsigned eaxc_id = 0, end = ul_eaxc.size(); eaxc_id != end; ++eaxc_id) {
        auto& frames = test_data[eaxc_id];
        for (auto& frame : frames) {
          set_header_parameters(frame, slot, symbol, ul_eaxc[eaxc_id]);
        }
      }
      // Send symbol.
      for (const auto& frames : test_data) {
        send(frames);
      }
    }
    nof_answered_slots.fetch_add(1, std::memory_order_relaxed);
    logger.info("RU sent UL in slot {}", slot);
  }

  /// Sends byte arrays to the loopback ethernet interface.
  void send(const std::vector<std::vector<uint8_t>>& frames)
  {
    for (const auto& frame : frames) {
      receiver.push_new_data(frame);
    }
    nof_sent_messages.fetch_add(frames.size(), std::memory_order_relaxed);
  }

  void set_header_parameters(span<uint8_t> frame, slot_point slot, unsigned symbol, unsigned eaxc)
  {
    // Real receiver sends VLAN parameters as part of a Ethernet frame.
    unsigned offset = (test_params.use_loopback_receiver) ? 4 : 0;

    // Set timestamp.
    uint8_t octet      = 0;
    frame[23 + offset] = uint8_t(slot.sfn());
    // Subframe index; offset: 4, 4 bits long.
    octet |= uint8_t(slot.subframe_index()) << 4u;
    // Four MSBs of the slot index within 1ms subframe; offset: 4, 6 bits long.
    octet |= uint8_t(slot.subframe_slot_index() >> 2u);
    frame[24 + offset] = octet;

    octet = 0;
    octet |= uint8_t(slot.subframe_slot_index() & 0x3) << 6u;
    octet |= uint8_t(symbol);
    frame[25 + offset] = octet;

    // Set sequence index.
    uint8_t& seq_id    = seq_counters[eaxc];
    frame[20 + offset] = seq_id++;
  }

  void initialize_header(span<uint8_t> frame, header_parameters params) const
  {
    // Doesn't include VLAN header, as the OFH receiver expects a NIC to strip it.
    static const uint8_t hdr_template_no_vlan[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                   0x00, 0xae, 0xfe, 0x10, 0x00, 0x1d, 0xea, 0x00, 0x00, 0x00, 0x80,
                                                   0x10, 0xee, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, 0x00};

    static const uint8_t hdr_template_vlan[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                                0x81, 0x00, 0x00, 0x02, 0xae, 0xfe, 0x10, 0x00, 0x1d, 0xea, 0x00, 0x00,
                                                0x00, 0x80, 0x10, 0xee, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x91, 0x00};

    const uint8_t* hdr_template_ptr = (test_params.use_loopback_receiver) ? hdr_template_vlan : hdr_template_no_vlan;
    const size_t   hdr_template_size =
        (test_params.use_loopback_receiver) ? sizeof(hdr_template_vlan) : sizeof(hdr_template_no_vlan);
    unsigned offset = (test_params.use_loopback_receiver) ? 4 : 0;

    // Copy default header.
    std::memcpy(frame.data(), hdr_template_ptr, hdr_template_size);

    // Set VLAN tag.
    if (test_params.use_loopback_receiver) {
      // MSBs containing the PCP + VID.
      frame[14] = static_cast<uint8_t>(vlan_tag >> 8);
      // LSBs containing the VID.
      frame[15] = static_cast<uint8_t>(vlan_tag & 0xff);
    }

    // Set correct payload size.
    uint16_t payload_size = htons(params.payload_size);
    std::memcpy(&frame[16 + offset], &payload_size, sizeof(uint16_t));

    // Set port ID.
    frame[19 + offset] = params.port;

    // Set start PRB and number of PRBs.
    frame[27 + offset] = uint8_t(params.start_prb >> 8u) & 0x3;
    frame[28 + offset] = uint8_t(params.start_prb);
    frame[29 + offset] = uint8_t((params.nof_prbs == nof_prb) ? 0 : params.nof_prbs);

    // Set compression header.
    uint8_t octet = 0U;
    octet |= uint8_t(compr_params.data_width) << 4U;
    octet |= uint8_t(to_underlying(compr_params.type));
    frame[30 + offset] = octet;
  }

  static void fill_random_data(span<uint8_t> frame)
  {
    std::uniform_int_distribution<uint8_t> dist{0, 255};
    std::generate(frame.begin(), frame.end(), [&]() { return dist(rgen); });
  }

  void prepare_test_data()
  {
    units::bytes ecpri_iq_data_header_size(8);
    units::bytes ofh_header_size(10);
    units::bytes ether_header_size(14);
    if (test_params.use_loopback_receiver) {
      // VLAN parameters are added in case real Ethernet receiver is used.
      ether_header_size = units::bytes(18);
    }
    unsigned headers_size = (ether_header_size + ecpri_iq_data_header_size + ofh_header_size).value();

    prb_size = units::bits(compr_params.data_width * NOF_SUBCARRIERS_PER_RB * 2 +
                           (compr_params.type == compression_type::BFP ? 8 : 0))
                   .round_up_to_bytes();
    unsigned iq_data_size = nof_prb * prb_size.value();

    // It is assumed that maximum 2 packets required to send all data for antenna.
    unsigned              nof_frames = ((headers_size + iq_data_size) > ethernet_frame_size) ? 2u : 1u;
    std::vector<unsigned> nof_frame_prbs;
    if (nof_frames == 1) {
      nof_frame_prbs.push_back(nof_prb);
    } else {
      unsigned nof_prbs_first  = (ethernet_frame_size - headers_size) / prb_size.value();
      unsigned nof_prbs_second = nof_prb - nof_prbs_first;
      nof_frame_prbs.push_back(nof_prbs_first);
      nof_frame_prbs.push_back(nof_prbs_second);
    }

    /// Initializes ethernet packet headers for all antennas. Timestamp, sequence index and symbol index will be updated
    /// on every transmission.
    for (unsigned port = 0; port != nof_antennas_ul; ++port) {
      test_data.emplace_back();
      std::vector<std::vector<uint8_t>>& ether_frames = test_data.back();

      unsigned start_prb = 0;
      for (unsigned j = 0; j != nof_frames; ++j) {
        unsigned data_size = nof_frame_prbs[j] * prb_size.value();

        ether_frames.emplace_back();
        std::vector<uint8_t>& frame = ether_frames.back();
        frame.resize(headers_size + data_size);

        // Prepare header.
        span<uint8_t>     frame_header(frame.data(), headers_size);
        header_parameters params;
        params.port         = ul_eaxc[port];
        params.payload_size = data_size + ofh_header_size.value() + ecpri::ECPRI_COMMON_HEADER_SIZE.value();
        params.start_prb    = start_prb;
        params.nof_prbs     = nof_frame_prbs[j];
        initialize_header(frame_header, params);

        // Prepare IQ data.
        fill_random_data(span<uint8_t>(frame).last(data_size));

        start_prb += nof_frame_prbs[j];
      }
    }
  }

public:
  /// Returns the number of uplink U-Plane messages sent per symbol and eAxC.
  unsigned get_nof_messages_per_symbol() const { return test_data.front().size(); }

  /// Number of uplink slots answered with U-Plane messages.
  std::atomic<unsigned> nof_answered_slots{0};
  /// Number of uplink U-Plane messages sent.
  std::atomic<unsigned> nof_sent_messages{0};

private:
  ocudulog::basic_logger&     logger;
  task_executor&              executor;
  test_ether_receiver&        receiver;
  const ru_compression_params compr_params;
  const unsigned              nof_prb;
  units::bytes                prb_size;
  /// Stores byte arrays for each antenna.
  std::vector<std::vector<std::vector<uint8_t>>>                     test_data;
  static_circular_map<uint8_t, uint8_t, MAX_SUPPORTED_EAXC_ID_VALUE> seq_counters;
  static_vector<unsigned, ofh::MAX_NOF_SUPPORTED_EAXC>               ul_eaxc;
};

/// \brief DU emulator that pushes resource grids to the OFH RU implementation.
///
/// Every slot notified by RU is processed in the DU emulator executor, until the configured number of test slots has
/// been processed. Then, empty downlink resource grids are pushed during the flush slots, see \ref get_nof_flush_slots.
class test_du_emulator
{
  /// Number of slots to wait after the OTA time of the last processed slot. This allows to finish uplink processing.
  static constexpr unsigned nof_rx_window_slots = 3;

public:
  test_du_emulator(ocudulog::basic_logger&        logger_,
                   task_executor&                 executor_,
                   resource_grid_pool&            dl_rg_pool_,
                   resource_grid_pool&            empty_dl_rg_pool_,
                   resource_grid_pool&            ul_rg_pool_,
                   ru_downlink_plane_handler&     dl_handler_,
                   ru_uplink_plane_handler&       ul_handler_,
                   test::dl_beam_registry&        beam_registry_,
                   const dummy_ru_error_notifier& error_notifier_) :
    logger(logger_),
    dl_rg_pool(dl_rg_pool_),
    empty_dl_rg_pool(empty_dl_rg_pool_),
    ul_rg_pool(ul_rg_pool_),
    executor(executor_),
    dl_handler(dl_handler_),
    ul_handler(ul_handler_),
    beam_registry(beam_registry_),
    error_notifier(error_notifier_)
  {
  }

  /// \brief Handles a new TTI boundary notified by the RU.
  ///
  /// \note this method is called from the RU timing thread.
  void handle_tti_boundary(slot_point slot)
  {
    // If we arrived at the end of the test, wait for the processing delay, as the TTI boundary leads the OTA time by
    // the processing delay.
    if (nof_dispatched_slots == test_params.nof_test_slots + get_nof_flush_slots()) {
      if (!is_test_finished() && (last_slot + processing_delay_slots + nof_rx_window_slots <= slot)) {
        test_finished.store(true, std::memory_order_relaxed);
      }
      return;
    }

    if (nof_dispatched_slots == 0) {
      // Start on a frame boundary, so that the test starts at the beginning of the TDD pattern.
      if (slot.slot_index() != 0) {
        return;
      }
      fmt::print("Initial slot set to {}\n", slot);
    }
    bool is_flush_slot = (nof_dispatched_slots >= test_params.nof_test_slots);
    ++nof_dispatched_slots;
    last_slot = slot;

    if (!executor.execute([this, slot, is_flush_slot]() {
          if (is_flush_slot) {
            flush_slot(slot);
          } else {
            process_slot(slot);
          }
        })) {
      logger.warning("Failed to dispatch DU emulator task for slot {}", slot);
    }
  }

  bool is_test_finished() const { return test_finished.load(std::memory_order_relaxed); }

  /// Number of downlink resource grids pushed to the OFH RU.
  std::atomic<unsigned> nof_dl_grids{0};
  /// Number of downlink resource grids dropped by the OFH RU for being late.
  std::atomic<unsigned> nof_late_dl_grids{0};
  /// Number of downlink C-Plane messages expected per eAxC (one per non-late grid).
  eaxc_counters nof_expected_dl_cplane_messages = {};
  /// Number of uplink requests sent to the OFH RU.
  std::atomic<unsigned> nof_ul_requests{0};

private:
  /// \brief Gets the number of slots flushing the OFH transmitter frame pools after the test slots.
  ///
  /// The OFH transmitter only accounts for the late messages left in its frame pool when a new resource grid reuses
  /// the pool slot. The flush covers every pool slot twice, which also accounts for the messages written in the pool
  /// after the first pass.
  static unsigned get_nof_flush_slots()
  {
    return 2 * static_cast<unsigned>(ether::eth_frame_pool::pool_size_in_slots());
  }

  /// Pushes an empty downlink resource grid, which only clears the OFH transmitter frame pools for the given slot.
  void flush_slot(slot_point slot)
  {
    shared_resource_grid dl_grid = empty_dl_rg_pool.allocate_resource_grid(slot);
    if (!dl_grid) {
      logger.warning("No empty resource grid is available for flushing DL slot {}", slot);
      return;
    }
    dl_handler.handle_dl_data({slot, 0}, dl_grid);
  }

  void process_slot(slot_point slot)
  {
    // Max attempts of allocating resource grid from the pool.
    static constexpr unsigned rg_alloc_max_attempts = 10;

    unsigned slot_id    = slot.slot_index() % tdd_pattern.dl_ul_tx_period_nof_slots;
    bool     is_dl_slot = (slot_id < tdd_pattern.nof_dl_slots);
    bool     is_ul_slot = (slot_id >= tdd_pattern.dl_ul_tx_period_nof_slots - tdd_pattern.nof_ul_slots);

    ocudu_assert(!(is_dl_slot && is_ul_slot), "Invalid slot type: both DL and UL can not be set simultaneously");

    int alloc_attempts = rg_alloc_max_attempts;
    // Push downlink data.
    if (is_dl_slot) {
      resource_grid_context context{slot, 0};
      shared_resource_grid  dl_grid;
      while (!dl_grid && alloc_attempts--) {
        dl_grid = dl_rg_pool.allocate_resource_grid(slot);
        if (!dl_grid) {
          std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
      }

      if (dl_grid) {
        test::dl_beam_list beams = get_transmitted_beams(dl_grid.get_reader());
        // Save this grid's beams in the beam registry, so that the test gateway can fetch it when analyzing the
        // received C-Plane messages.
        beam_registry.write(slot, beams);

        // Save the previous number of late grids to detect if the new grid is discarded as late or not.
        unsigned prev_nof_lates = error_notifier.nof_late_dl_grids.load(std::memory_order_relaxed);

        dl_handler.handle_dl_data(context, dl_grid);
        nof_dl_grids.fetch_add(1, std::memory_order_relaxed);

        if (error_notifier.nof_late_dl_grids.load(std::memory_order_relaxed) != prev_nof_lates) {
          nof_late_dl_grids.fetch_add(1, std::memory_order_relaxed);
        } else {
          // The k-th transmitted beam-port is carried in the spatial stream associated with the k-th DL eAxC.
          for (unsigned i_beam = 0, i_end = beams.size(); i_beam != i_end; ++i_beam) {
            nof_expected_dl_cplane_messages[test_params.dl_port_id[i_beam]].fetch_add(1, std::memory_order_relaxed);
          }
        }
        logger.info("DU emulator pushed DL data in slot {}", slot);
      } else {
        logger.warning("No resource grid is available for processing DL slot");
      }
    }

    // Request uplink data.
    if (is_ul_slot) {
      resource_grid_context context{slot, 0};
      shared_resource_grid  ul_grid;
      while (!ul_grid && alloc_attempts--) {
        ul_grid = ul_rg_pool.allocate_resource_grid(slot);
        if (!ul_grid) {
          std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
      }

      if (ul_grid) {
        ul_handler.handle_new_uplink_slot(context, ul_grid);
        nof_ul_requests.fetch_add(1, std::memory_order_relaxed);
        logger.info("DU emulator requested UL data in slot {}", slot);
      } else {
        logger.warning("No resource grid is available for processing UL slot");
      }
    }
  }

  /// \brief Gets the list of non-empty beam-ports of the given resource grid.
  ///
  /// Category A transmits every antenna port, whereas Category B only transmits the non-empty beam-ports.
  static test::dl_beam_list get_transmitted_beams(const resource_grid_reader& reader)
  {
    test::dl_beam_list beams;
    for (unsigned i_port = 0, i_end = reader.get_nof_ports(); i_port != i_end; ++i_port) {
      if (!is_cat_b_enabled() || !reader.is_empty(i_port)) {
        beams.push_back(i_port);
      }
    }
    return beams;
  }

  ocudulog::basic_logger&        logger;
  resource_grid_pool&            dl_rg_pool;
  resource_grid_pool&            empty_dl_rg_pool;
  resource_grid_pool&            ul_rg_pool;
  task_executor&                 executor;
  ru_downlink_plane_handler&     dl_handler;
  ru_uplink_plane_handler&       ul_handler;
  test::dl_beam_registry&        beam_registry;
  const dummy_ru_error_notifier& error_notifier;

  /// Number of slots dispatched for processing.
  unsigned nof_dispatched_slots = 0;
  /// Last slot dispatched for processing.
  slot_point        last_slot;
  std::atomic<bool> test_finished{false};
};

/// Dummy RU notifier class for timing events that forwards the TTI boundaries to the DU emulator.
class dummy_timing_notifier : public ru_timing_notifier
{
public:
  /// Connects the DU emulator.
  void connect_du(test_du_emulator& du_emulator_) { du_emulator.store(&du_emulator_, std::memory_order_release); }

  // See interface for documentation.
  void on_tti_boundary(const tti_boundary_context& slot_context) override
  {
    if (test_du_emulator* du = du_emulator.load(std::memory_order_acquire)) {
      du->handle_tti_boundary(slot_context.slot.without_hyper_sfn());
    }
  }

  // See interface for documentation.
  void on_ul_half_slot_boundary(slot_point slot) override {}

  // See interface for documentation.
  void on_ul_full_slot_boundary(slot_point slot) override {}

private:
  std::atomic<test_du_emulator*> du_emulator{nullptr};
};

/// Ethernet transmitter gateway that analyzes incoming packets and checks integrity of the DL packets, as well as asks
/// RU emulator for UL traffic generation.
class test_gateway : public ether::transmitter, private ether::transmitter_metrics_collector
{
  /// Minimum size of a message transmitted by the DU, it covers all the header fields peeked by the gateway.
  static constexpr unsigned min_message_size = 30;

public:
  test_gateway() :
    scs(test_params.scs),
    nof_symbols(get_nsymb_per_slot(cyclic_prefix::NORMAL)),
    seq_counter_initialized(MAX_SUPPORTED_EAXC_ID_VALUE)
  {
    for (unsigned K = 0; K != MAX_SUPPORTED_EAXC_ID_VALUE; ++K) {
      seq_counters.insert(K, 0);
    }
  }

  void connect_ru(test_ru_emulator* ru_emulator_) { ru_emulator = ru_emulator_; }

  /// \brief Connects the downlink C-Plane message checker.
  ///
  /// \note Must be called before starting the RU.
  void connect_dl_cplane_checker(test::dl_cplane_checker& checker) { dl_checker = &checker; }

  // See interface for documentation.
  void send(span<span<const uint8_t>> frames) override
  {
    for (auto frame : frames) {
      nof_tx_bytes.fetch_add(frame.size(), std::memory_order_relaxed);
      if (frame.size() < min_message_size) {
        nof_malformed_packets++;
        continue;
      }

      unsigned eaxc = peek_eaxc(frame);
      ocudu_assert(eaxc < MAX_SUPPORTED_EAXC_ID_VALUE, "Invalid eAxC={} detected", eaxc);
      bool is_uplane = (peek_message_type(frame) == ecpri::message_type::iq_data);

      // For DL messages check seq id and make sure packets for all antennas were transmitted.
      if (peek_direction(frame) == data_direction::downlink) {
        (is_uplane ? dl_uplane_counters : dl_cplane_counters)[eaxc].fetch_add(1, std::memory_order_relaxed);
        if (is_uplane) {
          check_and_update_sequence_id(frame);
        } else {
          ocudu_assert(dl_checker != nullptr, "DL C-Plane checker uninitialized");
          dl_checker->check(frame, peek_slot_point(frame), eaxc);
        }
        continue;
      }

      // The DU only transmits uplink C-Plane messages.
      ocudu_assert(!is_uplane, "Unexpected uplink U-Plane message transmitted by the DU");
      ul_cplane_counters[eaxc].fetch_add(1, std::memory_order_relaxed);

      // For UL message ask the RU emulator to send UP packets to the loopback interface, once per slot.
      ocudu_assert(ru_emulator != nullptr, "RU emulator uninitialized");
      slot_point slot = peek_slot_point(frame);
      if (slot != last_ul_slot) {
        ru_emulator->send_uplink_data(slot);
        last_ul_slot = slot;
      }
    }
  }

  // See interface for documentation.
  ether::transmitter_metrics_collector* get_metrics_collector() override { return this; }

  /// Downlink C-Plane messages.
  eaxc_counters dl_cplane_counters = {};
  /// Downlink U-Plane messages.
  eaxc_counters dl_uplane_counters = {};
  /// Uplink CPlane messages.
  eaxc_counters ul_cplane_counters = {};

private:
  // See interface for documentation.
  void collect_metrics(ether::transmitter_metrics& metric) override
  {
    metric                 = {};
    metric.total_nof_bytes = nof_tx_bytes.exchange(0, std::memory_order_relaxed);
  }

  void check_and_update_sequence_id(span<const uint8_t> message)
  {
    // Retrieve eAxC and SeqID from codified message.
    unsigned seq_id = message[24];
    unsigned eaxc   = peek_eaxc(message);

    ocudu_assert(eaxc < MAX_SUPPORTED_EAXC_ID_VALUE, "Invalid eAxC={} detected", eaxc);

    if (!seq_counter_initialized.test(eaxc)) {
      seq_counter_initialized.set(eaxc);
      seq_counters[eaxc] = seq_id + 1;
      return;
    }

    uint8_t& expected_seq_id = seq_counters[eaxc];
    if (seq_id == expected_seq_id) {
      expected_seq_id++;
      return;
    }

    nof_missing_dl_packets++;
    expected_seq_id = seq_id + 1;
  }

  static unsigned peek_eaxc(span<const uint8_t> message)
  {
    // eAxC is codified in the bytes 22-23 of the Ethernet packet.
    return (unsigned(message[22]) << 8u) | message[23];
  }

  static data_direction peek_direction(span<const uint8_t> message)
  {
    // Filter index is codified in the byte 26, bit 7.
    unsigned direction = (message[26] & 0x80) >> 7u;
    return (direction == 1) ? data_direction::downlink : data_direction::uplink;
  }

  static ecpri::message_type peek_message_type(span<const uint8_t> message)
  {
    return static_cast<ecpri::message_type>(message[19]);
  }

  slot_point peek_slot_point(span<const uint8_t> message) const
  {
    // Slot is codified in the bytes 27-29 of the Ethernet packet.
    if (message.size() < 30) {
      return slot_point{};
    }

    uint8_t  frame             = message[27];
    uint8_t  subframe_and_slot = message[28];
    uint8_t  subframe          = subframe_and_slot >> 4;
    unsigned slot_id           = 0;
    slot_id |= (subframe_and_slot & 0x0f) << 2;

    uint8_t slot_and_symbol = message[29];
    slot_id |= slot_and_symbol >> 6;

    return {to_numerology_value(scs), frame, subframe, slot_id};
  }

private:
  const subcarrier_spacing                                           scs;
  const unsigned                                                     nof_symbols;
  static_circular_map<uint8_t, uint8_t, MAX_SUPPORTED_EAXC_ID_VALUE> seq_counters;
  bounded_bitset<MAX_SUPPORTED_EAXC_ID_VALUE>                        seq_counter_initialized;
  test_ru_emulator*                                                  ru_emulator;
  test::dl_cplane_checker*                                           dl_checker = nullptr;
  std::atomic<uint64_t>                                              nof_tx_bytes{0};
  /// Last uplink slot requested to the RU emulator, only accessed from the transmitter thread.
  slot_point last_ul_slot;
};

/// Manages the workers of the test application and OFH RU.
struct worker_manager {
  static constexpr uint32_t task_worker_queue_size = 2048;
  /// DU emulator queue size.
  static constexpr uint32_t du_sim_queue_size = 8;

  worker_manager() { create_ofh_executors(); }

  void stop() { exec_mng.stop(); }

  void create_ofh_executors()
  {
    using namespace execution_config_helper;

    // Timing executor.
    {
      const std::string name      = "ru_timing";
      const std::string exec_name = "ru_timing_exec";

      const single_worker ru_worker{name,
                                    {exec_name, concurrent_queue_policy::lockfree_spsc, 4},
                                    std::chrono::microseconds{1},
                                    os_thread_realtime_priority::max() - 0};
      if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
      }
      ru_timing_exec = exec_mng.executors().at(exec_name);
    }

    // Executor for the Open Fronthaul User and Control messages codification.
    {
      unsigned          nof_workers = (test_params.is_downlink_parallelized) ? std::max(nof_antennas_dl / 2U, 1U) : 1U;
      const std::string name        = "ru_dl";
      const std::string exec_name   = "ru_dl_exec";

      const worker_pool ru_pool{name,
                                nof_workers,
                                {{exec_name, concurrent_queue_policy::locking_mpmc, task_worker_queue_size}},
                                std::chrono::microseconds(0),
                                os_thread_realtime_priority::max() - 5,
                                {}};
      if (!exec_mng.add_execution_context(create_execution_context(ru_pool))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_pool.name);
      }
      ru_dl_exec = exec_mng.executors().at(exec_name);
    }

    // Executor for Open Fronthaul messages transmission.
    {
      const std::string name      = "ru_txrx";
      const std::string exec_name = "ru_txrx_exec";

      const single_worker ru_worker{name,
                                    {exec_name, concurrent_queue_policy::lockfree_mpmc, task_worker_queue_size},
                                    std::chrono::microseconds{5},
                                    os_thread_realtime_priority::max() - 1};
      if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
      }
      ru_tx_exec = exec_mng.executors().at(exec_name);
    }

    // Executor for Open Fronthaul messages reception.
    {
      const std::string name      = "ru_rx";
      const std::string exec_name = "ru_rx_exec";

      const single_worker ru_worker{name,
                                    {exec_name, concurrent_queue_policy::lockfree_mpmc, task_worker_queue_size},
                                    std::chrono::microseconds{15},
                                    os_thread_realtime_priority::max() - 5};
      if (!exec_mng.add_execution_context(create_execution_context(ru_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_worker.name);
      }
      ru_rx_exec = exec_mng.executors().at(exec_name);
    }

    // Executor for DU emulator responsible for resource grids generation.
    {
      const std::string name      = "du_sim";
      const std::string exec_name = "du_sim_exec";

      const single_worker du_sim_worker{name,
                                        {exec_name, concurrent_queue_policy::locking_mpmc, du_sim_queue_size},
                                        std::nullopt,
                                        os_thread_realtime_priority::max() - 10};
      if (!exec_mng.add_execution_context(create_execution_context(du_sim_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", du_sim_worker.name);
      }
      test_du_sim_exec = exec_mng.executors().at(exec_name);
    }

    // Executor for RU emulator responsible for rx packets generation.
    {
      const std::string name      = "ru_sim";
      const std::string exec_name = "ru_sim_exec";

      const single_worker ru_sim_worker{name,
                                        {exec_name, concurrent_queue_policy::lockfree_spsc, task_worker_queue_size},
                                        std::chrono::microseconds{5},
                                        os_thread_realtime_priority::max() - 2};
      if (!exec_mng.add_execution_context(create_execution_context(ru_sim_worker))) {
        report_fatal_error("Failed to instantiate {} execution context", ru_sim_worker.name);
      }
      test_ru_sim_exec = exec_mng.executors().at(exec_name);
    }
  }

  task_execution_manager                  exec_mng;
  task_executor*                          ru_timing_exec = nullptr;
  task_executor*                          ru_dl_exec;
  task_executor*                          ru_tx_exec;
  task_executor*                          ru_rx_exec;
  task_executor*                          test_du_sim_exec;
  task_executor*                          test_ru_sim_exec;
  std::unique_ptr<ru_ofh_executor_mapper> ofh_exec_mapper;
};
} // namespace

static void configure_ofh_sector(ofh::sector_configuration& sector_cfg)
{
  // Default IQ data scaling to be applied prior to downlink data compression.
  const float iq_scaling                = 0.9f;
  sector_cfg.max_processing_delay_slots = processing_delay_slots;
  sector_cfg.uses_dpdk                  = false;
  sector_cfg.are_metrics_enabled        = true;
  sector_cfg.sector_id                  = 0;

  std::chrono::duration<double, std::nano> symbol_duration(
      (1e6 / (get_nsymb_per_slot(cyclic_prefix::NORMAL) * get_nof_slots_per_subframe(test_params.scs))));

  sector_cfg.interface                       = "lo";
  sector_cfg.mac_src_address                 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  sector_cfg.mac_dst_address                 = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  sector_cfg.mtu_size                        = test_params.mtu;
  sector_cfg.vlan_cfg_cp                     = ether::vlan_parameters{.tci_vid = vlan_tag};
  sector_cfg.vlan_cfg_up                     = ether::vlan_parameters{.tci_vid = vlan_tag};
  sector_cfg.scs                             = test_params.scs;
  sector_cfg.bw                              = test_params.channel_bw_mhz;
  sector_cfg.ru_operating_bw                 = sector_cfg.bw;
  sector_cfg.cp                              = cyclic_prefix::NORMAL;
  sector_cfg.is_prach_control_plane_enabled  = test_params.is_prach_control_plane_enabled;
  sector_cfg.ignore_ecpri_payload_size_field = test_params.ignore_ecpri_payload_size_field;
  sector_cfg.tx_window_timing_params         = {
      T1a_max_cp_dl, T1a_min_cp_dl, T1a_max_cp_ul, T1a_min_cp_ul, T1a_max_up, T1a_min_up};
  sector_cfg.rx_window_timing_params = {Ta4_min, Ta4_max};

  // Configure compression
  sector_cfg.dl_compression_params                = test_params.data_compr_params;
  sector_cfg.ul_compression_params                = test_params.data_compr_params;
  sector_cfg.prach_compression_params             = test_params.prach_compr_params;
  sector_cfg.iq_scaling                           = iq_scaling;
  sector_cfg.is_downlink_static_compr_hdr_enabled = test_params.is_downlink_static_comp_hdr_enabled;
  sector_cfg.is_uplink_static_compr_hdr_enabled   = test_params.is_uplink_static_comp_hdr_enabled;

  // Configure eAxCs.
  sector_cfg.prach_eaxc.assign(test_params.prach_port_id.begin(), test_params.prach_port_id.end());
  sector_cfg.dl_eaxc.assign(test_params.dl_port_id.begin(), test_params.dl_port_id.end());
  sector_cfg.ul_eaxc.assign(test_params.ul_port_id.begin(), test_params.ul_port_id.end());
  sector_cfg.nof_antennas_ul = nof_antennas_ul;

  // Configure downlink beamforming, the configuration validation guarantees a valid antenna topology.
  if (test_params.beamforming_cfg.enable) {
    sector_cfg.dl_beamforming = transmitter_beamforming_config{
        .topology         = *test::get_dl_antenna_topology(nof_antennas_dl),
        .bfw_compr_params = test_params.beamforming_cfg.bfw_compr_params,
    };
  }
}

static ru_ofh_configuration generate_ru_config()
{
  ru_ofh_configuration ru_cfg;

  ru_cfg.gps_Alpha = 0;
  ru_cfg.gps_Beta  = 0;

  ofh::sector_configuration& sector_cfg = ru_cfg.sector_configs.emplace_back();
  configure_ofh_sector(sector_cfg);

  return ru_cfg;
}

static ru_ofh_dependencies generate_ru_dependencies(ocudulog::basic_logger&             logger,
                                                    worker_manager&                     workers,
                                                    ru_timing_notifier*                 timing_notifier,
                                                    ru_uplink_plane_rx_symbol_notifier* rx_symbol_notifier,
                                                    test_gateway*&                      tx_gateway,
                                                    test_ether_receiver*&               eth_receiver,
                                                    ether::ethernet_rx_buffer_pool&     buffer_pool,
                                                    ru_error_notifier&                  error_notifier)
{
  ru_ofh_dependencies dependencies{
      .logger             = &logger,
      .timing_notifier    = timing_notifier,
      .error_notifier     = &error_notifier,
      .rx_symbol_notifier = rx_symbol_notifier,
      .rt_timing_executor = workers.ru_timing_exec,
  };

  // Build the sector executor mapper that owns the per-eAxC serialization strands.
  ru_ofh_executor_mapper_config exec_mapper_cfg{
      .dl_eaxc_per_sector = {test_params.dl_port_id},
      .downlink_executor  = workers.ru_dl_exec,
      .uplink_executor    = workers.ru_rx_exec,
      .txrx_executors     = {workers.ru_tx_exec},
      .timing_executor    = workers.ru_timing_exec,
  };
  workers.ofh_exec_mapper = create_ofh_ru_executor_mapper(exec_mapper_cfg);

  // Configure Ethernet gateway.
  auto gateway = std::make_unique<test_gateway>();
  tx_gateway   = gateway.get();

  // Configure Ethernet receiver.
  auto dummy_receiver = std::make_unique<dummy_eth_receiver>(logger, buffer_pool);
  eth_receiver        = dummy_receiver.get();

  dependencies.sector_dependencies.emplace_back(ofh::sector_dependencies{
      .logger          = &logger,
      .exec_mapper     = workers.ofh_exec_mapper->get_sector_mapper(0),
      .eth_transmitter = std::move(gateway),
      .eth_receiver    = std::move(dummy_receiver),
  });

  return dependencies;
}

/// \brief Creates the downlink resource grid pool filled with random data.
///
/// Category A grids have one port per DL eAxC, and all their ports carry data.
/// Category B grids have one port per beam of the antenna topology. For each allocated grid this function selects a
/// beam pattern and fills the data only in those beam-ports of the grid.
static std::unique_ptr<resource_grid_pool>
create_dl_resource_grid_pool(std::shared_ptr<resource_grid_factory> rg_factory, unsigned nof_prb)
{
  std::uniform_real_distribution<float>       dist(-1.0, +1.0);
  std::vector<std::unique_ptr<resource_grid>> dl_resource_grids;

  unsigned nof_ports = nof_antennas_dl;
  unsigned nof_trx   = nof_antennas_dl;
  if (is_cat_b_enabled()) {
    // The configuration validation guarantees a valid antenna topology.
    antenna_topology topology = *test::get_dl_antenna_topology(nof_antennas_dl);
    nof_ports                 = get_total_nof_beams(topology);
    nof_trx                   = get_total_nof_ports(topology);
  }

  // Create resource grids according to TDD pattern.
  for (unsigned rg_id = 0, e = processing_delay_slots * tdd_pattern.nof_dl_slots; rg_id != e; rg_id++) {
    dl_resource_grids.push_back(rg_factory->create(nof_ports, MAX_NSYMB_PER_SLOT, nof_prb * NOF_SUBCARRIERS_PER_RB));
    resource_grid& grid = *dl_resource_grids.back();

    test::dl_beam_list beams;
    if (is_cat_b_enabled()) {
      beams = test::generate_beam_pattern(rgen, rg_id, nof_ports, nof_trx, nof_antennas_dl);
    } else {
      beams.resize(nof_ports);
      std::iota(beams.begin(), beams.end(), 0);
    }

    // Pre-generate random downlink data.
    for (unsigned sym = 0; sym != get_nsymb_per_slot(cyclic_prefix::NORMAL); ++sym) {
      for (unsigned port : beams) {
        std::vector<cf_t> test_data(nof_prb * NOF_SUBCARRIERS_PER_RB);
        std::generate(test_data.begin(), test_data.end(), [&]() { return cf_t{dist(rgen), dist(rgen)}; });
        grid.get_writer().put(port, sym, 0, test_data);
      }
    }

    // As a sanity check make sure the grid exposes exactly the beam pattern (because the du_emulator will rely on the
    // grid allocation later).
    const resource_grid_reader& reader = grid.get_reader();
    for (unsigned port = 0; port != nof_ports; ++port) {
      ocudu_assert(reader.is_empty(port) != (std::find(beams.begin(), beams.end(), port) != beams.end()),
                   "Allocated resource grid port {} does not match the beam pattern",
                   port);
    }
  }
  return create_generic_resource_grid_pool(std::move(dl_resource_grids));
}

/// \brief Creates a pool of empty downlink resource grids, used to flush the OFH transmitter at the end of the test.
///
/// The grids have the same dimensions as the ones created by \ref create_dl_resource_grid_pool.
static std::unique_ptr<resource_grid_pool>
create_empty_dl_resource_grid_pool(std::shared_ptr<resource_grid_factory> rg_factory, unsigned nof_prb)
{
  unsigned nof_ports = nof_antennas_dl;
  if (is_cat_b_enabled()) {
    // The configuration validation guarantees a valid antenna topology.
    nof_ports = get_total_nof_beams(*test::get_dl_antenna_topology(nof_antennas_dl));
  }

  std::vector<std::unique_ptr<resource_grid>> dl_resource_grids;
  for (unsigned rg_id = 0; rg_id != processing_delay_slots; ++rg_id) {
    dl_resource_grids.push_back(rg_factory->create(nof_ports, MAX_NSYMB_PER_SLOT, nof_prb * NOF_SUBCARRIERS_PER_RB));
  }
  return create_generic_resource_grid_pool(std::move(dl_resource_grids));
}

static std::unique_ptr<resource_grid_pool>
create_ul_resource_grid_pool(std::shared_ptr<resource_grid_factory> rg_factory, unsigned nof_prb)
{
  std::vector<std::unique_ptr<resource_grid>> ul_resource_grids;
  for (unsigned rg_id = 0, e = processing_delay_slots * tdd_pattern.nof_ul_slots; rg_id != e; rg_id++) {
    ul_resource_grids.push_back(
        rg_factory->create(nof_antennas_ul, MAX_NSYMB_PER_SLOT, nof_prb * NOF_SUBCARRIERS_PER_RB));
  }
  return create_generic_resource_grid_pool(std::move(ul_resource_grids));
}

/// Checks that the given counter matches its expected value, printing the mismatch otherwise.
static bool check_counter(std::string_view name, unsigned value, unsigned expected)
{
  if (value == expected) {
    return true;
  }
  fmt::println("Unexpected number of {}: {}, expected {}", name, value, expected);
  return false;
}

/// Checks that the given counter does not exceed its maximum expected value, printing the mismatch otherwise.
static bool check_counter_upper_bound(std::string_view name, unsigned value, unsigned max_expected)
{
  if (value <= max_expected) {
    return true;
  }
  fmt::println("Unexpected number of {}: {}, expected at most {}", name, value, max_expected);
  return false;
}

/// \brief Logs a mismatch between the given counter and its expected value.
///
/// Used for the counters that depend on the stalls of the test execution environment, which do not fail the test.
static void log_counter_if_mismatch(std::string_view name, unsigned value, unsigned expected)
{
  if (value != expected) {
    fmt::println("Note: number of {} is {}, expected {}", name, value, expected);
  }
}

/// \brief Prints the RU metrics accumulated during the whole test and checks them for errors.
///
/// \return true if no error was detected, false otherwise.
static bool check_ru_metrics(const ofh::metrics& metrics)
{
  fmt::println("Timing: nof_skipped_symbols={}, skipped_symbols_max_burst={}",
               metrics.timing.nof_skipped_symbols,
               metrics.timing.skipped_symbols_max_burst);

  bool success = true;
  for (const sector_metrics& sector : metrics.sectors) {
    const transmitter_dl_metrics&               dl      = sector.tx_metrics.dl_metrics;
    const transmitter_ul_metrics&               ul      = sector.tx_metrics.ul_metrics;
    const received_messages_metrics&            rx_msgs = sector.rx_metrics.rx_messages_metrics;
    const closed_rx_window_metrics&             rx_win  = sector.rx_metrics.closed_window_metrics;
    const message_decoding_performance_metrics& rx_dec  = sector.rx_metrics.rx_decoding_perf_metrics;

    fmt::println("Sector#{} TX: tx_bytes={}, late_dl_grids={}, late_ul_requests={}, late_cp_dl={}, late_up_dl={}, "
                 "late_cp_ul={}, dispatch_failures_cp_dl={}, dispatch_failures_up_dl={}, dispatch_failures_ul={}",
                 sector.sector_id,
                 sector.tx_metrics.eth_transmitter_metrics.total_nof_bytes,
                 dl.nof_late_dl_grids,
                 ul.nof_late_ul_requests,
                 dl.nof_late_cp_dl,
                 dl.nof_late_up_dl,
                 ul.nof_late_cp_ul,
                 dl.dl_cp_metrics.nof_dispatch_failures,
                 dl.dl_up_metrics.nof_dispatch_failures,
                 ul.ul_cp_metrics.nof_dispatch_failures);
    fmt::println("Sector#{} RX: rx_bytes={}, on_time={}, early={}, late={}, missing_ul_symbols={}, "
                 "missing_prach_contexts={}, dropped_data={}, dropped_prach={}, past_seq_id={}, future_seq_id={}",
                 sector.sector_id,
                 sector.rx_metrics.eth_receiver_metrics.total_nof_bytes,
                 rx_msgs.nof_on_time_messages,
                 rx_msgs.nof_early_messages,
                 rx_msgs.nof_late_messages,
                 rx_win.nof_missing_uplink_symbols,
                 rx_win.nof_missing_prach_contexts,
                 rx_dec.data_processing_metrics.nof_dropped_messages,
                 rx_dec.prach_processing_metrics.nof_dropped_messages,
                 rx_dec.ecpri_metrics.nof_past_seq_id_messages,
                 rx_dec.ecpri_metrics.nof_future_seq_id_messages);

    // The test does not request PRACH, so any PRACH activity is an error.
    success &= check_counter("missing PRACH contexts", rx_win.nof_missing_prach_contexts, 0);
    success &= check_counter("dropped PRACH messages", rx_dec.prach_processing_metrics.nof_dropped_messages, 0);

    // The DU transmits and receives some traffic.
    if (sector.tx_metrics.eth_transmitter_metrics.total_nof_bytes == 0) {
      fmt::println("Sector#{}: no bytes transmitted", sector.sector_id);
      success = false;
    }
    if (rx_msgs.nof_on_time_messages + rx_msgs.nof_early_messages + rx_msgs.nof_late_messages == 0) {
      fmt::println("Sector#{}: no messages received", sector.sector_id);
      success = false;
    }

    // Only late messages can be dropped by the receiver.
    success &= check_counter_upper_bound(
        "dropped UL data messages", rx_dec.data_processing_metrics.nof_dropped_messages, rx_msgs.nof_late_messages);
  }

  return success;
}

/// \brief Checks the number of messages of one type transmitted by the DU against the expected values.
///
/// Every eAxC is bounded by the number of messages expected in it. The total over all eAxCs, after discounting the
/// late messages, is only logged, as the late counter is not given per eAxC and depends on the test environment.
static bool check_eaxc_counters(std::string_view              name,
                                const eaxc_counters&          counters,
                                const eaxc_expected_counters& nof_expected,
                                unsigned                      nof_late)
{
  bool     success        = true;
  unsigned total          = 0;
  unsigned total_expected = 0;

  for (unsigned eaxc = 0; eaxc != MAX_SUPPORTED_EAXC_ID_VALUE; ++eaxc) {
    unsigned value = counters[eaxc].load(std::memory_order_relaxed);
    total += value;
    total_expected += nof_expected[eaxc];
    success &= check_counter_upper_bound(fmt::format("{} messages in eAxC={}", name, eaxc), value, nof_expected[eaxc]);
  }
  log_counter_if_mismatch(fmt::format("{} messages", name), total, total_expected - nof_late);

  return success;
}

/// \brief Checks that every configured eAxC received at least one message.
///
/// Complements \ref check_eaxc_counters, whose upper bounds would hide an eAxC without traffic (they would also pass if
/// the test never pushed a grid for eAxC).
static bool check_eaxc_coverage(std::string_view              name,
                                span<const unsigned>          eaxcs,
                                const eaxc_counters&          counters,
                                const eaxc_expected_counters& nof_expected)
{
  bool success = true;
  for (unsigned eaxc : eaxcs) {
    if (nof_expected[eaxc] == 0) {
      fmt::println("No {} messages expected in configured eAxC={}, the test does not exercise it", name, eaxc);
      success = false;
    }
    if (counters[eaxc].load(std::memory_order_relaxed) == 0) {
      fmt::println("No {} messages received in configured eAxC={}", name, eaxc);
      success = false;
    }
  }
  return success;
}

/// \brief Checks the number of messages exchanged during the test against the expected values.
///
/// The expected values are derived from the resource grids and uplink requests handed by the DU emulator to the RU, as
/// every transmitted beam-port carries data in all its symbols. Only the message content, eAxC coverage and upper
/// bounds of the counters contribute to the final result. The counters accounting for late, dropped or missing messages
/// are only logged.
/// \return true if all the checks pass, false otherwise.
static bool check_message_counters(const sector_metrics&           metrics,
                                   const test_gateway&             gateway,
                                   const test::dl_cplane_checker&  cplane_checker,
                                   const test_du_emulator&         du_emulator,
                                   const test_ru_emulator&         ru_emulator,
                                   const dummy_rx_symbol_notifier& rx_symbol_notifier,
                                   unsigned                        nof_prb)
{
  const transmitter_dl_metrics&    dl      = metrics.tx_metrics.dl_metrics;
  const transmitter_ul_metrics&    ul      = metrics.tx_metrics.ul_metrics;
  const received_messages_metrics& rx_msgs = metrics.rx_metrics.rx_messages_metrics;

  unsigned nof_symbols  = get_nsymb_per_slot(cyclic_prefix::NORMAL);
  unsigned nof_dl_slots = du_emulator.nof_dl_grids - du_emulator.nof_late_dl_grids;
  // A dispatch failure drops a whole UL request, like a late one.
  unsigned nof_ul_slots =
      du_emulator.nof_ul_requests - ul.nof_late_ul_requests - ul.ul_cp_metrics.nof_dispatch_failures;
  unsigned nof_dl_uplane_packets_per_symbol =
      test::calculate_nof_dl_uplane_messages_per_symbol(test_params.mtu,
                                                        nof_prb,
                                                        test_params.data_compr_params,
                                                        test_params.is_downlink_static_comp_hdr_enabled,
                                                        true,
                                                        ocudulog::fetch_basic_logger("OFH_TEST"));
  unsigned nof_ul_answered_slots = ru_emulator.nof_answered_slots;

  fmt::println("Messages: dl_slots={}, ul_slots={}, ul_slots_answered_by_ru={}, dl_uplane_packets_per_symbol={}, "
               "ul_uplane_packets_per_symbol={}, invalid_ul_symbols={}",
               nof_dl_slots,
               nof_ul_slots,
               nof_ul_answered_slots,
               nof_dl_uplane_packets_per_symbol,
               ru_emulator.get_nof_messages_per_symbol(),
               rx_symbol_notifier.nof_invalid_symbols);

  // The late grids detected by the DU emulator are the ones reported by the RU metrics.
  log_counter_if_mismatch("late DL grids", du_emulator.nof_late_dl_grids, dl.nof_late_dl_grids);

  // Every transmitted beam-port carries one C-Plane message and the U-Plane messages of every symbol in its DL eAxC.
  // Every UL slot carries one C-Plane message per UL eAxC.
  unsigned nof_uplane_per_eaxc_slot = nof_symbols * nof_dl_uplane_packets_per_symbol;

  eaxc_expected_counters nof_expected_dl_cp = {};
  eaxc_expected_counters nof_expected_dl_up = {};
  eaxc_expected_counters nof_expected_ul_cp = {};
  for (unsigned eaxc = 0; eaxc != MAX_SUPPORTED_EAXC_ID_VALUE; ++eaxc) {
    nof_expected_dl_cp[eaxc] = du_emulator.nof_expected_dl_cplane_messages[eaxc];
    nof_expected_dl_up[eaxc] = nof_expected_dl_cp[eaxc] * nof_uplane_per_eaxc_slot;
  }
  for (unsigned eaxc : test_params.ul_port_id) {
    nof_expected_ul_cp[eaxc] = nof_ul_slots;
  }
  // A DL dispatch failure drops the C-Plane message, or the U-Plane messages of all symbols, of one eAxC in one slot.
  bool success = check_eaxc_counters("DL C-Plane",
                                     gateway.dl_cplane_counters,
                                     nof_expected_dl_cp,
                                     dl.nof_late_cp_dl + dl.dl_cp_metrics.nof_dispatch_failures);
  success &= check_eaxc_counters("DL U-Plane",
                                 gateway.dl_uplane_counters,
                                 nof_expected_dl_up,
                                 dl.nof_late_up_dl + dl.dl_up_metrics.nof_dispatch_failures * nof_uplane_per_eaxc_slot);
  success &= check_eaxc_counters("UL C-Plane", gateway.ul_cplane_counters, nof_expected_ul_cp, ul.nof_late_cp_ul);

  // Verify that every configured eAxC carries traffic at least once during the test.
  success &= check_eaxc_coverage("DL C-Plane", test_params.dl_port_id, gateway.dl_cplane_counters, nof_expected_dl_cp);
  success &= check_eaxc_coverage("DL U-Plane", test_params.dl_port_id, gateway.dl_uplane_counters, nof_expected_dl_up);
  success &= check_eaxc_coverage("UL C-Plane", test_params.ul_port_id, gateway.ul_cplane_counters, nof_expected_ul_cp);

  // Every DL C-Plane message carries the section of the beam-port transmitted in its eAxC.
  success &= check_counter("DL C-Plane messages with errors", cplane_checker.get_nof_errors(), 0);
  success &= check_counter("malformed packets", nof_malformed_packets, 0);

  log_counter_if_mismatch("unverified DL C-Plane messages", cplane_checker.get_nof_unverified_messages(), 0);

  // Dropped late DL U-Plane messages leave gaps in the sequence identifiers.
  if (nof_missing_dl_packets > dl.nof_late_up_dl) {
    fmt::println(
        "Note: number of missing DL packets is {}, expected at most {}", nof_missing_dl_packets, dl.nof_late_up_dl);
  }

  // The RU emulator answers every received UL slot with the U-Plane messages of all its symbols and UL eAxCs.
  unsigned nof_ul_uplane_sent =
      nof_ul_answered_slots * nof_symbols * test_params.ul_port_id.size() * ru_emulator.get_nof_messages_per_symbol();
  success &= check_counter_upper_bound("UL slots answered by the RU emulator", nof_ul_answered_slots, nof_ul_slots);
  log_counter_if_mismatch("UL U-Plane messages sent", ru_emulator.nof_sent_messages, nof_ul_uplane_sent);

  // The OFH receiver may miss the last messages sent when the RU stops.
  log_counter_if_mismatch("UL U-Plane messages received",
                          rx_msgs.nof_on_time_messages + rx_msgs.nof_early_messages + rx_msgs.nof_late_messages,
                          nof_ul_uplane_sent);

  // Every symbol of every UL slot is notified to the upper PHY, as invalid if it was not received on time.
  log_counter_if_mismatch("notified UL symbols",
                          rx_symbol_notifier.nof_valid_symbols + rx_symbol_notifier.nof_invalid_symbols,
                          nof_ul_slots * nof_symbols);
  success &= check_counter("PRACH windows", rx_symbol_notifier.nof_prach_windows, 0);

  return success;
}

int main(int argc, char** argv)
{
  static constexpr unsigned            BUFFER_SIZE = 9600;
  std::unique_ptr<test_ether_receiver> eth_receiver_ptr;

  auto parsed_params = test::parse_test_configuration(argc, argv);
  if (!parsed_params.has_value()) {
    return parsed_params.error();
  }
  test_params     = std::move(*parsed_params);
  tdd_pattern     = (test_params.tdd_pattern_str == "6d3u") ? tdd_pattern_6d3u : tdd_pattern_7d2u;
  nof_antennas_dl = test_params.dl_port_id.size();
  nof_antennas_ul = test_params.ul_port_id.size();

  // Set up logging.
  const std::string& log_filename = test_params.logger_cfg.filename;
  ocudulog::sink*    log_sink =
      (log_filename == "stdout") ? ocudulog::create_stdout_sink() : ocudulog::create_file_sink(log_filename);
  if (log_sink == nullptr) {
    report_error("Could not create application main log sink.\n");
  }
  ocudulog::set_default_sink(*log_sink);
  ocudulog::init();

  ocudulog::basic_logger& logger = ocudulog::fetch_basic_logger("OFH_TEST", false);
  logger.set_level(test_params.logger_cfg.level);
  ocudulog::fetch_basic_logger("PHY").set_level(ocudulog::basic_levels::error);

  unsigned nof_prb = get_max_Nprb(test_params.channel_bw_mhz, test_params.scs, frequency_range::FR1);

  // Set up resources used by the DU emulator.
  std::shared_ptr<resource_grid_factory> rg_factory = create_resource_grid_factory();
  report_fatal_error_if_not(rg_factory, "Invalid factory");

  auto dl_rg_pool       = create_dl_resource_grid_pool(rg_factory, nof_prb);
  auto empty_dl_rg_pool = create_empty_dl_resource_grid_pool(rg_factory, nof_prb);
  auto ul_rg_pool       = create_ul_resource_grid_pool(rg_factory, nof_prb);

  test::dl_beam_registry         beam_registry;
  ether::ethernet_rx_buffer_pool buffer_pool(BUFFER_SIZE);
  worker_manager                 workers;
  dummy_rx_symbol_notifier       rx_symbol_notifier;
  dummy_timing_notifier          timing_notifier;
  test_gateway*                  tx_gateway;
  test_ether_receiver*           eth_receiver;
  dummy_ru_error_notifier        error_notifier;

  ru_ofh_configuration    ru_cfg = generate_ru_config();
  test::dl_cplane_checker cplane_checker(
      beam_registry, {nof_prb, test_params.dl_port_id, ru_cfg.sector_configs.front().dl_beamforming});
  ru_ofh_dependencies ru_deps = generate_ru_dependencies(
      logger, workers, &timing_notifier, &rx_symbol_notifier, tx_gateway, eth_receiver, buffer_pool, error_notifier);

  if (test_params.use_loopback_receiver) {
    ru_deps.sector_dependencies[0].eth_receiver.reset();
    eth_receiver_ptr = std::make_unique<lo_eth_receiver>(logger);
    eth_receiver     = eth_receiver_ptr.get();
  }
  std::unique_ptr<radio_unit> ru_object =
      test_params.is_non_realtime
          ? test::create_non_rt_ofh_ru(ru_cfg, std::move(ru_deps), test_params.non_rt_time_scale)
          : create_ofh_ru(ru_cfg, std::move(ru_deps));

  // Get RU downlink plane handler.
  auto& ru_dl_handler = ru_object->get_downlink_plane_handler();
  auto& ru_ul_handler = ru_object->get_uplink_plane_handler();

  // Create RU emulator instance.
  test_ru_emulator ru_emulator(
      logger, *workers.test_ru_sim_exec, *eth_receiver, test_params.data_compr_params, nof_prb);

  // Create DU emulator instance.
  test_du_emulator du_emulator(logger,
                               *workers.test_du_sim_exec,
                               *dl_rg_pool,
                               *empty_dl_rg_pool,
                               *ul_rg_pool,
                               ru_dl_handler,
                               ru_ul_handler,
                               beam_registry,
                               error_notifier);

  // Connect Ethernet gateway to the RU emulator.
  tx_gateway->connect_ru(&ru_emulator);
  tx_gateway->connect_dl_cplane_checker(cplane_checker);

  // Start the RU.
  fmt::print("Starting RU...\n");
  ru_object->get_controller().get_operation_controller().start();
  // Connect its TTI boundary notifications to the DU emulator.
  timing_notifier.connect_du(du_emulator);
  fmt::print("Running the test...\n");

  // Wait until test is finished.
  while (!du_emulator.is_test_finished()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
  fmt::print("DU emulator stopped\n");

  fmt::print("Stopping the RU...\n");
  ru_object->get_controller().get_operation_controller().stop();
  fmt::print("RU stopped successfully.\n");

  workers.stop();
  ocudulog::flush();

  // Collects the metrics once for the entire test run.
  ru_metrics metrics;
  ru_object->get_metrics_collector()->collect_metrics(metrics);
  const ofh::metrics& ofh_metrics = std::get<ofh::metrics>(metrics.metrics);
  bool                success     = check_ru_metrics(ofh_metrics);
  success &= check_message_counters(
      ofh_metrics.sectors.front(), *tx_gateway, cplane_checker, du_emulator, ru_emulator, rx_symbol_notifier, nof_prb);

  fmt::print("Test finished, nof_missing_dl_packets={}, nof_malformed_packets={}\n",
             nof_missing_dl_packets,
             nof_malformed_packets);
  fmt::println("Test {}", success ? "PASSED" : "FAILED");

  return success ? EXIT_SUCCESS : EXIT_FAILURE;
}
