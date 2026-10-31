// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ocudu/adt/expected.h"
#include "ocudu/asn1/e2ap/e2ap.h"
#include "ocudu/asn1/e2ap/e2ap_pdu_contents.h"
#include "ocudu/support/async/protocol_transaction_manager.h"
#include "ocudu/support/error_handling.h"

namespace ocudu {

/// Extracts transaction id of Initiating message.
inline expected<uint8_t> get_transaction_id(const asn1::e2ap::init_msg_s& out)
{
  using namespace asn1::e2ap;
  switch (out.value.type().value) {
    case e2ap_elem_procs_o::init_msg_c::types_opts::e2setup_request:
      return out.value.e2setup_request()->transaction_id;
    case e2ap_elem_procs_o::init_msg_c::types_opts::e2_removal_request:
      return out.value.e2_removal_request()->transaction_id;
    default:
      break;
  }
  return make_unexpected(default_error_t{});
}

/// Extracts transaction id of Successful Outcome message.
inline expected<uint8_t> get_transaction_id(const asn1::e2ap::successful_outcome_s& out)
{
  using namespace asn1::e2ap;
  switch (out.value.type().value) {
    case e2ap_elem_procs_o::successful_outcome_c::types_opts::e2setup_resp:
      return out.value.e2setup_resp()->transaction_id;
    case e2ap_elem_procs_o::successful_outcome_c::types_opts::e2_removal_resp:
      return out.value.e2_removal_resp()->transaction_id;
    default:
      break;
  }
  return make_unexpected(default_error_t{});
}

/// Extracts transaction id of Unsuccessful Outcome message.
inline expected<uint8_t> get_transaction_id(const asn1::e2ap::unsuccessful_outcome_s& out)
{
  using namespace asn1::e2ap;
  switch (out.value.type().value) {
    case e2ap_elem_procs_o::unsuccessful_outcome_c::types_opts::e2setup_fail:
      return out.value.e2setup_fail()->transaction_id;
    case e2ap_elem_procs_o::unsuccessful_outcome_c::types_opts::e2_removal_fail:
      return out.value.e2_removal_fail()->transaction_id;
    default:
      break;
  }
  return make_unexpected(default_error_t{});
}

/// Extracts transaction id of E2AP PDU.
inline expected<uint8_t> get_transaction_id(const asn1::e2ap::e2ap_pdu_c& pdu)
{
  using namespace asn1::e2ap;
  switch (pdu.type().value) {
    case e2ap_pdu_c::types_opts::init_msg:
      return get_transaction_id(pdu.init_msg());
    case e2ap_pdu_c::types_opts::successful_outcome:
      return get_transaction_id(pdu.successful_outcome());
    case e2ap_pdu_c::types_opts::unsuccessful_outcome:
      return get_transaction_id(pdu.unsuccessful_outcome());
    default:
      break;
  }
  return make_unexpected(default_error_t{});
}

/// Checks whether the response/failure message received by a protocol transaction sink carries the expected
/// transaction id. Returns false if no message has been received (e.g. still pending, or completed via timeout,
/// cancellation or an abnormal condition), since there is no transaction id to compare against in those cases.
template <typename SuccessResp, typename FailureResp>
inline bool transaction_id_matches(const protocol_transaction_outcome_observer<SuccessResp, FailureResp>& sink,
                                   uint8_t expected_transaction_id)
{
  if (sink.successful()) {
    return sink.response()->transaction_id == expected_transaction_id;
  }
  if (sink.failed()) {
    return sink.failure()->transaction_id == expected_transaction_id;
  }
  return false;
}

/// Returns a human-readable string for an E2AP cause value.
inline const char* get_cause_str(const asn1::e2ap::cause_c& cause)
{
  using namespace asn1::e2ap;
  switch (cause.type()) {
    case cause_c::types_opts::ric_request:
      return cause.ric_request().to_string();
    case cause_c::types_opts::ric_service:
      return cause.ric_service().to_string();
    case cause_c::types_opts::e2_node:
      return cause.e2_node().to_string();
    case cause_c::types_opts::transport:
      return cause.transport().to_string();
    case cause_c::types_opts::protocol:
      return cause.protocol().to_string();
    case cause_c::types_opts::misc:
      return cause.misc().to_string();
    default:
      break;
  }
  return "unknown";
}

/// Extracts message type.
inline const char* get_message_type_str(const asn1::e2ap::e2ap_pdu_c& pdu)
{
  switch (pdu.type().value) {
    case asn1::e2ap::e2ap_pdu_c::types_opts::init_msg:
      return pdu.init_msg().value.type().to_string();
    case asn1::e2ap::e2ap_pdu_c::types_opts::successful_outcome:
      return pdu.successful_outcome().value.type().to_string();
    case asn1::e2ap::e2ap_pdu_c::types_opts::unsuccessful_outcome:
      return pdu.unsuccessful_outcome().value.type().to_string();
    default:
      break;
  }
  report_fatal_error("Invalid E2AP PDU type \"{}\"", pdu.type().to_string());
}
} // namespace ocudu
