// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "f1ap_ue_context.h"
#include "ue_bearer_manager.h"
#include "ocudu/f1ap/du/f1ap_du.h"
#include "ocudu/f1ap/du/f1ap_du_positioning_handler.h"
#include "ocudu/f1ap/f1ap_ue_id_types.h"
#include "ocudu/ran/du_types.h"
#include "ocudu/support/timers.h"
#include <chrono>
#include <optional>
#include <vector>

namespace ocudu {
namespace odu {

/// Periodic E-CID measurement of one UE, as per TS 38.473 section 8.13.12.
struct f1ap_du_e_cid_measurement {
  /// Measurement identifier that the LMF assigns, as per TS 38.473 section 9.3.1.189.
  uint16_t lmf_ue_meas_id;
  /// Measurement identifier that the gNB-RAN node assigns, as per TS 38.473 section 9.3.1.190.
  uint16_t ran_ue_meas_id;
  /// Quantities that the gNB-DU measures on every period.
  std::vector<e_cid_meas_quantity> quantities;
  /// Time between two reports.
  std::chrono::milliseconds period;
  /// Runs out at the end of every period.
  unique_timer report_timer;
  /// Set while a report waits for its measurement.
  bool report_pending = false;

  /// Returns true if the given identifiers belong to this measurement.
  bool matches(uint16_t lmf_id, uint16_t ran_id) const { return lmf_ue_meas_id == lmf_id and ran_ue_meas_id == ran_id; }
};

class f1ap_du_ue
{
public:
  f1ap_du_ue(du_ue_index_t          ue_index_,
             gnb_du_ue_f1ap_id_t    gnb_f1ap_du_ue_id_,
             f1ap_du_configurator&  du_handler_,
             f1ap_message_notifier& f1ap_msg_notifier_,
             task_executor&         ctrl_exec,
             task_executor&         ue_exec,
             timer_manager&         timers) :
    context(ue_index_, gnb_f1ap_du_ue_id_),
    f1ap_msg_notifier(f1ap_msg_notifier_),
    du_handler(du_handler_),
    bearers(context, f1ap_msg_notifier, du_handler, ctrl_exec, ue_exec, timers)
  {
  }

  f1ap_ue_context        context;
  f1ap_message_notifier& f1ap_msg_notifier;
  f1ap_du_configurator&  du_handler;
  ue_bearer_manager      bearers;

  /// Periodic E-CID measurement of this UE. Empty when no periodic measurement runs.
  std::optional<f1ap_du_e_cid_measurement> e_cid_meas;
};

} // namespace odu
} // namespace ocudu
