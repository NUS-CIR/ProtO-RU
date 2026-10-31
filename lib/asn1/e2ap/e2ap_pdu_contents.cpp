// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/e2ap/e2ap_pdu_contents.h"

using namespace asn1;
using namespace asn1::e2ap;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// E2RemovalFailureIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2_removal_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 1, 2};
  return map_enum_number(names, 3, idx, "id");
}
bool e2_removal_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2_removal_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2_removal_fail_ies_o::value_c e2_removal_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2_removal_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2_removal_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2_removal_fail_ies_o::value_c");
  }
}
uint16_t& e2_removal_fail_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& e2_removal_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& e2_removal_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& e2_removal_fail_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& e2_removal_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& e2_removal_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void e2_removal_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e2_removal_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE e2_removal_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_removal_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_removal_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_removal_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2_removal_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 3, value, "e2_removal_fail_ies_o::value_c::types");
}
uint8_t e2_removal_fail_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "e2_removal_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2_removal_fail_ies_o>;

OCUDUASN_CODE e2_removal_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_removal_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2_removal_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void e2_removal_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// E2RemovalRequestIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2_removal_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49};
  return map_enum_number(names, 1, idx, "id");
}
bool e2_removal_request_ies_o::is_id_valid(const uint32_t& id)
{
  return 49 == id;
}
crit_e e2_removal_request_ies_o::get_crit(const uint32_t& id)
{
  if (id == 49) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2_removal_request_ies_o::value_c e2_removal_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 49) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2_removal_request_ies_o::get_presence(const uint32_t& id)
{
  if (id == 49) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2_removal_request_ies_o::value_c::to_json(json_writer& j) const
{
  j.write_int(c);
}
OCUDUASN_CODE e2_removal_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_integer(bref, c, (uint16_t)0u, (uint16_t)255u, true, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_removal_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_integer(c, bref, (uint16_t)0u, (uint16_t)255u, true, true));
  return OCUDUASN_SUCCESS;
}

const char* e2_removal_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)"};
  return convert_enum_idx(names, 1, value, "e2_removal_request_ies_o::value_c::types");
}
uint8_t e2_removal_request_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "e2_removal_request_ies_o::value_c::types");
}

// E2RemovalResponseIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2_removal_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 2};
  return map_enum_number(names, 2, idx, "id");
}
bool e2_removal_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2_removal_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2_removal_resp_ies_o::value_c e2_removal_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2_removal_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2_removal_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2_removal_resp_ies_o::value_c");
  }
}
uint16_t& e2_removal_resp_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
crit_diagnostics_s& e2_removal_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& e2_removal_resp_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const crit_diagnostics_s& e2_removal_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void e2_removal_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e2_removal_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE e2_removal_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_removal_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_removal_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_removal_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2_removal_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "e2_removal_resp_ies_o::value_c::types");
}
uint8_t e2_removal_resp_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "e2_removal_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2_removal_resp_ies_o>;

OCUDUASN_CODE e2_removal_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_removal_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2_removal_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void e2_removal_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// E2ResourceStatusFailureIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2_res_status_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 117, 105, 1, 2};
  return map_enum_number(names, 5, idx, "id");
}
bool e2_res_status_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 117, 105, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2_res_status_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 117:
      return crit_e::reject;
    case 105:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2_res_status_fail_ies_o::value_c e2_res_status_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 117:
      ret.set(value_c::types::ric_load_meas_id);
      break;
    case 105:
      ret.set(value_c::types::e2node_load_meas_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2_res_status_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 117:
      return presence_e::mandatory;
    case 105:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2_res_status_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::ric_load_meas_id:
      c = uint16_t{};
      break;
    case types::e2node_load_meas_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_fail_ies_o::value_c");
  }
}
uint16_t& e2_res_status_fail_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& e2_res_status_fail_ies_o::value_c::ric_load_meas_id()
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& e2_res_status_fail_ies_o::value_c::e2node_load_meas_id()
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& e2_res_status_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& e2_res_status_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& e2_res_status_fail_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_fail_ies_o::value_c::ric_load_meas_id() const
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_fail_ies_o::value_c::e2node_load_meas_id() const
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& e2_res_status_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& e2_res_status_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void e2_res_status_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE e2_res_status_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_res_status_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2_res_status_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "INTEGER (1..4095,...)", "INTEGER (1..4095,...)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 5, value, "e2_res_status_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2_res_status_fail_ies_o>;

OCUDUASN_CODE e2_res_status_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, e2node_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_res_status_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2_res_status_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 105: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(e2node_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void e2_res_status_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 105);
  j.write_str("criticality", "reject");
  j.write_int("value", e2node_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// E2ResourceStatusRequestIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2_res_status_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 117, 105, 133, 135, 134};
  return map_enum_number(names, 6, idx, "id");
}
bool e2_res_status_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 117, 105, 133, 135, 134};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2_res_status_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 117:
      return crit_e::reject;
    case 105:
      return crit_e::ignore;
    case 133:
      return crit_e::ignore;
    case 135:
      return crit_e::ignore;
    case 134:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2_res_status_request_ies_o::value_c e2_res_status_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 117:
      ret.set(value_c::types::ric_load_meas_id);
      break;
    case 105:
      ret.set(value_c::types::e2node_load_meas_id);
      break;
    case 133:
      ret.set(value_c::types::regist_request_e2);
      break;
    case 135:
      ret.set(value_c::types::report_characteristics);
      break;
    case 134:
      ret.set(value_c::types::report_periodicity_e2);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2_res_status_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 117:
      return presence_e::mandatory;
    case 105:
      return presence_e::conditional;
    case 133:
      return presence_e::mandatory;
    case 135:
      return presence_e::conditional;
    case 134:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2_res_status_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::ric_load_meas_id:
      c = uint16_t{};
      break;
    case types::e2node_load_meas_id:
      c = uint16_t{};
      break;
    case types::regist_request_e2:
      c = regist_request_e2_e{};
      break;
    case types::report_characteristics:
      c = fixed_bitstring<8, false, true>{};
      break;
    case types::report_periodicity_e2:
      c = report_periodicity_e2_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_request_ies_o::value_c");
  }
}
uint16_t& e2_res_status_request_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& e2_res_status_request_ies_o::value_c::ric_load_meas_id()
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& e2_res_status_request_ies_o::value_c::e2node_load_meas_id()
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
regist_request_e2_e& e2_res_status_request_ies_o::value_c::regist_request_e2()
{
  assert_choice_type(types::regist_request_e2, type_, "Value");
  return c.get<regist_request_e2_e>();
}
fixed_bitstring<8, false, true>& e2_res_status_request_ies_o::value_c::report_characteristics()
{
  assert_choice_type(types::report_characteristics, type_, "Value");
  return c.get<fixed_bitstring<8, false, true>>();
}
report_periodicity_e2_e& e2_res_status_request_ies_o::value_c::report_periodicity_e2()
{
  assert_choice_type(types::report_periodicity_e2, type_, "Value");
  return c.get<report_periodicity_e2_e>();
}
const uint16_t& e2_res_status_request_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_request_ies_o::value_c::ric_load_meas_id() const
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_request_ies_o::value_c::e2node_load_meas_id() const
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const regist_request_e2_e& e2_res_status_request_ies_o::value_c::regist_request_e2() const
{
  assert_choice_type(types::regist_request_e2, type_, "Value");
  return c.get<regist_request_e2_e>();
}
const fixed_bitstring<8, false, true>& e2_res_status_request_ies_o::value_c::report_characteristics() const
{
  assert_choice_type(types::report_characteristics, type_, "Value");
  return c.get<fixed_bitstring<8, false, true>>();
}
const report_periodicity_e2_e& e2_res_status_request_ies_o::value_c::report_periodicity_e2() const
{
  assert_choice_type(types::report_periodicity_e2, type_, "Value");
  return c.get<report_periodicity_e2_e>();
}
void e2_res_status_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::regist_request_e2:
      j.write_str(c.get<regist_request_e2_e>().to_string());
      break;
    case types::report_characteristics:
      c.get<fixed_bitstring<8, false, true>>().to_json(j);
      break;
    case types::report_periodicity_e2:
      j.write_str(c.get<report_periodicity_e2_e>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_request_ies_o::value_c");
  }
}
OCUDUASN_CODE e2_res_status_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::regist_request_e2:
      HANDLE_CODE(c.get<regist_request_e2_e>().pack(bref));
      break;
    case types::report_characteristics:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().pack(bref)));
      break;
    case types::report_periodicity_e2:
      HANDLE_CODE(c.get<report_periodicity_e2_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_res_status_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::regist_request_e2:
      HANDLE_CODE(c.get<regist_request_e2_e>().unpack(bref));
      break;
    case types::report_characteristics:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().unpack(bref)));
      break;
    case types::report_periodicity_e2:
      HANDLE_CODE(c.get<report_periodicity_e2_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2_res_status_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)",
                                "INTEGER (1..4095,...)",
                                "INTEGER (1..4095,...)",
                                "RegistrationRequest-E2",
                                "BIT STRING",
                                "ReportingPeriodicity-E2"};
  return convert_enum_idx(names, 6, value, "e2_res_status_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2_res_status_request_ies_o>;

OCUDUASN_CODE e2_res_status_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += e2node_load_meas_id_present ? 1 : 0;
  nof_ies += report_characteristics_present ? 1 : 0;
  nof_ies += report_periodicity_e2_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  if (e2node_load_meas_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, e2node_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)133, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(regist_request_e2.pack(bref));
  }
  if (report_characteristics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)135, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(report_characteristics.pack(bref));
  }
  if (report_periodicity_e2_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)134, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(report_periodicity_e2.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_res_status_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2_res_status_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 105: {
        e2node_load_meas_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(e2node_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 133: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(regist_request_e2.unpack(bref));
        break;
      }
      case 135: {
        report_characteristics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(report_characteristics.unpack(bref));
        break;
      }
      case 134: {
        report_periodicity_e2_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(report_periodicity_e2.unpack(bref));
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
void e2_res_status_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_load_meas_id);
  j.end_obj();
  if (e2node_load_meas_id_present) {
    j.start_obj();
    j.write_int("id", 105);
    j.write_str("criticality", "ignore");
    j.write_int("value", e2node_load_meas_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 133);
  j.write_str("criticality", "ignore");
  j.write_str("value", regist_request_e2.to_string());
  j.end_obj();
  if (report_characteristics_present) {
    j.start_obj();
    j.write_int("id", 135);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    report_characteristics.to_json(j);
    j.end_obj();
  }
  if (report_periodicity_e2_present) {
    j.start_obj();
    j.write_int("id", 134);
    j.write_str("criticality", "ignore");
    j.write_str("value", report_periodicity_e2.to_string());
    j.end_obj();
  }
  j.end_array();
}

// E2ResourceStatusResponseIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2_res_status_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 117, 105, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool e2_res_status_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 117, 105, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2_res_status_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 117:
      return crit_e::reject;
    case 105:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2_res_status_resp_ies_o::value_c e2_res_status_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 117:
      ret.set(value_c::types::ric_load_meas_id);
      break;
    case 105:
      ret.set(value_c::types::e2node_load_meas_id);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2_res_status_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 117:
      return presence_e::mandatory;
    case 105:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2_res_status_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::ric_load_meas_id:
      c = uint16_t{};
      break;
    case types::e2node_load_meas_id:
      c = uint16_t{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_resp_ies_o::value_c");
  }
}
uint16_t& e2_res_status_resp_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& e2_res_status_resp_ies_o::value_c::ric_load_meas_id()
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& e2_res_status_resp_ies_o::value_c::e2node_load_meas_id()
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
crit_diagnostics_s& e2_res_status_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& e2_res_status_resp_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_resp_ies_o::value_c::ric_load_meas_id() const
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_resp_ies_o::value_c::e2node_load_meas_id() const
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const crit_diagnostics_s& e2_res_status_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void e2_res_status_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE e2_res_status_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_res_status_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2_res_status_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "INTEGER (1..4095,...)", "INTEGER (1..4095,...)", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "e2_res_status_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2_res_status_resp_ies_o>;

OCUDUASN_CODE e2_res_status_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, e2node_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_res_status_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2_res_status_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 105: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(e2node_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void e2_res_status_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 105);
  j.write_str("criticality", "reject");
  j.write_int("value", e2node_load_meas_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// E2ResourceStatusUpdateIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2_res_status_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 117, 105, 136, 137};
  return map_enum_number(names, 5, idx, "id");
}
bool e2_res_status_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 117, 105, 136, 137};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2_res_status_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 117:
      return crit_e::reject;
    case 105:
      return crit_e::reject;
    case 136:
      return crit_e::ignore;
    case 137:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2_res_status_upd_ies_o::value_c e2_res_status_upd_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 117:
      ret.set(value_c::types::ric_load_meas_id);
      break;
    case 105:
      ret.set(value_c::types::e2node_load_meas_id);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2_res_status_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 117:
      return presence_e::mandatory;
    case 105:
      return presence_e::mandatory;
    case 136:
      return presence_e::optional;
    case 137:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2_res_status_upd_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::ric_load_meas_id:
      c = uint16_t{};
      break;
    case types::e2node_load_meas_id:
      c = uint16_t{};
      break;
    case types::tnl_capacity_ind:
      break;
    case types::hardware_load_ind:
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_upd_ies_o::value_c");
  }
}
uint16_t& e2_res_status_upd_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& e2_res_status_upd_ies_o::value_c::ric_load_meas_id()
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& e2_res_status_upd_ies_o::value_c::e2node_load_meas_id()
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_upd_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_upd_ies_o::value_c::ric_load_meas_id() const
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& e2_res_status_upd_ies_o::value_c::e2node_load_meas_id() const
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
void e2_res_status_upd_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::tnl_capacity_ind:
      j.write_null();
      break;
    case types::hardware_load_ind:
      j.write_null();
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_upd_ies_o::value_c");
  }
}
OCUDUASN_CODE e2_res_status_upd_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::tnl_capacity_ind:
      break;
    case types::hardware_load_ind:
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_upd_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_res_status_upd_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::tnl_capacity_ind:
      break;
    case types::hardware_load_ind:
      break;
    default:
      log_invalid_choice_id(type_, "e2_res_status_upd_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2_res_status_upd_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "INTEGER (1..4095,...)", "INTEGER (1..4095,...)", "NULL", "NULL"};
  return convert_enum_idx(names, 5, value, "e2_res_status_upd_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2_res_status_upd_ies_o>;

OCUDUASN_CODE e2_res_status_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += tnl_capacity_ind_present ? 1 : 0;
  nof_ies += hardware_load_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, e2node_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  if (tnl_capacity_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)136, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
  }
  if (hardware_load_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)137, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2_res_status_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2_res_status_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 105: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(e2node_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 136: {
        tnl_capacity_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        break;
      }
      case 137: {
        hardware_load_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
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
void e2_res_status_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 105);
  j.write_str("criticality", "reject");
  j.write_int("value", e2node_load_meas_id);
  j.end_obj();
  if (tnl_capacity_ind_present) {
    j.start_obj();
    j.write_int("id", 136);
    j.write_str("criticality", "ignore");
    j.write_null("value");
    j.end_obj();
  }
  if (hardware_load_ind_present) {
    j.start_obj();
    j.write_int("id", 137);
    j.write_str("criticality", "ignore");
    j.write_null("value");
    j.end_obj();
  }
  j.end_array();
}

// E2connectionSetupFailed-Item ::= SEQUENCE
OCUDUASN_CODE e2conn_setup_failed_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(tnl_info.pack(bref));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_setup_failed_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(tnl_info.unpack(bref));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2conn_setup_failed_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tnlInformation");
  tnl_info.to_json(j);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// E2connectionSetupFailed-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2conn_setup_failed_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {41};
  return map_enum_number(names, 1, idx, "id");
}
bool e2conn_setup_failed_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 41 == id;
}
crit_e e2conn_setup_failed_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 41) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2conn_setup_failed_item_ies_o::value_c e2conn_setup_failed_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 41) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2conn_setup_failed_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 41) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2conn_setup_failed_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2conn_setup_failed_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_setup_failed_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2conn_setup_failed_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2connectionSetupFailed-Item"};
  return convert_enum_idx(names, 1, value, "e2conn_setup_failed_item_ies_o::value_c::types");
}
uint8_t e2conn_setup_failed_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2conn_setup_failed_item_ies_o::value_c::types");
}

// E2connectionUpdate-Item ::= SEQUENCE
OCUDUASN_CODE e2conn_upd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(tnl_info.pack(bref));
  HANDLE_CODE(tnl_usage.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(tnl_info.unpack(bref));
  HANDLE_CODE(tnl_usage.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2conn_upd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tnlInformation");
  tnl_info.to_json(j);
  j.write_str("tnlUsage", tnl_usage.to_string());
  j.end_obj();
}

// E2connectionUpdate-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2conn_upd_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {43};
  return map_enum_number(names, 1, idx, "id");
}
bool e2conn_upd_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 43 == id;
}
crit_e e2conn_upd_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 43) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2conn_upd_item_ies_o::value_c e2conn_upd_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 43) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2conn_upd_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 43) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2conn_upd_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2conn_upd_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2conn_upd_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2connectionUpdate-Item"};
  return convert_enum_idx(names, 1, value, "e2conn_upd_item_ies_o::value_c::types");
}
uint8_t e2conn_upd_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2conn_upd_item_ies_o::value_c::types");
}

// E2connectionUpdateRemove-Item ::= SEQUENCE
OCUDUASN_CODE e2conn_upd_rem_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(tnl_info.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_rem_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(tnl_info.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2conn_upd_rem_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tnlInformation");
  tnl_info.to_json(j);
  j.end_obj();
}

// E2connectionUpdateRemove-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2conn_upd_rem_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {47};
  return map_enum_number(names, 1, idx, "id");
}
bool e2conn_upd_rem_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 47 == id;
}
crit_e e2conn_upd_rem_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 47) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2conn_upd_rem_item_ies_o::value_c e2conn_upd_rem_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 47) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2conn_upd_rem_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 47) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2conn_upd_rem_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2conn_upd_rem_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_rem_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2conn_upd_rem_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2connectionUpdateRemove-Item"};
  return convert_enum_idx(names, 1, value, "e2conn_upd_rem_item_ies_o::value_c::types");
}
uint8_t e2conn_upd_rem_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2conn_upd_rem_item_ies_o::value_c::types");
}

// E2connectionUpdate-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2conn_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 44, 46, 45};
  return map_enum_number(names, 4, idx, "id");
}
bool e2conn_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 44, 46, 45};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2conn_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 44:
      return crit_e::reject;
    case 46:
      return crit_e::reject;
    case 45:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2conn_upd_ies_o::value_c e2conn_upd_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 44:
      ret.set(value_c::types::e2conn_upd_add);
      break;
    case 46:
      ret.set(value_c::types::e2conn_upd_rem);
      break;
    case 45:
      ret.set(value_c::types::e2conn_upd_modify);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2conn_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 44:
      return presence_e::optional;
    case 46:
      return presence_e::optional;
    case 45:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2conn_upd_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::e2conn_upd_add:
      c = e2conn_upd_list_l{};
      break;
    case types::e2conn_upd_rem:
      c = e2conn_upd_rem_list_l{};
      break;
    case types::e2conn_upd_modify:
      c = e2conn_upd_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_ies_o::value_c");
  }
}
uint16_t& e2conn_upd_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
e2conn_upd_list_l& e2conn_upd_ies_o::value_c::e2conn_upd_add()
{
  assert_choice_type(types::e2conn_upd_add, type_, "Value");
  return c.get<e2conn_upd_list_l>();
}
e2conn_upd_rem_list_l& e2conn_upd_ies_o::value_c::e2conn_upd_rem()
{
  assert_choice_type(types::e2conn_upd_rem, type_, "Value");
  return c.get<e2conn_upd_rem_list_l>();
}
e2conn_upd_list_l& e2conn_upd_ies_o::value_c::e2conn_upd_modify()
{
  assert_choice_type(types::e2conn_upd_modify, type_, "Value");
  return c.get<e2conn_upd_list_l>();
}
const uint16_t& e2conn_upd_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const e2conn_upd_list_l& e2conn_upd_ies_o::value_c::e2conn_upd_add() const
{
  assert_choice_type(types::e2conn_upd_add, type_, "Value");
  return c.get<e2conn_upd_list_l>();
}
const e2conn_upd_rem_list_l& e2conn_upd_ies_o::value_c::e2conn_upd_rem() const
{
  assert_choice_type(types::e2conn_upd_rem, type_, "Value");
  return c.get<e2conn_upd_rem_list_l>();
}
const e2conn_upd_list_l& e2conn_upd_ies_o::value_c::e2conn_upd_modify() const
{
  assert_choice_type(types::e2conn_upd_modify, type_, "Value");
  return c.get<e2conn_upd_list_l>();
}
void e2conn_upd_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2conn_upd_add:
      j.start_array("");
      for (const auto& e1 : c.get<e2conn_upd_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2conn_upd_rem:
      j.start_array("");
      for (const auto& e1 : c.get<e2conn_upd_rem_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2conn_upd_modify:
      j.start_array("");
      for (const auto& e1 : c.get<e2conn_upd_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_ies_o::value_c");
  }
}
OCUDUASN_CODE e2conn_upd_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::e2conn_upd_add:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2conn_upd_list_l>(), 1, 32, true));
      break;
    case types::e2conn_upd_rem:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2conn_upd_rem_list_l>(), 1, 32, true));
      break;
    case types::e2conn_upd_modify:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2conn_upd_list_l>(), 1, 32, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::e2conn_upd_add:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2conn_upd_list_l>(), bref, 1, 32, true));
      break;
    case types::e2conn_upd_rem:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2conn_upd_rem_list_l>(), bref, 1, 32, true));
      break;
    case types::e2conn_upd_modify:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2conn_upd_list_l>(), bref, 1, 32, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2conn_upd_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "E2connectionUpdate-List", "E2connectionUpdateRemove-List", "E2connectionUpdate-List"};
  return convert_enum_idx(names, 4, value, "e2conn_upd_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2conn_upd_ies_o>;

OCUDUASN_CODE e2conn_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += e2conn_upd_add_present ? 1 : 0;
  nof_ies += e2conn_upd_rem_present ? 1 : 0;
  nof_ies += e2conn_upd_modify_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (e2conn_upd_add_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)44, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2conn_upd_add, 1, 32, true));
  }
  if (e2conn_upd_rem_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)46, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2conn_upd_rem, 1, 32, true));
  }
  if (e2conn_upd_modify_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)45, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2conn_upd_modify, 1, 32, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2conn_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 44: {
        e2conn_upd_add_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2conn_upd_add, bref, 1, 32, true));
        break;
      }
      case 46: {
        e2conn_upd_rem_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2conn_upd_rem, bref, 1, 32, true));
        break;
      }
      case 45: {
        e2conn_upd_modify_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2conn_upd_modify, bref, 1, 32, true));
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
void e2conn_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (e2conn_upd_add_present) {
    j.start_obj();
    j.write_int("id", 44);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2conn_upd_add) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e2conn_upd_rem_present) {
    j.start_obj();
    j.write_int("id", 46);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2conn_upd_rem) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e2conn_upd_modify_present) {
    j.start_obj();
    j.write_int("id", 45);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2conn_upd_modify) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// E2connectionUpdateAck-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2conn_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 39, 40};
  return map_enum_number(names, 3, idx, "id");
}
bool e2conn_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 39, 40};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2conn_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 39:
      return crit_e::reject;
    case 40:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2conn_upd_ack_ies_o::value_c e2conn_upd_ack_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 39:
      ret.set(value_c::types::e2conn_setup);
      break;
    case 40:
      ret.set(value_c::types::e2conn_setup_failed);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2conn_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 39:
      return presence_e::optional;
    case 40:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2conn_upd_ack_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::e2conn_setup:
      c = e2conn_upd_list_l{};
      break;
    case types::e2conn_setup_failed:
      c = e2conn_setup_failed_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_ack_ies_o::value_c");
  }
}
uint16_t& e2conn_upd_ack_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
e2conn_upd_list_l& e2conn_upd_ack_ies_o::value_c::e2conn_setup()
{
  assert_choice_type(types::e2conn_setup, type_, "Value");
  return c.get<e2conn_upd_list_l>();
}
e2conn_setup_failed_list_l& e2conn_upd_ack_ies_o::value_c::e2conn_setup_failed()
{
  assert_choice_type(types::e2conn_setup_failed, type_, "Value");
  return c.get<e2conn_setup_failed_list_l>();
}
const uint16_t& e2conn_upd_ack_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const e2conn_upd_list_l& e2conn_upd_ack_ies_o::value_c::e2conn_setup() const
{
  assert_choice_type(types::e2conn_setup, type_, "Value");
  return c.get<e2conn_upd_list_l>();
}
const e2conn_setup_failed_list_l& e2conn_upd_ack_ies_o::value_c::e2conn_setup_failed() const
{
  assert_choice_type(types::e2conn_setup_failed, type_, "Value");
  return c.get<e2conn_setup_failed_list_l>();
}
void e2conn_upd_ack_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2conn_setup:
      j.start_array("");
      for (const auto& e1 : c.get<e2conn_upd_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2conn_setup_failed:
      j.start_array("");
      for (const auto& e1 : c.get<e2conn_setup_failed_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_ack_ies_o::value_c");
  }
}
OCUDUASN_CODE e2conn_upd_ack_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::e2conn_setup:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2conn_upd_list_l>(), 1, 32, true));
      break;
    case types::e2conn_setup_failed:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2conn_setup_failed_list_l>(), 1, 32, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_ack_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_ack_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::e2conn_setup:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2conn_upd_list_l>(), bref, 1, 32, true));
      break;
    case types::e2conn_setup_failed:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2conn_setup_failed_list_l>(), bref, 1, 32, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_ack_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2conn_upd_ack_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "E2connectionUpdate-List", "E2connectionSetupFailed-List"};
  return convert_enum_idx(names, 3, value, "e2conn_upd_ack_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2conn_upd_ack_ies_o>;

OCUDUASN_CODE e2conn_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += e2conn_setup_present ? 1 : 0;
  nof_ies += e2conn_setup_failed_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (e2conn_setup_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)39, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2conn_setup, 1, 32, true));
  }
  if (e2conn_setup_failed_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2conn_setup_failed, 1, 32, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2conn_upd_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 39: {
        e2conn_setup_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2conn_setup, bref, 1, 32, true));
        break;
      }
      case 40: {
        e2conn_setup_failed_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2conn_setup_failed, bref, 1, 32, true));
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
void e2conn_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (e2conn_setup_present) {
    j.start_obj();
    j.write_int("id", 39);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2conn_setup) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e2conn_setup_failed_present) {
    j.start_obj();
    j.write_int("id", 40);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2conn_setup_failed) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// E2connectionUpdateFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2conn_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 1, 31, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool e2conn_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 1, 31, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2conn_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 31:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2conn_upd_fail_ies_o::value_c e2conn_upd_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 31:
      ret.set(value_c::types::time_to_wait);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2conn_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    case 31:
      return presence_e::optional;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2conn_upd_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::time_to_wait:
      c = time_to_wait_e{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_fail_ies_o::value_c");
  }
}
uint16_t& e2conn_upd_fail_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& e2conn_upd_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
time_to_wait_e& e2conn_upd_fail_ies_o::value_c::time_to_wait()
{
  assert_choice_type(types::time_to_wait, type_, "Value");
  return c.get<time_to_wait_e>();
}
crit_diagnostics_s& e2conn_upd_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& e2conn_upd_fail_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& e2conn_upd_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const time_to_wait_e& e2conn_upd_fail_ies_o::value_c::time_to_wait() const
{
  assert_choice_type(types::time_to_wait, type_, "Value");
  return c.get<time_to_wait_e>();
}
const crit_diagnostics_s& e2conn_upd_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void e2conn_upd_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::time_to_wait:
      j.write_str(c.get<time_to_wait_e>().to_string());
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE e2conn_upd_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::time_to_wait:
      HANDLE_CODE(c.get<time_to_wait_e>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::time_to_wait:
      HANDLE_CODE(c.get<time_to_wait_e>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2conn_upd_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2conn_upd_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "Cause", "TimeToWait", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "e2conn_upd_fail_ies_o::value_c::types");
}
uint8_t e2conn_upd_fail_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "e2conn_upd_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2conn_upd_fail_ies_o>;

