// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ru_emulator_sdr.h"
#include "ocudu/phy/lower/lower_phy_configuration.h"
#include "ocudu/phy/lower/sampling_rate.h"
#include "ocudu/ran/band_helper.h"
#include "ocudu/ran/cyclic_prefix.h"
#include "ocudu/ran/resource_block.h"
#include "ocudu/support/error_handling.h"
#include <chrono>
#include <cmath>
#include <sstream>

using namespace ocudu;

/// Applies the centre-frequency offset and clock PPM calibration to a frequency, like the split-8 RU translator.
static double calibrate_center_freq_Hz(double center_freq_Hz, double freq_offset_Hz, double calibration_ppm)
{
  return (center_freq_Hz + freq_offset_Hz) * (1.0 + calibration_ppm * 1e-6);
}

/// Splits the given driver arguments string by ',' .
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

/// Extracts the ZMQ transmission or reception ports from the given driver arguments.
static std::vector<std::string> extract_zmq_ports(const std::string& driver_args, const std::string& port_id)
{
  std::vector<std::string> ports;
  for (const auto& arg : split_rf_driver_args(driver_args)) {
    if (arg.find(port_id) == std::string::npos) {
      continue;
    }
    auto eq = arg.find('=');
    ports.push_back(arg.substr(eq + 1));
  }
  return ports;
}

