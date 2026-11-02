// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "../f1ap_du_positioning_procedure_launcher.h"
#include "../ue_context/f1ap_ue_context.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/f1ap/du/f1ap_du_positioning_handler.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/support/async/async_task.h"
#include <chrono>
#include <optional>

namespace ocudu::odu {

class f1ap_du_ue;
class f1ap_du_ue_manager;

/// \brief Handles the E-CID Measurement Initiation procedure, as per TS 38.473 section 8.13.12.
///
/// The CU-CP asks the DU for the E-CID measurements of one UE. The DU measures the requested quantities and sends
/// the result in the E-CID MEASUREMENT INITIATION RESPONSE. The DU keeps no state after the response.
class f1ap_du_e_cid_measurement_initiation_procedure
{
public:
  f1ap_du_e_cid_measurement_initiation_procedure(const asn1::f1ap::e_c_id_meas_initiation_request_s& msg_,
                                                 f1ap_du_positioning_handler&                        du_mng_,
                                                 f1ap_du_ue_manager&                                 ues_,
                                                 f1ap_du_positioning_procedure_launcher&             launcher_);

  void operator()(coro_context<async_task<void>>& ctx);

  const char* name() const { return "E-CID Measurement Initiation Procedure"; }

private:
  /// \brief Reads the requested measurement quantities into \c quantities.
  ///
  /// Returns false if the DU cannot produce every requested quantity, as per TS 38.473 section 8.13.12.3.
  bool read_request();

  /// Returns true if the CU-CP asked for periodic reporting.
  bool is_periodic() const;

  /// Returns true if the CU-CP asked for the UL Angle of Arrival.
  bool is_aoa_requested() const;

  /// \brief Returns the period of the reports, or an empty value if the request carries no applicable periodicity.
  ///
  /// NR Angle of Arrival has its own periodicity IE. The E-CID Measurement Periodicity IE paces the other
  /// quantities, as per TS 38.473 section 9.2.12.20.
  std::optional<std::chrono::milliseconds> get_report_period() const;

  /// Starts the periodic reporting of this UE.
  void start_periodic_reporting();

  async_task<du_e_cid_meas_response> request_e_cid_measurement();

  /// \brief Puts the measurement in the UE context, before the gNB-DU measures for the first time.
  ///
  /// A second request and a termination command then find the measurement while the gNB-DU still measures.
  void add_periodic_measurement();

  /// Returns true if the UE still holds the measurement of this procedure.
  bool holds_periodic_measurement() const;

  /// \brief Sends the E-CID MEASUREMENT INITIATION RESPONSE.
  ///
  /// A periodic response carries no measurement result, as per TS 38.473 section 8.13.12.2.
  void send_response(bool with_result) const;
  void send_failure() const;

  const asn1::f1ap::e_c_id_meas_initiation_request_s msg;
  f1ap_du_positioning_handler&                       du_mng;
  f1ap_du_ue_manager&                                ues;
  f1ap_du_positioning_procedure_launcher&            launcher;
  /// Identifier of the UE. The UE context can be removed while the measurement runs, so the procedure looks the UE
  /// up again instead of holding a reference to it.
  const gnb_du_ue_f1ap_id_t du_ue_id;
  /// Copy of the UE context, for the procedure logs.
  const f1ap_ue_context   ue_ctxt;
  ocudulog::basic_logger& logger;

  std::vector<e_cid_meas_quantity> quantities;
  du_e_cid_meas_response           du_result;
};

} // namespace ocudu::odu
