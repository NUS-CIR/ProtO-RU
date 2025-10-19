/*
 *
 * Copyright 2021-2025 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include "ru_emulator_cli11_schema.h"
#include "helpers.h"
#include "ru_emulator_appconfig.h"
#include "apps/services/worker_manager/cli11_cpu_affinities_parser_helper.h"
#include "srsran/adt/expected.h"
#include "srsran/support/error_handling.h"
#include "srsran/support/cli11_utils.h"
#include "srsran/support/config_parsers.h"

using namespace srsran;

static void configure_cli11_log_args(CLI::App& app, ru_emulator_log_appconfig& log_params)
{
  /// Function to check that the log level is correct.
  auto check_log_level = [](const std::string& value) -> std::string {
    if (srslog::str_to_basic_level(value).has_value()) {
      return {};
    }

    return fmt::format("Log level '{}' not supported. Accepted values [none,info,debug,warning,error]", value);
  };
  /// Function to convert string parameter to srslog level.
  auto capture_log_level_function = [](srslog::basic_levels& level) {
    return [&level](const std::string& value) {
      auto val = srslog::str_to_basic_level(value);
      level    = (val) ? val.value() : srslog::basic_levels::none;
    };
  };

  app.add_option("--filename", log_params.filename, "Log file output path")->capture_default_str();
  add_option_function<std::string>(app, " --level", capture_log_level_function(log_params.level), "Log level")
      ->default_str(srslog::basic_level_to_string(log_params.level))
      ->check(check_log_level);
}

static void configure_cli11_lower_phy_threads_args(CLI::App& app, lower_phy_thread_profile& execution_profile)
{
  add_option_function<std::string>(
      app,
      "--execution_profile",
      [&execution_profile](const std::string& value) {
        if (value == "single") {
          execution_profile = lower_phy_thread_profile::single;
        } else if (value == "dual") {
          execution_profile = lower_phy_thread_profile::dual;
        } else if (value == "quad") {
          execution_profile = lower_phy_thread_profile::quad;
        }
      },
      "Lower physical layer executor profile [single, dual, quad].")
      ->check([](const std::string& value) -> std::string {
        if ((value == "single") || (value == "dual") || (value == "quad")) {
          return "";
        }

        return "Invalid executor profile. Valid profiles are: single, dual and quad.";
      })
      ->default_function([&execution_profile]() -> std::string {
        switch (execution_profile) {
          case lower_phy_thread_profile::blocking:
            return "blocking";
          case lower_phy_thread_profile::dual:
            return "dual";
          case lower_phy_thread_profile::quad:
            return "quad";
          case lower_phy_thread_profile::single:
            return "single";
          default:
            break;
        }
        return {};
      })
      ->capture_default_str();
}

static void configure_cli11_non_rt_threads_args(CLI::App& app, non_rt_threads_appconfig& config)
{
  add_option(app,
             "--nof_non_rt_threads",
             config.nof_non_rt_threads,
             "Number of non real time threads for processing of CP and UP data in upper layers.")
      ->capture_default_str()
      ->check(CLI::Number);
  add_option(app, "--non_rt_task_queue_size", config.non_rt_task_queue_size, "Non real time task worker queue size.")
      ->capture_default_str()
      ->check(CLI::Number);
}

static void configure_cli11_cpu_affinities_args(CLI::App& app, cpu_affinities_appconfig& config)
{
  auto parsing_isolated_cpus_fcn = [](std::optional<os_sched_affinity_bitmask>& isolated_cpu_cfg,
                                      const std::string&                        value,
                                      const std::string&                        property_name) {
    isolated_cpu_cfg.emplace();
    parse_affinity_mask(*isolated_cpu_cfg, value, property_name);

    if (isolated_cpu_cfg->all()) {
      report_error("Error in '{}' property: can not assign all available CPUs to the application", property_name);
    }
  };

  add_option_function<std::string>(
      app,
      "--isolated_cpus",
      [&config, &parsing_isolated_cpus_fcn](const std::string& value) {
        parsing_isolated_cpus_fcn(config.isolated_cpus, value, "isolated_cpus");
      },
      "CPU cores isolated for application");

  add_option_function<std::string>(
      app,
      "--low_priority_cpus",
      [&config](const std::string& value) {
        parse_affinity_mask(config.low_priority_cpu_cfg.mask, value, "low_priority_cpus");
      },
      "CPU cores assigned to low priority tasks");

  add_option_function<std::string>(
      app,
      "--low_priority_pinning",
      [&config](const std::string& value) {
        config.low_priority_cpu_cfg.pinning_policy = to_affinity_mask_policy(value);
        if (config.low_priority_cpu_cfg.pinning_policy == sched_affinity_mask_policy::last) {
          report_error("Incorrect value={} used in {} property", value, "low_priority_pinning");
        }
      },
      "Policy used for assigning CPU cores to low priority tasks");
}

static void configure_cli11_cell_affinity_args(CLI::App& app, ru_sdr_unit_cpu_affinities_cell_config& config)
{
  add_option_function<std::string>(
      app,
      "--l1_dl_cpus",
      [&config](const std::string& value) { parse_affinity_mask(config.l1_dl_cpu_cfg.mask, value, "l1_dl_cpus"); },
      "CPU cores assigned to L1 downlink tasks");

  add_option_function<std::string>(
      app,
      "--l1_ul_cpus",
      [&config](const std::string& value) { parse_affinity_mask(config.l1_ul_cpu_cfg.mask, value, "l1_ul_cpus"); },
      "CPU cores assigned to L1 uplink tasks");

  add_option_function<std::string>(
      app,
      "--l1_dl_pinning",
      [&config](const std::string& value) {
        config.l1_dl_cpu_cfg.pinning_policy = to_affinity_mask_policy(value);
        if (config.l1_dl_cpu_cfg.pinning_policy == sched_affinity_mask_policy::last) {
          report_error("Incorrect value={} used in {} property", value, "l1_dl_pinning");
        }
      },
      "Policy used for assigning CPU cores to L1 downlink tasks");

  add_option_function<std::string>(
      app,
      "--l1_ul_pinning",
      [&config](const std::string& value) {
        config.l1_ul_cpu_cfg.pinning_policy = to_affinity_mask_policy(value);
        if (config.l1_ul_cpu_cfg.pinning_policy == sched_affinity_mask_policy::last) {
          report_error("Incorrect value={} used in {} property", value, "l1_ul_pinning");
        }
      },
      "Policy used for assigning CPU cores to L1 uplink tasks");

  add_option_function<std::string>(
      app,
      "--ru_cpus",
      [&config](const std::string& value) { parse_affinity_mask(config.ru_cpu_cfg.mask, value, "ru_cpus"); },
      "Number of CPUs used for the Radio Unit tasks");

  app.add_option_function<std::string>(
      "--ru_pinning",
      [&config](const std::string& value) {
        config.ru_cpu_cfg.pinning_policy = to_affinity_mask_policy(value);
        if (config.ru_cpu_cfg.pinning_policy == sched_affinity_mask_policy::last) {
          report_error("Incorrect value={} used in {} property", value, "ru_pinning");
        }
      },
      "Policy used for assigning CPU cores to the Radio Unit tasks");
  
  add_option_function<std::string>(
      app,
      "--ofh_rx_cpus",
      [&config](const std::string& value) { parse_affinity_mask(config.rx_cpu_cfg.mask, value, "ofh_rx_cpus"); },
      "Number of CPUs used for the ofh reception tasks");

  app.add_option_function<std::string>(
      "--ofh_rx_pinning",
      [&config](const std::string& value) {
        config.rx_cpu_cfg.pinning_policy = to_affinity_mask_policy(value);
        if (config.rx_cpu_cfg.pinning_policy == sched_affinity_mask_policy::last) {
          report_error("Incorrect value={} used in {} property", value, "ofh_rx_pinning");
        }
      },
      "Policy used for assigning CPU cores to the ofh reception tasks");
  add_option_function<std::string>(
      app,
      "--timing_cpus",
      [&config](const std::string& value) { parse_affinity_mask(config.timing_cpu_cfg.mask, value, "timing_cpus"); },
      "Number of CPUs used for timing tasks");

  app.add_option_function<std::string>(
      "--timing_pinning",
      [&config](const std::string& value) {
        config.timing_cpu_cfg.pinning_policy = to_affinity_mask_policy(value);
        if (config.timing_cpu_cfg.pinning_policy == sched_affinity_mask_policy::last) {
          report_error("Incorrect value={} used in {} property", value, "timing_pinning");
        }
      },
      "Policy used for assigning CPU cores to timing tasks");
}

static void configure_cli11_expert_execution_args(CLI::App& app, ru_sdr_unit_expert_execution_config& config)
{
  // Affinity section.
  CLI::App* affinities_subcmd =
      add_subcommand(app, "affinities", "Application CPU affinities configuration")->configurable();
  configure_cli11_cpu_affinities_args(*affinities_subcmd, config.affinities);

  // Threads section.
  CLI::App* threads_subcmd = add_subcommand(app, "threads", "Threads configuration")->configurable();

  // Lower PHY threads.
  CLI::App* lower_phy_threads_subcmd =
      add_subcommand(*threads_subcmd, "lower_phy", "Lower PHY thread configuration")->configurable();
  configure_cli11_lower_phy_threads_args(*lower_phy_threads_subcmd, config.threads.execution_profile);

  // Non real time threads.
  CLI::App* non_rt_threads_subcmd =
      add_subcommand(*threads_subcmd, "non_rt", "Non real time thread configuration")->configurable();
  configure_cli11_non_rt_threads_args(*non_rt_threads_subcmd, config.threads.non_rt_threads);

  // Cell affinity section.
  add_option_cell(
      app,
      "--cell_affinities",
      [&config](const std::vector<std::string>& values) {
        config.cell_affinities.resize(values.size());

        for (unsigned i = 0, e = values.size(); i != e; ++i) {
          CLI::App subapp("SDR Expert execution cell CPU affinities",
                          "SDR Expert execution cell CPU affinities config, item #" + std::to_string(i));
          subapp.config_formatter(create_yaml_config_parser());
          subapp.allow_config_extras();
          configure_cli11_cell_affinity_args(subapp, config.cell_affinities[i]);
          std::istringstream ss(values[i]);
          subapp.parse_from_stream(ss);
        }
      },
      "Sets the cell CPU affinities configuration on a per cell basis");
}

static void configure_cli11_amplitude_control_args(CLI::App& app, amplitude_control_unit_config& amplitude_params)
{
  add_option(app,
             "--tx_gain_backoff",
             amplitude_params.gain_backoff_dB,
             "Gain back-off to accommodate the signal PAPR in decibels")
      ->capture_default_str();
  add_option(app, "--enable_clipping", amplitude_params.enable_clipping, "Signal clipping")->capture_default_str();
  add_option(app, "--ceiling", amplitude_params.power_ceiling_dBFS, "Clipping ceiling referenced to full scale")
      ->capture_default_str();
}

static void configure_cli11_ru_sdr_expert_args(CLI::App& app, ru_sdr_unit_expert_config& config)
{
  auto buffer_size_policy_check = [](const std::string& value) -> std::string {
    if (value == "auto" || value == "single-packet" || value == "half-slot" || value == "slot" ||
        value == "optimal-slot") {
      return {};
    }
    return "Invalid DL buffer size policy. Accepted values [auto,single-packet,half-slot,slot,optimal-slot]";
  };

  auto tx_mode_check = [](const std::string& value) -> std::string {
    if (value == "continuous" || value == "discontinuous" || value == "same-port") {
      return {};
    }
    return "Invalid transmission mode. Accepted values [continuous,discontinuous,same-port]";
  };

  add_option(app,
             "--low_phy_dl_throttling",
             config.lphy_dl_throttling,
             "Throttles the lower PHY DL baseband generation. The range is (0, 1). Set it to zero to disable it.")
      ->capture_default_str();
  add_option(app,
             "--tx_mode",
             config.transmission_mode,
             "Selects a radio transmission mode. Discontinuous modes are not supported by all radios.\n"
             "  continuous:    the TX chain is always active.\n"
             "  discontinuous: the transmitter stops when there is no data to transmit.\n"
             "  same-port:     the radio transmits and receives from the same antenna port.\n")
      ->capture_default_str()
      ->check(tx_mode_check);

  add_option(app,
             "--power_ramping_time_us",
             config.power_ramping_time_us,
             "Specifies the power ramping time in microseconds, it proactively initiates the transmission and "
             "mitigates transient effects.")
      ->capture_default_str();
  add_option(app,
             "--dl_buffer_size_policy",
             config.dl_buffer_size_policy,
             "Selects the size policy of the baseband buffers that pass DL samples from the lower PHY to the radio.")
      ->capture_default_str()
      ->check(buffer_size_policy_check);
}

static void configure_cli11_ru_sdr_args(CLI::App& app, ru_sdr_unit_config& config)
{
  add_option(app, "--srate", config.srate_MHz, "Sample rate in MHz")->capture_default_str();
  add_option(app, "--device_driver", config.device_driver, "Device driver name")->capture_default_str();
  add_option(app, "--device_args", config.device_arguments, "Optional device arguments")->capture_default_str();
  add_option(app, "--tx_gain", config.tx_gain_dB, "Transmit gain in decibels")->capture_default_str();
  add_option(app, "--rx_gain", config.rx_gain_dB, "Receive gain in decibels")->capture_default_str();
  add_option(app, "--freq_offset", config.center_freq_offset_Hz, "Center frequency offset in hertz")
      ->capture_default_str();
  add_option(app, "--clock_ppm", config.calibrate_clock_ppm, "Clock calibration in PPM.")->capture_default_str();
  add_option(app, "--lo_offset", config.lo_offset_MHz, "LO frequency offset in MHz")->capture_default_str();
  add_option(app, "--clock", config.clock_source, "Clock source")->capture_default_str();
  add_option(app, "--sync", config.synch_source, "Time synchronization source")->capture_default_str();
  add_option(app, "--otw_format", config.otw_format, "Over-the-wire format")->capture_default_str();
  add_option_function<std::string>(
      app,
      "--time_alignment_calibration",
      [&config](const std::string& value) {
        if (!value.empty() && value != "auto") {
          std::stringstream ss(value);
          int               ta_samples;
          ss >> ta_samples;
          config.time_alignment_calibration = ta_samples;
        }
      },
      "Rx to Tx radio time alignment calibration in samples.\n"
      "Positive values reduce the RF transmission delay with respect\n"
      "to the RF reception, while negative values increase it")
      ->check([](const std::string& value) -> std::string {
        // Check for valid option "auto".
        if (value == "auto") {
          return "";
        }

        // Check for a valid integer number;
        CLI::TypeValidator<int> IntegerValidator("INTEGER");
        return IntegerValidator(value);
      })
      ->default_str("auto");

  // Amplitude control configuration.
  CLI::App* amplitude_control_subcmd = add_subcommand(app, "amplitude_control", "Amplitude control parameters");
  configure_cli11_amplitude_control_args(*amplitude_control_subcmd, config.amplitude_cfg);

  // Expert configuration.
  CLI::App* expert_subcmd = add_subcommand(app, "expert_cfg", "Generic Radio Unit expert configuration");
  configure_cli11_ru_sdr_expert_args(*expert_subcmd, config.expert_cfg);
}

static void configure_cli11_ru_emu_dpdk_args(CLI::App& app, std::optional<ru_emulator_dpdk_appconfig>& config)
{
  config.emplace();

  app.add_option("--eal_args", config->eal_args, "EAL configuration parameters used to initialize DPDK");
}

static void configure_cli11_ru_emu_args(CLI::App& app, ru_emulator_ofh_appconfig& config)
{
  app.add_option_function<unsigned>(
         "--bandwidth",
         [&config](unsigned value) { config.bandwidth = MHz_to_bs_channel_bandwidth(value); },
         "Channel bandwidth in MHz")
      ->check([](const std::string& value) -> std::string {
        std::stringstream ss(value);
        unsigned          bw;
        ss >> bw;
        const std::string& error_message = "Error in the channel bandwidth property. Valid values "
                                           "[5,10,15,20,25,30,40,50,60,70,80,90,100]";

        return is_valid_bw(bw) ? "" : error_message;
      });

  auto compression_method_check = [](const std::string& value) -> std::string {
    if (value == "none" || value == "bfp") {
      return {};
    }

    return "Compression method not supported. Accepted values [none, bfp]";
  };

  app.add_option("--compr_method_ul", config.ul_compr_method, "Uplink compression method")
      ->capture_default_str()
      ->check(compression_method_check);
  app.add_option("--compr_bitwidth_ul", config.ul_compr_bitwidth, "Uplink compression bit width")
      ->capture_default_str()
      ->check(CLI::IsMember({9, 16}));
  app.add_option("--compr_method_dl", config.dl_compr_method, "Downlink compression method")
      ->capture_default_str()
      ->check(compression_method_check);
  app.add_option("--compr_bitwidth_dl", config.dl_compr_bitwidth, "Downlink compression bit width")
      ->capture_default_str()
      ->check(CLI::IsMember({9, 16}));
  add_option(app,
             "--enable_ul_static_compr_hdr",
             config.is_uplink_static_comp_hdr_enabled,
             "Uplink static compression header enabled flag")
      ->capture_default_str();
  add_option(app,
             "--enable_dl_static_compr_hdr",
             config.is_downlink_static_comp_hdr_enabled,
             "Downlink static compression header enabled flag")
      ->capture_default_str();
  add_option(app, "--iq_scaling", config.iq_scaling, "IQ scaling factor")
      ->capture_default_str()
      ->check(CLI::Range(0.0, 100.0));
  app.add_option("--network_interface", config.network_interface, "PCIe identifier of network device")
      ->capture_default_str();
  app.add_option("--ru_mac_addr", config.ru_mac_address, "Radio Unit MAC address")->capture_default_str();
  app.add_option("--du_mac_addr", config.du_mac_address, "Distributed Unit MAC address")->capture_default_str();
  app.add_option("--vlan_tag", config.vlan_tag, "V-LAN identifier")->capture_default_str()->check(CLI::Range(1, 4094));
  app.add_option("--enable_promiscuous", config.enable_promiscuous, "Promiscuous mode flag")->capture_default_str();
  app.add_option("--ul_port_id", config.ru_ul_port_id, "RU uplink port identifier")->capture_default_str();
  app.add_option("--dl_port_id", config.ru_dl_port_id, "RU downlink port identifier")->capture_default_str();
  app.add_option("--prach_port_id", config.ru_prach_port_id, "RU PRACH port identifier")->capture_default_str();

  // Note: For the timing parameters, worst case is 2 slots for scs 15KHz and 14 symbols. Implementation defined.
  app.add_option("--t2a_max_cp_dl", config.T2a_max_cp_dl, "T2a maximum value for downlink Control-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  app.add_option("--t2a_min_cp_dl", config.T2a_min_cp_dl, "T2a minimum value for downlink Control-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  app.add_option("--t2a_max_cp_ul", config.T2a_max_cp_ul, "T2a maximum value for uplink Control-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  app.add_option("--t2a_min_cp_ul", config.T2a_min_cp_ul, "T2a minimum value for uplink Control-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  app.add_option("--t2a_max_up", config.T2a_max_up, "T2a maximum value for User-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  app.add_option("--t2a_min_up", config.T2a_min_up, "T2a minimum value for User-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  app.add_option("--ta3_max_up", config.Ta3_max_up, "Ta3 maximum value for User-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  app.add_option("--ta3_min_up", config.Ta3_min_up, "Ta3 minimum value for User-Plane")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));

  app.add_option("--max_proc_delay", config.max_proc_delay, "maximum processing delay in slots for low-PHY layer.")
      ->capture_default_str()
      ->check(CLI::Range(1, 30));

  add_option(app, "--dl_arfcn", config.dl_arfcn, "Downlink ARFCN")->capture_default_str();
  add_auto_enum_option(app, "--band", config.band, "NR band");
  add_option_function<std::string>(
      app,
      "--common_scs",
      [&scs = config.common_scs](const std::string& value) -> std::string {
        scs = to_subcarrier_spacing(value);
        if (scs == subcarrier_spacing::invalid) {
          return fmt::format("Invalid common subcarrier spacing '{}'", value);
        }
        return {};
      },
      "Cell common subcarrier spacing")
      ->capture_default_str();
  add_option(app, "--nof_antennas_ul", config.nof_antennas_ul, "Number of antennas in uplink")
      ->capture_default_str();
  add_option(app, "--nof_antennas_dl", config.nof_antennas_dl, "Number of antennas in downlink")
      ->capture_default_str();
}

void srsran::configure_cli11_with_ru_emulator_appconfig_schema(CLI::App& app, ru_emulator_appconfig& ru_emu_parsed_cfg)
{
  // Logging section.
  CLI::App* log_subcmd = app.add_subcommand("log", "Logging configuration")->configurable();
  configure_cli11_log_args(*log_subcmd, ru_emu_parsed_cfg.log_cfg);

  // RU emulators section.
  CLI::App* ru_subcmd =
      app.add_subcommand("ru_emu", "Open Fronthaul Radio Unit emulator configuration")->configurable();

  // Cell parameters.
  ru_subcmd->add_option_function<std::vector<std::string>>(
      "--cells",
      [&ru_emu_parsed_cfg](const std::vector<std::string>& values) {
        ru_emu_parsed_cfg.ru_cfg.resize(values.size());

        for (unsigned i = 0, e = values.size(); i != e; ++i) {
          CLI::App subapp("RU emulators");
          subapp.config_formatter(create_yaml_config_parser());
          subapp.allow_config_extras(CLI::config_extras_mode::error);
          configure_cli11_ru_emu_args(subapp, ru_emu_parsed_cfg.ru_cfg[i]);
          std::istringstream ss(values[i]);
          subapp.parse_from_stream(ss);
        }
      },
      "Sets the RU emulator configuration");

  /// RU SDR section.
  CLI::App* ru_sdr_subcmd = add_subcommand(app, "radio", "SDR Radio Unit configuration")->configurable();
  configure_cli11_ru_sdr_args(*ru_sdr_subcmd, ru_emu_parsed_cfg.sdr_unit_config);
  // Expert section.
  CLI::App* expert_subcmd = app.add_subcommand("expert_execution", "Expert execution configuration")->configurable();
  configure_cli11_expert_execution_args(*expert_subcmd, ru_emu_parsed_cfg.sdr_unit_config.expert_execution_cfg);
  // dpdk section.
  CLI::App* dpdk_subcmd = app.add_subcommand("dpdk", "DPDK configuration")->configurable();
  configure_cli11_ru_emu_dpdk_args(*dpdk_subcmd, ru_emu_parsed_cfg.dpdk_config);

  app.callback([&]() {
    // Clean the DPDK optional.
    if (app.get_subcommand("dpdk")->count_all() == 0) {
      ru_emu_parsed_cfg.dpdk_config.reset();
    }
#ifdef DPDK_FOUND
    bool uses_dpdk = ru_emu_parsed_cfg.dpdk_config.has_value();
    if (uses_dpdk && ru_emu_parsed_cfg.dpdk_config->eal_args.empty()) {
      report_error("It is mandatory to fill the EAL configuration arguments to initialize DPDK correctly");
    }
#else
    if (ru_emu_parsed_cfg.dpdk_config.has_value()) {
      report_error("Unable to use DPDK as the application was not compiled with DPDK support");
    }
#endif
  });
}
