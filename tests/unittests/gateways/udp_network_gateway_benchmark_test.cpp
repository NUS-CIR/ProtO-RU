// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "tests/benchmarks/gateways/udp_network_gateway_benchmark_helpers.h"
#include <gtest/gtest.h>

using namespace ocudu;

TEST(udp_benchmark_test, empty_collector_has_zero_counts_and_span)
{
  dummy_network_gateway_data_notifier_with_src_addr collector{0, 0};
  EXPECT_EQ(collector.get_n_pdus(), 0);
  EXPECT_EQ(collector.get_rx_bytes(), 0);
  EXPECT_EQ(collector.get_t_rx(), std::chrono::microseconds::zero());
}

TEST(udp_benchmark_test, single_callback_has_no_observation_span)
{
  dummy_network_gateway_data_notifier_with_src_addr collector{0, 1};
  collector.on_new_pdu(make_tx_byte_buffer(37), sockaddr_storage{});
  EXPECT_EQ(collector.get_n_pdus(), 1);
  EXPECT_EQ(collector.get_rx_bytes(), 37);
  EXPECT_EQ(collector.get_t_rx(), std::chrono::microseconds::zero());
}

TEST(udp_benchmark_test, counting_stops_at_expected_count)
{
  dummy_network_gateway_data_notifier_with_src_addr collector{0, 2};
  for (unsigned i = 0; i != 3; ++i) {
    collector.on_new_pdu(make_tx_byte_buffer(10), sockaddr_storage{});
  }
  EXPECT_EQ(collector.get_n_pdus(), 2);
  EXPECT_EQ(collector.get_rx_bytes(), 20);
}

TEST(udp_benchmark_test, collectors_account_for_callbacks_independently)
{
  dummy_network_gateway_data_notifier_with_src_addr first{0, 3};
  dummy_network_gateway_data_notifier_with_src_addr second{0, 2};
  const auto                                        start = std::chrono::steady_clock::now();
  first.on_new_pdu(make_tx_byte_buffer(10), sockaddr_storage{});
  second.on_new_pdu(make_tx_byte_buffer(30), sockaddr_storage{});
  first.on_new_pdu(make_tx_byte_buffer(20), sockaddr_storage{});
  first.on_new_pdu(make_tx_byte_buffer(40), sockaddr_storage{});
  second.on_new_pdu(make_tx_byte_buffer(50), sockaddr_storage{});
  const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start);
  EXPECT_EQ(first.get_n_pdus(), 3);
  EXPECT_EQ(first.get_rx_bytes(), 70);
  EXPECT_EQ(second.get_n_pdus(), 2);
  EXPECT_EQ(second.get_rx_bytes(), 80);
  EXPECT_GE(second.get_t_rx(), std::chrono::microseconds::zero());
  EXPECT_LE(second.get_t_rx(), elapsed);
}
