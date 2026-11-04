// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "dynamic_o_du_unit_config_validator.h"
#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_config_validator.h"
#include "apps/units/flexible_o_du/o_du_low/du_low_config_validator.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_validator.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_config_validator.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/prach/prach_configuration.h"
#include <algorithm>

using namespace ocudu;

static std::vector<du_low_prach_validation_config> get_du_low_validation_dependencies(const du_high_unit_config& config)
{
  std::vector<du_low_prach_validation_config> out_cfg(config.cells_cfg.size());

  for (unsigned i = 0, e = config.cells_cfg.size(); i != e; ++i) {
    du_low_prach_validation_config&      out_cell = out_cfg[i];
    const du_high_unit_base_cell_config& in_cell  = config.cells_cfg[i].cell;

    // Get PRACH info.
    subcarrier_spacing  common_scs = in_cell.common_scs;
    prach_configuration prach_info = prach_configuration_get(band_helper::get_freq_range(in_cell.band.value()),
                                                             band_helper::get_duplex_mode(in_cell.band.value()),
                                                             in_cell.prach_cfg.prach_config_index.value());

    // PRACH format type.
    out_cell.format = prach_info.format;

    // Get preamble info.
    prach_preamble_information preamble_info =
        is_long_preamble(prach_info.format)
            ? get_prach_preamble_long_info(prach_info.format)
            : get_prach_preamble_short_info(prach_info.format, to_ra_subcarrier_spacing(common_scs), false);

    out_cell.prach_scs             = preamble_info.scs;
    out_cell.zero_correlation_zone = in_cell.prach_cfg.zero_correlation_zone;
    out_cell.nof_prach_ports       = in_cell.prach_cfg.ports.size();
    out_cell.nof_antennas_ul       = in_cell.nof_antennas_ul;
  }

  return out_cfg;
}

static std::vector<ru_ofh_cell_validation_config> get_ru_ofh_validation_dependencies(const du_high_unit_config& config)
{
  std::vector<ru_ofh_cell_validation_config> out_cfg(config.cells_cfg.size());

  for (unsigned i = 0, e = config.cells_cfg.size(); i != e; ++i) {
    ru_ofh_cell_validation_config&       out_cell = out_cfg[i];
    const du_high_unit_base_cell_config& in_cell  = config.cells_cfg[i].cell;

    // The DU high validation, which runs first, guarantees the topology is set.
    out_cell.scs             = in_cell.common_scs;
    out_cell.tx_ant_topology = *in_cell.tx_ant_topology;
    out_cell.nof_prach_ports = in_cell.prach_cfg.ports.size();
    out_cell.nof_antennas_dl = in_cell.nof_antennas_dl;
    out_cell.nof_antennas_ul = in_cell.nof_antennas_ul;
  }

  return out_cfg;
}

static std::vector<ru_sdr_cell_validation_config> get_ru_sdr_validation_dependencies(const du_high_unit_config& config)
{
  std::vector<ru_sdr_cell_validation_config> out_cfg(config.cells_cfg.size());

  for (unsigned i = 0, e = config.cells_cfg.size(); i != e; ++i) {
    ru_sdr_cell_validation_config&       out_cell = out_cfg[i];
    const du_high_unit_base_cell_config& in_cell  = config.cells_cfg[i].cell;

    // Validates the sampling rate is compatible with the PRACH sequence.
    out_cell.common_scs            = in_cell.common_scs;
    prach_configuration prach_info = prach_configuration_get(band_helper::get_freq_range(in_cell.band.value()),
                                                             band_helper::get_duplex_mode(in_cell.band.value()),
                                                             in_cell.prach_cfg.prach_config_index.value());
    out_cell.prach_format          = prach_info.format;
    out_cell.channel_bw_mhz        = in_cell.channel_bw_mhz;
    out_cell.dplx_mode             = band_helper::get_duplex_mode(in_cell.band.value());
    out_cell.preamble_info =
        is_long_preamble(prach_info.format)
            ? get_prach_preamble_long_info(prach_info.format)
            : get_prach_preamble_short_info(prach_info.format, to_ra_subcarrier_spacing(in_cell.common_scs), false);
    out_cell.nof_antennas_dl = in_cell.nof_antennas_dl;
    out_cell.nof_antennas_ul = in_cell.nof_antennas_ul;
  }

  return out_cfg;
}

/// Validates the FAPI dummy configuration against the DU high test mode configuration.
static bool validate_fapi_dummy_config(const du_high_unit_config& du_hi_cfg)
{
  if (!du_hi_cfg.is_testmode_enabled()) {
    fmt::print("The FAPI dummy requires the test mode UE to be configured\n");
    return false;
  }

  if (du_hi_cfg.test_mode_cfg.test_ue.auto_ack_indication_delay.has_value()) {
    fmt::print("The FAPI dummy is not compatible with the test mode auto ACK indication delay\n");
    return false;
  }

  // A single clock drives the slots of all the cells.
  const subcarrier_spacing scs = du_hi_cfg.cells_cfg.front().cell.common_scs;
  if (std::any_of(du_hi_cfg.cells_cfg.begin(), du_hi_cfg.cells_cfg.end(), [scs](const auto& cell) {
        return cell.cell.common_scs != scs;
      })) {
    fmt::print("The FAPI dummy requires all the cells to have the same subcarrier spacing\n");
    return false;
  }

  return true;
}

bool ocudu::validate_dynamic_o_du_unit_config(const dynamic_o_du_unit_config& config)
{
  if (!validate_o_du_high_config(config.odu_high_cfg)) {
    return false;
  }

  if (std::holds_alternative<fapi_dummy_unit_config>(config.odu_low_cfg)) {
    return validate_fapi_dummy_config(config.odu_high_cfg.du_high_cfg.config);
  }

  const dynamic_o_du_low& odu_low = std::get<dynamic_o_du_low>(config.odu_low_cfg);
  auto du_low_dependencies        = get_du_low_validation_dependencies(config.odu_high_cfg.du_high_cfg.config);
  if (!validate_du_low_config(odu_low.du_low_cfg, du_low_dependencies)) {
    return false;
  }

  if (auto* ru_ofh = std::get_if<ru_ofh_unit_parsed_config>(&odu_low.ru_cfg)) {
    auto ru_ofh_dependencies = get_ru_ofh_validation_dependencies(config.odu_high_cfg.du_high_cfg.config);
    return validate_ru_ofh_config(ru_ofh->config, ru_ofh_dependencies);
  }

  if (auto* ru_sdr = std::get_if<ru_sdr_unit_config>(&odu_low.ru_cfg)) {
    auto ru_sdr_dependencies = get_ru_sdr_validation_dependencies(config.odu_high_cfg.du_high_cfg.config);
    return validate_ru_sdr_config(*ru_sdr, ru_sdr_dependencies);
  }

  return true;
}