OCUDUASN_CODE e2conn_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (cause_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)31, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2conn_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2conn_upd_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 1: {
        cause_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 31: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void e2conn_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (cause_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    cause.to_json(j);
    j.end_obj();
  }
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// E2nodeComponentConfigAddition-Item ::= SEQUENCE
OCUDUASN_CODE e2node_component_cfg_addition_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(e2node_component_interface_type.pack(bref));
  HANDLE_CODE(e2node_component_id.pack(bref));
  HANDLE_CODE(e2node_component_cfg.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_addition_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(e2node_component_interface_type.unpack(bref));
  HANDLE_CODE(e2node_component_id.unpack(bref));
  HANDLE_CODE(e2node_component_cfg.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_cfg_addition_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("e2nodeComponentInterfaceType", e2node_component_interface_type.to_string());
  j.write_fieldname("e2nodeComponentID");
  e2node_component_id.to_json(j);
  j.write_fieldname("e2nodeComponentConfiguration");
  e2node_component_cfg.to_json(j);
  j.end_obj();
}

// E2nodeComponentConfigAddition-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_component_cfg_addition_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {51};
  return map_enum_number(names, 1, idx, "id");
}
bool e2node_component_cfg_addition_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 51 == id;
}
crit_e e2node_component_cfg_addition_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 51) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2node_component_cfg_addition_item_ies_o::value_c
e2node_component_cfg_addition_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 51) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_component_cfg_addition_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 51) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2node_component_cfg_addition_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2node_component_cfg_addition_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_addition_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2node_component_cfg_addition_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2nodeComponentConfigAddition-Item"};
  return convert_enum_idx(names, 1, value, "e2node_component_cfg_addition_item_ies_o::value_c::types");
}
uint8_t e2node_component_cfg_addition_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2node_component_cfg_addition_item_ies_o::value_c::types");
}

// E2nodeComponentConfigAdditionAck-Item ::= SEQUENCE
OCUDUASN_CODE e2node_component_cfg_addition_ack_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(e2node_component_interface_type.pack(bref));
  HANDLE_CODE(e2node_component_id.pack(bref));
  HANDLE_CODE(e2node_component_cfg_ack.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_addition_ack_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(e2node_component_interface_type.unpack(bref));
  HANDLE_CODE(e2node_component_id.unpack(bref));
  HANDLE_CODE(e2node_component_cfg_ack.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_cfg_addition_ack_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("e2nodeComponentInterfaceType", e2node_component_interface_type.to_string());
  j.write_fieldname("e2nodeComponentID");
  e2node_component_id.to_json(j);
  j.write_fieldname("e2nodeComponentConfigurationAck");
  e2node_component_cfg_ack.to_json(j);
  j.end_obj();
}

// E2nodeComponentConfigAdditionAck-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_component_cfg_addition_ack_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {53};
  return map_enum_number(names, 1, idx, "id");
}
bool e2node_component_cfg_addition_ack_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 53 == id;
}
crit_e e2node_component_cfg_addition_ack_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 53) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2node_component_cfg_addition_ack_item_ies_o::value_c
e2node_component_cfg_addition_ack_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 53) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_component_cfg_addition_ack_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 53) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2node_component_cfg_addition_ack_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2node_component_cfg_addition_ack_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_addition_ack_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2node_component_cfg_addition_ack_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2nodeComponentConfigAdditionAck-Item"};
  return convert_enum_idx(names, 1, value, "e2node_component_cfg_addition_ack_item_ies_o::value_c::types");
}
uint8_t e2node_component_cfg_addition_ack_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2node_component_cfg_addition_ack_item_ies_o::value_c::types");
}

// E2nodeComponentConfigRemoval-Item ::= SEQUENCE
OCUDUASN_CODE e2node_component_cfg_removal_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(e2node_component_interface_type.pack(bref));
  HANDLE_CODE(e2node_component_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_removal_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(e2node_component_interface_type.unpack(bref));
  HANDLE_CODE(e2node_component_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_cfg_removal_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("e2nodeComponentInterfaceType", e2node_component_interface_type.to_string());
  j.write_fieldname("e2nodeComponentID");
  e2node_component_id.to_json(j);
  j.end_obj();
}

// E2nodeComponentConfigRemoval-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_component_cfg_removal_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {55};
  return map_enum_number(names, 1, idx, "id");
}
bool e2node_component_cfg_removal_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 55 == id;
}
crit_e e2node_component_cfg_removal_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 55) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2node_component_cfg_removal_item_ies_o::value_c e2node_component_cfg_removal_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 55) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_component_cfg_removal_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 55) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2node_component_cfg_removal_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2node_component_cfg_removal_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_removal_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2node_component_cfg_removal_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2nodeComponentConfigRemoval-Item"};
  return convert_enum_idx(names, 1, value, "e2node_component_cfg_removal_item_ies_o::value_c::types");
}
uint8_t e2node_component_cfg_removal_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2node_component_cfg_removal_item_ies_o::value_c::types");
}

// E2nodeComponentConfigRemovalAck-Item ::= SEQUENCE
OCUDUASN_CODE e2node_component_cfg_removal_ack_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(e2node_component_interface_type.pack(bref));
  HANDLE_CODE(e2node_component_id.pack(bref));
  HANDLE_CODE(e2node_component_cfg_ack.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_removal_ack_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(e2node_component_interface_type.unpack(bref));
  HANDLE_CODE(e2node_component_id.unpack(bref));
  HANDLE_CODE(e2node_component_cfg_ack.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_cfg_removal_ack_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("e2nodeComponentInterfaceType", e2node_component_interface_type.to_string());
  j.write_fieldname("e2nodeComponentID");
  e2node_component_id.to_json(j);
  j.write_fieldname("e2nodeComponentConfigurationAck");
  e2node_component_cfg_ack.to_json(j);
  j.end_obj();
}

// E2nodeComponentConfigRemovalAck-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_component_cfg_removal_ack_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {57};
  return map_enum_number(names, 1, idx, "id");
}
bool e2node_component_cfg_removal_ack_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 57 == id;
}
crit_e e2node_component_cfg_removal_ack_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 57) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2node_component_cfg_removal_ack_item_ies_o::value_c
e2node_component_cfg_removal_ack_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 57) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_component_cfg_removal_ack_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 57) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2node_component_cfg_removal_ack_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2node_component_cfg_removal_ack_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_removal_ack_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2node_component_cfg_removal_ack_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2nodeComponentConfigRemovalAck-Item"};
  return convert_enum_idx(names, 1, value, "e2node_component_cfg_removal_ack_item_ies_o::value_c::types");
}
uint8_t e2node_component_cfg_removal_ack_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2node_component_cfg_removal_ack_item_ies_o::value_c::types");
}

// E2nodeComponentConfigUpdate-Item ::= SEQUENCE
OCUDUASN_CODE e2node_component_cfg_upd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(e2node_component_interface_type.pack(bref));
  HANDLE_CODE(e2node_component_id.pack(bref));
  HANDLE_CODE(e2node_component_cfg.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_upd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(e2node_component_interface_type.unpack(bref));
  HANDLE_CODE(e2node_component_id.unpack(bref));
  HANDLE_CODE(e2node_component_cfg.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_cfg_upd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("e2nodeComponentInterfaceType", e2node_component_interface_type.to_string());
  j.write_fieldname("e2nodeComponentID");
  e2node_component_id.to_json(j);
  j.write_fieldname("e2nodeComponentConfiguration");
  e2node_component_cfg.to_json(j);
  j.end_obj();
}

// E2nodeComponentConfigUpdate-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_component_cfg_upd_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {34};
  return map_enum_number(names, 1, idx, "id");
}
bool e2node_component_cfg_upd_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 34 == id;
}
crit_e e2node_component_cfg_upd_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 34) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2node_component_cfg_upd_item_ies_o::value_c e2node_component_cfg_upd_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 34) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_component_cfg_upd_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 34) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2node_component_cfg_upd_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2node_component_cfg_upd_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_upd_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2node_component_cfg_upd_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2nodeComponentConfigUpdate-Item"};
  return convert_enum_idx(names, 1, value, "e2node_component_cfg_upd_item_ies_o::value_c::types");
}
uint8_t e2node_component_cfg_upd_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2node_component_cfg_upd_item_ies_o::value_c::types");
}

// E2nodeComponentConfigUpdateAck-Item ::= SEQUENCE
OCUDUASN_CODE e2node_component_cfg_upd_ack_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(e2node_component_interface_type.pack(bref));
  HANDLE_CODE(e2node_component_id.pack(bref));
  HANDLE_CODE(e2node_component_cfg_ack.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_upd_ack_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(e2node_component_interface_type.unpack(bref));
  HANDLE_CODE(e2node_component_id.unpack(bref));
  HANDLE_CODE(e2node_component_cfg_ack.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_component_cfg_upd_ack_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("e2nodeComponentInterfaceType", e2node_component_interface_type.to_string());
  j.write_fieldname("e2nodeComponentID");
  e2node_component_id.to_json(j);
  j.write_fieldname("e2nodeComponentConfigurationAck");
  e2node_component_cfg_ack.to_json(j);
  j.end_obj();
}

// E2nodeComponentConfigUpdateAck-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_component_cfg_upd_ack_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {36};
  return map_enum_number(names, 1, idx, "id");
}
bool e2node_component_cfg_upd_ack_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 36 == id;
}
crit_e e2node_component_cfg_upd_ack_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 36) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2node_component_cfg_upd_ack_item_ies_o::value_c e2node_component_cfg_upd_ack_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 36) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_component_cfg_upd_ack_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 36) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2node_component_cfg_upd_ack_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2node_component_cfg_upd_ack_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_component_cfg_upd_ack_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2node_component_cfg_upd_ack_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2nodeComponentConfigUpdateAck-Item"};
  return convert_enum_idx(names, 1, value, "e2node_component_cfg_upd_ack_item_ies_o::value_c::types");
}
uint8_t e2node_component_cfg_upd_ack_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2node_component_cfg_upd_ack_item_ies_o::value_c::types");
}

// E2nodeTNLassociationRemoval-Item ::= SEQUENCE
OCUDUASN_CODE e2node_tnl_assoc_removal_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(tnl_info.pack(bref));
  HANDLE_CODE(tnl_info_ric.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_tnl_assoc_removal_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(tnl_info.unpack(bref));
  HANDLE_CODE(tnl_info_ric.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void e2node_tnl_assoc_removal_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("tnlInformation");
  tnl_info.to_json(j);
  j.write_fieldname("tnlInformationRIC");
  tnl_info_ric.to_json(j);
  j.end_obj();
}

// E2nodeTNLassociationRemoval-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_tnl_assoc_removal_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {59};
  return map_enum_number(names, 1, idx, "id");
}
bool e2node_tnl_assoc_removal_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 59 == id;
}
crit_e e2node_tnl_assoc_removal_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 59) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
e2node_tnl_assoc_removal_item_ies_o::value_c e2node_tnl_assoc_removal_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 59) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_tnl_assoc_removal_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 59) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void e2node_tnl_assoc_removal_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE e2node_tnl_assoc_removal_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_tnl_assoc_removal_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* e2node_tnl_assoc_removal_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"E2nodeTNLassociationRemoval-Item"};
  return convert_enum_idx(names, 1, value, "e2node_tnl_assoc_removal_item_ies_o::value_c::types");
}
uint8_t e2node_tnl_assoc_removal_item_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {2};
  return map_enum_number(numbers, 1, value, "e2node_tnl_assoc_removal_item_ies_o::value_c::types");
}

// E2nodeConfigurationUpdate-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_cfg_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 3, 50, 33, 54, 58};
  return map_enum_number(names, 6, idx, "id");
}
bool e2node_cfg_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 3, 50, 33, 54, 58};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2node_cfg_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 50:
      return crit_e::reject;
    case 33:
      return crit_e::reject;
    case 54:
      return crit_e::reject;
    case 58:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2node_cfg_upd_ies_o::value_c e2node_cfg_upd_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 3:
      ret.set(value_c::types::global_e2node_id);
      break;
    case 50:
      ret.set(value_c::types::e2node_component_cfg_addition);
      break;
    case 33:
      ret.set(value_c::types::e2node_component_cfg_upd);
      break;
    case 54:
      ret.set(value_c::types::e2node_component_cfg_removal);
      break;
    case 58:
      ret.set(value_c::types::e2node_tnl_assoc_removal);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_cfg_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 3:
      return presence_e::optional;
    case 50:
      return presence_e::optional;
    case 33:
      return presence_e::optional;
    case 54:
      return presence_e::optional;
    case 58:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2node_cfg_upd_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::global_e2node_id:
      c = global_e2node_id_c{};
      break;
    case types::e2node_component_cfg_addition:
      c = e2node_component_cfg_addition_list_l{};
      break;
    case types::e2node_component_cfg_upd:
      c = e2node_component_cfg_upd_list_l{};
      break;
    case types::e2node_component_cfg_removal:
      c = e2node_component_cfg_removal_list_l{};
      break;
    case types::e2node_tnl_assoc_removal:
      c = e2node_tnl_assoc_removal_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_ies_o::value_c");
  }
}
uint16_t& e2node_cfg_upd_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
global_e2node_id_c& e2node_cfg_upd_ies_o::value_c::global_e2node_id()
{
  assert_choice_type(types::global_e2node_id, type_, "Value");
  return c.get<global_e2node_id_c>();
}
e2node_component_cfg_addition_list_l& e2node_cfg_upd_ies_o::value_c::e2node_component_cfg_addition()
{
  assert_choice_type(types::e2node_component_cfg_addition, type_, "Value");
  return c.get<e2node_component_cfg_addition_list_l>();
}
e2node_component_cfg_upd_list_l& e2node_cfg_upd_ies_o::value_c::e2node_component_cfg_upd()
{
  assert_choice_type(types::e2node_component_cfg_upd, type_, "Value");
  return c.get<e2node_component_cfg_upd_list_l>();
}
e2node_component_cfg_removal_list_l& e2node_cfg_upd_ies_o::value_c::e2node_component_cfg_removal()
{
  assert_choice_type(types::e2node_component_cfg_removal, type_, "Value");
  return c.get<e2node_component_cfg_removal_list_l>();
}
e2node_tnl_assoc_removal_list_l& e2node_cfg_upd_ies_o::value_c::e2node_tnl_assoc_removal()
{
  assert_choice_type(types::e2node_tnl_assoc_removal, type_, "Value");
  return c.get<e2node_tnl_assoc_removal_list_l>();
}
const uint16_t& e2node_cfg_upd_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const global_e2node_id_c& e2node_cfg_upd_ies_o::value_c::global_e2node_id() const
{
  assert_choice_type(types::global_e2node_id, type_, "Value");
  return c.get<global_e2node_id_c>();
}
const e2node_component_cfg_addition_list_l& e2node_cfg_upd_ies_o::value_c::e2node_component_cfg_addition() const
{
  assert_choice_type(types::e2node_component_cfg_addition, type_, "Value");
  return c.get<e2node_component_cfg_addition_list_l>();
}
const e2node_component_cfg_upd_list_l& e2node_cfg_upd_ies_o::value_c::e2node_component_cfg_upd() const
{
  assert_choice_type(types::e2node_component_cfg_upd, type_, "Value");
  return c.get<e2node_component_cfg_upd_list_l>();
}
const e2node_component_cfg_removal_list_l& e2node_cfg_upd_ies_o::value_c::e2node_component_cfg_removal() const
{
  assert_choice_type(types::e2node_component_cfg_removal, type_, "Value");
  return c.get<e2node_component_cfg_removal_list_l>();
}
const e2node_tnl_assoc_removal_list_l& e2node_cfg_upd_ies_o::value_c::e2node_tnl_assoc_removal() const
{
  assert_choice_type(types::e2node_tnl_assoc_removal, type_, "Value");
  return c.get<e2node_tnl_assoc_removal_list_l>();
}
void e2node_cfg_upd_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::global_e2node_id:
      c.get<global_e2node_id_c>().to_json(j);
      break;
    case types::e2node_component_cfg_addition:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_component_cfg_addition_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2node_component_cfg_upd:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_component_cfg_upd_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2node_component_cfg_removal:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_component_cfg_removal_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2node_tnl_assoc_removal:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_tnl_assoc_removal_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_ies_o::value_c");
  }
}
OCUDUASN_CODE e2node_cfg_upd_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::global_e2node_id:
      HANDLE_CODE(c.get<global_e2node_id_c>().pack(bref));
      break;
    case types::e2node_component_cfg_addition:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_component_cfg_addition_list_l>(), 1, 1024, true));
      break;
    case types::e2node_component_cfg_upd:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_component_cfg_upd_list_l>(), 1, 1024, true));
      break;
    case types::e2node_component_cfg_removal:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_component_cfg_removal_list_l>(), 1, 1024, true));
      break;
    case types::e2node_tnl_assoc_removal:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_tnl_assoc_removal_list_l>(), 1, 32, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_cfg_upd_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::global_e2node_id:
      HANDLE_CODE(c.get<global_e2node_id_c>().unpack(bref));
      break;
    case types::e2node_component_cfg_addition:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_component_cfg_addition_list_l>(), bref, 1, 1024, true));
      break;
    case types::e2node_component_cfg_upd:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_component_cfg_upd_list_l>(), bref, 1, 1024, true));
      break;
    case types::e2node_component_cfg_removal:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_component_cfg_removal_list_l>(), bref, 1, 1024, true));
      break;
    case types::e2node_tnl_assoc_removal:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_tnl_assoc_removal_list_l>(), bref, 1, 32, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2node_cfg_upd_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)",
                                "GlobalE2node-ID",
                                "E2nodeComponentConfigAddition-List",
                                "E2nodeComponentConfigUpdate-List",
                                "E2nodeComponentConfigRemoval-List",
                                "E2nodeTNLassociationRemoval-List"};
  return convert_enum_idx(names, 6, value, "e2node_cfg_upd_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2node_cfg_upd_ies_o>;

OCUDUASN_CODE e2node_cfg_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += global_e2node_id_present ? 1 : 0;
  nof_ies += e2node_component_cfg_addition_present ? 1 : 0;
  nof_ies += e2node_component_cfg_upd_present ? 1 : 0;
  nof_ies += e2node_component_cfg_removal_present ? 1 : 0;
  nof_ies += e2node_tnl_assoc_removal_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (global_e2node_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)3, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(global_e2node_id.pack(bref));
  }
  if (e2node_component_cfg_addition_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)50, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_component_cfg_addition, 1, 1024, true));
  }
  if (e2node_component_cfg_upd_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)33, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_component_cfg_upd, 1, 1024, true));
  }
  if (e2node_component_cfg_removal_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)54, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_component_cfg_removal, 1, 1024, true));
  }
  if (e2node_tnl_assoc_removal_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)58, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_tnl_assoc_removal, 1, 32, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_cfg_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2node_cfg_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 3: {
        global_e2node_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(global_e2node_id.unpack(bref));
        break;
      }
      case 50: {
        e2node_component_cfg_addition_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_component_cfg_addition, bref, 1, 1024, true));
        break;
      }
      case 33: {
        e2node_component_cfg_upd_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_component_cfg_upd, bref, 1, 1024, true));
        break;
      }
      case 54: {
        e2node_component_cfg_removal_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_component_cfg_removal, bref, 1, 1024, true));
        break;
      }
      case 58: {
        e2node_tnl_assoc_removal_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_tnl_assoc_removal, bref, 1, 32, true));
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
void e2node_cfg_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (global_e2node_id_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    global_e2node_id.to_json(j);
    j.end_obj();
  }
  if (e2node_component_cfg_addition_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2node_component_cfg_addition) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e2node_component_cfg_upd_present) {
    j.start_obj();
    j.write_int("id", 33);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2node_component_cfg_upd) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e2node_component_cfg_removal_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2node_component_cfg_removal) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e2node_tnl_assoc_removal_present) {
    j.start_obj();
    j.write_int("id", 58);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2node_tnl_assoc_removal) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// E2nodeConfigurationUpdateAcknowledge-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_cfg_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 52, 35, 56};
  return map_enum_number(names, 4, idx, "id");
}
bool e2node_cfg_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 52, 35, 56};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2node_cfg_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 52:
      return crit_e::reject;
    case 35:
      return crit_e::reject;
    case 56:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2node_cfg_upd_ack_ies_o::value_c e2node_cfg_upd_ack_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 52:
      ret.set(value_c::types::e2node_component_cfg_addition_ack);
      break;
    case 35:
      ret.set(value_c::types::e2node_component_cfg_upd_ack);
      break;
    case 56:
      ret.set(value_c::types::e2node_component_cfg_removal_ack);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_cfg_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 52:
      return presence_e::optional;
    case 35:
      return presence_e::optional;
    case 56:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2node_cfg_upd_ack_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::e2node_component_cfg_addition_ack:
      c = e2node_component_cfg_addition_ack_list_l{};
      break;
    case types::e2node_component_cfg_upd_ack:
      c = e2node_component_cfg_upd_ack_list_l{};
      break;
    case types::e2node_component_cfg_removal_ack:
      c = e2node_component_cfg_removal_ack_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_ack_ies_o::value_c");
  }
}
uint16_t& e2node_cfg_upd_ack_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
e2node_component_cfg_addition_ack_list_l& e2node_cfg_upd_ack_ies_o::value_c::e2node_component_cfg_addition_ack()
{
  assert_choice_type(types::e2node_component_cfg_addition_ack, type_, "Value");
  return c.get<e2node_component_cfg_addition_ack_list_l>();
}
e2node_component_cfg_upd_ack_list_l& e2node_cfg_upd_ack_ies_o::value_c::e2node_component_cfg_upd_ack()
{
  assert_choice_type(types::e2node_component_cfg_upd_ack, type_, "Value");
  return c.get<e2node_component_cfg_upd_ack_list_l>();
}
e2node_component_cfg_removal_ack_list_l& e2node_cfg_upd_ack_ies_o::value_c::e2node_component_cfg_removal_ack()
{
  assert_choice_type(types::e2node_component_cfg_removal_ack, type_, "Value");
  return c.get<e2node_component_cfg_removal_ack_list_l>();
}
const uint16_t& e2node_cfg_upd_ack_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const e2node_component_cfg_addition_ack_list_l&
e2node_cfg_upd_ack_ies_o::value_c::e2node_component_cfg_addition_ack() const
{
  assert_choice_type(types::e2node_component_cfg_addition_ack, type_, "Value");
  return c.get<e2node_component_cfg_addition_ack_list_l>();
}
const e2node_component_cfg_upd_ack_list_l& e2node_cfg_upd_ack_ies_o::value_c::e2node_component_cfg_upd_ack() const
{
  assert_choice_type(types::e2node_component_cfg_upd_ack, type_, "Value");
  return c.get<e2node_component_cfg_upd_ack_list_l>();
}
const e2node_component_cfg_removal_ack_list_l&
e2node_cfg_upd_ack_ies_o::value_c::e2node_component_cfg_removal_ack() const
{
  assert_choice_type(types::e2node_component_cfg_removal_ack, type_, "Value");
  return c.get<e2node_component_cfg_removal_ack_list_l>();
}
void e2node_cfg_upd_ack_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_component_cfg_addition_ack:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_component_cfg_addition_ack_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2node_component_cfg_upd_ack:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_component_cfg_upd_ack_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2node_component_cfg_removal_ack:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_component_cfg_removal_ack_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_ack_ies_o::value_c");
  }
}
OCUDUASN_CODE e2node_cfg_upd_ack_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::e2node_component_cfg_addition_ack:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_component_cfg_addition_ack_list_l>(), 1, 1024, true));
      break;
    case types::e2node_component_cfg_upd_ack:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_component_cfg_upd_ack_list_l>(), 1, 1024, true));
      break;
    case types::e2node_component_cfg_removal_ack:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_component_cfg_removal_ack_list_l>(), 1, 1024, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_ack_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_cfg_upd_ack_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::e2node_component_cfg_addition_ack:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_component_cfg_addition_ack_list_l>(), bref, 1, 1024, true));
      break;
    case types::e2node_component_cfg_upd_ack:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_component_cfg_upd_ack_list_l>(), bref, 1, 1024, true));
      break;
    case types::e2node_component_cfg_removal_ack:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_component_cfg_removal_ack_list_l>(), bref, 1, 1024, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_ack_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2node_cfg_upd_ack_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)",
                                "E2nodeComponentConfigAdditionAck-List",
                                "E2nodeComponentConfigUpdateAck-List",
                                "E2nodeComponentConfigRemovalAck-List"};
  return convert_enum_idx(names, 4, value, "e2node_cfg_upd_ack_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2node_cfg_upd_ack_ies_o>;

OCUDUASN_CODE e2node_cfg_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += e2node_component_cfg_addition_ack_present ? 1 : 0;
  nof_ies += e2node_component_cfg_upd_ack_present ? 1 : 0;
  nof_ies += e2node_component_cfg_removal_ack_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (e2node_component_cfg_addition_ack_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)52, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_component_cfg_addition_ack, 1, 1024, true));
  }
  if (e2node_component_cfg_upd_ack_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)35, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_component_cfg_upd_ack, 1, 1024, true));
  }
  if (e2node_component_cfg_removal_ack_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)56, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_component_cfg_removal_ack, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_cfg_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2node_cfg_upd_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 52: {
        e2node_component_cfg_addition_ack_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_component_cfg_addition_ack, bref, 1, 1024, true));
        break;
      }
      case 35: {
        e2node_component_cfg_upd_ack_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_component_cfg_upd_ack, bref, 1, 1024, true));
        break;
      }
      case 56: {
        e2node_component_cfg_removal_ack_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_component_cfg_removal_ack, bref, 1, 1024, true));
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
void e2node_cfg_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (e2node_component_cfg_addition_ack_present) {
    j.start_obj();
    j.write_int("id", 52);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2node_component_cfg_addition_ack) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e2node_component_cfg_upd_ack_present) {
    j.start_obj();
    j.write_int("id", 35);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2node_component_cfg_upd_ack) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e2node_component_cfg_removal_ack_present) {
    j.start_obj();
    j.write_int("id", 56);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : e2node_component_cfg_removal_ack) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// E2nodeConfigurationUpdateFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2node_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 1, 31, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool e2node_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 1, 31, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2node_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 31:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2node_cfg_upd_fail_ies_o::value_c e2node_cfg_upd_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 31:
      ret.set(value_c::types::time_to_wait);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2node_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 31:
      return presence_e::optional;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2node_cfg_upd_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::time_to_wait:
      c = time_to_wait_e{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_fail_ies_o::value_c");
  }
}
uint16_t& e2node_cfg_upd_fail_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& e2node_cfg_upd_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
time_to_wait_e& e2node_cfg_upd_fail_ies_o::value_c::time_to_wait()
{
  assert_choice_type(types::time_to_wait, type_, "Value");
  return c.get<time_to_wait_e>();
}
crit_diagnostics_s& e2node_cfg_upd_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& e2node_cfg_upd_fail_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& e2node_cfg_upd_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const time_to_wait_e& e2node_cfg_upd_fail_ies_o::value_c::time_to_wait() const
{
  assert_choice_type(types::time_to_wait, type_, "Value");
  return c.get<time_to_wait_e>();
}
const crit_diagnostics_s& e2node_cfg_upd_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void e2node_cfg_upd_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::time_to_wait:
      j.write_str(c.get<time_to_wait_e>().to_string());
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE e2node_cfg_upd_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::time_to_wait:
      HANDLE_CODE(c.get<time_to_wait_e>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_cfg_upd_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::time_to_wait:
      HANDLE_CODE(c.get<time_to_wait_e>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2node_cfg_upd_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2node_cfg_upd_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "Cause", "TimeToWait", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "e2node_cfg_upd_fail_ies_o::value_c::types");
}
uint8_t e2node_cfg_upd_fail_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "e2node_cfg_upd_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2node_cfg_upd_fail_ies_o>;

