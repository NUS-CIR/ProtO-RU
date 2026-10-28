// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "apps/helpers/metrics/metrics_config.h"
#include "apps/helpers/network/udp_appconfig.h"
#include "apps/helpers/xnu/xnu_appconfig.h"
#include "apps/units/o_cu_up/cu_up/cu_up_unit_pcap_config.h"
#include "cu_up_unit_logger_config.h"
#include "ocudu/ran/gnb_cu_up_id.h"
#include "ocudu/ran/gnb_id.h"
#include "ocudu/ran/qos/five_qi.h"

namespace ocudu {

/// Configuration to enable/disable metrics per layer.
struct cu_up_unit_metrics_layer_config {
  bool enable_e1ap         = false;
  bool enable_pdcp         = false;
  bool enable_nrup         = false;
  bool skip_cu_up_executor = true;

  /// Returns true if one or more layers are enabled, false otherwise.
  bool are_metrics_enabled() const { return enable_pdcp || enable_nrup || enable_e1ap; }
};

/// Metrics configuration.
struct cu_up_unit_metrics_config {
  /// CU-UP statistics report period in milliseconds.
  std::chrono::milliseconds       cu_up_report_period = std::chrono::milliseconds(1000U);
  app_helpers::metrics_config     common_metrics_cfg;
  cu_up_unit_metrics_layer_config layers_cfg;
};

struct cu_up_unit_trace_config {
  bool cu_up_enable = false;
};

struct cu_up_unit_ngu_socket_config {
  std::string bind_addr      = "127.0.0.1";
  std::string bind_interface = "auto";
  std::string ext_addr       = "auto";
  /// SST of the S-NSSAI the socket is dedicated to. Without it, the socket serves every S-NSSAI that has no dedicated
  /// socket.
  std::optional<uint8_t> sst;
  /// SD of the S-NSSAI the socket is dedicated to. Without it, the socket serves the S-NSSAI that has no SD.
  std::optional<uint32_t> sd;
  udp_appconfig           udp_config = {};
};

/// GPTU parameters.
struct cu_up_unit_ngu_gtpu_config {
  unsigned                  gtpu_queue_size          = 2046;
  unsigned                  gtpu_batch_size          = 256;
  unsigned                  gtpu_reordering_timer_ms = 0;
  int64_t                   global_rate_limiter_bps  = -1;
  std::chrono::milliseconds rate_limiter_period{100};
  std::chrono::milliseconds gtpu_teid_release_linger_time{100};
  bool                      ignore_ue_ambr = true;
};

struct cu_up_unit_ngu_config {
  bool                                      no_core = false;
  std::vector<cu_up_unit_ngu_socket_config> ngu_socket_cfg;
  cu_up_unit_ngu_gtpu_config                gtpu_cfg;
};

/// GTP-U parameters for Xn-U.
struct cu_up_unit_xnu_gtpu_config {
  unsigned                  gtpu_queue_size          = 2046;
  unsigned                  gtpu_batch_size          = 256;
  unsigned                  gtpu_reordering_timer_ms = 0;
  std::chrono::milliseconds rate_limiter_period{100};
  std::chrono::milliseconds gtpu_teid_release_linger_time{100};
};

struct cu_up_unit_xnu_config {
  xnu_sockets_appconfig      sockets_cfg;
  cu_up_unit_xnu_gtpu_config gtpu_cfg;
};

/// F1-U configuration at CU_UP side
struct cu_cp_unit_f1u_config {
  uint32_t queue_size = 8192; ///< Queue size for F1-U PDUs
  uint32_t batch_size = 256;  ///< Batch size for F1-U PDUs
  int32_t  t_notify   = 5;    ///< Maximum backoff time for discard notifications from CU_UP to DU (ms)
};

/// QoS configuration.
struct cu_up_unit_qos_config {
  five_qi_t             five_qi = uint_to_five_qi(9);
  std::string           mode    = "am";
  cu_cp_unit_f1u_config f1u_cu_up;
};

struct cu_up_unit_test_mode_config {
  bool                      enabled           = false;
  bool                      integrity_enabled = true;
  bool                      ciphering_enabled = true;
  uint16_t                  nea_algo          = 2;
  uint16_t                  nia_algo          = 2;
  uint64_t                  ue_ambr           = 40000000000; // 40 gbps
  std::chrono::milliseconds attach_detach_period{0}; // Period for attaching detaching tests. 0 means always attached.
  std::chrono::milliseconds reestablish_period{0};   // Period for reestablishment tests. 0 means no re-establishments.
  std::string               f1u_peer_address = "127.0.10.2";
  uint32_t                  nof_ues          = 1;
};

struct cu_up_unit_execution_config {
  uint32_t dl_ue_executor_queue_size   = 8192;
  uint32_t ul_ue_executor_queue_size   = 8192;
  uint32_t ctrl_ue_executor_queue_size = 8192;
  unsigned strand_batch_size           = 256;
};

/// CU-UP application unit configuration.
struct cu_up_unit_config {
  /// gNB identifier.
  gnb_id_t gnb_id = {411, 22};
  /// CU-UP identifier.
  gnb_cu_up_id_t gnb_cu_up_id = gnb_cu_up_id_t::min;
  /// CU-UP maximum number of bearer contexts allowed by the CU-UP.
  uint32_t max_nof_ues = 16384;
  /// CU-UP warn if PDUs are dropped.
  bool warn_on_drop = false;
  /// NG-U configuration.
  cu_up_unit_ngu_config ngu_cfg;
  /// Xn-U configuration.
  cu_up_unit_xnu_config xnu_cfg;
  /// Execution configuration
  cu_up_unit_execution_config exec_cfg;
  /// Metrics.
  cu_up_unit_metrics_config metrics;
  /// Loggers.
  cu_up_unit_logger_config loggers;
  /// Trace.
  cu_up_unit_trace_config trace_cfg;
  /// PCAPs.
  cu_up_unit_pcap_config pcap_cfg;
  /// QoS configuration.
  std::vector<cu_up_unit_qos_config> qos_cfg;
  /// Supported PLMN IDs (up to 12 PLMNs).
  std::vector<std::string> plmn_list;
  /// Test mode.
  cu_up_unit_test_mode_config test_mode_cfg;
};

} // namespace ocudu
