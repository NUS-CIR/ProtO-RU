// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ngap_test_messages.h"
#include "lib/ngap/ngap_asn1_converters.h"
#include "ocudu/asn1/asn1_utils.h"
#include "ocudu/asn1/ngap/common.h"
#include "ocudu/asn1/ngap/ngap_ies.h"
#include "ocudu/asn1/ngap/ngap_pdu_contents.h"
#include "ocudu/ngap/ngap_handover.h"
#include "ocudu/ngap/ngap_message.h"
#include "ocudu/ngap/ngap_types.h"
#include "ocudu/ran/cu_cp_types.h"
#include "ocudu/ran/cu_types.h"
#include "ocudu/ran/plmn_identity.h"
#include <vector>

using namespace ocudu;
using namespace ocucp;
using namespace asn1::ngap;

bool ocudu::ocucp::is_same_pdu_type(const ngap_message& lhs, const ngap_message& rhs)
{
  if (lhs.pdu.type().value != rhs.pdu.type().value) {
    return false;
  }
  switch (lhs.pdu.type().value) {
    case ngap_pdu_c::types_opts::init_msg:
      return lhs.pdu.init_msg().value.type().value == rhs.pdu.init_msg().value.type().value;
    case ngap_pdu_c::types_opts::successful_outcome:
      return lhs.pdu.successful_outcome().value.type().value == rhs.pdu.successful_outcome().value.type().value;
    case ngap_pdu_c::types_opts::unsuccessful_outcome:
      return lhs.pdu.unsuccessful_outcome().value.type().value == rhs.pdu.unsuccessful_outcome().value.type().value;
    default:
      break;
  }
  return false;
}

bool ocudu::ocucp::is_pdu_type(const ngap_message& pdu, const asn1::ngap::ngap_elem_procs_o::init_msg_c::types type)
{
  if (pdu.pdu.type().value != ngap_pdu_c::types_opts::init_msg) {
    return false;
  }
  return pdu.pdu.init_msg().value.type().value == type;
}

bool ocudu::ocucp::is_pdu_type(const ngap_message&                                              pdu,
                               const asn1::ngap::ngap_elem_procs_o::successful_outcome_c::types type)
{
  if (pdu.pdu.type().value != ngap_pdu_c::types_opts::successful_outcome) {
    return false;
  }
  return pdu.pdu.successful_outcome().value.type().value == type;
}

ngap_message ocudu::ocucp::generate_ng_setup_response(plmn_identity plmn,
                                                      uint8_t       amf_pointer,
                                                      uint16_t      relative_amf_capacity,
                                                      uint16_t      amf_set_id)
{
  ngap_message ng_setup_response = {};

  ng_setup_response.pdu.set_successful_outcome();
  ng_setup_response.pdu.successful_outcome().load_info_obj(ASN1_NGAP_ID_NG_SETUP);

  auto& setup_res = ng_setup_response.pdu.successful_outcome().value.ng_setup_resp();
  setup_res->amf_name.from_string("open5gs-amf0");

  served_guami_item_s served_guami_item;
  served_guami_item.guami.plmn_id = plmn.to_bytes();
  served_guami_item.guami.amf_region_id.from_number(2);
  served_guami_item.guami.amf_set_id.from_number(amf_set_id);
  served_guami_item.guami.amf_pointer.from_number(amf_pointer);
  setup_res->served_guami_list.push_back(served_guami_item);

  setup_res->relative_amf_capacity = relative_amf_capacity;

  plmn_support_item_s plmn_support_item = {};
  plmn_support_item.plmn_id             = plmn.to_bytes();

  slice_support_item_s slice_support_item = {};
  slice_support_item.s_nssai.sst.from_number(1);
  plmn_support_item.slice_support_list.push_back(slice_support_item);

  setup_res->plmn_support_list.push_back(plmn_support_item);

  return ng_setup_response;
}

ngap_message ocudu::ocucp::generate_amf_configuration_update()
{
  ngap_message msg;
  msg.pdu.set_init_msg().load_info_obj(ASN1_NGAP_ID_AMF_CFG_UPD);
  return msg;
}

ngap_message ocudu::ocucp::generate_amf_status_indication(const guami_t&                    guami,
                                                          const std::optional<std::string>& backup_amf_name)
{
  ngap_message msg;
  msg.pdu.set_init_msg().load_info_obj(ASN1_NGAP_ID_AMF_STATUS_IND);

  unavailable_guami_item_s item;
  item.guami.plmn_id = guami.plmn.to_bytes();
  item.guami.amf_region_id.from_number(guami.amf_region_id);
  item.guami.amf_set_id.from_number(guami.amf_set_id);
  item.guami.amf_pointer.from_number(guami.amf_pointer);
  if (backup_amf_name.has_value()) {
    item.backup_amf_name_present = true;
    item.backup_amf_name.from_string(*backup_amf_name);
  }

  msg.pdu.init_msg().value.amf_status_ind()->unavailable_guami_list.push_back(item);
  return msg;
}

ngap_message
ocudu::ocucp::generate_overload_start(std::optional<asn1::ngap::overload_action_opts::options> overload_action,
                                      std::optional<uint8_t>                                   traffic_load_reduction)
{
  ngap_message msg;
  msg.pdu.set_init_msg().load_info_obj(ASN1_NGAP_ID_OVERLOAD_START);
  auto& overload_start = msg.pdu.init_msg().value.overload_start();
  if (overload_action.has_value()) {
    overload_start->amf_overload_resp_present                     = true;
    overload_start->amf_overload_resp.set_overload_action().value = *overload_action;
  }
  if (traffic_load_reduction.has_value()) {
    overload_start->amf_traffic_load_reduction_ind_present = true;
    overload_start->amf_traffic_load_reduction_ind         = *traffic_load_reduction;
  }
  return msg;
}

ngap_message ocudu::ocucp::generate_overload_stop()
{
  ngap_message msg;
  msg.pdu.set_init_msg().load_info_obj(ASN1_NGAP_ID_OVERLOAD_STOP);
  return msg;
}

ngap_message ocudu::ocucp::generate_reroute_nas_request(ran_ue_id_t         ran_ue_id,
                                                        const ngap_message& initial_ue_message,
                                                        uint16_t            amf_set_id)
{
  ngap_message msg;
  msg.pdu.set_init_msg().load_info_obj(ASN1_NGAP_ID_REROUTE_NAS_REQUEST);
  auto& reroute           = msg.pdu.init_msg().value.reroute_nas_request();
  reroute->ran_ue_ngap_id = to_underlying(ran_ue_id);
  reroute->amf_set_id.from_number(amf_set_id);

  byte_buffer   packed{byte_buffer::fallback_allocation_tag{}};
  asn1::bit_ref bref(packed);
  report_fatal_error_if_not(initial_ue_message.pdu.pack(bref) == asn1::OCUDUASN_SUCCESS,
                            "Failed to pack the InitialUEMessage");
  reroute->ngap_msg = std::move(packed);
  return msg;
}

ngap_message ocudu::ocucp::generate_ng_setup_failure()
{
  ngap_message ng_setup_failure = {};

  ng_setup_failure.pdu.set_unsuccessful_outcome();
  ng_setup_failure.pdu.unsuccessful_outcome().load_info_obj(ASN1_NGAP_ID_NG_SETUP);

  auto& setup_fail = ng_setup_failure.pdu.unsuccessful_outcome().value.ng_setup_fail();
  setup_fail->cause.set_radio_network();
  setup_fail->cause.radio_network()    = cause_radio_network_opts::options::unspecified;
  setup_fail->time_to_wait_present     = false;
  setup_fail->crit_diagnostics_present = false;
  // TODO: Add critical diagnostics.

  return ng_setup_failure;
}

ngap_message ocudu::ocucp::generate_ng_setup_failure_with_bad_plmn(time_to_wait_e time_to_wait)
{
  ngap_message ng_setup_failure = {};

  ng_setup_failure.pdu.set_unsuccessful_outcome();
  ng_setup_failure.pdu.unsuccessful_outcome().load_info_obj(ASN1_NGAP_ID_NG_SETUP);

  auto& setup_fail = ng_setup_failure.pdu.unsuccessful_outcome().value.ng_setup_fail();
  setup_fail->cause.set_misc();
  setup_fail->cause.misc()             = asn1::ngap::cause_misc_opts::unknown_plmn_or_sn_pn;
  setup_fail->time_to_wait_present     = true;
  setup_fail->time_to_wait.value       = time_to_wait;
  setup_fail->crit_diagnostics_present = false;
  // Fill critical diagnostics.

  return ng_setup_failure;
}

ngap_message ocudu::ocucp::generate_ng_setup_failure_with_time_to_wait(time_to_wait_e time_to_wait)
{
  ngap_message ng_setup_failure = generate_ng_setup_failure();

  auto& setup_fail                 = ng_setup_failure.pdu.unsuccessful_outcome().value.ng_setup_fail();
  setup_fail->time_to_wait_present = true;
  setup_fail->time_to_wait.value   = time_to_wait;

  return ng_setup_failure;
}

cu_cp_initial_ue_message ocudu::ocucp::generate_initial_ue_message(cu_cp_ue_index_t ue_index)
{
  cu_cp_initial_ue_message msg = {};
  msg.ue_index                 = ue_index;
  (void)msg.nas_pdu.resize(nas_pdu_len);
  msg.establishment_cause               = establishment_cause_t::mo_sig;
  msg.user_location_info.nr_cgi.plmn_id = plmn_identity::test_value();
  msg.user_location_info.nr_cgi.nci     = nr_cell_identity::create(gnb_id_t{411, 22}, 0).value();
  msg.user_location_info.tai.plmn_id    = plmn_identity::test_value();
  msg.user_location_info.tai.tac        = 7;
  return msg;
}

