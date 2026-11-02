// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "f1ap_test_messages.h"
#include "../lib/f1ap/asn1_helpers.h"
#include "../pdcp/pdcp_pdu_generator.h"
#include "../rrc/rrc_packed_test_messages.h"
#include "tests/test_doubles/utils/test_rng.h"
#include "ocudu/adt/format.h"
#include "ocudu/asn1/f1ap/common.h"
#include "ocudu/asn1/f1ap/f1ap_ies.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"
#include "ocudu/asn1/f1ap/f1ap_pdu_contents_ue.h"
#include "ocudu/f1ap/common/interface_management.h"
#include "ocudu/f1ap/f1ap_message.h"
#include "ocudu/ran/plmn_identity.h"
#include "ocudu/ran/positioning/positioning_ids.h"
#include "ocudu/ran/up_transport_layer_info.h"

using namespace ocudu;
using namespace asn1::f1ap;

f1ap_message ocudu::test_helpers::generate_f1ap_reset_message(
    const std::vector<std::pair<std::optional<gnb_du_ue_f1ap_id_t>, std::optional<gnb_cu_ue_f1ap_id_t>>>& ues_to_reset)
{
  f1ap_message msg;
  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_RESET);

  asn1::f1ap::reset_s& reset = msg.pdu.init_msg().value.reset();
  reset->cause.set_misc();
  reset->cause.misc().value = asn1::f1ap::cause_misc_opts::unspecified;

  if (ues_to_reset.empty()) {
    auto& f1 = reset->reset_type.set_f1_interface();
    f1.value = asn1::f1ap::reset_all_opts::reset_all;
  } else {
    auto& lst = reset->reset_type.set_part_of_f1_interface();
    lst.resize(ues_to_reset.size());
    for (unsigned i = 0; i != ues_to_reset.size(); ++i) {
      lst[i].load_info_obj(ASN1_F1AP_ID_UE_ASSOCIATED_LC_F1_CONN_ITEM);
      auto& conn                     = lst[i].value().ue_associated_lc_f1_conn_item();
      conn.gnb_du_ue_f1ap_id_present = ues_to_reset[i].first.has_value();
      if (conn.gnb_du_ue_f1ap_id_present) {
        conn.gnb_du_ue_f1ap_id = to_underlying(ues_to_reset[i].first.value());
      }
      conn.gnb_cu_ue_f1ap_id_present = ues_to_reset[i].second.has_value();
      if (conn.gnb_cu_ue_f1ap_id_present) {
        conn.gnb_cu_ue_f1ap_id = to_underlying(ues_to_reset[i].second.value());
      }
    }
  }

  return msg;
}

static byte_buffer generate_rrc_container(uint32_t pdcp_sn, unsigned pdu_len)
{
  return test_helpers::create_pdcp_pdu(
      pdcp_sn_size::size12bits, true, pdcp_sn, pdu_len, test_rng::uniform_int<uint8_t>());
}

gnb_du_served_cells_item_s ocudu::test_helpers::generate_served_cells_item(const served_cell_item_info& info)
{
  gnb_du_served_cells_item_s served_cells_item;
  served_cells_item.served_cell_info.nr_cgi.plmn_id = info.plmn_id.to_bytes();
  served_cells_item.served_cell_info.nr_cgi.nr_cell_id.from_number(info.nci.value());
  served_cells_item.served_cell_info.nr_pci              = info.pci;
  served_cells_item.served_cell_info.five_gs_tac_present = true;
  served_cells_item.served_cell_info.five_gs_tac.from_number(info.tac);

  served_plmns_item_s served_plmn;
  served_plmn.plmn_id = info.plmn_id.to_bytes();
  slice_support_item_s slice_support_item;
  slice_support_item.snssai.sst.from_number(1);
  served_plmn.ie_exts.tai_slice_support_list_present = true;
  served_plmn.ie_exts.tai_slice_support_list.push_back(slice_support_item);
  served_cells_item.served_cell_info.served_plmns.push_back(served_plmn);

  served_cells_item.served_cell_info.nr_mode_info.set_tdd();
  served_cells_item.served_cell_info.nr_mode_info.tdd().nr_freq_info.nr_arfcn = info.nr_arfcn.value();
  freq_band_nr_item_s freq_band_nr_item;
  freq_band_nr_item.freq_band_ind_nr = static_cast<uint16_t>(info.band);
  served_cells_item.served_cell_info.nr_mode_info.tdd().nr_freq_info.freq_band_list_nr.push_back(freq_band_nr_item);
  served_cells_item.served_cell_info.nr_mode_info.tdd().tx_bw.nr_scs.value = nr_scs_opts::scs30;
  served_cells_item.served_cell_info.nr_mode_info.tdd().tx_bw.nr_nrb.value = nr_nrb_opts::nrb51;
  served_cells_item.served_cell_info.meas_timing_cfg =
      test_helpers::create_meas_timing_cfg(info.meas_timing_cfg.carrier_freq, info.meas_timing_cfg.scs);

  served_cells_item.gnb_du_sys_info_present = true;
  served_cells_item.gnb_du_sys_info.mib_msg.from_string("02000c");
  served_cells_item.gnb_du_sys_info.sib1_msg.from_string(info.sib1_str);

  return served_cells_item;
}

f1ap_message ocudu::test_helpers::generate_f1_setup_request(gnb_du_id_t                               gnb_du_id,
                                                            const std::vector<served_cell_item_info>& cells)
{
  f1ap_message msg;
  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_F1_SETUP);

  auto& setup_req                = msg.pdu.init_msg().value.f1_setup_request();
  setup_req->transaction_id      = 99;
  setup_req->gnb_du_id           = (uint64_t)gnb_du_id;
  setup_req->gnb_du_name_present = true;
  setup_req->gnb_du_name.from_string("OCUDU DU");
  setup_req->gnb_du_rrc_version.latest_rrc_version.from_number(1);
  setup_req->gnb_du_served_cells_list_present = true;
  setup_req->gnb_du_served_cells_list.resize(cells.size());
  for (unsigned i = 0; i != cells.size(); ++i) {
    setup_req->gnb_du_served_cells_list[i].load_info_obj(ASN1_F1AP_ID_GNB_DU_SERVED_CELLS_ITEM);
    setup_req->gnb_du_served_cells_list[i].value().gnb_du_served_cells_item() = generate_served_cells_item(cells[i]);
  }

  return msg;
}

f1ap_message ocudu::test_helpers::generate_f1_setup_response(const f1ap_message& f1_setup_request, bool activate_cells)
{
  ocudu_assert(f1_setup_request.pdu.type().value == f1ap_pdu_c::types_opts::init_msg, "Expected F1 setup request");
  ocudu_assert(f1_setup_request.pdu.init_msg().value.type().value ==
                   f1ap_elem_procs_o::init_msg_c::types_opts::f1_setup_request,
               "Expected F1 setup request");
  const auto& req = f1_setup_request.pdu.init_msg().value.f1_setup_request();

  f1ap_message resp;
  resp.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_F1_SETUP);
  f1_setup_resp_s& f1_setup_resp = resp.pdu.successful_outcome().value.f1_setup_resp();

  f1_setup_resp->transaction_id      = req->transaction_id;
  f1_setup_resp->gnb_cu_name_present = true;
  f1_setup_resp->gnb_cu_name.from_string("ocu");
  f1_setup_resp->gnb_cu_rrc_version.latest_rrc_version.from_number(2);

  f1_setup_resp->cells_to_be_activ_list_present = activate_cells;
  if (activate_cells) {
    f1_setup_resp->cells_to_be_activ_list.resize(req->gnb_du_served_cells_list.size());
    for (unsigned i = 0; i != req->gnb_du_served_cells_list.size(); ++i) {
      const auto& req_cell = req->gnb_du_served_cells_list[i]->gnb_du_served_cells_item();
      f1_setup_resp->cells_to_be_activ_list[i].load_info_obj(ASN1_F1AP_ID_CELLS_TO_BE_ACTIV_LIST_ITEM);
      auto& cell          = f1_setup_resp->cells_to_be_activ_list[i].value().cells_to_be_activ_list_item();
      cell.nr_cgi         = req_cell.served_cell_info.nr_cgi;
      cell.nr_pci_present = true;
      cell.nr_pci         = req_cell.served_cell_info.nr_pci;
    }
  }

  return resp;
}

f1ap_message ocudu::test_helpers::generate_f1_setup_failure(const f1ap_message& f1_setup_request)
{
  ocudu_assert(f1_setup_request.pdu.type().value == f1ap_pdu_c::types_opts::init_msg, "Expected F1 setup request");
  ocudu_assert(f1_setup_request.pdu.init_msg().value.type().value ==
                   f1ap_elem_procs_o::init_msg_c::types_opts::f1_setup_request,
               "Expected F1 setup request");
  const auto& req = f1_setup_request.pdu.init_msg().value.f1_setup_request();

  f1ap_message resp;
  resp.pdu.set_unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_F1_SETUP);
  f1_setup_fail_s& f1_setup_fail = resp.pdu.unsuccessful_outcome().value.f1_setup_fail();

  f1_setup_fail->transaction_id = req->transaction_id;
  f1_setup_fail->cause.set(cause_c::types_opts::misc);
  f1_setup_fail->cause.misc().value = cause_misc_opts::unspecified;

  return resp;
}

