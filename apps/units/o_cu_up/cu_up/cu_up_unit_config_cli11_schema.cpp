// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_up_unit_config_cli11_schema.h"
#include "apps/helpers/logger/logger_appconfig_cli11_utils.h"
#include "apps/helpers/metrics/metrics_config_cli11_schema.h"
#include "apps/helpers/network/udp_cli11_schema.h"
#include "apps/helpers/xnu/xnu_cli11_schema.h"
#include "apps/units/o_cu_up/cu_up/cu_up_unit_config.h"
#include "apps/units/o_cu_up/cu_up/cu_up_unit_pcap_config.h"
#include "ocudu/ran/cu_up_constants.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/support/cli11_utils.h"
#include "ocudu/support/config_parsers.h"

using namespace ocudu;

static void configure_cli11_ngu_socket_args(CLI::App& app, cu_up_unit_ngu_socket_config& ngu_sock_params)
{
  add_option(app, "--bind_addr", ngu_sock_params.bind_addr, "Local IP address to bind for NG-U interface")
      ->check(CLI::ValidIPV4 | CLI::IsMember({"auto"}));
  add_option(app, "--bind_interface", ngu_sock_params.bind_interface, "Network device to bind for NG-U interface")
      ->capture_default_str();
  add_option(app,
             "--ext_addr",
             ngu_sock_params.ext_addr,
             "External IP address that is advertised to receive GTP-U packets from UPF via NG-U interface")
      ->check(CLI::ValidIPV4 | CLI::IsMember({"auto"}));
  add_option(
      app,
      "--sst",
      ngu_sock_params.sst,
      "Slice Service Type of the S-NSSAI the socket is dedicated to. Without it, the socket serves every S-NSSAI "
      "that has no dedicated socket")
      ->range(0, 255);
  add_option(app,
             "--sd",
             ngu_sock_params.sd,
             "Service Differentiator of the S-NSSAI the socket is dedicated to. Without it, the socket serves the "
             "S-NSSAI of the given SST that has no SD")
      ->range(0, 0xfffffe);

  configure_cli11_with_udp_config_schema(app, ngu_sock_params.udp_config);
}

static void configure_cli11_ngu_gtpu_args(CLI::App& app, cu_up_unit_ngu_gtpu_config& gtpu_cfg)
{
  add_option(app, "--queue_size", gtpu_cfg.gtpu_queue_size, "GTP-U queue size, in PDUs")->capture_default_str();
  add_option(app, "--batch_size", gtpu_cfg.gtpu_batch_size, "Maximum number of GTP-U PDUs processed in a batch")
      ->capture_default_str();
  add_option(app, "--global_rate_limiter", gtpu_cfg.global_rate_limiter_bps, "NG-U global rate limiter in bps")
      ->capture_default_str();
  add_option(
      app, "--reordering_timer", gtpu_cfg.gtpu_reordering_timer_ms, "GTP-U RX reordering timer (in milliseconds)")
      ->capture_default_str();
  add_option(
      app, "--rate_limiter_period", gtpu_cfg.rate_limiter_period, "GTP-U RX rate limiter period (in milliseconds)")
      ->capture_default_str();
  add_option(app,
             "--teid_release_linger_time",
             gtpu_cfg.gtpu_teid_release_linger_time,
             "Error indication suppression time for released TEIDs (in milliseconds)")
      ->capture_default_str();
  add_option(app, "--ignore_ue_ambr", gtpu_cfg.ignore_ue_ambr, "Ignore GTP-U DL UE-AMBR rate limiter")
      ->capture_default_str();
}

static void configure_cli11_execution_args(CLI::App& app, cu_up_unit_execution_config& exec_cfg)
{
  CLI::App* queues_subcmd = add_subcommand(app, "queues", "Task executor queue parameters")->configurable();
  add_option(*queues_subcmd,
             "--cu_up_dl_ue_executor_queue_size",
             exec_cfg.dl_ue_executor_queue_size,
             "CU-UP's DL UE executor queue size")
      ->capture_default_str();
  add_option(*queues_subcmd,
             "--cu_up_ul_ue_executor_queue_size",
             exec_cfg.ul_ue_executor_queue_size,
             "CU-UP's UL UE executor queue size")
      ->capture_default_str();
  add_option(*queues_subcmd,
             "--cu_up_ctrl_ue_executor_queue_size",
             exec_cfg.ctrl_ue_executor_queue_size,
             "CU-UP's CTRL UE executor queue size")
      ->capture_default_str();
  add_option(*queues_subcmd, "--cu_up_strand_batch_size", exec_cfg.strand_batch_size, "CU-UP's strands batch size")
      ->capture_default_str();
}

