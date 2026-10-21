// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "support/compare_sequences.h"
#include "ocudu/hal/dpdk/dpdk_eal_factory.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ofh/ethernet/dpdk/dpdk_ethernet_port_context.h"
#include "ocudu/ofh/ethernet/dpdk/dpdk_ethernet_rx_buffer.h"
#include <algorithm>
#include <chrono>
#include <gtest/gtest.h>
#include <numeric>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace ocudu;
using namespace ether;

namespace {

/// \brief DPDK port context tests over net_memif ports, whose PMD does not support configuring the MTU size.
///
/// The server and client ports are connected through the same socket, so that the frames sent through the client port
/// are received by the server port.
class dpdk_ethernet_port_context_test : public ::testing::Test
{
protected:
  static void SetUpTestSuite()
  {
    // The EAL runs without hugepages nor PCI devices, with a file prefix and socket names unique to the process.
    eal = dpdk::create_dpdk_eal(
        fmt::format("dpdk_ethernet_port_context_test --no-huge -m 512 --no-pci --no-shconf --no-telemetry -l 0 "
                    "--file-prefix=ocudu_port_context_test_{2} "
                    "--vdev={0},role=server,socket=ocudu_port_context_test_{2} "
                    "--vdev={1},role=client,socket=ocudu_port_context_test_{2}",
                    server_id,
                    client_id,
                    ::getpid()),
        ocudulog::fetch_basic_logger("DPDK"));
    if (eal == nullptr) {
      return;
    }

    server   = create_port(server_id);
    client   = create_port(client_id);
    links_up = wait_link_up(*server) && wait_link_up(*client);
  }

  void SetUp() override
  {
    if (eal == nullptr) {
      GTEST_SKIP() << "The DPDK EAL with net_memif ports could not be initialized";
    }
    ASSERT_TRUE(links_up) << "the net_memif ports did not connect";
  }

  static void TearDownTestSuite()
  {
    client.reset();
    server.reset();
    eal.reset();
  }

  /// Creates the port context of the given net_memif port.
  static std::shared_ptr<dpdk_port_context> create_port(const std::string& id)
  {
    return dpdk_port_context::create(
        {.id = id, .is_promiscuous_mode_enabled = false, .is_link_status_check_enabled = false, .mtu_size = MTU_SIZE});
  }

