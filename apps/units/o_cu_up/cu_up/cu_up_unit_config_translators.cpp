// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_up_unit_config_translators.h"
#include "apps/services/worker_manager/worker_manager_config.h"
#include "cu_up_unit_config.h"
#include "ocudu/cu_up/cu_up_configuration_helpers.h"

using namespace ocudu;

/// Returns the S-NSSAI each NG-U gateway is dedicated to, in the order the gateways are created.
static std::vector<std::optional<s_nssai_t>> generate_ngu_gw_snssais(const cu_up_unit_ngu_config& config)
{
  // Without a core, the CU-UP runs a single gateway that serves every S-NSSAI.
  if (config.no_core) {
    return {};
  }

  std::vector<std::optional<s_nssai_t>> gw_snssais;
  gw_snssais.reserve(config.ngu_socket_cfg.size());
  for (const cu_up_unit_ngu_socket_config& sock : config.ngu_socket_cfg) {
    if (not sock.sst.has_value()) {
      gw_snssais.emplace_back(std::nullopt);
      continue;
    }
    s_nssai_t snssai{slice_service_type{*sock.sst}};
    if (sock.sd.has_value()) {
      snssai.sd = slice_differentiator::create(*sock.sd).value();
    }
    gw_snssais.emplace_back(snssai);
  }
  return gw_snssais;
}

ocuup::cu_up_config ocudu::generate_cu_up_config(const cu_up_unit_config& config)
{
  // JSON metrics are not supported at E1AP for now, so only enable if log metrics are enabled.
  return {
      .qos = {},
      .ngu_cfg =
          ocuup::ngu_interface_config{
              .upf_port                  = GTPU_PORT,
              .global_rate_limiter_bps   = config.ngu_cfg.gtpu_cfg.global_rate_limiter_bps <= 0
                                               ? std::nullopt
                                               : std::optional<uint64_t>(config.ngu_cfg.gtpu_cfg.global_rate_limiter_bps),
              .gtpu_reordering_timer     = std::chrono::milliseconds{config.ngu_cfg.gtpu_cfg.gtpu_reordering_timer_ms},
              .gtpu_rate_limiting_period = config.ngu_cfg.gtpu_cfg.rate_limiter_period,
              .gtpu_teid_release_linger_time = config.ngu_cfg.gtpu_cfg.gtpu_teid_release_linger_time,
              .gtpu_ignore_ue_ambr           = config.ngu_cfg.gtpu_cfg.ignore_ue_ambr,
              .gtpu_queue_size               = config.ngu_cfg.gtpu_cfg.gtpu_queue_size,
              .gtpu_batch_size               = config.ngu_cfg.gtpu_cfg.gtpu_batch_size,
              .warn_on_drop                  = config.warn_on_drop,
              .gw_snssais                    = generate_ngu_gw_snssais(config.ngu_cfg)},
      .xnu_cfg =
          ocuup::xnu_interface_config{
              .gtpu_reordering_timer     = std::chrono::milliseconds{config.xnu_cfg.gtpu_cfg.gtpu_reordering_timer_ms},
              .gtpu_rate_limiting_period = config.xnu_cfg.gtpu_cfg.rate_limiter_period,
              .gtpu_teid_release_linger_time = config.xnu_cfg.gtpu_cfg.gtpu_teid_release_linger_time,
              .gtpu_queue_size               = config.xnu_cfg.gtpu_cfg.gtpu_queue_size,
              .gtpu_batch_size               = config.xnu_cfg.gtpu_cfg.gtpu_batch_size,
              .warn_on_drop                  = config.warn_on_drop},
      .test_mode_cfg         = ocuup::cu_up_test_mode_config{.enabled              = config.test_mode_cfg.enabled,
                                                             .integrity_enabled    = config.test_mode_cfg.integrity_enabled,
                                                             .ciphering_enabled    = config.test_mode_cfg.ciphering_enabled,
                                                             .nea_algo             = config.test_mode_cfg.nea_algo,
                                                             .nia_algo             = config.test_mode_cfg.nia_algo,
                                                             .ue_ambr              = config.test_mode_cfg.ue_ambr,
                                                             .attach_detach_period = config.test_mode_cfg.attach_detach_period,
                                                             .reestablish_period   = config.test_mode_cfg.reestablish_period,
                                                             .f1u_peer_address     = config.test_mode_cfg.f1u_peer_address,
                                                             .nof_ues              = config.test_mode_cfg.nof_ues},
      .gnb_id                = config.gnb_id,
      .cu_up_id              = config.gnb_cu_up_id,
      .max_nof_ues           = config.max_nof_ues,
      .cu_up_name            = fmt::format("ocuup_{}", config.gnb_cu_up_id),
      .e1ap_json_log_enabled = config.loggers.e1ap_json_enabled,
      .e1ap_metrics_period =
          timer_duration{config.metrics.layers_cfg.enable_e1ap && config.metrics.common_metrics_cfg.enable_log_metrics
                             ? config.metrics.cu_up_report_period
                             : std::chrono::milliseconds{0}},
      .plmns                    = config.plmn_list,
      .statistics_report_period = std::chrono::duration_cast<std::chrono::seconds>(config.metrics.cu_up_report_period),
  };
}

