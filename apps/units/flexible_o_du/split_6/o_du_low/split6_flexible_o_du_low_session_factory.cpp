// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "split6_flexible_o_du_low_session_factory.h"
#include "apps/services/worker_manager/worker_manager.h"
#include "apps/units/flexible_o_du/o_du_low/du_low_config_validator.h"
#include "apps/units/flexible_o_du/o_du_low/o_du_low_unit_factory.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_validator.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_factories.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_config_helpers.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_config_validator.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_factories.h"
#include "apps/units/flexible_o_du/split_helpers/flexible_o_du_configs.h"
#include "external/fmt/include/fmt/chrono.h"
#include "split6_constants.h"
#include "split6_flexible_o_du_low_session.h"
#include "ocudu/adt/format.h"
#include "ocudu/du/du_low/o_du_low_config.h"
#include "ocudu/fapi/cell_config.h"
#include "ocudu/fapi_adaptor/phy/p7/phy_fapi_p7_sector_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/phy/phy_fapi_fastpath_adaptor.h"
#include "ocudu/fapi_adaptor/phy/phy_fapi_sector_fastpath_adaptor.h"
#include "ocudu/ran/prach/prach_configuration.h"
#include "ocudu/ran/slot_pdu_capacity_constants.h"

using namespace ocudu;

/// \brief Returns the topology of the given number of transmit antennas.
///
/// Split 6 has no DU high, so this process derives the topology of the carrier that it receives.
/// TODO: FAPI carries no antenna topology, so only the default topologies are used. A DU high configured with another
/// topology would build a different codebook than the PHY.
static antenna_topology get_tx_antenna_topology(unsigned nof_tx_antennas)
{
  std::optional<antenna_topology> topology = get_single_panel_antenna_topology(nof_tx_antennas);
  report_fatal_error_if_not(
      topology.has_value(), "No antenna topology is defined for {} transmit antennas.", nof_tx_antennas);

  return *topology;
}

std::optional<std::chrono::system_clock::time_point>
split6_flexible_o_du_low_session_factory::start_time_calculator::calculate_start_time() const
{
  if (jitter.count() == 0) {
    return std::nullopt;
  }

  auto                      current_time = std::chrono::system_clock::now();
  std::chrono::milliseconds ms_since_epoch =
      std::chrono::duration_cast<std::chrono::milliseconds>(current_time.time_since_epoch());

  std::chrono::milliseconds offset_in_window = ms_since_epoch % window_size;

  if (offset_in_window < jitter || offset_in_window > jitter_upper_border) {
    logger.warning("Current start time is too close to the start window border, synchronization may fail");
  }

  auto start_time = std::chrono::floor<std::chrono::seconds>(current_time + (2 * window_size - offset_in_window));
  logger.debug("Start.request received at '{:%Y-%m-%d %H:%M:%S}' jitter_ms={} window_size={}. Calculated O-DU low "
               "start at '{:%Y-%m-%d %H:%M:%S}'",
               current_time,
               jitter,
               window_size,
               start_time);

  // Return next window start.
  return {start_time};
}

std::unique_ptr<split6_flexible_o_du_low_session>
split6_flexible_o_du_low_session_factory::create_o_du_low_session(const fapi::cell_configuration& config)
{
  auto odu = std::make_unique<split6_flexible_o_du_low_session>(
      notifier, std::chrono::milliseconds(unit_config.du_report_period));

  // Create Radio Unit.
  auto ru = create_radio_unit(*odu, config);
  if (!ru) {
    return nullptr;
  }

  // Create O-DU low.
  auto odu_low = create_o_du_low(config,
                                 {odu->get_upper_ru_dl_rg_adapter(),
                                  odu->get_upper_ru_ul_request_adapter(),
                                  workers.get_du_low_executor_mapper(),
                                  workers.get_cmd_line_executor()});

  if (!odu_low.o_du_lo) {
    return nullptr;
  }

  // Get the FAPI sector adaptor to get the dependencies.
  auto& fapi_sector_adaptor = odu_low.o_du_lo->get_phy_fapi_fastpath_adaptor()
                                  .get_sector_adaptor(split6_du_low::CELL_ID)
                                  .get_p7_sector_adaptor();

  // Create P7 requests adaptor.
  auto adaptor = p7_requests_adaptor_factory->create(config,
                                                     fapi_sector_adaptor.get_p7_requests_gateway(),
                                                     fapi_sector_adaptor.get_p7_last_request_notifier(),
                                                     ru->get_controller());

  if (!adaptor) {
    return nullptr;
  }

  odu->set_dependencies(std::move(adaptor),
                        std::move(odu_low.o_du_lo),
                        std::move(ru),
                        timers.create_unique_timer(workers.get_metrics_executor()));

  return odu;
}

