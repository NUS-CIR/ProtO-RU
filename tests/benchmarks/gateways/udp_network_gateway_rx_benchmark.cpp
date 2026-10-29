// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

/// To run this benchmark, you will need a separate traffic source.
/// As an example, Iperf2 can be used, like so:
/// iperf -c 127.0.0.1 -u -b 5G -p 56701  -P 2 -t 3600 --no-udp-fin

#include "udp_network_gateway_benchmark_helpers.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/executors/inline_task_executor.h"
#include "ocudu/support/executors/manual_task_worker.h"
#include "ocudu/support/io/io_broker_factory.h"
#include <arpa/inet.h>
#include <getopt.h>

using namespace ocudu;

struct bench_params {
  uint64_t    nof_pdus         = 100000;
  unsigned    slow_inter_rx_us = 500;
  unsigned    rx_mmsg          = 256;
  std::string bind_addr        = "127.0.0.1";
};

static void usage(const char* prog, const bench_params& params)
{
  fmt::print("Usage: {} [-b <bind addr>] [-n <nof PDUs>] [-m <rx mmsg>] [-u <t us>]\n", prog);
  fmt::print("\t-b Bind address for socket [Default {}]\n", params.bind_addr);
  fmt::print("\t-n Number of PDUs [Default {}]\n", params.nof_pdus);
  fmt::print("\t-u Notify large PDU inter arrival time longer than t microseconds [Default {}]\n",
             params.slow_inter_rx_us);
  fmt::print("\t-m Number of RX PDUs in a syscall (rx_mmsg) [Default {}]\n", params.rx_mmsg);
  fmt::print("\t-h Show this message\n");
}

static void parse_args(int argc, char** argv, bench_params& params)
{
  int opt = 0;
  while ((opt = getopt(argc, argv, "b:n:m:u:h")) != -1) {
    switch (opt) {
      case 'b':
        params.bind_addr = std::string(optarg);
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

  fmt::print("To run this benchmark, you will need a separate traffic source.\n"
             "As an example, Iperf2 can be used, like so:\n"
             "iperf -c 127.0.0.1 -u -b 5G -p 56701  -P 2 -t 3600 --no-udp-fin\n");

  // init GW logger
  ocudulog::fetch_basic_logger("IO-EPOLL", true).set_level(ocudulog::basic_levels::warning);
  ocudulog::fetch_basic_logger("UDP-GW", true).set_level(ocudulog::basic_levels::warning);
  ocudulog::fetch_basic_logger("UDP-GW", true).set_hex_dump_max_size(100);

  bench_params params{};
  parse_args(argc, argv, params);
  if (params.nof_pdus == 0) {
    report_error("Number of PDUs must be greater than zero");
  }
  if (params.rx_mmsg == 0) {
    report_error("RX max mmsg must be greater than zero");
  }

  udp_network_gateway_config gw_cfg;
  gw_cfg.bind_address      = params.bind_addr;
  gw_cfg.bind_port         = 56701;
  gw_cfg.non_blocking_mode = false;
  gw_cfg.rx_max_mmsg       = params.rx_mmsg;

  dummy_network_gateway_data_notifier_with_src_addr gw_dn{params.slow_inter_rx_us, params.nof_pdus};
  std::unique_ptr<udp_network_gateway>              gw;

  inline_task_executor io_rx_executor;
  manual_task_worker   io_tx_executor{128};

  gw = create_udp_network_gateway({gw_cfg, gw_dn, io_tx_executor, io_rx_executor});
  if (!gw->create_and_bind()) {
    report_error("Failed to bind UDP network gateway. bind_port={}", gw_cfg.bind_port);
  }

  std::unique_ptr<io_broker> epoll_broker;

  epoll_broker = create_io_broker(io_broker_type::epoll, io_broker_config{os_thread_realtime_priority::min() + 5});
  if (not gw->subscribe_to(*epoll_broker)) {
    report_fatal_error("Failed to register UDP network gateway at IO broker.");
  }

  // Wait for all packets to be received
  auto t_start = std::chrono::steady_clock::now();
  while (gw_dn.get_n_pdus() < params.nof_pdus) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }

  auto t_end    = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t_end - t_start);
  fmt::print("Rx done\n\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(750));

  gw.reset();

  uint64_t rx_duration_us = gw_dn.get_t_rx().count();
  uint64_t rx_bytes       = gw_dn.get_rx_bytes();
  uint64_t rx_bits        = rx_bytes * 8;
  uint64_t nof_rx_pdus    = gw_dn.get_n_pdus();

  fmt::print("Test duration: {} us\n\n", duration.count());
  fmt::print("Rx time: {} us\n\n", rx_duration_us);

  fmt::print("Rx Bytes: {} GB\n", rx_bytes * 1e-9);
  fmt::print("Rx bits: {} b\n\n", rx_bits);

  fmt::print(rx_duration_us > 0 ? "Rx data rate: {:.2f} Mbit/s\n" : "Rx data rate: N/A\n",
             rx_duration_us > 0 ? (long double)rx_bits / rx_duration_us : 0);
  fmt::print(rx_duration_us > 0 ? "Rx PDU rate: {:.2f} PDU/s\n\n" : "Rx PDU rate: N/A\n\n",
             rx_duration_us > 0 ? nof_rx_pdus / (rx_duration_us * 1e-6) : 0);

  if (nof_rx_pdus > 1) {
    fmt::print("PDU inter arrival time (min/avg/max) [us]: {}/{:.2f}/{}\n",
               gw_dn.get_t_min().count(),
               (double)gw_dn.get_t_sum().count() / (nof_rx_pdus - 1),
               gw_dn.get_t_max().count());
  } else {
    fmt::print("PDU inter arrival time (min/avg/max) [us]: N/A\n");
  }
}