f1ap_message ocudu::test_helpers::generate_gnb_du_configuration_update(
    gnb_du_id_t                                                               gnb_du_id,
    const std::vector<served_cell_item_info>&                                 cells_to_add,
    const std::vector<std::pair<nr_cell_global_id_t, served_cell_item_info>>& cells_to_mod,
    const std::vector<nr_cell_global_id_t>&                                   cells_to_rem)
{
  f1ap_message msg;
  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_GNB_DU_CFG_UPD);

  auto& upd              = msg.pdu.init_msg().value.gnb_du_cfg_upd();
  upd->transaction_id    = 98;
  upd->gnb_du_id_present = true;
  upd->gnb_du_id         = (uint64_t)gnb_du_id;

  if (not cells_to_add.empty()) {
    upd->served_cells_to_add_list_present = true;
    upd->served_cells_to_add_list.resize(cells_to_add.size());
    for (unsigned i = 0; i != cells_to_add.size(); ++i) {
      upd->served_cells_to_add_list[i].load_info_obj(ASN1_F1AP_ID_SERVED_CELLS_TO_ADD_ITEM);
      auto& item            = upd->served_cells_to_add_list[i].value().served_cells_to_add_item();
      auto  served          = generate_served_cells_item(cells_to_add[i]);
      item.served_cell_info = served.served_cell_info;
      if (served.gnb_du_sys_info_present) {
        item.gnb_du_sys_info_present = true;
        item.gnb_du_sys_info         = served.gnb_du_sys_info;
      }
    }
  }

  if (not cells_to_mod.empty()) {
    upd->served_cells_to_modify_list_present = true;
    upd->served_cells_to_modify_list.resize(cells_to_mod.size());
    for (unsigned i = 0; i != cells_to_mod.size(); ++i) {
      upd->served_cells_to_modify_list[i].load_info_obj(ASN1_F1AP_ID_SERVED_CELLS_TO_MODIFY_ITEM);
      auto& item              = upd->served_cells_to_modify_list[i].value().served_cells_to_modify_item();
      item.old_nr_cgi.plmn_id = cells_to_mod[i].first.plmn_id.to_bytes();
      item.old_nr_cgi.nr_cell_id.from_number(cells_to_mod[i].first.nci.value());
      auto served           = generate_served_cells_item(cells_to_mod[i].second);
      item.served_cell_info = served.served_cell_info;
      if (served.gnb_du_sys_info_present) {
        item.gnb_du_sys_info_present = true;
        item.gnb_du_sys_info         = served.gnb_du_sys_info;
      }
    }
  }

  if (not cells_to_rem.empty()) {
    upd->served_cells_to_delete_list_present = true;
    upd->served_cells_to_delete_list.resize(cells_to_rem.size());
    for (unsigned i = 0; i != cells_to_rem.size(); ++i) {
      upd->served_cells_to_delete_list[i].load_info_obj(ASN1_F1AP_ID_SERVED_CELLS_TO_DELETE_ITEM);
      auto& item              = upd->served_cells_to_delete_list[i].value().served_cells_to_delete_item();
      item.old_nr_cgi.plmn_id = cells_to_rem[i].plmn_id.to_bytes();
      item.old_nr_cgi.nr_cell_id.from_number(cells_to_rem[i].nci.value());
    }
  }

  return msg;
}

f1ap_message
ocudu::test_helpers::generate_gnb_du_configuration_update_acknowledge(const f1ap_message& gnb_du_config_update)
{
  const gnb_du_cfg_upd_s& req = gnb_du_config_update.pdu.init_msg().value.gnb_du_cfg_upd();

  f1ap_message msg;
  msg.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_GNB_DU_CFG_UPD);

  asn1::f1ap::gnb_du_cfg_upd_ack_s& ack = msg.pdu.successful_outcome().value.gnb_du_cfg_upd_ack();
  ack->transaction_id                   = req->transaction_id;

  return msg;
}

f1ap_message ocudu::test_helpers::generate_gnb_du_configuration_update_failure(const f1ap_message& gnb_du_config_update)
{
  const gnb_du_cfg_upd_s& req = gnb_du_config_update.pdu.init_msg().value.gnb_du_cfg_upd();

  f1ap_message msg;
  msg.pdu.set_unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_GNB_DU_CFG_UPD);
  asn1::f1ap::gnb_du_cfg_upd_fail_s& fail = msg.pdu.unsuccessful_outcome().value.gnb_du_cfg_upd_fail();
  fail->transaction_id                    = req->transaction_id;
  fail->cause.set_misc().value            = cause_misc_opts::unspecified;

  return msg;
}

f1ap_message
ocudu::test_helpers::generate_gnb_cu_configuration_update_request(unsigned                        transaction_id,
                                                                  span<const nr_cell_global_id_t> cgis_to_activate,
                                                                  span<const nr_cell_global_id_t> cgis_to_deactivate,
                                                                  span<const f1ap_cell_to_bar>    cells_to_bar)
{
  f1ap_message msg;

  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_GNB_CU_CFG_UPD);
  gnb_cu_cfg_upd_s& req = msg.pdu.init_msg().value.gnb_cu_cfg_upd();
  req->transaction_id   = transaction_id;

  req->cells_to_be_activ_list_present = not cgis_to_activate.empty();
  req->cells_to_be_activ_list.resize(cgis_to_activate.size());
  for (unsigned i = 0, e = cgis_to_activate.size(); i != e; ++i) {
    req->cells_to_be_activ_list[i].load_info_obj(ASN1_F1AP_ID_CELLS_TO_BE_ACTIV_LIST_ITEM);
    req->cells_to_be_activ_list[i].value().cells_to_be_activ_list_item().nr_cgi = cgi_to_asn1(cgis_to_activate[i]);
  }
  req->cells_to_be_deactiv_list_present = not cgis_to_deactivate.empty();
  req->cells_to_be_deactiv_list.resize(cgis_to_deactivate.size());
  for (unsigned i = 0, e = cgis_to_deactivate.size(); i != e; ++i) {
    req->cells_to_be_deactiv_list[i].load_info_obj(ASN1_F1AP_ID_CELLS_TO_BE_DEACTIV_LIST_ITEM);
    req->cells_to_be_deactiv_list[i].value().cells_to_be_deactiv_list_item().nr_cgi =
        cgi_to_asn1(cgis_to_deactivate[i]);
  }
  req->cells_to_be_barred_list_present = not cells_to_bar.empty();
  req->cells_to_be_barred_list.resize(cells_to_bar.size());
  for (unsigned i = 0, e = cells_to_bar.size(); i != e; ++i) {
    req->cells_to_be_barred_list[i].load_info_obj(ASN1_F1AP_ID_CELLS_TO_BE_BARRED_ITEM);
    auto& asn1_item             = req->cells_to_be_barred_list[i].value().cells_to_be_barred_item();
    asn1_item.nr_cgi            = cgi_to_asn1(cells_to_bar[i].cgi);
    asn1_item.cell_barred.value = cells_to_bar[i].barred ? cell_barred_opts::barred : cell_barred_opts::not_barred;
  }

  return msg;
}

f1ap_message ocudu::test_helpers::generate_gnb_cu_configuration_update_acknowledgement(
    const std::vector<f1ap_cell_failed_to_activate>& cells_failed_to_activate)
{
  f1ap_message gnb_cu_configuration_update_ack = {};

  gnb_cu_configuration_update_ack.pdu.set_successful_outcome();
  gnb_cu_configuration_update_ack.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_GNB_CU_CFG_UPD);

  auto& gnb_cu_cfg_upd_ack = gnb_cu_configuration_update_ack.pdu.successful_outcome().value.gnb_cu_cfg_upd_ack();

  gnb_cu_cfg_upd_ack->cells_failed_to_be_activ_list_present = !cells_failed_to_activate.empty();
  for (const auto& cell : cells_failed_to_activate) {
    asn1::protocol_ie_single_container_s<asn1::f1ap::cells_failed_to_be_activ_list_item_ies_o> asn1_cell_container;
    cells_failed_to_be_activ_list_item_s& asn1_cell = asn1_cell_container->cells_failed_to_be_activ_list_item();

    asn1_cell.nr_cgi = cgi_to_asn1(cell.cgi);
    asn1_cell.cause  = cause_to_asn1(cell.cause);

    gnb_cu_cfg_upd_ack->cells_failed_to_be_activ_list.push_back(asn1_cell_container);
  }

  return gnb_cu_configuration_update_ack;
}

f1ap_message ocudu::test_helpers::generate_gnb_cu_configuration_update_failure()
{
  f1ap_message gnb_cu_configuration_update_failure = {};

  gnb_cu_configuration_update_failure.pdu.set_unsuccessful_outcome();
  gnb_cu_configuration_update_failure.pdu.unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_GNB_CU_CFG_UPD);

  auto& gnb_cu_cfg_upd_fail =
      gnb_cu_configuration_update_failure.pdu.unsuccessful_outcome().value.gnb_cu_cfg_upd_fail();
  gnb_cu_cfg_upd_fail->cause.set_radio_network();
  gnb_cu_cfg_upd_fail->cause.radio_network() = cause_radio_network_opts::options::interaction_with_other_proc;

  return gnb_cu_configuration_update_failure;
}

