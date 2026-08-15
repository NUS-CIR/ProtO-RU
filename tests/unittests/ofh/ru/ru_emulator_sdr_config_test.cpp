// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#include "../../../../apps/examples/ofh/ru_emulator_sdr.h"
#include "ocudu/ran/band_helper.h"
#include <gtest/gtest.h>

using namespace ocudu;

namespace {

/// Baseline SDR appconfig: ZMQ driver with one TX and one RX port.
ru_emulator_sdr_appconfig make_sdr_config()
{
  ru_emulator_sdr_appconfig cfg;
  cfg.device_driver    = "zmq";
  cfg.device_arguments = "tx_port=tcp://*:5000,rx_port=tcp://*:6000";
  cfg.srate_MHz        = 23.04;
  return cfg;
}

} // namespace

TEST(ru_emulator_sdr_config_test, thirty_khz_tdd_band_derives_frequencies_from_arfcn)
{
  // DL ARFCN 632628 is in band n78 (TDD): F = 3000 MHz + 0.015 MHz * (632628 - 600000) = 3489.42 MHz, UL = DL.
  const double expected_freq_Hz = 3489.42e6;

  ru_sdr_configuration out = generate_ru_emulator_sdr_config(make_sdr_config(),
                                                             subcarrier_spacing::kHz30,
                                                             bs_channel_bandwidth::MHz20,
                                                             /*dl_arfcn=*/632628,
                                                             /*band=*/std::nullopt,
                                                             /*nof_tx_antennas=*/1,
                                                             /*nof_rx_antennas=*/1);

  ASSERT_EQ(out.lower_phy_config.size(), 1);
  const lower_phy_configuration& low = out.lower_phy_config.front();
  ASSERT_EQ(low.scs, subcarrier_spacing::kHz30);
  ASSERT_EQ(
      low.bandwidth_rb,
      band_helper::get_n_rbs_from_bw(bs_channel_bandwidth::MHz20, subcarrier_spacing::kHz30, frequency_range::FR1));
  ASSERT_DOUBLE_EQ(low.dl_freq_hz, expected_freq_Hz);
  ASSERT_DOUBLE_EQ(low.ul_freq_hz, expected_freq_Hz);

  ASSERT_EQ(out.radio_cfg.tx_streams.size(), 1);
  ASSERT_EQ(out.radio_cfg.tx_streams.front().channels.size(), 1);
  ASSERT_DOUBLE_EQ(out.radio_cfg.tx_streams.front().channels.front().freq.center_frequency_Hz, expected_freq_Hz);
  ASSERT_DOUBLE_EQ(out.radio_cfg.rx_streams.front().channels.front().freq.center_frequency_Hz, expected_freq_Hz);
  // The ZMQ ports are distributed to the radio channels.
  ASSERT_EQ(out.radio_cfg.tx_streams.front().channels.front().args, "tcp://*:5000");
  ASSERT_EQ(out.radio_cfg.rx_streams.front().channels.front().args, "tcp://*:6000");
}

TEST(ru_emulator_sdr_config_test, fifteen_khz_fdd_band_derives_duplex_frequencies)
{
  // DL ARFCN 365000 is in band n3 (FDD): DL = 0.005 MHz * 365000 = 1825 MHz, UL = DL - 95 MHz = 1730 MHz.
  ru_sdr_configuration out = generate_ru_emulator_sdr_config(make_sdr_config(),
                                                             subcarrier_spacing::kHz15,
                                                             bs_channel_bandwidth::MHz10,
                                                             /*dl_arfcn=*/365000,
                                                             /*band=*/std::nullopt,
                                                             /*nof_tx_antennas=*/1,
                                                             /*nof_rx_antennas=*/1);

  const lower_phy_configuration& low = out.lower_phy_config.front();
  ASSERT_EQ(low.scs, subcarrier_spacing::kHz15);
  ASSERT_EQ(
      low.bandwidth_rb,
      band_helper::get_n_rbs_from_bw(bs_channel_bandwidth::MHz10, subcarrier_spacing::kHz15, frequency_range::FR1));
  ASSERT_DOUBLE_EQ(low.dl_freq_hz, 1825e6);
  ASSERT_DOUBLE_EQ(low.ul_freq_hz, 1730e6);
  ASSERT_DOUBLE_EQ(out.radio_cfg.tx_streams.front().channels.front().freq.center_frequency_Hz, 1825e6);
  ASSERT_DOUBLE_EQ(out.radio_cfg.rx_streams.front().channels.front().freq.center_frequency_Hz, 1730e6);
}

TEST(ru_emulator_sdr_config_test, rf_calibration_applies_to_radio_but_not_lower_phy)
{
  ru_emulator_sdr_appconfig cfg = make_sdr_config();
  cfg.center_freq_offset_Hz     = 1000.0;
  cfg.calibrate_clock_ppm       = 2.0;

  const double nominal_Hz    = 3489.42e6;
  const double calibrated_Hz = (nominal_Hz + cfg.center_freq_offset_Hz) * (1.0 + cfg.calibrate_clock_ppm * 1e-6);

  ru_sdr_configuration out = generate_ru_emulator_sdr_config(cfg,
                                                             subcarrier_spacing::kHz30,
                                                             bs_channel_bandwidth::MHz20,
                                                             /*dl_arfcn=*/632628,
                                                             /*band=*/std::nullopt,
                                                             /*nof_tx_antennas=*/1,
                                                             /*nof_rx_antennas=*/1);

  // The radio tunes to the calibrated frequency while the lower PHY keeps the nominal one.
  ASSERT_DOUBLE_EQ(out.radio_cfg.tx_streams.front().channels.front().freq.center_frequency_Hz, calibrated_Hz);
  ASSERT_DOUBLE_EQ(out.lower_phy_config.front().dl_freq_hz, nominal_Hz);
}