std::map<five_qi_t, ocuup::cu_up_qos_config> ocudu::generate_cu_up_qos_config(const cu_up_unit_config& cu_up_config)
{
  if (cu_up_config.qos_cfg.empty()) {
    return config_helpers::make_default_cu_up_qos_config_list(
        cu_up_config.warn_on_drop,
        timer_duration(cu_up_config.metrics.layers_cfg.enable_pdcp ? cu_up_config.metrics.cu_up_report_period
                                                                   : std::chrono::milliseconds(0)),
        cu_up_config.test_mode_cfg.enabled);
  }

  std::map<five_qi_t, ocuup::cu_up_qos_config> out_cfg;

  for (const auto& qos : cu_up_config.qos_cfg) {
    if (out_cfg.find(qos.five_qi) != out_cfg.end()) {
      report_error("Duplicate 5QI configuration: {}\n", qos.five_qi);
    }

    // Convert PDCP custom config
    pdcp_custom_config& out_pdcp_custom = out_cfg[qos.five_qi].pdcp_custom_cfg;
    out_pdcp_custom.metrics_period =
        timer_duration(cu_up_config.metrics.layers_cfg.enable_pdcp ? cu_up_config.metrics.cu_up_report_period
                                                                   : std::chrono::milliseconds(0));
    out_pdcp_custom.rx.warn_on_drop = cu_up_config.warn_on_drop;
    out_pdcp_custom.tx.warn_on_drop = cu_up_config.warn_on_drop;
    out_pdcp_custom.tx.test_mode    = cu_up_config.test_mode_cfg.enabled;

    // Convert F1-U config
    ocuup::f1u_config& f1u_cfg = out_cfg[qos.five_qi].f1u_cfg;
    f1u_cfg.warn_on_drop       = cu_up_config.warn_on_drop;
    f1u_cfg.dl_t_notif_timer   = std::chrono::milliseconds(qos.f1u_cu_up.t_notify);
    f1u_cfg.queue_size         = qos.f1u_cu_up.queue_size;
    f1u_cfg.batch_size         = qos.f1u_cu_up.batch_size;
    f1u_cfg.metrics_period =
        timer_duration(cu_up_config.metrics.layers_cfg.enable_nrup ? cu_up_config.metrics.cu_up_report_period
                                                                   : std::chrono::milliseconds(0));
  }

  return out_cfg;
}

void ocudu::fill_cu_up_worker_manager_config(worker_manager_config& config, const cu_up_unit_config& unit_cfg)
{
  config.cu_up_cfg = worker_manager_config::cu_up_config{};

  config.cu_up_cfg->dl_ue_executor_queue_size   = unit_cfg.exec_cfg.dl_ue_executor_queue_size;
  config.cu_up_cfg->ul_ue_executor_queue_size   = unit_cfg.exec_cfg.ul_ue_executor_queue_size;
  config.cu_up_cfg->ctrl_ue_executor_queue_size = unit_cfg.exec_cfg.ctrl_ue_executor_queue_size;
  config.cu_up_cfg->strand_batch_size           = unit_cfg.exec_cfg.strand_batch_size;
  config.cu_up_cfg->executor_tracing_enable     = unit_cfg.trace_cfg.cu_up_enable;
  config.cu_up_cfg->skip_cu_up_executor         = unit_cfg.metrics.layers_cfg.skip_cu_up_executor;

  auto& pcap_cfg = config.pcap_cfg;
  if (unit_cfg.pcap_cfg.e1ap.enabled) {
    pcap_cfg.is_e1ap_enabled = true;
  }
  if (unit_cfg.pcap_cfg.ngu.enabled) {
    pcap_cfg.is_ngu_enabled = true;
  }
  if (unit_cfg.pcap_cfg.f1u.enabled) {
    pcap_cfg.is_f1u_enabled = true;
  }
}
