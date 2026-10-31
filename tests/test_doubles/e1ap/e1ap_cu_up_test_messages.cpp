// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "tests/test_doubles/e1ap/e1ap_cu_up_test_messages.h"
#include "ocudu/asn1/e1ap/common.h"
#include "ocudu/asn1/e1ap/e1ap_pdu_contents.h"
#include "ocudu/e1ap/common/e1ap_message.h"

using namespace ocudu;
using namespace ocuup;
using namespace asn1::e1ap;

cu_up_e1_setup_request ocudu::ocuup::generate_cu_up_e1_setup_request()
{
  cu_up_e1_setup_request request_msg = {};
  request_msg.gnb_cu_up_id           = 1;
  request_msg.gnb_cu_up_name         = "OCUDU CU-CP";
  request_msg.cn_support             = cu_up_cn_support_t::c_5gc;

  return request_msg;
}

e1ap_message ocudu::ocuup::generate_cu_up_e1_setup_response(unsigned transaction_id)
{
  e1ap_message e1_setup_response = {};
  e1_setup_response.pdu.set_successful_outcome();
  e1_setup_response.pdu.successful_outcome().load_info_obj(ASN1_E1AP_ID_GNB_CU_UP_E1_SETUP);

  auto& setup_res = e1_setup_response.pdu.successful_outcome().value.gnb_cu_up_e1_setup_resp();

  setup_res->transaction_id         = transaction_id;
  setup_res->gnb_cu_cp_name_present = true;
  setup_res->gnb_cu_cp_name.from_string("OCUDU CU-CP");

  return e1_setup_response;
}