ngap_message
ocudu::ocucp::generate_downlink_nas_transport_message(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id, byte_buffer nas_pdu)
{
  ngap_message dl_nas_transport = {};

  dl_nas_transport.pdu.set_init_msg();
  dl_nas_transport.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_DL_NAS_TRANSPORT);

  auto& dl_nas_transport_msg           = dl_nas_transport.pdu.init_msg().value.dl_nas_transport();
  dl_nas_transport_msg->amf_ue_ngap_id = to_underlying(amf_ue_id);
  dl_nas_transport_msg->ran_ue_ngap_id = to_underlying(ran_ue_id);
  if (nas_pdu.empty()) {
    (void)dl_nas_transport_msg->nas_pdu.resize(nas_pdu_len);
  } else {
    dl_nas_transport_msg->nas_pdu = nas_pdu.copy();
  }

  return dl_nas_transport;
}

ngap_message ocudu::ocucp::generate_downlink_nas_transport_message_with_ue_cap_info_request(amf_ue_id_t amf_ue_id,
                                                                                            ran_ue_id_t ran_ue_id,
                                                                                            byte_buffer nas_pdu)
{
  ngap_message dl_nas_transport = generate_downlink_nas_transport_message(amf_ue_id, ran_ue_id, std::move(nas_pdu));

  auto& dl_nas_transport_msg                        = dl_nas_transport.pdu.init_msg().value.dl_nas_transport();
  dl_nas_transport_msg->ue_cap_info_request_present = true;
  dl_nas_transport_msg->ue_cap_info_request         = ue_cap_info_request_opts::options::requested;

  return dl_nas_transport;
}

cu_cp_ul_nas_transport ocudu::ocucp::generate_ul_nas_transport_message(cu_cp_ue_index_t ue_index)
{
  cu_cp_ul_nas_transport ul_nas_transport = {};
  ul_nas_transport.ue_index               = ue_index;
  (void)ul_nas_transport.nas_pdu.resize(nas_pdu_len);
  ul_nas_transport.user_location_info.nr_cgi.plmn_id = plmn_identity::test_value();
  ul_nas_transport.user_location_info.nr_cgi.nci     = nr_cell_identity::create(gnb_id_t{411, 22}, 0).value();
  ul_nas_transport.user_location_info.tai.plmn_id    = plmn_identity::test_value();
  ul_nas_transport.user_location_info.tai.tac        = 7;

  return ul_nas_transport;
}

ngap_message ocudu::ocucp::generate_uplink_nas_transport_message(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id)
{
  ngap_message ul_nas_transport = {};

  ul_nas_transport.pdu.set_init_msg();
  ul_nas_transport.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UL_NAS_TRANSPORT);

  auto& ul_nas_transport_msg           = ul_nas_transport.pdu.init_msg().value.ul_nas_transport();
  ul_nas_transport_msg->amf_ue_ngap_id = to_underlying(amf_ue_id);
  ul_nas_transport_msg->ran_ue_ngap_id = to_underlying(ran_ue_id);
  (void)ul_nas_transport_msg->nas_pdu.resize(nas_pdu_len);

  auto& user_loc_info_nr = ul_nas_transport_msg->user_location_info.set_user_location_info_nr();
  user_loc_info_nr.nr_cgi.plmn_id.from_string("00f110");
  user_loc_info_nr.nr_cgi.nr_cell_id.from_number(nr_cell_identity::create(gnb_id_t{411, 22}, 0).value().value());
  user_loc_info_nr.tai.plmn_id.from_string("00f110");
  user_loc_info_nr.tai.tac.from_number(7);

  return ul_nas_transport;
}