f1ap_message ocudu::test_helpers::generate_f1_removal_request(unsigned transaction_id)
{
  f1ap_message req;
  req.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_F1_REMOVAL);
  f1_removal_request_s& ie = req.pdu.init_msg().value.f1_removal_request();

  ie->transaction_id = transaction_id;

  return req;
}

f1ap_message ocudu::test_helpers::generate_f1_removal_response(const f1ap_message& f1_removal_request)
{
  ocudu_assert(f1_removal_request.pdu.type().value == f1ap_pdu_c::types_opts::init_msg, "Expected F1 removal request");
  ocudu_assert(f1_removal_request.pdu.init_msg().value.type().value ==
                   f1ap_elem_procs_o::init_msg_c::types_opts::f1_removal_request,
               "Expected F1 removal request");

  f1ap_message resp;
  resp.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_F1_REMOVAL);
  f1_removal_resp_s& ie = resp.pdu.successful_outcome().value.f1_removal_resp();
  ie->transaction_id    = f1_removal_request.pdu.init_msg().value.f1_removal_request()->transaction_id;

  return resp;
}

static drbs_to_be_setup_item_s generate_drb_am_setup_item(drb_id_t drbid)
{
  using namespace asn1::f1ap;

  drbs_to_be_setup_item_s drb;
  drb.drb_id = to_underlying(drbid);
  drb.qos_info.set_choice_ext().load_info_obj(ASN1_F1AP_ID_DRB_INFO);
  auto& drb_info                                                 = drb.qos_info.choice_ext()->drb_info();
  drb_info.drb_qos.qos_characteristics.set_non_dyn_5qi().five_qi = 9;
  drb_info.drb_qos.ngran_alloc_retention_prio.prio_level         = 1;
  drb_info.drb_qos.ngran_alloc_retention_prio.pre_emption_cap.value =
      pre_emption_cap_opts::shall_not_trigger_pre_emption;
  drb_info.drb_qos.ngran_alloc_retention_prio.pre_emption_vulnerability.value =
      pre_emption_vulnerability_opts::not_pre_emptable;
  drb_info.drb_qos.reflective_qos_attribute_present = true;
  drb_info.drb_qos.reflective_qos_attribute.value =
      qos_flow_level_qos_params_s::reflective_qos_attribute_opts::subject_to;
  drb_info.snssai.sst.from_string("01");
  drb_info.snssai.sd.from_string("0027db");
  drb_info.flows_mapped_to_drb_list.resize(1);
  drb_info.flows_mapped_to_drb_list[0].qos_flow_id = 1;
  drb_info.flows_mapped_to_drb_list[0].qos_flow_level_qos_params.qos_characteristics =
      drb_info.drb_qos.qos_characteristics;
  drb_info.flows_mapped_to_drb_list[0].qos_flow_level_qos_params.ngran_alloc_retention_prio =
      drb_info.drb_qos.ngran_alloc_retention_prio;
  drb.rlc_mode.value         = rlc_mode_opts::rlc_am;
  drb.ie_exts_present        = true;
  drb.ie_exts.dl_pdcp_sn_len = pdcp_sn_len_opts::twelve_bits;
  drb.ul_up_tnl_info_to_be_setup_list.resize(1);
  auto& gtp_tun = drb.ul_up_tnl_info_to_be_setup_list[0].ul_up_tnl_info.set_gtp_tunnel();
  auto  addr    = transport_layer_address::create_from_string("127.0.0.1");
  tla_to_asn1_bitstring(gtp_tun.transport_layer_address, addr);
  gtp_tun.gtp_teid.from_number(1);

  return drb;
}

f1ap_message ocudu::test_helpers::generate_ue_context_setup_request(gnb_cu_ue_f1ap_id_t                cu_ue_id,
                                                                    std::optional<gnb_du_ue_f1ap_id_t> du_ue_id,
                                                                    uint32_t                     rrc_container_pdcp_sn,
                                                                    const std::vector<drb_id_t>& drbs_to_setup,
                                                                    nr_cell_global_id_t          nr_cgi)
{
  using namespace asn1::f1ap;
  f1ap_message msg;

  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_SETUP);
  ue_context_setup_request_s& dl_msg = msg.pdu.init_msg().value.ue_context_setup_request();

  dl_msg->gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  if (du_ue_id.has_value()) {
    dl_msg->gnb_du_ue_f1ap_id_present = true;
    dl_msg->gnb_du_ue_f1ap_id         = (unsigned)*du_ue_id;
  }

  // spCell.
  dl_msg->sp_cell_id.plmn_id = nr_cgi.plmn_id.to_bytes();
  dl_msg->sp_cell_id.nr_cell_id.from_number(nr_cgi.nci.value());

  // SRB2.
  dl_msg->srbs_to_be_setup_list_present = true;
  dl_msg->srbs_to_be_setup_list.resize(1);
  dl_msg->srbs_to_be_setup_list[0].load_info_obj(ASN1_F1AP_ID_SRBS_SETUP_ITEM);
  srbs_to_be_setup_item_s& srb2 = dl_msg->srbs_to_be_setup_list[0]->srbs_to_be_setup_item();
  srb2.srb_id                   = 2;

  // drbs-to-be-setup.
  dl_msg->drbs_to_be_setup_list_present = drbs_to_setup.size() > 0;
  dl_msg->drbs_to_be_setup_list.resize(drbs_to_setup.size());
  unsigned count = 0;
  for (drb_id_t drbid : drbs_to_setup) {
    dl_msg->drbs_to_be_setup_list[count].load_info_obj(ASN1_F1AP_ID_DRB_INFO);
    dl_msg->drbs_to_be_setup_list[count]->drbs_to_be_setup_item() = generate_drb_am_setup_item(drbid);
    ++count;
  }

  if (du_ue_id.has_value()) {
    // Note: In case of a new UE being created, an RRC container should not be stored, as it would have to wait for
    // the C-RNTI CE.
    dl_msg->rrc_container_present = true;
    bool success                  = dl_msg->rrc_container.append(
        generate_rrc_container(rrc_container_pdcp_sn, test_rng::uniform_int<unsigned>(3, 100)));
    report_error_if_not(success, "Failed to allocate RRC container");
  }

  return msg;
}

f1ap_message ocudu::test_helpers::generate_ue_context_setup_response(gnb_cu_ue_f1ap_id_t          cu_ue_id,
                                                                     gnb_du_ue_f1ap_id_t          du_ue_id,
                                                                     std::optional<rnti_t>        crnti,
                                                                     byte_buffer                  cell_group_config,
                                                                     const std::vector<drb_id_t>& drbs_setup_list)
{
  f1ap_message ue_context_setup_response = {};

  ue_context_setup_response.pdu.set_successful_outcome();
  ue_context_setup_response.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_SETUP);

  auto& ue_context_setup_resp = ue_context_setup_response.pdu.successful_outcome().value.ue_context_setup_resp();
  ue_context_setup_resp->gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  ue_context_setup_resp->gnb_du_ue_f1ap_id = (unsigned)du_ue_id;

  if (crnti.has_value()) {
    ue_context_setup_resp->c_rnti_present = true;
    ue_context_setup_resp->c_rnti         = (unsigned)crnti.value();
  }

  ue_context_setup_resp->du_to_cu_rrc_info.cell_group_cfg = cell_group_config.copy();

  ue_context_setup_resp->drbs_setup_list_present = !drbs_setup_list.empty();
  for (const auto& drb : drbs_setup_list) {
    ue_context_setup_resp->drbs_setup_list.push_back({});
    ue_context_setup_resp->drbs_setup_list.back().load_info_obj(ASN1_F1AP_ID_DRBS_SETUP_ITEM);
    ue_context_setup_resp->drbs_setup_list.back().value().drbs_setup_item().drb_id = to_underlying(drb);
  }

  return ue_context_setup_response;
}

f1ap_message ocudu::test_helpers::generate_ue_context_setup_failure(gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                    gnb_du_ue_f1ap_id_t du_ue_id)
{
  f1ap_message ue_context_setup_failure = {};

  ue_context_setup_failure.pdu.set_unsuccessful_outcome();
  ue_context_setup_failure.pdu.unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_SETUP);

  auto& ue_context_setup_fail = ue_context_setup_failure.pdu.unsuccessful_outcome().value.ue_context_setup_fail();
  ue_context_setup_fail->gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  ue_context_setup_fail->gnb_du_ue_f1ap_id = (unsigned)du_ue_id;
  ue_context_setup_fail->cause.set_radio_network();
  ue_context_setup_fail->cause.radio_network() =
      cause_radio_network_opts::options::unknown_or_already_allocated_gnb_cu_ue_f1ap_id;

  return ue_context_setup_failure;
}

f1ap_message ocudu::test_helpers::generate_ue_context_release_request(gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                      gnb_du_ue_f1ap_id_t du_ue_id)
{
  f1ap_message msg;
  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_RELEASE_REQUEST);

  auto& release_req              = msg.pdu.init_msg().value.ue_context_release_request();
  release_req->gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  release_req->gnb_du_ue_f1ap_id = (unsigned)du_ue_id;
  release_req->cause.set_radio_network();
  release_req->cause.radio_network().value = cause_radio_network_e::rl_fail_others;

  return msg;
}