e1ap_message ocudu::ocuup::generate_bearer_context_setup_request(unsigned cu_cp_ue_e1ap_id)
{
  e1ap_message bearer_context_setup_request = {};

  bearer_context_setup_request.pdu.set_init_msg();
  bearer_context_setup_request.pdu.init_msg().load_info_obj(ASN1_E1AP_ID_BEARER_CONTEXT_SETUP);

  auto& bearer_context_setup_req = bearer_context_setup_request.pdu.init_msg().value.bearer_context_setup_request();
  bearer_context_setup_req->gnb_cu_cp_ue_e1ap_id = cu_cp_ue_e1ap_id;
  bearer_context_setup_req->security_info.security_algorithm.ciphering_algorithm =
      asn1::e1ap::ciphering_algorithm_e::nea0;
  bearer_context_setup_req->security_info.up_securitykey.encryption_key.from_string("a6ae39efbe0d424cd85f4a9c3aee0414");
  bearer_context_setup_req->ue_dl_aggr_max_bit_rate = 1000000000U;
  bearer_context_setup_req->serving_plmn.from_string("02f899");
  bearer_context_setup_req->activity_notif_level        = asn1::e1ap::activity_notif_level_e::ue;
  bearer_context_setup_req->ue_inactivity_timer_present = true;
  bearer_context_setup_req->ue_inactivity_timer         = 60;

  bearer_context_setup_req->sys_bearer_context_setup_request.set_ng_ran_bearer_context_setup_request();

  auto& ng_ran_bearer_context_setup_req =
      bearer_context_setup_req->sys_bearer_context_setup_request.ng_ran_bearer_context_setup_request();

  asn1::e1ap::pdu_session_res_to_setup_item_s pdu_session_res_to_setup_item;
  pdu_session_res_to_setup_item.pdu_session_id   = 1;
  pdu_session_res_to_setup_item.pdu_session_type = asn1::e1ap::pdu_session_type_e::ipv4;
  pdu_session_res_to_setup_item.snssai.sst.from_number(1);
  pdu_session_res_to_setup_item.security_ind.integrity_protection_ind =
      asn1::e1ap::integrity_protection_ind_e::not_needed;
  pdu_session_res_to_setup_item.security_ind.confidentiality_protection_ind =
      asn1::e1ap::confidentiality_protection_ind_e::not_needed;
  pdu_session_res_to_setup_item.pdu_session_res_dl_ambr_present = true;
  pdu_session_res_to_setup_item.pdu_session_res_dl_ambr         = 100000000U;

  pdu_session_res_to_setup_item.ng_ul_up_tnl_info.set_gtp_tunnel();
  auto& gtp_tunnel = pdu_session_res_to_setup_item.ng_ul_up_tnl_info.gtp_tunnel();
  gtp_tunnel.transport_layer_address.from_number(2887058566);
  gtp_tunnel.gtp_teid.from_string("00000036");

  asn1::e1ap::drb_to_setup_item_ng_ran_s drb_to_setup_item_ng_ran;
  drb_to_setup_item_ng_ran.drb_id = 4;

  drb_to_setup_item_ng_ran.sdap_cfg.default_drb = asn1::e1ap::default_drb_e::true_value;
  drb_to_setup_item_ng_ran.sdap_cfg.sdap_hdr_ul = asn1::e1ap::sdap_hdr_ul_e::present;
  drb_to_setup_item_ng_ran.sdap_cfg.sdap_hdr_dl = asn1::e1ap::sdap_hdr_dl_e::absent;

  drb_to_setup_item_ng_ran.pdcp_cfg.pdcp_sn_size_ul                 = asn1::e1ap::pdcp_sn_size_e::s_neg18;
  drb_to_setup_item_ng_ran.pdcp_cfg.pdcp_sn_size_dl                 = asn1::e1ap::pdcp_sn_size_e::s_neg18;
  drb_to_setup_item_ng_ran.pdcp_cfg.rlc_mode                        = asn1::e1ap::rlc_mode_e::rlc_am;
  drb_to_setup_item_ng_ran.pdcp_cfg.t_reordering_timer_present      = true;
  drb_to_setup_item_ng_ran.pdcp_cfg.t_reordering_timer.t_reordering = asn1::e1ap::t_reordering_e::ms100;
  drb_to_setup_item_ng_ran.pdcp_cfg.discard_timer_present           = true;
  drb_to_setup_item_ng_ran.pdcp_cfg.discard_timer                   = asn1::e1ap::discard_timer_e::infinity;

  asn1::e1ap::cell_group_info_item_s cell_group_info_item = {};
  cell_group_info_item.cell_group_id                      = 0;
  drb_to_setup_item_ng_ran.cell_group_info.push_back(cell_group_info_item);

  asn1::e1ap::qos_flow_qos_param_item_s qo_s_flow_qos_param_item{};
  qo_s_flow_qos_param_item.qos_flow_id = 1;
  qo_s_flow_qos_param_item.qos_flow_level_qos_params.qos_characteristics.set_non_dyn_5qi();
  auto& qos_characteristics   = qo_s_flow_qos_param_item.qos_flow_level_qos_params.qos_characteristics.non_dyn_5qi();
  qos_characteristics.five_qi = 9;

  qo_s_flow_qos_param_item.qos_flow_level_qos_params.ngran_alloc_retention_prio.prio_level = 15;
  qo_s_flow_qos_param_item.qos_flow_level_qos_params.ngran_alloc_retention_prio.pre_emption_cap =
      asn1::e1ap::pre_emption_cap_e::shall_not_trigger_pre_emption;

  qo_s_flow_qos_param_item.qos_flow_level_qos_params.ngran_alloc_retention_prio.pre_emption_vulnerability =
      asn1::e1ap::pre_emption_vulnerability_e::pre_emptable;

  drb_to_setup_item_ng_ran.qos_flow_info_to_be_setup.push_back(qo_s_flow_qos_param_item);

  pdu_session_res_to_setup_item.drb_to_setup_list_ng_ran.push_back(drb_to_setup_item_ng_ran);

  ng_ran_bearer_context_setup_req.pdu_session_res_to_setup_list.push_back(pdu_session_res_to_setup_item);

  return bearer_context_setup_request;
}

