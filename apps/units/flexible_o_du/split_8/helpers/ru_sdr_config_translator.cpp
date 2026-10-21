// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ru_sdr_config_translator.h"
#include "apps/services/worker_manager/worker_manager_config.h"
#include "ru_sdr_config.h"
#include "ocudu/adt/format.h"
#include "ocudu/ran/antenna_topology.h"
#include "ocudu/ran/band_helper.h"
#include <sstream>

using namespace ocudu;

/// Generates a lower PHY configuration from the given RU and cell configurations.
static lower_phy_configuration generate_lower_phy_config(const flexible_o_du_ru_config::cell_config& config,
                                                         const ru_sdr_unit_config&                   ru_cfg,
                                                         unsigned           max_processing_delay_slot,
                                                         unsigned           sector_id,
                                                         const std::string& dft_processor_type)
{
  // Static configuration that the gnb supports.
  static constexpr cyclic_prefix cp = cyclic_prefix::NORMAL;

  lower_phy_configuration out_cfg;

  out_cfg.sector_id                  = sector_id;
  out_cfg.scs                        = config.scs;
  out_cfg.cp                         = cp;
  out_cfg.bandwidth_rb               = band_helper::get_n_rbs_from_bw(config.bw, config.scs, config.freq_range);
  out_cfg.dl_freq_hz                 = band_helper::nr_arfcn_to_freq(config.dl_arfcn);
  out_cfg.ul_freq_hz                 = band_helper::nr_arfcn_to_freq(config.ul_arfcn);
  out_cfg.tx_ant_topology            = config.tx_ant_topology;
  out_cfg.nof_rx_ports               = config.nof_rx_antennas;
  out_cfg.dft_window_offset          = 0.5F;
  out_cfg.max_processing_delay_slots = max_processing_delay_slot;
  out_cfg.srate                      = sampling_rate::from_MHz(ru_cfg.srate_MHz);
  out_cfg.ta_offset =
      band_helper::get_ta_offset(band_helper::get_band_from_dl_arfcn(config.dl_arfcn), false /* 5G SA mode */);

  if (ru_cfg.time_alignment_calibration.has_value()) {
    // Selects the user specific value.
    out_cfg.time_alignment_calibration = *ru_cfg.time_alignment_calibration;
  } else {
    // Selects a default parameter that ensures a valid time alignment in the MSG1 (PRACH).
    out_cfg.time_alignment_calibration = 0;
  }

  // Get lower PHY system time throttling.
  out_cfg.system_time_throttling = ru_cfg.expert_cfg.lphy_dl_throttling;

  // Set max concurrent PRACH requests to the max processing delay (in slots) plus 2 extra slots: one for sample
  // collection and one for potential processing delay.
  out_cfg.max_nof_prach_concurrent_requests = max_processing_delay_slot + 2;

  // Select RX buffer size policy.
  if (ru_cfg.device_driver == "zmq" || ru_cfg.device_driver == "difi") {
    out_cfg.baseband_rx_buffer_size_policy = lower_phy_baseband_buffer_size_policy::slot;
  } else if (ru_cfg.expert_execution_cfg.threads.execution_profile == lower_phy_thread_profile::single) {
    // For single executor, the same executor processes uplink and downlink. In this case, the processing is blocked
    // by the signal reception. The buffers must be smaller than a slot duration considering the downlink baseband
    // samples must arrive to the baseband device before the transmission time passes.
    out_cfg.baseband_rx_buffer_size_policy = lower_phy_baseband_buffer_size_policy::single_packet;
  } else {
    out_cfg.baseband_rx_buffer_size_policy = lower_phy_baseband_buffer_size_policy::single_packet;
  }

  out_cfg.gain_backoff_dB    = ru_cfg.amplitude_cfg.gain_backoff_dB;
  out_cfg.dft_processor_type = dft_processor_type;

  if (!is_valid_lower_phy_config(out_cfg)) {
    report_error("Invalid lower PHY configuration.\n");
  }

  return out_cfg;
}

/// Slice the given string by the ',' limiter, and returns a vector with each position containing one slice of the
/// string.
static std::vector<std::string> split_rf_driver_args(const std::string& driver_args)
{
  std::stringstream        ss(driver_args);
  std::vector<std::string> result;

  while (ss.good()) {
    std::string str;
    getline(ss, str, ',');
    if (!str.empty()) {
      result.push_back(str);
    }
  }

  return result;
}

/// Finds the values of the driver arguments named \c port_id, for example the ZMQ endpoints or the DIFI ports and
/// addresses, in the order they appear.
static std::vector<std::string> extract_stream_args(const std::string& driver_args, const std::string& port_id)
{
  std::vector<std::string> ports;

  std::vector<std::string> splitted_args = split_rf_driver_args(driver_args);
  for (const auto& arg : splitted_args) {
    auto it = arg.find(port_id);

    if (it == std::string::npos) {
      continue;
    }

    it = arg.find("=");
    ports.push_back(arg.substr(++it));
  }

  return ports;
}

