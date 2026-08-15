// SPDX-FileCopyrightText: Copyright (C) 2026 National University of Singapore
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI

#pragma once

#include "yaml-cpp/yaml.h"

namespace ocudu {

struct ru_emulator_appconfig;

/// \brief Fills the given YAML node with the RU emulator application configuration.
///
/// The emitted document uses the same keys the configuration parser accepts, so it can be written back to a file and
/// replayed with -c.
void fill_ru_emulator_appconfig_in_yaml_schema(YAML::Node& node, const ru_emulator_appconfig& config);

} // namespace ocudu
