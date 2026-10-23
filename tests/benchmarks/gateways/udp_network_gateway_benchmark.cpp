// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "udp_network_gateway_benchmark_helpers.h"
#include "ocudu/gateways/udp_network_gateway_factory.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/executors/inline_task_executor.h"
#include "ocudu/support/executors/manual_task_worker.h"
#include "ocudu/support/io/io_broker_factory.h"
#include <arpa/inet.h>
#include <getopt.h>
#include <limits>
#include <queue>

using namespace ocudu;

struct bench_params {
  unsigned pdu_len          = 1400;
  uint64_t nof_pdus         = 100000;
  unsigned slow_inter_rx_us = 500;
  uint16_t rx_port          = 56701;
  uint16_t tx_port          = 56702;
  unsigned rx_mmsg          = 256;
};

static void usage(const char* prog, const bench_params& params)
{
  fmt::print("Usage: {} [-n <nof PDUs>] [-l <PDU len>] [-u <t us>]\n", prog);
  fmt::print("\t-l PDU len [Default {}]\n", params.pdu_len);
  fmt::print("\t-n Number of PDUs [Default {}]\n", params.nof_pdus);
  fmt::print("\t-u Notify large PDU inter arrival time longer than t microseconds [Default {}]\n",
             params.slow_inter_rx_us);
  fmt::print("\t-p TX port [Default {}]\n", params.tx_port);
  fmt::print("\t-P RX port [Default {}]\n", params.rx_port);
  fmt::print("\t-h Show this message\n");
  fmt::print("Tx rates measure submissions; Rx rates use total observations over the first-to-last span.\n"
             "Unavailable measurements print N/A. Interarrival is not latency; unobserved is not proven loss.\n");
}

static void parse_args(int argc, char** argv, bench_params& params)
{
  int opt = 0;
  while ((opt = getopt(argc, argv, "l:n:m:u:p:P:h")) != -1) {
    switch (opt) {
      case 'l':
        params.pdu_len = std::strtol(optarg, nullptr, 10);
        break;
      case 'n':
        params.nof_pdus = std::strtol(optarg, nullptr, 10);
        break;
      case 'm':
        params.rx_mmsg = std::strtol(optarg, nullptr, 10);
        break;
      case 'u':
        params.slow_inter_rx_us = std::strtol(optarg, nullptr, 10);
        break;
      case 'p':
        params.tx_port = std::strtol(optarg, nullptr, 10);
        break;
      case 'P':
        params.rx_port = std::strtol(optarg, nullptr, 10);
        break;
      case 'h':
      default:
        usage(argv[0], params);
        std::exit(0);
    }
  }
}

