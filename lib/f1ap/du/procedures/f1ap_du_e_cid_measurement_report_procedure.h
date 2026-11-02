// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../f1ap_du_positioning_procedure_launcher.h"
#include "ocudu/f1ap/du/f1ap_du_positioning_handler.h"
#include "ocudu/f1ap/f1ap_ue_id_types.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/async/async_task.h"

namespace ocudu::odu {

class f1ap_du_ue;
class f1ap_du_ue_manager;

/// \brief Sends one E-CID MEASUREMENT REPORT of a periodic measurement, as per TS 38.473 section 8.13.14.
///
/// The procedure measures the quantities of the running measurement and sends one report. It then starts the next
/// period. If the measurement fails, it sends an E-CID MEASUREMENT FAILURE INDICATION and stops the measurement. The
/// gNB-CU then considers the measurement terminated, as per TS 38.473 section 8.13.13.2.
class f1ap_du_e_cid_measurement_report_procedure
{
public:
  f1ap_du_e_cid_measurement_report_procedure(f1ap_du_positioning_handler& du_mng_,
                                             f1ap_du_ue_manager&          ues_,
                                             gnb_du_ue_f1ap_id_t          du_ue_id_,
                                             uint16_t                     lmf_ue_meas_id_,
                                             uint16_t                     ran_ue_meas_id_);

  void operator()(coro_context<async_task<void>>& ctx);

  const char* name() const { return "E-CID Measurement Reporting"; }

private:
  /// Returns the UE that runs this measurement, or nullptr.
  f1ap_du_ue* find_ue() const;

  async_task<du_e_cid_meas_response> request_e_cid_measurement(const f1ap_du_ue& ue) const;

  void send_report(const f1ap_du_ue& ue, const du_e_cid_meas_response& du_result) const;
  void send_failure_indication(const f1ap_du_ue& ue) const;

  f1ap_du_positioning_handler& du_mng;
  f1ap_du_ue_manager&          ues;
  const gnb_du_ue_f1ap_id_t    du_ue_id;
  const uint16_t               lmf_ue_meas_id;
  const uint16_t               ran_ue_meas_id;
  ocudulog::basic_logger&      logger;

  du_e_cid_meas_response du_result;
};

/// \brief Starts the periodic reporting of the E-CID measurement of the given UE.
///
/// The UE must hold a measurement. The first report follows the first period.
void start_e_cid_measurement_reporting(f1ap_du_ue&                             ue,
                                       f1ap_du_positioning_handler&            du_mng,
                                       f1ap_du_ue_manager&                     ues,
                                       f1ap_du_positioning_procedure_launcher& launcher);

} // namespace ocudu::odu
