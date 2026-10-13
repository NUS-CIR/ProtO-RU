// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ru_sdr_config.h"
#include "ocudu/ran/bs_channel_bandwidth.h"
#include "ocudu/ran/duplex_mode.h"
#include "ocudu/ran/prach/prach_format_type.h"
#include "ocudu/ran/prach/prach_preamble_information.h"
#include "ocudu/ran/subcarrier_spacing.h"

namespace ocudu {

/// SDR cell configuration for validation.
struct ru_sdr_cell_validation_config {
  /// Cell subcarrier spacing.
  subcarrier_spacing common_scs;
  /// PRACH format.
  prach_format_type prach_format;
  /// PRACH preamble information.
  prach_preamble_information preamble_info;
  /// Channel bandwidth in MHz.
  bs_channel_bandwidth channel_bw_mhz;
  /// Duplex mode.
  duplex_mode dplx_mode;
  /// Number of transmit antennas.
  unsigned nof_antennas_dl;
  /// Number of receive antennas.
  unsigned nof_antennas_ul;
};

/// Validates the given SDR Radio Unit configuration. Returns true on success, false otherwise.
bool validate_ru_sdr_config(const ru_sdr_unit_config& config, span<const ru_sdr_cell_validation_config> cell_config);

} // namespace ocudu