static void configure_cli11_ngu_args(CLI::App& app, cu_up_unit_ngu_config& ngu_params)
{
  add_option(app, "--no_core", ngu_params.no_core, "Allow gNB to run without a core");

  // Add GTP-U options
  CLI::App* gtpu_subcmd = add_subcommand(app, "gtpu", "CU-UP NG-U GTP-U parameters")->configurable();
  configure_cli11_ngu_gtpu_args(*gtpu_subcmd, ngu_params.gtpu_cfg);

  // Add option for multiple sockets, for usage with different slices, 5QIs or parallization.
  add_option_object_list<cu_up_unit_ngu_socket_config>(app,
                                                       "--socket",
                                                       ngu_params.ngu_socket_cfg,
                                                       configure_cli11_ngu_socket_args,
                                                       "Configures UDP/IP socket parameters of the NG-U interface");
}

static void configure_cli11_test_mode_args(CLI::App& app, cu_up_unit_test_mode_config& test_mode_params)
{
  add_option(app, "--enable", test_mode_params.enabled, "Enable or disable CU-UP test mode");
  add_option(app, "--integrity_enable", test_mode_params.integrity_enabled, "Enable or disable PDCP integrity testing");
  add_option(app, "--ciphering_enable", test_mode_params.ciphering_enabled, "Enable or disable PDCP ciphering testing");
  add_option(app, "--nea_algo", test_mode_params.nea_algo, "NEA algo to use for testing. Valid values {0, 1, 2, 3}.")
      ->capture_default_str()
      ->range(0, 3);
  add_option(app, "--nia_algo", test_mode_params.nea_algo, "NIA algo to use for testing. Valid values {1, 2, 3}.")
      ->capture_default_str()
      ->range(1, 3);
  add_option(app, "--ue_ambr", test_mode_params.ue_ambr, "DL UE-AMBR used for testing in bps");
  add_option(app,
             "--attach_detach_period",
             test_mode_params.attach_detach_period,
             "Attach/detach period for test mode. 0 means always attached")
      ->capture_default_str();
  add_option(app,
             "--reestablish_period",
             test_mode_params.reestablish_period,
             "Reestablish period for test mode. 0 means always attached")
      ->capture_default_str();
  add_option(app, "--f1u_peer_address", test_mode_params.f1u_peer_address, "Address for DL F1-U packets for test mode")
      ->capture_default_str();
  add_option(app, "--nof_ues", test_mode_params.nof_ues, "Number of UEs used for test mode")->capture_default_str();
}

static void configure_cli11_xnu_gtpu_args(CLI::App& app, cu_up_unit_xnu_gtpu_config& gtpu_cfg)
{
  add_option(app, "--queue_size", gtpu_cfg.gtpu_queue_size, "GTP-U queue size, in PDUs")->capture_default_str();
  add_option(app, "--batch_size", gtpu_cfg.gtpu_batch_size, "Maximum number of GTP-U PDUs processed in a batch")
      ->capture_default_str();
  add_option(
      app, "--reordering_timer", gtpu_cfg.gtpu_reordering_timer_ms, "GTP-U RX reordering timer (in milliseconds)")
      ->capture_default_str();
  add_option(
      app, "--rate_limiter_period", gtpu_cfg.rate_limiter_period, "GTP-U RX rate limiter period (in milliseconds)")
      ->capture_default_str();
  add_option(app,
             "--teid_release_linger_time",
             gtpu_cfg.gtpu_teid_release_linger_time,
             "Error indication suppression time for released TEIDs (in milliseconds)")
      ->capture_default_str();
}

static void configure_cli11_xnu_args(CLI::App& app, cu_up_unit_xnu_config& xnu_params)
{
  // Add GTP-U options.
  CLI::App* gtpu_subcmd = add_subcommand(app, "gtpu", "CU-UP Xn-U GTP-U parameters")->configurable();
  configure_cli11_xnu_gtpu_args(*gtpu_subcmd, xnu_params.gtpu_cfg);

  configure_cli11_xnu_sockets_args(app, xnu_params.sockets_cfg);
}

