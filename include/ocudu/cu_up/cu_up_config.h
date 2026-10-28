// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/cu_up/cu_up_e1_setup_notifier.h"
#include "ocudu/cu_up/cu_up_executor_mapper.h"
#include "ocudu/cu_up/cu_up_types.h"
#include "ocudu/e1ap/cu_up/e1ap_configuration.h"
#include "ocudu/e1ap/gateways/e1_connection_client.h"
#include "ocudu/f1u/cu_up/f1u_gateway.h"
#include "ocudu/gtpu/gtpu_config.h"
#include "ocudu/gtpu/gtpu_gateway.h"
#include "ocudu/gtpu/gtpu_teid_pool.h"
#include "ocudu/pcap/dlt_pcap.h"
#include "ocudu/ran/gnb_cu_up_id.h"
#include "ocudu/ran/s_nssai.h"
#include "ocudu/support/timers.h"
#include <map>
#include <optional>
#include <vector>

namespace ocudu {

class io_broker;

namespace ocuup {

struct network_interface_config {
  /// Local IP address to bind for connection from DU to receive uplink user-plane traffic.
  std::string f1u_bind_addr = "127.0.2.1";

  /// Local port to bind for connection from DU to receive uplink user-plane traffic.
  int f1u_bind_port = GTPU_PORT;
};

struct ngu_interface_config {
  /// TS 29.281 Sec. 4.4.2.3 Encapsulated T-PDUs.
  int upf_port = GTPU_PORT;
  /// NG-U global rate limiter.
  std::optional<uint64_t> global_rate_limiter_bps;
  /// NG-U reordering timer.
  std::chrono::milliseconds gtpu_reordering_timer;
  /// NG-U token bucket rate limiting period for UE-AMBR.
  std::chrono::milliseconds gtpu_rate_limiting_period;
  /// Error indication suppression time for released TEIDs.
  std::chrono::milliseconds gtpu_teid_release_linger_time;
  /// Ignore DL UE-AMBR.
  bool gtpu_ignore_ue_ambr;
  /// GTP-U queue size in PDUs.
  uint32_t gtpu_queue_size;
  /// Maximum number of GTP-U PDUs processed in a batch.
  uint32_t gtpu_batch_size;
  /// Warn whenever a PDU is dropped.
  bool warn_on_drop;
  /// \brief S-NSSAI each NG-U gateway is dedicated to, in the order of \c cu_up_dependencies::ngu_gws.
  ///
  /// A gateway dedicated to an S-NSSAI serves only the PDU sessions of that S-NSSAI. A gateway without an entry, or
  /// with an empty one, is a default gateway and serves the PDU sessions of every S-NSSAI that has no dedicated
  /// gateway. Holds at most one entry per NG-U gateway.
  std::vector<std::optional<s_nssai_t>> gw_snssais;
};

struct xnu_interface_config {
  /// Xn-U reordering timer.
  std::chrono::milliseconds gtpu_reordering_timer;
  /// Xn-U token bucket rate limiting period.
  std::chrono::milliseconds gtpu_rate_limiting_period;
  /// Error indication suppression time for released TEIDs.
  std::chrono::milliseconds gtpu_teid_release_linger_time;
  /// GTP-U queue size in PDUs.
  uint32_t gtpu_queue_size;
  /// Maximum number of GTP-U PDUs processed in a batch.
  uint32_t gtpu_batch_size;
  /// Warn whenever a PDU is dropped.
  bool warn_on_drop;
};

/// Holds the CU-UP test mode configuration.
struct cu_up_test_mode_config {
  bool                      enabled           = false;
  bool                      integrity_enabled = true;
  bool                      ciphering_enabled = true;
  uint16_t                  nea_algo          = 2;
  uint16_t                  nia_algo          = 2;
  uint64_t                  ue_ambr           = 40000000000;
  std::chrono::milliseconds attach_detach_period{0};
  std::chrono::milliseconds reestablish_period{0};
  std::string               f1u_peer_address = "127.0.10.2";
  uint32_t                  nof_ues          = 1;
};

/// CU-UP configuration.
struct cu_up_config {
  /// 5QI as key.
  std::map<five_qi_t, cu_up_qos_config> qos;
  /// NG-U configuration.
  ngu_interface_config ngu_cfg;
  /// Xn-U configuration.
  xnu_interface_config xnu_cfg;
  /// Test mode configuration.
  cu_up_test_mode_config test_mode_cfg;
  /// gNodeB identifier.
  gnb_id_t gnb_id = {411, 22};
  /// CU-UP identifier.
  gnb_cu_up_id_t cu_up_id = gnb_cu_up_id_t::min;
  /// CU-UP maximum number of allowed Bearer Contexts.
  uint32_t max_nof_ues = 16384;
  /// CU-UP name.
  std::string cu_up_name = "ocuup_01";
  /// Whether to enable JSON logging of E1AP Tx and Rx messages.
  bool e1ap_json_log_enabled = false;
  /// E1AP metrics period.
  timer_duration e1ap_metrics_period{0};
  /// List of supported PLMNs (1..12). Must be populated before use.
  std::vector<std::string> plmns;
  /// CU-UP statistics report period in seconds.
  std::chrono::seconds statistics_report_period;
};

/// CU-UP dependencies.
struct cu_up_dependencies {
  /// Executor mapper.
  cu_up_executor_mapper& exec_mapper;
  /// F1-U TEID pool.
  gtpu_teid_pool& f1u_teid_allocator;
  /// F1-U gateway.
  f1u_cu_up_gateway& f1u_gateway;
  /// Time manager.
  timer_manager& timers;
  /// PCAP.
  dlt_pcap& gtpu_pcap;
  /// Logger.
  ocudulog::basic_logger& logger;
  /// PDCP metrics notifier.
  pdcp_metrics_notifier* pdcp_metric_notifier = nullptr;
  /// E1AP connection client.
  std::vector<e1_connection_client*> e1_conn_clients;
  /// NG-U gateways
  std::vector<std::unique_ptr<gtpu_gateway>> ngu_gws;
  /// Xn-U gateways.
  std::vector<std::unique_ptr<gtpu_gateway>> xnu_gws;
  /// Optional notifier invoked after every successful E1 Setup, including the ones that follow a reconnection.
  std::unique_ptr<cu_up_e1_setup_complete_notifier> e1_setup_notifier = nullptr;
};

} // namespace ocuup
} // namespace ocudu

namespace fmt {

// Network config formatter
template <>
struct formatter<ocudu::ocuup::network_interface_config> {
  template <typename ParseContext>
  auto parse(ParseContext& ctx)
  {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const ocudu::ocuup::network_interface_config& cfg, FormatContext& ctx) const
  {
    return format_to(ctx.out(), "f1u_bind_addr={} f1u_bind_port={}", cfg.f1u_bind_addr, cfg.f1u_bind_port);
  }
};
} // namespace fmt