  /// Waits until the link of the given port is up and returns true, or returns false after a timeout.
  static bool wait_link_up(const dpdk_port_context& port)
  {
    for (unsigned i = 0; i != MAX_NOF_WAIT_MS; ++i) {
      ::rte_eth_link link = {};
      if (::rte_eth_link_get_nowait(port.get_dpdk_port_id(), &link) == 0 && link.link_status == RTE_ETH_LINK_UP) {
        return true;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return false;
  }

  /// Returns the size of the mbuf data buffers of the ports.
  static unsigned get_buffer_size()
  {
    return ::rte_pktmbuf_data_room_size(client->get_mempool()) - RTE_PKTMBUF_HEADROOM;
  }

  /// \brief Creates a frame of the given size, as a chain of segments of up to the given size.
  ///
  /// Byte \c i of the frame is set to the least significant byte of \c i. Returns \c nullptr on failure.
  static ::rte_mbuf* create_frame(unsigned frame_size, unsigned segment_size)
  {
    ::rte_mbuf* frame = nullptr;
    for (unsigned offset = 0; offset != frame_size;) {
      ::rte_mbuf* segment = ::rte_pktmbuf_alloc(client->get_mempool());
      if (segment == nullptr) {
        ::rte_pktmbuf_free(frame);
        return nullptr;
      }
      unsigned nof_bytes = std::min(frame_size - offset, segment_size);
      auto*    data      = reinterpret_cast<uint8_t*>(::rte_pktmbuf_append(segment, nof_bytes));
      for (unsigned i = 0; i != nof_bytes; ++i) {
        data[i] = static_cast<uint8_t>(offset + i);
      }
      offset += nof_bytes;

      if (frame == nullptr) {
        frame = segment;
      } else if (::rte_pktmbuf_chain(frame, segment) != 0) {
        ::rte_pktmbuf_free(segment);
        ::rte_pktmbuf_free(frame);
        return nullptr;
      }
    }
    return frame;
  }

  /// Sends a frame of the given size through the client port, in segments of up to the buffer size.
  static bool send_frame(unsigned frame_size)
  {
    ::rte_mbuf* frame = create_frame(frame_size, get_buffer_size());
    if (frame == nullptr) {
      return false;
    }

    if (::rte_eth_tx_burst(client->get_dpdk_port_id(), 0, &frame, 1) != 1) {
      ::rte_pktmbuf_free(frame);
      return false;
    }
    return true;
  }

  /// Checks that the given frame data has the size and contents of a frame created by \c create_frame.
  static void check_frame_data(span<const uint8_t> data, unsigned frame_size)
  {
    ASSERT_EQ(data.size(), frame_size);
    std::vector<uint8_t> expected(frame_size);
    std::iota(expected.begin(), expected.end(), uint8_t{0});
    auto result = compare_sequences(data, span<const uint8_t>(expected));
    ASSERT_TRUE(result.has_value()) << result.error();
  }

  /// Receives a frame through the server port, or returns \c nullptr after a timeout.
  static ::rte_mbuf* receive_frame()
  {
    for (unsigned i = 0; i != MAX_NOF_WAIT_MS; ++i) {
      ::rte_mbuf* frame = nullptr;
      if (::rte_eth_rx_burst(server->get_dpdk_port_id(), 0, &frame, 1) == 1) {
        return frame;
      }
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return nullptr;
  }

  static constexpr units::bytes MTU_SIZE{9600};
  static constexpr unsigned     MAX_NOF_WAIT_MS = 2000;
  static constexpr const char*  server_id       = "net_memif0";
  static constexpr const char*  client_id       = "net_memif1";

  static std::unique_ptr<dpdk::dpdk_eal>    eal;
  static std::shared_ptr<dpdk_port_context> server;
  static std::shared_ptr<dpdk_port_context> client;
  static bool                               links_up;
};

std::unique_ptr<dpdk::dpdk_eal>    dpdk_ethernet_port_context_test::eal;
std::shared_ptr<dpdk_port_context> dpdk_ethernet_port_context_test::server;
std::shared_ptr<dpdk_port_context> dpdk_ethernet_port_context_test::client;
bool                               dpdk_ethernet_port_context_test::links_up = false;

} // namespace

TEST_F(dpdk_ethernet_port_context_test, port_without_mtu_configuration_carries_frames_of_the_configured_mtu)
{
  // Largest frame for the MTU: Ethernet header and VLAN tag, the CRC is not stored in the buffers. Depending on the
  // DPDK version, net_memif delivers it in a single segment or in one segment per shared memory buffer.
  static constexpr unsigned frame_size = MTU_SIZE.value() + RTE_ETHER_HDR_LEN + RTE_VLAN_HLEN;
  ASSERT_TRUE(send_frame(frame_size));
  ::rte_mbuf* frame = receive_frame();
  ASSERT_NE(frame, nullptr) << "the frame was not received";

  // The buffer is moved into a unique buffer, as the receiver does.
  unique_rx_buffer buffer(dpdk_rx_buffer_impl{frame});
  check_frame_data(buffer.data(), frame_size);
}

TEST_F(dpdk_ethernet_port_context_test, chained_frame_is_gathered_in_a_single_segment)
{
  // Frame in segments of 2048 bytes, as net_memif delivers it with its default shared memory buffer size.
  static constexpr unsigned nof_segments = 4;
  static constexpr unsigned segment_size = 2048;
  static constexpr unsigned frame_size   = nof_segments * segment_size;
  ::rte_mbuf*               frame        = create_frame(frame_size, segment_size);
  ASSERT_NE(frame, nullptr) << "the frame could not be created";
  ASSERT_EQ(frame->nb_segs, nof_segments);

  unique_rx_buffer buffer(dpdk_rx_buffer_impl{frame});
  check_frame_data(buffer.data(), frame_size);
}

TEST_F(dpdk_ethernet_port_context_test, frame_larger_than_a_buffer_is_discarded)
{
  // A port without MTU configuration does not drop a frame larger than a buffer, it receives it in several segments
  // that cannot be gathered in a single buffer.
  const unsigned frame_size = get_buffer_size() + 1000;
  ASSERT_TRUE(send_frame(frame_size));
  ::rte_mbuf* frame = receive_frame();
  ASSERT_NE(frame, nullptr) << "the frame was not received";
  ASSERT_EQ(frame->pkt_len, frame_size);
  ASSERT_GT(frame->nb_segs, 1);

  dpdk_rx_buffer_impl buffer(frame);
  ASSERT_TRUE(buffer.data().empty());
}
