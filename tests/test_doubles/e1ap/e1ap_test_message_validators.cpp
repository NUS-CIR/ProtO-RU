// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "e1ap_test_message_validators.h"
#include "ocudu/asn1/e1ap/common.h"
#include "ocudu/asn1/e1ap/e1ap.h"
#include "ocudu/asn1/e1ap/e1ap_pdu_contents.h"
#include "ocudu/e1ap/common/e1ap_message.h"
#include "ocudu/ran/cu_types.h"

using namespace ocudu;

#define TRUE_OR_RETURN(cond)                                                                                           \
  if (not(cond))                                                                                                       \
    return false;

static bool is_packable(const e1ap_message& msg)
{
  byte_buffer   temp_pdu;
  asn1::bit_ref bref{temp_pdu};
  return msg.pdu.pack(bref) == asn1::OCUDUASN_SUCCESS;
}

bool ocudu::test_helpers::is_valid_bearer_context_setup_request(const e1ap_message& msg)
{
  TRUE_OR_RETURN(msg.pdu.type() == asn1::e1ap::e1ap_pdu_c::types_opts::init_msg);
  TRUE_OR_RETURN(msg.pdu.init_msg().proc_code == ASN1_E1AP_ID_BEARER_CONTEXT_SETUP);
  TRUE_OR_RETURN(is_packable(msg));

  return true;
}

bool ocudu::test_helpers::is_valid_security_indication_with_bearer_context_setup_request(const e1ap_message&   msg,
                                                                                         pdu_session_id_t      psi,
                                                                                         security_indication_t sec_ind)
{
  TRUE_OR_RETURN(msg.pdu.type() == asn1::e1ap::e1ap_pdu_c::types_opts::init_msg);
  TRUE_OR_RETURN(msg.pdu.init_msg().proc_code == ASN1_E1AP_ID_BEARER_CONTEXT_SETUP);

  const asn1::e1ap::bearer_context_setup_request_s& req_ies = msg.pdu.init_msg().value.bearer_context_setup_request();

  TRUE_OR_RETURN(asn1::e1ap::sys_bearer_context_setup_request_c::types_opts::ng_ran_bearer_context_setup_request ==
                 req_ies->sys_bearer_context_setup_request.type());
  const asn1::e1ap::pdu_session_res_to_setup_list_l& asn1_pdu_session_res_item =
      req_ies->sys_bearer_context_setup_request.ng_ran_bearer_context_setup_request().pdu_session_res_to_setup_list;

  for (const asn1::e1ap::pdu_session_res_to_setup_item_s& pdu_session : asn1_pdu_session_res_item) {
    if (pdu_session.pdu_session_id != to_underlying(psi)) {
      continue;
    }
    asn1::e1ap::security_ind_s security_indication = pdu_session.security_ind;
    if (security_indication.integrity_protection_ind == asn1::e1ap::integrity_protection_ind_opts::required) {
      TRUE_OR_RETURN(sec_ind.integrity_protection_ind == integrity_protection_indication_t::required);
    } else if (security_indication.integrity_protection_ind == asn1::e1ap::integrity_protection_ind_opts::preferred) {
      TRUE_OR_RETURN(sec_ind.integrity_protection_ind == integrity_protection_indication_t::preferred);
    } else if (security_indication.integrity_protection_ind == asn1::e1ap::integrity_protection_ind_opts::not_needed) {
      TRUE_OR_RETURN(sec_ind.integrity_protection_ind == integrity_protection_indication_t::not_needed);
    }

    if (security_indication.confidentiality_protection_ind ==
        asn1::e1ap::confidentiality_protection_ind_opts::required) {
      TRUE_OR_RETURN(sec_ind.confidentiality_protection_ind == confidentiality_protection_indication_t::required);
    } else if (security_indication.confidentiality_protection_ind ==
               asn1::e1ap::confidentiality_protection_ind_opts::preferred) {
      TRUE_OR_RETURN(sec_ind.confidentiality_protection_ind == confidentiality_protection_indication_t::preferred);
    } else if (security_indication.confidentiality_protection_ind ==
               asn1::e1ap::confidentiality_protection_ind_opts::not_needed) {
      TRUE_OR_RETURN(sec_ind.confidentiality_protection_ind == confidentiality_protection_indication_t::not_needed);
    }

    TRUE_OR_RETURN(is_packable(msg));
  }

  return true;
}

bool ocudu::test_helpers::is_valid_bearer_context_modification_request(const e1ap_message& msg)
{
  TRUE_OR_RETURN(msg.pdu.type() == asn1::e1ap::e1ap_pdu_c::types_opts::init_msg);
  TRUE_OR_RETURN(msg.pdu.init_msg().proc_code == ASN1_E1AP_ID_BEARER_CONTEXT_MOD);
  TRUE_OR_RETURN(is_packable(msg));

  return true;
}

bool ocudu::test_helpers::is_valid_bearer_context_modification_request_with_ue_security_info(const e1ap_message& msg)
{
  TRUE_OR_RETURN(is_valid_bearer_context_modification_request(msg));
  TRUE_OR_RETURN(msg.pdu.init_msg().value.bearer_context_mod_request()->security_info_present);

  return true;
}

bool ocudu::test_helpers::is_valid_bearer_context_release_command(const e1ap_message& msg)
{
  TRUE_OR_RETURN(msg.pdu.type() == asn1::e1ap::e1ap_pdu_c::types_opts::init_msg);
  TRUE_OR_RETURN(msg.pdu.init_msg().proc_code == ASN1_E1AP_ID_BEARER_CONTEXT_RELEASE);
  TRUE_OR_RETURN(is_packable(msg));

  return true;
}

bool ocudu::test_helpers::is_valid_e1_reset(const e1ap_message& msg)
{
  TRUE_OR_RETURN(msg.pdu.type() == asn1::e1ap::e1ap_pdu_c::types_opts::init_msg);
  TRUE_OR_RETURN(msg.pdu.init_msg().proc_code == ASN1_E1AP_ID_RESET);
  TRUE_OR_RETURN(is_packable(msg));

  return true;
}
