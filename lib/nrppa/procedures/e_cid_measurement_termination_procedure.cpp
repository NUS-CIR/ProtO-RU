// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "e_cid_measurement_termination_procedure.h"
#include "ocudu/adt/format.h"
#include "ocudu/support/async/coroutine.h"

using namespace ocudu;
using namespace ocucp;

e_cid_measurement_termination_procedure::e_cid_measurement_termination_procedure(cu_cp_ue_index_t        ue_index_,
                                                                                 nrppa_ue_context_list&  ue_ctxt_list_,
                                                                                 nrppa_impl&             parent_,
                                                                                 ocudulog::basic_logger& logger_) :
  ue_index(ue_index_), ue_ctxt_list(ue_ctxt_list_), parent(parent_), logger(logger_)
{
}

void e_cid_measurement_termination_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  if (!ue_ctxt_list.contains(ue_index)) {
    logger.info("ue={}: Stopping \"{}\". UE was already removed", ue_index, name());
    CORO_EARLY_RETURN();
  }

  ue_ctxt = &ue_ctxt_list[ue_index];

  ue_ctxt->logger.log_info("\"{}\" started...", name());

  // Notify positioning manager about meas termination request.
  terminate_periodic_measurements();

  ue_ctxt->logger.log_info("\"{}\" finished successfully", name());

  CORO_RETURN();
}

void e_cid_measurement_termination_procedure::terminate_periodic_measurements()
{
  // Remove stored information from UE context.
  ue_ctxt->meas_quantities.clear();
  ue_ctxt->meas_periodicity_ms = std::nullopt;

  // Stop timer for periodic reporting.
  ue_ctxt->meas_report_timer.stop();

  // Stop the measurement the gNB-DU runs, as per TS 38.473 section 8.13.15.
  parent.terminate_du_e_cid_measurement(ue_index);
}
