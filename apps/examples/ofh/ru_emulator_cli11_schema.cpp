// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ru_emulator_cli11_schema.h"
#include "helpers.h"
#include "ru_emulator_appconfig.h"
#include "ocudu/ofh/ofh_constants.h"
#include "ocudu/support/cli11_utils.h"
#include "ocudu/support/config_parsers.h"

using namespace ocudu;

/// Validates one configured eAxC list against the fixed-size OFH containers and repositories.
static void validate_eaxc_list(span<const unsigned> eaxc_ids, const char* option_name)
{
  if (eaxc_ids.size() > ofh::MAX_NOF_SUPPORTED_EAXC) {
    throw CLI::ValidationError(option_name,
                               fmt::format("At most {} eAxC identifiers are supported", ofh::MAX_NOF_SUPPORTED_EAXC));
  }

  if (std::any_of(
          eaxc_ids.begin(), eaxc_ids.end(), [](unsigned eaxc) { return eaxc >= ofh::MAX_SUPPORTED_EAXC_ID_VALUE; })) {
    throw CLI::ValidationError(
        option_name,
        fmt::format("eAxC identifiers must be in the range [0, {}]", ofh::MAX_SUPPORTED_EAXC_ID_VALUE - 1));
  }

  std::vector<unsigned> sorted_ids(eaxc_ids.begin(), eaxc_ids.end());
  std::sort(sorted_ids.begin(), sorted_ids.end());
  if (std::adjacent_find(sorted_ids.begin(), sorted_ids.end()) != sorted_ids.end()) {
    throw CLI::ValidationError(option_name, "Duplicate eAxC identifiers are not allowed");
  }
}

/// Translates a string to the corresponding RU emulator's PRACH format.
static ru_emulator_prach_format str_to_prach_format(std::string s)
{
  std::transform(s.begin(), s.end(), s.begin(), ::toupper);

  if ("LONG" == s) {
    return ru_emulator_prach_format::LONG_F0;
  }
  if ("SHORT" == s) {
    return ru_emulator_prach_format::SHORT_B4;
  }
  return ru_emulator_prach_format::NONE;
}

/// Translates RU emulator's PRACH format to a string.
static std::string prach_format_to_str(ru_emulator_prach_format f)
{
  if (f == ru_emulator_prach_format::LONG_F0) {
    return "long";
  }
  if (f == ru_emulator_prach_format::SHORT_B4) {
    return "short";
  }
  return "";
}

static void configure_cli11_log_args(CLI::App& app, ru_emulator_log_appconfig& log_params)
{
  /// Function to check that the log level is correct.
  auto check_log_level = [](const std::string& value) -> std::string {
    if (ocudulog::str_to_basic_level(value).has_value()) {
      return {};
    }

    return fmt::format("Log level '{}' not supported. Accepted values [none,info,debug,warning,error]", value);
  };
  /// Function to convert string parameter to ocudulog level.
  auto capture_log_level_function = [](ocudulog::basic_levels& level) {
    return [&level](const std::string& value) {
      auto val = ocudulog::str_to_basic_level(value);
      level    = (val) ? val.value() : ocudulog::basic_levels::none;
    };
  };

  app.add_option("--filename", log_params.filename, "Log file output path")->capture_default_str();
  add_option_function<std::string>(app, " --level", capture_log_level_function(log_params.level), "Log level")
      ->default_str(ocudulog::basic_level_to_string(log_params.level))
      ->check(check_log_level);
}

static void configure_cli11_ru_emu_dpdk_args(CLI::App& app, std::optional<ru_emulator_dpdk_appconfig>& config)
{
  config.emplace();

  app.add_option("--eal_args", config->eal_args, "EAL configuration parameters used to initialize DPDK");
}