e1ap_message ocudu::ocuup::generate_invalid_bearer_context_setup_request(unsigned cu_cp_ue_e1ap_id)
{
  e1ap_message bearer_context_setup_request = {};

  bearer_context_setup_request.pdu.set_init_msg();
  bearer_context_setup_request.pdu.init_msg().load_info_obj(ASN1_E1AP_ID_BEARER_CONTEXT_SETUP);

  auto& bearer_context_setup_req = bearer_context_setup_request.pdu.init_msg().value.bearer_context_setup_request();
  bearer_context_setup_req->gnb_cu_cp_ue_e1ap_id = cu_cp_ue_e1ap_id;
  bearer_context_setup_req->security_info.security_algorithm.ciphering_algorithm =
      asn1::e1ap::ciphering_algorithm_e::nea0;
  bearer_context_setup_req->security_info.up_securitykey.encryption_key.from_string("a6ae39efbe0d424cd85f4a9c3aee0414");
  bearer_context_setup_req->ue_dl_aggr_max_bit_rate = 1000000000U;
  bearer_context_setup_req->serving_plmn.from_string("02f899");
  bearer_context_setup_req->activity_notif_level = asn1::e1ap::activity_notif_level_e::ue;

  bearer_context_setup_req->sys_bearer_context_setup_request.set_e_utran_bearer_context_setup_request();

  return bearer_context_setup_request;
}

e1ap_message ocudu::ocuup::generate_invalid_bearer_context_setup_request_inactivity_timer(unsigned cu_cp_ue_e1ap_id)
{
  e1ap_message bearer_context_setup_request = {};
  bearer_context_setup_request              = generate_bearer_context_setup_request(cu_cp_ue_e1ap_id);
  // Invalidate UE inactivity timer
  auto& bearer_context_setup_req = bearer_context_setup_request.pdu.init_msg().value.bearer_context_setup_request();
  bearer_context_setup_req->ue_inactivity_timer_present = false;
  return bearer_context_setup_request;
}

e1ap_message ocudu::ocuup::generate_bearer_context_modification_request(unsigned cu_cp_ue_e1ap_id,
                                                                        unsigned cu_up_ue_e1ap_id)
{
  e1ap_message bearer_context_modification_request = {};

  bearer_context_modification_request.pdu.set_init_msg();
  bearer_context_modification_request.pdu.init_msg().load_info_obj(ASN1_E1AP_ID_BEARER_CONTEXT_MOD);
  auto& bearer_context_mod_req = bearer_context_modification_request.pdu.init_msg().value.bearer_context_mod_request();
  bearer_context_mod_req->gnb_cu_cp_ue_e1ap_id                   = cu_cp_ue_e1ap_id;
  bearer_context_mod_req->sys_bearer_context_mod_request_present = true;
  bearer_context_mod_req->sys_bearer_context_mod_request.set_ng_ran_bearer_context_mod_request();

  auto& ng_ran_bearer_context_mod_req =
      bearer_context_mod_req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request();

  ng_ran_bearer_context_mod_req.pdu_session_res_to_modify_list_present = true;

  asn1::e1ap::pdu_session_res_to_modify_item_s pdu_session_res_to_setup_modify_item = {};
  pdu_session_res_to_setup_modify_item.pdu_session_id                               = 1;

  asn1::e1ap::drb_to_modify_item_ng_ran_s drb_to_mod_item_ng_ran;
  drb_to_mod_item_ng_ran.drb_id = 4;

  asn1::e1ap::up_params_item_s up_params_item;
  up_params_item.up_tnl_info.set_gtp_tunnel();
  auto& gtp_tunnel = up_params_item.up_tnl_info.gtp_tunnel();
  gtp_tunnel.transport_layer_address.from_number(2887061624);
  gtp_tunnel.gtp_teid.from_string("7e9fcae5");
  up_params_item.cell_group_id = 0;

  drb_to_mod_item_ng_ran.dl_up_params.push_back(up_params_item);

  pdu_session_res_to_setup_modify_item.drb_to_modify_list_ng_ran.push_back(drb_to_mod_item_ng_ran);

  ng_ran_bearer_context_mod_req.pdu_session_res_to_modify_list.push_back(pdu_session_res_to_setup_modify_item);

  return bearer_context_modification_request;
}