static void configure_cli11_cu_up_args(CLI::App& app, cu_up_unit_config& cu_up_params)
{
  // NG-U section.
  CLI::App* ngu_subcmd = add_subcommand(app, "ngu", "NG-U parameters")->configurable();
  configure_cli11_ngu_args(*ngu_subcmd, cu_up_params.ngu_cfg);

  // Xn-U section.
  CLI::App* xnu_subcmd = add_subcommand(app, "xnu", "Xn-U parameters")->configurable();
  configure_cli11_xnu_args(*xnu_subcmd, cu_up_params.xnu_cfg);

  // Test mode section.
  CLI::App* test_mode_subcmd = add_subcommand(app, "test_mode", "CU-UP test mode parameters")->configurable();
  configure_cli11_test_mode_args(*test_mode_subcmd, cu_up_params.test_mode_cfg);

  add_option(app,
             "--warn_on_drop",
             cu_up_params.warn_on_drop,
             "Log a warning for dropped packets in GTP-U, SDAP, PDCP and F1-U due to full queues")
      ->capture_default_str();

  add_option(app, "--max_nof_ues", cu_up_params.max_nof_ues, "Maximum number of Bearer Contexts allowed by the CU-UP")
      ->capture_default_str()
      ->range(static_cast<uint32_t>(1), MAX_NOF_CU_UP_UES);

  auto plmn_is_valid = [](const std::string& value) -> std::string {
    return plmn_identity::parse(value).has_value() ? "" : "Invalid PLMN format";
  };
  add_option(app, "--plmn_list", cu_up_params.plmn_list, "List of supported PLMN IDs (e.g. \"00101\")")
      ->capture_default_str()
      ->check(plmn_is_valid)
      ->pattern("^[0-9]{5,6}$")
      ->min_length(5)
      ->max_length(6);
}

static void configure_cli11_log_args(CLI::App& app, cu_up_unit_logger_config& log_params)
{
  app_helpers::add_log_option(app, log_params.pdcp_level, "--pdcp_level", "PDCP log level");
  app_helpers::add_log_option(app, log_params.sdap_level, "--sdap_level", "SDAP log level");
  app_helpers::add_log_option(app, log_params.gtpu_level, "--gtpu_level", "GTPU log level");
  app_helpers::add_log_option(app, log_params.e1ap_level, "--e1ap_level", "E1AP log level");
  app_helpers::add_log_option(app, log_params.f1u_level, "--f1u_level", "F1-U log level");
  app_helpers::add_log_option(app, log_params.cu_level, "--cu_level", "Log level for the CU");
  app_helpers::add_log_option(app, log_params.sec_level, "--sec_level", "Security functions log level");
  app_helpers::add_log_option(app, log_params.rohc_level, "--rohc_level", "ROHC log level");

  add_option(app,
             "--hex_max_size",
             log_params.hex_max_size,
             "Maximum number of bytes to print in hex (zero for no hex dumps, -1 for unlimited bytes)")
      ->capture_default_str()
      ->range(-1, 1024);
  add_option(app, "--e1ap_json_enabled", log_params.e1ap_json_enabled, "Enable JSON logging of E1AP PDUs")
      ->always_capture_default();
}

static void configure_cli11_pcap_args(CLI::App& app, cu_up_unit_pcap_config& pcap_params)
{
  // TODO: Rename prefix from n3_ to ngu_
  add_option(app, "--n3_filename", pcap_params.ngu.filename, "NG-U GTP-U PCAP file output path")->capture_default_str();
  add_option(app, "--n3_enable", pcap_params.ngu.enabled, "Enable NG-U GTP-U packet capture")->always_capture_default();
  add_option(app, "--f1u_filename", pcap_params.f1u.filename, "F1-U GTP-U PCAP file output path")
      ->capture_default_str();
  add_option(app, "--f1u_enable", pcap_params.f1u.enabled, "F1-U GTP-U PCAP")->always_capture_default();
  add_option(app, "--e1ap_filename", pcap_params.e1ap.filename, "E1AP PCAP file output path")->capture_default_str();
  add_option(app, "--e1ap_enable", pcap_params.e1ap.enabled, "E1AP PCAP")->always_capture_default();
}

static void configure_cli11_metrics_layers_args(CLI::App& app, cu_up_unit_metrics_layer_config& metrics_params)
{
  add_option(app, "--enable_e1ap", metrics_params.enable_e1ap, "Enable E1AP metrics")->capture_default_str();
  add_option(app, "--enable_pdcp", metrics_params.enable_pdcp, "Enable PDCP metrics")->capture_default_str();
  add_option(app, "--enable_nrup_cu", metrics_params.enable_nrup, "Enable NRUP metrics (CU side)")
      ->capture_default_str();
  add_option(app,
             "--skip_cu_up_executor",
             metrics_params.skip_cu_up_executor,
             "Whether to skip logging CU-UP executor metrics when executor logging is enabled application wide")
      ->capture_default_str();
}

