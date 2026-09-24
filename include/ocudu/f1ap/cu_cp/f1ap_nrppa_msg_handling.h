// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/ran/positioning/e_cid_measurement.h"
#include "ocudu/ran/positioning/positioning_messages.h"
#include "ocudu/support/async/async_task.h"

namespace ocudu::ocucp {

/// Handles incoming NRPPa messages to be transmitted over the F1 interface.
class f1ap_nrppa_message_handler
{
public:
  virtual ~f1ap_nrppa_message_handler() = default;

  /// Handles the given TRP information request message.
  virtual async_task<expected<trp_information_response_t, trp_information_failure_t>>
  handle_trp_information_request(const trp_information_request_t& request) = 0;

  /// Handles the given positioning information request message.
  virtual async_task<expected<positioning_information_response_t, positioning_information_failure_t>>
  handle_positioning_information_request(const positioning_information_request_t& request) = 0;

  /// Handles the given positioning activation request message.
  virtual async_task<expected<positioning_activation_response_t, positioning_activation_failure_t>>
  handle_positioning_activation_request(const positioning_activation_request_t& request) = 0;

  /// Handles the given positioning measurement request message.
  virtual async_task<expected<measurement_response_t, measurement_failure_t>>
  handle_positioning_measurement_request(const measurement_request_t& request) = 0;

  /// Handles the given E-CID measurement initiation request message.
  virtual async_task<expected<e_cid_measurement_response_t, e_cid_measurement_failure_t>>
  handle_e_cid_measurement_request(const e_cid_measurement_request_t& request) = 0;

  /// Sends an E-CID measurement termination command to the gNB-DU.
  virtual void handle_e_cid_measurement_termination_command(const e_cid_measurement_termination_command_t& command) = 0;
};

} // namespace ocudu::ocucp
