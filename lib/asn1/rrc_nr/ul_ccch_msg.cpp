// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/rrc_nr/ul_ccch_msg.h"
#include "ocudu/asn1/rrc_nr/ul_ccch_msg_ies.h"

using namespace asn1;
using namespace asn1::rrc_nr;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// UL-CCCH-MessageType ::= CHOICE
void ul_ccch_msg_type_c::set(types::options e)
{
  type_ = e;
}
ul_ccch_msg_type_c::c1_c_& ul_ccch_msg_type_c::set_c1()
{
  set(types::c1);
  return c;
}
void ul_ccch_msg_type_c::set_msg_class_ext()
{
  set(types::msg_class_ext);
}
void ul_ccch_msg_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::c1:
      j.write_fieldname("c1");
      c.to_json(j);
      break;
    case types::msg_class_ext:
      j.write_fieldname("messageClassExtension");
      j.start_obj();
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch_msg_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ul_ccch_msg_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::msg_class_ext:
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch_msg_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ccch_msg_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::msg_class_ext:
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch_msg_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void ul_ccch_msg_type_c::c1_c_::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::rrc_setup_request:
      c = rrc_setup_request_s{};
      break;
    case types::rrc_resume_request:
      c = rrc_resume_request_s{};
      break;
    case types::rrc_reest_request:
      c = rrc_reest_request_s{};
      break;
    case types::rrc_sys_info_request:
      c = rrc_sys_info_request_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch_msg_type_c::c1_c_");
  }
}
rrc_setup_request_s& ul_ccch_msg_type_c::c1_c_::set_rrc_setup_request()
{
  set(types::rrc_setup_request);
  return c.get<rrc_setup_request_s>();
}
rrc_resume_request_s& ul_ccch_msg_type_c::c1_c_::set_rrc_resume_request()
{
  set(types::rrc_resume_request);
  return c.get<rrc_resume_request_s>();
}
rrc_reest_request_s& ul_ccch_msg_type_c::c1_c_::set_rrc_reest_request()
{
  set(types::rrc_reest_request);
  return c.get<rrc_reest_request_s>();
}
rrc_sys_info_request_s& ul_ccch_msg_type_c::c1_c_::set_rrc_sys_info_request()
{
  set(types::rrc_sys_info_request);
  return c.get<rrc_sys_info_request_s>();
}
void ul_ccch_msg_type_c::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::rrc_setup_request:
      j.write_fieldname("rrcSetupRequest");
      c.get<rrc_setup_request_s>().to_json(j);
      break;
    case types::rrc_resume_request:
      j.write_fieldname("rrcResumeRequest");
      c.get<rrc_resume_request_s>().to_json(j);
      break;
    case types::rrc_reest_request:
      j.write_fieldname("rrcReestablishmentRequest");
      c.get<rrc_reest_request_s>().to_json(j);
      break;
    case types::rrc_sys_info_request:
      j.write_fieldname("rrcSystemInfoRequest");
      c.get<rrc_sys_info_request_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch_msg_type_c::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ul_ccch_msg_type_c::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::rrc_setup_request:
      HANDLE_CODE(c.get<rrc_setup_request_s>().pack(bref));
      break;
    case types::rrc_resume_request:
      HANDLE_CODE(c.get<rrc_resume_request_s>().pack(bref));
      break;
    case types::rrc_reest_request:
      HANDLE_CODE(c.get<rrc_reest_request_s>().pack(bref));
      break;
    case types::rrc_sys_info_request:
      HANDLE_CODE(c.get<rrc_sys_info_request_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch_msg_type_c::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ccch_msg_type_c::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::rrc_setup_request:
      HANDLE_CODE(c.get<rrc_setup_request_s>().unpack(bref));
      break;
    case types::rrc_resume_request:
      HANDLE_CODE(c.get<rrc_resume_request_s>().unpack(bref));
      break;
    case types::rrc_reest_request:
      HANDLE_CODE(c.get<rrc_reest_request_s>().unpack(bref));
      break;
    case types::rrc_sys_info_request:
      HANDLE_CODE(c.get<rrc_sys_info_request_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch_msg_type_c::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ul_ccch_msg_type_c::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {
      "rrcSetupRequest", "rrcResumeRequest", "rrcReestablishmentRequest", "rrcSystemInfoRequest"};
  return convert_enum_idx(names, 4, value, "ul_ccch_msg_type_c::c1_c_::types");
}

const char* ul_ccch_msg_type_c::types_opts::to_string() const
{
  static const char* names[] = {"c1", "messageClassExtension"};
  return convert_enum_idx(names, 2, value, "ul_ccch_msg_type_c::types");
}
uint8_t ul_ccch_msg_type_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ul_ccch_msg_type_c::types");
}

// UL-CCCH-Message ::= SEQUENCE
OCUDUASN_CODE ul_ccch_msg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(msg.pack(bref));

  bref.align_bytes_zero();

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ccch_msg_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(msg.unpack(bref));

  bref.align_bytes();

  return OCUDUASN_SUCCESS;
}
void ul_ccch_msg_s::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.start_obj("UL-CCCH-Message");
  j.write_fieldname("message");
  msg.to_json(j);
  j.end_obj();
  j.end_obj();
  j.end_array();
}