static std::vector<du_low_prach_validation_config>
get_du_low_validation_dependencies(const fapi::cell_configuration& config)
{
  std::vector<du_low_prach_validation_config> out_cfg(split6_du_low::NOF_CELLS_SUPPORTED);

  du_low_prach_validation_config& out_cell = out_cfg.front();

  // Get PRACH info.
  subcarrier_spacing common_scs = config.scs_common;

  prach_configuration prach_info = prach_configuration_get(
      split6_du_low::freq_range, config.duplex, config.prach_cfg.rach_cfg_generic.prach_config_index);
  // PRACH format type.
  out_cell.format = prach_info.format;

  // Get preamble info.
  prach_preamble_information preamble_info =
      is_long_preamble(prach_info.format)
          ? get_prach_preamble_long_info(prach_info.format)
          : get_prach_preamble_short_info(prach_info.format, to_ra_subcarrier_spacing(common_scs), false);

  out_cell.prach_scs             = preamble_info.scs;
  out_cell.zero_correlation_zone = config.prach_cfg.rach_cfg_generic.zero_correlation_zone_config;
  out_cell.nof_prach_ports       = split6_du_low::PRACH_NOF_PORTS;
  out_cell.nof_antennas_ul       = config.carrier_cfg.num_rx_ant;

  return out_cfg;
}

o_du_low_unit
split6_flexible_o_du_low_session_factory::create_o_du_low(const fapi::cell_configuration&   config,
                                                          const o_du_low_unit_dependencies& odu_low_dependencies)
{
  odu::cell_prach_ports_entry prach_ports = {split6_du_low::PRACH_PORT};

  auto du_low_dependencies = get_du_low_validation_dependencies(config);
  if (!validate_du_low_config(unit_config.du_low_cfg, du_low_dependencies)) {
    return {};
  }

  o_du_low_unit_config odu_low_cfg = {unit_config.du_low_cfg, {}, {}};

  // The SDR Radio Unit gain is known to this application unit, so it is subtracted from the configured receive gain is
  // applied by the external O-RU and is not visible here, so no correction is applied.
  const auto* sdr_cfg    = std::get_if<ru_sdr_unit_config>(&unit_config.ru_cfg);
  float       rx_gain_dB = sdr_cfg ? static_cast<float>(sdr_cfg->rx_gain_dB) : 0.0F;

  fapi_adaptor::phy_fapi_p7_sector_fastpath_adaptor_config p7_cfg = {
      .sector_id                     = 0,
      .nof_slots_request_headroom    = unit_config.du_low_cfg.expert_phy_cfg.nof_slots_request_headroom,
      .allow_request_on_empty_ul_tti = unit_config.du_low_cfg.expert_phy_cfg.allow_request_on_empty_uplink_slot,
      .scs                           = config.scs_common,
      .scs_common                    = config.scs_common,
      .carrier_cfg                   = config.carrier_cfg,
      .tx_ant_topology               = get_tx_antenna_topology(config.carrier_cfg.num_tx_ant),
      .prach_cfg                     = config.prach_cfg,
      .prach_ports                   = prach_ports,
      .dbfs_to_dbm_conversion_factor =
          unit_config.du_low_cfg.power_calibration.dbfs_to_dbm_conversion_factor - rx_gain_dB,
      .db_to_dbfs_conversion_factor = unit_config.du_low_cfg.power_calibration.db_to_dbfs_conversion_factor};

  odu_low_cfg.fapi_cfg.sectors.push_back({.p5_config = {.sector_id = 0}, .p7_config = p7_cfg});

  auto& du_low_cell = odu_low_cfg.cells.emplace_back();

  du_low_cell.duplex               = config.duplex;
  du_low_cell.freq_range           = split6_du_low::freq_range;
  du_low_cell.bw_rb                = config.carrier_cfg.dl_grid_size;
  du_low_cell.nof_rx_antennas      = config.carrier_cfg.num_rx_ant;
  du_low_cell.nof_tx_antennas      = config.carrier_cfg.num_tx_ant;
  du_low_cell.tx_ant_topology      = get_tx_antenna_topology(config.carrier_cfg.num_tx_ant);
  du_low_cell.prach_ports          = prach_ports;
  du_low_cell.scs_common           = config.scs_common;
  du_low_cell.prach_config_index   = config.prach_cfg.rach_cfg_generic.prach_config_index;
  du_low_cell.max_puschs_per_slot  = MAX_PUSCH_PDUS_PER_SLOT;
  du_low_cell.pusch_max_nof_layers = split6_du_low::PUSCH_MAX_NOF_LAYERS;

  // TDD pattern.
  du_low_cell.tdd_pattern = config.tdd_ul_dl_cfg_common;

  o_du_low_unit_factory odu_low_factory(unit_config.du_low_cfg.hal_config);

  return odu_low_factory.create(odu_low_cfg, odu_low_dependencies);
}