e1ap_message
ocudu::ocuup::generate_bearer_context_modification_request_with_data_forwarding_info(unsigned      cu_cp_ue_e1ap_id,
                                                                                     uint32_t      fwd_teid,
                                                                                     const char*   fwd_addr,
                                                                                     qos_flow_id_t qos_flow_id)
{
  e1ap_message msg = {};
  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_E1AP_ID_BEARER_CONTEXT_MOD);

  auto& req                                   = msg.pdu.init_msg().value.bearer_context_mod_request();
  req->gnb_cu_cp_ue_e1ap_id                   = cu_cp_ue_e1ap_id;
  req->sys_bearer_context_mod_request_present = true;
  req->sys_bearer_context_mod_request.set_ng_ran_bearer_context_mod_request();

  auto& ng_ran = req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request();
  ng_ran.pdu_session_res_to_modify_list_present = true;

  asn1::e1ap::pdu_session_res_to_modify_item_s item                = {};
  item.pdu_session_id                                              = 1;
  item.pdu_session_data_forwarding_info_present                    = true;
  item.pdu_session_data_forwarding_info.dl_data_forwarding_present = true;
  auto& gtp = item.pdu_session_data_forwarding_info.dl_data_forwarding.set_gtp_tunnel();
  gtp.gtp_teid.from_number(fwd_teid);
  transport_layer_address addr = transport_layer_address::create_from_string(fwd_addr);
  tla_to_asn1_bitstring(gtp.transport_layer_address, addr);

  // Report the QoS flow carried on that tunnel (TS 37.483 section 9.3.2.6).
  item.pdu_session_data_forwarding_info.ie_exts_present                                             = true;
  item.pdu_session_data_forwarding_info.ie_exts.data_forwardingto_ng_ran_qos_flow_info_list_present = true;
  asn1::e1ap::data_forwardingto_ng_ran_qos_flow_info_list_item_s asn1_qos_flow_item;
  asn1_qos_flow_item.qos_flow_id = to_underlying(qos_flow_id);
  item.pdu_session_data_forwarding_info.ie_exts.data_forwardingto_ng_ran_qos_flow_info_list.push_back(
      asn1_qos_flow_item);

  ng_ran.pdu_session_res_to_modify_list.push_back(item);

  return msg;
}

e1ap_message ocudu::ocuup::generate_bearer_context_modification_request_with_ng_ul_tnl(unsigned    cu_cp_ue_e1ap_id,
                                                                                       uint32_t    upf_teid,
                                                                                       const char* upf_addr)
{
  e1ap_message msg = {};
  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_E1AP_ID_BEARER_CONTEXT_MOD);

  auto& req                                   = msg.pdu.init_msg().value.bearer_context_mod_request();
  req->gnb_cu_cp_ue_e1ap_id                   = cu_cp_ue_e1ap_id;
  req->sys_bearer_context_mod_request_present = true;
  req->sys_bearer_context_mod_request.set_ng_ran_bearer_context_mod_request();

  auto& ng_ran = req->sys_bearer_context_mod_request.ng_ran_bearer_context_mod_request();
  ng_ran.pdu_session_res_to_modify_list_present = true;

  asn1::e1ap::pdu_session_res_to_modify_item_s item = {};
  item.pdu_session_id                               = 1;
  item.ng_ul_up_tnl_info_present                    = true;
  item.ng_ul_up_tnl_info.set_gtp_tunnel();
  auto& gtp = item.ng_ul_up_tnl_info.gtp_tunnel();
  gtp.gtp_teid.from_number(upf_teid);
  // Encode the IPv4 address as a 32-bit big-endian bitstring.
  transport_layer_address addr = transport_layer_address::create_from_string(upf_addr);
  tla_to_asn1_bitstring(gtp.transport_layer_address, addr);

  ng_ran.pdu_session_res_to_modify_list.push_back(item);

  return msg;
}

