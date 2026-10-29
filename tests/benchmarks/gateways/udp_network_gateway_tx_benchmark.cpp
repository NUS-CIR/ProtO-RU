// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "udp_network_gateway_benchmark_helpers.h"
#include "ocudu/gateways/udp_network_gateway_factory.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/executors/inline_task_executor.h"
#include "ocudu/support/executors/manual_task_worker.h"
#include <arpa/inet.h>
#include <getopt.h>
#include <limits>

using namespace ocudu;

struct bench_params {
  unsigned pdu_len  = 1400;
  uint64_t nof_pdus = 100000;
};

static void usage(const char* prog, const bench_params& params)
{
  fmt::print("Usage: {} [-n <nof PDUs>] [-l <PDU len>]\n", prog);
  fmt::print("\t-l PDU len [Default {}]\n", params.pdu_len);
  fmt::print("\t-n Number of PDUs [Default {}]\n", params.nof_pdus);
  fmt::print("\t-h Show this message\n");
}

static void parse_args(int argc, char** argv, bench_params& params)
{
  int opt = 0;
  while ((opt = getopt(argc, argv, "l:n:h")) != -1) {
    switch (opt) {
      case 'l':
        params.pdu_len = std::strtol(optarg, nullptr, 10);
        break;
      case 'n':
        params.nof_pdus = std::strtol(optarg, nullptr, 10);
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
  if (params.nof_pdus > std::numeric_limits<uint64_t>::max() / (params.pdu_len * 8ULL)) {
    report_error("Benchmark workload overflows the bit counter");
  }

  udp_network_gateway_config gw1_cfg;
  gw1_cfg.bind_address      = "127.0.0.1";
  gw1_cfg.bind_port         = 56701;
  gw1_cfg.non_blocking_mode = false;
  gw1_cfg.rx_max_mmsg       = 256;

  dummy_network_gateway_data_notifier_with_src_addr gw1_dn{0, params.nof_pdus}; // no rx required
  std::unique_ptr<udp_network_gateway>              gw1;
  std::unique_ptr<udp_network_gateway>              gw2;

  inline_task_executor io_rx_executor;
  inline_task_executor io_tx_executor;

  gw1 = create_udp_network_gateway({gw1_cfg, gw1_dn, io_tx_executor, io_rx_executor});

  if (!gw1->create_and_bind()) {
    report_error("Failed to bind UDP network gateway. bind_port={}", gw1_cfg.bind_port);
  }

  sockaddr_storage gw2_addr = to_sockaddr_storage("127.0.0.1", 56702);

  byte_buffer pdu = make_tx_byte_buffer(params.pdu_len);

  auto t_start = std::chrono::steady_clock::now();

  for (uint64_t n = 0; n < params.nof_pdus; n++) {
    gw1->handle_pdu(pdu.copy(), gw2_addr);
  }
  auto t_end    = std::chrono::steady_clock::now();
  auto duration = std::chrono::duration<double, std::micro>(t_end - t_start);
  fmt::print("Tx done\n\n");

  std::this_thread::sleep_for(std::chrono::milliseconds(750));

  double   tx_duration_us = duration.count();
  uint64_t tx_bytes       = params.pdu_len * params.nof_pdus;
  uint64_t tx_bits        = tx_bytes * 8;

  fmt::print("Tx time: {} us\n", tx_duration_us);
  fmt::print("Tx PDUs total: {:>7}\n", params.nof_pdus);
  fmt::print("Tx Bytes: {:.3f} GB\n\n", tx_bytes * 1e-9);

  fmt::print(tx_duration_us > 0 ? "Tx data rate: {:.2f} Mbit/s\n" : "Tx data rate: N/A\n",
             tx_duration_us > 0 ? (long double)tx_bits / tx_duration_us : 0);
  fmt::print(tx_duration_us > 0 ? "Tx PDU rate: {:.2f} PDU/s\n" : "Tx PDU rate: N/A\n",
             tx_duration_us > 0 ? params.nof_pdus / (tx_duration_us * 1e-6) : 0);
}
