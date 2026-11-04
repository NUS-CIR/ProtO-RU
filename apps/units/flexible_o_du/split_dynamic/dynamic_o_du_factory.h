// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "apps/units/flexible_o_du/split_dynamic/dynamic_o_du_unit_config.h"
#include "apps/units/flexible_o_du/split_helpers/flexible_o_du_factory.h"

namespace ocudu {

/// Dynamic O-RAN DU factory.
class dynamic_o_du_factory : public flexible_o_du_factory
{
  const dynamic_o_du_low& odu_low_cfg;

public:
  dynamic_o_du_factory(const o_du_high_unit_config& odu_high_cfg, const dynamic_o_du_low& odu_low_cfg_);

private:
  std::unique_ptr<radio_unit> create_radio_unit(const flexible_o_du_ru_config&       ru_config,
                                                const flexible_o_du_ru_dependencies& ru_dependencies) override;
};

} // namespace ocudu
