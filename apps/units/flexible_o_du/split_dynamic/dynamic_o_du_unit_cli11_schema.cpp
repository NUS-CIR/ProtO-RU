// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "dynamic_o_du_unit_cli11_schema.h"
#include "apps/helpers/metrics/metrics_config_cli11_schema.h"
#include "apps/services/worker_manager/cli11_cpu_affinities_parser_helper.h"
#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_config_cli11_schema.h"
#include "apps/units/flexible_o_du/o_du_low/du_low_config_cli11_schema.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config_cli11_schema.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_config_cli11_schema.h"
#include "dynamic_o_du_unit_config.h"
#include "ocudu/adt/format.h"
#include "ocudu/support/cli11_utils.h"
#include "ocudu/support/config_parsers.h"

using namespace ocudu;

static ru_ofh_unit_parsed_config ofh_cfg;
static ru_sdr_unit_config        sdr_cfg;
static ru_dummy_unit_config      dummy_cfg;
static fapi_dummy_unit_config    fapi_dummy_parsed_cfg;

static void configure_cli11_fapi_dummy_args(CLI::App& app, fapi_dummy_unit_config& config)
{
  static const std::map<std::string, fapi_adaptor::phy_dummy_fapi_adaptor_clock_mode> clock_modes = {
      {"realtime", fapi_adaptor::phy_dummy_fapi_adaptor_clock_mode::realtime},
      {"free_running", fapi_adaptor::phy_dummy_fapi_adaptor_clock_mode::free_running}};
  add_option(app, "--clock", config.clock_mode, "Slot clock mode. Values: realtime or free_running")
      ->transform(CLI::CheckedTransformer(clock_modes));
  add_option(app,
             "--ul_indication_delay_slots",
             config.ul_indication_delay_slots,
             "Number of slots between an uplink slot and the notification of its indications")
      ->capture_default_str()
      ->check(CLI::Range(1, 32));
  add_option(app,
             "--max_processing_delay_slots",
             config.max_processing_delay_slots,
             "Number of slots a slot indication is notified in advance of the transmission time")
      ->capture_default_str()
      ->check(CLI::Range(1, 30));
}

static void configure_cli11_ru_dummy_args(CLI::App& app, ru_dummy_unit_config& config)
{
  add_option(app, "--dl_processing_delay", config.dl_processing_delay, "DL processing processing delay in slots")
      ->capture_default_str();
  add_option(app,
             "--time_scaling",
             config.time_scaling,
             "Time scaling factor applied to the slot duration. Must be greater than zero. "
             "A value greater than one slows down the RU, while a value between zero and one speeds it up.")
      ->capture_default_str()
      ->non_negative();
}

static void configure_cli11_cell_affinity_args(CLI::App& app, ru_dummy_cpu_affinities_cell_unit_config& config)
{
  add_option_function<std::string>(
      app,
      "--ru_cpus",
      [&config](const std::string& value) { parse_affinity_mask(config.ru_cpu_cfg.mask, value, "ru_cpus"); },
      "Number of CPUs used for the Radio Unit tasks");

  add_option_function<std::string>(
      app,
      "--ru_pinning",
      [&config](const std::string& value) {
        config.ru_cpu_cfg.pinning_policy = to_affinity_mask_policy(value);
        if (config.ru_cpu_cfg.pinning_policy == sched_affinity_mask_policy::last) {
          report_error("Incorrect value={} used in {} property", value, "ru_pinning");
        }
      },
      "Policy used for assigning CPU cores to the Radio Unit tasks");
}

static void configure_cli11_expert_execution_args(CLI::App& app, ru_dummy_unit_config& config)
{
  // Cell affinity section.
  add_option_object_list<ru_dummy_cpu_affinities_cell_unit_config>(
      app,
      "--cell_affinities",
      config.cell_affinities,
      configure_cli11_cell_affinity_args,
      "Sets the cell CPU affinities configuration on a per cell basis");
}

static void configure_cli11_metrics_args(CLI::App& app, ru_dummy_unit_metrics_config& config)
{
  CLI::App* layers_subcmd = add_subcommand(app, "layers", "Layer basis metrics configuration")->configurable();
  add_option(*layers_subcmd, "--enable_ru", config.enable_ru_metrics, "Enable Radio Unit metrics");
}

void ocudu::configure_cli11_with_dynamic_o_du_unit_config_schema(CLI::App& app, dynamic_o_du_unit_config& parsed_cfg)
{
  configure_cli11_with_o_du_high_config_schema(app, parsed_cfg.odu_high_cfg);
  configure_cli11_with_du_low_config_schema(app, parsed_cfg.du_low_cfg);
  configure_cli11_with_ru_ofh_config_schema(app, ofh_cfg);
  configure_cli11_with_ru_sdr_config_schema(app, sdr_cfg);

  CLI::App* ru_dummy_subcmd = add_subcommand(app, "ru_dummy", "Dummy Radio Unit configuration")->configurable();
  configure_cli11_ru_dummy_args(*ru_dummy_subcmd, dummy_cfg);

  CLI::App* fapi_dummy_subcmd =
      add_subcommand(app, "fapi_dummy", "FAPI dummy configuration, which replaces the PHY and the Radio Unit")
          ->configurable();
  configure_cli11_fapi_dummy_args(*fapi_dummy_subcmd, fapi_dummy_parsed_cfg);

  // Metrics section.
  app_helpers::configure_cli11_with_metrics_appconfig_schema(app, dummy_cfg.metrics_cfg.metrics_cfg);
  CLI::App* metrics_subcmd = add_subcommand(app, "metrics", "Metrics configuration")->configurable();
  configure_cli11_metrics_args(*metrics_subcmd, dummy_cfg.metrics_cfg);

  // Expert execution section.
  CLI::App* expert_subcmd = add_subcommand(app, "expert_execution", "Expert execution configuration")->configurable();
  configure_cli11_expert_execution_args(*expert_subcmd, dummy_cfg);
}

