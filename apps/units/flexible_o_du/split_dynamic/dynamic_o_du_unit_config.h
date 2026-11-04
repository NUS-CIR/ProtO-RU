// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_config.h"
#include "apps/units/flexible_o_du/o_du_low/du_low_config.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_config.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_config.h"
#include "apps/units/flexible_o_du/split_helpers/fapi_dummy/fapi_dummy_unit_config.h"
#include <variant>

namespace ocudu {

/// CPU affinities configuration for the cell.
struct ru_dummy_cpu_affinities_cell_unit_config {
  /// Radio Unit workers CPU affinity mask.
  os_sched_affinity_config ru_cpu_cfg = {sched_affinity_mask_types::ru, {}, sched_affinity_mask_policy::mask};
};

/// Metrics configuration.
struct ru_dummy_unit_metrics_config {
  /// Metrics configuration.
  app_helpers::metrics_config metrics_cfg;
  /// Flag that control RU metrics.
  bool enable_ru_metrics = false;
};

/// gNB app dummy Radio Unit configuration.
struct ru_dummy_unit_config {
  /// \brief DL processing delay in slots.
  ///
  /// It is the number of slots that the RU expects the downlink resource grid in advance for having enough time margin
  /// for processing.
  unsigned dl_processing_delay = 1;
  /// \brief Time scaling factor applied to the slot duration based on the subcarrier spacing.
  ///
  /// This attribute adjusts the slot duration by scaling it:
  ///
  /// - A value below one shortens the slot duration.
  /// - A value above one increases the slot duration.
  /// - A value of one maintains the default slot duration.
  ///
  /// The default value of 1.0 corresponds to the standard slot duration with no scaling applied.
  float time_scaling = 1.0F;
  /// \brief CPU affinities per cell of the SDR Radio Unit.
  ///
  /// \note Add one cell by default.
  std::vector<ru_dummy_cpu_affinities_cell_unit_config> cell_affinities = {{}};
  /// Metrics configuration.
  ru_dummy_unit_metrics_config metrics_cfg;
};

/// Dynamic O-RAN DU low and Radio Unit configuration.
struct dynamic_o_du_low {
  /// DU low configuration.
  du_low_unit_config du_low_cfg;
  /// Radio Unit configuration.
  std::variant<ru_sdr_unit_config, ru_ofh_unit_parsed_config, ru_dummy_unit_config> ru_cfg = {ru_sdr_unit_config{}};
};

/// Dynamic O-RAN DU unit configuration.
struct dynamic_o_du_unit_config {
  /// O-DU high configuration.
  o_du_high_unit_config odu_high_cfg;
  /// O-DU low configuration, either the DU low with its Radio Unit or the FAPI dummy that replaces them.
  std::variant<dynamic_o_du_low, fapi_dummy_unit_config> odu_low_cfg = {dynamic_o_du_low{}};
};

} // namespace ocudu