f1ap_message ocudu::test_helpers::generate_ue_context_release_command(gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                      gnb_du_ue_f1ap_id_t du_ue_id,
                                                                      srb_id_t            srb_id,
                                                                      byte_buffer         rrc_container)
{
  f1ap_message msg;
  msg.pdu.set_init_msg();
  msg.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_RELEASE);

  auto& release_cmd              = msg.pdu.init_msg().value.ue_context_release_cmd();
  release_cmd->gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  release_cmd->gnb_du_ue_f1ap_id = (unsigned)du_ue_id;
  release_cmd->cause.set_radio_network();
  release_cmd->cause.radio_network().value = cause_radio_network_e::unspecified;
  if (not rrc_container.empty()) {
    release_cmd->srb_id_present        = true;
    release_cmd->srb_id                = srb_id_to_uint(srb_id);
    release_cmd->rrc_container_present = true;
    release_cmd->rrc_container         = std::move(rrc_container);
  }

  return msg;
}

f1ap_message ocudu::test_helpers::generate_ue_context_release_complete(const f1ap_message& ue_ctxt_release_cmd)
{
  ocudu_assert(ue_ctxt_release_cmd.pdu.type().value == f1ap_pdu_c::types_opts::init_msg, "Invalid argument message");
  ocudu_assert(ue_ctxt_release_cmd.pdu.init_msg().value.type().value ==
                   f1ap_elem_procs_o::init_msg_c::types_opts::ue_context_release_cmd,
               "Invalid argument message");

  const ue_context_release_cmd_s& cmd = ue_ctxt_release_cmd.pdu.init_msg().value.ue_context_release_cmd();

  return generate_ue_context_release_complete(int_to_gnb_cu_ue_f1ap_id(cmd->gnb_cu_ue_f1ap_id),
                                              int_to_gnb_du_ue_f1ap_id(cmd->gnb_du_ue_f1ap_id));
}

f1ap_message ocudu::test_helpers::generate_ue_context_release_complete(gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                       gnb_du_ue_f1ap_id_t du_ue_id)
{
  f1ap_message ue_ctxt_rel_complete_msg = {};
  ue_ctxt_rel_complete_msg.pdu.set_successful_outcome();
  ue_ctxt_rel_complete_msg.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_RELEASE);
  ue_context_release_complete_s& rel_complete_msg =
      ue_ctxt_rel_complete_msg.pdu.successful_outcome().value.ue_context_release_complete();

  rel_complete_msg->gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  rel_complete_msg->gnb_du_ue_f1ap_id = (unsigned)du_ue_id;

  return ue_ctxt_rel_complete_msg;
}

static asn1::f1ap::ul_up_tnl_info_to_be_setup_list_l generate_ul_up_tnl_info_to_be_setup_list_l()
{
  asn1::f1ap::ul_up_tnl_info_to_be_setup_list_l list;

  list.resize(1);
  auto& gtp_tun = list[0].ul_up_tnl_info.set_gtp_tunnel();
  auto  addr    = transport_layer_address::create_from_string("127.0.0.1");
  tla_to_asn1_bitstring(gtp_tun.transport_layer_address, addr);
  gtp_tun.gtp_teid.from_number(1);

  return list;
}

static asn1::f1ap::drbs_to_be_modified_item_s generate_to_modify_drb_am_mod_item(drb_id_t drbid)
{
  using namespace asn1::f1ap;

  drbs_to_be_modified_item_s drb;
  drb.drb_id                          = to_underlying(drbid);
  drb.ie_exts_present                 = true;
  drb.ie_exts.dl_pdcp_sn_len_present  = true;
  drb.ie_exts.dl_pdcp_sn_len          = pdcp_sn_len_opts::twelve_bits;
  drb.ul_up_tnl_info_to_be_setup_list = generate_ul_up_tnl_info_to_be_setup_list_l();
  return drb;
}

static asn1::f1ap::drbs_to_be_setup_mod_item_s generate_drb_am_mod_item(drb_id_t drbid)
{
  using namespace asn1::f1ap;
  drbs_to_be_setup_mod_item_s drb;
  drb.drb_id = to_underlying(drbid);
  drb.qos_info.set_choice_ext().load_info_obj(ASN1_F1AP_ID_DRB_INFO);
  auto& drb_info                                                 = drb.qos_info.choice_ext()->drb_info();
  drb_info.drb_qos.qos_characteristics.set_non_dyn_5qi().five_qi = 8;
  drb_info.drb_qos.ngran_alloc_retention_prio.prio_level         = 1;
  drb_info.drb_qos.ngran_alloc_retention_prio.pre_emption_cap.value =
      pre_emption_cap_opts::shall_not_trigger_pre_emption;
  drb_info.drb_qos.ngran_alloc_retention_prio.pre_emption_vulnerability.value =
      pre_emption_vulnerability_opts::not_pre_emptable;
  drb_info.drb_qos.reflective_qos_attribute_present = true;
  drb_info.drb_qos.reflective_qos_attribute.value =
      qos_flow_level_qos_params_s::reflective_qos_attribute_opts::subject_to;
  drb_info.flows_mapped_to_drb_list.resize(1);
  drb_info.flows_mapped_to_drb_list[0].qos_flow_id = 0;
  auto& qos_flow                                   = drb_info.flows_mapped_to_drb_list[0].qos_flow_level_qos_params;
  auto& desc                                       = qos_flow.qos_characteristics.set_non_dyn_5qi();
  desc.five_qi                                     = 8;
  qos_flow.ngran_alloc_retention_prio.pre_emption_cap.value = pre_emption_cap_opts::shall_not_trigger_pre_emption;
  qos_flow.ngran_alloc_retention_prio.pre_emption_vulnerability.value =
      pre_emption_vulnerability_opts::not_pre_emptable;
  drb_info.snssai.sst.from_string("01");
  drb_info.snssai.sd.from_string("0027db");
  drb.rlc_mode.value                  = rlc_mode_opts::rlc_am;
  drb.ie_exts_present                 = true;
  drb.ie_exts.dl_pdcp_sn_len_present  = true;
  drb.ie_exts.dl_pdcp_sn_len          = pdcp_sn_len_opts::twelve_bits;
  drb.ul_up_tnl_info_to_be_setup_list = generate_ul_up_tnl_info_to_be_setup_list_l();
  return drb;
}

f1ap_message
ocudu::test_helpers::generate_ue_context_modification_request(gnb_du_ue_f1ap_id_t                    du_ue_id,
                                                              gnb_cu_ue_f1ap_id_t                    cu_ue_id,
                                                              const std::initializer_list<drb_id_t>& drbs_to_setup,
                                                              const std::initializer_list<drb_id_t>& drbs_to_mod,
                                                              const std::initializer_list<drb_id_t>& drbs_to_rem,
                                                              byte_buffer                            rrc_container)
{
  using namespace asn1::f1ap;
  f1ap_message msg;

  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_MOD);
  ue_context_mod_request_s& dl_msg = msg.pdu.init_msg().value.ue_context_mod_request();
  dl_msg->gnb_cu_ue_f1ap_id        = to_underlying(cu_ue_id);
  dl_msg->gnb_du_ue_f1ap_id        = to_underlying(du_ue_id);

  dl_msg->drbs_to_be_setup_mod_list_present = drbs_to_setup.size() > 0;
  dl_msg->drbs_to_be_setup_mod_list.resize(drbs_to_setup.size());
  unsigned count = 0;
  for (drb_id_t drbid : drbs_to_setup) {
    dl_msg->drbs_to_be_setup_mod_list[count].load_info_obj(ASN1_F1AP_ID_DRBS_SETUP_MOD_ITEM);
    dl_msg->drbs_to_be_setup_mod_list[count]->drbs_to_be_setup_mod_item() = generate_drb_am_mod_item(drbid);
    ++count;
  }

  dl_msg->drbs_to_be_released_list_present = drbs_to_rem.size() > 0;
  dl_msg->drbs_to_be_released_list.resize(drbs_to_rem.size());
  count = 0;
  for (drb_id_t drbid : drbs_to_rem) {
    dl_msg->drbs_to_be_released_list[count].load_info_obj(ASN1_F1AP_ID_DRBS_TO_BE_RELEASED_ITEM);
    dl_msg->drbs_to_be_released_list[count]->drbs_to_be_released_item().drb_id = to_underlying(drbid);
    ++count;
  }

  dl_msg->drbs_to_be_modified_list_present = drbs_to_mod.size() > 0;
  dl_msg->drbs_to_be_modified_list.resize(drbs_to_mod.size());
  count = 0;
  for (drb_id_t drbid : drbs_to_mod) {
    dl_msg->drbs_to_be_modified_list[count].load_info_obj(ASN1_F1AP_ID_DRBS_TO_BE_MODIFIED_ITEM);
    dl_msg->drbs_to_be_modified_list[count]->drbs_to_be_modified_item() = generate_to_modify_drb_am_mod_item(drbid);
    ++count;
  }

  dl_msg->rrc_container_present = not rrc_container.empty();
  dl_msg->rrc_container         = rrc_container.copy();

  return msg;
}

