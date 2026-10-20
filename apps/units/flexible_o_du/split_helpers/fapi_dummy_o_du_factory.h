// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "apps/units/flexible_o_du/o_du_unit.h"
#include "fapi_dummy_unit_config.h"

namespace ocudu {

struct o_du_high_unit_config;

/// Creates an O-RAN DU whose O-DU high is connected to a FAPI dummy instead of a PHY and a Radio Unit.
o_du_unit create_fapi_dummy_o_du(const o_du_high_unit_config&  odu_high_cfg,
                                 const fapi_dummy_unit_config& fapi_dummy_cfg,
                                 const o_du_unit_dependencies& dependencies);

} // namespace ocudu
