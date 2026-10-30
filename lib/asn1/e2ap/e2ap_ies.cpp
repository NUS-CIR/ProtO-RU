// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/e2ap/e2ap_ies.h"

using namespace asn1;
using namespace asn1::e2ap;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// CauseRICrequest ::= ENUMERATED
const char* cause_ric_request_opts::to_string() const
{
  static const char* names[] = {"ran-function-id-invalid",
                                "action-not-supported",
                                "excessive-actions",
                                "duplicate-action",
                                "duplicate-event-trigger",
                                "function-resource-limit",
                                "request-id-unknown",
                                "inconsistent-action-subsequent-action-sequence",
                                "control-message-invalid",
                                "ric-call-process-id-invalid",
                                "control-timer-expired",
                                "control-failed-to-execute",
                                "system-not-ready",
                                "unspecified",
                                "ric-subscription-end-time-expired",
                                "ric-subscription-end-time-invalid",
                                "duplicate-ric-request-id",
                                "eventTriggerNotSupported",
                                "requested-information-unavailable",
                                "invalid-information-request"};
  return convert_enum_idx(names, 20, value, "cause_ric_request_e");
}

// CauseRICservice ::= ENUMERATED
const char* cause_ric_service_opts::to_string() const
{
  static const char* names[] = {"ran-function-not-supported", "excessive-functions", "ric-resource-limit"};
  return convert_enum_idx(names, 3, value, "cause_ric_service_e");
}

// CauseE2node ::= ENUMERATED
const char* cause_e2node_opts::to_string() const
{
  static const char* names[] = {"e2node-component-unknown"};
  return convert_enum_idx(names, 1, value, "cause_e2node_e");
}
uint8_t cause_e2node_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "cause_e2node_e");
}

// CauseTransport ::= ENUMERATED
const char* cause_transport_opts::to_string() const
{
  static const char* names[] = {"unspecified", "transport-resource-unavailable"};
  return convert_enum_idx(names, 2, value, "cause_transport_e");
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
  static const char* names[] = {"control-processing-overload", "hardware-failure", "om-intervention", "unspecified"};
  return convert_enum_idx(names, 4, value, "cause_misc_e");
}