OCUDUASN_CODE e2node_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)31, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2node_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2node_cfg_upd_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 31: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void e2node_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// E2setupFailureIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 1, 31, 2, 48};
  return map_enum_number(names, 5, idx, "id");
}
bool e2setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 1, 31, 2, 48};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 31:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    case 48:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2setup_fail_ies_o::value_c e2setup_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 31:
      ret.set(value_c::types::time_to_wait);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    case 48:
      ret.set(value_c::types::tnl_info);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 31:
      return presence_e::optional;
    case 2:
      return presence_e::optional;
    case 48:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2setup_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::time_to_wait:
      c = time_to_wait_e{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::tnl_info:
      c = tnl_info_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_fail_ies_o::value_c");
  }
}
uint16_t& e2setup_fail_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& e2setup_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
time_to_wait_e& e2setup_fail_ies_o::value_c::time_to_wait()
{
  assert_choice_type(types::time_to_wait, type_, "Value");
  return c.get<time_to_wait_e>();
}
crit_diagnostics_s& e2setup_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
tnl_info_s& e2setup_fail_ies_o::value_c::tnl_info()
{
  assert_choice_type(types::tnl_info, type_, "Value");
  return c.get<tnl_info_s>();
}
const uint16_t& e2setup_fail_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& e2setup_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const time_to_wait_e& e2setup_fail_ies_o::value_c::time_to_wait() const
{
  assert_choice_type(types::time_to_wait, type_, "Value");
  return c.get<time_to_wait_e>();
}
const crit_diagnostics_s& e2setup_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const tnl_info_s& e2setup_fail_ies_o::value_c::tnl_info() const
{
  assert_choice_type(types::tnl_info, type_, "Value");
  return c.get<tnl_info_s>();
}
void e2setup_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::time_to_wait:
      j.write_str(c.get<time_to_wait_e>().to_string());
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    case types::tnl_info:
      c.get<tnl_info_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE e2setup_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::time_to_wait:
      HANDLE_CODE(c.get<time_to_wait_e>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    case types::tnl_info:
      HANDLE_CODE(c.get<tnl_info_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2setup_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::time_to_wait:
      HANDLE_CODE(c.get<time_to_wait_e>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    case types::tnl_info:
      HANDLE_CODE(c.get<tnl_info_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2setup_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "Cause", "TimeToWait", "CriticalityDiagnostics", "TNLinformation"};
  return convert_enum_idx(names, 5, value, "e2setup_fail_ies_o::value_c::types");
}
uint8_t e2setup_fail_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "e2setup_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2setup_fail_ies_o>;

OCUDUASN_CODE e2setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += tnl_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)31, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (tnl_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)48, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(tnl_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2setup_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 31: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 48: {
        tnl_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(tnl_info.unpack(bref));
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
void e2setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (tnl_info_present) {
    j.start_obj();
    j.write_int("id", 48);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    tnl_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RANfunction-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(ran_function_definition.pack(bref));
  HANDLE_CODE(pack_integer(bref, ran_function_revision, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(ran_function_o_id.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(ran_function_definition.unpack(bref));
  HANDLE_CODE(unpack_integer(ran_function_revision, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(ran_function_o_id.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  j.write_str("ranFunctionDefinition", ran_function_definition.to_string());
  j.write_int("ranFunctionRevision", ran_function_revision);
  j.write_str("ranFunctionOID", ran_function_o_id.to_string());
  j.end_obj();
}

// RANfunction-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {8};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 8 == id;
}
crit_e ran_function_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 8) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_item_ies_o::value_c ran_function_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 8) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 8) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunction-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_item_ies_o::value_c::types");
}

// E2setupRequestIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 3, 10, 50};
  return map_enum_number(names, 4, idx, "id");
}
bool e2setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 3, 10, 50};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 10:
      return crit_e::reject;
    case 50:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2setup_request_ies_o::value_c e2setup_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 3:
      ret.set(value_c::types::global_e2node_id);
      break;
    case 10:
      ret.set(value_c::types::ran_functions_added);
      break;
    case 50:
      ret.set(value_c::types::e2node_component_cfg_addition);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 10:
      return presence_e::mandatory;
    case 50:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2setup_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::global_e2node_id:
      c = global_e2node_id_c{};
      break;
    case types::ran_functions_added:
      c = ran_functions_list_l{};
      break;
    case types::e2node_component_cfg_addition:
      c = e2node_component_cfg_addition_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_request_ies_o::value_c");
  }
}
uint16_t& e2setup_request_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
global_e2node_id_c& e2setup_request_ies_o::value_c::global_e2node_id()
{
  assert_choice_type(types::global_e2node_id, type_, "Value");
  return c.get<global_e2node_id_c>();
}
ran_functions_list_l& e2setup_request_ies_o::value_c::ran_functions_added()
{
  assert_choice_type(types::ran_functions_added, type_, "Value");
  return c.get<ran_functions_list_l>();
}
e2node_component_cfg_addition_list_l& e2setup_request_ies_o::value_c::e2node_component_cfg_addition()
{
  assert_choice_type(types::e2node_component_cfg_addition, type_, "Value");
  return c.get<e2node_component_cfg_addition_list_l>();
}
const uint16_t& e2setup_request_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const global_e2node_id_c& e2setup_request_ies_o::value_c::global_e2node_id() const
{
  assert_choice_type(types::global_e2node_id, type_, "Value");
  return c.get<global_e2node_id_c>();
}
const ran_functions_list_l& e2setup_request_ies_o::value_c::ran_functions_added() const
{
  assert_choice_type(types::ran_functions_added, type_, "Value");
  return c.get<ran_functions_list_l>();
}
const e2node_component_cfg_addition_list_l& e2setup_request_ies_o::value_c::e2node_component_cfg_addition() const
{
  assert_choice_type(types::e2node_component_cfg_addition, type_, "Value");
  return c.get<e2node_component_cfg_addition_list_l>();
}
void e2setup_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::global_e2node_id:
      c.get<global_e2node_id_c>().to_json(j);
      break;
    case types::ran_functions_added:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2node_component_cfg_addition:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_component_cfg_addition_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_request_ies_o::value_c");
  }
}
OCUDUASN_CODE e2setup_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::global_e2node_id:
      HANDLE_CODE(c.get<global_e2node_id_c>().pack(bref));
      break;
    case types::ran_functions_added:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_list_l>(), 1, 256, true));
      break;
    case types::e2node_component_cfg_addition:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_component_cfg_addition_list_l>(), 1, 1024, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2setup_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::global_e2node_id:
      HANDLE_CODE(c.get<global_e2node_id_c>().unpack(bref));
      break;
    case types::ran_functions_added:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_list_l>(), bref, 1, 256, true));
      break;
    case types::e2node_component_cfg_addition:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_component_cfg_addition_list_l>(), bref, 1, 1024, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2setup_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "GlobalE2node-ID", "RANfunctions-List", "E2nodeComponentConfigAddition-List"};
  return convert_enum_idx(names, 4, value, "e2setup_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e2setup_request_ies_o>;

OCUDUASN_CODE e2setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)3, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(global_e2node_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)10, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_added, 1, 256, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)50, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_component_cfg_addition, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2setup_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(global_e2node_id.unpack(bref));
        break;
      }
      case 10: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_added, bref, 1, 256, true));
        break;
      }
      case 50: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_component_cfg_addition, bref, 1, 1024, true));
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
void e2setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_e2node_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ran_functions_added) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 50);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : e2node_component_cfg_addition) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// RANfunctionID-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_id_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(pack_integer(bref, ran_function_revision, (uint16_t)0u, (uint16_t)4095u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_id_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(unpack_integer(ran_function_revision, bref, (uint16_t)0u, (uint16_t)4095u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_id_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  j.write_int("ranFunctionRevision", ran_function_revision);
  j.end_obj();
}

// RANfunctionID-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_id_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {6};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_id_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 6 == id;
}
crit_e ran_function_id_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 6) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_id_item_ies_o::value_c ran_function_id_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 6) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_id_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 6) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_id_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_id_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_id_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_id_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunctionID-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_id_item_ies_o::value_c::types");
}

// RANfunctionIDcause-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_id_cause_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_id_cause_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_id_cause_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RANfunctionIDcause-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_id_cause_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {7};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_id_cause_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 7 == id;
}
crit_e ran_function_id_cause_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 7) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_id_cause_item_ies_o::value_c ran_function_id_cause_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 7) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_id_cause_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 7) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_id_cause_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_id_cause_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_id_cause_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_id_cause_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunctionIDcause-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_id_cause_item_ies_o::value_c::types");
}

// RANfunction-AcceptModified-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_accept_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(rev_ran_function_definition.pack(bref));
  HANDLE_CODE(pack_integer(bref, ran_function_revision, (uint16_t)0u, (uint16_t)4095u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_accept_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(rev_ran_function_definition.unpack(bref));
  HANDLE_CODE(unpack_integer(ran_function_revision, bref, (uint16_t)0u, (uint16_t)4095u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_accept_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  j.write_str("revRanFunctionDefinition", rev_ran_function_definition.to_string());
  j.write_int("ranFunctionRevision", ran_function_revision);
  j.end_obj();
}

// RANfunction-AcceptModified-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_accept_modified_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {139};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_accept_modified_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 139 == id;
}
crit_e ran_function_accept_modified_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 139) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_accept_modified_item_ies_o::value_c ran_function_accept_modified_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 139) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_accept_modified_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 139) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_accept_modified_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_accept_modified_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_accept_modified_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_accept_modified_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunction-AcceptModified-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_accept_modified_item_ies_o::value_c::types");
}

// E2setupResponseIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t e2setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 4, 9, 13, 52, 138};
  return map_enum_number(names, 6, idx, "id");
}
bool e2setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 4, 9, 13, 52, 138};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e2setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 4:
      return crit_e::reject;
    case 9:
      return crit_e::reject;
    case 13:
      return crit_e::reject;
    case 52:
      return crit_e::reject;
    case 138:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e2setup_resp_ies_o::value_c e2setup_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 4:
      ret.set(value_c::types::global_ric_id);
      break;
    case 9:
      ret.set(value_c::types::ran_functions_accepted);
      break;
    case 13:
      ret.set(value_c::types::ran_functions_rejected);
      break;
    case 52:
      ret.set(value_c::types::e2node_component_cfg_addition_ack);
      break;
    case 138:
      ret.set(value_c::types::ran_functions_accept_modified_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e2setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 4:
      return presence_e::mandatory;
    case 9:
      return presence_e::optional;
    case 13:
      return presence_e::optional;
    case 52:
      return presence_e::mandatory;
    case 138:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e2setup_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::global_ric_id:
      c = global_ric_id_s{};
      break;
    case types::ran_functions_accepted:
      c = ran_functions_id_list_l{};
      break;
    case types::ran_functions_rejected:
      c = ran_functions_id_cause_list_l{};
      break;
    case types::e2node_component_cfg_addition_ack:
      c = e2node_component_cfg_addition_ack_list_l{};
      break;
    case types::ran_functions_accept_modified_list:
      c = ran_functions_accept_modified_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_resp_ies_o::value_c");
  }
}
uint16_t& e2setup_resp_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
global_ric_id_s& e2setup_resp_ies_o::value_c::global_ric_id()
{
  assert_choice_type(types::global_ric_id, type_, "Value");
  return c.get<global_ric_id_s>();
}
ran_functions_id_list_l& e2setup_resp_ies_o::value_c::ran_functions_accepted()
{
  assert_choice_type(types::ran_functions_accepted, type_, "Value");
  return c.get<ran_functions_id_list_l>();
}
ran_functions_id_cause_list_l& e2setup_resp_ies_o::value_c::ran_functions_rejected()
{
  assert_choice_type(types::ran_functions_rejected, type_, "Value");
  return c.get<ran_functions_id_cause_list_l>();
}
e2node_component_cfg_addition_ack_list_l& e2setup_resp_ies_o::value_c::e2node_component_cfg_addition_ack()
{
  assert_choice_type(types::e2node_component_cfg_addition_ack, type_, "Value");
  return c.get<e2node_component_cfg_addition_ack_list_l>();
}
ran_functions_accept_modified_list_l& e2setup_resp_ies_o::value_c::ran_functions_accept_modified_list()
{
  assert_choice_type(types::ran_functions_accept_modified_list, type_, "Value");
  return c.get<ran_functions_accept_modified_list_l>();
}
const uint16_t& e2setup_resp_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const global_ric_id_s& e2setup_resp_ies_o::value_c::global_ric_id() const
{
  assert_choice_type(types::global_ric_id, type_, "Value");
  return c.get<global_ric_id_s>();
}
const ran_functions_id_list_l& e2setup_resp_ies_o::value_c::ran_functions_accepted() const
{
  assert_choice_type(types::ran_functions_accepted, type_, "Value");
  return c.get<ran_functions_id_list_l>();
}
const ran_functions_id_cause_list_l& e2setup_resp_ies_o::value_c::ran_functions_rejected() const
{
  assert_choice_type(types::ran_functions_rejected, type_, "Value");
  return c.get<ran_functions_id_cause_list_l>();
}
const e2node_component_cfg_addition_ack_list_l& e2setup_resp_ies_o::value_c::e2node_component_cfg_addition_ack() const
{
  assert_choice_type(types::e2node_component_cfg_addition_ack, type_, "Value");
  return c.get<e2node_component_cfg_addition_ack_list_l>();
}
const ran_functions_accept_modified_list_l& e2setup_resp_ies_o::value_c::ran_functions_accept_modified_list() const
{
  assert_choice_type(types::ran_functions_accept_modified_list, type_, "Value");
  return c.get<ran_functions_accept_modified_list_l>();
}
void e2setup_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::global_ric_id:
      c.get<global_ric_id_s>().to_json(j);
      break;
    case types::ran_functions_accepted:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_id_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ran_functions_rejected:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_id_cause_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::e2node_component_cfg_addition_ack:
      j.start_array("");
      for (const auto& e1 : c.get<e2node_component_cfg_addition_ack_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ran_functions_accept_modified_list:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_accept_modified_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE e2setup_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::global_ric_id:
      HANDLE_CODE(c.get<global_ric_id_s>().pack(bref));
      break;
    case types::ran_functions_accepted:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_id_list_l>(), 1, 256, true));
      break;
    case types::ran_functions_rejected:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_id_cause_list_l>(), 1, 256, true));
      break;
    case types::e2node_component_cfg_addition_ack:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<e2node_component_cfg_addition_ack_list_l>(), 1, 1024, true));
      break;
    case types::ran_functions_accept_modified_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_accept_modified_list_l>(), 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2setup_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::global_ric_id:
      HANDLE_CODE(c.get<global_ric_id_s>().unpack(bref));
      break;
    case types::ran_functions_accepted:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_id_list_l>(), bref, 1, 256, true));
      break;
    case types::ran_functions_rejected:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_id_cause_list_l>(), bref, 1, 256, true));
      break;
    case types::e2node_component_cfg_addition_ack:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<e2node_component_cfg_addition_ack_list_l>(), bref, 1, 1024, true));
      break;
    case types::ran_functions_accept_modified_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_accept_modified_list_l>(), bref, 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "e2setup_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e2setup_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)",
                                "GlobalRIC-ID",
                                "RANfunctionsID-List",
                                "RANfunctionsIDcause-List",
                                "E2nodeComponentConfigAdditionAck-List",
                                "RANfunctions-AcceptModified-List"};
  return convert_enum_idx(names, 6, value, "e2setup_resp_ies_o::value_c::types");
}
uint8_t e2setup_resp_ies_o::value_c::types_opts::to_number() const
{
  switch (value) {
    case transaction_id:
      return 0;
    case e2node_component_cfg_addition_ack:
      return 2;
    default:
      invalid_enum_number(value, "e2setup_resp_ies_o::value_c::types");
  }
  return 0;
}

template struct asn1::protocol_ie_field_s<e2setup_resp_ies_o>;

OCUDUASN_CODE e2setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ran_functions_accepted_present ? 1 : 0;
  nof_ies += ran_functions_rejected_present ? 1 : 0;
  nof_ies += ran_functions_accept_modified_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)4, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(global_ric_id.pack(bref));
  }
  if (ran_functions_accepted_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)9, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_accepted, 1, 256, true));
  }
  if (ran_functions_rejected_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)13, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_rejected, 1, 256, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)52, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e2node_component_cfg_addition_ack, 1, 1024, true));
  }
  if (ran_functions_accept_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)138, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_accept_modified_list, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e2setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e2setup_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 4: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(global_ric_id.unpack(bref));
        break;
      }
      case 9: {
        ran_functions_accepted_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_accepted, bref, 1, 256, true));
        break;
      }
      case 13: {
        ran_functions_rejected_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_rejected, bref, 1, 256, true));
        break;
      }
      case 52: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e2node_component_cfg_addition_ack, bref, 1, 1024, true));
        break;
      }
      case 138: {
        ran_functions_accept_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_accept_modified_list, bref, 1, 256, true));
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
void e2setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 4);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_ric_id.to_json(j);
  j.end_obj();
  if (ran_functions_accepted_present) {
    j.start_obj();
    j.write_int("id", 9);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_accepted) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ran_functions_rejected_present) {
    j.start_obj();
    j.write_int("id", 13);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_rejected) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 52);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : e2node_component_cfg_addition_ack) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (ran_functions_accept_modified_list_present) {
    j.start_obj();
    j.write_int("id", 138);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_accept_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// ErrorIndication-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t error_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 29, 5, 1, 2};
  return map_enum_number(names, 5, idx, "id");
}
bool error_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 29, 5, 1, 2};
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
    case 49:
      return crit_e::reject;
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
error_ind_ies_o::value_c error_ind_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e error_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::optional;
    case 29:
      return presence_e::optional;
    case 5:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void error_ind_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "error_ind_ies_o::value_c");
  }
}
uint16_t& error_ind_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
ric_request_id_s& error_ind_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& error_ind_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& error_ind_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& error_ind_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& error_ind_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const ric_request_id_s& error_ind_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& error_ind_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& error_ind_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& error_ind_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void error_ind_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "error_ind_ies_o::value_c");
  }
}
OCUDUASN_CODE error_ind_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "error_ind_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE error_ind_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "error_ind_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* error_ind_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "RICrequestID", "INTEGER (0..4095)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 5, value, "error_ind_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<error_ind_ies_o>;

OCUDUASN_CODE error_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += transaction_id_present ? 1 : 0;
  nof_ies += ric_request_id_present ? 1 : 0;
  nof_ies += ran_function_id_present ? 1 : 0;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (transaction_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ric_request_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  if (ran_function_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  if (cause_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE error_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != error_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        transaction_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 29: {
        ric_request_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        ran_function_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 1: {
        cause_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
  if (transaction_id_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "reject");
    j.write_int("value", transaction_id);
    j.end_obj();
  }
  if (ric_request_id_present) {
    j.start_obj();
    j.write_int("id", 29);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ric_request_id.to_json(j);
    j.end_obj();
  }
  if (ran_function_id_present) {
    j.start_obj();
    j.write_int("id", 5);
    j.write_str("criticality", "reject");
    j.write_int("value", ran_function_id);
    j.end_obj();
  }
  if (cause_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cause.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICactionLoad-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_load_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_load_info.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_load_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_load_info.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_load_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_fieldname("ricActionLoadInformation");
  ric_action_load_info.to_json(j);
  j.end_obj();
}

// RICactionLoad-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_load_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {114};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_load_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 114 == id;
}
crit_e ric_action_load_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 114) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_load_item_ies_o::value_c ric_action_load_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 114) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_load_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 114) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_load_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_load_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_load_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_load_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICactionLoad-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_load_item_ies_o::value_c::types");
}

// RICsubscriptionLoad-ItemIE ::= SEQUENCE
OCUDUASN_CODE ric_sub_load_item_ie_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_sub_load_info_present, 1));
  HANDLE_CODE(bref.pack(ric_action_load_list.size() > 0, 1));

  HANDLE_CODE(ric_request_id.pack(bref));
  if (ric_sub_load_info_present) {
    HANDLE_CODE(ric_sub_load_info.pack(bref));
  }
  if (ric_action_load_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_action_load_list, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_load_item_ie_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ric_sub_load_info_present, 1));
  bool ric_action_load_list_present;
  HANDLE_CODE(bref.unpack(ric_action_load_list_present, 1));

  HANDLE_CODE(ric_request_id.unpack(bref));
  if (ric_sub_load_info_present) {
    HANDLE_CODE(ric_sub_load_info.unpack(bref));
  }
  if (ric_action_load_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_action_load_list, bref, 1, 16, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_load_item_ie_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ricRequestID");
  ric_request_id.to_json(j);
  if (ric_sub_load_info_present) {
    j.write_fieldname("ricSubscriptionLoadInformation");
    ric_sub_load_info.to_json(j);
  }
  if (ric_action_load_list.size() > 0) {
    j.start_array("ricActionLoad-list");
    for (const auto& e1 : ric_action_load_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RICsubscriptionLoad-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_load_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {118};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_sub_load_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 118 == id;
}
crit_e ric_sub_load_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 118) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_sub_load_item_ies_o::value_c ric_sub_load_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 118) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_load_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 118) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_load_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_sub_load_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_load_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_load_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICsubscriptionLoad-ItemIE"};
  return convert_enum_idx(names, 1, value, "ric_sub_load_item_ies_o::value_c::types");
}

// RANfunctionLoad-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_load_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ran_function_load_info_present, 1));
  HANDLE_CODE(bref.pack(ric_service_load_info_present, 1));
  HANDLE_CODE(bref.pack(ric_sub_load_list.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ran_function_load_info_present) {
    HANDLE_CODE(ran_function_load_info.pack(bref));
  }
  if (ric_service_load_info_present) {
    HANDLE_CODE(ric_service_load_info.pack(bref));
  }
  if (ric_sub_load_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_load_list, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_load_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ran_function_load_info_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_load_info_present, 1));
  bool ric_sub_load_list_present;
  HANDLE_CODE(bref.unpack(ric_sub_load_list_present, 1));

  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ran_function_load_info_present) {
    HANDLE_CODE(ran_function_load_info.unpack(bref));
  }
  if (ric_service_load_info_present) {
    HANDLE_CODE(ric_service_load_info.unpack(bref));
  }
  if (ric_sub_load_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_sub_load_list, bref, 1, 1024, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_load_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  if (ran_function_load_info_present) {
    j.write_fieldname("ranFunctionLoadInformation");
    ran_function_load_info.to_json(j);
  }
  if (ric_service_load_info_present) {
    j.write_fieldname("ricServiceLoadInformation");
    ric_service_load_info.to_json(j);
  }
  if (ric_sub_load_list.size() > 0) {
    j.start_array("ricSubscriptionLoad-list");
    for (const auto& e1 : ric_sub_load_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RANfunctionLoad-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_load_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {106};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_load_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 106 == id;
}
crit_e ran_function_load_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 106) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_load_item_ies_o::value_c ran_function_load_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 106) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_load_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 106) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_load_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_load_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_load_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_load_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunctionLoad-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_load_item_ies_o::value_c::types");
}

// RICactionLoadConfirm-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_load_confirm_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_load_confirm.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_load_confirm_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_load_confirm.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_load_confirm_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_str("ricActionLoadConfirm", "true");
  j.end_obj();
}

// RICactionLoadConfirm-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_load_confirm_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {115};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_load_confirm_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 115 == id;
}
crit_e ric_action_load_confirm_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 115) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_load_confirm_item_ies_o::value_c ric_action_load_confirm_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 115) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_load_confirm_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 115) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_load_confirm_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_load_confirm_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_load_confirm_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_load_confirm_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICactionLoadConfirm-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_load_confirm_item_ies_o::value_c::types");
}

// RICsubscriptionLoadConfirm-ItemIE ::= SEQUENCE
OCUDUASN_CODE ric_sub_load_confirm_item_ie_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_sub_load_confirm_present, 1));
  HANDLE_CODE(bref.pack(ric_action_load_confirm_list.size() > 0, 1));

  HANDLE_CODE(ric_request_id.pack(bref));
  if (ric_sub_load_confirm_present) {
    HANDLE_CODE(ric_sub_load_confirm.pack(bref));
  }
  if (ric_action_load_confirm_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_action_load_confirm_list, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_load_confirm_item_ie_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ric_sub_load_confirm_present, 1));
  bool ric_action_load_confirm_list_present;
  HANDLE_CODE(bref.unpack(ric_action_load_confirm_list_present, 1));

  HANDLE_CODE(ric_request_id.unpack(bref));
  if (ric_sub_load_confirm_present) {
    HANDLE_CODE(ric_sub_load_confirm.unpack(bref));
  }
  if (ric_action_load_confirm_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_action_load_confirm_list, bref, 1, 16, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_load_confirm_item_ie_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ricRequestID");
  ric_request_id.to_json(j);
  if (ric_sub_load_confirm_present) {
    j.write_str("ricSubscriptionLoadConfirm", "true");
  }
  if (ric_action_load_confirm_list.size() > 0) {
    j.start_array("ricActionLoadConfirm-list");
    for (const auto& e1 : ric_action_load_confirm_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RICsubscriptionLoadConfirm-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_load_confirm_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {119};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_sub_load_confirm_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 119 == id;
}
crit_e ric_sub_load_confirm_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 119) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_sub_load_confirm_item_ies_o::value_c ric_sub_load_confirm_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 119) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_load_confirm_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 119) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_load_confirm_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_sub_load_confirm_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_load_confirm_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_load_confirm_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICsubscriptionLoadConfirm-ItemIE"};
  return convert_enum_idx(names, 1, value, "ric_sub_load_confirm_item_ies_o::value_c::types");
}

// RANfunctionLoadConfirm-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_load_confirm_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ran_function_load_confirm_present, 1));
  HANDLE_CODE(bref.pack(ric_service_load_confirm_present, 1));
  HANDLE_CODE(bref.pack(ric_sub_load_confirm_list.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ran_function_load_confirm_present) {
    HANDLE_CODE(ran_function_load_confirm.pack(bref));
  }
  if (ric_service_load_confirm_present) {
    HANDLE_CODE(ric_service_load_confirm.pack(bref));
  }
  if (ric_sub_load_confirm_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_load_confirm_list, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_load_confirm_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ran_function_load_confirm_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_load_confirm_present, 1));
  bool ric_sub_load_confirm_list_present;
  HANDLE_CODE(bref.unpack(ric_sub_load_confirm_list_present, 1));

  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ran_function_load_confirm_present) {
    HANDLE_CODE(ran_function_load_confirm.unpack(bref));
  }
  if (ric_service_load_confirm_present) {
    HANDLE_CODE(ric_service_load_confirm.unpack(bref));
  }
  if (ric_sub_load_confirm_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_sub_load_confirm_list, bref, 1, 1024, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_load_confirm_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  if (ran_function_load_confirm_present) {
    j.write_str("ranFunctionLoadConfirm", "true");
  }
  if (ric_service_load_confirm_present) {
    j.write_fieldname("ricServiceLoadConfirm");
    ric_service_load_confirm.to_json(j);
  }
  if (ric_sub_load_confirm_list.size() > 0) {
    j.start_array("ricSubscriptionLoadConfirm-list");
    for (const auto& e1 : ric_sub_load_confirm_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RANfunctionLoadConfirm-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_load_confirm_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {108};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_load_confirm_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 108 == id;
}
crit_e ran_function_load_confirm_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 108) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_load_confirm_item_ies_o::value_c ran_function_load_confirm_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 108) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_load_confirm_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 108) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_load_confirm_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_load_confirm_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_load_confirm_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_load_confirm_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunctionLoadConfirm-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_load_confirm_item_ies_o::value_c::types");
}

// RICactionLoadRequest-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_load_request_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_load_request.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_load_request_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_load_request.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_load_request_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_str("ricActionLoadRequest", "true");
  j.end_obj();
}

// RICactionLoadRequest-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_load_request_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {116};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_load_request_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 116 == id;
}
crit_e ric_action_load_request_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 116) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_load_request_item_ies_o::value_c ric_action_load_request_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 116) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_load_request_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 116) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_load_request_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_load_request_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_load_request_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_load_request_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICactionLoadRequest-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_load_request_item_ies_o::value_c::types");
}

// RICsubscriptionLoadRequest-ItemIE ::= SEQUENCE
OCUDUASN_CODE ric_sub_load_request_item_ie_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_sub_load_request_present, 1));
  HANDLE_CODE(bref.pack(ric_action_load_request_list.size() > 0, 1));

  HANDLE_CODE(ric_request_id.pack(bref));
  if (ric_sub_load_request_present) {
    HANDLE_CODE(ric_sub_load_request.pack(bref));
  }
  if (ric_action_load_request_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_action_load_request_list, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_load_request_item_ie_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ric_sub_load_request_present, 1));
  bool ric_action_load_request_list_present;
  HANDLE_CODE(bref.unpack(ric_action_load_request_list_present, 1));

  HANDLE_CODE(ric_request_id.unpack(bref));
  if (ric_sub_load_request_present) {
    HANDLE_CODE(ric_sub_load_request.unpack(bref));
  }
  if (ric_action_load_request_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_action_load_request_list, bref, 1, 16, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_load_request_item_ie_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ricRequestID");
  ric_request_id.to_json(j);
  if (ric_sub_load_request_present) {
    j.write_str("ricSubscriptionLoadRequest", "true");
  }
  if (ric_action_load_request_list.size() > 0) {
    j.start_array("ricActionLoadRequest-list");
    for (const auto& e1 : ric_action_load_request_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RICsubscriptionLoadRequest-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_load_request_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {120};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_sub_load_request_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 120 == id;
}
crit_e ric_sub_load_request_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 120) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_sub_load_request_item_ies_o::value_c ric_sub_load_request_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 120) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_load_request_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 120) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_load_request_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_sub_load_request_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_load_request_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_load_request_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICsubscriptionLoadRequest-ItemIE"};
  return convert_enum_idx(names, 1, value, "ric_sub_load_request_item_ies_o::value_c::types");
}

