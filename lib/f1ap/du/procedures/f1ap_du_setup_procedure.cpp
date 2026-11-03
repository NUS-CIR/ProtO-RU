// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_du_setup_procedure.h"
#include "../../asn1_helpers.h"
#include "../../f1ap_asn1_utils.h"
#include "../f1ap_asn1_converters.h"
#include "../f1ap_du_context.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/asn1_utils.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/f1ap/f1ap_message.h"

using namespace ocudu;
using namespace ocudu::odu;
using namespace asn1::f1ap;

f1ap_du_setup_procedure::f1ap_du_setup_procedure(const f1_setup_request_message& request_,
                                                 f1ap_message_notifier&          cu_notif_,
                                                 f1ap_event_manager&             ev_mng_,
                                                 f1ap_du_context&                du_ctxt_) :
  request(request_),
  cu_notifier(cu_notif_),
  ev_mng(ev_mng_),
  logger(ocudulog::fetch_basic_logger("DU-F1")),
  du_ctxt(du_ctxt_)
{
}

void f1ap_du_setup_procedure::operator()(coro_context<async_task<f1_setup_result>>& ctx)
{
  CORO_BEGIN(ctx);

  transaction = ev_mng.transactions.create_transaction(request.response_timeout);
  if (not transaction.valid()) {
    CORO_EARLY_RETURN(create_f1_setup_result());
  }

  // Send request to CU.
  send_f1_setup_request();

  // Await CU response.
  // Note: The caller retries a failed F1 Setup, honouring the Time to Wait commanded by the CU-CP.
  CORO_AWAIT(transaction);

  // Forward procedure result to DU manager.
  CORO_RETURN(create_f1_setup_result());
}

void f1ap_du_setup_procedure::send_f1_setup_request()
{
  // Save the gNB-DU-Id before the F1 Setup is completed for the purpose of logging.
  du_ctxt.du_id = request.gnb_du_id;

  f1ap_message msg = {};
  // Set F1AP PDU contents.
  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_F1_SETUP);
  f1_setup_request_s& setup_req = msg.pdu.init_msg().value.f1_setup_request();

  setup_req->transaction_id = transaction.id();

  // DU-global parameters.
  setup_req->gnb_du_id           = static_cast<uint64_t>(request.gnb_du_id);
  setup_req->gnb_du_name_present = not request.gnb_du_name.empty();
  if (setup_req->gnb_du_name_present) {
    setup_req->gnb_du_name.from_string(request.gnb_du_name);
  }
  setup_req->gnb_du_rrc_version.latest_rrc_version.from_number(request.rrc_version);

  setup_req->gnb_du_served_cells_list_present = true;
  setup_req->gnb_du_served_cells_list.resize(request.served_cells.size());
  for (unsigned i = 0; i != request.served_cells.size(); ++i) {
    const f1_cell_setup_params& cell_cfg = request.served_cells[i];
    setup_req->gnb_du_served_cells_list[i].load_info_obj(ASN1_F1AP_ID_GNB_DU_SERVED_CELLS_LIST);
    gnb_du_served_cells_item_s& f1ap_cell = setup_req->gnb_du_served_cells_list[i]->gnb_du_served_cells_item();

    // Set base servedCellInfo
    f1ap_cell.served_cell_info = make_asn1_served_cell_info(cell_cfg.cell_info, cell_cfg.slices);

    // Add System Information related to the cell.
    f1ap_cell.gnb_du_sys_info_present  = true;
    f1ap_cell.gnb_du_sys_info.mib_msg  = cell_cfg.du_sys_info.packed_mib.copy();
    f1ap_cell.gnb_du_sys_info.sib1_msg = cell_cfg.du_sys_info.packed_sib1.copy();
  }

  // Capture packed request bytes for later use.
  {
    byte_buffer   packed{byte_buffer::fallback_allocation_tag{}};
    asn1::bit_ref bref(packed);
    if (msg.pdu.pack(bref) == asn1::OCUDUASN_SUCCESS) {
      captured_request = std::move(packed);
    } else {
      logger.warning("\"{}\": failed to pack F1SetupRequest", name());
    }
  }

  // send request
  logger.info("F1 Setup: Sending F1 Setup Request to CU-CP...");
  cu_notifier.on_new_message(msg);
}

