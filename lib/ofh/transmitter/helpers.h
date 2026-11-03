// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "ocudu/ofh/transmitter/ofh_transmitter_configuration.h"
#include <algorithm>

namespace ocudu {
namespace ofh {

/// Returns the maximum value between the minimum T1a values in symbol units.
inline unsigned get_biggest_min_tx_parameter(const tx_window_timing_parameters& tx_timing_params)
{
  return std::max({tx_timing_params.sym_cp_dl_end, tx_timing_params.sym_cp_ul_end, tx_timing_params.sym_up_dl_end});
}

} // namespace ofh
} // namespace ocudu
