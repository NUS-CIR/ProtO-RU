// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "cu_up_unit_config_yaml_writer.h"
#include "apps/helpers/metrics/metrics_config_yaml_writer.h"
#include "apps/helpers/network/udp_config_yaml_writer.h"
#include "apps/helpers/xnu/xnu_config_yaml_writer.h"
#include "cu_up_unit_config.h"
#include "ocudu/adt/span.h"

using namespace ocudu;

static void fill_cu_up_ngu_gtpu_section(YAML::Node& node, const cu_up_unit_ngu_gtpu_config& config)
{
  auto gtpu_node                        = node["gtpu"];
  gtpu_node["queue_size"]               = config.gtpu_queue_size;
  gtpu_node["batch_size"]               = config.gtpu_batch_size;
  gtpu_node["reordering_timer"]         = config.gtpu_reordering_timer_ms;
  gtpu_node["rate_limiter_period"]      = config.rate_limiter_period.count();
  gtpu_node["teid_release_linger_time"] = config.gtpu_teid_release_linger_time.count();
  gtpu_node["ignore_ue_ambr"]           = config.ignore_ue_ambr;
}

static void fill_cu_up_ngu_socket_entry(YAML::Node& node, const cu_up_unit_ngu_socket_config& config)
{
  node["bind_addr"]      = config.bind_addr;
  node["bind_interface"] = config.bind_interface;
  node["ext_addr"]       = config.ext_addr;
  if (config.sst.has_value()) {
    node["sst"] = static_cast<unsigned>(*config.sst);
  }
  if (config.sd.has_value()) {
    node["sd"] = *config.sd;
  }
  fill_udp_config_in_yaml_schema(node["udp"], config.udp_config);
}

static void fill_cu_up_ngu_socket_section(YAML::Node node, const std::vector<cu_up_unit_ngu_socket_config>& sock_cfg)
{
  auto sock_node = node["socket"];
  for (const auto& cfg : sock_cfg) {
    YAML::Node node_sock;
    fill_cu_up_ngu_socket_entry(node_sock, cfg);
    sock_node.push_back(node_sock);
  }
}

static void fill_cu_up_xnu_gtpu_section(YAML::Node& node, const cu_up_unit_xnu_gtpu_config& config)
{
  auto gtpu_node                        = node["gtpu"];
  gtpu_node["queue_size"]               = config.gtpu_queue_size;
  gtpu_node["batch_size"]               = config.gtpu_batch_size;
  gtpu_node["reordering_timer"]         = config.gtpu_reordering_timer_ms;
  gtpu_node["rate_limiter_period"]      = config.rate_limiter_period.count();
  gtpu_node["teid_release_linger_time"] = config.gtpu_teid_release_linger_time.count();
}

static void fill_cu_up_xnu_section(YAML::Node& node, const cu_up_unit_xnu_config& config)
{
  fill_cu_up_xnu_gtpu_section(node, config.gtpu_cfg);
  fill_xnu_config_yaml_schema(node, config.sockets_cfg);
}

static void fill_cu_up_ngu_section(YAML::Node node, const cu_up_unit_ngu_config& config)
{
  node["no_core"] = config.no_core;
  fill_cu_up_ngu_gtpu_section(node, config.gtpu_cfg);
  fill_cu_up_ngu_socket_section(node, config.ngu_socket_cfg);
}

static void fill_cu_up_metrics_layers_section(YAML::Node node, const cu_up_unit_metrics_layer_config& config)
{
  node["enable_e1ap"]         = config.enable_e1ap;
  node["enable_pdcp"]         = config.enable_pdcp;
  node["enable_nrup_cu"]      = config.enable_nrup;
  node["skip_cu_up_executor"] = config.skip_cu_up_executor;
}

static void fill_cu_up_metrics_section(YAML::Node node, const cu_up_unit_metrics_config& config)
{
  auto periodicity_node                   = node["periodicity"];
  periodicity_node["cu_up_report_period"] = config.cu_up_report_period.count();

  fill_cu_up_metrics_layers_section(node["layers"], config.layers_cfg);
}

static void fill_cu_up_trace_section(YAML::Node node, const cu_up_unit_trace_config& config)
{
  auto layers_node            = node["layers"];
  layers_node["cu_up_enable"] = config.cu_up_enable;
}