int main(int argc, char** argv)
{
  ocudulog::init();

  // init GW logger
  ocudulog::fetch_basic_logger("IO-EPOLL", true).set_level(ocudulog::basic_levels::warning);
  ocudulog::fetch_basic_logger("UDP-GW", true).set_level(ocudulog::basic_levels::warning);
  ocudulog::fetch_basic_logger("UDP-GW", true).set_hex_dump_max_size(100);

  bench_params params{};
  parse_args(argc, argv, params);
  if (params.nof_pdus == 0) {
    report_error("Number of PDUs must be greater than zero");
  }
  if (params.pdu_len == 0 || params.pdu_len > 9100) {
    report_error("PDU length must be in the range [1, 9100]");
  }
  if (params.rx_mmsg == 0) {
    report_error("RX max mmsg must be greater than zero");
  }
  if (params.nof_pdus > std::numeric_limits<uint64_t>::max() / (params.pdu_len * 8ULL)) {
    report_error("Benchmark workload overflows the bit counter");
  }

  udp_network_gateway_config gw1_cfg;
  gw1_cfg.bind_address      = "127.0.0.1";
  gw1_cfg.bind_port         = params.tx_port;
  gw1_cfg.non_blocking_mode = false;
  gw1_cfg.rx_max_mmsg       = params.rx_mmsg;

  udp_network_gateway_config gw2_cfg;
  gw2_cfg.bind_address      = "127.0.0.1";
  gw2_cfg.bind_port         = params.rx_port;
  gw2_cfg.non_blocking_mode = false;
  gw2_cfg.rx_max_mmsg       = params.rx_mmsg;

  dummy_network_gateway_data_notifier_with_src_addr gw1_dn{params.slow_inter_rx_us, params.nof_pdus};
  dummy_network_gateway_data_notifier_with_src_addr gw2_dn{params.slow_inter_rx_us, params.nof_pdus};
  std::unique_ptr<udp_network_gateway>              gw1;
  std::unique_ptr<udp_network_gateway>              gw2;

  inline_task_executor io_rx_executor;
  inline_task_executor io_tx_executor;

  gw1 = create_udp_network_gateway({gw1_cfg, gw1_dn, io_tx_executor, io_rx_executor});
  gw2 = create_udp_network_gateway({gw2_cfg, gw2_dn, io_tx_executor, io_rx_executor});

  if (!gw1->create_and_bind()) {
    report_error("Failed to bind UDP network gateway 1. bind_port={}", params.tx_port);
  }
  if (!gw2->create_and_bind()) {
    report_error("Failed to bind UDP network gateway 2. bind_port={}", params.rx_port);
  }

  std::unique_ptr<io_broker> epoll_broker;

  epoll_broker = create_io_broker(io_broker_type::epoll, io_broker_config{os_thread_realtime_priority::min() + 5});

  if (not gw1->subscribe_to(*epoll_broker)) {
    report_fatal_error("Failed to register UDP network gateway 1 at IO broker. socket_fd={}", gw1->get_socket_fd());
  }
  if (not gw2->subscribe_to(*epoll_broker)) {
    report_fatal_error("Failed to register UDP network gateway 2 at IO broker. socket_fd={}", gw2->get_socket_fd());
  }

  sockaddr_storage gw2_addr = to_sockaddr_storage(gw2_cfg.bind_address, gw2_cfg.bind_port);

  byte_buffer pdu = make_tx_byte_buffer(params.pdu_len);

  auto t_start = std::chrono::steady_clock::now();

  for (uint64_t n = 0; n < params.nof_pdus; n++) {
    gw1->handle_pdu(pdu.copy(), gw2_addr);
  }
  auto t_end    = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration<double, std::micro>(t_end - t_start);
  fmt::print("Tx done\n\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(750));
  gw1.reset();
  gw2.reset();

  double   tx_duration_us = duration.count();
  uint64_t rx_duration_us = gw2_dn.get_t_rx().count();

  fmt::print("Tx time: {} us\n", tx_duration_us);
  fmt::print("Rx time: {} us\n\n", rx_duration_us);

  uint64_t tx_bytes = params.pdu_len * params.nof_pdus;
  uint64_t rx_bytes = gw2_dn.get_rx_bytes();

  uint64_t tx_bits = tx_bytes * 8;
  uint64_t rx_bits = rx_bytes * 8;

  fmt::print("Tx Bytes: {} GB\n", tx_bytes * 1e-9);
  fmt::print("Rx Bytes: {} GB\n\n", rx_bytes * 1e-9);
  fmt::print("Tx bits: {} b\n", tx_bits);
  fmt::print("Rx bits: {} b\n\n", rx_bits);

  fmt::print(tx_duration_us > 0 ? "Tx data rate: {:.2f} Mbit/s\n" : "Tx data rate: N/A\n",
             tx_duration_us > 0 ? (long double)tx_bits / tx_duration_us : 0);
  fmt::print(rx_duration_us > 0 ? "Rx data rate: {:.2f} Mbit/s\n\n" : "Rx data rate: N/A\n\n",
             rx_duration_us > 0 ? (long double)rx_bits / rx_duration_us : 0);

  fmt::print(tx_duration_us > 0 ? "Tx PDU rate: {:.2f} PDU/s\n" : "Tx PDU rate: N/A\n",
             tx_duration_us > 0 ? params.nof_pdus / (tx_duration_us * 1e-6) : 0);
  fmt::print(rx_duration_us > 0 ? "Rx PDU rate: {:.2f} PDU/s\n\n" : "Rx PDU rate: N/A\n\n",
             rx_duration_us > 0 ? gw2_dn.get_n_pdus() / (rx_duration_us * 1e-6) : 0);

  fmt::print("Tx PDUs total: {:>7}\n", params.nof_pdus);
  fmt::print("Rx PDUs total: {:>7} ({:.2f}% unobserved)\n\n",
             gw2_dn.get_n_pdus(),
             (1 - ((double)gw2_dn.get_n_pdus() / params.nof_pdus)) * 100);

  if (gw2_dn.get_n_pdus() > 1) {
    fmt::print("PDU inter arrival time (min/avg/max) [us]: {}/{:.2f}/{}\n",
               gw2_dn.get_t_min().count(),
               (double)gw2_dn.get_t_sum().count() / (gw2_dn.get_n_pdus() - 1),
               gw2_dn.get_t_max().count());
  } else {
    fmt::print("PDU inter arrival time (min/avg/max) [us]: N/A\n");
  }
}
