// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "e2ap_removal_procedure.h"
#include "../common/e2ap_asn1_utils.h"
#include "ocudu/asn1/asn1_ap_utils.h"

using namespace ocudu;
using namespace asn1::e2ap;

e2ap_removal_procedure::e2ap_removal_procedure(e2_message_notifier&    notif_,
                                               e2_event_manager&       ev_mng_,
                                               ocudulog::basic_logger& logger_) :
  notifier(notif_), ev_mng(ev_mng_), logger(logger_)
{
}

void e2ap_removal_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  transaction_sink.subscribe_to(ev_mng.e2_removal_outcome, std::chrono::seconds{5});
  send_e2_removal_request();

  CORO_AWAIT(transaction_sink);

  if (transaction_sink.timeout_expired()) {
    logger.warning("E2 Removal procedure timed out waiting for RIC response. Proceeding with disconnection.");
  } else if (transaction_sink.protocol_transaction_failed()) {
    logger.warning("E2 Removal procedure was cancelled. Proceeding with disconnection.");
  } else if (not transaction_id_matches(transaction_sink, expected_transaction_id)) {
    logger.warning("E2 Removal procedure received a response with unexpected transaction id. Proceeding with "
                   "disconnection.");
  } else if (transaction_sink.successful()) {
    logger.info("E2 Removal procedure successful.");
  } else {
    logger.warning("E2 Removal procedure failed.");
  }

  CORO_RETURN();
}

void e2ap_removal_procedure::send_e2_removal_request()
{
  e2_message msg = {};
  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_E2AP_ID_E2REMOVAL);
  auto& rem_req           = msg.pdu.init_msg().value.e2_removal_request();
  rem_req->transaction_id = expected_transaction_id;

  notifier.on_new_message(msg);
}
