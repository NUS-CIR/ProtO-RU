// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ngap_pdu_session_resource_release_procedure.h"
#include "ngap_error_indication_helper.h"
#include "ocudu/asn1/ngap/common.h"
#include "ocudu/ngap/ngap_message.h"
#include "ocudu/ran/cause/common.h"
#include "ocudu/ran/cause/ngap_cause.h"

using namespace ocudu;
using namespace ocudu::ocucp;
using namespace asn1::ngap;

ngap_pdu_session_resource_release_procedure::ngap_pdu_session_resource_release_procedure(
    const ngap_pdu_session_resource_release_command& command_,
    const ngap_ue_ids&                               ue_ids_,
    ngap_cu_cp_notifier&                             cu_cp_notifier_,
    ngap_message_notifier&                           amf_notifier_,
    ngap_ue_logger&                                  logger_) :
  command(command_), ue_ids(ue_ids_), cu_cp_notifier(cu_cp_notifier_), amf_notifier(amf_notifier_), logger(logger_)
{
}

void ngap_pdu_session_resource_release_procedure::operator()(coro_context<async_task<void>>& ctx)
{
  CORO_BEGIN(ctx);

  logger.log_info("\"{}\" started...", name());

  // Handle mandatory IEs.
  CORO_AWAIT_VALUE(response, cu_cp_notifier.on_new_pdu_session_resource_release_command(command));

  // TODO: Handle optional IEs.

  if (validate_and_send_response()) {
    logger.log_info("\"{}\" finished successfully", name());
  } else {
    logger.log_warning("\"{}\" failed", name());
  }

  CORO_RETURN();
}