// RANfunctionLoadRequest-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_load_request_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ran_function_load_request_present, 1));
  HANDLE_CODE(bref.pack(ric_service_load_request_present, 1));
  HANDLE_CODE(bref.pack(ric_sub_load_request_list.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ran_function_load_request_present) {
    HANDLE_CODE(ran_function_load_request.pack(bref));
  }
  if (ric_service_load_request_present) {
    HANDLE_CODE(ric_service_load_request.pack(bref));
  }
  if (ric_sub_load_request_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_load_request_list, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_load_request_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ran_function_load_request_present, 1));
  HANDLE_CODE(bref.unpack(ric_service_load_request_present, 1));
  bool ric_sub_load_request_list_present;
  HANDLE_CODE(bref.unpack(ric_sub_load_request_list_present, 1));

  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ran_function_load_request_present) {
    HANDLE_CODE(ran_function_load_request.unpack(bref));
  }
  if (ric_service_load_request_present) {
    HANDLE_CODE(ric_service_load_request.unpack(bref));
  }
  if (ric_sub_load_request_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_sub_load_request_list, bref, 1, 1024, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_load_request_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  if (ran_function_load_request_present) {
    j.write_str("ranFunctionLoadRequest", "true");
  }
  if (ric_service_load_request_present) {
    j.write_fieldname("ricServiceLoadRequest");
    ric_service_load_request.to_json(j);
  }
  if (ric_sub_load_request_list.size() > 0) {
    j.start_array("ricSubscriptionLoadRequest-list");
    for (const auto& e1 : ric_sub_load_request_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RANfunctionLoadRequest-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_load_request_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {110};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_load_request_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 110 == id;
}
crit_e ran_function_load_request_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 110) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_load_request_item_ies_o::value_c ran_function_load_request_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 110) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_load_request_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 110) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_load_request_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_load_request_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_load_request_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_load_request_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunctionLoadRequest-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_load_request_item_ies_o::value_c::types");
}

// RICactionList-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.end_obj();
}

// RICaction-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {125};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 125 == id;
}
crit_e ric_action_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 125) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_item_ies_o::value_c ric_action_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 125) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 125) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICactionList-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_item_ies_o::value_c::types");
}

// RICsubscriptionList-Item ::= SEQUENCE
OCUDUASN_CODE ric_sub_list_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_action_list.size() > 0, 1));

  HANDLE_CODE(ric_request_id.pack(bref));
  if (ric_action_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_action_list, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_list_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ric_action_list_present;
  HANDLE_CODE(bref.unpack(ric_action_list_present, 1));

  HANDLE_CODE(ric_request_id.unpack(bref));
  if (ric_action_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_action_list, bref, 1, 16, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_list_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ricRequestID");
  ric_request_id.to_json(j);
  if (ric_action_list.size() > 0) {
    j.start_array("ricAction-list");
    for (const auto& e1 : ric_action_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RICsubscriptionList-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_list_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {126};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_sub_list_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 126 == id;
}
crit_e ric_sub_list_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 126) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_sub_list_item_ies_o::value_c ric_sub_list_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 126) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_list_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 126) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_list_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_sub_list_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_list_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_list_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICsubscriptionList-Item"};
  return convert_enum_idx(names, 1, value, "ric_sub_list_item_ies_o::value_c::types");
}

// RANfunctionStateConfirm-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_state_confirm_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_sub_suspended_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ric_sub_resumed_list.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ric_sub_suspended_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_suspended_list, 1, 1024, true));
  }
  if (ric_sub_resumed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_resumed_list, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_state_confirm_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ric_sub_suspended_list_present;
  HANDLE_CODE(bref.unpack(ric_sub_suspended_list_present, 1));
  bool ric_sub_resumed_list_present;
  HANDLE_CODE(bref.unpack(ric_sub_resumed_list_present, 1));

  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ric_sub_suspended_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_sub_suspended_list, bref, 1, 1024, true));
  }
  if (ric_sub_resumed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_sub_resumed_list, bref, 1, 1024, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_state_confirm_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  if (ric_sub_suspended_list.size() > 0) {
    j.start_array("ricSubscriptionSuspended-list");
    for (const auto& e1 : ric_sub_suspended_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ric_sub_resumed_list.size() > 0) {
    j.start_array("ricSubscriptionResumed-list");
    for (const auto& e1 : ric_sub_resumed_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RANfunctionStateConfirm-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_state_confirm_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {121};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_state_confirm_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 121 == id;
}
crit_e ran_function_state_confirm_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 121) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_state_confirm_item_ies_o::value_c ran_function_state_confirm_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 121) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_state_confirm_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 121) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_state_confirm_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_state_confirm_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_state_confirm_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_state_confirm_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunctionStateConfirm-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_state_confirm_item_ies_o::value_c::types");
}

// RANfunctionStateControl-Item ::= SEQUENCE
OCUDUASN_CODE ran_function_state_ctrl_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_sub_to_be_suspended_list.size() > 0, 1));
  HANDLE_CODE(bref.pack(ric_sub_to_be_resumed_list.size() > 0, 1));

  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ric_sub_to_be_suspended_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_to_be_suspended_list, 1, 1024, true));
  }
  if (ric_sub_to_be_resumed_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_to_be_resumed_list, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_state_ctrl_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ric_sub_to_be_suspended_list_present;
  HANDLE_CODE(bref.unpack(ric_sub_to_be_suspended_list_present, 1));
  bool ric_sub_to_be_resumed_list_present;
  HANDLE_CODE(bref.unpack(ric_sub_to_be_resumed_list_present, 1));

  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  if (ric_sub_to_be_suspended_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_sub_to_be_suspended_list, bref, 1, 1024, true));
  }
  if (ric_sub_to_be_resumed_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ric_sub_to_be_resumed_list, bref, 1, 1024, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ran_function_state_ctrl_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ranFunctionID", ran_function_id);
  if (ric_sub_to_be_suspended_list.size() > 0) {
    j.start_array("ricSubscriptionToBeSuspended-list");
    for (const auto& e1 : ric_sub_to_be_suspended_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ric_sub_to_be_resumed_list.size() > 0) {
    j.start_array("ricSubscriptionToBeResumed-list");
    for (const auto& e1 : ric_sub_to_be_resumed_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// RANfunctionStateControl-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ran_function_state_ctrl_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {123};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_function_state_ctrl_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 123 == id;
}
crit_e ran_function_state_ctrl_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 123) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ran_function_state_ctrl_item_ies_o::value_c ran_function_state_ctrl_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 123) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ran_function_state_ctrl_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 123) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ran_function_state_ctrl_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ran_function_state_ctrl_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_function_state_ctrl_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ran_function_state_ctrl_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RANfunctionStateControl-Item"};
  return convert_enum_idx(names, 1, value, "ran_function_state_ctrl_item_ies_o::value_c::types");
}

// RICaction-AddedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_added_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_added_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_added_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.end_obj();
}

// RICaction-AddedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_added_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {80};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_added_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 80 == id;
}
crit_e ric_action_added_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 80) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_added_for_mod_item_ies_o::value_c ric_action_added_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 80) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_added_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 80) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_added_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_added_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_added_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_added_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-AddedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_added_for_mod_item_ies_o::value_c::types");
}

// RICaction-Admitted-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_admitted_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_admitted_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_admitted_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.end_obj();
}

// RICaction-Admitted-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_admitted_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {14};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_admitted_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 14 == id;
}
crit_e ric_action_admitted_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 14) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_admitted_item_ies_o::value_c ric_action_admitted_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 14) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_admitted_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 14) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_admitted_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_admitted_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_admitted_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_admitted_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-Admitted-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_admitted_item_ies_o::value_c::types");
}

// RICaction-ConfirmedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_confirmed_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_confirmed_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_confirmed_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.end_obj();
}

// RICaction-ConfirmedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_confirmed_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {88};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_confirmed_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 88 == id;
}
crit_e ric_action_confirmed_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 88) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_confirmed_for_mod_item_ies_o::value_c ric_action_confirmed_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 88) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_confirmed_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 88) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_confirmed_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_confirmed_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_confirmed_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_confirmed_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-ConfirmedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_confirmed_for_mod_item_ies_o::value_c::types");
}

// RICaction-ConfirmedForRemoval-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_confirmed_for_removal_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_confirmed_for_removal_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_confirmed_for_removal_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.end_obj();
}

// RICaction-ConfirmedForRemoval-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_confirmed_for_removal_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {92};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_confirmed_for_removal_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 92 == id;
}
crit_e ric_action_confirmed_for_removal_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 92) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_confirmed_for_removal_item_ies_o::value_c
ric_action_confirmed_for_removal_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 92) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_confirmed_for_removal_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 92) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_confirmed_for_removal_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_confirmed_for_removal_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_confirmed_for_removal_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_confirmed_for_removal_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-ConfirmedForRemoval-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_confirmed_for_removal_item_ies_o::value_c::types");
}

// RICaction-FailedToBeAddedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_failed_to_be_added_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_failed_to_be_added_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_failed_to_be_added_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RICaction-FailedToBeAddedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_failed_to_be_added_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {82};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_failed_to_be_added_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 82 == id;
}
crit_e ric_action_failed_to_be_added_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 82) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_failed_to_be_added_for_mod_item_ies_o::value_c
ric_action_failed_to_be_added_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 82) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_failed_to_be_added_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 82) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_failed_to_be_added_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_failed_to_be_added_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_failed_to_be_added_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_failed_to_be_added_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-FailedToBeAddedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_failed_to_be_added_for_mod_item_ies_o::value_c::types");
}

// RICaction-FailedToBeModifiedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_failed_to_be_modified_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_failed_to_be_modified_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_failed_to_be_modified_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RICaction-FailedToBeModifiedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_failed_to_be_modified_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_failed_to_be_modified_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 78 == id;
}
crit_e ric_action_failed_to_be_modified_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 78) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_failed_to_be_modified_for_mod_item_ies_o::value_c
ric_action_failed_to_be_modified_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 78) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_failed_to_be_modified_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 78) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_failed_to_be_modified_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_failed_to_be_modified_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_failed_to_be_modified_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_failed_to_be_modified_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-FailedToBeModifiedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_failed_to_be_modified_for_mod_item_ies_o::value_c::types");
}

// RICaction-FailedToBeRemovedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_failed_to_be_remd_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_failed_to_be_remd_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_failed_to_be_remd_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RICaction-FailedToBeRemovedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_failed_to_be_remd_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {74};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_failed_to_be_remd_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 74 == id;
}
crit_e ric_action_failed_to_be_remd_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 74) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_failed_to_be_remd_for_mod_item_ies_o::value_c
ric_action_failed_to_be_remd_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 74) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_failed_to_be_remd_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 74) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_failed_to_be_remd_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_failed_to_be_remd_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_failed_to_be_remd_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_failed_to_be_remd_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-FailedToBeRemovedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_failed_to_be_remd_for_mod_item_ies_o::value_c::types");
}

// RICaction-ModifiedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_modified_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_modified_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_modified_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.end_obj();
}

// RICaction-ModifiedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_modified_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {76};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_modified_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 76 == id;
}
crit_e ric_action_modified_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 76) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_modified_for_mod_item_ies_o::value_c ric_action_modified_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 76) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_modified_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 76) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_modified_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_modified_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_modified_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_modified_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-ModifiedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_modified_for_mod_item_ies_o::value_c::types");
}

// RICaction-NotAdmitted-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_not_admitted_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_not_admitted_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_not_admitted_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RICaction-NotAdmitted-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_not_admitted_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {16};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_not_admitted_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 16 == id;
}
crit_e ric_action_not_admitted_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 16) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_not_admitted_item_ies_o::value_c ric_action_not_admitted_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 16) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_not_admitted_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 16) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_not_admitted_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_not_admitted_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_not_admitted_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_not_admitted_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-NotAdmitted-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_not_admitted_item_ies_o::value_c::types");
}

// RICaction-RefusedToBeModified-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_refused_to_be_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_refused_to_be_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_refused_to_be_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RICaction-RefusedToBeModified-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_refused_to_be_modified_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {90};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_refused_to_be_modified_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 90 == id;
}
crit_e ric_action_refused_to_be_modified_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 90) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_refused_to_be_modified_item_ies_o::value_c
ric_action_refused_to_be_modified_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 90) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_refused_to_be_modified_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 90) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_refused_to_be_modified_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_refused_to_be_modified_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_refused_to_be_modified_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_refused_to_be_modified_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-RefusedToBeModified-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_refused_to_be_modified_item_ies_o::value_c::types");
}

// RICaction-RefusedToBeRemoved-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_refused_to_be_remd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_refused_to_be_remd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_refused_to_be_remd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RICaction-RefusedToBeRemoved-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_refused_to_be_remd_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {94};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_refused_to_be_remd_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 94 == id;
}
crit_e ric_action_refused_to_be_remd_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 94) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_refused_to_be_remd_item_ies_o::value_c
ric_action_refused_to_be_remd_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 94) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_refused_to_be_remd_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 94) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_refused_to_be_remd_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_refused_to_be_remd_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_refused_to_be_remd_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_refused_to_be_remd_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-RefusedToBeRemoved-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_refused_to_be_remd_item_ies_o::value_c::types");
}

// RICaction-RemovedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_remd_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_remd_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_remd_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.end_obj();
}

// RICaction-RemovedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_remd_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {72};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_remd_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 72 == id;
}
crit_e ric_action_remd_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 72) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_remd_for_mod_item_ies_o::value_c ric_action_remd_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 72) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_remd_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 72) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_remd_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_remd_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_remd_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_remd_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-RemovedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_remd_for_mod_item_ies_o::value_c::types");
}

// RICaction-RequiredToBeModified-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_required_to_be_modified_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_time_to_wait.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_required_to_be_modified_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_time_to_wait.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_required_to_be_modified_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_str("ricTimeToWait", ric_time_to_wait.to_string());
  j.end_obj();
}

// RICaction-RequiredToBeModified-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_required_to_be_modified_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {84};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_required_to_be_modified_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 84 == id;
}
crit_e ric_action_required_to_be_modified_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 84) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_required_to_be_modified_item_ies_o::value_c
ric_action_required_to_be_modified_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 84) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_required_to_be_modified_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 84) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_required_to_be_modified_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_required_to_be_modified_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_required_to_be_modified_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_required_to_be_modified_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-RequiredToBeModified-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_required_to_be_modified_item_ies_o::value_c::types");
}

// RICaction-RequiredToBeRemoved-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_required_to_be_remd_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_required_to_be_remd_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_required_to_be_remd_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RICaction-RequiredToBeRemoved-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_required_to_be_remd_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {86};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_required_to_be_remd_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 86 == id;
}
crit_e ric_action_required_to_be_remd_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 86) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_required_to_be_remd_item_ies_o::value_c
ric_action_required_to_be_remd_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 86) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_required_to_be_remd_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 86) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_required_to_be_remd_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_required_to_be_remd_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_required_to_be_remd_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_required_to_be_remd_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-RequiredToBeRemoved-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_required_to_be_remd_item_ies_o::value_c::types");
}

// RICaction-ToBeAddedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_to_be_added_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_subsequent_action_present, 1));

  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_type.pack(bref));
  HANDLE_CODE(ric_action_definition.pack(bref));
  HANDLE_CODE(pack_integer(bref, ric_action_execution_order, (uint16_t)0u, (uint16_t)255u, true, true));
  if (ric_subsequent_action_present) {
    HANDLE_CODE(ric_subsequent_action.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_to_be_added_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ric_subsequent_action_present, 1));

  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_type.unpack(bref));
  HANDLE_CODE(ric_action_definition.unpack(bref));
  HANDLE_CODE(unpack_integer(ric_action_execution_order, bref, (uint16_t)0u, (uint16_t)255u, true, true));
  if (ric_subsequent_action_present) {
    HANDLE_CODE(ric_subsequent_action.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_to_be_added_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_str("ricActionType", ric_action_type.to_string());
  j.write_str("ricActionDefinition", ric_action_definition.to_string());
  j.write_int("ricActionExecutionOrder", ric_action_execution_order);
  if (ric_subsequent_action_present) {
    j.write_fieldname("ricSubsequentAction");
    ric_subsequent_action.to_json(j);
  }
  j.end_obj();
}

// RICaction-ToBeAddedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_to_be_added_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {70};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_to_be_added_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 70 == id;
}
crit_e ric_action_to_be_added_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 70) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_to_be_added_for_mod_item_ies_o::value_c
ric_action_to_be_added_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 70) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_to_be_added_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 70) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_to_be_added_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_to_be_added_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_to_be_added_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_to_be_added_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-ToBeAddedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_to_be_added_for_mod_item_ies_o::value_c::types");
}

// RICaction-ToBeModifiedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_to_be_modified_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ric_action_definition.size() > 0, 1));
  HANDLE_CODE(bref.pack(ric_action_execution_order_present, 1));
  HANDLE_CODE(bref.pack(ric_subsequent_action_present, 1));

  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  if (ric_action_definition.size() > 0) {
    HANDLE_CODE(ric_action_definition.pack(bref));
  }
  if (ric_action_execution_order_present) {
    HANDLE_CODE(pack_integer(bref, ric_action_execution_order, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ric_subsequent_action_present) {
    HANDLE_CODE(ric_subsequent_action.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_to_be_modified_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ric_action_definition_present;
  HANDLE_CODE(bref.unpack(ric_action_definition_present, 1));
  HANDLE_CODE(bref.unpack(ric_action_execution_order_present, 1));
  HANDLE_CODE(bref.unpack(ric_subsequent_action_present, 1));

  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  if (ric_action_definition_present) {
    HANDLE_CODE(ric_action_definition.unpack(bref));
  }
  if (ric_action_execution_order_present) {
    HANDLE_CODE(unpack_integer(ric_action_execution_order, bref, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ric_subsequent_action_present) {
    HANDLE_CODE(ric_subsequent_action.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_to_be_modified_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  if (ric_action_definition.size() > 0) {
    j.write_str("ricActionDefinition", ric_action_definition.to_string());
  }
  if (ric_action_execution_order_present) {
    j.write_int("ricActionExecutionOrder", ric_action_execution_order);
  }
  if (ric_subsequent_action_present) {
    j.write_fieldname("ricSubsequentAction");
    ric_subsequent_action.to_json(j);
  }
  j.end_obj();
}

// RICaction-ToBeModifiedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_to_be_modified_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {68};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_to_be_modified_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 68 == id;
}
crit_e ric_action_to_be_modified_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 68) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_to_be_modified_for_mod_item_ies_o::value_c
ric_action_to_be_modified_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 68) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_to_be_modified_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 68) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_to_be_modified_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_to_be_modified_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_to_be_modified_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_to_be_modified_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-ToBeModifiedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_to_be_modified_for_mod_item_ies_o::value_c::types");
}

// RICaction-ToBeRemovedForModification-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_to_be_remd_for_mod_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_to_be_remd_for_mod_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_to_be_remd_for_mod_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.end_obj();
}

// RICaction-ToBeRemovedForModification-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_to_be_remd_for_mod_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {66};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_to_be_remd_for_mod_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 66 == id;
}
crit_e ric_action_to_be_remd_for_mod_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 66) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_to_be_remd_for_mod_item_ies_o::value_c
ric_action_to_be_remd_for_mod_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 66) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_to_be_remd_for_mod_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 66) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_to_be_remd_for_mod_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_to_be_remd_for_mod_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_to_be_remd_for_mod_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_to_be_remd_for_mod_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-ToBeRemovedForModification-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_to_be_remd_for_mod_item_ies_o::value_c::types");
}

// RICaction-ToBeSetup-Item ::= SEQUENCE
OCUDUASN_CODE ric_action_to_be_setup_item_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags;
  group_flags[0] |= ric_action_execution_order_present;
  bref.pack(group_flags.any(), 1);
  HANDLE_CODE(bref.pack(ric_action_definition.size() > 0, 1));
  HANDLE_CODE(bref.pack(ric_subsequent_action_present, 1));

  HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_type.pack(bref));
  if (ric_action_definition.size() > 0) {
    HANDLE_CODE(ric_action_definition.pack(bref));
  }
  if (ric_subsequent_action_present) {
    HANDLE_CODE(ric_subsequent_action.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, true);

      HANDLE_CODE(pack_integer(bref, ric_action_execution_order, (uint16_t)0u, (uint16_t)255u, true, true));
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_to_be_setup_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  bool ric_action_definition_present;
  HANDLE_CODE(bref.unpack(ric_action_definition_present, 1));
  HANDLE_CODE(bref.unpack(ric_subsequent_action_present, 1));

  HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
  HANDLE_CODE(ric_action_type.unpack(bref));
  if (ric_action_definition_present) {
    HANDLE_CODE(ric_action_definition.unpack(bref));
  }
  if (ric_subsequent_action_present) {
    HANDLE_CODE(ric_subsequent_action.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref, true);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      ric_action_execution_order_present = true;
      HANDLE_CODE(unpack_integer(ric_action_execution_order, bref, (uint16_t)0u, (uint16_t)255u, true, true));
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_action_to_be_setup_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ricActionID", ric_action_id);
  j.write_str("ricActionType", ric_action_type.to_string());
  if (ric_action_definition.size() > 0) {
    j.write_str("ricActionDefinition", ric_action_definition.to_string());
  }
  if (ric_subsequent_action_present) {
    j.write_fieldname("ricSubsequentAction");
    ric_subsequent_action.to_json(j);
  }
  if (ric_action_execution_order_present) {
    j.write_int("ricActionExecutionOrder", ric_action_execution_order);
  }
  j.end_obj();
}

// RICaction-ToBeSetup-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_action_to_be_setup_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {19};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_action_to_be_setup_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 19 == id;
}
crit_e ric_action_to_be_setup_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 19) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_action_to_be_setup_item_ies_o::value_c ric_action_to_be_setup_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 19) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_action_to_be_setup_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 19) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_action_to_be_setup_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_action_to_be_setup_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_action_to_be_setup_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_action_to_be_setup_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICaction-ToBeSetup-Item"};
  return convert_enum_idx(names, 1, value, "ric_action_to_be_setup_item_ies_o::value_c::types");
}

// RICassistanceFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_assist_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 1, 2};
  return map_enum_number(names, 3, idx, "id");
}
bool ric_assist_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_assist_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_assist_fail_ies_o::value_c ric_assist_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_assist_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_assist_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_fail_ies_o::value_c");
  }
}
ric_request_id_s& ric_assist_fail_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
cause_c& ric_assist_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_assist_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_assist_fail_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const cause_c& ric_assist_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_assist_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_assist_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_assist_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_assist_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 3, value, "ric_assist_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_assist_fail_ies_o>;

OCUDUASN_CODE ric_assist_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_assist_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_assist_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICassistanceHalt-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_assist_halt_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_assist_halt_ies_o::is_id_valid(const uint32_t& id)
{
  return 29 == id;
}
crit_e ric_assist_halt_ies_o::get_crit(const uint32_t& id)
{
  if (id == 29) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_assist_halt_ies_o::value_c ric_assist_halt_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 29) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_assist_halt_ies_o::get_presence(const uint32_t& id)
{
  if (id == 29) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_assist_halt_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_assist_halt_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_halt_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_assist_halt_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID"};
  return convert_enum_idx(names, 1, value, "ric_assist_halt_ies_o::value_c::types");
}

// RICassistanceIndication-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_assist_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 130, 127, 129};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_assist_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 130, 127, 129};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_assist_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 130:
      return crit_e::reject;
    case 127:
      return crit_e::reject;
    case 129:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_assist_ind_ies_o::value_c ric_assist_ind_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 130:
      ret.set(value_c::types::ric_assist_sn);
      break;
    case 127:
      ret.set(value_c::types::ric_assist_hdr);
      break;
    case 129:
      ret.set(value_c::types::ric_assist_outcome);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_assist_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 130:
      return presence_e::mandatory;
    case 127:
      return presence_e::mandatory;
    case 129:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_assist_ind_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ric_assist_sn:
      c = uint32_t{};
      break;
    case types::ric_assist_hdr:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_assist_outcome:
      c = unbounded_octstring<true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_ind_ies_o::value_c");
  }
}
ric_request_id_s& ric_assist_ind_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint32_t& ric_assist_ind_ies_o::value_c::ric_assist_sn()
{
  assert_choice_type(types::ric_assist_sn, type_, "Value");
  return c.get<uint32_t>();
}
unbounded_octstring<true>& ric_assist_ind_ies_o::value_c::ric_assist_hdr()
{
  assert_choice_type(types::ric_assist_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_assist_ind_ies_o::value_c::ric_assist_outcome()
{
  assert_choice_type(types::ric_assist_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_request_id_s& ric_assist_ind_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint32_t& ric_assist_ind_ies_o::value_c::ric_assist_sn() const
{
  assert_choice_type(types::ric_assist_sn, type_, "Value");
  return c.get<uint32_t>();
}
const unbounded_octstring<true>& ric_assist_ind_ies_o::value_c::ric_assist_hdr() const
{
  assert_choice_type(types::ric_assist_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_assist_ind_ies_o::value_c::ric_assist_outcome() const
{
  assert_choice_type(types::ric_assist_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
void ric_assist_ind_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ric_assist_sn:
      j.write_int(c.get<uint32_t>());
      break;
    case types::ric_assist_hdr:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_assist_outcome:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_ind_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_assist_ind_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ric_assist_sn:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)65535u, false, true));
      break;
    case types::ric_assist_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_assist_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_ind_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_ind_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ric_assist_sn:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)65535u, false, true));
      break;
    case types::ric_assist_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_assist_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_ind_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_assist_ind_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..65535)", "OCTET STRING", "OCTET STRING"};
  return convert_enum_idx(names, 4, value, "ric_assist_ind_ies_o::value_c::types");
}
uint8_t ric_assist_ind_ies_o::value_c::types_opts::to_number() const
{
  if (value == ric_assist_sn) {
    return 0;
  }
  invalid_enum_number(value, "ric_assist_ind_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_assist_ind_ies_o>;

OCUDUASN_CODE ric_assist_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)130, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_assist_sn, (uint32_t)0u, (uint32_t)65535u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)127, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_assist_hdr.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)129, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_assist_outcome.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_assist_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 130: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_assist_sn, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
        break;
      }
      case 127: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_assist_hdr.unpack(bref));
        break;
      }
      case 129: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_assist_outcome.unpack(bref));
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
void ric_assist_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 130);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_assist_sn);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 127);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_assist_hdr.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 129);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_assist_outcome.to_string());
  j.end_obj();
  j.end_array();
}

// RICassistanceRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_assist_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 127, 128, 131, 132};
  return map_enum_number(names, 5, idx, "id");
}
bool ric_assist_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 127, 128, 131, 132};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_assist_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 127:
      return crit_e::reject;
    case 128:
      return crit_e::reject;
    case 131:
      return crit_e::reject;
    case 132:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_assist_request_ies_o::value_c ric_assist_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 127:
      ret.set(value_c::types::ric_assist_hdr);
      break;
    case 128:
      ret.set(value_c::types::ric_assist_msg);
      break;
    case 131:
      ret.set(value_c::types::ric_assist_upd);
      break;
    case 132:
      ret.set(value_c::types::ric_assist_upd_num);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_assist_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 127:
      return presence_e::mandatory;
    case 128:
      return presence_e::mandatory;
    case 131:
      return presence_e::optional;
    case 132:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_assist_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ric_assist_hdr:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_assist_msg:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_assist_upd:
      c = ric_assist_upd_e{};
      break;
    case types::ric_assist_upd_num:
      c = uint32_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_request_ies_o::value_c");
  }
}
ric_request_id_s& ric_assist_request_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
unbounded_octstring<true>& ric_assist_request_ies_o::value_c::ric_assist_hdr()
{
  assert_choice_type(types::ric_assist_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_assist_request_ies_o::value_c::ric_assist_msg()
{
  assert_choice_type(types::ric_assist_msg, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
ric_assist_upd_e& ric_assist_request_ies_o::value_c::ric_assist_upd()
{
  assert_choice_type(types::ric_assist_upd, type_, "Value");
  return c.get<ric_assist_upd_e>();
}
uint32_t& ric_assist_request_ies_o::value_c::ric_assist_upd_num()
{
  assert_choice_type(types::ric_assist_upd_num, type_, "Value");
  return c.get<uint32_t>();
}
const ric_request_id_s& ric_assist_request_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const unbounded_octstring<true>& ric_assist_request_ies_o::value_c::ric_assist_hdr() const
{
  assert_choice_type(types::ric_assist_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_assist_request_ies_o::value_c::ric_assist_msg() const
{
  assert_choice_type(types::ric_assist_msg, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_assist_upd_e& ric_assist_request_ies_o::value_c::ric_assist_upd() const
{
  assert_choice_type(types::ric_assist_upd, type_, "Value");
  return c.get<ric_assist_upd_e>();
}
const uint32_t& ric_assist_request_ies_o::value_c::ric_assist_upd_num() const
{
  assert_choice_type(types::ric_assist_upd_num, type_, "Value");
  return c.get<uint32_t>();
}
void ric_assist_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ric_assist_hdr:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_assist_msg:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_assist_upd:
      j.write_str("true");
      break;
    case types::ric_assist_upd_num:
      j.write_int(c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_assist_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ric_assist_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_assist_msg:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_assist_upd:
      HANDLE_CODE(c.get<ric_assist_upd_e>().pack(bref));
      break;
    case types::ric_assist_upd_num:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)65535u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ric_assist_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_assist_msg:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_assist_upd:
      HANDLE_CODE(c.get<ric_assist_upd_e>().unpack(bref));
      break;
    case types::ric_assist_upd_num:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)65535u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_assist_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "RICrequestID", "OCTET STRING", "OCTET STRING", "RICassistanceUpdate", "INTEGER (0..65535)"};
  return convert_enum_idx(names, 5, value, "ric_assist_request_ies_o::value_c::types");
}
uint8_t ric_assist_request_ies_o::value_c::types_opts::to_number() const
{
  if (value == ric_assist_upd_num) {
    return 0;
  }
  invalid_enum_number(value, "ric_assist_request_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_assist_request_ies_o>;

OCUDUASN_CODE ric_assist_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ric_assist_upd_present ? 1 : 0;
  nof_ies += ric_assist_upd_num_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)127, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_assist_hdr.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)128, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_assist_msg.pack(bref));
  }
  if (ric_assist_upd_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)131, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_assist_upd.pack(bref));
  }
  if (ric_assist_upd_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)132, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_assist_upd_num, (uint32_t)0u, (uint32_t)65535u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_assist_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 127: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_assist_hdr.unpack(bref));
        break;
      }
      case 128: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_assist_msg.unpack(bref));
        break;
      }
      case 131: {
        ric_assist_upd_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_assist_upd.unpack(bref));
        break;
      }
      case 132: {
        ric_assist_upd_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_assist_upd_num, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
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
void ric_assist_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 127);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_assist_hdr.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 128);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_assist_msg.to_string());
  j.end_obj();
  if (ric_assist_upd_present) {
    j.start_obj();
    j.write_int("id", 131);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (ric_assist_upd_num_present) {
    j.start_obj();
    j.write_int("id", 132);
    j.write_str("criticality", "reject");
    j.write_int("value", ric_assist_upd_num);
    j.end_obj();
  }
  j.end_array();
}

// RICassistanceResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_assist_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 127, 129};
  return map_enum_number(names, 3, idx, "id");
}
bool ric_assist_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 127, 129};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_assist_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 127:
      return crit_e::reject;
    case 129:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_assist_resp_ies_o::value_c ric_assist_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 127:
      ret.set(value_c::types::ric_assist_hdr);
      break;
    case 129:
      ret.set(value_c::types::ric_assist_outcome);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_assist_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 127:
      return presence_e::mandatory;
    case 129:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_assist_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ric_assist_hdr:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_assist_outcome:
      c = unbounded_octstring<true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_resp_ies_o::value_c");
  }
}
ric_request_id_s& ric_assist_resp_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
unbounded_octstring<true>& ric_assist_resp_ies_o::value_c::ric_assist_hdr()
{
  assert_choice_type(types::ric_assist_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_assist_resp_ies_o::value_c::ric_assist_outcome()
{
  assert_choice_type(types::ric_assist_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_request_id_s& ric_assist_resp_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const unbounded_octstring<true>& ric_assist_resp_ies_o::value_c::ric_assist_hdr() const
{
  assert_choice_type(types::ric_assist_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_assist_resp_ies_o::value_c::ric_assist_outcome() const
{
  assert_choice_type(types::ric_assist_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
void ric_assist_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ric_assist_hdr:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_assist_outcome:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_assist_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ric_assist_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_assist_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ric_assist_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_assist_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_assist_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_assist_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "OCTET STRING", "OCTET STRING"};
  return convert_enum_idx(names, 3, value, "ric_assist_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_assist_resp_ies_o>;

OCUDUASN_CODE ric_assist_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)127, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_assist_hdr.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)129, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_assist_outcome.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_assist_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_assist_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 127: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_assist_hdr.unpack(bref));
        break;
      }
      case 129: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_assist_outcome.unpack(bref));
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
void ric_assist_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 127);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_assist_hdr.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 129);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_assist_outcome.to_string());
  j.end_obj();
  j.end_array();
}

// RICcontrolAcknowledge-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_ctrl_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 20, 32};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_ctrl_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 20, 32};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_ctrl_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 20:
      return crit_e::reject;
    case 32:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_ctrl_ack_ies_o::value_c ric_ctrl_ack_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 20:
      ret.set(value_c::types::ric_call_process_id);
      break;
    case 32:
      ret.set(value_c::types::ric_ctrl_outcome);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_ctrl_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 20:
      return presence_e::optional;
    case 32:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_ctrl_ack_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_call_process_id:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_ctrl_outcome:
      c = unbounded_octstring<true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_ack_ies_o::value_c");
  }
}
ric_request_id_s& ric_ctrl_ack_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_ctrl_ack_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
unbounded_octstring<true>& ric_ctrl_ack_ies_o::value_c::ric_call_process_id()
{
  assert_choice_type(types::ric_call_process_id, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_ctrl_ack_ies_o::value_c::ric_ctrl_outcome()
{
  assert_choice_type(types::ric_ctrl_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_request_id_s& ric_ctrl_ack_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_ctrl_ack_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const unbounded_octstring<true>& ric_ctrl_ack_ies_o::value_c::ric_call_process_id() const
{
  assert_choice_type(types::ric_call_process_id, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_ctrl_ack_ies_o::value_c::ric_ctrl_outcome() const
{
  assert_choice_type(types::ric_ctrl_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
void ric_ctrl_ack_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_call_process_id:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_ctrl_outcome:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_ack_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_ctrl_ack_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_call_process_id:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_ctrl_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_ack_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_ctrl_ack_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_call_process_id:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_ctrl_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_ack_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_ctrl_ack_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)", "OCTET STRING", "OCTET STRING"};
  return convert_enum_idx(names, 4, value, "ric_ctrl_ack_ies_o::value_c::types");
}
uint8_t ric_ctrl_ack_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_ctrl_ack_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_ctrl_ack_ies_o>;

OCUDUASN_CODE ric_ctrl_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ric_call_process_id_present ? 1 : 0;
  nof_ies += ric_ctrl_outcome_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  if (ric_call_process_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)20, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_call_process_id.pack(bref));
  }
  if (ric_ctrl_outcome_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)32, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_ctrl_outcome.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_ctrl_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_ctrl_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 20: {
        ric_call_process_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_call_process_id.unpack(bref));
        break;
      }
      case 32: {
        ric_ctrl_outcome_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_ctrl_outcome.unpack(bref));
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
void ric_ctrl_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  if (ric_call_process_id_present) {
    j.start_obj();
    j.write_int("id", 20);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_call_process_id.to_string());
    j.end_obj();
  }
  if (ric_ctrl_outcome_present) {
    j.start_obj();
    j.write_int("id", 32);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_ctrl_outcome.to_string());
    j.end_obj();
  }
  j.end_array();
}

// RICcontrolFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_ctrl_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 20, 1, 32, 2};
  return map_enum_number(names, 6, idx, "id");
}
bool ric_ctrl_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 20, 1, 32, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_ctrl_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 20:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 32:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_ctrl_fail_ies_o::value_c ric_ctrl_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 20:
      ret.set(value_c::types::ric_call_process_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 32:
      ret.set(value_c::types::ric_ctrl_outcome);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_ctrl_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 20:
      return presence_e::optional;
    case 1:
      return presence_e::mandatory;
    case 32:
      return presence_e::optional;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_ctrl_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_call_process_id:
      c = unbounded_octstring<true>{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::ric_ctrl_outcome:
      c = unbounded_octstring<true>{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_fail_ies_o::value_c");
  }
}
ric_request_id_s& ric_ctrl_fail_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_ctrl_fail_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
unbounded_octstring<true>& ric_ctrl_fail_ies_o::value_c::ric_call_process_id()
{
  assert_choice_type(types::ric_call_process_id, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
cause_c& ric_ctrl_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
unbounded_octstring<true>& ric_ctrl_fail_ies_o::value_c::ric_ctrl_outcome()
{
  assert_choice_type(types::ric_ctrl_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
crit_diagnostics_s& ric_ctrl_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_ctrl_fail_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_ctrl_fail_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const unbounded_octstring<true>& ric_ctrl_fail_ies_o::value_c::ric_call_process_id() const
{
  assert_choice_type(types::ric_call_process_id, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const cause_c& ric_ctrl_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const unbounded_octstring<true>& ric_ctrl_fail_ies_o::value_c::ric_ctrl_outcome() const
{
  assert_choice_type(types::ric_ctrl_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const crit_diagnostics_s& ric_ctrl_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_ctrl_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_call_process_id:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::ric_ctrl_outcome:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_ctrl_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_call_process_id:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::ric_ctrl_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_ctrl_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_call_process_id:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::ric_ctrl_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_ctrl_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "RICrequestID", "INTEGER (0..4095)", "OCTET STRING", "Cause", "OCTET STRING", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 6, value, "ric_ctrl_fail_ies_o::value_c::types");
}
uint8_t ric_ctrl_fail_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_ctrl_fail_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_ctrl_fail_ies_o>;

OCUDUASN_CODE ric_ctrl_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ric_call_process_id_present ? 1 : 0;
  nof_ies += ric_ctrl_outcome_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  if (ric_call_process_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)20, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_call_process_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (ric_ctrl_outcome_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)32, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_ctrl_outcome.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_ctrl_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_ctrl_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 20: {
        ric_call_process_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_call_process_id.unpack(bref));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 32: {
        ric_ctrl_outcome_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_ctrl_outcome.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_ctrl_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  if (ric_call_process_id_present) {
    j.start_obj();
    j.write_int("id", 20);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_call_process_id.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (ric_ctrl_outcome_present) {
    j.start_obj();
    j.write_int("id", 32);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_ctrl_outcome.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICcontrolRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_ctrl_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 20, 22, 23, 21};
  return map_enum_number(names, 6, idx, "id");
}
bool ric_ctrl_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 20, 22, 23, 21};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_ctrl_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 20:
      return crit_e::reject;
    case 22:
      return crit_e::reject;
    case 23:
      return crit_e::reject;
    case 21:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_ctrl_request_ies_o::value_c ric_ctrl_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 20:
      ret.set(value_c::types::ric_call_process_id);
      break;
    case 22:
      ret.set(value_c::types::ric_ctrl_hdr);
      break;
    case 23:
      ret.set(value_c::types::ric_ctrl_msg);
      break;
    case 21:
      ret.set(value_c::types::ric_ctrl_ack_request);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_ctrl_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 20:
      return presence_e::optional;
    case 22:
      return presence_e::mandatory;
    case 23:
      return presence_e::mandatory;
    case 21:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_ctrl_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_call_process_id:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_ctrl_hdr:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_ctrl_msg:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_ctrl_ack_request:
      c = ric_ctrl_ack_request_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_request_ies_o::value_c");
  }
}
ric_request_id_s& ric_ctrl_request_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_ctrl_request_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
unbounded_octstring<true>& ric_ctrl_request_ies_o::value_c::ric_call_process_id()
{
  assert_choice_type(types::ric_call_process_id, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_ctrl_request_ies_o::value_c::ric_ctrl_hdr()
{
  assert_choice_type(types::ric_ctrl_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_ctrl_request_ies_o::value_c::ric_ctrl_msg()
{
  assert_choice_type(types::ric_ctrl_msg, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
ric_ctrl_ack_request_e& ric_ctrl_request_ies_o::value_c::ric_ctrl_ack_request()
{
  assert_choice_type(types::ric_ctrl_ack_request, type_, "Value");
  return c.get<ric_ctrl_ack_request_e>();
}
const ric_request_id_s& ric_ctrl_request_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_ctrl_request_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const unbounded_octstring<true>& ric_ctrl_request_ies_o::value_c::ric_call_process_id() const
{
  assert_choice_type(types::ric_call_process_id, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_ctrl_request_ies_o::value_c::ric_ctrl_hdr() const
{
  assert_choice_type(types::ric_ctrl_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_ctrl_request_ies_o::value_c::ric_ctrl_msg() const
{
  assert_choice_type(types::ric_ctrl_msg, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_ctrl_ack_request_e& ric_ctrl_request_ies_o::value_c::ric_ctrl_ack_request() const
{
  assert_choice_type(types::ric_ctrl_ack_request, type_, "Value");
  return c.get<ric_ctrl_ack_request_e>();
}
void ric_ctrl_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_call_process_id:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_ctrl_hdr:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_ctrl_msg:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_ctrl_ack_request:
      j.write_str(c.get<ric_ctrl_ack_request_e>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_ctrl_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_call_process_id:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_ctrl_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_ctrl_msg:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_ctrl_ack_request:
      HANDLE_CODE(c.get<ric_ctrl_ack_request_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_ctrl_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_call_process_id:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_ctrl_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_ctrl_msg:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_ctrl_ack_request:
      HANDLE_CODE(c.get<ric_ctrl_ack_request_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_ctrl_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_ctrl_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "RICrequestID", "INTEGER (0..4095)", "OCTET STRING", "OCTET STRING", "OCTET STRING", "RICcontrolAckRequest"};
  return convert_enum_idx(names, 6, value, "ric_ctrl_request_ies_o::value_c::types");
}
uint8_t ric_ctrl_request_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_ctrl_request_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_ctrl_request_ies_o>;

OCUDUASN_CODE ric_ctrl_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += ric_call_process_id_present ? 1 : 0;
  nof_ies += ric_ctrl_ack_request_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  if (ric_call_process_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)20, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_call_process_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)22, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_ctrl_hdr.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)23, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_ctrl_msg.pack(bref));
  }
  if (ric_ctrl_ack_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)21, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_ctrl_ack_request.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_ctrl_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_ctrl_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 20: {
        ric_call_process_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_call_process_id.unpack(bref));
        break;
      }
      case 22: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_ctrl_hdr.unpack(bref));
        break;
      }
      case 23: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_ctrl_msg.unpack(bref));
        break;
      }
      case 21: {
        ric_ctrl_ack_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_ctrl_ack_request.unpack(bref));
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
void ric_ctrl_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  if (ric_call_process_id_present) {
    j.start_obj();
    j.write_int("id", 20);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_call_process_id.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 22);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_ctrl_hdr.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 23);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_ctrl_msg.to_string());
  j.end_obj();
  if (ric_ctrl_ack_request_present) {
    j.start_obj();
    j.write_int("id", 21);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_ctrl_ack_request.to_string());
    j.end_obj();
  }
  j.end_array();
}

// RICindication-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 15, 27, 28, 25, 26, 20};
  return map_enum_number(names, 8, idx, "id");
}
bool ric_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 15, 27, 28, 25, 26, 20};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 15:
      return crit_e::reject;
    case 27:
      return crit_e::reject;
    case 28:
      return crit_e::reject;
    case 25:
      return crit_e::reject;
    case 26:
      return crit_e::reject;
    case 20:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_ind_ies_o::value_c ric_ind_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 15:
      ret.set(value_c::types::ric_action_id);
      break;
    case 27:
      ret.set(value_c::types::ric_ind_sn);
      break;
    case 28:
      ret.set(value_c::types::ric_ind_type);
      break;
    case 25:
      ret.set(value_c::types::ric_ind_hdr);
      break;
    case 26:
      ret.set(value_c::types::ric_ind_msg);
      break;
    case 20:
      ret.set(value_c::types::ric_call_process_id);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 27:
      return presence_e::optional;
    case 28:
      return presence_e::mandatory;
    case 25:
      return presence_e::mandatory;
    case 26:
      return presence_e::mandatory;
    case 20:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_ind_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_action_id:
      c = uint16_t{};
      break;
    case types::ric_ind_sn:
      c = uint32_t{};
      break;
    case types::ric_ind_type:
      c = ric_ind_type_e{};
      break;
    case types::ric_ind_hdr:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_ind_msg:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_call_process_id:
      c = unbounded_octstring<true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_ind_ies_o::value_c");
  }
}
ric_request_id_s& ric_ind_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_ind_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& ric_ind_ies_o::value_c::ric_action_id()
{
  assert_choice_type(types::ric_action_id, type_, "Value");
  return c.get<uint16_t>();
}
uint32_t& ric_ind_ies_o::value_c::ric_ind_sn()
{
  assert_choice_type(types::ric_ind_sn, type_, "Value");
  return c.get<uint32_t>();
}
ric_ind_type_e& ric_ind_ies_o::value_c::ric_ind_type()
{
  assert_choice_type(types::ric_ind_type, type_, "Value");
  return c.get<ric_ind_type_e>();
}
unbounded_octstring<true>& ric_ind_ies_o::value_c::ric_ind_hdr()
{
  assert_choice_type(types::ric_ind_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_ind_ies_o::value_c::ric_ind_msg()
{
  assert_choice_type(types::ric_ind_msg, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_ind_ies_o::value_c::ric_call_process_id()
{
  assert_choice_type(types::ric_call_process_id, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_request_id_s& ric_ind_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_ind_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& ric_ind_ies_o::value_c::ric_action_id() const
{
  assert_choice_type(types::ric_action_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint32_t& ric_ind_ies_o::value_c::ric_ind_sn() const
{
  assert_choice_type(types::ric_ind_sn, type_, "Value");
  return c.get<uint32_t>();
}
const ric_ind_type_e& ric_ind_ies_o::value_c::ric_ind_type() const
{
  assert_choice_type(types::ric_ind_type, type_, "Value");
  return c.get<ric_ind_type_e>();
}
const unbounded_octstring<true>& ric_ind_ies_o::value_c::ric_ind_hdr() const
{
  assert_choice_type(types::ric_ind_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_ind_ies_o::value_c::ric_ind_msg() const
{
  assert_choice_type(types::ric_ind_msg, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_ind_ies_o::value_c::ric_call_process_id() const
{
  assert_choice_type(types::ric_call_process_id, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
void ric_ind_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_action_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_ind_sn:
      j.write_int(c.get<uint32_t>());
      break;
    case types::ric_ind_type:
      j.write_str(c.get<ric_ind_type_e>().to_string());
      break;
    case types::ric_ind_hdr:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_ind_msg:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_call_process_id:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_ind_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_ind_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_action_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, false, true));
      break;
    case types::ric_ind_sn:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)65535u, false, true));
      break;
    case types::ric_ind_type:
      HANDLE_CODE(c.get<ric_ind_type_e>().pack(bref));
      break;
    case types::ric_ind_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_ind_msg:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_call_process_id:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_ind_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_ind_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_action_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, false, true));
      break;
    case types::ric_ind_sn:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)65535u, false, true));
      break;
    case types::ric_ind_type:
      HANDLE_CODE(c.get<ric_ind_type_e>().unpack(bref));
      break;
    case types::ric_ind_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_ind_msg:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_call_process_id:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_ind_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_ind_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID",
                                "INTEGER (0..4095)",
                                "INTEGER (0..255)",
                                "INTEGER (0..65535)",
                                "RICindicationType",
                                "OCTET STRING",
                                "OCTET STRING",
                                "OCTET STRING"};
  return convert_enum_idx(names, 8, value, "ric_ind_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_ind_ies_o>;

OCUDUASN_CODE ric_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 6;
  nof_ies += ric_ind_sn_present ? 1 : 0;
  nof_ies += ric_call_process_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)15, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_action_id, (uint16_t)0u, (uint16_t)255u, false, true));
  }
  if (ric_ind_sn_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)27, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_ind_sn, (uint32_t)0u, (uint32_t)65535u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)28, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_ind_type.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)25, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_ind_hdr.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)26, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_ind_msg.pack(bref));
  }
  if (ric_call_process_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)20, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_call_process_id.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 6;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_action_id, bref, (uint16_t)0u, (uint16_t)255u, false, true));
        break;
      }
      case 27: {
        ric_ind_sn_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_ind_sn, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
        break;
      }
      case 28: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_ind_type.unpack(bref));
        break;
      }
      case 25: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_ind_hdr.unpack(bref));
        break;
      }
      case 26: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_ind_msg.unpack(bref));
        break;
      }
      case 20: {
        ric_call_process_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_call_process_id.unpack(bref));
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
void ric_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_action_id);
  j.end_obj();
  if (ric_ind_sn_present) {
    j.start_obj();
    j.write_int("id", 27);
    j.write_str("criticality", "reject");
    j.write_int("value", ric_ind_sn);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 28);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_ind_type.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 25);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_ind_hdr.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 26);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_ind_msg.to_string());
  j.end_obj();
  if (ric_call_process_id_present) {
    j.start_obj();
    j.write_int("id", 20);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_call_process_id.to_string());
    j.end_obj();
  }
  j.end_array();
}

// RICqueryFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_query_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_query_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_query_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_query_fail_ies_o::value_c ric_query_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_query_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_query_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_fail_ies_o::value_c");
  }
}
ric_request_id_s& ric_query_fail_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_query_fail_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& ric_query_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_query_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_query_fail_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_query_fail_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& ric_query_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_query_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_query_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_query_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_query_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_query_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "ric_query_fail_ies_o::value_c::types");
}
uint8_t ric_query_fail_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_query_fail_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_query_fail_ies_o>;

OCUDUASN_CODE ric_query_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_query_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_query_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_query_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICqueryRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_query_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 95, 96};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_query_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 95, 96};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_query_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 95:
      return crit_e::reject;
    case 96:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_query_request_ies_o::value_c ric_query_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 95:
      ret.set(value_c::types::ric_query_hdr);
      break;
    case 96:
      ret.set(value_c::types::ric_query_definition);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_query_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 95:
      return presence_e::mandatory;
    case 96:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_query_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_query_hdr:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_query_definition:
      c = unbounded_octstring<true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_request_ies_o::value_c");
  }
}
ric_request_id_s& ric_query_request_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_query_request_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
unbounded_octstring<true>& ric_query_request_ies_o::value_c::ric_query_hdr()
{
  assert_choice_type(types::ric_query_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
unbounded_octstring<true>& ric_query_request_ies_o::value_c::ric_query_definition()
{
  assert_choice_type(types::ric_query_definition, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_request_id_s& ric_query_request_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_query_request_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const unbounded_octstring<true>& ric_query_request_ies_o::value_c::ric_query_hdr() const
{
  assert_choice_type(types::ric_query_hdr, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const unbounded_octstring<true>& ric_query_request_ies_o::value_c::ric_query_definition() const
{
  assert_choice_type(types::ric_query_definition, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
void ric_query_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_query_hdr:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_query_definition:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_query_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_query_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_query_definition:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_query_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_query_hdr:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_query_definition:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_query_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)", "OCTET STRING", "OCTET STRING"};
  return convert_enum_idx(names, 4, value, "ric_query_request_ies_o::value_c::types");
}
uint8_t ric_query_request_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_query_request_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_query_request_ies_o>;

OCUDUASN_CODE ric_query_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)95, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_query_hdr.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)96, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_query_definition.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_query_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_query_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 95: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_query_hdr.unpack(bref));
        break;
      }
      case 96: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_query_definition.unpack(bref));
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
void ric_query_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 95);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_query_hdr.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 96);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_query_definition.to_string());
  j.end_obj();
  j.end_array();
}

// RICqueryResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_query_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 97};
  return map_enum_number(names, 3, idx, "id");
}
bool ric_query_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 97};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_query_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 97:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_query_resp_ies_o::value_c ric_query_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 97:
      ret.set(value_c::types::ric_query_outcome);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_query_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 97:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_query_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_query_outcome:
      c = unbounded_octstring<true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_resp_ies_o::value_c");
  }
}
ric_request_id_s& ric_query_resp_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_query_resp_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
unbounded_octstring<true>& ric_query_resp_ies_o::value_c::ric_query_outcome()
{
  assert_choice_type(types::ric_query_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_request_id_s& ric_query_resp_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_query_resp_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const unbounded_octstring<true>& ric_query_resp_ies_o::value_c::ric_query_outcome() const
{
  assert_choice_type(types::ric_query_outcome, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
void ric_query_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_query_outcome:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_query_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_query_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_query_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_query_outcome:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_query_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_query_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)", "OCTET STRING"};
  return convert_enum_idx(names, 3, value, "ric_query_resp_ies_o::value_c::types");
}
uint8_t ric_query_resp_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_query_resp_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_query_resp_ies_o>;

OCUDUASN_CODE ric_query_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)97, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_query_outcome.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_query_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_query_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 97: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_query_outcome.unpack(bref));
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
void ric_query_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 97);
  j.write_str("criticality", "reject");
  j.write_str("value", ric_query_outcome.to_string());
  j.end_obj();
  j.end_array();
}

// RICserviceLoadStatusFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_service_load_status_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {117, 105, 1, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_service_load_status_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {117, 105, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_service_load_status_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 117:
      return crit_e::reject;
    case 105:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_service_load_status_fail_ies_o::value_c ric_service_load_status_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 117:
      ret.set(value_c::types::ric_load_meas_id);
      break;
    case 105:
      ret.set(value_c::types::e2node_load_meas_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_service_load_status_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 117:
      return presence_e::mandatory;
    case 105:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_service_load_status_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_load_meas_id:
      c = uint16_t{};
      break;
    case types::e2node_load_meas_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_fail_ies_o::value_c");
  }
}
uint16_t& ric_service_load_status_fail_ies_o::value_c::ric_load_meas_id()
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& ric_service_load_status_fail_ies_o::value_c::e2node_load_meas_id()
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& ric_service_load_status_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_service_load_status_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& ric_service_load_status_fail_ies_o::value_c::ric_load_meas_id() const
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& ric_service_load_status_fail_ies_o::value_c::e2node_load_meas_id() const
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& ric_service_load_status_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_service_load_status_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_service_load_status_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_service_load_status_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_status_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_service_load_status_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..4095,...)", "INTEGER (1..4095,...)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "ric_service_load_status_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_service_load_status_fail_ies_o>;