ru_sdr_configuration ocudu::generate_ru_emulator_sdr_config(const ru_emulator_sdr_appconfig& sdr_cfg,
                                                            subcarrier_spacing               scs,
                                                            bs_channel_bandwidth             bandwidth,
                                                            unsigned                         dl_arfcn,
                                                            std::optional<nr_band>           band,
                                                            unsigned                         nof_tx_antennas,
                                                            unsigned                         nof_rx_antennas)
{
  const frequency_range fr           = frequency_range::FR1;
  const unsigned        bandwidth_rb = band_helper::get_n_rbs_from_bw(bandwidth, scs, fr);
  const auto            bandwidth_sc = static_cast<float>(NOF_SUBCARRIERS_PER_RB * bandwidth_rb);

  // Derive DL/UL centre frequencies from the cell's DL ARFCN and NR band (the O-DU way), unless explicitly overridden.
  const arfcn_t dl_arfcn_ref{dl_arfcn};
  const nr_band nr_band_val  = band.value_or(band_helper::get_band_from_dl_arfcn(dl_arfcn_ref));
  const arfcn_t ul_arfcn_ref = band_helper::get_ul_arfcn_from_dl_arfcn(dl_arfcn_ref, nr_band_val);
  const double  dl_freq_Hz   = sdr_cfg.dl_freq_override_Hz.value_or(band_helper::nr_arfcn_to_freq(dl_arfcn_ref));
  const double  ul_freq_Hz   = sdr_cfg.ul_freq_override_Hz.value_or(band_helper::nr_arfcn_to_freq(ul_arfcn_ref));

  // Apply the RF calibration (centre-frequency offset + clock PPM) to the radio's actual tuning, and compute the LO
  // frequencies from the LO offset. The lower PHY keeps the nominal (uncalibrated) frequencies.
  const double dl_center_cal =
      calibrate_center_freq_Hz(dl_freq_Hz, sdr_cfg.center_freq_offset_Hz, sdr_cfg.calibrate_clock_ppm);
  const double ul_center_cal =
      calibrate_center_freq_Hz(ul_freq_Hz, sdr_cfg.center_freq_offset_Hz, sdr_cfg.calibrate_clock_ppm);
  const double dl_lo_cal = calibrate_center_freq_Hz(
      dl_freq_Hz + sdr_cfg.lo_offset_MHz * 1e6, sdr_cfg.center_freq_offset_Hz, sdr_cfg.calibrate_clock_ppm);
  const double ul_lo_cal = calibrate_center_freq_Hz(
      ul_freq_Hz + sdr_cfg.lo_offset_MHz * 1e6, sdr_cfg.center_freq_offset_Hz, sdr_cfg.calibrate_clock_ppm);
  const bool use_lo_offset = std::isnormal(sdr_cfg.lo_offset_MHz);

  ru_sdr_configuration out;
  out.are_metrics_enabled = false;
  out.device_driver       = sdr_cfg.device_driver;

  // Radio configuration.
  radio_configuration::radio& radio = out.radio_cfg;
  radio.args                        = sdr_cfg.device_arguments;
  radio.log_level                   = ocudulog::basic_levels::warning;
  radio.sampling_rate_Hz            = sdr_cfg.srate_MHz * 1e6;
  radio.otw_format                  = radio_configuration::to_otw_format(sdr_cfg.otw_format);
  radio.clock.clock                 = radio_configuration::to_clock_source(sdr_cfg.clock_source);
  radio.clock.sync                  = radio_configuration::to_clock_source(sdr_cfg.sync_source);
  radio.tx_mode                     = radio_configuration::to_transmission_mode(sdr_cfg.transmission_mode);
  radio.power_ramping_us            = sdr_cfg.power_ramping_us;

  const std::vector<std::string> zmq_tx_ports = extract_zmq_ports(sdr_cfg.device_arguments, "tx_port");
  const std::vector<std::string> zmq_rx_ports = extract_zmq_ports(sdr_cfg.device_arguments, "rx_port");

  radio_configuration::stream tx_stream;
  for (unsigned port = 0; port != nof_tx_antennas; ++port) {
    radio_configuration::channel ch = {};
    ch.freq.center_frequency_Hz     = dl_center_cal;
    ch.freq.lo_frequency_Hz         = use_lo_offset ? dl_lo_cal : 0.0;
    ch.gain_dB                      = sdr_cfg.tx_gain_dB;
    if (sdr_cfg.device_driver == "zmq") {
      if (port >= zmq_tx_ports.size()) {
        report_error("Not enough ZMQ 'tx_port' arguments for the SDR RU emulator\n");
      }
      ch.args = zmq_tx_ports[port];
    }
    tx_stream.channels.push_back(ch);
  }
  radio.tx_streams.push_back(tx_stream);

  radio_configuration::stream rx_stream;
  for (unsigned port = 0; port != nof_rx_antennas; ++port) {
    radio_configuration::channel ch = {};
    ch.freq.center_frequency_Hz     = ul_center_cal;
    ch.freq.lo_frequency_Hz         = use_lo_offset ? ul_lo_cal : 0.0;
    ch.gain_dB                      = sdr_cfg.rx_gain_dB;
    if (sdr_cfg.device_driver == "zmq") {
      if (port >= zmq_rx_ports.size()) {
        report_error("Not enough ZMQ 'rx_port' arguments for the SDR RU emulator\n");
      }
      ch.args = zmq_rx_ports[port];
    }
    rx_stream.channels.push_back(ch);
  }
  radio.rx_streams.push_back(rx_stream);

  // Lower PHY configuration (single sector).
  lower_phy_configuration low;
  low.sector_id                  = 0;
  low.scs                        = scs;
  low.cp                         = cyclic_prefix::NORMAL;
  low.bandwidth_rb               = bandwidth_rb;
  low.dl_freq_hz                 = dl_freq_Hz;
  low.ul_freq_hz                 = ul_freq_Hz;
  low.nof_tx_ports               = nof_tx_antennas;
  low.nof_rx_ports               = nof_rx_antennas;
  low.dft_window_offset          = 0.5F;
  low.max_processing_delay_slots = sdr_cfg.max_proc_delay;
  low.srate                      = sampling_rate::from_MHz(sdr_cfg.srate_MHz);
  low.ta_offset                  = band_helper::get_ta_offset(fr);
  low.time_alignment_calibration = sdr_cfg.time_alignment_calibration.value_or(0);
  low.system_time_throttling     = 0.0F;
  // One extra slot for sample collection and one for processing delay.
  low.max_nof_prach_concurrent_requests = sdr_cfg.max_proc_delay + 2;
  low.baseband_rx_buffer_size_policy    = (sdr_cfg.device_driver == "zmq")
                                              ? lower_phy_baseband_buffer_size_policy::slot
                                              : lower_phy_baseband_buffer_size_policy::single_packet;

  // Gain back-off accounting for the PAPR of the signal and the DFT power normalisation.
  low.amplitude_config.input_gain_dB   = -10.0F * std::log10(bandwidth_sc) - sdr_cfg.gain_backoff_dB;
  low.amplitude_config.ceiling_dBFS    = sdr_cfg.power_ceiling_dBFS;
  low.amplitude_config.enable_clipping = sdr_cfg.enable_clipping;
  low.amplitude_config.full_scale_lin  = 1.0F;

  if (!is_valid_lower_phy_config(low)) {
    report_error("Invalid lower PHY configuration for the SDR RU emulator\n");
  }
  out.lower_phy_config.push_back(low);

  // Anchor SFN0 to a PPS edge on real radios by starting at a near-future whole-second boundary (the SDR controller
  // reads the radio clock at that instant and aligns sfn0_ref_time to the next radio PPS). ZMQ has no clock
  // (read_current_time() == 0, which would make the controller's time_start underflow), so it stays free-running.
  if (sdr_cfg.device_driver != "zmq") {
    out.start_time =
        std::chrono::floor<std::chrono::seconds>(std::chrono::system_clock::now() + std::chrono::seconds(2));
  }

  return out;
}