f1ap_message
ocudu::test_helpers::generate_ue_context_modification_response(gnb_du_ue_f1ap_id_t          du_ue_id,
                                                               gnb_cu_ue_f1ap_id_t          cu_ue_id,
                                                               rnti_t                       crnti,
                                                               const std::vector<drb_id_t>& drbs_setup_mod_list,
                                                               const std::vector<drb_id_t>& drbs_modified_list,
                                                               byte_buffer                  cell_group_config)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome();
  pdu.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_MOD);

  auto& ue_context_mod_resp                        = pdu.pdu.successful_outcome().value.ue_context_mod_resp();
  ue_context_mod_resp->gnb_cu_ue_f1ap_id           = (unsigned)cu_ue_id;
  ue_context_mod_resp->gnb_du_ue_f1ap_id           = (unsigned)du_ue_id;
  ue_context_mod_resp->c_rnti_present              = true;
  ue_context_mod_resp->c_rnti                      = (unsigned)crnti;
  ue_context_mod_resp->drbs_setup_mod_list_present = true;

  ue_context_mod_resp->drbs_setup_mod_list_present = !drbs_setup_mod_list.empty();
  for (const auto& drb : drbs_setup_mod_list) {
    ue_context_mod_resp->drbs_setup_mod_list.push_back({});
    ue_context_mod_resp->drbs_setup_mod_list.back().load_info_obj(ASN1_F1AP_ID_DRBS_SETUP_MOD_ITEM);
    ue_context_mod_resp->drbs_setup_mod_list.back().value().drbs_setup_mod_item().drb_id = to_underlying(drb);
  }

  ue_context_mod_resp->drbs_modified_list_present = !drbs_modified_list.empty();
  for (const auto& drb : drbs_modified_list) {
    ue_context_mod_resp->drbs_modified_list.push_back({});
    ue_context_mod_resp->drbs_modified_list.back().load_info_obj(ASN1_F1AP_ID_DRBS_MODIFIED_ITEM);
    ue_context_mod_resp->drbs_modified_list.back().value().drbs_modified_item().drb_id = to_underlying(drb);
  }

  if (!cell_group_config.empty()) {
    ue_context_mod_resp->du_to_cu_rrc_info_present        = true;
    ue_context_mod_resp->du_to_cu_rrc_info.cell_group_cfg = cell_group_config.copy();
  }

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_ue_context_modification_failure(gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                           gnb_du_ue_f1ap_id_t du_ue_id)
{
  f1ap_message ue_context_modification_failure = {};

  ue_context_modification_failure.pdu.set_unsuccessful_outcome();
  ue_context_modification_failure.pdu.unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_UE_CONTEXT_MOD);

  auto& ue_context_mod_fail = ue_context_modification_failure.pdu.unsuccessful_outcome().value.ue_context_mod_fail();
  ue_context_mod_fail->gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  ue_context_mod_fail->gnb_du_ue_f1ap_id = (unsigned)du_ue_id;
  ue_context_mod_fail->cause.set_radio_network();
  ue_context_mod_fail->cause.radio_network() =
      cause_radio_network_opts::options::unknown_or_already_allocated_gnb_cu_ue_f1ap_id;

  return ue_context_modification_failure;
}

f1ap_message
ocudu::test_helpers::generate_init_ul_rrc_message_transfer_without_du_to_cu_container(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                                      rnti_t              crnti,
                                                                                      plmn_identity       plmn_id)
{
  f1ap_message init_ul_rrc_msg = generate_init_ul_rrc_message_transfer(du_ue_id, crnti, plmn_id);
  init_ul_rrc_msg.pdu.init_msg().value.init_ul_rrc_msg_transfer()->du_to_cu_rrc_container_present = false;

  return init_ul_rrc_msg;
}

f1ap_message ocudu::test_helpers::generate_init_ul_rrc_message_transfer(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                        rnti_t              crnti,
                                                                        plmn_identity       plmn_id,
                                                                        byte_buffer         cell_group_cfg,
                                                                        byte_buffer         rrc_container)
{
  f1ap_message init_ul_rrc_msg;

  init_ul_rrc_msg.pdu.set_init_msg();
  init_ul_rrc_msg.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_INIT_UL_RRC_MSG_TRANSFER);

  init_ul_rrc_msg_transfer_s& init_ul_rrc = init_ul_rrc_msg.pdu.init_msg().value.init_ul_rrc_msg_transfer();
  init_ul_rrc->gnb_du_ue_f1ap_id          = (unsigned)du_ue_id;

  nr_cell_identity nci = nr_cell_identity::create(gnb_id_t{411, 22}, 0).value();
  init_ul_rrc->nr_cgi.nr_cell_id.from_number(nci.value());
  init_ul_rrc->nr_cgi.plmn_id = plmn_id.to_bytes();
  init_ul_rrc->c_rnti         = to_underlying(crnti);

  init_ul_rrc->sul_access_ind_present = true;
  init_ul_rrc->sul_access_ind.value   = sul_access_ind_opts::options::true_value;

  if (rrc_container.empty()) {
    init_ul_rrc->rrc_container.from_string("1dec89d05766");
  } else {
    init_ul_rrc->rrc_container = std::move(rrc_container);
  }

  init_ul_rrc->du_to_cu_rrc_container_present = true;
  if (cell_group_cfg.empty()) {
    init_ul_rrc->du_to_cu_rrc_container.from_string(
        "5c00b001117aec701061e0007c20408d07810020a2090480ca8000f800000000008370842000088165000048200002069a06aa49880002"
        "00204000400d008013b64b1814400e468acf120000096070820f177e060870000000e25038000040bde802000400000000028201950300"
        "c400");
  } else {
    init_ul_rrc->du_to_cu_rrc_container = std::move(cell_group_cfg);
  }

  return init_ul_rrc_msg;
}

f1ap_message ocudu::test_helpers::generate_dl_rrc_message_transfer(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                   gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                   srb_id_t            srb_id,
                                                                   byte_buffer         rrc_container)
{
  f1ap_message msg;

  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_DL_RRC_MSG_TRANSFER);
  auto& dlmsg = *msg.pdu.init_msg().value.dl_rrc_msg_transfer();

  dlmsg.gnb_du_ue_f1ap_id = (unsigned)du_ue_id;
  dlmsg.gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  dlmsg.srb_id            = (uint8_t)srb_id;
  dlmsg.rrc_container     = std::move(rrc_container);

  return msg;
}

f1ap_message ocudu::test_helpers::generate_ul_rrc_message_transfer(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                   gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                   srb_id_t            srb_id,
                                                                   byte_buffer         rrc_container)
{
  f1ap_message msg;

  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_UL_RRC_MSG_TRANSFER);
  ul_rrc_msg_transfer_s& ulmsg = msg.pdu.init_msg().value.ul_rrc_msg_transfer();

  ulmsg->gnb_du_ue_f1ap_id = (unsigned)du_ue_id;
  ulmsg->gnb_cu_ue_f1ap_id = (unsigned)cu_ue_id;
  ulmsg->srb_id            = (uint8_t)srb_id;
  ulmsg->rrc_container     = std::move(rrc_container);

  return msg;
}

f1ap_message ocudu::test_helpers::generate_ul_rrc_message_transfer(gnb_du_ue_f1ap_id_t    du_ue_id,
                                                                   gnb_cu_ue_f1ap_id_t    cu_ue_id,
                                                                   srb_id_t               srb_id,
                                                                   uint32_t               pdcp_sn,
                                                                   byte_buffer            ul_dcch_msg,
                                                                   std::array<uint8_t, 4> mac)
{
  // > Prepend PDCP header and append MAC.
  std::array<uint8_t, 2> pdcp_header{static_cast<uint8_t>((pdcp_sn >> 8U) & 0x0fU),
                                     static_cast<uint8_t>(pdcp_sn & 0xffU)};
  report_fatal_error_if_not(ul_dcch_msg.prepend(pdcp_header), "bad alloc");

  // > Append MAC
  report_fatal_error_if_not(ul_dcch_msg.append(mac), "bad alloc");

  return generate_ul_rrc_message_transfer(du_ue_id, cu_ue_id, srb_id, std::move(ul_dcch_msg));
}

byte_buffer ocudu::test_helpers::create_dl_dcch_rrc_container(uint32_t                              pdcp_sn,
                                                              const std::initializer_list<uint8_t>& dl_dcch_msg)
{
  return create_dl_dcch_rrc_container(pdcp_sn, byte_buffer::create(dl_dcch_msg).value());
}

byte_buffer ocudu::test_helpers::create_dl_dcch_rrc_container(uint32_t pdcp_sn, const byte_buffer& dl_dcch_msg)
{
  byte_buffer container;

  // Add PDCP header.
  report_fatal_error_if_not(container.append(pdcp_sn >> 8U), "Failed to allocate");
  report_fatal_error_if_not(container.append(pdcp_sn & 0xfU), "Failed to allocate");

  // Add payload.
  report_fatal_error_if_not(container.append(dl_dcch_msg), "Failed to allocate");

  // Add dummy MAC.
  report_fatal_error_if_not(container.append({0x0, 0x0, 0x0, 0x0}), "Failed to allocate");

  return container;
}

