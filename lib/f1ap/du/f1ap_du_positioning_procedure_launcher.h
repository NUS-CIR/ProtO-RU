// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/support/async/async_task.h"

namespace ocudu::odu {

/// Starts the positioning measurement procedures that wait for an SRS outside the task loop of the UE.
class f1ap_du_positioning_procedure_launcher
{
public:
  virtual ~f1ap_du_positioning_procedure_launcher() = default;

  /// Returns a task that starts the given positioning measurement procedure when a task loop runs it.
  virtual async_task<void> start_positioning_measurement_procedure_task(async_task<void> proc) = 0;
};

} // namespace ocudu::odu
