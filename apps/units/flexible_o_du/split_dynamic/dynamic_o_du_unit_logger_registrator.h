// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "apps/units/flexible_o_du/o_du_high/o_du_high_unit_logger_registrator.h"
#include "apps/units/flexible_o_du/o_du_low/du_low_logger_registrator.h"
#include "apps/units/flexible_o_du/split_7_2/helpers/ru_ofh_logger_registrator.h"
#include "apps/units/flexible_o_du/split_8/helpers/ru_sdr_logger_registrator.h"
#include "dynamic_o_du_unit_config.h"

namespace ocudu {

/// Registers all the loggers for the O-RAN DU split dynamic.
inline void register_dynamic_o_du_loggers(const dynamic_o_du_unit_config& config)
{
  register_o_du_high_loggers(config.odu_high_cfg);

  const auto* odu_low = std::get_if<dynamic_o_du_low>(&config.odu_low_cfg);
  if (odu_low == nullptr) {
    return;
  }

  register_du_low_loggers(odu_low->du_low_cfg.loggers);

  if (const auto* ru_sdr = std::get_if<ru_sdr_unit_config>(&odu_low->ru_cfg)) {
    register_ru_sdr_logs(ru_sdr->loggers);
  } else if (const auto* ru_ofh = std::get_if<ru_ofh_unit_parsed_config>(&odu_low->ru_cfg)) {
    register_ru_ofh_loggers(ru_ofh->config.loggers);
  }
}

} // namespace ocudu