static void manage_ru(CLI::App& app, dynamic_o_du_unit_config& parsed_cfg)
{
  // Manage the RU optionals
  auto     ofh_subcmd        = app.get_subcommand("ru_ofh");
  auto     sdr_subcmd        = app.get_subcommand("ru_sdr");
  auto     dummy_subcmd      = app.get_subcommand("ru_dummy");
  unsigned nof_ofh_entries   = ofh_subcmd->count_all();
  unsigned nof_sdr_entries   = sdr_subcmd->count_all();
  unsigned nof_dummy_entries = dummy_subcmd->count_all();

  // Count the number of RU types.
  unsigned nof_ru_types = (nof_ofh_entries != 0) ? 1 : 0;
  nof_ru_types += (nof_sdr_entries != 0) ? 1 : 0;
  nof_ru_types += (nof_dummy_entries != 0) ? 1 : 0;

  if (nof_ru_types > 1) {
    ocudu_terminate("Radio Unit configuration allows either a SDR, Open Fronthaul, or Dummy configuration, but not "
                    "different types of them at the same time");
  }

  if (nof_ofh_entries != 0) {
    parsed_cfg.ru_cfg = ofh_cfg;
    sdr_subcmd->disabled();
    dummy_subcmd->disabled();

    return;
  }

  if (nof_sdr_entries != 0) {
    parsed_cfg.ru_cfg = sdr_cfg;
    ofh_subcmd->disabled();
    dummy_subcmd->disabled();

    return;
  }

  parsed_cfg.ru_cfg = dummy_cfg;
  sdr_subcmd->disabled();
  ofh_subcmd->disabled();
}

/// Sets the FAPI dummy configuration if its section is present, and warns about the ignored sections.
static void manage_fapi_dummy(CLI::App& app, dynamic_o_du_unit_config& parsed_cfg)
{
  if (app.get_subcommand("fapi_dummy")->count_all() == 0) {
    return;
  }

  parsed_cfg.fapi_dummy_cfg = fapi_dummy_parsed_cfg;

  // The sections that configure the PHY or the Radio Unit, which the FAPI dummy replaces.
  for (const char* section : {"ru_ofh", "ru_sdr", "ru_dummy", "expert_phy", "hal", "power_calibration"}) {
    const CLI::App* subcmd = app.get_subcommand_no_throw(section);
    if (subcmd != nullptr && subcmd->count_all() != 0) {
      fmt::println("Warning: the parameters of the '{}' section will be ignored, since the FAPI dummy replaces the PHY "
                   "and the Radio Unit",
                   section);
    }
  }
}

static bool get_is_rt_mode_enabled(const dynamic_o_du_unit_config& parsed_cfg)
{
  if (parsed_cfg.fapi_dummy_cfg.has_value()) {
    return parsed_cfg.fapi_dummy_cfg->clock_mode == fapi_adaptor::phy_dummy_fapi_adaptor_clock_mode::realtime;
  }

  const auto& ru_cfg = parsed_cfg.ru_cfg;
  if (std::holds_alternative<ru_dummy_unit_config>(ru_cfg)) {
    return true;
  }

  if (std::holds_alternative<ru_ofh_unit_parsed_config>(ru_cfg)) {
    return true;
  }

  return std::get<ru_sdr_unit_config>(ru_cfg).device_driver != "zmq";
}

void ocudu::autoderive_dynamic_o_du_parameters_after_parsing(CLI::App& app, dynamic_o_du_unit_config& parsed_cfg)
{
  const unsigned nof_cells = parsed_cfg.odu_high_cfg.du_high_cfg.config.cells_cfg.size();
  // Auto derive SDR parameters.
  autoderive_ru_sdr_parameters_after_parsing(app, sdr_cfg, nof_cells);

  // Set the parsed RU.
  manage_ru(app, parsed_cfg);
  manage_fapi_dummy(app, parsed_cfg);

  if (std::holds_alternative<ru_dummy_unit_config>(parsed_cfg.ru_cfg)) {
    auto& dummy = std::get<ru_dummy_unit_config>(parsed_cfg.ru_cfg);
    if (dummy.cell_affinities.size() < nof_cells) {
      dummy.cell_affinities.resize(nof_cells);
    }
  }

  // Derive the O-DU high parameters.
  autoderive_o_du_high_parameters_after_parsing(app, parsed_cfg.odu_high_cfg, get_is_rt_mode_enabled(parsed_cfg));

  // Auto derive DU low parameters.
  const auto&   cell = parsed_cfg.odu_high_cfg.du_high_cfg.config.cells_cfg.front().cell;
  const nr_band band = cell.band ? cell.band.value() : band_helper::get_band_from_dl_arfcn(cell.dl_f_ref_arfcn);
  autoderive_du_low_parameters_after_parsing(app, parsed_cfg.du_low_cfg, band_helper::get_duplex_mode(band));
}
