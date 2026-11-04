// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "dynamic_o_du_translators.h"
#include "apps/services/worker_manager/worker_manager_config.h"
#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_config_translators.h"
#include "apps/units/flexible_o_du/o_du_low/du_low_config_translator.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_translator.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_config_translator.h"
#include "dynamic_o_du_unit_config.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/bs_channel_bandwidth.h"

using namespace ocudu;

/// Derives the ratio between the number of UL slots and the TDD period. Returns 1 (all slots are UL) for FDD.
static double derive_ul_ratio(const std::optional<du_high_unit_tdd_ul_dl_config>& tdd_cfg)
{
  if (!tdd_cfg.has_value()) {
    return 1.0;
  }

  unsigned period_slots = tdd_cfg->pattern1.dl_ul_period_slots;
  unsigned nof_ul_slots = tdd_cfg->pattern1.nof_ul_slots + ((tdd_cfg->pattern1.nof_ul_symbols != 0) ? 1 : 0);
  if (tdd_cfg->pattern2.has_value()) {
    period_slots += tdd_cfg->pattern2->dl_ul_period_slots;
    nof_ul_slots += tdd_cfg->pattern2->nof_ul_slots + ((tdd_cfg->pattern2->nof_ul_symbols != 0) ? 1 : 0);
  }

  return static_cast<double>(nof_ul_slots) / static_cast<double>(period_slots);
}

ru_dummy_configuration ocudu::generate_ru_dummy_config(const ru_dummy_unit_config&                      ru_cfg,
                                                       span<const flexible_o_du_ru_config::cell_config> du_cells,
                                                       unsigned max_processing_delay_slots)
{
  ru_dummy_configuration out_cfg;

  const flexible_o_du_ru_config::cell_config& cell = du_cells.front();

  // Fill configuration parameters.
  out_cfg.are_metrics_enabled        = ru_cfg.metrics_cfg.enable_ru_metrics;
  out_cfg.scs                        = cell.scs;
  out_cfg.nof_sectors                = du_cells.size();
  out_cfg.max_processing_delay_slots = max_processing_delay_slots;
  out_cfg.dl_processing_delay        = ru_cfg.dl_processing_delay;
  out_cfg.time_scaling               = ru_cfg.time_scaling;

  return out_cfg;
}

static void fill_dummy_worker_manager_config(worker_manager_config& config, const ru_dummy_unit_config& ru_cfg)
{
  config.ru_dummy_cfg.emplace();

  ocudu_assert(config.config_affinities.size() == ru_cfg.cell_affinities.size(), "Invalid number of cell affinities");

  for (unsigned i = 0, e = ru_cfg.cell_affinities.size(); i != e; ++i) {
    config.config_affinities[i].push_back(ru_cfg.cell_affinities[i].ru_cpu_cfg);
  }
}

static void fill_fapi_dummy_worker_manager_config(worker_manager_config&        config,
                                                  const o_du_high_unit_config&  odu_high_cfg,
                                                  const fapi_dummy_unit_config& fapi_dummy_cfg)
{
  // A free running clock runs the DU high in blocking mode, so that tasks wait instead of being dropped.
  bool is_blocking_mode_enable =
      fapi_dummy_cfg.clock_mode == fapi_adaptor::phy_dummy_fapi_adaptor_clock_mode::free_running;
  fill_o_du_high_worker_manager_config(config, odu_high_cfg, is_blocking_mode_enable);

  const unsigned nof_cells = odu_high_cfg.du_high_cfg.config.cells_cfg.size();
  config.fapi_dummy_cfg.emplace(worker_manager_config::fapi_dummy_config{.nof_cells = nof_cells});

  ocudu_assert(config.config_affinities.size() == nof_cells, "Invalid number of cell affinities");
  for (auto& cell_affinities : config.config_affinities) {
    cell_affinities.push_back(fapi_dummy_cfg.clock_cpu_cfg);
  }
}

void ocudu::fill_dynamic_du_worker_manager_config(worker_manager_config&          config,
                                                  const dynamic_o_du_unit_config& unit_cfg)
{
  if (const auto* fapi_dummy_cfg = std::get_if<fapi_dummy_unit_config>(&unit_cfg.odu_low_cfg)) {
    fill_fapi_dummy_worker_manager_config(config, unit_cfg.odu_high_cfg, *fapi_dummy_cfg);
    return;
  }

  const dynamic_o_du_low& odu_low                 = std::get<dynamic_o_du_low>(unit_cfg.odu_low_cfg);
  bool                    is_blocking_mode_enable = false;
  if (std::holds_alternative<ru_sdr_unit_config>(odu_low.ru_cfg)) {
    const auto& driver      = std::get<ru_sdr_unit_config>(odu_low.ru_cfg).device_driver;
    is_blocking_mode_enable = (driver == "zmq");
  }
  fill_o_du_high_worker_manager_config(config, unit_cfg.odu_high_cfg, is_blocking_mode_enable);
  std::vector<du_low_cell_config> cell_params;
  cell_params.reserve(unit_cfg.odu_high_cfg.du_high_cfg.config.cells_cfg.size());
  for (const auto& cell : unit_cfg.odu_high_cfg.du_high_cfg.config.cells_cfg) {
    cell_params.push_back(du_low_cell_config{
        .nof_dl_antennas = cell.cell.nof_antennas_dl,
        .nof_ul_antennas = cell.cell.nof_antennas_ul,
        .channel_bw_mhz  = bs_channel_bandwidth_to_MHz(cell.cell.channel_bw_mhz),
        .pusch_max_nof_layers =
            cell.cell.pusch_cfg.enable_transform_precoding
                ? 1U
                : std::min({cell.cell.nof_antennas_ul, pusch_constants::MAX_NOF_LAYERS, cell.cell.pusch_cfg.max_rank}),
        .ul_ratio = derive_ul_ratio(cell.cell.tdd_ul_dl_cfg),
    });
  }
  fill_du_low_worker_manager_config(config, odu_low.du_low_cfg, is_blocking_mode_enable, cell_params);

  if (const auto* ru_sdr = std::get_if<ru_sdr_unit_config>(&odu_low.ru_cfg)) {
    fill_sdr_worker_manager_config(config, *ru_sdr);
  } else if (const auto* ru_ofh = std::get_if<ru_ofh_unit_parsed_config>(&odu_low.ru_cfg)) {
    fill_ofh_worker_manager_config(config, ru_ofh->config);
  } else if (const auto* ru_dummy = std::get_if<ru_dummy_unit_config>(&odu_low.ru_cfg)) {
    fill_dummy_worker_manager_config(config, *ru_dummy);
  }
}