OCUDUASN_CODE ric_service_load_status_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, e2node_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_status_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_service_load_status_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 105: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(e2node_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_service_load_status_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 105);
  j.write_str("criticality", "ignore");
  j.write_int("value", e2node_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICserviceLoadStatusRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_service_load_status_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {117, 105, 112, 111, 113};
  return map_enum_number(names, 5, idx, "id");
}
bool ric_service_load_status_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {117, 105, 112, 111, 113};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_service_load_status_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 117:
      return crit_e::reject;
    case 105:
      return crit_e::ignore;
    case 112:
      return crit_e::ignore;
    case 111:
      return crit_e::reject;
    case 113:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_service_load_status_request_ies_o::value_c ric_service_load_status_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 117:
      ret.set(value_c::types::ric_load_meas_id);
      break;
    case 105:
      ret.set(value_c::types::e2node_load_meas_id);
      break;
    case 112:
      ret.set(value_c::types::regist_request);
      break;
    case 111:
      ret.set(value_c::types::ran_function_load_request_list);
      break;
    case 113:
      ret.set(value_c::types::report_periodicity);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_service_load_status_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 117:
      return presence_e::mandatory;
    case 105:
      return presence_e::conditional;
    case 112:
      return presence_e::mandatory;
    case 111:
      return presence_e::conditional;
    case 113:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_service_load_status_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_load_meas_id:
      c = uint16_t{};
      break;
    case types::e2node_load_meas_id:
      c = uint16_t{};
      break;
    case types::regist_request:
      c = regist_request_e{};
      break;
    case types::ran_function_load_request_list:
      c = ran_function_load_request_list_l{};
      break;
    case types::report_periodicity:
      c = report_periodicity_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_request_ies_o::value_c");
  }
}
uint16_t& ric_service_load_status_request_ies_o::value_c::ric_load_meas_id()
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& ric_service_load_status_request_ies_o::value_c::e2node_load_meas_id()
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
regist_request_e& ric_service_load_status_request_ies_o::value_c::regist_request()
{
  assert_choice_type(types::regist_request, type_, "Value");
  return c.get<regist_request_e>();
}
ran_function_load_request_list_l& ric_service_load_status_request_ies_o::value_c::ran_function_load_request_list()
{
  assert_choice_type(types::ran_function_load_request_list, type_, "Value");
  return c.get<ran_function_load_request_list_l>();
}
report_periodicity_e& ric_service_load_status_request_ies_o::value_c::report_periodicity()
{
  assert_choice_type(types::report_periodicity, type_, "Value");
  return c.get<report_periodicity_e>();
}
const uint16_t& ric_service_load_status_request_ies_o::value_c::ric_load_meas_id() const
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& ric_service_load_status_request_ies_o::value_c::e2node_load_meas_id() const
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const regist_request_e& ric_service_load_status_request_ies_o::value_c::regist_request() const
{
  assert_choice_type(types::regist_request, type_, "Value");
  return c.get<regist_request_e>();
}
const ran_function_load_request_list_l&
ric_service_load_status_request_ies_o::value_c::ran_function_load_request_list() const
{
  assert_choice_type(types::ran_function_load_request_list, type_, "Value");
  return c.get<ran_function_load_request_list_l>();
}
const report_periodicity_e& ric_service_load_status_request_ies_o::value_c::report_periodicity() const
{
  assert_choice_type(types::report_periodicity, type_, "Value");
  return c.get<report_periodicity_e>();
}
void ric_service_load_status_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::regist_request:
      j.write_str(c.get<regist_request_e>().to_string());
      break;
    case types::ran_function_load_request_list:
      j.start_array("");
      for (const auto& e1 : c.get<ran_function_load_request_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::report_periodicity:
      j.write_str(c.get<report_periodicity_e>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_service_load_status_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::regist_request:
      HANDLE_CODE(c.get<regist_request_e>().pack(bref));
      break;
    case types::ran_function_load_request_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_function_load_request_list_l>(), 1, 256, true));
      break;
    case types::report_periodicity:
      HANDLE_CODE(c.get<report_periodicity_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_status_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::regist_request:
      HANDLE_CODE(c.get<regist_request_e>().unpack(bref));
      break;
    case types::ran_function_load_request_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_function_load_request_list_l>(), bref, 1, 256, true));
      break;
    case types::report_periodicity:
      HANDLE_CODE(c.get<report_periodicity_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_service_load_status_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..4095,...)",
                                "INTEGER (1..4095,...)",
                                "RegistrationRequest",
                                "RANfunctionLoadRequest-List",
                                "ReportingPeriodicity"};
  return convert_enum_idx(names, 5, value, "ric_service_load_status_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_service_load_status_request_ies_o>;

OCUDUASN_CODE ric_service_load_status_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += e2node_load_meas_id_present ? 1 : 0;
  nof_ies += ran_function_load_request_list_present ? 1 : 0;
  nof_ies += report_periodicity_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  if (e2node_load_meas_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, e2node_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)112, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(regist_request.pack(bref));
  }
  if (ran_function_load_request_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)111, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_function_load_request_list, 1, 256, true));
  }
  if (report_periodicity_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)113, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(report_periodicity.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_status_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_service_load_status_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 105: {
        e2node_load_meas_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(e2node_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 112: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(regist_request.unpack(bref));
        break;
      }
      case 111: {
        ran_function_load_request_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_function_load_request_list, bref, 1, 256, true));
        break;
      }
      case 113: {
        report_periodicity_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(report_periodicity.unpack(bref));
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
void ric_service_load_status_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_load_meas_id);
  j.end_obj();
  if (e2node_load_meas_id_present) {
    j.start_obj();
    j.write_int("id", 105);
    j.write_str("criticality", "ignore");
    j.write_int("value", e2node_load_meas_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 112);
  j.write_str("criticality", "ignore");
  j.write_str("value", regist_request.to_string());
  j.end_obj();
  if (ran_function_load_request_list_present) {
    j.start_obj();
    j.write_int("id", 111);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_function_load_request_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (report_periodicity_present) {
    j.start_obj();
    j.write_int("id", 113);
    j.write_str("criticality", "ignore");
    j.write_str("value", report_periodicity.to_string());
    j.end_obj();
  }
  j.end_array();
}

// RICserviceLoadStatusResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_service_load_status_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {117, 105, 109};
  return map_enum_number(names, 3, idx, "id");
}
bool ric_service_load_status_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {117, 105, 109};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_service_load_status_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 117:
      return crit_e::reject;
    case 105:
      return crit_e::ignore;
    case 109:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_service_load_status_resp_ies_o::value_c ric_service_load_status_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 117:
      ret.set(value_c::types::ric_load_meas_id);
      break;
    case 105:
      ret.set(value_c::types::e2node_load_meas_id);
      break;
    case 109:
      ret.set(value_c::types::ran_function_load_confirm_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_service_load_status_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 117:
      return presence_e::mandatory;
    case 105:
      return presence_e::mandatory;
    case 109:
      return presence_e::conditional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_service_load_status_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_load_meas_id:
      c = uint16_t{};
      break;
    case types::e2node_load_meas_id:
      c = uint16_t{};
      break;
    case types::ran_function_load_confirm_list:
      c = ran_function_load_confirm_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_resp_ies_o::value_c");
  }
}
uint16_t& ric_service_load_status_resp_ies_o::value_c::ric_load_meas_id()
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& ric_service_load_status_resp_ies_o::value_c::e2node_load_meas_id()
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
ran_function_load_confirm_list_l& ric_service_load_status_resp_ies_o::value_c::ran_function_load_confirm_list()
{
  assert_choice_type(types::ran_function_load_confirm_list, type_, "Value");
  return c.get<ran_function_load_confirm_list_l>();
}
const uint16_t& ric_service_load_status_resp_ies_o::value_c::ric_load_meas_id() const
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& ric_service_load_status_resp_ies_o::value_c::e2node_load_meas_id() const
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const ran_function_load_confirm_list_l&
ric_service_load_status_resp_ies_o::value_c::ran_function_load_confirm_list() const
{
  assert_choice_type(types::ran_function_load_confirm_list, type_, "Value");
  return c.get<ran_function_load_confirm_list_l>();
}
void ric_service_load_status_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ran_function_load_confirm_list:
      j.start_array("");
      for (const auto& e1 : c.get<ran_function_load_confirm_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_service_load_status_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::ran_function_load_confirm_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_function_load_confirm_list_l>(), 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_status_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::ran_function_load_confirm_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_function_load_confirm_list_l>(), bref, 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_status_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_service_load_status_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..4095,...)", "INTEGER (1..4095,...)", "RANfunctionLoadConfirm-List"};
  return convert_enum_idx(names, 3, value, "ric_service_load_status_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_service_load_status_resp_ies_o>;

OCUDUASN_CODE ric_service_load_status_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ran_function_load_confirm_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, e2node_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  if (ran_function_load_confirm_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)109, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_function_load_confirm_list, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_status_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_service_load_status_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 105: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(e2node_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 109: {
        ran_function_load_confirm_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_function_load_confirm_list, bref, 1, 256, true));
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
void ric_service_load_status_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 105);
  j.write_str("criticality", "ignore");
  j.write_int("value", e2node_load_meas_id);
  j.end_obj();
  if (ran_function_load_confirm_list_present) {
    j.start_obj();
    j.write_int("id", 109);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_function_load_confirm_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICserviceLoadUpdate-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_service_load_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {117, 105, 107};
  return map_enum_number(names, 3, idx, "id");
}
bool ric_service_load_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {117, 105, 107};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_service_load_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 117:
      return crit_e::reject;
    case 105:
      return crit_e::ignore;
    case 107:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_service_load_upd_ies_o::value_c ric_service_load_upd_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 117:
      ret.set(value_c::types::ric_load_meas_id);
      break;
    case 105:
      ret.set(value_c::types::e2node_load_meas_id);
      break;
    case 107:
      ret.set(value_c::types::ran_function_load_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_service_load_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 117:
      return presence_e::mandatory;
    case 105:
      return presence_e::mandatory;
    case 107:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_service_load_upd_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_load_meas_id:
      c = uint16_t{};
      break;
    case types::e2node_load_meas_id:
      c = uint16_t{};
      break;
    case types::ran_function_load_list:
      c = ran_function_load_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_upd_ies_o::value_c");
  }
}
uint16_t& ric_service_load_upd_ies_o::value_c::ric_load_meas_id()
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
uint16_t& ric_service_load_upd_ies_o::value_c::e2node_load_meas_id()
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
ran_function_load_list_l& ric_service_load_upd_ies_o::value_c::ran_function_load_list()
{
  assert_choice_type(types::ran_function_load_list, type_, "Value");
  return c.get<ran_function_load_list_l>();
}
const uint16_t& ric_service_load_upd_ies_o::value_c::ric_load_meas_id() const
{
  assert_choice_type(types::ric_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint16_t& ric_service_load_upd_ies_o::value_c::e2node_load_meas_id() const
{
  assert_choice_type(types::e2node_load_meas_id, type_, "Value");
  return c.get<uint16_t>();
}
const ran_function_load_list_l& ric_service_load_upd_ies_o::value_c::ran_function_load_list() const
{
  assert_choice_type(types::ran_function_load_list, type_, "Value");
  return c.get<ran_function_load_list_l>();
}
void ric_service_load_upd_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::e2node_load_meas_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ran_function_load_list:
      j.start_array("");
      for (const auto& e1 : c.get<ran_function_load_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_upd_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_service_load_upd_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::ran_function_load_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_function_load_list_l>(), 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_upd_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_upd_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::e2node_load_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)1u, (uint16_t)4095u, true, true));
      break;
    case types::ran_function_load_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_function_load_list_l>(), bref, 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_load_upd_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_service_load_upd_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..4095,...)", "INTEGER (1..4095,...)", "RANfunctionLoad-List"};
  return convert_enum_idx(names, 3, value, "ric_service_load_upd_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_service_load_upd_ies_o>;

OCUDUASN_CODE ric_service_load_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ric_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, e2node_load_meas_id, (uint16_t)1u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)107, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_function_load_list, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_load_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_service_load_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ric_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 105: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(e2node_load_meas_id, bref, (uint16_t)1u, (uint16_t)4095u, true, true));
        break;
      }
      case 107: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_function_load_list, bref, 1, 256, true));
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
void ric_service_load_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_int("value", ric_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 105);
  j.write_str("criticality", "ignore");
  j.write_int("value", e2node_load_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 107);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ran_function_load_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// RICserviceQuery-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_service_query_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 9};
  return map_enum_number(names, 2, idx, "id");
}
bool ric_service_query_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 9};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_service_query_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 9:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_service_query_ies_o::value_c ric_service_query_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 9:
      ret.set(value_c::types::ran_functions_accepted);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_service_query_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 9:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_service_query_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::ran_functions_accepted:
      c = ran_functions_id_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_query_ies_o::value_c");
  }
}
uint16_t& ric_service_query_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
ran_functions_id_list_l& ric_service_query_ies_o::value_c::ran_functions_accepted()
{
  assert_choice_type(types::ran_functions_accepted, type_, "Value");
  return c.get<ran_functions_id_list_l>();
}
const uint16_t& ric_service_query_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const ran_functions_id_list_l& ric_service_query_ies_o::value_c::ran_functions_accepted() const
{
  assert_choice_type(types::ran_functions_accepted, type_, "Value");
  return c.get<ran_functions_id_list_l>();
}
void ric_service_query_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ran_functions_accepted:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_id_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_query_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_service_query_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ran_functions_accepted:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_id_list_l>(), 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_query_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_query_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ran_functions_accepted:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_id_list_l>(), bref, 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_query_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_service_query_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "RANfunctionsID-List"};
  return convert_enum_idx(names, 2, value, "ric_service_query_ies_o::value_c::types");
}
uint8_t ric_service_query_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "ric_service_query_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_service_query_ies_o>;

OCUDUASN_CODE ric_service_query_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ran_functions_accepted_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ran_functions_accepted_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)9, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_accepted, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_query_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_service_query_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 9: {
        ran_functions_accepted_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_accepted, bref, 1, 256, true));
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
void ric_service_query_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ran_functions_accepted_present) {
    j.start_obj();
    j.write_int("id", 9);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_accepted) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICserviceUpdate-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_service_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 10, 12, 11};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_service_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 10, 12, 11};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_service_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 10:
      return crit_e::reject;
    case 12:
      return crit_e::reject;
    case 11:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_service_upd_ies_o::value_c ric_service_upd_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 10:
      ret.set(value_c::types::ran_functions_added);
      break;
    case 12:
      ret.set(value_c::types::ran_functions_modified);
      break;
    case 11:
      ret.set(value_c::types::ran_functions_deleted);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_service_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 12:
      return presence_e::optional;
    case 11:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_service_upd_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::ran_functions_added:
      c = ran_functions_list_l{};
      break;
    case types::ran_functions_modified:
      c = ran_functions_list_l{};
      break;
    case types::ran_functions_deleted:
      c = ran_functions_id_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_ies_o::value_c");
  }
}
uint16_t& ric_service_upd_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
ran_functions_list_l& ric_service_upd_ies_o::value_c::ran_functions_added()
{
  assert_choice_type(types::ran_functions_added, type_, "Value");
  return c.get<ran_functions_list_l>();
}
ran_functions_list_l& ric_service_upd_ies_o::value_c::ran_functions_modified()
{
  assert_choice_type(types::ran_functions_modified, type_, "Value");
  return c.get<ran_functions_list_l>();
}
ran_functions_id_list_l& ric_service_upd_ies_o::value_c::ran_functions_deleted()
{
  assert_choice_type(types::ran_functions_deleted, type_, "Value");
  return c.get<ran_functions_id_list_l>();
}
const uint16_t& ric_service_upd_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const ran_functions_list_l& ric_service_upd_ies_o::value_c::ran_functions_added() const
{
  assert_choice_type(types::ran_functions_added, type_, "Value");
  return c.get<ran_functions_list_l>();
}
const ran_functions_list_l& ric_service_upd_ies_o::value_c::ran_functions_modified() const
{
  assert_choice_type(types::ran_functions_modified, type_, "Value");
  return c.get<ran_functions_list_l>();
}
const ran_functions_id_list_l& ric_service_upd_ies_o::value_c::ran_functions_deleted() const
{
  assert_choice_type(types::ran_functions_deleted, type_, "Value");
  return c.get<ran_functions_id_list_l>();
}
void ric_service_upd_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ran_functions_added:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ran_functions_modified:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ran_functions_deleted:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_id_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_service_upd_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ran_functions_added:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_list_l>(), 1, 256, true));
      break;
    case types::ran_functions_modified:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_list_l>(), 1, 256, true));
      break;
    case types::ran_functions_deleted:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_id_list_l>(), 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_upd_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ran_functions_added:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_list_l>(), bref, 1, 256, true));
      break;
    case types::ran_functions_modified:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_list_l>(), bref, 1, 256, true));
      break;
    case types::ran_functions_deleted:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_id_list_l>(), bref, 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_service_upd_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "RANfunctions-List", "RANfunctions-List", "RANfunctionsID-List"};
  return convert_enum_idx(names, 4, value, "ric_service_upd_ies_o::value_c::types");
}
uint8_t ric_service_upd_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "ric_service_upd_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_service_upd_ies_o>;

OCUDUASN_CODE ric_service_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ran_functions_added_present ? 1 : 0;
  nof_ies += ran_functions_modified_present ? 1 : 0;
  nof_ies += ran_functions_deleted_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ran_functions_added_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)10, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_added, 1, 256, true));
  }
  if (ran_functions_modified_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)12, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_modified, 1, 256, true));
  }
  if (ran_functions_deleted_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)11, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_deleted, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_service_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 10: {
        ran_functions_added_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_added, bref, 1, 256, true));
        break;
      }
      case 12: {
        ran_functions_modified_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_modified, bref, 1, 256, true));
        break;
      }
      case 11: {
        ran_functions_deleted_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_deleted, bref, 1, 256, true));
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
void ric_service_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ran_functions_added_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_added) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ran_functions_modified_present) {
    j.start_obj();
    j.write_int("id", 12);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_modified) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ran_functions_deleted_present) {
    j.start_obj();
    j.write_int("id", 11);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_deleted) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICserviceUpdateAcknowledge-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_service_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 9, 13, 138};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_service_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 9, 13, 138};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_service_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 9:
      return crit_e::reject;
    case 13:
      return crit_e::reject;
    case 138:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_service_upd_ack_ies_o::value_c ric_service_upd_ack_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 9:
      ret.set(value_c::types::ran_functions_accepted);
      break;
    case 13:
      ret.set(value_c::types::ran_functions_rejected);
      break;
    case 138:
      ret.set(value_c::types::ran_functions_accept_modified_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_service_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 9:
      return presence_e::optional;
    case 13:
      return presence_e::optional;
    case 138:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_service_upd_ack_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::ran_functions_accepted:
      c = ran_functions_id_list_l{};
      break;
    case types::ran_functions_rejected:
      c = ran_functions_id_cause_list_l{};
      break;
    case types::ran_functions_accept_modified_list:
      c = ran_functions_accept_modified_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_ack_ies_o::value_c");
  }
}
uint16_t& ric_service_upd_ack_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
ran_functions_id_list_l& ric_service_upd_ack_ies_o::value_c::ran_functions_accepted()
{
  assert_choice_type(types::ran_functions_accepted, type_, "Value");
  return c.get<ran_functions_id_list_l>();
}
ran_functions_id_cause_list_l& ric_service_upd_ack_ies_o::value_c::ran_functions_rejected()
{
  assert_choice_type(types::ran_functions_rejected, type_, "Value");
  return c.get<ran_functions_id_cause_list_l>();
}
ran_functions_accept_modified_list_l& ric_service_upd_ack_ies_o::value_c::ran_functions_accept_modified_list()
{
  assert_choice_type(types::ran_functions_accept_modified_list, type_, "Value");
  return c.get<ran_functions_accept_modified_list_l>();
}
const uint16_t& ric_service_upd_ack_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const ran_functions_id_list_l& ric_service_upd_ack_ies_o::value_c::ran_functions_accepted() const
{
  assert_choice_type(types::ran_functions_accepted, type_, "Value");
  return c.get<ran_functions_id_list_l>();
}
const ran_functions_id_cause_list_l& ric_service_upd_ack_ies_o::value_c::ran_functions_rejected() const
{
  assert_choice_type(types::ran_functions_rejected, type_, "Value");
  return c.get<ran_functions_id_cause_list_l>();
}
const ran_functions_accept_modified_list_l&
ric_service_upd_ack_ies_o::value_c::ran_functions_accept_modified_list() const
{
  assert_choice_type(types::ran_functions_accept_modified_list, type_, "Value");
  return c.get<ran_functions_accept_modified_list_l>();
}
void ric_service_upd_ack_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ran_functions_accepted:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_id_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ran_functions_rejected:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_id_cause_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ran_functions_accept_modified_list:
      j.start_array("");
      for (const auto& e1 : c.get<ran_functions_accept_modified_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_ack_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_service_upd_ack_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ran_functions_accepted:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_id_list_l>(), 1, 256, true));
      break;
    case types::ran_functions_rejected:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_id_cause_list_l>(), 1, 256, true));
      break;
    case types::ran_functions_accept_modified_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_functions_accept_modified_list_l>(), 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_ack_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_upd_ack_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::ran_functions_accepted:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_id_list_l>(), bref, 1, 256, true));
      break;
    case types::ran_functions_rejected:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_id_cause_list_l>(), bref, 1, 256, true));
      break;
    case types::ran_functions_accept_modified_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_functions_accept_modified_list_l>(), bref, 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_ack_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_service_upd_ack_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (0..255,...)", "RANfunctionsID-List", "RANfunctionsIDcause-List", "RANfunctions-AcceptModified-List"};
  return convert_enum_idx(names, 4, value, "ric_service_upd_ack_ies_o::value_c::types");
}
uint8_t ric_service_upd_ack_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "ric_service_upd_ack_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_service_upd_ack_ies_o>;

OCUDUASN_CODE ric_service_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ran_functions_accepted_present ? 1 : 0;
  nof_ies += ran_functions_rejected_present ? 1 : 0;
  nof_ies += ran_functions_accept_modified_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ran_functions_accepted_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)9, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_accepted, 1, 256, true));
  }
  if (ran_functions_rejected_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)13, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_rejected, 1, 256, true));
  }
  if (ran_functions_accept_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)138, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_functions_accept_modified_list, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_service_upd_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 9: {
        ran_functions_accepted_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_accepted, bref, 1, 256, true));
        break;
      }
      case 13: {
        ran_functions_rejected_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_rejected, bref, 1, 256, true));
        break;
      }
      case 138: {
        ran_functions_accept_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_functions_accept_modified_list, bref, 1, 256, true));
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
void ric_service_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ran_functions_accepted_present) {
    j.start_obj();
    j.write_int("id", 9);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_accepted) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ran_functions_rejected_present) {
    j.start_obj();
    j.write_int("id", 13);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_rejected) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ran_functions_accept_modified_list_present) {
    j.start_obj();
    j.write_int("id", 138);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ran_functions_accept_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICserviceUpdateFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_service_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 1, 31, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_service_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 1, 31, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_service_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 49:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 31:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_service_upd_fail_ies_o::value_c ric_service_upd_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 31:
      ret.set(value_c::types::time_to_wait);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_service_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 31:
      return presence_e::optional;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_service_upd_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::time_to_wait:
      c = time_to_wait_e{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_fail_ies_o::value_c");
  }
}
uint16_t& ric_service_upd_fail_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& ric_service_upd_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
time_to_wait_e& ric_service_upd_fail_ies_o::value_c::time_to_wait()
{
  assert_choice_type(types::time_to_wait, type_, "Value");
  return c.get<time_to_wait_e>();
}
crit_diagnostics_s& ric_service_upd_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& ric_service_upd_fail_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& ric_service_upd_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const time_to_wait_e& ric_service_upd_fail_ies_o::value_c::time_to_wait() const
{
  assert_choice_type(types::time_to_wait, type_, "Value");
  return c.get<time_to_wait_e>();
}
const crit_diagnostics_s& ric_service_upd_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_service_upd_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::time_to_wait:
      j.write_str(c.get<time_to_wait_e>().to_string());
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_service_upd_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::time_to_wait:
      HANDLE_CODE(c.get<time_to_wait_e>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_upd_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::time_to_wait:
      HANDLE_CODE(c.get<time_to_wait_e>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_service_upd_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_service_upd_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "Cause", "TimeToWait", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "ric_service_upd_fail_ies_o::value_c::types");
}
uint8_t ric_service_upd_fail_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "ric_service_upd_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_service_upd_fail_ies_o>;

OCUDUASN_CODE ric_service_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)31, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_service_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_service_upd_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 31: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_service_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICsubscription-withCause-Item ::= SEQUENCE
OCUDUASN_CODE ric_sub_with_cause_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(ric_request_id.pack(bref));
  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(cause.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_with_cause_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(ric_request_id.unpack(bref));
  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(cause.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_with_cause_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ricRequestID");
  ric_request_id.to_json(j);
  j.write_int("ranFunctionID", ran_function_id);
  j.write_fieldname("cause");
  cause.to_json(j);
  j.end_obj();
}

// RICsubscription-withCause-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_with_cause_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {61};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_sub_with_cause_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 61 == id;
}
crit_e ric_sub_with_cause_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 61) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_sub_with_cause_item_ies_o::value_c ric_sub_with_cause_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 61) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_with_cause_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 61) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_with_cause_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_sub_with_cause_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_with_cause_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_with_cause_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICsubscription-withCause-Item"};
  return convert_enum_idx(names, 1, value, "ric_sub_with_cause_item_ies_o::value_c::types");
}

// RICsubscriptionAudit-Item ::= SEQUENCE
OCUDUASN_CODE ric_sub_audit_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(ric_request_id.pack(bref));
  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(ric_request_id.unpack(bref));
  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_audit_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ricRequestID");
  ric_request_id.to_json(j);
  j.write_int("ranFunctionID", ran_function_id);
  j.end_obj();
}

// RICsubscriptionAudit-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_audit_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {100};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_sub_audit_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 100 == id;
}
crit_e ric_sub_audit_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 100) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_sub_audit_item_ies_o::value_c ric_sub_audit_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 100) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_audit_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 100) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_audit_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_sub_audit_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_audit_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICsubscriptionAudit-Item"};
  return convert_enum_idx(names, 1, value, "ric_sub_audit_item_ies_o::value_c::types");
}

// RICsubscriptionAuditAction-Item ::= SEQUENCE
OCUDUASN_CODE ric_sub_audit_action_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(ric_request_id.pack(bref));
  HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(pack_dyn_seq_of(bref, ric_action_admitted_list, 1, 16, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_action_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(ric_request_id.unpack(bref));
  HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
  HANDLE_CODE(unpack_dyn_seq_of(ric_action_admitted_list, bref, 1, 16, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_audit_action_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("ricRequestID");
  ric_request_id.to_json(j);
  j.write_int("ranFunctionID", ran_function_id);
  j.start_array("ricAction-Admitted-List");
  for (const auto& e1 : ric_action_admitted_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// RICsubscriptionAuditAction-ItemIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_audit_action_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {101};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_sub_audit_action_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 101 == id;
}
crit_e ric_sub_audit_action_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 101) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_sub_audit_action_item_ies_o::value_c ric_sub_audit_action_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 101) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_audit_action_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 101) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_audit_action_item_ies_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ric_sub_audit_action_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_action_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_audit_action_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICsubscriptionAuditAction-Item"};
  return convert_enum_idx(names, 1, value, "ric_sub_audit_action_item_ies_o::value_c::types");
}

// RICsubscriptionAuditFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_audit_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 1, 2};
  return map_enum_number(names, 3, idx, "id");
}
bool ric_sub_audit_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_audit_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_audit_fail_ies_o::value_c ric_sub_audit_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_audit_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_audit_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_fail_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_audit_fail_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
cause_c& ric_sub_audit_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_sub_audit_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_sub_audit_fail_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const cause_c& ric_sub_audit_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_sub_audit_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_sub_audit_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_audit_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_audit_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 3, value, "ric_sub_audit_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_sub_audit_fail_ies_o>;