static flexible_o_du_ru_config generate_o_du_ru_config(const fapi::cell_configuration& config,
                                                       unsigned                        expected_max_proc_delay,
                                                       bool                            uses_ofh,
                                                       const std::string&              ru_dft_processor_type)
{
  flexible_o_du_ru_config out_cfg;
  out_cfg.prach_nof_ports = split6_du_low::PRACH_NOF_PORTS;

  // Open Fronthaul notifies OTA + max_proc_delay.
  if (uses_ofh) {
    out_cfg.max_processing_delay = expected_max_proc_delay;
  } else {
    // Split 8 notifies OTA + max_proc_delay + 1ms.
    out_cfg.max_processing_delay = expected_max_proc_delay - get_nof_slots_per_subframe(config.scs_common);
    out_cfg.dft_processor_type   = ru_dft_processor_type;
  }

  // Add one cell.
  auto& out_cell           = out_cfg.cells.emplace_back();
  out_cell.nof_rx_antennas = config.carrier_cfg.num_rx_ant;
  out_cell.nof_tx_antennas = config.carrier_cfg.num_tx_ant;
  out_cell.tx_ant_topology = get_tx_antenna_topology(config.carrier_cfg.num_tx_ant);
  out_cell.scs             = config.scs_common;
  out_cell.dl_arfcn        = config.carrier_cfg.dl_f_ref_arfcn;
  out_cell.ul_arfcn        = config.carrier_cfg.ul_f_ref_arfcn;
  out_cell.bw              = MHz_to_bs_channel_bandwidth(config.carrier_cfg.dl_bandwidth);
  out_cell.cp              = config.cp;
  out_cell.freq_range      = split6_du_low::freq_range;

  // TDD pattern.
  out_cell.tdd_config = config.tdd_ul_dl_cfg_common;

  return out_cfg;
}

static std::vector<ru_ofh_cell_validation_config>
get_ru_ofh_validation_dependencies(const fapi::cell_configuration& config)
{
  std::vector<ru_ofh_cell_validation_config> out_cfg(split6_du_low::NOF_CELLS_SUPPORTED);

  ru_ofh_cell_validation_config& out_cell = out_cfg.front();

  // Validates the sampling rate is compatible with the PRACH sequence.
  out_cell.scs             = config.scs_common;
  out_cell.tx_ant_topology = get_tx_antenna_topology(config.carrier_cfg.num_tx_ant);
  out_cell.nof_prach_ports = split6_du_low::PRACH_NOF_PORTS;
  out_cell.nof_antennas_dl = config.carrier_cfg.num_tx_ant;
  out_cell.nof_antennas_ul = config.carrier_cfg.num_rx_ant;

  return out_cfg;
}

