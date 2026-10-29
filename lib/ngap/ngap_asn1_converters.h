// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#pragma once

#include "ngap_asn1_utils.h"
#include "ocudu/asn1/ngap/ngap_ies.h"
#include "ocudu/ngap/ngap_context.h"
#include "ocudu/ngap/ngap_handover.h"
#include "ocudu/ngap/ngap_pdu_session.h"
#include "ocudu/ngap/ngap_setup.h"
#include "ocudu/ocudulog/ocudulog.h"
#include "ocudu/ran/cause/ngap_cause.h"
#include "ocudu/ran/cu_cp_location_reporting_types.h"
#include "ocudu/ran/cu_types.h"
#include "ocudu/ran/five_g_s_tmsi.h"
#include "ocudu/ran/inter_cu_handover_messages.h"
#include "ocudu/ran/rb_id.h"
#include "ocudu/ran/tai.h"
#include "ocudu/ran/up_transport_layer_info.h"
#include "ocudu/security/security_asn1_utils.h"
#include <variant>

namespace ocudu::ocucp {

// Helper to create PDU from NGAP message.
template <class T>
byte_buffer pack_into_pdu(const T& msg, const char* context_name = nullptr)
{
  context_name = context_name == nullptr ? __FUNCTION__ : context_name;
  byte_buffer   pdu{};
  asn1::bit_ref bref{pdu};
  if (msg.pack(bref) == asn1::OCUDUASN_ERROR_ENCODE_FAIL) {
    ocudulog::fetch_basic_logger("NGAP").error("Failed to pack message in {} - discarding it", context_name);
    pdu.clear();
  }
  return pdu;
}

/// \brief  Convert CU-CP security result to NGAP security result.
/// \param[in] security_result The CU-CP security result.
/// \return The NGAP security result.
inline asn1::ngap::security_result_s cu_cp_security_result_to_ngap_security_result(security_result_t security_result)
{
  asn1::ngap::security_result_s ngap_security_result;

  if (security_result.confidentiality_protection_result == confidentiality_protection_result_t::performed) {
    ngap_security_result.confidentiality_protection_result =
        asn1::ngap::confidentiality_protection_result_opts::options::performed;
  } else {
    ngap_security_result.confidentiality_protection_result =
        asn1::ngap::confidentiality_protection_result_opts::options::not_performed;
  }

  if (security_result.integrity_protection_result == integrity_protection_result_t::performed) {
    ngap_security_result.integrity_protection_result.value =
        asn1::ngap::integrity_protection_result_opts::options::performed;
  } else {
    ngap_security_result.integrity_protection_result =
        asn1::ngap::integrity_protection_result_opts::options::not_performed;
  }

  return ngap_security_result;
}

/// \brief Convert CU-CP Associated QoS Flow to NGAP Associated QoS Flow Item.
/// \param[in] cu_cp_qos_flow The CU-CP Associated QoS Flow.
/// \return The NGAP Associated QoS Flow Item.
inline asn1::ngap::associated_qos_flow_item_s
cu_cp_assoc_qos_flow_to_ngap_assoc_qos_flow_item(cu_cp_associated_qos_flow cu_cp_qos_flow)
{
  asn1::ngap::associated_qos_flow_item_s asn1_assoc_qos_item;

  asn1_assoc_qos_item.qos_flow_id = to_underlying(cu_cp_qos_flow.qos_flow_id);

  if (cu_cp_qos_flow.qos_flow_map_ind.has_value()) {
    asn1_assoc_qos_item.qos_flow_map_ind_present = true;

    if (cu_cp_qos_flow.qos_flow_map_ind.value() == cu_cp_qos_flow_map_indication::ul) {
      asn1_assoc_qos_item.qos_flow_map_ind.value =
          asn1::ngap::associated_qos_flow_item_s::qos_flow_map_ind_opts::options::ul;
    } else {
      asn1_assoc_qos_item.qos_flow_map_ind.value =
          asn1::ngap::associated_qos_flow_item_s::qos_flow_map_ind_opts::options::dl;
    }
  }

  return asn1_assoc_qos_item;
}

/// \brief Convert CU-CP QoS Flow Per TNL Info to NGAP QoS Flow Per TNL Info.
/// \param[in] cu_cp_qos_flow_info The CU-CP QoS Flow Per TNL Info.
/// \return The NGAP QoS Flow Per TNL Info.
inline asn1::ngap::qos_flow_per_tnl_info_s
cu_cp_qos_flow_per_tnl_info_to_ngap_qos_flow_per_tnl_info(const ngap_qos_flow_per_tnl_information& cu_cp_qos_flow_info)
{
  asn1::ngap::qos_flow_per_tnl_info_s ngap_qos_flow_info;

  up_transport_layer_info_to_asn1(ngap_qos_flow_info.up_transport_layer_info, cu_cp_qos_flow_info.up_tp_layer_info);

  for (const auto& cu_cp_assoc_qos_item : cu_cp_qos_flow_info.associated_qos_flow_list) {
    asn1::ngap::associated_qos_flow_item_s ngap_assoc_qos_item =
        cu_cp_assoc_qos_flow_to_ngap_assoc_qos_flow_item(cu_cp_assoc_qos_item);

    ngap_qos_flow_info.associated_qos_flow_list.push_back(ngap_assoc_qos_item);
  }

  return ngap_qos_flow_info;
}

/// \brief Convert \c ngap_cause_t type to NGAP cause.
/// \param[in] cause The ngap_cause_t type.
/// \return The NGAP cause.
inline asn1::ngap::cause_c cause_to_asn1(ngap_cause_t cause)
{
  asn1::ngap::cause_c asn1_cause;

  if (const auto* result = std::get_if<ngap_cause_radio_network_t>(&cause)) {
    asn1_cause.set_radio_network() = static_cast<asn1::ngap::cause_radio_network_opts::options>(*result);
    return asn1_cause;
  }
  if (const auto* result = std::get_if<ngap_cause_transport_t>(&cause)) {
    asn1_cause.set_transport() = static_cast<asn1::ngap::cause_transport_opts::options>(*result);
    return asn1_cause;
  }
  if (const auto* result = std::get_if<cause_nas_t>(&cause)) {
    asn1_cause.set_nas() = static_cast<asn1::ngap::cause_nas_opts::options>(*result);
    return asn1_cause;
  }
  if (const auto* result = std::get_if<cause_protocol_t>(&cause)) {
    asn1_cause.set_protocol() = static_cast<asn1::ngap::cause_protocol_opts::options>(*result);
    return asn1_cause;
  }
  if (const auto* result = std::get_if<ngap_cause_misc_t>(&cause)) {
    asn1_cause.set_misc() = static_cast<asn1::ngap::cause_misc_opts::options>(*result);
    return asn1_cause;
  }

  report_fatal_error("Cannot convert cause to NGAP type:{}", cause);
  return asn1_cause;
}

/// \brief Convert NGAP ASN1 cause to \c ngap_cause_t type.
/// \param[in] asn1_cause The ASN1 NGAP cause.
/// \return The ngap_cause_t type.
inline ngap_cause_t asn1_to_cause(asn1::ngap::cause_c asn1_cause)
{
  ngap_cause_t cause;

  switch (asn1_cause.type()) {
    case asn1::ngap::cause_c::types_opts::radio_network:
      cause = static_cast<ngap_cause_radio_network_t>(asn1_cause.radio_network().value);
      break;
    case asn1::ngap::cause_c::types_opts::transport:
      cause = static_cast<ngap_cause_transport_t>(asn1_cause.transport().value);
      break;
    case asn1::ngap::cause_c::types_opts::nas:
      cause = static_cast<cause_nas_t>(asn1_cause.nas().value);
      break;
    case asn1::ngap::cause_c::types_opts::protocol:
      cause = static_cast<cause_protocol_t>(asn1_cause.protocol().value);
      break;
    case asn1::ngap::cause_c::types_opts::misc:
      cause = static_cast<ngap_cause_misc_t>(asn1_cause.misc().value);
      break;
    default:
      // The ASN.1 defines no Cause extension. Use the unspecified cause for an unknown extension.
      cause = ngap_cause_misc_t::unspecified;
  }

  return cause;
}

/// \brief Convert CU-CP NRCGI to NR Cell Identity.
/// \param[in] ngap_cgi The NGAP NRCGI.
/// \return The NR Cell Identity.
inline nr_cell_identity cu_cp_nrcgi_to_nr_cell_identity(asn1::ngap::nr_cgi_s& ngap_cgi)
{
  return nr_cell_identity::create(ngap_cgi.nr_cell_id.to_number()).value();
}

/// \brief Convert common type \c establishment_cause_t to ASN.1.
/// \param[in] cause The common type establishment cause.
/// \return The ASN.1 establishment cause.
inline asn1::ngap::rrc_establishment_cause_e establishment_cause_to_asn1(establishment_cause_t cause)
{
  switch (cause) {
    case establishment_cause_t::emergency:
      return asn1::ngap::rrc_establishment_cause_e::emergency;
    case establishment_cause_t::high_prio_access:
      return asn1::ngap::rrc_establishment_cause_e::high_prio_access;
    case establishment_cause_t::mt_access:
      return asn1::ngap::rrc_establishment_cause_e::mt_access;
    case establishment_cause_t::mo_sig:
      return asn1::ngap::rrc_establishment_cause_e::mo_sig;
    case establishment_cause_t::mo_data:
      return asn1::ngap::rrc_establishment_cause_e::mo_data;
    case establishment_cause_t::mo_voice_call:
      return asn1::ngap::rrc_establishment_cause_e::mo_voice_call;
    case establishment_cause_t::mo_video_call:
      return asn1::ngap::rrc_establishment_cause_e::mo_video_call;
    case establishment_cause_t::mo_sms:
      return asn1::ngap::rrc_establishment_cause_e::mo_sms;
    case establishment_cause_t::mps_prio_access:
      return asn1::ngap::rrc_establishment_cause_e::mps_prio_access;
    case establishment_cause_t::mcs_prio_access:
      return asn1::ngap::rrc_establishment_cause_e::mcs_prio_access;
    default:
      return asn1::ngap::rrc_establishment_cause_e::not_available;
  }
}

/// \brief Convert CU-CP NRCGI to NR Cell Identity.
/// \param[in] ngap_cgi The NGAP NRCGI.
/// \return The NR Cell Identity.
inline asn1::ngap::user_location_info_nr_s
cu_cp_user_location_info_to_asn1(const cu_cp_user_location_info_nr& cu_cp_user_location_info)
{
  asn1::ngap::user_location_info_nr_s asn1_user_location_info;

  // Fill NR CGI. TS 38.300 sec. 16.14.5 has an NTN cell report the Mapped Cell ID of the area holding the UE
  // in place of its Uu Cell ID. Where none applies the Uu Cell ID stands: every cell outside NTN, and an NTN cell
  // whose areas name none for the UE position.
  asn1_user_location_info.nr_cgi.nr_cell_id.from_number(
      cu_cp_user_location_info.mapped_nci.value_or(cu_cp_user_location_info.nr_cgi.nci).value());
  asn1_user_location_info.nr_cgi.plmn_id = cu_cp_user_location_info.nr_cgi.plmn_id.to_bytes();
  // Fill TAI.
  asn1_user_location_info.tai.plmn_id = cu_cp_user_location_info.tai.plmn_id.to_bytes();
  asn1_user_location_info.tai.tac.from_number(cu_cp_user_location_info.tai.tac);
  // Fill timestamp.
  if (cu_cp_user_location_info.time_stamp.has_value()) {
    asn1_user_location_info.time_stamp_present = true;
    asn1_user_location_info.time_stamp.from_number(cu_cp_user_location_info.time_stamp.value());
  }
  // NR NTN TAI Information, TS 38.413 sec. 9.3.3.53: the TAI above carries one TAC, so report every broadcast TAC
  // here, plus the one derived from the UE location when known. Both are filled: an AMF that reads this IE ignores
  // the TAI, sec. 9.3.1.16, while an older one skips the IE and uses the TAI. Broadcasting several TACs is optional,
  // TS 38.300 sec. 16.14.3.1, so a single-TAC cell reports the IE too once a TAC is derived, sec. 16.14.5.
  if (not cu_cp_user_location_info.tac_list.empty() or cu_cp_user_location_info.ue_location_derived_tac.has_value()) {
    asn1_user_location_info.ie_exts_present                      = true;
    asn1_user_location_info.ie_exts.nr_ntn_tai_info_present      = true;
    asn1_user_location_info.ie_exts.nr_ntn_tai_info.serving_plmn = cu_cp_user_location_info.tai.plmn_id.to_bytes();

    // TS 38.331: a cell broadcasting a single TAC carries it in trackingAreaCode, which the CU-CP keeps in the TAI
    // alone. The TAC List in NR NTN takes one entry at least, TS 38.413 sec. 9.3.3.53, so report that TAC here.
    const tac_list_t single_tac{cu_cp_user_location_info.tai.tac};
    for (tac_t tac : cu_cp_user_location_info.tac_list.empty() ? single_tac : cu_cp_user_location_info.tac_list) {
      asn1::fixed_octstring<3, true> asn1_tac;
      asn1_tac.from_number(tac);
      asn1_user_location_info.ie_exts.nr_ntn_tai_info.tac_list_in_nr_ntn.push_back(asn1_tac);
    }

    // UE Location Derived TAC in NR NTN, TS 38.413 sec. 9.3.3.53: reported only when the UE location is known.
    if (cu_cp_user_location_info.ue_location_derived_tac.has_value()) {
      asn1_user_location_info.ie_exts.nr_ntn_tai_info.ue_location_derived_tac_in_nr_ntn_present = true;
      asn1_user_location_info.ie_exts.nr_ntn_tai_info.ue_location_derived_tac_in_nr_ntn.from_number(
          cu_cp_user_location_info.ue_location_derived_tac.value());
    }
  }

  return asn1_user_location_info;
}

/// \brief Convert ASN.1 cause to a human-readable string.
/// \param[in] cause The ASN.1 cause.
/// \return The humand-readable string.
inline std::string asn1_cause_to_string(const asn1::ngap::cause_c& cause)
{
  std::string cause_str;

  switch (cause.type()) {
    case asn1::ngap::cause_c::types_opts::radio_network:
      cause_str = cause.radio_network().to_string();
      break;
    case asn1::ngap::cause_c::types_opts::transport:
      cause_str = cause.transport().to_string();
      break;
    case asn1::ngap::cause_c::types_opts::protocol:
      cause_str = cause.protocol().to_string();
      break;
    case asn1::ngap::cause_c::types_opts::nas:
      cause_str = cause.nas().to_string();
      break;
    case asn1::ngap::cause_c::types_opts::misc:
      cause_str = cause.misc().to_string();
      break;
    default:
      cause_str = "unknown";
      break;
  }

  return cause_str;
}

/// \brief Convert common type Initial Context Setup Response message to NGAP Initial Context Setup Response message.
/// \param[out] resp The ASN1 NGAP Initial Context Setup Response message.
/// \param[in] cu_cp_resp The CU-CP Initial Context Setup Response message.
/// \return True on success, otherwise false.
template <typename template_asn1_item>
inline bool pdu_session_res_setup_response_item_to_asn1(template_asn1_item&                             asn1_resp,
                                                        const ngap_pdu_session_res_setup_response_item& resp)
{
  asn1_resp.pdu_session_id = to_underlying(resp.pdu_session_id);

  asn1::ngap::pdu_session_res_setup_resp_transfer_s response_transfer;

  // Fill DL QoS flow per TNL information.
  response_transfer.dl_qos_flow_per_tnl_info = cu_cp_qos_flow_per_tnl_info_to_ngap_qos_flow_per_tnl_info(
      resp.pdu_session_resource_setup_response_transfer.dlqos_flow_per_tnl_info);

  // Fill additional DL QoS flow per TNL information.
  for (const auto& cu_cp_qos_flow_info :
       resp.pdu_session_resource_setup_response_transfer.add_dl_qos_flow_per_tnl_info) {
    asn1::ngap::qos_flow_per_tnl_info_item_s ngap_qos_flow_info;
    ngap_qos_flow_info.qos_flow_per_tnl_info =
        cu_cp_qos_flow_per_tnl_info_to_ngap_qos_flow_per_tnl_info(cu_cp_qos_flow_info);
    response_transfer.add_dl_qos_flow_per_tnl_info.push_back(ngap_qos_flow_info);
  }

  // Fill QoS flow failed to setup list.
  for (const auto& failed_item : resp.pdu_session_resource_setup_response_transfer.qos_flow_failed_to_setup_list) {
    asn1::ngap::qos_flow_with_cause_item_s asn1_failed_item;
    asn1_failed_item.qos_flow_id = to_underlying(failed_item.qos_flow_id);
    asn1_failed_item.cause       = cause_to_asn1(failed_item.cause);
    response_transfer.qos_flow_failed_to_setup_list.push_back(asn1_failed_item);
  }

  // Fill security result.
  if (resp.pdu_session_resource_setup_response_transfer.security_result.has_value()) {
    response_transfer.security_result_present = true;
    response_transfer.security_result         = cu_cp_security_result_to_ngap_security_result(
        resp.pdu_session_resource_setup_response_transfer.security_result.value());
  }

  // Pack pdu_session_res_setup_resp_transfer_s.
  byte_buffer pdu = pack_into_pdu(response_transfer);

  if (!asn1_resp.pdu_session_res_setup_resp_transfer.resize(pdu.length())) {
    return false;
  }
  std::copy(pdu.begin(), pdu.end(), asn1_resp.pdu_session_res_setup_resp_transfer.begin());

  return true;
}

/// \brief Convert common type modify response item to ASN1 type message.
/// \param[out] asn1_resp The ASN1 NGAP struct.
/// \param[in] resp The common type struct.
/// \return True on success, otherwise false.
template <typename template_asn1_item>
inline bool pdu_session_res_modify_response_item_to_asn1(template_asn1_item& asn1_resp,
                                                         const ngap_pdu_session_resource_modify_response_item& resp)
{
  asn1_resp.pdu_session_id = to_underlying(resp.pdu_session_id);

  asn1::ngap::pdu_session_res_modify_resp_transfer_s response_transfer;

  // Fill additional DL QoS f low per TNL information.
  for (const auto& cu_cp_qos_flow_info : resp.transfer.add_dl_qos_flow_per_tnl_info) {
    asn1::ngap::qos_flow_per_tnl_info_item_s ngap_qos_flow_info;
    ngap_qos_flow_info.qos_flow_per_tnl_info =
        cu_cp_qos_flow_per_tnl_info_to_ngap_qos_flow_per_tnl_info(cu_cp_qos_flow_info);
    response_transfer.add_dl_qos_flow_per_tnl_info.push_back(ngap_qos_flow_info);
  }

  if (resp.transfer.qos_flow_add_or_modify_response_list.has_value()) {
    for (const auto& qos_flow : resp.transfer.qos_flow_add_or_modify_response_list.value()) {
      asn1::ngap::qos_flow_add_or_modify_resp_item_s asn1_item;
      asn1_item.qos_flow_id = to_underlying(qos_flow.qos_flow_id);
      response_transfer.qos_flow_add_or_modify_resp_list.push_back(asn1_item);
    }
  }

  // Fill the QoS flows that failed to be added or modified.
  for (const auto& qos_flow : resp.transfer.qos_flow_failed_to_add_or_modify_list) {
    asn1::ngap::qos_flow_with_cause_item_s asn1_item;
    asn1_item.qos_flow_id = to_underlying(qos_flow.qos_flow_id);
    asn1_item.cause       = cause_to_asn1(qos_flow.cause);
    response_transfer.qos_flow_failed_to_add_or_modify_list.push_back(asn1_item);
  }

  // Pack pdu_session_res_modify_resp_transfer_s.
  byte_buffer pdu = pack_into_pdu(response_transfer);

  if (!asn1_resp.pdu_session_res_modify_resp_transfer.resize(pdu.length())) {
    return false;
  }
  std::copy(pdu.begin(), pdu.end(), asn1_resp.pdu_session_res_modify_resp_transfer.begin());

  return true;
}

/// \brief Convert common type modify response item to ASN1 type message.
/// \param[out] asn1_resp The ASN1 NGAP struct.
/// \param[in] resp The common type struct.
/// \return True on success, otherwise false.
template <typename template_asn1_item>
inline bool pdu_session_res_failed_to_modify_item_to_asn1(template_asn1_item& asn1_resp,
                                                          const ngap_pdu_session_resource_failed_to_modify_item& resp)
{
  asn1_resp.pdu_session_id = to_underlying(resp.pdu_session_id);

  asn1::ngap::pdu_session_res_modify_unsuccessful_transfer_s response_transfer;
  response_transfer.cause = cause_to_asn1(resp.unsuccessful_transfer.cause);

  // Pack transfer.
  byte_buffer pdu = pack_into_pdu(response_transfer);

  if (!asn1_resp.pdu_session_res_modify_unsuccessful_transfer.resize(pdu.length())) {
    return false;
  }
  std::copy(pdu.begin(), pdu.end(), asn1_resp.pdu_session_res_modify_unsuccessful_transfer.begin());

  return true;
}

/// \brief Convert common type Initial Context Setup Response message to NGAP Initial Context Setup Response message.
/// \param[out] resp The ASN1 NGAP Initial Context Setup Response message.
/// \param[in] cu_cp_resp The CU-CP Initial Context Setup Response message.
/// \return True on success, otherwise false.
template <typename template_asn1_item>
inline bool pdu_session_res_setup_failed_item_to_asn1(template_asn1_item&                           asn1_resp,
                                                      const ngap_pdu_session_res_setup_failed_item& resp)
{
  asn1_resp.pdu_session_id = to_underlying(resp.pdu_session_id);

  asn1::ngap::pdu_session_res_setup_unsuccessful_transfer_s setup_unsuccessful_transfer;
  setup_unsuccessful_transfer.cause = cause_to_asn1(resp.unsuccessful_transfer.cause);

  // TODO: Add crit diagnostics.

  // Pack pdu_session_res_setup_unsuccessful_transfer_s.
  byte_buffer pdu = pack_into_pdu(setup_unsuccessful_transfer);

  if (!asn1_resp.pdu_session_res_setup_unsuccessful_transfer.resize(pdu.length())) {
    return false;
  }
  std::copy(pdu.begin(), pdu.end(), asn1_resp.pdu_session_res_setup_unsuccessful_transfer.begin());

  return true;
}

/// \brief Convert ASN.1 GUAMI to a common type.
/// \param[in] asn1_guami The ASN.1 GUAMI.
/// \return The common type GUAMI if the conversion is successful, otherwise an error message.
inline expected<guami_t, std::string> asn1_to_guami(const asn1::ngap::guami_s& asn1_guami)
{
  expected<plmn_identity> plmn_identity = plmn_identity::from_bytes(asn1_guami.plmn_id.to_bytes());
  if (!plmn_identity.has_value()) {
    return make_unexpected(
        fmt::format("Failed to convert GUAMI PLMN ID {} to common type", asn1_guami.plmn_id.to_string()));
  }

  guami_t guami;
  guami.plmn          = plmn_identity.value();
  guami.amf_region_id = asn1_guami.amf_region_id.to_number();
  guami.amf_set_id    = asn1_guami.amf_set_id.to_number();
  guami.amf_pointer   = asn1_guami.amf_pointer.to_number();

  return guami;
}

/// \brief Converts an ASN.1 overload action to the common type.
/// \param[in] asn1_action The ASN.1 overload action.
/// \return The overload action, or std::nullopt if the value is unknown.
inline std::optional<ngap_overload_action_t> asn1_to_overload_action(const asn1::ngap::overload_action_e& asn1_action)
{
  switch (asn1_action.value) {
    case asn1::ngap::overload_action_opts::reject_non_emergency_mo_dt:
      return ngap_overload_action_t::reject_non_emergency_mo_data;
    case asn1::ngap::overload_action_opts::reject_rrc_cr_sig:
      return ngap_overload_action_t::reject_signalling;
    case asn1::ngap::overload_action_opts::permit_emergency_sessions_and_mobile_terminated_services_only:
      return ngap_overload_action_t::only_emergency_and_mt;
    case asn1::ngap::overload_action_opts::permit_high_prio_sessions_and_mobile_terminated_services_only:
      return ngap_overload_action_t::only_high_priority_and_mt;
    default:
      return std::nullopt;
  }
}

/// \brief Converts an ASN.1 served GUAMI list to the common type.
/// \param[in] asn1_list The ASN.1 served GUAMI list, which \c validate_served_guami_list accepted.
/// \return The served GUAMI list.
inline std::vector<ngap_served_guami_item> asn1_to_served_guami_list(const asn1::ngap::served_guami_list_l& asn1_list)
{
  std::vector<ngap_served_guami_item> list;
  for (const auto& asn1_item : asn1_list) {
    ngap_served_guami_item item = {};
    item.guami                  = asn1_to_guami(asn1_item.guami).value();
    if (asn1_item.backup_amf_name_present) {
      item.backup_amf_name = asn1_item.backup_amf_name.to_string();
    }
    if (asn1_item.ie_exts_present and asn1_item.ie_exts.guami_type_present) {
      item.guami_type = asn1_item.ie_exts.guami_type.value == asn1::ngap::guami_type_opts::mapped
                            ? guami_type_t::mapped
                            : guami_type_t::native;
    }
    list.push_back(item);
  }
  return list;
}

/// \brief Converts an ASN.1 PLMN support list to the common type.
/// \param[in] asn1_list The ASN.1 PLMN support list, which \c validate_plmn_support_list accepted.
/// \return The PLMN support list.
inline std::vector<ngap_plmn_support_item> asn1_to_plmn_support_list(const asn1::ngap::plmn_support_list_l& asn1_list)
{
  std::vector<ngap_plmn_support_item> list;
  for (const auto& asn1_item : asn1_list) {
    ngap_plmn_support_item item = {};
    item.plmn_id                = plmn_identity::from_bytes(asn1_item.plmn_id.to_bytes()).value();
    for (const auto& asn1_slice_support_item : asn1_item.slice_support_list) {
      slice_support_item_t slice_support_item = {};
      slice_support_item.s_nssai.sst = slice_service_type{(uint8_t)asn1_slice_support_item.s_nssai.sst.to_number()};
      if (asn1_slice_support_item.s_nssai.sd_present) {
        slice_support_item.s_nssai.sd =
            slice_differentiator::create(asn1_slice_support_item.s_nssai.sd.to_number()).value();
      }
      item.slice_support_list.push_back(slice_support_item);
    }
    list.push_back(item);
  }
  return list;
}

/// \brief Converts type \c security_indication to an ASN.1 type.
/// \param[out] asn1obj ASN.1 object where the result of the conversion is stored.
/// \param[in] security_indication Security Indication IE contents.
inline void security_indication_to_asn1(asn1::ngap::security_ind_s& asn1obj, const security_indication_t& security_ind)
{
  switch (security_ind.integrity_protection_ind) {
    case integrity_protection_indication_t::not_needed:
    case integrity_protection_indication_t::preferred:
    case integrity_protection_indication_t::required:
      asn1obj.integrity_protection_ind.value =
          static_cast<asn1::ngap::integrity_protection_ind_opts::options>(security_ind.integrity_protection_ind);
      break;
    default:
      report_fatal_error("Cannot convert security indication to NGAP type");
  }

  switch (security_ind.confidentiality_protection_ind) {
    case confidentiality_protection_indication_t::not_needed:
    case confidentiality_protection_indication_t::preferred:
    case confidentiality_protection_indication_t::required:
      asn1obj.confidentiality_protection_ind.value =
          static_cast<asn1::ngap::confidentiality_protection_ind_opts::options>(security_ind.integrity_protection_ind);
      break;
    default:
      report_fatal_error("Cannot convert security indication to NGAP type");
  }
}

/// \brief Converts type \c security_indication to an ASN.1 type.
/// \param[out] asn1obj ASN.1 object where the result of the conversion is stored.
/// \param[in] security_indication Security Indication IE contents.
inline void asn1_to_security_indication(security_indication_t& security_ind, const asn1::ngap::security_ind_s& asn1obj)
{
  switch (asn1obj.integrity_protection_ind) {
    case asn1::ngap::integrity_protection_ind_opts::not_needed:
    case asn1::ngap::integrity_protection_ind_opts::preferred:
    case asn1::ngap::integrity_protection_ind_opts::required:
      security_ind.integrity_protection_ind =
          static_cast<integrity_protection_indication_t>(asn1obj.integrity_protection_ind.value);
      break;
    default:
      ocudulog::fetch_basic_logger("NGAP").error("Cannot convert security indication to NGAP type");
  }

  switch (asn1obj.confidentiality_protection_ind) {
    case asn1::ngap::confidentiality_protection_ind_opts::not_needed:
    case asn1::ngap::confidentiality_protection_ind_opts::preferred:
    case asn1::ngap::confidentiality_protection_ind_opts::required:
      security_ind.confidentiality_protection_ind =
          static_cast<confidentiality_protection_indication_t>(asn1obj.confidentiality_protection_ind.value);
      break;
    default:
      ocudulog::fetch_basic_logger("NGAP").error("Cannot convert security indication to NGAP type");
  }
}

/// \brief Converts ASN.1 handover type to NGAP handover type.
/// \param[out] handov_type NGAP handover type where the result of the conversion is stored.
/// \param[in] asn1_handov_type ASN.1 handover type.
inline void asn1_to_handov_type(ngap_handov_type& handov_type, const asn1::ngap::handov_type_e& asn1_handov_type)
{
  switch (asn1_handov_type) {
    case asn1::ngap::handov_type_opts::options::intra5gs:
      handov_type = ngap_handov_type::intra5gs;
      break;
    case asn1::ngap::handov_type_opts::options::fivegs_to_eps:
      handov_type = ngap_handov_type::fivegs_to_eps;
      break;
    case asn1::ngap::handov_type_opts::options::eps_to_5gs:
      handov_type = ngap_handov_type::eps_to_5gs;
      break;
    case asn1::ngap::handov_type_opts::options::fivegs_to_utran:
      handov_type = ngap_handov_type::fivegs_to_utran;
      break;
    default:
      // error
      ocudulog::fetch_basic_logger("NGAP").error("Cannot convert handov type to NGAP type");
  }
}

/// \brief Converts ASN.1 S-NSSAI type to common type.
/// \param[in] asn1_s_nssai The ASN.1 S-NSSAI type.
/// \return The common S-NSSAI type where the result of the conversion is stored.
inline s_nssai_t ngap_asn1_to_s_nssai(const asn1::ngap::s_nssai_s& asn1_s_nssai)
{
  s_nssai_t s_nssai;
  s_nssai.sst = slice_service_type{(uint8_t)asn1_s_nssai.sst.to_number()};
  if (asn1_s_nssai.sd_present) {
    s_nssai.sd = slice_differentiator::create(asn1_s_nssai.sd.to_number()).value();
  }

  return s_nssai;
}

/// \brief Converts common S-NSSAI type to ASN.1.
/// \param[out] s_nssai Common type S-NSSAI.
/// \return ASN.1 S-NSSAI type.
inline asn1::ngap::s_nssai_s s_nssai_to_asn1(const s_nssai_t& s_nssai)
{
  asn1::ngap::s_nssai_s asn1_s_nssai;
  asn1_s_nssai.sst.from_number(s_nssai.sst.value());

  if (s_nssai.sd.is_set()) {
    asn1_s_nssai.sd_present = true;
    asn1_s_nssai.sd.from_number(s_nssai.sd.value());
  }

  return asn1_s_nssai;
}

/// \brief Convert NGAP ASN.1 TAI to common type.
/// \param[in] asn1_tai The ASN.1 type TAI.
/// \return The common type TAI, or an error if the PLMN identity is invalid.
inline expected<tai_t> ngap_asn1_to_tai(const asn1::ngap::tai_s& asn1_tai)
{
  expected<plmn_identity> plmn_id = plmn_identity::from_bytes(asn1_tai.plmn_id.to_bytes());
  if (!plmn_id.has_value()) {
    ocudulog::fetch_basic_logger("NGAP").warning("Invalid PLMN ID {} in TAI", asn1_tai.plmn_id.to_string());
    return make_unexpected(default_error_t{});
  }

  tai_t tai;
  tai.plmn_id = plmn_id.value();
  tai.tac     = asn1_tai.tac.to_number();

  return tai;
}

/// \brief Convert NGAP ASN.1 security context to common type.
/// \param[out] sec_ctxt The common type security context.
/// \param[in] asn1_sec_cap The ASN.1 UE security capabilities.
/// \param[in] asn1_sec_ctxt The ASN.1 security context.
/// \return True on success, otherwise false.
inline bool asn1_to_security_context(security::security_context&           sec_ctxt,
                                     const asn1::ngap::ue_security_cap_s&  asn1_sec_cap,
                                     const asn1::ngap::security_context_s& asn1_sec_ctxt)
{
  security::asn1_to_key(sec_ctxt.k, asn1_sec_ctxt.next_hop_nh);
  sec_ctxt.ncc = asn1_sec_ctxt.next_hop_chaining_count;
  security::asn1_to_supported_algorithms(sec_ctxt.supported_int_algos, asn1_sec_cap.nr_integrity_protection_algorithms);
  security::asn1_to_supported_algorithms(sec_ctxt.supported_enc_algos, asn1_sec_cap.nr_encryption_algorithms);
  ocudulog::fetch_basic_logger("NGAP").debug(asn1_sec_ctxt.next_hop_nh.data(), 32, "K_gnb");
  ocudulog::fetch_basic_logger("NGAP").debug("Supported integrity algorithms: {}", sec_ctxt.supported_int_algos);
  ocudulog::fetch_basic_logger("NGAP").debug("Supported ciphering algorithms: {}", sec_ctxt.supported_enc_algos);

  return true;
}

/// \brief Convert NGAP ASN.1 to \c nr_cell_global_id_t.
/// \param[in] asn1_nr_cgi The ASN.1 type nr cgi.
/// \return The common type nr cgi, or an error if the PLMN identity is invalid.
inline expected<nr_cell_global_id_t> ngap_asn1_to_nr_cgi(const asn1::ngap::nr_cgi_s& asn1_nr_cgi)
{
  expected<plmn_identity> plmn_id = plmn_identity::from_bytes(asn1_nr_cgi.plmn_id.to_bytes());
  if (!plmn_id.has_value()) {
    ocudulog::fetch_basic_logger("NGAP").warning("Invalid PLMN ID {} in NR CGI", asn1_nr_cgi.plmn_id.to_string());
    return make_unexpected(default_error_t{});
  }

  nr_cell_global_id_t nr_cgi;

  // Fill NR cell ID. The 36-bit NR cell identity is always valid.
  nr_cgi.nci = nr_cell_identity::create(asn1_nr_cgi.nr_cell_id.to_number()).value();

  // Fill PLMN ID.
  nr_cgi.plmn_id = plmn_id.value();

  return nr_cgi;
}

/// \brief Convert \c nr_cell_global_id_t to NGAP ASN.1.
/// \param[in] nr_cgi The common type nr cgi.
/// \return The ASN.1 nr cgi.
inline asn1::ngap::nr_cgi_s nr_cgi_to_ngap_asn1(const nr_cell_global_id_t& nr_cgi)
{
  asn1::ngap::nr_cgi_s asn1_nr_cgi;

  // Fill NR cell ID.
  asn1_nr_cgi.nr_cell_id.from_number(nr_cgi.nci.value());

  // Fill PLMN ID.
  asn1_nr_cgi.plmn_id = nr_cgi.plmn_id.to_bytes();

  return asn1_nr_cgi;
}

/// \brief Convert NGAP ASN.1 Source NG-RAN Node to Target NG-RAN Node Transparent Container to common type.
/// \param[out] container     The common type container.
/// \param[in] asn1_container The ASN.1 container.
/// \return True on success, false if the container holds unsupported or invalid IEs.
inline bool asn1_to_source_to_target_transport_container(
    ngap_source_ngran_node_to_target_ngran_node_transparent_container&                container,
    const asn1::ngap::source_ngran_node_to_target_ngran_node_transparent_container_s& asn1_container)
{
  // Fill RRC container.
  container.rrc_container = asn1_container.rrc_container.copy();

  // Fill PDU session res info list.
  for (const auto& asn1_pdu_info_item : asn1_container.pdu_session_res_info_list) {
    cu_cp_pdu_session_res_info_item pdu_info_item;

    // Fill PDU session ID.
    pdu_info_item.pdu_session_id = uint_to_pdu_session_id(asn1_pdu_info_item.pdu_session_id);

    // Fill QoS flow info list.
    for (const auto& asn1_qos_flow : asn1_pdu_info_item.qos_flow_info_list) {
      cu_cp_qos_flow_info_item qos_flow;
      // Fill QoS flow ID.
      qos_flow.qos_flow_id = uint_to_qos_flow_id(asn1_qos_flow.qos_flow_id);

      // Fill DL forwarding.
      if (asn1_qos_flow.dl_forwarding_present) {
        qos_flow.dl_forwarding = asn1::enum_to_bool(asn1_qos_flow.dl_forwarding);
      }

      pdu_info_item.qos_flow_info_list.push_back(qos_flow);
    }

    // Fill DRBs to QoS flow map list.
    for (const auto& asn1_drbs_to_qos_flow_map : asn1_pdu_info_item.drbs_to_qos_flows_map_list) {
      cu_cp_drbs_to_qos_flows_map_item drbs_to_qos_flow_map;

      // Fill DRB ID.
      drbs_to_qos_flow_map.drb_id = uint_to_drb_id(asn1_drbs_to_qos_flow_map.drb_id);

      // Fill Associated QoS flow list.
      for (const auto& asn1_assoc_qos_flow : asn1_drbs_to_qos_flow_map.associated_qos_flow_list) {
        cu_cp_associated_qos_flow assoc_qos_flow;

        // Fill QoS flow ID.
        assoc_qos_flow.qos_flow_id = uint_to_qos_flow_id(asn1_assoc_qos_flow.qos_flow_id);

        // Fill QoS flow map indication.
        if (asn1_assoc_qos_flow.qos_flow_map_ind_present) {
          if (asn1_assoc_qos_flow.qos_flow_map_ind ==
              asn1::ngap::associated_qos_flow_item_s::qos_flow_map_ind_opts::options::ul) {
            assoc_qos_flow.qos_flow_map_ind = cu_cp_qos_flow_map_indication::ul;
          } else if (asn1_assoc_qos_flow.qos_flow_map_ind ==
                     asn1::ngap::associated_qos_flow_item_s::qos_flow_map_ind_opts::options::dl) {
            assoc_qos_flow.qos_flow_map_ind = cu_cp_qos_flow_map_indication::dl;
          }
        }

        drbs_to_qos_flow_map.associated_qos_flow_list.push_back(assoc_qos_flow);
      }

      pdu_info_item.drbs_to_qos_flows_map_list.push_back(drbs_to_qos_flow_map);
    }

    container.pdu_session_res_info_list.push_back(pdu_info_item);
  }

  // Fill ERAB info list.
  for (const auto& asn1_erab_info_item : asn1_container.erab_info_list) {
    ngap_erab_info_item erab_info_item;

    // Fill ERAB ID.
    erab_info_item.erab_id = asn1_erab_info_item.erab_id;

    // Fill DL forwarding.
    if (asn1_erab_info_item.dl_forwarding_present) {
      erab_info_item.dl_forwarding = asn1::enum_to_bool(asn1_erab_info_item.dl_forwarding);
    }

    container.erab_info_list.push_back(erab_info_item);
  }

  // Fill target cell ID.
  if (asn1_container.target_cell_id.type() != asn1::ngap::ngran_cgi_c::types_opts::nr_cgi) {
    ocudulog::fetch_basic_logger("NGAP").warning("Unsupported non-NR target cell ID in transparent container");
    return false;
  }
  expected<nr_cell_global_id_t> target_cell_id = ngap_asn1_to_nr_cgi(asn1_container.target_cell_id.nr_cgi());
  if (!target_cell_id.has_value()) {
    return false;
  }
  container.target_cell_id = target_cell_id.value();

  // Fill idx to RFSP.
  if (asn1_container.idx_to_rfsp_present) {
    container.idx_to_rfsp = asn1_container.idx_to_rfsp;
  }

  // Fill UE history info.
  for (const auto& asn1_last_item : asn1_container.ue_history_info) {
    ngap_last_visited_cell_item last_item;

    if (asn1_last_item.last_visited_cell_info.type() != asn1::ngap::last_visited_cell_info_c::types_opts::ngran_cell or
        asn1_last_item.last_visited_cell_info.ngran_cell().global_cell_id.type() !=
            asn1::ngap::ngran_cgi_c::types_opts::nr_cgi) {
      ocudulog::fetch_basic_logger("NGAP").debug("Ignoring non-NR cell in UE history information");
      // TODO: add handling for other types of last visited cells.
      continue;
    }
    const auto& asn1_cell_info = asn1_last_item.last_visited_cell_info.ngran_cell();

    // Fill last visited cell info.
    // Fill global cell ID.
    expected<nr_cell_global_id_t> global_cell_id = ngap_asn1_to_nr_cgi(asn1_cell_info.global_cell_id.nr_cgi());
    if (!global_cell_id.has_value()) {
      return false;
    }
    last_item.last_visited_cell_info.global_cell_id = global_cell_id.value();

    // Fill cell type.
    // Fill cell size.
    switch (asn1_cell_info.cell_type.cell_size) {
      case asn1::ngap::cell_size_opts::options::verysmall:
        last_item.last_visited_cell_info.cell_type.cell_size = ngap_cell_size::verysmall;
        break;
      case asn1::ngap::cell_size_opts::options::small:
        last_item.last_visited_cell_info.cell_type.cell_size = ngap_cell_size::small;
        break;
      case asn1::ngap::cell_size_opts::options::medium:
        last_item.last_visited_cell_info.cell_type.cell_size = ngap_cell_size::medium;
        break;
      case asn1::ngap::cell_size_opts::options::large:
        last_item.last_visited_cell_info.cell_type.cell_size = ngap_cell_size::large;
        break;
      default:
        // error
        ocudulog::fetch_basic_logger("NGAP").error("Cannot convert ASN.1 cell size to NGAP type");
    }

    // Fill time UE stayed in cell.
    last_item.last_visited_cell_info.time_ue_stayed_in_cell = asn1_cell_info.time_ue_stayed_in_cell;

    // Fill time UE stayed in cell enhanced granularity.
    if (asn1_cell_info.time_ue_stayed_in_cell_enhanced_granularity_present) {
      last_item.last_visited_cell_info.time_ue_stayed_in_cell_enhanced_granularity =
          asn1_cell_info.time_ue_stayed_in_cell_enhanced_granularity;
    }

    // Fill HO cause value.
    if (asn1_cell_info.ho_cause_value_present) {
      last_item.last_visited_cell_info.ho_cause_value = asn1_to_cause(asn1_cell_info.ho_cause_value);
    }

    container.ue_history_info.push_back(last_item);
  }

  return true;
}

/// \brief Convert NGAP ASN.1 to \c cu_cp_global_gnb_id.
/// \param[in] asn1_gnb_id The ASN.1 type global gnb id.
/// \return The common type global gnb id, or an error if the PLMN identity is invalid.
inline expected<cu_cp_global_gnb_id> ngap_asn1_to_global_gnb_id(const asn1::ngap::global_gnb_id_s& asn1_gnb_id)
{
  expected<plmn_identity> plmn_id = plmn_identity::from_bytes(asn1_gnb_id.plmn_id.to_bytes());
  if (!plmn_id.has_value()) {
    ocudulog::fetch_basic_logger("NGAP").warning("Invalid PLMN ID {} in global gNB ID",
                                                 asn1_gnb_id.plmn_id.to_string());
    return make_unexpected(default_error_t{});
  }

  cu_cp_global_gnb_id gnb_id;

  // Fill PLMN ID.
  gnb_id.plmn_id = plmn_id.value();

  // Fill gNB ID.
  gnb_id.gnb_id.id         = asn1_gnb_id.gnb_id.gnb_id().to_number();
  gnb_id.gnb_id.bit_length = asn1_gnb_id.gnb_id.gnb_id().length();

  return gnb_id;
}

/// \brief Convert common type PDU session resource admitted item to ASN.1.
/// \param[out] asn1_admitted_item The ASN.1 PDU session resource admitted item.
/// \param[in] admitted_item The common type PDU session resource admitted item.
/// \return True on success, otherwise false.
inline bool pdu_session_res_admitted_item_to_asn1(asn1::ngap::pdu_session_res_admitted_item_s&  asn1_admitted_item,
                                                  const cu_cp_ng_pdu_session_res_admitted_item& admitted_item)
{
  // Fill PDU session ID.
  asn1_admitted_item.pdu_session_id = to_underlying(admitted_item.pdu_session_id);

  // Fill HO request ACK transfer.
  asn1::ngap::ho_request_ack_transfer_s asn1_req_ack_transfer;

  // Fill DL NGU UP TNL info.
  up_transport_layer_info_to_asn1(asn1_req_ack_transfer.dl_ngu_up_tnl_info, admitted_item.dl_ngu_up_tnl_info);

  // Fill DL forwarding UP TNL info.
  if (admitted_item.dl_forwarding_up_tnl_info.has_value()) {
    asn1_req_ack_transfer.dl_forwarding_up_tnl_info_present = true;
    up_transport_layer_info_to_asn1(asn1_req_ack_transfer.dl_forwarding_up_tnl_info,
                                    admitted_item.dl_forwarding_up_tnl_info.value());
  }

  // Fill security result.
  if (admitted_item.security_result.has_value()) {
    asn1_req_ack_transfer.security_result_present = true;
    asn1_req_ack_transfer.security_result =
        cu_cp_security_result_to_ngap_security_result(admitted_item.security_result.value());
  }

  // Fill QoS flow setup resp list.
  for (const auto& qos_flow_item : admitted_item.qos_flows_setup_list) {
    asn1::ngap::qos_flow_item_with_data_forwarding_s asn1_qos_flow_item;

    // Fill QoS flow ID.
    asn1_qos_flow_item.qos_flow_id = to_underlying(qos_flow_item.qos_flow_id);

    // Fill data forwarding accepted.
    if (qos_flow_item.data_forwarding_accepted.has_value()) {
      asn1_qos_flow_item.data_forwarding_accepted_present = true;
      asn1::bool_to_enum(asn1_qos_flow_item.data_forwarding_accepted, qos_flow_item.data_forwarding_accepted.value());
    }

    asn1_req_ack_transfer.qos_flow_setup_resp_list.push_back(asn1_qos_flow_item);
  }

  // Fill QoS flow failed to setup list.
  for (const auto& qos_flow_failed_item : admitted_item.qos_flows_failed_to_setup_list) {
    if (!std::holds_alternative<ngap_cause_t>(qos_flow_failed_item.cause)) {
      return false;
    }
    asn1::ngap::qos_flow_with_cause_item_s asn1_qos_flow_failed_item;
    asn1_qos_flow_failed_item.qos_flow_id = to_underlying(qos_flow_failed_item.qos_flow_id);
    asn1_qos_flow_failed_item.cause       = cause_to_asn1(std::get<ngap_cause_t>(qos_flow_failed_item.cause));
    asn1_req_ack_transfer.qos_flow_failed_to_setup_list.push_back(asn1_qos_flow_failed_item);
  }

  // Fill data forwarding resp DRB list.
  for (const auto& drb_item : admitted_item.data_forwarding_info_from_target.data_forwarding_resp_drb_item_list) {
    asn1::ngap::data_forwarding_resp_drb_item_s asn1_drb_item;

    // Fill DRB ID.
    asn1_drb_item.drb_id = to_underlying(drb_item.drb_id);

    // Fill DL forwarding UP TNL info.
    if (drb_item.dl_forwarding_up_tnl.has_value()) {
      asn1_drb_item.dl_forwarding_up_tnl_info_present = true;
      up_transport_layer_info_to_asn1(asn1_drb_item.dl_forwarding_up_tnl_info, drb_item.dl_forwarding_up_tnl.value());
    }

    // Fill UL forwarding UP TNL info.
    if (drb_item.ul_forwarding_up_tnl.has_value()) {
      asn1_drb_item.ul_forwarding_up_tnl_info_present = true;
      up_transport_layer_info_to_asn1(asn1_drb_item.ul_forwarding_up_tnl_info, drb_item.ul_forwarding_up_tnl.value());
    }

    // Fill data forwarding resp DRB list.
    asn1_req_ack_transfer.data_forwarding_resp_drb_list.push_back(asn1_drb_item);
  }

  // Pack HO request ACK transfer.
  asn1_admitted_item.ho_request_ack_transfer = pack_into_pdu(asn1_req_ack_transfer);
  if (asn1_admitted_item.ho_request_ack_transfer.empty()) {
    ocudulog::fetch_basic_logger("NGAP").error("Error packing HO Request ACK transfer");
    return false;
  }

  return true;
}

/// \brief Convert common type PDU session resource failed to setup item to ASN.1.
/// \param[out] asn1_failed_item The ASN.1 PDU session resource failed to setup item.
/// \param[in] failed_item The common type PDU session resource failed to setup item.
/// \return True on success, otherwise false.
inline bool pdu_session_res_failed_to_setup_item_ho_ack_to_asn1(
    asn1::ngap::pdu_session_res_failed_to_setup_item_ho_ack_s& asn1_failed_item,
    const cu_cp_pdu_session_with_cause_item&                   failed_item)
{
  // Fill PDU session ID.
  asn1_failed_item.pdu_session_id = to_underlying(failed_item.pdu_session_id);

  // Fill HO res alloc unsuccessful transfer.
  asn1::ngap::ho_res_alloc_unsuccessful_transfer_s asn1_ho_res_alloc_unsuccessful_transfer;

  // Fill cause.
  if (!std::holds_alternative<ngap_cause_t>(failed_item.cause)) {
    return false;
  }
  asn1_ho_res_alloc_unsuccessful_transfer.cause = cause_to_asn1(std::get<ngap_cause_t>(failed_item.cause));

  // TODO: Add crit diagnostics

  // Pack HO res alloc unsuccessful transfer.
  asn1_failed_item.ho_res_alloc_unsuccessful_transfer = pack_into_pdu(asn1_ho_res_alloc_unsuccessful_transfer);
  if (asn1_failed_item.ho_res_alloc_unsuccessful_transfer.empty()) {
    ocudulog::fetch_basic_logger("NGAP").error("Error packing Ho Resource Alloc Unsuccessful transfer");
    return false;
  }

  return true;
}

/// \brief Convert common type target to source transport container to NGAP ASN.1.
/// \param[out] asn1_container The ASN.1 target to source transport container.
/// \param[in] container The common type target to source transport container.
/// \return True on success, otherwise false.
inline bool target_to_source_transport_container_to_asn1(byte_buffer& asn1_container, const byte_buffer& container)
{
  asn1::ngap::target_ngran_node_to_source_ngran_node_transparent_container_s asn1_container_struct;
  asn1_container_struct.rrc_container = container.copy();

  asn1_container = pack_into_pdu(asn1_container_struct);
  if (asn1_container.empty()) {
    ocudulog::fetch_basic_logger("NGAP").error("Error packing target to source transparent container");
    return false;
  }

  return true;
}

/// \brief Convert an NGAP ASN.1 Handover Command Transfer to \c cu_cp_data_forwarding_info_from_target_ng_ran_node.
/// \param[in] asn1_ho_cmd_transfer The ASN.1 Handover Command Transfer (TS 38.413 section 9.3.4.10).
/// \return The forwarding tunnels the source shall send the data it still holds for the UE to.
inline cu_cp_data_forwarding_info_from_target_ng_ran_node
asn1_to_data_forwarding_info_from_target(const asn1::ngap::ho_cmd_transfer_s& asn1_ho_cmd_transfer)
{
  cu_cp_data_forwarding_info_from_target_ng_ran_node data_forwarding_info;

  // Fill PDU session level DL forwarding UP TNL info.
  if (asn1_ho_cmd_transfer.dl_forwarding_up_tnl_info_present) {
    data_forwarding_info.pdu_session_level_dl_data_forwarding_info =
        asn1_to_up_transport_layer_info(asn1_ho_cmd_transfer.dl_forwarding_up_tnl_info);
  }

  // Fill QoS flows to be forwarded over the PDU session level tunnel.
  for (const auto& asn1_qos_flow : asn1_ho_cmd_transfer.qos_flow_to_be_forwarded_list) {
    data_forwarding_info.qos_flows_accepted_for_data_forwarding_list.push_back(
        uint_to_qos_flow_id(asn1_qos_flow.qos_flow_id));
  }

  // Fill data forwarding resp DRB list.
  for (const auto& asn1_drb_item : asn1_ho_cmd_transfer.data_forwarding_resp_drb_list) {
    cu_cp_data_forwarding_resp_drb_item drb_item;
    drb_item.drb_id = uint_to_drb_id(asn1_drb_item.drb_id);
    if (asn1_drb_item.dl_forwarding_up_tnl_info_present) {
      drb_item.dl_forwarding_up_tnl = asn1_to_up_transport_layer_info(asn1_drb_item.dl_forwarding_up_tnl_info);
    }
    if (asn1_drb_item.ul_forwarding_up_tnl_info_present) {
      drb_item.ul_forwarding_up_tnl = asn1_to_up_transport_layer_info(asn1_drb_item.ul_forwarding_up_tnl_info);
    }
    data_forwarding_info.data_forwarding_resp_drb_item_list.push_back(drb_item);
  }

  return data_forwarding_info;
}

/// \brief Convert NGAP ASN.1 to \c five_g_s_tmsi_t.
/// \param[in] asn1_ue_id The ASN.1 type ue paging ID.
/// \return The common type five_g_s_tmsi_t.
inline five_g_s_tmsi_t ngap_asn1_to_ue_paging_id(const asn1::ngap::ue_paging_id_c& asn1_ue_id)
{
  ocudu_assert(asn1_ue_id.type() == asn1::ngap::ue_paging_id_c::types_opts::five_g_s_tmsi, "Invalid UE paging ID type");

  return five_g_s_tmsi_t{asn1_ue_id.five_g_s_tmsi().amf_set_id.to_number(),
                         asn1_ue_id.five_g_s_tmsi().amf_pointer.to_number(),
                         asn1_ue_id.five_g_s_tmsi().five_g_tmsi.to_number()};
}

/// \brief Convert NGAP ASN.1 to \c gbr_qos_flow_information.
/// \param[in] asn1_gbr_qos_info The ASN.1 type gbr qos info.
/// \return The common type gbr qos flow information.
inline gbr_qos_flow_information
ngap_asn1_to_gbr_qos_flow_information(const asn1::ngap::gbr_qos_info_s& asn1_gbr_qos_info)
{
  gbr_qos_flow_information gbr_qos_info;
  gbr_qos_info.max_br_dl = asn1_gbr_qos_info.max_flow_bit_rate_dl;
  gbr_qos_info.max_br_ul = asn1_gbr_qos_info.max_flow_bit_rate_ul;
  gbr_qos_info.gbr_dl    = asn1_gbr_qos_info.guaranteed_flow_bit_rate_dl;
  gbr_qos_info.gbr_ul    = asn1_gbr_qos_info.guaranteed_flow_bit_rate_ul;
  if (asn1_gbr_qos_info.max_packet_loss_rate_dl_present) {
    gbr_qos_info.max_packet_loss_rate_dl = asn1_gbr_qos_info.max_packet_loss_rate_dl;
  }
  if (asn1_gbr_qos_info.max_packet_loss_rate_ul_present) {
    gbr_qos_info.max_packet_loss_rate_ul = asn1_gbr_qos_info.max_packet_loss_rate_ul;
  }

  return gbr_qos_info;
}

/// \brief Convert NGAP ASN.1 to \c pdu_session_type_t.
/// \param[out] pdu_session_type The common type pdu session type.
/// \param[in] ans1_pdu_session_type The ASN.1 type pdu session type.
/// \return True, if the conversion was successful, False otherwise.
inline bool asn1_to_pdu_session_type(pdu_session_type_t&                   pdu_session_type,
                                     const asn1::ngap::pdu_session_type_e& ans1_pdu_session_type)
{
  switch (ans1_pdu_session_type) {
    case asn1::ngap::pdu_session_type_e::pdu_session_type_opts::ipv4:
      pdu_session_type = pdu_session_type_t::ipv4;
      break;
    case asn1::ngap::pdu_session_type_e::pdu_session_type_opts::ipv6:
      pdu_session_type = pdu_session_type_t::ipv6;
      break;
    case asn1::ngap::pdu_session_type_e::pdu_session_type_opts::ipv4v6:
      pdu_session_type = pdu_session_type_t::ipv4v6;
      break;
    case asn1::ngap::pdu_session_type_e::pdu_session_type_opts::ethernet:
      pdu_session_type = pdu_session_type_t::ethernet;
      break;
    default:
      ocudulog::fetch_basic_logger("NGAP").error("Cannot convert ASN.1 PDU session type to common type");
      return false;
  }
  return true;
}

/// \brief Convert common type \c pdu_session_type_t to NGAP ASN.1.
/// \param[in] pdu_session_type The common type pdu session type.
/// \return The ASN.1 type pdu session type.
inline asn1::ngap::pdu_session_type_e pdu_session_type_to_asn1(const pdu_session_type_t& pdu_session_type)
{
  switch (pdu_session_type) {
    case pdu_session_type_t::ipv4:
      return asn1::ngap::pdu_session_type_e::ipv4;
    case pdu_session_type_t::ipv6:
      return asn1::ngap::pdu_session_type_e::ipv6;
    case pdu_session_type_t::ipv4v6:
      return asn1::ngap::pdu_session_type_e::ipv4v6;
    default:
      return asn1::ngap::pdu_session_type_e::ethernet;
  }
}

inline cu_cp_rrc_inactive_transition_report_request asn1_to_rrc_inactive_transition_report_request(
    const asn1::ngap::rrc_inactive_transition_report_request_e& asn1_rrc_inactive_transition_report_request)
{
  switch (asn1_rrc_inactive_transition_report_request) {
    case asn1::ngap::rrc_inactive_transition_report_request_opts::options::subsequent_state_transition_report:
      return cu_cp_rrc_inactive_transition_report_request::subsequent_state_transition_report;
    case asn1::ngap::rrc_inactive_transition_report_request_opts::options::single_rrc_connected_state_report:
      return cu_cp_rrc_inactive_transition_report_request::single_rrc_connected_state_report;
    default:
      return cu_cp_rrc_inactive_transition_report_request::cancel_report;
  }
}

/// \brief Convert ASN.1 periodic registration update timer to common type.
/// \param[in] asn1_periodic_registration_update_timer The ASN.1 periodic registration update timer.
/// \return The common type periodic registration update timer. If the timer is deactivated, an empty optional is
/// returned.
inline std::optional<std::chrono::seconds>
asn1_to_periodic_registration_update_timer(uint8_t asn1_periodic_registration_update_timer)
{
  std::chrono::seconds periodic_registration_update_timer;

  // 3GPP TS 38.413 section 9.3.3.24:
  // Bits 5 to 1 represent the binary coded timer value.
  // Bits 6 to 8 define the timer value unit for the Periodic Registration Update Timer as follows:
  //
  // Bits
  // 8 7 6
  // 0 0 0 value is incremented in multiples of 10 minutes
  // 0 0 1 value is incremented in multiples of 1 hour
  // 0 1 0 value is incremented in multiples of 10 hours
  // 0 1 1 value is incremented in multiples of 2 seconds
  // 1 0 0 value is incremented in multiples of 30 seconds
  // 1 0 1 value is incremented in multiples of 1 minute
  // 1 1 1 value indicates that the timer is deactivated.
  // 1 1 0 value is incremented in multiples of 1 hour in this version of the protocol.

  // Bits 5-1.
  uint8_t timer_value = asn1_periodic_registration_update_timer & 0x1f;

  // Bits 6-8.
  switch ((asn1_periodic_registration_update_timer >> 5) & 0x07) {
    case 0b000:
      // Timer value unit 10 minutes.
      periodic_registration_update_timer = std::chrono::seconds(timer_value * 600);
      break;
    case 0b001:
      // Timer value unit 1 hour.
      periodic_registration_update_timer = std::chrono::seconds(timer_value * 3600);
      break;
    case 0b010:
      // Timer value unit 10 hours.
      periodic_registration_update_timer = std::chrono::seconds(timer_value * 36000);
      break;
    case 0b011:
      // Timer value unit 2 seconds.
      periodic_registration_update_timer = std::chrono::seconds(timer_value * 2);
      break;
    case 0b100:
      // Timer value unit 30 seconds.
      periodic_registration_update_timer = std::chrono::seconds(timer_value * 30);
      break;
    case 0b101:
      // Timer value unit 1 minute.
      periodic_registration_update_timer = std::chrono::seconds(timer_value * 60);
      break;
    case 0b110:
      // Timer value unit 1 hour.
      periodic_registration_update_timer = std::chrono::seconds(timer_value * 3600);
      break;
    case 0b111:
      // Timer is deactivated.
      return std::nullopt;
    default:
      // Timer is deactivated.
      return std::nullopt;
  }

  return periodic_registration_update_timer;
}

/// \brief Convert NGAP ASN.1 Core Network Assistance Information for RRC INACTIVE to common type.
/// \return The common type assistance information, or an error if a TAI is invalid.
inline expected<cu_cp_core_network_assist_info_for_inactive> asn1_to_core_network_assist_info_for_inactive(
    const asn1::ngap::core_network_assist_info_for_inactive_s& asn1_cn_assist_info_for_inactive)
{
  cu_cp_core_network_assist_info_for_inactive cn_assist_info_for_inactive;

  if (asn1_cn_assist_info_for_inactive.ue_id_idx_value.type() ==
      asn1::ngap::ue_id_idx_value_c::types_opts::options::idx_len10) {
    cn_assist_info_for_inactive.ue_id_idx_value =
        asn1_cn_assist_info_for_inactive.ue_id_idx_value.idx_len10().to_number();
  }
  if (asn1_cn_assist_info_for_inactive.ue_specific_drx_present) {
    cn_assist_info_for_inactive.ue_specific_drx = asn1_cn_assist_info_for_inactive.ue_specific_drx.to_number();
  }
  cn_assist_info_for_inactive.periodic_registration_update_timer = asn1_to_periodic_registration_update_timer(
      asn1_cn_assist_info_for_inactive.periodic_regist_upd_timer.to_number());
  cn_assist_info_for_inactive.mico_mode_ind =
      asn1_cn_assist_info_for_inactive.mico_mode_ind_present &&
      (asn1_cn_assist_info_for_inactive.mico_mode_ind == asn1::ngap::mico_mode_ind_opts::options::true_value);
  for (const auto& tai_list_for_inactive_item : asn1_cn_assist_info_for_inactive.tai_list_for_inactive) {
    expected<tai_t> tai = ngap_asn1_to_tai(tai_list_for_inactive_item.tai);
    if (!tai.has_value()) {
      return make_unexpected(default_error_t{});
    }
    cn_assist_info_for_inactive.tai_list_for_inactive.push_back(tai.value());
  }

  if (asn1_cn_assist_info_for_inactive.ie_exts_present) {
    cn_assist_info_for_inactive.ie_exts.emplace();
    if (asn1_cn_assist_info_for_inactive.ie_exts.extended_ue_id_idx_value_present) {
      cn_assist_info_for_inactive.ie_exts->extended_ue_idx_value =
          asn1_cn_assist_info_for_inactive.ie_exts.extended_ue_id_idx_value.to_number();
    }
    if (asn1_cn_assist_info_for_inactive.ie_exts.nr_paginge_drx_info_present) {
      cn_assist_info_for_inactive.ie_exts->nr_paging_drx_info.emplace();
      cn_assist_info_for_inactive.ie_exts->nr_paging_drx_info->nr_paging_edrx_cycle =
          asn1_cn_assist_info_for_inactive.ie_exts.nr_paginge_drx_info.nr_paging_e_drx_cycle.to_number();
      if (asn1_cn_assist_info_for_inactive.ie_exts.nr_paginge_drx_info.nr_paging_time_win_present) {
        cn_assist_info_for_inactive.ie_exts->nr_paging_drx_info->nr_paging_time_window =
            asn1_cn_assist_info_for_inactive.ie_exts.nr_paginge_drx_info.nr_paging_time_win.to_number();
      }
    }
  }

  return cn_assist_info_for_inactive;
}

inline location_report_request::event_type
asn1_to_location_reporting_event_type(const asn1::ngap::event_type_e& asn1_reporting_trigger)
{
  switch (asn1_reporting_trigger) {
    case asn1::ngap::event_type_opts::options::direct:
      return location_report_request::event_type::direct;
    case asn1::ngap::event_type_opts::options::change_of_serve_cell:
      return location_report_request::event_type::change_of_serve_cell;
    case asn1::ngap::event_type_opts::options::ue_presence_in_area_of_interest:
      return location_report_request::event_type::ue_presence_in_area_of_interest;
    case asn1::ngap::event_type_opts::options::stop_change_of_serve_cell:
      return location_report_request::event_type::stop_change_of_serve_cell;
    case asn1::ngap::event_type_opts::options::stop_ue_presence_in_area_of_interest:
      return location_report_request::event_type::stop_ue_presence_in_area_of_interest;
    case asn1::ngap::event_type_opts::options::cancel_location_report_for_the_ue:
      return location_report_request::event_type::cancel_location_report_for_the_ue;
    case asn1::ngap::event_type_opts::options::change_of_serving_cell_and_ue_presence_in_the_area_of_interest:
      return location_report_request::event_type::change_of_serving_cell_and_ue_presence_in_the_area_of_interest;
    default:
      return location_report_request::event_type::nulltype;
  }
}

/// \brief Convert NGAP ASN1 AreaOfInterest IE to common type.
/// \return The common type area of interest, or an error if a TAI, CGI or RAN node ID is invalid.
inline expected<area_of_interest> asn1_to_area_of_interest(const asn1::ngap::area_of_interest_s& asn1_aoi)
{
  area_of_interest aoi;

  for (const auto& asn1_tai_item : asn1_aoi.area_of_interest_tai_list) {
    expected<tai_t> tai = ngap_asn1_to_tai(asn1_tai_item.tai);
    if (!tai.has_value()) {
      return make_unexpected(default_error_t{});
    }
    aoi.tai_list.push_back(tai.value());
  }

  for (const auto& asn1_cell_item : asn1_aoi.area_of_interest_cell_list) {
    if (asn1_cell_item.ngran_cgi.type() != asn1::ngap::ngran_cgi_c::types_opts::nr_cgi) {
      ocudulog::fetch_basic_logger("NGAP").warning("Ignoring non-NR CGI in AreaOfInterest cell list");
      // TODO: add handling for other types of CGIs
      continue;
    }
    expected<nr_cell_global_id_t> nr_cgi = ngap_asn1_to_nr_cgi(asn1_cell_item.ngran_cgi.nr_cgi());
    if (!nr_cgi.has_value()) {
      return make_unexpected(default_error_t{});
    }
    aoi.cell_list.push_back(nr_cgi.value());
  }

  for (const auto& asn1_ran_node_item : asn1_aoi.area_of_interest_ran_node_list) {
    if (asn1_ran_node_item.global_ran_node_id.type() != asn1::ngap::global_ran_node_id_c::types_opts::global_gnb_id) {
      ocudulog::fetch_basic_logger("NGAP").warning("Ignoring non-gNB RAN node ID in AreaOfInterest RAN node list");
      // TODO: add handling for other types of RAN Node IDs
      continue;
    }

    expected<cu_cp_global_gnb_id> gnb_id =
        ngap_asn1_to_global_gnb_id(asn1_ran_node_item.global_ran_node_id.global_gnb_id());
    if (!gnb_id.has_value()) {
      return make_unexpected(default_error_t{});
    }
    aoi.ran_node_list.push_back(gnb_id.value());
  }

  return aoi;
}

/// \brief Convert NGAP ASN1 LocationReportingRequestType IE to common type.
/// \return The common type request, or an error if an area of interest is invalid.
inline expected<location_report_request>
asn1_to_location_report_request(const asn1::ngap::location_report_request_type_s& asn1_type)
{
  location_report_request req;
  req.location_reporting_type = asn1_to_location_reporting_event_type(asn1_type.event_type);
  req.location_report_area    = location_report_request::report_area::cell;

  if (req.location_reporting_type == location_report_request::event_type::ue_presence_in_area_of_interest ||
      req.location_reporting_type ==
          location_report_request::event_type::change_of_serving_cell_and_ue_presence_in_the_area_of_interest) {
    for (const auto& asn1_aoi_item : asn1_type.area_of_interest_list) {
      area_of_interest_item aoi_item;
      aoi_item.location_report_ref_id = asn1_aoi_item.location_report_ref_id;
      expected<area_of_interest> aoi  = asn1_to_area_of_interest(asn1_aoi_item.area_of_interest);
      if (!aoi.has_value()) {
        return make_unexpected(default_error_t{});
      }
      aoi_item.aio = std::move(aoi.value());
      req.area_of_interest_list.push_back(std::move(aoi_item));
    }
  }

  if (req.location_reporting_type == location_report_request::event_type::stop_ue_presence_in_area_of_interest) {
    if (asn1_type.location_report_ref_id_to_be_cancelled_present) {
      req.location_report_ref_id_to_be_cancelled = asn1_type.location_report_ref_id_to_be_cancelled;
    }

    if (asn1_type.ie_exts_present && asn1_type.ie_exts.add_cancelledlocation_report_ref_id_list_present) {
      for (const auto& item : asn1_type.ie_exts.add_cancelledlocation_report_ref_id_list) {
        req.additional_location_report_ref_ids_to_be_cancelled.push_back(item.location_report_ref_id_to_be_cancelled);
      }
    }
  }

  return req;
}

/// \brief Convert common type event_type to NGAP ASN1 event_type_e.
inline asn1::ngap::event_type_e event_type_to_asn1(location_report_request::event_type event_type)
{
  switch (event_type) {
    case location_report_request::event_type::direct:
      return asn1::ngap::event_type_opts::options::direct;
    case location_report_request::event_type::change_of_serve_cell:
      return asn1::ngap::event_type_opts::options::change_of_serve_cell;
    case location_report_request::event_type::ue_presence_in_area_of_interest:
      return asn1::ngap::event_type_opts::options::ue_presence_in_area_of_interest;
    case location_report_request::event_type::stop_change_of_serve_cell:
      return asn1::ngap::event_type_opts::options::stop_change_of_serve_cell;
    case location_report_request::event_type::stop_ue_presence_in_area_of_interest:
      return asn1::ngap::event_type_opts::options::stop_ue_presence_in_area_of_interest;
    case location_report_request::event_type::cancel_location_report_for_the_ue:
      return asn1::ngap::event_type_opts::options::cancel_location_report_for_the_ue;
    case location_report_request::event_type::change_of_serving_cell_and_ue_presence_in_the_area_of_interest:
      return asn1::ngap::event_type_opts::options::change_of_serving_cell_and_ue_presence_in_the_area_of_interest;
    default:
      return asn1::ngap::event_type_opts::options::direct;
  }
}

/// \brief Convert common type ue_presence to NGAP ASN1 ue_presence_e.
inline asn1::ngap::ue_presence_e ue_presence_to_asn1(ue_presence ue_presence)
{
  switch (ue_presence) {
    case ue_presence::in:
      return asn1::ngap::ue_presence_opts::options::in;
    case ue_presence::out:
      return asn1::ngap::ue_presence_opts::options::out;
    default:
      return asn1::ngap::ue_presence_opts::options::unknown;
  }
}

/// \brief Convert common type AreaOfInterest to NGAP ASN1 area_of_interest_s.
inline asn1::ngap::area_of_interest_s area_of_interest_to_asn1(const area_of_interest& aoi)
{
  asn1::ngap::area_of_interest_s asn1_aoi;

  for (const auto& tai : aoi.tai_list) {
    asn1::ngap::area_of_interest_tai_item_s asn1_tai_item;
    asn1_tai_item.tai.plmn_id = tai.plmn_id.to_bytes();
    asn1_tai_item.tai.tac.from_number(tai.tac);
    asn1_aoi.area_of_interest_tai_list.push_back(asn1_tai_item);
  }

  for (const auto& cgi : aoi.cell_list) {
    asn1::ngap::area_of_interest_cell_item_s asn1_cell_item;
    auto&                                    asn1_nr_cgi = asn1_cell_item.ngran_cgi.set_nr_cgi();
    asn1_nr_cgi.nr_cell_id.from_number(cgi.nci.value());
    asn1_nr_cgi.plmn_id = cgi.plmn_id.to_bytes();
    asn1_aoi.area_of_interest_cell_list.push_back(asn1_cell_item);
  }

  for (const auto& gnb : aoi.ran_node_list) {
    asn1::ngap::area_of_interest_ran_node_item_s asn1_ran_node_item;
    auto&                                        asn1_gnb = asn1_ran_node_item.global_ran_node_id.set_global_gnb_id();
    asn1_gnb.plmn_id                                      = gnb.plmn_id.to_bytes();
    asn1_gnb.gnb_id.set_gnb_id().from_number(gnb.gnb_id.id, gnb.gnb_id.bit_length);
    asn1_aoi.area_of_interest_ran_node_list.push_back(asn1_ran_node_item);
  }

  return asn1_aoi;
}

/// \brief Convert common type LocationReportingRequestType to NGAP ASN1 location_report_request_type_s.
inline asn1::ngap::location_report_request_type_s location_report_request_to_asn1(const location_report_request& req)
{
  asn1::ngap::location_report_request_type_s asn1_req;

  asn1_req.event_type  = event_type_to_asn1(req.location_reporting_type);
  asn1_req.report_area = asn1::ngap::report_area_opts::options::cell;

  for (const auto& aoi_item : req.area_of_interest_list) {
    asn1::ngap::area_of_interest_item_s asn1_aoi_item;
    asn1_aoi_item.location_report_ref_id = aoi_item.location_report_ref_id;
    asn1_aoi_item.area_of_interest       = area_of_interest_to_asn1(aoi_item.aio);
    asn1_req.area_of_interest_list.push_back(asn1_aoi_item);
  }

  if (req.location_report_ref_id_to_be_cancelled.has_value()) {
    asn1_req.location_report_ref_id_to_be_cancelled_present = true;
    asn1_req.location_report_ref_id_to_be_cancelled         = req.location_report_ref_id_to_be_cancelled.value();
  }

  return asn1_req;
}

} // namespace ocudu::ocucp