OCUDUASN_CODE ric_sub_audit_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_audit_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_sub_audit_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionAuditRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_audit_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 98, 99};
  return map_enum_number(names, 3, idx, "id");
}
bool ric_sub_audit_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 98, 99};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_audit_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 98:
      return crit_e::reject;
    case 99:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_audit_request_ies_o::value_c ric_sub_audit_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 98:
      ret.set(value_c::types::ric_sub_audit_flag);
      break;
    case 99:
      ret.set(value_c::types::ric_sub_audit_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_audit_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 98:
      return presence_e::optional;
    case 99:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_audit_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ric_sub_audit_flag:
      c = ric_sub_audit_flag_s{};
      break;
    case types::ric_sub_audit_list:
      c = ric_sub_audit_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_request_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_audit_request_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
ric_sub_audit_flag_s& ric_sub_audit_request_ies_o::value_c::ric_sub_audit_flag()
{
  assert_choice_type(types::ric_sub_audit_flag, type_, "Value");
  return c.get<ric_sub_audit_flag_s>();
}
ric_sub_audit_list_l& ric_sub_audit_request_ies_o::value_c::ric_sub_audit_list()
{
  assert_choice_type(types::ric_sub_audit_list, type_, "Value");
  return c.get<ric_sub_audit_list_l>();
}
const ric_request_id_s& ric_sub_audit_request_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const ric_sub_audit_flag_s& ric_sub_audit_request_ies_o::value_c::ric_sub_audit_flag() const
{
  assert_choice_type(types::ric_sub_audit_flag, type_, "Value");
  return c.get<ric_sub_audit_flag_s>();
}
const ric_sub_audit_list_l& ric_sub_audit_request_ies_o::value_c::ric_sub_audit_list() const
{
  assert_choice_type(types::ric_sub_audit_list, type_, "Value");
  return c.get<ric_sub_audit_list_l>();
}
void ric_sub_audit_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ric_sub_audit_flag:
      c.get<ric_sub_audit_flag_s>().to_json(j);
      break;
    case types::ric_sub_audit_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_sub_audit_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_audit_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ric_sub_audit_flag:
      HANDLE_CODE(c.get<ric_sub_audit_flag_s>().pack(bref));
      break;
    case types::ric_sub_audit_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_sub_audit_list_l>(), 1, 2147483648, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ric_sub_audit_flag:
      HANDLE_CODE(c.get<ric_sub_audit_flag_s>().unpack(bref));
      break;
    case types::ric_sub_audit_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_sub_audit_list_l>(), bref, 1, 2147483648, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_audit_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "RICsubscriptionAuditFlag", "RICsubscriptionAuditList"};
  return convert_enum_idx(names, 3, value, "ric_sub_audit_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_sub_audit_request_ies_o>;

OCUDUASN_CODE ric_sub_audit_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ric_sub_audit_flag_present ? 1 : 0;
  nof_ies += ric_sub_audit_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  if (ric_sub_audit_flag_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)98, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_sub_audit_flag.pack(bref));
  }
  if (ric_sub_audit_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)99, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_audit_list, 1, 2147483648, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_audit_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 98: {
        ric_sub_audit_flag_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_sub_audit_flag.unpack(bref));
        break;
      }
      case 99: {
        ric_sub_audit_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_sub_audit_list, bref, 1, 2147483648, true));
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
void ric_sub_audit_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  if (ric_sub_audit_flag_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ric_sub_audit_flag.to_json(j);
    j.end_obj();
  }
  if (ric_sub_audit_list_present) {
    j.start_obj();
    j.write_int("id", 99);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ric_sub_audit_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionAuditResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_audit_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 102, 104, 103};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_sub_audit_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 102, 104, 103};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_audit_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 102:
      return crit_e::reject;
    case 104:
      return crit_e::reject;
    case 103:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_audit_resp_ies_o::value_c ric_sub_audit_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 102:
      ret.set(value_c::types::ric_sub_audit_confirmed_list);
      break;
    case 104:
      ret.set(value_c::types::ric_sub_audit_unkown_list);
      break;
    case 103:
      ret.set(value_c::types::ric_sub_audit_missing_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_audit_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 102:
      return presence_e::mandatory;
    case 104:
      return presence_e::optional;
    case 103:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_audit_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ric_sub_audit_confirmed_list:
      c = ric_sub_audit_action_list_l{};
      break;
    case types::ric_sub_audit_unkown_list:
      c = ric_sub_audit_list_l{};
      break;
    case types::ric_sub_audit_missing_list:
      c = ric_sub_audit_action_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_resp_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_audit_resp_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
ric_sub_audit_action_list_l& ric_sub_audit_resp_ies_o::value_c::ric_sub_audit_confirmed_list()
{
  assert_choice_type(types::ric_sub_audit_confirmed_list, type_, "Value");
  return c.get<ric_sub_audit_action_list_l>();
}
ric_sub_audit_list_l& ric_sub_audit_resp_ies_o::value_c::ric_sub_audit_unkown_list()
{
  assert_choice_type(types::ric_sub_audit_unkown_list, type_, "Value");
  return c.get<ric_sub_audit_list_l>();
}
ric_sub_audit_action_list_l& ric_sub_audit_resp_ies_o::value_c::ric_sub_audit_missing_list()
{
  assert_choice_type(types::ric_sub_audit_missing_list, type_, "Value");
  return c.get<ric_sub_audit_action_list_l>();
}
const ric_request_id_s& ric_sub_audit_resp_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const ric_sub_audit_action_list_l& ric_sub_audit_resp_ies_o::value_c::ric_sub_audit_confirmed_list() const
{
  assert_choice_type(types::ric_sub_audit_confirmed_list, type_, "Value");
  return c.get<ric_sub_audit_action_list_l>();
}
const ric_sub_audit_list_l& ric_sub_audit_resp_ies_o::value_c::ric_sub_audit_unkown_list() const
{
  assert_choice_type(types::ric_sub_audit_unkown_list, type_, "Value");
  return c.get<ric_sub_audit_list_l>();
}
const ric_sub_audit_action_list_l& ric_sub_audit_resp_ies_o::value_c::ric_sub_audit_missing_list() const
{
  assert_choice_type(types::ric_sub_audit_missing_list, type_, "Value");
  return c.get<ric_sub_audit_action_list_l>();
}
void ric_sub_audit_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ric_sub_audit_confirmed_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_sub_audit_action_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_sub_audit_unkown_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_sub_audit_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_sub_audit_missing_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_sub_audit_action_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_audit_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ric_sub_audit_confirmed_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_sub_audit_action_list_l>(), 1, 2147483648, true));
      break;
    case types::ric_sub_audit_unkown_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_sub_audit_list_l>(), 1, 2147483648, true));
      break;
    case types::ric_sub_audit_missing_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_sub_audit_action_list_l>(), 1, 2147483648, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ric_sub_audit_confirmed_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_sub_audit_action_list_l>(), bref, 1, 2147483648, true));
      break;
    case types::ric_sub_audit_unkown_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_sub_audit_list_l>(), bref, 1, 2147483648, true));
      break;
    case types::ric_sub_audit_missing_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_sub_audit_action_list_l>(), bref, 1, 2147483648, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_audit_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_audit_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "RICrequestID", "RICsubscriptionAuditActionList", "RICsubscriptionAuditList", "RICsubscriptionAuditActionList"};
  return convert_enum_idx(names, 4, value, "ric_sub_audit_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_sub_audit_resp_ies_o>;

OCUDUASN_CODE ric_sub_audit_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ric_sub_audit_unkown_list_present ? 1 : 0;
  nof_ies += ric_sub_audit_missing_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)102, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_audit_confirmed_list, 1, 2147483648, true));
  }
  if (ric_sub_audit_unkown_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)104, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_audit_unkown_list, 1, 2147483648, true));
  }
  if (ric_sub_audit_missing_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)103, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_sub_audit_missing_list, 1, 2147483648, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_audit_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_audit_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 102: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_sub_audit_confirmed_list, bref, 1, 2147483648, true));
        break;
      }
      case 104: {
        ric_sub_audit_unkown_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_sub_audit_unkown_list, bref, 1, 2147483648, true));
        break;
      }
      case 103: {
        ric_sub_audit_missing_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_sub_audit_missing_list, bref, 1, 2147483648, true));
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
void ric_sub_audit_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 102);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ric_sub_audit_confirmed_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (ric_sub_audit_unkown_list_present) {
    j.start_obj();
    j.write_int("id", 104);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ric_sub_audit_unkown_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_sub_audit_missing_list_present) {
    j.start_obj();
    j.write_int("id", 103);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ric_sub_audit_missing_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionDeleteFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_delete_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_sub_delete_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_delete_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_delete_fail_ies_o::value_c ric_sub_delete_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_delete_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_delete_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_fail_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_delete_fail_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_delete_fail_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& ric_sub_delete_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_sub_delete_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_sub_delete_fail_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_delete_fail_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& ric_sub_delete_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_sub_delete_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_sub_delete_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_delete_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_delete_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_delete_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "ric_sub_delete_fail_ies_o::value_c::types");
}
uint8_t ric_sub_delete_fail_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_delete_fail_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_delete_fail_ies_o>;

OCUDUASN_CODE ric_sub_delete_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_delete_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_delete_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_sub_delete_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionDeleteRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_delete_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5};
  return map_enum_number(names, 2, idx, "id");
}
bool ric_sub_delete_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_delete_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_delete_request_ies_o::value_c ric_sub_delete_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_delete_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_delete_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_request_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_delete_request_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_delete_request_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const ric_request_id_s& ric_sub_delete_request_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_delete_request_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
void ric_sub_delete_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_delete_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_delete_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_delete_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)"};
  return convert_enum_idx(names, 2, value, "ric_sub_delete_request_ies_o::value_c::types");
}
uint8_t ric_sub_delete_request_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_delete_request_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_delete_request_ies_o>;

OCUDUASN_CODE ric_sub_delete_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_delete_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_delete_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
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
void ric_sub_delete_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.end_array();
}

// RICsubscriptionDeleteRequired-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_delete_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {60};
  return map_enum_number(names, 1, idx, "id");
}
bool ric_sub_delete_required_ies_o::is_id_valid(const uint32_t& id)
{
  return 60 == id;
}
crit_e ric_sub_delete_required_ies_o::get_crit(const uint32_t& id)
{
  if (id == 60) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ric_sub_delete_required_ies_o::value_c ric_sub_delete_required_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 60) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_delete_required_ies_o::get_presence(const uint32_t& id)
{
  if (id == 60) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_delete_required_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_array("");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
}
OCUDUASN_CODE ric_sub_delete_required_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 1024, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_delete_required_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 1024, true));
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_delete_required_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICsubscription-List-withCause"};
  return convert_enum_idx(names, 1, value, "ric_sub_delete_required_ies_o::value_c::types");
}

// RICsubscriptionDeleteResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_delete_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5};
  return map_enum_number(names, 2, idx, "id");
}
bool ric_sub_delete_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_delete_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_delete_resp_ies_o::value_c ric_sub_delete_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_delete_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_delete_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_resp_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_delete_resp_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_delete_resp_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const ric_request_id_s& ric_sub_delete_resp_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_delete_resp_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
void ric_sub_delete_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_delete_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_delete_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_delete_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_delete_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)"};
  return convert_enum_idx(names, 2, value, "ric_sub_delete_resp_ies_o::value_c::types");
}
uint8_t ric_sub_delete_resp_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_delete_resp_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_delete_resp_ies_o>;

OCUDUASN_CODE ric_sub_delete_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_delete_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_delete_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
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
void ric_sub_delete_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.end_array();
}

// RICsubscriptionDetails ::= SEQUENCE
OCUDUASN_CODE ric_sub_details_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(ric_event_trigger_definition.pack(bref));
  HANDLE_CODE(pack_dyn_seq_of(bref, ric_action_to_be_setup_list, 1, 16, true));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_details_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(ric_event_trigger_definition.unpack(bref));
  HANDLE_CODE(unpack_dyn_seq_of(ric_action_to_be_setup_list, bref, 1, 16, true));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ric_sub_details_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("ricEventTriggerDefinition", ric_event_trigger_definition.to_string());
  j.start_array("ricAction-ToBeSetup-List");
  for (const auto& e1 : ric_action_to_be_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// RICsubscriptionFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_sub_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_fail_ies_o::value_c ric_sub_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_fail_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_fail_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_fail_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& ric_sub_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_sub_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_sub_fail_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_fail_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& ric_sub_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_sub_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_sub_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "ric_sub_fail_ies_o::value_c::types");
}
uint8_t ric_sub_fail_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_fail_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_fail_ies_o>;

OCUDUASN_CODE ric_sub_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_sub_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionModificationConfirm-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_mod_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 87, 89, 91, 93};
  return map_enum_number(names, 6, idx, "id");
}
bool ric_sub_mod_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 87, 89, 91, 93};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_mod_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 87:
      return crit_e::ignore;
    case 89:
      return crit_e::ignore;
    case 91:
      return crit_e::ignore;
    case 93:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_mod_confirm_ies_o::value_c ric_sub_mod_confirm_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 87:
      ret.set(value_c::types::ric_actions_confirmed_for_mod_list);
      break;
    case 89:
      ret.set(value_c::types::ric_actions_refused_to_be_modified_list);
      break;
    case 91:
      ret.set(value_c::types::ric_actions_confirmed_for_removal_list);
      break;
    case 93:
      ret.set(value_c::types::ric_actions_refused_to_be_remd_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_mod_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 87:
      return presence_e::optional;
    case 89:
      return presence_e::optional;
    case 91:
      return presence_e::optional;
    case 93:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_mod_confirm_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_actions_confirmed_for_mod_list:
      c = ric_actions_confirmed_for_mod_list_l{};
      break;
    case types::ric_actions_refused_to_be_modified_list:
      c = ric_actions_refused_to_be_modified_list_l{};
      break;
    case types::ric_actions_confirmed_for_removal_list:
      c = ric_actions_confirmed_for_removal_list_l{};
      break;
    case types::ric_actions_refused_to_be_remd_list:
      c = ric_actions_refused_to_be_remd_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_confirm_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_mod_confirm_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_mod_confirm_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
ric_actions_confirmed_for_mod_list_l& ric_sub_mod_confirm_ies_o::value_c::ric_actions_confirmed_for_mod_list()
{
  assert_choice_type(types::ric_actions_confirmed_for_mod_list, type_, "Value");
  return c.get<ric_actions_confirmed_for_mod_list_l>();
}
ric_actions_refused_to_be_modified_list_l& ric_sub_mod_confirm_ies_o::value_c::ric_actions_refused_to_be_modified_list()
{
  assert_choice_type(types::ric_actions_refused_to_be_modified_list, type_, "Value");
  return c.get<ric_actions_refused_to_be_modified_list_l>();
}
ric_actions_confirmed_for_removal_list_l& ric_sub_mod_confirm_ies_o::value_c::ric_actions_confirmed_for_removal_list()
{
  assert_choice_type(types::ric_actions_confirmed_for_removal_list, type_, "Value");
  return c.get<ric_actions_confirmed_for_removal_list_l>();
}
ric_actions_refused_to_be_remd_list_l& ric_sub_mod_confirm_ies_o::value_c::ric_actions_refused_to_be_remd_list()
{
  assert_choice_type(types::ric_actions_refused_to_be_remd_list, type_, "Value");
  return c.get<ric_actions_refused_to_be_remd_list_l>();
}
const ric_request_id_s& ric_sub_mod_confirm_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_mod_confirm_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const ric_actions_confirmed_for_mod_list_l&
ric_sub_mod_confirm_ies_o::value_c::ric_actions_confirmed_for_mod_list() const
{
  assert_choice_type(types::ric_actions_confirmed_for_mod_list, type_, "Value");
  return c.get<ric_actions_confirmed_for_mod_list_l>();
}
const ric_actions_refused_to_be_modified_list_l&
ric_sub_mod_confirm_ies_o::value_c::ric_actions_refused_to_be_modified_list() const
{
  assert_choice_type(types::ric_actions_refused_to_be_modified_list, type_, "Value");
  return c.get<ric_actions_refused_to_be_modified_list_l>();
}
const ric_actions_confirmed_for_removal_list_l&
ric_sub_mod_confirm_ies_o::value_c::ric_actions_confirmed_for_removal_list() const
{
  assert_choice_type(types::ric_actions_confirmed_for_removal_list, type_, "Value");
  return c.get<ric_actions_confirmed_for_removal_list_l>();
}
const ric_actions_refused_to_be_remd_list_l&
ric_sub_mod_confirm_ies_o::value_c::ric_actions_refused_to_be_remd_list() const
{
  assert_choice_type(types::ric_actions_refused_to_be_remd_list, type_, "Value");
  return c.get<ric_actions_refused_to_be_remd_list_l>();
}
void ric_sub_mod_confirm_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_actions_confirmed_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_confirmed_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_refused_to_be_modified_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_refused_to_be_modified_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_confirmed_for_removal_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_confirmed_for_removal_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_refused_to_be_remd_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_refused_to_be_remd_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_confirm_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_mod_confirm_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_actions_confirmed_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_confirmed_for_mod_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_refused_to_be_modified_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_refused_to_be_modified_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_confirmed_for_removal_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_confirmed_for_removal_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_refused_to_be_remd_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_refused_to_be_remd_list_l>(), 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_confirm_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_confirm_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_actions_confirmed_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_confirmed_for_mod_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_refused_to_be_modified_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_refused_to_be_modified_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_confirmed_for_removal_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_confirmed_for_removal_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_refused_to_be_remd_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_refused_to_be_remd_list_l>(), bref, 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_confirm_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_mod_confirm_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID",
                                "INTEGER (0..4095)",
                                "RICactions-ConfirmedForModification-List",
                                "RICactions-RefusedToBeModified-List",
                                "RICactions-ConfirmedForRemoval-List",
                                "RICactions-RefusedToBeRemoved-List"};
  return convert_enum_idx(names, 6, value, "ric_sub_mod_confirm_ies_o::value_c::types");
}
uint8_t ric_sub_mod_confirm_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_mod_confirm_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_mod_confirm_ies_o>;

OCUDUASN_CODE ric_sub_mod_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ric_actions_confirmed_for_mod_list_present ? 1 : 0;
  nof_ies += ric_actions_refused_to_be_modified_list_present ? 1 : 0;
  nof_ies += ric_actions_confirmed_for_removal_list_present ? 1 : 0;
  nof_ies += ric_actions_refused_to_be_remd_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  if (ric_actions_confirmed_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)87, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_confirmed_for_mod_list, 0, 16, true));
  }
  if (ric_actions_refused_to_be_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)89, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_refused_to_be_modified_list, 0, 16, true));
  }
  if (ric_actions_confirmed_for_removal_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)91, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_confirmed_for_removal_list, 0, 16, true));
  }
  if (ric_actions_refused_to_be_remd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)93, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_refused_to_be_remd_list, 0, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_mod_confirm_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 87: {
        ric_actions_confirmed_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_confirmed_for_mod_list, bref, 0, 16, true));
        break;
      }
      case 89: {
        ric_actions_refused_to_be_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_refused_to_be_modified_list, bref, 0, 16, true));
        break;
      }
      case 91: {
        ric_actions_confirmed_for_removal_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_confirmed_for_removal_list, bref, 0, 16, true));
        break;
      }
      case 93: {
        ric_actions_refused_to_be_remd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_refused_to_be_remd_list, bref, 0, 16, true));
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
void ric_sub_mod_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  if (ric_actions_confirmed_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 87);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_confirmed_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_refused_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 89);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_refused_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_confirmed_for_removal_list_present) {
    j.start_obj();
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_confirmed_for_removal_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_refused_to_be_remd_list_present) {
    j.start_obj();
    j.write_int("id", 93);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_refused_to_be_remd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionModificationFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_sub_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_mod_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_mod_fail_ies_o::value_c ric_sub_mod_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_mod_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_mod_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_fail_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_mod_fail_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_mod_fail_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& ric_sub_mod_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_sub_mod_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_sub_mod_fail_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_mod_fail_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& ric_sub_mod_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_sub_mod_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_sub_mod_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_mod_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_mod_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "ric_sub_mod_fail_ies_o::value_c::types");
}
uint8_t ric_sub_mod_fail_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_mod_fail_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_mod_fail_ies_o>;

OCUDUASN_CODE ric_sub_mod_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_mod_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_sub_mod_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionModificationRefuse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_mod_refuse_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_sub_mod_refuse_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_mod_refuse_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_mod_refuse_ies_o::value_c ric_sub_mod_refuse_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_mod_refuse_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_mod_refuse_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_refuse_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_mod_refuse_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_mod_refuse_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& ric_sub_mod_refuse_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_sub_mod_refuse_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_sub_mod_refuse_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_mod_refuse_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& ric_sub_mod_refuse_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_sub_mod_refuse_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_sub_mod_refuse_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_refuse_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_mod_refuse_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_refuse_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_refuse_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_refuse_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_mod_refuse_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "INTEGER (0..4095)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "ric_sub_mod_refuse_ies_o::value_c::types");
}
uint8_t ric_sub_mod_refuse_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_mod_refuse_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_mod_refuse_ies_o>;

OCUDUASN_CODE ric_sub_mod_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_refuse_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_mod_refuse_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_sub_mod_refuse_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionModificationRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 64, 65, 67, 69};
  return map_enum_number(names, 6, idx, "id");
}
bool ric_sub_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 64, 65, 67, 69};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 64:
      return crit_e::ignore;
    case 65:
      return crit_e::ignore;
    case 67:
      return crit_e::ignore;
    case 69:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_mod_request_ies_o::value_c ric_sub_mod_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 64:
      ret.set(value_c::types::ric_event_trigger_definition_to_be_modified);
      break;
    case 65:
      ret.set(value_c::types::ric_actions_to_be_remd_for_mod_list);
      break;
    case 67:
      ret.set(value_c::types::ric_actions_to_be_modified_for_mod_list);
      break;
    case 69:
      ret.set(value_c::types::ric_actions_to_be_added_for_mod_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 64:
      return presence_e::optional;
    case 65:
      return presence_e::optional;
    case 67:
      return presence_e::optional;
    case 69:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_mod_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_event_trigger_definition_to_be_modified:
      c = unbounded_octstring<true>{};
      break;
    case types::ric_actions_to_be_remd_for_mod_list:
      c = ric_actions_to_be_remd_for_mod_list_l{};
      break;
    case types::ric_actions_to_be_modified_for_mod_list:
      c = ric_actions_to_be_modified_for_mod_list_l{};
      break;
    case types::ric_actions_to_be_added_for_mod_list:
      c = ric_actions_to_be_added_for_mod_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_request_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_mod_request_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_mod_request_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
unbounded_octstring<true>& ric_sub_mod_request_ies_o::value_c::ric_event_trigger_definition_to_be_modified()
{
  assert_choice_type(types::ric_event_trigger_definition_to_be_modified, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
ric_actions_to_be_remd_for_mod_list_l& ric_sub_mod_request_ies_o::value_c::ric_actions_to_be_remd_for_mod_list()
{
  assert_choice_type(types::ric_actions_to_be_remd_for_mod_list, type_, "Value");
  return c.get<ric_actions_to_be_remd_for_mod_list_l>();
}
ric_actions_to_be_modified_for_mod_list_l& ric_sub_mod_request_ies_o::value_c::ric_actions_to_be_modified_for_mod_list()
{
  assert_choice_type(types::ric_actions_to_be_modified_for_mod_list, type_, "Value");
  return c.get<ric_actions_to_be_modified_for_mod_list_l>();
}
ric_actions_to_be_added_for_mod_list_l& ric_sub_mod_request_ies_o::value_c::ric_actions_to_be_added_for_mod_list()
{
  assert_choice_type(types::ric_actions_to_be_added_for_mod_list, type_, "Value");
  return c.get<ric_actions_to_be_added_for_mod_list_l>();
}
const ric_request_id_s& ric_sub_mod_request_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_mod_request_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const unbounded_octstring<true>& ric_sub_mod_request_ies_o::value_c::ric_event_trigger_definition_to_be_modified() const
{
  assert_choice_type(types::ric_event_trigger_definition_to_be_modified, type_, "Value");
  return c.get<unbounded_octstring<true>>();
}
const ric_actions_to_be_remd_for_mod_list_l&
ric_sub_mod_request_ies_o::value_c::ric_actions_to_be_remd_for_mod_list() const
{
  assert_choice_type(types::ric_actions_to_be_remd_for_mod_list, type_, "Value");
  return c.get<ric_actions_to_be_remd_for_mod_list_l>();
}
const ric_actions_to_be_modified_for_mod_list_l&
ric_sub_mod_request_ies_o::value_c::ric_actions_to_be_modified_for_mod_list() const
{
  assert_choice_type(types::ric_actions_to_be_modified_for_mod_list, type_, "Value");
  return c.get<ric_actions_to_be_modified_for_mod_list_l>();
}
const ric_actions_to_be_added_for_mod_list_l&
ric_sub_mod_request_ies_o::value_c::ric_actions_to_be_added_for_mod_list() const
{
  assert_choice_type(types::ric_actions_to_be_added_for_mod_list, type_, "Value");
  return c.get<ric_actions_to_be_added_for_mod_list_l>();
}
void ric_sub_mod_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_event_trigger_definition_to_be_modified:
      j.write_str(c.get<unbounded_octstring<true>>().to_string());
      break;
    case types::ric_actions_to_be_remd_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_to_be_remd_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_to_be_modified_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_to_be_modified_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_to_be_added_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_to_be_added_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_mod_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_event_trigger_definition_to_be_modified:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().pack(bref));
      break;
    case types::ric_actions_to_be_remd_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_to_be_remd_for_mod_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_to_be_modified_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_to_be_modified_for_mod_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_to_be_added_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_to_be_added_for_mod_list_l>(), 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_event_trigger_definition_to_be_modified:
      HANDLE_CODE(c.get<unbounded_octstring<true>>().unpack(bref));
      break;
    case types::ric_actions_to_be_remd_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_to_be_remd_for_mod_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_to_be_modified_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_to_be_modified_for_mod_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_to_be_added_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_to_be_added_for_mod_list_l>(), bref, 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_mod_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID",
                                "INTEGER (0..4095)",
                                "OCTET STRING",
                                "RICactions-ToBeRemovedForModification-List",
                                "RICactions-ToBeModifiedForModification-List",
                                "RICactions-ToBeAddedForModification-List"};
  return convert_enum_idx(names, 6, value, "ric_sub_mod_request_ies_o::value_c::types");
}
uint8_t ric_sub_mod_request_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_mod_request_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_mod_request_ies_o>;

OCUDUASN_CODE ric_sub_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ric_event_trigger_definition_to_be_modified_present ? 1 : 0;
  nof_ies += ric_actions_to_be_remd_for_mod_list_present ? 1 : 0;
  nof_ies += ric_actions_to_be_modified_for_mod_list_present ? 1 : 0;
  nof_ies += ric_actions_to_be_added_for_mod_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  if (ric_event_trigger_definition_to_be_modified_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)64, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_event_trigger_definition_to_be_modified.pack(bref));
  }
  if (ric_actions_to_be_remd_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)65, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_to_be_remd_for_mod_list, 0, 16, true));
  }
  if (ric_actions_to_be_modified_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)67, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_to_be_modified_for_mod_list, 0, 16, true));
  }
  if (ric_actions_to_be_added_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)69, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_to_be_added_for_mod_list, 0, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_mod_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 64: {
        ric_event_trigger_definition_to_be_modified_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_event_trigger_definition_to_be_modified.unpack(bref));
        break;
      }
      case 65: {
        ric_actions_to_be_remd_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_to_be_remd_for_mod_list, bref, 0, 16, true));
        break;
      }
      case 67: {
        ric_actions_to_be_modified_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_to_be_modified_for_mod_list, bref, 0, 16, true));
        break;
      }
      case 69: {
        ric_actions_to_be_added_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_to_be_added_for_mod_list, bref, 0, 16, true));
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
void ric_sub_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  if (ric_event_trigger_definition_to_be_modified_present) {
    j.start_obj();
    j.write_int("id", 64);
    j.write_str("criticality", "ignore");
    j.write_str("value", ric_event_trigger_definition_to_be_modified.to_string());
    j.end_obj();
  }
  if (ric_actions_to_be_remd_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 65);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_to_be_remd_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_to_be_modified_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 67);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_to_be_modified_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_to_be_added_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 69);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_to_be_added_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionModificationRequired-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_mod_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 83, 85};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_sub_mod_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 83, 85};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_mod_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 83:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_mod_required_ies_o::value_c ric_sub_mod_required_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 83:
      ret.set(value_c::types::ric_actions_required_to_be_modified_list);
      break;
    case 85:
      ret.set(value_c::types::ric_actions_required_to_be_remd_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_mod_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 83:
      return presence_e::optional;
    case 85:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_mod_required_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_actions_required_to_be_modified_list:
      c = ric_actions_required_to_be_modified_list_l{};
      break;
    case types::ric_actions_required_to_be_remd_list:
      c = ric_actions_required_to_be_remd_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_required_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_mod_required_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_mod_required_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
ric_actions_required_to_be_modified_list_l&
ric_sub_mod_required_ies_o::value_c::ric_actions_required_to_be_modified_list()
{
  assert_choice_type(types::ric_actions_required_to_be_modified_list, type_, "Value");
  return c.get<ric_actions_required_to_be_modified_list_l>();
}
ric_actions_required_to_be_remd_list_l& ric_sub_mod_required_ies_o::value_c::ric_actions_required_to_be_remd_list()
{
  assert_choice_type(types::ric_actions_required_to_be_remd_list, type_, "Value");
  return c.get<ric_actions_required_to_be_remd_list_l>();
}
const ric_request_id_s& ric_sub_mod_required_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_mod_required_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const ric_actions_required_to_be_modified_list_l&
ric_sub_mod_required_ies_o::value_c::ric_actions_required_to_be_modified_list() const
{
  assert_choice_type(types::ric_actions_required_to_be_modified_list, type_, "Value");
  return c.get<ric_actions_required_to_be_modified_list_l>();
}
const ric_actions_required_to_be_remd_list_l&
ric_sub_mod_required_ies_o::value_c::ric_actions_required_to_be_remd_list() const
{
  assert_choice_type(types::ric_actions_required_to_be_remd_list, type_, "Value");
  return c.get<ric_actions_required_to_be_remd_list_l>();
}
void ric_sub_mod_required_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_actions_required_to_be_modified_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_required_to_be_modified_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_required_to_be_remd_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_required_to_be_remd_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_required_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_mod_required_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_actions_required_to_be_modified_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_required_to_be_modified_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_required_to_be_remd_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_required_to_be_remd_list_l>(), 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_required_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_required_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_actions_required_to_be_modified_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_required_to_be_modified_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_required_to_be_remd_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_required_to_be_remd_list_l>(), bref, 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_required_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_mod_required_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID",
                                "INTEGER (0..4095)",
                                "RICactions-RequiredToBeModified-List",
                                "RICactions-RequiredToBeRemoved-List"};
  return convert_enum_idx(names, 4, value, "ric_sub_mod_required_ies_o::value_c::types");
}
uint8_t ric_sub_mod_required_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_mod_required_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_mod_required_ies_o>;

