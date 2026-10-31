// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/rrc_nr/ul_ccch1_msg.h"

using namespace asn1;
using namespace asn1::rrc_nr;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// RRCResumeRequest1-IEs ::= SEQUENCE
OCUDUASN_CODE rrc_resume_request1_ies_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(resume_id.pack(bref));
  HANDLE_CODE(resume_mac_i.pack(bref));
  HANDLE_CODE(resume_cause.pack(bref));
  HANDLE_CODE(spare.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_resume_request1_ies_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(resume_id.unpack(bref));
  HANDLE_CODE(resume_mac_i.unpack(bref));
  HANDLE_CODE(resume_cause.unpack(bref));
  HANDLE_CODE(spare.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void rrc_resume_request1_ies_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("resumeIdentity");
  resume_id.to_json(j);
  j.write_fieldname("resumeMAC-I");
  resume_mac_i.to_json(j);
  j.write_str("resumeCause", resume_cause.to_string());
  j.write_fieldname("spare");
  spare.to_json(j);
  j.end_obj();
}

// RRCResumeRequest1 ::= SEQUENCE
OCUDUASN_CODE rrc_resume_request1_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(rrc_resume_request1.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_resume_request1_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(rrc_resume_request1.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void rrc_resume_request1_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("rrcResumeRequest1");
  rrc_resume_request1.to_json(j);
  j.end_obj();
}

// UL-CCCH1-MessageType ::= CHOICE
void ul_ccch1_msg_type_c::set(types::options e)
{
  type_ = e;
}
ul_ccch1_msg_type_c::c1_c_& ul_ccch1_msg_type_c::set_c1()
{
  set(types::c1);
  return c;
}
void ul_ccch1_msg_type_c::set_msg_class_ext()
{
  set(types::msg_class_ext);
}
void ul_ccch1_msg_type_c::to_json(json_writer& j) const
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
      log_invalid_choice_id(type_, "ul_ccch1_msg_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ul_ccch1_msg_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::c1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::msg_class_ext:
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch1_msg_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ccch1_msg_type_c::unpack(cbit_ref& bref)
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
      log_invalid_choice_id(type_, "ul_ccch1_msg_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

void ul_ccch1_msg_type_c::c1_c_::set(types::options e)
{
  type_ = e;
}
rrc_resume_request1_s& ul_ccch1_msg_type_c::c1_c_::set_rrc_resume_request1()
{
  set(types::rrc_resume_request1);
  return c;
}
void ul_ccch1_msg_type_c::c1_c_::set_spare3()
{
  set(types::spare3);
}
void ul_ccch1_msg_type_c::c1_c_::set_spare2()
{
  set(types::spare2);
}
void ul_ccch1_msg_type_c::c1_c_::set_spare1()
{
  set(types::spare1);
}
void ul_ccch1_msg_type_c::c1_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::rrc_resume_request1:
      j.write_fieldname("rrcResumeRequest1");
      c.to_json(j);
      break;
    case types::spare3:
      j.write_null("spare3");
      break;
    case types::spare2:
      j.write_null("spare2");
      break;
    case types::spare1:
      j.write_null("spare1");
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch1_msg_type_c::c1_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE ul_ccch1_msg_type_c::c1_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::rrc_resume_request1:
      HANDLE_CODE(c.pack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch1_msg_type_c::c1_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ccch1_msg_type_c::c1_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::rrc_resume_request1:
      HANDLE_CODE(c.unpack(bref));
      break;
    case types::spare3:
      break;
    case types::spare2:
      break;
    case types::spare1:
      break;
    default:
      log_invalid_choice_id(type_, "ul_ccch1_msg_type_c::c1_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ul_ccch1_msg_type_c::c1_c_::types_opts::to_string() const
{
  static const char* names[] = {"rrcResumeRequest1", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 4, value, "ul_ccch1_msg_type_c::c1_c_::types");
}
uint8_t ul_ccch1_msg_type_c::c1_c_::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ul_ccch1_msg_type_c::c1_c_::types");
}

const char* ul_ccch1_msg_type_c::types_opts::to_string() const
{
  static const char* names[] = {"c1", "messageClassExtension"};
  return convert_enum_idx(names, 2, value, "ul_ccch1_msg_type_c::types");
}
uint8_t ul_ccch1_msg_type_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ul_ccch1_msg_type_c::types");
}

// UL-CCCH1-Message ::= SEQUENCE
OCUDUASN_CODE ul_ccch1_msg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(msg.pack(bref));

  bref.align_bytes_zero();

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ccch1_msg_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(msg.unpack(bref));

  bref.align_bytes();

  return OCUDUASN_SUCCESS;
}
void ul_ccch1_msg_s::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.start_obj("UL-CCCH1-Message");
  j.write_fieldname("message");
  msg.to_json(j);
  j.end_obj();
  j.end_obj();
  j.end_array();
}
