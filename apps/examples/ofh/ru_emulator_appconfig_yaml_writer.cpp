// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "ru_emulator_appconfig_yaml_writer.h"
#include "ru_emulator_appconfig.h"

using namespace ocudu;

/// Returns the configuration string accepted by the --prach_format option.
static std::string prach_format_to_str(ru_emulator_prach_format format)
{
  switch (format) {
    case ru_emulator_prach_format::LONG_F0:
      return "long";
    case ru_emulator_prach_format::SHORT_B4:
      return "short";
    default:
      return "none";
  }
}

/// Emits a CPU list as a flow sequence, matching how the sample configurations write it.
static void fill_cpu_list(YAML::Node node, const std::string& key, const std::vector<unsigned>& cpus)
{
  if (cpus.empty()) {
    return;
  }
  node[key] = cpus;
  node[key].SetStyle(YAML::EmitterStyle::Flow);
}

static void fill_log_section(YAML::Node node, const ru_emulator_log_appconfig& config)
{
  node["filename"] = config.filename;
  node["level"]    = ocudulog::basic_level_to_string(config.level);
}

static void fill_sdr_section(YAML::Node node, const ru_emulator_sdr_appconfig& config)
{
  node["device_driver"] = config.device_driver;
  node["device_args"]   = config.device_arguments;
  node["srate"]         = config.srate_MHz;
  if (config.dl_freq_override_Hz) {
    node["dl_freq_override"] = *config.dl_freq_override_Hz;
  }
  if (config.ul_freq_override_Hz) {
    node["ul_freq_override"] = *config.ul_freq_override_Hz;
  }
  node["tx_gain"]            = config.tx_gain_dB;
  node["rx_gain"]            = config.rx_gain_dB;
  node["center_freq_offset"] = config.center_freq_offset_Hz;
  node["calibrate_clock_ppm"] = config.calibrate_clock_ppm;
  node["lo_offset"]           = config.lo_offset_MHz;
  if (config.time_alignment_calibration) {
    node["time_alignment_calibration"] = *config.time_alignment_calibration;
  }
  node["transmission_mode"] = config.transmission_mode;
  node["power_ramping"]     = config.power_ramping_us;
  node["gain_backoff"]      = config.gain_backoff_dB;
  node["power_ceiling"]     = config.power_ceiling_dBFS;
  node["enable_clipping"]   = config.enable_clipping;
  node["otw_format"]        = config.otw_format;
  node["clock_source"]      = config.clock_source;
  node["sync_source"]       = config.sync_source;
  node["max_proc_delay"]    = config.max_proc_delay;
  node["execution_profile"] = config.execution_profile;
  fill_cpu_list(node, "ru_cpus", config.ru_cpus);
  node["pinning_policy"] = config.pinning_policy;
}

static YAML::Node build_cell_section(const ru_emulator_ofh_appconfig& config)
{
  YAML::Node node;

  node["network_interface"] = config.network_interface;
  node["ru_mac_addr"]       = config.ru_mac_address;
  node["du_mac_addr"]       = config.du_mac_address;
  // An unset VLAN identifier leaves the frames untagged, so the key is omitted rather than written as null.
  if (config.vlan_tag) {
    node["vlan_tag"] = *config.vlan_tag;
  }
  node["enable_promiscuous"] = config.enable_promiscuous;

  node["bandwidth"]  = bs_channel_bandwidth_to_MHz(config.bandwidth);
  node["common_scs"] = scs_to_khz(config.common_scs);
  node["dl_arfcn"]   = config.dl_arfcn;
  if (config.band) {
    node["band"] = static_cast<unsigned>(*config.band);
  }

  fill_cpu_list(node, "dl_port_id", config.ru_dl_port_id);
  fill_cpu_list(node, "ul_port_id", config.ru_ul_port_id);
  fill_cpu_list(node, "prach_port_id", config.ru_prach_port_id);
  node["prach_format"] = prach_format_to_str(config.prach_format);

  node["compr_method_ul"]       = config.ul_compr_method;
  node["compr_bitwidth_ul"]     = config.ul_compr_bitwidth;
  node["compr_method_dl"]       = config.dl_compr_method;
  node["compr_bitwidth_dl"]     = config.dl_compr_bitwidth;
  node["compr_method_prach"]    = config.prach_compr_method;
  node["compr_bitwidth_prach"]  = config.prach_compr_bitwidth;
  node["is_ul_static_compr_hdr"] = config.is_ul_static_compr_hdr;
  node["is_dl_static_compr_hdr"] = config.is_dl_static_compr_hdr;
  node["iq_scaling"]             = config.iq_scaling;

  node["t2a_max_cp_dl"] = config.T2a_max_cp_dl.count();
  node["t2a_min_cp_dl"] = config.T2a_min_cp_dl.count();
  node["t2a_max_cp_ul"] = config.T2a_max_cp_ul.count();
  node["t2a_min_cp_ul"] = config.T2a_min_cp_ul.count();
  node["t2a_max_up"]    = config.T2a_max_up.count();
  node["t2a_min_up"]    = config.T2a_min_up.count();
  node["ta3_max_up"]    = config.Ta3_max_up.count();
  node["ta3_min_up"]    = config.Ta3_min_up.count();

  fill_cpu_list(node, "ofh_cpus", config.ofh_cpus);

  // The SDR section is only present when this cell runs a real radio; without it the cell runs in loopback mode.
  if (config.sdr_config) {
    fill_sdr_section(node["sdr"], *config.sdr_config);
  }

  return node;
}

static void fill_ru_emu_section(YAML::Node node, const ru_emulator_appconfig& config)
{
  fill_cpu_list(node, "timing_cpus", config.timing_cpus);
  for (const auto& cell : config.ru_cfg) {
    node["cells"].push_back(build_cell_section(cell));
  }
}

void ocudu::fill_ru_emulator_appconfig_in_yaml_schema(YAML::Node& node, const ru_emulator_appconfig& config)
{
  fill_log_section(node["log"], config.log_cfg);
  fill_ru_emu_section(node["ru_emu"], config);
  if (config.dpdk_config) {
    node["dpdk"]["eal_args"] = config.dpdk_config->eal_args;
  }
}