static double calibrate_center_freq_Hz(double center_freq_Hz, double freq_offset_Hz, double calibration_ppm)
{
  return (center_freq_Hz + freq_offset_Hz) * (1.0 + calibration_ppm * 1e-6);
}

static void generate_radio_config(radio_configuration::radio&                      out_cfg,
                                  const ru_sdr_unit_config&                        ru_cfg,
                                  span<const flexible_o_du_ru_config::cell_config> cells)
{
  out_cfg.args             = ru_cfg.device_arguments;
  out_cfg.log_level        = ru_cfg.loggers.radio_level;
  out_cfg.sampling_rate_Hz = ru_cfg.srate_MHz * 1e6;
  out_cfg.otw_format       = radio_configuration::to_otw_format(ru_cfg.otw_format);
  out_cfg.clock.clock      = radio_configuration::to_clock_source(ru_cfg.clock_source);
  out_cfg.clock.sync       = radio_configuration::to_clock_source(ru_cfg.synch_source);
  out_cfg.tx_mode          = radio_configuration::to_transmission_mode(ru_cfg.expert_cfg.transmission_mode);
  out_cfg.power_ramping_us = ru_cfg.expert_cfg.power_ramping_time_us;

  std::vector<std::string> zmq_tx_addr = extract_stream_args(ru_cfg.device_arguments, "tx_port");
  std::vector<std::string> zmq_rx_addr = extract_stream_args(ru_cfg.device_arguments, "rx_port");

  std::vector<std::string> difi_tx_ports;
  std::vector<std::string> difi_rx_ports;
  std::vector<std::string> difi_tx_addrs;
  std::vector<std::string> difi_rx_addrs;
  if (ru_cfg.device_driver == "difi") {
    difi_tx_ports = extract_stream_args(ru_cfg.device_arguments, "tx_port");
    difi_rx_ports = extract_stream_args(ru_cfg.device_arguments, "rx_port");
    difi_tx_addrs = extract_stream_args(ru_cfg.device_arguments, "tx_addr");
    difi_rx_addrs = extract_stream_args(ru_cfg.device_arguments, "rx_addr");
  }
  // DIFI carries every antenna as a stream of its own, so the port lists hold one entry per antenna, numbered by cell
  // and then by antenna. Without a list, every stream takes the driver default port.
  unsigned difi_tx_port_index = 0;
  unsigned difi_rx_port_index = 0;

  // For each sector...
  for (unsigned sector_id = 0, e = cells.size(); sector_id != e; ++sector_id) {
    // Select cell configuration.
    const flexible_o_du_ru_config::cell_config& cell = cells[sector_id];

    // Each cell is mapped to a different stream.
    radio_configuration::stream tx_stream_config;
    radio_configuration::stream rx_stream_config;

    // Deduce center frequencies.
    double cell_tx_freq_Hz = band_helper::nr_arfcn_to_freq(cell.dl_arfcn);
    if (ru_cfg.dl_freq_override_Hz.has_value()) {
      cell_tx_freq_Hz = *ru_cfg.dl_freq_override_Hz;
    }

    double cell_rx_freq_Hz = band_helper::nr_arfcn_to_freq(cell.ul_arfcn);
    if (ru_cfg.ul_freq_override_Hz.has_value()) {
      cell_rx_freq_Hz = *ru_cfg.ul_freq_override_Hz;
    }

    // Correct actual RF center frequencies considering offset and PPM calibration.
    double center_tx_freq_cal_Hz =
        calibrate_center_freq_Hz(cell_tx_freq_Hz, ru_cfg.center_freq_offset_Hz, ru_cfg.calibrate_clock_ppm);
    double center_rx_freq_cal_Hz =
        calibrate_center_freq_Hz(cell_rx_freq_Hz, ru_cfg.center_freq_offset_Hz, ru_cfg.calibrate_clock_ppm);

    // Calculate actual LO frequencies considering LO frequency offset and the frequency correction.
    double lo_tx_freq_cal_Hz = calibrate_center_freq_Hz(
        cell_tx_freq_Hz + ru_cfg.lo_offset_MHz * 1e6, ru_cfg.center_freq_offset_Hz, ru_cfg.calibrate_clock_ppm);
    double lo_rx_freq_cal_Hz = calibrate_center_freq_Hz(
        cell_rx_freq_Hz + ru_cfg.lo_offset_MHz * 1e6, ru_cfg.center_freq_offset_Hz, ru_cfg.calibrate_clock_ppm);

    // For each DL antenna port in the cell...
    for (unsigned port_id = 0; port_id != cell.nof_tx_antennas; ++port_id) {
      // Create channel configuration and append it to the previous ones.
      radio_configuration::channel tx_ch_config = {};
      tx_ch_config.freq.center_frequency_Hz     = center_tx_freq_cal_Hz;
      if (std::isnormal(ru_cfg.lo_offset_MHz)) {
        tx_ch_config.freq.lo_frequency_Hz = lo_tx_freq_cal_Hz;
      } else {
        tx_ch_config.freq.lo_frequency_Hz = 0.0;
      }
      tx_ch_config.gain_dB = ru_cfg.tx_gain_dB;

      // Add the TX ports.
      if (ru_cfg.device_driver == "zmq") {
        if (sector_id * cell.nof_tx_antennas + port_id >= zmq_tx_addr.size()) {
          report_error("ZMQ transmission channel arguments out of bounds\n");
        }

        tx_ch_config.args = zmq_tx_addr[sector_id * cell.nof_tx_antennas + port_id];
      }
      if ((ru_cfg.device_driver == "difi") && (difi_tx_port_index < difi_tx_ports.size())) {
        tx_ch_config.args = fmt::format("port={}", difi_tx_ports[difi_tx_port_index++]);
      }
      tx_stream_config.channels.emplace_back(tx_ch_config);
    }

    // DIFI streams share one address, and each channel names its own port.
    if ((ru_cfg.device_driver == "difi") && !difi_tx_addrs.empty()) {
      tx_stream_config.args = fmt::format("ip={}", difi_tx_addrs.front());
    }
    out_cfg.tx_streams.emplace_back(tx_stream_config);

    // For each UL antenna port in the cell...
    for (unsigned port_id = 0; port_id != cell.nof_rx_antennas; ++port_id) {
      // Create channel configuration and append it to the previous ones.
      radio_configuration::channel rx_ch_config = {};
      rx_ch_config.freq.center_frequency_Hz     = center_rx_freq_cal_Hz;
      if (std::isnormal(ru_cfg.lo_offset_MHz)) {
        rx_ch_config.freq.lo_frequency_Hz = lo_rx_freq_cal_Hz;
      } else {
        rx_ch_config.freq.lo_frequency_Hz = 0.0;
      }
      rx_ch_config.gain_dB = ru_cfg.rx_gain_dB;

      // Add the RX ports.
      if (ru_cfg.device_driver == "zmq") {
        if (sector_id * cell.nof_rx_antennas + port_id >= zmq_rx_addr.size()) {
          report_error("ZMQ reception channel arguments out of bounds\n");
        }

        rx_ch_config.args = zmq_rx_addr[sector_id * cell.nof_rx_antennas + port_id];
      }
      if ((ru_cfg.device_driver == "difi") && (difi_rx_port_index < difi_rx_ports.size())) {
        rx_ch_config.args = fmt::format("port={}", difi_rx_ports[difi_rx_port_index++]);
      }
      rx_stream_config.channels.emplace_back(rx_ch_config);
    }

    if ((ru_cfg.device_driver == "difi") && !difi_rx_addrs.empty()) {
      rx_stream_config.args = fmt::format("ip={}", difi_rx_addrs.front());
    }
    out_cfg.rx_streams.emplace_back(rx_stream_config);
  }
}

