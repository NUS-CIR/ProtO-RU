// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/expected.h"
#include "ocudu/scheduler/scheduler_configurator.h"

namespace ocudu {

class cell_configuration;

namespace config_validators {

/// \brief Validates \c sched_ue_creation_request_message used to create a UE.
/// \param[in] msg scheduler ue creation request message to be validated.
/// \param[in] cell_cfg Cell configuration.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
error_type<std::string> validate_sched_ue_creation_request_message(const sched_ue_creation_request_message& msg,
                                                                   const cell_configuration&                cell_cfg);

/// \brief Validates a \c sched_ue_reconfiguration_message used to reconfigure a UE.
///
/// \param[in] msg scheduler UE reconfiguration request message to be validated.
/// \param[in] cell_cfg Cell configuration.
/// \return In case an invalid parameter is detected, returns a string containing an error message.
error_type<std::string> validate_sched_ue_reconfiguration_message(const sched_ue_reconfiguration_message& msg,
                                                                  const cell_configuration&               cell_cfg);

} // namespace config_validators
} // namespace ocudu
