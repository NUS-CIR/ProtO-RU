// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ru_emulator_appconfig.h"
#include "ocudu/ran/nr_band.h"
#include "ocudu/ran/subcarrier_spacing.h"
#include "ocudu/ru/sdr/ru_sdr_configuration.h"
#include <optional>

namespace ocudu {

/// \brief Builds the SDR Radio Unit configuration for one emulated RU (single sector).
///
/// Mirrors the O-DU's split-8 RU config translator (apps/units/flexible_o_du/split_8): it maps the emulator SDR
/// configuration into the radio and lower PHY configurations the in-tree \c create_sdr_ru consumes. DL/UL centre
/// frequencies are derived from \c dl_arfcn and \c band (the O-DU way) unless overridden in \c sdr_cfg.
/// The number of radio TX/RX antenna ports comes from \c nof_tx_antennas / \c nof_rx_antennas, which the caller derives
/// from the cell's downlink / uplink eAxC counts.
ru_sdr_configuration generate_ru_emulator_sdr_config(const ru_emulator_sdr_appconfig& sdr_cfg,
                                                     subcarrier_spacing               scs,
                                                     bs_channel_bandwidth             bandwidth,
                                                     unsigned                         dl_arfcn,
                                                     std::optional<nr_band>           band,
                                                     unsigned                         nof_tx_antennas,
                                                     unsigned                         nof_rx_antennas);

} // namespace ocudu