static void fill_cu_up_pcap_section(YAML::Node node, const cu_up_unit_pcap_config& config)
{
  // TODO: Rename prefix from n3_ to ngu_
  node["n3_filename"]   = config.ngu.filename;
  node["n3_enable"]     = config.ngu.enabled;
  node["f1u_filename"]  = config.f1u.filename;
  node["f1u_enable"]    = config.f1u.enabled;
  node["e1ap_filename"] = config.e1ap.filename;
  node["e1ap_enable"]   = config.e1ap.enabled;
}

static void fill_cu_up_log_section(YAML::Node node, const cu_up_unit_logger_config& config)
{
  node["pdcp_level"]   = ocudulog::basic_level_to_string(config.pdcp_level);
  node["sdap_level"]   = ocudulog::basic_level_to_string(config.sdap_level);
  node["gtpu_level"]   = ocudulog::basic_level_to_string(config.gtpu_level);
  node["f1u_level"]    = ocudulog::basic_level_to_string(config.f1u_level);
  node["cu_level"]     = ocudulog::basic_level_to_string(config.cu_level);
  node["sec_level"]    = ocudulog::basic_level_to_string(config.sec_level);
  node["rohc_level"]   = ocudulog::basic_level_to_string(config.rohc_level);
  node["hex_max_size"] = config.hex_max_size;
}

static void fill_cu_up_section(YAML::Node node, const cu_up_unit_config& config)
{
  node["warn_on_drop"] = config.warn_on_drop;
  if (!config.plmn_list.empty()) {
    node["plmn_list"] = config.plmn_list;
  }
}

static void fill_cu_up_f1_qos_section(YAML::Node node, const cu_cp_unit_f1u_config& config)
{
  node["backoff_timer"] = config.t_notify;
}

static void fill_cu_up_qos_entry(YAML::Node node, const cu_up_unit_qos_config& config)
{
  node["five_qi"] = to_underlying(config.five_qi);
  fill_cu_up_f1_qos_section(node["f1u_cu_up"], config.f1u_cu_up);
}

static YAML::Node get_last_entry(YAML::Node node)
{
  auto it = node.begin();
  for (unsigned i = 1, e = node.size(); i != e; ++i) {
    ++it;
  }
  return static_cast<YAML::Node>(*it);
}

static void fill_cu_up_qos_section(YAML::Node node, span<const cu_up_unit_qos_config> qos_cfg)
{
  auto qos_node = node["qos"];
  for (const auto& qos : qos_cfg) {
    auto node_entry = std::find_if(qos_node.begin(), qos_node.end(), [five = qos.five_qi](const YAML::Node& tmp) {
      return static_cast<uint16_t>(five) == tmp["five_qi"].as<uint16_t>();
    });
    if (node_entry != qos_node.end()) {
      YAML::Node node_five = *node_entry;
      fill_cu_up_qos_entry(node_five, qos);
    } else {
      qos_node.push_back(YAML::Node());
      fill_cu_up_qos_entry(get_last_entry(qos_node), qos);
    }
  }
}

void ocudu::fill_cu_up_config_in_yaml_schema(YAML::Node& node, const cu_up_unit_config& config)
{
  node["gnb_id"]            = config.gnb_id.id;
  node["gnb_id_bit_length"] = static_cast<unsigned>(config.gnb_id.bit_length);
  node["gnb_cu_up_id"]      = static_cast<uint64_t>(config.gnb_cu_up_id);

  app_helpers::fill_metrics_appconfig_in_yaml_schema(node, config.metrics.common_metrics_cfg);
  fill_cu_up_metrics_section(node["metrics"], config.metrics);

  fill_cu_up_trace_section(node["trace"], config.trace_cfg);
  fill_cu_up_log_section(node["log"], config.loggers);
  fill_cu_up_pcap_section(node["pcap"], config.pcap_cfg);

  YAML::Node cu_up_node = node["cu_up"];
  fill_cu_up_section(cu_up_node, config);
  fill_cu_up_ngu_section(cu_up_node["ngu"], config.ngu_cfg);
  YAML::Node xnu_node = cu_up_node["xnu"];
  fill_cu_up_xnu_section(xnu_node, config.xnu_cfg);

  fill_cu_up_qos_section(cu_up_node, config.qos_cfg);
}
