// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "dynamic_o_du_factory.h"
#include "apps/services/worker_manager/worker_manager.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_factories.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_factories.h"
#include "dynamic_o_du_translators.h"
#include "ocudu/adt/format.h"
#include "ocudu/ru/dummy/ru_dummy_factory.h"

using namespace ocudu;

static flexible_o_du_unit_config::ru_config
generate_ru_config(const std::variant<ru_sdr_unit_config, ru_ofh_unit_parsed_config, ru_dummy_unit_config>& ru_cfg)
{
  if (const auto* ru = std::get_if<ru_sdr_unit_config>(&ru_cfg)) {
    return flexible_o_du_unit_config::ru_config{.config            = ru->metrics_cfg.metrics_cfg,
                                                .enable_ru_metrics = ru->metrics_cfg.enable_ru_metrics,
                                                .rx_gain_dB        = static_cast<float>(ru->rx_gain_dB)};
  }

  if (const auto* ru = std::get_if<ru_dummy_unit_config>(&ru_cfg)) {
    return flexible_o_du_unit_config::ru_config{.config            = ru->metrics_cfg.metrics_cfg,
                                                .enable_ru_metrics = ru->metrics_cfg.enable_ru_metrics,
                                                .rx_gain_dB        = 0.F};
  }

  if (const auto* ru = std::get_if<ru_ofh_unit_parsed_config>(&ru_cfg)) {
    return flexible_o_du_unit_config::ru_config{.config            = ru->config.metrics_cfg.metrics_cfg,
                                                .enable_ru_metrics = ru->config.metrics_cfg.enable_ru_metrics,
                                                .rx_gain_dB        = 0.F};
  }

  return {};
}

dynamic_o_du_factory::dynamic_o_du_factory(const o_du_high_unit_config& odu_high_cfg,
                                           const dynamic_o_du_low&      odu_low_cfg_) :
  flexible_o_du_factory(flexible_o_du_unit_config{.odu_high_cfg = odu_high_cfg,
                                                  .du_low_cfg   = odu_low_cfg_.du_low_cfg,
                                                  .ru_cfg       = generate_ru_config(odu_low_cfg_.ru_cfg)}),
  odu_low_cfg(odu_low_cfg_)
{
}

static std::unique_ptr<radio_unit> create_dummy_radio_unit(const ru_dummy_unit_config&          ru_cfg,
                                                           const flexible_o_du_ru_config&       ru_config,
                                                           const flexible_o_du_ru_dependencies& ru_dependencies)
{
  ru_dummy_dependencies dependencies{
      .logger          = ocudulog::fetch_basic_logger("RU", true),
      .executor        = &ru_dependencies.workers.get_dummy_ru_executor_mapper().common_executor(),
      .symbol_notifier = ru_dependencies.symbol_notifier,
      .timing_notifier = ru_dependencies.timing_notifier,
      .error_notifier  = ru_dependencies.error_notifier,
  };

  return create_dummy_ru(generate_ru_dummy_config(ru_cfg, ru_config.cells, ru_config.max_processing_delay),
                         dependencies);
}

std::unique_ptr<radio_unit>
dynamic_o_du_factory::create_radio_unit(const flexible_o_du_ru_config&       ru_config,
                                        const flexible_o_du_ru_dependencies& ru_dependencies)
{
  const auto& ru_cfg = odu_low_cfg.ru_cfg;
  if (const auto* cfg = std::get_if<ru_ofh_unit_parsed_config>(&ru_cfg)) {
    return create_ofh_radio_unit(
        cfg->config, ru_config, ru_dependencies, odu_low_cfg.du_low_cfg.hal_config.has_value());
  }

  if (const auto* cfg = std::get_if<ru_sdr_unit_config>(&ru_cfg)) {
    return create_sdr_radio_unit(*cfg, ru_config, ru_dependencies);
  }

  return create_dummy_radio_unit(std::get<ru_dummy_unit_config>(ru_cfg), ru_config, ru_dependencies);
}