/// \brief Convert common type PDU Session Resource Release Response message to NGAP PDU Session Resource Release
/// Response.
/// \param[out] resp The ASN1 NGAP PDU Session Resource Release Response message.
/// \param[in] cu_cp_resp The CU-CP PDU Session Resource Release Response message.
/// \return True on success, otherwise false.
static bool
fill_asn1_pdu_session_resource_release_response(asn1::ngap::pdu_session_res_release_resp_s&       resp,
                                                const ngap_pdu_session_resource_release_response& cu_cp_resp)
{
  for (const auto& cu_cp_pdu_session_res_released_item : cu_cp_resp.released_pdu_sessions) {
    asn1::ngap::pdu_session_res_released_item_rel_res_s asn1_pdu_session_res_released_item;
    asn1_pdu_session_res_released_item.pdu_session_id =
        to_underlying(cu_cp_pdu_session_res_released_item.pdu_session_id);

    asn1::ngap::pdu_session_res_release_resp_transfer_s res_release_resp_transfer;

    if (cu_cp_pdu_session_res_released_item.resp_transfer.secondary_rat_usage_info.has_value()) {
      const auto& ocudu_rat_usage = *cu_cp_pdu_session_res_released_item.resp_transfer.secondary_rat_usage_info;
      res_release_resp_transfer.ie_exts_present                          = true;
      res_release_resp_transfer.ie_exts.secondary_rat_usage_info_present = true;
      secondary_rat_usage_info_s& asn1_rat_resp = res_release_resp_transfer.ie_exts.secondary_rat_usage_info;

      if (ocudu_rat_usage.pdu_session_usage_report.has_value()) {
        const auto& ocudu_pdu_ses                      = *ocudu_rat_usage.pdu_session_usage_report;
        asn1_rat_resp.pdu_session_usage_report_present = true;
        if (!asn1::string_to_enum(asn1_rat_resp.pdu_session_usage_report.rat_type, ocudu_pdu_ses.rat_type)) {
          return false;
        }

        for (const auto& pdu_session_usage_timed_item : ocudu_pdu_ses.pdu_session_timed_report_list) {
          asn1::ngap::volume_timed_report_item_s asn1_pdu_session_usage_timed_item;

          asn1_pdu_session_usage_timed_item.start_time_stamp.from_number(pdu_session_usage_timed_item.start_time_stamp);
          asn1_pdu_session_usage_timed_item.end_time_stamp.from_number(pdu_session_usage_timed_item.end_time_stamp);
          asn1_pdu_session_usage_timed_item.usage_count_ul = pdu_session_usage_timed_item.usage_count_ul;
          asn1_pdu_session_usage_timed_item.usage_count_dl = pdu_session_usage_timed_item.usage_count_dl;

          asn1_rat_resp.pdu_session_usage_report.pdu_session_timed_report_list.push_back(
              asn1_pdu_session_usage_timed_item);
        }
      }

      for (const auto& qos_flows_usage_report_item :
           cu_cp_pdu_session_res_released_item.resp_transfer.secondary_rat_usage_info.value()
               .qos_flows_usage_report_list) {
        asn1::ngap::qos_flows_usage_report_item_s asn1_qos_flows_usage_report_item;
        asn1_qos_flows_usage_report_item.qos_flow_id = to_underlying(qos_flows_usage_report_item.qos_flow_id);

        if (!asn1::string_to_enum(asn1_qos_flows_usage_report_item.rat_type, qos_flows_usage_report_item.rat_type)) {
          return false;
        }

        for (const auto& qos_flow_timed_report_item : qos_flows_usage_report_item.qos_flows_timed_report_list) {
          asn1::ngap::volume_timed_report_item_s asn1_qos_flow_timed_report_item;

          asn1_qos_flow_timed_report_item.start_time_stamp.from_number(qos_flow_timed_report_item.start_time_stamp);
          asn1_qos_flow_timed_report_item.end_time_stamp.from_number(qos_flow_timed_report_item.end_time_stamp);
          asn1_qos_flow_timed_report_item.usage_count_ul = qos_flow_timed_report_item.usage_count_ul;
          asn1_qos_flow_timed_report_item.usage_count_dl = qos_flow_timed_report_item.usage_count_dl;

          asn1_qos_flows_usage_report_item.qos_flows_timed_report_list.push_back(asn1_qos_flow_timed_report_item);
        }

        asn1_rat_resp.qos_flows_usage_report_list.push_back(asn1_qos_flows_usage_report_item);
      }

    } else {
    }

    // Pack PDU session resource release response transfer container.
    byte_buffer pdu = pack_into_pdu(res_release_resp_transfer);
    if (pdu.empty()) {
      return false;
    }
    asn1_pdu_session_res_released_item.pdu_session_res_release_resp_transfer = std::move(pdu);

    resp->pdu_session_res_released_list_rel_res.push_back(asn1_pdu_session_res_released_item);
  }

  if (cu_cp_resp.user_location_info.has_value()) {
    resp->user_location_info_present = true;
    resp->user_location_info.set_user_location_info_nr() =
        cu_cp_user_location_info_to_asn1(cu_cp_resp.user_location_info.value());
  }

  return true;
}

bool ngap_pdu_session_resource_release_procedure::validate_and_send_response()
{
  ngap_message ngap_msg = {};
  ngap_msg.pdu.set_successful_outcome().load_info_obj(ASN1_NGAP_ID_PDU_SESSION_RES_RELEASE);
  auto& pdu_session_res_release_resp = ngap_msg.pdu.successful_outcome().value.pdu_session_res_release_resp();

  pdu_session_res_release_resp->amf_ue_ngap_id = to_underlying(ue_ids.amf_ue_id);
  pdu_session_res_release_resp->ran_ue_ngap_id = to_underlying(ue_ids.ran_ue_id);

  if (response.released_pdu_sessions.empty()) {
    send_error_indication(amf_notifier,
                          logger.get_basic_logger(),
                          ue_ids.ran_ue_id,
                          ue_ids.amf_ue_id,
                          ngap_cause_radio_network_t::unknown_pdu_session_id);
    return false;
  }

  if (not fill_asn1_pdu_session_resource_release_response(pdu_session_res_release_resp, response)) {
    logger.log_warning("Cannot fill ASN1 PDUSessionResourceReleaseResponse");
    send_error_indication(
        amf_notifier, logger.get_basic_logger(), ue_ids.ran_ue_id, ue_ids.amf_ue_id, cause_protocol_t::semantic_error);
    return false;
  }

  // Forward message to AMF.
  if (!amf_notifier.on_new_message(ngap_msg)) {
    logger.log_warning("Cannot send PDUSessionResourceReleaseResponse");
    return false;
  }

  return true;
}
