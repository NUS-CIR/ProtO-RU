// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/xnap/xnap_pdu_contents.h"

using namespace asn1;
using namespace asn1::xnap;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// AccessAndMobilityIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t access_and_mob_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {205, 276, 385, 392};
  return map_enum_number(names, 4, idx, "id");
}
bool access_and_mob_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {205, 276, 385, 392};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e access_and_mob_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 205:
      return crit_e::ignore;
    case 276:
      return crit_e::ignore;
    case 385:
      return crit_e::ignore;
    case 392:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e access_and_mob_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 205:
      return presence_e::optional;
    case 276:
      return presence_e::optional;
    case 385:
      return presence_e::optional;
    case 392:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE access_and_mob_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += ra_report_present ? 1 : 0;
  nof_ies += successful_ho_report_info_present ? 1 : 0;
  nof_ies += successful_pscell_change_report_info_present ? 1 : 0;
  nof_ies += dl_lbt_fail_info_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (ra_report_present) {
    HANDLE_CODE(pack_ie(
        bref, 205, crit_e{crit_e::ignore}, [this, &bref]() { return pack_dyn_seq_of(bref, ra_report, 1, 64, true); }));
  }
  if (successful_ho_report_info_present) {
    HANDLE_CODE(pack_ie(bref, 276, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, successful_ho_report_info, 1, 64, true);
    }));
  }
  if (successful_pscell_change_report_info_present) {
    HANDLE_CODE(pack_ie(bref, 385, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, successful_pscell_change_report_info, 1, 64, true);
    }));
  }
  if (dl_lbt_fail_info_list_present) {
    HANDLE_CODE(pack_ie(bref, 392, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, dl_lbt_fail_info_list, 1, 64, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE access_and_mob_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &access_and_mob_ind_ies_o::get_crit));

    switch (id) {
      case 205: {
        ra_report_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(ra_report, bref, 1, 64, true); }));
        break;
      }
      case 276: {
        successful_ho_report_info_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(successful_ho_report_info, bref, 1, 64, true); }));
        break;
      }
      case 385: {
        successful_pscell_change_report_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(successful_pscell_change_report_info, bref, 1, 64, true);
        }));
        break;
      }
      case 392: {
        dl_lbt_fail_info_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(dl_lbt_fail_info_list, bref, 1, 64, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void access_and_mob_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (ra_report_present) {
    j.start_obj();
    j.write_int("id", 205);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ra_report) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (successful_ho_report_info_present) {
    j.start_obj();
    j.write_int("id", 276);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : successful_ho_report_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (successful_pscell_change_report_info_present) {
    j.start_obj();
    j.write_int("id", 385);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : successful_pscell_change_report_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (dl_lbt_fail_info_list_present) {
    j.start_obj();
    j.write_int("id", 392);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : dl_lbt_fail_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// SSBsActivated-Item ::= SEQUENCE
OCUDUASN_CODE ss_bs_activ_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, ssb_idx, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ss_bs_activ_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ssb_idx, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ss_bs_activ_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ssbIndex", ssb_idx);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ActivatedNRCellsAndSSBs-Item ::= SEQUENCE
OCUDUASN_CODE activ_nr_cells_and_ss_bs_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ss_bs_activ_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.pack(bref));
  if (ss_bs_activ_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ss_bs_activ_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE activ_nr_cells_and_ss_bs_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ss_bs_activ_list_present;
  HANDLE_CODE(bref.unpack(ss_bs_activ_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.unpack(bref));
  if (ss_bs_activ_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ss_bs_activ_list, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void activ_nr_cells_and_ss_bs_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("nrCGI");
  nr_cgi.to_json(j);
  if (ss_bs_activ_list.size() > 0) {
    j.start_array("sSBsActivatedList");
    for (const auto& e1 : ss_bs_activ_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ActivatedServedCells-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t activ_served_cells_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {417};
  return map_enum_number(names, 1, idx, "id");
}
bool activ_served_cells_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 417 == id;
}
crit_e activ_served_cells_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 417) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
activ_served_cells_ext_ies_o::value_c activ_served_cells_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 417) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e activ_served_cells_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 417) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void activ_served_cells_ext_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_array("");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
}
OCUDUASN_CODE activ_served_cells_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 16384, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE activ_served_cells_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 16384, true));
  return OCUDUASN_SUCCESS;
}

const char* activ_served_cells_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"ActivatedNRCellsAndSSBsList"};
  return convert_enum_idx(names, 1, value, "activ_served_cells_ext_ies_o::value_c::types");
}

// ActivatedServedCells ::= CHOICE
void activ_served_cells_c::destroy_()
{
  switch (type_) {
    case types::nr_cells:
      c.destroy<nr_cells_l_>();
      break;
    case types::e_utra_cells:
      c.destroy<e_utra_cells_l_>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void activ_served_cells_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::nr_cells:
      c.init<nr_cells_l_>();
      break;
    case types::e_utra_cells:
      c.init<e_utra_cells_l_>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "activ_served_cells_c");
  }
}
activ_served_cells_c::activ_served_cells_c(const activ_served_cells_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::nr_cells:
      c.init(other.c.get<nr_cells_l_>());
      break;
    case types::e_utra_cells:
      c.init(other.c.get<e_utra_cells_l_>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "activ_served_cells_c");
  }
}
activ_served_cells_c& activ_served_cells_c::operator=(const activ_served_cells_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::nr_cells:
      c.set(other.c.get<nr_cells_l_>());
      break;
    case types::e_utra_cells:
      c.set(other.c.get<e_utra_cells_l_>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "activ_served_cells_c");
  }

  return *this;
}
activ_served_cells_c::nr_cells_l_& activ_served_cells_c::set_nr_cells()
{
  set(types::nr_cells);
  return c.get<nr_cells_l_>();
}
activ_served_cells_c::e_utra_cells_l_& activ_served_cells_c::set_e_utra_cells()
{
  set(types::e_utra_cells);
  return c.get<e_utra_cells_l_>();
}
protocol_ie_single_container_s<activ_served_cells_ext_ies_o>& activ_served_cells_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>();
}
void activ_served_cells_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::nr_cells:
      j.start_array("nr-cells");
      for (const auto& e1 : c.get<nr_cells_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e_utra_cells:
      j.start_array("e-utra-cells");
      for (const auto& e1 : c.get<e_utra_cells_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "activ_served_cells_c");
  }
  j.end_obj();
}
OCUDUASN_CODE activ_served_cells_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::nr_cells:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<nr_cells_l_>(), 1, 16384, true));
      break;
    case types::e_utra_cells:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e_utra_cells_l_>(), 1, 16384, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "activ_served_cells_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE activ_served_cells_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::nr_cells:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<nr_cells_l_>(), bref, 1, 16384, true));
      break;
    case types::e_utra_cells:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e_utra_cells_l_>(), bref, 1, 16384, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<activ_served_cells_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "activ_served_cells_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* activ_served_cells_c::types_opts::to_string() const
{
  static const char* names[] = {"nr-cells", "e-utra-cells", "choice-extension"};
  return convert_enum_idx(names, 3, value, "activ_served_cells_c::types");
}

// QoSFlowsActivityNotifyItem ::= SEQUENCE
OCUDUASN_CODE qos_flows_activity_notify_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, true, true));
  HANDLE_CODE(pdu_session_level_up_activityreport.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flows_activity_notify_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, true, true));
  HANDLE_CODE(pdu_session_level_up_activityreport.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void qos_flows_activity_notify_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qosFlowIdentifier", qos_flow_id);
  j.write_str("pduSessionLevelUPactivityreport", pdu_session_level_up_activityreport.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDUSessionResourcesActivityNotify-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_activity_notify_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_session_level_up_activityreport_present, 1));
  HANDLE_CODE(bref.pack(qos_flows_activity_notify_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (pdu_session_level_up_activityreport_present) {
    HANDLE_CODE(pdu_session_level_up_activityreport.pack(bref));
  }
  if (qos_flows_activity_notify_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qos_flows_activity_notify_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_activity_notify_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(pdu_session_level_up_activityreport_present, 1));
  bool qos_flows_activity_notify_list_present;
  HANDLE_CODE(bref.unpack(qos_flows_activity_notify_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (pdu_session_level_up_activityreport_present) {
    HANDLE_CODE(pdu_session_level_up_activityreport.unpack(bref));
  }
  if (qos_flows_activity_notify_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qos_flows_activity_notify_list, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_activity_notify_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (pdu_session_level_up_activityreport_present) {
    j.write_str("pduSessionLevelUPactivityreport", pdu_session_level_up_activityreport.to_string());
  }
  if (qos_flows_activity_notify_list.size() > 0) {
    j.start_array("qosFlowsActivityNotifyList");
    for (const auto& e1 : qos_flows_activity_notify_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ActivityNotification-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t activity_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 92, 41, 121};
  return map_enum_number(names, 5, idx, "id");
}
bool activity_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 92, 41, 121};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e activity_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 92:
      return crit_e::ignore;
    case 41:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e activity_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 92:
      return presence_e::optional;
    case 41:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE activity_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += user_plane_traffic_activity_report_present ? 1 : 0;
  nof_ies += pdu_session_res_activity_notify_list_present ? 1 : 0;
  nof_ies += ran_paging_fail_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (user_plane_traffic_activity_report_present) {
    HANDLE_CODE(pack_ie(
        bref, 92, crit_e{crit_e::ignore}, [this, &bref]() { return user_plane_traffic_activity_report.pack(bref); }));
  }
  if (pdu_session_res_activity_notify_list_present) {
    HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_activity_notify_list, 1, 256, true);
    }));
  }
  if (ran_paging_fail_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return ran_paging_fail.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE activity_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &activity_notif_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 92: {
        user_plane_traffic_activity_report_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_plane_traffic_activity_report.unpack(bref); }));
        break;
      }
      case 41: {
        pdu_session_res_activity_notify_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_activity_notify_list, bref, 1, 256, true);
        }));
        break;
      }
      case 121: {
        ran_paging_fail_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_paging_fail.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void activity_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (user_plane_traffic_activity_report_present) {
    j.start_obj();
    j.write_int("id", 92);
    j.write_str("criticality", "ignore");
    j.write_str("value", user_plane_traffic_activity_report.to_string());
    j.end_obj();
  }
  if (pdu_session_res_activity_notify_list_present) {
    j.start_obj();
    j.write_int("id", 41);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_activity_notify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ran_paging_fail_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// AvailableFastMCGRecoveryViaSRB3 ::= ENUMERATED
const char* available_fast_mcg_recovery_via_srb3_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "available_fast_mcg_recovery_via_srb3_e");
}

// BearersSubjectToCounterCheck-Item ::= SEQUENCE
OCUDUASN_CODE bearers_subject_to_counter_check_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pack_integer(bref, ul_count, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  HANDLE_CODE(pack_integer(bref, dl_count, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bearers_subject_to_counter_check_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(unpack_integer(ul_count, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  HANDLE_CODE(unpack_integer(dl_count, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void bearers_subject_to_counter_check_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("drb-ID", drb_id);
  j.write_uint("ul-count", ul_count);
  j.write_uint("dl-count", dl_count);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BoundaryNodeCellsList-Item ::= SEQUENCE
OCUDUASN_CODE boundary_node_cells_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(boundary_node_cell_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE boundary_node_cells_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(boundary_node_cell_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void boundary_node_cells_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("boundaryNodeCellInformation");
  boundary_node_cell_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CPCCancel-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t cp_c_cancel_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 7, 80};
  return map_enum_number(names, 4, idx, "id");
}
bool cp_c_cancel_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 7, 80};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cp_c_cancel_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 80:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cp_c_cancel_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    case 80:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cp_c_cancel_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += cause_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (cause_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  }
  HANDLE_CODE(
      pack_ie(bref, 80, crit_e{crit_e::reject}, [this, &bref]() { return target_s_ng_ran_node_id.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cp_c_cancel_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cp_c_cancel_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        cause_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 80: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_s_ng_ran_node_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void cp_c_cancel_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (cause_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cause.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 80);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  target_s_ng_ran_node_id.to_json(j);
  j.end_obj();
  j.end_array();
}

// CellActivationFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t cell_activation_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {1, 7, 10, 130};
  return map_enum_number(names, 4, idx, "id");
}
bool cell_activation_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {1, 7, 10, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cell_activation_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 1:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cell_activation_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 1:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cell_activation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, activation_id_for_cell_activation, (uint16_t)0u, (uint16_t)255u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_activation_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cell_activation_fail_ies_o::get_crit));

    switch (id) {
      case 1: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(activation_id_for_cell_activation, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void cell_activation_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_int("value", activation_id_for_cell_activation);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// SSBsToBeActivated-Item ::= SEQUENCE
OCUDUASN_CODE ss_bs_to_be_activ_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, ssb_idx, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ss_bs_to_be_activ_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ssb_idx, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ss_bs_to_be_activ_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ssbIndex", ssb_idx);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ToBeActivatedNRCellsAndSSBs-Item ::= SEQUENCE
OCUDUASN_CODE to_be_activ_nr_cells_and_ss_bs_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ss_bstobe_activ_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.pack(bref));
  if (ss_bstobe_activ_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ss_bstobe_activ_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE to_be_activ_nr_cells_and_ss_bs_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ss_bstobe_activ_list_present;
  HANDLE_CODE(bref.unpack(ss_bstobe_activ_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.unpack(bref));
  if (ss_bstobe_activ_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ss_bstobe_activ_list, bref, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void to_be_activ_nr_cells_and_ss_bs_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("nrCGI");
  nr_cgi.to_json(j);
  if (ss_bstobe_activ_list.size() > 0) {
    j.start_array("sSBstobeActivatedList");
    for (const auto& e1 : ss_bstobe_activ_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ServedCellsToActivate-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t served_cells_to_activ_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {416};
  return map_enum_number(names, 1, idx, "id");
}
bool served_cells_to_activ_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 416 == id;
}
crit_e served_cells_to_activ_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 416) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
served_cells_to_activ_ext_ies_o::value_c served_cells_to_activ_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 416) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e served_cells_to_activ_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 416) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void served_cells_to_activ_ext_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_array("");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
}
OCUDUASN_CODE served_cells_to_activ_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 16384, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE served_cells_to_activ_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 16384, true));
  return OCUDUASN_SUCCESS;
}

const char* served_cells_to_activ_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"ToBeActivatedNRCellsAndSSBsList"};
  return convert_enum_idx(names, 1, value, "served_cells_to_activ_ext_ies_o::value_c::types");
}

// ServedCellsToActivate ::= CHOICE
void served_cells_to_activ_c::destroy_()
{
  switch (type_) {
    case types::nr_cells:
      c.destroy<nr_cells_l_>();
      break;
    case types::e_utra_cells:
      c.destroy<e_utra_cells_l_>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void served_cells_to_activ_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::nr_cells:
      c.init<nr_cells_l_>();
      break;
    case types::e_utra_cells:
      c.init<e_utra_cells_l_>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "served_cells_to_activ_c");
  }
}
served_cells_to_activ_c::served_cells_to_activ_c(const served_cells_to_activ_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::nr_cells:
      c.init(other.c.get<nr_cells_l_>());
      break;
    case types::e_utra_cells:
      c.init(other.c.get<e_utra_cells_l_>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "served_cells_to_activ_c");
  }
}
served_cells_to_activ_c& served_cells_to_activ_c::operator=(const served_cells_to_activ_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::nr_cells:
      c.set(other.c.get<nr_cells_l_>());
      break;
    case types::e_utra_cells:
      c.set(other.c.get<e_utra_cells_l_>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "served_cells_to_activ_c");
  }

  return *this;
}
served_cells_to_activ_c::nr_cells_l_& served_cells_to_activ_c::set_nr_cells()
{
  set(types::nr_cells);
  return c.get<nr_cells_l_>();
}
served_cells_to_activ_c::e_utra_cells_l_& served_cells_to_activ_c::set_e_utra_cells()
{
  set(types::e_utra_cells);
  return c.get<e_utra_cells_l_>();
}
protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>& served_cells_to_activ_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>();
}
void served_cells_to_activ_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::nr_cells:
      j.start_array("nr-cells");
      for (const auto& e1 : c.get<nr_cells_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e_utra_cells:
      j.start_array("e-utra-cells");
      for (const auto& e1 : c.get<e_utra_cells_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "served_cells_to_activ_c");
  }
  j.end_obj();
}
OCUDUASN_CODE served_cells_to_activ_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::nr_cells:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<nr_cells_l_>(), 1, 16384, true));
      break;
    case types::e_utra_cells:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e_utra_cells_l_>(), 1, 16384, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "served_cells_to_activ_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE served_cells_to_activ_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::nr_cells:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<nr_cells_l_>(), bref, 1, 16384, true));
      break;
    case types::e_utra_cells:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e_utra_cells_l_>(), bref, 1, 16384, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<served_cells_to_activ_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "served_cells_to_activ_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* served_cells_to_activ_c::types_opts::to_string() const
{
  static const char* names[] = {"nr-cells", "e-utra-cells", "choice-extension"};
  return convert_enum_idx(names, 3, value, "served_cells_to_activ_c::types");
}

// CellActivationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t cell_activation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {65, 1, 130};
  return map_enum_number(names, 3, idx, "id");
}
bool cell_activation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {65, 1, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cell_activation_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 65:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cell_activation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 65:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cell_activation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 65, crit_e{crit_e::reject}, [this, &bref]() { return served_cells_to_activ.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, activation_id_for_cell_activation, (uint16_t)0u, (uint16_t)255u, false, true);
  }));
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_activation_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cell_activation_request_ies_o::get_crit));

    switch (id) {
      case 65: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return served_cells_to_activ.unpack(bref); }));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(activation_id_for_cell_activation, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void cell_activation_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 65);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  served_cells_to_activ.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_int("value", activation_id_for_cell_activation);
  j.end_obj();
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// CellActivationResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t cell_activation_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 1, 10, 130};
  return map_enum_number(names, 4, idx, "id");
}
bool cell_activation_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 1, 10, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cell_activation_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 0:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cell_activation_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cell_activation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::reject}, [this, &bref]() { return activ_served_cells.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, activation_id_for_cell_activation, (uint16_t)0u, (uint16_t)255u, false, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_activation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cell_activation_resp_ies_o::get_crit));

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return activ_served_cells.unpack(bref); }));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(activation_id_for_cell_activation, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void cell_activation_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  activ_served_cells.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_int("value", activation_id_for_cell_activation);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// CellTrafficTraceIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t cell_traffic_trace_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 145, 263, 262, 226};
  return map_enum_number(names, 6, idx, "id");
}
bool cell_traffic_trace_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 145, 263, 262, 226};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cell_traffic_trace_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 145:
      return crit_e::ignore;
    case 263:
      return crit_e::ignore;
    case 262:
      return crit_e::ignore;
    case 226:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cell_traffic_trace_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 145:
      return presence_e::mandatory;
    case 263:
      return presence_e::mandatory;
    case 262:
      return presence_e::optional;
    case 226:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cell_traffic_trace_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += privacy_ind_present ? 1 : 0;
  nof_ies += trace_collection_entity_uri_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 145, crit_e{crit_e::ignore}, [this, &bref]() { return ng_ran_trace_id.pack(bref); }));
  HANDLE_CODE(pack_ie(
      bref, 263, crit_e{crit_e::ignore}, [this, &bref]() { return trace_collection_entity_ip_address.pack(bref); }));
  if (privacy_ind_present) {
    HANDLE_CODE(pack_ie(bref, 262, crit_e{crit_e::ignore}, [this, &bref]() { return privacy_ind.pack(bref); }));
  }
  if (trace_collection_entity_uri_present) {
    HANDLE_CODE(
        pack_ie(bref, 226, crit_e{crit_e::ignore}, [this, &bref]() { return trace_collection_entity_uri.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_traffic_trace_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cell_traffic_trace_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 145: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_trace_id.unpack(bref); }));
        break;
      }
      case 263: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_collection_entity_ip_address.unpack(bref); }));
        break;
      }
      case 262: {
        privacy_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return privacy_ind.unpack(bref); }));
        break;
      }
      case 226: {
        trace_collection_entity_uri_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_collection_entity_uri.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void cell_traffic_trace_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 145);
  j.write_str("criticality", "ignore");
  j.write_str("value", ng_ran_trace_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 263);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  trace_collection_entity_ip_address.to_json(j);
  j.end_obj();
  if (privacy_ind_present) {
    j.start_obj();
    j.write_int("id", 262);
    j.write_str("criticality", "ignore");
    j.write_str("value", privacy_ind.to_string());
    j.end_obj();
  }
  if (trace_collection_entity_uri_present) {
    j.start_obj();
    j.write_int("id", 226);
    j.write_str("criticality", "ignore");
    j.write_str("value", trace_collection_entity_uri.to_string());
    j.end_obj();
  }
  j.end_array();
}

// ConditionalHandoverCancel-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t conditional_ho_cancel_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73, 79, 7, 160, 445};
  return map_enum_number(names, 5, idx, "id");
}
bool conditional_ho_cancel_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73, 79, 7, 160, 445};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e conditional_ho_cancel_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::reject;
    case 79:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 160:
      return crit_e::reject;
    case 445:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e conditional_ho_cancel_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 79:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 160:
      return presence_e::optional;
    case 445:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE conditional_ho_cancel_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += target_cells_to_cancel_present ? 1 : 0;
  nof_ies += conditional_recfg_to_cancel_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, target_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (target_cells_to_cancel_present) {
    HANDLE_CODE(pack_ie(bref, 160, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, target_cells_to_cancel, 1, 8, true);
    }));
  }
  if (conditional_recfg_to_cancel_list_present) {
    HANDLE_CODE(pack_ie(bref, 445, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, conditional_recfg_to_cancel_list, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE conditional_ho_cancel_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &conditional_ho_cancel_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 79: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(target_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 160: {
        target_cells_to_cancel_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(target_cells_to_cancel, bref, 1, 8, true); }));
        break;
      }
      case 445: {
        conditional_recfg_to_cancel_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(conditional_recfg_to_cancel_list, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void conditional_ho_cancel_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "reject");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 79);
  j.write_str("criticality", "reject");
  j.write_uint("value", target_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (target_cells_to_cancel_present) {
    j.start_obj();
    j.write_int("id", 160);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : target_cells_to_cancel) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (conditional_recfg_to_cancel_list_present) {
    j.start_obj();
    j.write_int("id", 445);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : conditional_recfg_to_cancel_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// Configuration-rejected-by-M-NG-RANNode ::= SEQUENCE
OCUDUASN_CODE cfg_rejected_by_m_ng_ran_node_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(m_ng_ran_node_to_s_ng_ran_node_container.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(cause.pack(bref));
  if (m_ng_ran_node_to_s_ng_ran_node_container.size() > 0) {
    HANDLE_CODE(m_ng_ran_node_to_s_ng_ran_node_container.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_rejected_by_m_ng_ran_node_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool m_ng_ran_node_to_s_ng_ran_node_container_present;
  HANDLE_CODE(bref.unpack(m_ng_ran_node_to_s_ng_ran_node_container_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(cause.unpack(bref));
  if (m_ng_ran_node_to_s_ng_ran_node_container_present) {
    HANDLE_CODE(m_ng_ran_node_to_s_ng_ran_node_container.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cfg_rejected_by_m_ng_ran_node_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("cause");
  cause.to_json(j);
  if (m_ng_ran_node_to_s_ng_ran_node_container.size() > 0) {
    j.write_str("m-NG-RANNode-to-S-NG-RANNode-Container", m_ng_ran_node_to_s_ng_ran_node_container.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Configuration-successfully-applied-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
uint32_t cfg_successfully_applied_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {456};
  return map_enum_number(names, 1, idx, "id");
}
bool cfg_successfully_applied_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 456 == id;
}
crit_e cfg_successfully_applied_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 456) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e cfg_successfully_applied_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 456) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE cfg_successfully_applied_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += sk_counter_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (sk_counter_present) {
    HANDLE_CODE(pack_ie(bref, 456, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sk_counter, (uint32_t)0u, (uint32_t)65535u, false, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_successfully_applied_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cfg_successfully_applied_ext_ies_o::get_crit));

    if (id == 456) {
      {
        sk_counter_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sk_counter, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void cfg_successfully_applied_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (sk_counter_present) {
    j.start_obj();
    j.write_int("id", 456);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", sk_counter);
    j.end_obj();
  }
  j.end_array();
}

// Configuration-successfully-applied ::= SEQUENCE
OCUDUASN_CODE cfg_successfully_applied_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(m_ng_ran_node_to_s_ng_ran_node_container.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (m_ng_ran_node_to_s_ng_ran_node_container.size() > 0) {
    HANDLE_CODE(m_ng_ran_node_to_s_ng_ran_node_container.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_successfully_applied_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool m_ng_ran_node_to_s_ng_ran_node_container_present;
  HANDLE_CODE(bref.unpack(m_ng_ran_node_to_s_ng_ran_node_container_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (m_ng_ran_node_to_s_ng_ran_node_container_present) {
    HANDLE_CODE(m_ng_ran_node_to_s_ng_ran_node_container.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cfg_successfully_applied_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (m_ng_ran_node_to_s_ng_ran_node_container.size() > 0) {
    j.write_str("m-NG-RANNode-to-S-NG-RANNode-Container", m_ng_ran_node_to_s_ng_ran_node_container.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ConfigurationUpdate-gNB ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t cfg_upd_gnb_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {68, 8, 232, 347};
  return map_enum_number(names, 4, idx, "id");
}
bool cfg_upd_gnb_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {68, 8, 232, 347};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cfg_upd_gnb_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 68:
      return crit_e::ignore;
    case 8:
      return crit_e::ignore;
    case 232:
      return crit_e::ignore;
    case 347:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cfg_upd_gnb_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 68:
      return presence_e::optional;
    case 8:
      return presence_e::optional;
    case 232:
      return presence_e::optional;
    case 347:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// ConfigurationUpdate-ng-eNB ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t cfg_upd_ng_enb_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {66, 8, 232};
  return map_enum_number(names, 3, idx, "id");
}
bool cfg_upd_ng_enb_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {66, 8, 232};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cfg_upd_ng_enb_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 66:
      return crit_e::ignore;
    case 8:
      return crit_e::ignore;
    case 232:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cfg_upd_ng_enb_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 66:
      return presence_e::optional;
    case 8:
      return presence_e::optional;
    case 232:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cfg_upd_gnb_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += served_cells_to_upd_nr_present ? 1 : 0;
  nof_ies += cell_assist_info_nr_present ? 1 : 0;
  nof_ies += cell_assist_info_eutra_present ? 1 : 0;
  nof_ies += served_cell_specific_info_req_nr_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (served_cells_to_upd_nr_present) {
    HANDLE_CODE(
        pack_ie(bref, 68, crit_e{crit_e::ignore}, [this, &bref]() { return served_cells_to_upd_nr.pack(bref); }));
  }
  if (cell_assist_info_nr_present) {
    HANDLE_CODE(pack_ie(bref, 8, crit_e{crit_e::ignore}, [this, &bref]() { return cell_assist_info_nr.pack(bref); }));
  }
  if (cell_assist_info_eutra_present) {
    HANDLE_CODE(
        pack_ie(bref, 232, crit_e{crit_e::ignore}, [this, &bref]() { return cell_assist_info_eutra.pack(bref); }));
  }
  if (served_cell_specific_info_req_nr_present) {
    HANDLE_CODE(pack_ie(bref, 347, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, served_cell_specific_info_req_nr, 1, 16384, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_upd_gnb_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cfg_upd_gnb_o::get_crit));

    switch (id) {
      case 68: {
        served_cells_to_upd_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return served_cells_to_upd_nr.unpack(bref); }));
        break;
      }
      case 8: {
        cell_assist_info_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cell_assist_info_nr.unpack(bref); }));
        break;
      }
      case 232: {
        cell_assist_info_eutra_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cell_assist_info_eutra.unpack(bref); }));
        break;
      }
      case 347: {
        served_cell_specific_info_req_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(served_cell_specific_info_req_nr, bref, 1, 16384, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void cfg_upd_gnb_container::to_json(json_writer& j) const
{
  j.start_array();
  if (served_cells_to_upd_nr_present) {
    j.start_obj();
    j.write_int("id", 68);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    served_cells_to_upd_nr.to_json(j);
    j.end_obj();
  }
  if (cell_assist_info_nr_present) {
    j.start_obj();
    j.write_int("id", 8);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cell_assist_info_nr.to_json(j);
    j.end_obj();
  }
  if (cell_assist_info_eutra_present) {
    j.start_obj();
    j.write_int("id", 232);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cell_assist_info_eutra.to_json(j);
    j.end_obj();
  }
  if (served_cell_specific_info_req_nr_present) {
    j.start_obj();
    j.write_int("id", 347);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : served_cell_specific_info_req_nr) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

OCUDUASN_CODE cfg_upd_ng_enb_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += served_cells_to_upd_e_utra_present ? 1 : 0;
  nof_ies += cell_assist_info_nr_present ? 1 : 0;
  nof_ies += cell_assist_info_eutra_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (served_cells_to_upd_e_utra_present) {
    HANDLE_CODE(
        pack_ie(bref, 66, crit_e{crit_e::ignore}, [this, &bref]() { return served_cells_to_upd_e_utra.pack(bref); }));
  }
  if (cell_assist_info_nr_present) {
    HANDLE_CODE(pack_ie(bref, 8, crit_e{crit_e::ignore}, [this, &bref]() { return cell_assist_info_nr.pack(bref); }));
  }
  if (cell_assist_info_eutra_present) {
    HANDLE_CODE(
        pack_ie(bref, 232, crit_e{crit_e::ignore}, [this, &bref]() { return cell_assist_info_eutra.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_upd_ng_enb_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cfg_upd_ng_enb_o::get_crit));

    switch (id) {
      case 66: {
        served_cells_to_upd_e_utra_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return served_cells_to_upd_e_utra.unpack(bref); }));
        break;
      }
      case 8: {
        cell_assist_info_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cell_assist_info_nr.unpack(bref); }));
        break;
      }
      case 232: {
        cell_assist_info_eutra_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cell_assist_info_eutra.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void cfg_upd_ng_enb_container::to_json(json_writer& j) const
{
  j.start_array();
  if (served_cells_to_upd_e_utra_present) {
    j.start_obj();
    j.write_int("id", 66);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    served_cells_to_upd_e_utra.to_json(j);
    j.end_obj();
  }
  if (cell_assist_info_nr_present) {
    j.start_obj();
    j.write_int("id", 8);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cell_assist_info_nr.to_json(j);
    j.end_obj();
  }
  if (cell_assist_info_eutra_present) {
    j.start_obj();
    j.write_int("id", 232);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cell_assist_info_eutra.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ConfigurationUpdateInitiatingNodeChoice ::= CHOICE
void cfg_upd_init_node_choice_c::destroy_()
{
  switch (type_) {
    case types::gnb:
      c.destroy<cfg_upd_gnb_container>();
      break;
    case types::ng_enb:
      c.destroy<cfg_upd_ng_enb_container>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void cfg_upd_init_node_choice_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::gnb:
      c.init<cfg_upd_gnb_container>();
      break;
    case types::ng_enb:
      c.init<cfg_upd_ng_enb_container>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cfg_upd_init_node_choice_c");
  }
}
cfg_upd_init_node_choice_c::cfg_upd_init_node_choice_c(const cfg_upd_init_node_choice_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::gnb:
      c.init(other.c.get<cfg_upd_gnb_container>());
      break;
    case types::ng_enb:
      c.init(other.c.get<cfg_upd_ng_enb_container>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cfg_upd_init_node_choice_c");
  }
}
cfg_upd_init_node_choice_c& cfg_upd_init_node_choice_c::operator=(const cfg_upd_init_node_choice_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::gnb:
      c.set(other.c.get<cfg_upd_gnb_container>());
      break;
    case types::ng_enb:
      c.set(other.c.get<cfg_upd_ng_enb_container>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cfg_upd_init_node_choice_c");
  }

  return *this;
}
cfg_upd_gnb_container& cfg_upd_init_node_choice_c::set_gnb()
{
  set(types::gnb);
  return c.get<cfg_upd_gnb_container>();
}
cfg_upd_ng_enb_container& cfg_upd_init_node_choice_c::set_ng_enb()
{
  set(types::ng_enb);
  return c.get<cfg_upd_ng_enb_container>();
}
protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>&
cfg_upd_init_node_choice_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>();
}
void cfg_upd_init_node_choice_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::gnb:
      j.write_fieldname("gNB");
      c.get<cfg_upd_gnb_container>().to_json(j);
      break;
    case types::ng_enb:
      j.write_fieldname("ng-eNB");
      c.get<cfg_upd_ng_enb_container>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "cfg_upd_init_node_choice_c");
  }
  j.end_obj();
}
OCUDUASN_CODE cfg_upd_init_node_choice_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::gnb:
      HANDLE_CODE(c.get<cfg_upd_gnb_container>().pack(bref));
      break;
    case types::ng_enb:
      HANDLE_CODE(c.get<cfg_upd_ng_enb_container>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "cfg_upd_init_node_choice_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfg_upd_init_node_choice_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::gnb:
      HANDLE_CODE(c.get<cfg_upd_gnb_container>().unpack(bref));
      break;
    case types::ng_enb:
      HANDLE_CODE(c.get<cfg_upd_ng_enb_container>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<served_cells_to_upd_init_node_choice_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "cfg_upd_init_node_choice_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cfg_upd_init_node_choice_c::types_opts::to_string() const
{
  static const char* names[] = {"gNB", "ng-eNB", "choice-extension"};
  return convert_enum_idx(names, 3, value, "cfg_upd_init_node_choice_c::types");
}

// DLDiscarding ::= SEQUENCE
OCUDUASN_CODE dl_discarding_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, drbs_subject_to_dl_discarding, 1, 32, true));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_discarding_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(drbs_subject_to_dl_discarding, bref, 1, 32, true));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_discarding_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("dRBsSubjectToDLDiscarding");
  for (const auto& e1 : drbs_subject_to_dl_discarding) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// DataCollectionFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t data_collection_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {187, 188, 7, 10};
  return map_enum_number(names, 4, idx, "id");
}
bool data_collection_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {187, 188, 7, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e data_collection_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 187:
      return crit_e::reject;
    case 188:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e data_collection_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 187:
      return presence_e::mandatory;
    case 188:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE data_collection_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 187, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node1_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node2_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_collection_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &data_collection_fail_ies_o::get_crit));

    switch (id) {
      case 187: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node1_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 188: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node2_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void data_collection_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 187);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node1_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 188);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node2_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DataCollectionRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t data_collection_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {187, 188, 460, 461, 411, 462, 403, 408, 409};
  return map_enum_number(names, 9, idx, "id");
}
bool data_collection_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {187, 188, 460, 461, 411, 462, 403, 408, 409};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e data_collection_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 187:
      return crit_e::reject;
    case 188:
      return crit_e::ignore;
    case 460:
      return crit_e::reject;
    case 461:
      return crit_e::reject;
    case 411:
      return crit_e::ignore;
    case 462:
      return crit_e::ignore;
    case 403:
      return crit_e::ignore;
    case 408:
      return crit_e::ignore;
    case 409:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e data_collection_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 187:
      return presence_e::mandatory;
    case 188:
      return presence_e::conditional;
    case 460:
      return presence_e::mandatory;
    case 461:
      return presence_e::conditional;
    case 411:
      return presence_e::optional;
    case 462:
      return presence_e::optional;
    case 403:
      return presence_e::optional;
    case 408:
      return presence_e::optional;
    case 409:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE data_collection_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ngran_node2_meas_id_present ? 1 : 0;
  nof_ies += report_characteristics_for_data_collection_present ? 1 : 0;
  nof_ies += cell_to_report_for_data_collection_list_present ? 1 : 0;
  nof_ies += report_periodicity_for_data_collection_present ? 1 : 0;
  nof_ies += requested_prediction_time_present ? 1 : 0;
  nof_ies += ue_trajectory_collection_cfg_present ? 1 : 0;
  nof_ies += ue_performance_collection_cfg_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 187, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node1_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (ngran_node2_meas_id_present) {
    HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ngran_node2_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(
      bref, 460, crit_e{crit_e::reject}, [this, &bref]() { return regist_request_for_data_collection.pack(bref); }));
  if (report_characteristics_for_data_collection_present) {
    HANDLE_CODE(pack_ie(bref, 461, crit_e{crit_e::reject}, [this, &bref]() {
      return report_characteristics_for_data_collection.pack(bref);
    }));
  }
  if (cell_to_report_for_data_collection_list_present) {
    HANDLE_CODE(pack_ie(bref, 411, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, cell_to_report_for_data_collection_list, 1, 16384, true);
    }));
  }
  if (report_periodicity_for_data_collection_present) {
    HANDLE_CODE(pack_ie(bref, 462, crit_e{crit_e::ignore}, [this, &bref]() {
      return report_periodicity_for_data_collection.pack(bref);
    }));
  }
  if (requested_prediction_time_present) {
    HANDLE_CODE(pack_ie(bref, 403, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, requested_prediction_time, (uint8_t)1u, (uint8_t)60u, true, true);
    }));
  }
  if (ue_trajectory_collection_cfg_present) {
    HANDLE_CODE(pack_ie(
        bref, 408, crit_e{crit_e::ignore}, [this, &bref]() { return ue_trajectory_collection_cfg.pack(bref); }));
  }
  if (ue_performance_collection_cfg_present) {
    HANDLE_CODE(pack_ie(
        bref, 409, crit_e{crit_e::ignore}, [this, &bref]() { return ue_performance_collection_cfg.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_collection_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &data_collection_request_ies_o::get_crit));

    switch (id) {
      case 187: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node1_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 188: {
        ngran_node2_meas_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node2_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 460: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return regist_request_for_data_collection.unpack(bref); }));
        break;
      }
      case 461: {
        report_characteristics_for_data_collection_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return report_characteristics_for_data_collection.unpack(bref); }));
        break;
      }
      case 411: {
        cell_to_report_for_data_collection_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(cell_to_report_for_data_collection_list, bref, 1, 16384, true);
        }));
        break;
      }
      case 462: {
        report_periodicity_for_data_collection_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return report_periodicity_for_data_collection.unpack(bref); }));
        break;
      }
      case 403: {
        requested_prediction_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(requested_prediction_time, bref, (uint8_t)1u, (uint8_t)60u, true, true);
        }));
        break;
      }
      case 408: {
        ue_trajectory_collection_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_trajectory_collection_cfg.unpack(bref); }));
        break;
      }
      case 409: {
        ue_performance_collection_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_performance_collection_cfg.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void data_collection_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 187);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node1_meas_id);
  j.end_obj();
  if (ngran_node2_meas_id_present) {
    j.start_obj();
    j.write_int("id", 188);
    j.write_str("criticality", "ignore");
    j.write_int("value", ngran_node2_meas_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 460);
  j.write_str("criticality", "reject");
  j.write_str("value", regist_request_for_data_collection.to_string());
  j.end_obj();
  if (report_characteristics_for_data_collection_present) {
    j.start_obj();
    j.write_int("id", 461);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    report_characteristics_for_data_collection.to_json(j);
    j.end_obj();
  }
  if (cell_to_report_for_data_collection_list_present) {
    j.start_obj();
    j.write_int("id", 411);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cell_to_report_for_data_collection_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (report_periodicity_for_data_collection_present) {
    j.start_obj();
    j.write_int("id", 462);
    j.write_str("criticality", "ignore");
    j.write_str("value", report_periodicity_for_data_collection.to_string());
    j.end_obj();
  }
  if (requested_prediction_time_present) {
    j.start_obj();
    j.write_int("id", 403);
    j.write_str("criticality", "ignore");
    j.write_int("value", requested_prediction_time);
    j.end_obj();
  }
  if (ue_trajectory_collection_cfg_present) {
    j.start_obj();
    j.write_int("id", 408);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_trajectory_collection_cfg.to_json(j);
    j.end_obj();
  }
  if (ue_performance_collection_cfg_present) {
    j.start_obj();
    j.write_int("id", 409);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_performance_collection_cfg.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DataCollectionResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t data_collection_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {187, 188, 404, 405, 10};
  return map_enum_number(names, 5, idx, "id");
}
bool data_collection_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {187, 188, 404, 405, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e data_collection_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 187:
      return crit_e::reject;
    case 188:
      return crit_e::reject;
    case 404:
      return crit_e::reject;
    case 405:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e data_collection_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 187:
      return presence_e::mandatory;
    case 188:
      return presence_e::mandatory;
    case 404:
      return presence_e::optional;
    case 405:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE data_collection_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += node_meas_initiation_result_list_present ? 1 : 0;
  nof_ies += cell_meas_initiation_result_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 187, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node1_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node2_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (node_meas_initiation_result_list_present) {
    HANDLE_CODE(pack_ie(bref, 404, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, node_meas_initiation_result_list, 1, 124, true);
    }));
  }
  if (cell_meas_initiation_result_list_present) {
    HANDLE_CODE(pack_ie(bref, 405, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, cell_meas_initiation_result_list, 1, 16384, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_collection_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &data_collection_resp_ies_o::get_crit));

    switch (id) {
      case 187: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node1_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 188: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node2_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 404: {
        node_meas_initiation_result_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(node_meas_initiation_result_list, bref, 1, 124, true); }));
        break;
      }
      case 405: {
        cell_meas_initiation_result_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(cell_meas_initiation_result_list, bref, 1, 16384, true);
        }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void data_collection_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 187);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node1_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 188);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node2_meas_id);
  j.end_obj();
  if (node_meas_initiation_result_list_present) {
    j.start_obj();
    j.write_int("id", 404);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : node_meas_initiation_result_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (cell_meas_initiation_result_list_present) {
    j.start_obj();
    j.write_int("id", 405);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cell_meas_initiation_result_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DataCollectionUpdate-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t data_collection_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {187, 188, 410, 406, 463};
  return map_enum_number(names, 5, idx, "id");
}
bool data_collection_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {187, 188, 410, 406, 463};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e data_collection_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 187:
      return crit_e::reject;
    case 188:
      return crit_e::reject;
    case 410:
      return crit_e::ignore;
    case 406:
      return crit_e::ignore;
    case 463:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e data_collection_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 187:
      return presence_e::mandatory;
    case 188:
      return presence_e::mandatory;
    case 410:
      return presence_e::optional;
    case 406:
      return presence_e::optional;
    case 463:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE data_collection_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += cell_meas_result_for_data_collection_list_present ? 1 : 0;
  nof_ies += ue_associated_info_result_list_present ? 1 : 0;
  nof_ies += node_associated_info_result_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 187, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node1_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node2_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (cell_meas_result_for_data_collection_list_present) {
    HANDLE_CODE(pack_ie(bref, 410, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, cell_meas_result_for_data_collection_list, 1, 16384, true);
    }));
  }
  if (ue_associated_info_result_list_present) {
    HANDLE_CODE(pack_ie(bref, 406, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_associated_info_result_list, 1, 16, true);
    }));
  }
  if (node_associated_info_result_present) {
    HANDLE_CODE(
        pack_ie(bref, 463, crit_e{crit_e::ignore}, [this, &bref]() { return node_associated_info_result.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_collection_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &data_collection_upd_ies_o::get_crit));

    switch (id) {
      case 187: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node1_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 188: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node2_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 410: {
        cell_meas_result_for_data_collection_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(cell_meas_result_for_data_collection_list, bref, 1, 16384, true);
        }));
        break;
      }
      case 406: {
        ue_associated_info_result_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_associated_info_result_list, bref, 1, 16, true); }));
        break;
      }
      case 463: {
        node_associated_info_result_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return node_associated_info_result.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void data_collection_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 187);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node1_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 188);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node2_meas_id);
  j.end_obj();
  if (cell_meas_result_for_data_collection_list_present) {
    j.start_obj();
    j.write_int("id", 410);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cell_meas_result_for_data_collection_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_associated_info_result_list_present) {
    j.start_obj();
    j.write_int("id", 406);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_associated_info_result_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (node_associated_info_result_present) {
    j.start_obj();
    j.write_int("id", 463);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    node_associated_info_result.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DeactivateTraceIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t deactiv_trace_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 145};
  return map_enum_number(names, 3, idx, "id");
}
bool deactiv_trace_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 145};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e deactiv_trace_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 145:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e deactiv_trace_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 145:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE deactiv_trace_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 145, crit_e{crit_e::ignore}, [this, &bref]() { return ng_ran_trace_id.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE deactiv_trace_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &deactiv_trace_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 145: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_trace_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void deactiv_trace_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 145);
  j.write_str("criticality", "ignore");
  j.write_str("value", ng_ran_trace_id.to_string());
  j.end_obj();
  j.end_array();
}

// ResourceCoordRequest-ng-eNB-initiated ::= SEQUENCE
OCUDUASN_CODE res_coord_request_ng_enb_initiated_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(listof_e_utra_cells.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(data_traffic_res_ind.pack(bref));
  HANDLE_CODE(pack_integer(bref, spec_sharing_group_id, (uint16_t)1u, (uint16_t)16384u, false, true));
  if (listof_e_utra_cells.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, listof_e_utra_cells, 1, 16384, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_coord_request_ng_enb_initiated_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool listof_e_utra_cells_present;
  HANDLE_CODE(bref.unpack(listof_e_utra_cells_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(data_traffic_res_ind.unpack(bref));
  HANDLE_CODE(unpack_integer(spec_sharing_group_id, bref, (uint16_t)1u, (uint16_t)16384u, false, true));
  if (listof_e_utra_cells_present) {
    HANDLE_CODE(unpack_dyn_seq_of(listof_e_utra_cells, bref, 1, 16384, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_coord_request_ng_enb_initiated_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("dataTrafficResourceIndication");
  data_traffic_res_ind.to_json(j);
  j.write_int("spectrumSharingGroupID", spec_sharing_group_id);
  if (listof_e_utra_cells.size() > 0) {
    j.start_array("listofE-UTRACells");
    for (const auto& e1 : listof_e_utra_cells) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResourceCoordRequest-gNB-initiated ::= SEQUENCE
OCUDUASN_CODE res_coord_request_gnb_initiated_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(listof_e_utra_cells.size() > 0, 1));
  HANDLE_CODE(bref.pack(listof_nr_cells.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(data_traffic_res_ind.pack(bref));
  if (listof_e_utra_cells.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, listof_e_utra_cells, 1, 16384, true));
  }
  HANDLE_CODE(pack_integer(bref, spec_sharing_group_id, (uint16_t)1u, (uint16_t)16384u, false, true));
  if (listof_nr_cells.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, listof_nr_cells, 1, 16384, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_coord_request_gnb_initiated_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool listof_e_utra_cells_present;
  HANDLE_CODE(bref.unpack(listof_e_utra_cells_present, 1));
  bool listof_nr_cells_present;
  HANDLE_CODE(bref.unpack(listof_nr_cells_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(data_traffic_res_ind.unpack(bref));
  if (listof_e_utra_cells_present) {
    HANDLE_CODE(unpack_dyn_seq_of(listof_e_utra_cells, bref, 1, 16384, true));
  }
  HANDLE_CODE(unpack_integer(spec_sharing_group_id, bref, (uint16_t)1u, (uint16_t)16384u, false, true));
  if (listof_nr_cells_present) {
    HANDLE_CODE(unpack_dyn_seq_of(listof_nr_cells, bref, 1, 16384, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_coord_request_gnb_initiated_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("dataTrafficResourceIndication");
  data_traffic_res_ind.to_json(j);
  if (listof_e_utra_cells.size() > 0) {
    j.start_array("listofE-UTRACells");
    for (const auto& e1 : listof_e_utra_cells) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.write_int("spectrumSharingGroupID", spec_sharing_group_id);
  if (listof_nr_cells.size() > 0) {
    j.start_array("listofNRCells");
    for (const auto& e1 : listof_nr_cells) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// InitiatingNodeType-ResourceCoordRequest ::= CHOICE
void init_node_type_res_coord_request_c::destroy_()
{
  switch (type_) {
    case types::ng_enb:
      c.destroy<res_coord_request_ng_enb_initiated_s>();
      break;
    case types::gnb:
      c.destroy<res_coord_request_gnb_initiated_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void init_node_type_res_coord_request_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ng_enb:
      c.init<res_coord_request_ng_enb_initiated_s>();
      break;
    case types::gnb:
      c.init<res_coord_request_gnb_initiated_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "init_node_type_res_coord_request_c");
  }
}
init_node_type_res_coord_request_c::init_node_type_res_coord_request_c(const init_node_type_res_coord_request_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ng_enb:
      c.init(other.c.get<res_coord_request_ng_enb_initiated_s>());
      break;
    case types::gnb:
      c.init(other.c.get<res_coord_request_gnb_initiated_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "init_node_type_res_coord_request_c");
  }
}
init_node_type_res_coord_request_c&
init_node_type_res_coord_request_c::operator=(const init_node_type_res_coord_request_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ng_enb:
      c.set(other.c.get<res_coord_request_ng_enb_initiated_s>());
      break;
    case types::gnb:
      c.set(other.c.get<res_coord_request_gnb_initiated_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "init_node_type_res_coord_request_c");
  }

  return *this;
}
res_coord_request_ng_enb_initiated_s& init_node_type_res_coord_request_c::set_ng_enb()
{
  set(types::ng_enb);
  return c.get<res_coord_request_ng_enb_initiated_s>();
}
res_coord_request_gnb_initiated_s& init_node_type_res_coord_request_c::set_gnb()
{
  set(types::gnb);
  return c.get<res_coord_request_gnb_initiated_s>();
}
protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>&
init_node_type_res_coord_request_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>();
}
void init_node_type_res_coord_request_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ng_enb:
      j.write_fieldname("ng-eNB");
      c.get<res_coord_request_ng_enb_initiated_s>().to_json(j);
      break;
    case types::gnb:
      j.write_fieldname("gNB");
      c.get<res_coord_request_gnb_initiated_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "init_node_type_res_coord_request_c");
  }
  j.end_obj();
}
OCUDUASN_CODE init_node_type_res_coord_request_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ng_enb:
      HANDLE_CODE(c.get<res_coord_request_ng_enb_initiated_s>().pack(bref));
      break;
    case types::gnb:
      HANDLE_CODE(c.get<res_coord_request_gnb_initiated_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "init_node_type_res_coord_request_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE init_node_type_res_coord_request_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ng_enb:
      HANDLE_CODE(c.get<res_coord_request_ng_enb_initiated_s>().unpack(bref));
      break;
    case types::gnb:
      HANDLE_CODE(c.get<res_coord_request_gnb_initiated_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<init_node_type_res_coord_request_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "init_node_type_res_coord_request_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* init_node_type_res_coord_request_c::types_opts::to_string() const
{
  static const char* names[] = {"ng-eNB", "gNB", "choice-extension"};
  return convert_enum_idx(names, 3, value, "init_node_type_res_coord_request_c::types");
}

// E-UTRA-NR-CellResourceCoordinationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t e_utra_nr_cell_res_coordination_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {17, 130};
  return map_enum_number(names, 2, idx, "id");
}
bool e_utra_nr_cell_res_coordination_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {17, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_utra_nr_cell_res_coordination_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 17:
      return crit_e::reject;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e_utra_nr_cell_res_coordination_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 17:
      return presence_e::mandatory;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e_utra_nr_cell_res_coordination_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(
      bref, 17, crit_e{crit_e::reject}, [this, &bref]() { return init_node_type_res_coord_request.pack(bref); }));
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_utra_nr_cell_res_coordination_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e_utra_nr_cell_res_coordination_request_ies_o::get_crit));

    switch (id) {
      case 17: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return init_node_type_res_coord_request.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void e_utra_nr_cell_res_coordination_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 17);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  init_node_type_res_coord_request.to_json(j);
  j.end_obj();
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// ResourceCoordResponse-ng-eNB-initiated ::= SEQUENCE
OCUDUASN_CODE res_coord_resp_ng_enb_initiated_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(listof_e_utra_cells.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(data_traffic_res_ind.pack(bref));
  HANDLE_CODE(pack_integer(bref, spec_sharing_group_id, (uint16_t)1u, (uint16_t)16384u, false, true));
  if (listof_e_utra_cells.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, listof_e_utra_cells, 1, 16384, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_coord_resp_ng_enb_initiated_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool listof_e_utra_cells_present;
  HANDLE_CODE(bref.unpack(listof_e_utra_cells_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(data_traffic_res_ind.unpack(bref));
  HANDLE_CODE(unpack_integer(spec_sharing_group_id, bref, (uint16_t)1u, (uint16_t)16384u, false, true));
  if (listof_e_utra_cells_present) {
    HANDLE_CODE(unpack_dyn_seq_of(listof_e_utra_cells, bref, 1, 16384, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_coord_resp_ng_enb_initiated_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("dataTrafficResourceIndication");
  data_traffic_res_ind.to_json(j);
  j.write_int("spectrumSharingGroupID", spec_sharing_group_id);
  if (listof_e_utra_cells.size() > 0) {
    j.start_array("listofE-UTRACells");
    for (const auto& e1 : listof_e_utra_cells) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ResourceCoordResponse-gNB-initiated ::= SEQUENCE
OCUDUASN_CODE res_coord_resp_gnb_initiated_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(listof_nr_cells.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(data_traffic_res_ind.pack(bref));
  HANDLE_CODE(pack_integer(bref, spec_sharing_group_id, (uint16_t)1u, (uint16_t)16384u, false, true));
  if (listof_nr_cells.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, listof_nr_cells, 1, 16384, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_coord_resp_gnb_initiated_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool listof_nr_cells_present;
  HANDLE_CODE(bref.unpack(listof_nr_cells_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(data_traffic_res_ind.unpack(bref));
  HANDLE_CODE(unpack_integer(spec_sharing_group_id, bref, (uint16_t)1u, (uint16_t)16384u, false, true));
  if (listof_nr_cells_present) {
    HANDLE_CODE(unpack_dyn_seq_of(listof_nr_cells, bref, 1, 16384, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void res_coord_resp_gnb_initiated_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("dataTrafficResourceIndication");
  data_traffic_res_ind.to_json(j);
  j.write_int("spectrumSharingGroupID", spec_sharing_group_id);
  if (listof_nr_cells.size() > 0) {
    j.start_array("listofNRCells");
    for (const auto& e1 : listof_nr_cells) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RespondingNodeType-ResourceCoordResponse ::= CHOICE
void responding_node_type_res_coord_resp_c::destroy_()
{
  switch (type_) {
    case types::ng_enb:
      c.destroy<res_coord_resp_ng_enb_initiated_s>();
      break;
    case types::gnb:
      c.destroy<res_coord_resp_gnb_initiated_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void responding_node_type_res_coord_resp_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ng_enb:
      c.init<res_coord_resp_ng_enb_initiated_s>();
      break;
    case types::gnb:
      c.init<res_coord_resp_gnb_initiated_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_res_coord_resp_c");
  }
}
responding_node_type_res_coord_resp_c::responding_node_type_res_coord_resp_c(
    const responding_node_type_res_coord_resp_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ng_enb:
      c.init(other.c.get<res_coord_resp_ng_enb_initiated_s>());
      break;
    case types::gnb:
      c.init(other.c.get<res_coord_resp_gnb_initiated_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_res_coord_resp_c");
  }
}
responding_node_type_res_coord_resp_c&
responding_node_type_res_coord_resp_c::operator=(const responding_node_type_res_coord_resp_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ng_enb:
      c.set(other.c.get<res_coord_resp_ng_enb_initiated_s>());
      break;
    case types::gnb:
      c.set(other.c.get<res_coord_resp_gnb_initiated_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_res_coord_resp_c");
  }

  return *this;
}
res_coord_resp_ng_enb_initiated_s& responding_node_type_res_coord_resp_c::set_ng_enb()
{
  set(types::ng_enb);
  return c.get<res_coord_resp_ng_enb_initiated_s>();
}
res_coord_resp_gnb_initiated_s& responding_node_type_res_coord_resp_c::set_gnb()
{
  set(types::gnb);
  return c.get<res_coord_resp_gnb_initiated_s>();
}
protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>&
responding_node_type_res_coord_resp_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>();
}
void responding_node_type_res_coord_resp_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ng_enb:
      j.write_fieldname("ng-eNB");
      c.get<res_coord_resp_ng_enb_initiated_s>().to_json(j);
      break;
    case types::gnb:
      j.write_fieldname("gNB");
      c.get<res_coord_resp_gnb_initiated_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_res_coord_resp_c");
  }
  j.end_obj();
}
OCUDUASN_CODE responding_node_type_res_coord_resp_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ng_enb:
      HANDLE_CODE(c.get<res_coord_resp_ng_enb_initiated_s>().pack(bref));
      break;
    case types::gnb:
      HANDLE_CODE(c.get<res_coord_resp_gnb_initiated_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_res_coord_resp_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE responding_node_type_res_coord_resp_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ng_enb:
      HANDLE_CODE(c.get<res_coord_resp_ng_enb_initiated_s>().unpack(bref));
      break;
    case types::gnb:
      HANDLE_CODE(c.get<res_coord_resp_gnb_initiated_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<responding_node_type_res_coord_resp_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_res_coord_resp_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* responding_node_type_res_coord_resp_c::types_opts::to_string() const
{
  static const char* names[] = {"ng-eNB", "gNB", "choice-extension"};
  return convert_enum_idx(names, 3, value, "responding_node_type_res_coord_resp_c::types");
}

// E-UTRA-NR-CellResourceCoordinationResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t e_utra_nr_cell_res_coordination_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {59, 130};
  return map_enum_number(names, 2, idx, "id");
}
bool e_utra_nr_cell_res_coordination_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {59, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_utra_nr_cell_res_coordination_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 59:
      return crit_e::reject;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e_utra_nr_cell_res_coordination_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 59:
      return presence_e::mandatory;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e_utra_nr_cell_res_coordination_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(
      bref, 59, crit_e{crit_e::reject}, [this, &bref]() { return responding_node_type_res_coord_resp.pack(bref); }));
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_utra_nr_cell_res_coordination_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e_utra_nr_cell_res_coordination_resp_ies_o::get_crit));

    switch (id) {
      case 59: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return responding_node_type_res_coord_resp.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void e_utra_nr_cell_res_coordination_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 59);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  responding_node_type_res_coord_resp.to_json(j);
  j.end_obj();
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// FirstDLCount ::= SEQUENCE
OCUDUASN_CODE first_dl_count_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, drbs_subject_to_early_status_transfer, 1, 32, true));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE first_dl_count_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(drbs_subject_to_early_status_transfer, bref, 1, 32, true));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void first_dl_count_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("dRBsSubjectToEarlyStatusTransfer");
  for (const auto& e1 : drbs_subject_to_early_status_transfer) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// ProcedureStageChoice ::= CHOICE
void proc_stage_choice_c::destroy_()
{
  switch (type_) {
    case types::first_dl_count:
      c.destroy<first_dl_count_s>();
      break;
    case types::dl_discarding:
      c.destroy<dl_discarding_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void proc_stage_choice_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::first_dl_count:
      c.init<first_dl_count_s>();
      break;
    case types::dl_discarding:
      c.init<dl_discarding_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "proc_stage_choice_c");
  }
}
proc_stage_choice_c::proc_stage_choice_c(const proc_stage_choice_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::first_dl_count:
      c.init(other.c.get<first_dl_count_s>());
      break;
    case types::dl_discarding:
      c.init(other.c.get<dl_discarding_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "proc_stage_choice_c");
  }
}
proc_stage_choice_c& proc_stage_choice_c::operator=(const proc_stage_choice_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::first_dl_count:
      c.set(other.c.get<first_dl_count_s>());
      break;
    case types::dl_discarding:
      c.set(other.c.get<dl_discarding_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "proc_stage_choice_c");
  }

  return *this;
}
first_dl_count_s& proc_stage_choice_c::set_first_dl_count()
{
  set(types::first_dl_count);
  return c.get<first_dl_count_s>();
}
dl_discarding_s& proc_stage_choice_c::set_dl_discarding()
{
  set(types::dl_discarding);
  return c.get<dl_discarding_s>();
}
protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>& proc_stage_choice_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>();
}
void proc_stage_choice_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::first_dl_count:
      j.write_fieldname("first-dl-count");
      c.get<first_dl_count_s>().to_json(j);
      break;
    case types::dl_discarding:
      j.write_fieldname("dl-discarding");
      c.get<dl_discarding_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "proc_stage_choice_c");
  }
  j.end_obj();
}
OCUDUASN_CODE proc_stage_choice_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::first_dl_count:
      HANDLE_CODE(c.get<first_dl_count_s>().pack(bref));
      break;
    case types::dl_discarding:
      HANDLE_CODE(c.get<dl_discarding_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "proc_stage_choice_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE proc_stage_choice_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::first_dl_count:
      HANDLE_CODE(c.get<first_dl_count_s>().unpack(bref));
      break;
    case types::dl_discarding:
      HANDLE_CODE(c.get<dl_discarding_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<proc_stage_choice_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "proc_stage_choice_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* proc_stage_choice_c::types_opts::to_string() const
{
  static const char* names[] = {"first-dl-count", "dl-discarding", "choice-extension"};
  return convert_enum_idx(names, 3, value, "proc_stage_choice_c::types");
}

// EarlyStatusTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t early_status_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73, 79, 162};
  return map_enum_number(names, 3, idx, "id");
}
bool early_status_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73, 79, 162};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e early_status_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::reject;
    case 79:
      return crit_e::reject;
    case 162:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e early_status_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 79:
      return presence_e::mandatory;
    case 162:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE early_status_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, target_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 162, crit_e{crit_e::reject}, [this, &bref]() { return proc_stage.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE early_status_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &early_status_transfer_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 79: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(target_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 162: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return proc_stage.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void early_status_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "reject");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 79);
  j.write_str("criticality", "reject");
  j.write_uint("value", target_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 162);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  proc_stage.to_json(j);
  j.end_obj();
  j.end_array();
}

// ErrorIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t error_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 27, 7, 10, 130};
  return map_enum_number(names, 5, idx, "id");
}
bool error_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 27, 7, 10, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e error_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::ignore;
    case 27:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e error_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::optional;
    case 27:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE error_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += old_ng_ran_node_ue_xnap_id_present ? 1 : 0;
  nof_ies += new_ng_ran_node_ue_xnap_id_present ? 1 : 0;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (old_ng_ran_node_ue_xnap_id_present) {
    HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, old_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (new_ng_ran_node_ue_xnap_id_present) {
    HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (cause_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE error_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &error_ind_ies_o::get_crit));

    switch (id) {
      case 29: {
        old_ng_ran_node_ue_xnap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 27: {
        new_ng_ran_node_ue_xnap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        cause_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void error_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (old_ng_ran_node_ue_xnap_id_present) {
    j.start_obj();
    j.write_int("id", 29);
    j.write_str("criticality", "ignore");
    j.write_uint("value", old_ng_ran_node_ue_xnap_id);
    j.end_obj();
  }
  if (new_ng_ran_node_ue_xnap_id_present) {
    j.start_obj();
    j.write_int("id", 27);
    j.write_str("criticality", "ignore");
    j.write_uint("value", new_ng_ran_node_ue_xnap_id);
    j.end_obj();
  }
  if (cause_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cause.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// F1CTrafficTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t f1_c_traffic_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 296};
  return map_enum_number(names, 3, idx, "id");
}
bool f1_c_traffic_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 296};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e f1_c_traffic_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 296:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e f1_c_traffic_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 296:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE f1_c_traffic_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 296, crit_e{crit_e::reject}, [this, &bref]() { return f1_c_traffic_container.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_c_traffic_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &f1_c_traffic_transfer_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 296: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return f1_c_traffic_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void f1_c_traffic_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 296);
  j.write_str("criticality", "reject");
  j.write_str("value", f1_c_traffic_container.to_string());
  j.end_obj();
  j.end_array();
}

// FailureIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {177};
  return map_enum_number(names, 1, idx, "id");
}
bool fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  return 177 == id;
}
crit_e fail_ind_ies_o::get_crit(const uint32_t& id)
{
  if (id == 177) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e fail_ind_ies_o::get_presence(const uint32_t& id)
{
  if (id == 177) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(
      pack_ie(bref, 177, crit_e{crit_e::reject}, [this, &bref]() { return init_condition_fail_ind.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &fail_ind_ies_o::get_crit));

    if (id == 177) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return init_condition_fail_ind.unpack(bref); }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void fail_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 177);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  init_condition_fail_ind.to_json(j);
  j.end_obj();
  j.end_array();
}

// FastMCGRecoveryRRCTransfer ::= SEQUENCE
OCUDUASN_CODE fast_mcg_recovery_rrc_transfer_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(rrc_container.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE fast_mcg_recovery_rrc_transfer_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(rrc_container.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void fast_mcg_recovery_rrc_transfer_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("rrcContainer", rrc_container.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// HandoverCancel-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ho_cancel_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73, 79, 7, 160};
  return map_enum_number(names, 4, idx, "id");
}
bool ho_cancel_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73, 79, 7, 160};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_cancel_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::reject;
    case 79:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 160:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_cancel_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 79:
      return presence_e::optional;
    case 7:
      return presence_e::mandatory;
    case 160:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_cancel_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += target_ng_ran_node_ue_xnap_id_present ? 1 : 0;
  nof_ies += target_cells_to_cancel_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (target_ng_ran_node_ue_xnap_id_present) {
    HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, target_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (target_cells_to_cancel_present) {
    HANDLE_CODE(pack_ie(bref, 160, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, target_cells_to_cancel, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cancel_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_cancel_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 79: {
        target_ng_ran_node_ue_xnap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(target_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 160: {
        target_cells_to_cancel_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(target_cells_to_cancel, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_cancel_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "reject");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (target_ng_ran_node_ue_xnap_id_present) {
    j.start_obj();
    j.write_int("id", 79);
    j.write_str("criticality", "ignore");
    j.write_uint("value", target_ng_ran_node_ue_xnap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (target_cells_to_cancel_present) {
    j.start_obj();
    j.write_int("id", 160);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : target_cells_to_cancel) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// HandoverPreparationFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ho_prep_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73, 7, 10, 161};
  return map_enum_number(names, 4, idx, "id");
}
bool ho_prep_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73, 7, 10, 161};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_prep_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 161:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_prep_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 161:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_prep_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += requested_target_cell_global_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (requested_target_cell_global_id_present) {
    HANDLE_CODE(pack_ie(
        bref, 161, crit_e{crit_e::reject}, [this, &bref]() { return requested_target_cell_global_id.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_prep_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_prep_fail_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 161: {
        requested_target_cell_global_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_target_cell_global_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_prep_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "ignore");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (requested_target_cell_global_id_present) {
    j.start_obj();
    j.write_int("id", 161);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    requested_target_cell_global_id.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// HandoverReport-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ho_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {179, 180, 181, 182, 183, 184, 185, 176, 186, 291, 393, 394};
  return map_enum_number(names, 12, idx, "id");
}
bool ho_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {179, 180, 181, 182, 183, 184, 185, 176, 186, 291, 393, 394};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 179:
      return crit_e::ignore;
    case 180:
      return crit_e::ignore;
    case 181:
      return crit_e::ignore;
    case 182:
      return crit_e::ignore;
    case 183:
      return crit_e::ignore;
    case 184:
      return crit_e::ignore;
    case 185:
      return crit_e::ignore;
    case 176:
      return crit_e::ignore;
    case 186:
      return crit_e::ignore;
    case 291:
      return crit_e::ignore;
    case 393:
      return crit_e::ignore;
    case 394:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 179:
      return presence_e::mandatory;
    case 180:
      return presence_e::mandatory;
    case 181:
      return presence_e::mandatory;
    case 182:
      return presence_e::mandatory;
    case 183:
      return presence_e::conditional;
    case 184:
      return presence_e::conditional;
    case 185:
      return presence_e::optional;
    case 176:
      return presence_e::optional;
    case 186:
      return presence_e::optional;
    case 291:
      return presence_e::optional;
    case 393:
      return presence_e::optional;
    case 394:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += re_establishment_cell_cgi_present ? 1 : 0;
  nof_ies += target_cellin_eutran_present ? 1 : 0;
  nof_ies += source_cell_c_rnti_present ? 1 : 0;
  nof_ies += mob_info_present ? 1 : 0;
  nof_ies += ue_rlf_report_container_present ? 1 : 0;
  nof_ies += cho_cfg_present ? 1 : 0;
  nof_ies += target_cell_c_rnti_present ? 1 : 0;
  nof_ies += time_since_fail_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 179, crit_e{crit_e::ignore}, [this, &bref]() { return ho_report_type.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 180, crit_e{crit_e::ignore}, [this, &bref]() { return ho_cause.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 181, crit_e{crit_e::ignore}, [this, &bref]() { return source_cell_cgi.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 182, crit_e{crit_e::ignore}, [this, &bref]() { return target_cell_cgi.pack(bref); }));
  if (re_establishment_cell_cgi_present) {
    HANDLE_CODE(
        pack_ie(bref, 183, crit_e{crit_e::ignore}, [this, &bref]() { return re_establishment_cell_cgi.pack(bref); }));
  }
  if (target_cellin_eutran_present) {
    HANDLE_CODE(
        pack_ie(bref, 184, crit_e{crit_e::ignore}, [this, &bref]() { return target_cellin_eutran.pack(bref); }));
  }
  if (source_cell_c_rnti_present) {
    HANDLE_CODE(pack_ie(bref, 185, crit_e{crit_e::ignore}, [this, &bref]() { return source_cell_c_rnti.pack(bref); }));
  }
  if (mob_info_present) {
    HANDLE_CODE(pack_ie(bref, 176, crit_e{crit_e::ignore}, [this, &bref]() { return mob_info.pack(bref); }));
  }
  if (ue_rlf_report_container_present) {
    HANDLE_CODE(
        pack_ie(bref, 186, crit_e{crit_e::ignore}, [this, &bref]() { return ue_rlf_report_container.pack(bref); }));
  }
  if (cho_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 291, crit_e{crit_e::ignore}, [this, &bref]() { return cho_cfg.pack(bref); }));
  }
  if (target_cell_c_rnti_present) {
    HANDLE_CODE(pack_ie(bref, 393, crit_e{crit_e::ignore}, [this, &bref]() { return target_cell_c_rnti.pack(bref); }));
  }
  if (time_since_fail_present) {
    HANDLE_CODE(pack_ie(bref, 394, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, time_since_fail, (uint32_t)0u, (uint32_t)172800u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_report_ies_o::get_crit));

    switch (id) {
      case 179: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ho_report_type.unpack(bref); }));
        break;
      }
      case 180: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ho_cause.unpack(bref); }));
        break;
      }
      case 181: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_cell_cgi.unpack(bref); }));
        break;
      }
      case 182: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_cell_cgi.unpack(bref); }));
        break;
      }
      case 183: {
        re_establishment_cell_cgi_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return re_establishment_cell_cgi.unpack(bref); }));
        break;
      }
      case 184: {
        target_cellin_eutran_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_cellin_eutran.unpack(bref); }));
        break;
      }
      case 185: {
        source_cell_c_rnti_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_cell_c_rnti.unpack(bref); }));
        break;
      }
      case 176: {
        mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_info.unpack(bref); }));
        break;
      }
      case 186: {
        ue_rlf_report_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_rlf_report_container.unpack(bref); }));
        break;
      }
      case 291: {
        cho_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cho_cfg.unpack(bref); }));
        break;
      }
      case 393: {
        target_cell_c_rnti_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_cell_c_rnti.unpack(bref); }));
        break;
      }
      case 394: {
        time_since_fail_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(time_since_fail, bref, (uint32_t)0u, (uint32_t)172800u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 179);
  j.write_str("criticality", "ignore");
  j.write_str("value", ho_report_type.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 180);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  ho_cause.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 181);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  source_cell_cgi.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 182);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  target_cell_cgi.to_json(j);
  j.end_obj();
  if (re_establishment_cell_cgi_present) {
    j.start_obj();
    j.write_int("id", 183);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    re_establishment_cell_cgi.to_json(j);
    j.end_obj();
  }
  if (target_cellin_eutran_present) {
    j.start_obj();
    j.write_int("id", 184);
    j.write_str("criticality", "ignore");
    j.write_str("value", target_cellin_eutran.to_string());
    j.end_obj();
  }
  if (source_cell_c_rnti_present) {
    j.start_obj();
    j.write_int("id", 185);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_cell_c_rnti.to_json(j);
    j.end_obj();
  }
  if (mob_info_present) {
    j.start_obj();
    j.write_int("id", 176);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_info.to_json(j);
    j.end_obj();
  }
  if (ue_rlf_report_container_present) {
    j.start_obj();
    j.write_int("id", 186);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_rlf_report_container.to_json(j);
    j.end_obj();
  }
  if (cho_cfg_present) {
    j.start_obj();
    j.write_int("id", 291);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cho_cfg.to_json(j);
    j.end_obj();
  }
  if (target_cell_c_rnti_present) {
    j.start_obj();
    j.write_int("id", 393);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    target_cell_c_rnti.to_json(j);
    j.end_obj();
  }
  if (time_since_fail_present) {
    j.start_obj();
    j.write_int("id", 394);
    j.write_str("criticality", "ignore");
    j.write_int("value", time_since_fail);
    j.end_obj();
  }
  j.end_array();
}

// UEContextInfoHORequest-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
uint32_t ue_context_info_ho_request_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {155, 172, 171, 225, 227, 273, 346, 358, 399, 398};
  return map_enum_number(names, 10, idx, "id");
}
bool ue_context_info_ho_request_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {155, 172, 171, 225, 227, 273, 346, 358, 399, 398};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_info_ho_request_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 155:
      return crit_e::ignore;
    case 172:
      return crit_e::ignore;
    case 171:
      return crit_e::ignore;
    case 225:
      return crit_e::ignore;
    case 227:
      return crit_e::reject;
    case 273:
      return crit_e::ignore;
    case 346:
      return crit_e::ignore;
    case 358:
      return crit_e::ignore;
    case 399:
      return crit_e::ignore;
    case 398:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_info_ho_request_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 155:
      return presence_e::optional;
    case 172:
      return presence_e::optional;
    case 171:
      return presence_e::optional;
    case 225:
      return presence_e::optional;
    case 227:
      return presence_e::optional;
    case 273:
      return presence_e::optional;
    case 346:
      return presence_e::optional;
    case 358:
      return presence_e::optional;
    case 399:
      return presence_e::optional;
    case 398:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_info_ho_request_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += five_gc_mob_restrict_list_container_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += mdt_plmn_list_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  nof_ies += mbs_session_info_list_present ? 1 : 0;
  nof_ies += five_g_pro_se_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += ue_slice_max_bit_rate_list_present ? 1 : 0;
  nof_ies += nr_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += ltea2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (five_gc_mob_restrict_list_container_present) {
    HANDLE_CODE(pack_ie(
        bref, 155, crit_e{crit_e::ignore}, [this, &bref]() { return five_gc_mob_restrict_list_container.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 172, crit_e{crit_e::ignore}, [this, &bref]() { return nr_ue_sidelink_aggr_max_bit_rate.pack(bref); }));
  }
  if (lte_ue_sidelink_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 171, crit_e{crit_e::ignore}, [this, &bref]() { return lte_ue_sidelink_aggr_max_bit_rate.pack(bref); }));
  }
  if (mdt_plmn_list_present) {
    HANDLE_CODE(pack_ie(bref, 225, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, mdt_plmn_list, 1, 16, true);
    }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 227, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }
  if (mbs_session_info_list_present) {
    HANDLE_CODE(pack_ie(bref, 273, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, mbs_session_info_list, 1, 256, true);
    }));
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(bref, 346, crit_e{crit_e::ignore}, [this, &bref]() {
      return five_g_pro_se_ue_pc5_aggr_max_bit_rate.pack(bref);
    }));
  }
  if (ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 358, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 399, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (ltea2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 398, crit_e{crit_e::ignore}, [this, &bref]() { return ltea2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_info_ho_request_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_info_ho_request_ext_ies_o::get_crit));

    switch (id) {
      case 155: {
        five_gc_mob_restrict_list_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return five_gc_mob_restrict_list_container.unpack(bref); }));
        break;
      }
      case 172: {
        nr_ue_sidelink_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 171: {
        lte_ue_sidelink_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 225: {
        mdt_plmn_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(mdt_plmn_list, bref, 1, 16, true); }));
        break;
      }
      case 227: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 273: {
        mbs_session_info_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(mbs_session_info_list, bref, 1, 256, true); }));
        break;
      }
      case 346: {
        five_g_pro_se_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 358: {
        ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      case 399: {
        nr_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 398: {
        ltea2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltea2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void ue_context_info_ho_request_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (five_gc_mob_restrict_list_container_present) {
    j.start_obj();
    j.write_int("id", 155);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", five_gc_mob_restrict_list_container.to_string());
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 172);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    nr_ue_sidelink_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 171);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    lte_ue_sidelink_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (mdt_plmn_list_present) {
    j.start_obj();
    j.write_int("id", 225);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : mdt_plmn_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 227);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  if (mbs_session_info_list_present) {
    j.start_obj();
    j.write_int("id", 273);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : mbs_session_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 346);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    five_g_pro_se_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 358);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 399);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    nr_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (ltea2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 398);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    ltea2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextInfoHORequest ::= SEQUENCE
OCUDUASN_CODE ue_context_info_ho_request_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(idx_to_rat_freq_sel_prio_present, 1));
  HANDLE_CODE(bref.pack(location_report_info_present, 1));
  HANDLE_CODE(bref.pack(mrl_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, ng_c_ue_ref, (uint64_t)0u, (uint64_t)1099511627775u, false, true));
  HANDLE_CODE(cp_tnl_info_source.pack(bref));
  HANDLE_CODE(ue_security_cap.pack(bref));
  HANDLE_CODE(security_info.pack(bref));
  if (idx_to_rat_freq_sel_prio_present) {
    HANDLE_CODE(pack_integer(bref, idx_to_rat_freq_sel_prio, (uint16_t)1u, (uint16_t)256u, false, true));
  }
  HANDLE_CODE(ue_ambr.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_res_to_be_setup_list, 1, 256, true));
  HANDLE_CODE(rrc_context.pack(bref));
  if (location_report_info_present) {
    HANDLE_CODE(location_report_info.pack(bref));
  }
  if (mrl_present) {
    HANDLE_CODE(mrl.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_info_ho_request_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(idx_to_rat_freq_sel_prio_present, 1));
  HANDLE_CODE(bref.unpack(location_report_info_present, 1));
  HANDLE_CODE(bref.unpack(mrl_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ng_c_ue_ref, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true));
  HANDLE_CODE(cp_tnl_info_source.unpack(bref));
  HANDLE_CODE(ue_security_cap.unpack(bref));
  HANDLE_CODE(security_info.unpack(bref));
  if (idx_to_rat_freq_sel_prio_present) {
    HANDLE_CODE(unpack_integer(idx_to_rat_freq_sel_prio, bref, (uint16_t)1u, (uint16_t)256u, false, true));
  }
  HANDLE_CODE(ue_ambr.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(pdu_session_res_to_be_setup_list, bref, 1, 256, true));
  HANDLE_CODE(rrc_context.unpack(bref));
  if (location_report_info_present) {
    HANDLE_CODE(location_report_info.unpack(bref));
  }
  if (mrl_present) {
    HANDLE_CODE(mrl.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_info_ho_request_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_uint("ng-c-UE-reference", ng_c_ue_ref);
  j.write_fieldname("cp-TNL-info-source");
  cp_tnl_info_source.to_json(j);
  j.write_fieldname("ueSecurityCapabilities");
  ue_security_cap.to_json(j);
  j.write_fieldname("securityInformation");
  security_info.to_json(j);
  if (idx_to_rat_freq_sel_prio_present) {
    j.write_int("indexToRatFrequencySelectionPriority", idx_to_rat_freq_sel_prio);
  }
  j.write_fieldname("ue-AMBR");
  ue_ambr.to_json(j);
  j.start_array("pduSessionResourcesToBeSetup-List");
  for (const auto& e1 : pdu_session_res_to_be_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.write_str("rrc-Context", rrc_context.to_string());
  if (location_report_info_present) {
    j.write_fieldname("locationReportingInformation");
    location_report_info.to_json(j);
  }
  if (mrl_present) {
    j.write_fieldname("mrl");
    mrl.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// UEContextRefAtSN-HORequest ::= SEQUENCE
OCUDUASN_CODE ue_context_ref_at_sn_ho_request_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(global_ng_ran_node_id.pack(bref));
  HANDLE_CODE(pack_integer(bref, sn_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_ref_at_sn_ho_request_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(global_ng_ran_node_id.unpack(bref));
  HANDLE_CODE(unpack_integer(sn_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_ref_at_sn_ho_request_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("globalNG-RANNode-ID");
  global_ng_ran_node_id.to_json(j);
  j.write_uint("sN-NG-RANnodeUEXnAPID", sn_ng_ran_node_ue_xnap_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// HandoverRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ho_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73,  7,   78,  15,  83,  81,  22,  88,  87,  158, 170, 169, 173, 176, 178, 206, 298,
                                   325, 337, 344, 345, 376, 391, 395, 397, 396, 400, 401, 402, 415, 459, 453, 464};
  return map_enum_number(names, 33, idx, "id");
}
bool ho_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73,  7,   78,  15,  83,  81,  22,  88,  87,  158, 170, 169, 173, 176, 178, 206, 298,
                                   325, 337, 344, 345, 376, 391, 395, 397, 396, 400, 401, 402, 415, 459, 453, 464};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::reject;
    case 7:
      return crit_e::reject;
    case 78:
      return crit_e::reject;
    case 15:
      return crit_e::reject;
    case 83:
      return crit_e::reject;
    case 81:
      return crit_e::ignore;
    case 22:
      return crit_e::ignore;
    case 88:
      return crit_e::ignore;
    case 87:
      return crit_e::ignore;
    case 158:
      return crit_e::reject;
    case 170:
      return crit_e::ignore;
    case 169:
      return crit_e::ignore;
    case 173:
      return crit_e::ignore;
    case 176:
      return crit_e::ignore;
    case 178:
      return crit_e::ignore;
    case 206:
      return crit_e::reject;
    case 298:
      return crit_e::ignore;
    case 325:
      return crit_e::ignore;
    case 337:
      return crit_e::ignore;
    case 344:
      return crit_e::ignore;
    case 345:
      return crit_e::ignore;
    case 376:
      return crit_e::ignore;
    case 391:
      return crit_e::ignore;
    case 395:
      return crit_e::ignore;
    case 397:
      return crit_e::ignore;
    case 396:
      return crit_e::ignore;
    case 400:
      return crit_e::ignore;
    case 401:
      return crit_e::ignore;
    case 402:
      return crit_e::ignore;
    case 415:
      return crit_e::reject;
    case 459:
      return crit_e::ignore;
    case 453:
      return crit_e::reject;
    case 464:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 78:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 83:
      return presence_e::mandatory;
    case 81:
      return presence_e::optional;
    case 22:
      return presence_e::optional;
    case 88:
      return presence_e::mandatory;
    case 87:
      return presence_e::optional;
    case 158:
      return presence_e::optional;
    case 170:
      return presence_e::optional;
    case 169:
      return presence_e::optional;
    case 173:
      return presence_e::optional;
    case 176:
      return presence_e::optional;
    case 178:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 298:
      return presence_e::optional;
    case 325:
      return presence_e::optional;
    case 337:
      return presence_e::optional;
    case 344:
      return presence_e::optional;
    case 345:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 391:
      return presence_e::optional;
    case 395:
      return presence_e::optional;
    case 397:
      return presence_e::optional;
    case 396:
      return presence_e::optional;
    case 400:
      return presence_e::optional;
    case 401:
      return presence_e::optional;
    case 402:
      return presence_e::optional;
    case 415:
      return presence_e::optional;
    case 459:
      return presence_e::optional;
    case 453:
      return presence_e::optional;
    case 464:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 6;
  nof_ies += trace_activation_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += ue_context_ref_at_sn_ho_request_present ? 1 : 0;
  nof_ies += ch_oinfo_req_present ? 1 : 0;
  nof_ies += nr_v2x_services_authorized_present ? 1 : 0;
  nof_ies += ltev2x_services_authorized_present ? 1 : 0;
  nof_ies += pc5_qos_params_present ? 1 : 0;
  nof_ies += mob_info_present ? 1 : 0;
  nof_ies += ue_history_info_from_the_ue_present ? 1 : 0;
  nof_ies += iab_node_ind_present ? 1 : 0;
  nof_ies += no_pdu_session_ind_present ? 1 : 0;
  nof_ies += time_sync_assist_info_present ? 1 : 0;
  nof_ies += q_mcc_onfig_info_present ? 1 : 0;
  nof_ies += five_g_pro_se_authorized_present ? 1 : 0;
  nof_ies += five_g_pro_se_pc5_qos_params_present ? 1 : 0;
  nof_ies += iab_authorization_status_present ? 1 : 0;
  nof_ies += dl_lbt_fail_info_request_present ? 1 : 0;
  nof_ies += aerial_ue_sub_info_present ? 1 : 0;
  nof_ies += nr_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += ltea2_x_services_authorized_present ? 1 : 0;
  nof_ies += a2_xpc5_qos_params_present ? 1 : 0;
  nof_ies += cell_based_ue_trajectory_prediction_present ? 1 : 0;
  nof_ies += data_collection_id_present ? 1 : 0;
  nof_ies += candidate_relay_ue_info_list_present ? 1 : 0;
  nof_ies += source_sn_to_target_sn_qmc_info_present ? 1 : 0;
  nof_ies += mobile_iab_authorization_status_present ? 1 : 0;
  nof_ies += sl_positioning_ranging_services_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::reject}, [this, &bref]() { return cause.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 78, crit_e{crit_e::reject}, [this, &bref]() { return target_cell_global_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::reject}, [this, &bref]() { return guami.pack(bref); }));
  HANDLE_CODE(
      pack_ie(bref, 83, crit_e{crit_e::reject}, [this, &bref]() { return ue_context_info_ho_request.pack(bref); }));
  if (trace_activation_present) {
    HANDLE_CODE(pack_ie(bref, 81, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 22, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 88, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, ue_history_info, 1, 16, true);
  }));
  if (ue_context_ref_at_sn_ho_request_present) {
    HANDLE_CODE(pack_ie(
        bref, 87, crit_e{crit_e::ignore}, [this, &bref]() { return ue_context_ref_at_sn_ho_request.pack(bref); }));
  }
  if (ch_oinfo_req_present) {
    HANDLE_CODE(pack_ie(bref, 158, crit_e{crit_e::reject}, [this, &bref]() { return ch_oinfo_req.pack(bref); }));
  }
  if (nr_v2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 170, crit_e{crit_e::ignore}, [this, &bref]() { return nr_v2x_services_authorized.pack(bref); }));
  }
  if (ltev2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 169, crit_e{crit_e::ignore}, [this, &bref]() { return ltev2x_services_authorized.pack(bref); }));
  }
  if (pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 173, crit_e{crit_e::ignore}, [this, &bref]() { return pc5_qos_params.pack(bref); }));
  }
  if (mob_info_present) {
    HANDLE_CODE(pack_ie(bref, 176, crit_e{crit_e::ignore}, [this, &bref]() { return mob_info.pack(bref); }));
  }
  if (ue_history_info_from_the_ue_present) {
    HANDLE_CODE(
        pack_ie(bref, 178, crit_e{crit_e::ignore}, [this, &bref]() { return ue_history_info_from_the_ue.pack(bref); }));
  }
  if (iab_node_ind_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::reject}, [this, &bref]() { return iab_node_ind.pack(bref); }));
  }
  if (no_pdu_session_ind_present) {
    HANDLE_CODE(pack_ie(bref, 298, crit_e{crit_e::ignore}, [this, &bref]() { return no_pdu_session_ind.pack(bref); }));
  }
  if (time_sync_assist_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 325, crit_e{crit_e::ignore}, [this, &bref]() { return time_sync_assist_info.pack(bref); }));
  }
  if (q_mcc_onfig_info_present) {
    HANDLE_CODE(pack_ie(bref, 337, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_onfig_info.pack(bref); }));
  }
  if (five_g_pro_se_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 344, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_authorized.pack(bref); }));
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(
        bref, 345, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_pc5_qos_params.pack(bref); }));
  }
  if (iab_authorization_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 376, crit_e{crit_e::ignore}, [this, &bref]() { return iab_authorization_status.pack(bref); }));
  }
  if (dl_lbt_fail_info_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 391, crit_e{crit_e::ignore}, [this, &bref]() { return dl_lbt_fail_info_request.pack(bref); }));
  }
  if (aerial_ue_sub_info_present) {
    HANDLE_CODE(pack_ie(bref, 395, crit_e{crit_e::ignore}, [this, &bref]() { return aerial_ue_sub_info.pack(bref); }));
  }
  if (nr_a2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 397, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_services_authorized.pack(bref); }));
  }
  if (ltea2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 396, crit_e{crit_e::ignore}, [this, &bref]() { return ltea2_x_services_authorized.pack(bref); }));
  }
  if (a2_xpc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 400, crit_e{crit_e::ignore}, [this, &bref]() { return a2_xpc5_qos_params.pack(bref); }));
  }
  if (cell_based_ue_trajectory_prediction_present) {
    HANDLE_CODE(pack_ie(bref, 401, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, cell_based_ue_trajectory_prediction, 1, 16, true);
    }));
  }
  if (data_collection_id_present) {
    HANDLE_CODE(pack_ie(bref, 402, crit_e{crit_e::ignore}, [this, &bref]() { return data_collection_id.pack(bref); }));
  }
  if (candidate_relay_ue_info_list_present) {
    HANDLE_CODE(pack_ie(bref, 415, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, candidate_relay_ue_info_list, 1, 32, true);
    }));
  }
  if (source_sn_to_target_sn_qmc_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 459, crit_e{crit_e::ignore}, [this, &bref]() { return source_sn_to_target_sn_qmc_info.pack(bref); }));
  }
  if (mobile_iab_authorization_status_present) {
    HANDLE_CODE(pack_ie(
        bref, 453, crit_e{crit_e::reject}, [this, &bref]() { return mobile_iab_authorization_status.pack(bref); }));
  }
  if (sl_positioning_ranging_services_info_present) {
    HANDLE_CODE(pack_ie(bref, 464, crit_e{crit_e::ignore}, [this, &bref]() {
      return sl_positioning_ranging_services_info.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 6;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_request_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 78: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_cell_global_id.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return guami.unpack(bref); }));
        break;
      }
      case 83: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_info_ho_request.unpack(bref); }));
        break;
      }
      case 81: {
        trace_activation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_activation.unpack(bref); }));
        break;
      }
      case 22: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 88: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(ue_history_info, bref, 1, 16, true); }));
        break;
      }
      case 87: {
        ue_context_ref_at_sn_ho_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_ref_at_sn_ho_request.unpack(bref); }));
        break;
      }
      case 158: {
        ch_oinfo_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ch_oinfo_req.unpack(bref); }));
        break;
      }
      case 170: {
        nr_v2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_v2x_services_authorized.unpack(bref); }));
        break;
      }
      case 169: {
        ltev2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltev2x_services_authorized.unpack(bref); }));
        break;
      }
      case 173: {
        pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pc5_qos_params.unpack(bref); }));
        break;
      }
      case 176: {
        mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_info.unpack(bref); }));
        break;
      }
      case 178: {
        ue_history_info_from_the_ue_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_history_info_from_the_ue.unpack(bref); }));
        break;
      }
      case 206: {
        iab_node_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_node_ind.unpack(bref); }));
        break;
      }
      case 298: {
        no_pdu_session_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return no_pdu_session_ind.unpack(bref); }));
        break;
      }
      case 325: {
        time_sync_assist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_sync_assist_info.unpack(bref); }));
        break;
      }
      case 337: {
        q_mcc_onfig_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_onfig_info.unpack(bref); }));
        break;
      }
      case 344: {
        five_g_pro_se_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_authorized.unpack(bref); }));
        break;
      }
      case 345: {
        five_g_pro_se_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 376: {
        iab_authorization_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_authorization_status.unpack(bref); }));
        break;
      }
      case 391: {
        dl_lbt_fail_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return dl_lbt_fail_info_request.unpack(bref); }));
        break;
      }
      case 395: {
        aerial_ue_sub_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return aerial_ue_sub_info.unpack(bref); }));
        break;
      }
      case 397: {
        nr_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 396: {
        ltea2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltea2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 400: {
        a2_xpc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return a2_xpc5_qos_params.unpack(bref); }));
        break;
      }
      case 401: {
        cell_based_ue_trajectory_prediction_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(cell_based_ue_trajectory_prediction, bref, 1, 16, true);
        }));
        break;
      }
      case 402: {
        data_collection_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return data_collection_id.unpack(bref); }));
        break;
      }
      case 415: {
        candidate_relay_ue_info_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(candidate_relay_ue_info_list, bref, 1, 32, true); }));
        break;
      }
      case 459: {
        source_sn_to_target_sn_qmc_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_sn_to_target_sn_qmc_info.unpack(bref); }));
        break;
      }
      case 453: {
        mobile_iab_authorization_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_authorization_status.unpack(bref); }));
        break;
      }
      case 464: {
        sl_positioning_ranging_services_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sl_positioning_ranging_services_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "reject");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  target_cell_global_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  guami.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 83);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_context_info_ho_request.to_json(j);
  j.end_obj();
  if (trace_activation_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    trace_activation.to_json(j);
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 22);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 88);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : ue_history_info) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (ue_context_ref_at_sn_ho_request_present) {
    j.start_obj();
    j.write_int("id", 87);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_context_ref_at_sn_ho_request.to_json(j);
    j.end_obj();
  }
  if (ch_oinfo_req_present) {
    j.start_obj();
    j.write_int("id", 158);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ch_oinfo_req.to_json(j);
    j.end_obj();
  }
  if (nr_v2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 170);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_v2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltev2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 169);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltev2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 173);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (mob_info_present) {
    j.start_obj();
    j.write_int("id", 176);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_info.to_json(j);
    j.end_obj();
  }
  if (ue_history_info_from_the_ue_present) {
    j.start_obj();
    j.write_int("id", 178);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_history_info_from_the_ue.to_json(j);
    j.end_obj();
  }
  if (iab_node_ind_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (no_pdu_session_ind_present) {
    j.start_obj();
    j.write_int("id", 298);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (time_sync_assist_info_present) {
    j.start_obj();
    j.write_int("id", 325);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    time_sync_assist_info.to_json(j);
    j.end_obj();
  }
  if (q_mcc_onfig_info_present) {
    j.start_obj();
    j.write_int("id", 337);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_onfig_info.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_authorized_present) {
    j.start_obj();
    j.write_int("id", 344);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_authorized.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 345);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (iab_authorization_status_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "ignore");
    j.write_str("value", iab_authorization_status.to_string());
    j.end_obj();
  }
  if (dl_lbt_fail_info_request_present) {
    j.start_obj();
    j.write_int("id", 391);
    j.write_str("criticality", "ignore");
    j.write_str("value", "inquiry");
    j.end_obj();
  }
  if (aerial_ue_sub_info_present) {
    j.start_obj();
    j.write_int("id", 395);
    j.write_str("criticality", "ignore");
    j.write_str("value", aerial_ue_sub_info.to_string());
    j.end_obj();
  }
  if (nr_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 397);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltea2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 396);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltea2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (a2_xpc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 400);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    a2_xpc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (cell_based_ue_trajectory_prediction_present) {
    j.start_obj();
    j.write_int("id", 401);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cell_based_ue_trajectory_prediction) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (data_collection_id_present) {
    j.start_obj();
    j.write_int("id", 402);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    data_collection_id.to_json(j);
    j.end_obj();
  }
  if (candidate_relay_ue_info_list_present) {
    j.start_obj();
    j.write_int("id", 415);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : candidate_relay_ue_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (source_sn_to_target_sn_qmc_info_present) {
    j.start_obj();
    j.write_int("id", 459);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_sn_to_target_sn_qmc_info.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_authorization_status_present) {
    j.start_obj();
    j.write_int("id", 453);
    j.write_str("criticality", "reject");
    j.write_str("value", mobile_iab_authorization_status.to_string());
    j.end_obj();
  }
  if (sl_positioning_ranging_services_info_present) {
    j.start_obj();
    j.write_int("id", 464);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sl_positioning_ranging_services_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// HandoverRequestAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ho_request_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73, 79, 42, 43, 77, 86, 10, 137, 164, 159, 274, 61, 451};
  return map_enum_number(names, 13, idx, "id");
}
bool ho_request_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73, 79, 42, 43, 77, 86, 10, 137, 164, 159, 274, 61, 451};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_request_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::ignore;
    case 79:
      return crit_e::ignore;
    case 42:
      return crit_e::ignore;
    case 43:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 137:
      return crit_e::ignore;
    case 164:
      return crit_e::reject;
    case 159:
      return crit_e::reject;
    case 274:
      return crit_e::ignore;
    case 61:
      return crit_e::ignore;
    case 451:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_request_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 79:
      return presence_e::mandatory;
    case 42:
      return presence_e::mandatory;
    case 43:
      return presence_e::optional;
    case 77:
      return presence_e::mandatory;
    case 86:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 137:
      return presence_e::optional;
    case 164:
      return presence_e::optional;
    case 159:
      return presence_e::optional;
    case 274:
      return presence_e::optional;
    case 61:
      return presence_e::optional;
    case 451:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_request_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += pdu_session_res_not_admitted_list_present ? 1 : 0;
  nof_ies += ue_context_kept_ind_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += drbs_transferred_to_mn_present ? 1 : 0;
  nof_ies += daps_resp_info_list_present ? 1 : 0;
  nof_ies += ch_oinfo_ack_present ? 1 : 0;
  nof_ies += mbs_session_info_resp_list_present ? 1 : 0;
  nof_ies += rrc_cfg_ind_present ? 1 : 0;
  nof_ies += pdu_setbased_handling_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, target_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 42, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_admitted_list, 1, 256, true);
  }));
  if (pdu_session_res_not_admitted_list_present) {
    HANDLE_CODE(pack_ie(bref, 43, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_not_admitted_list, 1, 256, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 77, crit_e{crit_e::ignore}, [this, &bref]() {
    return target2_source_ng_ran_node_transp_container.pack(bref);
  }));
  if (ue_context_kept_ind_present) {
    HANDLE_CODE(pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return ue_context_kept_ind.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (drbs_transferred_to_mn_present) {
    HANDLE_CODE(pack_ie(bref, 137, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_transferred_to_mn, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
    }));
  }
  if (daps_resp_info_list_present) {
    HANDLE_CODE(pack_ie(bref, 164, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, daps_resp_info_list, 1, 32, true);
    }));
  }
  if (ch_oinfo_ack_present) {
    HANDLE_CODE(pack_ie(bref, 159, crit_e{crit_e::reject}, [this, &bref]() { return ch_oinfo_ack.pack(bref); }));
  }
  if (mbs_session_info_resp_list_present) {
    HANDLE_CODE(pack_ie(bref, 274, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, mbs_session_info_resp_list, 1, 256, true);
    }));
  }
  if (rrc_cfg_ind_present) {
    HANDLE_CODE(pack_ie(bref, 61, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_cfg_ind.pack(bref); }));
  }
  if (pdu_setbased_handling_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 451, crit_e{crit_e::ignore}, [this, &bref]() { return pdu_setbased_handling_ind.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_request_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_request_ack_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 79: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(target_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 42: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_admitted_list, bref, 1, 256, true); }));
        break;
      }
      case 43: {
        pdu_session_res_not_admitted_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_not_admitted_list, bref, 1, 256, true);
        }));
        break;
      }
      case 77: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return target2_source_ng_ran_node_transp_container.unpack(bref); }));
        break;
      }
      case 86: {
        ue_context_kept_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_kept_ind.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 137: {
        drbs_transferred_to_mn_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(
              drbs_transferred_to_mn, bref, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
        }));
        break;
      }
      case 164: {
        daps_resp_info_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(daps_resp_info_list, bref, 1, 32, true); }));
        break;
      }
      case 159: {
        ch_oinfo_ack_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ch_oinfo_ack.unpack(bref); }));
        break;
      }
      case 274: {
        mbs_session_info_resp_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(mbs_session_info_resp_list, bref, 1, 256, true); }));
        break;
      }
      case 61: {
        rrc_cfg_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_cfg_ind.unpack(bref); }));
        break;
      }
      case 451: {
        pdu_setbased_handling_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pdu_setbased_handling_ind.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_request_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "ignore");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 79);
  j.write_str("criticality", "ignore");
  j.write_uint("value", target_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 42);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_admitted_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (pdu_session_res_not_admitted_list_present) {
    j.start_obj();
    j.write_int("id", 43);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_not_admitted_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 77);
  j.write_str("criticality", "ignore");
  j.write_str("value", target2_source_ng_ran_node_transp_container.to_string());
  j.end_obj();
  if (ue_context_kept_ind_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (drbs_transferred_to_mn_present) {
    j.start_obj();
    j.write_int("id", 137);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_transferred_to_mn) {
      j.write_int(e1);
    }
    j.end_array();
    j.end_obj();
  }
  if (daps_resp_info_list_present) {
    j.start_obj();
    j.write_int("id", 164);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : daps_resp_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ch_oinfo_ack_present) {
    j.start_obj();
    j.write_int("id", 159);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ch_oinfo_ack.to_json(j);
    j.end_obj();
  }
  if (mbs_session_info_resp_list_present) {
    j.start_obj();
    j.write_int("id", 274);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : mbs_session_info_resp_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (rrc_cfg_ind_present) {
    j.start_obj();
    j.write_int("id", 61);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_cfg_ind.to_string());
    j.end_obj();
  }
  if (pdu_setbased_handling_ind_present) {
    j.start_obj();
    j.write_int("id", 451);
    j.write_str("criticality", "ignore");
    j.write_str("value", "supported");
    j.end_obj();
  }
  j.end_array();
}

// HandoverSuccess-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ho_success_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73, 79, 161, 444};
  return map_enum_number(names, 4, idx, "id");
}
bool ho_success_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73, 79, 161, 444};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_success_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::reject;
    case 79:
      return crit_e::reject;
    case 161:
      return crit_e::reject;
    case 444:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_success_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 79:
      return presence_e::mandatory;
    case 161:
      return presence_e::mandatory;
    case 444:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_success_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += accessed_pscell_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, target_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 161, crit_e{crit_e::reject}, [this, &bref]() { return requested_target_cell_global_id.pack(bref); }));
  if (accessed_pscell_id_present) {
    HANDLE_CODE(pack_ie(bref, 444, crit_e{crit_e::ignore}, [this, &bref]() { return accessed_pscell_id.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_success_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_success_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 79: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(target_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 161: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_target_cell_global_id.unpack(bref); }));
        break;
      }
      case 444: {
        accessed_pscell_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return accessed_pscell_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_success_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "reject");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 79);
  j.write_str("criticality", "reject");
  j.write_uint("value", target_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 161);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  requested_target_cell_global_id.to_json(j);
  j.end_obj();
  if (accessed_pscell_id_present) {
    j.start_obj();
    j.write_int("id", 444);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    accessed_pscell_id.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ParentNodeCellsList-Item ::= SEQUENCE
OCUDUASN_CODE parent_node_cells_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(parent_node_cell_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE parent_node_cells_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(parent_node_cell_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void parent_node_cells_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("parentNodeCellInformation");
  parent_node_cell_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// IABResourceCoordinationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t iab_res_coordination_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {314, 313, 315, 316};
  return map_enum_number(names, 4, idx, "id");
}
bool iab_res_coordination_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {314, 313, 315, 316};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_res_coordination_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 314:
      return crit_e::reject;
    case 313:
      return crit_e::reject;
    case 315:
      return crit_e::reject;
    case 316:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_res_coordination_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 314:
      return presence_e::mandatory;
    case 313:
      return presence_e::mandatory;
    case 315:
      return presence_e::optional;
    case 316:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_res_coordination_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += boundary_node_cells_list_present ? 1 : 0;
  nof_ies += parent_node_cells_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 314, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 313, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(
        bref, non_f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (boundary_node_cells_list_present) {
    HANDLE_CODE(pack_ie(bref, 315, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, boundary_node_cells_list, 1, 512, true);
    }));
  }
  if (parent_node_cells_list_present) {
    HANDLE_CODE(pack_ie(bref, 316, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, parent_node_cells_list, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_res_coordination_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_res_coordination_request_ies_o::get_crit));

    switch (id) {
      case 314: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 313: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              non_f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 315: {
        boundary_node_cells_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(boundary_node_cells_list, bref, 1, 512, true); }));
        break;
      }
      case 316: {
        parent_node_cells_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(parent_node_cells_list, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_res_coordination_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 314);
  j.write_str("criticality", "reject");
  j.write_uint("value", f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 313);
  j.write_str("criticality", "reject");
  j.write_uint("value", non_f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  if (boundary_node_cells_list_present) {
    j.start_obj();
    j.write_int("id", 315);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : boundary_node_cells_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (parent_node_cells_list_present) {
    j.start_obj();
    j.write_int("id", 316);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : parent_node_cells_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// IABResourceCoordinationResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t iab_res_coordination_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {314, 313, 315, 316};
  return map_enum_number(names, 4, idx, "id");
}
bool iab_res_coordination_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {314, 313, 315, 316};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_res_coordination_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 314:
      return crit_e::reject;
    case 313:
      return crit_e::reject;
    case 315:
      return crit_e::reject;
    case 316:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_res_coordination_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 314:
      return presence_e::mandatory;
    case 313:
      return presence_e::mandatory;
    case 315:
      return presence_e::optional;
    case 316:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_res_coordination_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += boundary_node_cells_list_present ? 1 : 0;
  nof_ies += parent_node_cells_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 314, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 313, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(
        bref, non_f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (boundary_node_cells_list_present) {
    HANDLE_CODE(pack_ie(bref, 315, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, boundary_node_cells_list, 1, 512, true);
    }));
  }
  if (parent_node_cells_list_present) {
    HANDLE_CODE(pack_ie(bref, 316, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, parent_node_cells_list, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_res_coordination_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_res_coordination_resp_ies_o::get_crit));

    switch (id) {
      case 314: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 313: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              non_f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 315: {
        boundary_node_cells_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(boundary_node_cells_list, bref, 1, 512, true); }));
        break;
      }
      case 316: {
        parent_node_cells_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(parent_node_cells_list, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_res_coordination_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 314);
  j.write_str("criticality", "reject");
  j.write_uint("value", f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 313);
  j.write_str("criticality", "reject");
  j.write_uint("value", non_f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  if (boundary_node_cells_list_present) {
    j.start_obj();
    j.write_int("id", 315);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : boundary_node_cells_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (parent_node_cells_list_present) {
    j.start_obj();
    j.write_int("id", 316);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : parent_node_cells_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// IABTNLAddressToBeReleased-Item ::= SEQUENCE
OCUDUASN_CODE iab_tnl_address_to_be_released_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(iab_tnl_address.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_tnl_address_to_be_released_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(iab_tnl_address.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void iab_tnl_address_to_be_released_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("iabTNLAddress");
  iab_tnl_address.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// IABTransportMigrationManagementReject-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t iab_transport_migration_management_reject_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {314, 313, 7, 10};
  return map_enum_number(names, 4, idx, "id");
}
bool iab_transport_migration_management_reject_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {314, 313, 7, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_transport_migration_management_reject_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 314:
      return crit_e::reject;
    case 313:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_transport_migration_management_reject_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 314:
      return presence_e::mandatory;
    case 313:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_transport_migration_management_reject_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 314, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 313, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(
        bref, non_f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_transport_migration_management_reject_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_transport_migration_management_reject_ies_o::get_crit));

    switch (id) {
      case 314: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 313: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              non_f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_transport_migration_management_reject_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 314);
  j.write_str("criticality", "reject");
  j.write_uint("value", f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 313);
  j.write_str("criticality", "reject");
  j.write_uint("value", non_f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TrafficToBeAdded-Item ::= SEQUENCE
OCUDUASN_CODE traffic_to_be_added_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(f1_terminating_topology_bh_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  HANDLE_CODE(traffic_profile.pack(bref));
  if (f1_terminating_topology_bh_info_present) {
    HANDLE_CODE(f1_terminating_topology_bh_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_to_be_added_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(f1_terminating_topology_bh_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  HANDLE_CODE(traffic_profile.unpack(bref));
  if (f1_terminating_topology_bh_info_present) {
    HANDLE_CODE(f1_terminating_topology_bh_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_to_be_added_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  j.write_fieldname("trafficProfile");
  traffic_profile.to_json(j);
  if (f1_terminating_topology_bh_info_present) {
    j.write_fieldname("f1-TerminatingTopologyBHInformation");
    f1_terminating_topology_bh_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TrafficToBeModified-Item ::= SEQUENCE
OCUDUASN_CODE traffic_to_be_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(traffic_profile_present, 1));
  HANDLE_CODE(bref.pack(f1_terminating_topology_bh_info_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (traffic_profile_present) {
    HANDLE_CODE(traffic_profile.pack(bref));
  }
  if (f1_terminating_topology_bh_info_present) {
    HANDLE_CODE(f1_terminating_topology_bh_info.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_to_be_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(traffic_profile_present, 1));
  HANDLE_CODE(bref.unpack(f1_terminating_topology_bh_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (traffic_profile_present) {
    HANDLE_CODE(traffic_profile.unpack(bref));
  }
  if (f1_terminating_topology_bh_info_present) {
    HANDLE_CODE(f1_terminating_topology_bh_info.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_to_be_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  if (traffic_profile_present) {
    j.write_fieldname("trafficProfile");
    traffic_profile.to_json(j);
  }
  if (f1_terminating_topology_bh_info_present) {
    j.write_fieldname("f1-TerminatingTopologyBHInformation");
    f1_terminating_topology_bh_info.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// IABTransportMigrationManagementRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t iab_transport_migration_management_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {314, 313, 301, 302, 303, 299, 321, 454};
  return map_enum_number(names, 8, idx, "id");
}
bool iab_transport_migration_management_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {314, 313, 301, 302, 303, 299, 321, 454};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_transport_migration_management_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 314:
      return crit_e::reject;
    case 313:
      return crit_e::reject;
    case 301:
      return crit_e::reject;
    case 302:
      return crit_e::reject;
    case 303:
      return crit_e::reject;
    case 299:
      return crit_e::reject;
    case 321:
      return crit_e::reject;
    case 454:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_transport_migration_management_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 314:
      return presence_e::mandatory;
    case 313:
      return presence_e::mandatory;
    case 301:
      return presence_e::optional;
    case 302:
      return presence_e::optional;
    case 303:
      return presence_e::optional;
    case 299:
      return presence_e::optional;
    case 321:
      return presence_e::optional;
    case 454:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_transport_migration_management_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += traffic_to_be_added_list_present ? 1 : 0;
  nof_ies += traffic_to_be_modified_list_present ? 1 : 0;
  nof_ies += traffic_to_be_release_info_present ? 1 : 0;
  nof_ies += iab_tnl_address_request_present ? 1 : 0;
  nof_ies += iab_tnl_address_exception_present ? 1 : 0;
  nof_ies += miab_mt_bap_address_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 314, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 313, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(
        bref, non_f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (traffic_to_be_added_list_present) {
    HANDLE_CODE(pack_ie(bref, 301, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_to_be_added_list, 1, 1024, true);
    }));
  }
  if (traffic_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 302, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_to_be_modified_list, 1, 1024, true);
    }));
  }
  if (traffic_to_be_release_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 303, crit_e{crit_e::reject}, [this, &bref]() { return traffic_to_be_release_info.pack(bref); }));
  }
  if (iab_tnl_address_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return iab_tnl_address_request.pack(bref); }));
  }
  if (iab_tnl_address_exception_present) {
    HANDLE_CODE(pack_ie(bref, 321, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, iab_tnl_address_exception, 1, 1024, true);
    }));
  }
  if (miab_mt_bap_address_present) {
    HANDLE_CODE(pack_ie(bref, 454, crit_e{crit_e::reject}, [this, &bref]() { return miab_mt_bap_address.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_transport_migration_management_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_transport_migration_management_request_ies_o::get_crit));

    switch (id) {
      case 314: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 313: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              non_f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 301: {
        traffic_to_be_added_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_to_be_added_list, bref, 1, 1024, true); }));
        break;
      }
      case 302: {
        traffic_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_to_be_modified_list, bref, 1, 1024, true); }));
        break;
      }
      case 303: {
        traffic_to_be_release_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return traffic_to_be_release_info.unpack(bref); }));
        break;
      }
      case 299: {
        iab_tnl_address_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_tnl_address_request.unpack(bref); }));
        break;
      }
      case 321: {
        iab_tnl_address_exception_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(iab_tnl_address_exception, bref, 1, 1024, true); }));
        break;
      }
      case 454: {
        miab_mt_bap_address_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return miab_mt_bap_address.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_transport_migration_management_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 314);
  j.write_str("criticality", "reject");
  j.write_uint("value", f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 313);
  j.write_str("criticality", "reject");
  j.write_uint("value", non_f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  if (traffic_to_be_added_list_present) {
    j.start_obj();
    j.write_int("id", 301);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_to_be_added_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (traffic_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 302);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (traffic_to_be_release_info_present) {
    j.start_obj();
    j.write_int("id", 303);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    traffic_to_be_release_info.to_json(j);
    j.end_obj();
  }
  if (iab_tnl_address_request_present) {
    j.start_obj();
    j.write_int("id", 299);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    iab_tnl_address_request.to_json(j);
    j.end_obj();
  }
  if (iab_tnl_address_exception_present) {
    j.start_obj();
    j.write_int("id", 321);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : iab_tnl_address_exception) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (miab_mt_bap_address_present) {
    j.start_obj();
    j.write_int("id", 454);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    miab_mt_bap_address.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TrafficAdded-Item ::= SEQUENCE
OCUDUASN_CODE traffic_added_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  HANDLE_CODE(non_f1_terminating_topology_bh_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_added_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  HANDLE_CODE(non_f1_terminating_topology_bh_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_added_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  j.write_fieldname("non-F1-TerminatingTopologyBHInformation");
  non_f1_terminating_topology_bh_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TrafficModified-Item ::= SEQUENCE
OCUDUASN_CODE traffic_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  HANDLE_CODE(non_f1_terminating_topology_bh_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  HANDLE_CODE(non_f1_terminating_topology_bh_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  j.write_fieldname("non-F1-TerminatingTopologyBHInformation");
  non_f1_terminating_topology_bh_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TrafficNotAdded-Item ::= SEQUENCE
OCUDUASN_CODE traffic_not_added_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(casue_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (casue_present) {
    HANDLE_CODE(casue.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_not_added_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(casue_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (casue_present) {
    HANDLE_CODE(casue.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_not_added_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  if (casue_present) {
    j.write_fieldname("casue");
    casue.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TrafficNotModified-Item ::= SEQUENCE
OCUDUASN_CODE traffic_not_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cause_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (cause_present) {
    HANDLE_CODE(cause.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_not_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(cause_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (cause_present) {
    HANDLE_CODE(cause.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_not_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  if (cause_present) {
    j.write_fieldname("cause");
    cause.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TrafficReleased-Item ::= SEQUENCE
OCUDUASN_CODE traffic_released_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bh_info_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (bh_info_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, bh_info_list, 1, 1024, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_released_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool bh_info_list_present;
  HANDLE_CODE(bref.unpack(bh_info_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (bh_info_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(bh_info_list, bref, 1, 1024, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_released_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  if (bh_info_list.size() > 0) {
    j.start_array("bHInfoList");
    for (const auto& e1 : bh_info_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// IABTransportMigrationManagementResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t iab_transport_migration_management_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {314, 313, 304, 305, 306, 307, 300, 310};
  return map_enum_number(names, 8, idx, "id");
}
bool iab_transport_migration_management_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {314, 313, 304, 305, 306, 307, 300, 310};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_transport_migration_management_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 314:
      return crit_e::reject;
    case 313:
      return crit_e::reject;
    case 304:
      return crit_e::reject;
    case 305:
      return crit_e::reject;
    case 306:
      return crit_e::reject;
    case 307:
      return crit_e::reject;
    case 300:
      return crit_e::reject;
    case 310:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_transport_migration_management_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 314:
      return presence_e::mandatory;
    case 313:
      return presence_e::mandatory;
    case 304:
      return presence_e::optional;
    case 305:
      return presence_e::optional;
    case 306:
      return presence_e::optional;
    case 307:
      return presence_e::optional;
    case 300:
      return presence_e::optional;
    case 310:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_transport_migration_management_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += traffic_added_list_present ? 1 : 0;
  nof_ies += traffic_modified_list_present ? 1 : 0;
  nof_ies += traffic_not_added_list_present ? 1 : 0;
  nof_ies += traffic_not_modified_list_present ? 1 : 0;
  nof_ies += iab_tnl_address_resp_present ? 1 : 0;
  nof_ies += traffic_released_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 314, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 313, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(
        bref, non_f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (traffic_added_list_present) {
    HANDLE_CODE(pack_ie(bref, 304, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_added_list, 1, 1024, true);
    }));
  }
  if (traffic_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 305, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_modified_list, 1, 1024, true);
    }));
  }
  if (traffic_not_added_list_present) {
    HANDLE_CODE(pack_ie(bref, 306, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_not_added_list, 1, 1024, true);
    }));
  }
  if (traffic_not_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 307, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_not_modified_list, 1, 1024, true);
    }));
  }
  if (iab_tnl_address_resp_present) {
    HANDLE_CODE(
        pack_ie(bref, 300, crit_e{crit_e::reject}, [this, &bref]() { return iab_tnl_address_resp.pack(bref); }));
  }
  if (traffic_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 310, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_released_list, 1, 1024, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_transport_migration_management_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_transport_migration_management_resp_ies_o::get_crit));

    switch (id) {
      case 314: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 313: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              non_f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 304: {
        traffic_added_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_added_list, bref, 1, 1024, true); }));
        break;
      }
      case 305: {
        traffic_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_modified_list, bref, 1, 1024, true); }));
        break;
      }
      case 306: {
        traffic_not_added_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_not_added_list, bref, 1, 1024, true); }));
        break;
      }
      case 307: {
        traffic_not_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_not_modified_list, bref, 1, 1024, true); }));
        break;
      }
      case 300: {
        iab_tnl_address_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_tnl_address_resp.unpack(bref); }));
        break;
      }
      case 310: {
        traffic_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_released_list, bref, 1, 1024, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_transport_migration_management_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 314);
  j.write_str("criticality", "reject");
  j.write_uint("value", f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 313);
  j.write_str("criticality", "reject");
  j.write_uint("value", non_f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  if (traffic_added_list_present) {
    j.start_obj();
    j.write_int("id", 304);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_added_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (traffic_modified_list_present) {
    j.start_obj();
    j.write_int("id", 305);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (traffic_not_added_list_present) {
    j.start_obj();
    j.write_int("id", 306);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_not_added_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (traffic_not_modified_list_present) {
    j.start_obj();
    j.write_int("id", 307);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_not_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (iab_tnl_address_resp_present) {
    j.start_obj();
    j.write_int("id", 300);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    iab_tnl_address_resp.to_json(j);
    j.end_obj();
  }
  if (traffic_released_list_present) {
    j.start_obj();
    j.write_int("id", 310);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// TrafficRequiredToBeModified-Item ::= SEQUENCE
OCUDUASN_CODE traffic_required_to_be_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  HANDLE_CODE(non_f1_terminating_topology_bh_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_required_to_be_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  HANDLE_CODE(non_f1_terminating_topology_bh_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_required_to_be_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  j.write_fieldname("non-f1-TerminatingTopologyBHInformation");
  non_f1_terminating_topology_bh_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// IABTransportMigrationModificationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t iab_transport_migration_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {314, 313, 308, 303, 311, 312, 376, 453};
  return map_enum_number(names, 8, idx, "id");
}
bool iab_transport_migration_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {314, 313, 308, 303, 311, 312, 376, 453};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_transport_migration_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 314:
      return crit_e::reject;
    case 313:
      return crit_e::reject;
    case 308:
      return crit_e::reject;
    case 303:
      return crit_e::reject;
    case 311:
      return crit_e::reject;
    case 312:
      return crit_e::reject;
    case 376:
      return crit_e::ignore;
    case 453:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_transport_migration_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 314:
      return presence_e::mandatory;
    case 313:
      return presence_e::mandatory;
    case 308:
      return presence_e::optional;
    case 303:
      return presence_e::optional;
    case 311:
      return presence_e::optional;
    case 312:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 453:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_transport_migration_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += traffic_required_to_be_modified_list_present ? 1 : 0;
  nof_ies += traffic_to_be_release_info_present ? 1 : 0;
  nof_ies += iab_tnl_address_to_be_added_present ? 1 : 0;
  nof_ies += iab_tnl_address_to_be_released_list_present ? 1 : 0;
  nof_ies += iab_authorization_status_present ? 1 : 0;
  nof_ies += mobile_iab_authorization_status_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 314, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 313, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(
        bref, non_f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (traffic_required_to_be_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 308, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_required_to_be_modified_list, 1, 1024, true);
    }));
  }
  if (traffic_to_be_release_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 303, crit_e{crit_e::reject}, [this, &bref]() { return traffic_to_be_release_info.pack(bref); }));
  }
  if (iab_tnl_address_to_be_added_present) {
    HANDLE_CODE(
        pack_ie(bref, 311, crit_e{crit_e::reject}, [this, &bref]() { return iab_tnl_address_to_be_added.pack(bref); }));
  }
  if (iab_tnl_address_to_be_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 312, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, iab_tnl_address_to_be_released_list, 1, 1024, true);
    }));
  }
  if (iab_authorization_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 376, crit_e{crit_e::ignore}, [this, &bref]() { return iab_authorization_status.pack(bref); }));
  }
  if (mobile_iab_authorization_status_present) {
    HANDLE_CODE(pack_ie(
        bref, 453, crit_e{crit_e::ignore}, [this, &bref]() { return mobile_iab_authorization_status.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_transport_migration_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_transport_migration_mod_request_ies_o::get_crit));

    switch (id) {
      case 314: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 313: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              non_f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 308: {
        traffic_required_to_be_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(traffic_required_to_be_modified_list, bref, 1, 1024, true);
        }));
        break;
      }
      case 303: {
        traffic_to_be_release_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return traffic_to_be_release_info.unpack(bref); }));
        break;
      }
      case 311: {
        iab_tnl_address_to_be_added_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_tnl_address_to_be_added.unpack(bref); }));
        break;
      }
      case 312: {
        iab_tnl_address_to_be_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(iab_tnl_address_to_be_released_list, bref, 1, 1024, true);
        }));
        break;
      }
      case 376: {
        iab_authorization_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_authorization_status.unpack(bref); }));
        break;
      }
      case 453: {
        mobile_iab_authorization_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_authorization_status.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_transport_migration_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 314);
  j.write_str("criticality", "reject");
  j.write_uint("value", f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 313);
  j.write_str("criticality", "reject");
  j.write_uint("value", non_f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  if (traffic_required_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 308);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_required_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (traffic_to_be_release_info_present) {
    j.start_obj();
    j.write_int("id", 303);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    traffic_to_be_release_info.to_json(j);
    j.end_obj();
  }
  if (iab_tnl_address_to_be_added_present) {
    j.start_obj();
    j.write_int("id", 311);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    iab_tnl_address_to_be_added.to_json(j);
    j.end_obj();
  }
  if (iab_tnl_address_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 312);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : iab_tnl_address_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (iab_authorization_status_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "ignore");
    j.write_str("value", iab_authorization_status.to_string());
    j.end_obj();
  }
  if (mobile_iab_authorization_status_present) {
    j.start_obj();
    j.write_int("id", 453);
    j.write_str("criticality", "ignore");
    j.write_str("value", mobile_iab_authorization_status.to_string());
    j.end_obj();
  }
  j.end_array();
}

// TrafficRequiredModified-Item ::= SEQUENCE
OCUDUASN_CODE traffic_required_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, traffic_idx, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE traffic_required_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(traffic_idx, bref, (uint16_t)1u, (uint16_t)1024u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void traffic_required_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("trafficIndex", traffic_idx);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// IABTransportMigrationModificationResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t iab_transport_migration_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {314, 313, 309, 310};
  return map_enum_number(names, 4, idx, "id");
}
bool iab_transport_migration_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {314, 313, 309, 310};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_transport_migration_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 314:
      return crit_e::reject;
    case 313:
      return crit_e::reject;
    case 309:
      return crit_e::reject;
    case 310:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_transport_migration_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 314:
      return presence_e::mandatory;
    case 313:
      return presence_e::mandatory;
    case 309:
      return presence_e::optional;
    case 310:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_transport_migration_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += traffic_required_modified_list_present ? 1 : 0;
  nof_ies += traffic_released_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 314, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 313, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(
        bref, non_f1_terminating_iab_donor_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (traffic_required_modified_list_present) {
    HANDLE_CODE(pack_ie(bref, 309, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_required_modified_list, 1, 1024, true);
    }));
  }
  if (traffic_released_list_present) {
    HANDLE_CODE(pack_ie(bref, 310, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, traffic_released_list, 1, 1024, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_transport_migration_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &iab_transport_migration_mod_resp_ies_o::get_crit));

    switch (id) {
      case 314: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 313: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              non_f1_terminating_iab_donor_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 309: {
        traffic_required_modified_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_required_modified_list, bref, 1, 1024, true); }));
        break;
      }
      case 310: {
        traffic_released_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(traffic_released_list, bref, 1, 1024, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void iab_transport_migration_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 314);
  j.write_str("criticality", "reject");
  j.write_uint("value", f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 313);
  j.write_str("criticality", "reject");
  j.write_uint("value", non_f1_terminating_iab_donor_ue_xnap_id);
  j.end_obj();
  if (traffic_required_modified_list_present) {
    j.start_obj();
    j.write_int("id", 309);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_required_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (traffic_released_list_present) {
    j.start_obj();
    j.write_int("id", 310);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : traffic_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// MobilityChangeAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t mob_change_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {194, 195, 10};
  return map_enum_number(names, 3, idx, "id");
}
bool mob_change_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {194, 195, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mob_change_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 194:
      return crit_e::reject;
    case 195:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mob_change_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 194:
      return presence_e::mandatory;
    case 195:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mob_change_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 194, crit_e{crit_e::reject}, [this, &bref]() { return ng_ran_node1_cell_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 195, crit_e{crit_e::reject}, [this, &bref]() { return ng_ran_node2_cell_id.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mob_change_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mob_change_ack_ies_o::get_crit));

    switch (id) {
      case 194: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_node1_cell_id.unpack(bref); }));
        break;
      }
      case 195: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_node2_cell_id.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mob_change_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 194);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ng_ran_node1_cell_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 195);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ng_ran_node2_cell_id.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MobilityChangeFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t mob_change_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {194, 195, 7, 198, 10, 294};
  return map_enum_number(names, 6, idx, "id");
}
bool mob_change_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {194, 195, 7, 198, 10, 294};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mob_change_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 194:
      return crit_e::reject;
    case 195:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 198:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 294:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mob_change_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 194:
      return presence_e::mandatory;
    case 195:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 198:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 294:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mob_change_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += mob_params_mod_range_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += ng_ran_node2_ssb_offsets_mod_range_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 194, crit_e{crit_e::reject}, [this, &bref]() { return ng_ran_node1_cell_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 195, crit_e{crit_e::reject}, [this, &bref]() { return ng_ran_node2_cell_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (mob_params_mod_range_present) {
    HANDLE_CODE(
        pack_ie(bref, 198, crit_e{crit_e::ignore}, [this, &bref]() { return mob_params_mod_range.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (ng_ran_node2_ssb_offsets_mod_range_present) {
    HANDLE_CODE(pack_ie(bref, 294, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ng_ran_node2_ssb_offsets_mod_range, 1, 64, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mob_change_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mob_change_fail_ies_o::get_crit));

    switch (id) {
      case 194: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_node1_cell_id.unpack(bref); }));
        break;
      }
      case 195: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_node2_cell_id.unpack(bref); }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 198: {
        mob_params_mod_range_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_params_mod_range.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 294: {
        ng_ran_node2_ssb_offsets_mod_range_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ng_ran_node2_ssb_offsets_mod_range, bref, 1, 64, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mob_change_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 194);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ng_ran_node1_cell_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 195);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ng_ran_node2_cell_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (mob_params_mod_range_present) {
    j.start_obj();
    j.write_int("id", 198);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_params_mod_range.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (ng_ran_node2_ssb_offsets_mod_range_present) {
    j.start_obj();
    j.write_int("id", 294);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ng_ran_node2_ssb_offsets_mod_range) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// MobilityChangeRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t mob_change_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {194, 195, 196, 197, 7, 280};
  return map_enum_number(names, 6, idx, "id");
}
bool mob_change_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {194, 195, 196, 197, 7, 280};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mob_change_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 194:
      return crit_e::reject;
    case 195:
      return crit_e::reject;
    case 196:
      return crit_e::ignore;
    case 197:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 280:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mob_change_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 194:
      return presence_e::mandatory;
    case 195:
      return presence_e::mandatory;
    case 196:
      return presence_e::optional;
    case 197:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 280:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mob_change_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += ng_ran_node1_mob_params_present ? 1 : 0;
  nof_ies += ssb_offsets_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 194, crit_e{crit_e::reject}, [this, &bref]() { return ng_ran_node1_cell_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 195, crit_e{crit_e::reject}, [this, &bref]() { return ng_ran_node2_cell_id.pack(bref); }));
  if (ng_ran_node1_mob_params_present) {
    HANDLE_CODE(
        pack_ie(bref, 196, crit_e{crit_e::ignore}, [this, &bref]() { return ng_ran_node1_mob_params.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(
      bref, 197, crit_e{crit_e::reject}, [this, &bref]() { return ng_ran_node2_proposed_mob_params.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (ssb_offsets_list_present) {
    HANDLE_CODE(pack_ie(bref, 280, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ssb_offsets_list, 1, 64, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mob_change_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mob_change_request_ies_o::get_crit));

    switch (id) {
      case 194: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_node1_cell_id.unpack(bref); }));
        break;
      }
      case 195: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_node2_cell_id.unpack(bref); }));
        break;
      }
      case 196: {
        ng_ran_node1_mob_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_node1_mob_params.unpack(bref); }));
        break;
      }
      case 197: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ng_ran_node2_proposed_mob_params.unpack(bref); }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 280: {
        ssb_offsets_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(ssb_offsets_list, bref, 1, 64, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mob_change_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 194);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ng_ran_node1_cell_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 195);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ng_ran_node2_cell_id.to_json(j);
  j.end_obj();
  if (ng_ran_node1_mob_params_present) {
    j.start_obj();
    j.write_int("id", 196);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ng_ran_node1_mob_params.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 197);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ng_ran_node2_proposed_mob_params.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (ssb_offsets_list_present) {
    j.start_obj();
    j.write_int("id", 280);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ssb_offsets_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// NGRANNodeConfigurationUpdate-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ngran_node_cfg_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {75, 9, 99, 101, 100, 14, 118, 119, 130, 141, 282, 341, 342, 343};
  return map_enum_number(names, 14, idx, "id");
}
bool ngran_node_cfg_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {75, 9, 99, 101, 100, 14, 118, 119, 130, 141, 282, 341, 342, 343};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ngran_node_cfg_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 75:
      return crit_e::reject;
    case 9:
      return crit_e::ignore;
    case 99:
      return crit_e::ignore;
    case 101:
      return crit_e::ignore;
    case 100:
      return crit_e::ignore;
    case 14:
      return crit_e::reject;
    case 118:
      return crit_e::reject;
    case 119:
      return crit_e::reject;
    case 130:
      return crit_e::reject;
    case 141:
      return crit_e::ignore;
    case 282:
      return crit_e::reject;
    case 341:
      return crit_e::ignore;
    case 342:
      return crit_e::ignore;
    case 343:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ngran_node_cfg_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 75:
      return presence_e::optional;
    case 9:
      return presence_e::mandatory;
    case 99:
      return presence_e::optional;
    case 101:
      return presence_e::optional;
    case 100:
      return presence_e::optional;
    case 14:
      return presence_e::optional;
    case 118:
      return presence_e::optional;
    case 119:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    case 141:
      return presence_e::optional;
    case 282:
      return presence_e::optional;
    case 341:
      return presence_e::optional;
    case 342:
      return presence_e::optional;
    case 343:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ngran_node_cfg_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += tai_support_list_present ? 1 : 0;
  nof_ies += tnl_a_to_add_list_present ? 1 : 0;
  nof_ies += tnl_a_to_rem_list_present ? 1 : 0;
  nof_ies += tnl_a_to_upd_list_present ? 1 : 0;
  nof_ies += global_ng_ran_node_id_present ? 1 : 0;
  nof_ies += amf_region_info_to_add_present ? 1 : 0;
  nof_ies += amf_region_info_to_delete_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  nof_ies += tnl_cfg_info_present ? 1 : 0;
  nof_ies += coverage_mod_list_present ? 1 : 0;
  nof_ies += local_ng_ran_node_id_present ? 1 : 0;
  nof_ies += neighbour_ng_ran_node_list_present ? 1 : 0;
  nof_ies += local_ng_ran_node_id_removal_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (tai_support_list_present) {
    HANDLE_CODE(pack_ie(bref, 75, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, tai_support_list, 1, 256, true);
    }));
  }
  HANDLE_CODE(
      pack_ie(bref, 9, crit_e{crit_e::ignore}, [this, &bref]() { return cfg_upd_init_node_choice.pack(bref); }));
  if (tnl_a_to_add_list_present) {
    HANDLE_CODE(pack_ie(bref, 99, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, tnl_a_to_add_list, 1, 32, true);
    }));
  }
  if (tnl_a_to_rem_list_present) {
    HANDLE_CODE(pack_ie(bref, 101, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, tnl_a_to_rem_list, 1, 32, true);
    }));
  }
  if (tnl_a_to_upd_list_present) {
    HANDLE_CODE(pack_ie(bref, 100, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, tnl_a_to_upd_list, 1, 32, true);
    }));
  }
  if (global_ng_ran_node_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 14, crit_e{crit_e::reject}, [this, &bref]() { return global_ng_ran_node_id.pack(bref); }));
  }
  if (amf_region_info_to_add_present) {
    HANDLE_CODE(pack_ie(bref, 118, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, amf_region_info_to_add, 1, 16, true);
    }));
  }
  if (amf_region_info_to_delete_present) {
    HANDLE_CODE(pack_ie(bref, 119, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, amf_region_info_to_delete, 1, 16, true);
    }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }
  if (tnl_cfg_info_present) {
    HANDLE_CODE(pack_ie(bref, 141, crit_e{crit_e::ignore}, [this, &bref]() { return tnl_cfg_info.pack(bref); }));
  }
  if (coverage_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 282, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, coverage_mod_list, 0, 16384, true);
    }));
  }
  if (local_ng_ran_node_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 341, crit_e{crit_e::ignore}, [this, &bref]() { return local_ng_ran_node_id.pack(bref); }));
  }
  if (neighbour_ng_ran_node_list_present) {
    HANDLE_CODE(pack_ie(bref, 342, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, neighbour_ng_ran_node_list, 0, 256, true);
    }));
  }
  if (local_ng_ran_node_id_removal_present) {
    HANDLE_CODE(pack_ie(
        bref, 343, crit_e{crit_e::ignore}, [this, &bref]() { return local_ng_ran_node_id_removal.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ngran_node_cfg_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ngran_node_cfg_upd_ies_o::get_crit));

    switch (id) {
      case 75: {
        tai_support_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(tai_support_list, bref, 1, 256, true); }));
        break;
      }
      case 9: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cfg_upd_init_node_choice.unpack(bref); }));
        break;
      }
      case 99: {
        tnl_a_to_add_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(tnl_a_to_add_list, bref, 1, 32, true); }));
        break;
      }
      case 101: {
        tnl_a_to_rem_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(tnl_a_to_rem_list, bref, 1, 32, true); }));
        break;
      }
      case 100: {
        tnl_a_to_upd_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(tnl_a_to_upd_list, bref, 1, 32, true); }));
        break;
      }
      case 14: {
        global_ng_ran_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 118: {
        amf_region_info_to_add_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(amf_region_info_to_add, bref, 1, 16, true); }));
        break;
      }
      case 119: {
        amf_region_info_to_delete_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(amf_region_info_to_delete, bref, 1, 16, true); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 141: {
        tnl_cfg_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return tnl_cfg_info.unpack(bref); }));
        break;
      }
      case 282: {
        coverage_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(coverage_mod_list, bref, 0, 16384, true); }));
        break;
      }
      case 341: {
        local_ng_ran_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return local_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 342: {
        neighbour_ng_ran_node_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(neighbour_ng_ran_node_list, bref, 0, 256, true); }));
        break;
      }
      case 343: {
        local_ng_ran_node_id_removal_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return local_ng_ran_node_id_removal.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ngran_node_cfg_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (tai_support_list_present) {
    j.start_obj();
    j.write_int("id", 75);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : tai_support_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 9);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cfg_upd_init_node_choice.to_json(j);
  j.end_obj();
  if (tnl_a_to_add_list_present) {
    j.start_obj();
    j.write_int("id", 99);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : tnl_a_to_add_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (tnl_a_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 101);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : tnl_a_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (tnl_a_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 100);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : tnl_a_to_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (global_ng_ran_node_id_present) {
    j.start_obj();
    j.write_int("id", 14);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    global_ng_ran_node_id.to_json(j);
    j.end_obj();
  }
  if (amf_region_info_to_add_present) {
    j.start_obj();
    j.write_int("id", 118);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : amf_region_info_to_add) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (amf_region_info_to_delete_present) {
    j.start_obj();
    j.write_int("id", 119);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : amf_region_info_to_delete) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  if (tnl_cfg_info_present) {
    j.start_obj();
    j.write_int("id", 141);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    tnl_cfg_info.to_json(j);
    j.end_obj();
  }
  if (coverage_mod_list_present) {
    j.start_obj();
    j.write_int("id", 282);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : coverage_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (local_ng_ran_node_id_present) {
    j.start_obj();
    j.write_int("id", 341);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    local_ng_ran_node_id.to_json(j);
    j.end_obj();
  }
  if (neighbour_ng_ran_node_list_present) {
    j.start_obj();
    j.write_int("id", 342);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : neighbour_ng_ran_node_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (local_ng_ran_node_id_removal_present) {
    j.start_obj();
    j.write_int("id", 343);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    local_ng_ran_node_id_removal.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RespondingNodeTypeConfigUpdateAck-ng-eNB-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
uint32_t responding_node_type_cfg_upd_ack_ng_enb_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {18, 156, 157};
  return map_enum_number(names, 3, idx, "id");
}
bool responding_node_type_cfg_upd_ack_ng_enb_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {18, 156, 157};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e responding_node_type_cfg_upd_ack_ng_enb_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 18:
      return crit_e::ignore;
    case 156:
      return crit_e::ignore;
    case 157:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e responding_node_type_cfg_upd_ack_ng_enb_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 18:
      return presence_e::optional;
    case 156:
      return presence_e::optional;
    case 157:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE responding_node_type_cfg_upd_ack_ng_enb_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += list_of_served_cells_e_utra_present ? 1 : 0;
  nof_ies += partial_list_ind_eutra_present ? 1 : 0;
  nof_ies += cell_and_capacity_assist_info_eutra_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (list_of_served_cells_e_utra_present) {
    HANDLE_CODE(pack_ie(bref, 18, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, list_of_served_cells_e_utra, 1, 16384, true);
    }));
  }
  if (partial_list_ind_eutra_present) {
    HANDLE_CODE(
        pack_ie(bref, 156, crit_e{crit_e::ignore}, [this, &bref]() { return partial_list_ind_eutra.pack(bref); }));
  }
  if (cell_and_capacity_assist_info_eutra_present) {
    HANDLE_CODE(pack_ie(
        bref, 157, crit_e{crit_e::ignore}, [this, &bref]() { return cell_and_capacity_assist_info_eutra.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE responding_node_type_cfg_upd_ack_ng_enb_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &responding_node_type_cfg_upd_ack_ng_enb_ext_ies_o::get_crit));

    switch (id) {
      case 18: {
        list_of_served_cells_e_utra_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(list_of_served_cells_e_utra, bref, 1, 16384, true); }));
        break;
      }
      case 156: {
        partial_list_ind_eutra_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return partial_list_ind_eutra.unpack(bref); }));
        break;
      }
      case 157: {
        cell_and_capacity_assist_info_eutra_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return cell_and_capacity_assist_info_eutra.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void responding_node_type_cfg_upd_ack_ng_enb_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (list_of_served_cells_e_utra_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : list_of_served_cells_e_utra) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (partial_list_ind_eutra_present) {
    j.start_obj();
    j.write_int("id", 156);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "partial");
    j.end_obj();
  }
  if (cell_and_capacity_assist_info_eutra_present) {
    j.start_obj();
    j.write_int("id", 157);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    cell_and_capacity_assist_info_eutra.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RespondingNodeTypeConfigUpdateAck-ng-eNB ::= SEQUENCE
OCUDUASN_CODE responding_node_type_cfg_upd_ack_ng_enb_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE responding_node_type_cfg_upd_ack_ng_enb_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void responding_node_type_cfg_upd_ack_ng_enb_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// RespondingNodeTypeConfigUpdateAck-gNB-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
uint32_t responding_node_type_cfg_upd_ack_gnb_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {142, 144};
  return map_enum_number(names, 2, idx, "id");
}
bool responding_node_type_cfg_upd_ack_gnb_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {142, 144};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e responding_node_type_cfg_upd_ack_gnb_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 142:
      return crit_e::ignore;
    case 144:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e responding_node_type_cfg_upd_ack_gnb_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 142:
      return presence_e::optional;
    case 144:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE responding_node_type_cfg_upd_ack_gnb_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += partial_list_ind_nr_present ? 1 : 0;
  nof_ies += cell_and_capacity_assist_info_nr_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (partial_list_ind_nr_present) {
    HANDLE_CODE(pack_ie(bref, 142, crit_e{crit_e::ignore}, [this, &bref]() { return partial_list_ind_nr.pack(bref); }));
  }
  if (cell_and_capacity_assist_info_nr_present) {
    HANDLE_CODE(pack_ie(
        bref, 144, crit_e{crit_e::ignore}, [this, &bref]() { return cell_and_capacity_assist_info_nr.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE responding_node_type_cfg_upd_ack_gnb_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &responding_node_type_cfg_upd_ack_gnb_ext_ies_o::get_crit));

    switch (id) {
      case 142: {
        partial_list_ind_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return partial_list_ind_nr.unpack(bref); }));
        break;
      }
      case 144: {
        cell_and_capacity_assist_info_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cell_and_capacity_assist_info_nr.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void responding_node_type_cfg_upd_ack_gnb_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (partial_list_ind_nr_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "partial");
    j.end_obj();
  }
  if (cell_and_capacity_assist_info_nr_present) {
    j.start_obj();
    j.write_int("id", 144);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    cell_and_capacity_assist_info_nr.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RespondingNodeTypeConfigUpdateAck-gNB ::= SEQUENCE
OCUDUASN_CODE responding_node_type_cfg_upd_ack_gnb_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(served_nr_cells.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (served_nr_cells.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, served_nr_cells, 1, 16384, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE responding_node_type_cfg_upd_ack_gnb_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool served_nr_cells_present;
  HANDLE_CODE(bref.unpack(served_nr_cells_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (served_nr_cells_present) {
    HANDLE_CODE(unpack_dyn_seq_of(served_nr_cells, bref, 1, 16384, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void responding_node_type_cfg_upd_ack_gnb_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (served_nr_cells.size() > 0) {
    j.start_array("served-NR-Cells");
    for (const auto& e1 : served_nr_cells) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// RespondingNodeTypeConfigUpdateAck ::= CHOICE
void responding_node_type_cfg_upd_ack_c::destroy_()
{
  switch (type_) {
    case types::ng_enb:
      c.destroy<responding_node_type_cfg_upd_ack_ng_enb_s>();
      break;
    case types::gnb:
      c.destroy<responding_node_type_cfg_upd_ack_gnb_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void responding_node_type_cfg_upd_ack_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ng_enb:
      c.init<responding_node_type_cfg_upd_ack_ng_enb_s>();
      break;
    case types::gnb:
      c.init<responding_node_type_cfg_upd_ack_gnb_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_cfg_upd_ack_c");
  }
}
responding_node_type_cfg_upd_ack_c::responding_node_type_cfg_upd_ack_c(const responding_node_type_cfg_upd_ack_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ng_enb:
      c.init(other.c.get<responding_node_type_cfg_upd_ack_ng_enb_s>());
      break;
    case types::gnb:
      c.init(other.c.get<responding_node_type_cfg_upd_ack_gnb_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_cfg_upd_ack_c");
  }
}
responding_node_type_cfg_upd_ack_c&
responding_node_type_cfg_upd_ack_c::operator=(const responding_node_type_cfg_upd_ack_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ng_enb:
      c.set(other.c.get<responding_node_type_cfg_upd_ack_ng_enb_s>());
      break;
    case types::gnb:
      c.set(other.c.get<responding_node_type_cfg_upd_ack_gnb_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_cfg_upd_ack_c");
  }

  return *this;
}
responding_node_type_cfg_upd_ack_ng_enb_s& responding_node_type_cfg_upd_ack_c::set_ng_enb()
{
  set(types::ng_enb);
  return c.get<responding_node_type_cfg_upd_ack_ng_enb_s>();
}
responding_node_type_cfg_upd_ack_gnb_s& responding_node_type_cfg_upd_ack_c::set_gnb()
{
  set(types::gnb);
  return c.get<responding_node_type_cfg_upd_ack_gnb_s>();
}
protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>&
responding_node_type_cfg_upd_ack_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>();
}
void responding_node_type_cfg_upd_ack_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ng_enb:
      j.write_fieldname("ng-eNB");
      c.get<responding_node_type_cfg_upd_ack_ng_enb_s>().to_json(j);
      break;
    case types::gnb:
      j.write_fieldname("gNB");
      c.get<responding_node_type_cfg_upd_ack_gnb_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_cfg_upd_ack_c");
  }
  j.end_obj();
}
OCUDUASN_CODE responding_node_type_cfg_upd_ack_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ng_enb:
      HANDLE_CODE(c.get<responding_node_type_cfg_upd_ack_ng_enb_s>().pack(bref));
      break;
    case types::gnb:
      HANDLE_CODE(c.get<responding_node_type_cfg_upd_ack_gnb_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_cfg_upd_ack_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE responding_node_type_cfg_upd_ack_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ng_enb:
      HANDLE_CODE(c.get<responding_node_type_cfg_upd_ack_ng_enb_s>().unpack(bref));
      break;
    case types::gnb:
      HANDLE_CODE(c.get<responding_node_type_cfg_upd_ack_gnb_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<responding_node_type_cfg_upd_ack_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "responding_node_type_cfg_upd_ack_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* responding_node_type_cfg_upd_ack_c::types_opts::to_string() const
{
  static const char* names[] = {"ng-eNB", "gNB", "choice-extension"};
  return convert_enum_idx(names, 3, value, "responding_node_type_cfg_upd_ack_c::types");
}

// NGRANNodeConfigurationUpdateAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ngran_node_cfg_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {58, 102, 103, 10, 130, 141, 341, 342, 343};
  return map_enum_number(names, 9, idx, "id");
}
bool ngran_node_cfg_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {58, 102, 103, 10, 130, 141, 341, 342, 343};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ngran_node_cfg_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 58:
      return crit_e::ignore;
    case 102:
      return crit_e::ignore;
    case 103:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    case 141:
      return crit_e::ignore;
    case 341:
      return crit_e::ignore;
    case 342:
      return crit_e::ignore;
    case 343:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ngran_node_cfg_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 58:
      return presence_e::mandatory;
    case 102:
      return presence_e::optional;
    case 103:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    case 141:
      return presence_e::optional;
    case 341:
      return presence_e::optional;
    case 342:
      return presence_e::optional;
    case 343:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ngran_node_cfg_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += tnl_a_setup_list_present ? 1 : 0;
  nof_ies += tnl_a_failed_to_setup_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  nof_ies += tnl_cfg_info_present ? 1 : 0;
  nof_ies += local_ng_ran_node_id_present ? 1 : 0;
  nof_ies += neighbour_ng_ran_node_list_present ? 1 : 0;
  nof_ies += local_ng_ran_node_id_removal_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(
      bref, 58, crit_e{crit_e::ignore}, [this, &bref]() { return responding_node_type_cfg_upd_ack.pack(bref); }));
  if (tnl_a_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 102, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, tnl_a_setup_list, 1, 32, true);
    }));
  }
  if (tnl_a_failed_to_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 103, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, tnl_a_failed_to_setup_list, 1, 32, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }
  if (tnl_cfg_info_present) {
    HANDLE_CODE(pack_ie(bref, 141, crit_e{crit_e::ignore}, [this, &bref]() { return tnl_cfg_info.pack(bref); }));
  }
  if (local_ng_ran_node_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 341, crit_e{crit_e::ignore}, [this, &bref]() { return local_ng_ran_node_id.pack(bref); }));
  }
  if (neighbour_ng_ran_node_list_present) {
    HANDLE_CODE(pack_ie(bref, 342, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, neighbour_ng_ran_node_list, 0, 256, true);
    }));
  }
  if (local_ng_ran_node_id_removal_present) {
    HANDLE_CODE(pack_ie(
        bref, 343, crit_e{crit_e::ignore}, [this, &bref]() { return local_ng_ran_node_id_removal.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ngran_node_cfg_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ngran_node_cfg_upd_ack_ies_o::get_crit));

    switch (id) {
      case 58: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return responding_node_type_cfg_upd_ack.unpack(bref); }));
        break;
      }
      case 102: {
        tnl_a_setup_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(tnl_a_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 103: {
        tnl_a_failed_to_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(tnl_a_failed_to_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 141: {
        tnl_cfg_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return tnl_cfg_info.unpack(bref); }));
        break;
      }
      case 341: {
        local_ng_ran_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return local_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 342: {
        neighbour_ng_ran_node_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(neighbour_ng_ran_node_list, bref, 0, 256, true); }));
        break;
      }
      case 343: {
        local_ng_ran_node_id_removal_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return local_ng_ran_node_id_removal.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ngran_node_cfg_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 58);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  responding_node_type_cfg_upd_ack.to_json(j);
  j.end_obj();
  if (tnl_a_setup_list_present) {
    j.start_obj();
    j.write_int("id", 102);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : tnl_a_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (tnl_a_failed_to_setup_list_present) {
    j.start_obj();
    j.write_int("id", 103);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : tnl_a_failed_to_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  if (tnl_cfg_info_present) {
    j.start_obj();
    j.write_int("id", 141);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    tnl_cfg_info.to_json(j);
    j.end_obj();
  }
  if (local_ng_ran_node_id_present) {
    j.start_obj();
    j.write_int("id", 341);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    local_ng_ran_node_id.to_json(j);
    j.end_obj();
  }
  if (neighbour_ng_ran_node_list_present) {
    j.start_obj();
    j.write_int("id", 342);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : neighbour_ng_ran_node_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (local_ng_ran_node_id_removal_present) {
    j.start_obj();
    j.write_int("id", 343);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    local_ng_ran_node_id_removal.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// NGRANNodeConfigurationUpdateFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ngran_node_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {7, 76, 10, 130};
  return map_enum_number(names, 4, idx, "id");
}
bool ngran_node_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {7, 76, 10, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ngran_node_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 7:
      return crit_e::ignore;
    case 76:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ngran_node_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 7:
      return presence_e::mandatory;
    case 76:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ngran_node_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 76, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ngran_node_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ngran_node_cfg_upd_fail_ies_o::get_crit));

    switch (id) {
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 76: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ngran_node_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 76);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionResourcesNotify-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_notify_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, qos_flows_notif_contr_ind_info, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_notify_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(qos_flows_notif_contr_ind_info, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_notify_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  j.start_array("qosFlowsNotificationContrIndInfo");
  for (const auto& e1 : qos_flows_notif_contr_ind_info) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NotificationControlIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t notif_ctrl_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 44};
  return map_enum_number(names, 3, idx, "id");
}
bool notif_ctrl_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 44};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e notif_ctrl_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 44:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e notif_ctrl_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 44:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE notif_ctrl_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_res_notify_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_notify_list_present) {
    HANDLE_CODE(pack_ie(bref, 44, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_notify_list, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE notif_ctrl_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &notif_ctrl_ind_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 44: {
        pdu_session_res_notify_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_notify_list, bref, 1, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void notif_ctrl_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (pdu_session_res_notify_list_present) {
    j.start_obj();
    j.write_int("id", 44);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_notify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PDUSession-SNChangeConfirm-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
uint32_t pdu_session_sn_change_confirm_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {369};
  return map_enum_number(names, 1, idx, "id");
}
bool pdu_session_sn_change_confirm_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 369 == id;
}
crit_e pdu_session_sn_change_confirm_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 369) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e pdu_session_sn_change_confirm_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 369) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE pdu_session_sn_change_confirm_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += add_listof_pdu_session_res_change_confirm_info_sn_terminated_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (add_listof_pdu_session_res_change_confirm_info_sn_terminated_present) {
    HANDLE_CODE(pack_ie(bref, 369, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, add_listof_pdu_session_res_change_confirm_info_sn_terminated, 1, 7, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_sn_change_confirm_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_sn_change_confirm_item_ext_ies_o::get_crit));

    if (id == 369) {
      {
        add_listof_pdu_session_res_change_confirm_info_sn_terminated_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(add_listof_pdu_session_res_change_confirm_info_sn_terminated, bref, 1, 7, true);
        }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_sn_change_confirm_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (add_listof_pdu_session_res_change_confirm_info_sn_terminated_present) {
    j.start_obj();
    j.write_int("id", 369);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : add_listof_pdu_session_res_change_confirm_info_sn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PDUSession-SNChangeConfirm-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_sn_change_confirm_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_sn_change_confirm_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_sn_change_confirm_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSession-SNChangeRequired-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_sn_change_required_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_sn_change_required_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_sn_change_required_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionAdmittedToBeAddedSNModResponse-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_admitted_to_be_added_sn_mod_resp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_admitted_to_be_added_sn_mod_resp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_admitted_to_be_added_sn_mod_resp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionAdmittedToBeModifiedSNModResponse-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_admitted_to_be_modified_sn_mod_resp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_admitted_to_be_modified_sn_mod_resp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_admitted_to_be_modified_sn_mod_resp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionAdmittedToBeReleasedSNModResponse ::= SEQUENCE
OCUDUASN_CODE pdu_session_admitted_to_be_released_sn_mod_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(mn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (sn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sn_terminated, 1, 256, true));
  }
  if (mn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mn_terminated, 1, 256, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_admitted_to_be_released_sn_mod_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool sn_terminated_present;
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  bool mn_terminated_present;
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (sn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sn_terminated, bref, 1, 256, true));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mn_terminated, bref, 1, 256, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_admitted_to_be_released_sn_mod_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sn_terminated.size() > 0) {
    j.start_array("sn-terminated");
    for (const auto& e1 : sn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mn_terminated.size() > 0) {
    j.start_array("mn-terminated");
    for (const auto& e1 : mn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionAdmitted-SNModResponse ::= SEQUENCE
OCUDUASN_CODE pdu_session_admitted_sn_mod_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_session_res_admitted_to_be_added.size() > 0, 1));
  HANDLE_CODE(bref.pack(pdu_session_res_admitted_to_be_modified.size() > 0, 1));
  HANDLE_CODE(bref.pack(pdu_session_res_admitted_to_be_released_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (pdu_session_res_admitted_to_be_added.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_res_admitted_to_be_added, 1, 256, true));
  }
  if (pdu_session_res_admitted_to_be_modified.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_res_admitted_to_be_modified, 1, 256, true));
  }
  if (pdu_session_res_admitted_to_be_released_present) {
    HANDLE_CODE(pdu_session_res_admitted_to_be_released.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_admitted_sn_mod_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool pdu_session_res_admitted_to_be_added_present;
  HANDLE_CODE(bref.unpack(pdu_session_res_admitted_to_be_added_present, 1));
  bool pdu_session_res_admitted_to_be_modified_present;
  HANDLE_CODE(bref.unpack(pdu_session_res_admitted_to_be_modified_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_res_admitted_to_be_released_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (pdu_session_res_admitted_to_be_added_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_session_res_admitted_to_be_added, bref, 1, 256, true));
  }
  if (pdu_session_res_admitted_to_be_modified_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_session_res_admitted_to_be_modified, bref, 1, 256, true));
  }
  if (pdu_session_res_admitted_to_be_released_present) {
    HANDLE_CODE(pdu_session_res_admitted_to_be_released.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_admitted_sn_mod_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdu_session_res_admitted_to_be_added.size() > 0) {
    j.start_array("pduSessionResourcesAdmittedToBeAdded");
    for (const auto& e1 : pdu_session_res_admitted_to_be_added) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdu_session_res_admitted_to_be_modified.size() > 0) {
    j.start_array("pduSessionResourcesAdmittedToBeModified");
    for (const auto& e1 : pdu_session_res_admitted_to_be_modified) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdu_session_res_admitted_to_be_released_present) {
    j.write_fieldname("pduSessionResourcesAdmittedToBeReleased");
    pdu_session_res_admitted_to_be_released.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionAdmittedAddedAddReqAck-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_admitted_added_add_req_ack_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_admitted_added_add_req_ack_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_admitted_added_add_req_ack_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionAdmittedModSNModConfirm-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_admitted_mod_sn_mod_confirm_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_admitted_mod_sn_mod_confirm_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_admitted_mod_sn_mod_confirm_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionDataForwarding-SNModResponse ::= SEQUENCE
OCUDUASN_CODE pdu_session_data_forwarding_sn_mod_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, sn_terminated, 1, 256, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_data_forwarding_sn_mod_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(sn_terminated, bref, 1, 256, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_data_forwarding_sn_mod_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("sn-terminated");
  for (const auto& e1 : sn_terminated) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDUSessionNotAdmitted-SNModResponse-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
uint32_t pdu_session_not_admitted_sn_mod_resp_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {43};
  return map_enum_number(names, 1, idx, "id");
}
bool pdu_session_not_admitted_sn_mod_resp_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 43 == id;
}
crit_e pdu_session_not_admitted_sn_mod_resp_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 43) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e pdu_session_not_admitted_sn_mod_resp_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 43) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE pdu_session_not_admitted_sn_mod_resp_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pdu_session_res_not_admitted_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (pdu_session_res_not_admitted_list_present) {
    HANDLE_CODE(pack_ie(bref, 43, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_not_admitted_list, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_not_admitted_sn_mod_resp_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_not_admitted_sn_mod_resp_ext_ies_o::get_crit));

    if (id == 43) {
      {
        pdu_session_res_not_admitted_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_not_admitted_list, bref, 1, 256, true);
        }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_not_admitted_sn_mod_resp_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (pdu_session_res_not_admitted_list_present) {
    j.start_obj();
    j.write_int("id", 43);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : pdu_session_res_not_admitted_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionNotAdmitted-SNModResponse ::= SEQUENCE
OCUDUASN_CODE pdu_session_not_admitted_sn_mod_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_session_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (pdu_session_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_list, 1, 256, integer_packer<uint16_t>(0, 255, false, true), true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_not_admitted_sn_mod_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool pdu_session_list_present;
  HANDLE_CODE(bref.unpack(pdu_session_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (pdu_session_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_session_list, bref, 1, 256, integer_packer<uint16_t>(0, 255, false, true), true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_not_admitted_sn_mod_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdu_session_list.size() > 0) {
    j.start_array("pdu-Session-List");
    for (const auto& e1 : pdu_session_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionNotAdmittedAddReqAck ::= SEQUENCE
OCUDUASN_CODE pdu_session_not_admitted_add_req_ack_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_session_res_not_admitted_sn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(pdu_session_res_not_admitted_m_nterminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (pdu_session_res_not_admitted_sn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_res_not_admitted_sn_terminated, 1, 256, true));
  }
  if (pdu_session_res_not_admitted_m_nterminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_res_not_admitted_m_nterminated, 1, 256, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_not_admitted_add_req_ack_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool pdu_session_res_not_admitted_sn_terminated_present;
  HANDLE_CODE(bref.unpack(pdu_session_res_not_admitted_sn_terminated_present, 1));
  bool pdu_session_res_not_admitted_m_nterminated_present;
  HANDLE_CODE(bref.unpack(pdu_session_res_not_admitted_m_nterminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (pdu_session_res_not_admitted_sn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_session_res_not_admitted_sn_terminated, bref, 1, 256, true));
  }
  if (pdu_session_res_not_admitted_m_nterminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_session_res_not_admitted_m_nterminated, bref, 1, 256, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_not_admitted_add_req_ack_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdu_session_res_not_admitted_sn_terminated.size() > 0) {
    j.start_array("pduSessionResourcesNotAdmitted-SNterminated");
    for (const auto& e1 : pdu_session_res_not_admitted_sn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdu_session_res_not_admitted_m_nterminated.size() > 0) {
    j.start_array("pduSessionResourcesNotAdmitted-MNterminated");
    for (const auto& e1 : pdu_session_res_not_admitted_m_nterminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionReleasedList-RelConf ::= SEQUENCE
OCUDUASN_CODE pdu_session_released_list_rel_conf_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_sessions_released_list_sn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (pdu_sessions_released_list_sn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_sessions_released_list_sn_terminated, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_released_list_rel_conf_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool pdu_sessions_released_list_sn_terminated_present;
  HANDLE_CODE(bref.unpack(pdu_sessions_released_list_sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (pdu_sessions_released_list_sn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_sessions_released_list_sn_terminated, bref, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_released_list_rel_conf_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdu_sessions_released_list_sn_terminated.size() > 0) {
    j.start_array("pduSessionsReleasedList-SNterminated");
    for (const auto& e1 : pdu_sessions_released_list_sn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDUSessionReleasedSNModConfirm ::= SEQUENCE
OCUDUASN_CODE pdu_session_released_sn_mod_confirm_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(mn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (sn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sn_terminated, 1, 256, true));
  }
  if (mn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mn_terminated, 1, 256, integer_packer<uint16_t>(0, 255, false, true), true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_released_sn_mod_confirm_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool sn_terminated_present;
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  bool mn_terminated_present;
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (sn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sn_terminated, bref, 1, 256, true));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mn_terminated, bref, 1, 256, integer_packer<uint16_t>(0, 255, false, true), true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_released_sn_mod_confirm_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sn_terminated.size() > 0) {
    j.start_array("sn-terminated");
    for (const auto& e1 : sn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mn_terminated.size() > 0) {
    j.start_array("mn-terminated");
    for (const auto& e1 : mn_terminated) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionToBeAddedAddReq-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_to_be_added_add_req_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_pdu_session_ambr_present, 1));
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(s_nssai.pack(bref));
  if (sn_pdu_session_ambr_present) {
    HANDLE_CODE(sn_pdu_session_ambr.pack(bref));
  }
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_to_be_added_add_req_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_pdu_session_ambr_present, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(s_nssai.unpack(bref));
  if (sn_pdu_session_ambr_present) {
    HANDLE_CODE(sn_pdu_session_ambr.unpack(bref));
  }
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_to_be_added_add_req_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  j.write_fieldname("s-NSSAI");
  s_nssai.to_json(j);
  if (sn_pdu_session_ambr_present) {
    j.write_fieldname("sN-PDUSessionAMBR");
    sn_pdu_session_ambr.to_json(j);
  }
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionToBeModifiedSNModRequired-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_to_be_modified_sn_mod_required_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_to_be_modified_sn_mod_required_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_to_be_modified_sn_mod_required_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionToBeReleasedList-RelReqAck ::= SEQUENCE
OCUDUASN_CODE pdu_session_to_be_released_list_rel_req_ack_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_sessions_to_be_released_list_sn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (pdu_sessions_to_be_released_list_sn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_sessions_to_be_released_list_sn_terminated, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_to_be_released_list_rel_req_ack_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool pdu_sessions_to_be_released_list_sn_terminated_present;
  HANDLE_CODE(bref.unpack(pdu_sessions_to_be_released_list_sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (pdu_sessions_to_be_released_list_sn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_sessions_to_be_released_list_sn_terminated, bref, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_to_be_released_list_rel_req_ack_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdu_sessions_to_be_released_list_sn_terminated.size() > 0) {
    j.start_array("pduSessionsToBeReleasedList-SNterminated");
    for (const auto& e1 : pdu_sessions_to_be_released_list_sn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDUSessionToBeReleasedList-RelRqd ::= SEQUENCE
OCUDUASN_CODE pdu_session_to_be_released_list_rel_rqd_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_sessions_to_be_released_list_sn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (pdu_sessions_to_be_released_list_sn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_sessions_to_be_released_list_sn_terminated, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_to_be_released_list_rel_rqd_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool pdu_sessions_to_be_released_list_sn_terminated_present;
  HANDLE_CODE(bref.unpack(pdu_sessions_to_be_released_list_sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (pdu_sessions_to_be_released_list_sn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_sessions_to_be_released_list_sn_terminated, bref, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_to_be_released_list_rel_rqd_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdu_sessions_to_be_released_list_sn_terminated.size() > 0) {
    j.start_array("pduSessionsToBeReleasedList-SNterminated");
    for (const auto& e1 : pdu_sessions_to_be_released_list_sn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDUSessionToBeReleasedSNModRequired ::= SEQUENCE
OCUDUASN_CODE pdu_session_to_be_released_sn_mod_required_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(mn_terminated.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (sn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sn_terminated, 1, 256, true));
  }
  if (mn_terminated.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mn_terminated, 1, 256, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_to_be_released_sn_mod_required_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool sn_terminated_present;
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  bool mn_terminated_present;
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (sn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sn_terminated, bref, 1, 256, true));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mn_terminated, bref, 1, 256, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_to_be_released_sn_mod_required_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sn_terminated.size() > 0) {
    j.start_array("sn-terminated");
    for (const auto& e1 : sn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mn_terminated.size() > 0) {
    j.start_array("mn-terminated");
    for (const auto& e1 : mn_terminated) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionsToBeAdded-SNModRequest-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
uint32_t pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {249};
  return map_enum_number(names, 1, idx, "id");
}
bool pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 249 == id;
}
crit_e pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 249) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 249) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pdu_session_expected_ue_activity_behaviour_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (pdu_session_expected_ue_activity_behaviour_present) {
    HANDLE_CODE(pack_ie(bref, 249, crit_e{crit_e::ignore}, [this, &bref]() {
      return pdu_session_expected_ue_activity_behaviour.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_o::get_crit));

    if (id == 249) {
      {
        pdu_session_expected_ue_activity_behaviour_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_expected_ue_activity_behaviour.unpack(bref); }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_sessions_to_be_added_sn_mod_request_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (pdu_session_expected_ue_activity_behaviour_present) {
    j.start_obj();
    j.write_int("id", 249);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    pdu_session_expected_ue_activity_behaviour.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionsToBeAdded-SNModRequest-Item ::= SEQUENCE
OCUDUASN_CODE pdu_sessions_to_be_added_sn_mod_request_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_pdu_session_ambr_present, 1));
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(s_nssai.pack(bref));
  if (sn_pdu_session_ambr_present) {
    HANDLE_CODE(sn_pdu_session_ambr.pack(bref));
  }
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_sessions_to_be_added_sn_mod_request_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_pdu_session_ambr_present, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(s_nssai.unpack(bref));
  if (sn_pdu_session_ambr_present) {
    HANDLE_CODE(sn_pdu_session_ambr.unpack(bref));
  }
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_sessions_to_be_added_sn_mod_request_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  j.write_fieldname("s-NSSAI");
  s_nssai.to_json(j);
  if (sn_pdu_session_ambr_present) {
    j.write_fieldname("sN-PDUSessionAMBR");
    sn_pdu_session_ambr.to_json(j);
  }
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionsToBeModified-SNModRequest-Item-ExtIEs ::= OBJECT SET OF XNAP-PROTOCOL-EXTENSION
uint32_t pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {116, 249, 469};
  return map_enum_number(names, 3, idx, "id");
}
bool pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {116, 249, 469};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 116:
      return crit_e::reject;
    case 249:
      return crit_e::ignore;
    case 469:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 116:
      return presence_e::optional;
    case 249:
      return presence_e::optional;
    case 469:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += s_nssai_present ? 1 : 0;
  nof_ies += pdu_session_expected_ue_activity_behaviour_present ? 1 : 0;
  nof_ies += user_plane_fail_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (s_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 116, crit_e{crit_e::reject}, [this, &bref]() { return s_nssai.pack(bref); }));
  }
  if (pdu_session_expected_ue_activity_behaviour_present) {
    HANDLE_CODE(pack_ie(bref, 249, crit_e{crit_e::ignore}, [this, &bref]() {
      return pdu_session_expected_ue_activity_behaviour.pack(bref);
    }));
  }
  if (user_plane_fail_ind_present) {
    HANDLE_CODE(pack_ie(bref, 469, crit_e{crit_e::ignore}, [this, &bref]() { return user_plane_fail_ind.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_o::get_crit));

    switch (id) {
      case 116: {
        s_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_nssai.unpack(bref); }));
        break;
      }
      case 249: {
        pdu_session_expected_ue_activity_behaviour_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_expected_ue_activity_behaviour.unpack(bref); }));
        break;
      }
      case 469: {
        user_plane_fail_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_plane_fail_ind.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_sessions_to_be_modified_sn_mod_request_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (s_nssai_present) {
    j.start_obj();
    j.write_int("id", 116);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    s_nssai.to_json(j);
    j.end_obj();
  }
  if (pdu_session_expected_ue_activity_behaviour_present) {
    j.start_obj();
    j.write_int("id", 249);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    pdu_session_expected_ue_activity_behaviour.to_json(j);
    j.end_obj();
  }
  if (user_plane_fail_ind_present) {
    j.start_obj();
    j.write_int("id", 469);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    user_plane_fail_ind.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionsToBeModified-SNModRequest-Item ::= SEQUENCE
OCUDUASN_CODE pdu_sessions_to_be_modified_sn_mod_request_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sn_pdu_session_ambr_present, 1));
  HANDLE_CODE(bref.pack(sn_terminated_present, 1));
  HANDLE_CODE(bref.pack(mn_terminated_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_pdu_session_ambr_present) {
    HANDLE_CODE(sn_pdu_session_ambr.pack(bref));
  }
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.pack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_sessions_to_be_modified_sn_mod_request_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sn_pdu_session_ambr_present, 1));
  HANDLE_CODE(bref.unpack(sn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(mn_terminated_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (sn_pdu_session_ambr_present) {
    HANDLE_CODE(sn_pdu_session_ambr.unpack(bref));
  }
  if (sn_terminated_present) {
    HANDLE_CODE(sn_terminated.unpack(bref));
  }
  if (mn_terminated_present) {
    HANDLE_CODE(mn_terminated.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_sessions_to_be_modified_sn_mod_request_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pduSessionId", pdu_session_id);
  if (sn_pdu_session_ambr_present) {
    j.write_fieldname("sN-PDUSessionAMBR");
    sn_pdu_session_ambr.to_json(j);
  }
  if (sn_terminated_present) {
    j.write_fieldname("sn-terminated");
    sn_terminated.to_json(j);
  }
  if (mn_terminated_present) {
    j.write_fieldname("mn-terminated");
    mn_terminated.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDUSessionsToBeReleased-SNModRequest-List ::= SEQUENCE
OCUDUASN_CODE pdu_sessions_to_be_released_sn_mod_request_list_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_session_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (pdu_session_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_list, 1, 256, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_sessions_to_be_released_sn_mod_request_list_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool pdu_session_list_present;
  HANDLE_CODE(bref.unpack(pdu_session_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (pdu_session_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_session_list, bref, 1, 256, true));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_sessions_to_be_released_sn_mod_request_list_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdu_session_list.size() > 0) {
    j.start_array("pdu-session-list");
    for (const auto& e1 : pdu_session_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PartialUEContextTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t partial_ue_context_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {27, 29, 354, 380};
  return map_enum_number(names, 4, idx, "id");
}
bool partial_ue_context_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {27, 29, 354, 380};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e partial_ue_context_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 27:
      return crit_e::reject;
    case 29:
      return crit_e::ignore;
    case 354:
      return crit_e::ignore;
    case 380:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e partial_ue_context_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 27:
      return presence_e::mandatory;
    case 29:
      return presence_e::mandatory;
    case 354:
      return presence_e::mandatory;
    case 380:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE partial_ue_context_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pos_partial_ue_context_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, old_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 354, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_partial_ue_context_info.pack(bref); }));
  if (pos_partial_ue_context_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 380, crit_e{crit_e::ignore}, [this, &bref]() { return pos_partial_ue_context_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE partial_ue_context_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &partial_ue_context_transfer_ies_o::get_crit));

    switch (id) {
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 354: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_partial_ue_context_info.unpack(bref); }));
        break;
      }
      case 380: {
        pos_partial_ue_context_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pos_partial_ue_context_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void partial_ue_context_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "reject");
  j.write_uint("value", new_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "ignore");
  j.write_uint("value", old_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 354);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  sdt_partial_ue_context_info.to_json(j);
  j.end_obj();
  if (pos_partial_ue_context_info_present) {
    j.start_obj();
    j.write_int("id", 380);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pos_partial_ue_context_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PartialUEContextTransferAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t partial_ue_context_transfer_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {27, 29, 355, 10, 381};
  return map_enum_number(names, 5, idx, "id");
}
bool partial_ue_context_transfer_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {27, 29, 355, 10, 381};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e partial_ue_context_transfer_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 27:
      return crit_e::ignore;
    case 29:
      return crit_e::ignore;
    case 355:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 381:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e partial_ue_context_transfer_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 27:
      return presence_e::mandatory;
    case 29:
      return presence_e::mandatory;
    case 355:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 381:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE partial_ue_context_transfer_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += sdt_data_forwarding_drb_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += srs_cfg_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, old_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (sdt_data_forwarding_drb_list_present) {
    HANDLE_CODE(pack_ie(bref, 355, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, sdt_data_forwarding_drb_list, 1, 32, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (srs_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 381, crit_e{crit_e::ignore}, [this, &bref]() { return srs_cfg.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE partial_ue_context_transfer_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &partial_ue_context_transfer_ack_ies_o::get_crit));

    switch (id) {
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 355: {
        sdt_data_forwarding_drb_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(sdt_data_forwarding_drb_list, bref, 1, 32, true); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 381: {
        srs_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_cfg.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void partial_ue_context_transfer_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "ignore");
  j.write_uint("value", new_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "ignore");
  j.write_uint("value", old_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (sdt_data_forwarding_drb_list_present) {
    j.start_obj();
    j.write_int("id", 355);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : sdt_data_forwarding_drb_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 381);
    j.write_str("criticality", "ignore");
    j.write_str("value", srs_cfg.to_string());
    j.end_obj();
  }
  j.end_array();
}

// PartialUEContextTransferFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t partial_ue_context_transfer_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {27, 29, 7, 10};
  return map_enum_number(names, 4, idx, "id");
}
bool partial_ue_context_transfer_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {27, 29, 7, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e partial_ue_context_transfer_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 27:
      return crit_e::ignore;
    case 29:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e partial_ue_context_transfer_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 27:
      return presence_e::mandatory;
    case 29:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE partial_ue_context_transfer_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, old_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE partial_ue_context_transfer_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &partial_ue_context_transfer_fail_ies_o::get_crit));

    switch (id) {
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void partial_ue_context_transfer_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "ignore");
  j.write_uint("value", new_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "ignore");
  j.write_uint("value", old_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// Value ::= OPEN TYPE
void xnap_private_ies_empty_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.end_obj();
}
OCUDUASN_CODE xnap_private_ies_empty_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE xnap_private_ies_empty_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}

const char* xnap_private_ies_empty_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {};
  return convert_enum_idx(names, 0, value, "xnap_private_ies_empty_o::value_c::types");
}

OCUDUASN_CODE private_ie_container_empty_l::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE private_ie_container_empty_l::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));
  if (nof_ies > 0) {
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void private_ie_container_empty_l::to_json(json_writer& j) const
{
  j.start_array();
  j.end_array();
}

// PrivateMessage ::= SEQUENCE
OCUDUASN_CODE private_msg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(private_ies.pack(bref));

  bref.align_bytes_zero();

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE private_msg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(private_ies.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  bref.align_bytes();

  return OCUDUASN_SUCCESS;
}
void private_msg_s::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.start_obj("PrivateMessage");
  j.write_fieldname("privateIEs");
  private_ies.to_json(j);
  j.end_obj();
  j.end_obj();
  j.end_array();
}

// QoE-Measurement-Results ::= SEQUENCE
OCUDUASN_CODE qo_e_meas_results_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(rrc_container_for_rv_qo_e_report.size() > 0, 1));
  HANDLE_CODE(bref.pack(rrc_container_for_qo_e_report.size() > 0, 1));
  HANDLE_CODE(bref.pack(app_layer_session_status_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(qoe_ref.pack(bref));
  if (rrc_container_for_rv_qo_e_report.size() > 0) {
    HANDLE_CODE(rrc_container_for_rv_qo_e_report.pack(bref));
  }
  if (rrc_container_for_qo_e_report.size() > 0) {
    HANDLE_CODE(rrc_container_for_qo_e_report.pack(bref));
  }
  if (app_layer_session_status_present) {
    HANDLE_CODE(app_layer_session_status.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qo_e_meas_results_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool rrc_container_for_rv_qo_e_report_present;
  HANDLE_CODE(bref.unpack(rrc_container_for_rv_qo_e_report_present, 1));
  bool rrc_container_for_qo_e_report_present;
  HANDLE_CODE(bref.unpack(rrc_container_for_qo_e_report_present, 1));
  HANDLE_CODE(bref.unpack(app_layer_session_status_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(qoe_ref.unpack(bref));
  if (rrc_container_for_rv_qo_e_report_present) {
    HANDLE_CODE(rrc_container_for_rv_qo_e_report.unpack(bref));
  }
  if (rrc_container_for_qo_e_report_present) {
    HANDLE_CODE(rrc_container_for_qo_e_report.unpack(bref));
  }
  if (app_layer_session_status_present) {
    HANDLE_CODE(app_layer_session_status.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void qo_e_meas_results_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("qOEReference", qoe_ref.to_string());
  if (rrc_container_for_rv_qo_e_report.size() > 0) {
    j.write_str("rrcContainerForRVQoEReport", rrc_container_for_rv_qo_e_report.to_string());
  }
  if (rrc_container_for_qo_e_report.size() > 0) {
    j.write_str("rrcContainerForQoEReport", rrc_container_for_qo_e_report.to_string());
  }
  if (app_layer_session_status_present) {
    j.write_str("appLayerSessionStatus", app_layer_session_status.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* qo_e_meas_results_s::app_layer_session_status_opts::to_string() const
{
  static const char* names[] = {"started", "stopped"};
  return convert_enum_idx(names, 2, value, "qo_e_meas_results_s::app_layer_session_status_e_");
}

// RANMulticastGroupPaging-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ran_multicast_group_paging_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {269, 270, 271};
  return map_enum_number(names, 3, idx, "id");
}
bool ran_multicast_group_paging_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {269, 270, 271};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ran_multicast_group_paging_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 269:
      return crit_e::reject;
    case 270:
      return crit_e::reject;
    case 271:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ran_multicast_group_paging_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 269:
      return presence_e::mandatory;
    case 270:
      return presence_e::mandatory;
    case 271:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ran_multicast_group_paging_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 269, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 270, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, ue_id_idx_list_mbs_group_paging, 1, 4096, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 271, crit_e{crit_e::reject}, [this, &bref]() { return multicast_ran_paging_area.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_multicast_group_paging_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ran_multicast_group_paging_ies_o::get_crit));

    switch (id) {
      case 269: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 270: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_id_idx_list_mbs_group_paging, bref, 1, 4096, true); }));
        break;
      }
      case 271: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return multicast_ran_paging_area.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ran_multicast_group_paging_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 269);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 270);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ue_id_idx_list_mbs_group_paging) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 271);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  multicast_ran_paging_area.to_json(j);
  j.end_obj();
  j.end_array();
}

// RANPaging-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ran_paging_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {89, 90, 31, 52, 53, 5, 122, 244, 245, 248, 348, 349, 356, 357, 372, 379, 432};
  return map_enum_number(names, 17, idx, "id");
}
bool ran_paging_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {89, 90, 31, 52, 53, 5, 122, 244, 245, 248, 348, 349, 356, 357, 372, 379, 432};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ran_paging_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 89:
      return crit_e::reject;
    case 90:
      return crit_e::ignore;
    case 31:
      return crit_e::ignore;
    case 52:
      return crit_e::reject;
    case 53:
      return crit_e::ignore;
    case 5:
      return crit_e::ignore;
    case 122:
      return crit_e::ignore;
    case 244:
      return crit_e::ignore;
    case 245:
      return crit_e::ignore;
    case 248:
      return crit_e::ignore;
    case 348:
      return crit_e::ignore;
    case 349:
      return crit_e::ignore;
    case 356:
      return crit_e::ignore;
    case 357:
      return crit_e::ignore;
    case 372:
      return crit_e::ignore;
    case 379:
      return crit_e::ignore;
    case 432:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ran_paging_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 89:
      return presence_e::mandatory;
    case 90:
      return presence_e::mandatory;
    case 31:
      return presence_e::mandatory;
    case 52:
      return presence_e::mandatory;
    case 53:
      return presence_e::optional;
    case 5:
      return presence_e::optional;
    case 122:
      return presence_e::optional;
    case 244:
      return presence_e::optional;
    case 245:
      return presence_e::optional;
    case 248:
      return presence_e::optional;
    case 348:
      return presence_e::optional;
    case 349:
      return presence_e::optional;
    case 356:
      return presence_e::optional;
    case 357:
      return presence_e::optional;
    case 372:
      return presence_e::optional;
    case 379:
      return presence_e::optional;
    case 432:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ran_paging_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += paging_prio_present ? 1 : 0;
  nof_ies += assist_data_for_ran_paging_present ? 1 : 0;
  nof_ies += ue_radio_cap_for_paging_present ? 1 : 0;
  nof_ies += extended_ue_id_idx_value_present ? 1 : 0;
  nof_ies += eutra_paginge_drx_info_present ? 1 : 0;
  nof_ies += ue_specific_drx_present ? 1 : 0;
  nof_ies += nr_paginge_drx_info_present ? 1 : 0;
  nof_ies += nr_paginge_drx_infofor_rrc_inactive_present ? 1 : 0;
  nof_ies += paging_cause_present ? 1 : 0;
  nof_ies += pe_ip_sassist_info_present ? 1 : 0;
  nof_ies += hashed_ue_id_idx_value_present ? 1 : 0;
  nof_ies += mt_sdt_info_present ? 1 : 0;
  nof_ies += nr_paging_longe_drx_infofor_rrc_inactive_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() { return ue_id_idx_value.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 90, crit_e{crit_e::ignore}, [this, &bref]() { return ue_ran_paging_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 31, crit_e{crit_e::ignore}, [this, &bref]() { return paging_drx.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 52, crit_e{crit_e::reject}, [this, &bref]() { return ran_paging_area.pack(bref); }));
  if (paging_prio_present) {
    HANDLE_CODE(pack_ie(bref, 53, crit_e{crit_e::ignore}, [this, &bref]() { return paging_prio.pack(bref); }));
  }
  if (assist_data_for_ran_paging_present) {
    HANDLE_CODE(
        pack_ie(bref, 5, crit_e{crit_e::ignore}, [this, &bref]() { return assist_data_for_ran_paging.pack(bref); }));
  }
  if (ue_radio_cap_for_paging_present) {
    HANDLE_CODE(
        pack_ie(bref, 122, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap_for_paging.pack(bref); }));
  }
  if (extended_ue_id_idx_value_present) {
    HANDLE_CODE(
        pack_ie(bref, 244, crit_e{crit_e::ignore}, [this, &bref]() { return extended_ue_id_idx_value.pack(bref); }));
  }
  if (eutra_paginge_drx_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 245, crit_e{crit_e::ignore}, [this, &bref]() { return eutra_paginge_drx_info.pack(bref); }));
  }
  if (ue_specific_drx_present) {
    HANDLE_CODE(pack_ie(bref, 248, crit_e{crit_e::ignore}, [this, &bref]() { return ue_specific_drx.pack(bref); }));
  }
  if (nr_paginge_drx_info_present) {
    HANDLE_CODE(pack_ie(bref, 348, crit_e{crit_e::ignore}, [this, &bref]() { return nr_paginge_drx_info.pack(bref); }));
  }
  if (nr_paginge_drx_infofor_rrc_inactive_present) {
    HANDLE_CODE(pack_ie(
        bref, 349, crit_e{crit_e::ignore}, [this, &bref]() { return nr_paginge_drx_infofor_rrc_inactive.pack(bref); }));
  }
  if (paging_cause_present) {
    HANDLE_CODE(pack_ie(bref, 356, crit_e{crit_e::ignore}, [this, &bref]() { return paging_cause.pack(bref); }));
  }
  if (pe_ip_sassist_info_present) {
    HANDLE_CODE(pack_ie(bref, 357, crit_e{crit_e::ignore}, [this, &bref]() { return pe_ip_sassist_info.pack(bref); }));
  }
  if (hashed_ue_id_idx_value_present) {
    HANDLE_CODE(
        pack_ie(bref, 372, crit_e{crit_e::ignore}, [this, &bref]() { return hashed_ue_id_idx_value.pack(bref); }));
  }
  if (mt_sdt_info_present) {
    HANDLE_CODE(pack_ie(bref, 379, crit_e{crit_e::ignore}, [this, &bref]() { return mt_sdt_info.pack(bref); }));
  }
  if (nr_paging_longe_drx_infofor_rrc_inactive_present) {
    HANDLE_CODE(pack_ie(bref, 432, crit_e{crit_e::ignore}, [this, &bref]() {
      return nr_paging_longe_drx_infofor_rrc_inactive.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_paging_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ran_paging_ies_o::get_crit));

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_id_idx_value.unpack(bref); }));
        break;
      }
      case 90: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_ran_paging_id.unpack(bref); }));
        break;
      }
      case 31: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_drx.unpack(bref); }));
        break;
      }
      case 52: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_paging_area.unpack(bref); }));
        break;
      }
      case 53: {
        paging_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_prio.unpack(bref); }));
        break;
      }
      case 5: {
        assist_data_for_ran_paging_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return assist_data_for_ran_paging.unpack(bref); }));
        break;
      }
      case 122: {
        ue_radio_cap_for_paging_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_for_paging.unpack(bref); }));
        break;
      }
      case 244: {
        extended_ue_id_idx_value_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_ue_id_idx_value.unpack(bref); }));
        break;
      }
      case 245: {
        eutra_paginge_drx_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return eutra_paginge_drx_info.unpack(bref); }));
        break;
      }
      case 248: {
        ue_specific_drx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_specific_drx.unpack(bref); }));
        break;
      }
      case 348: {
        nr_paginge_drx_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_paginge_drx_info.unpack(bref); }));
        break;
      }
      case 349: {
        nr_paginge_drx_infofor_rrc_inactive_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return nr_paginge_drx_infofor_rrc_inactive.unpack(bref); }));
        break;
      }
      case 356: {
        paging_cause_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_cause.unpack(bref); }));
        break;
      }
      case 357: {
        pe_ip_sassist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pe_ip_sassist_info.unpack(bref); }));
        break;
      }
      case 372: {
        hashed_ue_id_idx_value_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return hashed_ue_id_idx_value.unpack(bref); }));
        break;
      }
      case 379: {
        mt_sdt_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mt_sdt_info.unpack(bref); }));
        break;
      }
      case 432: {
        nr_paging_longe_drx_infofor_rrc_inactive_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return nr_paging_longe_drx_infofor_rrc_inactive.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ran_paging_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_id_idx_value.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 90);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  ue_ran_paging_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 31);
  j.write_str("criticality", "ignore");
  j.write_str("value", paging_drx.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 52);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ran_paging_area.to_json(j);
  j.end_obj();
  if (paging_prio_present) {
    j.start_obj();
    j.write_int("id", 53);
    j.write_str("criticality", "ignore");
    j.write_str("value", paging_prio.to_string());
    j.end_obj();
  }
  if (assist_data_for_ran_paging_present) {
    j.start_obj();
    j.write_int("id", 5);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    assist_data_for_ran_paging.to_json(j);
    j.end_obj();
  }
  if (ue_radio_cap_for_paging_present) {
    j.start_obj();
    j.write_int("id", 122);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_radio_cap_for_paging.to_json(j);
    j.end_obj();
  }
  if (extended_ue_id_idx_value_present) {
    j.start_obj();
    j.write_int("id", 244);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_ue_id_idx_value.to_json(j);
    j.end_obj();
  }
  if (eutra_paginge_drx_info_present) {
    j.start_obj();
    j.write_int("id", 245);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    eutra_paginge_drx_info.to_json(j);
    j.end_obj();
  }
  if (ue_specific_drx_present) {
    j.start_obj();
    j.write_int("id", 248);
    j.write_str("criticality", "ignore");
    j.write_str("value", ue_specific_drx.to_string());
    j.end_obj();
  }
  if (nr_paginge_drx_info_present) {
    j.start_obj();
    j.write_int("id", 348);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_paginge_drx_info.to_json(j);
    j.end_obj();
  }
  if (nr_paginge_drx_infofor_rrc_inactive_present) {
    j.start_obj();
    j.write_int("id", 349);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_paginge_drx_infofor_rrc_inactive.to_json(j);
    j.end_obj();
  }
  if (paging_cause_present) {
    j.start_obj();
    j.write_int("id", 356);
    j.write_str("criticality", "ignore");
    j.write_str("value", "voice");
    j.end_obj();
  }
  if (pe_ip_sassist_info_present) {
    j.start_obj();
    j.write_int("id", 357);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pe_ip_sassist_info.to_json(j);
    j.end_obj();
  }
  if (hashed_ue_id_idx_value_present) {
    j.start_obj();
    j.write_int("id", 372);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    hashed_ue_id_idx_value.to_json(j);
    j.end_obj();
  }
  if (mt_sdt_info_present) {
    j.start_obj();
    j.write_int("id", 379);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mt_sdt_info.to_json(j);
    j.end_obj();
  }
  if (nr_paging_longe_drx_infofor_rrc_inactive_present) {
    j.start_obj();
    j.write_int("id", 432);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_paging_longe_drx_infofor_rrc_inactive.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SplitSRB-RRCTransfer ::= SEQUENCE
OCUDUASN_CODE split_srb_rrc_transfer_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(rrc_container.size() > 0, 1));
  HANDLE_CODE(bref.pack(delivery_status_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (rrc_container.size() > 0) {
    HANDLE_CODE(rrc_container.pack(bref));
  }
  HANDLE_CODE(srb_type.pack(bref));
  if (delivery_status_present) {
    HANDLE_CODE(pack_integer(bref, delivery_status, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE split_srb_rrc_transfer_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool rrc_container_present;
  HANDLE_CODE(bref.unpack(rrc_container_present, 1));
  HANDLE_CODE(bref.unpack(delivery_status_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (rrc_container_present) {
    HANDLE_CODE(rrc_container.unpack(bref));
  }
  HANDLE_CODE(srb_type.unpack(bref));
  if (delivery_status_present) {
    HANDLE_CODE(unpack_integer(delivery_status, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void split_srb_rrc_transfer_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (rrc_container.size() > 0) {
    j.write_str("rrcContainer", rrc_container.to_string());
  }
  j.write_str("srbType", srb_type.to_string());
  if (delivery_status_present) {
    j.write_int("deliveryStatus", delivery_status);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* split_srb_rrc_transfer_s::srb_type_opts::to_string() const
{
  static const char* names[] = {"srb1", "srb2"};
  return convert_enum_idx(names, 2, value, "split_srb_rrc_transfer_s::srb_type_e_");
}
uint8_t split_srb_rrc_transfer_s::srb_type_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "split_srb_rrc_transfer_s::srb_type_e_");
}

// UEReportRRCTransfer ::= SEQUENCE
OCUDUASN_CODE ue_report_rrc_transfer_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(rrc_container.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_report_rrc_transfer_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(rrc_container.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_report_rrc_transfer_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("rrcContainer", rrc_container.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SDT-SRB-between-NewNode-OldNode ::= SEQUENCE
OCUDUASN_CODE sdt_srb_between_new_node_old_node_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(rrc_container.pack(bref));
  HANDLE_CODE(pack_integer(bref, srb_id, (uint8_t)0u, (uint8_t)4u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sdt_srb_between_new_node_old_node_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(rrc_container.unpack(bref));
  HANDLE_CODE(unpack_integer(srb_id, bref, (uint8_t)0u, (uint8_t)4u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sdt_srb_between_new_node_old_node_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("rrcContainer", rrc_container.to_string());
  j.write_int("srb-ID", srb_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RRCTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t rrc_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 74, 28, 147, 152, 352, 436};
  return map_enum_number(names, 8, idx, "id");
}
bool rrc_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 74, 28, 147, 152, 352, 436};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e rrc_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 74:
      return crit_e::reject;
    case 28:
      return crit_e::reject;
    case 147:
      return crit_e::ignore;
    case 152:
      return crit_e::ignore;
    case 352:
      return crit_e::ignore;
    case 436:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e rrc_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 74:
      return presence_e::optional;
    case 28:
      return presence_e::optional;
    case 147:
      return presence_e::optional;
    case 152:
      return presence_e::optional;
    case 352:
      return presence_e::optional;
    case 436:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE rrc_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += split_srb_rrc_transfer_present ? 1 : 0;
  nof_ies += ue_report_rrc_transfer_present ? 1 : 0;
  nof_ies += fast_mcg_recovery_rrc_transfer_sn_to_mn_present ? 1 : 0;
  nof_ies += fast_mcg_recovery_rrc_transfer_mn_to_sn_present ? 1 : 0;
  nof_ies += sdt_srb_between_new_node_old_node_present ? 1 : 0;
  nof_ies += qo_e_meas_results_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (split_srb_rrc_transfer_present) {
    HANDLE_CODE(
        pack_ie(bref, 74, crit_e{crit_e::reject}, [this, &bref]() { return split_srb_rrc_transfer.pack(bref); }));
  }
  if (ue_report_rrc_transfer_present) {
    HANDLE_CODE(
        pack_ie(bref, 28, crit_e{crit_e::reject}, [this, &bref]() { return ue_report_rrc_transfer.pack(bref); }));
  }
  if (fast_mcg_recovery_rrc_transfer_sn_to_mn_present) {
    HANDLE_CODE(pack_ie(bref, 147, crit_e{crit_e::ignore}, [this, &bref]() {
      return fast_mcg_recovery_rrc_transfer_sn_to_mn.pack(bref);
    }));
  }
  if (fast_mcg_recovery_rrc_transfer_mn_to_sn_present) {
    HANDLE_CODE(pack_ie(bref, 152, crit_e{crit_e::ignore}, [this, &bref]() {
      return fast_mcg_recovery_rrc_transfer_mn_to_sn.pack(bref);
    }));
  }
  if (sdt_srb_between_new_node_old_node_present) {
    HANDLE_CODE(pack_ie(
        bref, 352, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_srb_between_new_node_old_node.pack(bref); }));
  }
  if (qo_e_meas_results_present) {
    HANDLE_CODE(pack_ie(bref, 436, crit_e{crit_e::ignore}, [this, &bref]() { return qo_e_meas_results.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &rrc_transfer_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 74: {
        split_srb_rrc_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return split_srb_rrc_transfer.unpack(bref); }));
        break;
      }
      case 28: {
        ue_report_rrc_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_report_rrc_transfer.unpack(bref); }));
        break;
      }
      case 147: {
        fast_mcg_recovery_rrc_transfer_sn_to_mn_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return fast_mcg_recovery_rrc_transfer_sn_to_mn.unpack(bref); }));
        break;
      }
      case 152: {
        fast_mcg_recovery_rrc_transfer_mn_to_sn_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return fast_mcg_recovery_rrc_transfer_mn_to_sn.unpack(bref); }));
        break;
      }
      case 352: {
        sdt_srb_between_new_node_old_node_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_srb_between_new_node_old_node.unpack(bref); }));
        break;
      }
      case 436: {
        qo_e_meas_results_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return qo_e_meas_results.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void rrc_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (split_srb_rrc_transfer_present) {
    j.start_obj();
    j.write_int("id", 74);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    split_srb_rrc_transfer.to_json(j);
    j.end_obj();
  }
  if (ue_report_rrc_transfer_present) {
    j.start_obj();
    j.write_int("id", 28);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ue_report_rrc_transfer.to_json(j);
    j.end_obj();
  }
  if (fast_mcg_recovery_rrc_transfer_sn_to_mn_present) {
    j.start_obj();
    j.write_int("id", 147);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    fast_mcg_recovery_rrc_transfer_sn_to_mn.to_json(j);
    j.end_obj();
  }
  if (fast_mcg_recovery_rrc_transfer_mn_to_sn_present) {
    j.start_obj();
    j.write_int("id", 152);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    fast_mcg_recovery_rrc_transfer_mn_to_sn.to_json(j);
    j.end_obj();
  }
  if (sdt_srb_between_new_node_old_node_present) {
    j.start_obj();
    j.write_int("id", 352);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sdt_srb_between_new_node_old_node.to_json(j);
    j.end_obj();
  }
  if (qo_e_meas_results_present) {
    j.start_obj();
    j.write_int("id", 436);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    qo_e_meas_results.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RachIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t rach_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {389};
  return map_enum_number(names, 1, idx, "id");
}
bool rach_ind_ies_o::is_id_valid(const uint32_t& id)
{
  return 389 == id;
}
crit_e rach_ind_ies_o::get_crit(const uint32_t& id)
{
  if (id == 389) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e rach_ind_ies_o::get_presence(const uint32_t& id)
{
  if (id == 389) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE rach_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 389, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, ra_report_ind_list, 1, 64, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rach_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &rach_ind_ies_o::get_crit));

    if (id == 389) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ra_report_ind_list, bref, 1, 64, true); }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void rach_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 389);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ra_report_ind_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// ReleaseFastMCGRecoveryViaSRB3 ::= ENUMERATED
const char* release_fast_mcg_recovery_via_srb3_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "release_fast_mcg_recovery_via_srb3_e");
}

// RequestedFastMCGRecoveryViaSRB3 ::= ENUMERATED
const char* requested_fast_mcg_recovery_via_srb3_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "requested_fast_mcg_recovery_via_srb3_e");
}

// RequestedFastMCGRecoveryViaSRB3Release ::= ENUMERATED
const char* requested_fast_mcg_recovery_via_srb3_release_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "requested_fast_mcg_recovery_via_srb3_release_e");
}

// ResetRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t reset_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {56, 7, 130};
  return map_enum_number(names, 3, idx, "id");
}
bool reset_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {56, 7, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e reset_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 56:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e reset_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 56:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE reset_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(
      pack_ie(bref, 56, crit_e{crit_e::reject}, [this, &bref]() { return reset_request_type_info.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reset_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &reset_request_ies_o::get_crit));

    switch (id) {
      case 56: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return reset_request_type_info.unpack(bref); }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void reset_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 56);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  reset_request_type_info.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// ResetResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t reset_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57, 10, 130};
  return map_enum_number(names, 3, idx, "id");
}
bool reset_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {57, 10, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e reset_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 57:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e reset_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 57:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE reset_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::reject}, [this, &bref]() { return reset_resp_type_info.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reset_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &reset_resp_ies_o::get_crit));

    switch (id) {
      case 57: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return reset_resp_type_info.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void reset_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 57);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  reset_resp_type_info.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t res_status_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {187, 188, 7, 10};
  return map_enum_number(names, 4, idx, "id");
}
bool res_status_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {187, 188, 7, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e res_status_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 187:
      return crit_e::reject;
    case 188:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 187:
      return presence_e::mandatory;
    case 188:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 187, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node1_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node2_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &res_status_fail_ies_o::get_crit));

    switch (id) {
      case 187: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node1_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 188: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node2_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void res_status_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 187);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node1_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 188);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node2_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t res_status_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {187, 188, 189, 190, 191, 192};
  return map_enum_number(names, 6, idx, "id");
}
bool res_status_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {187, 188, 189, 190, 191, 192};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e res_status_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 187:
      return crit_e::reject;
    case 188:
      return crit_e::ignore;
    case 189:
      return crit_e::reject;
    case 190:
      return crit_e::reject;
    case 191:
      return crit_e::ignore;
    case 192:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 187:
      return presence_e::mandatory;
    case 188:
      return presence_e::conditional;
    case 189:
      return presence_e::mandatory;
    case 190:
      return presence_e::conditional;
    case 191:
      return presence_e::optional;
    case 192:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ngran_node2_meas_id_present ? 1 : 0;
  nof_ies += report_characteristics_present ? 1 : 0;
  nof_ies += cell_to_report_present ? 1 : 0;
  nof_ies += report_periodicity_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 187, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node1_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (ngran_node2_meas_id_present) {
    HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ngran_node2_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 189, crit_e{crit_e::reject}, [this, &bref]() { return regist_request.pack(bref); }));
  if (report_characteristics_present) {
    HANDLE_CODE(
        pack_ie(bref, 190, crit_e{crit_e::reject}, [this, &bref]() { return report_characteristics.pack(bref); }));
  }
  if (cell_to_report_present) {
    HANDLE_CODE(pack_ie(bref, 191, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, cell_to_report, 1, 16384, true);
    }));
  }
  if (report_periodicity_present) {
    HANDLE_CODE(pack_ie(bref, 192, crit_e{crit_e::ignore}, [this, &bref]() { return report_periodicity.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &res_status_request_ies_o::get_crit));

    switch (id) {
      case 187: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node1_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 188: {
        ngran_node2_meas_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node2_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 189: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return regist_request.unpack(bref); }));
        break;
      }
      case 190: {
        report_characteristics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return report_characteristics.unpack(bref); }));
        break;
      }
      case 191: {
        cell_to_report_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(cell_to_report, bref, 1, 16384, true); }));
        break;
      }
      case 192: {
        report_periodicity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return report_periodicity.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void res_status_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 187);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node1_meas_id);
  j.end_obj();
  if (ngran_node2_meas_id_present) {
    j.start_obj();
    j.write_int("id", 188);
    j.write_str("criticality", "ignore");
    j.write_int("value", ngran_node2_meas_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 189);
  j.write_str("criticality", "reject");
  j.write_str("value", regist_request.to_string());
  j.end_obj();
  if (report_characteristics_present) {
    j.start_obj();
    j.write_int("id", 190);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    report_characteristics.to_json(j);
    j.end_obj();
  }
  if (cell_to_report_present) {
    j.start_obj();
    j.write_int("id", 191);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cell_to_report) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (report_periodicity_present) {
    j.start_obj();
    j.write_int("id", 192);
    j.write_str("criticality", "ignore");
    j.write_str("value", report_periodicity.to_string());
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t res_status_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {187, 188, 10};
  return map_enum_number(names, 3, idx, "id");
}
bool res_status_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {187, 188, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e res_status_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 187:
      return crit_e::reject;
    case 188:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 187:
      return presence_e::mandatory;
    case 188:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 187, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node1_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node2_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &res_status_resp_ies_o::get_crit));

    switch (id) {
      case 187: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node1_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 188: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node2_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void res_status_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 187);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node1_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 188);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node2_meas_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusUpdate-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t res_status_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {187, 188, 193};
  return map_enum_number(names, 3, idx, "id");
}
bool res_status_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {187, 188, 193};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e res_status_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 187:
      return crit_e::reject;
    case 188:
      return crit_e::reject;
    case 193:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 187:
      return presence_e::mandatory;
    case 188:
      return presence_e::mandatory;
    case 193:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 187, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node1_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 188, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ngran_node2_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 193, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, cell_meas_result, 1, 16384, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &res_status_upd_ies_o::get_crit));

    switch (id) {
      case 187: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node1_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 188: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ngran_node2_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 193: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(cell_meas_result, bref, 1, 16384, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void res_status_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 187);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node1_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 188);
  j.write_str("criticality", "reject");
  j.write_int("value", ngran_node2_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 193);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : cell_meas_result) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// ResponseType-ReconfComplete ::= CHOICE
void resp_type_reconf_complete_c::destroy_()
{
  switch (type_) {
    case types::cfg_successfully_applied:
      c.destroy<cfg_successfully_applied_s>();
      break;
    case types::cfg_rejected_by_m_ng_ran_node:
      c.destroy<cfg_rejected_by_m_ng_ran_node_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void resp_type_reconf_complete_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::cfg_successfully_applied:
      c.init<cfg_successfully_applied_s>();
      break;
    case types::cfg_rejected_by_m_ng_ran_node:
      c.init<cfg_rejected_by_m_ng_ran_node_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "resp_type_reconf_complete_c");
  }
}
resp_type_reconf_complete_c::resp_type_reconf_complete_c(const resp_type_reconf_complete_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::cfg_successfully_applied:
      c.init(other.c.get<cfg_successfully_applied_s>());
      break;
    case types::cfg_rejected_by_m_ng_ran_node:
      c.init(other.c.get<cfg_rejected_by_m_ng_ran_node_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "resp_type_reconf_complete_c");
  }
}
resp_type_reconf_complete_c& resp_type_reconf_complete_c::operator=(const resp_type_reconf_complete_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::cfg_successfully_applied:
      c.set(other.c.get<cfg_successfully_applied_s>());
      break;
    case types::cfg_rejected_by_m_ng_ran_node:
      c.set(other.c.get<cfg_rejected_by_m_ng_ran_node_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "resp_type_reconf_complete_c");
  }

  return *this;
}
cfg_successfully_applied_s& resp_type_reconf_complete_c::set_cfg_successfully_applied()
{
  set(types::cfg_successfully_applied);
  return c.get<cfg_successfully_applied_s>();
}
cfg_rejected_by_m_ng_ran_node_s& resp_type_reconf_complete_c::set_cfg_rejected_by_m_ng_ran_node()
{
  set(types::cfg_rejected_by_m_ng_ran_node);
  return c.get<cfg_rejected_by_m_ng_ran_node_s>();
}
protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>& resp_type_reconf_complete_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>();
}
void resp_type_reconf_complete_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cfg_successfully_applied:
      j.write_fieldname("configuration-successfully-applied");
      c.get<cfg_successfully_applied_s>().to_json(j);
      break;
    case types::cfg_rejected_by_m_ng_ran_node:
      j.write_fieldname("configuration-rejected-by-M-NG-RANNode");
      c.get<cfg_rejected_by_m_ng_ran_node_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "resp_type_reconf_complete_c");
  }
  j.end_obj();
}
OCUDUASN_CODE resp_type_reconf_complete_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::cfg_successfully_applied:
      HANDLE_CODE(c.get<cfg_successfully_applied_s>().pack(bref));
      break;
    case types::cfg_rejected_by_m_ng_ran_node:
      HANDLE_CODE(c.get<cfg_rejected_by_m_ng_ran_node_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "resp_type_reconf_complete_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE resp_type_reconf_complete_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::cfg_successfully_applied:
      HANDLE_CODE(c.get<cfg_successfully_applied_s>().unpack(bref));
      break;
    case types::cfg_rejected_by_m_ng_ran_node:
      HANDLE_CODE(c.get<cfg_rejected_by_m_ng_ran_node_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<resp_type_reconf_complete_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "resp_type_reconf_complete_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* resp_type_reconf_complete_c::types_opts::to_string() const
{
  static const char* names[] = {
      "configuration-successfully-applied", "configuration-rejected-by-M-NG-RANNode", "choice-extension"};
  return convert_enum_idx(names, 3, value, "resp_type_reconf_complete_c::types");
}

// ResponseInfo-ReconfCompl ::= SEQUENCE
OCUDUASN_CODE resp_info_reconf_compl_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(resp_type_reconf_complete.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE resp_info_reconf_compl_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(resp_type_reconf_complete.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void resp_info_reconf_compl_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("responseType-ReconfComplete");
  resp_type_reconf_complete.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RetrieveUEContextConfirm-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t retrieve_ue_context_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 27, 86, 353};
  return map_enum_number(names, 4, idx, "id");
}
bool retrieve_ue_context_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 27, 86, 353};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e retrieve_ue_context_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::ignore;
    case 27:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    case 353:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e retrieve_ue_context_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 27:
      return presence_e::mandatory;
    case 86:
      return presence_e::optional;
    case 353:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE retrieve_ue_context_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ue_context_kept_ind_present ? 1 : 0;
  nof_ies += sdt_termination_request_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, old_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ue_context_kept_ind_present) {
    HANDLE_CODE(pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return ue_context_kept_ind.pack(bref); }));
  }
  if (sdt_termination_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 353, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_termination_request.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE retrieve_ue_context_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &retrieve_ue_context_confirm_ies_o::get_crit));

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 86: {
        ue_context_kept_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_kept_ind.unpack(bref); }));
        break;
      }
      case 353: {
        sdt_termination_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_termination_request.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void retrieve_ue_context_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "ignore");
  j.write_uint("value", old_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "ignore");
  j.write_uint("value", new_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (ue_context_kept_ind_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (sdt_termination_request_present) {
    j.start_obj();
    j.write_int("id", 353);
    j.write_str("criticality", "ignore");
    j.write_str("value", sdt_termination_request.to_string());
    j.end_obj();
  }
  j.end_array();
}

// RetrieveUEContextFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t retrieve_ue_context_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {27, 30, 7, 10, 545};
  return map_enum_number(names, 5, idx, "id");
}
bool retrieve_ue_context_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {27, 30, 7, 10, 545};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e retrieve_ue_context_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 27:
      return crit_e::ignore;
    case 30:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 545:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e retrieve_ue_context_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 27:
      return presence_e::mandatory;
    case 30:
      return presence_e::optional;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 545:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE retrieve_ue_context_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += oldto_new_ng_ran_node_resume_container_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += semipersistent_positioning_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (oldto_new_ng_ran_node_resume_container_present) {
    HANDLE_CODE(pack_ie(bref, 30, crit_e{crit_e::ignore}, [this, &bref]() {
      return oldto_new_ng_ran_node_resume_container.pack(bref);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (semipersistent_positioning_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 545, crit_e{crit_e::ignore}, [this, &bref]() { return semipersistent_positioning_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE retrieve_ue_context_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &retrieve_ue_context_fail_ies_o::get_crit));

    switch (id) {
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 30: {
        oldto_new_ng_ran_node_resume_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return oldto_new_ng_ran_node_resume_container.unpack(bref); }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 545: {
        semipersistent_positioning_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return semipersistent_positioning_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void retrieve_ue_context_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "ignore");
  j.write_uint("value", new_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (oldto_new_ng_ran_node_resume_container_present) {
    j.start_obj();
    j.write_int("id", 30);
    j.write_str("criticality", "ignore");
    j.write_str("value", oldto_new_ng_ran_node_resume_container.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (semipersistent_positioning_info_present) {
    j.start_obj();
    j.write_int("id", 545);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    semipersistent_positioning_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RetrieveUEContextRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t retrieve_ue_context_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {27, 82, 21, 26, 62, 351, 473};
  return map_enum_number(names, 7, idx, "id");
}
bool retrieve_ue_context_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {27, 82, 21, 26, 62, 351, 473};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e retrieve_ue_context_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 27:
      return crit_e::reject;
    case 82:
      return crit_e::reject;
    case 21:
      return crit_e::reject;
    case 26:
      return crit_e::reject;
    case 62:
      return crit_e::ignore;
    case 351:
      return crit_e::ignore;
    case 473:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e retrieve_ue_context_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 27:
      return presence_e::mandatory;
    case 82:
      return presence_e::mandatory;
    case 21:
      return presence_e::mandatory;
    case 26:
      return presence_e::mandatory;
    case 62:
      return presence_e::optional;
    case 351:
      return presence_e::optional;
    case 473:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE retrieve_ue_context_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += rrc_resume_cause_present ? 1 : 0;
  nof_ies += sdt_support_request_present ? 1 : 0;
  nof_ies += srs_positioning_cfg_or_activation_request_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 82, crit_e{crit_e::reject}, [this, &bref]() { return ue_context_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 21, crit_e{crit_e::reject}, [this, &bref]() { return mac_i.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::reject}, [this, &bref]() { return new_ng_ran_cell_id.pack(bref); }));
  if (rrc_resume_cause_present) {
    HANDLE_CODE(pack_ie(bref, 62, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_resume_cause.pack(bref); }));
  }
  if (sdt_support_request_present) {
    HANDLE_CODE(pack_ie(bref, 351, crit_e{crit_e::ignore}, [this, &bref]() { return sdt_support_request.pack(bref); }));
  }
  if (srs_positioning_cfg_or_activation_request_present) {
    HANDLE_CODE(pack_ie(bref, 473, crit_e{crit_e::ignore}, [this, &bref]() {
      return srs_positioning_cfg_or_activation_request.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE retrieve_ue_context_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &retrieve_ue_context_request_ies_o::get_crit));

    switch (id) {
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 82: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_id.unpack(bref); }));
        break;
      }
      case 21: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mac_i.unpack(bref); }));
        break;
      }
      case 26: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return new_ng_ran_cell_id.unpack(bref); }));
        break;
      }
      case 62: {
        rrc_resume_cause_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_resume_cause.unpack(bref); }));
        break;
      }
      case 351: {
        sdt_support_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sdt_support_request.unpack(bref); }));
        break;
      }
      case 473: {
        srs_positioning_cfg_or_activation_request_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return srs_positioning_cfg_or_activation_request.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void retrieve_ue_context_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "reject");
  j.write_uint("value", new_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 82);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_context_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 21);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mac_i.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 26);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  new_ng_ran_cell_id.to_json(j);
  j.end_obj();
  if (rrc_resume_cause_present) {
    j.start_obj();
    j.write_int("id", 62);
    j.write_str("criticality", "ignore");
    j.write_str("value", "rna-Update");
    j.end_obj();
  }
  if (sdt_support_request_present) {
    j.start_obj();
    j.write_int("id", 351);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sdt_support_request.to_json(j);
    j.end_obj();
  }
  if (srs_positioning_cfg_or_activation_request_present) {
    j.start_obj();
    j.write_int("id", 473);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// RetrieveUEContextResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t retrieve_ue_context_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {27,  29,  15, 84,  81,  22,  20,  10,  170, 169, 173, 88,  178,
                                   225, 206, 87, 325, 337, 344, 345, 395, 397, 396, 400, 453, 464};
  return map_enum_number(names, 26, idx, "id");
}
bool retrieve_ue_context_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {27,  29,  15, 84,  81,  22,  20,  10,  170, 169, 173, 88,  178,
                                   225, 206, 87, 325, 337, 344, 345, 395, 397, 396, 400, 453, 464};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e retrieve_ue_context_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 27:
      return crit_e::ignore;
    case 29:
      return crit_e::ignore;
    case 15:
      return crit_e::reject;
    case 84:
      return crit_e::reject;
    case 81:
      return crit_e::ignore;
    case 22:
      return crit_e::ignore;
    case 20:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 170:
      return crit_e::ignore;
    case 169:
      return crit_e::ignore;
    case 173:
      return crit_e::ignore;
    case 88:
      return crit_e::ignore;
    case 178:
      return crit_e::ignore;
    case 225:
      return crit_e::ignore;
    case 206:
      return crit_e::reject;
    case 87:
      return crit_e::ignore;
    case 325:
      return crit_e::ignore;
    case 337:
      return crit_e::ignore;
    case 344:
      return crit_e::ignore;
    case 345:
      return crit_e::ignore;
    case 395:
      return crit_e::ignore;
    case 397:
      return crit_e::ignore;
    case 396:
      return crit_e::ignore;
    case 400:
      return crit_e::ignore;
    case 453:
      return crit_e::reject;
    case 464:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e retrieve_ue_context_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 27:
      return presence_e::mandatory;
    case 29:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 84:
      return presence_e::mandatory;
    case 81:
      return presence_e::optional;
    case 22:
      return presence_e::optional;
    case 20:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 170:
      return presence_e::optional;
    case 169:
      return presence_e::optional;
    case 173:
      return presence_e::optional;
    case 88:
      return presence_e::optional;
    case 178:
      return presence_e::optional;
    case 225:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 87:
      return presence_e::optional;
    case 325:
      return presence_e::optional;
    case 337:
      return presence_e::optional;
    case 344:
      return presence_e::optional;
    case 345:
      return presence_e::optional;
    case 395:
      return presence_e::optional;
    case 397:
      return presence_e::optional;
    case 396:
      return presence_e::optional;
    case 400:
      return presence_e::optional;
    case 453:
      return presence_e::optional;
    case 464:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE retrieve_ue_context_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += trace_activation_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += location_report_info_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += nr_v2x_services_authorized_present ? 1 : 0;
  nof_ies += ltev2x_services_authorized_present ? 1 : 0;
  nof_ies += pc5_qos_params_present ? 1 : 0;
  nof_ies += ue_history_info_present ? 1 : 0;
  nof_ies += ue_history_info_from_the_ue_present ? 1 : 0;
  nof_ies += mdt_plmn_list_present ? 1 : 0;
  nof_ies += iab_node_ind_present ? 1 : 0;
  nof_ies += ue_context_ref_at_sn_ho_request_present ? 1 : 0;
  nof_ies += time_sync_assist_info_present ? 1 : 0;
  nof_ies += q_mcc_onfig_info_present ? 1 : 0;
  nof_ies += five_g_pro_se_authorized_present ? 1 : 0;
  nof_ies += five_g_pro_se_pc5_qos_params_present ? 1 : 0;
  nof_ies += aerial_ue_sub_info_present ? 1 : 0;
  nof_ies += nr_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += ltea2_x_services_authorized_present ? 1 : 0;
  nof_ies += a2_xpc5_qos_params_present ? 1 : 0;
  nof_ies += mobile_iab_authorization_status_present ? 1 : 0;
  nof_ies += sl_positioning_ranging_services_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, old_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::reject}, [this, &bref]() { return guami.pack(bref); }));
  HANDLE_CODE(pack_ie(
      bref, 84, crit_e{crit_e::reject}, [this, &bref]() { return ue_context_info_retr_ue_ctxt_resp.pack(bref); }));
  if (trace_activation_present) {
    HANDLE_CODE(pack_ie(bref, 81, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 22, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  if (location_report_info_present) {
    HANDLE_CODE(pack_ie(bref, 20, crit_e{crit_e::ignore}, [this, &bref]() { return location_report_info.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (nr_v2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 170, crit_e{crit_e::ignore}, [this, &bref]() { return nr_v2x_services_authorized.pack(bref); }));
  }
  if (ltev2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 169, crit_e{crit_e::ignore}, [this, &bref]() { return ltev2x_services_authorized.pack(bref); }));
  }
  if (pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 173, crit_e{crit_e::ignore}, [this, &bref]() { return pc5_qos_params.pack(bref); }));
  }
  if (ue_history_info_present) {
    HANDLE_CODE(pack_ie(bref, 88, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_history_info, 1, 16, true);
    }));
  }
  if (ue_history_info_from_the_ue_present) {
    HANDLE_CODE(
        pack_ie(bref, 178, crit_e{crit_e::ignore}, [this, &bref]() { return ue_history_info_from_the_ue.pack(bref); }));
  }
  if (mdt_plmn_list_present) {
    HANDLE_CODE(pack_ie(bref, 225, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, mdt_plmn_list, 1, 16, true);
    }));
  }
  if (iab_node_ind_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::reject}, [this, &bref]() { return iab_node_ind.pack(bref); }));
  }
  if (ue_context_ref_at_sn_ho_request_present) {
    HANDLE_CODE(pack_ie(
        bref, 87, crit_e{crit_e::ignore}, [this, &bref]() { return ue_context_ref_at_sn_ho_request.pack(bref); }));
  }
  if (time_sync_assist_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 325, crit_e{crit_e::ignore}, [this, &bref]() { return time_sync_assist_info.pack(bref); }));
  }
  if (q_mcc_onfig_info_present) {
    HANDLE_CODE(pack_ie(bref, 337, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_onfig_info.pack(bref); }));
  }
  if (five_g_pro_se_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 344, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_authorized.pack(bref); }));
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(
        bref, 345, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_pc5_qos_params.pack(bref); }));
  }
  if (aerial_ue_sub_info_present) {
    HANDLE_CODE(pack_ie(bref, 395, crit_e{crit_e::ignore}, [this, &bref]() { return aerial_ue_sub_info.pack(bref); }));
  }
  if (nr_a2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 397, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_services_authorized.pack(bref); }));
  }
  if (ltea2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 396, crit_e{crit_e::ignore}, [this, &bref]() { return ltea2_x_services_authorized.pack(bref); }));
  }
  if (a2_xpc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 400, crit_e{crit_e::ignore}, [this, &bref]() { return a2_xpc5_qos_params.pack(bref); }));
  }
  if (mobile_iab_authorization_status_present) {
    HANDLE_CODE(pack_ie(
        bref, 453, crit_e{crit_e::reject}, [this, &bref]() { return mobile_iab_authorization_status.pack(bref); }));
  }
  if (sl_positioning_ranging_services_info_present) {
    HANDLE_CODE(pack_ie(bref, 464, crit_e{crit_e::ignore}, [this, &bref]() {
      return sl_positioning_ranging_services_info.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE retrieve_ue_context_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &retrieve_ue_context_resp_ies_o::get_crit));

    switch (id) {
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return guami.unpack(bref); }));
        break;
      }
      case 84: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_info_retr_ue_ctxt_resp.unpack(bref); }));
        break;
      }
      case 81: {
        trace_activation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_activation.unpack(bref); }));
        break;
      }
      case 22: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 20: {
        location_report_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_report_info.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 170: {
        nr_v2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_v2x_services_authorized.unpack(bref); }));
        break;
      }
      case 169: {
        ltev2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltev2x_services_authorized.unpack(bref); }));
        break;
      }
      case 173: {
        pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pc5_qos_params.unpack(bref); }));
        break;
      }
      case 88: {
        ue_history_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(ue_history_info, bref, 1, 16, true); }));
        break;
      }
      case 178: {
        ue_history_info_from_the_ue_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_history_info_from_the_ue.unpack(bref); }));
        break;
      }
      case 225: {
        mdt_plmn_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(mdt_plmn_list, bref, 1, 16, true); }));
        break;
      }
      case 206: {
        iab_node_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_node_ind.unpack(bref); }));
        break;
      }
      case 87: {
        ue_context_ref_at_sn_ho_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_ref_at_sn_ho_request.unpack(bref); }));
        break;
      }
      case 325: {
        time_sync_assist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_sync_assist_info.unpack(bref); }));
        break;
      }
      case 337: {
        q_mcc_onfig_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_onfig_info.unpack(bref); }));
        break;
      }
      case 344: {
        five_g_pro_se_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_authorized.unpack(bref); }));
        break;
      }
      case 345: {
        five_g_pro_se_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 395: {
        aerial_ue_sub_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return aerial_ue_sub_info.unpack(bref); }));
        break;
      }
      case 397: {
        nr_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 396: {
        ltea2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltea2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 400: {
        a2_xpc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return a2_xpc5_qos_params.unpack(bref); }));
        break;
      }
      case 453: {
        mobile_iab_authorization_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_authorization_status.unpack(bref); }));
        break;
      }
      case 464: {
        sl_positioning_ranging_services_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sl_positioning_ranging_services_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void retrieve_ue_context_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "ignore");
  j.write_uint("value", new_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "ignore");
  j.write_uint("value", old_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  guami.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 84);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_context_info_retr_ue_ctxt_resp.to_json(j);
  j.end_obj();
  if (trace_activation_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    trace_activation.to_json(j);
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 22);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  if (location_report_info_present) {
    j.start_obj();
    j.write_int("id", 20);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    location_report_info.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (nr_v2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 170);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_v2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltev2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 169);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltev2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 173);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (ue_history_info_present) {
    j.start_obj();
    j.write_int("id", 88);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_history_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_history_info_from_the_ue_present) {
    j.start_obj();
    j.write_int("id", 178);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_history_info_from_the_ue.to_json(j);
    j.end_obj();
  }
  if (mdt_plmn_list_present) {
    j.start_obj();
    j.write_int("id", 225);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : mdt_plmn_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (iab_node_ind_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (ue_context_ref_at_sn_ho_request_present) {
    j.start_obj();
    j.write_int("id", 87);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_context_ref_at_sn_ho_request.to_json(j);
    j.end_obj();
  }
  if (time_sync_assist_info_present) {
    j.start_obj();
    j.write_int("id", 325);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    time_sync_assist_info.to_json(j);
    j.end_obj();
  }
  if (q_mcc_onfig_info_present) {
    j.start_obj();
    j.write_int("id", 337);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_onfig_info.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_authorized_present) {
    j.start_obj();
    j.write_int("id", 344);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_authorized.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 345);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (aerial_ue_sub_info_present) {
    j.start_obj();
    j.write_int("id", 395);
    j.write_str("criticality", "ignore");
    j.write_str("value", aerial_ue_sub_info.to_string());
    j.end_obj();
  }
  if (nr_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 397);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltea2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 396);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltea2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (a2_xpc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 400);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    a2_xpc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_authorization_status_present) {
    j.start_obj();
    j.write_int("id", 453);
    j.write_str("criticality", "reject");
    j.write_str("value", mobile_iab_authorization_status.to_string());
    j.end_obj();
  }
  if (sl_positioning_ranging_services_info_present) {
    j.start_obj();
    j.write_int("id", 464);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sl_positioning_ranging_services_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNStatusTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_status_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73, 79, 12, 291, 176};
  return map_enum_number(names, 5, idx, "id");
}
bool sn_status_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73, 79, 12, 291, 176};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_status_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::reject;
    case 79:
      return crit_e::reject;
    case 12:
      return crit_e::ignore;
    case 291:
      return crit_e::ignore;
    case 176:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_status_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 79:
      return presence_e::mandatory;
    case 12:
      return presence_e::mandatory;
    case 291:
      return presence_e::optional;
    case 176:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_status_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += cho_cfg_present ? 1 : 0;
  nof_ies += mob_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, target_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 12, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, drbs_subject_to_status_transfer_list, 1, 32, true);
  }));
  if (cho_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 291, crit_e{crit_e::ignore}, [this, &bref]() { return cho_cfg.pack(bref); }));
  }
  if (mob_info_present) {
    HANDLE_CODE(pack_ie(bref, 176, crit_e{crit_e::ignore}, [this, &bref]() { return mob_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_status_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_status_transfer_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 79: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(target_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 12: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(drbs_subject_to_status_transfer_list, bref, 1, 32, true);
        }));
        break;
      }
      case 291: {
        cho_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cho_cfg.unpack(bref); }));
        break;
      }
      case 176: {
        mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_status_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "reject");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 79);
  j.write_str("criticality", "reject");
  j.write_uint("value", target_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 12);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : drbs_subject_to_status_transfer_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (cho_cfg_present) {
    j.start_obj();
    j.write_int("id", 291);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cho_cfg.to_json(j);
    j.end_obj();
  }
  if (mob_info_present) {
    j.start_obj();
    j.write_int("id", 176);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeAdditionRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_addition_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23,  91,  69,  70,  64,  25,  16,  47,  24,  71,  13,  54,  32,  94,
                                   95,  105, 113, 110, 117, 22,  126, 131, 81,  148, 227, 254, 261, 88,
                                   178, 290, 206, 298, 322, 326, 328, 359, 363, 378, 434, 459, 376, 457};
  return map_enum_number(names, 42, idx, "id");
}
bool sn_ode_addition_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23,  91,  69,  70,  64,  25,  16,  47,  24,  71,  13,  54,  32,  94,
                                   95,  105, 113, 110, 117, 22,  126, 131, 81,  148, 227, 254, 261, 88,
                                   178, 290, 206, 298, 322, 326, 328, 359, 363, 378, 434, 459, 376, 457};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_addition_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 91:
      return crit_e::reject;
    case 69:
      return crit_e::reject;
    case 70:
      return crit_e::reject;
    case 64:
      return crit_e::ignore;
    case 25:
      return crit_e::ignore;
    case 16:
      return crit_e::reject;
    case 47:
      return crit_e::reject;
    case 24:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 13:
      return crit_e::ignore;
    case 54:
      return crit_e::reject;
    case 32:
      return crit_e::reject;
    case 94:
      return crit_e::ignore;
    case 95:
      return crit_e::reject;
    case 105:
      return crit_e::reject;
    case 113:
      return crit_e::reject;
    case 110:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 22:
      return crit_e::ignore;
    case 126:
      return crit_e::ignore;
    case 131:
      return crit_e::reject;
    case 81:
      return crit_e::ignore;
    case 148:
      return crit_e::ignore;
    case 227:
      return crit_e::reject;
    case 254:
      return crit_e::ignore;
    case 261:
      return crit_e::ignore;
    case 88:
      return crit_e::ignore;
    case 178:
      return crit_e::ignore;
    case 290:
      return crit_e::ignore;
    case 206:
      return crit_e::reject;
    case 298:
      return crit_e::ignore;
    case 322:
      return crit_e::reject;
    case 326:
      return crit_e::ignore;
    case 328:
      return crit_e::reject;
    case 359:
      return crit_e::reject;
    case 363:
      return crit_e::reject;
    case 378:
      return crit_e::ignore;
    case 434:
      return crit_e::ignore;
    case 459:
      return crit_e::ignore;
    case 376:
      return crit_e::ignore;
    case 457:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_addition_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 91:
      return presence_e::mandatory;
    case 69:
      return presence_e::mandatory;
    case 70:
      return presence_e::mandatory;
    case 64:
      return presence_e::optional;
    case 25:
      return presence_e::optional;
    case 16:
      return presence_e::optional;
    case 47:
      return presence_e::mandatory;
    case 24:
      return presence_e::mandatory;
    case 71:
      return presence_e::optional;
    case 13:
      return presence_e::optional;
    case 54:
      return presence_e::optional;
    case 32:
      return presence_e::optional;
    case 94:
      return presence_e::optional;
    case 95:
      return presence_e::conditional;
    case 105:
      return presence_e::optional;
    case 113:
      return presence_e::optional;
    case 110:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 22:
      return presence_e::optional;
    case 126:
      return presence_e::optional;
    case 131:
      return presence_e::optional;
    case 81:
      return presence_e::optional;
    case 148:
      return presence_e::optional;
    case 227:
      return presence_e::optional;
    case 254:
      return presence_e::optional;
    case 261:
      return presence_e::optional;
    case 88:
      return presence_e::optional;
    case 178:
      return presence_e::optional;
    case 290:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 298:
      return presence_e::optional;
    case 322:
      return presence_e::optional;
    case 326:
      return presence_e::optional;
    case 328:
      return presence_e::optional;
    case 359:
      return presence_e::optional;
    case 363:
      return presence_e::optional;
    case 378:
      return presence_e::optional;
    case 434:
      return presence_e::optional;
    case 459:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 457:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_addition_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 6;
  nof_ies += sel_plmn_present ? 1 : 0;
  nof_ies += mob_restrict_list_present ? 1 : 0;
  nof_ies += idx_to_rat_frequ_sel_prio_present ? 1 : 0;
  nof_ies += s_ng_ran_node_ue_xnap_id_present ? 1 : 0;
  nof_ies += expected_ue_behaviour_present ? 1 : 0;
  nof_ies += requested_split_srb_present ? 1 : 0;
  nof_ies += pcell_id_present ? 1 : 0;
  nof_ies += desired_act_notif_level_present ? 1 : 0;
  nof_ies += available_drb_ids_present ? 1 : 0;
  nof_ies += s_ng_ran_node_max_ip_data_rate_ul_present ? 1 : 0;
  nof_ies += s_ng_ran_node_max_ip_data_rate_dl_present ? 1 : 0;
  nof_ies += location_info_snr_eporting_present ? 1 : 0;
  nof_ies += mr_dc_res_coordination_info_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += ne_dc_tdm_pattern_present ? 1 : 0;
  nof_ies += s_ng_ran_node_addition_trigger_ind_present ? 1 : 0;
  nof_ies += trace_activation_present ? 1 : 0;
  nof_ies += requested_fast_mcg_recovery_via_srb3_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  nof_ies += source_ng_ran_node_id_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_list_present ? 1 : 0;
  nof_ies += ue_history_info_present ? 1 : 0;
  nof_ies += ue_history_info_from_the_ue_present ? 1 : 0;
  nof_ies += pscell_change_history_present ? 1 : 0;
  nof_ies += iab_node_ind_present ? 1 : 0;
  nof_ies += no_pdu_session_ind_present ? 1 : 0;
  nof_ies += ch_oinfo_add_req_present ? 1 : 0;
  nof_ies += scg_activation_request_present ? 1 : 0;
  nof_ies += cp_a_info_request_present ? 1 : 0;
  nof_ies += s_ng_ran_node_ue_slice_mbr_present ? 1 : 0;
  nof_ies += f1_terminating_iab_donor_ind_present ? 1 : 0;
  nof_ies += sel_n_id_present ? 1 : 0;
  nof_ies += q_mcc_oordination_request_present ? 1 : 0;
  nof_ies += source_sn_to_target_sn_qmc_info_present ? 1 : 0;
  nof_ies += iab_authorization_status_present ? 1 : 0;
  nof_ies += source_m_ng_ran_node_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 91, crit_e{crit_e::reject}, [this, &bref]() { return ue_security_cap.pack(bref); }));
  HANDLE_CODE(
      pack_ie(bref, 69, crit_e{crit_e::reject}, [this, &bref]() { return s_ng_ra_nnode_security_key.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 70, crit_e{crit_e::reject}, [this, &bref]() { return s_ng_ran_node_ue_ambr.pack(bref); }));
  if (sel_plmn_present) {
    HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::ignore}, [this, &bref]() { return sel_plmn.pack(bref); }));
  }
  if (mob_restrict_list_present) {
    HANDLE_CODE(pack_ie(bref, 25, crit_e{crit_e::ignore}, [this, &bref]() { return mob_restrict_list.pack(bref); }));
  }
  if (idx_to_rat_frequ_sel_prio_present) {
    HANDLE_CODE(pack_ie(bref, 16, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, idx_to_rat_frequ_sel_prio, (uint16_t)1u, (uint16_t)256u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 47, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_to_be_added_add_req, 1, 256, true);
  }));
  HANDLE_CODE(pack_ie(bref, 24, crit_e{crit_e::reject}, [this, &bref]() { return mn_to_sn_container.pack(bref); }));
  if (s_ng_ran_node_ue_xnap_id_present) {
    HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (expected_ue_behaviour_present) {
    HANDLE_CODE(
        pack_ie(bref, 13, crit_e{crit_e::ignore}, [this, &bref]() { return expected_ue_behaviour.pack(bref); }));
  }
  if (requested_split_srb_present) {
    HANDLE_CODE(pack_ie(bref, 54, crit_e{crit_e::reject}, [this, &bref]() { return requested_split_srb.pack(bref); }));
  }
  if (pcell_id_present) {
    HANDLE_CODE(pack_ie(bref, 32, crit_e{crit_e::reject}, [this, &bref]() { return pcell_id.pack(bref); }));
  }
  if (desired_act_notif_level_present) {
    HANDLE_CODE(
        pack_ie(bref, 94, crit_e{crit_e::ignore}, [this, &bref]() { return desired_act_notif_level.pack(bref); }));
  }
  if (available_drb_ids_present) {
    HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, available_drb_ids, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
    }));
  }
  if (s_ng_ran_node_max_ip_data_rate_ul_present) {
    HANDLE_CODE(pack_ie(bref, 105, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, s_ng_ran_node_max_ip_data_rate_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (s_ng_ran_node_max_ip_data_rate_dl_present) {
    HANDLE_CODE(pack_ie(bref, 113, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, s_ng_ran_node_max_ip_data_rate_dl, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (location_info_snr_eporting_present) {
    HANDLE_CODE(
        pack_ie(bref, 110, crit_e{crit_e::ignore}, [this, &bref]() { return location_info_snr_eporting.pack(bref); }));
  }
  if (mr_dc_res_coordination_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return mr_dc_res_coordination_info.pack(bref); }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 22, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  if (ne_dc_tdm_pattern_present) {
    HANDLE_CODE(pack_ie(bref, 126, crit_e{crit_e::ignore}, [this, &bref]() { return ne_dc_tdm_pattern.pack(bref); }));
  }
  if (s_ng_ran_node_addition_trigger_ind_present) {
    HANDLE_CODE(pack_ie(
        bref, 131, crit_e{crit_e::reject}, [this, &bref]() { return s_ng_ran_node_addition_trigger_ind.pack(bref); }));
  }
  if (trace_activation_present) {
    HANDLE_CODE(pack_ie(bref, 81, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));
  }
  if (requested_fast_mcg_recovery_via_srb3_present) {
    HANDLE_CODE(pack_ie(bref, 148, crit_e{crit_e::ignore}, [this, &bref]() {
      return requested_fast_mcg_recovery_via_srb3.pack(bref);
    }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 227, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }
  if (source_ng_ran_node_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 254, crit_e{crit_e::ignore}, [this, &bref]() { return source_ng_ran_node_id.pack(bref); }));
  }
  if (management_based_mdt_plmn_list_present) {
    HANDLE_CODE(pack_ie(bref, 261, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_list, 1, 16, true);
    }));
  }
  if (ue_history_info_present) {
    HANDLE_CODE(pack_ie(bref, 88, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_history_info, 1, 16, true);
    }));
  }
  if (ue_history_info_from_the_ue_present) {
    HANDLE_CODE(
        pack_ie(bref, 178, crit_e{crit_e::ignore}, [this, &bref]() { return ue_history_info_from_the_ue.pack(bref); }));
  }
  if (pscell_change_history_present) {
    HANDLE_CODE(
        pack_ie(bref, 290, crit_e{crit_e::ignore}, [this, &bref]() { return pscell_change_history.pack(bref); }));
  }
  if (iab_node_ind_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::reject}, [this, &bref]() { return iab_node_ind.pack(bref); }));
  }
  if (no_pdu_session_ind_present) {
    HANDLE_CODE(pack_ie(bref, 298, crit_e{crit_e::ignore}, [this, &bref]() { return no_pdu_session_ind.pack(bref); }));
  }
  if (ch_oinfo_add_req_present) {
    HANDLE_CODE(pack_ie(bref, 322, crit_e{crit_e::reject}, [this, &bref]() { return ch_oinfo_add_req.pack(bref); }));
  }
  if (scg_activation_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 326, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_request.pack(bref); }));
  }
  if (cp_a_info_request_present) {
    HANDLE_CODE(pack_ie(bref, 328, crit_e{crit_e::reject}, [this, &bref]() { return cp_a_info_request.pack(bref); }));
  }
  if (s_ng_ran_node_ue_slice_mbr_present) {
    HANDLE_CODE(pack_ie(bref, 359, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, s_ng_ran_node_ue_slice_mbr, 1, 8, true);
    }));
  }
  if (f1_terminating_iab_donor_ind_present) {
    HANDLE_CODE(pack_ie(
        bref, 363, crit_e{crit_e::reject}, [this, &bref]() { return f1_terminating_iab_donor_ind.pack(bref); }));
  }
  if (sel_n_id_present) {
    HANDLE_CODE(pack_ie(bref, 378, crit_e{crit_e::ignore}, [this, &bref]() { return sel_n_id.pack(bref); }));
  }
  if (q_mcc_oordination_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 434, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_oordination_request.pack(bref); }));
  }
  if (source_sn_to_target_sn_qmc_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 459, crit_e{crit_e::ignore}, [this, &bref]() { return source_sn_to_target_sn_qmc_info.pack(bref); }));
  }
  if (iab_authorization_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 376, crit_e{crit_e::ignore}, [this, &bref]() { return iab_authorization_status.pack(bref); }));
  }
  if (source_m_ng_ran_node_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 457, crit_e{crit_e::ignore}, [this, &bref]() { return source_m_ng_ran_node_id.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_addition_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 6;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_addition_request_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 91: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_security_cap.unpack(bref); }));
        break;
      }
      case 69: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_ng_ra_nnode_security_key.unpack(bref); }));
        break;
      }
      case 70: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_ng_ran_node_ue_ambr.unpack(bref); }));
        break;
      }
      case 64: {
        sel_plmn_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sel_plmn.unpack(bref); }));
        break;
      }
      case 25: {
        mob_restrict_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_restrict_list.unpack(bref); }));
        break;
      }
      case 16: {
        idx_to_rat_frequ_sel_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(idx_to_rat_frequ_sel_prio, bref, (uint16_t)1u, (uint16_t)256u, false, true);
        }));
        break;
      }
      case 47: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_to_be_added_add_req, bref, 1, 256, true); }));
        break;
      }
      case 24: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mn_to_sn_container.unpack(bref); }));
        break;
      }
      case 71: {
        s_ng_ran_node_ue_xnap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 13: {
        expected_ue_behaviour_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return expected_ue_behaviour.unpack(bref); }));
        break;
      }
      case 54: {
        requested_split_srb_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_split_srb.unpack(bref); }));
        break;
      }
      case 32: {
        pcell_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pcell_id.unpack(bref); }));
        break;
      }
      case 94: {
        desired_act_notif_level_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return desired_act_notif_level.unpack(bref); }));
        break;
      }
      case 95: {
        available_drb_ids_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(available_drb_ids, bref, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
        }));
        break;
      }
      case 105: {
        s_ng_ran_node_max_ip_data_rate_ul_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              s_ng_ran_node_max_ip_data_rate_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 113: {
        s_ng_ran_node_max_ip_data_rate_dl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              s_ng_ran_node_max_ip_data_rate_dl, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 110: {
        location_info_snr_eporting_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_info_snr_eporting.unpack(bref); }));
        break;
      }
      case 117: {
        mr_dc_res_coordination_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mr_dc_res_coordination_info.unpack(bref); }));
        break;
      }
      case 22: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 126: {
        ne_dc_tdm_pattern_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ne_dc_tdm_pattern.unpack(bref); }));
        break;
      }
      case 131: {
        s_ng_ran_node_addition_trigger_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_ng_ran_node_addition_trigger_ind.unpack(bref); }));
        break;
      }
      case 81: {
        trace_activation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_activation.unpack(bref); }));
        break;
      }
      case 148: {
        requested_fast_mcg_recovery_via_srb3_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return requested_fast_mcg_recovery_via_srb3.unpack(bref); }));
        break;
      }
      case 227: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 254: {
        source_ng_ran_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 261: {
        management_based_mdt_plmn_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(management_based_mdt_plmn_list, bref, 1, 16, true); }));
        break;
      }
      case 88: {
        ue_history_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(ue_history_info, bref, 1, 16, true); }));
        break;
      }
      case 178: {
        ue_history_info_from_the_ue_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_history_info_from_the_ue.unpack(bref); }));
        break;
      }
      case 290: {
        pscell_change_history_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pscell_change_history.unpack(bref); }));
        break;
      }
      case 206: {
        iab_node_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_node_ind.unpack(bref); }));
        break;
      }
      case 298: {
        no_pdu_session_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return no_pdu_session_ind.unpack(bref); }));
        break;
      }
      case 322: {
        ch_oinfo_add_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ch_oinfo_add_req.unpack(bref); }));
        break;
      }
      case 326: {
        scg_activation_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_request.unpack(bref); }));
        break;
      }
      case 328: {
        cp_a_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cp_a_info_request.unpack(bref); }));
        break;
      }
      case 359: {
        s_ng_ran_node_ue_slice_mbr_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(s_ng_ran_node_ue_slice_mbr, bref, 1, 8, true); }));
        break;
      }
      case 363: {
        f1_terminating_iab_donor_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return f1_terminating_iab_donor_ind.unpack(bref); }));
        break;
      }
      case 378: {
        sel_n_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sel_n_id.unpack(bref); }));
        break;
      }
      case 434: {
        q_mcc_oordination_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_oordination_request.unpack(bref); }));
        break;
      }
      case 459: {
        source_sn_to_target_sn_qmc_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_sn_to_target_sn_qmc_info.unpack(bref); }));
        break;
      }
      case 376: {
        iab_authorization_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_authorization_status.unpack(bref); }));
        break;
      }
      case 457: {
        source_m_ng_ran_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_m_ng_ran_node_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_addition_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 91);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_security_cap.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 69);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  s_ng_ra_nnode_security_key.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 70);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  s_ng_ran_node_ue_ambr.to_json(j);
  j.end_obj();
  if (sel_plmn_present) {
    j.start_obj();
    j.write_int("id", 64);
    j.write_str("criticality", "ignore");
    j.write_str("value", sel_plmn.to_string());
    j.end_obj();
  }
  if (mob_restrict_list_present) {
    j.start_obj();
    j.write_int("id", 25);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_restrict_list.to_json(j);
    j.end_obj();
  }
  if (idx_to_rat_frequ_sel_prio_present) {
    j.start_obj();
    j.write_int("id", 16);
    j.write_str("criticality", "reject");
    j.write_int("value", idx_to_rat_frequ_sel_prio);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 47);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_to_be_added_add_req) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 24);
  j.write_str("criticality", "reject");
  j.write_str("value", mn_to_sn_container.to_string());
  j.end_obj();
  if (s_ng_ran_node_ue_xnap_id_present) {
    j.start_obj();
    j.write_int("id", 71);
    j.write_str("criticality", "reject");
    j.write_uint("value", s_ng_ran_node_ue_xnap_id);
    j.end_obj();
  }
  if (expected_ue_behaviour_present) {
    j.start_obj();
    j.write_int("id", 13);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    expected_ue_behaviour.to_json(j);
    j.end_obj();
  }
  if (requested_split_srb_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "reject");
    j.write_str("value", requested_split_srb.to_string());
    j.end_obj();
  }
  if (pcell_id_present) {
    j.start_obj();
    j.write_int("id", 32);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    pcell_id.to_json(j);
    j.end_obj();
  }
  if (desired_act_notif_level_present) {
    j.start_obj();
    j.write_int("id", 94);
    j.write_str("criticality", "ignore");
    j.write_str("value", desired_act_notif_level.to_string());
    j.end_obj();
  }
  if (available_drb_ids_present) {
    j.start_obj();
    j.write_int("id", 95);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : available_drb_ids) {
      j.write_int(e1);
    }
    j.end_array();
    j.end_obj();
  }
  if (s_ng_ran_node_max_ip_data_rate_ul_present) {
    j.start_obj();
    j.write_int("id", 105);
    j.write_str("criticality", "reject");
    j.write_uint("value", s_ng_ran_node_max_ip_data_rate_ul);
    j.end_obj();
  }
  if (s_ng_ran_node_max_ip_data_rate_dl_present) {
    j.start_obj();
    j.write_int("id", 113);
    j.write_str("criticality", "reject");
    j.write_uint("value", s_ng_ran_node_max_ip_data_rate_dl);
    j.end_obj();
  }
  if (location_info_snr_eporting_present) {
    j.start_obj();
    j.write_int("id", 110);
    j.write_str("criticality", "ignore");
    j.write_str("value", "pSCell");
    j.end_obj();
  }
  if (mr_dc_res_coordination_info_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mr_dc_res_coordination_info.to_json(j);
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 22);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  if (ne_dc_tdm_pattern_present) {
    j.start_obj();
    j.write_int("id", 126);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ne_dc_tdm_pattern.to_json(j);
    j.end_obj();
  }
  if (s_ng_ran_node_addition_trigger_ind_present) {
    j.start_obj();
    j.write_int("id", 131);
    j.write_str("criticality", "reject");
    j.write_str("value", s_ng_ran_node_addition_trigger_ind.to_string());
    j.end_obj();
  }
  if (trace_activation_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    trace_activation.to_json(j);
    j.end_obj();
  }
  if (requested_fast_mcg_recovery_via_srb3_present) {
    j.start_obj();
    j.write_int("id", 148);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 227);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  if (source_ng_ran_node_id_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_ng_ran_node_id.to_json(j);
    j.end_obj();
  }
  if (management_based_mdt_plmn_list_present) {
    j.start_obj();
    j.write_int("id", 261);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_history_info_present) {
    j.start_obj();
    j.write_int("id", 88);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_history_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_history_info_from_the_ue_present) {
    j.start_obj();
    j.write_int("id", 178);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_history_info_from_the_ue.to_json(j);
    j.end_obj();
  }
  if (pscell_change_history_present) {
    j.start_obj();
    j.write_int("id", 290);
    j.write_str("criticality", "ignore");
    j.write_str("value", "reporting-full-history");
    j.end_obj();
  }
  if (iab_node_ind_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (no_pdu_session_ind_present) {
    j.start_obj();
    j.write_int("id", 298);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (ch_oinfo_add_req_present) {
    j.start_obj();
    j.write_int("id", 322);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ch_oinfo_add_req.to_json(j);
    j.end_obj();
  }
  if (scg_activation_request_present) {
    j.start_obj();
    j.write_int("id", 326);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_request.to_string());
    j.end_obj();
  }
  if (cp_a_info_request_present) {
    j.start_obj();
    j.write_int("id", 328);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    cp_a_info_request.to_json(j);
    j.end_obj();
  }
  if (s_ng_ran_node_ue_slice_mbr_present) {
    j.start_obj();
    j.write_int("id", 359);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : s_ng_ran_node_ue_slice_mbr) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (f1_terminating_iab_donor_ind_present) {
    j.start_obj();
    j.write_int("id", 363);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (sel_n_id_present) {
    j.start_obj();
    j.write_int("id", 378);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sel_n_id.to_json(j);
    j.end_obj();
  }
  if (q_mcc_oordination_request_present) {
    j.start_obj();
    j.write_int("id", 434);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_oordination_request.to_json(j);
    j.end_obj();
  }
  if (source_sn_to_target_sn_qmc_info_present) {
    j.start_obj();
    j.write_int("id", 459);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_sn_to_target_sn_qmc_info.to_json(j);
    j.end_obj();
  }
  if (iab_authorization_status_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "ignore");
    j.write_str("value", iab_authorization_status.to_string());
    j.end_obj();
  }
  if (source_m_ng_ran_node_id_present) {
    j.start_obj();
    j.write_int("id", 457);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_m_ng_ran_node_id.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeAdditionRequestAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_addition_request_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 34, 37, 72, 2, 61, 10, 111, 117, 149, 253, 327, 329, 287, 435, 446, 442};
  return map_enum_number(names, 18, idx, "id");
}
bool sn_ode_addition_request_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 34, 37, 72, 2, 61, 10, 111, 117, 149, 253, 327, 329, 287, 435, 446, 442};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_addition_request_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 34:
      return crit_e::ignore;
    case 37:
      return crit_e::ignore;
    case 72:
      return crit_e::reject;
    case 2:
      return crit_e::reject;
    case 61:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    case 111:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 149:
      return crit_e::ignore;
    case 253:
      return crit_e::ignore;
    case 327:
      return crit_e::ignore;
    case 329:
      return crit_e::ignore;
    case 287:
      return crit_e::ignore;
    case 435:
      return crit_e::ignore;
    case 446:
      return crit_e::reject;
    case 442:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_addition_request_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 34:
      return presence_e::mandatory;
    case 37:
      return presence_e::optional;
    case 72:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    case 61:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 111:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 149:
      return presence_e::optional;
    case 253:
      return presence_e::optional;
    case 327:
      return presence_e::optional;
    case 329:
      return presence_e::optional;
    case 287:
      return presence_e::optional;
    case 435:
      return presence_e::optional;
    case 446:
      return presence_e::optional;
    case 442:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_addition_request_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += pdu_session_not_admitted_add_req_ack_present ? 1 : 0;
  nof_ies += admitted_split_srb_present ? 1 : 0;
  nof_ies += rrc_cfg_ind_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += location_info_sn_present ? 1 : 0;
  nof_ies += mr_dc_res_coordination_info_present ? 1 : 0;
  nof_ies += available_fast_mcg_recovery_via_srb3_present ? 1 : 0;
  nof_ies += direct_forwarding_path_availability_present ? 1 : 0;
  nof_ies += scg_activation_status_present ? 1 : 0;
  nof_ies += cp_a_info_ack_present ? 1 : 0;
  nof_ies += sn_mob_info_present ? 1 : 0;
  nof_ies += q_mcc_oordination_resp_present ? 1 : 0;
  nof_ies += ch_oinfo_add_req_ack_present ? 1 : 0;
  nof_ies += direct_forwarding_path_availability_with_source_mn_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 34, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_admitted_added_add_req_ack, 1, 256, true);
  }));
  if (pdu_session_not_admitted_add_req_ack_present) {
    HANDLE_CODE(pack_ie(
        bref, 37, crit_e{crit_e::ignore}, [this, &bref]() { return pdu_session_not_admitted_add_req_ack.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 72, crit_e{crit_e::reject}, [this, &bref]() { return sn_to_mn_container.pack(bref); }));
  if (admitted_split_srb_present) {
    HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() { return admitted_split_srb.pack(bref); }));
  }
  if (rrc_cfg_ind_present) {
    HANDLE_CODE(pack_ie(bref, 61, crit_e{crit_e::reject}, [this, &bref]() { return rrc_cfg_ind.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (location_info_sn_present) {
    HANDLE_CODE(pack_ie(bref, 111, crit_e{crit_e::ignore}, [this, &bref]() { return location_info_sn.pack(bref); }));
  }
  if (mr_dc_res_coordination_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return mr_dc_res_coordination_info.pack(bref); }));
  }
  if (available_fast_mcg_recovery_via_srb3_present) {
    HANDLE_CODE(pack_ie(bref, 149, crit_e{crit_e::ignore}, [this, &bref]() {
      return available_fast_mcg_recovery_via_srb3.pack(bref);
    }));
  }
  if (direct_forwarding_path_availability_present) {
    HANDLE_CODE(pack_ie(
        bref, 253, crit_e{crit_e::ignore}, [this, &bref]() { return direct_forwarding_path_availability.pack(bref); }));
  }
  if (scg_activation_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 327, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_status.pack(bref); }));
  }
  if (cp_a_info_ack_present) {
    HANDLE_CODE(pack_ie(bref, 329, crit_e{crit_e::ignore}, [this, &bref]() { return cp_a_info_ack.pack(bref); }));
  }
  if (sn_mob_info_present) {
    HANDLE_CODE(pack_ie(bref, 287, crit_e{crit_e::ignore}, [this, &bref]() { return sn_mob_info.pack(bref); }));
  }
  if (q_mcc_oordination_resp_present) {
    HANDLE_CODE(
        pack_ie(bref, 435, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_oordination_resp.pack(bref); }));
  }
  if (ch_oinfo_add_req_ack_present) {
    HANDLE_CODE(
        pack_ie(bref, 446, crit_e{crit_e::reject}, [this, &bref]() { return ch_oinfo_add_req_ack.pack(bref); }));
  }
  if (direct_forwarding_path_availability_with_source_mn_present) {
    HANDLE_CODE(pack_ie(bref, 442, crit_e{crit_e::ignore}, [this, &bref]() {
      return direct_forwarding_path_availability_with_source_mn.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_addition_request_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_addition_request_ack_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 34: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_admitted_added_add_req_ack, bref, 1, 256, true);
        }));
        break;
      }
      case 37: {
        pdu_session_not_admitted_add_req_ack_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_not_admitted_add_req_ack.unpack(bref); }));
        break;
      }
      case 72: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_to_mn_container.unpack(bref); }));
        break;
      }
      case 2: {
        admitted_split_srb_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return admitted_split_srb.unpack(bref); }));
        break;
      }
      case 61: {
        rrc_cfg_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_cfg_ind.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 111: {
        location_info_sn_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_info_sn.unpack(bref); }));
        break;
      }
      case 117: {
        mr_dc_res_coordination_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mr_dc_res_coordination_info.unpack(bref); }));
        break;
      }
      case 149: {
        available_fast_mcg_recovery_via_srb3_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return available_fast_mcg_recovery_via_srb3.unpack(bref); }));
        break;
      }
      case 253: {
        direct_forwarding_path_availability_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return direct_forwarding_path_availability.unpack(bref); }));
        break;
      }
      case 327: {
        scg_activation_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_status.unpack(bref); }));
        break;
      }
      case 329: {
        cp_a_info_ack_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cp_a_info_ack.unpack(bref); }));
        break;
      }
      case 287: {
        sn_mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_mob_info.unpack(bref); }));
        break;
      }
      case 435: {
        q_mcc_oordination_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_oordination_resp.unpack(bref); }));
        break;
      }
      case 446: {
        ch_oinfo_add_req_ack_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ch_oinfo_add_req_ack.unpack(bref); }));
        break;
      }
      case 442: {
        direct_forwarding_path_availability_with_source_mn_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return direct_forwarding_path_availability_with_source_mn.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_addition_request_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 34);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_admitted_added_add_req_ack) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (pdu_session_not_admitted_add_req_ack_present) {
    j.start_obj();
    j.write_int("id", 37);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_not_admitted_add_req_ack.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 72);
  j.write_str("criticality", "reject");
  j.write_str("value", sn_to_mn_container.to_string());
  j.end_obj();
  if (admitted_split_srb_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "reject");
    j.write_str("value", admitted_split_srb.to_string());
    j.end_obj();
  }
  if (rrc_cfg_ind_present) {
    j.start_obj();
    j.write_int("id", 61);
    j.write_str("criticality", "reject");
    j.write_str("value", rrc_cfg_ind.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (location_info_sn_present) {
    j.start_obj();
    j.write_int("id", 111);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    location_info_sn.to_json(j);
    j.end_obj();
  }
  if (mr_dc_res_coordination_info_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mr_dc_res_coordination_info.to_json(j);
    j.end_obj();
  }
  if (available_fast_mcg_recovery_via_srb3_present) {
    j.start_obj();
    j.write_int("id", 149);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (direct_forwarding_path_availability_present) {
    j.start_obj();
    j.write_int("id", 253);
    j.write_str("criticality", "ignore");
    j.write_str("value", "direct-path-available");
    j.end_obj();
  }
  if (scg_activation_status_present) {
    j.start_obj();
    j.write_int("id", 327);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_status.to_string());
    j.end_obj();
  }
  if (cp_a_info_ack_present) {
    j.start_obj();
    j.write_int("id", 329);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cp_a_info_ack.to_json(j);
    j.end_obj();
  }
  if (sn_mob_info_present) {
    j.start_obj();
    j.write_int("id", 287);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sn_mob_info.to_json(j);
    j.end_obj();
  }
  if (q_mcc_oordination_resp_present) {
    j.start_obj();
    j.write_int("id", 435);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_oordination_resp.to_json(j);
    j.end_obj();
  }
  if (ch_oinfo_add_req_ack_present) {
    j.start_obj();
    j.write_int("id", 446);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ch_oinfo_add_req_ack.to_json(j);
    j.end_obj();
  }
  if (direct_forwarding_path_availability_with_source_mn_present) {
    j.start_obj();
    j.write_int("id", 442);
    j.write_str("criticality", "ignore");
    j.write_str("value", "direct-path-available");
    j.end_obj();
  }
  j.end_array();
}

// SNodeAdditionRequestReject-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_addition_request_reject_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 7, 10, 32};
  return map_enum_number(names, 5, idx, "id");
}
bool sn_ode_addition_request_reject_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 7, 10, 32};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_addition_request_reject_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 32:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_addition_request_reject_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 32:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_addition_request_reject_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += pcell_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (pcell_id_present) {
    HANDLE_CODE(pack_ie(bref, 32, crit_e{crit_e::reject}, [this, &bref]() { return pcell_id.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_addition_request_reject_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_addition_request_reject_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 32: {
        pcell_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pcell_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_addition_request_reject_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (pcell_id_present) {
    j.start_obj();
    j.write_int("id", 32);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    pcell_id.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeChangeConfirm-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_change_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 45, 10, 331, 24};
  return map_enum_number(names, 6, idx, "id");
}
bool sn_ode_change_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 45, 10, 331, 24};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_change_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 45:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 331:
      return crit_e::ignore;
    case 24:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_change_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 45:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 331:
      return presence_e::optional;
    case 24:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_change_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_sn_change_confirm_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += c_pci_nformation_confirm_present ? 1 : 0;
  nof_ies += mn_to_sn_container_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_sn_change_confirm_list_present) {
    HANDLE_CODE(pack_ie(bref, 45, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_sn_change_confirm_list, 1, 256, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (c_pci_nformation_confirm_present) {
    HANDLE_CODE(
        pack_ie(bref, 331, crit_e{crit_e::ignore}, [this, &bref]() { return c_pci_nformation_confirm.pack(bref); }));
  }
  if (mn_to_sn_container_present) {
    HANDLE_CODE(pack_ie(bref, 24, crit_e{crit_e::ignore}, [this, &bref]() { return mn_to_sn_container.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_change_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_change_confirm_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 45: {
        pdu_session_sn_change_confirm_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_sn_change_confirm_list, bref, 1, 256, true);
        }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 331: {
        c_pci_nformation_confirm_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return c_pci_nformation_confirm.unpack(bref); }));
        break;
      }
      case 24: {
        mn_to_sn_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mn_to_sn_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_change_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (pdu_session_sn_change_confirm_list_present) {
    j.start_obj();
    j.write_int("id", 45);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_sn_change_confirm_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (c_pci_nformation_confirm_present) {
    j.start_obj();
    j.write_int("id", 331);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    c_pci_nformation_confirm.to_json(j);
    j.end_obj();
  }
  if (mn_to_sn_container_present) {
    j.start_obj();
    j.write_int("id", 24);
    j.write_str("criticality", "ignore");
    j.write_str("value", mn_to_sn_container.to_string());
    j.end_obj();
  }
  j.end_array();
}

// SNodeChangeRefuse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_change_refuse_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 7, 10};
  return map_enum_number(names, 4, idx, "id");
}
bool sn_ode_change_refuse_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 7, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_change_refuse_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_change_refuse_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_change_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_change_refuse_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_change_refuse_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_change_refuse_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeChangeRequired-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_change_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 80, 7, 46, 72, 279, 287, 288, 330, 459};
  return map_enum_number(names, 11, idx, "id");
}
bool sn_ode_change_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 80, 7, 46, 72, 279, 287, 288, 330, 459};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_change_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 80:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 46:
      return crit_e::ignore;
    case 72:
      return crit_e::reject;
    case 279:
      return crit_e::ignore;
    case 287:
      return crit_e::ignore;
    case 288:
      return crit_e::ignore;
    case 330:
      return crit_e::ignore;
    case 459:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_change_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 80:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 46:
      return presence_e::optional;
    case 72:
      return presence_e::mandatory;
    case 279:
      return presence_e::optional;
    case 287:
      return presence_e::optional;
    case 288:
      return presence_e::optional;
    case 330:
      return presence_e::optional;
    case 459:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_change_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += pdu_session_sn_change_required_list_present ? 1 : 0;
  nof_ies += scg_ue_history_info_present ? 1 : 0;
  nof_ies += sn_mob_info_present ? 1 : 0;
  nof_ies += source_pscell_id_present ? 1 : 0;
  nof_ies += c_pci_nformation_required_present ? 1 : 0;
  nof_ies += source_sn_to_target_sn_qmc_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 80, crit_e{crit_e::reject}, [this, &bref]() { return target_s_ng_ran_node_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (pdu_session_sn_change_required_list_present) {
    HANDLE_CODE(pack_ie(bref, 46, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_sn_change_required_list, 1, 256, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 72, crit_e{crit_e::reject}, [this, &bref]() { return sn_to_mn_container.pack(bref); }));
  if (scg_ue_history_info_present) {
    HANDLE_CODE(pack_ie(bref, 279, crit_e{crit_e::ignore}, [this, &bref]() { return scg_ue_history_info.pack(bref); }));
  }
  if (sn_mob_info_present) {
    HANDLE_CODE(pack_ie(bref, 287, crit_e{crit_e::ignore}, [this, &bref]() { return sn_mob_info.pack(bref); }));
  }
  if (source_pscell_id_present) {
    HANDLE_CODE(pack_ie(bref, 288, crit_e{crit_e::ignore}, [this, &bref]() { return source_pscell_id.pack(bref); }));
  }
  if (c_pci_nformation_required_present) {
    HANDLE_CODE(
        pack_ie(bref, 330, crit_e{crit_e::ignore}, [this, &bref]() { return c_pci_nformation_required.pack(bref); }));
  }
  if (source_sn_to_target_sn_qmc_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 459, crit_e{crit_e::ignore}, [this, &bref]() { return source_sn_to_target_sn_qmc_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_change_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_change_required_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 80: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_s_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 46: {
        pdu_session_sn_change_required_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_sn_change_required_list, bref, 1, 256, true);
        }));
        break;
      }
      case 72: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_to_mn_container.unpack(bref); }));
        break;
      }
      case 279: {
        scg_ue_history_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_ue_history_info.unpack(bref); }));
        break;
      }
      case 287: {
        sn_mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_mob_info.unpack(bref); }));
        break;
      }
      case 288: {
        source_pscell_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_pscell_id.unpack(bref); }));
        break;
      }
      case 330: {
        c_pci_nformation_required_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return c_pci_nformation_required.unpack(bref); }));
        break;
      }
      case 459: {
        source_sn_to_target_sn_qmc_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_sn_to_target_sn_qmc_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_change_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 80);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  target_s_ng_ran_node_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (pdu_session_sn_change_required_list_present) {
    j.start_obj();
    j.write_int("id", 46);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_sn_change_required_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 72);
  j.write_str("criticality", "reject");
  j.write_str("value", sn_to_mn_container.to_string());
  j.end_obj();
  if (scg_ue_history_info_present) {
    j.start_obj();
    j.write_int("id", 279);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    scg_ue_history_info.to_json(j);
    j.end_obj();
  }
  if (sn_mob_info_present) {
    j.start_obj();
    j.write_int("id", 287);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sn_mob_info.to_json(j);
    j.end_obj();
  }
  if (source_pscell_id_present) {
    j.start_obj();
    j.write_int("id", 288);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_pscell_id.to_json(j);
    j.end_obj();
  }
  if (c_pci_nformation_required_present) {
    j.start_obj();
    j.write_int("id", 330);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    c_pci_nformation_required.to_json(j);
    j.end_obj();
  }
  if (source_sn_to_target_sn_qmc_info_present) {
    j.start_obj();
    j.write_int("id", 459);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_sn_to_target_sn_qmc_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeCounterCheckRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_counter_check_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 6};
  return map_enum_number(names, 3, idx, "id");
}
bool sn_ode_counter_check_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 6};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_counter_check_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 6:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_counter_check_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 6:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_counter_check_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 6, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, bearers_subject_to_counter_check, 1, 32, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_counter_check_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_counter_check_request_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(bearers_subject_to_counter_check, bref, 1, 32, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_counter_check_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 6);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : bearers_subject_to_counter_check) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// SNodeModificationConfirm-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_mod_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 35, 40, 24, 96, 10, 117, 435};
  return map_enum_number(names, 9, idx, "id");
}
bool sn_ode_mod_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 35, 40, 24, 96, 10, 117, 435};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_mod_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 35:
      return crit_e::ignore;
    case 40:
      return crit_e::ignore;
    case 24:
      return crit_e::ignore;
    case 96:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 435:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_mod_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 35:
      return presence_e::optional;
    case 40:
      return presence_e::optional;
    case 24:
      return presence_e::optional;
    case 96:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 435:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_mod_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_admitted_mod_sn_mod_confirm_present ? 1 : 0;
  nof_ies += pdu_session_released_sn_mod_confirm_present ? 1 : 0;
  nof_ies += mn_to_sn_container_present ? 1 : 0;
  nof_ies += add_drb_ids_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += mr_dc_res_coordination_info_present ? 1 : 0;
  nof_ies += q_mcc_oordination_resp_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_admitted_mod_sn_mod_confirm_present) {
    HANDLE_CODE(pack_ie(bref, 35, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_admitted_mod_sn_mod_confirm, 1, 256, true);
    }));
  }
  if (pdu_session_released_sn_mod_confirm_present) {
    HANDLE_CODE(pack_ie(
        bref, 40, crit_e{crit_e::ignore}, [this, &bref]() { return pdu_session_released_sn_mod_confirm.pack(bref); }));
  }
  if (mn_to_sn_container_present) {
    HANDLE_CODE(pack_ie(bref, 24, crit_e{crit_e::ignore}, [this, &bref]() { return mn_to_sn_container.pack(bref); }));
  }
  if (add_drb_ids_present) {
    HANDLE_CODE(pack_ie(bref, 96, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, add_drb_ids, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (mr_dc_res_coordination_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return mr_dc_res_coordination_info.pack(bref); }));
  }
  if (q_mcc_oordination_resp_present) {
    HANDLE_CODE(
        pack_ie(bref, 435, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_oordination_resp.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_mod_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_mod_confirm_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 35: {
        pdu_session_admitted_mod_sn_mod_confirm_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_admitted_mod_sn_mod_confirm, bref, 1, 256, true);
        }));
        break;
      }
      case 40: {
        pdu_session_released_sn_mod_confirm_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_released_sn_mod_confirm.unpack(bref); }));
        break;
      }
      case 24: {
        mn_to_sn_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mn_to_sn_container.unpack(bref); }));
        break;
      }
      case 96: {
        add_drb_ids_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(add_drb_ids, bref, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
        }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 117: {
        mr_dc_res_coordination_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mr_dc_res_coordination_info.unpack(bref); }));
        break;
      }
      case 435: {
        q_mcc_oordination_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_oordination_resp.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_mod_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (pdu_session_admitted_mod_sn_mod_confirm_present) {
    j.start_obj();
    j.write_int("id", 35);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_admitted_mod_sn_mod_confirm) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_released_sn_mod_confirm_present) {
    j.start_obj();
    j.write_int("id", 40);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_released_sn_mod_confirm.to_json(j);
    j.end_obj();
  }
  if (mn_to_sn_container_present) {
    j.start_obj();
    j.write_int("id", 24);
    j.write_str("criticality", "ignore");
    j.write_str("value", mn_to_sn_container.to_string());
    j.end_obj();
  }
  if (add_drb_ids_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : add_drb_ids) {
      j.write_int(e1);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (mr_dc_res_coordination_info_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mr_dc_res_coordination_info.to_json(j);
    j.end_obj();
  }
  if (q_mcc_oordination_resp_present) {
    j.start_obj();
    j.write_int("id", 435);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_oordination_resp.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeModificationRefuse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_mod_refuse_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 7, 24, 10};
  return map_enum_number(names, 5, idx, "id");
}
bool sn_ode_mod_refuse_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 7, 24, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_mod_refuse_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 24:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_mod_refuse_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 24:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_mod_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += mn_to_sn_container_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (mn_to_sn_container_present) {
    HANDLE_CODE(pack_ie(bref, 24, crit_e{crit_e::ignore}, [this, &bref]() { return mn_to_sn_container.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_mod_refuse_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_mod_refuse_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 24: {
        mn_to_sn_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mn_to_sn_container.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_mod_refuse_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (mn_to_sn_container_present) {
    j.start_obj();
    j.write_int("id", 24);
    j.write_str("criticality", "ignore");
    j.write_str("value", mn_to_sn_container.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextInfo-SNModRequest ::= SEQUENCE
OCUDUASN_CODE ue_context_info_sn_mod_request_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ue_security_cap_present, 1));
  HANDLE_CODE(bref.pack(s_ng_ra_nnode_security_key_present, 1));
  HANDLE_CODE(bref.pack(s_ng_ra_nnode_ue_ambr_present, 1));
  HANDLE_CODE(bref.pack(idx_to_rat_freq_sel_prio_present, 1));
  HANDLE_CODE(bref.pack(lower_layer_presence_status_change_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_res_to_be_added.size() > 0, 1));
  HANDLE_CODE(bref.pack(pdu_session_res_to_be_modified.size() > 0, 1));
  HANDLE_CODE(bref.pack(pdu_session_res_to_be_released_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (ue_security_cap_present) {
    HANDLE_CODE(ue_security_cap.pack(bref));
  }
  if (s_ng_ra_nnode_security_key_present) {
    HANDLE_CODE(s_ng_ra_nnode_security_key.pack(bref));
  }
  if (s_ng_ra_nnode_ue_ambr_present) {
    HANDLE_CODE(s_ng_ra_nnode_ue_ambr.pack(bref));
  }
  if (idx_to_rat_freq_sel_prio_present) {
    HANDLE_CODE(pack_integer(bref, idx_to_rat_freq_sel_prio, (uint16_t)1u, (uint16_t)256u, false, true));
  }
  if (lower_layer_presence_status_change_present) {
    HANDLE_CODE(lower_layer_presence_status_change.pack(bref));
  }
  if (pdu_session_res_to_be_added.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_res_to_be_added, 1, 256, true));
  }
  if (pdu_session_res_to_be_modified.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_res_to_be_modified, 1, 256, true));
  }
  if (pdu_session_res_to_be_released_present) {
    HANDLE_CODE(pdu_session_res_to_be_released.pack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_info_sn_mod_request_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ue_security_cap_present, 1));
  HANDLE_CODE(bref.unpack(s_ng_ra_nnode_security_key_present, 1));
  HANDLE_CODE(bref.unpack(s_ng_ra_nnode_ue_ambr_present, 1));
  HANDLE_CODE(bref.unpack(idx_to_rat_freq_sel_prio_present, 1));
  HANDLE_CODE(bref.unpack(lower_layer_presence_status_change_present, 1));
  bool pdu_session_res_to_be_added_present;
  HANDLE_CODE(bref.unpack(pdu_session_res_to_be_added_present, 1));
  bool pdu_session_res_to_be_modified_present;
  HANDLE_CODE(bref.unpack(pdu_session_res_to_be_modified_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_res_to_be_released_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (ue_security_cap_present) {
    HANDLE_CODE(ue_security_cap.unpack(bref));
  }
  if (s_ng_ra_nnode_security_key_present) {
    HANDLE_CODE(s_ng_ra_nnode_security_key.unpack(bref));
  }
  if (s_ng_ra_nnode_ue_ambr_present) {
    HANDLE_CODE(s_ng_ra_nnode_ue_ambr.unpack(bref));
  }
  if (idx_to_rat_freq_sel_prio_present) {
    HANDLE_CODE(unpack_integer(idx_to_rat_freq_sel_prio, bref, (uint16_t)1u, (uint16_t)256u, false, true));
  }
  if (lower_layer_presence_status_change_present) {
    HANDLE_CODE(lower_layer_presence_status_change.unpack(bref));
  }
  if (pdu_session_res_to_be_added_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_session_res_to_be_added, bref, 1, 256, true));
  }
  if (pdu_session_res_to_be_modified_present) {
    HANDLE_CODE(unpack_dyn_seq_of(pdu_session_res_to_be_modified, bref, 1, 256, true));
  }
  if (pdu_session_res_to_be_released_present) {
    HANDLE_CODE(pdu_session_res_to_be_released.unpack(bref));
  }
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_info_sn_mod_request_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ue_security_cap_present) {
    j.write_fieldname("ueSecurityCapabilities");
    ue_security_cap.to_json(j);
  }
  if (s_ng_ra_nnode_security_key_present) {
    j.write_fieldname("s-ng-RANnode-SecurityKey");
    s_ng_ra_nnode_security_key.to_json(j);
  }
  if (s_ng_ra_nnode_ue_ambr_present) {
    j.write_fieldname("s-ng-RANnodeUE-AMBR");
    s_ng_ra_nnode_ue_ambr.to_json(j);
  }
  if (idx_to_rat_freq_sel_prio_present) {
    j.write_int("indexToRatFrequencySelectionPriority", idx_to_rat_freq_sel_prio);
  }
  if (lower_layer_presence_status_change_present) {
    j.write_str("lowerLayerPresenceStatusChange", lower_layer_presence_status_change.to_string());
  }
  if (pdu_session_res_to_be_added.size() > 0) {
    j.start_array("pduSessionResourceToBeAdded");
    for (const auto& e1 : pdu_session_res_to_be_added) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdu_session_res_to_be_modified.size() > 0) {
    j.start_array("pduSessionResourceToBeModified");
    for (const auto& e1 : pdu_session_res_to_be_modified) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdu_session_res_to_be_released_present) {
    j.write_fieldname("pduSessionResourceToBeReleased");
    pdu_session_res_to_be_released.to_json(j);
  }
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// Src-SN-to-Tgt-SNQMCInfoInquiry ::= ENUMERATED
const char* src_sn_to_tgt_sn_qmc_info_inquiry_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "src_sn_to_tgt_sn_qmc_info_inquiry_e");
}

// SNodeModificationRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23,  71,  7,   33,  64,  25,  63,  85,  24,  54,  55,  94,
                                   96,  105, 113, 110, 117, 32,  126, 148, 150, 229, 260, 293,
                                   178, 323, 326, 332, 335, 359, 362, 378, 434, 441, 376};
  return map_enum_number(names, 35, idx, "id");
}
bool sn_ode_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23,  71,  7,   33,  64,  25,  63,  85,  24,  54,  55,  94,
                                   96,  105, 113, 110, 117, 32,  126, 148, 150, 229, 260, 293,
                                   178, 323, 326, 332, 335, 359, 362, 378, 434, 441, 376};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 33:
      return crit_e::ignore;
    case 64:
      return crit_e::ignore;
    case 25:
      return crit_e::ignore;
    case 63:
      return crit_e::ignore;
    case 85:
      return crit_e::reject;
    case 24:
      return crit_e::ignore;
    case 54:
      return crit_e::ignore;
    case 55:
      return crit_e::ignore;
    case 94:
      return crit_e::ignore;
    case 96:
      return crit_e::reject;
    case 105:
      return crit_e::reject;
    case 113:
      return crit_e::reject;
    case 110:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 32:
      return crit_e::reject;
    case 126:
      return crit_e::ignore;
    case 148:
      return crit_e::ignore;
    case 150:
      return crit_e::ignore;
    case 229:
      return crit_e::ignore;
    case 260:
      return crit_e::ignore;
    case 293:
      return crit_e::ignore;
    case 178:
      return crit_e::ignore;
    case 323:
      return crit_e::ignore;
    case 326:
      return crit_e::ignore;
    case 332:
      return crit_e::ignore;
    case 335:
      return crit_e::ignore;
    case 359:
      return crit_e::ignore;
    case 362:
      return crit_e::ignore;
    case 378:
      return crit_e::ignore;
    case 434:
      return crit_e::ignore;
    case 441:
      return crit_e::ignore;
    case 376:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 33:
      return presence_e::optional;
    case 64:
      return presence_e::optional;
    case 25:
      return presence_e::optional;
    case 63:
      return presence_e::optional;
    case 85:
      return presence_e::optional;
    case 24:
      return presence_e::optional;
    case 54:
      return presence_e::optional;
    case 55:
      return presence_e::optional;
    case 94:
      return presence_e::optional;
    case 96:
      return presence_e::optional;
    case 105:
      return presence_e::optional;
    case 113:
      return presence_e::optional;
    case 110:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 32:
      return presence_e::optional;
    case 126:
      return presence_e::optional;
    case 148:
      return presence_e::optional;
    case 150:
      return presence_e::optional;
    case 229:
      return presence_e::optional;
    case 260:
      return presence_e::optional;
    case 293:
      return presence_e::optional;
    case 178:
      return presence_e::optional;
    case 323:
      return presence_e::optional;
    case 326:
      return presence_e::optional;
    case 332:
      return presence_e::optional;
    case 335:
      return presence_e::optional;
    case 359:
      return presence_e::optional;
    case 362:
      return presence_e::optional;
    case 378:
      return presence_e::optional;
    case 434:
      return presence_e::optional;
    case 441:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pdcp_change_ind_present ? 1 : 0;
  nof_ies += sel_plmn_present ? 1 : 0;
  nof_ies += mob_restrict_list_present ? 1 : 0;
  nof_ies += scg_cfg_query_present ? 1 : 0;
  nof_ies += ue_context_info_sn_mod_request_present ? 1 : 0;
  nof_ies += mn_to_sn_container_present ? 1 : 0;
  nof_ies += requested_split_srb_present ? 1 : 0;
  nof_ies += requested_split_srb_release_present ? 1 : 0;
  nof_ies += desired_act_notif_level_present ? 1 : 0;
  nof_ies += add_drb_ids_present ? 1 : 0;
  nof_ies += s_ng_ran_node_max_ip_data_rate_ul_present ? 1 : 0;
  nof_ies += s_ng_ran_node_max_ip_data_rate_dl_present ? 1 : 0;
  nof_ies += location_info_snr_eporting_present ? 1 : 0;
  nof_ies += mr_dc_res_coordination_info_present ? 1 : 0;
  nof_ies += pcell_id_present ? 1 : 0;
  nof_ies += ne_dc_tdm_pattern_present ? 1 : 0;
  nof_ies += requested_fast_mcg_recovery_via_srb3_present ? 1 : 0;
  nof_ies += requested_fast_mcg_recovery_via_srb3_release_present ? 1 : 0;
  nof_ies += sn_triggered_present ? 1 : 0;
  nof_ies += target_node_id_present ? 1 : 0;
  nof_ies += pscell_history_info_retrieve_present ? 1 : 0;
  nof_ies += ue_history_info_from_the_ue_present ? 1 : 0;
  nof_ies += ch_oinfo_mod_req_present ? 1 : 0;
  nof_ies += scg_activation_request_present ? 1 : 0;
  nof_ies += cp_a_info_mod_req_present ? 1 : 0;
  nof_ies += c_pci_nformation_upd_present ? 1 : 0;
  nof_ies += s_ng_ran_node_ue_slice_mbr_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_mod_list_present ? 1 : 0;
  nof_ies += sel_n_id_present ? 1 : 0;
  nof_ies += q_mcc_oordination_request_present ? 1 : 0;
  nof_ies += src_sn_to_tgt_sn_qmc_info_inquiry_present ? 1 : 0;
  nof_ies += iab_authorization_status_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (pdcp_change_ind_present) {
    HANDLE_CODE(pack_ie(bref, 33, crit_e{crit_e::ignore}, [this, &bref]() { return pdcp_change_ind.pack(bref); }));
  }
  if (sel_plmn_present) {
    HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::ignore}, [this, &bref]() { return sel_plmn.pack(bref); }));
  }
  if (mob_restrict_list_present) {
    HANDLE_CODE(pack_ie(bref, 25, crit_e{crit_e::ignore}, [this, &bref]() { return mob_restrict_list.pack(bref); }));
  }
  if (scg_cfg_query_present) {
    HANDLE_CODE(pack_ie(bref, 63, crit_e{crit_e::ignore}, [this, &bref]() { return scg_cfg_query.pack(bref); }));
  }
  if (ue_context_info_sn_mod_request_present) {
    HANDLE_CODE(pack_ie(
        bref, 85, crit_e{crit_e::reject}, [this, &bref]() { return ue_context_info_sn_mod_request.pack(bref); }));
  }
  if (mn_to_sn_container_present) {
    HANDLE_CODE(pack_ie(bref, 24, crit_e{crit_e::ignore}, [this, &bref]() { return mn_to_sn_container.pack(bref); }));
  }
  if (requested_split_srb_present) {
    HANDLE_CODE(pack_ie(bref, 54, crit_e{crit_e::ignore}, [this, &bref]() { return requested_split_srb.pack(bref); }));
  }
  if (requested_split_srb_release_present) {
    HANDLE_CODE(
        pack_ie(bref, 55, crit_e{crit_e::ignore}, [this, &bref]() { return requested_split_srb_release.pack(bref); }));
  }
  if (desired_act_notif_level_present) {
    HANDLE_CODE(
        pack_ie(bref, 94, crit_e{crit_e::ignore}, [this, &bref]() { return desired_act_notif_level.pack(bref); }));
  }
  if (add_drb_ids_present) {
    HANDLE_CODE(pack_ie(bref, 96, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, add_drb_ids, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
    }));
  }
  if (s_ng_ran_node_max_ip_data_rate_ul_present) {
    HANDLE_CODE(pack_ie(bref, 105, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, s_ng_ran_node_max_ip_data_rate_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (s_ng_ran_node_max_ip_data_rate_dl_present) {
    HANDLE_CODE(pack_ie(bref, 113, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, s_ng_ran_node_max_ip_data_rate_dl, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
    }));
  }
  if (location_info_snr_eporting_present) {
    HANDLE_CODE(
        pack_ie(bref, 110, crit_e{crit_e::ignore}, [this, &bref]() { return location_info_snr_eporting.pack(bref); }));
  }
  if (mr_dc_res_coordination_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return mr_dc_res_coordination_info.pack(bref); }));
  }
  if (pcell_id_present) {
    HANDLE_CODE(pack_ie(bref, 32, crit_e{crit_e::reject}, [this, &bref]() { return pcell_id.pack(bref); }));
  }
  if (ne_dc_tdm_pattern_present) {
    HANDLE_CODE(pack_ie(bref, 126, crit_e{crit_e::ignore}, [this, &bref]() { return ne_dc_tdm_pattern.pack(bref); }));
  }
  if (requested_fast_mcg_recovery_via_srb3_present) {
    HANDLE_CODE(pack_ie(bref, 148, crit_e{crit_e::ignore}, [this, &bref]() {
      return requested_fast_mcg_recovery_via_srb3.pack(bref);
    }));
  }
  if (requested_fast_mcg_recovery_via_srb3_release_present) {
    HANDLE_CODE(pack_ie(bref, 150, crit_e{crit_e::ignore}, [this, &bref]() {
      return requested_fast_mcg_recovery_via_srb3_release.pack(bref);
    }));
  }
  if (sn_triggered_present) {
    HANDLE_CODE(pack_ie(bref, 229, crit_e{crit_e::ignore}, [this, &bref]() { return sn_triggered.pack(bref); }));
  }
  if (target_node_id_present) {
    HANDLE_CODE(pack_ie(bref, 260, crit_e{crit_e::ignore}, [this, &bref]() { return target_node_id.pack(bref); }));
  }
  if (pscell_history_info_retrieve_present) {
    HANDLE_CODE(pack_ie(
        bref, 293, crit_e{crit_e::ignore}, [this, &bref]() { return pscell_history_info_retrieve.pack(bref); }));
  }
  if (ue_history_info_from_the_ue_present) {
    HANDLE_CODE(
        pack_ie(bref, 178, crit_e{crit_e::ignore}, [this, &bref]() { return ue_history_info_from_the_ue.pack(bref); }));
  }
  if (ch_oinfo_mod_req_present) {
    HANDLE_CODE(pack_ie(bref, 323, crit_e{crit_e::ignore}, [this, &bref]() { return ch_oinfo_mod_req.pack(bref); }));
  }
  if (scg_activation_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 326, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_request.pack(bref); }));
  }
  if (cp_a_info_mod_req_present) {
    HANDLE_CODE(pack_ie(bref, 332, crit_e{crit_e::ignore}, [this, &bref]() { return cp_a_info_mod_req.pack(bref); }));
  }
  if (c_pci_nformation_upd_present) {
    HANDLE_CODE(
        pack_ie(bref, 335, crit_e{crit_e::ignore}, [this, &bref]() { return c_pci_nformation_upd.pack(bref); }));
  }
  if (s_ng_ran_node_ue_slice_mbr_present) {
    HANDLE_CODE(pack_ie(bref, 359, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, s_ng_ran_node_ue_slice_mbr, 1, 8, true);
    }));
  }
  if (management_based_mdt_plmn_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 362, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_mod_list, 0, 16, true);
    }));
  }
  if (sel_n_id_present) {
    HANDLE_CODE(pack_ie(bref, 378, crit_e{crit_e::ignore}, [this, &bref]() { return sel_n_id.pack(bref); }));
  }
  if (q_mcc_oordination_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 434, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_oordination_request.pack(bref); }));
  }
  if (src_sn_to_tgt_sn_qmc_info_inquiry_present) {
    HANDLE_CODE(pack_ie(
        bref, 441, crit_e{crit_e::ignore}, [this, &bref]() { return src_sn_to_tgt_sn_qmc_info_inquiry.pack(bref); }));
  }
  if (iab_authorization_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 376, crit_e{crit_e::ignore}, [this, &bref]() { return iab_authorization_status.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_mod_request_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 33: {
        pdcp_change_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pdcp_change_ind.unpack(bref); }));
        break;
      }
      case 64: {
        sel_plmn_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sel_plmn.unpack(bref); }));
        break;
      }
      case 25: {
        mob_restrict_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_restrict_list.unpack(bref); }));
        break;
      }
      case 63: {
        scg_cfg_query_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_cfg_query.unpack(bref); }));
        break;
      }
      case 85: {
        ue_context_info_sn_mod_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_info_sn_mod_request.unpack(bref); }));
        break;
      }
      case 24: {
        mn_to_sn_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mn_to_sn_container.unpack(bref); }));
        break;
      }
      case 54: {
        requested_split_srb_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_split_srb.unpack(bref); }));
        break;
      }
      case 55: {
        requested_split_srb_release_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_split_srb_release.unpack(bref); }));
        break;
      }
      case 94: {
        desired_act_notif_level_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return desired_act_notif_level.unpack(bref); }));
        break;
      }
      case 96: {
        add_drb_ids_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(add_drb_ids, bref, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
        }));
        break;
      }
      case 105: {
        s_ng_ran_node_max_ip_data_rate_ul_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              s_ng_ran_node_max_ip_data_rate_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 113: {
        s_ng_ran_node_max_ip_data_rate_dl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(
              s_ng_ran_node_max_ip_data_rate_dl, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true);
        }));
        break;
      }
      case 110: {
        location_info_snr_eporting_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_info_snr_eporting.unpack(bref); }));
        break;
      }
      case 117: {
        mr_dc_res_coordination_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mr_dc_res_coordination_info.unpack(bref); }));
        break;
      }
      case 32: {
        pcell_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pcell_id.unpack(bref); }));
        break;
      }
      case 126: {
        ne_dc_tdm_pattern_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ne_dc_tdm_pattern.unpack(bref); }));
        break;
      }
      case 148: {
        requested_fast_mcg_recovery_via_srb3_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return requested_fast_mcg_recovery_via_srb3.unpack(bref); }));
        break;
      }
      case 150: {
        requested_fast_mcg_recovery_via_srb3_release_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return requested_fast_mcg_recovery_via_srb3_release.unpack(bref); }));
        break;
      }
      case 229: {
        sn_triggered_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_triggered.unpack(bref); }));
        break;
      }
      case 260: {
        target_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_node_id.unpack(bref); }));
        break;
      }
      case 293: {
        pscell_history_info_retrieve_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pscell_history_info_retrieve.unpack(bref); }));
        break;
      }
      case 178: {
        ue_history_info_from_the_ue_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_history_info_from_the_ue.unpack(bref); }));
        break;
      }
      case 323: {
        ch_oinfo_mod_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ch_oinfo_mod_req.unpack(bref); }));
        break;
      }
      case 326: {
        scg_activation_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_request.unpack(bref); }));
        break;
      }
      case 332: {
        cp_a_info_mod_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cp_a_info_mod_req.unpack(bref); }));
        break;
      }
      case 335: {
        c_pci_nformation_upd_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return c_pci_nformation_upd.unpack(bref); }));
        break;
      }
      case 359: {
        s_ng_ran_node_ue_slice_mbr_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(s_ng_ran_node_ue_slice_mbr, bref, 1, 8, true); }));
        break;
      }
      case 362: {
        management_based_mdt_plmn_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(management_based_mdt_plmn_mod_list, bref, 0, 16, true);
        }));
        break;
      }
      case 378: {
        sel_n_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sel_n_id.unpack(bref); }));
        break;
      }
      case 434: {
        q_mcc_oordination_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_oordination_request.unpack(bref); }));
        break;
      }
      case 441: {
        src_sn_to_tgt_sn_qmc_info_inquiry_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return src_sn_to_tgt_sn_qmc_info_inquiry.unpack(bref); }));
        break;
      }
      case 376: {
        iab_authorization_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_authorization_status.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (pdcp_change_ind_present) {
    j.start_obj();
    j.write_int("id", 33);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdcp_change_ind.to_json(j);
    j.end_obj();
  }
  if (sel_plmn_present) {
    j.start_obj();
    j.write_int("id", 64);
    j.write_str("criticality", "ignore");
    j.write_str("value", sel_plmn.to_string());
    j.end_obj();
  }
  if (mob_restrict_list_present) {
    j.start_obj();
    j.write_int("id", 25);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_restrict_list.to_json(j);
    j.end_obj();
  }
  if (scg_cfg_query_present) {
    j.start_obj();
    j.write_int("id", 63);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (ue_context_info_sn_mod_request_present) {
    j.start_obj();
    j.write_int("id", 85);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ue_context_info_sn_mod_request.to_json(j);
    j.end_obj();
  }
  if (mn_to_sn_container_present) {
    j.start_obj();
    j.write_int("id", 24);
    j.write_str("criticality", "ignore");
    j.write_str("value", mn_to_sn_container.to_string());
    j.end_obj();
  }
  if (requested_split_srb_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.write_str("value", requested_split_srb.to_string());
    j.end_obj();
  }
  if (requested_split_srb_release_present) {
    j.start_obj();
    j.write_int("id", 55);
    j.write_str("criticality", "ignore");
    j.write_str("value", requested_split_srb_release.to_string());
    j.end_obj();
  }
  if (desired_act_notif_level_present) {
    j.start_obj();
    j.write_int("id", 94);
    j.write_str("criticality", "ignore");
    j.write_str("value", desired_act_notif_level.to_string());
    j.end_obj();
  }
  if (add_drb_ids_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : add_drb_ids) {
      j.write_int(e1);
    }
    j.end_array();
    j.end_obj();
  }
  if (s_ng_ran_node_max_ip_data_rate_ul_present) {
    j.start_obj();
    j.write_int("id", 105);
    j.write_str("criticality", "reject");
    j.write_uint("value", s_ng_ran_node_max_ip_data_rate_ul);
    j.end_obj();
  }
  if (s_ng_ran_node_max_ip_data_rate_dl_present) {
    j.start_obj();
    j.write_int("id", 113);
    j.write_str("criticality", "reject");
    j.write_uint("value", s_ng_ran_node_max_ip_data_rate_dl);
    j.end_obj();
  }
  if (location_info_snr_eporting_present) {
    j.start_obj();
    j.write_int("id", 110);
    j.write_str("criticality", "ignore");
    j.write_str("value", "pSCell");
    j.end_obj();
  }
  if (mr_dc_res_coordination_info_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mr_dc_res_coordination_info.to_json(j);
    j.end_obj();
  }
  if (pcell_id_present) {
    j.start_obj();
    j.write_int("id", 32);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    pcell_id.to_json(j);
    j.end_obj();
  }
  if (ne_dc_tdm_pattern_present) {
    j.start_obj();
    j.write_int("id", 126);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ne_dc_tdm_pattern.to_json(j);
    j.end_obj();
  }
  if (requested_fast_mcg_recovery_via_srb3_present) {
    j.start_obj();
    j.write_int("id", 148);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (requested_fast_mcg_recovery_via_srb3_release_present) {
    j.start_obj();
    j.write_int("id", 150);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (sn_triggered_present) {
    j.start_obj();
    j.write_int("id", 229);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (target_node_id_present) {
    j.start_obj();
    j.write_int("id", 260);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    target_node_id.to_json(j);
    j.end_obj();
  }
  if (pscell_history_info_retrieve_present) {
    j.start_obj();
    j.write_int("id", 293);
    j.write_str("criticality", "ignore");
    j.write_str("value", "query");
    j.end_obj();
  }
  if (ue_history_info_from_the_ue_present) {
    j.start_obj();
    j.write_int("id", 178);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_history_info_from_the_ue.to_json(j);
    j.end_obj();
  }
  if (ch_oinfo_mod_req_present) {
    j.start_obj();
    j.write_int("id", 323);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ch_oinfo_mod_req.to_json(j);
    j.end_obj();
  }
  if (scg_activation_request_present) {
    j.start_obj();
    j.write_int("id", 326);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_request.to_string());
    j.end_obj();
  }
  if (cp_a_info_mod_req_present) {
    j.start_obj();
    j.write_int("id", 332);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cp_a_info_mod_req.to_json(j);
    j.end_obj();
  }
  if (c_pci_nformation_upd_present) {
    j.start_obj();
    j.write_int("id", 335);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    c_pci_nformation_upd.to_json(j);
    j.end_obj();
  }
  if (s_ng_ran_node_ue_slice_mbr_present) {
    j.start_obj();
    j.write_int("id", 359);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : s_ng_ran_node_ue_slice_mbr) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (management_based_mdt_plmn_mod_list_present) {
    j.start_obj();
    j.write_int("id", 362);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_mod_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (sel_n_id_present) {
    j.start_obj();
    j.write_int("id", 378);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sel_n_id.to_json(j);
    j.end_obj();
  }
  if (q_mcc_oordination_request_present) {
    j.start_obj();
    j.write_int("id", 434);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_oordination_request.to_json(j);
    j.end_obj();
  }
  if (src_sn_to_tgt_sn_qmc_info_inquiry_present) {
    j.start_obj();
    j.write_int("id", 441);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (iab_authorization_status_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "ignore");
    j.write_str("value", iab_authorization_status.to_string());
    j.end_obj();
  }
  j.end_array();
}

// SNodeModificationRequestAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_mod_request_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23,  71, 36,  38,  72,  2,   3,   10,  111, 117,
                                   123, 61, 149, 151, 253, 279, 327, 333, 435, 459};
  return map_enum_number(names, 20, idx, "id");
}
bool sn_ode_mod_request_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23,  71, 36,  38,  72,  2,   3,   10,  111, 117,
                                   123, 61, 149, 151, 253, 279, 327, 333, 435, 459};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_mod_request_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 36:
      return crit_e::ignore;
    case 38:
      return crit_e::ignore;
    case 72:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    case 3:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 111:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 123:
      return crit_e::ignore;
    case 61:
      return crit_e::reject;
    case 149:
      return crit_e::ignore;
    case 151:
      return crit_e::ignore;
    case 253:
      return crit_e::ignore;
    case 279:
      return crit_e::ignore;
    case 327:
      return crit_e::ignore;
    case 333:
      return crit_e::ignore;
    case 435:
      return crit_e::ignore;
    case 459:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_mod_request_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 36:
      return presence_e::optional;
    case 38:
      return presence_e::optional;
    case 72:
      return presence_e::optional;
    case 2:
      return presence_e::optional;
    case 3:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 111:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 123:
      return presence_e::optional;
    case 61:
      return presence_e::optional;
    case 149:
      return presence_e::optional;
    case 151:
      return presence_e::optional;
    case 253:
      return presence_e::optional;
    case 279:
      return presence_e::optional;
    case 327:
      return presence_e::optional;
    case 333:
      return presence_e::optional;
    case 435:
      return presence_e::optional;
    case 459:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_mod_request_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_admitted_sn_mod_resp_present ? 1 : 0;
  nof_ies += pdu_session_not_admitted_sn_mod_resp_present ? 1 : 0;
  nof_ies += sn_to_mn_container_present ? 1 : 0;
  nof_ies += admitted_split_srb_present ? 1 : 0;
  nof_ies += admitted_split_srb_release_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += location_info_sn_present ? 1 : 0;
  nof_ies += mr_dc_res_coordination_info_present ? 1 : 0;
  nof_ies += pdu_session_data_forwarding_sn_mod_resp_present ? 1 : 0;
  nof_ies += rrc_cfg_ind_present ? 1 : 0;
  nof_ies += available_fast_mcg_recovery_via_srb3_present ? 1 : 0;
  nof_ies += release_fast_mcg_recovery_via_srb3_present ? 1 : 0;
  nof_ies += direct_forwarding_path_availability_present ? 1 : 0;
  nof_ies += scg_ue_history_info_present ? 1 : 0;
  nof_ies += scg_activation_status_present ? 1 : 0;
  nof_ies += cp_a_info_mod_req_ack_present ? 1 : 0;
  nof_ies += q_mcc_oordination_resp_present ? 1 : 0;
  nof_ies += source_sn_to_target_sn_qmc_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_admitted_sn_mod_resp_present) {
    HANDLE_CODE(pack_ie(
        bref, 36, crit_e{crit_e::ignore}, [this, &bref]() { return pdu_session_admitted_sn_mod_resp.pack(bref); }));
  }
  if (pdu_session_not_admitted_sn_mod_resp_present) {
    HANDLE_CODE(pack_ie(
        bref, 38, crit_e{crit_e::ignore}, [this, &bref]() { return pdu_session_not_admitted_sn_mod_resp.pack(bref); }));
  }
  if (sn_to_mn_container_present) {
    HANDLE_CODE(pack_ie(bref, 72, crit_e{crit_e::ignore}, [this, &bref]() { return sn_to_mn_container.pack(bref); }));
  }
  if (admitted_split_srb_present) {
    HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::ignore}, [this, &bref]() { return admitted_split_srb.pack(bref); }));
  }
  if (admitted_split_srb_release_present) {
    HANDLE_CODE(
        pack_ie(bref, 3, crit_e{crit_e::ignore}, [this, &bref]() { return admitted_split_srb_release.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (location_info_sn_present) {
    HANDLE_CODE(pack_ie(bref, 111, crit_e{crit_e::ignore}, [this, &bref]() { return location_info_sn.pack(bref); }));
  }
  if (mr_dc_res_coordination_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return mr_dc_res_coordination_info.pack(bref); }));
  }
  if (pdu_session_data_forwarding_sn_mod_resp_present) {
    HANDLE_CODE(pack_ie(bref, 123, crit_e{crit_e::ignore}, [this, &bref]() {
      return pdu_session_data_forwarding_sn_mod_resp.pack(bref);
    }));
  }
  if (rrc_cfg_ind_present) {
    HANDLE_CODE(pack_ie(bref, 61, crit_e{crit_e::reject}, [this, &bref]() { return rrc_cfg_ind.pack(bref); }));
  }
  if (available_fast_mcg_recovery_via_srb3_present) {
    HANDLE_CODE(pack_ie(bref, 149, crit_e{crit_e::ignore}, [this, &bref]() {
      return available_fast_mcg_recovery_via_srb3.pack(bref);
    }));
  }
  if (release_fast_mcg_recovery_via_srb3_present) {
    HANDLE_CODE(pack_ie(
        bref, 151, crit_e{crit_e::ignore}, [this, &bref]() { return release_fast_mcg_recovery_via_srb3.pack(bref); }));
  }
  if (direct_forwarding_path_availability_present) {
    HANDLE_CODE(pack_ie(
        bref, 253, crit_e{crit_e::ignore}, [this, &bref]() { return direct_forwarding_path_availability.pack(bref); }));
  }
  if (scg_ue_history_info_present) {
    HANDLE_CODE(pack_ie(bref, 279, crit_e{crit_e::ignore}, [this, &bref]() { return scg_ue_history_info.pack(bref); }));
  }
  if (scg_activation_status_present) {
    HANDLE_CODE(
        pack_ie(bref, 327, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_status.pack(bref); }));
  }
  if (cp_a_info_mod_req_ack_present) {
    HANDLE_CODE(
        pack_ie(bref, 333, crit_e{crit_e::ignore}, [this, &bref]() { return cp_a_info_mod_req_ack.pack(bref); }));
  }
  if (q_mcc_oordination_resp_present) {
    HANDLE_CODE(
        pack_ie(bref, 435, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_oordination_resp.pack(bref); }));
  }
  if (source_sn_to_target_sn_qmc_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 459, crit_e{crit_e::ignore}, [this, &bref]() { return source_sn_to_target_sn_qmc_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_mod_request_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_mod_request_ack_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 36: {
        pdu_session_admitted_sn_mod_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pdu_session_admitted_sn_mod_resp.unpack(bref); }));
        break;
      }
      case 38: {
        pdu_session_not_admitted_sn_mod_resp_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_not_admitted_sn_mod_resp.unpack(bref); }));
        break;
      }
      case 72: {
        sn_to_mn_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_to_mn_container.unpack(bref); }));
        break;
      }
      case 2: {
        admitted_split_srb_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return admitted_split_srb.unpack(bref); }));
        break;
      }
      case 3: {
        admitted_split_srb_release_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return admitted_split_srb_release.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 111: {
        location_info_sn_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_info_sn.unpack(bref); }));
        break;
      }
      case 117: {
        mr_dc_res_coordination_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mr_dc_res_coordination_info.unpack(bref); }));
        break;
      }
      case 123: {
        pdu_session_data_forwarding_sn_mod_resp_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_data_forwarding_sn_mod_resp.unpack(bref); }));
        break;
      }
      case 61: {
        rrc_cfg_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_cfg_ind.unpack(bref); }));
        break;
      }
      case 149: {
        available_fast_mcg_recovery_via_srb3_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return available_fast_mcg_recovery_via_srb3.unpack(bref); }));
        break;
      }
      case 151: {
        release_fast_mcg_recovery_via_srb3_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return release_fast_mcg_recovery_via_srb3.unpack(bref); }));
        break;
      }
      case 253: {
        direct_forwarding_path_availability_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return direct_forwarding_path_availability.unpack(bref); }));
        break;
      }
      case 279: {
        scg_ue_history_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_ue_history_info.unpack(bref); }));
        break;
      }
      case 327: {
        scg_activation_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_status.unpack(bref); }));
        break;
      }
      case 333: {
        cp_a_info_mod_req_ack_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cp_a_info_mod_req_ack.unpack(bref); }));
        break;
      }
      case 435: {
        q_mcc_oordination_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_oordination_resp.unpack(bref); }));
        break;
      }
      case 459: {
        source_sn_to_target_sn_qmc_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_sn_to_target_sn_qmc_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_mod_request_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (pdu_session_admitted_sn_mod_resp_present) {
    j.start_obj();
    j.write_int("id", 36);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_admitted_sn_mod_resp.to_json(j);
    j.end_obj();
  }
  if (pdu_session_not_admitted_sn_mod_resp_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_not_admitted_sn_mod_resp.to_json(j);
    j.end_obj();
  }
  if (sn_to_mn_container_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    j.write_str("value", sn_to_mn_container.to_string());
    j.end_obj();
  }
  if (admitted_split_srb_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_str("value", admitted_split_srb.to_string());
    j.end_obj();
  }
  if (admitted_split_srb_release_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "ignore");
    j.write_str("value", admitted_split_srb_release.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (location_info_sn_present) {
    j.start_obj();
    j.write_int("id", 111);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    location_info_sn.to_json(j);
    j.end_obj();
  }
  if (mr_dc_res_coordination_info_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mr_dc_res_coordination_info.to_json(j);
    j.end_obj();
  }
  if (pdu_session_data_forwarding_sn_mod_resp_present) {
    j.start_obj();
    j.write_int("id", 123);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_data_forwarding_sn_mod_resp.to_json(j);
    j.end_obj();
  }
  if (rrc_cfg_ind_present) {
    j.start_obj();
    j.write_int("id", 61);
    j.write_str("criticality", "reject");
    j.write_str("value", rrc_cfg_ind.to_string());
    j.end_obj();
  }
  if (available_fast_mcg_recovery_via_srb3_present) {
    j.start_obj();
    j.write_int("id", 149);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (release_fast_mcg_recovery_via_srb3_present) {
    j.start_obj();
    j.write_int("id", 151);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (direct_forwarding_path_availability_present) {
    j.start_obj();
    j.write_int("id", 253);
    j.write_str("criticality", "ignore");
    j.write_str("value", "direct-path-available");
    j.end_obj();
  }
  if (scg_ue_history_info_present) {
    j.start_obj();
    j.write_int("id", 279);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    scg_ue_history_info.to_json(j);
    j.end_obj();
  }
  if (scg_activation_status_present) {
    j.start_obj();
    j.write_int("id", 327);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_status.to_string());
    j.end_obj();
  }
  if (cp_a_info_mod_req_ack_present) {
    j.start_obj();
    j.write_int("id", 333);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cp_a_info_mod_req_ack.to_json(j);
    j.end_obj();
  }
  if (q_mcc_oordination_resp_present) {
    j.start_obj();
    j.write_int("id", 435);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_oordination_resp.to_json(j);
    j.end_obj();
  }
  if (source_sn_to_target_sn_qmc_info_present) {
    j.start_obj();
    j.write_int("id", 459);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_sn_to_target_sn_qmc_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeModificationRequestReject-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_mod_request_reject_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 7, 10};
  return map_enum_number(names, 4, idx, "id");
}
bool sn_ode_mod_request_reject_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 7, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_mod_request_reject_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_mod_request_reject_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_mod_request_reject_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_mod_request_reject_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_mod_request_reject_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_mod_request_reject_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeModificationRequired-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_mod_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23,  71,  7,   33,  48,  51,  72,  97,  98,  111, 117, 61,
                                   149, 151, 247, 279, 326, 336, 365, 390, 434, 425, 466};
  return map_enum_number(names, 23, idx, "id");
}
bool sn_ode_mod_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23,  71,  7,   33,  48,  51,  72,  97,  98,  111, 117, 61,
                                   149, 151, 247, 279, 326, 336, 365, 390, 434, 425, 466};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_mod_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 33:
      return crit_e::ignore;
    case 48:
      return crit_e::ignore;
    case 51:
      return crit_e::ignore;
    case 72:
      return crit_e::ignore;
    case 97:
      return crit_e::ignore;
    case 98:
      return crit_e::ignore;
    case 111:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 61:
      return crit_e::reject;
    case 149:
      return crit_e::ignore;
    case 151:
      return crit_e::ignore;
    case 247:
      return crit_e::ignore;
    case 279:
      return crit_e::ignore;
    case 326:
      return crit_e::ignore;
    case 336:
      return crit_e::ignore;
    case 365:
      return crit_e::ignore;
    case 390:
      return crit_e::ignore;
    case 434:
      return crit_e::ignore;
    case 425:
      return crit_e::reject;
    case 466:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_mod_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 33:
      return presence_e::optional;
    case 48:
      return presence_e::optional;
    case 51:
      return presence_e::optional;
    case 72:
      return presence_e::optional;
    case 97:
      return presence_e::optional;
    case 98:
      return presence_e::optional;
    case 111:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 61:
      return presence_e::optional;
    case 149:
      return presence_e::optional;
    case 151:
      return presence_e::optional;
    case 247:
      return presence_e::optional;
    case 279:
      return presence_e::optional;
    case 326:
      return presence_e::optional;
    case 336:
      return presence_e::optional;
    case 365:
      return presence_e::optional;
    case 390:
      return presence_e::optional;
    case 434:
      return presence_e::optional;
    case 425:
      return presence_e::optional;
    case 466:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_mod_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pdcp_change_ind_present ? 1 : 0;
  nof_ies += pdu_session_to_be_modified_sn_mod_required_present ? 1 : 0;
  nof_ies += pdu_session_to_be_released_sn_mod_required_present ? 1 : 0;
  nof_ies += sn_to_mn_container_present ? 1 : 0;
  nof_ies += spare_drb_ids_present ? 1 : 0;
  nof_ies += required_nof_drb_ids_present ? 1 : 0;
  nof_ies += location_info_sn_present ? 1 : 0;
  nof_ies += mr_dc_res_coordination_info_present ? 1 : 0;
  nof_ies += rrc_cfg_ind_present ? 1 : 0;
  nof_ies += available_fast_mcg_recovery_via_srb3_present ? 1 : 0;
  nof_ies += release_fast_mcg_recovery_via_srb3_present ? 1 : 0;
  nof_ies += scg_ind_present ? 1 : 0;
  nof_ies += scg_ue_history_info_present ? 1 : 0;
  nof_ies += scg_activation_request_present ? 1 : 0;
  nof_ies += cp_ac_info_mod_required_present ? 1 : 0;
  nof_ies += sc_grecfg_notif_present ? 1 : 0;
  nof_ies += spr_availability_present ? 1 : 0;
  nof_ies += q_mcc_oordination_request_present ? 1 : 0;
  nof_ies += s_cp_ac_request_present ? 1 : 0;
  nof_ies += pdu_sessions_list_to_be_released_up_error_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (pdcp_change_ind_present) {
    HANDLE_CODE(pack_ie(bref, 33, crit_e{crit_e::ignore}, [this, &bref]() { return pdcp_change_ind.pack(bref); }));
  }
  if (pdu_session_to_be_modified_sn_mod_required_present) {
    HANDLE_CODE(pack_ie(bref, 48, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_to_be_modified_sn_mod_required, 1, 256, true);
    }));
  }
  if (pdu_session_to_be_released_sn_mod_required_present) {
    HANDLE_CODE(pack_ie(bref, 51, crit_e{crit_e::ignore}, [this, &bref]() {
      return pdu_session_to_be_released_sn_mod_required.pack(bref);
    }));
  }
  if (sn_to_mn_container_present) {
    HANDLE_CODE(pack_ie(bref, 72, crit_e{crit_e::ignore}, [this, &bref]() { return sn_to_mn_container.pack(bref); }));
  }
  if (spare_drb_ids_present) {
    HANDLE_CODE(pack_ie(bref, 97, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, spare_drb_ids, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
    }));
  }
  if (required_nof_drb_ids_present) {
    HANDLE_CODE(pack_ie(bref, 98, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, required_nof_drb_ids, (uint8_t)1u, (uint8_t)32u, true, true);
    }));
  }
  if (location_info_sn_present) {
    HANDLE_CODE(pack_ie(bref, 111, crit_e{crit_e::ignore}, [this, &bref]() { return location_info_sn.pack(bref); }));
  }
  if (mr_dc_res_coordination_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return mr_dc_res_coordination_info.pack(bref); }));
  }
  if (rrc_cfg_ind_present) {
    HANDLE_CODE(pack_ie(bref, 61, crit_e{crit_e::reject}, [this, &bref]() { return rrc_cfg_ind.pack(bref); }));
  }
  if (available_fast_mcg_recovery_via_srb3_present) {
    HANDLE_CODE(pack_ie(bref, 149, crit_e{crit_e::ignore}, [this, &bref]() {
      return available_fast_mcg_recovery_via_srb3.pack(bref);
    }));
  }
  if (release_fast_mcg_recovery_via_srb3_present) {
    HANDLE_CODE(pack_ie(
        bref, 151, crit_e{crit_e::ignore}, [this, &bref]() { return release_fast_mcg_recovery_via_srb3.pack(bref); }));
  }
  if (scg_ind_present) {
    HANDLE_CODE(pack_ie(bref, 247, crit_e{crit_e::ignore}, [this, &bref]() { return scg_ind.pack(bref); }));
  }
  if (scg_ue_history_info_present) {
    HANDLE_CODE(pack_ie(bref, 279, crit_e{crit_e::ignore}, [this, &bref]() { return scg_ue_history_info.pack(bref); }));
  }
  if (scg_activation_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 326, crit_e{crit_e::ignore}, [this, &bref]() { return scg_activation_request.pack(bref); }));
  }
  if (cp_ac_info_mod_required_present) {
    HANDLE_CODE(
        pack_ie(bref, 336, crit_e{crit_e::ignore}, [this, &bref]() { return cp_ac_info_mod_required.pack(bref); }));
  }
  if (sc_grecfg_notif_present) {
    HANDLE_CODE(pack_ie(bref, 365, crit_e{crit_e::ignore}, [this, &bref]() { return sc_grecfg_notif.pack(bref); }));
  }
  if (spr_availability_present) {
    HANDLE_CODE(pack_ie(bref, 390, crit_e{crit_e::ignore}, [this, &bref]() { return spr_availability.pack(bref); }));
  }
  if (q_mcc_oordination_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 434, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_oordination_request.pack(bref); }));
  }
  if (s_cp_ac_request_present) {
    HANDLE_CODE(pack_ie(bref, 425, crit_e{crit_e::reject}, [this, &bref]() { return s_cp_ac_request.pack(bref); }));
  }
  if (pdu_sessions_list_to_be_released_up_error_present) {
    HANDLE_CODE(pack_ie(bref, 466, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_sessions_list_to_be_released_up_error, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_mod_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_mod_required_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 33: {
        pdcp_change_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pdcp_change_ind.unpack(bref); }));
        break;
      }
      case 48: {
        pdu_session_to_be_modified_sn_mod_required_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_to_be_modified_sn_mod_required, bref, 1, 256, true);
        }));
        break;
      }
      case 51: {
        pdu_session_to_be_released_sn_mod_required_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_to_be_released_sn_mod_required.unpack(bref); }));
        break;
      }
      case 72: {
        sn_to_mn_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_to_mn_container.unpack(bref); }));
        break;
      }
      case 97: {
        spare_drb_ids_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(spare_drb_ids, bref, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
        }));
        break;
      }
      case 98: {
        required_nof_drb_ids_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(required_nof_drb_ids, bref, (uint8_t)1u, (uint8_t)32u, true, true);
        }));
        break;
      }
      case 111: {
        location_info_sn_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_info_sn.unpack(bref); }));
        break;
      }
      case 117: {
        mr_dc_res_coordination_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mr_dc_res_coordination_info.unpack(bref); }));
        break;
      }
      case 61: {
        rrc_cfg_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_cfg_ind.unpack(bref); }));
        break;
      }
      case 149: {
        available_fast_mcg_recovery_via_srb3_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return available_fast_mcg_recovery_via_srb3.unpack(bref); }));
        break;
      }
      case 151: {
        release_fast_mcg_recovery_via_srb3_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return release_fast_mcg_recovery_via_srb3.unpack(bref); }));
        break;
      }
      case 247: {
        scg_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_ind.unpack(bref); }));
        break;
      }
      case 279: {
        scg_ue_history_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_ue_history_info.unpack(bref); }));
        break;
      }
      case 326: {
        scg_activation_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_activation_request.unpack(bref); }));
        break;
      }
      case 336: {
        cp_ac_info_mod_required_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cp_ac_info_mod_required.unpack(bref); }));
        break;
      }
      case 365: {
        sc_grecfg_notif_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sc_grecfg_notif.unpack(bref); }));
        break;
      }
      case 390: {
        spr_availability_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return spr_availability.unpack(bref); }));
        break;
      }
      case 434: {
        q_mcc_oordination_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_oordination_request.unpack(bref); }));
        break;
      }
      case 425: {
        s_cp_ac_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_cp_ac_request.unpack(bref); }));
        break;
      }
      case 466: {
        pdu_sessions_list_to_be_released_up_error_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_sessions_list_to_be_released_up_error, bref, 1, 256, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_mod_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (pdcp_change_ind_present) {
    j.start_obj();
    j.write_int("id", 33);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdcp_change_ind.to_json(j);
    j.end_obj();
  }
  if (pdu_session_to_be_modified_sn_mod_required_present) {
    j.start_obj();
    j.write_int("id", 48);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_to_be_modified_sn_mod_required) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_to_be_released_sn_mod_required_present) {
    j.start_obj();
    j.write_int("id", 51);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_to_be_released_sn_mod_required.to_json(j);
    j.end_obj();
  }
  if (sn_to_mn_container_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    j.write_str("value", sn_to_mn_container.to_string());
    j.end_obj();
  }
  if (spare_drb_ids_present) {
    j.start_obj();
    j.write_int("id", 97);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : spare_drb_ids) {
      j.write_int(e1);
    }
    j.end_array();
    j.end_obj();
  }
  if (required_nof_drb_ids_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "ignore");
    j.write_int("value", required_nof_drb_ids);
    j.end_obj();
  }
  if (location_info_sn_present) {
    j.start_obj();
    j.write_int("id", 111);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    location_info_sn.to_json(j);
    j.end_obj();
  }
  if (mr_dc_res_coordination_info_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mr_dc_res_coordination_info.to_json(j);
    j.end_obj();
  }
  if (rrc_cfg_ind_present) {
    j.start_obj();
    j.write_int("id", 61);
    j.write_str("criticality", "reject");
    j.write_str("value", rrc_cfg_ind.to_string());
    j.end_obj();
  }
  if (available_fast_mcg_recovery_via_srb3_present) {
    j.start_obj();
    j.write_int("id", 149);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (release_fast_mcg_recovery_via_srb3_present) {
    j.start_obj();
    j.write_int("id", 151);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (scg_ind_present) {
    j.start_obj();
    j.write_int("id", 247);
    j.write_str("criticality", "ignore");
    j.write_str("value", "released");
    j.end_obj();
  }
  if (scg_ue_history_info_present) {
    j.start_obj();
    j.write_int("id", 279);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    scg_ue_history_info.to_json(j);
    j.end_obj();
  }
  if (scg_activation_request_present) {
    j.start_obj();
    j.write_int("id", 326);
    j.write_str("criticality", "ignore");
    j.write_str("value", scg_activation_request.to_string());
    j.end_obj();
  }
  if (cp_ac_info_mod_required_present) {
    j.start_obj();
    j.write_int("id", 336);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cp_ac_info_mod_required.to_json(j);
    j.end_obj();
  }
  if (sc_grecfg_notif_present) {
    j.start_obj();
    j.write_int("id", 365);
    j.write_str("criticality", "ignore");
    j.write_str("value", sc_grecfg_notif.to_string());
    j.end_obj();
  }
  if (spr_availability_present) {
    j.start_obj();
    j.write_int("id", 390);
    j.write_str("criticality", "ignore");
    j.write_str("value", "spr-available");
    j.end_obj();
  }
  if (q_mcc_oordination_request_present) {
    j.start_obj();
    j.write_int("id", 434);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_oordination_request.to_json(j);
    j.end_obj();
  }
  if (s_cp_ac_request_present) {
    j.start_obj();
    j.write_int("id", 425);
    j.write_str("criticality", "reject");
    j.write_str("value", "initiation");
    j.end_obj();
  }
  if (pdu_sessions_list_to_be_released_up_error_present) {
    j.start_obj();
    j.write_int("id", 466);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_sessions_list_to_be_released_up_error) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// SNodeReconfigurationComplete-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_recfg_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 60};
  return map_enum_number(names, 3, idx, "id");
}
bool sn_ode_recfg_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 60};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_recfg_complete_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 60:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_recfg_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 60:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_recfg_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 60, crit_e{crit_e::ignore}, [this, &bref]() { return resp_info_reconf_compl.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_recfg_complete_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_recfg_complete_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 60: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return resp_info_reconf_compl.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_recfg_complete_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 60);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  resp_info_reconf_compl.to_json(j);
  j.end_obj();
  j.end_array();
}

// SNodeReleaseConfirm-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_release_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 39, 10};
  return map_enum_number(names, 4, idx, "id");
}
bool sn_ode_release_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 39, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_release_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 39:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_release_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 39:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_release_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_released_list_rel_conf_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_released_list_rel_conf_present) {
    HANDLE_CODE(pack_ie(
        bref, 39, crit_e{crit_e::ignore}, [this, &bref]() { return pdu_session_released_list_rel_conf.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_release_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_release_confirm_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 39: {
        pdu_session_released_list_rel_conf_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pdu_session_released_list_rel_conf.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_release_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (pdu_session_released_list_rel_conf_present) {
    j.start_obj();
    j.write_int("id", 39);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_released_list_rel_conf.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeReleaseReject-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_release_reject_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 7, 10};
  return map_enum_number(names, 4, idx, "id");
}
bool sn_ode_release_reject_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 7, 10};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_release_reject_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_release_reject_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::optional;
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_release_reject_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += s_ng_ran_node_ue_xnap_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (s_ng_ran_node_ue_xnap_id_present) {
    HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_release_reject_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_release_reject_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        s_ng_ran_node_ue_xnap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_release_reject_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (s_ng_ran_node_ue_xnap_id_present) {
    j.start_obj();
    j.write_int("id", 71);
    j.write_str("criticality", "reject");
    j.write_uint("value", s_ng_ran_node_ue_xnap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeReleaseRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 7, 50, 86, 24, 137};
  return map_enum_number(names, 7, idx, "id");
}
bool sn_ode_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 7, 50, 86, 24, 137};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 50:
      return crit_e::ignore;
    case 86:
      return crit_e::ignore;
    case 24:
      return crit_e::ignore;
    case 137:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::optional;
    case 7:
      return presence_e::mandatory;
    case 50:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    case 24:
      return presence_e::optional;
    case 137:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += s_ng_ran_node_ue_xnap_id_present ? 1 : 0;
  nof_ies += pdu_session_to_be_released_rel_req_present ? 1 : 0;
  nof_ies += ue_context_kept_ind_present ? 1 : 0;
  nof_ies += mn_to_sn_container_present ? 1 : 0;
  nof_ies += drbs_transferred_to_mn_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (s_ng_ran_node_ue_xnap_id_present) {
    HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (pdu_session_to_be_released_rel_req_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_to_be_released_rel_req, 1, 256, true);
    }));
  }
  if (ue_context_kept_ind_present) {
    HANDLE_CODE(pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() { return ue_context_kept_ind.pack(bref); }));
  }
  if (mn_to_sn_container_present) {
    HANDLE_CODE(pack_ie(bref, 24, crit_e{crit_e::ignore}, [this, &bref]() { return mn_to_sn_container.pack(bref); }));
  }
  if (drbs_transferred_to_mn_present) {
    HANDLE_CODE(pack_ie(bref, 137, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, drbs_transferred_to_mn, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_release_request_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        s_ng_ran_node_ue_xnap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 50: {
        pdu_session_to_be_released_rel_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_to_be_released_rel_req, bref, 1, 256, true);
        }));
        break;
      }
      case 86: {
        ue_context_kept_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_kept_ind.unpack(bref); }));
        break;
      }
      case 24: {
        mn_to_sn_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mn_to_sn_container.unpack(bref); }));
        break;
      }
      case 137: {
        drbs_transferred_to_mn_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(
              drbs_transferred_to_mn, bref, 1, 32, integer_packer<uint8_t>(1, 32, true, true), true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (s_ng_ran_node_ue_xnap_id_present) {
    j.start_obj();
    j.write_int("id", 71);
    j.write_str("criticality", "reject");
    j.write_uint("value", s_ng_ran_node_ue_xnap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (pdu_session_to_be_released_rel_req_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_to_be_released_rel_req) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_context_kept_ind_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (mn_to_sn_container_present) {
    j.start_obj();
    j.write_int("id", 24);
    j.write_str("criticality", "ignore");
    j.write_str("value", mn_to_sn_container.to_string());
    j.end_obj();
  }
  if (drbs_transferred_to_mn_present) {
    j.start_obj();
    j.write_int("id", 137);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : drbs_transferred_to_mn) {
      j.write_int(e1);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// SNodeReleaseRequestAcknowledge-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_release_request_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 104, 10, 279, 287};
  return map_enum_number(names, 6, idx, "id");
}
bool sn_ode_release_request_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 104, 10, 279, 287};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_release_request_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 104:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 279:
      return crit_e::ignore;
    case 287:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_release_request_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::optional;
    case 104:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 279:
      return presence_e::optional;
    case 287:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_release_request_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += s_ng_ran_node_ue_xnap_id_present ? 1 : 0;
  nof_ies += pdu_session_to_be_released_rel_req_ack_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += scg_ue_history_info_present ? 1 : 0;
  nof_ies += sn_mob_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (s_ng_ran_node_ue_xnap_id_present) {
    HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (pdu_session_to_be_released_rel_req_ack_present) {
    HANDLE_CODE(pack_ie(bref, 104, crit_e{crit_e::ignore}, [this, &bref]() {
      return pdu_session_to_be_released_rel_req_ack.pack(bref);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (scg_ue_history_info_present) {
    HANDLE_CODE(pack_ie(bref, 279, crit_e{crit_e::ignore}, [this, &bref]() { return scg_ue_history_info.pack(bref); }));
  }
  if (sn_mob_info_present) {
    HANDLE_CODE(pack_ie(bref, 287, crit_e{crit_e::ignore}, [this, &bref]() { return sn_mob_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_release_request_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_release_request_ack_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        s_ng_ran_node_ue_xnap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 104: {
        pdu_session_to_be_released_rel_req_ack_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_to_be_released_rel_req_ack.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 279: {
        scg_ue_history_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_ue_history_info.unpack(bref); }));
        break;
      }
      case 287: {
        sn_mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_mob_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_release_request_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (s_ng_ran_node_ue_xnap_id_present) {
    j.start_obj();
    j.write_int("id", 71);
    j.write_str("criticality", "reject");
    j.write_uint("value", s_ng_ran_node_ue_xnap_id);
    j.end_obj();
  }
  if (pdu_session_to_be_released_rel_req_ack_present) {
    j.start_obj();
    j.write_int("id", 104);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_to_be_released_rel_req_ack.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (scg_ue_history_info_present) {
    j.start_obj();
    j.write_int("id", 279);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    scg_ue_history_info.to_json(j);
    j.end_obj();
  }
  if (sn_mob_info_present) {
    j.start_obj();
    j.write_int("id", 287);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sn_mob_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SNodeReleaseRequired-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t sn_ode_release_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 49, 7, 72, 279, 466};
  return map_enum_number(names, 7, idx, "id");
}
bool sn_ode_release_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 49, 7, 72, 279, 466};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sn_ode_release_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 49:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 72:
      return crit_e::ignore;
    case 279:
      return crit_e::ignore;
    case 466:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sn_ode_release_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 49:
      return presence_e::optional;
    case 7:
      return presence_e::mandatory;
    case 72:
      return presence_e::optional;
    case 279:
      return presence_e::optional;
    case 466:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sn_ode_release_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pdu_session_to_be_released_list_rel_rqd_present ? 1 : 0;
  nof_ies += sn_to_mn_container_present ? 1 : 0;
  nof_ies += scg_ue_history_info_present ? 1 : 0;
  nof_ies += pdu_sessions_list_to_be_released_up_error_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_to_be_released_list_rel_rqd_present) {
    HANDLE_CODE(pack_ie(bref, 49, crit_e{crit_e::ignore}, [this, &bref]() {
      return pdu_session_to_be_released_list_rel_rqd.pack(bref);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (sn_to_mn_container_present) {
    HANDLE_CODE(pack_ie(bref, 72, crit_e{crit_e::ignore}, [this, &bref]() { return sn_to_mn_container.pack(bref); }));
  }
  if (scg_ue_history_info_present) {
    HANDLE_CODE(pack_ie(bref, 279, crit_e{crit_e::ignore}, [this, &bref]() { return scg_ue_history_info.pack(bref); }));
  }
  if (pdu_sessions_list_to_be_released_up_error_present) {
    HANDLE_CODE(pack_ie(bref, 466, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_sessions_list_to_be_released_up_error, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sn_ode_release_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &sn_ode_release_required_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 49: {
        pdu_session_to_be_released_list_rel_rqd_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return pdu_session_to_be_released_list_rel_rqd.unpack(bref); }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 72: {
        sn_to_mn_container_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_to_mn_container.unpack(bref); }));
        break;
      }
      case 279: {
        scg_ue_history_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_ue_history_info.unpack(bref); }));
        break;
      }
      case 466: {
        pdu_sessions_list_to_be_released_up_error_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_sessions_list_to_be_released_up_error, bref, 1, 256, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void sn_ode_release_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (pdu_session_to_be_released_list_rel_rqd_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdu_session_to_be_released_list_rel_rqd.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (sn_to_mn_container_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    j.write_str("value", sn_to_mn_container.to_string());
    j.end_obj();
  }
  if (scg_ue_history_info_present) {
    j.start_obj();
    j.write_int("id", 279);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    scg_ue_history_info.to_json(j);
    j.end_obj();
  }
  if (pdu_sessions_list_to_be_released_up_error_present) {
    j.start_obj();
    j.write_int("id", 466);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_sessions_list_to_be_released_up_error) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// ScgFailureInformationReport-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t scg_fail_info_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 284, 285, 286, 287, 388};
  return map_enum_number(names, 7, idx, "id");
}
bool scg_fail_info_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 284, 285, 286, 287, 388};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e scg_fail_info_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    case 284:
      return crit_e::ignore;
    case 285:
      return crit_e::ignore;
    case 286:
      return crit_e::ignore;
    case 287:
      return crit_e::ignore;
    case 388:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e scg_fail_info_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 284:
      return presence_e::optional;
    case 285:
      return presence_e::optional;
    case 286:
      return presence_e::mandatory;
    case 287:
      return presence_e::optional;
    case 388:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE scg_fail_info_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += source_pscell_cgi_present ? 1 : 0;
  nof_ies += failed_pscell_cgi_present ? 1 : 0;
  nof_ies += sn_mob_info_present ? 1 : 0;
  nof_ies += cp_ac_cfg_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (source_pscell_cgi_present) {
    HANDLE_CODE(pack_ie(bref, 284, crit_e{crit_e::ignore}, [this, &bref]() { return source_pscell_cgi.pack(bref); }));
  }
  if (failed_pscell_cgi_present) {
    HANDLE_CODE(pack_ie(bref, 285, crit_e{crit_e::ignore}, [this, &bref]() { return failed_pscell_cgi.pack(bref); }));
  }
  HANDLE_CODE(
      pack_ie(bref, 286, crit_e{crit_e::ignore}, [this, &bref]() { return scg_fail_report_container.pack(bref); }));
  if (sn_mob_info_present) {
    HANDLE_CODE(pack_ie(bref, 287, crit_e{crit_e::ignore}, [this, &bref]() { return sn_mob_info.pack(bref); }));
  }
  if (cp_ac_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 388, crit_e{crit_e::ignore}, [this, &bref]() { return cp_ac_cfg.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE scg_fail_info_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &scg_fail_info_report_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 284: {
        source_pscell_cgi_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_pscell_cgi.unpack(bref); }));
        break;
      }
      case 285: {
        failed_pscell_cgi_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return failed_pscell_cgi.unpack(bref); }));
        break;
      }
      case 286: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return scg_fail_report_container.unpack(bref); }));
        break;
      }
      case 287: {
        sn_mob_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sn_mob_info.unpack(bref); }));
        break;
      }
      case 388: {
        cp_ac_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cp_ac_cfg.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void scg_fail_info_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (source_pscell_cgi_present) {
    j.start_obj();
    j.write_int("id", 284);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_pscell_cgi.to_json(j);
    j.end_obj();
  }
  if (failed_pscell_cgi_present) {
    j.start_obj();
    j.write_int("id", 285);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    failed_pscell_cgi.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 286);
  j.write_str("criticality", "ignore");
  j.write_str("value", scg_fail_report_container.to_string());
  j.end_obj();
  if (sn_mob_info_present) {
    j.start_obj();
    j.write_int("id", 287);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sn_mob_info.to_json(j);
    j.end_obj();
  }
  if (cp_ac_cfg_present) {
    j.start_obj();
    j.write_int("id", 388);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cp_ac_cfg.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ScgFailureTransfer-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t scg_fail_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71};
  return map_enum_number(names, 2, idx, "id");
}
bool scg_fail_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e scg_fail_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::ignore;
    case 71:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e scg_fail_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE scg_fail_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE scg_fail_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &scg_fail_transfer_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void scg_fail_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "ignore");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "ignore");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.end_array();
}

// SecondaryRATDataUsageReport-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t secondary_rat_data_usage_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 107};
  return map_enum_number(names, 3, idx, "id");
}
bool secondary_rat_data_usage_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 107};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e secondary_rat_data_usage_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 107:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e secondary_rat_data_usage_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 107:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE secondary_rat_data_usage_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 107, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_secondary_rat_usage_list, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE secondary_rat_data_usage_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &secondary_rat_data_usage_report_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 107: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_secondary_rat_usage_list, bref, 1, 256, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void secondary_rat_data_usage_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 107);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_secondary_rat_usage_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// TraceStartIEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t trace_start_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 71, 81};
  return map_enum_number(names, 3, idx, "id");
}
bool trace_start_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 71, 81};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trace_start_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 81:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trace_start_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::mandatory;
    case 71:
      return presence_e::mandatory;
    case 81:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE trace_start_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += trace_activation_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, m_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, s_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (trace_activation_present) {
    HANDLE_CODE(pack_ie(bref, 81, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trace_start_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &trace_start_ies_o::get_crit));

    switch (id) {
      case 23: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(m_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 71: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(s_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 81: {
        trace_activation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_activation.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void trace_start_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_uint("value", m_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 71);
  j.write_str("criticality", "reject");
  j.write_uint("value", s_ng_ran_node_ue_xnap_id);
  j.end_obj();
  if (trace_activation_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    trace_activation.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextRelease-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t ue_context_release_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {73, 79};
  return map_enum_number(names, 2, idx, "id");
}
bool ue_context_release_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {73, 79};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_release_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 73:
      return crit_e::reject;
    case 79:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_release_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 73:
      return presence_e::mandatory;
    case 79:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_release_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, source_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, target_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_release_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_release_ies_o::get_crit));

    switch (id) {
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 79: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(target_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_release_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "reject");
  j.write_uint("value", source_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 79);
  j.write_str("criticality", "reject");
  j.write_uint("value", target_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.end_array();
}

// XnRemovalFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t xn_removal_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {7, 10, 130};
  return map_enum_number(names, 3, idx, "id");
}
bool xn_removal_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {7, 10, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e xn_removal_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 7:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e xn_removal_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 7:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE xn_removal_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE xn_removal_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &xn_removal_fail_ies_o::get_crit));

    switch (id) {
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void xn_removal_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// XnRemovalRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t xn_removal_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {14, 93, 130};
  return map_enum_number(names, 3, idx, "id");
}
bool xn_removal_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {14, 93, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e xn_removal_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 14:
      return crit_e::reject;
    case 93:
      return crit_e::reject;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e xn_removal_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 14:
      return presence_e::mandatory;
    case 93:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE xn_removal_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += xn_removal_thres_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 14, crit_e{crit_e::reject}, [this, &bref]() { return global_ng_ran_node_id.pack(bref); }));
  if (xn_removal_thres_present) {
    HANDLE_CODE(pack_ie(bref, 93, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, xn_removal_thres, (uint8_t)1u, (uint8_t)8u, true, true);
    }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE xn_removal_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &xn_removal_request_ies_o::get_crit));

    switch (id) {
      case 14: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 93: {
        xn_removal_thres_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(xn_removal_thres, bref, (uint8_t)1u, (uint8_t)8u, true, true);
        }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void xn_removal_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 14);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_ng_ran_node_id.to_json(j);
  j.end_obj();
  if (xn_removal_thres_present) {
    j.start_obj();
    j.write_int("id", 93);
    j.write_str("criticality", "reject");
    j.write_int("value", xn_removal_thres);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// XnRemovalResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t xn_removal_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {14, 10, 130};
  return map_enum_number(names, 3, idx, "id");
}
bool xn_removal_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {14, 10, 130};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e xn_removal_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 14:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e xn_removal_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 14:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE xn_removal_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 14, crit_e{crit_e::reject}, [this, &bref]() { return global_ng_ran_node_id.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE xn_removal_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &xn_removal_resp_ies_o::get_crit));

    switch (id) {
      case 14: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void xn_removal_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 14);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_ng_ran_node_id.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  j.end_array();
}

// XnSetupFailure-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t xn_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {7, 76, 10, 130, 143};
  return map_enum_number(names, 5, idx, "id");
}
bool xn_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {7, 76, 10, 130, 143};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e xn_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 7:
      return crit_e::ignore;
    case 76:
      return crit_e::ignore;
    case 10:
      return crit_e::ignore;
    case 130:
      return crit_e::reject;
    case 143:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e xn_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 7:
      return presence_e::mandatory;
    case 76:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    case 143:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE xn_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  nof_ies += msg_oversize_notif_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 76, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }
  if (msg_oversize_notif_present) {
    HANDLE_CODE(pack_ie(bref, 143, crit_e{crit_e::ignore}, [this, &bref]() { return msg_oversize_notif.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE xn_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &xn_setup_fail_ies_o::get_crit));

    switch (id) {
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 76: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 143: {
        msg_oversize_notif_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return msg_oversize_notif.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void xn_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 76);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  if (msg_oversize_notif_present) {
    j.start_obj();
    j.write_int("id", 143);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    msg_oversize_notif.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// XnSetupRequest-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t xn_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {14, 75, 4, 19, 18, 130, 141, 142, 144, 156, 157, 341, 342};
  return map_enum_number(names, 13, idx, "id");
}
bool xn_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {14, 75, 4, 19, 18, 130, 141, 142, 144, 156, 157, 341, 342};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e xn_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 14:
      return crit_e::reject;
    case 75:
      return crit_e::reject;
    case 4:
      return crit_e::reject;
    case 19:
      return crit_e::reject;
    case 18:
      return crit_e::reject;
    case 130:
      return crit_e::reject;
    case 141:
      return crit_e::ignore;
    case 142:
      return crit_e::ignore;
    case 144:
      return crit_e::ignore;
    case 156:
      return crit_e::ignore;
    case 157:
      return crit_e::ignore;
    case 341:
      return crit_e::ignore;
    case 342:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e xn_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 14:
      return presence_e::mandatory;
    case 75:
      return presence_e::mandatory;
    case 4:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    case 18:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    case 141:
      return presence_e::optional;
    case 142:
      return presence_e::optional;
    case 144:
      return presence_e::optional;
    case 156:
      return presence_e::optional;
    case 157:
      return presence_e::optional;
    case 341:
      return presence_e::optional;
    case 342:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE xn_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += list_of_served_cells_nr_present ? 1 : 0;
  nof_ies += list_of_served_cells_e_utra_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  nof_ies += tnl_cfg_info_present ? 1 : 0;
  nof_ies += partial_list_ind_nr_present ? 1 : 0;
  nof_ies += cell_and_capacity_assist_info_nr_present ? 1 : 0;
  nof_ies += partial_list_ind_eutra_present ? 1 : 0;
  nof_ies += cell_and_capacity_assist_info_eutra_present ? 1 : 0;
  nof_ies += local_ng_ran_node_id_present ? 1 : 0;
  nof_ies += neighbour_ng_ran_node_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 14, crit_e{crit_e::reject}, [this, &bref]() { return global_ng_ran_node_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 75, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, tai_support_list, 1, 256, true);
  }));
  HANDLE_CODE(pack_ie(bref, 4, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, amf_region_info, 1, 16, true);
  }));
  if (list_of_served_cells_nr_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, list_of_served_cells_nr, 1, 16384, true);
    }));
  }
  if (list_of_served_cells_e_utra_present) {
    HANDLE_CODE(pack_ie(bref, 18, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, list_of_served_cells_e_utra, 1, 16384, true);
    }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }
  if (tnl_cfg_info_present) {
    HANDLE_CODE(pack_ie(bref, 141, crit_e{crit_e::ignore}, [this, &bref]() { return tnl_cfg_info.pack(bref); }));
  }
  if (partial_list_ind_nr_present) {
    HANDLE_CODE(pack_ie(bref, 142, crit_e{crit_e::ignore}, [this, &bref]() { return partial_list_ind_nr.pack(bref); }));
  }
  if (cell_and_capacity_assist_info_nr_present) {
    HANDLE_CODE(pack_ie(
        bref, 144, crit_e{crit_e::ignore}, [this, &bref]() { return cell_and_capacity_assist_info_nr.pack(bref); }));
  }
  if (partial_list_ind_eutra_present) {
    HANDLE_CODE(
        pack_ie(bref, 156, crit_e{crit_e::ignore}, [this, &bref]() { return partial_list_ind_eutra.pack(bref); }));
  }
  if (cell_and_capacity_assist_info_eutra_present) {
    HANDLE_CODE(pack_ie(
        bref, 157, crit_e{crit_e::ignore}, [this, &bref]() { return cell_and_capacity_assist_info_eutra.pack(bref); }));
  }
  if (local_ng_ran_node_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 341, crit_e{crit_e::ignore}, [this, &bref]() { return local_ng_ran_node_id.pack(bref); }));
  }
  if (neighbour_ng_ran_node_list_present) {
    HANDLE_CODE(pack_ie(bref, 342, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, neighbour_ng_ran_node_list, 0, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE xn_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &xn_setup_request_ies_o::get_crit));

    switch (id) {
      case 14: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 75: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(tai_support_list, bref, 1, 256, true); }));
        break;
      }
      case 4: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(amf_region_info, bref, 1, 16, true); }));
        break;
      }
      case 19: {
        list_of_served_cells_nr_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(list_of_served_cells_nr, bref, 1, 16384, true); }));
        break;
      }
      case 18: {
        list_of_served_cells_e_utra_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(list_of_served_cells_e_utra, bref, 1, 16384, true); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 141: {
        tnl_cfg_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return tnl_cfg_info.unpack(bref); }));
        break;
      }
      case 142: {
        partial_list_ind_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return partial_list_ind_nr.unpack(bref); }));
        break;
      }
      case 144: {
        cell_and_capacity_assist_info_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cell_and_capacity_assist_info_nr.unpack(bref); }));
        break;
      }
      case 156: {
        partial_list_ind_eutra_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return partial_list_ind_eutra.unpack(bref); }));
        break;
      }
      case 157: {
        cell_and_capacity_assist_info_eutra_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return cell_and_capacity_assist_info_eutra.unpack(bref); }));
        break;
      }
      case 341: {
        local_ng_ran_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return local_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 342: {
        neighbour_ng_ran_node_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(neighbour_ng_ran_node_list, bref, 0, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void xn_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 14);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_ng_ran_node_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 75);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : tai_support_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 4);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : amf_region_info) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (list_of_served_cells_nr_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : list_of_served_cells_nr) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (list_of_served_cells_e_utra_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : list_of_served_cells_e_utra) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  if (tnl_cfg_info_present) {
    j.start_obj();
    j.write_int("id", 141);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    tnl_cfg_info.to_json(j);
    j.end_obj();
  }
  if (partial_list_ind_nr_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_str("value", "partial");
    j.end_obj();
  }
  if (cell_and_capacity_assist_info_nr_present) {
    j.start_obj();
    j.write_int("id", 144);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cell_and_capacity_assist_info_nr.to_json(j);
    j.end_obj();
  }
  if (partial_list_ind_eutra_present) {
    j.start_obj();
    j.write_int("id", 156);
    j.write_str("criticality", "ignore");
    j.write_str("value", "partial");
    j.end_obj();
  }
  if (cell_and_capacity_assist_info_eutra_present) {
    j.start_obj();
    j.write_int("id", 157);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cell_and_capacity_assist_info_eutra.to_json(j);
    j.end_obj();
  }
  if (local_ng_ran_node_id_present) {
    j.start_obj();
    j.write_int("id", 341);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    local_ng_ran_node_id.to_json(j);
    j.end_obj();
  }
  if (neighbour_ng_ran_node_list_present) {
    j.start_obj();
    j.write_int("id", 342);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : neighbour_ng_ran_node_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// XnSetupResponse-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t xn_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {14, 75, 19, 18, 10, 4, 130, 141, 142, 144, 156, 157, 341, 342};
  return map_enum_number(names, 14, idx, "id");
}
bool xn_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {14, 75, 19, 18, 10, 4, 130, 141, 142, 144, 156, 157, 341, 342};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e xn_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 14:
      return crit_e::reject;
    case 75:
      return crit_e::reject;
    case 19:
      return crit_e::reject;
    case 18:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    case 4:
      return crit_e::reject;
    case 130:
      return crit_e::reject;
    case 141:
      return crit_e::ignore;
    case 142:
      return crit_e::ignore;
    case 144:
      return crit_e::ignore;
    case 156:
      return crit_e::ignore;
    case 157:
      return crit_e::ignore;
    case 341:
      return crit_e::ignore;
    case 342:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e xn_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 14:
      return presence_e::mandatory;
    case 75:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    case 18:
      return presence_e::optional;
    case 10:
      return presence_e::optional;
    case 4:
      return presence_e::optional;
    case 130:
      return presence_e::optional;
    case 141:
      return presence_e::optional;
    case 142:
      return presence_e::optional;
    case 144:
      return presence_e::optional;
    case 156:
      return presence_e::optional;
    case 157:
      return presence_e::optional;
    case 341:
      return presence_e::optional;
    case 342:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE xn_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += list_of_served_cells_nr_present ? 1 : 0;
  nof_ies += list_of_served_cells_e_utra_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += amf_region_info_present ? 1 : 0;
  nof_ies += interface_instance_ind_present ? 1 : 0;
  nof_ies += tnl_cfg_info_present ? 1 : 0;
  nof_ies += partial_list_ind_nr_present ? 1 : 0;
  nof_ies += cell_and_capacity_assist_info_nr_present ? 1 : 0;
  nof_ies += partial_list_ind_eutra_present ? 1 : 0;
  nof_ies += cell_and_capacity_assist_info_eutra_present ? 1 : 0;
  nof_ies += local_ng_ran_node_id_present ? 1 : 0;
  nof_ies += neighbour_ng_ran_node_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 14, crit_e{crit_e::reject}, [this, &bref]() { return global_ng_ran_node_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 75, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, tai_support_list, 1, 256, true);
  }));
  if (list_of_served_cells_nr_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, list_of_served_cells_nr, 1, 16384, true);
    }));
  }
  if (list_of_served_cells_e_utra_present) {
    HANDLE_CODE(pack_ie(bref, 18, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, list_of_served_cells_e_utra, 1, 16384, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (amf_region_info_present) {
    HANDLE_CODE(pack_ie(bref, 4, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, amf_region_info, 1, 16, true);
    }));
  }
  if (interface_instance_ind_present) {
    HANDLE_CODE(pack_ie(bref, 130, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, interface_instance_ind, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }
  if (tnl_cfg_info_present) {
    HANDLE_CODE(pack_ie(bref, 141, crit_e{crit_e::ignore}, [this, &bref]() { return tnl_cfg_info.pack(bref); }));
  }
  if (partial_list_ind_nr_present) {
    HANDLE_CODE(pack_ie(bref, 142, crit_e{crit_e::ignore}, [this, &bref]() { return partial_list_ind_nr.pack(bref); }));
  }
  if (cell_and_capacity_assist_info_nr_present) {
    HANDLE_CODE(pack_ie(
        bref, 144, crit_e{crit_e::ignore}, [this, &bref]() { return cell_and_capacity_assist_info_nr.pack(bref); }));
  }
  if (partial_list_ind_eutra_present) {
    HANDLE_CODE(
        pack_ie(bref, 156, crit_e{crit_e::ignore}, [this, &bref]() { return partial_list_ind_eutra.pack(bref); }));
  }
  if (cell_and_capacity_assist_info_eutra_present) {
    HANDLE_CODE(pack_ie(
        bref, 157, crit_e{crit_e::ignore}, [this, &bref]() { return cell_and_capacity_assist_info_eutra.pack(bref); }));
  }
  if (local_ng_ran_node_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 341, crit_e{crit_e::ignore}, [this, &bref]() { return local_ng_ran_node_id.pack(bref); }));
  }
  if (neighbour_ng_ran_node_list_present) {
    HANDLE_CODE(pack_ie(bref, 342, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, neighbour_ng_ran_node_list, 0, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE xn_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &xn_setup_resp_ies_o::get_crit));

    switch (id) {
      case 14: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 75: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(tai_support_list, bref, 1, 256, true); }));
        break;
      }
      case 19: {
        list_of_served_cells_nr_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(list_of_served_cells_nr, bref, 1, 16384, true); }));
        break;
      }
      case 18: {
        list_of_served_cells_e_utra_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(list_of_served_cells_e_utra, bref, 1, 16384, true); }));
        break;
      }
      case 10: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 4: {
        amf_region_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(amf_region_info, bref, 1, 16, true); }));
        break;
      }
      case 130: {
        interface_instance_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(interface_instance_ind, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 141: {
        tnl_cfg_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return tnl_cfg_info.unpack(bref); }));
        break;
      }
      case 142: {
        partial_list_ind_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return partial_list_ind_nr.unpack(bref); }));
        break;
      }
      case 144: {
        cell_and_capacity_assist_info_nr_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cell_and_capacity_assist_info_nr.unpack(bref); }));
        break;
      }
      case 156: {
        partial_list_ind_eutra_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return partial_list_ind_eutra.unpack(bref); }));
        break;
      }
      case 157: {
        cell_and_capacity_assist_info_eutra_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return cell_and_capacity_assist_info_eutra.unpack(bref); }));
        break;
      }
      case 341: {
        local_ng_ran_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return local_ng_ran_node_id.unpack(bref); }));
        break;
      }
      case 342: {
        neighbour_ng_ran_node_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(neighbour_ng_ran_node_list, bref, 0, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void xn_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 14);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_ng_ran_node_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 75);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : tai_support_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (list_of_served_cells_nr_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : list_of_served_cells_nr) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (list_of_served_cells_e_utra_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : list_of_served_cells_e_utra) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (amf_region_info_present) {
    j.start_obj();
    j.write_int("id", 4);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : amf_region_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (interface_instance_ind_present) {
    j.start_obj();
    j.write_int("id", 130);
    j.write_str("criticality", "reject");
    j.write_int("value", interface_instance_ind);
    j.end_obj();
  }
  if (tnl_cfg_info_present) {
    j.start_obj();
    j.write_int("id", 141);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    tnl_cfg_info.to_json(j);
    j.end_obj();
  }
  if (partial_list_ind_nr_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_str("value", "partial");
    j.end_obj();
  }
  if (cell_and_capacity_assist_info_nr_present) {
    j.start_obj();
    j.write_int("id", 144);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cell_and_capacity_assist_info_nr.to_json(j);
    j.end_obj();
  }
  if (partial_list_ind_eutra_present) {
    j.start_obj();
    j.write_int("id", 156);
    j.write_str("criticality", "ignore");
    j.write_str("value", "partial");
    j.end_obj();
  }
  if (cell_and_capacity_assist_info_eutra_present) {
    j.start_obj();
    j.write_int("id", 157);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cell_and_capacity_assist_info_eutra.to_json(j);
    j.end_obj();
  }
  if (local_ng_ran_node_id_present) {
    j.start_obj();
    j.write_int("id", 341);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    local_ng_ran_node_id.to_json(j);
    j.end_obj();
  }
  if (neighbour_ng_ran_node_list_present) {
    j.start_obj();
    j.write_int("id", 342);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : neighbour_ng_ran_node_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// XnUAddressIndication-IEs ::= OBJECT SET OF XNAP-PROTOCOL-IES
uint32_t xn_u_address_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {27, 29, 11, 165, 246, 334, 375, 274, 451};
  return map_enum_number(names, 9, idx, "id");
}
bool xn_u_address_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {27, 29, 11, 165, 246, 334, 375, 274, 451};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e xn_u_address_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 27:
      return crit_e::ignore;
    case 29:
      return crit_e::ignore;
    case 11:
      return crit_e::reject;
    case 165:
      return crit_e::reject;
    case 246:
      return crit_e::ignore;
    case 334:
      return crit_e::reject;
    case 375:
      return crit_e::ignore;
    case 274:
      return crit_e::ignore;
    case 451:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e xn_u_address_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 27:
      return presence_e::mandatory;
    case 29:
      return presence_e::mandatory;
    case 11:
      return presence_e::mandatory;
    case 165:
      return presence_e::optional;
    case 246:
      return presence_e::optional;
    case 334:
      return presence_e::optional;
    case 375:
      return presence_e::optional;
    case 274:
      return presence_e::optional;
    case 451:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE xn_u_address_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += cho_mrdc_ind_present ? 1 : 0;
  nof_ies += cho_mrdc_early_data_forwarding_present ? 1 : 0;
  nof_ies += cp_c_data_forwarding_ind_present ? 1 : 0;
  nof_ies += mbs_data_forwarding_ind_present ? 1 : 0;
  nof_ies += mbs_session_info_resp_list_present ? 1 : 0;
  nof_ies += pdu_setbased_handling_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, new_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, old_ng_ran_node_ue_xnap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 11, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, xn_u_address_infoper_pdu_session_list, 1, 256, true);
  }));
  if (cho_mrdc_ind_present) {
    HANDLE_CODE(pack_ie(bref, 165, crit_e{crit_e::reject}, [this, &bref]() { return cho_mrdc_ind.pack(bref); }));
  }
  if (cho_mrdc_early_data_forwarding_present) {
    HANDLE_CODE(pack_ie(
        bref, 246, crit_e{crit_e::ignore}, [this, &bref]() { return cho_mrdc_early_data_forwarding.pack(bref); }));
  }
  if (cp_c_data_forwarding_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 334, crit_e{crit_e::reject}, [this, &bref]() { return cp_c_data_forwarding_ind.pack(bref); }));
  }
  if (mbs_data_forwarding_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 375, crit_e{crit_e::ignore}, [this, &bref]() { return mbs_data_forwarding_ind.pack(bref); }));
  }
  if (mbs_session_info_resp_list_present) {
    HANDLE_CODE(pack_ie(bref, 274, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, mbs_session_info_resp_list, 1, 256, true);
    }));
  }
  if (pdu_setbased_handling_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 451, crit_e{crit_e::ignore}, [this, &bref]() { return pdu_setbased_handling_ind.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE xn_u_address_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &xn_u_address_ind_ies_o::get_crit));

    switch (id) {
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(old_ng_ran_node_ue_xnap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 11: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(xn_u_address_infoper_pdu_session_list, bref, 1, 256, true);
        }));
        break;
      }
      case 165: {
        cho_mrdc_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cho_mrdc_ind.unpack(bref); }));
        break;
      }
      case 246: {
        cho_mrdc_early_data_forwarding_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cho_mrdc_early_data_forwarding.unpack(bref); }));
        break;
      }
      case 334: {
        cp_c_data_forwarding_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cp_c_data_forwarding_ind.unpack(bref); }));
        break;
      }
      case 375: {
        mbs_data_forwarding_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_data_forwarding_ind.unpack(bref); }));
        break;
      }
      case 274: {
        mbs_session_info_resp_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(mbs_session_info_resp_list, bref, 1, 256, true); }));
        break;
      }
      case 451: {
        pdu_setbased_handling_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pdu_setbased_handling_ind.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void xn_u_address_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "ignore");
  j.write_uint("value", new_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "ignore");
  j.write_uint("value", old_ng_ran_node_ue_xnap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 11);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : xn_u_address_infoper_pdu_session_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (cho_mrdc_ind_present) {
    j.start_obj();
    j.write_int("id", 165);
    j.write_str("criticality", "reject");
    j.write_str("value", cho_mrdc_ind.to_string());
    j.end_obj();
  }
  if (cho_mrdc_early_data_forwarding_present) {
    j.start_obj();
    j.write_int("id", 246);
    j.write_str("criticality", "ignore");
    j.write_str("value", "stop");
    j.end_obj();
  }
  if (cp_c_data_forwarding_ind_present) {
    j.start_obj();
    j.write_int("id", 334);
    j.write_str("criticality", "reject");
    j.write_str("value", cp_c_data_forwarding_ind.to_string());
    j.end_obj();
  }
  if (mbs_data_forwarding_ind_present) {
    j.start_obj();
    j.write_int("id", 375);
    j.write_str("criticality", "ignore");
    j.write_str("value", "mbs-only");
    j.end_obj();
  }
  if (mbs_session_info_resp_list_present) {
    j.start_obj();
    j.write_int("id", 274);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : mbs_session_info_resp_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_setbased_handling_ind_present) {
    j.start_obj();
    j.write_int("id", 451);
    j.write_str("criticality", "ignore");
    j.write_str("value", "supported");
    j.end_obj();
  }
  j.end_array();
}