static void configure_cli11_sdr_args(CLI::App& app, std::optional<ru_emulator_sdr_appconfig>& sdr_config)
{
  sdr_config.emplace();
  ru_emulator_sdr_appconfig& cfg = *sdr_config;

  app.add_option("--device_driver", cfg.device_driver, "Radio device driver (uhd or zmq)")->capture_default_str();
  app.add_option("--device_args", cfg.device_arguments, "Radio device arguments")->capture_default_str();
  app.add_option("--srate", cfg.srate_MHz, "Sampling rate in MHz")->capture_default_str();
  app.add_option("--dl_freq_override",
                 cfg.dl_freq_override_Hz,
                 "Downlink centre frequency override in Hz (else "
                 "derived from the cell DL ARFCN and NR band)");
  app.add_option("--ul_freq_override",
                 cfg.ul_freq_override_Hz,
                 "Uplink centre frequency override in Hz (else "
                 "derived from the cell UL ARFCN and NR band)");
  app.add_option("--tx_gain", cfg.tx_gain_dB, "Transmit gain in dB")->capture_default_str();
  app.add_option("--rx_gain", cfg.rx_gain_dB, "Receive gain in dB")->capture_default_str();
  app.add_option("--center_freq_offset", cfg.center_freq_offset_Hz, "Centre frequency offset in Hz (RF calibration)")
      ->capture_default_str();
  app.add_option("--calibrate_clock_ppm", cfg.calibrate_clock_ppm, "Clock calibration in PPM (carrier frequency)")
      ->capture_default_str();
  app.add_option("--lo_offset", cfg.lo_offset_MHz, "LO offset in MHz (moves LO leakage out of the channel)")
      ->capture_default_str();
  app.add_option("--time_alignment_calibration",
                 cfg.time_alignment_calibration,
                 "Rx-to-Tx time-alignment calibration in samples (overrides the RF driver default)");
  app.add_option("--transmission_mode",
                 cfg.transmission_mode,
                 "Radio transmission mode "
                 "(continuous, discontinuous, same-port)")
      ->capture_default_str();
  app.add_option("--power_ramping", cfg.power_ramping_us, "Transmit power ramping time in microseconds")
      ->capture_default_str();
  app.add_option("--gain_backoff", cfg.gain_backoff_dB, "Amplitude gain back-off in dB (PAPR + DFT headroom)")
      ->capture_default_str();
  app.add_option("--power_ceiling", cfg.power_ceiling_dBFS, "Amplitude ceiling in dBFS")->capture_default_str();
  app.add_option("--enable_clipping", cfg.enable_clipping, "Enable amplitude clipping at the ceiling")
      ->capture_default_str();
  app.add_option("--otw_format", cfg.otw_format, "Over-the-wire sample format")->capture_default_str();
  app.add_option("--clock_source", cfg.clock_source, "Clock source")->capture_default_str();
  app.add_option("--sync_source", cfg.sync_source, "Synchronisation source")->capture_default_str();
  app.add_option("--max_proc_delay", cfg.max_proc_delay, "Maximum lower PHY processing delay in slots")
      ->capture_default_str();
  app.add_option("--execution_profile",
                 cfg.execution_profile,
                 "Lower-PHY baseband execution profile (auto, sequential, single, dual, triple)")
      ->capture_default_str()
      ->check(CLI::IsMember({"auto", "sequential", "single", "dual", "triple"}));
  app.add_option("--ru_cpus", cfg.ru_cpus, "CPUs the SDR radio and baseband workers are pinned to")
      ->capture_default_str();
  app.add_option("--pinning_policy", cfg.pinning_policy, "Thread pinning policy within ru_cpus (mask, round-robin)")
      ->capture_default_str()
      ->check(CLI::IsMember({"mask", "round-robin"}));
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

  add_option_function<std::string>(
      app,
      "--common_scs",
      [&config](const std::string& value) { config.common_scs = to_subcarrier_spacing(value); },
      "Cell common subcarrier spacing (15 or 30 kHz)")
      ->default_str(to_string(config.common_scs))
      ->check([](const std::string& value) -> std::string {
        const subcarrier_spacing scs = to_subcarrier_spacing(value);
        if (scs == subcarrier_spacing::kHz15 || scs == subcarrier_spacing::kHz30) {
          return {};
        }
        return fmt::format("Common subcarrier spacing '{}' not supported. Accepted values [15, 30]", value);
      });

  app.add_option("--dl_arfcn", config.dl_arfcn, "Downlink ARFCN (cell centre-frequency reference)")
      ->capture_default_str();
  app.add_option_function<unsigned>(
      "--band",
      [&config](const unsigned& value) {
        if (value != 0) {
          config.band = static_cast<nr_band>(value);
        }
      },
      "NR band number (derived from the DL ARFCN when unset)");

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
  app.add_option("--compr_method_prach", config.prach_compr_method, "PRACH compression method")
      ->capture_default_str()
      ->check(compression_method_check);
  app.add_option("--compr_bitwidth_prach", config.prach_compr_bitwidth, "PRACH compression bit width")
      ->capture_default_str()
      ->check(CLI::IsMember({9, 16}));
  app.add_option("--iq_scaling", config.iq_scaling, "IQ scaling applied before compression")->capture_default_str();
  app.add_option("--is_ul_static_compr_hdr", config.is_ul_static_compr_hdr, "Uplink static compression header flag")
      ->capture_default_str();
  app.add_option("--is_dl_static_compr_hdr", config.is_dl_static_compr_hdr, "Downlink static compression header flag")
      ->capture_default_str();
  app.add_option("--network_interface", config.network_interface, "PCIe identifier of network device")
      ->capture_default_str();
  app.add_option("--ru_mac_addr", config.ru_mac_address, "Radio Unit MAC address")->capture_default_str();
  app.add_option("--du_mac_addr", config.du_mac_address, "Distributed Unit MAC address")->capture_default_str();
  app.add_option("--vlan_tag", config.vlan_tag, "V-LAN identifier (omit to transmit untagged frames)")
      ->capture_default_str()
      ->check(CLI::Range(1, 4094));
  app.add_option("--enable_promiscuous", config.enable_promiscuous, "Promiscuous mode flag")->capture_default_str();
  app.add_option("--ofh_cpus",
                 config.ofh_cpus,
                 "CPUs the Open Fronthaul receive + decode workers (ru_rx, ru_emu) are pinned to")
      ->capture_default_str();
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
  app.add_option("--ta3_max_up", config.Ta3_max_up, "Ta3 maximum value for the uplink User-Plane (RU transmit window)")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));
  app.add_option("--ta3_min_up", config.Ta3_min_up, "Ta3 minimum value for the uplink User-Plane (RU transmit window)")
      ->capture_default_str()
      ->check(CLI::Range(0, 5000));

  // Function to capture the PRACH format.
  auto capture_prach_format_function = [](ru_emulator_prach_format& format) {
    return [&format](const std::string& value) { format = str_to_prach_format(value); };
  };

  // Function to check that the log level is correct.
  auto check_prach_format = [](const std::string& value) -> std::string {
    if (str_to_prach_format(value) != ocudu::ru_emulator_prach_format::NONE) {
      return {};
    }
    return fmt::format("PRACH format '{}' not supported. Accepted values [long,short]. Set to 'long' to use format 0, "
                       "or 'short' to use format B4",
                       value);
  };

  add_option_function<std::string>(app,
                                   "--prach_format",
                                   capture_prach_format_function(config.prach_format),
                                   "PRACH format. Set to 'long' to use format 0, or 'short' to use format B4")
      ->default_str(prach_format_to_str(config.prach_format))
      ->check(check_prach_format);

  // Optional SDR (radio) section. When present, this RU runs in SDR mode; when absent, in loopback (test-IQ) mode.
  CLI::App* sdr_subcmd = app.add_subcommand("sdr", "SDR (radio) configuration")->configurable();
  configure_cli11_sdr_args(*sdr_subcmd, config.sdr_config);

  // Clean the SDR optional if the section was not configured.
  app.callback([&app, &config]() {
    if (app.get_subcommand("sdr")->count_all() == 0) {
      config.sdr_config.reset();
    }

    validate_eaxc_list(config.ru_ul_port_id, "--ul_port_id");
    validate_eaxc_list(config.ru_dl_port_id, "--dl_port_id");
    validate_eaxc_list(config.ru_prach_port_id, "--prach_port_id");
  });
}

void ocudu::configure_cli11_with_ru_emulator_appconfig_schema(CLI::App& app, ru_emulator_appconfig& ru_emu_parsed_cfg)
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

  ru_subcmd
      ->add_option(
          "--timing_cpus", ru_emu_parsed_cfg.timing_cpus, "CPUs the shared GPS timing worker (ru_timing) is pinned to")
      ->capture_default_str();

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