ru_sdr_configuration ocudu::generate_ru_sdr_config(const ru_sdr_unit_config&                        ru_cfg,
                                                   span<const flexible_o_du_ru_config::cell_config> cells,
                                                   unsigned           max_processing_delay_slots,
                                                   const std::string& dft_processor_type)
{
  ru_sdr_configuration out_cfg;
  out_cfg.are_metrics_enabled = ru_cfg.metrics_cfg.enable_ru_metrics;
  out_cfg.device_driver       = ru_cfg.device_driver;
  out_cfg.start_time          = ru_cfg.start_time;
  generate_radio_config(out_cfg.radio_cfg, ru_cfg, cells);

  unsigned sector_id = 0;
  for (const auto& cell : cells) {
    out_cfg.lower_phy_config.push_back(
        generate_lower_phy_config(cell, ru_cfg, max_processing_delay_slots, sector_id++, dft_processor_type));
  }

  return out_cfg;
}

void ocudu::fill_sdr_worker_manager_config(worker_manager_config& config, const ru_sdr_unit_config& ru_cfg)
{
  unsigned nof_cells = ru_cfg.expert_execution_cfg.cell_affinities.size();

  // The ZMQ driver requires sequential lower PHY execution to gurantee the order of the slot processing.
  worker_manager_config::ru_sdr_config::lower_phy_thread_profile thread_profile =
      (ru_cfg.device_driver != "zmq") ? static_cast<worker_manager_config::ru_sdr_config::lower_phy_thread_profile>(
                                            ru_cfg.expert_execution_cfg.threads.execution_profile)
                                      : worker_manager_config::ru_sdr_config::lower_phy_thread_profile::sequential;

  config.ru_sdr_cfg.emplace(
      worker_manager_config::ru_sdr_config{.profile                 = thread_profile,
                                           .nof_cells               = nof_cells,
                                           .executor_tracing_enable = ru_cfg.tracer.executor_tracing_enable});

  ocudu_assert(config.config_affinities.size() == ru_cfg.expert_execution_cfg.cell_affinities.size(),
               "Invalid number of cell affinities");

  for (unsigned i = 0; i != nof_cells; ++i) {
    config.config_affinities[i].push_back(ru_cfg.expert_execution_cfg.cell_affinities[i].ru_cpu_cfg);
  }
}