static bool validate_f1_response(const f1ap_transaction_response& transaction_resp)
{
  if (transaction_resp.has_value() and transaction_resp.value().value.type().value !=
                                           f1ap_elem_procs_o::successful_outcome_c::types_opts::f1_setup_resp) {
    // Unexpected successful outcome type.
    return false;
  }
  if (not transaction_resp.has_value() and transaction_resp.error().value.type().value !=
                                               f1ap_elem_procs_o::unsuccessful_outcome_c::types_opts::f1_setup_fail) {
    // Unexpected unsuccessful outcome type.
    return false;
  }

  return true;
}

f1_setup_result f1ap_du_setup_procedure::create_f1_setup_result()
{
  if (not transaction.valid()) {
    // Transaction could not be allocated.
    logger.error("{}: Procedure cancelled. Cause: Failed to allocate transaction.", name());
    f1_setup_failure fail{};
    fail.result   = f1_setup_failure::result_code::proc_failure;
    du_ctxt.du_id = gnb_du_id_t::invalid;
    return make_unexpected(fail);
  }

  if (transaction.aborted()) {
    // Abortion/timeout case.
    logger.error("{}: Procedure cancelled. Cause: Timeout reached.", name());
    f1_setup_failure fail{};
    fail.result   = f1_setup_failure::result_code::timeout;
    du_ctxt.du_id = gnb_du_id_t::invalid;
    return make_unexpected(fail);
  }
  const f1ap_transaction_response& cu_pdu_response = transaction.response();

  if (not validate_f1_response(cu_pdu_response)) {
    // Response content is invalid.
    logger.error(
        "{}: Received PDU with unexpected PDU type {}", name(), cu_pdu_response.value().value.type().to_string());
    f1_setup_failure fail{};
    fail.result   = f1_setup_failure::result_code::invalid_response;
    du_ctxt.du_id = gnb_du_id_t::invalid;
    return make_unexpected(fail);
  }

  if (not cu_pdu_response.has_value()) {
    // F1 Setup Failure case.
    const auto&      asn1_fail = *cu_pdu_response.error().value.f1_setup_fail();
    f1_setup_failure failure{};
    failure.result                 = f1_setup_failure::result_code::f1_setup_failure;
    failure.f1_setup_failure_cause = get_cause_str(asn1_fail.cause);
    if (asn1_fail.time_to_wait_present) {
      failure.time_to_wait = std::chrono::seconds{asn1_fail.time_to_wait.to_number()};
    }
    logger.debug("{}: F1 Setup Failure with cause \"{}\"", name(), get_cause_str(asn1_fail.cause));
    du_ctxt.du_id = gnb_du_id_t::invalid;

    return make_unexpected(failure);
  }

  // F1 Setup Response case.

  const auto&      asn1_success = *cu_pdu_response.value().value.f1_setup_resp();
  f1_setup_success success{};

  // Capture packed response bytes.
  success.packed_f1_setup_request = std::move(captured_request);
  {
    f1ap_message resp_msg                 = {};
    resp_msg.pdu.set_successful_outcome() = cu_pdu_response.value();
    byte_buffer   packed{byte_buffer::fallback_allocation_tag{}};
    asn1::bit_ref bref(packed);
    if (resp_msg.pdu.pack(bref) == asn1::OCUDUASN_SUCCESS) {
      success.packed_f1_setup_response = std::move(packed);
    } else {
      logger.warning("\"{}\": failed to pack F1SetupResponse", name());
    }
  }

  // Update F1 DU Context (taking values from request).
  du_ctxt.du_id       = request.gnb_du_id;
  du_ctxt.gnb_du_name = request.gnb_du_name;
  du_ctxt.f1c_setup   = true;
  du_ctxt.served_cells.clear();
  for (unsigned i = 0, sz = request.served_cells.size(); i != sz; ++i) {
    const du_cell_index_t cell_idx = request.served_cells[i].cell_index;
    du_ctxt.served_cells.emplace(cell_idx,
                                 f1ap_du_cell_context{cell_idx,
                                                      request.served_cells[i].cell_info.nr_cgi,
                                                      request.served_cells[i].cell_info.ntn_link_rtt});
  }
  if (asn1_success.cells_to_be_activ_list_present) {
    success.cells_to_activate.resize(asn1_success.cells_to_be_activ_list.size());
    for (unsigned i = 0, sz = asn1_success.cells_to_be_activ_list.size(); i != sz; ++i) {
      auto&               asn1_cell    = asn1_success.cells_to_be_activ_list[i]->cells_to_be_activ_list_item();
      nr_cell_global_id_t cgi          = cgi_from_asn1(asn1_cell.nr_cgi).value();
      success.cells_to_activate[i].cgi = cgi;
    }
  }

  logger.info("{}: Procedure completed successfully.", name());

  return success;
}