// CauseServiceLayer ::= SEQUENCE
OCUDUASN_CODE cause_service_layer_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(service_layer_cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cause_service_layer_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(service_layer_cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cause_service_layer_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("serviceLayerCause", service_layer_cause.to_string());
  j.end_obj();
}

// Cause ::= CHOICE
void cause_c::destroy_()
{
  switch (type_) {
    case types::service_layer:
      c.destroy<cause_service_layer_s>();
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
    case types::ric_request:
      break;
    case types::ric_service:
      break;
    case types::e2_node:
      break;
    case types::transport:
      break;
    case types::protocol:
      break;
    case types::misc:
      break;
    case types::service_layer:
      c.init<cause_service_layer_s>();
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
    case types::ric_request:
      c.init(other.c.get<cause_ric_request_e>());
      break;
    case types::ric_service:
      c.init(other.c.get<cause_ric_service_e>());
      break;
    case types::e2_node:
      c.init(other.c.get<cause_e2node_e>());
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
    case types::service_layer:
      c.init(other.c.get<cause_service_layer_s>());
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
    case types::ric_request:
      c.set(other.c.get<cause_ric_request_e>());
      break;
    case types::ric_service:
      c.set(other.c.get<cause_ric_service_e>());
      break;
    case types::e2_node:
      c.set(other.c.get<cause_e2node_e>());
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
    case types::service_layer:
      c.set(other.c.get<cause_service_layer_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cause_c");
  }

  return *this;
}
cause_ric_request_e& cause_c::set_ric_request()
{
  set(types::ric_request);
  return c.get<cause_ric_request_e>();
}
cause_ric_service_e& cause_c::set_ric_service()
{
  set(types::ric_service);
  return c.get<cause_ric_service_e>();
}
cause_e2node_e& cause_c::set_e2_node()
{
  set(types::e2_node);
  return c.get<cause_e2node_e>();
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
cause_service_layer_s& cause_c::set_service_layer()
{
  set(types::service_layer);
  return c.get<cause_service_layer_s>();
}
void cause_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ric_request:
      j.write_str("ricRequest", c.get<cause_ric_request_e>().to_string());
      break;
    case types::ric_service:
      j.write_str("ricService", c.get<cause_ric_service_e>().to_string());
      break;
    case types::e2_node:
      j.write_str("e2Node", "e2node-component-unknown");
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
    case types::service_layer:
      j.write_fieldname("serviceLayer");
      c.get<cause_service_layer_s>().to_json(j);
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
    case types::ric_request:
      HANDLE_CODE(c.get<cause_ric_request_e>().pack(bref));
      break;
    case types::ric_service:
      HANDLE_CODE(c.get<cause_ric_service_e>().pack(bref));
      break;
    case types::e2_node:
      HANDLE_CODE(c.get<cause_e2node_e>().pack(bref));
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
    case types::service_layer: {
      varlength_field_pack_guard varlen_scope(bref, true);
      HANDLE_CODE(c.get<cause_service_layer_s>().pack(bref));
    } break;
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
    case types::ric_request:
      HANDLE_CODE(c.get<cause_ric_request_e>().unpack(bref));
      break;
    case types::ric_service:
      HANDLE_CODE(c.get<cause_ric_service_e>().unpack(bref));
      break;
    case types::e2_node:
      HANDLE_CODE(c.get<cause_e2node_e>().unpack(bref));
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
    case types::service_layer: {
      varlength_field_unpack_guard varlen_scope(bref, true);
      HANDLE_CODE(c.get<cause_service_layer_s>().unpack(bref));
    } break;
    default:
      log_invalid_choice_id(type_, "cause_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cause_c::types_opts::to_string() const
{
  static const char* names[] = {"ricRequest", "ricService", "e2Node", "transport", "protocol", "misc", "serviceLayer"};
  return convert_enum_idx(names, 7, value, "cause_c::types");
}
uint8_t cause_c::types_opts::to_number() const
{
  if (value == e2_node) {
    return 2;
  }
  invalid_enum_number(value, "cause_c::types");
  return 0;
}

// RICrequestID ::= SEQUENCE
OCUDUASN_CODE ric_request_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_requestor_id, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(pack_integer(bref, ric_instance_id, (uint32_t)0u, (uint32_t)65535u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_request_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_requestor_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(unpack_integer(ric_instance_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_request_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricRequestorID", ric_requestor_id);
  j.write_int("ricInstanceID", ric_instance_id);
  j.end_obj();
}

// TypeOfError ::= ENUMERATED
const char* type_of_error_opts::to_string() const
{
  static const char* names[] = {"not-understood", "missing"};
  return convert_enum_idx(names, 2, value, "type_of_error_e");
}

// CriticalityDiagnostics-IE-Item ::= SEQUENCE
OCUDUASN_CODE crit_diagnostics_ie_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(ie_crit.pack(bref));
  HANDLE_CODE(pack_integer(bref, ie_id, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(type_of_error.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE crit_diagnostics_ie_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(ie_crit.unpack(bref));
  HANDLE_CODE(unpack_integer(ie_id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
  HANDLE_CODE(type_of_error.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void crit_diagnostics_ie_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("iECriticality", ie_crit.to_string());
  j.write_int("iE-ID", ie_id);
  j.write_str("typeOfError", type_of_error.to_string());
  j.end_obj();
}

// CriticalityDiagnostics ::= SEQUENCE
OCUDUASN_CODE crit_diagnostics_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(proc_code_present, 1));
  HANDLE_CODE(bref.pack(trigger_msg_present, 1));
  HANDLE_CODE(bref.pack(proc_crit_present, 1));
  HANDLE_CODE(bref.pack(ric_requestor_id_present, 1));
  HANDLE_CODE(bref.pack(ies_crit_diagnostics.size() > 0, 1));

  if (proc_code_present) {
    HANDLE_CODE(pack_integer(bref, proc_code, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (trigger_msg_present) {
    HANDLE_CODE(trigger_msg.pack(bref));
  }
  if (proc_crit_present) {
    HANDLE_CODE(proc_crit.pack(bref));
  }
  if (ric_requestor_id_present) {
    HANDLE_CODE(ric_requestor_id.pack(bref));
  }
  if (ies_crit_diagnostics.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ies_crit_diagnostics, 1, 256, true));
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
  HANDLE_CODE(bref.unpack(ric_requestor_id_present, 1));
  bool ies_crit_diagnostics_present;
  HANDLE_CODE(bref.unpack(ies_crit_diagnostics_present, 1));

  if (proc_code_present) {
    HANDLE_CODE(unpack_integer(proc_code, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (trigger_msg_present) {
    HANDLE_CODE(trigger_msg.unpack(bref));
  }
  if (proc_crit_present) {
    HANDLE_CODE(proc_crit.unpack(bref));
  }
  if (ric_requestor_id_present) {
    HANDLE_CODE(ric_requestor_id.unpack(bref));
  }
  if (ies_crit_diagnostics_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ies_crit_diagnostics, bref, 1, 256, true));
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
  if (ric_requestor_id_present) {
    j.write_fieldname("ricRequestorID");
    ric_requestor_id.to_json(j);
  }
  if (ies_crit_diagnostics.size() > 0) {
    j.start_array("iEsCriticalityDiagnostics");
    for (const auto& e1 : ies_crit_diagnostics) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// E2nodeComponentConfiguration ::= SEQUENCE
OCUDUASN_CODE e2node_component_cfg_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(e2node_component_request_part.pack(bref));
  HANDLE_CODE(e2node_component_resp_part.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(e2node_component_request_part.unpack(bref));
  HANDLE_CODE(e2node_component_resp_part.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("e2nodeComponentRequestPart", e2node_component_request_part.to_string());
  j.write_str("e2nodeComponentResponsePart", e2node_component_resp_part.to_string());
  j.end_obj();
}

// E2nodeComponentConfigurationAck ::= SEQUENCE
OCUDUASN_CODE e2node_component_cfg_ack_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(fail_cause_present, 1));

  HANDLE_CODE(upd_outcome.pack(bref));
  if (fail_cause_present) {
    HANDLE_CODE(fail_cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_ack_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(fail_cause_present, 1));

  HANDLE_CODE(upd_outcome.unpack(bref));
  if (fail_cause_present) {
    HANDLE_CODE(fail_cause.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_cfg_ack_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("updateOutcome", upd_outcome.to_string());
  if (fail_cause_present) {
    j.write_fieldname("failureCause");
    fail_cause.to_json(j);
  }
  j.end_obj();
}

const char* e2node_component_cfg_ack_s::upd_outcome_opts::to_string() const
{
  static const char* names[] = {"success", "failure"};
  return convert_enum_idx(names, 2, value, "e2node_component_cfg_ack_s::upd_outcome_e_");
}

// E2nodeComponentInterfaceNG ::= SEQUENCE
OCUDUASN_CODE e2node_component_interface_ng_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(amf_name.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_interface_ng_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(amf_name.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_interface_ng_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("amf-name", amf_name.to_string());
  j.end_obj();
}

// GNB-ID-Choice ::= CHOICE
void gnb_id_choice_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("gnb-ID", c.to_string());
  j.end_obj();
}
OCUDUASN_CODE gnb_id_choice_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_enum(bref, type()));
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_id_choice_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(unpack_enum(e, bref));
  if (e != type()) {
    log_invalid_choice_id(e, "gnb_id_choice_c");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* gnb_id_choice_c::types_opts::to_string() const
{
  static const char* names[] = {"gnb-ID"};
  return convert_enum_idx(names, 1, value, "gnb_id_choice_c::types");
}

// GlobalgNB-ID ::= SEQUENCE
OCUDUASN_CODE global_gnb_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(gnb_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_gnb_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(gnb_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void global_gnb_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("plmn-id", plmn_id.to_string());
  j.write_fieldname("gnb-id");
  gnb_id.to_json(j);
  j.end_obj();
}

// ENB-ID-Choice ::= CHOICE
void enb_id_choice_c::destroy_()
{
  switch (type_) {
    case types::enb_id_macro:
      c.destroy<fixed_bitstring<20, false, true>>();
      break;
    case types::enb_id_shortmacro:
      c.destroy<fixed_bitstring<18, false, true>>();
      break;
    case types::enb_id_longmacro:
      c.destroy<fixed_bitstring<21, false, true>>();
      break;
    default:
      break;
  }
}
void enb_id_choice_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::enb_id_macro:
      c.init<fixed_bitstring<20, false, true>>();
      break;
    case types::enb_id_shortmacro:
      c.init<fixed_bitstring<18, false, true>>();
      break;
    case types::enb_id_longmacro:
      c.init<fixed_bitstring<21, false, true>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_choice_c");
  }
}
enb_id_choice_c::enb_id_choice_c(const enb_id_choice_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::enb_id_macro:
      c.init(other.c.get<fixed_bitstring<20, false, true>>());
      break;
    case types::enb_id_shortmacro:
      c.init(other.c.get<fixed_bitstring<18, false, true>>());
      break;
    case types::enb_id_longmacro:
      c.init(other.c.get<fixed_bitstring<21, false, true>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_choice_c");
  }
}
enb_id_choice_c& enb_id_choice_c::operator=(const enb_id_choice_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::enb_id_macro:
      c.set(other.c.get<fixed_bitstring<20, false, true>>());
      break;
    case types::enb_id_shortmacro:
      c.set(other.c.get<fixed_bitstring<18, false, true>>());
      break;
    case types::enb_id_longmacro:
      c.set(other.c.get<fixed_bitstring<21, false, true>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_choice_c");
  }

  return *this;
}
fixed_bitstring<20, false, true>& enb_id_choice_c::set_enb_id_macro()
{
  set(types::enb_id_macro);
  return c.get<fixed_bitstring<20, false, true>>();
}
fixed_bitstring<18, false, true>& enb_id_choice_c::set_enb_id_shortmacro()
{
  set(types::enb_id_shortmacro);
  return c.get<fixed_bitstring<18, false, true>>();
}
fixed_bitstring<21, false, true>& enb_id_choice_c::set_enb_id_longmacro()
{
  set(types::enb_id_longmacro);
  return c.get<fixed_bitstring<21, false, true>>();
}
void enb_id_choice_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::enb_id_macro:
      j.write_str("enb-ID-macro", c.get<fixed_bitstring<20, false, true>>().to_string());
      break;
    case types::enb_id_shortmacro:
      j.write_str("enb-ID-shortmacro", c.get<fixed_bitstring<18, false, true>>().to_string());
      break;
    case types::enb_id_longmacro:
      j.write_str("enb-ID-longmacro", c.get<fixed_bitstring<21, false, true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_choice_c");
  }
  j.end_obj();
}
OCUDUASN_CODE enb_id_choice_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::enb_id_macro:
      HANDLE_CODE((c.get<fixed_bitstring<20, false, true>>().pack(bref)));
      break;
    case types::enb_id_shortmacro:
      HANDLE_CODE((c.get<fixed_bitstring<18, false, true>>().pack(bref)));
      break;
    case types::enb_id_longmacro:
      HANDLE_CODE((c.get<fixed_bitstring<21, false, true>>().pack(bref)));
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_choice_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE enb_id_choice_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::enb_id_macro:
      HANDLE_CODE((c.get<fixed_bitstring<20, false, true>>().unpack(bref)));
      break;
    case types::enb_id_shortmacro:
      HANDLE_CODE((c.get<fixed_bitstring<18, false, true>>().unpack(bref)));
      break;
    case types::enb_id_longmacro:
      HANDLE_CODE((c.get<fixed_bitstring<21, false, true>>().unpack(bref)));
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_choice_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* enb_id_choice_c::types_opts::to_string() const
{
  static const char* names[] = {"enb-ID-macro", "enb-ID-shortmacro", "enb-ID-longmacro"};
  return convert_enum_idx(names, 3, value, "enb_id_choice_c::types");
}

// GlobalngeNB-ID ::= SEQUENCE
OCUDUASN_CODE globalng_enb_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(enb_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE globalng_enb_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(enb_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void globalng_enb_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("plmn-id", plmn_id.to_string());
  j.write_fieldname("enb-id");
  enb_id.to_json(j);
  j.end_obj();
}

// GlobalNG-RANNode-ID ::= CHOICE
void global_ng_ran_node_id_c::destroy_()
{
  switch (type_) {
    case types::gnb:
      c.destroy<global_gnb_id_s>();
      break;
    case types::ng_enb:
      c.destroy<globalng_enb_id_s>();
      break;
    default:
      break;
  }
}
void global_ng_ran_node_id_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::gnb:
      c.init<global_gnb_id_s>();
      break;
    case types::ng_enb:
      c.init<globalng_enb_id_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "global_ng_ran_node_id_c");
  }
}
global_ng_ran_node_id_c::global_ng_ran_node_id_c(const global_ng_ran_node_id_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::gnb:
      c.init(other.c.get<global_gnb_id_s>());
      break;
    case types::ng_enb:
      c.init(other.c.get<globalng_enb_id_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "global_ng_ran_node_id_c");
  }
}
global_ng_ran_node_id_c& global_ng_ran_node_id_c::operator=(const global_ng_ran_node_id_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::gnb:
      c.set(other.c.get<global_gnb_id_s>());
      break;
    case types::ng_enb:
      c.set(other.c.get<globalng_enb_id_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "global_ng_ran_node_id_c");
  }

  return *this;
}
global_gnb_id_s& global_ng_ran_node_id_c::set_gnb()
{
  set(types::gnb);
  return c.get<global_gnb_id_s>();
}
globalng_enb_id_s& global_ng_ran_node_id_c::set_ng_enb()
{
  set(types::ng_enb);
  return c.get<globalng_enb_id_s>();
}
void global_ng_ran_node_id_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::gnb:
      j.write_fieldname("gNB");
      c.get<global_gnb_id_s>().to_json(j);
      break;
    case types::ng_enb:
      j.write_fieldname("ng-eNB");
      c.get<globalng_enb_id_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "global_ng_ran_node_id_c");
  }
  j.end_obj();
}
OCUDUASN_CODE global_ng_ran_node_id_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::gnb:
      HANDLE_CODE(c.get<global_gnb_id_s>().pack(bref));
      break;
    case types::ng_enb:
      HANDLE_CODE(c.get<globalng_enb_id_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "global_ng_ran_node_id_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_ng_ran_node_id_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::gnb:
      HANDLE_CODE(c.get<global_gnb_id_s>().unpack(bref));
      break;
    case types::ng_enb:
      HANDLE_CODE(c.get<globalng_enb_id_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "global_ng_ran_node_id_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* global_ng_ran_node_id_c::types_opts::to_string() const
{
  static const char* names[] = {"gNB", "ng-eNB"};
  return convert_enum_idx(names, 2, value, "global_ng_ran_node_id_c::types");
}

// E2nodeComponentInterfaceXn ::= SEQUENCE
OCUDUASN_CODE e2node_component_interface_xn_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(global_ng_ran_node_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_interface_xn_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(global_ng_ran_node_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_interface_xn_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("global-NG-RAN-Node-ID");
  global_ng_ran_node_id.to_json(j);
  j.end_obj();
}

// E2nodeComponentInterfaceE1 ::= SEQUENCE
OCUDUASN_CODE e2node_component_interface_e1_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, gnb_cu_up_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_interface_e1_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(gnb_cu_up_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_interface_e1_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("gNB-CU-UP-ID", gnb_cu_up_id);
  j.end_obj();
}

// E2nodeComponentInterfaceF1 ::= SEQUENCE
OCUDUASN_CODE e2node_component_interface_f1_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, gnb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_interface_f1_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(gnb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_interface_f1_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("gNB-DU-ID", gnb_du_id);
  j.end_obj();
}

// E2nodeComponentInterfaceW1 ::= SEQUENCE
OCUDUASN_CODE e2node_component_interface_w1_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ng_enb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_interface_w1_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ng_enb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_interface_w1_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ng-eNB-DU-ID", ng_enb_du_id);
  j.end_obj();
}

// E2nodeComponentInterfaceS1 ::= SEQUENCE
OCUDUASN_CODE e2node_component_interface_s1_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(mme_name.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_interface_s1_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(mme_name.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_interface_s1_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mme-name", mme_name.to_string());
  j.end_obj();
}

// ENB-ID ::= CHOICE
void enb_id_c::destroy_()
{
  switch (type_) {
    case types::macro_enb_id:
      c.destroy<fixed_bitstring<20, false, true>>();
      break;
    case types::home_enb_id:
      c.destroy<fixed_bitstring<28, false, true>>();
      break;
    case types::short_macro_enb_id:
      c.destroy<fixed_bitstring<18, false, true>>();
      break;
    case types::long_macro_enb_id:
      c.destroy<fixed_bitstring<21, false, true>>();
      break;
    default:
      break;
  }
}
void enb_id_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::macro_enb_id:
      c.init<fixed_bitstring<20, false, true>>();
      break;
    case types::home_enb_id:
      c.init<fixed_bitstring<28, false, true>>();
      break;
    case types::short_macro_enb_id:
      c.init<fixed_bitstring<18, false, true>>();
      break;
    case types::long_macro_enb_id:
      c.init<fixed_bitstring<21, false, true>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_c");
  }
}
enb_id_c::enb_id_c(const enb_id_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::macro_enb_id:
      c.init(other.c.get<fixed_bitstring<20, false, true>>());
      break;
    case types::home_enb_id:
      c.init(other.c.get<fixed_bitstring<28, false, true>>());
      break;
    case types::short_macro_enb_id:
      c.init(other.c.get<fixed_bitstring<18, false, true>>());
      break;
    case types::long_macro_enb_id:
      c.init(other.c.get<fixed_bitstring<21, false, true>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_c");
  }
}
enb_id_c& enb_id_c::operator=(const enb_id_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::macro_enb_id:
      c.set(other.c.get<fixed_bitstring<20, false, true>>());
      break;
    case types::home_enb_id:
      c.set(other.c.get<fixed_bitstring<28, false, true>>());
      break;
    case types::short_macro_enb_id:
      c.set(other.c.get<fixed_bitstring<18, false, true>>());
      break;
    case types::long_macro_enb_id:
      c.set(other.c.get<fixed_bitstring<21, false, true>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_c");
  }

  return *this;
}
fixed_bitstring<20, false, true>& enb_id_c::set_macro_enb_id()
{
  set(types::macro_enb_id);
  return c.get<fixed_bitstring<20, false, true>>();
}
fixed_bitstring<28, false, true>& enb_id_c::set_home_enb_id()
{
  set(types::home_enb_id);
  return c.get<fixed_bitstring<28, false, true>>();
}
fixed_bitstring<18, false, true>& enb_id_c::set_short_macro_enb_id()
{
  set(types::short_macro_enb_id);
  return c.get<fixed_bitstring<18, false, true>>();
}
fixed_bitstring<21, false, true>& enb_id_c::set_long_macro_enb_id()
{
  set(types::long_macro_enb_id);
  return c.get<fixed_bitstring<21, false, true>>();
}
void enb_id_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::macro_enb_id:
      j.write_str("macro-eNB-ID", c.get<fixed_bitstring<20, false, true>>().to_string());
      break;
    case types::home_enb_id:
      j.write_str("home-eNB-ID", c.get<fixed_bitstring<28, false, true>>().to_string());
      break;
    case types::short_macro_enb_id:
      j.write_str("short-Macro-eNB-ID", c.get<fixed_bitstring<18, false, true>>().to_string());
      break;
    case types::long_macro_enb_id:
      j.write_str("long-Macro-eNB-ID", c.get<fixed_bitstring<21, false, true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "enb_id_c");
  }
  j.end_obj();
}
OCUDUASN_CODE enb_id_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::macro_enb_id:
      HANDLE_CODE((c.get<fixed_bitstring<20, false, true>>().pack(bref)));
      break;
    case types::home_enb_id:
      HANDLE_CODE((c.get<fixed_bitstring<28, false, true>>().pack(bref)));
      break;
    case types::short_macro_enb_id: {
      varlength_field_pack_guard varlen_scope(bref, true);
      HANDLE_CODE((c.get<fixed_bitstring<18, false, true>>().pack(bref)));
    } break;
    case types::long_macro_enb_id: {
      varlength_field_pack_guard varlen_scope(bref, true);
      HANDLE_CODE((c.get<fixed_bitstring<21, false, true>>().pack(bref)));
    } break;
    default:
      log_invalid_choice_id(type_, "enb_id_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE enb_id_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::macro_enb_id:
      HANDLE_CODE((c.get<fixed_bitstring<20, false, true>>().unpack(bref)));
      break;
    case types::home_enb_id:
      HANDLE_CODE((c.get<fixed_bitstring<28, false, true>>().unpack(bref)));
      break;
    case types::short_macro_enb_id: {
      varlength_field_unpack_guard varlen_scope(bref, true);
      HANDLE_CODE((c.get<fixed_bitstring<18, false, true>>().unpack(bref)));
    } break;
    case types::long_macro_enb_id: {
      varlength_field_unpack_guard varlen_scope(bref, true);
      HANDLE_CODE((c.get<fixed_bitstring<21, false, true>>().unpack(bref)));
    } break;
    default:
      log_invalid_choice_id(type_, "enb_id_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* enb_id_c::types_opts::to_string() const
{
  static const char* names[] = {"macro-eNB-ID", "home-eNB-ID", "short-Macro-eNB-ID", "long-Macro-eNB-ID"};
  return convert_enum_idx(names, 4, value, "enb_id_c::types");
}

// GlobalENB-ID ::= SEQUENCE
OCUDUASN_CODE global_enb_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(enb_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_enb_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(enb_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void global_enb_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  j.write_fieldname("eNB-ID");
  enb_id.to_json(j);
  j.end_obj();
}

// ENGNB-ID ::= CHOICE
void en_gnb_id_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("gNB-ID", c.to_string());
  j.end_obj();
}
OCUDUASN_CODE en_gnb_id_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_enum(bref, type()));
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE en_gnb_id_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(unpack_enum(e, bref));
  if (e != type()) {
    log_invalid_choice_id(e, "en_gnb_id_c");
    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* en_gnb_id_c::types_opts::to_string() const
{
  static const char* names[] = {"gNB-ID"};
  return convert_enum_idx(names, 1, value, "en_gnb_id_c::types");
}

// GlobalenGNB-ID ::= SEQUENCE
OCUDUASN_CODE globalen_gnb_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(gnb_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE globalen_gnb_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(gnb_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void globalen_gnb_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  j.write_fieldname("gNB-ID");
  gnb_id.to_json(j);
  j.end_obj();
}

// E2nodeComponentInterfaceX2 ::= SEQUENCE
OCUDUASN_CODE e2node_component_interface_x2_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(global_enb_id_present, 1));
  HANDLE_CODE(bref.pack(global_en_gnb_id_present, 1));

  if (global_enb_id_present) {
    HANDLE_CODE(global_enb_id.pack(bref));
  }
  if (global_en_gnb_id_present) {
    HANDLE_CODE(global_en_gnb_id.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_interface_x2_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(global_enb_id_present, 1));
  HANDLE_CODE(bref.unpack(global_en_gnb_id_present, 1));

  if (global_enb_id_present) {
    HANDLE_CODE(global_enb_id.unpack(bref));
  }
  if (global_en_gnb_id_present) {
    HANDLE_CODE(global_en_gnb_id.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_interface_x2_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (global_enb_id_present) {
    j.write_fieldname("global-eNB-ID");
    global_enb_id.to_json(j);
  }
  if (global_en_gnb_id_present) {
    j.write_fieldname("global-en-gNB-ID");
    global_en_gnb_id.to_json(j);
  }
  j.end_obj();
}

// E2nodeComponentID ::= CHOICE
void e2node_component_id_c::destroy_()
{
  switch (type_) {
    case types::e2node_component_interface_type_ng:
      c.destroy<e2node_component_interface_ng_s>();
      break;
    case types::e2node_component_interface_type_xn:
      c.destroy<e2node_component_interface_xn_s>();
      break;
    case types::e2node_component_interface_type_e1:
      c.destroy<e2node_component_interface_e1_s>();
      break;
    case types::e2node_component_interface_type_f1:
      c.destroy<e2node_component_interface_f1_s>();
      break;
    case types::e2node_component_interface_type_w1:
      c.destroy<e2node_component_interface_w1_s>();
      break;
    case types::e2node_component_interface_type_s1:
      c.destroy<e2node_component_interface_s1_s>();
      break;
    case types::e2node_component_interface_type_x2:
      c.destroy<e2node_component_interface_x2_s>();
      break;
    default:
      break;
  }
}
void e2node_component_id_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::e2node_component_interface_type_ng:
      c.init<e2node_component_interface_ng_s>();
      break;
    case types::e2node_component_interface_type_xn:
      c.init<e2node_component_interface_xn_s>();
      break;
    case types::e2node_component_interface_type_e1:
      c.init<e2node_component_interface_e1_s>();
      break;
    case types::e2node_component_interface_type_f1:
      c.init<e2node_component_interface_f1_s>();
      break;
    case types::e2node_component_interface_type_w1:
      c.init<e2node_component_interface_w1_s>();
      break;
    case types::e2node_component_interface_type_s1:
      c.init<e2node_component_interface_s1_s>();
      break;
    case types::e2node_component_interface_type_x2:
      c.init<e2node_component_interface_x2_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2node_component_id_c");
  }
}
e2node_component_id_c::e2node_component_id_c(const e2node_component_id_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::e2node_component_interface_type_ng:
      c.init(other.c.get<e2node_component_interface_ng_s>());
      break;
    case types::e2node_component_interface_type_xn:
      c.init(other.c.get<e2node_component_interface_xn_s>());
      break;
    case types::e2node_component_interface_type_e1:
      c.init(other.c.get<e2node_component_interface_e1_s>());
      break;
    case types::e2node_component_interface_type_f1:
      c.init(other.c.get<e2node_component_interface_f1_s>());
      break;
    case types::e2node_component_interface_type_w1:
      c.init(other.c.get<e2node_component_interface_w1_s>());
      break;
    case types::e2node_component_interface_type_s1:
      c.init(other.c.get<e2node_component_interface_s1_s>());
      break;
    case types::e2node_component_interface_type_x2:
      c.init(other.c.get<e2node_component_interface_x2_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2node_component_id_c");
  }
}
e2node_component_id_c& e2node_component_id_c::operator=(const e2node_component_id_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::e2node_component_interface_type_ng:
      c.set(other.c.get<e2node_component_interface_ng_s>());
      break;
    case types::e2node_component_interface_type_xn:
      c.set(other.c.get<e2node_component_interface_xn_s>());
      break;
    case types::e2node_component_interface_type_e1:
      c.set(other.c.get<e2node_component_interface_e1_s>());
      break;
    case types::e2node_component_interface_type_f1:
      c.set(other.c.get<e2node_component_interface_f1_s>());
      break;
    case types::e2node_component_interface_type_w1:
      c.set(other.c.get<e2node_component_interface_w1_s>());
      break;
    case types::e2node_component_interface_type_s1:
      c.set(other.c.get<e2node_component_interface_s1_s>());
      break;
    case types::e2node_component_interface_type_x2:
      c.set(other.c.get<e2node_component_interface_x2_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2node_component_id_c");
  }

  return *this;
}
e2node_component_interface_ng_s& e2node_component_id_c::set_e2node_component_interface_type_ng()
{
  set(types::e2node_component_interface_type_ng);
  return c.get<e2node_component_interface_ng_s>();
}
e2node_component_interface_xn_s& e2node_component_id_c::set_e2node_component_interface_type_xn()
{
  set(types::e2node_component_interface_type_xn);
  return c.get<e2node_component_interface_xn_s>();
}
e2node_component_interface_e1_s& e2node_component_id_c::set_e2node_component_interface_type_e1()
{
  set(types::e2node_component_interface_type_e1);
  return c.get<e2node_component_interface_e1_s>();
}
e2node_component_interface_f1_s& e2node_component_id_c::set_e2node_component_interface_type_f1()
{
  set(types::e2node_component_interface_type_f1);
  return c.get<e2node_component_interface_f1_s>();
}
e2node_component_interface_w1_s& e2node_component_id_c::set_e2node_component_interface_type_w1()
{
  set(types::e2node_component_interface_type_w1);
  return c.get<e2node_component_interface_w1_s>();
}
e2node_component_interface_s1_s& e2node_component_id_c::set_e2node_component_interface_type_s1()
{
  set(types::e2node_component_interface_type_s1);
  return c.get<e2node_component_interface_s1_s>();
}
e2node_component_interface_x2_s& e2node_component_id_c::set_e2node_component_interface_type_x2()
{
  set(types::e2node_component_interface_type_x2);
  return c.get<e2node_component_interface_x2_s>();
}
void e2node_component_id_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::e2node_component_interface_type_ng:
      j.write_fieldname("e2nodeComponentInterfaceTypeNG");
      c.get<e2node_component_interface_ng_s>().to_json(j);
      break;
    case types::e2node_component_interface_type_xn:
      j.write_fieldname("e2nodeComponentInterfaceTypeXn");
      c.get<e2node_component_interface_xn_s>().to_json(j);
      break;
    case types::e2node_component_interface_type_e1:
      j.write_fieldname("e2nodeComponentInterfaceTypeE1");
      c.get<e2node_component_interface_e1_s>().to_json(j);
      break;
    case types::e2node_component_interface_type_f1:
      j.write_fieldname("e2nodeComponentInterfaceTypeF1");
      c.get<e2node_component_interface_f1_s>().to_json(j);
      break;
    case types::e2node_component_interface_type_w1:
      j.write_fieldname("e2nodeComponentInterfaceTypeW1");
      c.get<e2node_component_interface_w1_s>().to_json(j);
      break;
    case types::e2node_component_interface_type_s1:
      j.write_fieldname("e2nodeComponentInterfaceTypeS1");
      c.get<e2node_component_interface_s1_s>().to_json(j);
      break;
    case types::e2node_component_interface_type_x2:
      j.write_fieldname("e2nodeComponentInterfaceTypeX2");
      c.get<e2node_component_interface_x2_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e2node_component_id_c");
  }
  j.end_obj();
}
OCUDUASN_CODE e2node_component_id_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::e2node_component_interface_type_ng:
      HANDLE_CODE(c.get<e2node_component_interface_ng_s>().pack(bref));
      break;
    case types::e2node_component_interface_type_xn:
      HANDLE_CODE(c.get<e2node_component_interface_xn_s>().pack(bref));
      break;
    case types::e2node_component_interface_type_e1:
      HANDLE_CODE(c.get<e2node_component_interface_e1_s>().pack(bref));
      break;
    case types::e2node_component_interface_type_f1:
      HANDLE_CODE(c.get<e2node_component_interface_f1_s>().pack(bref));
      break;
    case types::e2node_component_interface_type_w1:
      HANDLE_CODE(c.get<e2node_component_interface_w1_s>().pack(bref));
      break;
    case types::e2node_component_interface_type_s1:
      HANDLE_CODE(c.get<e2node_component_interface_s1_s>().pack(bref));
      break;
    case types::e2node_component_interface_type_x2:
      HANDLE_CODE(c.get<e2node_component_interface_x2_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2node_component_id_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_id_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::e2node_component_interface_type_ng:
      HANDLE_CODE(c.get<e2node_component_interface_ng_s>().unpack(bref));
      break;
    case types::e2node_component_interface_type_xn:
      HANDLE_CODE(c.get<e2node_component_interface_xn_s>().unpack(bref));
      break;
    case types::e2node_component_interface_type_e1:
      HANDLE_CODE(c.get<e2node_component_interface_e1_s>().unpack(bref));
      break;
    case types::e2node_component_interface_type_f1:
      HANDLE_CODE(c.get<e2node_component_interface_f1_s>().unpack(bref));
      break;
    case types::e2node_component_interface_type_w1:
      HANDLE_CODE(c.get<e2node_component_interface_w1_s>().unpack(bref));
      break;
    case types::e2node_component_interface_type_s1:
      HANDLE_CODE(c.get<e2node_component_interface_s1_s>().unpack(bref));
      break;
    case types::e2node_component_interface_type_x2:
      HANDLE_CODE(c.get<e2node_component_interface_x2_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2node_component_id_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2node_component_id_c::types_opts::to_string() const
{
  static const char* names[] = {"e2nodeComponentInterfaceTypeNG",
                                "e2nodeComponentInterfaceTypeXn",
                                "e2nodeComponentInterfaceTypeE1",
                                "e2nodeComponentInterfaceTypeF1",
                                "e2nodeComponentInterfaceTypeW1",
                                "e2nodeComponentInterfaceTypeS1",
                                "e2nodeComponentInterfaceTypeX2"};
  return convert_enum_idx(names, 7, value, "e2node_component_id_c::types");
}

// E2nodeComponentInterfaceType ::= ENUMERATED
const char* e2node_component_interface_type_opts::to_string() const
{
  static const char* names[] = {"ng", "xn", "e1", "f1", "w1", "s1", "x2"};
  return convert_enum_idx(names, 7, value, "e2node_component_interface_type_e");
}

// GlobalE2node-gNB-ID ::= SEQUENCE
OCUDUASN_CODE global_e2node_gnb_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(global_en_gnb_id_present, 1));
  HANDLE_CODE(bref.pack(gnb_cu_up_id_present, 1));
  HANDLE_CODE(bref.pack(gnb_du_id_present, 1));

  HANDLE_CODE(global_gnb_id.pack(bref));
  if (global_en_gnb_id_present) {
    HANDLE_CODE(global_en_gnb_id.pack(bref));
  }
  if (gnb_cu_up_id_present) {
    HANDLE_CODE(pack_integer(bref, gnb_cu_up_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }
  if (gnb_du_id_present) {
    HANDLE_CODE(pack_integer(bref, gnb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_e2node_gnb_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(global_en_gnb_id_present, 1));
  HANDLE_CODE(bref.unpack(gnb_cu_up_id_present, 1));
  HANDLE_CODE(bref.unpack(gnb_du_id_present, 1));

  HANDLE_CODE(global_gnb_id.unpack(bref));
  if (global_en_gnb_id_present) {
    HANDLE_CODE(global_en_gnb_id.unpack(bref));
  }
  if (gnb_cu_up_id_present) {
    HANDLE_CODE(unpack_integer(gnb_cu_up_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }
  if (gnb_du_id_present) {
    HANDLE_CODE(unpack_integer(gnb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void global_e2node_gnb_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("global-gNB-ID");
  global_gnb_id.to_json(j);
  if (global_en_gnb_id_present) {
    j.write_fieldname("global-en-gNB-ID");
    global_en_gnb_id.to_json(j);
  }
  if (gnb_cu_up_id_present) {
    j.write_int("gNB-CU-UP-ID", gnb_cu_up_id);
  }
  if (gnb_du_id_present) {
    j.write_int("gNB-DU-ID", gnb_du_id);
  }
  j.end_obj();
}

// GlobalE2node-en-gNB-ID ::= SEQUENCE
OCUDUASN_CODE global_e2node_en_gnb_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(en_gnb_cu_up_id_present, 1));
  HANDLE_CODE(bref.pack(en_gnb_du_id_present, 1));

  HANDLE_CODE(global_en_gnb_id.pack(bref));
  if (en_gnb_cu_up_id_present) {
    HANDLE_CODE(pack_integer(bref, en_gnb_cu_up_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }
  if (en_gnb_du_id_present) {
    HANDLE_CODE(pack_integer(bref, en_gnb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_e2node_en_gnb_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(en_gnb_cu_up_id_present, 1));
  HANDLE_CODE(bref.unpack(en_gnb_du_id_present, 1));

  HANDLE_CODE(global_en_gnb_id.unpack(bref));
  if (en_gnb_cu_up_id_present) {
    HANDLE_CODE(unpack_integer(en_gnb_cu_up_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }
  if (en_gnb_du_id_present) {
    HANDLE_CODE(unpack_integer(en_gnb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void global_e2node_en_gnb_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("global-en-gNB-ID");
  global_en_gnb_id.to_json(j);
  if (en_gnb_cu_up_id_present) {
    j.write_int("en-gNB-CU-UP-ID", en_gnb_cu_up_id);
  }
  if (en_gnb_du_id_present) {
    j.write_int("en-gNB-DU-ID", en_gnb_du_id);
  }
  j.end_obj();
}

// GlobalE2node-ng-eNB-ID ::= SEQUENCE
OCUDUASN_CODE global_e2node_ng_enb_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(global_enb_id_present, 1));
  HANDLE_CODE(bref.pack(ng_enb_du_id_present, 1));

  HANDLE_CODE(global_ng_enb_id.pack(bref));
  if (global_enb_id_present) {
    HANDLE_CODE(global_enb_id.pack(bref));
  }
  if (ng_enb_du_id_present) {
    HANDLE_CODE(pack_integer(bref, ng_enb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_e2node_ng_enb_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(global_enb_id_present, 1));
  HANDLE_CODE(bref.unpack(ng_enb_du_id_present, 1));

  HANDLE_CODE(global_ng_enb_id.unpack(bref));
  if (global_enb_id_present) {
    HANDLE_CODE(global_enb_id.unpack(bref));
  }
  if (ng_enb_du_id_present) {
    HANDLE_CODE(unpack_integer(ng_enb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void global_e2node_ng_enb_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("global-ng-eNB-ID");
  global_ng_enb_id.to_json(j);
  if (global_enb_id_present) {
    j.write_fieldname("global-eNB-ID");
    global_enb_id.to_json(j);
  }
  if (ng_enb_du_id_present) {
    j.write_int("ngENB-DU-ID", ng_enb_du_id);
  }
  j.end_obj();
}

// GlobalE2node-eNB-ID ::= SEQUENCE
OCUDUASN_CODE global_e2node_enb_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(global_enb_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_e2node_enb_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(global_enb_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void global_e2node_enb_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("global-eNB-ID");
  global_enb_id.to_json(j);
  j.end_obj();
}

// GlobalE2node-ID ::= CHOICE
void global_e2node_id_c::destroy_()
{
  switch (type_) {
    case types::gnb:
      c.destroy<global_e2node_gnb_id_s>();
      break;
    case types::en_gnb:
      c.destroy<global_e2node_en_gnb_id_s>();
      break;
    case types::ng_enb:
      c.destroy<global_e2node_ng_enb_id_s>();
      break;
    case types::enb:
      c.destroy<global_e2node_enb_id_s>();
      break;
    default:
      break;
  }
}
void global_e2node_id_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::gnb:
      c.init<global_e2node_gnb_id_s>();
      break;
    case types::en_gnb:
      c.init<global_e2node_en_gnb_id_s>();
      break;
    case types::ng_enb:
      c.init<global_e2node_ng_enb_id_s>();
      break;
    case types::enb:
      c.init<global_e2node_enb_id_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "global_e2node_id_c");
  }
}
global_e2node_id_c::global_e2node_id_c(const global_e2node_id_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::gnb:
      c.init(other.c.get<global_e2node_gnb_id_s>());
      break;
    case types::en_gnb:
      c.init(other.c.get<global_e2node_en_gnb_id_s>());
      break;
    case types::ng_enb:
      c.init(other.c.get<global_e2node_ng_enb_id_s>());
      break;
    case types::enb:
      c.init(other.c.get<global_e2node_enb_id_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "global_e2node_id_c");
  }
}
global_e2node_id_c& global_e2node_id_c::operator=(const global_e2node_id_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::gnb:
      c.set(other.c.get<global_e2node_gnb_id_s>());
      break;
    case types::en_gnb:
      c.set(other.c.get<global_e2node_en_gnb_id_s>());
      break;
    case types::ng_enb:
      c.set(other.c.get<global_e2node_ng_enb_id_s>());
      break;
    case types::enb:
      c.set(other.c.get<global_e2node_enb_id_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "global_e2node_id_c");
  }

  return *this;
}
global_e2node_gnb_id_s& global_e2node_id_c::set_gnb()
{
  set(types::gnb);
  return c.get<global_e2node_gnb_id_s>();
}
global_e2node_en_gnb_id_s& global_e2node_id_c::set_en_gnb()
{
  set(types::en_gnb);
  return c.get<global_e2node_en_gnb_id_s>();
}
global_e2node_ng_enb_id_s& global_e2node_id_c::set_ng_enb()
{
  set(types::ng_enb);
  return c.get<global_e2node_ng_enb_id_s>();
}
global_e2node_enb_id_s& global_e2node_id_c::set_enb()
{
  set(types::enb);
  return c.get<global_e2node_enb_id_s>();
}
void global_e2node_id_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::gnb:
      j.write_fieldname("gNB");
      c.get<global_e2node_gnb_id_s>().to_json(j);
      break;
    case types::en_gnb:
      j.write_fieldname("en-gNB");
      c.get<global_e2node_en_gnb_id_s>().to_json(j);
      break;
    case types::ng_enb:
      j.write_fieldname("ng-eNB");
      c.get<global_e2node_ng_enb_id_s>().to_json(j);
      break;
    case types::enb:
      j.write_fieldname("eNB");
      c.get<global_e2node_enb_id_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "global_e2node_id_c");
  }
  j.end_obj();
}
OCUDUASN_CODE global_e2node_id_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::gnb:
      HANDLE_CODE(c.get<global_e2node_gnb_id_s>().pack(bref));
      break;
    case types::en_gnb:
      HANDLE_CODE(c.get<global_e2node_en_gnb_id_s>().pack(bref));
      break;
    case types::ng_enb:
      HANDLE_CODE(c.get<global_e2node_ng_enb_id_s>().pack(bref));
      break;
    case types::enb:
      HANDLE_CODE(c.get<global_e2node_enb_id_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "global_e2node_id_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_e2node_id_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::gnb:
      HANDLE_CODE(c.get<global_e2node_gnb_id_s>().unpack(bref));
      break;
    case types::en_gnb:
      HANDLE_CODE(c.get<global_e2node_en_gnb_id_s>().unpack(bref));
      break;
    case types::ng_enb:
      HANDLE_CODE(c.get<global_e2node_ng_enb_id_s>().unpack(bref));
      break;
    case types::enb:
      HANDLE_CODE(c.get<global_e2node_enb_id_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "global_e2node_id_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* global_e2node_id_c::types_opts::to_string() const
{
  static const char* names[] = {"gNB", "en-gNB", "ng-eNB", "eNB"};
  return convert_enum_idx(names, 4, value, "global_e2node_id_c::types");
}

// GlobalRIC-ID ::= SEQUENCE
OCUDUASN_CODE global_ric_id_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(plmn_id.pack(bref));
  HANDLE_CODE(ric_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE global_ric_id_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(plmn_id.unpack(bref));
  HANDLE_CODE(ric_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void global_ric_id_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("pLMN-Identity", plmn_id.to_string());
  j.write_str("ric-ID", ric_id.to_string());
  j.end_obj();
}

// ListedRecordsOnly ::= ENUMERATED
const char* listed_records_only_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "listed_records_only_e");
}

// RICactionType ::= ENUMERATED
const char* ric_action_type_opts::to_string() const
{
  static const char* names[] = {"report", "insert", "policy"};
  return convert_enum_idx(names, 3, value, "ric_action_type_e");
}

// RICassistanceUpdate ::= ENUMERATED
const char* ric_assist_upd_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "ric_assist_upd_e");
}

// RICcontrolAckRequest ::= ENUMERATED
const char* ric_ctrl_ack_request_opts::to_string() const
{
  static const char* names[] = {"noAck", "ack"};
  return convert_enum_idx(names, 2, value, "ric_ctrl_ack_request_e");
}

// RICindicationType ::= ENUMERATED
const char* ric_ind_type_opts::to_string() const
{
  static const char* names[] = {"report", "insert"};
  return convert_enum_idx(names, 2, value, "ric_ind_type_e");
}

// RICloadConfirm ::= ENUMERATED
const char* ric_load_confirm_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "ric_load_confirm_e");
}

// RICloadInformation ::= SEQUENCE
OCUDUASN_CODE ric_load_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(load_status_present, 1));
  HANDLE_CODE(bref.pack(load_estimate_present, 1));

  if (load_status_present) {
    HANDLE_CODE(load_status.pack(bref));
  }
  if (load_estimate_present) {
    HANDLE_CODE(pack_integer(bref, load_estimate, (uint8_t)0u, (uint8_t)100u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_load_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(load_status_present, 1));
  HANDLE_CODE(bref.unpack(load_estimate_present, 1));

  if (load_status_present) {
    HANDLE_CODE(load_status.unpack(bref));
  }
  if (load_estimate_present) {
    HANDLE_CODE(unpack_integer(load_estimate, bref, (uint8_t)0u, (uint8_t)100u, false, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_load_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (load_status_present) {
    j.write_str("loadStatus", load_status.to_string());
  }
  if (load_estimate_present) {
    j.write_int("loadEstimate", load_estimate);
  }
  j.end_obj();
}

const char* ric_load_info_s::load_status_opts::to_string() const
{
  static const char* names[] = {"overload", "notoverload"};
  return convert_enum_idx(names, 2, value, "ric_load_info_s::load_status_e_");
}

// RICloadRequest ::= ENUMERATED
const char* ric_load_request_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "ric_load_request_e");
}

// RICserviceLoadConfirm ::= SEQUENCE
OCUDUASN_CODE ric_service_load_confirm_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_service_report_load_confirm_present, 1));
  HANDLE_CODE(bref.pack(ric_service_insert_load_confirm_present, 1));
  HANDLE_CODE(bref.pack(ric_service_ctrl_load_confirm_present, 1));
  HANDLE_CODE(bref.pack(ric_service_policy_load_confirm_present, 1));
  HANDLE_CODE(bref.pack(ric_service_query_load_confirm_present, 1));

  if (ric_service_report_load_confirm_present) {
    HANDLE_CODE(ric_service_report_load_confirm.pack(bref));
  }
  if (ric_service_insert_load_confirm_present) {
    HANDLE_CODE(ric_service_insert_load_confirm.pack(bref));
  }
  if (ric_service_ctrl_load_confirm_present) {
    HANDLE_CODE(ric_service_ctrl_load_confirm.pack(bref));
  }
  if (ric_service_policy_load_confirm_present) {
    HANDLE_CODE(ric_service_policy_load_confirm.pack(bref));
  }
  if (ric_service_query_load_confirm_present) {
    HANDLE_CODE(ric_service_query_load_confirm.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_confirm_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ric_service_report_load_confirm_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_insert_load_confirm_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_ctrl_load_confirm_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_policy_load_confirm_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_query_load_confirm_present, 1));

  if (ric_service_report_load_confirm_present) {
    HANDLE_CODE(ric_service_report_load_confirm.unpack(bref));
  }
  if (ric_service_insert_load_confirm_present) {
    HANDLE_CODE(ric_service_insert_load_confirm.unpack(bref));
  }
  if (ric_service_ctrl_load_confirm_present) {
    HANDLE_CODE(ric_service_ctrl_load_confirm.unpack(bref));
  }
  if (ric_service_policy_load_confirm_present) {
    HANDLE_CODE(ric_service_policy_load_confirm.unpack(bref));
  }
  if (ric_service_query_load_confirm_present) {
    HANDLE_CODE(ric_service_query_load_confirm.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_service_load_confirm_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ric_service_report_load_confirm_present) {
    j.write_str("ricServiceReportLoadConfirm", "true");
  }
  if (ric_service_insert_load_confirm_present) {
    j.write_str("ricServiceInsertLoadConfirm", "true");
  }
  if (ric_service_ctrl_load_confirm_present) {
    j.write_str("ricServiceControlLoadConfirm", "true");
  }
  if (ric_service_policy_load_confirm_present) {
    j.write_str("ricServicePolicyLoadConfirm", "true");
  }
  if (ric_service_query_load_confirm_present) {
    j.write_str("ricServiceQueryLoadConfirm", "true");
  }
  j.end_obj();
}

// RICserviceLoadInformation ::= SEQUENCE
OCUDUASN_CODE ric_service_load_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_service_report_load_info_present, 1));
  HANDLE_CODE(bref.pack(ric_service_insert_load_info_present, 1));
  HANDLE_CODE(bref.pack(ric_service_ctrl_load_info_present, 1));
  HANDLE_CODE(bref.pack(ric_service_policy_load_info_present, 1));
  HANDLE_CODE(bref.pack(ric_service_query_load_info_present, 1));

  if (ric_service_report_load_info_present) {
    HANDLE_CODE(ric_service_report_load_info.pack(bref));
  }
  if (ric_service_insert_load_info_present) {
    HANDLE_CODE(ric_service_insert_load_info.pack(bref));
  }
  if (ric_service_ctrl_load_info_present) {
    HANDLE_CODE(ric_service_ctrl_load_info.pack(bref));
  }
  if (ric_service_policy_load_info_present) {
    HANDLE_CODE(ric_service_policy_load_info.pack(bref));
  }
  if (ric_service_query_load_info_present) {
    HANDLE_CODE(ric_service_query_load_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ric_service_report_load_info_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_insert_load_info_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_ctrl_load_info_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_policy_load_info_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_query_load_info_present, 1));

  if (ric_service_report_load_info_present) {
    HANDLE_CODE(ric_service_report_load_info.unpack(bref));
  }
  if (ric_service_insert_load_info_present) {
    HANDLE_CODE(ric_service_insert_load_info.unpack(bref));
  }
  if (ric_service_ctrl_load_info_present) {
    HANDLE_CODE(ric_service_ctrl_load_info.unpack(bref));
  }
  if (ric_service_policy_load_info_present) {
    HANDLE_CODE(ric_service_policy_load_info.unpack(bref));
  }
  if (ric_service_query_load_info_present) {
    HANDLE_CODE(ric_service_query_load_info.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_service_load_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ric_service_report_load_info_present) {
    j.write_fieldname("ricServiceReportLoadInformation");
    ric_service_report_load_info.to_json(j);
  }
  if (ric_service_insert_load_info_present) {
    j.write_fieldname("ricServiceInsertLoadInformation");
    ric_service_insert_load_info.to_json(j);
  }
  if (ric_service_ctrl_load_info_present) {
    j.write_fieldname("ricServiceControlLoadInformation");
    ric_service_ctrl_load_info.to_json(j);
  }
  if (ric_service_policy_load_info_present) {
    j.write_fieldname("ricServicePolicyLoadInformation");
    ric_service_policy_load_info.to_json(j);
  }
  if (ric_service_query_load_info_present) {
    j.write_fieldname("ricServiceQueryLoadInformation");
    ric_service_query_load_info.to_json(j);
  }
  j.end_obj();
}

// RICserviceLoadRequest ::= SEQUENCE
OCUDUASN_CODE ric_service_load_request_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_service_report_load_request_present, 1));
  HANDLE_CODE(bref.pack(ric_service_insert_load_request_present, 1));
  HANDLE_CODE(bref.pack(ric_service_ctrl_load_request_present, 1));
  HANDLE_CODE(bref.pack(ric_service_policy_load_request_present, 1));
  HANDLE_CODE(bref.pack(ric_service_query_load_request_present, 1));

  if (ric_service_report_load_request_present) {
    HANDLE_CODE(ric_service_report_load_request.pack(bref));
  }
  if (ric_service_insert_load_request_present) {
    HANDLE_CODE(ric_service_insert_load_request.pack(bref));
  }
  if (ric_service_ctrl_load_request_present) {
    HANDLE_CODE(ric_service_ctrl_load_request.pack(bref));
  }
  if (ric_service_policy_load_request_present) {
    HANDLE_CODE(ric_service_policy_load_request.pack(bref));
  }
  if (ric_service_query_load_request_present) {
    HANDLE_CODE(ric_service_query_load_request.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_request_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ric_service_report_load_request_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_insert_load_request_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_ctrl_load_request_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_policy_load_request_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_query_load_request_present, 1));

  if (ric_service_report_load_request_present) {
    HANDLE_CODE(ric_service_report_load_request.unpack(bref));
  }
  if (ric_service_insert_load_request_present) {
    HANDLE_CODE(ric_service_insert_load_request.unpack(bref));
  }
  if (ric_service_ctrl_load_request_present) {
    HANDLE_CODE(ric_service_ctrl_load_request.unpack(bref));
  }
  if (ric_service_policy_load_request_present) {
    HANDLE_CODE(ric_service_policy_load_request.unpack(bref));
  }
  if (ric_service_query_load_request_present) {
    HANDLE_CODE(ric_service_query_load_request.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_service_load_request_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ric_service_report_load_request_present) {
    j.write_str("ricServiceReportLoadRequest", "true");
  }
  if (ric_service_insert_load_request_present) {
    j.write_str("ricServiceInsertLoadRequest", "true");
  }
  if (ric_service_ctrl_load_request_present) {
    j.write_str("ricServiceControlLoadRequest", "true");
  }
  if (ric_service_policy_load_request_present) {
    j.write_str("ricServicePolicyLoadRequest", "true");
  }
  if (ric_service_query_load_request_present) {
    j.write_str("ricServiceQueryLoadRequest", "true");
  }
  j.end_obj();
}

// RICsubscriptionAuditFlag ::= SEQUENCE
OCUDUASN_CODE ric_sub_audit_flag_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(listed_records_only_present, 1));

  if (listed_records_only_present) {
    HANDLE_CODE(listed_records_only.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_flag_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(listed_records_only_present, 1));

  if (listed_records_only_present) {
    HANDLE_CODE(listed_records_only.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_audit_flag_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (listed_records_only_present) {
    j.write_str("listedRecordsOnly", "true");
  }
  j.end_obj();
}

// RICsubsequentActionType ::= ENUMERATED
const char* ric_subsequent_action_type_opts::to_string() const
{
  static const char* names[] = {"continue", "wait"};
  return convert_enum_idx(names, 2, value, "ric_subsequent_action_type_e");
}

// RICtimeToWait ::= ENUMERATED
const char* ric_time_to_wait_opts::to_string() const
{
  static const char* names[] = {"w1ms",
                                "w2ms",
                                "w5ms",
                                "w10ms",
                                "w20ms",
                                "w30ms",
                                "w40ms",
                                "w50ms",
                                "w100ms",
                                "w200ms",
                                "w500ms",
                                "w1s",
                                "w2s",
                                "w5s",
                                "w10s",
                                "w20s",
                                "w60s"};
  return convert_enum_idx(names, 17, value, "ric_time_to_wait_e");
}

// RICsubsequentAction ::= SEQUENCE
OCUDUASN_CODE ric_subsequent_action_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(ric_subsequent_action_type.pack(bref));
  HANDLE_CODE(ric_time_to_wait.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_subsequent_action_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(ric_subsequent_action_type.unpack(bref));
  HANDLE_CODE(ric_time_to_wait.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_subsequent_action_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("ricSubsequentActionType", ric_subsequent_action_type.to_string());
  j.write_str("ricTimeToWait", ric_time_to_wait.to_string());
  j.end_obj();
}

// RegistrationRequest ::= ENUMERATED
const char* regist_request_opts::to_string() const
{
  static const char* names[] = {"start", "stop", "add"};
  return convert_enum_idx(names, 3, value, "regist_request_e");
}

// RegistrationRequest-E2 ::= ENUMERATED
const char* regist_request_e2_opts::to_string() const
{
  static const char* names[] = {"start", "stop", "add"};
  return convert_enum_idx(names, 3, value, "regist_request_e2_e");
}

// ReportingPeriodicity ::= ENUMERATED
const char* report_periodicity_opts::to_string() const
{
  static const char* names[] = {"ms500", "ms1000", "ms2000", "ms5000", "ms10000"};
  return convert_enum_idx(names, 5, value, "report_periodicity_e");
}
uint16_t report_periodicity_opts::to_number() const
{
  static const uint16_t numbers[] = {500, 1000, 2000, 5000, 10000};
  return map_enum_number(numbers, 5, value, "report_periodicity_e");
}

// ReportingPeriodicity-E2 ::= ENUMERATED
const char* report_periodicity_e2_opts::to_string() const
{
  static const char* names[] = {"ms500", "ms1000", "ms2000", "ms5000", "ms10000"};
  return convert_enum_idx(names, 5, value, "report_periodicity_e2_e");
}
uint16_t report_periodicity_e2_opts::to_number() const
{
  static const uint16_t numbers[] = {500, 1000, 2000, 5000, 10000};
  return map_enum_number(numbers, 5, value, "report_periodicity_e2_e");
}

// TNLinformation ::= SEQUENCE
OCUDUASN_CODE tnl_info_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(tnl_port_present, 1));

  HANDLE_CODE(tnl_address.pack(bref));
  if (tnl_port_present) {
    HANDLE_CODE(tnl_port.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tnl_info_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(tnl_port_present, 1));

  HANDLE_CODE(tnl_address.unpack(bref));
  if (tnl_port_present) {
    HANDLE_CODE(tnl_port.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void tnl_info_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("tnlAddress", tnl_address.to_string());
  if (tnl_port_present) {
    j.write_str("tnlPort", tnl_port.to_string());
  }
  j.end_obj();
}

// TNLusage ::= ENUMERATED
const char* tnl_usage_opts::to_string() const
{
  static const char* names[] = {"ric-service", "support-function", "both"};
  return convert_enum_idx(names, 3, value, "tnl_usage_e");
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