byte_buffer ocudu::test_helpers::extract_dl_dcch_msg(const byte_buffer& rrc_container)
{
  byte_buffer pdu = rrc_container.deep_copy().value();
  report_fatal_error_if_not(pdu.length() >= 7, "Invalid RRC container");

  // Remove PDCP header.
  pdu.trim_head(2);

  // Remove MAC.
  pdu.trim_tail(4);

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_f1ap_paging_message(uint64_t tmsi48)
{
  f1ap_message msg;

  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_PAGING);
  paging_s& paging = msg.pdu.init_msg().value.paging();

  // As per TS 38.304, UE ID is 5G-S-TMSI mod 1024 when eDRX is inactive.
  paging->ue_id_idx_value.set_idx_len10().from_number(tmsi48 % 1024);
  paging->paging_id.set_cn_ue_paging_id().set_five_g_s_tmsi().from_number(tmsi48);

  return msg;
}

f1ap_message ocudu::test_helpers::generate_trp_information_request()
{
  f1ap_message msg;

  msg.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_TRP_INFO_EXCHANGE);
  auto& req = msg.pdu.init_msg().value.trp_info_request();

  req->transaction_id = 0;

  std::vector<asn1::f1ap::trp_info_type_item_opts::options> trp_info_type_list = {
      asn1::f1ap::trp_info_type_item_opts::options::nr_pci,
      asn1::f1ap::trp_info_type_item_opts::options::ng_ran_cgi,
      asn1::f1ap::trp_info_type_item_opts::options::arfcn};

  for (const auto& trp_info_type_item : trp_info_type_list) {
    asn1::protocol_ie_single_container_s<asn1::f1ap::trp_info_type_item_trp_req_o> trp_info_type_item_container;
    trp_info_type_item_container->trp_info_type_item() = trp_info_type_item;

    req->trp_info_type_list_trp_req.push_back(trp_info_type_item_container);
  }

  return msg;
}

f1ap_message ocudu::test_helpers::generate_trp_information_response(const std::vector<trp_id_t>& trp_ids)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome();
  pdu.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_TRP_INFO_EXCHANGE);

  auto& trp_info_resp           = pdu.pdu.successful_outcome().value.trp_info_resp();
  trp_info_resp->transaction_id = 0;

  for (const auto& trp_id : trp_ids) {
    unsigned pci         = 1;
    unsigned nr_cgi_uint = 6576;

    asn1::protocol_ie_single_container_s<asn1::f1ap::trp_info_item_trp_resp_o> trp_resp_item_container;
    asn1::f1ap::trp_info_item_s& trp_info_item = trp_resp_item_container->trp_info_item();
    trp_info_item.trp_info.trp_id              = to_underlying(trp_id);

    // Add PCI.
    asn1::f1ap::trp_info_type_resp_item_c trp_info_type_resp_item;
    trp_info_type_resp_item.set_pci_nr() = pci;
    trp_info_item.trp_info.trp_info_type_resp_list.push_back(trp_info_type_resp_item);

    // Add NR CGI.
    asn1::f1ap::trp_info_type_resp_item_c trp_info_type_resp_item_2;
    asn1::f1ap::nr_cgi_s                  asn1_cgi;
    asn1_cgi.plmn_id.from_string("00f110");
    asn1_cgi.nr_cell_id.from_number(nr_cgi_uint);
    trp_info_type_resp_item_2.set_ng_ran_cgi() = asn1_cgi;
    trp_info_item.trp_info.trp_info_type_resp_list.push_back(trp_info_type_resp_item_2);

    // Add ARFCN.
    asn1::f1ap::trp_info_type_resp_item_c trp_info_type_resp_item_3;
    trp_info_type_resp_item_3.set_nr_arfcn() = 652054;
    trp_info_item.trp_info.trp_info_type_resp_list.push_back(trp_info_type_resp_item_3);

    trp_info_resp->trp_info_list_trp_resp.push_back(trp_resp_item_container);

    ++pci;
    ++nr_cgi_uint;
  }

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_trp_information_failure()
{
  f1ap_message pdu = {};

  pdu.pdu.set_unsuccessful_outcome();
  pdu.pdu.unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_TRP_INFO_EXCHANGE);

  auto& trp_info_fail                      = pdu.pdu.unsuccessful_outcome().value.trp_info_fail();
  trp_info_fail->transaction_id            = 0;
  trp_info_fail->cause.set_radio_network() = cause_radio_network_e::unspecified;

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_positioning_information_request(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                           gnb_cu_ue_f1ap_id_t cu_ue_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_init_msg();
  pdu.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_POSITIONING_INFO_EXCHANGE);

  auto& pos_info_req                                             = pdu.pdu.init_msg().value.positioning_info_request();
  pos_info_req->gnb_cu_ue_f1ap_id                                = to_underlying(cu_ue_id);
  pos_info_req->gnb_du_ue_f1ap_id                                = to_underlying(du_ue_id);
  pos_info_req->requested_srs_tx_characteristics_present         = true;
  pos_info_req->requested_srs_tx_characteristics.nof_txs_present = true;
  pos_info_req->requested_srs_tx_characteristics.nof_txs         = 0;
  pos_info_req->requested_srs_tx_characteristics.res_type =
      asn1::f1ap::requested_srs_tx_characteristics_s::res_type_opts::options::periodic;
  pos_info_req->requested_srs_tx_characteristics.bw_srs.set_fr1() = asn1::f1ap::fr1_bw_opts::options::bw100;

  return pdu;
}

static inline asn1::f1ap::srs_configuration_s
generate_srs_configuration(subcarrier_spacing scs = subcarrier_spacing::kHz15, unsigned offset = 0U)
{
  asn1::f1ap::srs_configuration_s srs_configuration;

  asn1::f1ap::srs_carrier_list_item_s srs_carrier_item;
  srs_carrier_item.point_a = 632016;

  ocudu_assert(scs <= subcarrier_spacing::kHz30 or offset == 0U,
               "Only SCS 15 kHz and 30kHz SCSs are currently supported in this test");

  // Fill UL CH BW per SCS list.
  scs_specific_carrier_s scs_specific_carrier;
  scs_specific_carrier.offset_to_carrier = 0;
  scs_specific_carrier.subcarrier_spacing =
      scs == ocudu::subcarrier_spacing::kHz15
          ? asn1::f1ap::scs_specific_carrier_s::subcarrier_spacing_opts::options::khz15
          : asn1::f1ap::scs_specific_carrier_s::subcarrier_spacing_opts::options::khz30;
  scs_specific_carrier.carrier_bw = 52;

  srs_carrier_item.ul_ch_bw_per_scs_list.push_back(scs_specific_carrier);

  // Fill Active UL BWP.
  srs_carrier_item.active_ul_bwp.location_and_bw = 13750;
  srs_carrier_item.active_ul_bwp.subcarrier_spacing =
      asn1::f1ap::active_ul_bwp_s::subcarrier_spacing_opts::options::khz15;
  srs_carrier_item.active_ul_bwp.cp                         = asn1::f1ap::active_ul_bwp_s::cp_opts::options::normal;
  srs_carrier_item.active_ul_bwp.tx_direct_current_location = 3300;

  asn1::f1ap::srs_res_s srs_res;
  srs_res.srs_res_id       = 0;
  srs_res.nrof_srs_ports   = asn1::f1ap::srs_res_s::nrof_srs_ports_opts::options::port1;
  srs_res.tx_comb.set_n4() = asn1::f1ap::tx_comb_c::n4_s_{
      .comb_offset_n4  = 0,
      .cyclic_shift_n4 = 0,
  };
  srs_res.start_position       = 0;
  srs_res.nrof_symbols         = asn1::f1ap::srs_res_s::nrof_symbols_opts::options::n1;
  srs_res.repeat_factor        = asn1::f1ap::srs_res_s::repeat_factor_opts::options::n1;
  srs_res.freq_domain_position = 0;
  // Kept clear of the common PUCCH resources at the edges of the 52-CRB BWP configured above (CRBs [0, 4) and
  // [48, 52)), matching the same (c_srs, freq_domain_shift) the DU's own SRS resource manager would auto-compute for
  // this cell config; otherwise this (neighbor-cell) SRS-to-measure resource collides with the local common PUCCH.
  srs_res.freq_domain_shift     = 6;
  srs_res.c_srs                 = 11;
  srs_res.b_srs                 = 0;
  srs_res.b_hop                 = 0;
  srs_res.group_or_seq_hop      = asn1::f1ap::srs_res_s::group_or_seq_hop_opts::options::neither;
  auto& periodic_res_type       = srs_res.res_type.set_periodic();
  periodic_res_type.periodicity = asn1::f1ap::res_type_periodic_s::periodicity_opts::options::slot80;
  periodic_res_type.offset      = offset;
  srs_carrier_item.active_ul_bwp.srs_cfg.srs_res_list.push_back(srs_res);

  asn1::f1ap::srs_res_set_s srs_res_set;
  srs_res_set.srs_res_set_id = 0U;
  srs_res_set.srs_res_id_list.push_back(srs_res.srs_res_id);
  auto& periodic_res_set_type        = srs_res_set.res_set_type.set_periodic();
  periodic_res_set_type.periodic_set = asn1::f1ap::res_set_type_periodic_s::periodic_set_opts::options::true_value;

  srs_carrier_item.active_ul_bwp.srs_cfg.srs_res_set_list.push_back(srs_res_set);

  srs_carrier_item.pci_present = false;

  srs_configuration.srs_carrier_list.push_back(srs_carrier_item);

  return srs_configuration;
}

