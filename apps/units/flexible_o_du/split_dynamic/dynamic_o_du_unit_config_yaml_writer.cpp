// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "dynamic_o_du_unit_config_yaml_writer.h"
#include "apps/helpers/metrics/metrics_config_yaml_writer.h"
#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_config_yaml_writer.h"
#include "apps/units/flexible_o_du/o_du_low/du_low_config_yaml_writer.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_yaml_writer.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_config_yaml_writer.h"
#include "dynamic_o_du_unit_config.h"
#include "ocudu/adt/format.h"

using namespace ocudu;

static void fill_ru_dummy_metrics_section(YAML::Node node, const ru_dummy_unit_metrics_config& config)
{
  app_helpers::fill_metrics_appconfig_in_yaml_schema(node, config.metrics_cfg);

  auto metrics_node        = node["metrics"];
  auto layers_node         = metrics_node["layers"];
  layers_node["enable_ru"] = config.enable_ru_metrics;
}

static void fill_ru_dummy_config(YAML::Node node, const ru_dummy_unit_config& config)
{
  fill_ru_dummy_metrics_section(node, config.metrics_cfg);

  auto ru_dummy_node                   = node["ru_dummy"];
  ru_dummy_node["dl_processing_delay"] = config.dl_processing_delay;
  ru_dummy_node["time_scaling"]        = config.time_scaling;

  auto expert_exec_node = node["expert_execution"];

  auto cell_affinities_node = expert_exec_node["cell_affinities"];
  while (config.cell_affinities.size() > cell_affinities_node.size()) {
    cell_affinities_node.push_back(YAML::Node());
  }

  unsigned index = 0;
  for (auto cell : cell_affinities_node) {
    const auto& expert = config.cell_affinities[index];

    if (expert.ru_cpu_cfg.mask.any()) {
      cell["ru_cpus"] = fmt::format("{:n}", span<const size_t>(expert.ru_cpu_cfg.mask.get_cpu_ids()));
    }
    cell["ru_pinning"] = to_string(expert.ru_cpu_cfg.pinning_policy);

    ++index;
  }
}

static void fill_fapi_dummy_config_in_yaml_schema(YAML::Node& node, const fapi_dummy_unit_config& config)
{
  YAML::Node fapi_dummy_node = node["fapi_dummy"];
  fapi_dummy_node["clock"] =
      (config.clock_mode == fapi_adaptor::phy_dummy_fapi_adaptor_clock_mode::realtime) ? "realtime" : "free_running";
  fapi_dummy_node["ul_indication_delay_slots"]  = config.ul_indication_delay_slots;
  fapi_dummy_node["max_processing_delay_slots"] = config.max_processing_delay_slots;
}

void ocudu::fill_dynamic_o_du_unit_config_in_yaml_schema(YAML::Node& node, const dynamic_o_du_unit_config& config)
{
  fill_o_du_high_config_in_yaml_schema(node, config.odu_high_cfg);

  if (config.fapi_dummy_cfg.has_value()) {
    fill_fapi_dummy_config_in_yaml_schema(node, *config.fapi_dummy_cfg);
    return;
  }

  fill_du_low_config_in_yaml_schema(node, config.du_low_cfg);

  if (std::holds_alternative<ru_dummy_unit_config>(config.ru_cfg)) {
    fill_ru_dummy_config(node, std::get<ru_dummy_unit_config>(config.ru_cfg));
  } else if (std::holds_alternative<ru_ofh_unit_parsed_config>(config.ru_cfg)) {
    fill_ru_ofh_config_in_yaml_schema(node, std::get<ru_ofh_unit_parsed_config>(config.ru_cfg).config);
  } else if (std::holds_alternative<ru_sdr_unit_config>(config.ru_cfg)) {
    fill_ru_sdr_config_in_yaml_schema(node, std::get<ru_sdr_unit_config>(config.ru_cfg));
  }
}
