// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/expected.h"
#include "ocudu/phy/upper/equalization/channel_equalizer_algorithm_type.h"
#include "ocudu/phy/upper/signal_processors/channel_estimator/port_channel_estimator_parameters.h"
#include "ocudu/ran/dmrs/dmrs.h"
#include "ocudu/ran/pdsch/pdsch_mcs.h"
#include "ocudu/ran/sch/sch_mcs.h"
#include <string>
#include <thread>
#include <vector>

namespace ocudu {

/// Configuration parameters parsed from CLI11.
struct pxsch_bler_test_configuration {
  unsigned                                         max_nof_threads = std::min(8U, std::thread::hardware_concurrency());
  bool                                             show_stats      = true;
  unsigned                                         nof_repetitions = 1000;
  std::string                                      channel_delay_profile            = "single-tap";
  std::string                                      channel_fading_distribution      = "uniform-phase";
  float                                            sinr_dB                          = 60.0F;
  float                                            cfo_Hz                           = 0.0F;
  float                                            doppler_Hz                       = 0.0F;
  std::string                                      mimo_correlation                 = "low";
  unsigned                                         nof_corrupted_re_per_ofdm_symbol = 0;
  unsigned                                         nof_rx_ports                     = 2;
  unsigned                                         nof_layers                       = 1;
  unsigned                                         bwp_size_rb                      = 273;
  pdsch_mcs_table                                  mcs_table                        = pdsch_mcs_table::qam64;
  sch_mcs_index                                    mcs_index                        = 20;
  bool                                             enable_dc_position               = false;
  std::string                                      pxsch_type                       = "auto";
  port_channel_estimator_td_interpolation_strategy td_interpolation_strategy =
      port_channel_estimator_td_interpolation_strategy::average;
  port_channel_estimator_fd_smoothing_strategy fd_smoothing_strategy =
      port_channel_estimator_fd_smoothing_strategy::filter;
  channel_equalizer_algorithm_type equalizer_algorithm = channel_equalizer_algorithm_type::mmse;
  dmrs_additional_positions        dmrs_additional_pos = dmrs_additional_positions::pos2;
  unsigned                         nof_ldpc_iterations = 10;
  unsigned                         ul_slot_offset      = 0;
  std::vector<unsigned>            rep_rv_sequence     = {0};
  std::string                      log_level           = "warning";
};

/// \brief Parse CLI11 arguments into a \c pxsch_bler_test_configuration.
///
/// \return The PxSCH BLER test configuration if the parsing succeeds, otherwise the process exit code (zero if the help
/// was requested).
expected<pxsch_bler_test_configuration, int> parse_configuration(int argc, char** argv);

} // namespace ocudu