f1ap_message ocudu::test_helpers::generate_positioning_information_response(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                            gnb_cu_ue_f1ap_id_t cu_ue_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome();
  pdu.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_POSITIONING_INFO_EXCHANGE);

  auto& pos_info_resp              = pdu.pdu.successful_outcome().value.positioning_info_resp();
  pos_info_resp->gnb_cu_ue_f1ap_id = to_underlying(cu_ue_id);
  pos_info_resp->gnb_du_ue_f1ap_id = to_underlying(du_ue_id);

  pos_info_resp->srs_cfg_present = true;
  pos_info_resp->srs_cfg         = generate_srs_configuration();

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_positioning_information_failure(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                           gnb_cu_ue_f1ap_id_t cu_ue_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_unsuccessful_outcome();
  pdu.pdu.unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_POSITIONING_INFO_EXCHANGE);

  auto& pos_info_fail                      = pdu.pdu.unsuccessful_outcome().value.positioning_info_fail();
  pos_info_fail->gnb_cu_ue_f1ap_id         = to_underlying(cu_ue_id);
  pos_info_fail->gnb_du_ue_f1ap_id         = to_underlying(du_ue_id);
  pos_info_fail->cause.set_radio_network() = cause_radio_network_e::unspecified;

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_positioning_activation_response(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                           gnb_cu_ue_f1ap_id_t cu_ue_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome();
  pdu.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_POSITIONING_ACTIVATION);

  auto& pos_info_resp              = pdu.pdu.successful_outcome().value.positioning_activation_resp();
  pos_info_resp->gnb_cu_ue_f1ap_id = to_underlying(cu_ue_id);
  pos_info_resp->gnb_du_ue_f1ap_id = to_underlying(du_ue_id);

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_positioning_activation_failure(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                          gnb_cu_ue_f1ap_id_t cu_ue_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_unsuccessful_outcome();
  pdu.pdu.unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_POSITIONING_ACTIVATION);

  auto& pos_info_fail                      = pdu.pdu.unsuccessful_outcome().value.positioning_activation_fail();
  pos_info_fail->gnb_cu_ue_f1ap_id         = to_underlying(cu_ue_id);
  pos_info_fail->gnb_du_ue_f1ap_id         = to_underlying(du_ue_id);
  pos_info_fail->cause.set_radio_network() = cause_radio_network_e::unspecified;

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_e_cid_measurement_initiation_request(
    gnb_cu_ue_f1ap_id_t                                                        cu_ue_id,
    gnb_du_ue_f1ap_id_t                                                        du_ue_id,
    const std::vector<asn1::f1ap::e_c_id_meas_quantities_value_opts::options>& quantities,
    asn1::f1ap::e_c_id_report_characteristics_opts::options                    report_characteristics,
    uint16_t                                                                   lmf_ue_meas_id,
    uint16_t                                                                   ran_ue_meas_id,
    std::optional<asn1::f1ap::pos_meas_periodicity_nr_ao_a_opts::options>      meas_periodicity_nr_aoa,
    std::optional<asn1::f1ap::meas_periodicity_opts::options>                  e_cid_meas_periodicity)
{
  f1ap_message pdu = {};

  pdu.pdu.set_init_msg();
  pdu.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_INITIATION);

  auto& req                          = pdu.pdu.init_msg().value.e_c_id_meas_initiation_request();
  req->gnb_cu_ue_f1ap_id             = to_underlying(cu_ue_id);
  req->gnb_du_ue_f1ap_id             = to_underlying(du_ue_id);
  req->lmf_ue_meas_id                = lmf_ue_meas_id;
  req->ran_ue_meas_id                = ran_ue_meas_id;
  req->e_c_id_report_characteristics = report_characteristics;

  req->e_c_id_meas_quantities.resize(quantities.size());
  for (unsigned i = 0, e = quantities.size(); i != e; ++i) {
    req->e_c_id_meas_quantities[i].load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_QUANTITIES_ITEM);
    req->e_c_id_meas_quantities[i]->e_c_id_meas_quantities_item().e_c_id_meas_quantities_value = quantities[i];
  }

  if (meas_periodicity_nr_aoa.has_value()) {
    req->pos_meas_periodicity_nr_ao_a_present = true;
    req->pos_meas_periodicity_nr_ao_a         = meas_periodicity_nr_aoa.value();
  }
  if (e_cid_meas_periodicity.has_value()) {
    req->e_c_id_meas_periodicity_present = true;
    req->e_c_id_meas_periodicity         = e_cid_meas_periodicity.value();
  }

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_e_cid_measurement_termination_command(gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                                 gnb_du_ue_f1ap_id_t du_ue_id,
                                                                                 uint16_t            lmf_ue_meas_id,
                                                                                 uint16_t            ran_ue_meas_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_init_msg();
  pdu.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_TERMINATION);

  auto& cmd              = pdu.pdu.init_msg().value.e_c_id_meas_termination_cmd();
  cmd->gnb_cu_ue_f1ap_id = to_underlying(cu_ue_id);
  cmd->gnb_du_ue_f1ap_id = to_underlying(du_ue_id);
  cmd->lmf_ue_meas_id    = lmf_ue_meas_id;
  cmd->ran_ue_meas_id    = ran_ue_meas_id;

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_positioning_measurement_request(
    const std::vector<trp_id_t>&                                trp_ids,
    lmf_meas_id_t                                               lmf_meas_id,
    ran_meas_id_t                                               ran_meas_id,
    const std::vector<asn1::f1ap::pos_meas_type_opts::options>& pos_meas_type_list,
    subcarrier_spacing                                          scs,
    unsigned                                                    srs_offset,
    std::optional<uint8_t>                                      resp_time_ten_msec)
{
  f1ap_message pdu = {};

  pdu.pdu.set_init_msg();
  pdu.pdu.init_msg().load_info_obj(ASN1_F1AP_ID_POSITIONING_MEAS_EXCHANGE);

  auto& pos_meas_req           = pdu.pdu.init_msg().value.positioning_meas_request();
  pos_meas_req->transaction_id = 1;
  pos_meas_req->lmf_meas_id    = to_underlying(lmf_meas_id);
  pos_meas_req->ran_meas_id    = to_underlying(ran_meas_id);

  for (const auto trp_id : trp_ids) {
    pos_meas_req->trp_meas_request_list.push_back(asn1::f1ap::trp_meas_request_item_s{.trp_id = to_underlying(trp_id)});
  }

  pos_meas_req->pos_report_characteristics = asn1::f1ap::pos_report_characteristics_opts::options::ondemand;

  for (const auto& pos_meas_type : pos_meas_type_list) {
    asn1::f1ap::pos_meas_quantities_item_s meas_quantities_item;
    meas_quantities_item.pos_meas_type = pos_meas_type;
    if (pos_meas_type == asn1::f1ap::pos_meas_type_opts::options::ul_rtoa) {
      meas_quantities_item.timing_report_granularity_factor_present = true;
      meas_quantities_item.timing_report_granularity_factor         = 1;
    }

    pos_meas_req->pos_meas_quantities.push_back(meas_quantities_item);
  }

  if (resp_time_ten_msec.has_value()) {
    pos_meas_req->resp_time_present   = true;
    pos_meas_req->resp_time.time      = resp_time_ten_msec.value();
    pos_meas_req->resp_time.time_unit = asn1::f1ap::resp_time_s::time_unit_opts::ten_milliseconds;
  }

  pos_meas_req->srs_cfg_present = true;
  pos_meas_req->srs_cfg         = generate_srs_configuration(scs, srs_offset);

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_positioning_measurement_response(lmf_meas_id_t                lmf_meas_id,
                                                                            ran_meas_id_t                ran_meas_id,
                                                                            const std::vector<trp_id_t>& trp_ids,
                                                                            unsigned                     transaction_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome();
  pdu.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_POSITIONING_MEAS_EXCHANGE);

  auto& pos_meas_resp           = pdu.pdu.successful_outcome().value.positioning_meas_resp();
  pos_meas_resp->transaction_id = transaction_id;
  pos_meas_resp->lmf_meas_id    = to_underlying(lmf_meas_id);
  pos_meas_resp->ran_meas_id    = to_underlying(ran_meas_id);

  for (const auto& trp_id : trp_ids) {
    // Create positioning measurement result item for each TRP ID.
    asn1::f1ap::pos_meas_result_list_item_s meas_resp_item;
    meas_resp_item.trp_id = to_underlying(trp_id);

    // Add positioning measurement result item.
    asn1::f1ap::pos_meas_result_item_s pos_meas_result_item;

    // > Add UL RTOA measurement result.
    asn1::f1ap::ul_rtoa_meas_s& ul_rtoa = pos_meas_result_item.measured_results_value.set_ul_rtoa();
    ul_rtoa.ul_rtoa_meas_item.set_k0()  = 985471;

    // Add time stamp.
    pos_meas_result_item.time_stamp.sys_frame_num         = 25;
    pos_meas_result_item.time_stamp.slot_idx.set_scs_30() = 0;

    // Add measurement quality.
    pos_meas_result_item.meas_quality_present = true;
    asn1::f1ap::timing_meas_quality_s& timing_meas_qualitiy =
        pos_meas_result_item.meas_quality.tr_pmeas_quality_item.set_timing_meas_quality();
    timing_meas_qualitiy.meas_quality = 0;
    timing_meas_qualitiy.resolution   = asn1::f1ap::timing_meas_quality_s::resolution_opts::options::m0dot1;

    meas_resp_item.pos_meas_result.push_back(pos_meas_result_item);

    pos_meas_resp->pos_meas_result_list_present = true;
    pos_meas_resp->pos_meas_result_list.push_back(meas_resp_item);
  }

  return pdu;
}