TEST(ru_emulator_sdr_config_test, lo_offset_sets_lo_frequency)
{
  ru_emulator_sdr_appconfig cfg = make_sdr_config();
  cfg.lo_offset_MHz             = 10.0;

  ru_sdr_configuration out = generate_ru_emulator_sdr_config(cfg,
                                                             subcarrier_spacing::kHz30,
                                                             bs_channel_bandwidth::MHz20,
                                                             /*dl_arfcn=*/632628,
                                                             /*band=*/std::nullopt,
                                                             /*nof_tx_antennas=*/1,
                                                             /*nof_rx_antennas=*/1);
  ASSERT_DOUBLE_EQ(out.radio_cfg.tx_streams.front().channels.front().freq.lo_frequency_Hz, 3489.42e6 + 10e6);

  // Without an LO offset the LO frequency is left unset.
  ru_sdr_configuration out_no_lo = generate_ru_emulator_sdr_config(make_sdr_config(),
                                                                   subcarrier_spacing::kHz30,
                                                                   bs_channel_bandwidth::MHz20,
                                                                   /*dl_arfcn=*/632628,
                                                                   /*band=*/std::nullopt,
                                                                   /*nof_tx_antennas=*/1,
                                                                   /*nof_rx_antennas=*/1);
  ASSERT_DOUBLE_EQ(out_no_lo.radio_cfg.tx_streams.front().channels.front().freq.lo_frequency_Hz, 0.0);
}

TEST(ru_emulator_sdr_config_test, frequency_overrides_take_precedence_over_arfcn)
{
  ru_emulator_sdr_appconfig cfg = make_sdr_config();
  cfg.dl_freq_override_Hz       = 2400e6;
  cfg.ul_freq_override_Hz       = 2500e6;

  ru_sdr_configuration out = generate_ru_emulator_sdr_config(cfg,
                                                             subcarrier_spacing::kHz30,
                                                             bs_channel_bandwidth::MHz20,
                                                             /*dl_arfcn=*/632628,
                                                             /*band=*/std::nullopt,
                                                             /*nof_tx_antennas=*/1,
                                                             /*nof_rx_antennas=*/1);
  ASSERT_DOUBLE_EQ(out.lower_phy_config.front().dl_freq_hz, 2400e6);
  ASSERT_DOUBLE_EQ(out.lower_phy_config.front().ul_freq_hz, 2500e6);
}

TEST(ru_emulator_sdr_config_test, max_proc_delay_propagates_to_lower_phy)
{
  ru_emulator_sdr_appconfig cfg = make_sdr_config();
  cfg.max_proc_delay            = 4;

  ru_sdr_configuration out = generate_ru_emulator_sdr_config(cfg,
                                                             subcarrier_spacing::kHz30,
                                                             bs_channel_bandwidth::MHz20,
                                                             /*dl_arfcn=*/632628,
                                                             /*band=*/std::nullopt,
                                                             /*nof_tx_antennas=*/1,
                                                             /*nof_rx_antennas=*/1);
  ASSERT_EQ(out.lower_phy_config.front().max_processing_delay_slots, 4);
  // One extra slot for sample collection and one for processing delay.
  ASSERT_EQ(out.lower_phy_config.front().max_nof_prach_concurrent_requests, 6);
}

TEST(ru_emulator_sdr_config_test, sfn0_start_time_is_anchored_only_on_real_radios)
{
  // ZMQ has no radio clock: the start time stays unset (free-running).
  ru_sdr_configuration zmq_out = generate_ru_emulator_sdr_config(make_sdr_config(),
                                                                 subcarrier_spacing::kHz30,
                                                                 bs_channel_bandwidth::MHz20,
                                                                 /*dl_arfcn=*/632628,
                                                                 /*band=*/std::nullopt,
                                                                 /*nof_tx_antennas=*/1,
                                                                 /*nof_rx_antennas=*/1);
  ASSERT_FALSE(zmq_out.start_time.has_value());

  // A real radio anchors SFN0 to a near-future whole-second boundary.
  ru_emulator_sdr_appconfig uhd_cfg = make_sdr_config();
  uhd_cfg.device_driver             = "uhd";
  uhd_cfg.device_arguments          = "";

  ru_sdr_configuration uhd_out = generate_ru_emulator_sdr_config(uhd_cfg,
                                                                 subcarrier_spacing::kHz30,
                                                                 bs_channel_bandwidth::MHz20,
                                                                 /*dl_arfcn=*/632628,
                                                                 /*band=*/std::nullopt,
                                                                 /*nof_tx_antennas=*/1,
                                                                 /*nof_rx_antennas=*/1);
  ASSERT_TRUE(uhd_out.start_time.has_value());
  ASSERT_GT(*uhd_out.start_time, std::chrono::system_clock::now());
}