e1ap_message ocudu::ocuup::generate_invalid_bearer_context_modification_request(unsigned cu_cp_ue_e1ap_id,
                                                                                unsigned cu_up_ue_e1ap_id)
{
  e1ap_message bearer_context_modification_request = {};

  bearer_context_modification_request.pdu.set_init_msg();
  bearer_context_modification_request.pdu.init_msg().load_info_obj(ASN1_E1AP_ID_BEARER_CONTEXT_MOD);

  auto& bearer_context_mod_req = bearer_context_modification_request.pdu.init_msg().value.bearer_context_mod_request();
  bearer_context_mod_req->gnb_cu_cp_ue_e1ap_id = cu_cp_ue_e1ap_id;
  bearer_context_mod_req->gnb_cu_up_ue_e1ap_id = cu_up_ue_e1ap_id;

  bearer_context_mod_req->sys_bearer_context_mod_request_present = true;
  bearer_context_mod_req->sys_bearer_context_mod_request.set_e_utran_bearer_context_mod_request();

  return bearer_context_modification_request;
}

e1ap_message ocudu::ocuup::generate_bearer_context_release_command(unsigned cu_cp_ue_e1ap_id, unsigned cu_up_ue_e1ap_id)
{
  e1ap_message bearer_context_release_command = {};

  bearer_context_release_command.pdu.set_init_msg();
  bearer_context_release_command.pdu.init_msg().load_info_obj(ASN1_E1AP_ID_BEARER_CONTEXT_RELEASE);

  auto& bearer_context_release_cmd = bearer_context_release_command.pdu.init_msg().value.bearer_context_release_cmd();
  bearer_context_release_cmd->gnb_cu_cp_ue_e1ap_id = cu_cp_ue_e1ap_id;
  bearer_context_release_cmd->gnb_cu_up_ue_e1ap_id = cu_up_ue_e1ap_id;
  bearer_context_release_cmd->cause.set_radio_network();

  return bearer_context_release_command;
}

e1ap_message
ocudu::ocuup::generate_e1_reset(const std::vector<std::pair<gnb_cu_cp_ue_e1ap_id_t, gnb_cu_up_ue_e1ap_id_t>>& ues)
{
  e1ap_message msg = {};

  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_E1AP_ID_RESET);

  reset_s& e1_reset = msg.pdu.init_msg().value.reset();
  if (ues.empty()) {
    e1_reset->reset_type.set_e1_interface();
  } else {
    ue_associated_lc_e1_conn_list_res_l& reset_part_of_e1_interface = e1_reset->reset_type.set_part_of_e1_interface();
    for (const auto& ue_ids : ues) {
      asn1::protocol_ie_single_container_s<asn1::e1ap::ue_associated_lc_e1_conn_item_res_o> item_container;
      asn1::e1ap::ue_associated_lc_e1_conn_item_s& conn_item = item_container->ue_associated_lc_e1_conn_item();

      if (ue_ids.first != gnb_cu_cp_ue_e1ap_id_t::invalid) {
        conn_item.gnb_cu_cp_ue_e1ap_id_present = true;
        conn_item.gnb_cu_cp_ue_e1ap_id         = to_underlying(ue_ids.first);
      }
      if (ue_ids.second != gnb_cu_up_ue_e1ap_id_t::invalid) {
        conn_item.gnb_cu_up_ue_e1ap_id_present = true;
        conn_item.gnb_cu_up_ue_e1ap_id         = to_underlying(ue_ids.second);
      }

      reset_part_of_e1_interface.push_back(item_container);
    }
  }

  return msg;
}