static std::vector<ru_sdr_cell_validation_config>
get_ru_sdr_validation_dependencies(const fapi::cell_configuration& config)
{
  std::vector<ru_sdr_cell_validation_config> out_cfg(split6_du_low::NOF_CELLS_SUPPORTED);

  ru_sdr_cell_validation_config& out_cell = out_cfg.front();

  // Validates the sampling rate is compatible with the PRACH sequence.
  out_cell.common_scs     = config.scs_common;
  out_cell.channel_bw_mhz = MHz_to_bs_channel_bandwidth(config.carrier_cfg.dl_bandwidth);
  out_cell.dplx_mode      = config.duplex;

  prach_configuration prach_info = prach_configuration_get(
      frequency_range::FR1, out_cell.dplx_mode, config.prach_cfg.rach_cfg_generic.prach_config_index);

  out_cell.prach_format = prach_info.format;

  out_cell.preamble_info =
      is_long_preamble(prach_info.format)
          ? get_prach_preamble_long_info(prach_info.format)
          : get_prach_preamble_short_info(prach_info.format, to_ra_subcarrier_spacing(config.scs_common), false);

  out_cell.nof_antennas_dl = config.carrier_cfg.num_tx_ant;
  out_cell.nof_antennas_ul = config.carrier_cfg.num_rx_ant;

  return out_cfg;
}

std::unique_ptr<radio_unit>
split6_flexible_o_du_low_session_factory::create_radio_unit(split6_flexible_o_du_low_session& odu_low,
                                                            const fapi::cell_configuration&   config)
{
  auto ru_config = generate_o_du_ru_config(config,
                                           unit_config.du_low_cfg.expert_phy_cfg.max_processing_delay_slots,
                                           std::holds_alternative<ru_ofh_unit_parsed_config>(unit_config.ru_cfg),
                                           unit_config.du_low_cfg.expert_phy_cfg.dft_processor_type);

  const auto& ru_cfg = unit_config.ru_cfg;

  flexible_o_du_ru_dependencies ru_dependencies{workers,
                                                odu_low.get_upper_ru_ul_adapter(),
                                                odu_low.get_upper_ru_timing_adapter(),
                                                odu_low.get_upper_ru_error_adapter()};

  if (const auto* cfg = std::get_if<ru_ofh_unit_parsed_config>(&ru_cfg)) {
    auto ru_ofh_dependencies = get_ru_ofh_validation_dependencies(config);
    if (!validate_ru_ofh_config(cfg->config, ru_ofh_dependencies)) {
      return nullptr;
    }

    return create_ofh_radio_unit(
        cfg->config, ru_config, ru_dependencies, unit_config.du_low_cfg.hal_config.has_value());
  }

  if (const auto* cfg = std::get_if<ru_sdr_unit_config>(&ru_cfg)) {
    ru_sdr_unit_config updated_srate_config = *cfg;
    auto               ru_sdr_dependencies  = get_ru_sdr_validation_dependencies(config);

    // Derive the sampling rate from the cell configuration unless the user configured it.
    if (unit_config.derive_sdr_srate) {
      std::optional<double> srate_MHz = derive_ru_sdr_srate_MHz(ru_sdr_dependencies.front(), split6_du_low::freq_range);
      if (!srate_MHz) {
        fmt::println("No standard sampling rate is valid for the cell configuration. Configure 'ru_sdr.srate'.");
        return nullptr;
      }
      updated_srate_config.srate_MHz = *srate_MHz;
    }

    // Update the start time of the radio.
    updated_srate_config.start_time = start_time_calc.calculate_start_time();

    if (!validate_ru_sdr_config(updated_srate_config, ru_sdr_dependencies)) {
      return nullptr;
    }

    return create_sdr_radio_unit(updated_srate_config, ru_config, ru_dependencies);
  }

  report_error("Could not detect a valid Radio Unit configuration");
}