f1ap_message
ocudu::test_helpers::generate_positioning_measurement_response_with_aoa(lmf_meas_id_t                lmf_meas_id,
                                                                        ran_meas_id_t                ran_meas_id,
                                                                        const std::vector<trp_id_t>& trp_ids,
                                                                        uint16_t                     azimuth_ao_a,
                                                                        unsigned                     transaction_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome();
  pdu.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_POSITIONING_MEAS_EXCHANGE);

  auto& pos_meas_resp           = pdu.pdu.successful_outcome().value.positioning_meas_resp();
  pos_meas_resp->transaction_id = transaction_id;
  pos_meas_resp->lmf_meas_id    = to_underlying(lmf_meas_id);
  pos_meas_resp->ran_meas_id    = to_underlying(ran_meas_id);

  for (const auto& trp_id : trp_ids) {
    // Create positioning measurement result item for each TRP ID.
    asn1::f1ap::pos_meas_result_list_item_s meas_resp_item;
    meas_resp_item.trp_id = to_underlying(trp_id);

    // Add positioning measurement result item.
    asn1::f1ap::pos_meas_result_item_s pos_meas_result_item;

    // > Add UL Angle of Arrival measurement result.
    asn1::f1ap::ul_ao_a_s& ul_aoa = pos_meas_result_item.measured_results_value.set_ul_angle_of_arrival();
    ul_aoa.azimuth_ao_a           = azimuth_ao_a;

    // Add time stamp.
    pos_meas_result_item.time_stamp.sys_frame_num         = 25;
    pos_meas_result_item.time_stamp.slot_idx.set_scs_30() = 0;

    meas_resp_item.pos_meas_result.push_back(pos_meas_result_item);

    pos_meas_resp->pos_meas_result_list_present = true;
    pos_meas_resp->pos_meas_result_list.push_back(meas_resp_item);
  }

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_positioning_measurement_response_with_zoa(
    lmf_meas_id_t                           lmf_meas_id,
    ran_meas_id_t                           ran_meas_id,
    const std::vector<trp_id_t>&            trp_ids,
    uint16_t                                zenith_ao_a,
    std::optional<lcs_to_gcs_translation_t> lcs_to_gcs_translation,
    unsigned                                transaction_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome();
  pdu.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_POSITIONING_MEAS_EXCHANGE);

  auto& pos_meas_resp           = pdu.pdu.successful_outcome().value.positioning_meas_resp();
  pos_meas_resp->transaction_id = transaction_id;
  pos_meas_resp->lmf_meas_id    = to_underlying(lmf_meas_id);
  pos_meas_resp->ran_meas_id    = to_underlying(ran_meas_id);

  for (const auto& trp_id : trp_ids) {
    asn1::f1ap::pos_meas_result_list_item_s meas_resp_item;
    meas_resp_item.trp_id = to_underlying(trp_id);

    asn1::f1ap::pos_meas_result_item_s pos_meas_result_item;

    // > Add Zenith Angle of Arrival Information measurement result.
    auto& ext = pos_meas_result_item.measured_results_value.set_choice_ext();
    ext->set(asn1::f1ap::measured_results_value_ext_ies_o::value_c::types_opts::zo_a_info);
    asn1::f1ap::zo_a_info_s& zoa = ext->zo_a_info();
    zoa.zenith_ao_a              = zenith_ao_a;
    if (lcs_to_gcs_translation.has_value()) {
      zoa.lcs_to_gcs_translation_present = true;
      zoa.lcs_to_gcs_translation.alpha   = lcs_to_gcs_translation->alpha;
      zoa.lcs_to_gcs_translation.beta    = lcs_to_gcs_translation->beta;
      zoa.lcs_to_gcs_translation.gamma   = lcs_to_gcs_translation->gamma;
    }

    // Add time stamp.
    pos_meas_result_item.time_stamp.sys_frame_num         = 25;
    pos_meas_result_item.time_stamp.slot_idx.set_scs_30() = 0;

    meas_resp_item.pos_meas_result.push_back(pos_meas_result_item);

    pos_meas_resp->pos_meas_result_list_present = true;
    pos_meas_resp->pos_meas_result_list.push_back(meas_resp_item);
  }

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_positioning_measurement_failure(lmf_meas_id_t lmf_meas_id,
                                                                           ran_meas_id_t ran_meas_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_unsuccessful_outcome();
  pdu.pdu.unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_POSITIONING_MEAS_EXCHANGE);

  auto& pos_meas_fail                      = pdu.pdu.unsuccessful_outcome().value.positioning_meas_fail();
  pos_meas_fail->transaction_id            = 1;
  pos_meas_fail->lmf_meas_id               = to_underlying(lmf_meas_id);
  pos_meas_fail->ran_meas_id               = to_underlying(ran_meas_id);
  pos_meas_fail->cause.set_radio_network() = cause_radio_network_e::unspecified;

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_f1ap_write_replace_warning_request(unsigned transaction_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_init_msg().load_info_obj(ASN1_F1AP_ID_WRITE_REPLACE_WARNING);
  auto& req = pdu.pdu.init_msg().value.write_replace_warning_request();

  req->transaction_id        = transaction_id;
  req->pws_sys_info.sib_type = 6;
  req->pws_sys_info.sib_msg.from_string("deadbeef");
  req->repeat_period           = 4;
  req->numof_broadcast_request = 2;

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_f1ap_write_replace_warning_response(
    unsigned                                transaction_id,
    const std::vector<nr_cell_global_id_t>& completed_cells)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome().load_info_obj(ASN1_F1AP_ID_WRITE_REPLACE_WARNING);
  auto& resp = pdu.pdu.successful_outcome().value.write_replace_warning_resp();

  resp->transaction_id = transaction_id;

  if (not completed_cells.empty()) {
    resp->cells_broadcast_completed_list_present = true;
    for (const auto& cgi : completed_cells) {
      asn1::protocol_ie_single_container_s<cells_broadcast_completed_list_item_ies_o> item;
      item->cells_broadcast_completed_item().nr_cgi = cgi_to_asn1(cgi);
      resp->cells_broadcast_completed_list.push_back(item);
    }
  }

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_e_cid_measurement_initiation_response(gnb_du_ue_f1ap_id_t     du_ue_id,
                                                                                 gnb_cu_ue_f1ap_id_t     cu_ue_id,
                                                                                 uint16_t                lmf_ue_meas_id,
                                                                                 uint16_t                ran_ue_meas_id,
                                                                                 std::optional<uint16_t> azimuth_aoa,
                                                                                 std::optional<uint16_t> zenith_aoa)
{
  f1ap_message pdu = {};

  pdu.pdu.set_successful_outcome();
  pdu.pdu.successful_outcome().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_INITIATION);

  auto& resp              = pdu.pdu.successful_outcome().value.e_c_id_meas_initiation_resp();
  resp->gnb_cu_ue_f1ap_id = to_underlying(cu_ue_id);
  resp->gnb_du_ue_f1ap_id = to_underlying(du_ue_id);
  resp->lmf_ue_meas_id    = lmf_ue_meas_id;
  resp->ran_ue_meas_id    = ran_ue_meas_id;

  if (azimuth_aoa.has_value()) {
    resp->e_c_id_meas_result_present = true;

    asn1::f1ap::e_c_id_measured_results_item_s item;
    auto&                                      aoa = item.e_c_id_measured_results_value.set_value_angleof_arrival_nr();
    aoa.azimuth_ao_a                               = azimuth_aoa.value();
    if (zenith_aoa.has_value()) {
      aoa.zenith_ao_a_present = true;
      aoa.zenith_ao_a         = zenith_aoa.value();
    }

    resp->e_c_id_meas_result.measured_results_list.push_back(item);
  }

  return pdu;
}

f1ap_message ocudu::test_helpers::generate_e_cid_measurement_initiation_failure(gnb_du_ue_f1ap_id_t du_ue_id,
                                                                                gnb_cu_ue_f1ap_id_t cu_ue_id,
                                                                                uint16_t            lmf_ue_meas_id,
                                                                                uint16_t            ran_ue_meas_id)
{
  f1ap_message pdu = {};

  pdu.pdu.set_unsuccessful_outcome();
  pdu.pdu.unsuccessful_outcome().load_info_obj(ASN1_F1AP_ID_E_C_ID_MEAS_INITIATION);

  auto& fail                      = pdu.pdu.unsuccessful_outcome().value.e_c_id_meas_initiation_fail();
  fail->gnb_cu_ue_f1ap_id         = to_underlying(cu_ue_id);
  fail->gnb_du_ue_f1ap_id         = to_underlying(du_ue_id);
  fail->lmf_ue_meas_id            = lmf_ue_meas_id;
  fail->ran_ue_meas_id            = ran_ue_meas_id;
  fail->cause.set_radio_network() = asn1::f1ap::cause_radio_network_opts::unspecified;

  return pdu;
}