static void configure_cli11_metrics_args(CLI::App& app, cu_up_unit_metrics_config& metrics_params)
{
  auto* periodicity_subcmd = add_subcommand(app, "periodicity", "Metrics periodicity configuration")->configurable();
  add_option(*periodicity_subcmd,
             "--cu_up_report_period",
             metrics_params.cu_up_report_period,
             "CU-UP metrics report period in milliseconds")
      ->capture_default_str();

  auto* layers_subcmd = add_subcommand(app, "layers", "Layer basis metrics configuration")->configurable();
  configure_cli11_metrics_layers_args(*layers_subcmd, metrics_params.layers_cfg);
}

static void configure_cli11_trace(CLI::App& app, cu_up_unit_trace_config& trace_params)
{
  CLI::App* layers_subcmd = add_subcommand(app, "layers", "Metrics configuration")->configurable();
  add_option(*layers_subcmd, "--cu_up_enable", trace_params.cu_up_enable, "Enable tracing for CU-UP executors")
      ->capture_default_str();
}

static void configure_cli11_f1u_cu_up_args(CLI::App& app, cu_cp_unit_f1u_config& f1u_cu_up_params)
{
  add_option(app, "--backoff_timer", f1u_cu_up_params.t_notify, "F1-U backoff timer (ms)")->capture_default_str();
  add_option(app, "--queue_size", f1u_cu_up_params.queue_size, "F1-U backoff timer (ms)")->capture_default_str();
  add_option(app, "--batch_size", f1u_cu_up_params.batch_size, "F1-U backoff timer (ms)")->capture_default_str();
}

static void configure_cli11_qos_args(CLI::App& app, cu_up_unit_qos_config& qos_params)
{
  add_option(app, "--five_qi", qos_params.five_qi, "5QI")->capture_default_str()->range(0, 255);

  CLI::App* f1u_cu_up_subcmd = add_subcommand(app, "f1u_cu_up", "F1-U parameters at CU_UP side");
  configure_cli11_f1u_cu_up_args(*f1u_cu_up_subcmd, qos_params.f1u_cu_up);
}

void ocudu::configure_cli11_with_cu_up_unit_config_schema(CLI::App& app, cu_up_unit_config& unit_cfg)
{
  add_option(app, "--gnb_id", unit_cfg.gnb_id.id, "gNodeB identifier")->capture_default_str();
  // Adding a default function to display correctly the uint8_t type.
  add_option(app, "--gnb_id_bit_length", unit_cfg.gnb_id.bit_length, "gNodeB identifier length in bits")
      ->default_function([&value = unit_cfg.gnb_id.bit_length]() { return std::to_string(value); })
      ->capture_default_str()
      ->range(22, 32);
  add_option(app, "--gnb_cu_up_id", unit_cfg.gnb_cu_up_id, "gNB-CU-UP Id")
      ->capture_default_str()
      ->range(static_cast<uint64_t>(0U), static_cast<uint64_t>((uint64_t(1) << 36) - 1));

  // CU-UP section.
  CLI::App* cu_up_subcmd = add_subcommand(app, "cu_up", "CU-UP parameters")->configurable();
  configure_cli11_cu_up_args(*cu_up_subcmd, unit_cfg);

  // Execution section.
  CLI::App* exec_subcmd = add_subcommand(app, "expert_execution", "Execution parameters")->configurable();
  configure_cli11_execution_args(*exec_subcmd, unit_cfg.exec_cfg);

  // Loggers section.
  CLI::App* log_subcmd = add_subcommand(app, "log", "Logging configuration")->configurable();
  configure_cli11_log_args(*log_subcmd, unit_cfg.loggers);

  // PCAP section.
  CLI::App* pcap_subcmd = add_subcommand(app, "pcap", "Logging configuration")->configurable();
  configure_cli11_pcap_args(*pcap_subcmd, unit_cfg.pcap_cfg);

  // Metrics section.
  CLI::App* metrics_subcmd = add_subcommand(app, "metrics", "Metrics configuration")->configurable();
  configure_cli11_metrics_args(*metrics_subcmd, unit_cfg.metrics);
  app_helpers::configure_cli11_with_metrics_appconfig_schema(app, unit_cfg.metrics.common_metrics_cfg);

  // Trace section.
  CLI::App* tracing_subcmd = add_subcommand(app, "trace", "General tracer configuration")->configurable();
  configure_cli11_trace(*tracing_subcmd, unit_cfg.trace_cfg);

  // QoS section.
  add_option_object_list<cu_up_unit_qos_config>(
      app,
      "--qos",
      unit_cfg.qos_cfg,
      configure_cli11_qos_args,
      "Configures RLC and PDCP radio bearers on a per 5QI basis.",
      // The gNB declares --qos in the DU, CU-CP and CU-UP alike, so every unit parses each element and must
      // tolerate the keys owned by the sibling units.
      nullptr,
      object_list_extras::tolerate_unknown);
}
