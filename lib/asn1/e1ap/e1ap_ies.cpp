// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/e1ap/e1ap_ies.h"

using namespace asn1;
using namespace asn1::e1ap;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// TriggeringMessage ::= ENUMERATED
const char* trigger_msg_opts::to_string() const
{
  static const char* names[] = {"initiating-message", "successful-outcome", "unsuccessful-outcome"};
  return convert_enum_idx(names, 3, value, "trigger_msg_e");
}

// DRB-Activity ::= ENUMERATED
const char* drb_activity_opts::to_string() const
{
  static const char* names[] = {"active", "not-active"};
  return convert_enum_idx(names, 2, value, "drb_activity_e");
}

// DRB-Activity-Item ::= SEQUENCE
OCUDUASN_CODE drb_activity_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(drb_activity.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_activity_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(drb_activity.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_activity_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_str("dRB-Activity", drb_activity.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Resource-Activity ::= ENUMERATED
const char* pdu_session_res_activity_opts::to_string() const
{
  static const char* names[] = {"active", "not-active"};
  return convert_enum_idx(names, 2, value, "pdu_session_res_activity_e");
}

// PDU-Session-Resource-Activity-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_activity_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pdu_session_res_activity.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_activity_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pdu_session_res_activity.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_activity_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.write_str("pDU-Session-Resource-Activity", pdu_session_res_activity.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// UE-Activity ::= ENUMERATED
const char* ue_activity_opts::to_string() const
{
  static const char* names[] = {"active", "not-active"};
  return convert_enum_idx(names, 2, value, "ue_activity_e");
}

// ActivityInformation ::= CHOICE
void activity_info_c::destroy_()
{
  switch (type_) {
    case types::drb_activity_list:
      c.destroy<drb_activity_list_l>();
      break;
    case types::pdu_session_res_activity_list:
      c.destroy<pdu_session_res_activity_list_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<activity_info_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void activity_info_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::drb_activity_list:
      c.init<drb_activity_list_l>();
      break;
    case types::pdu_session_res_activity_list:
      c.init<pdu_session_res_activity_list_l>();
      break;
    case types::ue_activity:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<activity_info_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "activity_info_c");
  }
}
activity_info_c::activity_info_c(const activity_info_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::drb_activity_list:
      c.init(other.c.get<drb_activity_list_l>());
      break;
    case types::pdu_session_res_activity_list:
      c.init(other.c.get<pdu_session_res_activity_list_l>());
      break;
    case types::ue_activity:
      c.init(other.c.get<ue_activity_e>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<activity_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "activity_info_c");
  }
}
activity_info_c& activity_info_c::operator=(const activity_info_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::drb_activity_list:
      c.set(other.c.get<drb_activity_list_l>());
      break;
    case types::pdu_session_res_activity_list:
      c.set(other.c.get<pdu_session_res_activity_list_l>());
      break;
    case types::ue_activity:
      c.set(other.c.get<ue_activity_e>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<activity_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "activity_info_c");
  }

  return *this;
}
drb_activity_list_l& activity_info_c::set_drb_activity_list()
{
  set(types::drb_activity_list);
  return c.get<drb_activity_list_l>();
}
pdu_session_res_activity_list_l& activity_info_c::set_pdu_session_res_activity_list()
{
  set(types::pdu_session_res_activity_list);
  return c.get<pdu_session_res_activity_list_l>();
}
ue_activity_e& activity_info_c::set_ue_activity()
{
  set(types::ue_activity);
  return c.get<ue_activity_e>();
}
protocol_ie_single_container_s<activity_info_ext_ies_o>& activity_info_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<activity_info_ext_ies_o>>();
}
void activity_info_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::drb_activity_list:
      j.start_array("dRB-Activity-List");
      for (const auto& e1 : c.get<drb_activity_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::pdu_session_res_activity_list:
      j.start_array("pDU-Session-Resource-Activity-List");
      for (const auto& e1 : c.get<pdu_session_res_activity_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ue_activity:
      j.write_str("uE-Activity", c.get<ue_activity_e>().to_string());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<activity_info_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "activity_info_c");
  }
  j.end_obj();
}
OCUDUASN_CODE activity_info_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::drb_activity_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<drb_activity_list_l>(), 1, 32, true));
      break;
    case types::pdu_session_res_activity_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<pdu_session_res_activity_list_l>(), 1, 256, true));
      break;
    case types::ue_activity:
      HANDLE_CODE(c.get<ue_activity_e>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<activity_info_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "activity_info_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE activity_info_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::drb_activity_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<drb_activity_list_l>(), bref, 1, 32, true));
      break;
    case types::pdu_session_res_activity_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<pdu_session_res_activity_list_l>(), bref, 1, 256, true));
      break;
    case types::ue_activity:
      HANDLE_CODE(c.get<ue_activity_e>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<activity_info_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "activity_info_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* activity_info_c::types_opts::to_string() const
{
  static const char* names[] = {
      "dRB-Activity-List", "pDU-Session-Resource-Activity-List", "uE-Activity", "choice-extension"};
  return convert_enum_idx(names, 4, value, "activity_info_c::types");
}

// ActivityNotificationLevel ::= ENUMERATED
const char* activity_notif_level_opts::to_string() const
{
  static const char* names[] = {"drb", "pdu-session", "ue"};
  return convert_enum_idx(names, 3, value, "activity_notif_level_e");
}

// AdditionalHandoverInfo ::= ENUMERATED
const char* add_ho_info_opts::to_string() const
{
  static const char* names[] = {"discard-pdpc-SN"};
  return convert_enum_idx(names, 1, value, "add_ho_info_e");
}

// AdditionalPDCPduplicationInformation ::= ENUMERATED
const char* add_pdcp_dupl_info_opts::to_string() const
{
  static const char* names[] = {"three", "four"};
  return convert_enum_idx(names, 2, value, "add_pdcp_dupl_info_e");
}
uint8_t add_pdcp_dupl_info_opts::to_number() const
{
  static const uint8_t numbers[] = {3, 4};
  return map_enum_number(numbers, 2, value, "add_pdcp_dupl_info_e");
}

// PacketErrorRate ::= SEQUENCE
OCUDUASN_CODE packet_error_rate_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, per_scalar, (uint8_t)0u, (uint8_t)9u, true, true));
  HANDLE_CODE(pack_integer(bref, per_exponent, (uint8_t)0u, (uint8_t)9u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE packet_error_rate_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(per_scalar, bref, (uint8_t)0u, (uint8_t)9u, true, true));
  HANDLE_CODE(unpack_integer(per_exponent, bref, (uint8_t)0u, (uint8_t)9u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void packet_error_rate_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pER-Scalar", per_scalar);
  j.write_int("pER-Exponent", per_exponent);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// AlternativeQoSParaSetItem-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t alt_qos_para_set_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {216};
  return map_enum_number(names, 1, idx, "id");
}
bool alt_qos_para_set_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 216 == id;
}
crit_e alt_qos_para_set_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 216) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e alt_qos_para_set_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 216) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE alt_qos_para_set_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += max_data_burst_volume_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (max_data_burst_volume_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)216, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, max_data_burst_volume, (uint16_t)0u, (uint16_t)4095u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE alt_qos_para_set_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != alt_qos_para_set_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 216) {
      {
        max_data_burst_volume_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(max_data_burst_volume, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void alt_qos_para_set_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (max_data_burst_volume_present) {
    j.start_obj();
    j.write_int("id", 216);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", max_data_burst_volume);
    j.end_obj();
  }
  j.end_array();
}

// AlternativeQoSParaSetItem ::= SEQUENCE
OCUDUASN_CODE alt_qos_para_set_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(guaranteed_flow_bit_rate_dl_present, 1));
  HANDLE_CODE(bref.pack(guaranteed_flow_bit_rate_ul_present, 1));
  HANDLE_CODE(bref.pack(packet_delay_budget_present, 1));
  HANDLE_CODE(bref.pack(packet_error_rate_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, alt_qos_param_idx, (uint8_t)1u, (uint8_t)8u, true, true));
  if (guaranteed_flow_bit_rate_dl_present) {
    HANDLE_CODE(pack_integer(bref, guaranteed_flow_bit_rate_dl, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  if (guaranteed_flow_bit_rate_ul_present) {
    HANDLE_CODE(pack_integer(bref, guaranteed_flow_bit_rate_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  if (packet_delay_budget_present) {
    HANDLE_CODE(pack_integer(bref, packet_delay_budget, (uint16_t)0u, (uint16_t)1023u, true, true));
  }
  if (packet_error_rate_present) {
    HANDLE_CODE(packet_error_rate.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE alt_qos_para_set_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(guaranteed_flow_bit_rate_dl_present, 1));
  HANDLE_CODE(bref.unpack(guaranteed_flow_bit_rate_ul_present, 1));
  HANDLE_CODE(bref.unpack(packet_delay_budget_present, 1));
  HANDLE_CODE(bref.unpack(packet_error_rate_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(alt_qos_param_idx, bref, (uint8_t)1u, (uint8_t)8u, true, true));
  if (guaranteed_flow_bit_rate_dl_present) {
    HANDLE_CODE(unpack_integer(guaranteed_flow_bit_rate_dl, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  if (guaranteed_flow_bit_rate_ul_present) {
    HANDLE_CODE(unpack_integer(guaranteed_flow_bit_rate_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  if (packet_delay_budget_present) {
    HANDLE_CODE(unpack_integer(packet_delay_budget, bref, (uint16_t)0u, (uint16_t)1023u, true, true));
  }
  if (packet_error_rate_present) {
    HANDLE_CODE(packet_error_rate.unpack(bref));
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
void alt_qos_para_set_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("alternativeQoSParameterIndex", alt_qos_param_idx);
  if (guaranteed_flow_bit_rate_dl_present) {
    j.write_uint("guaranteedFlowBitRateDL", guaranteed_flow_bit_rate_dl);
  }
  if (guaranteed_flow_bit_rate_ul_present) {
    j.write_uint("guaranteedFlowBitRateUL", guaranteed_flow_bit_rate_ul);
  }
  if (packet_delay_budget_present) {
    j.write_int("packetDelayBudget", packet_delay_budget);
  }
  if (packet_error_rate_present) {
    j.write_fieldname("packetErrorRate");
    packet_error_rate.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GTPTunnel ::= SEQUENCE
OCUDUASN_CODE gtp_tunnel_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(transport_layer_address.pack(bref));
  HANDLE_CODE(gtp_teid.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gtp_tunnel_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(transport_layer_address.unpack(bref));
  HANDLE_CODE(gtp_teid.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void gtp_tunnel_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("transportLayerAddress");
  transport_layer_address.to_json(j);
  j.write_str("gTP-TEID", gtp_teid.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// UP-TNL-Information ::= CHOICE
void up_tnl_info_c::destroy_()
{
  switch (type_) {
    case types::gtp_tunnel:
      c.destroy<gtp_tunnel_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<up_tnl_info_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void up_tnl_info_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::gtp_tunnel:
      c.init<gtp_tunnel_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<up_tnl_info_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "up_tnl_info_c");
  }
}
up_tnl_info_c::up_tnl_info_c(const up_tnl_info_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::gtp_tunnel:
      c.init(other.c.get<gtp_tunnel_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<up_tnl_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "up_tnl_info_c");
  }
}
up_tnl_info_c& up_tnl_info_c::operator=(const up_tnl_info_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::gtp_tunnel:
      c.set(other.c.get<gtp_tunnel_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<up_tnl_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "up_tnl_info_c");
  }

  return *this;
}
gtp_tunnel_s& up_tnl_info_c::set_gtp_tunnel()
{
  set(types::gtp_tunnel);
  return c.get<gtp_tunnel_s>();
}
protocol_ie_single_container_s<up_tnl_info_ext_ies_o>& up_tnl_info_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<up_tnl_info_ext_ies_o>>();
}
void up_tnl_info_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::gtp_tunnel:
      j.write_fieldname("gTPTunnel");
      c.get<gtp_tunnel_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<up_tnl_info_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "up_tnl_info_c");
  }
  j.end_obj();
}
OCUDUASN_CODE up_tnl_info_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::gtp_tunnel:
      HANDLE_CODE(c.get<gtp_tunnel_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<up_tnl_info_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "up_tnl_info_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE up_tnl_info_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::gtp_tunnel:
      HANDLE_CODE(c.get<gtp_tunnel_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<up_tnl_info_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "up_tnl_info_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* up_tnl_info_c::types_opts::to_string() const
{
  static const char* names[] = {"gTPTunnel", "choice-extension"};
  return convert_enum_idx(names, 2, value, "up_tnl_info_c::types");
}

// MBSF1UInformationAtCU ::= SEQUENCE
OCUDUASN_CODE mbsf1_u_info_at_cu_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mbs_f1u_info_at_cu.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbsf1_u_info_at_cu_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mbs_f1u_info_at_cu.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbsf1_u_info_at_cu_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mbs-f1u-info-at-CU");
  mbs_f1u_info_at_cu.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// LocationDependentMBSF1UInformationAtCU-Item ::= SEQUENCE
OCUDUASN_CODE location_dependent_mbsf1_u_info_at_cu_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_f1u_info_at_cu.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_dependent_mbsf1_u_info_at_cu_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_f1u_info_at_cu.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void location_dependent_mbsf1_u_info_at_cu_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mbsAreaSession-ID", mbs_area_session_id);
  j.write_fieldname("mbs-f1u-info-at-CU");
  mbs_f1u_info_at_cu.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextF1U-TNLInfoatCU ::= CHOICE
void bc_bearer_context_f1_u_tnl_infoat_cu_c::destroy_()
{
  switch (type_) {
    case types::locationindependent:
      c.destroy<mbsf1_u_info_at_cu_s>();
      break;
    case types::locationdependent:
      c.destroy<location_dependent_mbsf1_u_info_at_cu_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void bc_bearer_context_f1_u_tnl_infoat_cu_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::locationindependent:
      c.init<mbsf1_u_info_at_cu_s>();
      break;
    case types::locationdependent:
      c.init<location_dependent_mbsf1_u_info_at_cu_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_cu_c");
  }
}
bc_bearer_context_f1_u_tnl_infoat_cu_c::bc_bearer_context_f1_u_tnl_infoat_cu_c(
    const bc_bearer_context_f1_u_tnl_infoat_cu_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::locationindependent:
      c.init(other.c.get<mbsf1_u_info_at_cu_s>());
      break;
    case types::locationdependent:
      c.init(other.c.get<location_dependent_mbsf1_u_info_at_cu_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_cu_c");
  }
}
bc_bearer_context_f1_u_tnl_infoat_cu_c&
bc_bearer_context_f1_u_tnl_infoat_cu_c::operator=(const bc_bearer_context_f1_u_tnl_infoat_cu_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::locationindependent:
      c.set(other.c.get<mbsf1_u_info_at_cu_s>());
      break;
    case types::locationdependent:
      c.set(other.c.get<location_dependent_mbsf1_u_info_at_cu_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_cu_c");
  }

  return *this;
}
mbsf1_u_info_at_cu_s& bc_bearer_context_f1_u_tnl_infoat_cu_c::set_locationindependent()
{
  set(types::locationindependent);
  return c.get<mbsf1_u_info_at_cu_s>();
}
location_dependent_mbsf1_u_info_at_cu_l& bc_bearer_context_f1_u_tnl_infoat_cu_c::set_locationdependent()
{
  set(types::locationdependent);
  return c.get<location_dependent_mbsf1_u_info_at_cu_l>();
}
protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>&
bc_bearer_context_f1_u_tnl_infoat_cu_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>>();
}
void bc_bearer_context_f1_u_tnl_infoat_cu_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::locationindependent:
      j.write_fieldname("locationindependent");
      c.get<mbsf1_u_info_at_cu_s>().to_json(j);
      break;
    case types::locationdependent:
      j.start_array("locationdependent");
      for (const auto& e1 : c.get<location_dependent_mbsf1_u_info_at_cu_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_cu_c");
  }
  j.end_obj();
}
OCUDUASN_CODE bc_bearer_context_f1_u_tnl_infoat_cu_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mbsf1_u_info_at_cu_s>().pack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<location_dependent_mbsf1_u_info_at_cu_l>(), 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_cu_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_f1_u_tnl_infoat_cu_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mbsf1_u_info_at_cu_s>().unpack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<location_dependent_mbsf1_u_info_at_cu_l>(), bref, 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_cu_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_cu_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* bc_bearer_context_f1_u_tnl_infoat_cu_c::types_opts::to_string() const
{
  static const char* names[] = {"locationindependent", "locationdependent", "choice-extension"};
  return convert_enum_idx(names, 3, value, "bc_bearer_context_f1_u_tnl_infoat_cu_c::types");
}

// F1UTunnelNotEstablished ::= ENUMERATED
const char* f1_u_tunnel_not_established_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "f1_u_tunnel_not_established_e");
}

// MBSF1UInformationAtDU-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t mbsf1_u_info_at_du_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {207};
  return map_enum_number(names, 1, idx, "id");
}
bool mbsf1_u_info_at_du_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 207 == id;
}
crit_e mbsf1_u_info_at_du_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 207) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e mbsf1_u_info_at_du_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 207) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE mbsf1_u_info_at_du_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += f1_u_tunnel_not_established_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (f1_u_tunnel_not_established_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)207, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(f1_u_tunnel_not_established.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbsf1_u_info_at_du_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != mbsf1_u_info_at_du_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 207) {
      {
        f1_u_tunnel_not_established_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(f1_u_tunnel_not_established.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void mbsf1_u_info_at_du_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (f1_u_tunnel_not_established_present) {
    j.start_obj();
    j.write_int("id", 207);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  j.end_array();
}

// MBSF1UInformationAtDU ::= SEQUENCE
OCUDUASN_CODE mbsf1_u_info_at_du_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mbs_f1u_info_at_du.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbsf1_u_info_at_du_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mbs_f1u_info_at_du.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbsf1_u_info_at_du_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mbs-f1u-info-at-DU");
  mbs_f1u_info_at_du.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// LocationDependentMBSF1UInformationAtDU-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t location_dependent_mbsf1_u_info_at_du_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {207};
  return map_enum_number(names, 1, idx, "id");
}
bool location_dependent_mbsf1_u_info_at_du_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 207 == id;
}
crit_e location_dependent_mbsf1_u_info_at_du_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 207) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e location_dependent_mbsf1_u_info_at_du_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 207) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE location_dependent_mbsf1_u_info_at_du_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += f1_u_tunnel_not_established_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (f1_u_tunnel_not_established_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)207, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(f1_u_tunnel_not_established.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_dependent_mbsf1_u_info_at_du_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != location_dependent_mbsf1_u_info_at_du_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 207) {
      {
        f1_u_tunnel_not_established_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(f1_u_tunnel_not_established.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void location_dependent_mbsf1_u_info_at_du_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (f1_u_tunnel_not_established_present) {
    j.start_obj();
    j.write_int("id", 207);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  j.end_array();
}

// LocationDependentMBSF1UInformationAtDU-Item ::= SEQUENCE
OCUDUASN_CODE location_dependent_mbsf1_u_info_at_du_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_f1u_info_at_du.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_dependent_mbsf1_u_info_at_du_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_f1u_info_at_du.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void location_dependent_mbsf1_u_info_at_du_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mbsAreaSession-ID", mbs_area_session_id);
  j.write_fieldname("mbs-f1u-info-at-DU");
  mbs_f1u_info_at_du.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextF1U-TNLInfoatDU ::= CHOICE
void bc_bearer_context_f1_u_tnl_infoat_du_c::destroy_()
{
  switch (type_) {
    case types::locationindependent:
      c.destroy<mbsf1_u_info_at_du_s>();
      break;
    case types::locationdependent:
      c.destroy<location_dependent_mbsf1_u_info_at_du_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void bc_bearer_context_f1_u_tnl_infoat_du_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::locationindependent:
      c.init<mbsf1_u_info_at_du_s>();
      break;
    case types::locationdependent:
      c.init<location_dependent_mbsf1_u_info_at_du_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_du_c");
  }
}
bc_bearer_context_f1_u_tnl_infoat_du_c::bc_bearer_context_f1_u_tnl_infoat_du_c(
    const bc_bearer_context_f1_u_tnl_infoat_du_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::locationindependent:
      c.init(other.c.get<mbsf1_u_info_at_du_s>());
      break;
    case types::locationdependent:
      c.init(other.c.get<location_dependent_mbsf1_u_info_at_du_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_du_c");
  }
}
bc_bearer_context_f1_u_tnl_infoat_du_c&
bc_bearer_context_f1_u_tnl_infoat_du_c::operator=(const bc_bearer_context_f1_u_tnl_infoat_du_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::locationindependent:
      c.set(other.c.get<mbsf1_u_info_at_du_s>());
      break;
    case types::locationdependent:
      c.set(other.c.get<location_dependent_mbsf1_u_info_at_du_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_du_c");
  }

  return *this;
}
mbsf1_u_info_at_du_s& bc_bearer_context_f1_u_tnl_infoat_du_c::set_locationindependent()
{
  set(types::locationindependent);
  return c.get<mbsf1_u_info_at_du_s>();
}
location_dependent_mbsf1_u_info_at_du_l& bc_bearer_context_f1_u_tnl_infoat_du_c::set_locationdependent()
{
  set(types::locationdependent);
  return c.get<location_dependent_mbsf1_u_info_at_du_l>();
}
protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>&
bc_bearer_context_f1_u_tnl_infoat_du_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>>();
}
void bc_bearer_context_f1_u_tnl_infoat_du_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::locationindependent:
      j.write_fieldname("locationindependent");
      c.get<mbsf1_u_info_at_du_s>().to_json(j);
      break;
    case types::locationdependent:
      j.start_array("locationdependent");
      for (const auto& e1 : c.get<location_dependent_mbsf1_u_info_at_du_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_du_c");
  }
  j.end_obj();
}
OCUDUASN_CODE bc_bearer_context_f1_u_tnl_infoat_du_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mbsf1_u_info_at_du_s>().pack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<location_dependent_mbsf1_u_info_at_du_l>(), 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_du_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_f1_u_tnl_infoat_du_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mbsf1_u_info_at_du_s>().unpack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<location_dependent_mbsf1_u_info_at_du_l>(), bref, 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bc_bearer_context_f1_u_tnl_infoat_du_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_f1_u_tnl_infoat_du_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* bc_bearer_context_f1_u_tnl_infoat_du_c::types_opts::to_string() const
{
  static const char* names[] = {"locationindependent", "locationdependent", "choice-extension"};
  return convert_enum_idx(names, 3, value, "bc_bearer_context_f1_u_tnl_infoat_du_c::types");
}

// MBSNGUInformationAt5GC-Multicast ::= SEQUENCE
OCUDUASN_CODE mb_sn_gu_info_at5_gc_multicast_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ipmc_address.pack(bref));
  HANDLE_CODE(ipsource_address.pack(bref));
  HANDLE_CODE(gtp_dl_teid.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mb_sn_gu_info_at5_gc_multicast_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ipmc_address.unpack(bref));
  HANDLE_CODE(ipsource_address.unpack(bref));
  HANDLE_CODE(gtp_dl_teid.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mb_sn_gu_info_at5_gc_multicast_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ipmcAddress");
  ipmc_address.to_json(j);
  j.write_fieldname("ipsourceAddress");
  ipsource_address.to_json(j);
  j.write_str("gtpDLTEID", gtp_dl_teid.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MBSNGUInformationAt5GC ::= CHOICE
void mb_sn_gu_info_at5_gc_c::destroy_()
{
  switch (type_) {
    case types::multicast:
      c.destroy<mb_sn_gu_info_at5_gc_multicast_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void mb_sn_gu_info_at5_gc_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::multicast:
      c.init<mb_sn_gu_info_at5_gc_multicast_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at5_gc_c");
  }
}
mb_sn_gu_info_at5_gc_c::mb_sn_gu_info_at5_gc_c(const mb_sn_gu_info_at5_gc_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::multicast:
      c.init(other.c.get<mb_sn_gu_info_at5_gc_multicast_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at5_gc_c");
  }
}
mb_sn_gu_info_at5_gc_c& mb_sn_gu_info_at5_gc_c::operator=(const mb_sn_gu_info_at5_gc_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::multicast:
      c.set(other.c.get<mb_sn_gu_info_at5_gc_multicast_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at5_gc_c");
  }

  return *this;
}
mb_sn_gu_info_at5_gc_multicast_s& mb_sn_gu_info_at5_gc_c::set_multicast()
{
  set(types::multicast);
  return c.get<mb_sn_gu_info_at5_gc_multicast_s>();
}
protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>& mb_sn_gu_info_at5_gc_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>>();
}
void mb_sn_gu_info_at5_gc_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::multicast:
      j.write_fieldname("multicast");
      c.get<mb_sn_gu_info_at5_gc_multicast_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at5_gc_c");
  }
  j.end_obj();
}
OCUDUASN_CODE mb_sn_gu_info_at5_gc_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::multicast:
      HANDLE_CODE(c.get<mb_sn_gu_info_at5_gc_multicast_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at5_gc_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mb_sn_gu_info_at5_gc_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::multicast:
      HANDLE_CODE(c.get<mb_sn_gu_info_at5_gc_multicast_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mb_sn_gu_info_at5_gc_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at5_gc_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mb_sn_gu_info_at5_gc_c::types_opts::to_string() const
{
  static const char* names[] = {"multicast", "choice-extension"};
  return convert_enum_idx(names, 2, value, "mb_sn_gu_info_at5_gc_c::types");
}

// LocationDependentMBSNGUInformationAt5GC-Item ::= SEQUENCE
OCUDUASN_CODE location_dependent_mb_sn_gu_info_at5_gc_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_ngu_info_at5_gc.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_dependent_mb_sn_gu_info_at5_gc_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_ngu_info_at5_gc.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void location_dependent_mb_sn_gu_info_at5_gc_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mbsAreaSession-ID", mbs_area_session_id);
  j.write_fieldname("mbsNGUInformationAt5GC");
  mbs_ngu_info_at5_gc.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextNGU-TNLInfoat5GC ::= CHOICE
void bc_bearer_context_ngu_tnl_infoat5_gc_c::destroy_()
{
  switch (type_) {
    case types::locationindependent:
      c.destroy<mb_sn_gu_info_at5_gc_c>();
      break;
    case types::locationdependent:
      c.destroy<location_dependent_mb_sn_gu_info_at5_gc_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void bc_bearer_context_ngu_tnl_infoat5_gc_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::locationindependent:
      c.init<mb_sn_gu_info_at5_gc_c>();
      break;
    case types::locationdependent:
      c.init<location_dependent_mb_sn_gu_info_at5_gc_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat5_gc_c");
  }
}
bc_bearer_context_ngu_tnl_infoat5_gc_c::bc_bearer_context_ngu_tnl_infoat5_gc_c(
    const bc_bearer_context_ngu_tnl_infoat5_gc_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::locationindependent:
      c.init(other.c.get<mb_sn_gu_info_at5_gc_c>());
      break;
    case types::locationdependent:
      c.init(other.c.get<location_dependent_mb_sn_gu_info_at5_gc_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat5_gc_c");
  }
}
bc_bearer_context_ngu_tnl_infoat5_gc_c&
bc_bearer_context_ngu_tnl_infoat5_gc_c::operator=(const bc_bearer_context_ngu_tnl_infoat5_gc_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::locationindependent:
      c.set(other.c.get<mb_sn_gu_info_at5_gc_c>());
      break;
    case types::locationdependent:
      c.set(other.c.get<location_dependent_mb_sn_gu_info_at5_gc_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat5_gc_c");
  }

  return *this;
}
mb_sn_gu_info_at5_gc_c& bc_bearer_context_ngu_tnl_infoat5_gc_c::set_locationindependent()
{
  set(types::locationindependent);
  return c.get<mb_sn_gu_info_at5_gc_c>();
}
location_dependent_mb_sn_gu_info_at5_gc_l& bc_bearer_context_ngu_tnl_infoat5_gc_c::set_locationdependent()
{
  set(types::locationdependent);
  return c.get<location_dependent_mb_sn_gu_info_at5_gc_l>();
}
protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>&
bc_bearer_context_ngu_tnl_infoat5_gc_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>>();
}
void bc_bearer_context_ngu_tnl_infoat5_gc_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::locationindependent:
      j.write_fieldname("locationindependent");
      c.get<mb_sn_gu_info_at5_gc_c>().to_json(j);
      break;
    case types::locationdependent:
      j.start_array("locationdependent");
      for (const auto& e1 : c.get<location_dependent_mb_sn_gu_info_at5_gc_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat5_gc_c");
  }
  j.end_obj();
}
OCUDUASN_CODE bc_bearer_context_ngu_tnl_infoat5_gc_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mb_sn_gu_info_at5_gc_c>().pack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<location_dependent_mb_sn_gu_info_at5_gc_l>(), 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat5_gc_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_ngu_tnl_infoat5_gc_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mb_sn_gu_info_at5_gc_c>().unpack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<location_dependent_mb_sn_gu_info_at5_gc_l>(), bref, 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat5_gc_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat5_gc_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* bc_bearer_context_ngu_tnl_infoat5_gc_c::types_opts::to_string() const
{
  static const char* names[] = {"locationindependent", "locationdependent", "choice-extension"};
  return convert_enum_idx(names, 3, value, "bc_bearer_context_ngu_tnl_infoat5_gc_c::types");
}

// MBSNGUInformationAtNGRAN ::= CHOICE
void mb_sn_gu_info_at_ngran_c::destroy_()
{
  switch (type_) {
    case types::unicast:
      c.destroy<up_tnl_info_c>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void mb_sn_gu_info_at_ngran_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::unicast:
      c.init<up_tnl_info_c>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at_ngran_c");
  }
}
mb_sn_gu_info_at_ngran_c::mb_sn_gu_info_at_ngran_c(const mb_sn_gu_info_at_ngran_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::unicast:
      c.init(other.c.get<up_tnl_info_c>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at_ngran_c");
  }
}
mb_sn_gu_info_at_ngran_c& mb_sn_gu_info_at_ngran_c::operator=(const mb_sn_gu_info_at_ngran_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::unicast:
      c.set(other.c.get<up_tnl_info_c>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at_ngran_c");
  }

  return *this;
}
up_tnl_info_c& mb_sn_gu_info_at_ngran_c::set_unicast()
{
  set(types::unicast);
  return c.get<up_tnl_info_c>();
}
protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>& mb_sn_gu_info_at_ngran_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>>();
}
void mb_sn_gu_info_at_ngran_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::unicast:
      j.write_fieldname("unicast");
      c.get<up_tnl_info_c>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at_ngran_c");
  }
  j.end_obj();
}
OCUDUASN_CODE mb_sn_gu_info_at_ngran_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::unicast:
      HANDLE_CODE(c.get<up_tnl_info_c>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at_ngran_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mb_sn_gu_info_at_ngran_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::unicast:
      HANDLE_CODE(c.get<up_tnl_info_c>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mb_sn_gu_info_at_ngran_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mb_sn_gu_info_at_ngran_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mb_sn_gu_info_at_ngran_c::types_opts::to_string() const
{
  static const char* names[] = {"unicast", "choice-extension"};
  return convert_enum_idx(names, 2, value, "mb_sn_gu_info_at_ngran_c::types");
}

// LocationDependentMBSNGUInformationAtNGRAN-Item ::= SEQUENCE
OCUDUASN_CODE location_dependent_mb_sn_gu_info_at_ngran_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_ngu_info_at_ngran.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_dependent_mb_sn_gu_info_at_ngran_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_ngu_info_at_ngran.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void location_dependent_mb_sn_gu_info_at_ngran_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mbsAreaSession-ID", mbs_area_session_id);
  j.write_fieldname("mbsNGUInformationAtNGRAN");
  mbs_ngu_info_at_ngran.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextNGU-TNLInfoatNGRAN ::= CHOICE
void bc_bearer_context_ngu_tnl_infoat_ngran_c::destroy_()
{
  switch (type_) {
    case types::locationindependent:
      c.destroy<mb_sn_gu_info_at_ngran_c>();
      break;
    case types::locationdependent:
      c.destroy<location_dependent_mb_sn_gu_info_at_ngran_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void bc_bearer_context_ngu_tnl_infoat_ngran_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::locationindependent:
      c.init<mb_sn_gu_info_at_ngran_c>();
      break;
    case types::locationdependent:
      c.init<location_dependent_mb_sn_gu_info_at_ngran_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_c");
  }
}
bc_bearer_context_ngu_tnl_infoat_ngran_c::bc_bearer_context_ngu_tnl_infoat_ngran_c(
    const bc_bearer_context_ngu_tnl_infoat_ngran_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::locationindependent:
      c.init(other.c.get<mb_sn_gu_info_at_ngran_c>());
      break;
    case types::locationdependent:
      c.init(other.c.get<location_dependent_mb_sn_gu_info_at_ngran_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_c");
  }
}
bc_bearer_context_ngu_tnl_infoat_ngran_c&
bc_bearer_context_ngu_tnl_infoat_ngran_c::operator=(const bc_bearer_context_ngu_tnl_infoat_ngran_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::locationindependent:
      c.set(other.c.get<mb_sn_gu_info_at_ngran_c>());
      break;
    case types::locationdependent:
      c.set(other.c.get<location_dependent_mb_sn_gu_info_at_ngran_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_c");
  }

  return *this;
}
mb_sn_gu_info_at_ngran_c& bc_bearer_context_ngu_tnl_infoat_ngran_c::set_locationindependent()
{
  set(types::locationindependent);
  return c.get<mb_sn_gu_info_at_ngran_c>();
}
location_dependent_mb_sn_gu_info_at_ngran_l& bc_bearer_context_ngu_tnl_infoat_ngran_c::set_locationdependent()
{
  set(types::locationdependent);
  return c.get<location_dependent_mb_sn_gu_info_at_ngran_l>();
}
protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>&
bc_bearer_context_ngu_tnl_infoat_ngran_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>();
}
void bc_bearer_context_ngu_tnl_infoat_ngran_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::locationindependent:
      j.write_fieldname("locationindependent");
      c.get<mb_sn_gu_info_at_ngran_c>().to_json(j);
      break;
    case types::locationdependent:
      j.start_array("locationdependent");
      for (const auto& e1 : c.get<location_dependent_mb_sn_gu_info_at_ngran_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_c");
  }
  j.end_obj();
}
OCUDUASN_CODE bc_bearer_context_ngu_tnl_infoat_ngran_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mb_sn_gu_info_at_ngran_c>().pack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<location_dependent_mb_sn_gu_info_at_ngran_l>(), 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_ngu_tnl_infoat_ngran_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mb_sn_gu_info_at_ngran_c>().unpack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<location_dependent_mb_sn_gu_info_at_ngran_l>(), bref, 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(
          c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* bc_bearer_context_ngu_tnl_infoat_ngran_c::types_opts::to_string() const
{
  static const char* names[] = {"locationindependent", "locationdependent", "choice-extension"};
  return convert_enum_idx(names, 3, value, "bc_bearer_context_ngu_tnl_infoat_ngran_c::types");
}

// MBSNGUInformationAtNGRAN-Request ::= ENUMERATED
const char* mb_sn_gu_info_at_ngran_request_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "mb_sn_gu_info_at_ngran_request_e");
}

// MBSNGUInformationAtNGRAN-Request-Item ::= SEQUENCE
OCUDUASN_CODE mb_sn_gu_info_at_ngran_request_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_ngu_info_at_ngran_request.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mb_sn_gu_info_at_ngran_request_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_ngu_info_at_ngran_request.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mb_sn_gu_info_at_ngran_request_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mbsAreaSession-ID", mbs_area_session_id);
  j.write_str("mbsNGUInformationAtNGRAN-Request", "true");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextNGU-TNLInfoatNGRAN-Request ::= CHOICE
void bc_bearer_context_ngu_tnl_infoat_ngran_request_c::destroy_()
{
  switch (type_) {
    case types::locationdependent:
      c.destroy<mb_sn_gu_info_at_ngran_request_list_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void bc_bearer_context_ngu_tnl_infoat_ngran_request_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::locationindependent:
      break;
    case types::locationdependent:
      c.init<mb_sn_gu_info_at_ngran_request_list_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_request_c");
  }
}
bc_bearer_context_ngu_tnl_infoat_ngran_request_c::bc_bearer_context_ngu_tnl_infoat_ngran_request_c(
    const bc_bearer_context_ngu_tnl_infoat_ngran_request_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::locationindependent:
      c.init(other.c.get<mb_sn_gu_info_at_ngran_request_e>());
      break;
    case types::locationdependent:
      c.init(other.c.get<mb_sn_gu_info_at_ngran_request_list_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_request_c");
  }
}
bc_bearer_context_ngu_tnl_infoat_ngran_request_c& bc_bearer_context_ngu_tnl_infoat_ngran_request_c::operator=(
    const bc_bearer_context_ngu_tnl_infoat_ngran_request_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::locationindependent:
      c.set(other.c.get<mb_sn_gu_info_at_ngran_request_e>());
      break;
    case types::locationdependent:
      c.set(other.c.get<mb_sn_gu_info_at_ngran_request_list_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_request_c");
  }

  return *this;
}
mb_sn_gu_info_at_ngran_request_e& bc_bearer_context_ngu_tnl_infoat_ngran_request_c::set_locationindependent()
{
  set(types::locationindependent);
  return c.get<mb_sn_gu_info_at_ngran_request_e>();
}
mb_sn_gu_info_at_ngran_request_list_l& bc_bearer_context_ngu_tnl_infoat_ngran_request_c::set_locationdependent()
{
  set(types::locationdependent);
  return c.get<mb_sn_gu_info_at_ngran_request_list_l>();
}
protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>&
bc_bearer_context_ngu_tnl_infoat_ngran_request_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>>();
}
void bc_bearer_context_ngu_tnl_infoat_ngran_request_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::locationindependent:
      j.write_str("locationindependent", "true");
      break;
    case types::locationdependent:
      j.start_array("locationdependent");
      for (const auto& e1 : c.get<mb_sn_gu_info_at_ngran_request_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_request_c");
  }
  j.end_obj();
}
OCUDUASN_CODE bc_bearer_context_ngu_tnl_infoat_ngran_request_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mb_sn_gu_info_at_ngran_request_e>().pack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<mb_sn_gu_info_at_ngran_request_list_l>(), 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(
          c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_request_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_ngu_tnl_infoat_ngran_request_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mb_sn_gu_info_at_ngran_request_e>().unpack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<mb_sn_gu_info_at_ngran_request_list_l>(), bref, 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(
          c.get<protocol_ie_single_container_s<bc_bearer_context_ngu_tnl_infoat_ngran_request_ext_ies_o>>().unpack(
              bref));
      break;
    default:
      log_invalid_choice_id(type_, "bc_bearer_context_ngu_tnl_infoat_ngran_request_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* bc_bearer_context_ngu_tnl_infoat_ngran_request_c::types_opts::to_string() const
{
  static const char* names[] = {"locationindependent", "locationdependent", "choice-extension"};
  return convert_enum_idx(names, 3, value, "bc_bearer_context_ngu_tnl_infoat_ngran_request_c::types");
}

// PDCP-SN-Size ::= ENUMERATED
const char* pdcp_sn_size_opts::to_string() const
{
  static const char* names[] = {"s-12", "s-18", "s-7", "s-15", "s-16"};
  return convert_enum_idx(names, 5, value, "pdcp_sn_size_e");
}
int8_t pdcp_sn_size_opts::to_number() const
{
  static const int8_t numbers[] = {-12, -18, -7, -15, -16};
  return map_enum_number(numbers, 5, value, "pdcp_sn_size_e");
}

// RLC-Mode ::= ENUMERATED
const char* rlc_mode_opts::to_string() const
{
  static const char* names[] = {
      "rlc-tm", "rlc-am", "rlc-um-bidirectional", "rlc-um-unidirectional-ul", "rlc-um-unidirectional-dl"};
  return convert_enum_idx(names, 5, value, "rlc_mode_e");
}

// ROHC ::= SEQUENCE
OCUDUASN_CODE rohc_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(continue_rohc_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, max_c_id, (uint16_t)0u, (uint16_t)16383u, true, true));
  HANDLE_CODE(pack_integer(bref, rohc_profiles, (uint16_t)0u, (uint16_t)511u, true, true));
  if (continue_rohc_present) {
    HANDLE_CODE(continue_rohc.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rohc_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(continue_rohc_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(max_c_id, bref, (uint16_t)0u, (uint16_t)16383u, true, true));
  HANDLE_CODE(unpack_integer(rohc_profiles, bref, (uint16_t)0u, (uint16_t)511u, true, true));
  if (continue_rohc_present) {
    HANDLE_CODE(continue_rohc.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void rohc_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("maxCID", max_c_id);
  j.write_int("rOHC-Profiles", rohc_profiles);
  if (continue_rohc_present) {
    j.write_str("continueROHC", "true");
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* rohc_s::continue_rohc_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "rohc_s::continue_rohc_e_");
}

// UplinkOnlyROHC ::= SEQUENCE
OCUDUASN_CODE ul_only_rohc_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(continue_rohc_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, max_c_id, (uint16_t)0u, (uint16_t)16383u, true, true));
  HANDLE_CODE(pack_integer(bref, rohc_profiles, (uint16_t)0u, (uint16_t)511u, true, true));
  if (continue_rohc_present) {
    HANDLE_CODE(continue_rohc.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_only_rohc_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(continue_rohc_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(max_c_id, bref, (uint16_t)0u, (uint16_t)16383u, true, true));
  HANDLE_CODE(unpack_integer(rohc_profiles, bref, (uint16_t)0u, (uint16_t)511u, true, true));
  if (continue_rohc_present) {
    HANDLE_CODE(continue_rohc.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ul_only_rohc_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("maxCID", max_c_id);
  j.write_int("rOHC-Profiles", rohc_profiles);
  if (continue_rohc_present) {
    j.write_str("continueROHC", "true");
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* ul_only_rohc_s::continue_rohc_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "ul_only_rohc_s::continue_rohc_e_");
}

// ROHC-Parameters ::= CHOICE
void rohc_params_c::destroy_()
{
  switch (type_) {
    case types::rohc:
      c.destroy<rohc_s>();
      break;
    case types::ul_only_rohc:
      c.destroy<ul_only_rohc_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<rohc_params_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void rohc_params_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::rohc:
      c.init<rohc_s>();
      break;
    case types::ul_only_rohc:
      c.init<ul_only_rohc_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<rohc_params_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rohc_params_c");
  }
}
rohc_params_c::rohc_params_c(const rohc_params_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::rohc:
      c.init(other.c.get<rohc_s>());
      break;
    case types::ul_only_rohc:
      c.init(other.c.get<ul_only_rohc_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<rohc_params_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rohc_params_c");
  }
}
rohc_params_c& rohc_params_c::operator=(const rohc_params_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::rohc:
      c.set(other.c.get<rohc_s>());
      break;
    case types::ul_only_rohc:
      c.set(other.c.get<ul_only_rohc_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<rohc_params_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rohc_params_c");
  }

  return *this;
}
rohc_s& rohc_params_c::set_rohc()
{
  set(types::rohc);
  return c.get<rohc_s>();
}
ul_only_rohc_s& rohc_params_c::set_ul_only_rohc()
{
  set(types::ul_only_rohc);
  return c.get<ul_only_rohc_s>();
}
protocol_ie_single_container_s<rohc_params_ext_ies_o>& rohc_params_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<rohc_params_ext_ies_o>>();
}
void rohc_params_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::rohc:
      j.write_fieldname("rOHC");
      c.get<rohc_s>().to_json(j);
      break;
    case types::ul_only_rohc:
      j.write_fieldname("uPlinkOnlyROHC");
      c.get<ul_only_rohc_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<rohc_params_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "rohc_params_c");
  }
  j.end_obj();
}
OCUDUASN_CODE rohc_params_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::rohc:
      HANDLE_CODE(c.get<rohc_s>().pack(bref));
      break;
    case types::ul_only_rohc:
      HANDLE_CODE(c.get<ul_only_rohc_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<rohc_params_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "rohc_params_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rohc_params_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::rohc:
      HANDLE_CODE(c.get<rohc_s>().unpack(bref));
      break;
    case types::ul_only_rohc:
      HANDLE_CODE(c.get<ul_only_rohc_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<rohc_params_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "rohc_params_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* rohc_params_c::types_opts::to_string() const
{
  static const char* names[] = {"rOHC", "uPlinkOnlyROHC", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "rohc_params_c::types");
}

// T-Reordering ::= ENUMERATED
const char* t_reordering_opts::to_string() const
{
  static const char* names[] = {"ms0",    "ms1",    "ms2",    "ms4",    "ms5",    "ms8",    "ms10",   "ms15",
                                "ms20",   "ms30",   "ms40",   "ms50",   "ms60",   "ms80",   "ms100",  "ms120",
                                "ms140",  "ms160",  "ms180",  "ms200",  "ms220",  "ms240",  "ms260",  "ms280",
                                "ms300",  "ms500",  "ms750",  "ms1000", "ms1250", "ms1500", "ms1750", "ms2000",
                                "ms2250", "ms2500", "ms2750", "ms3000"};
  return convert_enum_idx(names, 36, value, "t_reordering_e");
}
uint16_t t_reordering_opts::to_number() const
{
  static const uint16_t numbers[] = {0,   1,   2,   4,    5,    8,    10,   15,   20,   30,   40,   50,
                                     60,  80,  100, 120,  140,  160,  180,  200,  220,  240,  260,  280,
                                     300, 500, 750, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, 3000};
  return map_enum_number(numbers, 36, value, "t_reordering_e");
}

// T-ReorderingTimer ::= SEQUENCE
OCUDUASN_CODE t_reordering_timer_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(t_reordering.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE t_reordering_timer_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(t_reordering.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void t_reordering_timer_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("t-Reordering", t_reordering.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DiscardTimer ::= ENUMERATED
const char* discard_timer_opts::to_string() const
{
  static const char* names[] = {"ms10",
                                "ms20",
                                "ms30",
                                "ms40",
                                "ms50",
                                "ms60",
                                "ms75",
                                "ms100",
                                "ms150",
                                "ms200",
                                "ms250",
                                "ms300",
                                "ms500",
                                "ms750",
                                "ms1500",
                                "infinity"};
  return convert_enum_idx(names, 16, value, "discard_timer_e");
}
int16_t discard_timer_opts::to_number() const
{
  static const int16_t numbers[] = {10, 20, 30, 40, 50, 60, 75, 100, 150, 200, 250, 300, 500, 750, 1500, -1};
  return map_enum_number(numbers, 16, value, "discard_timer_e");
}

// ULDataSplitThreshold ::= ENUMERATED
const char* ul_data_split_thres_opts::to_string() const
{
  static const char* names[] = {"b0",       "b100",     "b200",     "b400",     "b800",     "b1600",
                                "b3200",    "b6400",    "b12800",   "b25600",   "b51200",   "b102400",
                                "b204800",  "b409600",  "b819200",  "b1228800", "b1638400", "b2457600",
                                "b3276800", "b4096000", "b4915200", "b5734400", "b6553600", "infinity"};
  return convert_enum_idx(names, 24, value, "ul_data_split_thres_e");
}
int32_t ul_data_split_thres_opts::to_number() const
{
  static const int32_t numbers[] = {0,       100,     200,     400,     800,     1600,    3200,    6400,
                                    12800,   25600,   51200,   102400,  204800,  409600,  819200,  1228800,
                                    1638400, 2457600, 3276800, 4096000, 4915200, 5734400, 6553600, -1};
  return map_enum_number(numbers, 24, value, "ul_data_split_thres_e");
}

// PDCP-Duplication ::= ENUMERATED
const char* pdcp_dupl_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "pdcp_dupl_e");
}

// PDCP-Reestablishment ::= ENUMERATED
const char* pdcp_reest_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "pdcp_reest_e");
}

// PDCP-DataRecovery ::= ENUMERATED
const char* pdcp_data_recovery_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "pdcp_data_recovery_e");
}

// Duplication-Activation ::= ENUMERATED
const char* dupl_activation_opts::to_string() const
{
  static const char* names[] = {"active", "inactive"};
  return convert_enum_idx(names, 2, value, "dupl_activation_e");
}

// OutOfOrderDelivery ::= ENUMERATED
const char* out_of_order_delivery_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "out_of_order_delivery_e");
}

// PDCP-StatusReportIndication ::= ENUMERATED
const char* pdcp_status_report_ind_opts::to_string() const
{
  static const char* names[] = {"downlink", "uplink", "both"};
  return convert_enum_idx(names, 3, value, "pdcp_status_report_ind_e");
}

// EHC-Common-Parameters ::= SEQUENCE
OCUDUASN_CODE ehc_common_params_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ehc_c_id_len.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ehc_common_params_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ehc_c_id_len.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ehc_common_params_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("ehc-CID-Length", ehc_c_id_len.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* ehc_common_params_s::ehc_c_id_len_opts::to_string() const
{
  static const char* names[] = {"bits7", "bits15"};
  return convert_enum_idx(names, 2, value, "ehc_common_params_s::ehc_c_id_len_e_");
}
uint8_t ehc_common_params_s::ehc_c_id_len_opts::to_number() const
{
  static const uint8_t numbers[] = {7, 15};
  return map_enum_number(numbers, 2, value, "ehc_common_params_s::ehc_c_id_len_e_");
}

// EHC-Downlink-Parameters-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t ehc_dl_params_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {137};
  return map_enum_number(names, 1, idx, "id");
}
bool ehc_dl_params_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 137 == id;
}
crit_e ehc_dl_params_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 137) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e ehc_dl_params_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 137) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE ehc_dl_params_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += max_c_id_ehc_dl_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (max_c_id_ehc_dl_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)137, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, max_c_id_ehc_dl, (uint32_t)1u, (uint32_t)32767u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ehc_dl_params_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ehc_dl_params_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 137) {
      {
        max_c_id_ehc_dl_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(max_c_id_ehc_dl, bref, (uint32_t)1u, (uint32_t)32767u, true, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void ehc_dl_params_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (max_c_id_ehc_dl_present) {
    j.start_obj();
    j.write_int("id", 137);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", max_c_id_ehc_dl);
    j.end_obj();
  }
  j.end_array();
}

// EHC-Downlink-Parameters ::= SEQUENCE
OCUDUASN_CODE ehc_dl_params_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(drb_continue_ehc_dl.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ehc_dl_params_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(drb_continue_ehc_dl.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ehc_dl_params_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("drb-ContinueEHC-DL", drb_continue_ehc_dl.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* ehc_dl_params_s::drb_continue_ehc_dl_opts::to_string() const
{
  static const char* names[] = {"true", "false"};
  return convert_enum_idx(names, 2, value, "ehc_dl_params_s::drb_continue_ehc_dl_e_");
}

// EHC-Uplink-Parameters ::= SEQUENCE
OCUDUASN_CODE ehc_ul_params_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(drb_continue_ehc_ul.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ehc_ul_params_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(drb_continue_ehc_ul.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ehc_ul_params_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("drb-ContinueEHC-UL", drb_continue_ehc_ul.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* ehc_ul_params_s::drb_continue_ehc_ul_opts::to_string() const
{
  static const char* names[] = {"true", "false"};
  return convert_enum_idx(names, 2, value, "ehc_ul_params_s::drb_continue_ehc_ul_e_");
}

// EHC-Parameters ::= SEQUENCE
OCUDUASN_CODE ehc_params_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ehc_dl_present, 1));
  HANDLE_CODE(bref.pack(ehc_ul_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ehc_common.pack(bref));
  if (ehc_dl_present) {
    HANDLE_CODE(ehc_dl.pack(bref));
  }
  if (ehc_ul_present) {
    HANDLE_CODE(ehc_ul.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ehc_params_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ehc_dl_present, 1));
  HANDLE_CODE(bref.unpack(ehc_ul_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ehc_common.unpack(bref));
  if (ehc_dl_present) {
    HANDLE_CODE(ehc_dl.unpack(bref));
  }
  if (ehc_ul_present) {
    HANDLE_CODE(ehc_ul.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ehc_params_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ehc-Common");
  ehc_common.to_json(j);
  if (ehc_dl_present) {
    j.write_fieldname("ehc-Downlink");
    ehc_dl.to_json(j);
  }
  if (ehc_ul_present) {
    j.write_fieldname("ehc-Uplink");
    ehc_ul.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BufferSize ::= ENUMERATED
const char* buffer_size_opts::to_string() const
{
  static const char* names[] = {"kbyte2", "kbyte4", "kbyte8"};
  return convert_enum_idx(names, 3, value, "buffer_size_e");
}
uint8_t buffer_size_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 8};
  return map_enum_number(numbers, 3, value, "buffer_size_e");
}

// Dictionary ::= ENUMERATED
const char* dictionary_opts::to_string() const
{
  static const char* names[] = {"sip-SDP", "operator"};
  return convert_enum_idx(names, 2, value, "dictionary_e");
}

// UDC-Parameters-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t udc_params_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {186};
  return map_enum_number(names, 1, idx, "id");
}
bool udc_params_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 186 == id;
}
crit_e udc_params_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 186) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e udc_params_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 186) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE udc_params_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += version_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (version_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)186, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, version_id, (uint8_t)0u, (uint8_t)15u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE udc_params_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != udc_params_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 186) {
      {
        version_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(version_id, bref, (uint8_t)0u, (uint8_t)15u, false, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void udc_params_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (version_id_present) {
    j.start_obj();
    j.write_int("id", 186);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", version_id);
    j.end_obj();
  }
  j.end_array();
}

// UDC-Parameters ::= SEQUENCE
OCUDUASN_CODE udc_params_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(dictionary_present, 1));
  HANDLE_CODE(bref.pack(continue_udc_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(buffer_size.pack(bref));
  if (dictionary_present) {
    HANDLE_CODE(dictionary.pack(bref));
  }
  if (continue_udc_present) {
    HANDLE_CODE(continue_udc.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE udc_params_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(dictionary_present, 1));
  HANDLE_CODE(bref.unpack(continue_udc_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(buffer_size.unpack(bref));
  if (dictionary_present) {
    HANDLE_CODE(dictionary.unpack(bref));
  }
  if (continue_udc_present) {
    HANDLE_CODE(continue_udc.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void udc_params_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("bufferSize", buffer_size.to_string());
  if (dictionary_present) {
    j.write_str("dictionary", dictionary.to_string());
  }
  if (continue_udc_present) {
    j.write_str("continueUDC", "true");
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* udc_params_s::continue_udc_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "udc_params_s::continue_udc_e_");
}

// DiscardTimerExtended ::= ENUMERATED
const char* discard_timer_extended_opts::to_string() const
{
  static const char* names[] = {"ms0dot5", "ms1", "ms2", "ms4", "ms6", "ms8", "ms2000"};
  return convert_enum_idx(names, 7, value, "discard_timer_extended_e");
}
float discard_timer_extended_opts::to_number() const
{
  static const float numbers[] = {0.5, 1.0, 2.0, 4.0, 6.0, 8.0, 2000.0};
  return map_enum_number(numbers, 7, value, "discard_timer_extended_e");
}
const char* discard_timer_extended_opts::to_number_string() const
{
  static const char* number_strs[] = {"0.5", "1", "2", "4", "6", "8", "2000"};
  return convert_enum_idx(number_strs, 7, value, "discard_timer_extended_e");
}

// PSIbasedDiscardTimer ::= ENUMERATED
const char* psi_based_discard_timer_opts::to_string() const
{
  static const char* names[] = {"ms0",
                                "ms2",
                                "ms4",
                                "ms6",
                                "ms8",
                                "ms10",
                                "ms12",
                                "ms14",
                                "ms18",
                                "ms22",
                                "ms26",
                                "ms30",
                                "ms40",
                                "ms50",
                                "ms75",
                                "ms100"};
  return convert_enum_idx(names, 16, value, "psi_based_discard_timer_e");
}
uint8_t psi_based_discard_timer_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 2, 4, 6, 8, 10, 12, 14, 18, 22, 26, 30, 40, 50, 75, 100};
  return map_enum_number(numbers, 16, value, "psi_based_discard_timer_e");
}

// PDCPSNGapReport ::= ENUMERATED
const char* pdcp_sn_gap_report_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "pdcp_sn_gap_report_e");
}

// PDCP-Configuration-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdcp_cfg_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {88, 104, 118, 153, 177, 214, 218};
  return map_enum_number(names, 7, idx, "id");
}
bool pdcp_cfg_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {88, 104, 118, 153, 177, 214, 218};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdcp_cfg_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 88:
      return crit_e::ignore;
    case 104:
      return crit_e::ignore;
    case 118:
      return crit_e::ignore;
    case 153:
      return crit_e::ignore;
    case 177:
      return crit_e::reject;
    case 214:
      return crit_e::ignore;
    case 218:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdcp_cfg_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 88:
      return presence_e::optional;
    case 104:
      return presence_e::optional;
    case 118:
      return presence_e::optional;
    case 153:
      return presence_e::optional;
    case 177:
      return presence_e::optional;
    case 214:
      return presence_e::optional;
    case 218:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdcp_cfg_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pdcp_status_report_ind_present ? 1 : 0;
  nof_ies += add_pdcp_dupl_info_present ? 1 : 0;
  nof_ies += ehc_params_present ? 1 : 0;
  nof_ies += udc_params_present ? 1 : 0;
  nof_ies += discard_timer_extended_present ? 1 : 0;
  nof_ies += psi_based_discard_timer_present ? 1 : 0;
  nof_ies += pdcp_sn_gap_report_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (pdcp_status_report_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)88, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdcp_status_report_ind.pack(bref));
  }
  if (add_pdcp_dupl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)104, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(add_pdcp_dupl_info.pack(bref));
  }
  if (ehc_params_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)118, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ehc_params.pack(bref));
  }
  if (udc_params_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)153, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(udc_params.pack(bref));
  }
  if (discard_timer_extended_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)177, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(discard_timer_extended.pack(bref));
  }
  if (psi_based_discard_timer_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)214, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(psi_based_discard_timer.pack(bref));
  }
  if (pdcp_sn_gap_report_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)218, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdcp_sn_gap_report.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcp_cfg_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdcp_cfg_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 88: {
        pdcp_status_report_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdcp_status_report_ind.unpack(bref));
        break;
      }
      case 104: {
        add_pdcp_dupl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(add_pdcp_dupl_info.unpack(bref));
        break;
      }
      case 118: {
        ehc_params_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ehc_params.unpack(bref));
        break;
      }
      case 153: {
        udc_params_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(udc_params.unpack(bref));
        break;
      }
      case 177: {
        discard_timer_extended_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(discard_timer_extended.unpack(bref));
        break;
      }
      case 214: {
        psi_based_discard_timer_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(psi_based_discard_timer.unpack(bref));
        break;
      }
      case 218: {
        pdcp_sn_gap_report_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdcp_sn_gap_report.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdcp_cfg_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (pdcp_status_report_ind_present) {
    j.start_obj();
    j.write_int("id", 88);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", pdcp_status_report_ind.to_string());
    j.end_obj();
  }
  if (add_pdcp_dupl_info_present) {
    j.start_obj();
    j.write_int("id", 104);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", add_pdcp_dupl_info.to_string());
    j.end_obj();
  }
  if (ehc_params_present) {
    j.start_obj();
    j.write_int("id", 118);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    ehc_params.to_json(j);
    j.end_obj();
  }
  if (udc_params_present) {
    j.start_obj();
    j.write_int("id", 153);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    udc_params.to_json(j);
    j.end_obj();
  }
  if (discard_timer_extended_present) {
    j.start_obj();
    j.write_int("id", 177);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", discard_timer_extended.to_string());
    j.end_obj();
  }
  if (psi_based_discard_timer_present) {
    j.start_obj();
    j.write_int("id", 214);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", psi_based_discard_timer.to_string());
    j.end_obj();
  }
  if (pdcp_sn_gap_report_present) {
    j.start_obj();
    j.write_int("id", 218);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  j.end_array();
}

// PDCP-Configuration ::= SEQUENCE
OCUDUASN_CODE pdcp_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(rohc_params_present, 1));
  HANDLE_CODE(bref.pack(t_reordering_timer_present, 1));
  HANDLE_CODE(bref.pack(discard_timer_present, 1));
  HANDLE_CODE(bref.pack(ul_data_split_thres_present, 1));
  HANDLE_CODE(bref.pack(pdcp_dupl_present, 1));
  HANDLE_CODE(bref.pack(pdcp_reest_present, 1));
  HANDLE_CODE(bref.pack(pdcp_data_recovery_present, 1));
  HANDLE_CODE(bref.pack(dupl_activation_present, 1));
  HANDLE_CODE(bref.pack(out_of_order_delivery_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pdcp_sn_size_ul.pack(bref));
  HANDLE_CODE(pdcp_sn_size_dl.pack(bref));
  HANDLE_CODE(rlc_mode.pack(bref));
  if (rohc_params_present) {
    HANDLE_CODE(rohc_params.pack(bref));
  }
  if (t_reordering_timer_present) {
    HANDLE_CODE(t_reordering_timer.pack(bref));
  }
  if (discard_timer_present) {
    HANDLE_CODE(discard_timer.pack(bref));
  }
  if (ul_data_split_thres_present) {
    HANDLE_CODE(ul_data_split_thres.pack(bref));
  }
  if (pdcp_dupl_present) {
    HANDLE_CODE(pdcp_dupl.pack(bref));
  }
  if (pdcp_reest_present) {
    HANDLE_CODE(pdcp_reest.pack(bref));
  }
  if (pdcp_data_recovery_present) {
    HANDLE_CODE(pdcp_data_recovery.pack(bref));
  }
  if (dupl_activation_present) {
    HANDLE_CODE(dupl_activation.pack(bref));
  }
  if (out_of_order_delivery_present) {
    HANDLE_CODE(out_of_order_delivery.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcp_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(rohc_params_present, 1));
  HANDLE_CODE(bref.unpack(t_reordering_timer_present, 1));
  HANDLE_CODE(bref.unpack(discard_timer_present, 1));
  HANDLE_CODE(bref.unpack(ul_data_split_thres_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_dupl_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_reest_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_data_recovery_present, 1));
  HANDLE_CODE(bref.unpack(dupl_activation_present, 1));
  HANDLE_CODE(bref.unpack(out_of_order_delivery_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(pdcp_sn_size_ul.unpack(bref));
  HANDLE_CODE(pdcp_sn_size_dl.unpack(bref));
  HANDLE_CODE(rlc_mode.unpack(bref));
  if (rohc_params_present) {
    HANDLE_CODE(rohc_params.unpack(bref));
  }
  if (t_reordering_timer_present) {
    HANDLE_CODE(t_reordering_timer.unpack(bref));
  }
  if (discard_timer_present) {
    HANDLE_CODE(discard_timer.unpack(bref));
  }
  if (ul_data_split_thres_present) {
    HANDLE_CODE(ul_data_split_thres.unpack(bref));
  }
  if (pdcp_dupl_present) {
    HANDLE_CODE(pdcp_dupl.unpack(bref));
  }
  if (pdcp_reest_present) {
    HANDLE_CODE(pdcp_reest.unpack(bref));
  }
  if (pdcp_data_recovery_present) {
    HANDLE_CODE(pdcp_data_recovery.unpack(bref));
  }
  if (dupl_activation_present) {
    HANDLE_CODE(dupl_activation.unpack(bref));
  }
  if (out_of_order_delivery_present) {
    HANDLE_CODE(out_of_order_delivery.unpack(bref));
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
void pdcp_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pDCP-SN-Size-UL", pdcp_sn_size_ul.to_string());
  j.write_str("pDCP-SN-Size-DL", pdcp_sn_size_dl.to_string());
  j.write_str("rLC-Mode", rlc_mode.to_string());
  if (rohc_params_present) {
    j.write_fieldname("rOHC-Parameters");
    rohc_params.to_json(j);
  }
  if (t_reordering_timer_present) {
    j.write_fieldname("t-ReorderingTimer");
    t_reordering_timer.to_json(j);
  }
  if (discard_timer_present) {
    j.write_str("discardTimer", discard_timer.to_string());
  }
  if (ul_data_split_thres_present) {
    j.write_str("uLDataSplitThreshold", ul_data_split_thres.to_string());
  }
  if (pdcp_dupl_present) {
    j.write_str("pDCP-Duplication", "true");
  }
  if (pdcp_reest_present) {
    j.write_str("pDCP-Reestablishment", "true");
  }
  if (pdcp_data_recovery_present) {
    j.write_str("pDCP-DataRecovery", "true");
  }
  if (dupl_activation_present) {
    j.write_str("duplication-Activation", dupl_activation.to_string());
  }
  if (out_of_order_delivery_present) {
    j.write_str("outOfOrderDelivery", "true");
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Non-Dynamic5QIDescriptor-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t non_dyn_5qi_descriptor_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {101, 102};
  return map_enum_number(names, 2, idx, "id");
}
bool non_dyn_5qi_descriptor_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {101, 102};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e non_dyn_5qi_descriptor_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 101:
      return crit_e::ignore;
    case 102:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e non_dyn_5qi_descriptor_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 101:
      return presence_e::optional;
    case 102:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE non_dyn_5qi_descriptor_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += cn_packet_delay_budget_dl_present ? 1 : 0;
  nof_ies += cn_packet_delay_budget_ul_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (cn_packet_delay_budget_dl_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)101, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, cn_packet_delay_budget_dl, (uint32_t)1u, (uint32_t)65535u, true, true));
  }
  if (cn_packet_delay_budget_ul_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)102, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, cn_packet_delay_budget_ul, (uint32_t)1u, (uint32_t)65535u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE non_dyn_5qi_descriptor_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != non_dyn_5qi_descriptor_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 101: {
        cn_packet_delay_budget_dl_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(cn_packet_delay_budget_dl, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
        break;
      }
      case 102: {
        cn_packet_delay_budget_ul_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(cn_packet_delay_budget_ul, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void non_dyn_5qi_descriptor_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (cn_packet_delay_budget_dl_present) {
    j.start_obj();
    j.write_int("id", 101);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", cn_packet_delay_budget_dl);
    j.end_obj();
  }
  if (cn_packet_delay_budget_ul_present) {
    j.start_obj();
    j.write_int("id", 102);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", cn_packet_delay_budget_ul);
    j.end_obj();
  }
  j.end_array();
}

// Non-Dynamic5QIDescriptor ::= SEQUENCE
OCUDUASN_CODE non_dyn_5qi_descriptor_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(qos_prio_level_present, 1));
  HANDLE_CODE(bref.pack(averaging_win_present, 1));
  HANDLE_CODE(bref.pack(max_data_burst_volume_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, five_qi, (uint16_t)0u, (uint16_t)255u, true, true));
  if (qos_prio_level_present) {
    HANDLE_CODE(pack_integer(bref, qos_prio_level, (uint16_t)0u, (uint16_t)127u, true, true));
  }
  if (averaging_win_present) {
    HANDLE_CODE(pack_integer(bref, averaging_win, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (max_data_burst_volume_present) {
    HANDLE_CODE(pack_integer(bref, max_data_burst_volume, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE non_dyn_5qi_descriptor_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(qos_prio_level_present, 1));
  HANDLE_CODE(bref.unpack(averaging_win_present, 1));
  HANDLE_CODE(bref.unpack(max_data_burst_volume_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(five_qi, bref, (uint16_t)0u, (uint16_t)255u, true, true));
  if (qos_prio_level_present) {
    HANDLE_CODE(unpack_integer(qos_prio_level, bref, (uint16_t)0u, (uint16_t)127u, true, true));
  }
  if (averaging_win_present) {
    HANDLE_CODE(unpack_integer(averaging_win, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (max_data_burst_volume_present) {
    HANDLE_CODE(unpack_integer(max_data_burst_volume, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void non_dyn_5qi_descriptor_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("fiveQI", five_qi);
  if (qos_prio_level_present) {
    j.write_int("qoSPriorityLevel", qos_prio_level);
  }
  if (averaging_win_present) {
    j.write_int("averagingWindow", averaging_win);
  }
  if (max_data_burst_volume_present) {
    j.write_int("maxDataBurstVolume", max_data_burst_volume);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Dynamic5QIDescriptor-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t dyn_5qi_descriptor_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {103, 101, 102};
  return map_enum_number(names, 3, idx, "id");
}
bool dyn_5qi_descriptor_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {103, 101, 102};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dyn_5qi_descriptor_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 103:
      return crit_e::ignore;
    case 101:
      return crit_e::ignore;
    case 102:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dyn_5qi_descriptor_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 103:
      return presence_e::optional;
    case 101:
      return presence_e::optional;
    case 102:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dyn_5qi_descriptor_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += extended_packet_delay_budget_present ? 1 : 0;
  nof_ies += cn_packet_delay_budget_dl_present ? 1 : 0;
  nof_ies += cn_packet_delay_budget_ul_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (extended_packet_delay_budget_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)103, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, extended_packet_delay_budget, (uint32_t)1u, (uint32_t)65535u, true, true));
  }
  if (cn_packet_delay_budget_dl_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)101, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, cn_packet_delay_budget_dl, (uint32_t)1u, (uint32_t)65535u, true, true));
  }
  if (cn_packet_delay_budget_ul_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)102, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, cn_packet_delay_budget_ul, (uint32_t)1u, (uint32_t)65535u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dyn_5qi_descriptor_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != dyn_5qi_descriptor_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 103: {
        extended_packet_delay_budget_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(extended_packet_delay_budget, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
        break;
      }
      case 101: {
        cn_packet_delay_budget_dl_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(cn_packet_delay_budget_dl, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
        break;
      }
      case 102: {
        cn_packet_delay_budget_ul_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(cn_packet_delay_budget_ul, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void dyn_5qi_descriptor_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (extended_packet_delay_budget_present) {
    j.start_obj();
    j.write_int("id", 103);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", extended_packet_delay_budget);
    j.end_obj();
  }
  if (cn_packet_delay_budget_dl_present) {
    j.start_obj();
    j.write_int("id", 101);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", cn_packet_delay_budget_dl);
    j.end_obj();
  }
  if (cn_packet_delay_budget_ul_present) {
    j.start_obj();
    j.write_int("id", 102);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", cn_packet_delay_budget_ul);
    j.end_obj();
  }
  j.end_array();
}

// Dynamic5QIDescriptor ::= SEQUENCE
OCUDUASN_CODE dyn_5qi_descriptor_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(five_qi_present, 1));
  HANDLE_CODE(bref.pack(delay_crit_present, 1));
  HANDLE_CODE(bref.pack(averaging_win_present, 1));
  HANDLE_CODE(bref.pack(max_data_burst_volume_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_prio_level, (uint16_t)0u, (uint16_t)127u, true, true));
  HANDLE_CODE(pack_integer(bref, packet_delay_budget, (uint16_t)0u, (uint16_t)1023u, true, true));
  HANDLE_CODE(packet_error_rate.pack(bref));
  if (five_qi_present) {
    HANDLE_CODE(pack_integer(bref, five_qi, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (delay_crit_present) {
    HANDLE_CODE(delay_crit.pack(bref));
  }
  if (averaging_win_present) {
    HANDLE_CODE(pack_integer(bref, averaging_win, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (max_data_burst_volume_present) {
    HANDLE_CODE(pack_integer(bref, max_data_burst_volume, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dyn_5qi_descriptor_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(five_qi_present, 1));
  HANDLE_CODE(bref.unpack(delay_crit_present, 1));
  HANDLE_CODE(bref.unpack(averaging_win_present, 1));
  HANDLE_CODE(bref.unpack(max_data_burst_volume_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_prio_level, bref, (uint16_t)0u, (uint16_t)127u, true, true));
  HANDLE_CODE(unpack_integer(packet_delay_budget, bref, (uint16_t)0u, (uint16_t)1023u, true, true));
  HANDLE_CODE(packet_error_rate.unpack(bref));
  if (five_qi_present) {
    HANDLE_CODE(unpack_integer(five_qi, bref, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (delay_crit_present) {
    HANDLE_CODE(delay_crit.unpack(bref));
  }
  if (averaging_win_present) {
    HANDLE_CODE(unpack_integer(averaging_win, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (max_data_burst_volume_present) {
    HANDLE_CODE(unpack_integer(max_data_burst_volume, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void dyn_5qi_descriptor_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoSPriorityLevel", qos_prio_level);
  j.write_int("packetDelayBudget", packet_delay_budget);
  j.write_fieldname("packetErrorRate");
  packet_error_rate.to_json(j);
  if (five_qi_present) {
    j.write_int("fiveQI", five_qi);
  }
  if (delay_crit_present) {
    j.write_str("delayCritical", delay_crit.to_string());
  }
  if (averaging_win_present) {
    j.write_int("averagingWindow", averaging_win);
  }
  if (max_data_burst_volume_present) {
    j.write_int("maxDataBurstVolume", max_data_burst_volume);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* dyn_5qi_descriptor_s::delay_crit_opts::to_string() const
{
  static const char* names[] = {"delay-critical", "non-delay-critical"};
  return convert_enum_idx(names, 2, value, "dyn_5qi_descriptor_s::delay_crit_e_");
}

// QoS-Characteristics ::= CHOICE
void qos_characteristics_c::destroy_()
{
  switch (type_) {
    case types::non_dyn_5qi:
      c.destroy<non_dyn_5qi_descriptor_s>();
      break;
    case types::dyn_5qi:
      c.destroy<dyn_5qi_descriptor_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void qos_characteristics_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::non_dyn_5qi:
      c.init<non_dyn_5qi_descriptor_s>();
      break;
    case types::dyn_5qi:
      c.init<dyn_5qi_descriptor_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "qos_characteristics_c");
  }
}
qos_characteristics_c::qos_characteristics_c(const qos_characteristics_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::non_dyn_5qi:
      c.init(other.c.get<non_dyn_5qi_descriptor_s>());
      break;
    case types::dyn_5qi:
      c.init(other.c.get<dyn_5qi_descriptor_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "qos_characteristics_c");
  }
}
qos_characteristics_c& qos_characteristics_c::operator=(const qos_characteristics_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::non_dyn_5qi:
      c.set(other.c.get<non_dyn_5qi_descriptor_s>());
      break;
    case types::dyn_5qi:
      c.set(other.c.get<dyn_5qi_descriptor_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "qos_characteristics_c");
  }

  return *this;
}
non_dyn_5qi_descriptor_s& qos_characteristics_c::set_non_dyn_5qi()
{
  set(types::non_dyn_5qi);
  return c.get<non_dyn_5qi_descriptor_s>();
}
dyn_5qi_descriptor_s& qos_characteristics_c::set_dyn_5qi()
{
  set(types::dyn_5qi);
  return c.get<dyn_5qi_descriptor_s>();
}
protocol_ie_single_container_s<qos_characteristics_ext_ies_o>& qos_characteristics_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>();
}
void qos_characteristics_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::non_dyn_5qi:
      j.write_fieldname("non-Dynamic-5QI");
      c.get<non_dyn_5qi_descriptor_s>().to_json(j);
      break;
    case types::dyn_5qi:
      j.write_fieldname("dynamic-5QI");
      c.get<dyn_5qi_descriptor_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "qos_characteristics_c");
  }
  j.end_obj();
}
OCUDUASN_CODE qos_characteristics_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::non_dyn_5qi:
      HANDLE_CODE(c.get<non_dyn_5qi_descriptor_s>().pack(bref));
      break;
    case types::dyn_5qi:
      HANDLE_CODE(c.get<dyn_5qi_descriptor_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "qos_characteristics_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_characteristics_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::non_dyn_5qi:
      HANDLE_CODE(c.get<non_dyn_5qi_descriptor_s>().unpack(bref));
      break;
    case types::dyn_5qi:
      HANDLE_CODE(c.get<dyn_5qi_descriptor_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<qos_characteristics_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "qos_characteristics_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* qos_characteristics_c::types_opts::to_string() const
{
  static const char* names[] = {"non-Dynamic-5QI", "dynamic-5QI", "choice-extension"};
  return convert_enum_idx(names, 3, value, "qos_characteristics_c::types");
}

// Pre-emptionCapability ::= ENUMERATED
const char* pre_emption_cap_opts::to_string() const
{
  static const char* names[] = {"shall-not-trigger-pre-emption", "may-trigger-pre-emption"};
  return convert_enum_idx(names, 2, value, "pre_emption_cap_e");
}

// Pre-emptionVulnerability ::= ENUMERATED
const char* pre_emption_vulnerability_opts::to_string() const
{
  static const char* names[] = {"not-pre-emptable", "pre-emptable"};
  return convert_enum_idx(names, 2, value, "pre_emption_vulnerability_e");
}

// NGRANAllocationAndRetentionPriority ::= SEQUENCE
OCUDUASN_CODE ngran_alloc_and_retention_prio_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, prio_level, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(pre_emption_cap.pack(bref));
  HANDLE_CODE(pre_emption_vulnerability.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ngran_alloc_and_retention_prio_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prio_level, bref, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(pre_emption_cap.unpack(bref));
  HANDLE_CODE(pre_emption_vulnerability.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ngran_alloc_and_retention_prio_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("priorityLevel", prio_level);
  j.write_str("pre-emptionCapability", pre_emption_cap.to_string());
  j.write_str("pre-emptionVulnerability", pre_emption_vulnerability.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GBR-QosFlowInformation-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t gbr_qos_flow_info_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {124};
  return map_enum_number(names, 1, idx, "id");
}
bool gbr_qos_flow_info_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 124 == id;
}
crit_e gbr_qos_flow_info_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 124) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e gbr_qos_flow_info_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 124) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE gbr_qos_flow_info_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += alt_qos_para_set_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (alt_qos_para_set_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)124, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, alt_qos_para_set_list, 1, 8, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gbr_qos_flow_info_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gbr_qos_flow_info_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 124) {
      {
        alt_qos_para_set_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(alt_qos_para_set_list, bref, 1, 8, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void gbr_qos_flow_info_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (alt_qos_para_set_list_present) {
    j.start_obj();
    j.write_int("id", 124);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : alt_qos_para_set_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// GBR-QoSFlowInformation ::= SEQUENCE
OCUDUASN_CODE gbr_qos_flow_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(max_packet_loss_rate_dl_present, 1));
  HANDLE_CODE(bref.pack(max_packet_loss_rate_ul_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, max_flow_bit_rate_dl, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(pack_integer(bref, max_flow_bit_rate_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(pack_integer(bref, guaranteed_flow_bit_rate_dl, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(pack_integer(bref, guaranteed_flow_bit_rate_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  if (max_packet_loss_rate_dl_present) {
    HANDLE_CODE(pack_integer(bref, max_packet_loss_rate_dl, (uint16_t)0u, (uint16_t)1000u, true, true));
  }
  if (max_packet_loss_rate_ul_present) {
    HANDLE_CODE(pack_integer(bref, max_packet_loss_rate_ul, (uint16_t)0u, (uint16_t)1000u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gbr_qos_flow_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(max_packet_loss_rate_dl_present, 1));
  HANDLE_CODE(bref.unpack(max_packet_loss_rate_ul_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(max_flow_bit_rate_dl, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(unpack_integer(max_flow_bit_rate_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(unpack_integer(guaranteed_flow_bit_rate_dl, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(unpack_integer(guaranteed_flow_bit_rate_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  if (max_packet_loss_rate_dl_present) {
    HANDLE_CODE(unpack_integer(max_packet_loss_rate_dl, bref, (uint16_t)0u, (uint16_t)1000u, true, true));
  }
  if (max_packet_loss_rate_ul_present) {
    HANDLE_CODE(unpack_integer(max_packet_loss_rate_ul, bref, (uint16_t)0u, (uint16_t)1000u, true, true));
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
void gbr_qos_flow_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_uint("maxFlowBitRateDownlink", max_flow_bit_rate_dl);
  j.write_uint("maxFlowBitRateUplink", max_flow_bit_rate_ul);
  j.write_uint("guaranteedFlowBitRateDownlink", guaranteed_flow_bit_rate_dl);
  j.write_uint("guaranteedFlowBitRateUplink", guaranteed_flow_bit_rate_ul);
  if (max_packet_loss_rate_dl_present) {
    j.write_int("maxPacketLossRateDownlink", max_packet_loss_rate_dl);
  }
  if (max_packet_loss_rate_ul_present) {
    j.write_int("maxPacketLossRateUplink", max_packet_loss_rate_ul);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// QosMonitoringRequest ::= ENUMERATED
const char* qos_monitoring_request_opts::to_string() const
{
  static const char* names[] = {"ul", "dl", "both"};
  return convert_enum_idx(names, 3, value, "qos_monitoring_request_e");
}

// QosMonitoringDisabled ::= ENUMERATED
const char* qos_monitoring_disabled_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "qos_monitoring_disabled_e");
}

// PDUSetQoSInformation ::= SEQUENCE
OCUDUASN_CODE pdu_set_qos_info_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(pdu_set_delay_budget_present, 1));
  HANDLE_CODE(bref.pack(pdu_set_error_rate_present, 1));
  HANDLE_CODE(bref.pack(pdu_set_integrated_handling_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (pdu_set_delay_budget_present) {
    HANDLE_CODE(pack_integer(bref, pdu_set_delay_budget, (uint32_t)1u, (uint32_t)65535u, true, true));
  }
  if (pdu_set_error_rate_present) {
    HANDLE_CODE(pdu_set_error_rate.pack(bref));
  }
  if (pdu_set_integrated_handling_info_present) {
    HANDLE_CODE(pdu_set_integrated_handling_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_set_qos_info_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(pdu_set_delay_budget_present, 1));
  HANDLE_CODE(bref.unpack(pdu_set_error_rate_present, 1));
  HANDLE_CODE(bref.unpack(pdu_set_integrated_handling_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (pdu_set_delay_budget_present) {
    HANDLE_CODE(unpack_integer(pdu_set_delay_budget, bref, (uint32_t)1u, (uint32_t)65535u, true, true));
  }
  if (pdu_set_error_rate_present) {
    HANDLE_CODE(pdu_set_error_rate.unpack(bref));
  }
  if (pdu_set_integrated_handling_info_present) {
    HANDLE_CODE(pdu_set_integrated_handling_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void pdu_set_qos_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdu_set_delay_budget_present) {
    j.write_int("pduSetDelayBudget", pdu_set_delay_budget);
  }
  if (pdu_set_error_rate_present) {
    j.write_fieldname("pduSetErrorRate");
    pdu_set_error_rate.to_json(j);
  }
  if (pdu_set_integrated_handling_info_present) {
    j.write_str("pduSetIntegratedHandlingInformation", pdu_set_integrated_handling_info.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* pdu_set_qos_info_s::pdu_set_integrated_handling_info_opts::to_string() const
{
  static const char* names[] = {"true", "false"};
  return convert_enum_idx(names, 2, value, "pdu_set_qos_info_s::pdu_set_integrated_handling_info_e_");
}

// PDUSetQoSParameters ::= SEQUENCE
OCUDUASN_CODE pdu_set_qos_params_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ul_pdu_set_qos_info_present, 1));
  HANDLE_CODE(bref.pack(dl_pdu_set_qos_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (ul_pdu_set_qos_info_present) {
    HANDLE_CODE(ul_pdu_set_qos_info.pack(bref));
  }
  if (dl_pdu_set_qos_info_present) {
    HANDLE_CODE(dl_pdu_set_qos_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_set_qos_params_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ul_pdu_set_qos_info_present, 1));
  HANDLE_CODE(bref.unpack(dl_pdu_set_qos_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (ul_pdu_set_qos_info_present) {
    HANDLE_CODE(ul_pdu_set_qos_info.unpack(bref));
  }
  if (dl_pdu_set_qos_info_present) {
    HANDLE_CODE(dl_pdu_set_qos_info.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void pdu_set_qos_params_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ul_pdu_set_qos_info_present) {
    j.write_fieldname("ulPDUSetQoSInformation");
    ul_pdu_set_qos_info.to_json(j);
  }
  if (dl_pdu_set_qos_info_present) {
    j.write_fieldname("dlPDUSetQoSInformation");
    dl_pdu_set_qos_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// QoSFlowLevelQoSParameters-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t qos_flow_level_qos_params_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {87, 126, 132, 133, 142, 201};
  return map_enum_number(names, 6, idx, "id");
}
bool qos_flow_level_qos_params_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {87, 126, 132, 133, 142, 201};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e qos_flow_level_qos_params_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 87:
      return crit_e::ignore;
    case 126:
      return crit_e::ignore;
    case 132:
      return crit_e::ignore;
    case 133:
      return crit_e::ignore;
    case 142:
      return crit_e::ignore;
    case 201:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e qos_flow_level_qos_params_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 87:
      return presence_e::optional;
    case 126:
      return presence_e::optional;
    case 132:
      return presence_e::optional;
    case 133:
      return presence_e::optional;
    case 142:
      return presence_e::optional;
    case 201:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE qos_flow_level_qos_params_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += qos_monitoring_request_present ? 1 : 0;
  nof_ies += mcg_offered_gbr_qos_flow_info_present ? 1 : 0;
  nof_ies += qos_monitoring_report_freq_present ? 1 : 0;
  nof_ies += qos_monitoring_disabled_present ? 1 : 0;
  nof_ies += data_forwarding_source_ip_address_present ? 1 : 0;
  nof_ies += pdu_set_qos_params_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (qos_monitoring_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)87, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(qos_monitoring_request.pack(bref));
  }
  if (mcg_offered_gbr_qos_flow_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)126, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mcg_offered_gbr_qos_flow_info.pack(bref));
  }
  if (qos_monitoring_report_freq_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)132, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, qos_monitoring_report_freq, (uint16_t)1u, (uint16_t)1800u, true, true));
  }
  if (qos_monitoring_disabled_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)133, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(qos_monitoring_disabled.pack(bref));
  }
  if (data_forwarding_source_ip_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)142, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(data_forwarding_source_ip_address.pack(bref));
  }
  if (pdu_set_qos_params_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)201, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdu_set_qos_params.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_level_qos_params_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != qos_flow_level_qos_params_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 87: {
        qos_monitoring_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(qos_monitoring_request.unpack(bref));
        break;
      }
      case 126: {
        mcg_offered_gbr_qos_flow_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mcg_offered_gbr_qos_flow_info.unpack(bref));
        break;
      }
      case 132: {
        qos_monitoring_report_freq_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(qos_monitoring_report_freq, bref, (uint16_t)1u, (uint16_t)1800u, true, true));
        break;
      }
      case 133: {
        qos_monitoring_disabled_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(qos_monitoring_disabled.unpack(bref));
        break;
      }
      case 142: {
        data_forwarding_source_ip_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(data_forwarding_source_ip_address.unpack(bref));
        break;
      }
      case 201: {
        pdu_set_qos_params_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdu_set_qos_params.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void qos_flow_level_qos_params_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (qos_monitoring_request_present) {
    j.start_obj();
    j.write_int("id", 87);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", qos_monitoring_request.to_string());
    j.end_obj();
  }
  if (mcg_offered_gbr_qos_flow_info_present) {
    j.start_obj();
    j.write_int("id", 126);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mcg_offered_gbr_qos_flow_info.to_json(j);
    j.end_obj();
  }
  if (qos_monitoring_report_freq_present) {
    j.start_obj();
    j.write_int("id", 132);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", qos_monitoring_report_freq);
    j.end_obj();
  }
  if (qos_monitoring_disabled_present) {
    j.start_obj();
    j.write_int("id", 133);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  if (data_forwarding_source_ip_address_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    data_forwarding_source_ip_address.to_json(j);
    j.end_obj();
  }
  if (pdu_set_qos_params_present) {
    j.start_obj();
    j.write_int("id", 201);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    pdu_set_qos_params.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// QoSFlowLevelQoSParameters ::= SEQUENCE
OCUDUASN_CODE qos_flow_level_qos_params_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(gbr_qos_flow_info_present, 1));
  HANDLE_CODE(bref.pack(reflective_qos_attribute_present, 1));
  HANDLE_CODE(bref.pack(add_qos_info_present, 1));
  HANDLE_CODE(bref.pack(paging_policy_idx_present, 1));
  HANDLE_CODE(bref.pack(reflective_qos_ind_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(qos_characteristics.pack(bref));
  HANDLE_CODE(ngran_alloc_retention_prio.pack(bref));
  if (gbr_qos_flow_info_present) {
    HANDLE_CODE(gbr_qos_flow_info.pack(bref));
  }
  if (reflective_qos_attribute_present) {
    HANDLE_CODE(reflective_qos_attribute.pack(bref));
  }
  if (add_qos_info_present) {
    HANDLE_CODE(add_qos_info.pack(bref));
  }
  if (paging_policy_idx_present) {
    HANDLE_CODE(pack_integer(bref, paging_policy_idx, (uint8_t)1u, (uint8_t)8u, true, true));
  }
  if (reflective_qos_ind_present) {
    HANDLE_CODE(reflective_qos_ind.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_level_qos_params_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(gbr_qos_flow_info_present, 1));
  HANDLE_CODE(bref.unpack(reflective_qos_attribute_present, 1));
  HANDLE_CODE(bref.unpack(add_qos_info_present, 1));
  HANDLE_CODE(bref.unpack(paging_policy_idx_present, 1));
  HANDLE_CODE(bref.unpack(reflective_qos_ind_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(qos_characteristics.unpack(bref));
  HANDLE_CODE(ngran_alloc_retention_prio.unpack(bref));
  if (gbr_qos_flow_info_present) {
    HANDLE_CODE(gbr_qos_flow_info.unpack(bref));
  }
  if (reflective_qos_attribute_present) {
    HANDLE_CODE(reflective_qos_attribute.unpack(bref));
  }
  if (add_qos_info_present) {
    HANDLE_CODE(add_qos_info.unpack(bref));
  }
  if (paging_policy_idx_present) {
    HANDLE_CODE(unpack_integer(paging_policy_idx, bref, (uint8_t)1u, (uint8_t)8u, true, true));
  }
  if (reflective_qos_ind_present) {
    HANDLE_CODE(reflective_qos_ind.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void qos_flow_level_qos_params_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("qoS-Characteristics");
  qos_characteristics.to_json(j);
  j.write_fieldname("nGRANallocationRetentionPriority");
  ngran_alloc_retention_prio.to_json(j);
  if (gbr_qos_flow_info_present) {
    j.write_fieldname("gBR-QoS-Flow-Information");
    gbr_qos_flow_info.to_json(j);
  }
  if (reflective_qos_attribute_present) {
    j.write_str("reflective-QoS-Attribute", "subject-to");
  }
  if (add_qos_info_present) {
    j.write_str("additional-QoS-Information", "more-likely");
  }
  if (paging_policy_idx_present) {
    j.write_int("paging-Policy-Index", paging_policy_idx);
  }
  if (reflective_qos_ind_present) {
    j.write_str("reflective-QoS-Indicator", "enabled");
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* qos_flow_level_qos_params_s::reflective_qos_attribute_opts::to_string() const
{
  static const char* names[] = {"subject-to"};
  return convert_enum_idx(names, 1, value, "qos_flow_level_qos_params_s::reflective_qos_attribute_e_");
}

const char* qos_flow_level_qos_params_s::add_qos_info_opts::to_string() const
{
  static const char* names[] = {"more-likely"};
  return convert_enum_idx(names, 1, value, "qos_flow_level_qos_params_s::add_qos_info_e_");
}

const char* qos_flow_level_qos_params_s::reflective_qos_ind_opts::to_string() const
{
  static const char* names[] = {"enabled"};
  return convert_enum_idx(names, 1, value, "qos_flow_level_qos_params_s::reflective_qos_ind_e_");
}

// QoS-Flow-Mapping-Indication ::= ENUMERATED
const char* qos_flow_map_ind_opts::to_string() const
{
  static const char* names[] = {"ul", "dl"};
  return convert_enum_idx(names, 2, value, "qos_flow_map_ind_e");
}

// RedundantQoSFlowIndicator ::= ENUMERATED
const char* redundant_qos_flow_ind_opts::to_string() const
{
  static const char* names[] = {"true", "false"};
  return convert_enum_idx(names, 2, value, "redundant_qos_flow_ind_e");
}

// N6JitterInformation ::= SEQUENCE
OCUDUASN_CODE n6_jitter_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, n6_jitter_lower_bound, (int16_t)-127, (int16_t)127, false, true));
  HANDLE_CODE(pack_integer(bref, n6_jitter_upper_bound, (int16_t)-127, (int16_t)127, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE n6_jitter_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(n6_jitter_lower_bound, bref, (int16_t)-127, (int16_t)127, false, true));
  HANDLE_CODE(unpack_integer(n6_jitter_upper_bound, bref, (int16_t)-127, (int16_t)127, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void n6_jitter_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("n6JitterLowerBound", n6_jitter_lower_bound);
  j.write_int("n6JitterUpperBound", n6_jitter_upper_bound);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TSCAssistanceInformation-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t tsc_assist_info_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {152, 202};
  return map_enum_number(names, 2, idx, "id");
}
bool tsc_assist_info_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {152, 202};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e tsc_assist_info_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 152:
      return crit_e::ignore;
    case 202:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e tsc_assist_info_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 152:
      return presence_e::optional;
    case 202:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE tsc_assist_info_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += survival_time_present ? 1 : 0;
  nof_ies += n6_jitter_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (survival_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)152, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, survival_time, (uint32_t)0u, (uint32_t)1920000u, true, true));
  }
  if (n6_jitter_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)202, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(n6_jitter_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tsc_assist_info_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != tsc_assist_info_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 152: {
        survival_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(survival_time, bref, (uint32_t)0u, (uint32_t)1920000u, true, true));
        break;
      }
      case 202: {
        n6_jitter_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(n6_jitter_info.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void tsc_assist_info_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (survival_time_present) {
    j.start_obj();
    j.write_int("id", 152);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", survival_time);
    j.end_obj();
  }
  if (n6_jitter_info_present) {
    j.start_obj();
    j.write_int("id", 202);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    n6_jitter_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TSCAssistanceInformation ::= SEQUENCE
OCUDUASN_CODE tsc_assist_info_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(burst_arrival_time.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, periodicity, (uint32_t)0u, (uint32_t)640000u, true, true));
  if (burst_arrival_time.size() > 0) {
    HANDLE_CODE(burst_arrival_time.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tsc_assist_info_s::unpack(cbit_ref& bref)
{
  bool burst_arrival_time_present;
  HANDLE_CODE(bref.unpack(burst_arrival_time_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(periodicity, bref, (uint32_t)0u, (uint32_t)640000u, true, true));
  if (burst_arrival_time_present) {
    HANDLE_CODE(burst_arrival_time.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void tsc_assist_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("periodicity", periodicity);
  if (burst_arrival_time.size() > 0) {
    j.write_str("burstArrivalTime", burst_arrival_time.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TSCTrafficCharacteristics ::= SEQUENCE
OCUDUASN_CODE tsc_traffic_characteristics_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(tsc_traffic_characteristics_ul_present, 1));
  HANDLE_CODE(bref.pack(tsc_traffic_characteristics_dl_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (tsc_traffic_characteristics_ul_present) {
    HANDLE_CODE(tsc_traffic_characteristics_ul.pack(bref));
  }
  if (tsc_traffic_characteristics_dl_present) {
    HANDLE_CODE(tsc_traffic_characteristics_dl.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tsc_traffic_characteristics_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(tsc_traffic_characteristics_ul_present, 1));
  HANDLE_CODE(bref.unpack(tsc_traffic_characteristics_dl_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (tsc_traffic_characteristics_ul_present) {
    HANDLE_CODE(tsc_traffic_characteristics_ul.unpack(bref));
  }
  if (tsc_traffic_characteristics_dl_present) {
    HANDLE_CODE(tsc_traffic_characteristics_dl.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void tsc_traffic_characteristics_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (tsc_traffic_characteristics_ul_present) {
    j.write_fieldname("tSCTrafficCharacteristicsUL");
    tsc_traffic_characteristics_ul.to_json(j);
  }
  if (tsc_traffic_characteristics_dl_present) {
    j.write_fieldname("tSCTrafficCharacteristicsDL");
    tsc_traffic_characteristics_dl.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ECNMarkingorCongestionInformationReportingRequest ::= CHOICE
void ecn_markingor_congestion_info_report_request_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void ecn_markingor_congestion_info_report_request_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ecn_markingat_ngran:
      break;
    case types::ecn_markingat_up_f:
      break;
    case types::congestion_info:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ecn_markingor_congestion_info_report_request_c");
  }
}
ecn_markingor_congestion_info_report_request_c::ecn_markingor_congestion_info_report_request_c(
    const ecn_markingor_congestion_info_report_request_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ecn_markingat_ngran:
      c.init(other.c.get<ecn_markingat_ngran_e_>());
      break;
    case types::ecn_markingat_up_f:
      c.init(other.c.get<ecn_markingat_up_f_e_>());
      break;
    case types::congestion_info:
      c.init(other.c.get<congestion_info_e_>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ecn_markingor_congestion_info_report_request_c");
  }
}
ecn_markingor_congestion_info_report_request_c&
ecn_markingor_congestion_info_report_request_c::operator=(const ecn_markingor_congestion_info_report_request_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ecn_markingat_ngran:
      c.set(other.c.get<ecn_markingat_ngran_e_>());
      break;
    case types::ecn_markingat_up_f:
      c.set(other.c.get<ecn_markingat_up_f_e_>());
      break;
    case types::congestion_info:
      c.set(other.c.get<congestion_info_e_>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ecn_markingor_congestion_info_report_request_c");
  }

  return *this;
}
ecn_markingor_congestion_info_report_request_c::ecn_markingat_ngran_e_&
ecn_markingor_congestion_info_report_request_c::set_ecn_markingat_ngran()
{
  set(types::ecn_markingat_ngran);
  return c.get<ecn_markingat_ngran_e_>();
}
ecn_markingor_congestion_info_report_request_c::ecn_markingat_up_f_e_&
ecn_markingor_congestion_info_report_request_c::set_ecn_markingat_up_f()
{
  set(types::ecn_markingat_up_f);
  return c.get<ecn_markingat_up_f_e_>();
}
ecn_markingor_congestion_info_report_request_c::congestion_info_e_&
ecn_markingor_congestion_info_report_request_c::set_congestion_info()
{
  set(types::congestion_info);
  return c.get<congestion_info_e_>();
}
protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>&
ecn_markingor_congestion_info_report_request_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>();
}
void ecn_markingor_congestion_info_report_request_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ecn_markingat_ngran:
      j.write_str("eCNMarkingatNGRAN", c.get<ecn_markingat_ngran_e_>().to_string());
      break;
    case types::ecn_markingat_up_f:
      j.write_str("eCNMarkingatUPF", c.get<ecn_markingat_up_f_e_>().to_string());
      break;
    case types::congestion_info:
      j.write_str("congestionInformation", c.get<congestion_info_e_>().to_string());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ecn_markingor_congestion_info_report_request_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ecn_markingor_congestion_info_report_request_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ecn_markingat_ngran:
      HANDLE_CODE(c.get<ecn_markingat_ngran_e_>().pack(bref));
      break;
    case types::ecn_markingat_up_f:
      HANDLE_CODE(c.get<ecn_markingat_up_f_e_>().pack(bref));
      break;
    case types::congestion_info:
      HANDLE_CODE(c.get<congestion_info_e_>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(
          c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ecn_markingor_congestion_info_report_request_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ecn_markingor_congestion_info_report_request_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ecn_markingat_ngran:
      HANDLE_CODE(c.get<ecn_markingat_ngran_e_>().unpack(bref));
      break;
    case types::ecn_markingat_up_f:
      HANDLE_CODE(c.get<ecn_markingat_up_f_e_>().unpack(bref));
      break;
    case types::congestion_info:
      HANDLE_CODE(c.get<congestion_info_e_>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(
          c.get<protocol_ie_single_container_s<ecn_markingor_congestion_info_report_request_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ecn_markingor_congestion_info_report_request_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ecn_markingor_congestion_info_report_request_c::ecn_markingat_ngran_opts::to_string() const
{
  static const char* names[] = {"ul", "dl", "both", "stop"};
  return convert_enum_idx(names, 4, value, "ecn_markingor_congestion_info_report_request_c::ecn_markingat_ngran_e_");
}

const char* ecn_markingor_congestion_info_report_request_c::ecn_markingat_up_f_opts::to_string() const
{
  static const char* names[] = {"ul", "dl", "both", "stop"};
  return convert_enum_idx(names, 4, value, "ecn_markingor_congestion_info_report_request_c::ecn_markingat_up_f_e_");
}

const char* ecn_markingor_congestion_info_report_request_c::congestion_info_opts::to_string() const
{
  static const char* names[] = {"ul", "dl", "both", "stop"};
  return convert_enum_idx(names, 4, value, "ecn_markingor_congestion_info_report_request_c::congestion_info_e_");
}

const char* ecn_markingor_congestion_info_report_request_c::types_opts::to_string() const
{
  static const char* names[] = {"eCNMarkingatNGRAN", "eCNMarkingatUPF", "congestionInformation", "choice-extension"};
  return convert_enum_idx(names, 4, value, "ecn_markingor_congestion_info_report_request_c::types");
}

// QoS-Flow-QoS-Parameter-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t qos_flow_qos_param_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {99, 100, 203};
  return map_enum_number(names, 3, idx, "id");
}
bool qos_flow_qos_param_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {99, 100, 203};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e qos_flow_qos_param_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 99:
      return crit_e::ignore;
    case 100:
      return crit_e::ignore;
    case 203:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e qos_flow_qos_param_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 99:
      return presence_e::optional;
    case 100:
      return presence_e::optional;
    case 203:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE qos_flow_qos_param_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += redundant_qos_flow_ind_present ? 1 : 0;
  nof_ies += tsc_traffic_characteristics_present ? 1 : 0;
  nof_ies += ecn_markingor_congestion_info_report_request_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (redundant_qos_flow_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)99, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_qos_flow_ind.pack(bref));
  }
  if (tsc_traffic_characteristics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)100, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(tsc_traffic_characteristics.pack(bref));
  }
  if (ecn_markingor_congestion_info_report_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)203, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ecn_markingor_congestion_info_report_request.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_qos_param_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != qos_flow_qos_param_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 99: {
        redundant_qos_flow_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_qos_flow_ind.unpack(bref));
        break;
      }
      case 100: {
        tsc_traffic_characteristics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(tsc_traffic_characteristics.unpack(bref));
        break;
      }
      case 203: {
        ecn_markingor_congestion_info_report_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ecn_markingor_congestion_info_report_request.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void qos_flow_qos_param_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (redundant_qos_flow_ind_present) {
    j.start_obj();
    j.write_int("id", 99);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", redundant_qos_flow_ind.to_string());
    j.end_obj();
  }
  if (tsc_traffic_characteristics_present) {
    j.start_obj();
    j.write_int("id", 100);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    tsc_traffic_characteristics.to_json(j);
    j.end_obj();
  }
  if (ecn_markingor_congestion_info_report_request_present) {
    j.start_obj();
    j.write_int("id", 203);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    ecn_markingor_congestion_info_report_request.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// QoS-Flow-QoS-Parameter-Item ::= SEQUENCE
OCUDUASN_CODE qos_flow_qos_param_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qos_flow_map_ind_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(qos_flow_level_qos_params.pack(bref));
  if (qos_flow_map_ind_present) {
    HANDLE_CODE(qos_flow_map_ind.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_qos_param_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(qos_flow_map_ind_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(qos_flow_level_qos_params.unpack(bref));
  if (qos_flow_map_ind_present) {
    HANDLE_CODE(qos_flow_map_ind.unpack(bref));
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
void qos_flow_qos_param_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoS-Flow-Identifier", qos_flow_id);
  j.write_fieldname("qoSFlowLevelQoSParameters");
  qos_flow_level_qos_params.to_json(j);
  if (qos_flow_map_ind_present) {
    j.write_str("qoSFlowMappingIndication", qos_flow_map_ind.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// F1U-TNL-InfoToAdd-Item ::= SEQUENCE
OCUDUASN_CODE f1_u_tnl_info_to_add_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_u_tnl_info_to_add_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void f1_u_tnl_info_to_add_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("broadcastF1U-ContextReferenceE1", broadcast_f1_u_context_ref_e1.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCMRBSetupConfiguration-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t bcmrb_setup_cfg_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {208};
  return map_enum_number(names, 1, idx, "id");
}
bool bcmrb_setup_cfg_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 208 == id;
}
crit_e bcmrb_setup_cfg_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 208) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e bcmrb_setup_cfg_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 208) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE bcmrb_setup_cfg_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += f1_u_tnl_info_to_add_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (f1_u_tnl_info_to_add_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)208, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, f1_u_tnl_info_to_add_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_setup_cfg_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != bcmrb_setup_cfg_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 208) {
      {
        f1_u_tnl_info_to_add_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(f1_u_tnl_info_to_add_list, bref, 1, 512, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void bcmrb_setup_cfg_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (f1_u_tnl_info_to_add_list_present) {
    j.start_obj();
    j.write_int("id", 208);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : f1_u_tnl_info_to_add_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// BCMRBSetupConfiguration-Item ::= SEQUENCE
OCUDUASN_CODE bcmrb_setup_cfg_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qos_flow_level_qos_params_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(mbs_pdcp_cfg.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, qos_flow_qos_param_list, 1, 64, true));
  if (qos_flow_level_qos_params_present) {
    HANDLE_CODE(qos_flow_level_qos_params.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_setup_cfg_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(qos_flow_level_qos_params_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(mbs_pdcp_cfg.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(qos_flow_qos_param_list, bref, 1, 64, true));
  if (qos_flow_level_qos_params_present) {
    HANDLE_CODE(qos_flow_level_qos_params.unpack(bref));
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
void bcmrb_setup_cfg_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  j.write_fieldname("mbs-pdcp-config");
  mbs_pdcp_cfg.to_json(j);
  j.start_array("qoS-Flow-QoS-Parameter-List");
  for (const auto& e1 : qos_flow_qos_param_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (qos_flow_level_qos_params_present) {
    j.write_fieldname("qoSFlowLevelQoSParameters");
    qos_flow_level_qos_params.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// F1U-TNL-InfoToAddOrModify-Item ::= SEQUENCE
OCUDUASN_CODE f1_u_tnl_info_to_add_or_modify_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bc_bearer_context_f1_u_tnl_infoat_du_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.pack(bref));
  if (bc_bearer_context_f1_u_tnl_infoat_du_present) {
    HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_du.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_u_tnl_info_to_add_or_modify_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(bc_bearer_context_f1_u_tnl_infoat_du_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.unpack(bref));
  if (bc_bearer_context_f1_u_tnl_infoat_du_present) {
    HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_du.unpack(bref));
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
void f1_u_tnl_info_to_add_or_modify_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("broadcastF1U-ContextReferenceE1", broadcast_f1_u_context_ref_e1.to_string());
  if (bc_bearer_context_f1_u_tnl_infoat_du_present) {
    j.write_fieldname("bcBearerContextF1U-TNLInfoatDU");
    bc_bearer_context_f1_u_tnl_infoat_du.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// F1U-TNL-InfoToRelease-Item ::= SEQUENCE
OCUDUASN_CODE f1_u_tnl_info_to_release_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_u_tnl_info_to_release_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void f1_u_tnl_info_to_release_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("broadcastF1U-ContextReferenceE1", broadcast_f1_u_context_ref_e1.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCMRBModifyConfiguration-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t bcmrb_modify_cfg_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {210, 212};
  return map_enum_number(names, 2, idx, "id");
}
bool bcmrb_modify_cfg_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {210, 212};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bcmrb_modify_cfg_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 210:
      return crit_e::ignore;
    case 212:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bcmrb_modify_cfg_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 210:
      return presence_e::optional;
    case 212:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bcmrb_modify_cfg_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += f1_u_tnl_info_to_add_or_modify_list_present ? 1 : 0;
  nof_ies += f1_u_tnl_info_to_release_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (f1_u_tnl_info_to_add_or_modify_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)210, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, f1_u_tnl_info_to_add_or_modify_list, 1, 512, true));
  }
  if (f1_u_tnl_info_to_release_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)212, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, f1_u_tnl_info_to_release_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_modify_cfg_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != bcmrb_modify_cfg_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 210: {
        f1_u_tnl_info_to_add_or_modify_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(f1_u_tnl_info_to_add_or_modify_list, bref, 1, 512, true));
        break;
      }
      case 212: {
        f1_u_tnl_info_to_release_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(f1_u_tnl_info_to_release_list, bref, 1, 512, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void bcmrb_modify_cfg_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (f1_u_tnl_info_to_add_or_modify_list_present) {
    j.start_obj();
    j.write_int("id", 210);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : f1_u_tnl_info_to_add_or_modify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (f1_u_tnl_info_to_release_list_present) {
    j.start_obj();
    j.write_int("id", 212);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : f1_u_tnl_info_to_release_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// BCMRBModifyConfiguration-Item ::= SEQUENCE
OCUDUASN_CODE bcmrb_modify_cfg_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bc_bearer_context_f1_u_tnl_infoat_du_present, 1));
  HANDLE_CODE(bref.pack(mbs_pdcp_cfg_present, 1));
  HANDLE_CODE(bref.pack(qos_flow_qos_param_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(qos_flow_level_qos_params_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (bc_bearer_context_f1_u_tnl_infoat_du_present) {
    HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_du.pack(bref));
  }
  if (mbs_pdcp_cfg_present) {
    HANDLE_CODE(mbs_pdcp_cfg.pack(bref));
  }
  if (qos_flow_qos_param_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qos_flow_qos_param_list, 1, 64, true));
  }
  if (qos_flow_level_qos_params_present) {
    HANDLE_CODE(qos_flow_level_qos_params.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_modify_cfg_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(bc_bearer_context_f1_u_tnl_infoat_du_present, 1));
  HANDLE_CODE(bref.unpack(mbs_pdcp_cfg_present, 1));
  bool qos_flow_qos_param_list_present;
  HANDLE_CODE(bref.unpack(qos_flow_qos_param_list_present, 1));
  HANDLE_CODE(bref.unpack(qos_flow_level_qos_params_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (bc_bearer_context_f1_u_tnl_infoat_du_present) {
    HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_du.unpack(bref));
  }
  if (mbs_pdcp_cfg_present) {
    HANDLE_CODE(mbs_pdcp_cfg.unpack(bref));
  }
  if (qos_flow_qos_param_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qos_flow_qos_param_list, bref, 1, 64, true));
  }
  if (qos_flow_level_qos_params_present) {
    HANDLE_CODE(qos_flow_level_qos_params.unpack(bref));
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
void bcmrb_modify_cfg_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (bc_bearer_context_f1_u_tnl_infoat_du_present) {
    j.write_fieldname("bcBearerContextF1U-TNLInfoatDU");
    bc_bearer_context_f1_u_tnl_infoat_du.to_json(j);
  }
  if (mbs_pdcp_cfg_present) {
    j.write_fieldname("mbs-pdcp-config");
    mbs_pdcp_cfg.to_json(j);
  }
  if (qos_flow_qos_param_list.size() > 0) {
    j.start_array("qoS-Flow-QoS-Parameter-List");
    for (const auto& e1 : qos_flow_qos_param_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (qos_flow_level_qos_params_present) {
    j.write_fieldname("qoSFlowLevelQoSParameters");
    qos_flow_level_qos_params.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextToModify-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t bc_bearer_context_to_modify_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {217};
  return map_enum_number(names, 1, idx, "id");
}
bool bc_bearer_context_to_modify_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 217 == id;
}
crit_e bc_bearer_context_to_modify_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 217) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e bc_bearer_context_to_modify_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 217) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE bc_bearer_context_to_modify_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += bc_bearer_context_ngu_tnl_infoat_ngran_request_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (bc_bearer_context_ngu_tnl_infoat_ngran_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)217, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat_ngran_request.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_to_modify_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != bc_bearer_context_to_modify_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 217) {
      {
        bc_bearer_context_ngu_tnl_infoat_ngran_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat_ngran_request.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void bc_bearer_context_to_modify_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (bc_bearer_context_ngu_tnl_infoat_ngran_request_present) {
    j.start_obj();
    j.write_int("id", 217);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    bc_bearer_context_ngu_tnl_infoat_ngran_request.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BCBearerContextToModify ::= SEQUENCE
OCUDUASN_CODE bc_bearer_context_to_modify_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bc_bearer_context_ngu_tnl_infoat5_gc_present, 1));
  HANDLE_CODE(bref.pack(bc_mrb_to_setup_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(bc_mrb_to_modify_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(bc_mrb_to_rem_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (bc_bearer_context_ngu_tnl_infoat5_gc_present) {
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat5_gc.pack(bref));
  }
  if (bc_mrb_to_setup_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_to_setup_list, 1, 32, true));
  }
  if (bc_mrb_to_modify_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_to_modify_list, 1, 32, true));
  }
  if (bc_mrb_to_rem_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_to_rem_list, 1, 32, integer_packer<uint16_t>(1, 512, true, true), true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_to_modify_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(bc_bearer_context_ngu_tnl_infoat5_gc_present, 1));
  bool bc_mrb_to_setup_list_present;
  HANDLE_CODE(bref.unpack(bc_mrb_to_setup_list_present, 1));
  bool bc_mrb_to_modify_list_present;
  HANDLE_CODE(bref.unpack(bc_mrb_to_modify_list_present, 1));
  bool bc_mrb_to_rem_list_present;
  HANDLE_CODE(bref.unpack(bc_mrb_to_rem_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (bc_bearer_context_ngu_tnl_infoat5_gc_present) {
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat5_gc.unpack(bref));
  }
  if (bc_mrb_to_setup_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_to_setup_list, bref, 1, 32, true));
  }
  if (bc_mrb_to_modify_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_to_modify_list, bref, 1, 32, true));
  }
  if (bc_mrb_to_rem_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_to_rem_list, bref, 1, 32, integer_packer<uint16_t>(1, 512, true, true), true));
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
void bc_bearer_context_to_modify_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (bc_bearer_context_ngu_tnl_infoat5_gc_present) {
    j.write_fieldname("bcBearerContextNGU-TNLInfoat5GC");
    bc_bearer_context_ngu_tnl_infoat5_gc.to_json(j);
  }
  if (bc_mrb_to_setup_list.size() > 0) {
    j.start_array("bcMRBToSetupList");
    for (const auto& e1 : bc_mrb_to_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (bc_mrb_to_modify_list.size() > 0) {
    j.start_array("bcMRBToModifyList");
    for (const auto& e1 : bc_mrb_to_modify_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (bc_mrb_to_rem_list.size() > 0) {
    j.start_array("bcMRBToRemoveList");
    for (const auto& e1 : bc_mrb_to_rem_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextToModifyConfirm ::= SEQUENCE
OCUDUASN_CODE bc_bearer_context_to_modify_confirm_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_to_modify_confirm_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void bc_bearer_context_to_modify_confirm_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextToModifyRequired ::= SEQUENCE
OCUDUASN_CODE bc_bearer_context_to_modify_required_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bc_mrb_to_rem_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (bc_mrb_to_rem_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_to_rem_list, 1, 32, integer_packer<uint16_t>(1, 512, true, true), true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_to_modify_required_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool bc_mrb_to_rem_list_present;
  HANDLE_CODE(bref.unpack(bc_mrb_to_rem_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (bc_mrb_to_rem_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_to_rem_list, bref, 1, 32, integer_packer<uint16_t>(1, 512, true, true), true));
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
void bc_bearer_context_to_modify_required_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (bc_mrb_to_rem_list.size() > 0) {
    j.start_array("bcMRBToRemoveList");
    for (const auto& e1 : bc_mrb_to_rem_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ECNMarkingorCongestionInformationReportingStatus ::= ENUMERATED
const char* ecn_markingor_congestion_info_report_status_opts::to_string() const
{
  static const char* names[] = {"active", "not-active"};
  return convert_enum_idx(names, 2, value, "ecn_markingor_congestion_info_report_status_e");
}

// QoS-Flow-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t qos_flow_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {80, 142, 204};
  return map_enum_number(names, 3, idx, "id");
}
bool qos_flow_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {80, 142, 204};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e qos_flow_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 80:
      return crit_e::ignore;
    case 142:
      return crit_e::ignore;
    case 204:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e qos_flow_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 80:
      return presence_e::optional;
    case 142:
      return presence_e::optional;
    case 204:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE qos_flow_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += qos_flow_map_ind_present ? 1 : 0;
  nof_ies += data_forwarding_source_ip_address_present ? 1 : 0;
  nof_ies += ecn_markingor_congestion_info_report_status_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (qos_flow_map_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)80, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(qos_flow_map_ind.pack(bref));
  }
  if (data_forwarding_source_ip_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)142, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(data_forwarding_source_ip_address.pack(bref));
  }
  if (ecn_markingor_congestion_info_report_status_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)204, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ecn_markingor_congestion_info_report_status.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != qos_flow_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 80: {
        qos_flow_map_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(qos_flow_map_ind.unpack(bref));
        break;
      }
      case 142: {
        data_forwarding_source_ip_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(data_forwarding_source_ip_address.unpack(bref));
        break;
      }
      case 204: {
        ecn_markingor_congestion_info_report_status_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ecn_markingor_congestion_info_report_status.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void qos_flow_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (qos_flow_map_ind_present) {
    j.start_obj();
    j.write_int("id", 80);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", qos_flow_map_ind.to_string());
    j.end_obj();
  }
  if (data_forwarding_source_ip_address_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    data_forwarding_source_ip_address.to_json(j);
    j.end_obj();
  }
  if (ecn_markingor_congestion_info_report_status_present) {
    j.start_obj();
    j.write_int("id", 204);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", ecn_markingor_congestion_info_report_status.to_string());
    j.end_obj();
  }
  j.end_array();
}

// QoS-Flow-Item ::= SEQUENCE
OCUDUASN_CODE qos_flow_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void qos_flow_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoS-Flow-Identifier", qos_flow_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CauseRadioNetwork ::= ENUMERATED
const char* cause_radio_network_opts::to_string() const
{
  static const char* names[] = {"unspecified",
                                "unknown-or-already-allocated-gnb-cu-cp-ue-e1ap-id",
                                "unknown-or-already-allocated-gnb-cu-up-ue-e1ap-id",
                                "unknown-or-inconsistent-pair-of-ue-e1ap-id",
                                "interaction-with-other-procedure",
                                "pPDCP-Count-wrap-around",
                                "not-supported-QCI-value",
                                "not-supported-5QI-value",
                                "encryption-algorithms-not-supported",
                                "integrity-protection-algorithms-not-supported",
                                "uP-integrity-protection-not-possible",
                                "uP-confidentiality-protection-not-possible",
                                "multiple-PDU-Session-ID-Instances",
                                "unknown-PDU-Session-ID",
                                "multiple-QoS-Flow-ID-Instances",
                                "unknown-QoS-Flow-ID",
                                "multiple-DRB-ID-Instances",
                                "unknown-DRB-ID",
                                "invalid-QoS-combination",
                                "procedure-cancelled",
                                "normal-release",
                                "no-radio-resources-available",
                                "action-desirable-for-radio-reasons",
                                "resources-not-available-for-the-slice",
                                "pDCP-configuration-not-supported",
                                "ue-dl-max-IP-data-rate-reason",
                                "uP-integrity-protection-failure",
                                "release-due-to-pre-emption",
                                "rsn-not-available-for-the-up",
                                "nPN-not-supported",
                                "report-characteristic-empty",
                                "existing-measurement-ID",
                                "measurement-temporarily-not-available",
                                "measurement-not-supported-for-the-object",
                                "scg-activation-deactivation-failure",
                                "scg-deactivation-failure-due-to-data-transmission",
                                "unknown-or-already-allocated-gNB-CU-CP-MBS-E1AP-ID",
                                "unknown-or-already-allocated-gNB-CU-UP-MBS-E1AP-ID",
                                "unknown-or-inconsistent-pair-of-MBS-E1AP-ID",
                                "unknown-or-inconsistent-MRB-ID"};
  return convert_enum_idx(names, 40, value, "cause_radio_network_e");
}

// CauseTransport ::= ENUMERATED
const char* cause_transport_opts::to_string() const
{
  static const char* names[] = {"unspecified", "transport-resource-unavailable", "unknown-TNL-address-for-IAB"};
  return convert_enum_idx(names, 3, value, "cause_transport_e");
}

// CauseProtocol ::= ENUMERATED
const char* cause_protocol_opts::to_string() const
{
  static const char* names[] = {"transfer-syntax-error",
                                "abstract-syntax-error-reject",
                                "abstract-syntax-error-ignore-and-notify",
                                "message-not-compatible-with-receiver-state",
                                "semantic-error",
                                "abstract-syntax-error-falsely-constructed-message",
                                "unspecified"};
  return convert_enum_idx(names, 7, value, "cause_protocol_e");
}

// CauseMisc ::= ENUMERATED
const char* cause_misc_opts::to_string() const
{
  static const char* names[] = {"control-processing-overload",
                                "not-enough-user-plane-processing-resources",
                                "hardware-failure",
                                "om-intervention",
                                "unspecified"};
  return convert_enum_idx(names, 5, value, "cause_misc_e");
}

// Cause ::= CHOICE
void cause_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<cause_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void cause_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::radio_network:
      break;
    case types::transport:
      break;
    case types::protocol:
      break;
    case types::misc:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<cause_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }
}
cause_c::cause_c(const cause_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::radio_network:
      c.init(other.c.get<cause_radio_network_e>());
      break;
    case types::transport:
      c.init(other.c.get<cause_transport_e>());
      break;
    case types::protocol:
      c.init(other.c.get<cause_protocol_e>());
      break;
    case types::misc:
      c.init(other.c.get<cause_misc_e>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<cause_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }
}
cause_c& cause_c::operator=(const cause_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::radio_network:
      c.set(other.c.get<cause_radio_network_e>());
      break;
    case types::transport:
      c.set(other.c.get<cause_transport_e>());
      break;
    case types::protocol:
      c.set(other.c.get<cause_protocol_e>());
      break;
    case types::misc:
      c.set(other.c.get<cause_misc_e>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<cause_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }

  return *this;
}
cause_radio_network_e& cause_c::set_radio_network()
{
  set(types::radio_network);
  return c.get<cause_radio_network_e>();
}
cause_transport_e& cause_c::set_transport()
{
  set(types::transport);
  return c.get<cause_transport_e>();
}
cause_protocol_e& cause_c::set_protocol()
{
  set(types::protocol);
  return c.get<cause_protocol_e>();
}
cause_misc_e& cause_c::set_misc()
{
  set(types::misc);
  return c.get<cause_misc_e>();
}
protocol_ie_single_container_s<cause_ext_ies_o>& cause_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<cause_ext_ies_o>>();
}
void cause_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::radio_network:
      j.write_str("radioNetwork", c.get<cause_radio_network_e>().to_string());
      break;
    case types::transport:
      j.write_str("transport", c.get<cause_transport_e>().to_string());
      break;
    case types::protocol:
      j.write_str("protocol", c.get<cause_protocol_e>().to_string());
      break;
    case types::misc:
      j.write_str("misc", c.get<cause_misc_e>().to_string());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<cause_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }
  j.end_obj();
}
OCUDUASN_CODE cause_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::radio_network:
      HANDLE_CODE(c.get<cause_radio_network_e>().pack(bref));
      break;
    case types::transport:
      HANDLE_CODE(c.get<cause_transport_e>().pack(bref));
      break;
    case types::protocol:
      HANDLE_CODE(c.get<cause_protocol_e>().pack(bref));
      break;
    case types::misc:
      HANDLE_CODE(c.get<cause_misc_e>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<cause_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cause_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::radio_network:
      HANDLE_CODE(c.get<cause_radio_network_e>().unpack(bref));
      break;
    case types::transport:
      HANDLE_CODE(c.get<cause_transport_e>().unpack(bref));
      break;
    case types::protocol:
      HANDLE_CODE(c.get<cause_protocol_e>().unpack(bref));
      break;
    case types::misc:
      HANDLE_CODE(c.get<cause_misc_e>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<cause_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cause_c::types_opts::to_string() const
{
  static const char* names[] = {"radioNetwork", "transport", "protocol", "misc", "choice-extension"};
  return convert_enum_idx(names, 5, value, "cause_c::types");
}
uint8_t cause_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "cause_c::types");
}

// QoS-Flow-Failed-Item ::= SEQUENCE
OCUDUASN_CODE qos_flow_failed_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_failed_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void qos_flow_failed_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoS-Flow-Identifier", qos_flow_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// F1U-TNL-InfoAddedOrModified-Item ::= SEQUENCE
OCUDUASN_CODE f1_u_tnl_info_added_or_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bc_bearer_context_f1_u_tnl_infoat_cu_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.pack(bref));
  if (bc_bearer_context_f1_u_tnl_infoat_cu_present) {
    HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_cu.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_u_tnl_info_added_or_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(bc_bearer_context_f1_u_tnl_infoat_cu_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.unpack(bref));
  if (bc_bearer_context_f1_u_tnl_infoat_cu_present) {
    HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_cu.unpack(bref));
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
void f1_u_tnl_info_added_or_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("broadcastF1U-ContextReferenceE1", broadcast_f1_u_context_ref_e1.to_string());
  if (bc_bearer_context_f1_u_tnl_infoat_cu_present) {
    j.write_fieldname("bcBearerContextF1U-TNLInfoatCU");
    bc_bearer_context_f1_u_tnl_infoat_cu.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCMRBSetupModifyResponseList-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t bcmrb_setup_modify_resp_list_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {211};
  return map_enum_number(names, 1, idx, "id");
}
bool bcmrb_setup_modify_resp_list_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 211 == id;
}
crit_e bcmrb_setup_modify_resp_list_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 211) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e bcmrb_setup_modify_resp_list_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 211) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE bcmrb_setup_modify_resp_list_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += f1_u_tnl_info_added_or_modified_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (f1_u_tnl_info_added_or_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)211, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, f1_u_tnl_info_added_or_modified_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_setup_modify_resp_list_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != bcmrb_setup_modify_resp_list_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 211) {
      {
        f1_u_tnl_info_added_or_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(f1_u_tnl_info_added_or_modified_list, bref, 1, 512, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void bcmrb_setup_modify_resp_list_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (f1_u_tnl_info_added_or_modified_list_present) {
    j.start_obj();
    j.write_int("id", 211);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : f1_u_tnl_info_added_or_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// BCMRBSetupModifyResponseList-Item ::= SEQUENCE
OCUDUASN_CODE bcmrb_setup_modify_resp_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qosflow_setup.size() > 0, 1));
  HANDLE_CODE(bref.pack(qosflow_failed.size() > 0, 1));
  HANDLE_CODE(bref.pack(bc_bearer_context_f1_u_tnl_infoat_cu_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (qosflow_setup.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qosflow_setup, 1, 64, true));
  }
  if (qosflow_failed.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qosflow_failed, 1, 64, true));
  }
  if (bc_bearer_context_f1_u_tnl_infoat_cu_present) {
    HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_cu.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_setup_modify_resp_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool qosflow_setup_present;
  HANDLE_CODE(bref.unpack(qosflow_setup_present, 1));
  bool qosflow_failed_present;
  HANDLE_CODE(bref.unpack(qosflow_failed_present, 1));
  HANDLE_CODE(bref.unpack(bc_bearer_context_f1_u_tnl_infoat_cu_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (qosflow_setup_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qosflow_setup, bref, 1, 64, true));
  }
  if (qosflow_failed_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qosflow_failed, bref, 1, 64, true));
  }
  if (bc_bearer_context_f1_u_tnl_infoat_cu_present) {
    HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_cu.unpack(bref));
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
void bcmrb_setup_modify_resp_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (qosflow_setup.size() > 0) {
    j.start_array("qosflow-setup");
    for (const auto& e1 : qosflow_setup) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (qosflow_failed.size() > 0) {
    j.start_array("qosflow-failed");
    for (const auto& e1 : qosflow_failed) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (bc_bearer_context_f1_u_tnl_infoat_cu_present) {
    j.write_fieldname("bcBearerContextF1U-TNLInfoatCU");
    bc_bearer_context_f1_u_tnl_infoat_cu.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCMRBFailedList-Item ::= SEQUENCE
OCUDUASN_CODE bcmrb_failed_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_failed_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void bcmrb_failed_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextToModifyResponse ::= SEQUENCE
OCUDUASN_CODE bc_bearer_context_to_modify_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bc_bearer_context_ngu_tnl_infoat_ngran_present, 1));
  HANDLE_CODE(bref.pack(bc_mrb_failed_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(available_bcmrb_cfg.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (bc_bearer_context_ngu_tnl_infoat_ngran_present) {
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat_ngran.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_setup_modify_resp_list, 1, 32, true));
  if (bc_mrb_failed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_failed_list, 1, 32, true));
  }
  if (available_bcmrb_cfg.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, available_bcmrb_cfg, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_to_modify_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(bc_bearer_context_ngu_tnl_infoat_ngran_present, 1));
  bool bc_mrb_failed_list_present;
  HANDLE_CODE(bref.unpack(bc_mrb_failed_list_present, 1));
  bool available_bcmrb_cfg_present;
  HANDLE_CODE(bref.unpack(available_bcmrb_cfg_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (bc_bearer_context_ngu_tnl_infoat_ngran_present) {
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat_ngran.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_setup_modify_resp_list, bref, 1, 32, true));
  if (bc_mrb_failed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_failed_list, bref, 1, 32, true));
  }
  if (available_bcmrb_cfg_present) {
    HANDLE_CODE(unpack_dyn_seq_of(available_bcmrb_cfg, bref, 1, 32, true));
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
void bc_bearer_context_to_modify_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (bc_bearer_context_ngu_tnl_infoat_ngran_present) {
    j.write_fieldname("bcBearerContextNGU-TNLInfoatNGRAN");
    bc_bearer_context_ngu_tnl_infoat_ngran.to_json(j);
  }
  j.start_array("bcMRBSetupModifyResponseList");
  for (const auto& e1 : bc_mrb_setup_modify_resp_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (bc_mrb_failed_list.size() > 0) {
    j.start_array("bcMRBFailedList");
    for (const auto& e1 : bc_mrb_failed_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (available_bcmrb_cfg.size() > 0) {
    j.start_array("availableBCMRBConfig");
    for (const auto& e1 : available_bcmrb_cfg) {
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

// SNSSAI ::= SEQUENCE
OCUDUASN_CODE snssai_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sd_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(sst.pack(bref));
  if (sd_present) {
    HANDLE_CODE(sd.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE snssai_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sd_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(sst.unpack(bref));
  if (sd_present) {
    HANDLE_CODE(sd.unpack(bref));
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
void snssai_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("sST", sst.to_string());
  if (sd_present) {
    j.write_str("sD", sd.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// RequestedAction4AvailNGUTermination ::= ENUMERATED
const char* requested_action4_avail_ngu_termination_opts::to_string() const
{
  static const char* names[] = {"apply-available-configuration",
                                "apply-requested-configuration",
                                "apply-available-configuration-if-same-as-requested"};
  return convert_enum_idx(names, 3, value, "requested_action4_avail_ngu_termination_e");
}

// BCBearerContextToSetup ::= SEQUENCE
OCUDUASN_CODE bc_bearer_context_to_setup_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bc_bearer_context_ngu_tnl_infoat5_gc_present, 1));
  HANDLE_CODE(bref.pack(requested_action_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(snssai.pack(bref));
  if (bc_bearer_context_ngu_tnl_infoat5_gc_present) {
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat5_gc.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_to_setup_list, 1, 32, true));
  if (requested_action_present) {
    HANDLE_CODE(requested_action.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_to_setup_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(bc_bearer_context_ngu_tnl_infoat5_gc_present, 1));
  HANDLE_CODE(bref.unpack(requested_action_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(snssai.unpack(bref));
  if (bc_bearer_context_ngu_tnl_infoat5_gc_present) {
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat5_gc.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_to_setup_list, bref, 1, 32, true));
  if (requested_action_present) {
    HANDLE_CODE(requested_action.unpack(bref));
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
void bc_bearer_context_to_setup_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("snssai");
  snssai.to_json(j);
  if (bc_bearer_context_ngu_tnl_infoat5_gc_present) {
    j.write_fieldname("bcBearerContextNGU-TNLInfoat5GC");
    bc_bearer_context_ngu_tnl_infoat5_gc.to_json(j);
  }
  j.start_array("bcMRBToSetupList");
  for (const auto& e1 : bc_mrb_to_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (requested_action_present) {
    j.write_str("requestedAction", requested_action.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// F1U-TNL-InfoAdded-Item ::= SEQUENCE
OCUDUASN_CODE f1_u_tnl_info_added_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.pack(bref));
  HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_cu.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_u_tnl_info_added_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(broadcast_f1_u_context_ref_e1.unpack(bref));
  HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_cu.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void f1_u_tnl_info_added_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("broadcastF1U-ContextReferenceE1", broadcast_f1_u_context_ref_e1.to_string());
  j.write_fieldname("bcBearerContextF1U-TNLInfoatCU");
  bc_bearer_context_f1_u_tnl_infoat_cu.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCMRBSetupResponseList-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t bcmrb_setup_resp_list_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {209};
  return map_enum_number(names, 1, idx, "id");
}
bool bcmrb_setup_resp_list_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 209 == id;
}
crit_e bcmrb_setup_resp_list_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 209) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e bcmrb_setup_resp_list_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 209) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE bcmrb_setup_resp_list_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += f1_u_tnl_info_added_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (f1_u_tnl_info_added_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)209, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, f1_u_tnl_info_added_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_setup_resp_list_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != bcmrb_setup_resp_list_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 209) {
      {
        f1_u_tnl_info_added_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(f1_u_tnl_info_added_list, bref, 1, 512, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void bcmrb_setup_resp_list_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (f1_u_tnl_info_added_list_present) {
    j.start_obj();
    j.write_int("id", 209);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : f1_u_tnl_info_added_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// BCMRBSetupResponseList-Item ::= SEQUENCE
OCUDUASN_CODE bcmrb_setup_resp_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qosflow_failed.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, qosflow_setup, 1, 64, true));
  if (qosflow_failed.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qosflow_failed, 1, 64, true));
  }
  HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_cu.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bcmrb_setup_resp_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool qosflow_failed_present;
  HANDLE_CODE(bref.unpack(qosflow_failed_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(unpack_dyn_seq_of(qosflow_setup, bref, 1, 64, true));
  if (qosflow_failed_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qosflow_failed, bref, 1, 64, true));
  }
  HANDLE_CODE(bc_bearer_context_f1_u_tnl_infoat_cu.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void bcmrb_setup_resp_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  j.start_array("qosflow-setup");
  for (const auto& e1 : qosflow_setup) {
    e1.to_json(j);
  }
  j.end_array();
  if (qosflow_failed.size() > 0) {
    j.start_array("qosflow-failed");
    for (const auto& e1 : qosflow_failed) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.write_fieldname("bcBearerContextF1U-TNLInfoatCU");
  bc_bearer_context_f1_u_tnl_infoat_cu.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// BCBearerContextToSetupResponse ::= SEQUENCE
OCUDUASN_CODE bc_bearer_context_to_setup_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(bc_bearer_context_ngu_tnl_infoat_ngran_present, 1));
  HANDLE_CODE(bref.pack(bc_mrb_failed_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(available_bcmrb_cfg.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (bc_bearer_context_ngu_tnl_infoat_ngran_present) {
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat_ngran.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_setup_resp_list, 1, 32, true));
  if (bc_mrb_failed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, bc_mrb_failed_list, 1, 32, true));
  }
  if (available_bcmrb_cfg.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, available_bcmrb_cfg, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bc_bearer_context_to_setup_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(bc_bearer_context_ngu_tnl_infoat_ngran_present, 1));
  bool bc_mrb_failed_list_present;
  HANDLE_CODE(bref.unpack(bc_mrb_failed_list_present, 1));
  bool available_bcmrb_cfg_present;
  HANDLE_CODE(bref.unpack(available_bcmrb_cfg_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (bc_bearer_context_ngu_tnl_infoat_ngran_present) {
    HANDLE_CODE(bc_bearer_context_ngu_tnl_infoat_ngran.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_setup_resp_list, bref, 1, 32, true));
  if (bc_mrb_failed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(bc_mrb_failed_list, bref, 1, 32, true));
  }
  if (available_bcmrb_cfg_present) {
    HANDLE_CODE(unpack_dyn_seq_of(available_bcmrb_cfg, bref, 1, 32, true));
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
void bc_bearer_context_to_setup_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (bc_bearer_context_ngu_tnl_infoat_ngran_present) {
    j.write_fieldname("bcBearerContextNGU-TNLInfoatNGRAN");
    bc_bearer_context_ngu_tnl_infoat_ngran.to_json(j);
  }
  j.start_array("bcMRBSetupResponseList");
  for (const auto& e1 : bc_mrb_setup_resp_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (bc_mrb_failed_list.size() > 0) {
    j.start_array("bcMRBFailedList");
    for (const auto& e1 : bc_mrb_failed_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (available_bcmrb_cfg.size() > 0) {
    j.start_array("availableBCMRBConfig");
    for (const auto& e1 : available_bcmrb_cfg) {
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

// BearerContextStatusChange ::= ENUMERATED
const char* bearer_context_status_change_opts::to_string() const
{
  static const char* names[] = {"suspend", "resume", "resumeforSDT"};
  return convert_enum_idx(names, 3, value, "bearer_context_status_change_e");
}

// CHOInitiation ::= ENUMERATED
const char* cho_initiation_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "cho_initiation_e");
}

// CNSupport ::= ENUMERATED
const char* cn_support_opts::to_string() const
{
  static const char* names[] = {"c-epc", "c-5gc", "both"};
  return convert_enum_idx(names, 3, value, "cn_support_e");
}
int8_t cn_support_opts::to_number() const
{
  if (value == c_5gc) {
    return -5;
  }
  invalid_enum_number(value, "cn_support_e");
  return 0;
}

// Endpoint-IP-address-and-port ::= SEQUENCE
OCUDUASN_CODE endpoint_ip_address_and_port_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(endpoint_ip_address.pack(bref));
  HANDLE_CODE(port_num.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE endpoint_ip_address_and_port_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(endpoint_ip_address.unpack(bref));
  HANDLE_CODE(port_num.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void endpoint_ip_address_and_port_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("endpoint-IP-Address");
  endpoint_ip_address.to_json(j);
  j.write_fieldname("portNumber");
  port_num.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CP-TNL-Information-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-IES
uint32_t cp_tnl_info_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {74};
  return map_enum_number(names, 1, idx, "id");
}
bool cp_tnl_info_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 74 == id;
}
crit_e cp_tnl_info_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 74) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
cp_tnl_info_ext_ies_o::value_c cp_tnl_info_ext_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 74) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e cp_tnl_info_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 74) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void cp_tnl_info_ext_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE cp_tnl_info_ext_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cp_tnl_info_ext_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* cp_tnl_info_ext_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"Endpoint-IP-address-and-port"};
  return convert_enum_idx(names, 1, value, "cp_tnl_info_ext_ies_o::value_c::types");
}

// CP-TNL-Information ::= CHOICE
void cp_tnl_info_c::destroy_()
{
  switch (type_) {
    case types::endpoint_ip_address:
      c.destroy<bounded_bitstring<1, 160, true, true>>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void cp_tnl_info_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::endpoint_ip_address:
      c.init<bounded_bitstring<1, 160, true, true>>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cp_tnl_info_c");
  }
}
cp_tnl_info_c::cp_tnl_info_c(const cp_tnl_info_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::endpoint_ip_address:
      c.init(other.c.get<bounded_bitstring<1, 160, true, true>>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cp_tnl_info_c");
  }
}
cp_tnl_info_c& cp_tnl_info_c::operator=(const cp_tnl_info_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::endpoint_ip_address:
      c.set(other.c.get<bounded_bitstring<1, 160, true, true>>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cp_tnl_info_c");
  }

  return *this;
}
bounded_bitstring<1, 160, true, true>& cp_tnl_info_c::set_endpoint_ip_address()
{
  set(types::endpoint_ip_address);
  return c.get<bounded_bitstring<1, 160, true, true>>();
}
protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>& cp_tnl_info_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>>();
}
void cp_tnl_info_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::endpoint_ip_address:
      j.write_fieldname("endpoint-IP-Address");
      c.get<bounded_bitstring<1, 160, true, true>>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "cp_tnl_info_c");
  }
  j.end_obj();
}
OCUDUASN_CODE cp_tnl_info_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::endpoint_ip_address:
      HANDLE_CODE((c.get<bounded_bitstring<1, 160, true, true>>().pack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "cp_tnl_info_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cp_tnl_info_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::endpoint_ip_address:
      HANDLE_CODE((c.get<bounded_bitstring<1, 160, true, true>>().unpack(bref)));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<cp_tnl_info_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "cp_tnl_info_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cp_tnl_info_c::types_opts::to_string() const
{
  static const char* names[] = {"endpoint-IP-Address", "choice-extension"};
  return convert_enum_idx(names, 2, value, "cp_tnl_info_c::types");
}

// UL-Configuration ::= ENUMERATED
const char* ul_cfg_opts::to_string() const
{
  static const char* names[] = {"no-data", "shared", "only"};
  return convert_enum_idx(names, 3, value, "ul_cfg_e");
}

// DL-TX-Stop ::= ENUMERATED
const char* dl_tx_stop_opts::to_string() const
{
  static const char* names[] = {"stop", "resume"};
  return convert_enum_idx(names, 2, value, "dl_tx_stop_e");
}

// RAT-Type ::= ENUMERATED
const char* rat_type_opts::to_string() const
{
  static const char* names[] = {"e-UTRA", "nR"};
  return convert_enum_idx(names, 2, value, "rat_type_e");
}

// Cell-Group-Information-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t cell_group_info_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {127};
  return map_enum_number(names, 1, idx, "id");
}
bool cell_group_info_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 127 == id;
}
crit_e cell_group_info_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 127) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e cell_group_info_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 127) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE cell_group_info_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += nof_tunnels_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (nof_tunnels_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)127, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, nof_tunnels, (uint8_t)1u, (uint8_t)4u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_group_info_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != cell_group_info_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 127) {
      {
        nof_tunnels_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(nof_tunnels, bref, (uint8_t)1u, (uint8_t)4u, true, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void cell_group_info_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (nof_tunnels_present) {
    j.start_obj();
    j.write_int("id", 127);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", nof_tunnels);
    j.end_obj();
  }
  j.end_array();
}

// Cell-Group-Information-Item ::= SEQUENCE
OCUDUASN_CODE cell_group_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ul_cfg_present, 1));
  HANDLE_CODE(bref.pack(dl_tx_stop_present, 1));
  HANDLE_CODE(bref.pack(rat_type_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, cell_group_id, (uint8_t)0u, (uint8_t)3u, true, true));
  if (ul_cfg_present) {
    HANDLE_CODE(ul_cfg.pack(bref));
  }
  if (dl_tx_stop_present) {
    HANDLE_CODE(dl_tx_stop.pack(bref));
  }
  if (rat_type_present) {
    HANDLE_CODE(rat_type.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_group_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ul_cfg_present, 1));
  HANDLE_CODE(bref.unpack(dl_tx_stop_present, 1));
  HANDLE_CODE(bref.unpack(rat_type_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(cell_group_id, bref, (uint8_t)0u, (uint8_t)3u, true, true));
  if (ul_cfg_present) {
    HANDLE_CODE(ul_cfg.unpack(bref));
  }
  if (dl_tx_stop_present) {
    HANDLE_CODE(dl_tx_stop.unpack(bref));
  }
  if (rat_type_present) {
    HANDLE_CODE(rat_type.unpack(bref));
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
void cell_group_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("cell-Group-ID", cell_group_id);
  if (ul_cfg_present) {
    j.write_str("uL-Configuration", ul_cfg.to_string());
  }
  if (dl_tx_stop_present) {
    j.write_str("dL-TX-Stop", dl_tx_stop.to_string());
  }
  if (rat_type_present) {
    j.write_str("rAT-Type", rat_type.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CipheringAlgorithm ::= ENUMERATED
const char* ciphering_algorithm_opts::to_string() const
{
  static const char* names[] = {"nEA0", "c-128-NEA1", "c-128-NEA2", "c-128-NEA3"};
  return convert_enum_idx(names, 4, value, "ciphering_algorithm_e");
}

// ConfidentialityProtectionIndication ::= ENUMERATED
const char* confidentiality_protection_ind_opts::to_string() const
{
  static const char* names[] = {"required", "preferred", "not-needed"};
  return convert_enum_idx(names, 3, value, "confidentiality_protection_ind_e");
}

// ConfidentialityProtectionResult ::= ENUMERATED
const char* confidentiality_protection_result_opts::to_string() const
{
  static const char* names[] = {"performed", "not-performed"};
  return convert_enum_idx(names, 2, value, "confidentiality_protection_result_e");
}

// TypeOfError ::= ENUMERATED
const char* type_of_error_opts::to_string() const
{
  static const char* names[] = {"not-understood", "missing"};
  return convert_enum_idx(names, 2, value, "type_of_error_e");
}

OCUDUASN_CODE crit_diagnostics_ie_list_item_s_::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ie_crit.pack(bref));
  HANDLE_CODE(pack_integer(bref, ie_id, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(type_of_error.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE crit_diagnostics_ie_list_item_s_::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ie_crit.unpack(bref));
  HANDLE_CODE(unpack_integer(ie_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(type_of_error.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void crit_diagnostics_ie_list_item_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("iECriticality", ie_crit.to_string());
  j.write_int("iE-ID", ie_id);
  j.write_str("typeOfError", type_of_error.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// CriticalityDiagnostics ::= SEQUENCE
OCUDUASN_CODE crit_diagnostics_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(proc_code_present, 1));
  HANDLE_CODE(bref.pack(trigger_msg_present, 1));
  HANDLE_CODE(bref.pack(proc_crit_present, 1));
  HANDLE_CODE(bref.pack(transaction_id_present, 1));
  HANDLE_CODE(bref.pack(ies_crit_diagnostics.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (proc_code_present) {
    HANDLE_CODE(pack_integer(bref, proc_code, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (trigger_msg_present) {
    HANDLE_CODE(trigger_msg.pack(bref));
  }
  if (proc_crit_present) {
    HANDLE_CODE(proc_crit.pack(bref));
  }
  if (transaction_id_present) {
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ies_crit_diagnostics.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ies_crit_diagnostics, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE crit_diagnostics_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(proc_code_present, 1));
  HANDLE_CODE(bref.unpack(trigger_msg_present, 1));
  HANDLE_CODE(bref.unpack(proc_crit_present, 1));
  HANDLE_CODE(bref.unpack(transaction_id_present, 1));
  bool ies_crit_diagnostics_present;
  HANDLE_CODE(bref.unpack(ies_crit_diagnostics_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (proc_code_present) {
    HANDLE_CODE(unpack_integer(proc_code, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (trigger_msg_present) {
    HANDLE_CODE(trigger_msg.unpack(bref));
  }
  if (proc_crit_present) {
    HANDLE_CODE(proc_crit.unpack(bref));
  }
  if (transaction_id_present) {
    HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ies_crit_diagnostics_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ies_crit_diagnostics, bref, 1, 256, true));
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
void crit_diagnostics_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (proc_code_present) {
    j.write_int("procedureCode", proc_code);
  }
  if (trigger_msg_present) {
    j.write_str("triggeringMessage", trigger_msg.to_string());
  }
  if (proc_crit_present) {
    j.write_str("procedureCriticality", proc_crit.to_string());
  }
  if (transaction_id_present) {
    j.write_int("transactionID", transaction_id);
  }
  if (ies_crit_diagnostics.size() > 0) {
    j.start_array("iEsCriticalityDiagnostics");
    for (const auto& e1 : ies_crit_diagnostics) {
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

// DAPSRequestInfo ::= SEQUENCE
OCUDUASN_CODE daps_request_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(daps_ind.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE daps_request_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(daps_ind.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void daps_request_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("dapsIndicator", "daps-HO-required");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* daps_request_info_s::daps_ind_opts::to_string() const
{
  static const char* names[] = {"daps-HO-required"};
  return convert_enum_idx(names, 1, value, "daps_request_info_s::daps_ind_e_");
}

// PDCP-Count ::= SEQUENCE
OCUDUASN_CODE pdcp_count_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdcp_sn, (uint32_t)0u, (uint32_t)262143u, false, true));
  HANDLE_CODE(pack_integer(bref, hfn, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcp_count_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdcp_sn, bref, (uint32_t)0u, (uint32_t)262143u, false, true));
  HANDLE_CODE(unpack_integer(hfn, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdcp_count_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDCP-SN", pdcp_sn);
  j.write_uint("hFN", hfn);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DLDiscarding ::= SEQUENCE
OCUDUASN_CODE dl_discarding_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(dl_discarding_count_val.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_discarding_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(dl_discarding_count_val.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void dl_discarding_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("dLDiscardingCountVal");
  dl_discarding_count_val.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DLUPTNLAddressToUpdateItem ::= SEQUENCE
OCUDUASN_CODE dl_up_tnl_address_to_upd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(old_tnl_adress.pack(bref));
  HANDLE_CODE(new_tnl_adress.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_up_tnl_address_to_upd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(old_tnl_adress.unpack(bref));
  HANDLE_CODE(new_tnl_adress.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_up_tnl_address_to_upd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("oldTNLAdress");
  old_tnl_adress.to_json(j);
  j.write_fieldname("newTNLAdress");
  new_tnl_adress.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Confirm-Modified-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_confirm_modified_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cell_group_info.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (cell_group_info.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_info, 1, 4, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_confirm_modified_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool cell_group_info_present;
  HANDLE_CODE(bref.unpack(cell_group_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (cell_group_info_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cell_group_info, bref, 1, 4, true));
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
void drb_confirm_modified_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (cell_group_info.size() > 0) {
    j.start_array("cell-Group-Information");
    for (const auto& e1 : cell_group_info) {
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

// DRB-Confirm-Modified-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_confirm_modified_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(cell_group_info.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (cell_group_info.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_info, 1, 4, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_confirm_modified_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool cell_group_info_present;
  HANDLE_CODE(bref.unpack(cell_group_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (cell_group_info_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cell_group_info, bref, 1, 4, true));
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
void drb_confirm_modified_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (cell_group_info.size() > 0) {
    j.start_array("cell-Group-Information");
    for (const auto& e1 : cell_group_info) {
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

// DRB-Failed-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_failed_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_failed_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_failed_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Failed-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_failed_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_failed_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_failed_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Failed-Mod-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_failed_mod_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_failed_mod_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_failed_mod_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Failed-Mod-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_failed_mod_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_failed_mod_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_failed_mod_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Failed-To-Modify-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_failed_to_modify_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_failed_to_modify_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_failed_to_modify_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Failed-To-Modify-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_failed_to_modify_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_failed_to_modify_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_failed_to_modify_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Measurement-Results-Information-Item ::= SEQUENCE
OCUDUASN_CODE drb_meas_results_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ul_d1_result_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (ul_d1_result_present) {
    HANDLE_CODE(pack_integer(bref, ul_d1_result, (uint16_t)0u, (uint16_t)10000u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_meas_results_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ul_d1_result_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (ul_d1_result_present) {
    HANDLE_CODE(unpack_integer(ul_d1_result, bref, (uint16_t)0u, (uint16_t)10000u, true, true));
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
void drb_meas_results_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (ul_d1_result_present) {
    j.write_int("uL-D1-Result", ul_d1_result);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRBBStatusTransfer ::= SEQUENCE
OCUDUASN_CODE drb_b_status_transfer_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(receive_statusof_pdcp_sdu_present, 1));
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  if (receive_statusof_pdcp_sdu_present) {
    HANDLE_CODE(receive_statusof_pdcp_sdu.pack(bref));
  }
  HANDLE_CODE(count_value.pack(bref));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_b_status_transfer_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(receive_statusof_pdcp_sdu_present, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  if (receive_statusof_pdcp_sdu_present) {
    HANDLE_CODE(receive_statusof_pdcp_sdu.unpack(bref));
  }
  HANDLE_CODE(count_value.unpack(bref));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_b_status_transfer_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (receive_statusof_pdcp_sdu_present) {
    j.write_fieldname("receiveStatusofPDCPSDU");
    receive_statusof_pdcp_sdu.to_json(j);
  }
  j.write_fieldname("countValue");
  count_value.to_json(j);
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// PDCP-SN-Status-Information ::= SEQUENCE
OCUDUASN_CODE pdcp_sn_status_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_ext_present, 1));

  HANDLE_CODE(pdcp_status_transfer_ul.pack(bref));
  HANDLE_CODE(pdcp_status_transfer_dl.pack(bref));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcp_sn_status_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_ext_present, 1));

  HANDLE_CODE(pdcp_status_transfer_ul.unpack(bref));
  HANDLE_CODE(pdcp_status_transfer_dl.unpack(bref));
  if (ie_ext_present) {
    HANDLE_CODE(ie_ext.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdcp_sn_status_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("pdcpStatusTransfer-UL");
  pdcp_status_transfer_ul.to_json(j);
  j.write_fieldname("pdcpStatusTransfer-DL");
  pdcp_status_transfer_dl.to_json(j);
  if (ie_ext_present) {
    j.write_fieldname("iE-Extension");
    ie_ext.to_json(j);
  }
  j.end_obj();
}

// QoS-Mapping-Information ::= SEQUENCE
OCUDUASN_CODE qos_map_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(dscp_present, 1));
  HANDLE_CODE(bref.pack(flow_label_present, 1));

  if (dscp_present) {
    HANDLE_CODE(dscp.pack(bref));
  }
  if (flow_label_present) {
    HANDLE_CODE(flow_label.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_map_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(dscp_present, 1));
  HANDLE_CODE(bref.unpack(flow_label_present, 1));

  if (dscp_present) {
    HANDLE_CODE(dscp.unpack(bref));
  }
  if (flow_label_present) {
    HANDLE_CODE(flow_label.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void qos_map_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (dscp_present) {
    j.write_fieldname("dscp");
    dscp.to_json(j);
  }
  if (flow_label_present) {
    j.write_fieldname("flow-label");
    flow_label.to_json(j);
  }
  j.end_obj();
}

// IndirectPathIndication ::= ENUMERATED
const char* indirect_path_ind_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "indirect_path_ind_e");
}

// UP-Parameters-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t up_params_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {107, 206};
  return map_enum_number(names, 2, idx, "id");
}
bool up_params_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {107, 206};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e up_params_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 107:
      return crit_e::reject;
    case 206:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e up_params_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 107:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE up_params_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += qos_map_info_present ? 1 : 0;
  nof_ies += indirect_path_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (qos_map_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)107, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(qos_map_info.pack(bref));
  }
  if (indirect_path_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)206, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(indirect_path_ind.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE up_params_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != up_params_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 107: {
        qos_map_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(qos_map_info.unpack(bref));
        break;
      }
      case 206: {
        indirect_path_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(indirect_path_ind.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void up_params_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (qos_map_info_present) {
    j.start_obj();
    j.write_int("id", 107);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    qos_map_info.to_json(j);
    j.end_obj();
  }
  if (indirect_path_ind_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  j.end_array();
}

// UP-Parameters-Item ::= SEQUENCE
OCUDUASN_CODE up_params_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(up_tnl_info.pack(bref));
  HANDLE_CODE(pack_integer(bref, cell_group_id, (uint8_t)0u, (uint8_t)3u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE up_params_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(up_tnl_info.unpack(bref));
  HANDLE_CODE(unpack_integer(cell_group_id, bref, (uint8_t)0u, (uint8_t)3u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void up_params_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("uP-TNL-Information");
  up_tnl_info.to_json(j);
  j.write_int("cell-Group-ID", cell_group_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Modified-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_modified_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(s1_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.pack(pdcp_sn_status_info_present, 1));
  HANDLE_CODE(bref.pack(ul_up_transport_params.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (s1_dl_up_tnl_info_present) {
    HANDLE_CODE(s1_dl_up_tnl_info.pack(bref));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.pack(bref));
  }
  if (ul_up_transport_params.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ul_up_transport_params, 1, 8, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_modified_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(s1_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_sn_status_info_present, 1));
  bool ul_up_transport_params_present;
  HANDLE_CODE(bref.unpack(ul_up_transport_params_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (s1_dl_up_tnl_info_present) {
    HANDLE_CODE(s1_dl_up_tnl_info.unpack(bref));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.unpack(bref));
  }
  if (ul_up_transport_params_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ul_up_transport_params, bref, 1, 8, true));
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
void drb_modified_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (s1_dl_up_tnl_info_present) {
    j.write_fieldname("s1-DL-UP-TNL-Information");
    s1_dl_up_tnl_info.to_json(j);
  }
  if (pdcp_sn_status_info_present) {
    j.write_fieldname("pDCP-SN-Status-Information");
    pdcp_sn_status_info.to_json(j);
  }
  if (ul_up_transport_params.size() > 0) {
    j.start_array("uL-UP-Transport-Parameters");
    for (const auto& e1 : ul_up_transport_params) {
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

// FirstDLCount ::= SEQUENCE
OCUDUASN_CODE first_dl_count_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(first_dl_count_val.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE first_dl_count_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(first_dl_count_val.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void first_dl_count_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("firstDLCountVal");
  first_dl_count_val.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// EarlyForwardingCOUNTInfo ::= CHOICE
void early_forwarding_count_info_c::destroy_()
{
  switch (type_) {
    case types::first_dl_count:
      c.destroy<first_dl_count_s>();
      break;
    case types::dl_discarding_count:
      c.destroy<dl_discarding_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void early_forwarding_count_info_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::first_dl_count:
      c.init<first_dl_count_s>();
      break;
    case types::dl_discarding_count:
      c.init<dl_discarding_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "early_forwarding_count_info_c");
  }
}
early_forwarding_count_info_c::early_forwarding_count_info_c(const early_forwarding_count_info_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::first_dl_count:
      c.init(other.c.get<first_dl_count_s>());
      break;
    case types::dl_discarding_count:
      c.init(other.c.get<dl_discarding_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "early_forwarding_count_info_c");
  }
}
early_forwarding_count_info_c& early_forwarding_count_info_c::operator=(const early_forwarding_count_info_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::first_dl_count:
      c.set(other.c.get<first_dl_count_s>());
      break;
    case types::dl_discarding_count:
      c.set(other.c.get<dl_discarding_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "early_forwarding_count_info_c");
  }

  return *this;
}
first_dl_count_s& early_forwarding_count_info_c::set_first_dl_count()
{
  set(types::first_dl_count);
  return c.get<first_dl_count_s>();
}
dl_discarding_s& early_forwarding_count_info_c::set_dl_discarding_count()
{
  set(types::dl_discarding_count);
  return c.get<dl_discarding_s>();
}
protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>& early_forwarding_count_info_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>>();
}
void early_forwarding_count_info_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::first_dl_count:
      j.write_fieldname("firstDLCount");
      c.get<first_dl_count_s>().to_json(j);
      break;
    case types::dl_discarding_count:
      j.write_fieldname("dLDiscardingCount");
      c.get<dl_discarding_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "early_forwarding_count_info_c");
  }
  j.end_obj();
}
OCUDUASN_CODE early_forwarding_count_info_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::first_dl_count:
      HANDLE_CODE(c.get<first_dl_count_s>().pack(bref));
      break;
    case types::dl_discarding_count:
      HANDLE_CODE(c.get<dl_discarding_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "early_forwarding_count_info_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE early_forwarding_count_info_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::first_dl_count:
      HANDLE_CODE(c.get<first_dl_count_s>().unpack(bref));
      break;
    case types::dl_discarding_count:
      HANDLE_CODE(c.get<dl_discarding_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<early_forwarding_count_info_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "early_forwarding_count_info_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* early_forwarding_count_info_c::types_opts::to_string() const
{
  static const char* names[] = {"firstDLCount", "dLDiscardingCount", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "early_forwarding_count_info_c::types");
}

// DRB-Modified-Item-NG-RAN-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t drb_modified_item_ng_ran_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {123, 71};
  return map_enum_number(names, 2, idx, "id");
}
bool drb_modified_item_ng_ran_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {123, 71};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e drb_modified_item_ng_ran_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 123:
      return crit_e::reject;
    case 71:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e drb_modified_item_ng_ran_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 123:
      return presence_e::optional;
    case 71:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE drb_modified_item_ng_ran_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += early_forwarding_count_info_present ? 1 : 0;
  nof_ies += old_qos_flow_map_ul_endmarkerexpected_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (early_forwarding_count_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)123, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(early_forwarding_count_info.pack(bref));
  }
  if (old_qos_flow_map_ul_endmarkerexpected_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)71, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, old_qos_flow_map_ul_endmarkerexpected, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_modified_item_ng_ran_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != drb_modified_item_ng_ran_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 123: {
        early_forwarding_count_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(early_forwarding_count_info.unpack(bref));
        break;
      }
      case 71: {
        old_qos_flow_map_ul_endmarkerexpected_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(old_qos_flow_map_ul_endmarkerexpected, bref, 1, 64, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void drb_modified_item_ng_ran_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (early_forwarding_count_info_present) {
    j.start_obj();
    j.write_int("id", 123);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    early_forwarding_count_info.to_json(j);
    j.end_obj();
  }
  if (old_qos_flow_map_ul_endmarkerexpected_present) {
    j.start_obj();
    j.write_int("id", 71);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : old_qos_flow_map_ul_endmarkerexpected) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// DRB-Modified-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_modified_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ul_up_transport_params.size() > 0, 1));
  HANDLE_CODE(bref.pack(pdcp_sn_status_info_present, 1));
  HANDLE_CODE(bref.pack(flow_setup_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(flow_failed_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (ul_up_transport_params.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ul_up_transport_params, 1, 8, true));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.pack(bref));
  }
  if (flow_setup_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, flow_setup_list, 1, 64, true));
  }
  if (flow_failed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, flow_failed_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_modified_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ul_up_transport_params_present;
  HANDLE_CODE(bref.unpack(ul_up_transport_params_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_sn_status_info_present, 1));
  bool flow_setup_list_present;
  HANDLE_CODE(bref.unpack(flow_setup_list_present, 1));
  bool flow_failed_list_present;
  HANDLE_CODE(bref.unpack(flow_failed_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (ul_up_transport_params_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ul_up_transport_params, bref, 1, 8, true));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.unpack(bref));
  }
  if (flow_setup_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(flow_setup_list, bref, 1, 64, true));
  }
  if (flow_failed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(flow_failed_list, bref, 1, 64, true));
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
void drb_modified_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (ul_up_transport_params.size() > 0) {
    j.start_array("uL-UP-Transport-Parameters");
    for (const auto& e1 : ul_up_transport_params) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdcp_sn_status_info_present) {
    j.write_fieldname("pDCP-SN-Status-Information");
    pdcp_sn_status_info.to_json(j);
  }
  if (flow_setup_list.size() > 0) {
    j.start_array("flow-Setup-List");
    for (const auto& e1 : flow_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (flow_failed_list.size() > 0) {
    j.start_array("flow-Failed-List");
    for (const auto& e1 : flow_failed_list) {
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

// QoS-Flow-Removed-Item ::= SEQUENCE
OCUDUASN_CODE qos_flow_remd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qos_flow_released_in_session_present, 1));
  HANDLE_CODE(bref.pack(qos_flow_accumulated_session_time_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (qos_flow_released_in_session_present) {
    HANDLE_CODE(qos_flow_released_in_session.pack(bref));
  }
  if (qos_flow_accumulated_session_time_present) {
    HANDLE_CODE(qos_flow_accumulated_session_time.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_remd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(qos_flow_released_in_session_present, 1));
  HANDLE_CODE(bref.unpack(qos_flow_accumulated_session_time_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (qos_flow_released_in_session_present) {
    HANDLE_CODE(qos_flow_released_in_session.unpack(bref));
  }
  if (qos_flow_accumulated_session_time_present) {
    HANDLE_CODE(qos_flow_accumulated_session_time.unpack(bref));
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
void qos_flow_remd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoS-Flow-Identifier", qos_flow_id);
  if (qos_flow_released_in_session_present) {
    j.write_str("qoS-Flow-Released-In-Session", qos_flow_released_in_session.to_string());
  }
  if (qos_flow_accumulated_session_time_present) {
    j.write_str("qoS-Flow-Accumulated-Session-Time", qos_flow_accumulated_session_time.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* qos_flow_remd_item_s::qos_flow_released_in_session_opts::to_string() const
{
  static const char* names[] = {"released-in-session", "not-released-in-session"};
  return convert_enum_idx(names, 2, value, "qos_flow_remd_item_s::qos_flow_released_in_session_e_");
}

// DRB-Removed-Item ::= SEQUENCE
OCUDUASN_CODE drb_remd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(drb_released_in_session_present, 1));
  HANDLE_CODE(bref.pack(drb_accumulated_session_time_present, 1));
  HANDLE_CODE(bref.pack(qos_flow_remd_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (drb_released_in_session_present) {
    HANDLE_CODE(drb_released_in_session.pack(bref));
  }
  if (drb_accumulated_session_time_present) {
    HANDLE_CODE(drb_accumulated_session_time.pack(bref));
  }
  if (qos_flow_remd_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qos_flow_remd_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_remd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(drb_released_in_session_present, 1));
  HANDLE_CODE(bref.unpack(drb_accumulated_session_time_present, 1));
  bool qos_flow_remd_list_present;
  HANDLE_CODE(bref.unpack(qos_flow_remd_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (drb_released_in_session_present) {
    HANDLE_CODE(drb_released_in_session.unpack(bref));
  }
  if (drb_accumulated_session_time_present) {
    HANDLE_CODE(drb_accumulated_session_time.unpack(bref));
  }
  if (qos_flow_remd_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qos_flow_remd_list, bref, 1, 64, true));
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
void drb_remd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (drb_released_in_session_present) {
    j.write_str("dRB-Released-In-Session", drb_released_in_session.to_string());
  }
  if (drb_accumulated_session_time_present) {
    j.write_str("dRB-Accumulated-Session-Time", drb_accumulated_session_time.to_string());
  }
  if (qos_flow_remd_list.size() > 0) {
    j.start_array("qoS-Flow-Removed-List");
    for (const auto& e1 : qos_flow_remd_list) {
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

const char* drb_remd_item_s::drb_released_in_session_opts::to_string() const
{
  static const char* names[] = {"released-in-session", "not-released-in-session"};
  return convert_enum_idx(names, 2, value, "drb_remd_item_s::drb_released_in_session_e_");
}

// GNB-CU-UP-CellGroupRelatedConfiguration-Item ::= SEQUENCE
OCUDUASN_CODE gnb_cu_up_cell_group_related_cfg_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ul_cfg_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, cell_group_id, (uint8_t)0u, (uint8_t)3u, true, true));
  HANDLE_CODE(up_tnl_info.pack(bref));
  if (ul_cfg_present) {
    HANDLE_CODE(ul_cfg.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_cell_group_related_cfg_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ul_cfg_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(cell_group_id, bref, (uint8_t)0u, (uint8_t)3u, true, true));
  HANDLE_CODE(up_tnl_info.unpack(bref));
  if (ul_cfg_present) {
    HANDLE_CODE(ul_cfg.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void gnb_cu_up_cell_group_related_cfg_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("cell-Group-ID", cell_group_id);
  j.write_fieldname("uP-TNL-Information");
  up_tnl_info.to_json(j);
  if (ul_cfg_present) {
    j.write_str("uL-Configuration", ul_cfg.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Required-To-Modify-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_required_to_modify_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(s1_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.pack(gnb_cu_up_cell_group_related_cfg.size() > 0, 1));
  HANDLE_CODE(bref.pack(cause_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (s1_dl_up_tnl_info_present) {
    HANDLE_CODE(s1_dl_up_tnl_info.pack(bref));
  }
  if (gnb_cu_up_cell_group_related_cfg.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_cu_up_cell_group_related_cfg, 1, 8, true));
  }
  if (cause_present) {
    HANDLE_CODE(cause.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_required_to_modify_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(s1_dl_up_tnl_info_present, 1));
  bool gnb_cu_up_cell_group_related_cfg_present;
  HANDLE_CODE(bref.unpack(gnb_cu_up_cell_group_related_cfg_present, 1));
  HANDLE_CODE(bref.unpack(cause_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (s1_dl_up_tnl_info_present) {
    HANDLE_CODE(s1_dl_up_tnl_info.unpack(bref));
  }
  if (gnb_cu_up_cell_group_related_cfg_present) {
    HANDLE_CODE(unpack_dyn_seq_of(gnb_cu_up_cell_group_related_cfg, bref, 1, 8, true));
  }
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
void drb_required_to_modify_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (s1_dl_up_tnl_info_present) {
    j.write_fieldname("s1-DL-UP-TNL-Information");
    s1_dl_up_tnl_info.to_json(j);
  }
  if (gnb_cu_up_cell_group_related_cfg.size() > 0) {
    j.start_array("gNB-CU-UP-CellGroupRelatedConfiguration");
    for (const auto& e1 : gnb_cu_up_cell_group_related_cfg) {
      e1.to_json(j);
    }
    j.end_array();
  }
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

// DRB-Required-To-Modify-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_required_to_modify_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(gnb_cu_up_cell_group_related_cfg.size() > 0, 1));
  HANDLE_CODE(bref.pack(flow_to_rem.size() > 0, 1));
  HANDLE_CODE(bref.pack(cause_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (gnb_cu_up_cell_group_related_cfg.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_cu_up_cell_group_related_cfg, 1, 8, true));
  }
  if (flow_to_rem.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, flow_to_rem, 1, 64, true));
  }
  if (cause_present) {
    HANDLE_CODE(cause.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_required_to_modify_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool gnb_cu_up_cell_group_related_cfg_present;
  HANDLE_CODE(bref.unpack(gnb_cu_up_cell_group_related_cfg_present, 1));
  bool flow_to_rem_present;
  HANDLE_CODE(bref.unpack(flow_to_rem_present, 1));
  HANDLE_CODE(bref.unpack(cause_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (gnb_cu_up_cell_group_related_cfg_present) {
    HANDLE_CODE(unpack_dyn_seq_of(gnb_cu_up_cell_group_related_cfg, bref, 1, 8, true));
  }
  if (flow_to_rem_present) {
    HANDLE_CODE(unpack_dyn_seq_of(flow_to_rem, bref, 1, 64, true));
  }
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
void drb_required_to_modify_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (gnb_cu_up_cell_group_related_cfg.size() > 0) {
    j.start_array("gNB-CU-UP-CellGroupRelatedConfiguration");
    for (const auto& e1 : gnb_cu_up_cell_group_related_cfg) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (flow_to_rem.size() > 0) {
    j.start_array("flow-To-Remove");
    for (const auto& e1 : flow_to_rem) {
      e1.to_json(j);
    }
    j.end_array();
  }
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

// DRB-Required-To-Remove-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_required_to_rem_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_required_to_rem_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_required_to_rem_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Required-To-Remove-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_required_to_rem_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_required_to_rem_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_required_to_rem_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DataForwardingtoNG-RANQoSFlowInformationList-Item ::= SEQUENCE
OCUDUASN_CODE data_forwardingto_ng_ran_qos_flow_info_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_forwardingto_ng_ran_qos_flow_info_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void data_forwardingto_ng_ran_qos_flow_info_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoS-Flow-Identifier", qos_flow_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDUSetbasedHandlingIndicator ::= ENUMERATED
const char* pdu_setbased_handling_ind_opts::to_string() const
{
  static const char* names[] = {"supported"};
  return convert_enum_idx(names, 1, value, "pdu_setbased_handling_ind_e");
}

// Data-Forwarding-Information-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t data_forwarding_info_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {136, 205};
  return map_enum_number(names, 2, idx, "id");
}
bool data_forwarding_info_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {136, 205};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e data_forwarding_info_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 136:
      return crit_e::ignore;
    case 205:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e data_forwarding_info_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 136:
      return presence_e::optional;
    case 205:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE data_forwarding_info_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += data_forwardingto_ng_ran_qos_flow_info_list_present ? 1 : 0;
  nof_ies += pdu_setbased_handling_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (data_forwardingto_ng_ran_qos_flow_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)136, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, data_forwardingto_ng_ran_qos_flow_info_list, 1, 64, true));
  }
  if (pdu_setbased_handling_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)205, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdu_setbased_handling_ind.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_forwarding_info_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != data_forwarding_info_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 136: {
        data_forwardingto_ng_ran_qos_flow_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(data_forwardingto_ng_ran_qos_flow_info_list, bref, 1, 64, true));
        break;
      }
      case 205: {
        pdu_setbased_handling_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdu_setbased_handling_ind.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void data_forwarding_info_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (data_forwardingto_ng_ran_qos_flow_info_list_present) {
    j.start_obj();
    j.write_int("id", 136);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : data_forwardingto_ng_ran_qos_flow_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_setbased_handling_ind_present) {
    j.start_obj();
    j.write_int("id", 205);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "supported");
    j.end_obj();
  }
  j.end_array();
}

// Data-Forwarding-Information ::= SEQUENCE
OCUDUASN_CODE data_forwarding_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ul_data_forwarding_present, 1));
  HANDLE_CODE(bref.pack(dl_data_forwarding_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (ul_data_forwarding_present) {
    HANDLE_CODE(ul_data_forwarding.pack(bref));
  }
  if (dl_data_forwarding_present) {
    HANDLE_CODE(dl_data_forwarding.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_forwarding_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ul_data_forwarding_present, 1));
  HANDLE_CODE(bref.unpack(dl_data_forwarding_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (ul_data_forwarding_present) {
    HANDLE_CODE(ul_data_forwarding.unpack(bref));
  }
  if (dl_data_forwarding_present) {
    HANDLE_CODE(dl_data_forwarding.unpack(bref));
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
void data_forwarding_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ul_data_forwarding_present) {
    j.write_fieldname("uL-Data-Forwarding");
    ul_data_forwarding.to_json(j);
  }
  if (dl_data_forwarding_present) {
    j.write_fieldname("dL-Data-Forwarding");
    dl_data_forwarding.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// IntegrityProtectionResult ::= ENUMERATED
const char* integrity_protection_result_opts::to_string() const
{
  static const char* names[] = {"performed", "not-performed"};
  return convert_enum_idx(names, 2, value, "integrity_protection_result_e");
}

// SecurityResult ::= SEQUENCE
OCUDUASN_CODE security_result_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(integrity_protection_result.pack(bref));
  HANDLE_CODE(confidentiality_protection_result.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE security_result_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(integrity_protection_result.unpack(bref));
  HANDLE_CODE(confidentiality_protection_result.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void security_result_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("integrityProtectionResult", integrity_protection_result.to_string());
  j.write_str("confidentialityProtectionResult", confidentiality_protection_result.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Setup-Item-EUTRAN-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t drb_setup_item_eutran_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {142, 173};
  return map_enum_number(names, 2, idx, "id");
}
bool drb_setup_item_eutran_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {142, 173};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e drb_setup_item_eutran_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 142:
      return crit_e::ignore;
    case 173:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e drb_setup_item_eutran_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 142:
      return presence_e::optional;
    case 173:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE drb_setup_item_eutran_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += data_forwarding_source_ip_address_present ? 1 : 0;
  nof_ies += security_result_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (data_forwarding_source_ip_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)142, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(data_forwarding_source_ip_address.pack(bref));
  }
  if (security_result_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)173, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(security_result.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_setup_item_eutran_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != drb_setup_item_eutran_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 142: {
        data_forwarding_source_ip_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(data_forwarding_source_ip_address.unpack(bref));
        break;
      }
      case 173: {
        security_result_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(security_result.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void drb_setup_item_eutran_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (data_forwarding_source_ip_address_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    data_forwarding_source_ip_address.to_json(j);
    j.end_obj();
  }
  if (security_result_present) {
    j.start_obj();
    j.write_int("id", 173);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    security_result.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DRB-Setup-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_setup_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.pack(s1_dl_up_unchanged_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(s1_dl_up_tnl_info.pack(bref));
  if (data_forwarding_info_resp_present) {
    HANDLE_CODE(data_forwarding_info_resp.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, ul_up_transport_params, 1, 8, true));
  if (s1_dl_up_unchanged_present) {
    HANDLE_CODE(s1_dl_up_unchanged.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_setup_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.unpack(s1_dl_up_unchanged_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(s1_dl_up_tnl_info.unpack(bref));
  if (data_forwarding_info_resp_present) {
    HANDLE_CODE(data_forwarding_info_resp.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(ul_up_transport_params, bref, 1, 8, true));
  if (s1_dl_up_unchanged_present) {
    HANDLE_CODE(s1_dl_up_unchanged.unpack(bref));
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
void drb_setup_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("s1-DL-UP-TNL-Information");
  s1_dl_up_tnl_info.to_json(j);
  if (data_forwarding_info_resp_present) {
    j.write_fieldname("data-Forwarding-Information-Response");
    data_forwarding_info_resp.to_json(j);
  }
  j.start_array("uL-UP-Transport-Parameters");
  for (const auto& e1 : ul_up_transport_params) {
    e1.to_json(j);
  }
  j.end_array();
  if (s1_dl_up_unchanged_present) {
    j.write_str("s1-DL-UP-Unchanged", "true");
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* drb_setup_item_eutran_s::s1_dl_up_unchanged_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "drb_setup_item_eutran_s::s1_dl_up_unchanged_e_");
}

// DRB-Setup-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_setup_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(drb_data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.pack(flow_failed_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (drb_data_forwarding_info_resp_present) {
    HANDLE_CODE(drb_data_forwarding_info_resp.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, ul_up_transport_params, 1, 8, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, flow_setup_list, 1, 64, true));
  if (flow_failed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, flow_failed_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_setup_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(drb_data_forwarding_info_resp_present, 1));
  bool flow_failed_list_present;
  HANDLE_CODE(bref.unpack(flow_failed_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (drb_data_forwarding_info_resp_present) {
    HANDLE_CODE(drb_data_forwarding_info_resp.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(ul_up_transport_params, bref, 1, 8, true));
  HANDLE_CODE(unpack_dyn_seq_of(flow_setup_list, bref, 1, 64, true));
  if (flow_failed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(flow_failed_list, bref, 1, 64, true));
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
void drb_setup_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (drb_data_forwarding_info_resp_present) {
    j.write_fieldname("dRB-data-Forwarding-Information-Response");
    drb_data_forwarding_info_resp.to_json(j);
  }
  j.start_array("uL-UP-Transport-Parameters");
  for (const auto& e1 : ul_up_transport_params) {
    e1.to_json(j);
  }
  j.end_array();
  j.start_array("flow-Setup-List");
  for (const auto& e1 : flow_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (flow_failed_list.size() > 0) {
    j.start_array("flow-Failed-List");
    for (const auto& e1 : flow_failed_list) {
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

// DRB-Setup-Mod-Item-EUTRAN-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t drb_setup_mod_item_eutran_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {173, 142};
  return map_enum_number(names, 2, idx, "id");
}
bool drb_setup_mod_item_eutran_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {173, 142};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e drb_setup_mod_item_eutran_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 173:
      return crit_e::ignore;
    case 142:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e drb_setup_mod_item_eutran_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 173:
      return presence_e::optional;
    case 142:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE drb_setup_mod_item_eutran_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += security_result_present ? 1 : 0;
  nof_ies += data_forwarding_source_ip_address_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (security_result_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)173, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(security_result.pack(bref));
  }
  if (data_forwarding_source_ip_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)142, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(data_forwarding_source_ip_address.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_setup_mod_item_eutran_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != drb_setup_mod_item_eutran_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 173: {
        security_result_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(security_result.unpack(bref));
        break;
      }
      case 142: {
        data_forwarding_source_ip_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(data_forwarding_source_ip_address.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void drb_setup_mod_item_eutran_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (security_result_present) {
    j.start_obj();
    j.write_int("id", 173);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    security_result.to_json(j);
    j.end_obj();
  }
  if (data_forwarding_source_ip_address_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    data_forwarding_source_ip_address.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DRB-Setup-Mod-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_setup_mod_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(s1_dl_up_tnl_info.pack(bref));
  if (data_forwarding_info_resp_present) {
    HANDLE_CODE(data_forwarding_info_resp.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, ul_up_transport_params, 1, 8, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_setup_mod_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(s1_dl_up_tnl_info.unpack(bref));
  if (data_forwarding_info_resp_present) {
    HANDLE_CODE(data_forwarding_info_resp.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(ul_up_transport_params, bref, 1, 8, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_setup_mod_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("s1-DL-UP-TNL-Information");
  s1_dl_up_tnl_info.to_json(j);
  if (data_forwarding_info_resp_present) {
    j.write_fieldname("data-Forwarding-Information-Response");
    data_forwarding_info_resp.to_json(j);
  }
  j.start_array("uL-UP-Transport-Parameters");
  for (const auto& e1 : ul_up_transport_params) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Setup-Mod-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_setup_mod_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(drb_data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.pack(flow_failed_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (drb_data_forwarding_info_resp_present) {
    HANDLE_CODE(drb_data_forwarding_info_resp.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, ul_up_transport_params, 1, 8, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, flow_setup_list, 1, 64, true));
  if (flow_failed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, flow_failed_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_setup_mod_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(drb_data_forwarding_info_resp_present, 1));
  bool flow_failed_list_present;
  HANDLE_CODE(bref.unpack(flow_failed_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (drb_data_forwarding_info_resp_present) {
    HANDLE_CODE(drb_data_forwarding_info_resp.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(ul_up_transport_params, bref, 1, 8, true));
  HANDLE_CODE(unpack_dyn_seq_of(flow_setup_list, bref, 1, 64, true));
  if (flow_failed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(flow_failed_list, bref, 1, 64, true));
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
void drb_setup_mod_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (drb_data_forwarding_info_resp_present) {
    j.write_fieldname("dRB-data-Forwarding-Information-Response");
    drb_data_forwarding_info_resp.to_json(j);
  }
  j.start_array("uL-UP-Transport-Parameters");
  for (const auto& e1 : ul_up_transport_params) {
    e1.to_json(j);
  }
  j.end_array();
  j.start_array("flow-Setup-List");
  for (const auto& e1 : flow_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (flow_failed_list.size() > 0) {
    j.start_array("flow-Failed-List");
    for (const auto& e1 : flow_failed_list) {
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

// DRB-Status-Item ::= SEQUENCE
OCUDUASN_CODE drb_status_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdcp_dl_count_present, 1));
  HANDLE_CODE(bref.pack(pdcp_ul_count_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (pdcp_dl_count_present) {
    HANDLE_CODE(pdcp_dl_count.pack(bref));
  }
  if (pdcp_ul_count_present) {
    HANDLE_CODE(pdcp_ul_count.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_status_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(pdcp_dl_count_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_ul_count_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (pdcp_dl_count_present) {
    HANDLE_CODE(pdcp_dl_count.unpack(bref));
  }
  if (pdcp_ul_count_present) {
    HANDLE_CODE(pdcp_ul_count.unpack(bref));
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
void drb_status_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (pdcp_dl_count_present) {
    j.write_fieldname("pDCP-DL-Count");
    pdcp_dl_count.to_json(j);
  }
  if (pdcp_ul_count_present) {
    j.write_fieldname("pDCP-UL-Count");
    pdcp_ul_count.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// EUTRANAllocationAndRetentionPriority ::= SEQUENCE
OCUDUASN_CODE eutran_alloc_and_retention_prio_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, prio_level, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(pre_emption_cap.pack(bref));
  HANDLE_CODE(pre_emption_vulnerability.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_alloc_and_retention_prio_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(prio_level, bref, (uint8_t)0u, (uint8_t)15u, false, true));
  HANDLE_CODE(pre_emption_cap.unpack(bref));
  HANDLE_CODE(pre_emption_vulnerability.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void eutran_alloc_and_retention_prio_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("priorityLevel", prio_level);
  j.write_str("pre-emptionCapability", pre_emption_cap.to_string());
  j.write_str("pre-emptionVulnerability", pre_emption_vulnerability.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GBR-QosInformation ::= SEQUENCE
OCUDUASN_CODE gbr_qos_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, erab_max_bitrate_dl, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(pack_integer(bref, erab_max_bitrate_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(pack_integer(bref, erab_guaranteed_bitrate_dl, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(pack_integer(bref, erab_guaranteed_bitrate_ul, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gbr_qos_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(erab_max_bitrate_dl, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(unpack_integer(erab_max_bitrate_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(unpack_integer(erab_guaranteed_bitrate_dl, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  HANDLE_CODE(unpack_integer(erab_guaranteed_bitrate_ul, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void gbr_qos_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_uint("e-RAB-MaximumBitrateDL", erab_max_bitrate_dl);
  j.write_uint("e-RAB-MaximumBitrateUL", erab_max_bitrate_ul);
  j.write_uint("e-RAB-GuaranteedBitrateDL", erab_guaranteed_bitrate_dl);
  j.write_uint("e-RAB-GuaranteedBitrateUL", erab_guaranteed_bitrate_ul);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// EUTRAN-QoS ::= SEQUENCE
OCUDUASN_CODE eutran_qos_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(gbr_qos_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qci, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(eutran_alloc_and_retention_prio.pack(bref));
  if (gbr_qos_info_present) {
    HANDLE_CODE(gbr_qos_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_qos_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(gbr_qos_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qci, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(eutran_alloc_and_retention_prio.unpack(bref));
  if (gbr_qos_info_present) {
    HANDLE_CODE(gbr_qos_info.unpack(bref));
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
void eutran_qos_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qCI", qci);
  j.write_fieldname("eUTRANallocationAndRetentionPriority");
  eutran_alloc_and_retention_prio.to_json(j);
  if (gbr_qos_info_present) {
    j.write_fieldname("gbrQosInformation");
    gbr_qos_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDCP-SN-Status-Request ::= ENUMERATED
const char* pdcp_sn_status_request_opts::to_string() const
{
  static const char* names[] = {"requested"};
  return convert_enum_idx(names, 1, value, "pdcp_sn_status_request_e");
}

// DRB-To-Modify-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_to_modify_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdcp_cfg_present, 1));
  HANDLE_CODE(bref.pack(eutran_qos_present, 1));
  HANDLE_CODE(bref.pack(s1_ul_up_tnl_info_present, 1));
  HANDLE_CODE(bref.pack(data_forwarding_info_present, 1));
  HANDLE_CODE(bref.pack(pdcp_sn_status_request_present, 1));
  HANDLE_CODE(bref.pack(pdcp_sn_status_info_present, 1));
  HANDLE_CODE(bref.pack(dl_up_params.size() > 0, 1));
  HANDLE_CODE(bref.pack(cell_group_to_add.size() > 0, 1));
  HANDLE_CODE(bref.pack(cell_group_to_modify.size() > 0, 1));
  HANDLE_CODE(bref.pack(cell_group_to_rem.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (pdcp_cfg_present) {
    HANDLE_CODE(pdcp_cfg.pack(bref));
  }
  if (eutran_qos_present) {
    HANDLE_CODE(eutran_qos.pack(bref));
  }
  if (s1_ul_up_tnl_info_present) {
    HANDLE_CODE(s1_ul_up_tnl_info.pack(bref));
  }
  if (data_forwarding_info_present) {
    HANDLE_CODE(data_forwarding_info.pack(bref));
  }
  if (pdcp_sn_status_request_present) {
    HANDLE_CODE(pdcp_sn_status_request.pack(bref));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.pack(bref));
  }
  if (dl_up_params.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, dl_up_params, 1, 8, true));
  }
  if (cell_group_to_add.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_to_add, 1, 4, true));
  }
  if (cell_group_to_modify.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_to_modify, 1, 4, true));
  }
  if (cell_group_to_rem.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_to_rem, 1, 4, true));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, drb_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_modify_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(pdcp_cfg_present, 1));
  HANDLE_CODE(bref.unpack(eutran_qos_present, 1));
  HANDLE_CODE(bref.unpack(s1_ul_up_tnl_info_present, 1));
  HANDLE_CODE(bref.unpack(data_forwarding_info_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_sn_status_request_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_sn_status_info_present, 1));
  bool dl_up_params_present;
  HANDLE_CODE(bref.unpack(dl_up_params_present, 1));
  bool cell_group_to_add_present;
  HANDLE_CODE(bref.unpack(cell_group_to_add_present, 1));
  bool cell_group_to_modify_present;
  HANDLE_CODE(bref.unpack(cell_group_to_modify_present, 1));
  bool cell_group_to_rem_present;
  HANDLE_CODE(bref.unpack(cell_group_to_rem_present, 1));
  HANDLE_CODE(bref.unpack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (pdcp_cfg_present) {
    HANDLE_CODE(pdcp_cfg.unpack(bref));
  }
  if (eutran_qos_present) {
    HANDLE_CODE(eutran_qos.unpack(bref));
  }
  if (s1_ul_up_tnl_info_present) {
    HANDLE_CODE(s1_ul_up_tnl_info.unpack(bref));
  }
  if (data_forwarding_info_present) {
    HANDLE_CODE(data_forwarding_info.unpack(bref));
  }
  if (pdcp_sn_status_request_present) {
    HANDLE_CODE(pdcp_sn_status_request.unpack(bref));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.unpack(bref));
  }
  if (dl_up_params_present) {
    HANDLE_CODE(unpack_dyn_seq_of(dl_up_params, bref, 1, 8, true));
  }
  if (cell_group_to_add_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cell_group_to_add, bref, 1, 4, true));
  }
  if (cell_group_to_modify_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cell_group_to_modify, bref, 1, 4, true));
  }
  if (cell_group_to_rem_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cell_group_to_rem, bref, 1, 4, true));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(drb_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
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
void drb_to_modify_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (pdcp_cfg_present) {
    j.write_fieldname("pDCP-Configuration");
    pdcp_cfg.to_json(j);
  }
  if (eutran_qos_present) {
    j.write_fieldname("eUTRAN-QoS");
    eutran_qos.to_json(j);
  }
  if (s1_ul_up_tnl_info_present) {
    j.write_fieldname("s1-UL-UP-TNL-Information");
    s1_ul_up_tnl_info.to_json(j);
  }
  if (data_forwarding_info_present) {
    j.write_fieldname("data-Forwarding-Information");
    data_forwarding_info.to_json(j);
  }
  if (pdcp_sn_status_request_present) {
    j.write_str("pDCP-SN-Status-Request", "requested");
  }
  if (pdcp_sn_status_info_present) {
    j.write_fieldname("pDCP-SN-Status-Information");
    pdcp_sn_status_info.to_json(j);
  }
  if (dl_up_params.size() > 0) {
    j.start_array("dL-UP-Parameters");
    for (const auto& e1 : dl_up_params) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cell_group_to_add.size() > 0) {
    j.start_array("cell-Group-To-Add");
    for (const auto& e1 : cell_group_to_add) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cell_group_to_modify.size() > 0) {
    j.start_array("cell-Group-To-Modify");
    for (const auto& e1 : cell_group_to_modify) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cell_group_to_rem.size() > 0) {
    j.start_array("cell-Group-To-Remove");
    for (const auto& e1 : cell_group_to_rem) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_inactivity_timer_present) {
    j.write_int("dRB-Inactivity-Timer", drb_inactivity_timer);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DefaultDRB ::= ENUMERATED
const char* default_drb_opts::to_string() const
{
  static const char* names[] = {"true", "false"};
  return convert_enum_idx(names, 2, value, "default_drb_e");
}

// SDAP-Header-UL ::= ENUMERATED
const char* sdap_hdr_ul_opts::to_string() const
{
  static const char* names[] = {"present", "absent"};
  return convert_enum_idx(names, 2, value, "sdap_hdr_ul_e");
}

// SDAP-Header-DL ::= ENUMERATED
const char* sdap_hdr_dl_opts::to_string() const
{
  static const char* names[] = {"present", "absent"};
  return convert_enum_idx(names, 2, value, "sdap_hdr_dl_e");
}

// SDAP-Configuration ::= SEQUENCE
OCUDUASN_CODE sdap_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(default_drb.pack(bref));
  HANDLE_CODE(sdap_hdr_ul.pack(bref));
  HANDLE_CODE(sdap_hdr_dl.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sdap_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(default_drb.unpack(bref));
  HANDLE_CODE(sdap_hdr_ul.unpack(bref));
  HANDLE_CODE(sdap_hdr_dl.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sdap_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("defaultDRB", default_drb.to_string());
  j.write_str("sDAP-Header-UL", sdap_hdr_ul.to_string());
  j.write_str("sDAP-Header-DL", sdap_hdr_dl.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// EarlyForwardingCOUNTReq ::= ENUMERATED
const char* early_forwarding_count_req_opts::to_string() const
{
  static const char* names[] = {"first-dl-count", "dl-discarding"};
  return convert_enum_idx(names, 2, value, "early_forwarding_count_req_e");
}

// EarlyDataForwardingIndicator ::= ENUMERATED
const char* early_data_forwarding_ind_opts::to_string() const
{
  static const char* names[] = {"stop"};
  return convert_enum_idx(names, 1, value, "early_data_forwarding_ind_e");
}

// SDTindicatorMod ::= ENUMERATED
const char* sd_tind_mod_opts::to_string() const
{
  static const char* names[] = {"true", "false"};
  return convert_enum_idx(names, 2, value, "sd_tind_mod_e");
}

// PDCP-COUNT-Reset ::= ENUMERATED
const char* pdcp_count_reset_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "pdcp_count_reset_e");
}

// DRB-To-Modify-Item-NG-RAN-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t drb_to_modify_item_ng_ran_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {71, 72, 122, 123, 120, 140, 176, 184};
  return map_enum_number(names, 8, idx, "id");
}
bool drb_to_modify_item_ng_ran_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {71, 72, 122, 123, 120, 140, 176, 184};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e drb_to_modify_item_ng_ran_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 71:
      return crit_e::reject;
    case 72:
      return crit_e::ignore;
    case 122:
      return crit_e::reject;
    case 123:
      return crit_e::reject;
    case 120:
      return crit_e::ignore;
    case 140:
      return crit_e::ignore;
    case 176:
      return crit_e::reject;
    case 184:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e drb_to_modify_item_ng_ran_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 71:
      return presence_e::optional;
    case 72:
      return presence_e::optional;
    case 122:
      return presence_e::optional;
    case 123:
      return presence_e::optional;
    case 120:
      return presence_e::optional;
    case 140:
      return presence_e::optional;
    case 176:
      return presence_e::optional;
    case 184:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE drb_to_modify_item_ng_ran_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += old_qos_flow_map_ul_endmarkerexpected_present ? 1 : 0;
  nof_ies += drb_qos_present ? 1 : 0;
  nof_ies += early_forwarding_count_req_present ? 1 : 0;
  nof_ies += early_forwarding_count_info_present ? 1 : 0;
  nof_ies += daps_request_info_present ? 1 : 0;
  nof_ies += early_data_forwarding_ind_present ? 1 : 0;
  nof_ies += sd_tind_mod_present ? 1 : 0;
  nof_ies += pdcp_count_reset_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (old_qos_flow_map_ul_endmarkerexpected_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)71, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, old_qos_flow_map_ul_endmarkerexpected, 1, 64, true));
  }
  if (drb_qos_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)72, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(drb_qos.pack(bref));
  }
  if (early_forwarding_count_req_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)122, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(early_forwarding_count_req.pack(bref));
  }
  if (early_forwarding_count_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)123, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(early_forwarding_count_info.pack(bref));
  }
  if (daps_request_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)120, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(daps_request_info.pack(bref));
  }
  if (early_data_forwarding_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)140, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(early_data_forwarding_ind.pack(bref));
  }
  if (sd_tind_mod_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)176, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sd_tind_mod.pack(bref));
  }
  if (pdcp_count_reset_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)184, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdcp_count_reset.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_modify_item_ng_ran_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != drb_to_modify_item_ng_ran_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 71: {
        old_qos_flow_map_ul_endmarkerexpected_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(old_qos_flow_map_ul_endmarkerexpected, bref, 1, 64, true));
        break;
      }
      case 72: {
        drb_qos_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(drb_qos.unpack(bref));
        break;
      }
      case 122: {
        early_forwarding_count_req_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(early_forwarding_count_req.unpack(bref));
        break;
      }
      case 123: {
        early_forwarding_count_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(early_forwarding_count_info.unpack(bref));
        break;
      }
      case 120: {
        daps_request_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(daps_request_info.unpack(bref));
        break;
      }
      case 140: {
        early_data_forwarding_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(early_data_forwarding_ind.unpack(bref));
        break;
      }
      case 176: {
        sd_tind_mod_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sd_tind_mod.unpack(bref));
        break;
      }
      case 184: {
        pdcp_count_reset_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdcp_count_reset.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void drb_to_modify_item_ng_ran_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (old_qos_flow_map_ul_endmarkerexpected_present) {
    j.start_obj();
    j.write_int("id", 71);
    j.write_str("criticality", "reject");
    j.start_array("extensionValue");
    for (const auto& e1 : old_qos_flow_map_ul_endmarkerexpected) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (drb_qos_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    drb_qos.to_json(j);
    j.end_obj();
  }
  if (early_forwarding_count_req_present) {
    j.start_obj();
    j.write_int("id", 122);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", early_forwarding_count_req.to_string());
    j.end_obj();
  }
  if (early_forwarding_count_info_present) {
    j.start_obj();
    j.write_int("id", 123);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    early_forwarding_count_info.to_json(j);
    j.end_obj();
  }
  if (daps_request_info_present) {
    j.start_obj();
    j.write_int("id", 120);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    daps_request_info.to_json(j);
    j.end_obj();
  }
  if (early_data_forwarding_ind_present) {
    j.start_obj();
    j.write_int("id", 140);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "stop");
    j.end_obj();
  }
  if (sd_tind_mod_present) {
    j.start_obj();
    j.write_int("id", 176);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", sd_tind_mod.to_string());
    j.end_obj();
  }
  if (pdcp_count_reset_present) {
    j.start_obj();
    j.write_int("id", 184);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  j.end_array();
}

// DRB-To-Modify-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_to_modify_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(sdap_cfg_present, 1));
  HANDLE_CODE(bref.pack(pdcp_cfg_present, 1));
  HANDLE_CODE(bref.pack(drb_data_forwarding_info_present, 1));
  HANDLE_CODE(bref.pack(pdcp_sn_status_request_present, 1));
  HANDLE_CODE(bref.pack(pdcp_sn_status_info_present, 1));
  HANDLE_CODE(bref.pack(dl_up_params.size() > 0, 1));
  HANDLE_CODE(bref.pack(cell_group_to_add.size() > 0, 1));
  HANDLE_CODE(bref.pack(cell_group_to_modify.size() > 0, 1));
  HANDLE_CODE(bref.pack(cell_group_to_rem.size() > 0, 1));
  HANDLE_CODE(bref.pack(flow_map_info.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (sdap_cfg_present) {
    HANDLE_CODE(sdap_cfg.pack(bref));
  }
  if (pdcp_cfg_present) {
    HANDLE_CODE(pdcp_cfg.pack(bref));
  }
  if (drb_data_forwarding_info_present) {
    HANDLE_CODE(drb_data_forwarding_info.pack(bref));
  }
  if (pdcp_sn_status_request_present) {
    HANDLE_CODE(pdcp_sn_status_request.pack(bref));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.pack(bref));
  }
  if (dl_up_params.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, dl_up_params, 1, 8, true));
  }
  if (cell_group_to_add.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_to_add, 1, 4, true));
  }
  if (cell_group_to_modify.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_to_modify, 1, 4, true));
  }
  if (cell_group_to_rem.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_to_rem, 1, 4, true));
  }
  if (flow_map_info.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, flow_map_info, 1, 64, true));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, drb_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_modify_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(sdap_cfg_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_cfg_present, 1));
  HANDLE_CODE(bref.unpack(drb_data_forwarding_info_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_sn_status_request_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_sn_status_info_present, 1));
  bool dl_up_params_present;
  HANDLE_CODE(bref.unpack(dl_up_params_present, 1));
  bool cell_group_to_add_present;
  HANDLE_CODE(bref.unpack(cell_group_to_add_present, 1));
  bool cell_group_to_modify_present;
  HANDLE_CODE(bref.unpack(cell_group_to_modify_present, 1));
  bool cell_group_to_rem_present;
  HANDLE_CODE(bref.unpack(cell_group_to_rem_present, 1));
  bool flow_map_info_present;
  HANDLE_CODE(bref.unpack(flow_map_info_present, 1));
  HANDLE_CODE(bref.unpack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (sdap_cfg_present) {
    HANDLE_CODE(sdap_cfg.unpack(bref));
  }
  if (pdcp_cfg_present) {
    HANDLE_CODE(pdcp_cfg.unpack(bref));
  }
  if (drb_data_forwarding_info_present) {
    HANDLE_CODE(drb_data_forwarding_info.unpack(bref));
  }
  if (pdcp_sn_status_request_present) {
    HANDLE_CODE(pdcp_sn_status_request.unpack(bref));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.unpack(bref));
  }
  if (dl_up_params_present) {
    HANDLE_CODE(unpack_dyn_seq_of(dl_up_params, bref, 1, 8, true));
  }
  if (cell_group_to_add_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cell_group_to_add, bref, 1, 4, true));
  }
  if (cell_group_to_modify_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cell_group_to_modify, bref, 1, 4, true));
  }
  if (cell_group_to_rem_present) {
    HANDLE_CODE(unpack_dyn_seq_of(cell_group_to_rem, bref, 1, 4, true));
  }
  if (flow_map_info_present) {
    HANDLE_CODE(unpack_dyn_seq_of(flow_map_info, bref, 1, 64, true));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(drb_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
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
void drb_to_modify_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (sdap_cfg_present) {
    j.write_fieldname("sDAP-Configuration");
    sdap_cfg.to_json(j);
  }
  if (pdcp_cfg_present) {
    j.write_fieldname("pDCP-Configuration");
    pdcp_cfg.to_json(j);
  }
  if (drb_data_forwarding_info_present) {
    j.write_fieldname("dRB-Data-Forwarding-Information");
    drb_data_forwarding_info.to_json(j);
  }
  if (pdcp_sn_status_request_present) {
    j.write_str("pDCP-SN-Status-Request", "requested");
  }
  if (pdcp_sn_status_info_present) {
    j.write_fieldname("pdcp-SN-Status-Information");
    pdcp_sn_status_info.to_json(j);
  }
  if (dl_up_params.size() > 0) {
    j.start_array("dL-UP-Parameters");
    for (const auto& e1 : dl_up_params) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cell_group_to_add.size() > 0) {
    j.start_array("cell-Group-To-Add");
    for (const auto& e1 : cell_group_to_add) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cell_group_to_modify.size() > 0) {
    j.start_array("cell-Group-To-Modify");
    for (const auto& e1 : cell_group_to_modify) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (cell_group_to_rem.size() > 0) {
    j.start_array("cell-Group-To-Remove");
    for (const auto& e1 : cell_group_to_rem) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (flow_map_info.size() > 0) {
    j.start_array("flow-Mapping-Information");
    for (const auto& e1 : flow_map_info) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_inactivity_timer_present) {
    j.write_int("dRB-Inactivity-Timer", drb_inactivity_timer);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-To-Remove-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_to_rem_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_rem_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_to_rem_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-To-Remove-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_to_rem_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_rem_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_to_rem_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Data-Forwarding-Request ::= ENUMERATED
const char* data_forwarding_request_opts::to_string() const
{
  static const char* names[] = {"uL", "dL", "both"};
  return convert_enum_idx(names, 3, value, "data_forwarding_request_e");
}

// QoS-Flow-Mapping-Item ::= SEQUENCE
OCUDUASN_CODE qos_flow_map_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qos_flow_map_ind_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (qos_flow_map_ind_present) {
    HANDLE_CODE(qos_flow_map_ind.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flow_map_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(qos_flow_map_ind_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (qos_flow_map_ind_present) {
    HANDLE_CODE(qos_flow_map_ind.unpack(bref));
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
void qos_flow_map_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoS-Flow-Identifier", qos_flow_id);
  if (qos_flow_map_ind_present) {
    j.write_str("qoSFlowMappingIndication", qos_flow_map_ind.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Data-Forwarding-Information-Request ::= SEQUENCE
OCUDUASN_CODE data_forwarding_info_request_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qos_flows_forwarded_on_fwd_tunnels.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(data_forwarding_request.pack(bref));
  if (qos_flows_forwarded_on_fwd_tunnels.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qos_flows_forwarded_on_fwd_tunnels, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_forwarding_info_request_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool qos_flows_forwarded_on_fwd_tunnels_present;
  HANDLE_CODE(bref.unpack(qos_flows_forwarded_on_fwd_tunnels_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(data_forwarding_request.unpack(bref));
  if (qos_flows_forwarded_on_fwd_tunnels_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qos_flows_forwarded_on_fwd_tunnels, bref, 1, 64, true));
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
void data_forwarding_info_request_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("data-Forwarding-Request", data_forwarding_request.to_string());
  if (qos_flows_forwarded_on_fwd_tunnels.size() > 0) {
    j.start_array("qoS-Flows-Forwarded-On-Fwd-Tunnels");
    for (const auto& e1 : qos_flows_forwarded_on_fwd_tunnels) {
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

// IntegrityProtectionIndication ::= ENUMERATED
const char* integrity_protection_ind_opts::to_string() const
{
  static const char* names[] = {"required", "preferred", "not-needed"};
  return convert_enum_idx(names, 3, value, "integrity_protection_ind_e");
}

// MaxIPrate ::= ENUMERATED
const char* max_ip_rate_opts::to_string() const
{
  static const char* names[] = {"bitrate64kbs", "max-UErate"};
  return convert_enum_idx(names, 2, value, "max_ip_rate_e");
}
uint8_t max_ip_rate_opts::to_number() const
{
  static const uint8_t numbers[] = {64};
  return map_enum_number(numbers, 1, value, "max_ip_rate_e");
}

// MaximumIPdatarate ::= SEQUENCE
OCUDUASN_CODE max_ip_datarate_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(max_ip_rate.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE max_ip_datarate_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(max_ip_rate.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void max_ip_datarate_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("maxIPrate", max_ip_rate.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SecurityIndication ::= SEQUENCE
OCUDUASN_CODE security_ind_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(max_ip_datarate_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(integrity_protection_ind.pack(bref));
  HANDLE_CODE(confidentiality_protection_ind.pack(bref));
  if (max_ip_datarate_present) {
    HANDLE_CODE(max_ip_datarate.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE security_ind_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(max_ip_datarate_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(integrity_protection_ind.unpack(bref));
  HANDLE_CODE(confidentiality_protection_ind.unpack(bref));
  if (max_ip_datarate_present) {
    HANDLE_CODE(max_ip_datarate.unpack(bref));
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
void security_ind_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("integrityProtectionIndication", integrity_protection_ind.to_string());
  j.write_str("confidentialityProtectionIndication", confidentiality_protection_ind.to_string());
  if (max_ip_datarate_present) {
    j.write_fieldname("maximumIPdatarate");
    max_ip_datarate.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-To-Setup-Item-EUTRAN-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t drb_to_setup_item_eutran_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {142, 172};
  return map_enum_number(names, 2, idx, "id");
}
bool drb_to_setup_item_eutran_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {142, 172};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e drb_to_setup_item_eutran_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 142:
      return crit_e::ignore;
    case 172:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e drb_to_setup_item_eutran_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 142:
      return presence_e::optional;
    case 172:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE drb_to_setup_item_eutran_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += data_forwarding_source_ip_address_present ? 1 : 0;
  nof_ies += security_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (data_forwarding_source_ip_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)142, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(data_forwarding_source_ip_address.pack(bref));
  }
  if (security_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)172, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(security_ind.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_setup_item_eutran_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != drb_to_setup_item_eutran_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 142: {
        data_forwarding_source_ip_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(data_forwarding_source_ip_address.unpack(bref));
        break;
      }
      case 172: {
        security_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(security_ind.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void drb_to_setup_item_eutran_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (data_forwarding_source_ip_address_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    data_forwarding_source_ip_address.to_json(j);
    j.end_obj();
  }
  if (security_ind_present) {
    j.start_obj();
    j.write_int("id", 172);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    security_ind.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DRB-To-Setup-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_to_setup_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.pack(dl_up_params.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(existing_allocated_s1_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pdcp_cfg.pack(bref));
  HANDLE_CODE(eutran_qos.pack(bref));
  HANDLE_CODE(s1_ul_up_tnl_info.pack(bref));
  if (data_forwarding_info_request_present) {
    HANDLE_CODE(data_forwarding_info_request.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_info, 1, 4, true));
  if (dl_up_params.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, dl_up_params, 1, 8, true));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, drb_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (existing_allocated_s1_dl_up_tnl_info_present) {
    HANDLE_CODE(existing_allocated_s1_dl_up_tnl_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_setup_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(data_forwarding_info_request_present, 1));
  bool dl_up_params_present;
  HANDLE_CODE(bref.unpack(dl_up_params_present, 1));
  HANDLE_CODE(bref.unpack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(existing_allocated_s1_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pdcp_cfg.unpack(bref));
  HANDLE_CODE(eutran_qos.unpack(bref));
  HANDLE_CODE(s1_ul_up_tnl_info.unpack(bref));
  if (data_forwarding_info_request_present) {
    HANDLE_CODE(data_forwarding_info_request.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(cell_group_info, bref, 1, 4, true));
  if (dl_up_params_present) {
    HANDLE_CODE(unpack_dyn_seq_of(dl_up_params, bref, 1, 8, true));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(drb_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (existing_allocated_s1_dl_up_tnl_info_present) {
    HANDLE_CODE(existing_allocated_s1_dl_up_tnl_info.unpack(bref));
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
void drb_to_setup_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("pDCP-Configuration");
  pdcp_cfg.to_json(j);
  j.write_fieldname("eUTRAN-QoS");
  eutran_qos.to_json(j);
  j.write_fieldname("s1-UL-UP-TNL-Information");
  s1_ul_up_tnl_info.to_json(j);
  if (data_forwarding_info_request_present) {
    j.write_fieldname("data-Forwarding-Information-Request");
    data_forwarding_info_request.to_json(j);
  }
  j.start_array("cell-Group-Information");
  for (const auto& e1 : cell_group_info) {
    e1.to_json(j);
  }
  j.end_array();
  if (dl_up_params.size() > 0) {
    j.start_array("dL-UP-Parameters");
    for (const auto& e1 : dl_up_params) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_inactivity_timer_present) {
    j.write_int("dRB-Inactivity-Timer", drb_inactivity_timer);
  }
  if (existing_allocated_s1_dl_up_tnl_info_present) {
    j.write_fieldname("existing-Allocated-S1-DL-UP-TNL-Info");
    existing_allocated_s1_dl_up_tnl_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// IgnoreMappingRuleIndication ::= ENUMERATED
const char* ignore_map_rule_ind_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "ignore_map_rule_ind_e");
}

// QoS-Flows-DRB-Remapping ::= ENUMERATED
const char* qos_flows_drb_remap_opts::to_string() const
{
  static const char* names[] = {"update", "source-configuration"};
  return convert_enum_idx(names, 2, value, "qos_flows_drb_remap_e");
}

// SDTindicatorSetup ::= ENUMERATED
const char* sd_tind_setup_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "sd_tind_setup_e");
}

// SpecialTriggeringPurpose ::= ENUMERATED
const char* special_trigger_purpose_opts::to_string() const
{
  static const char* names[] = {"indirect-data-forwarding"};
  return convert_enum_idx(names, 1, value, "special_trigger_purpose_e");
}

// DRB-To-Setup-Item-NG-RAN-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t drb_to_setup_item_ng_ran_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {72, 120, 138, 141, 175, 198};
  return map_enum_number(names, 6, idx, "id");
}
bool drb_to_setup_item_ng_ran_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {72, 120, 138, 141, 175, 198};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e drb_to_setup_item_ng_ran_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 72:
      return crit_e::ignore;
    case 120:
      return crit_e::ignore;
    case 138:
      return crit_e::reject;
    case 141:
      return crit_e::reject;
    case 175:
      return crit_e::reject;
    case 198:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e drb_to_setup_item_ng_ran_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 72:
      return presence_e::optional;
    case 120:
      return presence_e::optional;
    case 138:
      return presence_e::optional;
    case 141:
      return presence_e::optional;
    case 175:
      return presence_e::optional;
    case 198:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE drb_to_setup_item_ng_ran_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += drb_qos_present ? 1 : 0;
  nof_ies += daps_request_info_present ? 1 : 0;
  nof_ies += ignore_map_rule_ind_present ? 1 : 0;
  nof_ies += qos_flows_drb_remap_present ? 1 : 0;
  nof_ies += sd_tind_setup_present ? 1 : 0;
  nof_ies += special_trigger_purpose_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (drb_qos_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)72, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(drb_qos.pack(bref));
  }
  if (daps_request_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)120, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(daps_request_info.pack(bref));
  }
  if (ignore_map_rule_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)138, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ignore_map_rule_ind.pack(bref));
  }
  if (qos_flows_drb_remap_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)141, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(qos_flows_drb_remap.pack(bref));
  }
  if (sd_tind_setup_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)175, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sd_tind_setup.pack(bref));
  }
  if (special_trigger_purpose_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)198, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(special_trigger_purpose.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_setup_item_ng_ran_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != drb_to_setup_item_ng_ran_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 72: {
        drb_qos_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(drb_qos.unpack(bref));
        break;
      }
      case 120: {
        daps_request_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(daps_request_info.unpack(bref));
        break;
      }
      case 138: {
        ignore_map_rule_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ignore_map_rule_ind.unpack(bref));
        break;
      }
      case 141: {
        qos_flows_drb_remap_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(qos_flows_drb_remap.unpack(bref));
        break;
      }
      case 175: {
        sd_tind_setup_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sd_tind_setup.unpack(bref));
        break;
      }
      case 198: {
        special_trigger_purpose_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(special_trigger_purpose.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void drb_to_setup_item_ng_ran_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (drb_qos_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    drb_qos.to_json(j);
    j.end_obj();
  }
  if (daps_request_info_present) {
    j.start_obj();
    j.write_int("id", 120);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    daps_request_info.to_json(j);
    j.end_obj();
  }
  if (ignore_map_rule_ind_present) {
    j.start_obj();
    j.write_int("id", 138);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  if (qos_flows_drb_remap_present) {
    j.start_obj();
    j.write_int("id", 141);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", qos_flows_drb_remap.to_string());
    j.end_obj();
  }
  if (sd_tind_setup_present) {
    j.start_obj();
    j.write_int("id", 175);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  if (special_trigger_purpose_present) {
    j.start_obj();
    j.write_int("id", 198);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "indirect-data-forwarding");
    j.end_obj();
  }
  j.end_array();
}

// DRB-To-Setup-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_to_setup_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(drb_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.pack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(pdcp_sn_status_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(sdap_cfg.pack(bref));
  HANDLE_CODE(pdcp_cfg.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_info, 1, 4, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, qos_flow_info_to_be_setup, 1, 64, true));
  if (drb_data_forwarding_info_request_present) {
    HANDLE_CODE(drb_data_forwarding_info_request.pack(bref));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, drb_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_setup_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(drb_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.unpack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_sn_status_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(sdap_cfg.unpack(bref));
  HANDLE_CODE(pdcp_cfg.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(cell_group_info, bref, 1, 4, true));
  HANDLE_CODE(unpack_dyn_seq_of(qos_flow_info_to_be_setup, bref, 1, 64, true));
  if (drb_data_forwarding_info_request_present) {
    HANDLE_CODE(drb_data_forwarding_info_request.unpack(bref));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(drb_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.unpack(bref));
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
void drb_to_setup_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("sDAP-Configuration");
  sdap_cfg.to_json(j);
  j.write_fieldname("pDCP-Configuration");
  pdcp_cfg.to_json(j);
  j.start_array("cell-Group-Information");
  for (const auto& e1 : cell_group_info) {
    e1.to_json(j);
  }
  j.end_array();
  j.start_array("qos-flow-Information-To-Be-Setup");
  for (const auto& e1 : qos_flow_info_to_be_setup) {
    e1.to_json(j);
  }
  j.end_array();
  if (drb_data_forwarding_info_request_present) {
    j.write_fieldname("dRB-Data-Forwarding-Information-Request");
    drb_data_forwarding_info_request.to_json(j);
  }
  if (drb_inactivity_timer_present) {
    j.write_int("dRB-Inactivity-Timer", drb_inactivity_timer);
  }
  if (pdcp_sn_status_info_present) {
    j.write_fieldname("pDCP-SN-Status-Information");
    pdcp_sn_status_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-To-Setup-Mod-Item-EUTRAN-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t drb_to_setup_mod_item_eutran_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {172, 142};
  return map_enum_number(names, 2, idx, "id");
}
bool drb_to_setup_mod_item_eutran_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {172, 142};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e drb_to_setup_mod_item_eutran_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 172:
      return crit_e::reject;
    case 142:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e drb_to_setup_mod_item_eutran_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 172:
      return presence_e::optional;
    case 142:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE drb_to_setup_mod_item_eutran_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += security_ind_present ? 1 : 0;
  nof_ies += data_forwarding_source_ip_address_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (security_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)172, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(security_ind.pack(bref));
  }
  if (data_forwarding_source_ip_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)142, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(data_forwarding_source_ip_address.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_setup_mod_item_eutran_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != drb_to_setup_mod_item_eutran_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 172: {
        security_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(security_ind.unpack(bref));
        break;
      }
      case 142: {
        data_forwarding_source_ip_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(data_forwarding_source_ip_address.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void drb_to_setup_mod_item_eutran_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (security_ind_present) {
    j.start_obj();
    j.write_int("id", 172);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    security_ind.to_json(j);
    j.end_obj();
  }
  if (data_forwarding_source_ip_address_present) {
    j.start_obj();
    j.write_int("id", 142);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    data_forwarding_source_ip_address.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DRB-To-Setup-Mod-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drb_to_setup_mod_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.pack(dl_up_params.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pdcp_cfg.pack(bref));
  HANDLE_CODE(eutran_qos.pack(bref));
  HANDLE_CODE(s1_ul_up_tnl_info.pack(bref));
  if (data_forwarding_info_request_present) {
    HANDLE_CODE(data_forwarding_info_request.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_info, 1, 4, true));
  if (dl_up_params.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, dl_up_params, 1, 8, true));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, drb_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_setup_mod_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(data_forwarding_info_request_present, 1));
  bool dl_up_params_present;
  HANDLE_CODE(bref.unpack(dl_up_params_present, 1));
  HANDLE_CODE(bref.unpack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pdcp_cfg.unpack(bref));
  HANDLE_CODE(eutran_qos.unpack(bref));
  HANDLE_CODE(s1_ul_up_tnl_info.unpack(bref));
  if (data_forwarding_info_request_present) {
    HANDLE_CODE(data_forwarding_info_request.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(cell_group_info, bref, 1, 4, true));
  if (dl_up_params_present) {
    HANDLE_CODE(unpack_dyn_seq_of(dl_up_params, bref, 1, 8, true));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(drb_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
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
void drb_to_setup_mod_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("pDCP-Configuration");
  pdcp_cfg.to_json(j);
  j.write_fieldname("eUTRAN-QoS");
  eutran_qos.to_json(j);
  j.write_fieldname("s1-UL-UP-TNL-Information");
  s1_ul_up_tnl_info.to_json(j);
  if (data_forwarding_info_request_present) {
    j.write_fieldname("data-Forwarding-Information-Request");
    data_forwarding_info_request.to_json(j);
  }
  j.start_array("cell-Group-Information");
  for (const auto& e1 : cell_group_info) {
    e1.to_json(j);
  }
  j.end_array();
  if (dl_up_params.size() > 0) {
    j.start_array("dL-UP-Parameters");
    for (const auto& e1 : dl_up_params) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_inactivity_timer_present) {
    j.write_int("dRB-Inactivity-Timer", drb_inactivity_timer);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-To-Setup-Mod-Item-NG-RAN-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t drb_to_setup_mod_item_ng_ran_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {72, 138, 120, 175, 198};
  return map_enum_number(names, 5, idx, "id");
}
bool drb_to_setup_mod_item_ng_ran_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {72, 138, 120, 175, 198};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e drb_to_setup_mod_item_ng_ran_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 72:
      return crit_e::ignore;
    case 138:
      return crit_e::reject;
    case 120:
      return crit_e::ignore;
    case 175:
      return crit_e::reject;
    case 198:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e drb_to_setup_mod_item_ng_ran_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 72:
      return presence_e::optional;
    case 138:
      return presence_e::optional;
    case 120:
      return presence_e::optional;
    case 175:
      return presence_e::optional;
    case 198:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE drb_to_setup_mod_item_ng_ran_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += drb_qos_present ? 1 : 0;
  nof_ies += ignore_map_rule_ind_present ? 1 : 0;
  nof_ies += daps_request_info_present ? 1 : 0;
  nof_ies += sd_tind_setup_present ? 1 : 0;
  nof_ies += special_trigger_purpose_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (drb_qos_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)72, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(drb_qos.pack(bref));
  }
  if (ignore_map_rule_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)138, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ignore_map_rule_ind.pack(bref));
  }
  if (daps_request_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)120, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(daps_request_info.pack(bref));
  }
  if (sd_tind_setup_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)175, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sd_tind_setup.pack(bref));
  }
  if (special_trigger_purpose_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)198, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(special_trigger_purpose.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_setup_mod_item_ng_ran_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != drb_to_setup_mod_item_ng_ran_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 72: {
        drb_qos_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(drb_qos.unpack(bref));
        break;
      }
      case 138: {
        ignore_map_rule_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ignore_map_rule_ind.unpack(bref));
        break;
      }
      case 120: {
        daps_request_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(daps_request_info.unpack(bref));
        break;
      }
      case 175: {
        sd_tind_setup_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sd_tind_setup.unpack(bref));
        break;
      }
      case 198: {
        special_trigger_purpose_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(special_trigger_purpose.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void drb_to_setup_mod_item_ng_ran_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (drb_qos_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    drb_qos.to_json(j);
    j.end_obj();
  }
  if (ignore_map_rule_ind_present) {
    j.start_obj();
    j.write_int("id", 138);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  if (daps_request_info_present) {
    j.start_obj();
    j.write_int("id", 120);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    daps_request_info.to_json(j);
    j.end_obj();
  }
  if (sd_tind_setup_present) {
    j.start_obj();
    j.write_int("id", 175);
    j.write_str("criticality", "reject");
    j.write_str("extensionValue", "true");
    j.end_obj();
  }
  if (special_trigger_purpose_present) {
    j.start_obj();
    j.write_int("id", 198);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "indirect-data-forwarding");
    j.end_obj();
  }
  j.end_array();
}

// DRB-To-Setup-Mod-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drb_to_setup_mod_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(drb_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.pack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(pdcp_sn_status_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(sdap_cfg.pack(bref));
  HANDLE_CODE(pdcp_cfg.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, cell_group_info, 1, 4, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, flow_map_info, 1, 64, true));
  if (drb_data_forwarding_info_request_present) {
    HANDLE_CODE(drb_data_forwarding_info_request.pack(bref));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, drb_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_to_setup_mod_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(drb_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.unpack(drb_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(pdcp_sn_status_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(sdap_cfg.unpack(bref));
  HANDLE_CODE(pdcp_cfg.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(cell_group_info, bref, 1, 4, true));
  HANDLE_CODE(unpack_dyn_seq_of(flow_map_info, bref, 1, 64, true));
  if (drb_data_forwarding_info_request_present) {
    HANDLE_CODE(drb_data_forwarding_info_request.unpack(bref));
  }
  if (drb_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(drb_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (pdcp_sn_status_info_present) {
    HANDLE_CODE(pdcp_sn_status_info.unpack(bref));
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
void drb_to_setup_mod_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("sDAP-Configuration");
  sdap_cfg.to_json(j);
  j.write_fieldname("pDCP-Configuration");
  pdcp_cfg.to_json(j);
  j.start_array("cell-Group-Information");
  for (const auto& e1 : cell_group_info) {
    e1.to_json(j);
  }
  j.end_array();
  j.start_array("flow-Mapping-Information");
  for (const auto& e1 : flow_map_info) {
    e1.to_json(j);
  }
  j.end_array();
  if (drb_data_forwarding_info_request_present) {
    j.write_fieldname("dRB-Data-Forwarding-Information-Request");
    drb_data_forwarding_info_request.to_json(j);
  }
  if (drb_inactivity_timer_present) {
    j.write_int("dRB-Inactivity-Timer", drb_inactivity_timer);
  }
  if (pdcp_sn_status_info_present) {
    j.write_fieldname("pDCP-SN-Status-Information");
    pdcp_sn_status_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRB-Usage-Report-Item ::= SEQUENCE
OCUDUASN_CODE drb_usage_report_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(start_time_stamp.pack(bref));
  HANDLE_CODE(end_time_stamp.pack(bref));
  HANDLE_CODE(pack_integer(bref, usage_count_ul, (uint64_t)0u, (uint64_t)18446744073709551615u, false, true));
  HANDLE_CODE(pack_integer(bref, usage_count_dl, (uint64_t)0u, (uint64_t)18446744073709551615u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drb_usage_report_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(start_time_stamp.unpack(bref));
  HANDLE_CODE(end_time_stamp.unpack(bref));
  HANDLE_CODE(unpack_integer(usage_count_ul, bref, (uint64_t)0u, (uint64_t)18446744073709551615u, false, true));
  HANDLE_CODE(unpack_integer(usage_count_dl, bref, (uint64_t)0u, (uint64_t)18446744073709551615u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drb_usage_report_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("startTimeStamp", start_time_stamp.to_string());
  j.write_str("endTimeStamp", end_time_stamp.to_string());
  j.write_uint("usageCountUL", usage_count_ul);
  j.write_uint("usageCountDL", usage_count_dl);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRBs-Subject-To-Counter-Check-Item-EUTRAN ::= SEQUENCE
OCUDUASN_CODE drbs_subject_to_counter_check_item_eutran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pdcp_ul_count.pack(bref));
  HANDLE_CODE(pdcp_dl_count.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drbs_subject_to_counter_check_item_eutran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pdcp_ul_count.unpack(bref));
  HANDLE_CODE(pdcp_dl_count.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drbs_subject_to_counter_check_item_eutran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("pDCP-UL-Count");
  pdcp_ul_count.to_json(j);
  j.write_fieldname("pDCP-DL-Count");
  pdcp_dl_count.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRBs-Subject-To-Counter-Check-Item-NG-RAN ::= SEQUENCE
OCUDUASN_CODE drbs_subject_to_counter_check_item_ng_ran_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pdcp_ul_count.pack(bref));
  HANDLE_CODE(pdcp_dl_count.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drbs_subject_to_counter_check_item_ng_ran_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(pdcp_ul_count.unpack(bref));
  HANDLE_CODE(pdcp_dl_count.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drbs_subject_to_counter_check_item_ng_ran_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("pDCP-UL-Count");
  pdcp_ul_count.to_json(j);
  j.write_fieldname("pDCP-DL-Count");
  pdcp_dl_count.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DRBs-Subject-To-Early-Forwarding-Item ::= SEQUENCE
OCUDUASN_CODE drbs_subject_to_early_forwarding_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(dl_count_value.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drbs_subject_to_early_forwarding_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(dl_count_value.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void drbs_subject_to_early_forwarding_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_fieldname("dLCountValue");
  dl_count_value.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Data-Usage-Report-Item ::= SEQUENCE
OCUDUASN_CODE data_usage_report_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, drb_id, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(rat_type.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, drb_usage_report_list, 1, 2, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_usage_report_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(drb_id, bref, (uint8_t)1u, (uint8_t)32u, true, true));
  HANDLE_CODE(rat_type.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(drb_usage_report_list, bref, 1, 2, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void data_usage_report_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dRB-ID", drb_id);
  j.write_str("rAT-Type", rat_type.to_string());
  j.start_array("dRB-Usage-Report-List");
  for (const auto& e1 : drb_usage_report_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MRDC-Data-Usage-Report-Item ::= SEQUENCE
OCUDUASN_CODE mrdc_data_usage_report_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(start_time_stamp.pack(bref));
  HANDLE_CODE(end_time_stamp.pack(bref));
  HANDLE_CODE(pack_integer(bref, usage_count_ul, (uint64_t)0u, (uint64_t)18446744073709551615u, false, true));
  HANDLE_CODE(pack_integer(bref, usage_count_dl, (uint64_t)0u, (uint64_t)18446744073709551615u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrdc_data_usage_report_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(start_time_stamp.unpack(bref));
  HANDLE_CODE(end_time_stamp.unpack(bref));
  HANDLE_CODE(unpack_integer(usage_count_ul, bref, (uint64_t)0u, (uint64_t)18446744073709551615u, false, true));
  HANDLE_CODE(unpack_integer(usage_count_dl, bref, (uint64_t)0u, (uint64_t)18446744073709551615u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mrdc_data_usage_report_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("startTimeStamp", start_time_stamp.to_string());
  j.write_str("endTimeStamp", end_time_stamp.to_string());
  j.write_uint("usageCountUL", usage_count_ul);
  j.write_uint("usageCountDL", usage_count_dl);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Data-Usage-per-PDU-Session-Report ::= SEQUENCE
OCUDUASN_CODE data_usage_per_pdu_session_report_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(secondary_rat_type.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, pdu_session_timed_report_list, 1, 2, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_usage_per_pdu_session_report_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(secondary_rat_type.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(pdu_session_timed_report_list, bref, 1, 2, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void data_usage_per_pdu_session_report_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("secondaryRATType", secondary_rat_type.to_string());
  j.start_array("pDU-session-Timed-Report-List");
  for (const auto& e1 : pdu_session_timed_report_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* data_usage_per_pdu_session_report_s::secondary_rat_type_opts::to_string() const
{
  static const char* names[] = {"nR", "e-UTRA"};
  return convert_enum_idx(names, 2, value, "data_usage_per_pdu_session_report_s::secondary_rat_type_e_");
}

// Data-Usage-per-QoS-Flow-Item ::= SEQUENCE
OCUDUASN_CODE data_usage_per_qos_flow_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(secondary_rat_type.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, qos_flow_timed_report_list, 1, 2, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_usage_per_qos_flow_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(secondary_rat_type.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(qos_flow_timed_report_list, bref, 1, 2, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void data_usage_per_qos_flow_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoS-Flow-Identifier", qos_flow_id);
  j.write_str("secondaryRATType", secondary_rat_type.to_string());
  j.start_array("qoS-Flow-Timed-Report-List");
  for (const auto& e1 : qos_flow_timed_report_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* data_usage_per_qos_flow_item_s::secondary_rat_type_opts::to_string() const
{
  static const char* names[] = {"nR", "e-UTRA"};
  return convert_enum_idx(names, 2, value, "data_usage_per_qos_flow_item_s::secondary_rat_type_e_");
}

// DataDiscardRequired ::= ENUMERATED
const char* data_discard_required_opts::to_string() const
{
  static const char* names[] = {"required"};
  return convert_enum_idx(names, 1, value, "data_discard_required_e");
}

// QoS-Flows-to-be-forwarded-Item ::= SEQUENCE
OCUDUASN_CODE qos_flows_to_be_forwarded_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_flows_to_be_forwarded_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void qos_flows_to_be_forwarded_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("qoS-Flow-Identifier", qos_flow_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DataForwardingtoE-UTRANInformationListItem ::= SEQUENCE
OCUDUASN_CODE data_forwardingto_e_utran_info_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(data_forwarding_tunnel_info.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, qos_flows_to_be_forwarded_list, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE data_forwardingto_e_utran_info_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(data_forwarding_tunnel_info.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(qos_flows_to_be_forwarded_list, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void data_forwardingto_e_utran_info_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("data-forwarding-tunnel-information");
  data_forwarding_tunnel_info.to_json(j);
  j.start_array("qoS-Flows-to-be-forwarded-List");
  for (const auto& e1 : qos_flows_to_be_forwarded_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// DirectForwardingPathAvailability ::= ENUMERATED
const char* direct_forwarding_path_availability_opts::to_string() const
{
  static const char* names[] = {"inter-system-direct-path-available", "intra-system-direct-path-available"};
  return convert_enum_idx(names, 2, value, "direct_forwarding_path_availability_e");
}

// ECGI ::= SEQUENCE
OCUDUASN_CODE ecgi_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(eutran_cell_id.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ecgi_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(eutran_cell_id.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ecgi_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  j.write_fieldname("eUTRAN-Cell-Identity");
  eutran_cell_id.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ECGI-Support-Item ::= SEQUENCE
OCUDUASN_CODE ecgi_support_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ecgi.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ecgi_support_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ecgi.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ecgi_support_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("eCGI");
  ecgi.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// EUTRAN-QoS-Support-Item ::= SEQUENCE
OCUDUASN_CODE eutran_qos_support_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(eutran_qos.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE eutran_qos_support_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(eutran_qos.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void eutran_qos_support_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("eUTRAN-QoS");
  eutran_qos.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Extended-GNB-CU-CP-Name ::= SEQUENCE
OCUDUASN_CODE extended_gnb_cu_cp_name_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(gnb_cu_cp_name_visible_string_present, 1));
  HANDLE_CODE(bref.pack(gnb_cu_cp_name_utf8_string_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (gnb_cu_cp_name_visible_string_present) {
    HANDLE_CODE(gnb_cu_cp_name_visible_string.pack(bref));
  }
  if (gnb_cu_cp_name_utf8_string_present) {
    HANDLE_CODE(gnb_cu_cp_name_utf8_string.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE extended_gnb_cu_cp_name_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(gnb_cu_cp_name_visible_string_present, 1));
  HANDLE_CODE(bref.unpack(gnb_cu_cp_name_utf8_string_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (gnb_cu_cp_name_visible_string_present) {
    HANDLE_CODE(gnb_cu_cp_name_visible_string.unpack(bref));
  }
  if (gnb_cu_cp_name_utf8_string_present) {
    HANDLE_CODE(gnb_cu_cp_name_utf8_string.unpack(bref));
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
void extended_gnb_cu_cp_name_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (gnb_cu_cp_name_visible_string_present) {
    j.write_str("gNB-CU-CP-NameVisibleString", gnb_cu_cp_name_visible_string.to_string());
  }
  if (gnb_cu_cp_name_utf8_string_present) {
    j.write_str("gNB-CU-CP-NameUTF8String", gnb_cu_cp_name_utf8_string.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Extended-GNB-CU-UP-Name ::= SEQUENCE
OCUDUASN_CODE extended_gnb_cu_up_name_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(gnb_cu_up_name_visible_string_present, 1));
  HANDLE_CODE(bref.pack(gnb_cu_up_name_utf8_string_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (gnb_cu_up_name_visible_string_present) {
    HANDLE_CODE(gnb_cu_up_name_visible_string.pack(bref));
  }
  if (gnb_cu_up_name_utf8_string_present) {
    HANDLE_CODE(gnb_cu_up_name_utf8_string.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE extended_gnb_cu_up_name_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(gnb_cu_up_name_visible_string_present, 1));
  HANDLE_CODE(bref.unpack(gnb_cu_up_name_utf8_string_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (gnb_cu_up_name_visible_string_present) {
    HANDLE_CODE(gnb_cu_up_name_visible_string.unpack(bref));
  }
  if (gnb_cu_up_name_utf8_string_present) {
    HANDLE_CODE(gnb_cu_up_name_utf8_string.unpack(bref));
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
void extended_gnb_cu_up_name_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (gnb_cu_up_name_visible_string_present) {
    j.write_str("gNB-CU-UP-NameVisibleString", gnb_cu_up_name_visible_string.to_string());
  }
  if (gnb_cu_up_name_utf8_string_present) {
    j.write_str("gNB-CU-UP-NameUTF8String", gnb_cu_up_name_utf8_string.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NR-CGI ::= SEQUENCE
OCUDUASN_CODE nr_cgi_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(nr_cell_id.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE nr_cgi_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(nr_cell_id.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void nr_cgi_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  j.write_fieldname("nR-Cell-Identity");
  nr_cell_id.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Extended-NR-CGI-Support-Item ::= SEQUENCE
OCUDUASN_CODE extended_nr_cgi_support_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE extended_nr_cgi_support_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void extended_nr_cgi_support_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("nR-CGI");
  nr_cgi.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Slice-Support-Item ::= SEQUENCE
OCUDUASN_CODE slice_support_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(snssai.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE slice_support_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(snssai.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void slice_support_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sNSSAI");
  snssai.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GNB-CU-CP-TNLA-Failed-To-Setup-Item ::= SEQUENCE
OCUDUASN_CODE gnb_cu_cp_tnl_a_failed_to_setup_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.pack(bref));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_tnl_a_failed_to_setup_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.unpack(bref));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_tnl_a_failed_to_setup_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tNLAssociationTransportLayerAddress");
  tnl_assoc_transport_layer_address.to_json(j);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GNB-CU-CP-TNLA-Setup-Item ::= SEQUENCE
OCUDUASN_CODE gnb_cu_cp_tnl_a_setup_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_tnl_a_setup_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_tnl_a_setup_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tNLAssociationTransportLayerAddress");
  tnl_assoc_transport_layer_address.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TNLAssociationUsage ::= ENUMERATED
const char* tnl_assoc_usage_opts::to_string() const
{
  static const char* names[] = {"ue", "non-ue", "both"};
  return convert_enum_idx(names, 3, value, "tnl_assoc_usage_e");
}

// GNB-CU-CP-TNLA-To-Add-Item ::= SEQUENCE
OCUDUASN_CODE gnb_cu_cp_tnl_a_to_add_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.pack(bref));
  HANDLE_CODE(tnl_assoc_usage.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_tnl_a_to_add_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.unpack(bref));
  HANDLE_CODE(tnl_assoc_usage.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_tnl_a_to_add_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tNLAssociationTransportLayerAddress");
  tnl_assoc_transport_layer_address.to_json(j);
  j.write_str("tNLAssociationUsage", tnl_assoc_usage.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GNB-CU-CP-TNLA-To-Remove-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t gnb_cu_cp_tnl_a_to_rem_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {75};
  return map_enum_number(names, 1, idx, "id");
}
bool gnb_cu_cp_tnl_a_to_rem_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 75 == id;
}
crit_e gnb_cu_cp_tnl_a_to_rem_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 75) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e gnb_cu_cp_tnl_a_to_rem_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 75) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE gnb_cu_cp_tnl_a_to_rem_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += tnl_assoc_transport_layer_address_gnb_cu_up_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (tnl_assoc_transport_layer_address_gnb_cu_up_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)75, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(tnl_assoc_transport_layer_address_gnb_cu_up.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_tnl_a_to_rem_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_cu_cp_tnl_a_to_rem_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 75) {
      {
        tnl_assoc_transport_layer_address_gnb_cu_up_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(tnl_assoc_transport_layer_address_gnb_cu_up.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_tnl_a_to_rem_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (tnl_assoc_transport_layer_address_gnb_cu_up_present) {
    j.start_obj();
    j.write_int("id", 75);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    tnl_assoc_transport_layer_address_gnb_cu_up.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNB-CU-CP-TNLA-To-Remove-Item ::= SEQUENCE
OCUDUASN_CODE gnb_cu_cp_tnl_a_to_rem_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_tnl_a_to_rem_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_tnl_a_to_rem_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tNLAssociationTransportLayerAddress");
  tnl_assoc_transport_layer_address.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GNB-CU-CP-TNLA-To-Update-Item ::= SEQUENCE
OCUDUASN_CODE gnb_cu_cp_tnl_a_to_upd_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(tnl_assoc_usage_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.pack(bref));
  if (tnl_assoc_usage_present) {
    HANDLE_CODE(tnl_assoc_usage.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cp_tnl_a_to_upd_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(tnl_assoc_usage_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.unpack(bref));
  if (tnl_assoc_usage_present) {
    HANDLE_CODE(tnl_assoc_usage.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void gnb_cu_cp_tnl_a_to_upd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tNLAssociationTransportLayerAddress");
  tnl_assoc_transport_layer_address.to_json(j);
  if (tnl_assoc_usage_present) {
    j.write_str("tNLAssociationUsage", tnl_assoc_usage.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GlobalMBSSessionID ::= SEQUENCE
OCUDUASN_CODE global_mbs_session_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(nid_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(tmgi.pack(bref));
  if (nid_present) {
    HANDLE_CODE(nid.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_mbs_session_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(nid_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(tmgi.unpack(bref));
  if (nid_present) {
    HANDLE_CODE(nid.unpack(bref));
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
void global_mbs_session_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("tmgi", tmgi.to_string());
  if (nid_present) {
    j.write_fieldname("nid");
    nid.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MBS-Support-Info-ToAdd-Item ::= SEQUENCE
OCUDUASN_CODE mbs_support_info_to_add_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(global_mbs_session_id.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_support_info_to_add_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(global_mbs_session_id.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbs_support_info_to_add_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("globalMBSSessionID");
  global_mbs_session_id.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MBS-Support-Info-ToRemove-Item ::= SEQUENCE
OCUDUASN_CODE mbs_support_info_to_rem_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(global_mbs_session_id.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_support_info_to_rem_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(global_mbs_session_id.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbs_support_info_to_rem_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("globalMBSSessionID");
  global_mbs_session_id.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GNB-CU-UP-MBS-Support-Info ::= SEQUENCE
OCUDUASN_CODE gnb_cu_up_mbs_support_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_support_info_to_add_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(mbs_support_info_to_rem_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (mbs_support_info_to_add_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_support_info_to_add_list, 1, 512, true));
  }
  if (mbs_support_info_to_rem_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_support_info_to_rem_list, 1, 512, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_mbs_support_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool mbs_support_info_to_add_list_present;
  HANDLE_CODE(bref.unpack(mbs_support_info_to_add_list_present, 1));
  bool mbs_support_info_to_rem_list_present;
  HANDLE_CODE(bref.unpack(mbs_support_info_to_rem_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (mbs_support_info_to_add_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_support_info_to_add_list, bref, 1, 512, true));
  }
  if (mbs_support_info_to_rem_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_support_info_to_rem_list, bref, 1, 512, true));
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
void gnb_cu_up_mbs_support_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mbs_support_info_to_add_list.size() > 0) {
    j.start_array("mbs-Support-Info-ToAdd-List");
    for (const auto& e1 : mbs_support_info_to_add_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mbs_support_info_to_rem_list.size() > 0) {
    j.start_array("mbs-Support-Info-ToRemove-List");
    for (const auto& e1 : mbs_support_info_to_rem_list) {
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

// GNB-CU-UP-OverloadInformation ::= ENUMERATED
const char* gnb_cu_up_overload_info_opts::to_string() const
{
  static const char* names[] = {"overloaded", "not-overloaded"};
  return convert_enum_idx(names, 2, value, "gnb_cu_up_overload_info_e");
}

// GNB-CU-UP-TNLA-To-Remove-Item ::= SEQUENCE
OCUDUASN_CODE gnb_cu_up_tnl_a_to_rem_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(tnl_assoc_transport_layer_address_gnb_cu_cp_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.pack(bref));
  if (tnl_assoc_transport_layer_address_gnb_cu_cp_present) {
    HANDLE_CODE(tnl_assoc_transport_layer_address_gnb_cu_cp.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_up_tnl_a_to_rem_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(tnl_assoc_transport_layer_address_gnb_cu_cp_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(tnl_assoc_transport_layer_address.unpack(bref));
  if (tnl_assoc_transport_layer_address_gnb_cu_cp_present) {
    HANDLE_CODE(tnl_assoc_transport_layer_address_gnb_cu_cp.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void gnb_cu_up_tnl_a_to_rem_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tNLAssociationTransportLayerAddress");
  tnl_assoc_transport_layer_address.to_json(j);
  if (tnl_assoc_transport_layer_address_gnb_cu_cp_present) {
    j.write_fieldname("tNLAssociationTransportLayerAddressgNBCUCP");
    tnl_assoc_transport_layer_address_gnb_cu_cp.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// GTPTLA-Item ::= SEQUENCE
OCUDUASN_CODE gtp_tla_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(gtp_transport_layer_addresses.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gtp_tla_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(gtp_transport_layer_addresses.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void gtp_tla_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("gTPTransportLayerAddresses");
  gtp_transport_layer_addresses.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// HW-CapacityIndicator ::= SEQUENCE
OCUDUASN_CODE hw_capacity_ind_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, offered_throughput, (uint32_t)1u, (uint32_t)16777216u, true, true));
  HANDLE_CODE(pack_integer(bref, available_throughput, (uint16_t)0u, (uint16_t)100u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE hw_capacity_ind_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(offered_throughput, bref, (uint32_t)1u, (uint32_t)16777216u, true, true));
  HANDLE_CODE(unpack_integer(available_throughput, bref, (uint16_t)0u, (uint16_t)100u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void hw_capacity_ind_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("offeredThroughput", offered_throughput);
  j.write_int("availableThroughput", available_throughput);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// IAB-Donor-CU-UPPSKInfo-Item ::= SEQUENCE
OCUDUASN_CODE iab_donor_cu_up_psk_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(iab_donor_cu_up_psk.pack(bref));
  HANDLE_CODE(iab_donor_cu_up_ip_address.pack(bref));
  HANDLE_CODE(iab_du_ip_address.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_donor_cu_up_psk_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(iab_donor_cu_up_psk.unpack(bref));
  HANDLE_CODE(iab_donor_cu_up_ip_address.unpack(bref));
  HANDLE_CODE(iab_du_ip_address.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void iab_donor_cu_up_psk_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("iAB-donor-CU-UPPSK", iab_donor_cu_up_psk.to_string());
  j.write_fieldname("iAB-donor-CU-UPIPAddress");
  iab_donor_cu_up_ip_address.to_json(j);
  j.write_fieldname("iAB-DUIPAddress");
  iab_du_ip_address.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// M4period ::= ENUMERATED
const char* m4period_opts::to_string() const
{
  static const char* names[] = {"ms1024", "ms2048", "ms5120", "ms10240", "min1"};
  return convert_enum_idx(names, 5, value, "m4period_e");
}
uint16_t m4period_opts::to_number() const
{
  static const uint16_t numbers[] = {1024, 2048, 5120, 10240, 1};
  return map_enum_number(numbers, 5, value, "m4period_e");
}

// Links-to-log ::= ENUMERATED
const char* links_to_log_opts::to_string() const
{
  static const char* names[] = {"uplink", "downlink", "both-uplink-and-downlink"};
  return convert_enum_idx(names, 3, value, "links_to_log_e");
}

// M4ReportAmount ::= ENUMERATED
const char* m4_report_amount_opts::to_string() const
{
  static const char* names[] = {"r1", "r2", "r4", "r8", "r16", "r32", "r64", "infinity"};
  return convert_enum_idx(names, 8, value, "m4_report_amount_e");
}
int8_t m4_report_amount_opts::to_number() const
{
  static const int8_t numbers[] = {1, 2, 4, 8, 16, 32, 64, -1};
  return map_enum_number(numbers, 8, value, "m4_report_amount_e");
}

// M4Configuration-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t m4_cfg_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {147};
  return map_enum_number(names, 1, idx, "id");
}
bool m4_cfg_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 147 == id;
}
crit_e m4_cfg_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 147) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e m4_cfg_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 147) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE m4_cfg_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += m4_report_amount_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (m4_report_amount_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)147, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(m4_report_amount.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE m4_cfg_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != m4_cfg_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 147) {
      {
        m4_report_amount_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(m4_report_amount.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void m4_cfg_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (m4_report_amount_present) {
    j.start_obj();
    j.write_int("id", 147);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", m4_report_amount.to_string());
    j.end_obj();
  }
  j.end_array();
}

// M4Configuration ::= SEQUENCE
OCUDUASN_CODE m4_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(m4period.pack(bref));
  HANDLE_CODE(m4_links_to_log.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE m4_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(m4period.unpack(bref));
  HANDLE_CODE(m4_links_to_log.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void m4_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("m4period", m4period.to_string());
  j.write_str("m4-links-to-log", m4_links_to_log.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// M6report-Interval ::= ENUMERATED
const char* m6report_interv_opts::to_string() const
{
  static const char* names[] = {"ms120",
                                "ms240",
                                "ms480",
                                "ms640",
                                "ms1024",
                                "ms2048",
                                "ms5120",
                                "ms10240",
                                "ms20480",
                                "ms40960",
                                "min1",
                                "min6",
                                "min12",
                                "min30"};
  return convert_enum_idx(names, 14, value, "m6report_interv_e");
}
uint16_t m6report_interv_opts::to_number() const
{
  static const uint16_t numbers[] = {120, 240, 480, 640, 1024, 2048, 5120, 10240, 20480, 40960, 1, 6, 12, 30};
  return map_enum_number(numbers, 14, value, "m6report_interv_e");
}

// M6ReportAmount ::= ENUMERATED
const char* m6_report_amount_opts::to_string() const
{
  static const char* names[] = {"r1", "r2", "r4", "r8", "r16", "r32", "r64", "infinity"};
  return convert_enum_idx(names, 8, value, "m6_report_amount_e");
}
int8_t m6_report_amount_opts::to_number() const
{
  static const int8_t numbers[] = {1, 2, 4, 8, 16, 32, 64, -1};
  return map_enum_number(numbers, 8, value, "m6_report_amount_e");
}

// M6Configuration-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t m6_cfg_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {148};
  return map_enum_number(names, 1, idx, "id");
}
bool m6_cfg_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 148 == id;
}
crit_e m6_cfg_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 148) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e m6_cfg_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 148) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE m6_cfg_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += m6_report_amount_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (m6_report_amount_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)148, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(m6_report_amount.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE m6_cfg_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != m6_cfg_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 148) {
      {
        m6_report_amount_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(m6_report_amount.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void m6_cfg_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (m6_report_amount_present) {
    j.start_obj();
    j.write_int("id", 148);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", m6_report_amount.to_string());
    j.end_obj();
  }
  j.end_array();
}

// M6Configuration ::= SEQUENCE
OCUDUASN_CODE m6_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(m6report_interv.pack(bref));
  HANDLE_CODE(m6_links_to_log.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE m6_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(m6report_interv.unpack(bref));
  HANDLE_CODE(m6_links_to_log.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void m6_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("m6report-Interval", m6report_interv.to_string());
  j.write_str("m6-links-to-log", m6_links_to_log.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// M7ReportAmount ::= ENUMERATED
const char* m7_report_amount_opts::to_string() const
{
  static const char* names[] = {"r1", "r2", "r4", "r8", "r16", "r32", "r64", "infinity"};
  return convert_enum_idx(names, 8, value, "m7_report_amount_e");
}
int8_t m7_report_amount_opts::to_number() const
{
  static const int8_t numbers[] = {1, 2, 4, 8, 16, 32, 64, -1};
  return map_enum_number(numbers, 8, value, "m7_report_amount_e");
}

// M7Configuration-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t m7_cfg_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {149};
  return map_enum_number(names, 1, idx, "id");
}
bool m7_cfg_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 149 == id;
}
crit_e m7_cfg_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 149) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e m7_cfg_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 149) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE m7_cfg_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += m7_report_amount_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (m7_report_amount_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)149, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(m7_report_amount.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE m7_cfg_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != m7_cfg_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 149) {
      {
        m7_report_amount_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(m7_report_amount.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void m7_cfg_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (m7_report_amount_present) {
    j.start_obj();
    j.write_int("id", 149);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", m7_report_amount.to_string());
    j.end_obj();
  }
  j.end_array();
}

// M7Configuration ::= SEQUENCE
OCUDUASN_CODE m7_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, m7period, (uint8_t)1u, (uint8_t)60u, true, true));
  HANDLE_CODE(m7_links_to_log.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE m7_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(m7period, bref, (uint8_t)1u, (uint8_t)60u, true, true));
  HANDLE_CODE(m7_links_to_log.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void m7_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("m7period", m7period);
  j.write_str("m7-links-to-log", m7_links_to_log.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ImmediateMDT ::= SEQUENCE
OCUDUASN_CODE immediate_mdt_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(meas_four_present, 1));
  HANDLE_CODE(bref.pack(meas_six_present, 1));
  HANDLE_CODE(bref.pack(meas_seven_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(meass_to_activ.pack(bref));
  if (meas_four_present) {
    HANDLE_CODE(meas_four.pack(bref));
  }
  if (meas_six_present) {
    HANDLE_CODE(meas_six.pack(bref));
  }
  if (meas_seven_present) {
    HANDLE_CODE(meas_seven.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE immediate_mdt_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(meas_four_present, 1));
  HANDLE_CODE(bref.unpack(meas_six_present, 1));
  HANDLE_CODE(bref.unpack(meas_seven_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(meass_to_activ.unpack(bref));
  if (meas_four_present) {
    HANDLE_CODE(meas_four.unpack(bref));
  }
  if (meas_six_present) {
    HANDLE_CODE(meas_six.unpack(bref));
  }
  if (meas_seven_present) {
    HANDLE_CODE(meas_seven.unpack(bref));
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
void immediate_mdt_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("measurementsToActivate");
  meass_to_activ.to_json(j);
  if (meas_four_present) {
    j.write_fieldname("measurementFour");
    meas_four.to_json(j);
  }
  if (meas_six_present) {
    j.write_fieldname("measurementSix");
    meas_six.to_json(j);
  }
  if (meas_seven_present) {
    j.write_fieldname("measurementSeven");
    meas_seven.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// InactivityInformationRequest ::= ENUMERATED
const char* inactivity_info_request_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "inactivity_info_request_e");
}

// IntegrityProtectionAlgorithm ::= ENUMERATED
const char* integrity_protection_algorithm_opts::to_string() const
{
  static const char* names[] = {"nIA0", "i-128-NIA1", "i-128-NIA2", "i-128-NIA3"};
  return convert_enum_idx(names, 4, value, "integrity_protection_algorithm_e");
}

// MBS-DL-Data-Arrival ::= SEQUENCE
OCUDUASN_CODE mbs_dl_data_arrival_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ppi_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(dl_data_arrival.pack(bref));
  if (ppi_present) {
    HANDLE_CODE(pack_integer(bref, ppi, (uint8_t)0u, (uint8_t)7u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_dl_data_arrival_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ppi_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(dl_data_arrival.unpack(bref));
  if (ppi_present) {
    HANDLE_CODE(unpack_integer(ppi, bref, (uint8_t)0u, (uint8_t)7u, true, true));
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
void mbs_dl_data_arrival_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("dlDataArrival", "true");
  if (ppi_present) {
    j.write_int("ppi", ppi);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* mbs_dl_data_arrival_s::dl_data_arrival_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "mbs_dl_data_arrival_s::dl_data_arrival_e_");
}

// MBS-PDCP-COUNT-Req ::= ENUMERATED
const char* mbs_pdcp_count_req_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "mbs_pdcp_count_req_e");
}

// MBS-ServiceAreaTAIList-Item ::= SEQUENCE
OCUDUASN_CODE mbs_service_area_tai_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(five_gs_tac.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_service_area_tai_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(five_gs_tac.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbs_service_area_tai_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("plmn-ID", plmn_id.to_string());
  j.write_str("fiveGS-TAC", five_gs_tac.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MBS-ServiceAreaInformation ::= SEQUENCE
OCUDUASN_CODE mbs_service_area_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_service_area_cell_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(mbs_service_area_tai_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (mbs_service_area_cell_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_service_area_cell_list, 1, 512, true));
  }
  if (mbs_service_area_tai_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_service_area_tai_list, 1, 512, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_service_area_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool mbs_service_area_cell_list_present;
  HANDLE_CODE(bref.unpack(mbs_service_area_cell_list_present, 1));
  bool mbs_service_area_tai_list_present;
  HANDLE_CODE(bref.unpack(mbs_service_area_tai_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (mbs_service_area_cell_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_service_area_cell_list, bref, 1, 512, true));
  }
  if (mbs_service_area_tai_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_service_area_tai_list, bref, 1, 512, true));
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
void mbs_service_area_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mbs_service_area_cell_list.size() > 0) {
    j.start_array("mBS-ServiceAreaCellList");
    for (const auto& e1 : mbs_service_area_cell_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mbs_service_area_tai_list.size() > 0) {
    j.start_array("mBS-ServiceAreaTAIList");
    for (const auto& e1 : mbs_service_area_tai_list) {
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

// MBS-ServiceAreaInformationItem ::= SEQUENCE
OCUDUASN_CODE mbs_service_area_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_service_area_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_service_area_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
  HANDLE_CODE(mbs_service_area_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbs_service_area_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mBS-AreaSessionID", mbs_area_session_id);
  j.write_fieldname("mBS-ServiceAreaInformation");
  mbs_service_area_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MBS-ServiceArea ::= SEQUENCE
OCUDUASN_CODE mbs_service_area_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_service_area_info_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (mbs_service_area_info_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mbs_service_area_info_list, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_service_area_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool mbs_service_area_info_list_present;
  HANDLE_CODE(bref.unpack(mbs_service_area_info_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (mbs_service_area_info_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mbs_service_area_info_list, bref, 1, 256, true));
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
void mbs_service_area_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mbs_service_area_info_list.size() > 0) {
    j.start_array("mBS-ServiceAreaInformationList");
    for (const auto& e1 : mbs_service_area_info_list) {
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

// MBSMulticastF1UContextDescriptor ::= SEQUENCE
OCUDUASN_CODE mbs_multicast_f1_u_context_descriptor_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_area_session_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(multicast_f1_u_context_ref_e1.pack(bref));
  HANDLE_CODE(mc_f1_u_ctxtusage.pack(bref));
  if (mbs_area_session_present) {
    HANDLE_CODE(pack_integer(bref, mbs_area_session, (uint32_t)0u, (uint32_t)65535u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_multicast_f1_u_context_descriptor_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mbs_area_session_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(multicast_f1_u_context_ref_e1.unpack(bref));
  HANDLE_CODE(mc_f1_u_ctxtusage.unpack(bref));
  if (mbs_area_session_present) {
    HANDLE_CODE(unpack_integer(mbs_area_session, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
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
void mbs_multicast_f1_u_context_descriptor_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("multicastF1UContextReferenceE1", multicast_f1_u_context_ref_e1.to_string());
  j.write_str("mc-F1UCtxtusage", mc_f1_u_ctxtusage.to_string());
  if (mbs_area_session_present) {
    j.write_int("mbsAreaSession", mbs_area_session);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* mbs_multicast_f1_u_context_descriptor_s::mc_f1_u_ctxtusage_opts::to_string() const
{
  static const char* names[] = {"ptm", "ptp", "ptp-retransmission", "ptp-forwarding"};
  return convert_enum_idx(names, 4, value, "mbs_multicast_f1_u_context_descriptor_s::mc_f1_u_ctxtusage_e_");
}

// MBSSessionAssociatedInformation-Item ::= SEQUENCE
OCUDUASN_CODE mbs_session_associated_info_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mbs_qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(pack_integer(bref, associated_unicast_qos_flow_id, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_session_associated_info_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mbs_qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  HANDLE_CODE(unpack_integer(associated_unicast_qos_flow_id, bref, (uint8_t)0u, (uint8_t)63u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbs_session_associated_info_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mbs-QoS-Flow-Identifier", mbs_qos_flow_id);
  j.write_int("associated-unicast-QoS-Flow-Identifier", associated_unicast_qos_flow_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MBSSessionAssociatedInfoNonSupportToSupport ::= SEQUENCE
OCUDUASN_CODE mbs_session_associated_info_non_support_to_support_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, ue_ref_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, associated_qos_flow_info_list, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_session_associated_info_non_support_to_support_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(ue_ref_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(associated_qos_flow_info_list, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbs_session_associated_info_non_support_to_support_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_uint("ue-Reference-ID", ue_ref_id);
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.start_array("associatedQoSFlowInformationList");
  for (const auto& e1 : associated_qos_flow_info_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MBSSessionAssociatedInformation ::= SEQUENCE
OCUDUASN_CODE mbs_session_associated_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_dyn_seq_of(bref, mbs_session_associated_info_list, 1, 64, true));
  HANDLE_CODE(mbs_session_forwarding_address.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_session_associated_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_dyn_seq_of(mbs_session_associated_info_list, bref, 1, 64, true));
  HANDLE_CODE(mbs_session_forwarding_address.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mbs_session_associated_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("mbsSessionAssociatedInformationList");
  for (const auto& e1 : mbs_session_associated_info_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.write_fieldname("mbsSessionForwardingAddress");
  mbs_session_forwarding_address.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCBearerContext-Inactivity ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_inactivity_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mc_bearer_context_inactivity_ind.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_inactivity_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mc_bearer_context_inactivity_ind.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_inactivity_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mcBearerContext-Inactivity-Indication", "true");
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* mc_bearer_context_inactivity_s::mc_bearer_context_inactivity_ind_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "mc_bearer_context_inactivity_s::mc_bearer_context_inactivity_ind_e_");
}

// MBSSessionResourceNotification ::= CHOICE
void mbs_session_res_notif_c::destroy_()
{
  switch (type_) {
    case types::mbs_dl_data_arrival:
      c.destroy<mbs_dl_data_arrival_s>();
      break;
    case types::inactivity:
      c.destroy<mc_bearer_context_inactivity_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void mbs_session_res_notif_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::mbs_dl_data_arrival:
      c.init<mbs_dl_data_arrival_s>();
      break;
    case types::inactivity:
      c.init<mc_bearer_context_inactivity_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_session_res_notif_c");
  }
}
mbs_session_res_notif_c::mbs_session_res_notif_c(const mbs_session_res_notif_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::mbs_dl_data_arrival:
      c.init(other.c.get<mbs_dl_data_arrival_s>());
      break;
    case types::inactivity:
      c.init(other.c.get<mc_bearer_context_inactivity_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_session_res_notif_c");
  }
}
mbs_session_res_notif_c& mbs_session_res_notif_c::operator=(const mbs_session_res_notif_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::mbs_dl_data_arrival:
      c.set(other.c.get<mbs_dl_data_arrival_s>());
      break;
    case types::inactivity:
      c.set(other.c.get<mc_bearer_context_inactivity_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_session_res_notif_c");
  }

  return *this;
}
mbs_dl_data_arrival_s& mbs_session_res_notif_c::set_mbs_dl_data_arrival()
{
  set(types::mbs_dl_data_arrival);
  return c.get<mbs_dl_data_arrival_s>();
}
mc_bearer_context_inactivity_s& mbs_session_res_notif_c::set_inactivity()
{
  set(types::inactivity);
  return c.get<mc_bearer_context_inactivity_s>();
}
protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>& mbs_session_res_notif_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>>();
}
void mbs_session_res_notif_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::mbs_dl_data_arrival:
      j.write_fieldname("mbs-DL-Data-Arrival");
      c.get<mbs_dl_data_arrival_s>().to_json(j);
      break;
    case types::inactivity:
      j.write_fieldname("inactivity");
      c.get<mc_bearer_context_inactivity_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "mbs_session_res_notif_c");
  }
  j.end_obj();
}
OCUDUASN_CODE mbs_session_res_notif_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::mbs_dl_data_arrival:
      HANDLE_CODE(c.get<mbs_dl_data_arrival_s>().pack(bref));
      break;
    case types::inactivity:
      HANDLE_CODE(c.get<mc_bearer_context_inactivity_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mbs_session_res_notif_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_session_res_notif_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::mbs_dl_data_arrival:
      HANDLE_CODE(c.get<mbs_dl_data_arrival_s>().unpack(bref));
      break;
    case types::inactivity:
      HANDLE_CODE(c.get<mc_bearer_context_inactivity_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mbs_session_res_notif_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mbs_session_res_notif_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mbs_session_res_notif_c::types_opts::to_string() const
{
  static const char* names[] = {"mbs-DL-Data-Arrival", "inactivity", "choice-extension"};
  return convert_enum_idx(names, 3, value, "mbs_session_res_notif_c::types");
}

// MCBearerContextF1UTNLInfoatDU ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_f1_u_tnl_infoat_du_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mbs_f1_u_infoat_du.pack(bref));
  HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_f1_u_tnl_infoat_du_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mbs_f1_u_infoat_du.unpack(bref));
  HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_f1_u_tnl_infoat_du_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mbsF1UInfoatDU");
  mbs_f1_u_infoat_du.to_json(j);
  j.write_fieldname("mbsMulticastF1UContextDescriptor");
  mbs_multicast_f1_u_context_descriptor.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCBearerContextNGU-TNLInfoatNGRAN ::= CHOICE
void mc_bearer_context_ngu_tnl_infoat_ngran_c::destroy_()
{
  switch (type_) {
    case types::locationindependent:
      c.destroy<mb_sn_gu_info_at_ngran_c>();
      break;
    case types::locationdependent:
      c.destroy<location_dependent_mb_sn_gu_info_at_ngran_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void mc_bearer_context_ngu_tnl_infoat_ngran_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::locationindependent:
      c.init<mb_sn_gu_info_at_ngran_c>();
      break;
    case types::locationdependent:
      c.init<location_dependent_mb_sn_gu_info_at_ngran_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mc_bearer_context_ngu_tnl_infoat_ngran_c");
  }
}
mc_bearer_context_ngu_tnl_infoat_ngran_c::mc_bearer_context_ngu_tnl_infoat_ngran_c(
    const mc_bearer_context_ngu_tnl_infoat_ngran_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::locationindependent:
      c.init(other.c.get<mb_sn_gu_info_at_ngran_c>());
      break;
    case types::locationdependent:
      c.init(other.c.get<location_dependent_mb_sn_gu_info_at_ngran_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mc_bearer_context_ngu_tnl_infoat_ngran_c");
  }
}
mc_bearer_context_ngu_tnl_infoat_ngran_c&
mc_bearer_context_ngu_tnl_infoat_ngran_c::operator=(const mc_bearer_context_ngu_tnl_infoat_ngran_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::locationindependent:
      c.set(other.c.get<mb_sn_gu_info_at_ngran_c>());
      break;
    case types::locationdependent:
      c.set(other.c.get<location_dependent_mb_sn_gu_info_at_ngran_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mc_bearer_context_ngu_tnl_infoat_ngran_c");
  }

  return *this;
}
mb_sn_gu_info_at_ngran_c& mc_bearer_context_ngu_tnl_infoat_ngran_c::set_locationindependent()
{
  set(types::locationindependent);
  return c.get<mb_sn_gu_info_at_ngran_c>();
}
location_dependent_mb_sn_gu_info_at_ngran_l& mc_bearer_context_ngu_tnl_infoat_ngran_c::set_locationdependent()
{
  set(types::locationdependent);
  return c.get<location_dependent_mb_sn_gu_info_at_ngran_l>();
}
protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>&
mc_bearer_context_ngu_tnl_infoat_ngran_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>();
}
void mc_bearer_context_ngu_tnl_infoat_ngran_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::locationindependent:
      j.write_fieldname("locationindependent");
      c.get<mb_sn_gu_info_at_ngran_c>().to_json(j);
      break;
    case types::locationdependent:
      j.start_array("locationdependent");
      for (const auto& e1 : c.get<location_dependent_mb_sn_gu_info_at_ngran_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "mc_bearer_context_ngu_tnl_infoat_ngran_c");
  }
  j.end_obj();
}
OCUDUASN_CODE mc_bearer_context_ngu_tnl_infoat_ngran_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mb_sn_gu_info_at_ngran_c>().pack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<location_dependent_mb_sn_gu_info_at_ngran_l>(), 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mc_bearer_context_ngu_tnl_infoat_ngran_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_ngu_tnl_infoat_ngran_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::locationindependent:
      HANDLE_CODE(c.get<mb_sn_gu_info_at_ngran_c>().unpack(bref));
      break;
    case types::locationdependent:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<location_dependent_mb_sn_gu_info_at_ngran_l>(), bref, 1, 256, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(
          c.get<protocol_ie_single_container_s<mc_bearer_context_ngu_tnl_infoat_ngran_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mc_bearer_context_ngu_tnl_infoat_ngran_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mc_bearer_context_ngu_tnl_infoat_ngran_c::types_opts::to_string() const
{
  static const char* names[] = {"locationindependent", "locationdependent", "choice-extension"};
  return convert_enum_idx(names, 3, value, "mc_bearer_context_ngu_tnl_infoat_ngran_c::types");
}

// MCBearerContextNGU-TNLInfoatNGRANModifyResponse ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_area_session_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mbs_ngu_infoat_ngran.pack(bref));
  if (mbs_area_session_present) {
    HANDLE_CODE(pack_integer(bref, mbs_area_session, (uint32_t)0u, (uint32_t)65535u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mbs_area_session_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mbs_ngu_infoat_ngran.unpack(bref));
  if (mbs_area_session_present) {
    HANDLE_CODE(unpack_integer(mbs_area_session, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
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
void mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mbs-NGU-InfoatNGRAN");
  mbs_ngu_infoat_ngran.to_json(j);
  if (mbs_area_session_present) {
    j.write_int("mbsAreaSession", mbs_area_session);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCBearerContextNGUTNLInfoat5GC ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_ngu_tnl_infoat5_gc_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_area_session_id_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mbs_ngu_info_at5_gc.pack(bref));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_ngu_tnl_infoat5_gc_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mbs_area_session_id_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mbs_ngu_info_at5_gc.unpack(bref));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
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
void mc_bearer_context_ngu_tnl_infoat5_gc_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mbsNGUInformationAt5GC");
  mbs_ngu_info_at5_gc.to_json(j);
  if (mbs_area_session_id_present) {
    j.write_int("mbsAreaSession-ID", mbs_area_session_id);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCBearerContextNGUTnlInfoatNGRANRequest ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_ngu_tnl_infoat_ngran_request_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_area_session_id_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ng_ranngu_tnl_requested.pack(bref));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_ngu_tnl_infoat_ngran_request_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mbs_area_session_id_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ng_ranngu_tnl_requested.unpack(bref));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
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
void mc_bearer_context_ngu_tnl_infoat_ngran_request_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("ngRANNGUTNLRequested", "requested");
  if (mbs_area_session_id_present) {
    j.write_int("mbsAreaSession-ID", mbs_area_session_id);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* mc_bearer_context_ngu_tnl_infoat_ngran_request_s::ng_ranngu_tnl_requested_opts::to_string() const
{
  static const char* names[] = {"requested"};
  return convert_enum_idx(
      names, 1, value, "mc_bearer_context_ngu_tnl_infoat_ngran_request_s::ng_ranngu_tnl_requested_e_");
}

// MCBearerContextStatusChange ::= ENUMERATED
const char* mc_bearer_context_status_change_opts::to_string() const
{
  static const char* names[] = {"suspend", "resume"};
  return convert_enum_idx(names, 2, value, "mc_bearer_context_status_change_e");
}

// MCMRBSetupModifyConfiguration-Item ::= SEQUENCE
OCUDUASN_CODE mcmrb_setup_modify_cfg_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(f1u_tnl_at_du_present, 1));
  HANDLE_CODE(bref.pack(mbs_pdcp_cfg_present, 1));
  HANDLE_CODE(bref.pack(qos_flow_qos_param_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(mrb_qos_present, 1));
  HANDLE_CODE(bref.pack(mbs_pdcp_count_req_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (f1u_tnl_at_du_present) {
    HANDLE_CODE(f1u_tnl_at_du.pack(bref));
  }
  if (mbs_pdcp_cfg_present) {
    HANDLE_CODE(mbs_pdcp_cfg.pack(bref));
  }
  if (qos_flow_qos_param_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qos_flow_qos_param_list, 1, 64, true));
  }
  if (mrb_qos_present) {
    HANDLE_CODE(mrb_qos.pack(bref));
  }
  if (mbs_pdcp_count_req_present) {
    HANDLE_CODE(mbs_pdcp_count_req.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcmrb_setup_modify_cfg_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(f1u_tnl_at_du_present, 1));
  HANDLE_CODE(bref.unpack(mbs_pdcp_cfg_present, 1));
  bool qos_flow_qos_param_list_present;
  HANDLE_CODE(bref.unpack(qos_flow_qos_param_list_present, 1));
  HANDLE_CODE(bref.unpack(mrb_qos_present, 1));
  HANDLE_CODE(bref.unpack(mbs_pdcp_count_req_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (f1u_tnl_at_du_present) {
    HANDLE_CODE(f1u_tnl_at_du.unpack(bref));
  }
  if (mbs_pdcp_cfg_present) {
    HANDLE_CODE(mbs_pdcp_cfg.unpack(bref));
  }
  if (qos_flow_qos_param_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qos_flow_qos_param_list, bref, 1, 64, true));
  }
  if (mrb_qos_present) {
    HANDLE_CODE(mrb_qos.unpack(bref));
  }
  if (mbs_pdcp_count_req_present) {
    HANDLE_CODE(mbs_pdcp_count_req.unpack(bref));
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
void mcmrb_setup_modify_cfg_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (f1u_tnl_at_du_present) {
    j.write_fieldname("f1uTNLatDU");
    f1u_tnl_at_du.to_json(j);
  }
  if (mbs_pdcp_cfg_present) {
    j.write_fieldname("mbs-pdcp-config");
    mbs_pdcp_cfg.to_json(j);
  }
  if (qos_flow_qos_param_list.size() > 0) {
    j.start_array("qoS-Flow-QoS-Parameter-List");
    for (const auto& e1 : qos_flow_qos_param_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mrb_qos_present) {
    j.write_fieldname("mrbQoS");
    mrb_qos.to_json(j);
  }
  if (mbs_pdcp_count_req_present) {
    j.write_str("mbs-PDCP-COUNT-Req", "true");
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MRB-ProgressInformationType ::= ENUMERATED
const char* mrb_progress_info_type_opts::to_string() const
{
  static const char* names[] = {"oldest-available", "last-delivered"};
  return convert_enum_idx(names, 2, value, "mrb_progress_info_type_e");
}

// MRBForwardingResourceRequest-Item ::= SEQUENCE
OCUDUASN_CODE mrb_forwarding_res_request_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mrb_progress_request_type_present, 1));
  HANDLE_CODE(bref.pack(mrb_forwarding_address_request_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (mrb_progress_request_type_present) {
    HANDLE_CODE(mrb_progress_request_type.pack(bref));
  }
  if (mrb_forwarding_address_request_present) {
    HANDLE_CODE(mrb_forwarding_address_request.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_forwarding_res_request_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mrb_progress_request_type_present, 1));
  HANDLE_CODE(bref.unpack(mrb_forwarding_address_request_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (mrb_progress_request_type_present) {
    HANDLE_CODE(mrb_progress_request_type.unpack(bref));
  }
  if (mrb_forwarding_address_request_present) {
    HANDLE_CODE(mrb_forwarding_address_request.unpack(bref));
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
void mrb_forwarding_res_request_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (mrb_progress_request_type_present) {
    j.write_str("mrbProgressRequestType", mrb_progress_request_type.to_string());
  }
  if (mrb_forwarding_address_request_present) {
    j.write_str("mrbForwardingAddressRequest", "request");
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* mrb_forwarding_res_request_item_s::mrb_forwarding_address_request_opts::to_string() const
{
  static const char* names[] = {"request"};
  return convert_enum_idx(names, 1, value, "mrb_forwarding_res_request_item_s::mrb_forwarding_address_request_e_");
}

// MCForwardingResourceRequest ::= SEQUENCE
OCUDUASN_CODE mc_forwarding_res_request_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_area_session_id_present, 1));
  HANDLE_CODE(bref.pack(mrb_forwarding_res_request_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.pack(bref));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  }
  if (mrb_forwarding_res_request_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mrb_forwarding_res_request_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_forwarding_res_request_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mbs_area_session_id_present, 1));
  bool mrb_forwarding_res_request_list_present;
  HANDLE_CODE(bref.unpack(mrb_forwarding_res_request_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.unpack(bref));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
  }
  if (mrb_forwarding_res_request_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mrb_forwarding_res_request_list, bref, 1, 64, true));
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
void mc_forwarding_res_request_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mcForwardingResourceID", mc_forwarding_res_id.to_string());
  if (mbs_area_session_id_present) {
    j.write_int("mbsAreaSession-ID", mbs_area_session_id);
  }
  if (mrb_forwarding_res_request_list.size() > 0) {
    j.start_array("mrbForwardingResourceRequestList");
    for (const auto& e1 : mrb_forwarding_res_request_list) {
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

// MRB-ProgressInformationSNs ::= CHOICE
void mrb_progress_info_sn_s_c::destroy_()
{
  switch (type_) {
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void mrb_progress_info_sn_s_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::pdcp_sn12:
      break;
    case types::pdcp_sn18:
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mrb_progress_info_sn_s_c");
  }
}
mrb_progress_info_sn_s_c::mrb_progress_info_sn_s_c(const mrb_progress_info_sn_s_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::pdcp_sn12:
      c.init(other.c.get<uint16_t>());
      break;
    case types::pdcp_sn18:
      c.init(other.c.get<uint32_t>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mrb_progress_info_sn_s_c");
  }
}
mrb_progress_info_sn_s_c& mrb_progress_info_sn_s_c::operator=(const mrb_progress_info_sn_s_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::pdcp_sn12:
      c.set(other.c.get<uint16_t>());
      break;
    case types::pdcp_sn18:
      c.set(other.c.get<uint32_t>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mrb_progress_info_sn_s_c");
  }

  return *this;
}
uint16_t& mrb_progress_info_sn_s_c::set_pdcp_sn12()
{
  set(types::pdcp_sn12);
  return c.get<uint16_t>();
}
uint32_t& mrb_progress_info_sn_s_c::set_pdcp_sn18()
{
  set(types::pdcp_sn18);
  return c.get<uint32_t>();
}
protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>& mrb_progress_info_sn_s_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>>();
}
void mrb_progress_info_sn_s_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::pdcp_sn12:
      j.write_int("pdcp-SN12", c.get<uint16_t>());
      break;
    case types::pdcp_sn18:
      j.write_int("pdcp-SN18", c.get<uint32_t>());
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "mrb_progress_info_sn_s_c");
  }
  j.end_obj();
}
OCUDUASN_CODE mrb_progress_info_sn_s_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::pdcp_sn12:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::pdcp_sn18:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)262143u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mrb_progress_info_sn_s_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_progress_info_sn_s_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::pdcp_sn12:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::pdcp_sn18:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)262143u, false, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mrb_progress_info_sn_s_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mrb_progress_info_sn_s_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mrb_progress_info_sn_s_c::types_opts::to_string() const
{
  static const char* names[] = {"pdcp-SN12", "pdcp-SN18", "choice-extension"};
  return convert_enum_idx(names, 3, value, "mrb_progress_info_sn_s_c::types");
}
uint8_t mrb_progress_info_sn_s_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {12, 18};
  return map_enum_number(numbers, 2, value, "mrb_progress_info_sn_s_c::types");
}

// MRB-ProgressInformation ::= SEQUENCE
OCUDUASN_CODE mrb_progress_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mrb_progress_info_sn_s.pack(bref));
  HANDLE_CODE(mrb_progress_info_type.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_progress_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mrb_progress_info_sn_s.unpack(bref));
  HANDLE_CODE(mrb_progress_info_type.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mrb_progress_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("mrb-ProgressInformationSNs");
  mrb_progress_info_sn_s.to_json(j);
  j.write_str("mrb-ProgressInformationType", mrb_progress_info_type.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MRBForwardingResourceIndication-Item ::= SEQUENCE
OCUDUASN_CODE mrb_forwarding_res_ind_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mrb_progress_info_present, 1));
  HANDLE_CODE(bref.pack(mrb_forwarding_address_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (mrb_progress_info_present) {
    HANDLE_CODE(mrb_progress_info.pack(bref));
  }
  if (mrb_forwarding_address_present) {
    HANDLE_CODE(mrb_forwarding_address.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_forwarding_res_ind_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mrb_progress_info_present, 1));
  HANDLE_CODE(bref.unpack(mrb_forwarding_address_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (mrb_progress_info_present) {
    HANDLE_CODE(mrb_progress_info.unpack(bref));
  }
  if (mrb_forwarding_address_present) {
    HANDLE_CODE(mrb_forwarding_address.unpack(bref));
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
void mrb_forwarding_res_ind_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (mrb_progress_info_present) {
    j.write_fieldname("mrb-ProgressInformation");
    mrb_progress_info.to_json(j);
  }
  if (mrb_forwarding_address_present) {
    j.write_fieldname("mrbForwardingAddress");
    mrb_forwarding_address.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCForwardingResourceIndication ::= SEQUENCE
OCUDUASN_CODE mc_forwarding_res_ind_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mrb_forwarding_res_ind_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(mbs_session_associated_info_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.pack(bref));
  if (mrb_forwarding_res_ind_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mrb_forwarding_res_ind_list, 1, 64, true));
  }
  if (mbs_session_associated_info_present) {
    HANDLE_CODE(mbs_session_associated_info.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_forwarding_res_ind_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool mrb_forwarding_res_ind_list_present;
  HANDLE_CODE(bref.unpack(mrb_forwarding_res_ind_list_present, 1));
  HANDLE_CODE(bref.unpack(mbs_session_associated_info_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.unpack(bref));
  if (mrb_forwarding_res_ind_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mrb_forwarding_res_ind_list, bref, 1, 64, true));
  }
  if (mbs_session_associated_info_present) {
    HANDLE_CODE(mbs_session_associated_info.unpack(bref));
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
void mc_forwarding_res_ind_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mcForwardingResourceID", mc_forwarding_res_id.to_string());
  if (mrb_forwarding_res_ind_list.size() > 0) {
    j.start_array("mrbForwardingResourceIndicationList");
    for (const auto& e1 : mrb_forwarding_res_ind_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mbs_session_associated_info_present) {
    j.write_fieldname("mbsSessionAssociatedInformation");
    mbs_session_associated_info.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCForwardingResourceRelease ::= SEQUENCE
OCUDUASN_CODE mc_forwarding_res_release_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_forwarding_res_release_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mc_forwarding_res_release_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mcForwardingResourceID", mc_forwarding_res_id.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCBearerContextToModify-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t mc_bearer_context_to_modify_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {179, 180, 182, 185, 192, 193};
  return map_enum_number(names, 6, idx, "id");
}
bool mc_bearer_context_to_modify_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {179, 180, 182, 185, 192, 193};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_to_modify_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 179:
      return crit_e::ignore;
    case 180:
      return crit_e::ignore;
    case 182:
      return crit_e::ignore;
    case 185:
      return crit_e::ignore;
    case 192:
      return crit_e::ignore;
    case 193:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_to_modify_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 179:
      return presence_e::optional;
    case 180:
      return presence_e::optional;
    case 182:
      return presence_e::optional;
    case 185:
      return presence_e::optional;
    case 192:
      return presence_e::optional;
    case 193:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_to_modify_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += mc_forwarding_res_request_present ? 1 : 0;
  nof_ies += mc_forwarding_res_ind_present ? 1 : 0;
  nof_ies += mc_forwarding_res_release_present ? 1 : 0;
  nof_ies += mbs_session_associated_info_non_support_to_support_present ? 1 : 0;
  nof_ies += mc_bearer_context_inactivity_timer_present ? 1 : 0;
  nof_ies += mc_bearer_context_status_change_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (mc_forwarding_res_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)179, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mc_forwarding_res_request.pack(bref));
  }
  if (mc_forwarding_res_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)180, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mc_forwarding_res_ind.pack(bref));
  }
  if (mc_forwarding_res_release_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)182, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mc_forwarding_res_release.pack(bref));
  }
  if (mbs_session_associated_info_non_support_to_support_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)185, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_session_associated_info_non_support_to_support.pack(bref));
  }
  if (mc_bearer_context_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)192, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, mc_bearer_context_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (mc_bearer_context_status_change_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)193, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mc_bearer_context_status_change.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_modify_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != mc_bearer_context_to_modify_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 179: {
        mc_forwarding_res_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mc_forwarding_res_request.unpack(bref));
        break;
      }
      case 180: {
        mc_forwarding_res_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mc_forwarding_res_ind.unpack(bref));
        break;
      }
      case 182: {
        mc_forwarding_res_release_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mc_forwarding_res_release.unpack(bref));
        break;
      }
      case 185: {
        mbs_session_associated_info_non_support_to_support_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_session_associated_info_non_support_to_support.unpack(bref));
        break;
      }
      case 192: {
        mc_bearer_context_inactivity_timer_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(
            unpack_integer(mc_bearer_context_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
        break;
      }
      case 193: {
        mc_bearer_context_status_change_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mc_bearer_context_status_change.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_to_modify_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (mc_forwarding_res_request_present) {
    j.start_obj();
    j.write_int("id", 179);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mc_forwarding_res_request.to_json(j);
    j.end_obj();
  }
  if (mc_forwarding_res_ind_present) {
    j.start_obj();
    j.write_int("id", 180);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mc_forwarding_res_ind.to_json(j);
    j.end_obj();
  }
  if (mc_forwarding_res_release_present) {
    j.start_obj();
    j.write_int("id", 182);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mc_forwarding_res_release.to_json(j);
    j.end_obj();
  }
  if (mbs_session_associated_info_non_support_to_support_present) {
    j.start_obj();
    j.write_int("id", 185);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mbs_session_associated_info_non_support_to_support.to_json(j);
    j.end_obj();
  }
  if (mc_bearer_context_inactivity_timer_present) {
    j.start_obj();
    j.write_int("id", 192);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", mc_bearer_context_inactivity_timer);
    j.end_obj();
  }
  if (mc_bearer_context_status_change_present) {
    j.start_obj();
    j.write_int("id", 193);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", mc_bearer_context_status_change.to_string());
    j.end_obj();
  }
  j.end_array();
}

// MCBearerContextToModify ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_to_modify_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mc_bearer_context_ngu_tnl_infoat5_gc_present, 1));
  HANDLE_CODE(bref.pack(mc_bearer_context_ngu_tnl_infoat_ngran_request_present, 1));
  HANDLE_CODE(bref.pack(mbs_multicast_f1_u_context_descriptor_present, 1));
  HANDLE_CODE(bref.pack(requested_action_present, 1));
  HANDLE_CODE(bref.pack(mc_mrb_to_setup_modify_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(mc_mrb_to_rem_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (mc_bearer_context_ngu_tnl_infoat5_gc_present) {
    HANDLE_CODE(mc_bearer_context_ngu_tnl_infoat5_gc.pack(bref));
  }
  if (mc_bearer_context_ngu_tnl_infoat_ngran_request_present) {
    HANDLE_CODE(mc_bearer_context_ngu_tnl_infoat_ngran_request.pack(bref));
  }
  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  if (requested_action_present) {
    HANDLE_CODE(requested_action.pack(bref));
  }
  if (mc_mrb_to_setup_modify_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_to_setup_modify_list, 1, 32, true));
  }
  if (mc_mrb_to_rem_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_to_rem_list, 1, 32, integer_packer<uint16_t>(1, 512, true, true), true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_modify_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mc_bearer_context_ngu_tnl_infoat5_gc_present, 1));
  HANDLE_CODE(bref.unpack(mc_bearer_context_ngu_tnl_infoat_ngran_request_present, 1));
  HANDLE_CODE(bref.unpack(mbs_multicast_f1_u_context_descriptor_present, 1));
  HANDLE_CODE(bref.unpack(requested_action_present, 1));
  bool mc_mrb_to_setup_modify_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_to_setup_modify_list_present, 1));
  bool mc_mrb_to_rem_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_to_rem_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (mc_bearer_context_ngu_tnl_infoat5_gc_present) {
    HANDLE_CODE(mc_bearer_context_ngu_tnl_infoat5_gc.unpack(bref));
  }
  if (mc_bearer_context_ngu_tnl_infoat_ngran_request_present) {
    HANDLE_CODE(mc_bearer_context_ngu_tnl_infoat_ngran_request.unpack(bref));
  }
  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
  }
  if (requested_action_present) {
    HANDLE_CODE(requested_action.unpack(bref));
  }
  if (mc_mrb_to_setup_modify_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_to_setup_modify_list, bref, 1, 32, true));
  }
  if (mc_mrb_to_rem_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_to_rem_list, bref, 1, 32, integer_packer<uint16_t>(1, 512, true, true), true));
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
void mc_bearer_context_to_modify_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mc_bearer_context_ngu_tnl_infoat5_gc_present) {
    j.write_fieldname("mcBearerContextNGUTNLInfoat5GC");
    mc_bearer_context_ngu_tnl_infoat5_gc.to_json(j);
  }
  if (mc_bearer_context_ngu_tnl_infoat_ngran_request_present) {
    j.write_fieldname("mcBearerContextNGUTnlInfoatNGRANRequest");
    mc_bearer_context_ngu_tnl_infoat_ngran_request.to_json(j);
  }
  if (mbs_multicast_f1_u_context_descriptor_present) {
    j.write_fieldname("mbsMulticastF1UContextDescriptor");
    mbs_multicast_f1_u_context_descriptor.to_json(j);
  }
  if (requested_action_present) {
    j.write_str("requestedAction", requested_action.to_string());
  }
  if (mc_mrb_to_setup_modify_list.size() > 0) {
    j.start_array("mcMRBToSetupModifyList");
    for (const auto& e1 : mc_mrb_to_setup_modify_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mc_mrb_to_rem_list.size() > 0) {
    j.start_array("mcMRBToRemoveList");
    for (const auto& e1 : mc_mrb_to_rem_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCMRBModifyConfirmList-Item ::= SEQUENCE
OCUDUASN_CODE mcmrb_modify_confirm_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcmrb_modify_confirm_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mcmrb_modify_confirm_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCBearerContextToModifyConfirm ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_to_modify_confirm_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_multicast_f1_u_context_descriptor_present, 1));
  HANDLE_CODE(bref.pack(mc_mrb_modify_confirm_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  if (mc_mrb_modify_confirm_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_modify_confirm_list, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_modify_confirm_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mbs_multicast_f1_u_context_descriptor_present, 1));
  bool mc_mrb_modify_confirm_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_modify_confirm_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
  }
  if (mc_mrb_modify_confirm_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_modify_confirm_list, bref, 1, 32, true));
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
void mc_bearer_context_to_modify_confirm_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mbs_multicast_f1_u_context_descriptor_present) {
    j.write_fieldname("mbsMulticastF1UContextDescriptor");
    mbs_multicast_f1_u_context_descriptor.to_json(j);
  }
  if (mc_mrb_modify_confirm_list.size() > 0) {
    j.start_array("mcMRBModifyConfirmList");
    for (const auto& e1 : mc_mrb_modify_confirm_list) {
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

// MCMRBModifyRequiredConfiguration-Item ::= SEQUENCE
OCUDUASN_CODE mcmrb_modify_required_cfg_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_pdcp_count_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (mbs_pdcp_count_present) {
    HANDLE_CODE(mbs_pdcp_count.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcmrb_modify_required_cfg_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mbs_pdcp_count_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (mbs_pdcp_count_present) {
    HANDLE_CODE(mbs_pdcp_count.unpack(bref));
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
void mcmrb_modify_required_cfg_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (mbs_pdcp_count_present) {
    j.write_fieldname("mBS-PDCP-COUNT");
    mbs_pdcp_count.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCForwardingResourceReleaseIndication ::= SEQUENCE
OCUDUASN_CODE mc_forwarding_res_release_ind_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_forwarding_res_release_ind_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mc_forwarding_res_release_ind_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mcForwardingResourceID", mc_forwarding_res_id.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCBearerContextToModifyRequired-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t mc_bearer_context_to_modify_required_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {183};
  return map_enum_number(names, 1, idx, "id");
}
bool mc_bearer_context_to_modify_required_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 183 == id;
}
crit_e mc_bearer_context_to_modify_required_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 183) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e mc_bearer_context_to_modify_required_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 183) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE mc_bearer_context_to_modify_required_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += mc_forwarding_res_release_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (mc_forwarding_res_release_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)183, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mc_forwarding_res_release_ind.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_modify_required_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != mc_bearer_context_to_modify_required_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 183) {
      {
        mc_forwarding_res_release_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mc_forwarding_res_release_ind.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_to_modify_required_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (mc_forwarding_res_release_ind_present) {
    j.start_obj();
    j.write_int("id", 183);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mc_forwarding_res_release_ind.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MCBearerContextToModifyRequired ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_to_modify_required_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mbs_multicast_f1_u_context_descriptor_present, 1));
  HANDLE_CODE(bref.pack(mc_mrb_to_rem_required_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(mc_mrb_to_modify_required_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  if (mc_mrb_to_rem_required_list.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, mc_mrb_to_rem_required_list, 1, 32, integer_packer<uint16_t>(1, 512, true, true), true));
  }
  if (mc_mrb_to_modify_required_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_to_modify_required_list, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_modify_required_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mbs_multicast_f1_u_context_descriptor_present, 1));
  bool mc_mrb_to_rem_required_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_to_rem_required_list_present, 1));
  bool mc_mrb_to_modify_required_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_to_modify_required_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
  }
  if (mc_mrb_to_rem_required_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(
        mc_mrb_to_rem_required_list, bref, 1, 32, integer_packer<uint16_t>(1, 512, true, true), true));
  }
  if (mc_mrb_to_modify_required_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_to_modify_required_list, bref, 1, 32, true));
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
void mc_bearer_context_to_modify_required_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mbs_multicast_f1_u_context_descriptor_present) {
    j.write_fieldname("mbsMulticastF1UContextDescriptor");
    mbs_multicast_f1_u_context_descriptor.to_json(j);
  }
  if (mc_mrb_to_rem_required_list.size() > 0) {
    j.start_array("mcMRBToRemoveRequiredList");
    for (const auto& e1 : mc_mrb_to_rem_required_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (mc_mrb_to_modify_required_list.size() > 0) {
    j.start_array("mcMRBToModifyRequiredList");
    for (const auto& e1 : mc_mrb_to_modify_required_list) {
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

// MCMRBSetupModifyResponseList-Item ::= SEQUENCE
OCUDUASN_CODE mcmrb_setup_modify_resp_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qosflow_setup.size() > 0, 1));
  HANDLE_CODE(bref.pack(qosflow_failed.size() > 0, 1));
  HANDLE_CODE(bref.pack(mc_bearer_context_f1_u_tnl_infoat_cu_present, 1));
  HANDLE_CODE(bref.pack(mbs_pdcp_count_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (qosflow_setup.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qosflow_setup, 1, 64, true));
  }
  if (qosflow_failed.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qosflow_failed, 1, 64, true));
  }
  if (mc_bearer_context_f1_u_tnl_infoat_cu_present) {
    HANDLE_CODE(mc_bearer_context_f1_u_tnl_infoat_cu.pack(bref));
  }
  if (mbs_pdcp_count_present) {
    HANDLE_CODE(mbs_pdcp_count.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcmrb_setup_modify_resp_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool qosflow_setup_present;
  HANDLE_CODE(bref.unpack(qosflow_setup_present, 1));
  bool qosflow_failed_present;
  HANDLE_CODE(bref.unpack(qosflow_failed_present, 1));
  HANDLE_CODE(bref.unpack(mc_bearer_context_f1_u_tnl_infoat_cu_present, 1));
  HANDLE_CODE(bref.unpack(mbs_pdcp_count_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (qosflow_setup_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qosflow_setup, bref, 1, 64, true));
  }
  if (qosflow_failed_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qosflow_failed, bref, 1, 64, true));
  }
  if (mc_bearer_context_f1_u_tnl_infoat_cu_present) {
    HANDLE_CODE(mc_bearer_context_f1_u_tnl_infoat_cu.unpack(bref));
  }
  if (mbs_pdcp_count_present) {
    HANDLE_CODE(mbs_pdcp_count.unpack(bref));
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
void mcmrb_setup_modify_resp_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (qosflow_setup.size() > 0) {
    j.start_array("qosflow-setup");
    for (const auto& e1 : qosflow_setup) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (qosflow_failed.size() > 0) {
    j.start_array("qosflow-failed");
    for (const auto& e1 : qosflow_failed) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mc_bearer_context_f1_u_tnl_infoat_cu_present) {
    j.write_fieldname("mcBearerContextF1UTNLInfoatCU");
    mc_bearer_context_f1_u_tnl_infoat_cu.to_json(j);
  }
  if (mbs_pdcp_count_present) {
    j.write_fieldname("mBS-PDCP-COUNT");
    mbs_pdcp_count.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCMRBFailedList-Item ::= SEQUENCE
OCUDUASN_CODE mcmrb_failed_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcmrb_failed_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mcmrb_failed_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCMRBSetupConfiguration-Item ::= SEQUENCE
OCUDUASN_CODE mcmrb_setup_cfg_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qos_flow_level_qos_params_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(mbs_pdcp_cfg.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, qos_flow_qos_param_list, 1, 64, true));
  if (qos_flow_level_qos_params_present) {
    HANDLE_CODE(qos_flow_level_qos_params.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcmrb_setup_cfg_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(qos_flow_level_qos_params_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(mbs_pdcp_cfg.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(qos_flow_qos_param_list, bref, 1, 64, true));
  if (qos_flow_level_qos_params_present) {
    HANDLE_CODE(qos_flow_level_qos_params.unpack(bref));
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
void mcmrb_setup_cfg_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  j.write_fieldname("mbs-pdcp-config");
  mbs_pdcp_cfg.to_json(j);
  j.start_array("qoS-Flow-QoS-Parameter-List");
  for (const auto& e1 : qos_flow_qos_param_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (qos_flow_level_qos_params_present) {
    j.write_fieldname("qoSFlowLevelQoSParameters");
    qos_flow_level_qos_params.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MRBForwardingResourceResponse-Item ::= SEQUENCE
OCUDUASN_CODE mrb_forwarding_res_resp_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mrb_progress_info_present, 1));
  HANDLE_CODE(bref.pack(mrb_forwarding_address_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  if (mrb_progress_info_present) {
    HANDLE_CODE(mrb_progress_info.pack(bref));
  }
  if (mrb_forwarding_address_present) {
    HANDLE_CODE(mrb_forwarding_address.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrb_forwarding_res_resp_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mrb_progress_info_present, 1));
  HANDLE_CODE(bref.unpack(mrb_forwarding_address_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  if (mrb_progress_info_present) {
    HANDLE_CODE(mrb_progress_info.unpack(bref));
  }
  if (mrb_forwarding_address_present) {
    HANDLE_CODE(mrb_forwarding_address.unpack(bref));
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
void mrb_forwarding_res_resp_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  if (mrb_progress_info_present) {
    j.write_fieldname("mrb-ProgressInformation");
    mrb_progress_info.to_json(j);
  }
  if (mrb_forwarding_address_present) {
    j.write_fieldname("mrbForwardingAddress");
    mrb_forwarding_address.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCForwardingResourceResponse ::= SEQUENCE
OCUDUASN_CODE mc_forwarding_res_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mrb_forwarding_res_resp_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.pack(bref));
  if (mrb_forwarding_res_resp_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mrb_forwarding_res_resp_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_forwarding_res_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool mrb_forwarding_res_resp_list_present;
  HANDLE_CODE(bref.unpack(mrb_forwarding_res_resp_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mc_forwarding_res_id.unpack(bref));
  if (mrb_forwarding_res_resp_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mrb_forwarding_res_resp_list, bref, 1, 64, true));
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
void mc_forwarding_res_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mcForwardingResourceID", mc_forwarding_res_id.to_string());
  if (mrb_forwarding_res_resp_list.size() > 0) {
    j.start_array("mrbForwardingResourceResponseList");
    for (const auto& e1 : mrb_forwarding_res_resp_list) {
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

// MCBearerContextToModifyResponse-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t mc_bearer_context_to_modify_resp_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {181};
  return map_enum_number(names, 1, idx, "id");
}
bool mc_bearer_context_to_modify_resp_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 181 == id;
}
crit_e mc_bearer_context_to_modify_resp_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 181) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e mc_bearer_context_to_modify_resp_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 181) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE mc_bearer_context_to_modify_resp_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += mc_forwarding_res_resp_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (mc_forwarding_res_resp_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)181, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mc_forwarding_res_resp.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_modify_resp_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != mc_bearer_context_to_modify_resp_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 181) {
      {
        mc_forwarding_res_resp_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mc_forwarding_res_resp.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_to_modify_resp_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (mc_forwarding_res_resp_present) {
    j.start_obj();
    j.write_int("id", 181);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mc_forwarding_res_resp.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MCBearerContextToModifyResponse ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_to_modify_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp_present, 1));
  HANDLE_CODE(bref.pack(mbs_multicast_f1_u_context_descriptor_present, 1));
  HANDLE_CODE(bref.pack(mc_mrb_modify_setup_resp_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(mc_mrb_failed_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(available_mcmrb_cfg.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp_present) {
    HANDLE_CODE(mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp.pack(bref));
  }
  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  if (mc_mrb_modify_setup_resp_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_modify_setup_resp_list, 1, 32, true));
  }
  if (mc_mrb_failed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_failed_list, 1, 32, true));
  }
  if (available_mcmrb_cfg.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, available_mcmrb_cfg, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_modify_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp_present, 1));
  HANDLE_CODE(bref.unpack(mbs_multicast_f1_u_context_descriptor_present, 1));
  bool mc_mrb_modify_setup_resp_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_modify_setup_resp_list_present, 1));
  bool mc_mrb_failed_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_failed_list_present, 1));
  bool available_mcmrb_cfg_present;
  HANDLE_CODE(bref.unpack(available_mcmrb_cfg_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp_present) {
    HANDLE_CODE(mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp.unpack(bref));
  }
  if (mbs_multicast_f1_u_context_descriptor_present) {
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
  }
  if (mc_mrb_modify_setup_resp_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_modify_setup_resp_list, bref, 1, 32, true));
  }
  if (mc_mrb_failed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_failed_list, bref, 1, 32, true));
  }
  if (available_mcmrb_cfg_present) {
    HANDLE_CODE(unpack_dyn_seq_of(available_mcmrb_cfg, bref, 1, 32, true));
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
void mc_bearer_context_to_modify_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp_present) {
    j.write_fieldname("mcBearerContextNGU-TNLInfoatNGRANModifyResponse");
    mc_bearer_context_ngu_tnl_infoat_ngran_modify_resp.to_json(j);
  }
  if (mbs_multicast_f1_u_context_descriptor_present) {
    j.write_fieldname("mbsMulticastF1UContextDescriptor");
    mbs_multicast_f1_u_context_descriptor.to_json(j);
  }
  if (mc_mrb_modify_setup_resp_list.size() > 0) {
    j.start_array("mcMRBModifySetupResponseList");
    for (const auto& e1 : mc_mrb_modify_setup_resp_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mc_mrb_failed_list.size() > 0) {
    j.start_array("mcMRBFailedList");
    for (const auto& e1 : mc_mrb_failed_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (available_mcmrb_cfg.size() > 0) {
    j.start_array("availableMCMRBConfig");
    for (const auto& e1 : available_mcmrb_cfg) {
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

// MCBearerContextToSetup-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t mc_bearer_context_to_setup_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {185, 189, 192, 193};
  return map_enum_number(names, 4, idx, "id");
}
bool mc_bearer_context_to_setup_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {185, 189, 192, 193};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mc_bearer_context_to_setup_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 185:
      return crit_e::ignore;
    case 189:
      return crit_e::ignore;
    case 192:
      return crit_e::ignore;
    case 193:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mc_bearer_context_to_setup_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 185:
      return presence_e::optional;
    case 189:
      return presence_e::optional;
    case 192:
      return presence_e::optional;
    case 193:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mc_bearer_context_to_setup_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += mbs_session_associated_info_non_support_to_support_present ? 1 : 0;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  nof_ies += mc_bearer_context_inactivity_timer_present ? 1 : 0;
  nof_ies += mc_bearer_context_status_change_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (mbs_session_associated_info_non_support_to_support_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)185, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_session_associated_info_non_support_to_support.pack(bref));
  }
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)189, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true));
  }
  if (mc_bearer_context_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)192, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, mc_bearer_context_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (mc_bearer_context_status_change_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)193, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mc_bearer_context_status_change.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_setup_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != mc_bearer_context_to_setup_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 185: {
        mbs_session_associated_info_non_support_to_support_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_session_associated_info_non_support_to_support.unpack(bref));
        break;
      }
      case 189: {
        mbs_area_session_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true));
        break;
      }
      case 192: {
        mc_bearer_context_inactivity_timer_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(
            unpack_integer(mc_bearer_context_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
        break;
      }
      case 193: {
        mc_bearer_context_status_change_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mc_bearer_context_status_change.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void mc_bearer_context_to_setup_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (mbs_session_associated_info_non_support_to_support_present) {
    j.start_obj();
    j.write_int("id", 185);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mbs_session_associated_info_non_support_to_support.to_json(j);
    j.end_obj();
  }
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 189);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", mbs_area_session_id);
    j.end_obj();
  }
  if (mc_bearer_context_inactivity_timer_present) {
    j.start_obj();
    j.write_int("id", 192);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", mc_bearer_context_inactivity_timer);
    j.end_obj();
  }
  if (mc_bearer_context_status_change_present) {
    j.start_obj();
    j.write_int("id", 193);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", mc_bearer_context_status_change.to_string());
    j.end_obj();
  }
  j.end_array();
}

// MCBearerContextToSetup ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_to_setup_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mc_mrb_to_setup_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(requested_action_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(snssai.pack(bref));
  if (mc_mrb_to_setup_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_to_setup_list, 1, 32, true));
  }
  if (requested_action_present) {
    HANDLE_CODE(requested_action.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_setup_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool mc_mrb_to_setup_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_to_setup_list_present, 1));
  HANDLE_CODE(bref.unpack(requested_action_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(snssai.unpack(bref));
  if (mc_mrb_to_setup_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_to_setup_list, bref, 1, 32, true));
  }
  if (requested_action_present) {
    HANDLE_CODE(requested_action.unpack(bref));
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
void mc_bearer_context_to_setup_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("snssai");
  snssai.to_json(j);
  if (mc_mrb_to_setup_list.size() > 0) {
    j.start_array("mcMRBToSetupList");
    for (const auto& e1 : mc_mrb_to_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (requested_action_present) {
    j.write_str("requestedAction", requested_action.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCMRBSetupResponseList-Item ::= SEQUENCE
OCUDUASN_CODE mcmrb_setup_resp_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(qosflow_failed.size() > 0, 1));
  HANDLE_CODE(bref.pack(mbs_pdcp_count_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mrb_id, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, qosflow_setup, 1, 64, true));
  if (qosflow_failed.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, qosflow_failed, 1, 64, true));
  }
  if (mbs_pdcp_count_present) {
    HANDLE_CODE(mbs_pdcp_count.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mcmrb_setup_resp_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool qosflow_failed_present;
  HANDLE_CODE(bref.unpack(qosflow_failed_present, 1));
  HANDLE_CODE(bref.unpack(mbs_pdcp_count_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mrb_id, bref, (uint16_t)1u, (uint16_t)512u, true, true));
  HANDLE_CODE(unpack_dyn_seq_of(qosflow_setup, bref, 1, 64, true));
  if (qosflow_failed_present) {
    HANDLE_CODE(unpack_dyn_seq_of(qosflow_failed, bref, 1, 64, true));
  }
  if (mbs_pdcp_count_present) {
    HANDLE_CODE(mbs_pdcp_count.unpack(bref));
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
void mcmrb_setup_resp_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mrb-ID", mrb_id);
  j.start_array("qosflow-setup");
  for (const auto& e1 : qosflow_setup) {
    e1.to_json(j);
  }
  j.end_array();
  if (qosflow_failed.size() > 0) {
    j.start_array("qosflow-failed");
    for (const auto& e1 : qosflow_failed) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mbs_pdcp_count_present) {
    j.write_fieldname("mBS-PDCP-COUNT");
    mbs_pdcp_count.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MCBearerContextToSetupResponse ::= SEQUENCE
OCUDUASN_CODE mc_bearer_context_to_setup_resp_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(mc_bearer_context_ngu_tnl_infoat_ngran_present, 1));
  HANDLE_CODE(bref.pack(mc_mrb_setup_resp_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(mc_mrb_failed_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(available_mcmrb_cfg.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (mc_bearer_context_ngu_tnl_infoat_ngran_present) {
    HANDLE_CODE(mc_bearer_context_ngu_tnl_infoat_ngran.pack(bref));
  }
  if (mc_mrb_setup_resp_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_setup_resp_list, 1, 32, true));
  }
  if (mc_mrb_failed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_mrb_failed_list, 1, 32, true));
  }
  if (available_mcmrb_cfg.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, available_mcmrb_cfg, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mc_bearer_context_to_setup_resp_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(mc_bearer_context_ngu_tnl_infoat_ngran_present, 1));
  bool mc_mrb_setup_resp_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_setup_resp_list_present, 1));
  bool mc_mrb_failed_list_present;
  HANDLE_CODE(bref.unpack(mc_mrb_failed_list_present, 1));
  bool available_mcmrb_cfg_present;
  HANDLE_CODE(bref.unpack(available_mcmrb_cfg_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (mc_bearer_context_ngu_tnl_infoat_ngran_present) {
    HANDLE_CODE(mc_bearer_context_ngu_tnl_infoat_ngran.unpack(bref));
  }
  if (mc_mrb_setup_resp_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_setup_resp_list, bref, 1, 32, true));
  }
  if (mc_mrb_failed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(mc_mrb_failed_list, bref, 1, 32, true));
  }
  if (available_mcmrb_cfg_present) {
    HANDLE_CODE(unpack_dyn_seq_of(available_mcmrb_cfg, bref, 1, 32, true));
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
void mc_bearer_context_to_setup_resp_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (mc_bearer_context_ngu_tnl_infoat_ngran_present) {
    j.write_fieldname("mcBearerContextNGU-TNLInfoatNGRAN");
    mc_bearer_context_ngu_tnl_infoat_ngran.to_json(j);
  }
  if (mc_mrb_setup_resp_list.size() > 0) {
    j.start_array("mcMRBSetupResponseList");
    for (const auto& e1 : mc_mrb_setup_resp_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (mc_mrb_failed_list.size() > 0) {
    j.start_array("mcMRBFailedList");
    for (const auto& e1 : mc_mrb_failed_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (available_mcmrb_cfg.size() > 0) {
    j.start_array("availableMCMRBConfig");
    for (const auto& e1 : available_mcmrb_cfg) {
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

// MDT-Activation ::= ENUMERATED
const char* mdt_activation_opts::to_string() const
{
  static const char* names[] = {"immediate-MDT-only", "immediate-MDT-and-Trace"};
  return convert_enum_idx(names, 2, value, "mdt_activation_e");
}

// MDTMode ::= CHOICE
void mdt_mode_c::destroy_()
{
  switch (type_) {
    case types::immediate_mdt:
      c.destroy<immediate_mdt_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<mdt_mode_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void mdt_mode_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::immediate_mdt:
      c.init<immediate_mdt_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<mdt_mode_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mdt_mode_c");
  }
}
mdt_mode_c::mdt_mode_c(const mdt_mode_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::immediate_mdt:
      c.init(other.c.get<immediate_mdt_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<mdt_mode_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mdt_mode_c");
  }
}
mdt_mode_c& mdt_mode_c::operator=(const mdt_mode_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::immediate_mdt:
      c.set(other.c.get<immediate_mdt_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<mdt_mode_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mdt_mode_c");
  }

  return *this;
}
immediate_mdt_s& mdt_mode_c::set_immediate_mdt()
{
  set(types::immediate_mdt);
  return c.get<immediate_mdt_s>();
}
protocol_ie_single_container_s<mdt_mode_ext_ies_o>& mdt_mode_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<mdt_mode_ext_ies_o>>();
}
void mdt_mode_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::immediate_mdt:
      j.write_fieldname("immediateMDT");
      c.get<immediate_mdt_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<mdt_mode_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "mdt_mode_c");
  }
  j.end_obj();
}
OCUDUASN_CODE mdt_mode_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::immediate_mdt:
      HANDLE_CODE(c.get<immediate_mdt_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mdt_mode_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mdt_mode_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mdt_mode_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::immediate_mdt:
      HANDLE_CODE(c.get<immediate_mdt_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<mdt_mode_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "mdt_mode_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mdt_mode_c::types_opts::to_string() const
{
  static const char* names[] = {"immediateMDT", "choice-extension"};
  return convert_enum_idx(names, 2, value, "mdt_mode_c::types");
}

// MDT-Configuration ::= SEQUENCE
OCUDUASN_CODE mdt_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(mdt_activation.pack(bref));
  HANDLE_CODE(mdt_mode.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mdt_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(mdt_activation.unpack(bref));
  HANDLE_CODE(mdt_mode.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mdt_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mdt-Activation", mdt_activation.to_string());
  j.write_fieldname("mDTMode");
  mdt_mode.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MDTPollutedMeasurementIndicator ::= ENUMERATED
const char* mdt_polluted_meas_ind_opts::to_string() const
{
  static const char* names[] = {"iDC", "no-IDC"};
  return convert_enum_idx(names, 2, value, "mdt_polluted_meas_ind_e");
}

// MRDC-Usage-Information ::= SEQUENCE
OCUDUASN_CODE mrdc_usage_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(data_usage_per_pdu_session_report_present, 1));
  HANDLE_CODE(bref.pack(data_usage_per_qos_flow_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (data_usage_per_pdu_session_report_present) {
    HANDLE_CODE(data_usage_per_pdu_session_report.pack(bref));
  }
  if (data_usage_per_qos_flow_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, data_usage_per_qos_flow_list, 1, 64, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mrdc_usage_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(data_usage_per_pdu_session_report_present, 1));
  bool data_usage_per_qos_flow_list_present;
  HANDLE_CODE(bref.unpack(data_usage_per_qos_flow_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (data_usage_per_pdu_session_report_present) {
    HANDLE_CODE(data_usage_per_pdu_session_report.unpack(bref));
  }
  if (data_usage_per_qos_flow_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(data_usage_per_qos_flow_list, bref, 1, 64, true));
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
void mrdc_usage_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (data_usage_per_pdu_session_report_present) {
    j.write_fieldname("data-Usage-per-PDU-Session-Report");
    data_usage_per_pdu_session_report.to_json(j);
  }
  if (data_usage_per_qos_flow_list.size() > 0) {
    j.start_array("data-Usage-per-QoS-Flow-List");
    for (const auto& e1 : data_usage_per_qos_flow_list) {
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

// MT-SDT-Information ::= SEQUENCE
OCUDUASN_CODE mt_sdt_info_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, mt_sdt_data_size, (uint32_t)1u, (uint32_t)96000u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mt_sdt_info_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(mt_sdt_data_size, bref, (uint32_t)1u, (uint32_t)96000u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void mt_sdt_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mT-SDT-Data-Size", mt_sdt_data_size);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// MT-SDT-Information-Request ::= ENUMERATED
const char* mt_sdt_info_request_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "mt_sdt_info_request_e");
}

// NG-RAN-QoS-Support-Item ::= SEQUENCE
OCUDUASN_CODE ng_ran_qos_support_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(non_dyn_5qi_descriptor.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_ran_qos_support_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(non_dyn_5qi_descriptor.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ng_ran_qos_support_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("non-Dynamic5QIDescriptor");
  non_dyn_5qi_descriptor.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NPNContextInfo-SNPN ::= SEQUENCE
OCUDUASN_CODE npn_context_info_sn_pn_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(nid.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE npn_context_info_sn_pn_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(nid.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void npn_context_info_sn_pn_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("nID");
  nid.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NPNContextInfo ::= CHOICE
void npn_context_info_c::destroy_()
{
  switch (type_) {
    case types::sn_pn:
      c.destroy<npn_context_info_sn_pn_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<npn_context_info_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void npn_context_info_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::sn_pn:
      c.init<npn_context_info_sn_pn_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<npn_context_info_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "npn_context_info_c");
  }
}
npn_context_info_c::npn_context_info_c(const npn_context_info_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::sn_pn:
      c.init(other.c.get<npn_context_info_sn_pn_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<npn_context_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "npn_context_info_c");
  }
}
npn_context_info_c& npn_context_info_c::operator=(const npn_context_info_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::sn_pn:
      c.set(other.c.get<npn_context_info_sn_pn_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<npn_context_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "npn_context_info_c");
  }

  return *this;
}
npn_context_info_sn_pn_s& npn_context_info_c::set_sn_pn()
{
  set(types::sn_pn);
  return c.get<npn_context_info_sn_pn_s>();
}
protocol_ie_single_container_s<npn_context_info_ext_ies_o>& npn_context_info_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<npn_context_info_ext_ies_o>>();
}
void npn_context_info_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::sn_pn:
      j.write_fieldname("sNPN");
      c.get<npn_context_info_sn_pn_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<npn_context_info_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "npn_context_info_c");
  }
  j.end_obj();
}
OCUDUASN_CODE npn_context_info_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::sn_pn:
      HANDLE_CODE(c.get<npn_context_info_sn_pn_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<npn_context_info_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "npn_context_info_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE npn_context_info_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::sn_pn:
      HANDLE_CODE(c.get<npn_context_info_sn_pn_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<npn_context_info_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "npn_context_info_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* npn_context_info_c::types_opts::to_string() const
{
  static const char* names[] = {"sNPN", "choice-extension"};
  return convert_enum_idx(names, 2, value, "npn_context_info_c::types");
}

// NPNSupportInfo-SNPN ::= SEQUENCE
OCUDUASN_CODE npn_support_info_sn_pn_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(nid.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE npn_support_info_sn_pn_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(nid.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void npn_support_info_sn_pn_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("nID");
  nid.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// NPNSupportInfo ::= CHOICE
void npn_support_info_c::destroy_()
{
  switch (type_) {
    case types::sn_pn:
      c.destroy<npn_support_info_sn_pn_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void npn_support_info_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::sn_pn:
      c.init<npn_support_info_sn_pn_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "npn_support_info_c");
  }
}
npn_support_info_c::npn_support_info_c(const npn_support_info_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::sn_pn:
      c.init(other.c.get<npn_support_info_sn_pn_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "npn_support_info_c");
  }
}
npn_support_info_c& npn_support_info_c::operator=(const npn_support_info_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::sn_pn:
      c.set(other.c.get<npn_support_info_sn_pn_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "npn_support_info_c");
  }

  return *this;
}
npn_support_info_sn_pn_s& npn_support_info_c::set_sn_pn()
{
  set(types::sn_pn);
  return c.get<npn_support_info_sn_pn_s>();
}
protocol_ie_single_container_s<npn_support_info_ext_ies_o>& npn_support_info_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>();
}
void npn_support_info_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::sn_pn:
      j.write_fieldname("sNPN");
      c.get<npn_support_info_sn_pn_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "npn_support_info_c");
  }
  j.end_obj();
}
OCUDUASN_CODE npn_support_info_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::sn_pn:
      HANDLE_CODE(c.get<npn_support_info_sn_pn_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "npn_support_info_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE npn_support_info_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::sn_pn:
      HANDLE_CODE(c.get<npn_support_info_sn_pn_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<npn_support_info_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "npn_support_info_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* npn_support_info_c::types_opts::to_string() const
{
  static const char* names[] = {"sNPN", "choice-extension"};
  return convert_enum_idx(names, 2, value, "npn_support_info_c::types");
}

// NR-CGI-Support-Item ::= SEQUENCE
OCUDUASN_CODE nr_cgi_support_item_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE nr_cgi_support_item_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(nr_cgi.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void nr_cgi_support_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("nR-CGI");
  nr_cgi.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// New-UL-TNL-Information-Required ::= ENUMERATED
const char* new_ul_tnl_info_required_opts::to_string() const
{
  static const char* names[] = {"required"};
  return convert_enum_idx(names, 1, value, "new_ul_tnl_info_required_e");
}

// PDU-Session-Resource-Confirm-Modified-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_confirm_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(drb_confirm_modified_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (drb_confirm_modified_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_confirm_modified_list_ng_ran, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_confirm_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool drb_confirm_modified_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_confirm_modified_list_ng_ran_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (drb_confirm_modified_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_confirm_modified_list_ng_ran, bref, 1, 32, true));
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
void pdu_session_res_confirm_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  if (drb_confirm_modified_list_ng_ran.size() > 0) {
    j.start_array("dRB-Confirm-Modified-List-NG-RAN");
    for (const auto& e1 : drb_confirm_modified_list_ng_ran) {
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

// PDU-Session-Resource-Data-Usage-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_data_usage_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(mrdc_usage_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_data_usage_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(mrdc_usage_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_data_usage_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.write_fieldname("mRDC-Usage-Information");
  mrdc_usage_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Resource-Failed-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_failed_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_failed_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_failed_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Resource-Failed-Mod-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_failed_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_failed_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_failed_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Resource-Failed-To-Modify-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_failed_to_modify_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_failed_to_modify_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_failed_to_modify_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Resource-Modified-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdu_session_res_modified_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {98};
  return map_enum_number(names, 1, idx, "id");
}
bool pdu_session_res_modified_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 98 == id;
}
crit_e pdu_session_res_modified_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 98) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e pdu_session_res_modified_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 98) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE pdu_session_res_modified_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += redundant_n_g_dl_up_tnl_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (redundant_n_g_dl_up_tnl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)98, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_n_g_dl_up_tnl_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_modified_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdu_session_res_modified_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 98) {
      {
        redundant_n_g_dl_up_tnl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_n_g_dl_up_tnl_info.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_res_modified_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (redundant_n_g_dl_up_tnl_info_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_n_g_dl_up_tnl_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDU-Session-Resource-Modified-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ng_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.pack(security_result_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.pack(drb_setup_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_failed_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_modified_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_failed_to_modify_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (ng_dl_up_tnl_info_present) {
    HANDLE_CODE(ng_dl_up_tnl_info.pack(bref));
  }
  if (security_result_present) {
    HANDLE_CODE(security_result.pack(bref));
  }
  if (pdu_session_data_forwarding_info_resp_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_resp.pack(bref));
  }
  if (drb_setup_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_setup_list_ng_ran, 1, 32, true));
  }
  if (drb_failed_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_failed_list_ng_ran, 1, 32, true));
  }
  if (drb_modified_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_modified_list_ng_ran, 1, 32, true));
  }
  if (drb_failed_to_modify_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_failed_to_modify_list_ng_ran, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ng_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.unpack(security_result_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_data_forwarding_info_resp_present, 1));
  bool drb_setup_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_setup_list_ng_ran_present, 1));
  bool drb_failed_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_failed_list_ng_ran_present, 1));
  bool drb_modified_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_modified_list_ng_ran_present, 1));
  bool drb_failed_to_modify_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_failed_to_modify_list_ng_ran_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (ng_dl_up_tnl_info_present) {
    HANDLE_CODE(ng_dl_up_tnl_info.unpack(bref));
  }
  if (security_result_present) {
    HANDLE_CODE(security_result.unpack(bref));
  }
  if (pdu_session_data_forwarding_info_resp_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_resp.unpack(bref));
  }
  if (drb_setup_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_setup_list_ng_ran, bref, 1, 32, true));
  }
  if (drb_failed_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_failed_list_ng_ran, bref, 1, 32, true));
  }
  if (drb_modified_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_modified_list_ng_ran, bref, 1, 32, true));
  }
  if (drb_failed_to_modify_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_failed_to_modify_list_ng_ran, bref, 1, 32, true));
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
void pdu_session_res_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  if (ng_dl_up_tnl_info_present) {
    j.write_fieldname("nG-DL-UP-TNL-Information");
    ng_dl_up_tnl_info.to_json(j);
  }
  if (security_result_present) {
    j.write_fieldname("securityResult");
    security_result.to_json(j);
  }
  if (pdu_session_data_forwarding_info_resp_present) {
    j.write_fieldname("pDU-Session-Data-Forwarding-Information-Response");
    pdu_session_data_forwarding_info_resp.to_json(j);
  }
  if (drb_setup_list_ng_ran.size() > 0) {
    j.start_array("dRB-Setup-List-NG-RAN");
    for (const auto& e1 : drb_setup_list_ng_ran) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_failed_list_ng_ran.size() > 0) {
    j.start_array("dRB-Failed-List-NG-RAN");
    for (const auto& e1 : drb_failed_list_ng_ran) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_modified_list_ng_ran.size() > 0) {
    j.start_array("dRB-Modified-List-NG-RAN");
    for (const auto& e1 : drb_modified_list_ng_ran) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_failed_to_modify_list_ng_ran.size() > 0) {
    j.start_array("dRB-Failed-To-Modify-List-NG-RAN");
    for (const auto& e1 : drb_failed_to_modify_list_ng_ran) {
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

// PDU-Session-Resource-Required-To-Modify-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdu_session_res_required_to_modify_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {98};
  return map_enum_number(names, 1, idx, "id");
}
bool pdu_session_res_required_to_modify_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 98 == id;
}
crit_e pdu_session_res_required_to_modify_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 98) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e pdu_session_res_required_to_modify_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 98) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE pdu_session_res_required_to_modify_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += redundant_n_g_dl_up_tnl_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (redundant_n_g_dl_up_tnl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)98, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_n_g_dl_up_tnl_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_required_to_modify_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdu_session_res_required_to_modify_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 98) {
      {
        redundant_n_g_dl_up_tnl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_n_g_dl_up_tnl_info.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_res_required_to_modify_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (redundant_n_g_dl_up_tnl_info_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_n_g_dl_up_tnl_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDU-Session-Resource-Required-To-Modify-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_required_to_modify_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ng_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.pack(drb_required_to_modify_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_required_to_rem_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (ng_dl_up_tnl_info_present) {
    HANDLE_CODE(ng_dl_up_tnl_info.pack(bref));
  }
  if (drb_required_to_modify_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_required_to_modify_list_ng_ran, 1, 32, true));
  }
  if (drb_required_to_rem_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_required_to_rem_list_ng_ran, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_required_to_modify_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ng_dl_up_tnl_info_present, 1));
  bool drb_required_to_modify_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_required_to_modify_list_ng_ran_present, 1));
  bool drb_required_to_rem_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_required_to_rem_list_ng_ran_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (ng_dl_up_tnl_info_present) {
    HANDLE_CODE(ng_dl_up_tnl_info.unpack(bref));
  }
  if (drb_required_to_modify_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_required_to_modify_list_ng_ran, bref, 1, 32, true));
  }
  if (drb_required_to_rem_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_required_to_rem_list_ng_ran, bref, 1, 32, true));
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
void pdu_session_res_required_to_modify_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  if (ng_dl_up_tnl_info_present) {
    j.write_fieldname("nG-DL-UP-TNL-Information");
    ng_dl_up_tnl_info.to_json(j);
  }
  if (drb_required_to_modify_list_ng_ran.size() > 0) {
    j.start_array("dRB-Required-To-Modify-List-NG-RAN");
    for (const auto& e1 : drb_required_to_modify_list_ng_ran) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_required_to_rem_list_ng_ran.size() > 0) {
    j.start_array("dRB-Required-To-Remove-List-NG-RAN");
    for (const auto& e1 : drb_required_to_rem_list_ng_ran) {
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

// RSN ::= ENUMERATED
const char* r_sn_opts::to_string() const
{
  static const char* names[] = {"v1", "v2"};
  return convert_enum_idx(names, 2, value, "r_sn_e");
}
uint8_t r_sn_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "r_sn_e");
}

// RedundantPDUSessionInformation-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t redundant_pdu_session_info_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {151};
  return map_enum_number(names, 1, idx, "id");
}
bool redundant_pdu_session_info_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 151 == id;
}
crit_e redundant_pdu_session_info_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 151) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e redundant_pdu_session_info_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 151) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE redundant_pdu_session_info_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pdu_session_pair_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (pdu_session_pair_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)151, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, pdu_session_pair_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE redundant_pdu_session_info_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != redundant_pdu_session_info_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 151) {
      {
        pdu_session_pair_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(pdu_session_pair_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void redundant_pdu_session_info_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (pdu_session_pair_id_present) {
    j.start_obj();
    j.write_int("id", 151);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", pdu_session_pair_id);
    j.end_obj();
  }
  j.end_array();
}

// RedundantPDUSessionInformation ::= SEQUENCE
OCUDUASN_CODE redundant_pdu_session_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(rsn.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE redundant_pdu_session_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(rsn.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void redundant_pdu_session_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("rSN", rsn.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Resource-Setup-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdu_session_res_setup_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {98, 106};
  return map_enum_number(names, 2, idx, "id");
}
bool pdu_session_res_setup_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {98, 106};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_setup_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 98:
      return crit_e::ignore;
    case 106:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_setup_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 98:
      return presence_e::optional;
    case 106:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_setup_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += redundant_n_g_dl_up_tnl_info_present ? 1 : 0;
  nof_ies += redundant_pdu_session_info_used_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (redundant_n_g_dl_up_tnl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)98, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_n_g_dl_up_tnl_info.pack(bref));
  }
  if (redundant_pdu_session_info_used_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)106, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_pdu_session_info_used.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_setup_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdu_session_res_setup_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 98: {
        redundant_n_g_dl_up_tnl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_n_g_dl_up_tnl_info.unpack(bref));
        break;
      }
      case 106: {
        redundant_pdu_session_info_used_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_pdu_session_info_used.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_res_setup_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (redundant_n_g_dl_up_tnl_info_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_n_g_dl_up_tnl_info.to_json(j);
    j.end_obj();
  }
  if (redundant_pdu_session_info_used_present) {
    j.start_obj();
    j.write_int("id", 106);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_pdu_session_info_used.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDU-Session-Resource-Setup-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_setup_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(security_result_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.pack(ng_dl_up_unchanged_present, 1));
  HANDLE_CODE(bref.pack(drb_failed_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (security_result_present) {
    HANDLE_CODE(security_result.pack(bref));
  }
  HANDLE_CODE(ng_dl_up_tnl_info.pack(bref));
  if (pdu_session_data_forwarding_info_resp_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_resp.pack(bref));
  }
  if (ng_dl_up_unchanged_present) {
    HANDLE_CODE(ng_dl_up_unchanged.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, drb_setup_list_ng_ran, 1, 32, true));
  if (drb_failed_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_failed_list_ng_ran, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_setup_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(security_result_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.unpack(ng_dl_up_unchanged_present, 1));
  bool drb_failed_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_failed_list_ng_ran_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (security_result_present) {
    HANDLE_CODE(security_result.unpack(bref));
  }
  HANDLE_CODE(ng_dl_up_tnl_info.unpack(bref));
  if (pdu_session_data_forwarding_info_resp_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_resp.unpack(bref));
  }
  if (ng_dl_up_unchanged_present) {
    HANDLE_CODE(ng_dl_up_unchanged.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(drb_setup_list_ng_ran, bref, 1, 32, true));
  if (drb_failed_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_failed_list_ng_ran, bref, 1, 32, true));
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
void pdu_session_res_setup_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  if (security_result_present) {
    j.write_fieldname("securityResult");
    security_result.to_json(j);
  }
  j.write_fieldname("nG-DL-UP-TNL-Information");
  ng_dl_up_tnl_info.to_json(j);
  if (pdu_session_data_forwarding_info_resp_present) {
    j.write_fieldname("pDU-Session-Data-Forwarding-Information-Response");
    pdu_session_data_forwarding_info_resp.to_json(j);
  }
  if (ng_dl_up_unchanged_present) {
    j.write_str("nG-DL-UP-Unchanged", "true");
  }
  j.start_array("dRB-Setup-List-NG-RAN");
  for (const auto& e1 : drb_setup_list_ng_ran) {
    e1.to_json(j);
  }
  j.end_array();
  if (drb_failed_list_ng_ran.size() > 0) {
    j.start_array("dRB-Failed-List-NG-RAN");
    for (const auto& e1 : drb_failed_list_ng_ran) {
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

const char* pdu_session_res_setup_item_s::ng_dl_up_unchanged_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "pdu_session_res_setup_item_s::ng_dl_up_unchanged_e_");
}

// PDU-Session-Resource-Setup-Mod-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdu_session_res_setup_mod_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {98};
  return map_enum_number(names, 1, idx, "id");
}
bool pdu_session_res_setup_mod_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 98 == id;
}
crit_e pdu_session_res_setup_mod_item_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 98) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e pdu_session_res_setup_mod_item_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 98) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE pdu_session_res_setup_mod_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += redundant_n_g_dl_up_tnl_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (redundant_n_g_dl_up_tnl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)98, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_n_g_dl_up_tnl_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_setup_mod_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdu_session_res_setup_mod_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 98) {
      {
        redundant_n_g_dl_up_tnl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_n_g_dl_up_tnl_info.unpack(bref));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
      break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_res_setup_mod_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (redundant_n_g_dl_up_tnl_info_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_n_g_dl_up_tnl_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDU-Session-Resource-Setup-Mod-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_setup_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(security_result_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_data_forwarding_info_resp_present, 1));
  HANDLE_CODE(bref.pack(drb_failed_mod_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (security_result_present) {
    HANDLE_CODE(security_result.pack(bref));
  }
  HANDLE_CODE(ng_dl_up_tnl_info.pack(bref));
  if (pdu_session_data_forwarding_info_resp_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_resp.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, drb_setup_mod_list_ng_ran, 1, 32, true));
  if (drb_failed_mod_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_failed_mod_list_ng_ran, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_setup_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(security_result_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_data_forwarding_info_resp_present, 1));
  bool drb_failed_mod_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_failed_mod_list_ng_ran_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (security_result_present) {
    HANDLE_CODE(security_result.unpack(bref));
  }
  HANDLE_CODE(ng_dl_up_tnl_info.unpack(bref));
  if (pdu_session_data_forwarding_info_resp_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_resp.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(drb_setup_mod_list_ng_ran, bref, 1, 32, true));
  if (drb_failed_mod_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_failed_mod_list_ng_ran, bref, 1, 32, true));
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
void pdu_session_res_setup_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  if (security_result_present) {
    j.write_fieldname("securityResult");
    security_result.to_json(j);
  }
  j.write_fieldname("nG-DL-UP-TNL-Information");
  ng_dl_up_tnl_info.to_json(j);
  if (pdu_session_data_forwarding_info_resp_present) {
    j.write_fieldname("pDU-Session-Data-Forwarding-Information-Response");
    pdu_session_data_forwarding_info_resp.to_json(j);
  }
  j.start_array("dRB-Setup-Mod-List-NG-RAN");
  for (const auto& e1 : drb_setup_mod_list_ng_ran) {
    e1.to_json(j);
  }
  j.end_array();
  if (drb_failed_mod_list_ng_ran.size() > 0) {
    j.start_array("dRB-Failed-Mod-List-NG-RAN");
    for (const auto& e1 : drb_failed_mod_list_ng_ran) {
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

// UserPlaneFailureType ::= ENUMERATED
const char* user_plane_fail_type_opts::to_string() const
{
  static const char* names[] = {"gtp-u-error-indication-received", "up-path-failure"};
  return convert_enum_idx(names, 2, value, "user_plane_fail_type_e");
}

// UserPlaneFailureIndication ::= SEQUENCE
OCUDUASN_CODE user_plane_fail_ind_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(user_plane_fail_type.pack(bref));
  HANDLE_CODE(ng_dl_up_tnl_info.pack(bref));
  HANDLE_CODE(ng_ul_up_tnl_info.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE user_plane_fail_ind_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(user_plane_fail_type.unpack(bref));
  HANDLE_CODE(ng_dl_up_tnl_info.unpack(bref));
  HANDLE_CODE(ng_ul_up_tnl_info.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void user_plane_fail_ind_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("userPlaneFailureType", user_plane_fail_type.to_string());
  j.write_fieldname("nG-DL-UP-TNL-Information");
  ng_dl_up_tnl_info.to_json(j);
  j.write_fieldname("nG-UL-UP-TNL-Information");
  ng_ul_up_tnl_info.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Resource-To-Modify-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdu_session_res_to_modify_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {69, 78, 97, 96, 131, 143, 190, 219, 198};
  return map_enum_number(names, 9, idx, "id");
}
bool pdu_session_res_to_modify_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {69, 78, 97, 96, 131, 143, 190, 219, 198};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_to_modify_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 69:
      return crit_e::reject;
    case 78:
      return crit_e::ignore;
    case 97:
      return crit_e::ignore;
    case 96:
      return crit_e::ignore;
    case 131:
      return crit_e::ignore;
    case 143:
      return crit_e::ignore;
    case 190:
      return crit_e::ignore;
    case 219:
      return crit_e::ignore;
    case 198:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_to_modify_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 69:
      return presence_e::optional;
    case 78:
      return presence_e::optional;
    case 97:
      return presence_e::optional;
    case 96:
      return presence_e::optional;
    case 131:
      return presence_e::optional;
    case 143:
      return presence_e::optional;
    case 190:
      return presence_e::optional;
    case 219:
      return presence_e::optional;
    case 198:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_to_modify_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += snssai_present ? 1 : 0;
  nof_ies += common_network_instance_present ? 1 : 0;
  nof_ies += redundant_n_g_ul_up_tnl_info_present ? 1 : 0;
  nof_ies += redundant_common_network_instance_present ? 1 : 0;
  nof_ies += data_forwardingto_e_utran_info_list_present ? 1 : 0;
  nof_ies += security_ind_modify_present ? 1 : 0;
  nof_ies += secondary_pdu_session_data_forwarding_info_present ? 1 : 0;
  nof_ies += user_plane_fail_ind_present ? 1 : 0;
  nof_ies += special_trigger_purpose_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (snssai_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)69, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(snssai.pack(bref));
  }
  if (common_network_instance_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(common_network_instance.pack(bref));
  }
  if (redundant_n_g_ul_up_tnl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)97, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_n_g_ul_up_tnl_info.pack(bref));
  }
  if (redundant_common_network_instance_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)96, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_common_network_instance.pack(bref));
  }
  if (data_forwardingto_e_utran_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)131, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, data_forwardingto_e_utran_info_list, 1, 256, true));
  }
  if (security_ind_modify_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)143, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(security_ind_modify.pack(bref));
  }
  if (secondary_pdu_session_data_forwarding_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)190, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(secondary_pdu_session_data_forwarding_info.pack(bref));
  }
  if (user_plane_fail_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)219, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(user_plane_fail_ind.pack(bref));
  }
  if (special_trigger_purpose_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)198, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(special_trigger_purpose.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_to_modify_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdu_session_res_to_modify_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 69: {
        snssai_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(snssai.unpack(bref));
        break;
      }
      case 78: {
        common_network_instance_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(common_network_instance.unpack(bref));
        break;
      }
      case 97: {
        redundant_n_g_ul_up_tnl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_n_g_ul_up_tnl_info.unpack(bref));
        break;
      }
      case 96: {
        redundant_common_network_instance_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_common_network_instance.unpack(bref));
        break;
      }
      case 131: {
        data_forwardingto_e_utran_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(data_forwardingto_e_utran_info_list, bref, 1, 256, true));
        break;
      }
      case 143: {
        security_ind_modify_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(security_ind_modify.unpack(bref));
        break;
      }
      case 190: {
        secondary_pdu_session_data_forwarding_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(secondary_pdu_session_data_forwarding_info.unpack(bref));
        break;
      }
      case 219: {
        user_plane_fail_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(user_plane_fail_ind.unpack(bref));
        break;
      }
      case 198: {
        special_trigger_purpose_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(special_trigger_purpose.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_res_to_modify_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (snssai_present) {
    j.start_obj();
    j.write_int("id", 69);
    j.write_str("criticality", "reject");
    j.write_fieldname("extensionValue");
    snssai.to_json(j);
    j.end_obj();
  }
  if (common_network_instance_present) {
    j.start_obj();
    j.write_int("id", 78);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", common_network_instance.to_string());
    j.end_obj();
  }
  if (redundant_n_g_ul_up_tnl_info_present) {
    j.start_obj();
    j.write_int("id", 97);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_n_g_ul_up_tnl_info.to_json(j);
    j.end_obj();
  }
  if (redundant_common_network_instance_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", redundant_common_network_instance.to_string());
    j.end_obj();
  }
  if (data_forwardingto_e_utran_info_list_present) {
    j.start_obj();
    j.write_int("id", 131);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : data_forwardingto_e_utran_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (security_ind_modify_present) {
    j.start_obj();
    j.write_int("id", 143);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    security_ind_modify.to_json(j);
    j.end_obj();
  }
  if (secondary_pdu_session_data_forwarding_info_present) {
    j.start_obj();
    j.write_int("id", 190);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    secondary_pdu_session_data_forwarding_info.to_json(j);
    j.end_obj();
  }
  if (user_plane_fail_ind_present) {
    j.start_obj();
    j.write_int("id", 219);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    user_plane_fail_ind.to_json(j);
    j.end_obj();
  }
  if (special_trigger_purpose_present) {
    j.start_obj();
    j.write_int("id", 198);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "indirect-data-forwarding");
    j.end_obj();
  }
  j.end_array();
}

// PDU-Session-Resource-To-Modify-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_to_modify_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(security_ind_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_res_dl_ambr_present, 1));
  HANDLE_CODE(bref.pack(ng_ul_up_tnl_info_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_data_forwarding_info_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(network_instance_present, 1));
  HANDLE_CODE(bref.pack(drb_to_setup_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_to_modify_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(drb_to_rem_list_ng_ran.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (security_ind_present) {
    HANDLE_CODE(security_ind.pack(bref));
  }
  if (pdu_session_res_dl_ambr_present) {
    HANDLE_CODE(pack_integer(bref, pdu_session_res_dl_ambr, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  if (ng_ul_up_tnl_info_present) {
    HANDLE_CODE(ng_ul_up_tnl_info.pack(bref));
  }
  if (pdu_session_data_forwarding_info_request_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_request.pack(bref));
  }
  if (pdu_session_data_forwarding_info_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info.pack(bref));
  }
  if (pdu_session_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, pdu_session_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (network_instance_present) {
    HANDLE_CODE(pack_integer(bref, network_instance, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  if (drb_to_setup_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_to_setup_list_ng_ran, 1, 32, true));
  }
  if (drb_to_modify_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_to_modify_list_ng_ran, 1, 32, true));
  }
  if (drb_to_rem_list_ng_ran.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_to_rem_list_ng_ran, 1, 32, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_to_modify_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(security_ind_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_res_dl_ambr_present, 1));
  HANDLE_CODE(bref.unpack(ng_ul_up_tnl_info_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_data_forwarding_info_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(network_instance_present, 1));
  bool drb_to_setup_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_to_setup_list_ng_ran_present, 1));
  bool drb_to_modify_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_to_modify_list_ng_ran_present, 1));
  bool drb_to_rem_list_ng_ran_present;
  HANDLE_CODE(bref.unpack(drb_to_rem_list_ng_ran_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (security_ind_present) {
    HANDLE_CODE(security_ind.unpack(bref));
  }
  if (pdu_session_res_dl_ambr_present) {
    HANDLE_CODE(unpack_integer(pdu_session_res_dl_ambr, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  if (ng_ul_up_tnl_info_present) {
    HANDLE_CODE(ng_ul_up_tnl_info.unpack(bref));
  }
  if (pdu_session_data_forwarding_info_request_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_request.unpack(bref));
  }
  if (pdu_session_data_forwarding_info_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info.unpack(bref));
  }
  if (pdu_session_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(pdu_session_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (network_instance_present) {
    HANDLE_CODE(unpack_integer(network_instance, bref, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  if (drb_to_setup_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_to_setup_list_ng_ran, bref, 1, 32, true));
  }
  if (drb_to_modify_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_to_modify_list_ng_ran, bref, 1, 32, true));
  }
  if (drb_to_rem_list_ng_ran_present) {
    HANDLE_CODE(unpack_dyn_seq_of(drb_to_rem_list_ng_ran, bref, 1, 32, true));
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
void pdu_session_res_to_modify_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  if (security_ind_present) {
    j.write_fieldname("securityIndication");
    security_ind.to_json(j);
  }
  if (pdu_session_res_dl_ambr_present) {
    j.write_uint("pDU-Session-Resource-DL-AMBR", pdu_session_res_dl_ambr);
  }
  if (ng_ul_up_tnl_info_present) {
    j.write_fieldname("nG-UL-UP-TNL-Information");
    ng_ul_up_tnl_info.to_json(j);
  }
  if (pdu_session_data_forwarding_info_request_present) {
    j.write_fieldname("pDU-Session-Data-Forwarding-Information-Request");
    pdu_session_data_forwarding_info_request.to_json(j);
  }
  if (pdu_session_data_forwarding_info_present) {
    j.write_fieldname("pDU-Session-Data-Forwarding-Information");
    pdu_session_data_forwarding_info.to_json(j);
  }
  if (pdu_session_inactivity_timer_present) {
    j.write_int("pDU-Session-Inactivity-Timer", pdu_session_inactivity_timer);
  }
  if (network_instance_present) {
    j.write_int("networkInstance", network_instance);
  }
  if (drb_to_setup_list_ng_ran.size() > 0) {
    j.start_array("dRB-To-Setup-List-NG-RAN");
    for (const auto& e1 : drb_to_setup_list_ng_ran) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_to_modify_list_ng_ran.size() > 0) {
    j.start_array("dRB-To-Modify-List-NG-RAN");
    for (const auto& e1 : drb_to_modify_list_ng_ran) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (drb_to_rem_list_ng_ran.size() > 0) {
    j.start_array("dRB-To-Remove-List-NG-RAN");
    for (const auto& e1 : drb_to_rem_list_ng_ran) {
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

// UserPlaneErrorIndicator ::= ENUMERATED
const char* user_plane_error_ind_opts::to_string() const
{
  static const char* names[] = {"gTP-U-error-indication-received"};
  return convert_enum_idx(names, 1, value, "user_plane_error_ind_e");
}

// PDU-Session-Resource-To-Remove-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdu_session_res_to_rem_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 215};
  return map_enum_number(names, 2, idx, "id");
}
bool pdu_session_res_to_rem_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 215};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_to_rem_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 0:
      return crit_e::ignore;
    case 215:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_to_rem_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 0:
      return presence_e::optional;
    case 215:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_to_rem_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += user_plane_error_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (cause_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (user_plane_error_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)215, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(user_plane_error_ind.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_to_rem_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdu_session_res_to_rem_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 0: {
        cause_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 215: {
        user_plane_error_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(user_plane_error_ind.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_res_to_rem_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (cause_present) {
    j.start_obj();
    j.write_int("id", 0);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    cause.to_json(j);
    j.end_obj();
  }
  if (user_plane_error_ind_present) {
    j.start_obj();
    j.write_int("id", 215);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "gTP-U-error-indication-received");
    j.end_obj();
  }
  j.end_array();
}

// PDU-Session-Resource-To-Remove-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_to_rem_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_to_rem_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_to_rem_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Type ::= ENUMERATED
const char* pdu_session_type_opts::to_string() const
{
  static const char* names[] = {"ipv4", "ipv6", "ipv4v6", "ethernet", "unstructured"};
  return convert_enum_idx(names, 5, value, "pdu_session_type_e");
}

// PDU-Session-Resource-To-Setup-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdu_session_res_to_setup_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 97, 96, 105, 198};
  return map_enum_number(names, 5, idx, "id");
}
bool pdu_session_res_to_setup_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 97, 96, 105, 198};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_to_setup_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::ignore;
    case 97:
      return crit_e::ignore;
    case 96:
      return crit_e::ignore;
    case 105:
      return crit_e::ignore;
    case 198:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_to_setup_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::optional;
    case 97:
      return presence_e::optional;
    case 96:
      return presence_e::optional;
    case 105:
      return presence_e::optional;
    case 198:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_to_setup_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += common_network_instance_present ? 1 : 0;
  nof_ies += redundant_n_g_ul_up_tnl_info_present ? 1 : 0;
  nof_ies += redundant_common_network_instance_present ? 1 : 0;
  nof_ies += redundant_pdu_session_info_present ? 1 : 0;
  nof_ies += special_trigger_purpose_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (common_network_instance_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(common_network_instance.pack(bref));
  }
  if (redundant_n_g_ul_up_tnl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)97, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_n_g_ul_up_tnl_info.pack(bref));
  }
  if (redundant_common_network_instance_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)96, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_common_network_instance.pack(bref));
  }
  if (redundant_pdu_session_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_pdu_session_info.pack(bref));
  }
  if (special_trigger_purpose_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)198, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(special_trigger_purpose.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_to_setup_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdu_session_res_to_setup_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        common_network_instance_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(common_network_instance.unpack(bref));
        break;
      }
      case 97: {
        redundant_n_g_ul_up_tnl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_n_g_ul_up_tnl_info.unpack(bref));
        break;
      }
      case 96: {
        redundant_common_network_instance_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_common_network_instance.unpack(bref));
        break;
      }
      case 105: {
        redundant_pdu_session_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_pdu_session_info.unpack(bref));
        break;
      }
      case 198: {
        special_trigger_purpose_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(special_trigger_purpose.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_res_to_setup_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (common_network_instance_present) {
    j.start_obj();
    j.write_int("id", 78);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", common_network_instance.to_string());
    j.end_obj();
  }
  if (redundant_n_g_ul_up_tnl_info_present) {
    j.start_obj();
    j.write_int("id", 97);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_n_g_ul_up_tnl_info.to_json(j);
    j.end_obj();
  }
  if (redundant_common_network_instance_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", redundant_common_network_instance.to_string());
    j.end_obj();
  }
  if (redundant_pdu_session_info_present) {
    j.start_obj();
    j.write_int("id", 105);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_pdu_session_info.to_json(j);
    j.end_obj();
  }
  if (special_trigger_purpose_present) {
    j.start_obj();
    j.write_int("id", 198);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "indirect-data-forwarding");
    j.end_obj();
  }
  j.end_array();
}

// PDU-Session-Resource-To-Setup-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_to_setup_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_session_res_dl_ambr_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(existing_allocated_ng_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.pack(network_instance_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pdu_session_type.pack(bref));
  HANDLE_CODE(snssai.pack(bref));
  HANDLE_CODE(security_ind.pack(bref));
  if (pdu_session_res_dl_ambr_present) {
    HANDLE_CODE(pack_integer(bref, pdu_session_res_dl_ambr, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  HANDLE_CODE(ng_ul_up_tnl_info.pack(bref));
  if (pdu_session_data_forwarding_info_request_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_request.pack(bref));
  }
  if (pdu_session_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, pdu_session_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (existing_allocated_ng_dl_up_tnl_info_present) {
    HANDLE_CODE(existing_allocated_ng_dl_up_tnl_info.pack(bref));
  }
  if (network_instance_present) {
    HANDLE_CODE(pack_integer(bref, network_instance, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, drb_to_setup_list_ng_ran, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_to_setup_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(pdu_session_res_dl_ambr_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(existing_allocated_ng_dl_up_tnl_info_present, 1));
  HANDLE_CODE(bref.unpack(network_instance_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pdu_session_type.unpack(bref));
  HANDLE_CODE(snssai.unpack(bref));
  HANDLE_CODE(security_ind.unpack(bref));
  if (pdu_session_res_dl_ambr_present) {
    HANDLE_CODE(unpack_integer(pdu_session_res_dl_ambr, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  HANDLE_CODE(ng_ul_up_tnl_info.unpack(bref));
  if (pdu_session_data_forwarding_info_request_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_request.unpack(bref));
  }
  if (pdu_session_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(pdu_session_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  if (existing_allocated_ng_dl_up_tnl_info_present) {
    HANDLE_CODE(existing_allocated_ng_dl_up_tnl_info.unpack(bref));
  }
  if (network_instance_present) {
    HANDLE_CODE(unpack_integer(network_instance, bref, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  HANDLE_CODE(unpack_dyn_seq_of(drb_to_setup_list_ng_ran, bref, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_to_setup_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.write_str("pDU-Session-Type", pdu_session_type.to_string());
  j.write_fieldname("sNSSAI");
  snssai.to_json(j);
  j.write_fieldname("securityIndication");
  security_ind.to_json(j);
  if (pdu_session_res_dl_ambr_present) {
    j.write_uint("pDU-Session-Resource-DL-AMBR", pdu_session_res_dl_ambr);
  }
  j.write_fieldname("nG-UL-UP-TNL-Information");
  ng_ul_up_tnl_info.to_json(j);
  if (pdu_session_data_forwarding_info_request_present) {
    j.write_fieldname("pDU-Session-Data-Forwarding-Information-Request");
    pdu_session_data_forwarding_info_request.to_json(j);
  }
  if (pdu_session_inactivity_timer_present) {
    j.write_int("pDU-Session-Inactivity-Timer", pdu_session_inactivity_timer);
  }
  if (existing_allocated_ng_dl_up_tnl_info_present) {
    j.write_fieldname("existing-Allocated-NG-DL-UP-TNL-Info");
    existing_allocated_ng_dl_up_tnl_info.to_json(j);
  }
  if (network_instance_present) {
    j.write_int("networkInstance", network_instance);
  }
  j.start_array("dRB-To-Setup-List-NG-RAN");
  for (const auto& e1 : drb_to_setup_list_ng_ran) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-Resource-To-Setup-Mod-Item-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t pdu_session_res_to_setup_mod_item_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {79, 78, 97, 96, 198};
  return map_enum_number(names, 5, idx, "id");
}
bool pdu_session_res_to_setup_mod_item_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {79, 78, 97, 96, 198};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_to_setup_mod_item_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 79:
      return crit_e::ignore;
    case 78:
      return crit_e::ignore;
    case 97:
      return crit_e::ignore;
    case 96:
      return crit_e::ignore;
    case 198:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_to_setup_mod_item_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 79:
      return presence_e::optional;
    case 78:
      return presence_e::optional;
    case 97:
      return presence_e::optional;
    case 96:
      return presence_e::optional;
    case 198:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_to_setup_mod_item_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += network_instance_present ? 1 : 0;
  nof_ies += common_network_instance_present ? 1 : 0;
  nof_ies += redundant_n_g_ul_up_tnl_info_present ? 1 : 0;
  nof_ies += redundant_common_network_instance_present ? 1 : 0;
  nof_ies += special_trigger_purpose_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (network_instance_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)79, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, network_instance, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  if (common_network_instance_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(common_network_instance.pack(bref));
  }
  if (redundant_n_g_ul_up_tnl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)97, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_n_g_ul_up_tnl_info.pack(bref));
  }
  if (redundant_common_network_instance_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)96, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(redundant_common_network_instance.pack(bref));
  }
  if (special_trigger_purpose_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)198, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(special_trigger_purpose.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_to_setup_mod_item_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdu_session_res_to_setup_mod_item_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 79: {
        network_instance_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(network_instance, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 78: {
        common_network_instance_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(common_network_instance.unpack(bref));
        break;
      }
      case 97: {
        redundant_n_g_ul_up_tnl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_n_g_ul_up_tnl_info.unpack(bref));
        break;
      }
      case 96: {
        redundant_common_network_instance_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(redundant_common_network_instance.unpack(bref));
        break;
      }
      case 198: {
        special_trigger_purpose_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(special_trigger_purpose.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void pdu_session_res_to_setup_mod_item_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (network_instance_present) {
    j.start_obj();
    j.write_int("id", 79);
    j.write_str("criticality", "ignore");
    j.write_int("extensionValue", network_instance);
    j.end_obj();
  }
  if (common_network_instance_present) {
    j.start_obj();
    j.write_int("id", 78);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", common_network_instance.to_string());
    j.end_obj();
  }
  if (redundant_n_g_ul_up_tnl_info_present) {
    j.start_obj();
    j.write_int("id", 97);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    redundant_n_g_ul_up_tnl_info.to_json(j);
    j.end_obj();
  }
  if (redundant_common_network_instance_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", redundant_common_network_instance.to_string());
    j.end_obj();
  }
  if (special_trigger_purpose_present) {
    j.start_obj();
    j.write_int("id", 198);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", "indirect-data-forwarding");
    j.end_obj();
  }
  j.end_array();
}

// PDU-Session-Resource-To-Setup-Mod-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_res_to_setup_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(pdu_session_res_ambr_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.pack(pdu_session_inactivity_timer_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pdu_session_type.pack(bref));
  HANDLE_CODE(snssai.pack(bref));
  HANDLE_CODE(security_ind.pack(bref));
  if (pdu_session_res_ambr_present) {
    HANDLE_CODE(pack_integer(bref, pdu_session_res_ambr, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  HANDLE_CODE(ng_ul_up_tnl_info.pack(bref));
  if (pdu_session_data_forwarding_info_request_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_request.pack(bref));
  }
  if (pdu_session_inactivity_timer_present) {
    HANDLE_CODE(pack_integer(bref, pdu_session_inactivity_timer, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, drb_to_setup_mod_list_ng_ran, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_to_setup_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(pdu_session_res_ambr_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_data_forwarding_info_request_present, 1));
  HANDLE_CODE(bref.unpack(pdu_session_inactivity_timer_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pdu_session_type.unpack(bref));
  HANDLE_CODE(snssai.unpack(bref));
  HANDLE_CODE(security_ind.unpack(bref));
  if (pdu_session_res_ambr_present) {
    HANDLE_CODE(unpack_integer(pdu_session_res_ambr, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  }
  HANDLE_CODE(ng_ul_up_tnl_info.unpack(bref));
  if (pdu_session_data_forwarding_info_request_present) {
    HANDLE_CODE(pdu_session_data_forwarding_info_request.unpack(bref));
  }
  if (pdu_session_inactivity_timer_present) {
    HANDLE_CODE(unpack_integer(pdu_session_inactivity_timer, bref, (uint16_t)1u, (uint16_t)7200u, true, true));
  }
  HANDLE_CODE(unpack_dyn_seq_of(drb_to_setup_mod_list_ng_ran, bref, 1, 32, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_to_setup_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.write_str("pDU-Session-Type", pdu_session_type.to_string());
  j.write_fieldname("sNSSAI");
  snssai.to_json(j);
  j.write_fieldname("securityIndication");
  security_ind.to_json(j);
  if (pdu_session_res_ambr_present) {
    j.write_uint("pDU-Session-Resource-AMBR", pdu_session_res_ambr);
  }
  j.write_fieldname("nG-UL-UP-TNL-Information");
  ng_ul_up_tnl_info.to_json(j);
  if (pdu_session_data_forwarding_info_request_present) {
    j.write_fieldname("pDU-Session-Data-Forwarding-Information-Request");
    pdu_session_data_forwarding_info_request.to_json(j);
  }
  if (pdu_session_inactivity_timer_present) {
    j.write_int("pDU-Session-Inactivity-Timer", pdu_session_inactivity_timer);
  }
  j.start_array("dRB-To-Setup-Mod-List-NG-RAN");
  for (const auto& e1 : drb_to_setup_mod_list_ng_ran) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PDU-Session-To-Notify-Item ::= SEQUENCE
OCUDUASN_CODE pdu_session_to_notify_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, pdu_session_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, qos_flow_list, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_to_notify_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(pdu_session_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(qos_flow_list, bref, 1, 64, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_to_notify_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pDU-Session-ID", pdu_session_id);
  j.start_array("qoS-Flow-List");
  for (const auto& e1 : qos_flow_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// PrivacyIndicator ::= ENUMERATED
const char* privacy_ind_opts::to_string() const
{
  static const char* names[] = {"immediate-MDT", "logged-MDT"};
  return convert_enum_idx(names, 2, value, "privacy_ind_e");
}

// QoS-Parameters-Support-List ::= SEQUENCE
OCUDUASN_CODE qos_params_support_list_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(eutran_qos_support_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ng_ran_qos_support_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (eutran_qos_support_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, eutran_qos_support_list, 1, 256, true));
  }
  if (ng_ran_qos_support_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ng_ran_qos_support_list, 1, 256, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qos_params_support_list_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool eutran_qos_support_list_present;
  HANDLE_CODE(bref.unpack(eutran_qos_support_list_present, 1));
  bool ng_ran_qos_support_list_present;
  HANDLE_CODE(bref.unpack(ng_ran_qos_support_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (eutran_qos_support_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(eutran_qos_support_list, bref, 1, 256, true));
  }
  if (ng_ran_qos_support_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ng_ran_qos_support_list, bref, 1, 256, true));
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
void qos_params_support_list_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (eutran_qos_support_list.size() > 0) {
    j.start_array("eUTRAN-QoS-Support-List");
    for (const auto& e1 : eutran_qos_support_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ng_ran_qos_support_list.size() > 0) {
    j.start_array("nG-RAN-QoS-Support-List");
    for (const auto& e1 : ng_ran_qos_support_list) {
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

// RegistrationRequest ::= ENUMERATED
const char* regist_request_opts::to_string() const
{
  static const char* names[] = {"start", "stop"};
  return convert_enum_idx(names, 2, value, "regist_request_e");
}

// ReportingPeriodicity ::= ENUMERATED
const char* report_periodicity_opts::to_string() const
{
  static const char* names[] = {"ms500",
                                "ms1000",
                                "ms2000",
                                "ms5000",
                                "ms10000",
                                "ms20000",
                                "ms30000",
                                "ms40000",
                                "ms50000",
                                "ms60000",
                                "ms70000",
                                "ms80000",
                                "ms90000",
                                "ms100000",
                                "ms110000",
                                "ms120000"};
  return convert_enum_idx(names, 16, value, "report_periodicity_e");
}
uint32_t report_periodicity_opts::to_number() const
{
  static const uint32_t numbers[] = {
      500, 1000, 2000, 5000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000, 110000, 120000};
  return map_enum_number(numbers, 16, value, "report_periodicity_e");
}

// SCGActivationStatus ::= ENUMERATED
const char* scg_activation_status_opts::to_string() const
{
  static const char* names[] = {"scg-activated", "scg-deactivated"};
  return convert_enum_idx(names, 2, value, "scg_activation_status_e");
}

// SDT-data-size-threshold-Crossed ::= ENUMERATED
const char* sdt_data_size_thres_crossed_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "sdt_data_size_thres_crossed_e");
}

// SDTContinueROHC ::= ENUMERATED
const char* sdt_continue_rohc_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "sdt_continue_rohc_e");
}

// SecurityAlgorithm ::= SEQUENCE
OCUDUASN_CODE security_algorithm_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(integrity_protection_algorithm_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ciphering_algorithm.pack(bref));
  if (integrity_protection_algorithm_present) {
    HANDLE_CODE(integrity_protection_algorithm.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE security_algorithm_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(integrity_protection_algorithm_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ciphering_algorithm.unpack(bref));
  if (integrity_protection_algorithm_present) {
    HANDLE_CODE(integrity_protection_algorithm.unpack(bref));
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
void security_algorithm_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("cipheringAlgorithm", ciphering_algorithm.to_string());
  if (integrity_protection_algorithm_present) {
    j.write_str("integrityProtectionAlgorithm", integrity_protection_algorithm.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// UPSecuritykey ::= SEQUENCE
OCUDUASN_CODE up_securitykey_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(integrity_protection_key.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(encryption_key.pack(bref));
  if (integrity_protection_key.size() > 0) {
    HANDLE_CODE(integrity_protection_key.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE up_securitykey_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool integrity_protection_key_present;
  HANDLE_CODE(bref.unpack(integrity_protection_key_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(encryption_key.unpack(bref));
  if (integrity_protection_key_present) {
    HANDLE_CODE(integrity_protection_key.unpack(bref));
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
void up_securitykey_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("encryptionKey", encryption_key.to_string());
  if (integrity_protection_key.size() > 0) {
    j.write_str("integrityProtectionKey", integrity_protection_key.to_string());
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SecurityInformation ::= SEQUENCE
OCUDUASN_CODE security_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(security_algorithm.pack(bref));
  HANDLE_CODE(up_securitykey.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE security_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(security_algorithm.unpack(bref));
  HANDLE_CODE(up_securitykey.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void security_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("securityAlgorithm");
  security_algorithm.to_json(j);
  j.write_fieldname("uPSecuritykey");
  up_securitykey.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TNL-AvailableCapacityIndicator ::= SEQUENCE
OCUDUASN_CODE tnl_available_capacity_ind_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, dl_tnl_offered_capacity, (uint32_t)0u, (uint32_t)16777216u, true, true));
  HANDLE_CODE(pack_integer(bref, dl_tnl_available_capacity, (uint16_t)0u, (uint16_t)100u, true, true));
  HANDLE_CODE(pack_integer(bref, ul_tnl_offered_capacity, (uint32_t)0u, (uint32_t)16777216u, true, true));
  HANDLE_CODE(pack_integer(bref, ul_tnl_available_capacity, (uint16_t)0u, (uint16_t)100u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tnl_available_capacity_ind_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(dl_tnl_offered_capacity, bref, (uint32_t)0u, (uint32_t)16777216u, true, true));
  HANDLE_CODE(unpack_integer(dl_tnl_available_capacity, bref, (uint16_t)0u, (uint16_t)100u, true, true));
  HANDLE_CODE(unpack_integer(ul_tnl_offered_capacity, bref, (uint32_t)0u, (uint32_t)16777216u, true, true));
  HANDLE_CODE(unpack_integer(ul_tnl_available_capacity, bref, (uint16_t)0u, (uint16_t)100u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void tnl_available_capacity_ind_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("dL-TNL-OfferedCapacity", dl_tnl_offered_capacity);
  j.write_int("dL-TNL-AvailableCapacity", dl_tnl_available_capacity);
  j.write_int("uL-TNL-OfferedCapacity", ul_tnl_offered_capacity);
  j.write_int("uL-TNL-AvailableCapacity", ul_tnl_available_capacity);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// TimeToWait ::= ENUMERATED
const char* time_to_wait_opts::to_string() const
{
  static const char* names[] = {"v1s", "v2s", "v5s", "v10s", "v20s", "v60s"};
  return convert_enum_idx(names, 6, value, "time_to_wait_e");
}
uint8_t time_to_wait_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 5, 10, 20, 60};
  return map_enum_number(numbers, 6, value, "time_to_wait_e");
}

// TraceDepth ::= ENUMERATED
const char* trace_depth_opts::to_string() const
{
  static const char* names[] = {"minimum",
                                "medium",
                                "maximum",
                                "minimumWithoutVendorSpecificExtension",
                                "mediumWithoutVendorSpecificExtension",
                                "maximumWithoutVendorSpecificExtension"};
  return convert_enum_idx(names, 6, value, "trace_depth_e");
}

// TraceActivation-ExtIEs ::= OBJECT SET OF E1AP-PROTOCOL-EXTENSION
uint32_t trace_activation_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {112, 116};
  return map_enum_number(names, 2, idx, "id");
}
bool trace_activation_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {112, 116};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trace_activation_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 112:
      return crit_e::ignore;
    case 116:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trace_activation_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 112:
      return presence_e::optional;
    case 116:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE trace_activation_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += mdt_cfg_present ? 1 : 0;
  nof_ies += trace_collection_entity_uri_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (mdt_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)112, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mdt_cfg.pack(bref));
  }
  if (trace_collection_entity_uri_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)116, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(trace_collection_entity_uri.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trace_activation_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trace_activation_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 112: {
        mdt_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mdt_cfg.unpack(bref));
        break;
      }
      case 116: {
        trace_collection_entity_uri_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(trace_collection_entity_uri.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void trace_activation_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (mdt_cfg_present) {
    j.start_obj();
    j.write_int("id", 112);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    mdt_cfg.to_json(j);
    j.end_obj();
  }
  if (trace_collection_entity_uri_present) {
    j.start_obj();
    j.write_int("id", 116);
    j.write_str("criticality", "ignore");
    j.write_str("extensionValue", trace_collection_entity_uri.to_string());
    j.end_obj();
  }
  j.end_array();
}

// TraceActivation ::= SEQUENCE
OCUDUASN_CODE trace_activation_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(trace_id.pack(bref));
  HANDLE_CODE(interfaces_to_trace.pack(bref));
  HANDLE_CODE(trace_depth.pack(bref));
  HANDLE_CODE(trace_collection_entity_ip_address.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trace_activation_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(trace_id.unpack(bref));
  HANDLE_CODE(interfaces_to_trace.unpack(bref));
  HANDLE_CODE(trace_depth.unpack(bref));
  HANDLE_CODE(trace_collection_entity_ip_address.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void trace_activation_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("traceID", trace_id.to_string());
  j.write_fieldname("interfacesToTrace");
  interfaces_to_trace.to_json(j);
  j.write_str("traceDepth", trace_depth.to_string());
  j.write_fieldname("traceCollectionEntityIPAddress");
  trace_collection_entity_ip_address.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// Transport-UP-Layer-Addresses-Info-To-Add-Item ::= SEQUENCE
OCUDUASN_CODE transport_up_layer_addresses_info_to_add_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(gtp_transport_layer_addresses_to_add.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ip_sec_transport_layer_address.pack(bref));
  if (gtp_transport_layer_addresses_to_add.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, gtp_transport_layer_addresses_to_add, 1, 16, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE transport_up_layer_addresses_info_to_add_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool gtp_transport_layer_addresses_to_add_present;
  HANDLE_CODE(bref.unpack(gtp_transport_layer_addresses_to_add_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ip_sec_transport_layer_address.unpack(bref));
  if (gtp_transport_layer_addresses_to_add_present) {
    HANDLE_CODE(unpack_dyn_seq_of(gtp_transport_layer_addresses_to_add, bref, 1, 16, true));
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
void transport_up_layer_addresses_info_to_add_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("iP-SecTransportLayerAddress");
  ip_sec_transport_layer_address.to_json(j);
  if (gtp_transport_layer_addresses_to_add.size() > 0) {
    j.start_array("gTPTransportLayerAddressesToAdd");
    for (const auto& e1 : gtp_transport_layer_addresses_to_add) {
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

// Transport-UP-Layer-Addresses-Info-To-Remove-Item ::= SEQUENCE
OCUDUASN_CODE transport_up_layer_addresses_info_to_rem_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(gtp_transport_layer_addresses_to_rem.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(ip_sec_transport_layer_address.pack(bref));
  if (gtp_transport_layer_addresses_to_rem.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, gtp_transport_layer_addresses_to_rem, 1, 16, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE transport_up_layer_addresses_info_to_rem_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool gtp_transport_layer_addresses_to_rem_present;
  HANDLE_CODE(bref.unpack(gtp_transport_layer_addresses_to_rem_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(ip_sec_transport_layer_address.unpack(bref));
  if (gtp_transport_layer_addresses_to_rem_present) {
    HANDLE_CODE(unpack_dyn_seq_of(gtp_transport_layer_addresses_to_rem, bref, 1, 16, true));
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
void transport_up_layer_addresses_info_to_rem_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("iP-SecTransportLayerAddress");
  ip_sec_transport_layer_address.to_json(j);
  if (gtp_transport_layer_addresses_to_rem.size() > 0) {
    j.start_array("gTPTransportLayerAddressesToRemove");
    for (const auto& e1 : gtp_transport_layer_addresses_to_rem) {
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

// Transport-Layer-Address-Info ::= SEQUENCE
OCUDUASN_CODE transport_layer_address_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(transport_up_layer_addresses_info_to_add_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(transport_up_layer_addresses_info_to_rem_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (transport_up_layer_addresses_info_to_add_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, transport_up_layer_addresses_info_to_add_list, 1, 16, true));
  }
  if (transport_up_layer_addresses_info_to_rem_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, transport_up_layer_addresses_info_to_rem_list, 1, 16, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE transport_layer_address_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool transport_up_layer_addresses_info_to_add_list_present;
  HANDLE_CODE(bref.unpack(transport_up_layer_addresses_info_to_add_list_present, 1));
  bool transport_up_layer_addresses_info_to_rem_list_present;
  HANDLE_CODE(bref.unpack(transport_up_layer_addresses_info_to_rem_list_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (transport_up_layer_addresses_info_to_add_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(transport_up_layer_addresses_info_to_add_list, bref, 1, 16, true));
  }
  if (transport_up_layer_addresses_info_to_rem_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(transport_up_layer_addresses_info_to_rem_list, bref, 1, 16, true));
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
void transport_layer_address_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (transport_up_layer_addresses_info_to_add_list.size() > 0) {
    j.start_array("transport-UP-Layer-Addresses-Info-To-Add-List");
    for (const auto& e1 : transport_up_layer_addresses_info_to_add_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (transport_up_layer_addresses_info_to_rem_list.size() > 0) {
    j.start_array("transport-UP-Layer-Addresses-Info-To-Remove-List");
    for (const auto& e1 : transport_up_layer_addresses_info_to_rem_list) {
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

// UE-associatedLogicalE1-ConnectionItem ::= SEQUENCE
OCUDUASN_CODE ue_associated_lc_e1_conn_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(gnb_cu_cp_ue_e1ap_id_present, 1));
  HANDLE_CODE(bref.pack(gnb_cu_up_ue_e1ap_id_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  if (gnb_cu_cp_ue_e1ap_id_present) {
    HANDLE_CODE(pack_integer(bref, gnb_cu_cp_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (gnb_cu_up_ue_e1ap_id_present) {
    HANDLE_CODE(pack_integer(bref, gnb_cu_up_ue_e1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_associated_lc_e1_conn_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(gnb_cu_cp_ue_e1ap_id_present, 1));
  HANDLE_CODE(bref.unpack(gnb_cu_up_ue_e1ap_id_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  if (gnb_cu_cp_ue_e1ap_id_present) {
    HANDLE_CODE(unpack_integer(gnb_cu_cp_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (gnb_cu_up_ue_e1ap_id_present) {
    HANDLE_CODE(unpack_integer(gnb_cu_up_ue_e1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
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
void ue_associated_lc_e1_conn_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (gnb_cu_cp_ue_e1ap_id_present) {
    j.write_uint("gNB-CU-CP-UE-E1AP-ID", gnb_cu_cp_ue_e1ap_id);
  }
  if (gnb_cu_up_ue_e1ap_id_present) {
    j.write_uint("gNB-CU-UP-UE-E1AP-ID", gnb_cu_up_ue_e1ap_id);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// UESliceMaximumBitRateItem ::= SEQUENCE
OCUDUASN_CODE ue_slice_max_bit_rate_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(snssai.pack(bref));
  HANDLE_CODE(pack_integer(bref, ue_slice_max_bit_rate_dl, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_slice_max_bit_rate_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(snssai.unpack(bref));
  HANDLE_CODE(unpack_integer(ue_slice_max_bit_rate_dl, bref, (uint64_t)0u, (uint64_t)4000000000000u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ue_slice_max_bit_rate_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("sNSSAI");
  snssai.to_json(j);
  j.write_uint("uESliceMaximumBitRateDL", ue_slice_max_bit_rate_dl);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// ULUPTNLAddressToUpdateItem ::= SEQUENCE
OCUDUASN_CODE ul_up_tnl_address_to_upd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(old_tnl_adress.pack(bref));
  HANDLE_CODE(new_tnl_adress.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_up_tnl_address_to_upd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(old_tnl_adress.unpack(bref));
  HANDLE_CODE(new_tnl_adress.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ul_up_tnl_address_to_upd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("oldTNLAdress");
  old_tnl_adress.to_json(j);
  j.write_fieldname("newTNLAdress");
  new_tnl_adress.to_json(j);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}