ngap_message ocudu::ocucp::generate_initial_context_setup_request_base(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = {};

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_INIT_CONTEXT_SETUP);

  auto& init_context_setup_req           = ngap_msg.pdu.init_msg().value.init_context_setup_request();
  init_context_setup_req->amf_ue_ngap_id = to_underlying(amf_ue_id);
  init_context_setup_req->ran_ue_ngap_id = to_underlying(ran_ue_id);

  init_context_setup_req->guami.plmn_id.from_string("00f110");
  init_context_setup_req->guami.amf_region_id.from_number(4);
  init_context_setup_req->guami.amf_set_id.from_number(1);
  init_context_setup_req->guami.amf_pointer.from_number(0);

  init_context_setup_req->nas_pdu_present = true;
  init_context_setup_req->nas_pdu.from_string(
      "7e02c4f6c22f017e0042010177000bf202f8998000410000001054070002f8990000011500210201005e01b6");

  allowed_nssai_item_s allowed_nssai = {};
  allowed_nssai.s_nssai.sst.from_number(1);
  allowed_nssai.s_nssai.sd_present = true;
  allowed_nssai.s_nssai.sd.from_string("db2700");

  init_context_setup_req->allowed_nssai.push_back(allowed_nssai);

  init_context_setup_req->masked_imeisv_present = true;
  init_context_setup_req->masked_imeisv.from_string(
      "0000000100100011010001010110011100000000111111111111111100000001"); // 0123456700ffff01

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_initial_context_setup_request_message(
    amf_ue_id_t                                                amf_ue_id,
    ran_ue_id_t                                                ran_ue_id,
    std::optional<cu_cp_core_network_assist_info_for_inactive> cn_assist_info_for_inactive,
    std::optional<location_report_request>                     location_reporting_request)
{
  ngap_message ngap_msg = generate_initial_context_setup_request_base(amf_ue_id, ran_ue_id);

  if (cn_assist_info_for_inactive.has_value()) {
    auto& init_context_setup_req = ngap_msg.pdu.init_msg().value.init_context_setup_request();
    init_context_setup_req->core_network_assist_info_for_inactive_present = true;
    init_context_setup_req->core_network_assist_info_for_inactive.ue_id_idx_value.set_idx_len10().from_number(
        cn_assist_info_for_inactive->ue_id_idx_value);
    if (cn_assist_info_for_inactive->ue_specific_drx.has_value()) {
      init_context_setup_req->core_network_assist_info_for_inactive.ue_specific_drx_present = true;
      asn1::number_to_enum(init_context_setup_req->core_network_assist_info_for_inactive.ue_specific_drx,
                           cn_assist_info_for_inactive->ue_specific_drx.value());
    }
    init_context_setup_req->core_network_assist_info_for_inactive.periodic_regist_upd_timer.from_number(190);
    init_context_setup_req->core_network_assist_info_for_inactive.mico_mode_ind_present = false;
    for (const auto& tai : cn_assist_info_for_inactive->tai_list_for_inactive) {
      tai_list_for_inactive_item_s asn1_tai;
      asn1_tai.tai.plmn_id = tai.plmn_id.to_bytes();
      asn1_tai.tai.tac.from_number(tai.tac);
      init_context_setup_req->core_network_assist_info_for_inactive.tai_list_for_inactive.push_back(asn1_tai);
    }
  }

  auto& init_context_setup_req = ngap_msg.pdu.init_msg().value.init_context_setup_request();
  init_context_setup_req->ue_security_cap.nr_encryption_algorithms.from_number(49152);
  init_context_setup_req->ue_security_cap.nr_integrity_protection_algorithms.from_number(49152);
  init_context_setup_req->ue_security_cap.eutr_aencryption_algorithms.from_number(0);
  init_context_setup_req->ue_security_cap.eutr_aintegrity_protection_algorithms.from_number(0);

  init_context_setup_req->security_key.from_string(
      "1111111000001101100111110001011010001110110010111010001100110111100111011000110110011010000110000011000010111000"
      "0010001100001010000001111111100000100111101011000011110000110101110010001010001010101000101100101100100000001110"
      "00010001000110001101110101100110"); // fe0d9f168ecba3379d8d9a1830b8230a07f827ac3c35c8a2a8b2c80e1118dd66

  if (location_reporting_request.has_value()) {
    init_context_setup_req->location_report_request_type_present = true;
    init_context_setup_req->location_report_request_type = location_report_request_to_asn1(*location_reporting_request);
  }

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_initial_context_setup_request_message_with_pdu_session(amf_ue_id_t amf_ue_id,
                                                                                                 ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = generate_valid_initial_context_setup_request_message(amf_ue_id, ran_ue_id);

  auto& init_context_setup_req = ngap_msg.pdu.init_msg().value.init_context_setup_request();
  init_context_setup_req->pdu_session_res_setup_list_cxt_req_present = true;

  asn1::ngap::pdu_session_res_setup_item_cxt_req_s pdu_session_item;
  pdu_session_item.pdu_session_id = 1U;
  pdu_session_item.s_nssai.sst.from_number(1U);
  pdu_session_item.pdu_session_res_setup_request_transfer =
      make_byte_buffer("0000040082000a0c400000003040000000008b000a01f00a3213020000049a00860001000088000700010000091c00")
          .value();

  init_context_setup_req->pdu_session_res_setup_list_cxt_req.push_back(pdu_session_item);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_invalid_initial_context_setup_request_message(amf_ue_id_t amf_ue_id,
                                                                                  ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = generate_initial_context_setup_request_base(amf_ue_id, ran_ue_id);

  // NIA0 is not allowed.
  auto& init_context_setup_req = ngap_msg.pdu.init_msg().value.init_context_setup_request();
  init_context_setup_req->ue_security_cap.nr_encryption_algorithms.from_number(0);
  init_context_setup_req->ue_security_cap.nr_integrity_protection_algorithms.from_number(0);
  init_context_setup_req->ue_security_cap.eutr_aencryption_algorithms.from_number(0);
  init_context_setup_req->ue_security_cap.eutr_aintegrity_protection_algorithms.from_number(0);

  return ngap_msg;
}

ngap_message
ocudu::ocucp::generate_invalid_initial_context_setup_request_message_with_pdu_session(amf_ue_id_t amf_ue_id,
                                                                                      ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = generate_invalid_initial_context_setup_request_message(amf_ue_id, ran_ue_id);

  auto& init_context_setup_req = ngap_msg.pdu.init_msg().value.init_context_setup_request();
  init_context_setup_req->pdu_session_res_setup_list_cxt_req_present = true;

  asn1::ngap::pdu_session_res_setup_item_cxt_req_s pdu_session_item;
  pdu_session_item.pdu_session_id = 1U;
  pdu_session_item.s_nssai.sst.from_number(1U);
  pdu_session_item.pdu_session_res_setup_request_transfer =
      make_byte_buffer("0000040082000a0c400000003040000000008b000a01f00a3213020000049a00860001000088000700010000091c00")
          .value();

  init_context_setup_req->pdu_session_res_setup_list_cxt_req.push_back(pdu_session_item);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_ue_context_modification_request_base(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = {};

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UE_CONTEXT_MOD);

  auto& ue_ctxt_mod           = ngap_msg.pdu.init_msg().value.ue_context_mod_request();
  ue_ctxt_mod->amf_ue_ngap_id = to_underlying(amf_ue_id);
  ue_ctxt_mod->ran_ue_ngap_id = to_underlying(ran_ue_id);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_ue_context_modification_request_message(
    amf_ue_id_t                                                amf_ue_id,
    ran_ue_id_t                                                ran_ue_id,
    std::optional<aggregate_maximum_bit_rate_t>                ue_ambr,
    std::optional<cu_cp_core_network_assist_info_for_inactive> cn_assist_info_for_inactive,
    std::optional<guami_t>                                     new_guami)
{
  ngap_message ngap_msg = generate_ue_context_modification_request_base(amf_ue_id, ran_ue_id);

  if (ue_ambr.has_value()) {
    auto& ue_ctxt_mod                                         = ngap_msg.pdu.init_msg().value.ue_context_mod_request();
    ue_ctxt_mod->ue_aggr_max_bit_rate_present                 = true;
    ue_ctxt_mod->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_dl = ue_ambr->dl;
    ue_ctxt_mod->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_ul = ue_ambr->ul;
  }

  if (cn_assist_info_for_inactive.has_value()) {
    auto& ue_ctxt_mod                                          = ngap_msg.pdu.init_msg().value.ue_context_mod_request();
    ue_ctxt_mod->core_network_assist_info_for_inactive_present = true;
    ue_ctxt_mod->core_network_assist_info_for_inactive.ue_id_idx_value.set_idx_len10().from_number(
        cn_assist_info_for_inactive->ue_id_idx_value);
    if (cn_assist_info_for_inactive->ue_specific_drx.has_value()) {
      ue_ctxt_mod->core_network_assist_info_for_inactive.ue_specific_drx_present = true;
      asn1::number_to_enum(ue_ctxt_mod->core_network_assist_info_for_inactive.ue_specific_drx,
                           cn_assist_info_for_inactive->ue_specific_drx.value());
    }
    ue_ctxt_mod->core_network_assist_info_for_inactive.periodic_regist_upd_timer.from_number(190);
    ue_ctxt_mod->core_network_assist_info_for_inactive.mico_mode_ind_present = false;
    for (const auto& tai : cn_assist_info_for_inactive->tai_list_for_inactive) {
      tai_list_for_inactive_item_s asn1_tai;
      asn1_tai.tai.plmn_id = tai.plmn_id.to_bytes();
      asn1_tai.tai.tac.from_number(tai.tac);
      ue_ctxt_mod->core_network_assist_info_for_inactive.tai_list_for_inactive.push_back(asn1_tai);
    }
  }

  if (new_guami.has_value()) {
    auto& ue_ctxt_mod              = ngap_msg.pdu.init_msg().value.ue_context_mod_request();
    ue_ctxt_mod->new_guami_present = true;
    ue_ctxt_mod->new_guami.plmn_id.from_string("00f110");
    ue_ctxt_mod->new_guami.amf_region_id.from_number(4);
    ue_ctxt_mod->new_guami.amf_set_id.from_number(1);
    ue_ctxt_mod->new_guami.amf_pointer.from_number(0);
  }

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_invalid_ue_context_modification_request_message(amf_ue_id_t amf_ue_id,
                                                                                    ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = generate_ue_context_modification_request_base(amf_ue_id, ran_ue_id);

  // Set invalid PLMN in GUAMI.
  auto& ue_context_mod_req              = ngap_msg.pdu.init_msg().value.ue_context_mod_request();
  ue_context_mod_req->new_guami_present = true;
  ue_context_mod_req->new_guami.plmn_id.from_string("ffffff"); // Invalid PLMN.

  return ngap_msg;
}

ngap_message
ocudu::ocucp::generate_valid_ue_context_release_command_with_amf_ue_ngap_id(amf_ue_id_t                       amf_ue_id,
                                                                            asn1::ngap::cause_radio_network_e cause)
{
  ngap_message ngap_msg = {};

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UE_CONTEXT_RELEASE);

  auto& ue_context_release_cmd                             = ngap_msg.pdu.init_msg().value.ue_context_release_cmd();
  ue_context_release_cmd->ue_ngap_ids.set_amf_ue_ngap_id() = to_underlying(amf_ue_id);
  ue_context_release_cmd->cause.set_radio_network()        = cause;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_ue_context_release_command_with_ue_ngap_id_pair(amf_ue_id_t amf_ue_id,
                                                                                          ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = {};

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UE_CONTEXT_RELEASE);

  auto& ue_context_release_cmd = ngap_msg.pdu.init_msg().value.ue_context_release_cmd();
  auto& ue_id_pair             = ue_context_release_cmd->ue_ngap_ids.set_ue_ngap_id_pair();
  ue_id_pair.amf_ue_ngap_id    = to_underlying(amf_ue_id);
  ue_id_pair.ran_ue_ngap_id    = to_underlying(ran_ue_id);
  auto& cause                  = ue_context_release_cmd->cause.set_radio_network();
  cause                        = asn1::ngap::cause_radio_network_opts::options::radio_conn_with_ue_lost;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_ue_context_release_command_with_choice_exts_ue_ngap_ids()
{
  ngap_message ngap_msg = {};

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_UE_CONTEXT_RELEASE);

  auto& ue_context_release_cmd = ngap_msg.pdu.init_msg().value.ue_context_release_cmd();
  ue_context_release_cmd->ue_ngap_ids.set_choice_exts();
  auto& cause = ue_context_release_cmd->cause.set_radio_network();
  cause       = asn1::ngap::cause_radio_network_opts::options::radio_conn_with_ue_lost;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_pdu_session_resource_setup_request_base(amf_ue_id_t amf_ue_id,
                                                                            ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_PDU_SESSION_RES_SETUP);

  auto& pdu_session_res_setup_req           = ngap_msg.pdu.init_msg().value.pdu_session_res_setup_request();
  pdu_session_res_setup_req->amf_ue_ngap_id = to_underlying(amf_ue_id);
  pdu_session_res_setup_req->ran_ue_ngap_id = to_underlying(ran_ue_id);

  pdu_session_res_setup_req->ue_aggr_max_bit_rate_present                 = true;
  pdu_session_res_setup_req->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_dl = 300000000U;
  pdu_session_res_setup_req->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_ul = 200000000U;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_pdu_session_resource_setup_request_message(
    amf_ue_id_t                                                amf_ue_id,
    ran_ue_id_t                                                ran_ue_id,
    const std::map<pdu_session_id_t, pdu_session_test_params>& pdu_sessions,
    std::optional<security_indication_t>                       sec_ind)
{
  ngap_message ngap_msg = generate_pdu_session_resource_setup_request_base(amf_ue_id, ran_ue_id);

  auto& pdu_session_res_setup_req = ngap_msg.pdu.init_msg().value.pdu_session_res_setup_request();

  for (const auto& [pdu_session_id, pdu_session_params] : pdu_sessions) {
    pdu_session_res_setup_item_su_req_s pdu_session_res_item;

    pdu_session_res_item.pdu_session_id = to_underlying(pdu_session_id);

    // Fill PDU session NAS PDU.
    pdu_session_res_item.pdu_session_nas_pdu.from_string("7e02e9b0a23c027e006801006e2e0115c211000901000631310101ff08060"
                                                         "6014a06014a2905010c02010c2204010027db79000608204101"
                                                         "01087b002080802110030000108106ac1503648306ac150364000d04ac150"
                                                         "364001002054e251c036f61690469707634066d6e6330393906"
                                                         "6d636332303804677072731201");

    // Fill S-NSSAI.
    pdu_session_res_item.s_nssai.sst.from_number(1);
    pdu_session_res_item.s_nssai.sd_present = true;
    pdu_session_res_item.s_nssai.sd.from_string("0027db");

    // Fill PDU session resource setup request transfer.
    asn1::ngap::pdu_session_res_setup_request_transfer_s asn1_setup_req_transfer;
    {
      // Fill PDU session aggregate maximum bit rate.
      asn1_setup_req_transfer->pdu_session_aggr_max_bit_rate_present                          = true;
      asn1_setup_req_transfer->pdu_session_aggr_max_bit_rate.pdu_session_aggr_max_bit_rate_dl = 1000000000U;
      asn1_setup_req_transfer->pdu_session_aggr_max_bit_rate.pdu_session_aggr_max_bit_rate_ul = 1000000000U;

      // Fill UL NGU UP TNL Info.
      asn1_setup_req_transfer->ul_ngu_up_tnl_info.set_gtp_tunnel();
      auto addr = transport_layer_address::create_from_string("127.0.0.1");
      tla_to_asn1_bitstring(asn1_setup_req_transfer->ul_ngu_up_tnl_info.gtp_tunnel().transport_layer_address, addr);
      asn1_setup_req_transfer->ul_ngu_up_tnl_info.gtp_tunnel().gtp_teid.from_number(0x00005e6c);

      // Fill PDU session type.
      asn1_setup_req_transfer->pdu_session_type = pdu_session_type_to_asn1(pdu_session_params.pdu_session_type);

      // Fill QoS flow setup request list.
      for (const auto& qos_flow_test_item : pdu_session_params.qos_flows) {
        qos_flow_setup_request_item_s qos_flow_setup_req_item;
        qos_flow_setup_req_item.qos_flow_id = to_underlying(qos_flow_test_item.qos_flow_id);

        // Fill QoS characteristics.
        qos_flow_setup_req_item.qos_flow_level_qos_params.qos_characteristics.set_non_dyn5qi();
        qos_flow_setup_req_item.qos_flow_level_qos_params.qos_characteristics.non_dyn5qi().five_qi =
            qos_flow_test_item.five_qi;

        // Fill allocation and retention priority.
        qos_flow_setup_req_item.qos_flow_level_qos_params.alloc_and_retention_prio.prio_level_arp = 8;
        qos_flow_setup_req_item.qos_flow_level_qos_params.alloc_and_retention_prio.pre_emption_cap =
            pre_emption_cap_opts::shall_not_trigger_pre_emption;
        qos_flow_setup_req_item.qos_flow_level_qos_params.alloc_and_retention_prio.pre_emption_vulnerability =
            pre_emption_vulnerability_opts::not_pre_emptable;

        // Fill gbr QoS info.
        qos_flow_setup_req_item.qos_flow_level_qos_params.gbr_qos_info_present                     = true;
        qos_flow_setup_req_item.qos_flow_level_qos_params.gbr_qos_info.max_flow_bit_rate_dl        = 1000000000U;
        qos_flow_setup_req_item.qos_flow_level_qos_params.gbr_qos_info.max_flow_bit_rate_ul        = 1000000000U;
        qos_flow_setup_req_item.qos_flow_level_qos_params.gbr_qos_info.guaranteed_flow_bit_rate_dl = 1000000000U;
        qos_flow_setup_req_item.qos_flow_level_qos_params.gbr_qos_info.guaranteed_flow_bit_rate_ul = 1000000000U;

        asn1_setup_req_transfer->qos_flow_setup_request_list.push_back(qos_flow_setup_req_item);
      }

      if (sec_ind) {
        asn1_setup_req_transfer->security_ind_present = true;
        security_indication_to_asn1(asn1_setup_req_transfer->security_ind, *sec_ind);
      }
    }
    pdu_session_res_item.pdu_session_res_setup_request_transfer = pack_into_pdu(asn1_setup_req_transfer);

    pdu_session_res_setup_req->pdu_session_res_setup_list_su_req.push_back(pdu_session_res_item);
  }

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_invalid_pdu_session_resource_setup_request_message(amf_ue_id_t amf_ue_id,
                                                                                       ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = generate_pdu_session_resource_setup_request_base(amf_ue_id, ran_ue_id);

  auto& pdu_session_res_setup_req = ngap_msg.pdu.init_msg().value.pdu_session_res_setup_request();

  // Fill PDU sessions with duplicate ids.
  for (auto it = 0; it < 2; ++it) {
    pdu_session_res_setup_item_su_req_s pdu_session_res_item;

    pdu_session_res_item.pdu_session_id = 1;

    // Fill PDU session NAS PDU.
    pdu_session_res_item.pdu_session_nas_pdu.from_string("7e02e9b0a23c027e006801006e2e0115c211000901000631310101ff08060"
                                                         "6014a06014a2905010c02010c2204010027db79000608204101"
                                                         "01087b002080802110030000108106ac1503648306ac150364000d04ac150"
                                                         "364001002054e251c036f61690469707634066d6e6330393906"
                                                         "6d636332303804677072731201");

    // Fill S-NSSAI.
    pdu_session_res_item.s_nssai.sst.from_number(1);
    pdu_session_res_item.s_nssai.sd_present = true;
    pdu_session_res_item.s_nssai.sd.from_string("0027db");

    // Fill PDU session resource setup request transfer.
    pdu_session_res_item.pdu_session_res_setup_request_transfer.from_string(
        "0000040082000a0c400000003040000000008b000a01f00a321302000028d600860001000088000700010000091c00");

    pdu_session_res_setup_req->pdu_session_res_setup_list_su_req.push_back(pdu_session_res_item);
  }

  return ngap_msg;
}

ngap_message
ocudu::ocucp::generate_pdu_session_resource_setup_request_with_pdu_session_type_ipv4_and_ipv4v6_transport_layer_address(
    amf_ue_id_t amf_ue_id,
    ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = generate_valid_pdu_session_resource_setup_request_message(
      amf_ue_id, ran_ue_id, {{uint_to_pdu_session_id(1), {pdu_session_type_t::ipv4, {{uint_to_qos_flow_id(1), 9}}}}});

  // Fill invalid PDU session resource setup request transfer.
  auto& pdu_session_res_setup_req = ngap_msg.pdu.init_msg().value.pdu_session_res_setup_request();
  pdu_session_res_setup_req->pdu_session_res_setup_list_su_req.begin()
      ->pdu_session_res_setup_request_transfer.from_string("0000040082000a0c1dcd6500301dcd6500008b001a09f00a0c01bbfdf66"
                                                           "cf3768500000000000000000bf1010000010086000100008800"
                                                           "0700010000093800");

  return ngap_msg;
}

ngap_pdu_session_resource_setup_response
ocudu::ocucp::generate_ngap_pdu_session_resource_setup_response(ngap_pdu_session_resource_setup_request& request)
{
  ngap_pdu_session_resource_setup_response pdu_session_res_setup_resp;

  for (const auto& pdu_session : request.pdu_session_res_setup_items) {
    ngap_pdu_session_res_setup_response_item pdu_session_setup_response_item;
    pdu_session_setup_response_item.pdu_session_id = pdu_session.pdu_session_id;

    auto& dlqos_flow_per_tnl_info =
        pdu_session_setup_response_item.pdu_session_resource_setup_response_transfer.dlqos_flow_per_tnl_info;
    dlqos_flow_per_tnl_info.up_tp_layer_info = {transport_layer_address::create_from_string("0.0.0.0"),
                                                int_to_gtpu_teid(0)};
    cu_cp_associated_qos_flow assoc_qos_flow;
    assoc_qos_flow.qos_flow_id = uint_to_qos_flow_id(1);
    dlqos_flow_per_tnl_info.associated_qos_flow_list.emplace(uint_to_qos_flow_id(1), assoc_qos_flow);

    pdu_session_res_setup_resp.pdu_session_res_setup_response_items.emplace(pdu_session.pdu_session_id,
                                                                            pdu_session_setup_response_item);
  }

  return pdu_session_res_setup_resp;
}

ngap_message ocudu::ocucp::generate_pdu_session_resource_release_command_base(amf_ue_id_t amf_ue_id,
                                                                              ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_PDU_SESSION_RES_RELEASE);

  auto& pdu_session_res_release_cmd           = ngap_msg.pdu.init_msg().value.pdu_session_res_release_cmd();
  pdu_session_res_release_cmd->amf_ue_ngap_id = to_underlying(amf_ue_id);
  pdu_session_res_release_cmd->ran_ue_ngap_id = to_underlying(ran_ue_id);

  return ngap_msg;
}

ngap_message
ocudu::ocucp::generate_valid_pdu_session_resource_release_command(amf_ue_id_t                          amf_ue_id,
                                                                  ran_ue_id_t                          ran_ue_id,
                                                                  const std::vector<pdu_session_id_t>& pdu_session_ids)
{
  ngap_message ngap_msg = generate_pdu_session_resource_release_command_base(amf_ue_id, ran_ue_id);

  auto& pdu_session_res_release_cmd = ngap_msg.pdu.init_msg().value.pdu_session_res_release_cmd();

  // Fill PDU session NAS PDU.
  pdu_session_res_release_cmd->nas_pdu_present = true;
  pdu_session_res_release_cmd->nas_pdu         = make_byte_buffer("7e02bcb47dc1137e00680100052e01b3d3241201").value();

  // Fill  PDU session resource to release list.
  for (pdu_session_id_t pdu_session_id : pdu_session_ids) {
    asn1::ngap::pdu_session_res_to_release_item_rel_cmd_s pdu_session_res_to_release_item_rel_cmd;
    pdu_session_res_to_release_item_rel_cmd.pdu_session_id                       = to_underlying(pdu_session_id);
    pdu_session_res_to_release_item_rel_cmd.pdu_session_res_release_cmd_transfer = make_byte_buffer("10").value();
    pdu_session_res_release_cmd->pdu_session_res_to_release_list_rel_cmd.push_back(
        pdu_session_res_to_release_item_rel_cmd);
  }

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_pdu_session_resource_release_command(amf_ue_id_t      amf_ue_id,
                                                                               ran_ue_id_t      ran_ue_id,
                                                                               pdu_session_id_t pdu_session_id)
{
  return generate_valid_pdu_session_resource_release_command(
      amf_ue_id, ran_ue_id, std::vector<pdu_session_id_t>{pdu_session_id});
}

ngap_message ocudu::ocucp::generate_pdu_session_resource_release_command_with_duplicate_pdu_session_id(
    amf_ue_id_t      amf_ue_id,
    ran_ue_id_t      ran_ue_id,
    pdu_session_id_t pdu_session_id)
{
  return generate_valid_pdu_session_resource_release_command(
      amf_ue_id, ran_ue_id, std::vector<pdu_session_id_t>{pdu_session_id, pdu_session_id});
}

ngap_message ocudu::ocucp::generate_invalid_pdu_session_resource_release_command(amf_ue_id_t amf_ue_id,
                                                                                 ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = generate_pdu_session_resource_release_command_base(amf_ue_id, ran_ue_id);

  return ngap_msg;
}

ngap_pdu_session_resource_release_response
ocudu::ocucp::generate_ngap_pdu_session_resource_release_response(pdu_session_id_t pdu_session_id)
{
  ngap_pdu_session_resource_release_response pdu_session_res_release_resp;

  ngap_pdu_session_res_released_item_rel_res pdu_session_res_released_item_rel_res;

  pdu_session_res_released_item_rel_res.pdu_session_id = pdu_session_id;

  pdu_session_res_release_resp.released_pdu_sessions.emplace(pdu_session_id, pdu_session_res_released_item_rel_res);

  return pdu_session_res_release_resp;
}

ngap_message ocudu::ocucp::generate_pdu_session_resource_modify_request_base(amf_ue_id_t amf_ue_id,
                                                                             ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_PDU_SESSION_RES_MODIFY);

  auto& pdu_session_res_modify_req           = ngap_msg.pdu.init_msg().value.pdu_session_res_modify_request();
  pdu_session_res_modify_req->amf_ue_ngap_id = to_underlying(amf_ue_id);
  pdu_session_res_modify_req->ran_ue_ngap_id = to_underlying(ran_ue_id);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_pdu_session_resource_modify_request_message(
    amf_ue_id_t                       amf_ue_id,
    ran_ue_id_t                       ran_ue_id,
    pdu_session_id_t                  pdu_session_id,
    const std::vector<qos_flow_id_t>& qos_flow_add_or_modify_list,
    const std::vector<qos_flow_id_t>& qos_flow_to_release_list)
{
  ngap_message ngap_msg = generate_pdu_session_resource_modify_request_base(amf_ue_id, ran_ue_id);

  auto& pdu_session_res_modify_req = ngap_msg.pdu.init_msg().value.pdu_session_res_modify_request();

  pdu_session_res_modify_item_mod_req_s pdu_session_res_item;

  pdu_session_res_item.pdu_session_id = to_underlying(pdu_session_id);

  // Fill PDU session resource modify request transfer.
  asn1::ngap::pdu_session_res_modify_request_transfer_s pdu_session_res_modify_request_transfer;
  {
    // Fill QoS flow add or modify.
    pdu_session_res_modify_request_transfer->qos_flow_add_or_modify_request_list_present =
        !qos_flow_add_or_modify_list.empty();
    for (const auto& qos_flow_id : qos_flow_add_or_modify_list) {
      // Fill QoS flow modify request list.

      asn1::ngap::qos_flow_add_or_modify_request_item_s qos_flow_add_item;

      // Fill QoS flow identifier.
      qos_flow_add_item.qos_flow_id = to_underlying(qos_flow_id);

      // Fill QoS characteristics.
      qos_flow_add_item.qos_flow_level_qos_params_present = true;
      qos_flow_add_item.qos_flow_level_qos_params.qos_characteristics.set_non_dyn5qi();
      qos_flow_add_item.qos_flow_level_qos_params.qos_characteristics.non_dyn5qi().five_qi = 9;

      // Fill allocation and retention priority.
      qos_flow_add_item.qos_flow_level_qos_params.alloc_and_retention_prio.prio_level_arp = 8;
      qos_flow_add_item.qos_flow_level_qos_params.alloc_and_retention_prio.pre_emption_cap =
          asn1::ngap::pre_emption_cap_opts::shall_not_trigger_pre_emption;
      qos_flow_add_item.qos_flow_level_qos_params.alloc_and_retention_prio.pre_emption_vulnerability =
          asn1::ngap::pre_emption_vulnerability_opts::not_pre_emptable;

      pdu_session_res_modify_request_transfer->qos_flow_add_or_modify_request_list.push_back(qos_flow_add_item);
    }

    // Fill QoS flow to release.
    pdu_session_res_modify_request_transfer->qos_flow_to_release_list_present = !qos_flow_to_release_list.empty();
    for (const auto& qos_flow_id : qos_flow_to_release_list) {
      asn1::ngap::qos_flow_with_cause_item_s qos_flow_release_item;

      // Fill QoS flow identifier.
      qos_flow_release_item.qos_flow_id = to_underlying(qos_flow_id);

      // Fill cause.
      qos_flow_release_item.cause.set_radio_network();
      qos_flow_release_item.cause.radio_network() = asn1::ngap::cause_radio_network_opts::options::unspecified;

      pdu_session_res_modify_request_transfer->qos_flow_to_release_list.push_back(qos_flow_release_item);
    }
  }

  pdu_session_res_item.pdu_session_res_modify_request_transfer = pack_into_pdu(pdu_session_res_modify_request_transfer);

  pdu_session_res_modify_req->pdu_session_res_modify_list_mod_req.push_back(pdu_session_res_item);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_invalid_pdu_session_resource_modify_request_message(amf_ue_id_t      amf_ue_id,
                                                                                        ran_ue_id_t      ran_ue_id,
                                                                                        pdu_session_id_t pdu_session_id)
{
  ngap_message ngap_msg = generate_pdu_session_resource_modify_request_base(amf_ue_id, ran_ue_id);

  auto& pdu_session_res_modify_req = ngap_msg.pdu.init_msg().value.pdu_session_res_modify_request();

  // Fill pdu sessions with duplicate IDs.
  for (auto it = 0; it < 2; ++it) {
    pdu_session_res_modify_item_mod_req_s pdu_session_res_item;

    pdu_session_res_item.pdu_session_id = to_underlying(pdu_session_id);

    // Fill PDU session resource modify request transfer.
    pdu_session_res_modify_request_transfer_s pdu_session_res_modify_request_transfer;

    // Fill QoS flow add or modify request item.
    pdu_session_res_modify_request_transfer->qos_flow_add_or_modify_request_list_present = true;
    qos_flow_add_or_modify_request_item_s qos_flow_add_item;

    // Fill QoS flow identifier.
    qos_flow_add_item.qos_flow_id = 1;

    pdu_session_res_modify_request_transfer->qos_flow_add_or_modify_request_list.push_back(qos_flow_add_item);

    pdu_session_res_item.pdu_session_res_modify_request_transfer =
        pack_into_pdu(pdu_session_res_modify_request_transfer);

    pdu_session_res_modify_req->pdu_session_res_modify_list_mod_req.push_back(pdu_session_res_item);
  }

  return ngap_msg;
}

ngap_pdu_session_resource_modify_response
ocudu::ocucp::generate_ngap_pdu_session_resource_modify_response(pdu_session_id_t pdu_session_id,
                                                                 qos_flow_id_t    qos_flow_id)
{
  ngap_pdu_session_resource_modify_response pdu_session_res_modify_resp;

  ngap_pdu_session_resource_modify_response_item pdu_session_modify_response_item;
  pdu_session_modify_response_item.pdu_session_id = pdu_session_id;

  ngap_qos_flow_per_tnl_information qos_flow_per_tnl_info;
  qos_flow_per_tnl_info.up_tp_layer_info = {transport_layer_address::create_from_string("127.0.0.1"),
                                            int_to_gtpu_teid(1)};

  cu_cp_associated_qos_flow assoc_qos_flow;
  assoc_qos_flow.qos_flow_id = qos_flow_id;

  qos_flow_per_tnl_info.associated_qos_flow_list.emplace(qos_flow_id, assoc_qos_flow);

  pdu_session_modify_response_item.transfer.add_dl_qos_flow_per_tnl_info.push_back(qos_flow_per_tnl_info);

  pdu_session_res_modify_resp.pdu_session_res_modify_list.emplace(pdu_session_id, pdu_session_modify_response_item);

  return pdu_session_res_modify_resp;
}

ngap_message ocudu::ocucp::generate_valid_minimal_paging_message()
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_PAGING);
  auto& paging = ngap_msg.pdu.init_msg().value.paging();

  // Fill UE paging id.
  auto& five_g_s_tmsi = paging->ue_paging_id.set_five_g_s_tmsi();
  five_g_s_tmsi.amf_set_id.from_number(1);
  five_g_s_tmsi.amf_pointer.from_number(0);
  five_g_s_tmsi.five_g_tmsi.from_number(4211117727);

  // Fill TAI list for paging.
  asn1::ngap::tai_list_for_paging_item_s paging_item;
  paging_item.tai.plmn_id.from_string("00f110");
  paging_item.tai.tac.from_number(7);
  paging->tai_list_for_paging.push_back(paging_item);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_paging_message()
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_PAGING);
  auto& paging = ngap_msg.pdu.init_msg().value.paging();

  // Fill UE paging id.
  auto& five_g_s_tmsi = paging->ue_paging_id.set_five_g_s_tmsi();
  five_g_s_tmsi.amf_set_id.from_number(1);
  five_g_s_tmsi.amf_pointer.from_number(0);
  five_g_s_tmsi.five_g_tmsi.from_number(4211117727);

  // Fill paging DRX.
  paging->paging_drx_present = true;
  paging->paging_drx.value   = asn1::ngap::paging_drx_opts::options::v64;

  // Fill TAI list for paging.
  asn1::ngap::tai_list_for_paging_item_s paging_item;
  paging_item.tai.plmn_id.from_string("00f110");
  paging_item.tai.tac.from_number(7);
  paging->tai_list_for_paging.push_back(paging_item);

  // Fill paging prio.
  paging->paging_prio_present = true;
  paging->paging_prio.value   = asn1::ngap::paging_prio_opts::options::priolevel5;

  // Fill UE radio capabilities for paging.
  paging->ue_radio_cap_for_paging_present                       = true;
  paging->ue_radio_cap_for_paging.ue_radio_cap_for_paging_of_nr = make_byte_buffer("deadbeef").value();

  // Fill paging origin.
  paging->paging_origin_present = true;
  paging->paging_origin.value   = asn1::ngap::paging_origin_opts::options::non_neg3gpp;

  // Fill assist data for paging.
  paging->assist_data_for_paging_present                                   = true;
  paging->assist_data_for_paging.assist_data_for_recommended_cells_present = true;

  // Fill eDRX information for paging.
  paging->nr_paginge_drx_info_present                    = true;
  paging->nr_paginge_drx_info.nr_paging_e_drx_cycle      = nr_paging_e_drx_cycle_e::hfhalf;
  paging->nr_paginge_drx_info.nr_paging_time_win_present = true;
  paging->nr_paginge_drx_info.nr_paging_time_win         = nr_paging_time_win_e::s1;

  asn1::ngap::recommended_cell_item_s recommended_cell_item;
  auto&                               nr_cgi = recommended_cell_item.ngran_cgi.set_nr_cgi();
  nr_cgi.plmn_id.from_string("00f110");
  nr_cell_identity nci = nr_cell_identity::create(gnb_id_t{411, 22}, 0).value();
  nr_cgi.nr_cell_id.from_number(nci.value());
  recommended_cell_item.time_stayed_in_cell_present = true;
  recommended_cell_item.time_stayed_in_cell         = 5;

  paging->assist_data_for_paging.assist_data_for_recommended_cells.recommended_cells_for_paging.recommended_cell_list
      .push_back(recommended_cell_item);

  paging->assist_data_for_paging.paging_attempt_info_present                        = true;
  paging->assist_data_for_paging.paging_attempt_info.paging_attempt_count           = 3;
  paging->assist_data_for_paging.paging_attempt_info.intended_nof_paging_attempts   = 4;
  paging->assist_data_for_paging.paging_attempt_info.next_paging_area_scope_present = true;
  paging->assist_data_for_paging.paging_attempt_info.next_paging_area_scope.value =
      asn1::ngap::next_paging_area_scope_opts::options::changed;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_invalid_paging_message()
{
  ngap_message ngap_msg = {};

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_PAGING);

  // Fill UE paging id.
  auto& paging = ngap_msg.pdu.init_msg().value.paging();
  paging->ue_paging_id.set_choice_exts();
  return ngap_msg;
}

ngap_message
ocudu::ocucp::generate_error_indication_message(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id, ngap_cause_t cause)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_ERROR_IND);

  auto& error_indication = ngap_msg.pdu.init_msg().value.error_ind();

  error_indication->amf_ue_ngap_id_present = true;
  error_indication->amf_ue_ngap_id         = to_underlying(amf_ue_id);

  error_indication->ran_ue_ngap_id_present = true;
  error_indication->ran_ue_ngap_id         = to_underlying(ran_ue_id);

  error_indication->cause_present = true;
  error_indication->cause         = cause_to_asn1(cause);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_handover_request(amf_ue_id_t amf_ue_id, const handover_request_params& params)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_HO_RES_ALLOC);

  auto& ho_request = ngap_msg.pdu.init_msg().value.ho_request();

  ho_request->amf_ue_ngap_id = to_underlying(amf_ue_id);
  // Fill Handover type.
  ho_request->handov_type = asn1::ngap::handov_type_opts::options::intra5gs;
  // Fill cause.
  ho_request->cause.set_radio_network() = asn1::ngap::cause_radio_network_opts::options::ho_desirable_for_radio_reason;
  // Fill UE aggregated max bit rate.
  ho_request->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_dl = 1073741824;
  ho_request->ue_aggr_max_bit_rate.ue_aggr_max_bit_rate_ul = 1073741824;
  // Fill UE security capabilities.
  ho_request->ue_security_cap.nr_encryption_algorithms.from_number(49152);
  ho_request->ue_security_cap.nr_integrity_protection_algorithms.from_number(49152);
  ho_request->ue_security_cap.eutr_aencryption_algorithms.from_number(0);
  ho_request->ue_security_cap.eutr_aintegrity_protection_algorithms.from_number(0);
  // Fill security context
  ho_request->security_context.next_hop_chaining_count = 2;
  ho_request->security_context.next_hop_nh.from_string(
      "1000100100100011110101001110000001001000000001111000010110011110001010011100101010110010000001010110110000100111"
      "0110101111001100000001100111100010001011111000111111101000100110011101110111100010101101000101000010100001000001"
      "00000101010000111100001010001001"); // 8923d4e04807859e29cab2056c276bcc06788be3fa267778ad1428410543c289
  // Fill PDU session resource setup list HO req.
  asn1::ngap::pdu_session_res_setup_item_ho_req_s setup_item;
  setup_item.pdu_session_id = 1;
  setup_item.s_nssai.sst.from_number(1);
  setup_item.ho_request_transfer =
      make_byte_buffer(
          "0000050082000a0c400000003040000000008b000a01f00a32130200001b13007f00010000860001000088000700010000091c00")
          .value();
  {
    // The Data Forwarding Not Possible IE decides whether the target may set up forwarding tunnels for the PDU session,
    // so set it explicitly rather than inheriting whatever the transfer above happens to carry.
    asn1::ngap::pdu_session_res_setup_request_transfer_s transfer;
    asn1::cbit_ref transfer_bref({setup_item.ho_request_transfer.begin(), setup_item.ho_request_transfer.end()});
    report_error_if_not(transfer.unpack(transfer_bref) == asn1::OCUDUASN_SUCCESS,
                        "Failed to unpack Handover Request Transfer");
    transfer->data_forwarding_not_possible_present = params.data_forwarding_not_possible;
    transfer->data_forwarding_not_possible =
        asn1::ngap::data_forwarding_not_possible_opts::data_forwarding_not_possible;
    transfer->direct_forwarding_path_availability_present = params.direct_forwarding_path_available;
    transfer->direct_forwarding_path_availability =
        asn1::ngap::direct_forwarding_path_availability_opts::direct_path_available;
    setup_item.ho_request_transfer = pack_into_pdu(transfer);
  }
  ho_request->pdu_session_res_setup_list_ho_req.push_back(setup_item);
  // Fill allowed S-NSSAI.
  asn1::ngap::allowed_nssai_item_s allowed_nssai;
  allowed_nssai.s_nssai.sst.from_number(1);
  ho_request->allowed_nssai.push_back(allowed_nssai);
  // Fill masked IMEISV
  ho_request->masked_imeisv_present = true;
  ho_request->masked_imeisv.from_number(81985526923525889); // 0123456700ffff01
  // Fill source to target transparent container.
  asn1::ngap::source_ngran_node_to_target_ngran_node_transparent_container_s transparent_container;
  // Fill RRC container.
  transparent_container.rrc_container =
      make_byte_buffer("0021930680ce811d1968097e340e1480005824c5c00060fc2c00637fe002e00131401a0000000880058d006007a071e"
                       "439f0000240400e03"
                       "00000000100186c0000700809df0000000000000103a0002000012cb2800281c50f0007000f00000004008010240a0")
          .value();
  // Fill PDU session resource info list.
  asn1::ngap::pdu_session_res_info_item_s session_item;
  session_item.pdu_session_id = 1;
  asn1::ngap::qos_flow_info_item_s flow_item;
  flow_item.qos_flow_id = 1;
  if (params.propose_dl_data_forwarding) {
    flow_item.dl_forwarding_present = true;
    flow_item.dl_forwarding         = asn1::ngap::dl_forwarding_opts::dl_forwarding_proposed;
  }
  session_item.qos_flow_info_list.push_back(flow_item);
  if (params.include_drb_to_qos_flow_mapping) {
    // Report this source's own DRB-to-QoS-flow mapping (DRB1 <-> QFI1, matching the admitted PDU session).
    asn1::ngap::drbs_to_qos_flows_map_item_s drb_to_qos_flow_map_item;
    drb_to_qos_flow_map_item.drb_id = 1;
    asn1::ngap::associated_qos_flow_item_s assoc_qos_flow;
    assoc_qos_flow.qos_flow_id = 1;
    drb_to_qos_flow_map_item.associated_qos_flow_list.push_back(assoc_qos_flow);
    session_item.drbs_to_qos_flows_map_list.push_back(drb_to_qos_flow_map_item);
  }
  transparent_container.pdu_session_res_info_list.push_back(session_item);
  // Fill NR CGI.
  auto& cgi = transparent_container.target_cell_id.set_nr_cgi();
  cgi.plmn_id.from_string(params.invalid_target_cell_plmn_id ? "ffffff" : "00f110");
  nr_cell_identity nci = nr_cell_identity::create(gnb_id_t{411, 22}, 0).value();
  cgi.nr_cell_id.from_number(nci.value());
  // Fill UE history info.
  asn1::ngap::last_visited_cell_item_s cell_item;
  auto&                                cell       = cell_item.last_visited_cell_info.set_ngran_cell();
  auto&                                ngran_cell = cell.global_cell_id.set_nr_cgi();
  ngran_cell.plmn_id.from_string("00f110");
  ngran_cell.nr_cell_id.from_number(nci.value());
  cell.cell_type.cell_size    = asn1::ngap::cell_size_opts::options::small;
  cell.time_ue_stayed_in_cell = 0;
  transparent_container.ue_history_info.push_back(cell_item);

  byte_buffer   pdu{};
  asn1::bit_ref bref{pdu};
  if (transparent_container.pack(bref) == asn1::OCUDUASN_ERROR_ENCODE_FAIL) {
    ocudulog::fetch_basic_logger("NGAP").error("Failed to pack source to target container.");
  }
  ho_request->source_to_target_transparent_container = pdu.copy();

  // Fill GUAMI.
  ho_request->guami.plmn_id.from_string("00f110");
  ho_request->guami.amf_region_id.from_number(2);
  ho_request->guami.amf_set_id.from_number(1);
  ho_request->guami.amf_pointer.from_number(0);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_handover_preparation_failure(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_unsuccessful_outcome();
  ngap_msg.pdu.unsuccessful_outcome().load_info_obj(ASN1_NGAP_ID_HO_PREP);

  auto& ho_prep_fail = ngap_msg.pdu.unsuccessful_outcome().value.ho_prep_fail();

  ho_prep_fail->amf_ue_ngap_id = to_underlying(amf_ue_id);
  ho_prep_fail->ran_ue_ngap_id = to_underlying(ran_ue_id);

  // Fill cause.
  ho_prep_fail->cause.set_radio_network() = asn1::ngap::cause_radio_network_opts::options::unspecified;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_valid_handover_command(amf_ue_id_t amf_ue_id,
                                                           ran_ue_id_t ran_ue_id,
                                                           bool        with_data_forwarding_info,
                                                           bool        with_forwarding_tunnel)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_successful_outcome();
  ngap_msg.pdu.successful_outcome().load_info_obj(ASN1_NGAP_ID_HO_PREP);

  auto& ho_cmd = ngap_msg.pdu.successful_outcome().value.ho_cmd();

  ho_cmd->amf_ue_ngap_id = to_underlying(amf_ue_id);
  ho_cmd->ran_ue_ngap_id = to_underlying(ran_ue_id);
  // Fill handover type.
  ho_cmd->handov_type = asn1::ngap::handov_type_opts::options::intra5gs;

  // Fill PDU session resource HO list.
  ho_cmd->pdu_session_res_ho_list_present = true;
  asn1::ngap::pdu_session_res_ho_item_s ho_item;
  ho_item.pdu_session_id  = 1;
  ho_item.ho_cmd_transfer = make_byte_buffer("00").value();
  if (with_data_forwarding_info) {
    // Report a PDU session level DL forwarding tunnel. Its endpoint belongs either to the handover target, when the
    // 5GC uses a direct forwarding path, or to a UPF the 5GC inserted for indirect forwarding. The source treats both
    // the same way.
    asn1::ngap::ho_cmd_transfer_s ho_cmd_transfer;
    if (with_forwarding_tunnel) {
      ho_cmd_transfer.dl_forwarding_up_tnl_info_present = true;
      auto& session_tunnel                              = ho_cmd_transfer.dl_forwarding_up_tnl_info.set_gtp_tunnel();
      session_tunnel.transport_layer_address.from_number(2887058953);
      session_tunnel.gtp_teid.from_string("20000283");
    }

    asn1::ngap::qos_flow_to_be_forwarded_item_s flow_to_be_forwarded;
    flow_to_be_forwarded.qos_flow_id = 1;
    ho_cmd_transfer.qos_flow_to_be_forwarded_list.push_back(flow_to_be_forwarded);

    if (with_forwarding_tunnel) {
      // Report a DRB level tunnel alongside it, as a 5GC that relays the target's per DRB endpoints does.
      asn1::ngap::data_forwarding_resp_drb_item_s drb_item;
      drb_item.drb_id                            = 1;
      drb_item.dl_forwarding_up_tnl_info_present = true;
      auto& drb_tunnel                           = drb_item.dl_forwarding_up_tnl_info.set_gtp_tunnel();
      drb_tunnel.transport_layer_address.from_number(2887058953);
      drb_tunnel.gtp_teid.from_string("40000283");
      ho_cmd_transfer.data_forwarding_resp_drb_list.push_back(drb_item);
    }

    ho_item.ho_cmd_transfer = pack_into_pdu(ho_cmd_transfer);
  }
  ho_cmd->pdu_session_res_ho_list.push_back(ho_item);

  // Fill target to source transparent container.
  asn1::ngap::target_ngran_node_to_source_ngran_node_transparent_container_s transparent_container;
  // Fill RRC container.
  transparent_container.rrc_container =
      make_byte_buffer(
          "081a115568220201204550001e1004bcc012121600020509a0000193f7c7000000243434840be2e0260030258380f80408d078100009"
          "39dc601349798002692f120200046402051320c6b6c6bb003704020000080800041a235246c013497890000023271adb19127c058332"
          "55ff8092748837146e30dc71b9637dfab6387580221603400c162300e0102908024985950001ff000000000306e10840003c02ca0041"
          "8000001034c080a28500071c48000133557c841c001040c2050c1c9c48a163068e1e408800004280004005a8000864428000c645a800"
          "10024280014025a8001862428001c625a800200842800240c8200a0320902c0c8280c0320b0340c8300e0320d03c0c83810162080440"
          "e829024b92a4a1814388e8acf1379340e9041e2efc0c10e0000001c7feb311aa6ab940b000010cbb00000000000000000008422b5514"
          "011c00401020800388402710038082042000710804e10070204104000e21009c200e0608108001c420138601c10104100038840270c0"
          "020000002086020406080706800071c40000002004000806000809002200a60000231002271c00600040")
          .value();
  ho_cmd->target_to_source_transparent_container = pack_into_pdu(transparent_container);

  return ngap_msg;
}

ngap_handover_preparation_request ocudu::ocucp::generate_handover_preparation_request(
    cu_cp_ue_index_t                                          ue_index,
    const std::map<pdu_session_id_t, up_pdu_session_context>& pdu_sessions,
    nr_cell_identity                                          nci,
    uint32_t                                                  gnb_id_bit_length,
    plmn_identity                                             target_plmn)
{
  ngap_handover_preparation_request request = {};
  request.ue_index                          = ue_index;
  request.target_id.gnb_id                  = nci.gnb_id(gnb_id_bit_length);
  request.target_id.plmn = target_plmn, request.target_id.tac = 7;
  request.nci = nci;
  // Fill create a map of all PDU sessions and their associated QoS flows, grouped by DRB.
  for (const auto& pdu_session : pdu_sessions) {
    std::vector<cu_cp_drbs_to_qos_flows_map_item> drbs_to_qos_flows_map;
    for (const auto& drb : pdu_session.second.drbs) {
      cu_cp_drbs_to_qos_flows_map_item drb_item;
      drb_item.drb_id = drb.first;
      for (const auto& qos_flow : drb.second.qos_flows) {
        drb_item.associated_qos_flow_list.push_back(cu_cp_associated_qos_flow{qos_flow.first, std::nullopt});
      }
      drbs_to_qos_flows_map.push_back(drb_item);
    }
    request.pdu_sessions.insert({pdu_session.first, drbs_to_qos_flows_map});
  }

  return request;
}

ngap_message ocudu::ocucp::generate_valid_dl_ran_status_transfer(amf_ue_id_t                  amf_ue_id,
                                                                 ran_ue_id_t                  ran_ue_id,
                                                                 const std::vector<drb_id_t>& drb_ids)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_DL_RAN_STATUS_TRANSFER);

  auto& dl_status = ngap_msg.pdu.init_msg().value.dl_ran_status_transfer();

  dl_status->amf_ue_ngap_id = to_underlying(amf_ue_id);
  dl_status->ran_ue_ngap_id = to_underlying(ran_ue_id);

  for (drb_id_t drb_id : drb_ids) {
    asn1::ngap::drbs_subject_to_status_transfer_item_s drb_item;
    drb_item.drb_id = static_cast<uint8_t>(drb_id);
    drb_item.drb_status_ul.set_drb_status_ul12();
    drb_item.drb_status_dl.set_drb_status_dl12();

    dl_status->ran_status_transfer_transparent_container.drbs_subject_to_status_transfer_list.push_back(drb_item);
  }

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_handover_cancel_ack(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_successful_outcome();
  ngap_msg.pdu.successful_outcome().load_info_obj(ASN1_NGAP_ID_HO_CANCEL);

  auto& ho_cancel_ack = ngap_msg.pdu.successful_outcome().value.ho_cancel_ack();

  ho_cancel_ack->amf_ue_ngap_id = to_underlying(amf_ue_id);
  ho_cancel_ack->ran_ue_ngap_id = to_underlying(ran_ue_id);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_ng_reset_ack(const asn1::ngap::ue_associated_lc_ng_conn_list_l& ng_reset_ues)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_successful_outcome();
  ngap_msg.pdu.successful_outcome().load_info_obj(ASN1_NGAP_ID_NG_RESET);
  if (ng_reset_ues.size() > 0) {
    auto& ng_reset_ack                                  = ngap_msg.pdu.successful_outcome().value.ng_reset_ack();
    ng_reset_ack->ue_associated_lc_ng_conn_list_present = true;
    ng_reset_ack->ue_associated_lc_ng_conn_list         = ng_reset_ues;
  }

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_location_reporting_control_message(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg;

  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_LOCATION_REPORT_CTRL);

  auto& loc_rep_control = ngap_msg.pdu.init_msg().value.location_report_ctrl();

  loc_rep_control->amf_ue_ngap_id = to_underlying(amf_ue_id);
  loc_rep_control->ran_ue_ngap_id = to_underlying(ran_ue_id);

  // Fill location reporting type.
  loc_rep_control->location_report_request_type.event_type = asn1::ngap::event_type_opts::options::direct;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_location_reporting_control_message_with_cell_change(amf_ue_id_t amf_ue_id,
                                                                                        ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg = generate_location_reporting_control_message(amf_ue_id, ran_ue_id);
  ngap_msg.pdu.init_msg().value.location_report_ctrl()->location_report_request_type.event_type =
      asn1::ngap::event_type_opts::options::change_of_serve_cell;
  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_location_reporting_control_message_with_cell_change_and_ue_presence(
    amf_ue_id_t                 amf_ue_id,
    ran_ue_id_t                 ran_ue_id,
    const std::vector<uint8_t>& ref_ids)
{
  ngap_message ngap_msg = generate_location_reporting_control_message(amf_ue_id, ran_ue_id);

  auto& loc_rep_ctrl = ngap_msg.pdu.init_msg().value.location_report_ctrl();
  loc_rep_ctrl->location_report_request_type.event_type =
      asn1::ngap::event_type_opts::options::change_of_serving_cell_and_ue_presence_in_the_area_of_interest;

  for (uint8_t ref_id : ref_ids) {
    asn1::ngap::area_of_interest_item_s aoi_item;
    aoi_item.location_report_ref_id = ref_id;
    loc_rep_ctrl->location_report_request_type.area_of_interest_list.push_back(aoi_item);
  }

  return ngap_msg;
}

ngap_message
ocudu::ocucp::generate_location_reporting_control_message_with_ue_presence(amf_ue_id_t                 amf_ue_id,
                                                                           ran_ue_id_t                 ran_ue_id,
                                                                           const std::vector<uint8_t>& ref_ids)
{
  ngap_message ngap_msg = generate_location_reporting_control_message(amf_ue_id, ran_ue_id);

  auto& loc_rep_ctrl = ngap_msg.pdu.init_msg().value.location_report_ctrl();
  loc_rep_ctrl->location_report_request_type.event_type =
      asn1::ngap::event_type_opts::options::ue_presence_in_area_of_interest;

  for (uint8_t ref_id : ref_ids) {
    asn1::ngap::area_of_interest_item_s aoi_item;
    aoi_item.location_report_ref_id = ref_id;
    loc_rep_ctrl->location_report_request_type.area_of_interest_list.push_back(aoi_item);
  }

  return ngap_msg;
}

ngap_message
ocudu::ocucp::generate_path_switch_request_failure(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id, ngap_cause_t cause)
{
  ngap_message ngap_msg;
  ngap_msg.pdu.set_unsuccessful_outcome();
  ngap_msg.pdu.unsuccessful_outcome().load_info_obj(ASN1_NGAP_ID_PATH_SWITCH_REQUEST);
  auto& path_switch_req_fail = ngap_msg.pdu.unsuccessful_outcome().value.path_switch_request_fail();

  path_switch_req_fail->amf_ue_ngap_id = to_underlying(amf_ue_id);
  path_switch_req_fail->ran_ue_ngap_id = to_underlying(ran_ue_id);

  pdu_session_res_released_item_ps_fail_s pdu_session_res_released_item_ps_fail;
  pdu_session_res_released_item_ps_fail.pdu_session_id = 1;

  path_switch_request_unsuccessful_transfer_s path_switch_request_unsuccessful_transfer;
  path_switch_request_unsuccessful_transfer.cause = cause_to_asn1(cause);
  pdu_session_res_released_item_ps_fail.path_switch_request_unsuccessful_transfer =
      pack_into_pdu(path_switch_request_unsuccessful_transfer);

  path_switch_req_fail->pdu_session_res_released_list_ps_fail.push_back(pdu_session_res_released_item_ps_fail);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_path_switch_request_ack(amf_ue_id_t amf_ue_id, ran_ue_id_t ran_ue_id)
{
  ngap_message ngap_msg;
  ngap_msg.pdu.set_successful_outcome();
  ngap_msg.pdu.successful_outcome().load_info_obj(ASN1_NGAP_ID_PATH_SWITCH_REQUEST);
  auto& path_switch_req_ack = ngap_msg.pdu.successful_outcome().value.path_switch_request_ack();

  path_switch_req_ack->amf_ue_ngap_id = to_underlying(amf_ue_id);
  path_switch_req_ack->ran_ue_ngap_id = to_underlying(ran_ue_id);

  // Fill UE security capabilities.
  path_switch_req_ack->ue_security_cap.nr_encryption_algorithms.from_number(49152);
  path_switch_req_ack->ue_security_cap.nr_integrity_protection_algorithms.from_number(49152);
  path_switch_req_ack->ue_security_cap.eutr_aencryption_algorithms.from_number(0);
  path_switch_req_ack->ue_security_cap.eutr_aintegrity_protection_algorithms.from_number(0);

  // Fill security context.
  path_switch_req_ack->security_context.next_hop_chaining_count = 2;
  path_switch_req_ack->security_context.next_hop_nh.from_string(
      "1000100100100011110101001110000001001000000001111000010110011110001010011100101010110010000001010110110000100111"
      "0110101111001100000001100111100010001011111000111111101000100110011101110111100010101101000101000010100001000001"
      "00000101010000111100001010001001"); // 8923d4e04807859e29cab2056c276bcc06788be3fa267778ad1428410543c289

  // Fill PDU session resource switched list.
  pdu_session_res_switched_item_s pdu_session_res_switched_item;
  pdu_session_res_switched_item.pdu_session_id = 1;

  path_switch_request_ack_transfer_s path_switch_request_ack_transfer;
  pdu_session_res_switched_item.path_switch_request_ack_transfer = pack_into_pdu(path_switch_request_ack_transfer);
  path_switch_req_ack->pdu_session_res_switched_list.push_back(pdu_session_res_switched_item);

  // Fill allowed NSSAI.
  asn1::ngap::allowed_nssai_item_s allowed_nssai;
  allowed_nssai.s_nssai.sst.from_number(1);
  path_switch_req_ack->allowed_nssai.push_back(allowed_nssai);

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_path_switch_request_ack_with_ul_tunnel(amf_ue_id_t             amf_ue_id,
                                                                           ran_ue_id_t             ran_ue_id,
                                                                           pdu_session_id_t        psi,
                                                                           transport_layer_address ul_tla,
                                                                           gtpu_teid_t             ul_teid)
{
  ngap_message msg                 = generate_path_switch_request_ack(amf_ue_id, ran_ue_id);
  auto&        path_switch_req_ack = msg.pdu.successful_outcome().value.path_switch_request_ack();

  // Overwrite the PDU session switched list with one entry that carries UL tunnel info.
  path_switch_req_ack->pdu_session_res_switched_list.clear();

  pdu_session_res_switched_item_s switched_item;
  switched_item.pdu_session_id = to_underlying(psi);

  path_switch_request_ack_transfer_s ack_transfer;
  ack_transfer.ul_ngu_up_tnl_info_present = true;
  ack_transfer.ul_ngu_up_tnl_info.set_gtp_tunnel();
  tla_to_asn1_bitstring(ack_transfer.ul_ngu_up_tnl_info.gtp_tunnel().transport_layer_address, ul_tla);
  ack_transfer.ul_ngu_up_tnl_info.gtp_tunnel().gtp_teid.from_number(ul_teid.value());

  switched_item.path_switch_request_ack_transfer = pack_into_pdu(ack_transfer);
  path_switch_req_ack->pdu_session_res_switched_list.push_back(switched_item);

  return msg;
}

ngap_message ocudu::ocucp::generate_write_replace_warning_request()
{
  ngap_message ngap_msg;
  ngap_msg.pdu.set_init_msg();
  ngap_msg.pdu.init_msg().load_info_obj(ASN1_NGAP_ID_WRITE_REPLACE_WARNING);
  auto& req = ngap_msg.pdu.init_msg().value.write_replace_warning_request();

  req->msg_id.from_number(0x1234, 16);
  req->serial_num.from_number(0x0001, 16);
  req->repeat_period            = 4;
  req->nof_broadcasts_requested = 2;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_write_replace_warning_request_with_optionals()
{
  ngap_message ngap_msg = generate_write_replace_warning_request();
  auto&        req      = ngap_msg.pdu.init_msg().value.write_replace_warning_request();

  // Warning Type: ETWS primary notification (0x0180).
  req->warning_type_present = true;
  req->warning_type.from_string("0180");

  // Data Coding Scheme: 0x0f (GSM 7-bit default alphabet).
  req->data_coding_scheme_present = true;
  req->data_coding_scheme.from_number(0x0f, 8);

  // Warning Message Contents (short test message).
  req->warning_msg_contents_present = true;
  req->warning_msg_contents.resize(3);
  req->warning_msg_contents[0] = 0xaa;
  req->warning_msg_contents[1] = 0xbb;
  req->warning_msg_contents[2] = 0xcc;

  // Warning Area List: one NR CGI.
  req->warning_area_list_present   = true;
  auto&                nr_cgi_list = req->warning_area_list.set_nr_cgi_list_for_warning();
  asn1::ngap::nr_cgi_s asn1_cgi;
  asn1_cgi.plmn_id.from_string("00f110");
  asn1_cgi.nr_cell_id.from_number(0x19b01, 36);
  nr_cgi_list.push_back(asn1_cgi);

  // Concurrent Warning Message Indication.
  req->concurrent_warning_msg_ind_present = true;
  req->concurrent_warning_msg_ind.value   = asn1::ngap::concurrent_warning_msg_ind_opts::options::true_value;

  return ngap_msg;
}

ngap_message
ocudu::ocucp::generate_write_replace_warning_request_with_nr_cgi_list(const std::vector<nr_cell_global_id_t>& cgis)
{
  ngap_message ngap_msg = generate_write_replace_warning_request();
  auto&        req      = ngap_msg.pdu.init_msg().value.write_replace_warning_request();

  req->warning_area_list_present = true;
  auto& nr_cgi_list              = req->warning_area_list.set_nr_cgi_list_for_warning();
  for (const auto& cgi : cgis) {
    asn1::ngap::nr_cgi_s asn1_cgi;
    asn1_cgi.nr_cell_id.from_number(cgi.nci.value(), 36);
    asn1_cgi.plmn_id = cgi.plmn_id.to_bytes();
    nr_cgi_list.push_back(asn1_cgi);
  }

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_write_replace_warning_request_with_tai_list(const std::vector<tai_t>& tais)
{
  ngap_message ngap_msg = generate_write_replace_warning_request();
  auto&        req      = ngap_msg.pdu.init_msg().value.write_replace_warning_request();

  req->warning_area_list_present = true;
  auto& tai_list                 = req->warning_area_list.set_tai_list_for_warning();
  for (const auto& tai : tais) {
    asn1::ngap::tai_s asn1_tai;
    asn1_tai.plmn_id = tai.plmn_id.to_bytes();
    asn1_tai.tac.from_number(tai.tac);
    tai_list.push_back(asn1_tai);
  }

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_write_replace_warning_request_with_msg_no_dcs()
{
  ngap_message ngap_msg = generate_write_replace_warning_request();
  auto&        req      = ngap_msg.pdu.init_msg().value.write_replace_warning_request();

  // Warning Message Contents present but Data Coding Scheme absent.
  req->warning_msg_contents_present = true;
  req->warning_msg_contents.resize(3);
  req->warning_msg_contents[0] = 0xaa;
  req->warning_msg_contents[1] = 0xbb;
  req->warning_msg_contents[2] = 0xcc;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_write_replace_warning_request_cmas()
{
  ngap_message ngap_msg = generate_write_replace_warning_request();
  auto&        req      = ngap_msg.pdu.init_msg().value.write_replace_warning_request();

  req->data_coding_scheme_present = true;
  req->data_coding_scheme.from_number(0x0f, 8);
  req->warning_msg_contents_present = true;
  req->warning_msg_contents.resize(3);
  req->warning_msg_contents[0] = 0xaa;
  req->warning_msg_contents[1] = 0xbb;
  req->warning_msg_contents[2] = 0xcc;

  return ngap_msg;
}

ngap_message ocudu::ocucp::generate_write_replace_warning_request_etws_full()
{
  ngap_message ngap_msg = generate_write_replace_warning_request();
  auto&        req      = ngap_msg.pdu.init_msg().value.write_replace_warning_request();

  req->warning_type_present = true;
  req->warning_type.from_string("0180");
  req->data_coding_scheme_present = true;
  req->data_coding_scheme.from_number(0x0f, 8);
  req->warning_msg_contents_present = true;
  req->warning_msg_contents.resize(3);
  req->warning_msg_contents[0] = 0xaa;
  req->warning_msg_contents[1] = 0xbb;
  req->warning_msg_contents[2] = 0xcc;

  return ngap_msg;
}