OCUDUASN_CODE ric_sub_mod_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ric_actions_required_to_be_modified_list_present ? 1 : 0;
  nof_ies += ric_actions_required_to_be_remd_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  if (ric_actions_required_to_be_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)83, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_required_to_be_modified_list, 0, 16, true));
  }
  if (ric_actions_required_to_be_remd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)85, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_required_to_be_remd_list, 0, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_mod_required_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 83: {
        ric_actions_required_to_be_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_required_to_be_modified_list, bref, 0, 16, true));
        break;
      }
      case 85: {
        ric_actions_required_to_be_remd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_required_to_be_remd_list, bref, 0, 16, true));
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
void ric_sub_mod_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  if (ric_actions_required_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_required_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_required_to_be_remd_list_present) {
    j.start_obj();
    j.write_int("id", 85);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_required_to_be_remd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionModificationResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 71, 73, 75, 77, 79, 81};
  return map_enum_number(names, 8, idx, "id");
}
bool ric_sub_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 71, 73, 75, 77, 79, 81};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 71:
      return crit_e::ignore;
    case 73:
      return crit_e::ignore;
    case 75:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 79:
      return crit_e::ignore;
    case 81:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_mod_resp_ies_o::value_c ric_sub_mod_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 71:
      ret.set(value_c::types::ric_actions_remd_for_mod_list);
      break;
    case 73:
      ret.set(value_c::types::ric_actions_failed_to_be_remd_for_mod_list);
      break;
    case 75:
      ret.set(value_c::types::ric_actions_modified_for_mod_list);
      break;
    case 77:
      ret.set(value_c::types::ric_actions_failed_to_be_modified_for_mod_list);
      break;
    case 79:
      ret.set(value_c::types::ric_actions_added_for_mod_list);
      break;
    case 81:
      ret.set(value_c::types::ric_actions_failed_to_be_added_for_mod_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 71:
      return presence_e::optional;
    case 73:
      return presence_e::optional;
    case 75:
      return presence_e::optional;
    case 77:
      return presence_e::optional;
    case 79:
      return presence_e::optional;
    case 81:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_mod_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_actions_remd_for_mod_list:
      c = ric_actions_remd_for_mod_list_l{};
      break;
    case types::ric_actions_failed_to_be_remd_for_mod_list:
      c = ric_actions_failed_to_be_remd_for_mod_list_l{};
      break;
    case types::ric_actions_modified_for_mod_list:
      c = ric_actions_modified_for_mod_list_l{};
      break;
    case types::ric_actions_failed_to_be_modified_for_mod_list:
      c = ric_actions_failed_to_be_modified_for_mod_list_l{};
      break;
    case types::ric_actions_added_for_mod_list:
      c = ric_actions_added_for_mod_list_l{};
      break;
    case types::ric_actions_failed_to_be_added_for_mod_list:
      c = ric_actions_failed_to_be_added_for_mod_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_resp_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_mod_resp_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_mod_resp_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
ric_actions_remd_for_mod_list_l& ric_sub_mod_resp_ies_o::value_c::ric_actions_remd_for_mod_list()
{
  assert_choice_type(types::ric_actions_remd_for_mod_list, type_, "Value");
  return c.get<ric_actions_remd_for_mod_list_l>();
}
ric_actions_failed_to_be_remd_for_mod_list_l&
ric_sub_mod_resp_ies_o::value_c::ric_actions_failed_to_be_remd_for_mod_list()
{
  assert_choice_type(types::ric_actions_failed_to_be_remd_for_mod_list, type_, "Value");
  return c.get<ric_actions_failed_to_be_remd_for_mod_list_l>();
}
ric_actions_modified_for_mod_list_l& ric_sub_mod_resp_ies_o::value_c::ric_actions_modified_for_mod_list()
{
  assert_choice_type(types::ric_actions_modified_for_mod_list, type_, "Value");
  return c.get<ric_actions_modified_for_mod_list_l>();
}
ric_actions_failed_to_be_modified_for_mod_list_l&
ric_sub_mod_resp_ies_o::value_c::ric_actions_failed_to_be_modified_for_mod_list()
{
  assert_choice_type(types::ric_actions_failed_to_be_modified_for_mod_list, type_, "Value");
  return c.get<ric_actions_failed_to_be_modified_for_mod_list_l>();
}
ric_actions_added_for_mod_list_l& ric_sub_mod_resp_ies_o::value_c::ric_actions_added_for_mod_list()
{
  assert_choice_type(types::ric_actions_added_for_mod_list, type_, "Value");
  return c.get<ric_actions_added_for_mod_list_l>();
}
ric_actions_failed_to_be_added_for_mod_list_l&
ric_sub_mod_resp_ies_o::value_c::ric_actions_failed_to_be_added_for_mod_list()
{
  assert_choice_type(types::ric_actions_failed_to_be_added_for_mod_list, type_, "Value");
  return c.get<ric_actions_failed_to_be_added_for_mod_list_l>();
}
const ric_request_id_s& ric_sub_mod_resp_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_mod_resp_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const ric_actions_remd_for_mod_list_l& ric_sub_mod_resp_ies_o::value_c::ric_actions_remd_for_mod_list() const
{
  assert_choice_type(types::ric_actions_remd_for_mod_list, type_, "Value");
  return c.get<ric_actions_remd_for_mod_list_l>();
}
const ric_actions_failed_to_be_remd_for_mod_list_l&
ric_sub_mod_resp_ies_o::value_c::ric_actions_failed_to_be_remd_for_mod_list() const
{
  assert_choice_type(types::ric_actions_failed_to_be_remd_for_mod_list, type_, "Value");
  return c.get<ric_actions_failed_to_be_remd_for_mod_list_l>();
}
const ric_actions_modified_for_mod_list_l& ric_sub_mod_resp_ies_o::value_c::ric_actions_modified_for_mod_list() const
{
  assert_choice_type(types::ric_actions_modified_for_mod_list, type_, "Value");
  return c.get<ric_actions_modified_for_mod_list_l>();
}
const ric_actions_failed_to_be_modified_for_mod_list_l&
ric_sub_mod_resp_ies_o::value_c::ric_actions_failed_to_be_modified_for_mod_list() const
{
  assert_choice_type(types::ric_actions_failed_to_be_modified_for_mod_list, type_, "Value");
  return c.get<ric_actions_failed_to_be_modified_for_mod_list_l>();
}
const ric_actions_added_for_mod_list_l& ric_sub_mod_resp_ies_o::value_c::ric_actions_added_for_mod_list() const
{
  assert_choice_type(types::ric_actions_added_for_mod_list, type_, "Value");
  return c.get<ric_actions_added_for_mod_list_l>();
}
const ric_actions_failed_to_be_added_for_mod_list_l&
ric_sub_mod_resp_ies_o::value_c::ric_actions_failed_to_be_added_for_mod_list() const
{
  assert_choice_type(types::ric_actions_failed_to_be_added_for_mod_list, type_, "Value");
  return c.get<ric_actions_failed_to_be_added_for_mod_list_l>();
}
void ric_sub_mod_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_actions_remd_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_remd_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_failed_to_be_remd_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_failed_to_be_remd_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_modified_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_modified_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_failed_to_be_modified_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_failed_to_be_modified_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_added_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_added_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_failed_to_be_added_for_mod_list:
      j.start_array("");
      for (const auto& e1 : c.get<ric_actions_failed_to_be_added_for_mod_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_mod_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_actions_remd_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_remd_for_mod_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_failed_to_be_remd_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_failed_to_be_remd_for_mod_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_modified_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_modified_for_mod_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_failed_to_be_modified_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_failed_to_be_modified_for_mod_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_added_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_added_for_mod_list_l>(), 0, 16, true));
      break;
    case types::ric_actions_failed_to_be_added_for_mod_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_actions_failed_to_be_added_for_mod_list_l>(), 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_actions_remd_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_remd_for_mod_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_failed_to_be_remd_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_failed_to_be_remd_for_mod_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_modified_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_modified_for_mod_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_failed_to_be_modified_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_failed_to_be_modified_for_mod_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_added_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_added_for_mod_list_l>(), bref, 0, 16, true));
      break;
    case types::ric_actions_failed_to_be_added_for_mod_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_actions_failed_to_be_added_for_mod_list_l>(), bref, 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_mod_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_mod_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID",
                                "INTEGER (0..4095)",
                                "RICactions-RemovedForModification-List",
                                "RICactions-FailedToBeRemovedForModification-List",
                                "RICactions-ModifiedForModification-List",
                                "RICactions-FailedToBeModifiedForModification-List",
                                "RICactions-AddedForModification-List",
                                "RICactions-FailedToBeAddedForModification-List"};
  return convert_enum_idx(names, 8, value, "ric_sub_mod_resp_ies_o::value_c::types");
}
uint8_t ric_sub_mod_resp_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_mod_resp_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_mod_resp_ies_o>;

OCUDUASN_CODE ric_sub_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ric_actions_remd_for_mod_list_present ? 1 : 0;
  nof_ies += ric_actions_failed_to_be_remd_for_mod_list_present ? 1 : 0;
  nof_ies += ric_actions_modified_for_mod_list_present ? 1 : 0;
  nof_ies += ric_actions_failed_to_be_modified_for_mod_list_present ? 1 : 0;
  nof_ies += ric_actions_added_for_mod_list_present ? 1 : 0;
  nof_ies += ric_actions_failed_to_be_added_for_mod_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  if (ric_actions_remd_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)71, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_remd_for_mod_list, 0, 16, true));
  }
  if (ric_actions_failed_to_be_remd_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)73, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_failed_to_be_remd_for_mod_list, 0, 16, true));
  }
  if (ric_actions_modified_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)75, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_modified_for_mod_list, 0, 16, true));
  }
  if (ric_actions_failed_to_be_modified_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_failed_to_be_modified_for_mod_list, 0, 16, true));
  }
  if (ric_actions_added_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)79, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_added_for_mod_list, 0, 16, true));
  }
  if (ric_actions_failed_to_be_added_for_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)81, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_failed_to_be_added_for_mod_list, 0, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_mod_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 71: {
        ric_actions_remd_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_remd_for_mod_list, bref, 0, 16, true));
        break;
      }
      case 73: {
        ric_actions_failed_to_be_remd_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_failed_to_be_remd_for_mod_list, bref, 0, 16, true));
        break;
      }
      case 75: {
        ric_actions_modified_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_modified_for_mod_list, bref, 0, 16, true));
        break;
      }
      case 77: {
        ric_actions_failed_to_be_modified_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_failed_to_be_modified_for_mod_list, bref, 0, 16, true));
        break;
      }
      case 79: {
        ric_actions_added_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_added_for_mod_list, bref, 0, 16, true));
        break;
      }
      case 81: {
        ric_actions_failed_to_be_added_for_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_failed_to_be_added_for_mod_list, bref, 0, 16, true));
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
void ric_sub_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  if (ric_actions_remd_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 71);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_remd_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_failed_to_be_remd_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 73);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_failed_to_be_remd_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_modified_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 75);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_modified_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_failed_to_be_modified_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_failed_to_be_modified_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_added_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 79);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_added_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ric_actions_failed_to_be_added_for_mod_list_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ric_actions_failed_to_be_added_for_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 30, 62, 63};
  return map_enum_number(names, 5, idx, "id");
}
bool ric_sub_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 30, 62, 63};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 30:
      return crit_e::reject;
    case 62:
      return crit_e::reject;
    case 63:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_request_ies_o::value_c ric_sub_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 30:
      ret.set(value_c::types::ric_sub_details);
      break;
    case 62:
      ret.set(value_c::types::ric_sub_start_time);
      break;
    case 63:
      ret.set(value_c::types::ric_sub_end_time);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 30:
      return presence_e::mandatory;
    case 62:
      return presence_e::optional;
    case 63:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_sub_details:
      c = ric_sub_details_s{};
      break;
    case types::ric_sub_start_time:
      c = fixed_octstring<8, true>{};
      break;
    case types::ric_sub_end_time:
      c = fixed_octstring<8, true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_request_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_request_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_request_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
ric_sub_details_s& ric_sub_request_ies_o::value_c::ric_sub_details()
{
  assert_choice_type(types::ric_sub_details, type_, "Value");
  return c.get<ric_sub_details_s>();
}
fixed_octstring<8, true>& ric_sub_request_ies_o::value_c::ric_sub_start_time()
{
  assert_choice_type(types::ric_sub_start_time, type_, "Value");
  return c.get<fixed_octstring<8, true>>();
}
fixed_octstring<8, true>& ric_sub_request_ies_o::value_c::ric_sub_end_time()
{
  assert_choice_type(types::ric_sub_end_time, type_, "Value");
  return c.get<fixed_octstring<8, true>>();
}
const ric_request_id_s& ric_sub_request_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_request_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const ric_sub_details_s& ric_sub_request_ies_o::value_c::ric_sub_details() const
{
  assert_choice_type(types::ric_sub_details, type_, "Value");
  return c.get<ric_sub_details_s>();
}
const fixed_octstring<8, true>& ric_sub_request_ies_o::value_c::ric_sub_start_time() const
{
  assert_choice_type(types::ric_sub_start_time, type_, "Value");
  return c.get<fixed_octstring<8, true>>();
}
const fixed_octstring<8, true>& ric_sub_request_ies_o::value_c::ric_sub_end_time() const
{
  assert_choice_type(types::ric_sub_end_time, type_, "Value");
  return c.get<fixed_octstring<8, true>>();
}
void ric_sub_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_sub_details:
      c.get<ric_sub_details_s>().to_json(j);
      break;
    case types::ric_sub_start_time:
      j.write_str(c.get<fixed_octstring<8, true>>().to_string());
      break;
    case types::ric_sub_end_time:
      j.write_str(c.get<fixed_octstring<8, true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_sub_details:
      HANDLE_CODE(c.get<ric_sub_details_s>().pack(bref));
      break;
    case types::ric_sub_start_time:
      HANDLE_CODE((c.get<fixed_octstring<8, true>>().pack(bref)));
      break;
    case types::ric_sub_end_time:
      HANDLE_CODE((c.get<fixed_octstring<8, true>>().pack(bref)));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_sub_details:
      HANDLE_CODE(c.get<ric_sub_details_s>().unpack(bref));
      break;
    case types::ric_sub_start_time:
      HANDLE_CODE((c.get<fixed_octstring<8, true>>().unpack(bref)));
      break;
    case types::ric_sub_end_time:
      HANDLE_CODE((c.get<fixed_octstring<8, true>>().unpack(bref)));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "RICrequestID", "INTEGER (0..4095)", "RICsubscriptionDetails", "OCTET STRING", "OCTET STRING"};
  return convert_enum_idx(names, 5, value, "ric_sub_request_ies_o::value_c::types");
}
uint8_t ric_sub_request_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_request_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_request_ies_o>;

OCUDUASN_CODE ric_sub_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ric_sub_start_time_present ? 1 : 0;
  nof_ies += ric_sub_end_time_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)30, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_sub_details.pack(bref));
  }
  if (ric_sub_start_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)62, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_sub_start_time.pack(bref));
  }
  if (ric_sub_end_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)63, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_sub_end_time.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 30: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_sub_details.unpack(bref));
        break;
      }
      case 62: {
        ric_sub_start_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_sub_start_time.unpack(bref));
        break;
      }
      case 63: {
        ric_sub_end_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_sub_end_time.unpack(bref));
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
void ric_sub_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 30);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_sub_details.to_json(j);
  j.end_obj();
  if (ric_sub_start_time_present) {
    j.start_obj();
    j.write_int("id", 62);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_sub_start_time.to_string());
    j.end_obj();
  }
  if (ric_sub_end_time_present) {
    j.start_obj();
    j.write_int("id", 63);
    j.write_str("criticality", "reject");
    j.write_str("value", ric_sub_end_time.to_string());
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 5, 17, 18};
  return map_enum_number(names, 4, idx, "id");
}
bool ric_sub_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 5, 17, 18};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 17:
      return crit_e::reject;
    case 18:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_resp_ies_o::value_c ric_sub_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 5:
      ret.set(value_c::types::ran_function_id);
      break;
    case 17:
      ret.set(value_c::types::ric_actions_admitted);
      break;
    case 18:
      ret.set(value_c::types::ric_actions_not_admitted);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 5:
      return presence_e::mandatory;
    case 17:
      return presence_e::mandatory;
    case 18:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_id:
      c = uint16_t{};
      break;
    case types::ric_actions_admitted:
      c = ric_action_admitted_list_l{};
      break;
    case types::ric_actions_not_admitted:
      c = ric_action_not_admitted_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_resp_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_resp_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
uint16_t& ric_sub_resp_ies_o::value_c::ran_function_id()
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
ric_action_admitted_list_l& ric_sub_resp_ies_o::value_c::ric_actions_admitted()
{
  assert_choice_type(types::ric_actions_admitted, type_, "Value");
  return c.get<ric_action_admitted_list_l>();
}
ric_action_not_admitted_list_l& ric_sub_resp_ies_o::value_c::ric_actions_not_admitted()
{
  assert_choice_type(types::ric_actions_not_admitted, type_, "Value");
  return c.get<ric_action_not_admitted_list_l>();
}
const ric_request_id_s& ric_sub_resp_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const uint16_t& ric_sub_resp_ies_o::value_c::ran_function_id() const
{
  assert_choice_type(types::ran_function_id, type_, "Value");
  return c.get<uint16_t>();
}
const ric_action_admitted_list_l& ric_sub_resp_ies_o::value_c::ric_actions_admitted() const
{
  assert_choice_type(types::ric_actions_admitted, type_, "Value");
  return c.get<ric_action_admitted_list_l>();
}
const ric_action_not_admitted_list_l& ric_sub_resp_ies_o::value_c::ric_actions_not_admitted() const
{
  assert_choice_type(types::ric_actions_not_admitted, type_, "Value");
  return c.get<ric_action_not_admitted_list_l>();
}
void ric_sub_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::ric_actions_admitted:
      j.start_array("");
      for (const auto& e1 : c.get<ric_action_admitted_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::ric_actions_not_admitted:
      j.start_array("");
      for (const auto& e1 : c.get<ric_action_not_admitted_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_actions_admitted:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_action_admitted_list_l>(), 1, 16, true));
      break;
    case types::ric_actions_not_admitted:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ric_action_not_admitted_list_l>(), 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, false, true));
      break;
    case types::ric_actions_admitted:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_action_admitted_list_l>(), bref, 1, 16, true));
      break;
    case types::ric_actions_not_admitted:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ric_action_not_admitted_list_l>(), bref, 0, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "RICrequestID", "INTEGER (0..4095)", "RICaction-Admitted-List", "RICaction-NotAdmitted-List"};
  return convert_enum_idx(names, 4, value, "ric_sub_resp_ies_o::value_c::types");
}
uint8_t ric_sub_resp_ies_o::value_c::types_opts::to_number() const
{
  if (value == ran_function_id) {
    return 0;
  }
  invalid_enum_number(value, "ric_sub_resp_ies_o::value_c::types");
  return 0;
}

template struct asn1::protocol_ie_field_s<ric_sub_resp_ies_o>;

OCUDUASN_CODE ric_sub_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ric_actions_not_admitted_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_function_id, (uint16_t)0u, (uint16_t)4095u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)17, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_admitted, 1, 16, true));
  }
  if (ric_actions_not_admitted_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)18, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ric_actions_not_admitted, 0, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_function_id, bref, (uint16_t)0u, (uint16_t)4095u, false, true));
        break;
      }
      case 17: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_admitted, bref, 1, 16, true));
        break;
      }
      case 18: {
        ric_actions_not_admitted_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ric_actions_not_admitted, bref, 0, 16, true));
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
void ric_sub_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_function_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 17);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ric_actions_admitted) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (ric_actions_not_admitted_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ric_actions_not_admitted) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionStateControlFailure-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_state_ctrl_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 1, 2};
  return map_enum_number(names, 3, idx, "id");
}
bool ric_sub_state_ctrl_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 1, 2};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_state_ctrl_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_state_ctrl_fail_ies_o::value_c ric_sub_state_ctrl_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_state_ctrl_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_state_ctrl_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_fail_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_state_ctrl_fail_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
cause_c& ric_sub_state_ctrl_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& ric_sub_state_ctrl_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ric_request_id_s& ric_sub_state_ctrl_fail_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const cause_c& ric_sub_state_ctrl_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& ric_sub_state_ctrl_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void ric_sub_state_ctrl_fail_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_fail_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_state_ctrl_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_state_ctrl_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_state_ctrl_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 3, value, "ric_sub_state_ctrl_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_sub_state_ctrl_fail_ies_o>;

OCUDUASN_CODE ric_sub_state_ctrl_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_state_ctrl_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_state_ctrl_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void ric_sub_state_ctrl_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RICsubscriptionStateControlRequest-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_state_ctrl_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 124};
  return map_enum_number(names, 2, idx, "id");
}
bool ric_sub_state_ctrl_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 124};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_state_ctrl_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 124:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_state_ctrl_request_ies_o::value_c ric_sub_state_ctrl_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 124:
      ret.set(value_c::types::ran_function_state_ctrl_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_state_ctrl_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 124:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_state_ctrl_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_state_ctrl_list:
      c = ran_function_state_ctrl_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_request_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_state_ctrl_request_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
ran_function_state_ctrl_list_l& ric_sub_state_ctrl_request_ies_o::value_c::ran_function_state_ctrl_list()
{
  assert_choice_type(types::ran_function_state_ctrl_list, type_, "Value");
  return c.get<ran_function_state_ctrl_list_l>();
}
const ric_request_id_s& ric_sub_state_ctrl_request_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const ran_function_state_ctrl_list_l& ric_sub_state_ctrl_request_ies_o::value_c::ran_function_state_ctrl_list() const
{
  assert_choice_type(types::ran_function_state_ctrl_list, type_, "Value");
  return c.get<ran_function_state_ctrl_list_l>();
}
void ric_sub_state_ctrl_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_state_ctrl_list:
      j.start_array("");
      for (const auto& e1 : c.get<ran_function_state_ctrl_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_request_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_state_ctrl_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_state_ctrl_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_function_state_ctrl_list_l>(), 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_state_ctrl_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_state_ctrl_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_function_state_ctrl_list_l>(), bref, 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_state_ctrl_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "RANfunctionStateControl-List"};
  return convert_enum_idx(names, 2, value, "ric_sub_state_ctrl_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_sub_state_ctrl_request_ies_o>;

OCUDUASN_CODE ric_sub_state_ctrl_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)124, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_function_state_ctrl_list, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_state_ctrl_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_state_ctrl_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 124: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_function_state_ctrl_list, bref, 1, 256, true));
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
void ric_sub_state_ctrl_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 124);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ran_function_state_ctrl_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// RICsubscriptionStateControlResponse-IEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t ric_sub_state_ctrl_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {29, 122};
  return map_enum_number(names, 2, idx, "id");
}
bool ric_sub_state_ctrl_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {29, 122};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ric_sub_state_ctrl_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 29:
      return crit_e::reject;
    case 122:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
ric_sub_state_ctrl_resp_ies_o::value_c ric_sub_state_ctrl_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 29:
      ret.set(value_c::types::ric_request_id);
      break;
    case 122:
      ret.set(value_c::types::ran_function_state_confirm_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ric_sub_state_ctrl_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 29:
      return presence_e::mandatory;
    case 122:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void ric_sub_state_ctrl_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::ric_request_id:
      c = ric_request_id_s{};
      break;
    case types::ran_function_state_confirm_list:
      c = ran_function_state_confirm_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_resp_ies_o::value_c");
  }
}
ric_request_id_s& ric_sub_state_ctrl_resp_ies_o::value_c::ric_request_id()
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
ran_function_state_confirm_list_l& ric_sub_state_ctrl_resp_ies_o::value_c::ran_function_state_confirm_list()
{
  assert_choice_type(types::ran_function_state_confirm_list, type_, "Value");
  return c.get<ran_function_state_confirm_list_l>();
}
const ric_request_id_s& ric_sub_state_ctrl_resp_ies_o::value_c::ric_request_id() const
{
  assert_choice_type(types::ric_request_id, type_, "Value");
  return c.get<ric_request_id_s>();
}
const ran_function_state_confirm_list_l& ric_sub_state_ctrl_resp_ies_o::value_c::ran_function_state_confirm_list() const
{
  assert_choice_type(types::ran_function_state_confirm_list, type_, "Value");
  return c.get<ran_function_state_confirm_list_l>();
}
void ric_sub_state_ctrl_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::ric_request_id:
      c.get<ric_request_id_s>().to_json(j);
      break;
    case types::ran_function_state_confirm_list:
      j.start_array("");
      for (const auto& e1 : c.get<ran_function_state_confirm_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE ric_sub_state_ctrl_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().pack(bref));
      break;
    case types::ran_function_state_confirm_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ran_function_state_confirm_list_l>(), 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_state_ctrl_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::ric_request_id:
      HANDLE_CODE(c.get<ric_request_id_s>().unpack(bref));
      break;
    case types::ran_function_state_confirm_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ran_function_state_confirm_list_l>(), bref, 1, 256, true));
      break;
    default:
      log_invalid_choice_id(type_, "ric_sub_state_ctrl_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ric_sub_state_ctrl_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RICrequestID", "RANfunctionStateConfirm-List"};
  return convert_enum_idx(names, 2, value, "ric_sub_state_ctrl_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<ric_sub_state_ctrl_resp_ies_o>;

OCUDUASN_CODE ric_sub_state_ctrl_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ric_request_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)122, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ran_function_state_confirm_list, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ric_sub_state_ctrl_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ric_sub_state_ctrl_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ric_request_id.unpack(bref));
        break;
      }
      case 122: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ran_function_state_confirm_list, bref, 1, 256, true));
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
void ric_sub_state_ctrl_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ric_request_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 122);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ran_function_state_confirm_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// ResetRequestIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t reset_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool reset_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 1};
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
    case 49:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
reset_request_ies_o::value_c reset_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 1:
      ret.set(value_c::types::cause);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e reset_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 1:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void reset_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "reset_request_ies_o::value_c");
  }
}
uint16_t& reset_request_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
cause_c& reset_request_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const uint16_t& reset_request_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const cause_c& reset_request_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
void reset_request_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::cause:
      c.get<cause_c>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "reset_request_ies_o::value_c");
  }
}
OCUDUASN_CODE reset_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "reset_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reset_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "reset_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* reset_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "Cause"};
  return convert_enum_idx(names, 2, value, "reset_request_ies_o::value_c::types");
}
uint8_t reset_request_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "reset_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<reset_request_ies_o>;

OCUDUASN_CODE reset_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
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
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != reset_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 1: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
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
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// ResetResponseIEs ::= OBJECT SET OF E2AP-PROTOCOL-IES
uint32_t reset_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {49, 2};
  return map_enum_number(names, 2, idx, "id");
}
bool reset_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {49, 2};
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
    case 49:
      return crit_e::reject;
    case 2:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
reset_resp_ies_o::value_c reset_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 49:
      ret.set(value_c::types::transaction_id);
      break;
    case 2:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e reset_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 49:
      return presence_e::mandatory;
    case 2:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void reset_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::transaction_id:
      c = uint16_t{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "reset_resp_ies_o::value_c");
  }
}
uint16_t& reset_resp_ies_o::value_c::transaction_id()
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
crit_diagnostics_s& reset_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& reset_resp_ies_o::value_c::transaction_id() const
{
  assert_choice_type(types::transaction_id, type_, "Value");
  return c.get<uint16_t>();
}
const crit_diagnostics_s& reset_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void reset_resp_ies_o::value_c::to_json(json_writer& j) const
{
  switch (type_) {
    case types::transaction_id:
      j.write_int(c.get<uint16_t>());
      break;
    case types::crit_diagnostics:
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "reset_resp_ies_o::value_c");
  }
}
OCUDUASN_CODE reset_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "reset_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reset_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::transaction_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u, true, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "reset_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* reset_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (0..255,...)", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "reset_resp_ies_o::value_c::types");
}
uint8_t reset_resp_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "reset_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<reset_resp_ies_o>;

OCUDUASN_CODE reset_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
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
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != reset_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 49: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 2: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
  j.write_int("id", 49);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}
