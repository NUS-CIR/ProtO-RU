// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/rrc_nr/cell_group_config.h"

using namespace asn1;
using namespace asn1::rrc_nr;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// AutonomousDenialParameters-r18 ::= SEQUENCE
OCUDUASN_CODE autonomous_denial_params_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(autonomous_denial_slots_r18.pack(bref));
  HANDLE_CODE(autonomous_denial_validity_r18.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE autonomous_denial_params_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(autonomous_denial_slots_r18.unpack(bref));
  HANDLE_CODE(autonomous_denial_validity_r18.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void autonomous_denial_params_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("autonomousDenialSlots-r18", autonomous_denial_slots_r18.to_string());
  j.write_str("autonomousDenialValidity-r18", autonomous_denial_validity_r18.to_string());
  j.end_obj();
}

const char* autonomous_denial_params_r18_s::autonomous_denial_slots_r18_opts::to_string() const
{
  static const char* names[] = {"n2", "n5", "n10", "n15", "n20", "n30", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "autonomous_denial_params_r18_s::autonomous_denial_slots_r18_e_");
}
uint8_t autonomous_denial_params_r18_s::autonomous_denial_slots_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 5, 10, 15, 20, 30};
  return map_enum_number(numbers, 6, value, "autonomous_denial_params_r18_s::autonomous_denial_slots_r18_e_");
}

const char* autonomous_denial_params_r18_s::autonomous_denial_validity_r18_opts::to_string() const
{
  static const char* names[] = {"n200", "n500", "n1000", "n2000"};
  return convert_enum_idx(names, 4, value, "autonomous_denial_params_r18_s::autonomous_denial_validity_r18_e_");
}
uint16_t autonomous_denial_params_r18_s::autonomous_denial_validity_r18_opts::to_number() const
{
  static const uint16_t numbers[] = {200, 500, 1000, 2000};
  return map_enum_number(numbers, 4, value, "autonomous_denial_params_r18_s::autonomous_denial_validity_r18_e_");
}

// BH-LogicalChannelIdentity-r16 ::= CHOICE
void bh_lc_ch_id_r16_c::destroy_() {}
void bh_lc_ch_id_r16_c::set(types::options e)
{
  destroy_();
  type_ = e;
}
bh_lc_ch_id_r16_c::bh_lc_ch_id_r16_c(const bh_lc_ch_id_r16_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::bh_lc_ch_id_r16:
      c.init(other.c.get<uint8_t>());
      break;
    case types::bh_lc_ch_id_ext_r16:
      c.init(other.c.get<uint32_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bh_lc_ch_id_r16_c");
  }
}
bh_lc_ch_id_r16_c& bh_lc_ch_id_r16_c::operator=(const bh_lc_ch_id_r16_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::bh_lc_ch_id_r16:
      c.set(other.c.get<uint8_t>());
      break;
    case types::bh_lc_ch_id_ext_r16:
      c.set(other.c.get<uint32_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "bh_lc_ch_id_r16_c");
  }

  return *this;
}
uint8_t& bh_lc_ch_id_r16_c::set_bh_lc_ch_id_r16()
{
  set(types::bh_lc_ch_id_r16);
  return c.get<uint8_t>();
}
uint32_t& bh_lc_ch_id_r16_c::set_bh_lc_ch_id_ext_r16()
{
  set(types::bh_lc_ch_id_ext_r16);
  return c.get<uint32_t>();
}
void bh_lc_ch_id_r16_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::bh_lc_ch_id_r16:
      j.write_int("bh-LogicalChannelIdentity-r16", c.get<uint8_t>());
      break;
    case types::bh_lc_ch_id_ext_r16:
      j.write_int("bh-LogicalChannelIdentityExt-r16", c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "bh_lc_ch_id_r16_c");
  }
  j.end_obj();
}
OCUDUASN_CODE bh_lc_ch_id_r16_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::bh_lc_ch_id_r16:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)32u));
      break;
    case types::bh_lc_ch_id_ext_r16:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)320u, (uint32_t)65855u));
      break;
    default:
      log_invalid_choice_id(type_, "bh_lc_ch_id_r16_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bh_lc_ch_id_r16_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::bh_lc_ch_id_r16:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)32u));
      break;
    case types::bh_lc_ch_id_ext_r16:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)320u, (uint32_t)65855u));
      break;
    default:
      log_invalid_choice_id(type_, "bh_lc_ch_id_r16_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* bh_lc_ch_id_r16_c::types_opts::to_string() const
{
  static const char* names[] = {"bh-LogicalChannelIdentity-r16", "bh-LogicalChannelIdentityExt-r16"};
  return convert_enum_idx(names, 2, value, "bh_lc_ch_id_r16_c::types");
}

// UL-AM-RLC ::= SEQUENCE
OCUDUASN_CODE ul_am_rlc_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sn_field_len_present, 1));

  if (sn_field_len_present) {
    HANDLE_CODE(sn_field_len.pack(bref));
  }
  HANDLE_CODE(t_poll_retx.pack(bref));
  HANDLE_CODE(poll_pdu.pack(bref));
  HANDLE_CODE(poll_byte.pack(bref));
  HANDLE_CODE(max_retx_thres.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_am_rlc_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sn_field_len_present, 1));

  if (sn_field_len_present) {
    HANDLE_CODE(sn_field_len.unpack(bref));
  }
  HANDLE_CODE(t_poll_retx.unpack(bref));
  HANDLE_CODE(poll_pdu.unpack(bref));
  HANDLE_CODE(poll_byte.unpack(bref));
  HANDLE_CODE(max_retx_thres.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ul_am_rlc_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sn_field_len_present) {
    j.write_str("sn-FieldLength", sn_field_len.to_string());
  }
  j.write_str("t-PollRetransmit", t_poll_retx.to_string());
  j.write_str("pollPDU", poll_pdu.to_string());
  j.write_str("pollByte", poll_byte.to_string());
  j.write_str("maxRetxThreshold", max_retx_thres.to_string());
  j.end_obj();
}

const char* ul_am_rlc_s::max_retx_thres_opts::to_string() const
{
  static const char* names[] = {"t1", "t2", "t3", "t4", "t6", "t8", "t16", "t32"};
  return convert_enum_idx(names, 8, value, "ul_am_rlc_s::max_retx_thres_e_");
}
uint8_t ul_am_rlc_s::max_retx_thres_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3, 4, 6, 8, 16, 32};
  return map_enum_number(numbers, 8, value, "ul_am_rlc_s::max_retx_thres_e_");
}

// T-Reassembly ::= ENUMERATED
const char* t_reassembly_opts::to_string() const
{
  static const char* names[] = {"ms0",   "ms5",   "ms10",  "ms15",  "ms20",  "ms25",  "ms30",  "ms35",
                                "ms40",  "ms45",  "ms50",  "ms55",  "ms60",  "ms65",  "ms70",  "ms75",
                                "ms80",  "ms85",  "ms90",  "ms95",  "ms100", "ms110", "ms120", "ms130",
                                "ms140", "ms150", "ms160", "ms170", "ms180", "ms190", "ms200", "spare1"};
  return convert_enum_idx(names, 32, value, "t_reassembly_e");
}
uint8_t t_reassembly_opts::to_number() const
{
  static const uint8_t numbers[] = {0,  5,  10, 15, 20,  25,  30,  35,  40,  45,  50,  55,  60,  65,  70, 75,
                                    80, 85, 90, 95, 100, 110, 120, 130, 140, 150, 160, 170, 180, 190, 200};
  return map_enum_number(numbers, 31, value, "t_reassembly_e");
}

// T-StatusProhibit ::= ENUMERATED
const char* t_status_prohibit_opts::to_string() const
{
  static const char* names[] = {
      "ms0",   "ms5",   "ms10",   "ms15",   "ms20",   "ms25",   "ms30",   "ms35",   "ms40",  "ms45",  "ms50",
      "ms55",  "ms60",  "ms65",   "ms70",   "ms75",   "ms80",   "ms85",   "ms90",   "ms95",  "ms100", "ms105",
      "ms110", "ms115", "ms120",  "ms125",  "ms130",  "ms135",  "ms140",  "ms145",  "ms150", "ms155", "ms160",
      "ms165", "ms170", "ms175",  "ms180",  "ms185",  "ms190",  "ms195",  "ms200",  "ms205", "ms210", "ms215",
      "ms220", "ms225", "ms230",  "ms235",  "ms240",  "ms245",  "ms250",  "ms300",  "ms350", "ms400", "ms450",
      "ms500", "ms800", "ms1000", "ms1200", "ms1600", "ms2000", "ms2400", "spare2", "spare1"};
  return convert_enum_idx(names, 64, value, "t_status_prohibit_e");
}
uint16_t t_status_prohibit_opts::to_number() const
{
  static const uint16_t numbers[] = {0,   5,   10,  15,  20,  25,  30,  35,  40,  45,   50,   55,   60,   65,  70,  75,
                                     80,  85,  90,  95,  100, 105, 110, 115, 120, 125,  130,  135,  140,  145, 150, 155,
                                     160, 165, 170, 175, 180, 185, 190, 195, 200, 205,  210,  215,  220,  225, 230, 235,
                                     240, 245, 250, 300, 350, 400, 450, 500, 800, 1000, 1200, 1600, 2000, 2400};
  return map_enum_number(numbers, 62, value, "t_status_prohibit_e");
}

// DL-AM-RLC ::= SEQUENCE
OCUDUASN_CODE dl_am_rlc_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sn_field_len_present, 1));

  if (sn_field_len_present) {
    HANDLE_CODE(sn_field_len.pack(bref));
  }
  HANDLE_CODE(t_reassembly.pack(bref));
  HANDLE_CODE(t_status_prohibit.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_am_rlc_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sn_field_len_present, 1));

  if (sn_field_len_present) {
    HANDLE_CODE(sn_field_len.unpack(bref));
  }
  HANDLE_CODE(t_reassembly.unpack(bref));
  HANDLE_CODE(t_status_prohibit.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void dl_am_rlc_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sn_field_len_present) {
    j.write_str("sn-FieldLength", sn_field_len.to_string());
  }
  j.write_str("t-Reassembly", t_reassembly.to_string());
  j.write_str("t-StatusProhibit", t_status_prohibit.to_string());
  j.end_obj();
}

// UL-UM-RLC ::= SEQUENCE
OCUDUASN_CODE ul_um_rlc_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sn_field_len_present, 1));

  if (sn_field_len_present) {
    HANDLE_CODE(sn_field_len.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_um_rlc_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sn_field_len_present, 1));

  if (sn_field_len_present) {
    HANDLE_CODE(sn_field_len.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ul_um_rlc_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sn_field_len_present) {
    j.write_str("sn-FieldLength", sn_field_len.to_string());
  }
  j.end_obj();
}

// DL-UM-RLC ::= SEQUENCE
OCUDUASN_CODE dl_um_rlc_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sn_field_len_present, 1));

  if (sn_field_len_present) {
    HANDLE_CODE(sn_field_len.pack(bref));
  }
  HANDLE_CODE(t_reassembly.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_um_rlc_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sn_field_len_present, 1));

  if (sn_field_len_present) {
    HANDLE_CODE(sn_field_len.unpack(bref));
  }
  HANDLE_CODE(t_reassembly.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void dl_um_rlc_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sn_field_len_present) {
    j.write_str("sn-FieldLength", sn_field_len.to_string());
  }
  j.write_str("t-Reassembly", t_reassembly.to_string());
  j.end_obj();
}

// RLC-Config ::= CHOICE
void rlc_cfg_c::destroy_()
{
  switch (type_) {
    case types::am:
      c.destroy<am_s_>();
      break;
    case types::um_bi_dir:
      c.destroy<um_bi_dir_s_>();
      break;
    case types::um_uni_dir_ul:
      c.destroy<um_uni_dir_ul_s_>();
      break;
    case types::um_uni_dir_dl:
      c.destroy<um_uni_dir_dl_s_>();
      break;
    default:
      break;
  }
}
void rlc_cfg_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::am:
      c.init<am_s_>();
      break;
    case types::um_bi_dir:
      c.init<um_bi_dir_s_>();
      break;
    case types::um_uni_dir_ul:
      c.init<um_uni_dir_ul_s_>();
      break;
    case types::um_uni_dir_dl:
      c.init<um_uni_dir_dl_s_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rlc_cfg_c");
  }
}
rlc_cfg_c::rlc_cfg_c(const rlc_cfg_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::am:
      c.init(other.c.get<am_s_>());
      break;
    case types::um_bi_dir:
      c.init(other.c.get<um_bi_dir_s_>());
      break;
    case types::um_uni_dir_ul:
      c.init(other.c.get<um_uni_dir_ul_s_>());
      break;
    case types::um_uni_dir_dl:
      c.init(other.c.get<um_uni_dir_dl_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rlc_cfg_c");
  }
}
rlc_cfg_c& rlc_cfg_c::operator=(const rlc_cfg_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::am:
      c.set(other.c.get<am_s_>());
      break;
    case types::um_bi_dir:
      c.set(other.c.get<um_bi_dir_s_>());
      break;
    case types::um_uni_dir_ul:
      c.set(other.c.get<um_uni_dir_ul_s_>());
      break;
    case types::um_uni_dir_dl:
      c.set(other.c.get<um_uni_dir_dl_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rlc_cfg_c");
  }

  return *this;
}
rlc_cfg_c::am_s_& rlc_cfg_c::set_am()
{
  set(types::am);
  return c.get<am_s_>();
}
rlc_cfg_c::um_bi_dir_s_& rlc_cfg_c::set_um_bi_dir()
{
  set(types::um_bi_dir);
  return c.get<um_bi_dir_s_>();
}
rlc_cfg_c::um_uni_dir_ul_s_& rlc_cfg_c::set_um_uni_dir_ul()
{
  set(types::um_uni_dir_ul);
  return c.get<um_uni_dir_ul_s_>();
}
rlc_cfg_c::um_uni_dir_dl_s_& rlc_cfg_c::set_um_uni_dir_dl()
{
  set(types::um_uni_dir_dl);
  return c.get<um_uni_dir_dl_s_>();
}
void rlc_cfg_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::am:
      j.write_fieldname("am");
      j.start_obj();
      j.write_fieldname("ul-AM-RLC");
      c.get<am_s_>().ul_am_rlc.to_json(j);
      j.write_fieldname("dl-AM-RLC");
      c.get<am_s_>().dl_am_rlc.to_json(j);
      j.end_obj();
      break;
    case types::um_bi_dir:
      j.write_fieldname("um-Bi-Directional");
      j.start_obj();
      j.write_fieldname("ul-UM-RLC");
      c.get<um_bi_dir_s_>().ul_um_rlc.to_json(j);
      j.write_fieldname("dl-UM-RLC");
      c.get<um_bi_dir_s_>().dl_um_rlc.to_json(j);
      j.end_obj();
      break;
    case types::um_uni_dir_ul:
      j.write_fieldname("um-Uni-Directional-UL");
      j.start_obj();
      j.write_fieldname("ul-UM-RLC");
      c.get<um_uni_dir_ul_s_>().ul_um_rlc.to_json(j);
      j.end_obj();
      break;
    case types::um_uni_dir_dl:
      j.write_fieldname("um-Uni-Directional-DL");
      j.start_obj();
      j.write_fieldname("dl-UM-RLC");
      c.get<um_uni_dir_dl_s_>().dl_um_rlc.to_json(j);
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "rlc_cfg_c");
  }
  j.end_obj();
}
OCUDUASN_CODE rlc_cfg_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::am:
      HANDLE_CODE(c.get<am_s_>().ul_am_rlc.pack(bref));
      HANDLE_CODE(c.get<am_s_>().dl_am_rlc.pack(bref));
      break;
    case types::um_bi_dir:
      HANDLE_CODE(c.get<um_bi_dir_s_>().ul_um_rlc.pack(bref));
      HANDLE_CODE(c.get<um_bi_dir_s_>().dl_um_rlc.pack(bref));
      break;
    case types::um_uni_dir_ul:
      HANDLE_CODE(c.get<um_uni_dir_ul_s_>().ul_um_rlc.pack(bref));
      break;
    case types::um_uni_dir_dl:
      HANDLE_CODE(c.get<um_uni_dir_dl_s_>().dl_um_rlc.pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "rlc_cfg_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rlc_cfg_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::am:
      HANDLE_CODE(c.get<am_s_>().ul_am_rlc.unpack(bref));
      HANDLE_CODE(c.get<am_s_>().dl_am_rlc.unpack(bref));
      break;
    case types::um_bi_dir:
      HANDLE_CODE(c.get<um_bi_dir_s_>().ul_um_rlc.unpack(bref));
      HANDLE_CODE(c.get<um_bi_dir_s_>().dl_um_rlc.unpack(bref));
      break;
    case types::um_uni_dir_ul:
      HANDLE_CODE(c.get<um_uni_dir_ul_s_>().ul_um_rlc.unpack(bref));
      break;
    case types::um_uni_dir_dl:
      HANDLE_CODE(c.get<um_uni_dir_dl_s_>().dl_um_rlc.unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "rlc_cfg_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* rlc_cfg_c::types_opts::to_string() const
{
  static const char* names[] = {"am", "um-Bi-Directional", "um-Uni-Directional-UL", "um-Uni-Directional-DL"};
  return convert_enum_idx(names, 4, value, "rlc_cfg_c::types");
}

// LogicalChannelConfig ::= SEQUENCE
OCUDUASN_CODE lc_ch_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= ch_access_prio_r16_present;
  group_flags[0] |= bit_rate_multiplier_r16_present;
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), ul_specific_params_present}));

  if (ul_specific_params_present) {
    HANDLE_CODE(ul_specific_params.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {ch_access_prio_r16_present, bit_rate_multiplier_r16_present}));
      if (ch_access_prio_r16_present) {
        HANDLE_CODE(pack_integer(bref, ch_access_prio_r16, (uint8_t)1u, (uint8_t)4u));
      }
      if (bit_rate_multiplier_r16_present) {
        HANDLE_CODE(bit_rate_multiplier_r16.pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE lc_ch_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &ul_specific_params_present}));

  if (ul_specific_params_present) {
    HANDLE_CODE(ul_specific_params.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref, {&ch_access_prio_r16_present, &bit_rate_multiplier_r16_present}));
      if (ch_access_prio_r16_present) {
        HANDLE_CODE(unpack_integer(ch_access_prio_r16, bref, (uint8_t)1u, (uint8_t)4u));
      }
      if (bit_rate_multiplier_r16_present) {
        HANDLE_CODE(bit_rate_multiplier_r16.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void lc_ch_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ul_specific_params_present) {
    j.write_fieldname("ul-SpecificParameters");
    ul_specific_params.to_json(j);
  }
  if (ch_access_prio_r16_present) {
    j.write_int("channelAccessPriority-r16", ch_access_prio_r16);
  }
  if (bit_rate_multiplier_r16_present) {
    j.write_str("bitRateMultiplier-r16", bit_rate_multiplier_r16.to_string());
  }
  j.end_obj();
}

OCUDUASN_CODE lc_ch_cfg_s::ul_specific_params_s_::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(3);
  group_flags[0] |= bit_rate_query_prohibit_timer_present;
  group_flags[1] |= allowed_cg_list_r16.is_present();
  group_flags[1] |= allowed_phy_prio_idx_r16_present;
  group_flags[2] |= lc_ch_group_iab_ext_r17_present;
  group_flags[2] |= allowed_harq_mode_r17_present;
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         allowed_serving_cells.size() > 0,
                         allowed_scs_list.size() > 0,
                         max_pusch_dur_present,
                         cfg_grant_type1_allowed_present,
                         lc_ch_group_present,
                         sched_request_id_present}));

  HANDLE_CODE(pack_integer(bref, prio, (uint8_t)1u, (uint8_t)16u));
  HANDLE_CODE(prioritised_bit_rate.pack(bref));
  HANDLE_CODE(bucket_size_dur.pack(bref));
  if (allowed_serving_cells.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, allowed_serving_cells, 1, 31, integer_packer<uint8_t>(0, 31)));
  }
  if (allowed_scs_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, allowed_scs_list, 1, 5));
  }
  if (max_pusch_dur_present) {
    HANDLE_CODE(max_pusch_dur.pack(bref));
  }
  if (lc_ch_group_present) {
    HANDLE_CODE(pack_integer(bref, lc_ch_group, (uint8_t)0u, (uint8_t)7u));
  }
  if (sched_request_id_present) {
    HANDLE_CODE(pack_integer(bref, sched_request_id, (uint8_t)0u, (uint8_t)7u));
  }
  HANDLE_CODE(bref.pack(lc_ch_sr_mask, 1));
  HANDLE_CODE(bref.pack(lc_ch_sr_delay_timer_applied, 1));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bit_rate_query_prohibit_timer.pack(bref));
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {allowed_cg_list_r16.is_present(), allowed_phy_prio_idx_r16_present}));
      if (allowed_cg_list_r16.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *allowed_cg_list_r16, 0, 31, integer_packer<uint8_t>(0, 31)));
      }
      if (allowed_phy_prio_idx_r16_present) {
        HANDLE_CODE(allowed_phy_prio_idx_r16.pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {lc_ch_group_iab_ext_r17_present, allowed_harq_mode_r17_present}));
      if (lc_ch_group_iab_ext_r17_present) {
        HANDLE_CODE(pack_integer(bref, lc_ch_group_iab_ext_r17, (uint16_t)0u, (uint16_t)255u));
      }
      if (allowed_harq_mode_r17_present) {
        HANDLE_CODE(allowed_harq_mode_r17.pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE lc_ch_cfg_s::ul_specific_params_s_::unpack(cbit_ref& bref)
{
  bool ext;
  bool allowed_serving_cells_present;
  bool allowed_scs_list_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &allowed_serving_cells_present,
                           &allowed_scs_list_present,
                           &max_pusch_dur_present,
                           &cfg_grant_type1_allowed_present,
                           &lc_ch_group_present,
                           &sched_request_id_present}));

  HANDLE_CODE(unpack_integer(prio, bref, (uint8_t)1u, (uint8_t)16u));
  HANDLE_CODE(prioritised_bit_rate.unpack(bref));
  HANDLE_CODE(bucket_size_dur.unpack(bref));
  if (allowed_serving_cells_present) {
    HANDLE_CODE(unpack_dyn_seq_of(allowed_serving_cells, bref, 1, 31, integer_packer<uint8_t>(0, 31)));
  }
  if (allowed_scs_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(allowed_scs_list, bref, 1, 5));
  }
  if (max_pusch_dur_present) {
    HANDLE_CODE(max_pusch_dur.unpack(bref));
  }
  if (lc_ch_group_present) {
    HANDLE_CODE(unpack_integer(lc_ch_group, bref, (uint8_t)0u, (uint8_t)7u));
  }
  if (sched_request_id_present) {
    HANDLE_CODE(unpack_integer(sched_request_id, bref, (uint8_t)0u, (uint8_t)7u));
  }
  HANDLE_CODE(bref.unpack(lc_ch_sr_mask, 1));
  HANDLE_CODE(bref.unpack(lc_ch_sr_delay_timer_applied, 1));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      bit_rate_query_prohibit_timer_present = true;
      HANDLE_CODE(bit_rate_query_prohibit_timer.unpack(bref));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(allowed_cg_list_r16, bref);
      HANDLE_CODE(bref.unpack(allowed_phy_prio_idx_r16_present, 1));
      if (allowed_cg_list_r16.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*allowed_cg_list_r16, bref, 0, 31, integer_packer<uint8_t>(0, 31)));
      }
      if (allowed_phy_prio_idx_r16_present) {
        HANDLE_CODE(allowed_phy_prio_idx_r16.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref, {&lc_ch_group_iab_ext_r17_present, &allowed_harq_mode_r17_present}));
      if (lc_ch_group_iab_ext_r17_present) {
        HANDLE_CODE(unpack_integer(lc_ch_group_iab_ext_r17, bref, (uint16_t)0u, (uint16_t)255u));
      }
      if (allowed_harq_mode_r17_present) {
        HANDLE_CODE(allowed_harq_mode_r17.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void lc_ch_cfg_s::ul_specific_params_s_::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("priority", prio);
  j.write_str("prioritisedBitRate", prioritised_bit_rate.to_string());
  j.write_str("bucketSizeDuration", bucket_size_dur.to_string());
  if (allowed_serving_cells.size() > 0) {
    j.start_array("allowedServingCells");
    for (const auto& e1 : allowed_serving_cells) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (allowed_scs_list.size() > 0) {
    j.start_array("allowedSCS-List");
    for (const auto& e1 : allowed_scs_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
  }
  if (max_pusch_dur_present) {
    j.write_str("maxPUSCH-Duration", max_pusch_dur.to_string());
  }
  if (cfg_grant_type1_allowed_present) {
    j.write_str("configuredGrantType1Allowed", "true");
  }
  if (lc_ch_group_present) {
    j.write_int("logicalChannelGroup", lc_ch_group);
  }
  if (sched_request_id_present) {
    j.write_int("schedulingRequestID", sched_request_id);
  }
  j.write_bool("logicalChannelSR-Mask", lc_ch_sr_mask);
  j.write_bool("logicalChannelSR-DelayTimerApplied", lc_ch_sr_delay_timer_applied);
  if (bit_rate_query_prohibit_timer_present) {
    j.write_str("bitRateQueryProhibitTimer", bit_rate_query_prohibit_timer.to_string());
  }
  if (allowed_cg_list_r16.is_present()) {
    j.start_array("allowedCG-List-r16");
    for (const auto& e1 : *allowed_cg_list_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (allowed_phy_prio_idx_r16_present) {
    j.write_str("allowedPHY-PriorityIndex-r16", allowed_phy_prio_idx_r16.to_string());
  }
  if (lc_ch_group_iab_ext_r17_present) {
    j.write_int("logicalChannelGroupIAB-Ext-r17", lc_ch_group_iab_ext_r17);
  }
  if (allowed_harq_mode_r17_present) {
    j.write_str("allowedHARQ-mode-r17", allowed_harq_mode_r17.to_string());
  }
  j.end_obj();
}

const char* lc_ch_cfg_s::ul_specific_params_s_::prioritised_bit_rate_opts::to_string() const
{
  static const char* names[] = {"kBps0",
                                "kBps8",
                                "kBps16",
                                "kBps32",
                                "kBps64",
                                "kBps128",
                                "kBps256",
                                "kBps512",
                                "kBps1024",
                                "kBps2048",
                                "kBps4096",
                                "kBps8192",
                                "kBps16384",
                                "kBps32768",
                                "kBps65536",
                                "infinity"};
  return convert_enum_idx(names, 16, value, "lc_ch_cfg_s::ul_specific_params_s_::prioritised_bit_rate_e_");
}
int32_t lc_ch_cfg_s::ul_specific_params_s_::prioritised_bit_rate_opts::to_number() const
{
  static const int32_t numbers[] = {0, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, -1};
  return map_enum_number(numbers, 16, value, "lc_ch_cfg_s::ul_specific_params_s_::prioritised_bit_rate_e_");
}

const char* lc_ch_cfg_s::ul_specific_params_s_::bucket_size_dur_opts::to_string() const
{
  static const char* names[] = {"ms5",
                                "ms10",
                                "ms20",
                                "ms50",
                                "ms100",
                                "ms150",
                                "ms300",
                                "ms500",
                                "ms1000",
                                "spare7",
                                "spare6",
                                "spare5",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(names, 16, value, "lc_ch_cfg_s::ul_specific_params_s_::bucket_size_dur_e_");
}
uint16_t lc_ch_cfg_s::ul_specific_params_s_::bucket_size_dur_opts::to_number() const
{
  static const uint16_t numbers[] = {5, 10, 20, 50, 100, 150, 300, 500, 1000};
  return map_enum_number(numbers, 9, value, "lc_ch_cfg_s::ul_specific_params_s_::bucket_size_dur_e_");
}

const char* lc_ch_cfg_s::ul_specific_params_s_::max_pusch_dur_opts::to_string() const
{
  static const char* names[] = {"ms0p02", "ms0p04", "ms0p0625", "ms0p125", "ms0p25", "ms0p5", "ms0p01-v1700", "spare1"};
  return convert_enum_idx(names, 8, value, "lc_ch_cfg_s::ul_specific_params_s_::max_pusch_dur_e_");
}

const char* lc_ch_cfg_s::ul_specific_params_s_::bit_rate_query_prohibit_timer_opts::to_string() const
{
  static const char* names[] = {"s0", "s0dot4", "s0dot8", "s1dot6", "s3", "s6", "s12", "s30"};
  return convert_enum_idx(names, 8, value, "lc_ch_cfg_s::ul_specific_params_s_::bit_rate_query_prohibit_timer_e_");
}
float lc_ch_cfg_s::ul_specific_params_s_::bit_rate_query_prohibit_timer_opts::to_number() const
{
  static const float numbers[] = {0.0, 0.4, 0.8, 1.6, 3.0, 6.0, 12.0, 30.0};
  return map_enum_number(numbers, 8, value, "lc_ch_cfg_s::ul_specific_params_s_::bit_rate_query_prohibit_timer_e_");
}
const char* lc_ch_cfg_s::ul_specific_params_s_::bit_rate_query_prohibit_timer_opts::to_number_string() const
{
  static const char* number_strs[] = {"0", "0.4", "0.8", "1.6", "3", "6", "12", "30"};
  return convert_enum_idx(
      number_strs, 8, value, "lc_ch_cfg_s::ul_specific_params_s_::bit_rate_query_prohibit_timer_e_");
}

const char* lc_ch_cfg_s::ul_specific_params_s_::allowed_phy_prio_idx_r16_opts::to_string() const
{
  static const char* names[] = {"p0", "p1"};
  return convert_enum_idx(names, 2, value, "lc_ch_cfg_s::ul_specific_params_s_::allowed_phy_prio_idx_r16_e_");
}
uint8_t lc_ch_cfg_s::ul_specific_params_s_::allowed_phy_prio_idx_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 1};
  return map_enum_number(numbers, 2, value, "lc_ch_cfg_s::ul_specific_params_s_::allowed_phy_prio_idx_r16_e_");
}

const char* lc_ch_cfg_s::ul_specific_params_s_::allowed_harq_mode_r17_opts::to_string() const
{
  static const char* names[] = {"harqModeA", "harqModeB"};
  return convert_enum_idx(names, 2, value, "lc_ch_cfg_s::ul_specific_params_s_::allowed_harq_mode_r17_e_");
}

const char* lc_ch_cfg_s::bit_rate_multiplier_r16_opts::to_string() const
{
  static const char* names[] = {"x40", "x70", "x100", "x200"};
  return convert_enum_idx(names, 4, value, "lc_ch_cfg_s::bit_rate_multiplier_r16_e_");
}
uint8_t lc_ch_cfg_s::bit_rate_multiplier_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {40, 70, 100, 200};
  return map_enum_number(numbers, 4, value, "lc_ch_cfg_s::bit_rate_multiplier_r16_e_");
}

// BH-RLC-ChannelConfig-r16 ::= SEQUENCE
OCUDUASN_CODE bh_rlc_ch_cfg_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref,
      {false, bh_lc_ch_id_r16_present, reestablish_rlc_r16_present, rlc_cfg_r16_present, mac_lc_ch_cfg_r16_present}));

  if (bh_lc_ch_id_r16_present) {
    HANDLE_CODE(bh_lc_ch_id_r16.pack(bref));
  }
  HANDLE_CODE(bh_rlc_ch_id_r16.pack(bref));
  if (rlc_cfg_r16_present) {
    HANDLE_CODE(rlc_cfg_r16.pack(bref));
  }
  if (mac_lc_ch_cfg_r16_present) {
    HANDLE_CODE(mac_lc_ch_cfg_r16.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bh_rlc_ch_cfg_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &bh_lc_ch_id_r16_present,
                           &reestablish_rlc_r16_present,
                           &rlc_cfg_r16_present,
                           &mac_lc_ch_cfg_r16_present}));

  if (bh_lc_ch_id_r16_present) {
    HANDLE_CODE(bh_lc_ch_id_r16.unpack(bref));
  }
  HANDLE_CODE(bh_rlc_ch_id_r16.unpack(bref));
  if (rlc_cfg_r16_present) {
    HANDLE_CODE(rlc_cfg_r16.unpack(bref));
  }
  if (mac_lc_ch_cfg_r16_present) {
    HANDLE_CODE(mac_lc_ch_cfg_r16.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void bh_rlc_ch_cfg_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (bh_lc_ch_id_r16_present) {
    j.write_fieldname("bh-LogicalChannelIdentity-r16");
    bh_lc_ch_id_r16.to_json(j);
  }
  j.write_fieldname("bh-RLC-ChannelID-r16");
  bh_rlc_ch_id_r16.to_json(j);
  if (reestablish_rlc_r16_present) {
    j.write_str("reestablishRLC-r16", "true");
  }
  if (rlc_cfg_r16_present) {
    j.write_fieldname("rlc-Config-r16");
    rlc_cfg_r16.to_json(j);
  }
  if (mac_lc_ch_cfg_r16_present) {
    j.write_fieldname("mac-LogicalChannelConfig-r16");
    mac_lc_ch_cfg_r16.to_json(j);
  }
  j.end_obj();
}

// CarrierState-r17 ::= CHOICE
void carrier_state_r17_c::set(types::options e)
{
  type_ = e;
}
void carrier_state_r17_c::set_de_activ_r17()
{
  set(types::de_activ_r17);
}
uint8_t& carrier_state_r17_c::set_active_bwp_r17()
{
  set(types::active_bwp_r17);
  return c;
}
void carrier_state_r17_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::de_activ_r17:
      j.write_null("deActivated-r17");
      break;
    case types::active_bwp_r17:
      j.write_int("activeBWP-r17", c);
      break;
    default:
      log_invalid_choice_id(type_, "carrier_state_r17_c");
  }
  j.end_obj();
}
OCUDUASN_CODE carrier_state_r17_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::de_activ_r17:
      break;
    case types::active_bwp_r17:
      HANDLE_CODE(pack_integer(bref, c, (uint8_t)0u, (uint8_t)4u));
      break;
    default:
      log_invalid_choice_id(type_, "carrier_state_r17_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE carrier_state_r17_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::de_activ_r17:
      break;
    case types::active_bwp_r17:
      HANDLE_CODE(unpack_integer(c, bref, (uint8_t)0u, (uint8_t)4u));
      break;
    default:
      log_invalid_choice_id(type_, "carrier_state_r17_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* carrier_state_r17_c::types_opts::to_string() const
{
  static const char* names[] = {"deActivated-r17", "activeBWP-r17"};
  return convert_enum_idx(names, 2, value, "carrier_state_r17_c::types");
}

// CC-State-r17 ::= SEQUENCE
OCUDUASN_CODE cc_state_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {dl_carrier_r17_present, ul_carrier_r17_present}));

  if (dl_carrier_r17_present) {
    HANDLE_CODE(dl_carrier_r17.pack(bref));
  }
  if (ul_carrier_r17_present) {
    HANDLE_CODE(ul_carrier_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cc_state_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&dl_carrier_r17_present, &ul_carrier_r17_present}));

  if (dl_carrier_r17_present) {
    HANDLE_CODE(dl_carrier_r17.unpack(bref));
  }
  if (ul_carrier_r17_present) {
    HANDLE_CODE(ul_carrier_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void cc_state_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (dl_carrier_r17_present) {
    j.write_fieldname("dlCarrier-r17");
    dl_carrier_r17.to_json(j);
  }
  if (ul_carrier_r17_present) {
    j.write_fieldname("ulCarrier-r17");
    ul_carrier_r17.to_json(j);
  }
  j.end_obj();
}

// CFRA-SSB-Resource ::= SEQUENCE
OCUDUASN_CODE cfra_ssb_res_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= msg_a_pusch_res_idx_r16_present;
  HANDLE_CODE(bref.pack(group_flags.any(), 1));

  HANDLE_CODE(pack_integer(bref, ssb, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(pack_integer(bref, ra_preamb_idx, (uint8_t)0u, (uint8_t)63u));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(msg_a_pusch_res_idx_r16_present, 1));
      if (msg_a_pusch_res_idx_r16_present) {
        HANDLE_CODE(pack_integer(bref, msg_a_pusch_res_idx_r16, (uint16_t)0u, (uint16_t)3071u));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfra_ssb_res_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_integer(ssb, bref, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(unpack_integer(ra_preamb_idx, bref, (uint8_t)0u, (uint8_t)63u));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(msg_a_pusch_res_idx_r16_present, 1));
      if (msg_a_pusch_res_idx_r16_present) {
        HANDLE_CODE(unpack_integer(msg_a_pusch_res_idx_r16, bref, (uint16_t)0u, (uint16_t)3071u));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cfra_ssb_res_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ssb", ssb);
  j.write_int("ra-PreambleIndex", ra_preamb_idx);
  if (msg_a_pusch_res_idx_r16_present) {
    j.write_int("msgA-PUSCH-Resource-Index-r16", msg_a_pusch_res_idx_r16);
  }
  j.end_obj();
}

// CFRA-CSIRS-Resource ::= SEQUENCE
OCUDUASN_CODE cfra_csi_rs_res_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pack_integer(bref, csi_rs, (uint8_t)0u, (uint8_t)95u));
  HANDLE_CODE(pack_dyn_seq_of(bref, ra_occasion_list, 1, 64, integer_packer<uint16_t>(0, 511)));
  HANDLE_CODE(pack_integer(bref, ra_preamb_idx, (uint8_t)0u, (uint8_t)63u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfra_csi_rs_res_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_integer(csi_rs, bref, (uint8_t)0u, (uint8_t)95u));
  HANDLE_CODE(unpack_dyn_seq_of(ra_occasion_list, bref, 1, 64, integer_packer<uint16_t>(0, 511)));
  HANDLE_CODE(unpack_integer(ra_preamb_idx, bref, (uint8_t)0u, (uint8_t)63u));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cfra_csi_rs_res_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("csi-RS", csi_rs);
  j.start_array("ra-OccasionList");
  for (const auto& e1 : ra_occasion_list) {
    j.write_int(e1);
  }
  j.end_array();
  j.write_int("ra-PreambleIndex", ra_preamb_idx);
  j.end_obj();
}

// CFRA ::= SEQUENCE
OCUDUASN_CODE cfra_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(2);
  group_flags[0] |= total_nof_ra_preambs_present;
  group_flags[1] |= msg1_repeat_num_r18_present;
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), occasions_present}));

  if (occasions_present) {
    HANDLE_CODE(bref.pack(occasions.ssb_per_rach_occasion_present, 1));
    HANDLE_CODE(occasions.rach_cfg_generic.pack(bref));
    if (occasions.ssb_per_rach_occasion_present) {
      HANDLE_CODE(occasions.ssb_per_rach_occasion.pack(bref));
    }
  }
  HANDLE_CODE(res.pack(bref));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(total_nof_ra_preambs_present, 1));
      if (total_nof_ra_preambs_present) {
        HANDLE_CODE(pack_integer(bref, total_nof_ra_preambs, (uint8_t)1u, (uint8_t)63u));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(msg1_repeat_num_r18_present, 1));
      if (msg1_repeat_num_r18_present) {
        HANDLE_CODE(msg1_repeat_num_r18.pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfra_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &occasions_present}));

  if (occasions_present) {
    HANDLE_CODE(bref.unpack(occasions.ssb_per_rach_occasion_present, 1));
    HANDLE_CODE(occasions.rach_cfg_generic.unpack(bref));
    if (occasions.ssb_per_rach_occasion_present) {
      HANDLE_CODE(occasions.ssb_per_rach_occasion.unpack(bref));
    }
  }
  HANDLE_CODE(res.unpack(bref));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(total_nof_ra_preambs_present, 1));
      if (total_nof_ra_preambs_present) {
        HANDLE_CODE(unpack_integer(total_nof_ra_preambs, bref, (uint8_t)1u, (uint8_t)63u));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(msg1_repeat_num_r18_present, 1));
      if (msg1_repeat_num_r18_present) {
        HANDLE_CODE(msg1_repeat_num_r18.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cfra_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (occasions_present) {
    j.write_fieldname("occasions");
    j.start_obj();
    j.write_fieldname("rach-ConfigGeneric");
    occasions.rach_cfg_generic.to_json(j);
    if (occasions.ssb_per_rach_occasion_present) {
      j.write_str("ssb-perRACH-Occasion", occasions.ssb_per_rach_occasion.to_string());
    }
    j.end_obj();
  }
  j.write_fieldname("resources");
  res.to_json(j);
  if (total_nof_ra_preambs_present) {
    j.write_int("totalNumberOfRA-Preambles", total_nof_ra_preambs);
  }
  if (msg1_repeat_num_r18_present) {
    j.write_str("msg1-RepetitionNum-r18", msg1_repeat_num_r18.to_string());
  }
  j.end_obj();
}

const char* cfra_s::occasions_s_::ssb_per_rach_occasion_opts::to_string() const
{
  static const char* names[] = {"oneEighth", "oneFourth", "oneHalf", "one", "two", "four", "eight", "sixteen"};
  return convert_enum_idx(names, 8, value, "cfra_s::occasions_s_::ssb_per_rach_occasion_e_");
}
float cfra_s::occasions_s_::ssb_per_rach_occasion_opts::to_number() const
{
  static const float numbers[] = {0.125, 0.25, 0.5, 1.0, 2.0, 4.0, 8.0, 6.0};
  return map_enum_number(numbers, 8, value, "cfra_s::occasions_s_::ssb_per_rach_occasion_e_");
}
const char* cfra_s::occasions_s_::ssb_per_rach_occasion_opts::to_number_string() const
{
  static const char* number_strs[] = {"1/8", "1/4", "1/2", "1", "2", "4", "8", "6"};
  return convert_enum_idx(number_strs, 8, value, "cfra_s::occasions_s_::ssb_per_rach_occasion_e_");
}

void cfra_s::res_c_::destroy_()
{
  switch (type_) {
    case types::ssb:
      c.destroy<ssb_s_>();
      break;
    case types::csirs:
      c.destroy<csirs_s_>();
      break;
    default:
      break;
  }
}
void cfra_s::res_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ssb:
      c.init<ssb_s_>();
      break;
    case types::csirs:
      c.init<csirs_s_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cfra_s::res_c_");
  }
}
cfra_s::res_c_::res_c_(const cfra_s::res_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ssb:
      c.init(other.c.get<ssb_s_>());
      break;
    case types::csirs:
      c.init(other.c.get<csirs_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cfra_s::res_c_");
  }
}
cfra_s::res_c_& cfra_s::res_c_::operator=(const cfra_s::res_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ssb:
      c.set(other.c.get<ssb_s_>());
      break;
    case types::csirs:
      c.set(other.c.get<csirs_s_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "cfra_s::res_c_");
  }

  return *this;
}
cfra_s::res_c_::ssb_s_& cfra_s::res_c_::set_ssb()
{
  set(types::ssb);
  return c.get<ssb_s_>();
}
cfra_s::res_c_::csirs_s_& cfra_s::res_c_::set_csirs()
{
  set(types::csirs);
  return c.get<csirs_s_>();
}
void cfra_s::res_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ssb:
      j.write_fieldname("ssb");
      j.start_obj();
      j.start_array("ssb-ResourceList");
      for (const auto& e1 : c.get<ssb_s_>().ssb_res_list) {
        e1.to_json(j);
      }
      j.end_array();
      j.write_int("ra-ssb-OccasionMaskIndex", c.get<ssb_s_>().ra_ssb_occasion_mask_idx);
      j.end_obj();
      break;
    case types::csirs:
      j.write_fieldname("csirs");
      j.start_obj();
      j.start_array("csirs-ResourceList");
      for (const auto& e1 : c.get<csirs_s_>().csirs_res_list) {
        e1.to_json(j);
      }
      j.end_array();
      j.write_int("rsrp-ThresholdCSI-RS", c.get<csirs_s_>().rsrp_thres_csi_rs);
      j.end_obj();
      break;
    default:
      log_invalid_choice_id(type_, "cfra_s::res_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE cfra_s::res_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ssb:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ssb_s_>().ssb_res_list, 1, 64));
      HANDLE_CODE(pack_integer(bref, c.get<ssb_s_>().ra_ssb_occasion_mask_idx, (uint8_t)0u, (uint8_t)15u));
      break;
    case types::csirs:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<csirs_s_>().csirs_res_list, 1, 96));
      HANDLE_CODE(pack_integer(bref, c.get<csirs_s_>().rsrp_thres_csi_rs, (uint8_t)0u, (uint8_t)127u));
      break;
    default:
      log_invalid_choice_id(type_, "cfra_s::res_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfra_s::res_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ssb:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ssb_s_>().ssb_res_list, bref, 1, 64));
      HANDLE_CODE(unpack_integer(c.get<ssb_s_>().ra_ssb_occasion_mask_idx, bref, (uint8_t)0u, (uint8_t)15u));
      break;
    case types::csirs:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<csirs_s_>().csirs_res_list, bref, 1, 96));
      HANDLE_CODE(unpack_integer(c.get<csirs_s_>().rsrp_thres_csi_rs, bref, (uint8_t)0u, (uint8_t)127u));
      break;
    default:
      log_invalid_choice_id(type_, "cfra_s::res_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* cfra_s::res_c_::types_opts::to_string() const
{
  static const char* names[] = {"ssb", "csirs"};
  return convert_enum_idx(names, 2, value, "cfra_s::res_c_::types");
}

const char* cfra_s::msg1_repeat_num_r18_opts::to_string() const
{
  static const char* names[] = {"n2", "n4", "n8", "spare1"};
  return convert_enum_idx(names, 4, value, "cfra_s::msg1_repeat_num_r18_e_");
}
uint8_t cfra_s::msg1_repeat_num_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 8};
  return map_enum_number(numbers, 3, value, "cfra_s::msg1_repeat_num_r18_e_");
}

// CFRA-TwoStep-r16 ::= SEQUENCE
OCUDUASN_CODE cfra_two_step_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, occasions_two_step_ra_r16_present, msg_a_trans_max_r16_present}));

  if (occasions_two_step_ra_r16_present) {
    HANDLE_CODE(occasions_two_step_ra_r16.rach_cfg_generic_two_step_ra_r16.pack(bref));
    HANDLE_CODE(occasions_two_step_ra_r16.ssb_per_rach_occasion_two_step_ra_r16.pack(bref));
  }
  HANDLE_CODE(msg_a_cfra_pusch_r16.pack(bref));
  if (msg_a_trans_max_r16_present) {
    HANDLE_CODE(msg_a_trans_max_r16.pack(bref));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, res_two_step_r16.ssb_res_list, 1, 64));
  HANDLE_CODE(pack_integer(bref, res_two_step_r16.ra_ssb_occasion_mask_idx, (uint8_t)0u, (uint8_t)15u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cfra_two_step_r16_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &occasions_two_step_ra_r16_present, &msg_a_trans_max_r16_present}));

  if (occasions_two_step_ra_r16_present) {
    HANDLE_CODE(occasions_two_step_ra_r16.rach_cfg_generic_two_step_ra_r16.unpack(bref));
    HANDLE_CODE(occasions_two_step_ra_r16.ssb_per_rach_occasion_two_step_ra_r16.unpack(bref));
  }
  HANDLE_CODE(msg_a_cfra_pusch_r16.unpack(bref));
  if (msg_a_trans_max_r16_present) {
    HANDLE_CODE(msg_a_trans_max_r16.unpack(bref));
  }
  HANDLE_CODE(unpack_dyn_seq_of(res_two_step_r16.ssb_res_list, bref, 1, 64));
  HANDLE_CODE(unpack_integer(res_two_step_r16.ra_ssb_occasion_mask_idx, bref, (uint8_t)0u, (uint8_t)15u));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cfra_two_step_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (occasions_two_step_ra_r16_present) {
    j.write_fieldname("occasionsTwoStepRA-r16");
    j.start_obj();
    j.write_fieldname("rach-ConfigGenericTwoStepRA-r16");
    occasions_two_step_ra_r16.rach_cfg_generic_two_step_ra_r16.to_json(j);
    j.write_str("ssb-PerRACH-OccasionTwoStepRA-r16",
                occasions_two_step_ra_r16.ssb_per_rach_occasion_two_step_ra_r16.to_string());
    j.end_obj();
  }
  j.write_fieldname("msgA-CFRA-PUSCH-r16");
  msg_a_cfra_pusch_r16.to_json(j);
  if (msg_a_trans_max_r16_present) {
    j.write_str("msgA-TransMax-r16", msg_a_trans_max_r16.to_string());
  }
  j.write_fieldname("resourcesTwoStep-r16");
  j.start_obj();
  j.start_array("ssb-ResourceList");
  for (const auto& e1 : res_two_step_r16.ssb_res_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.write_int("ra-ssb-OccasionMaskIndex", res_two_step_r16.ra_ssb_occasion_mask_idx);
  j.end_obj();
  j.end_obj();
}

const char*
cfra_two_step_r16_s::occasions_two_step_ra_r16_s_::ssb_per_rach_occasion_two_step_ra_r16_opts::to_string() const
{
  static const char* names[] = {"oneEighth", "oneFourth", "oneHalf", "one", "two", "four", "eight", "sixteen"};
  return convert_enum_idx(
      names, 8, value, "cfra_two_step_r16_s::occasions_two_step_ra_r16_s_::ssb_per_rach_occasion_two_step_ra_r16_e_");
}
float cfra_two_step_r16_s::occasions_two_step_ra_r16_s_::ssb_per_rach_occasion_two_step_ra_r16_opts::to_number() const
{
  static const float numbers[] = {0.125, 0.25, 0.5, 1.0, 2.0, 4.0, 8.0, 6.0};
  return map_enum_number(
      numbers, 8, value, "cfra_two_step_r16_s::occasions_two_step_ra_r16_s_::ssb_per_rach_occasion_two_step_ra_r16_e_");
}
const char*
cfra_two_step_r16_s::occasions_two_step_ra_r16_s_::ssb_per_rach_occasion_two_step_ra_r16_opts::to_number_string() const
{
  static const char* number_strs[] = {"1/8", "1/4", "1/2", "1", "2", "4", "8", "6"};
  return convert_enum_idx(
      number_strs,
      8,
      value,
      "cfra_two_step_r16_s::occasions_two_step_ra_r16_s_::ssb_per_rach_occasion_two_step_ra_r16_e_");
}

const char* cfra_two_step_r16_s::msg_a_trans_max_r16_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4", "n6", "n8", "n10", "n20", "n50", "n100", "n200"};
  return convert_enum_idx(names, 10, value, "cfra_two_step_r16_s::msg_a_trans_max_r16_e_");
}
uint8_t cfra_two_step_r16_s::msg_a_trans_max_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 6, 8, 10, 20, 50, 100, 200};
  return map_enum_number(numbers, 10, value, "cfra_two_step_r16_s::msg_a_trans_max_r16_e_");
}

// CellDTRX-DCI-config-r18 ::= SEQUENCE
OCUDUASN_CODE cell_dtrx_dci_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, cell_dtrx_rnti_r18, (uint32_t)0u, (uint32_t)65535u));
  HANDLE_CODE(pack_integer(bref, size_dci_2_9_r18, (uint8_t)1u, (uint8_t)140u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_dtrx_dci_cfg_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(cell_dtrx_rnti_r18, bref, (uint32_t)0u, (uint32_t)65535u));
  HANDLE_CODE(unpack_integer(size_dci_2_9_r18, bref, (uint8_t)1u, (uint8_t)140u));

  return OCUDUASN_SUCCESS;
}
void cell_dtrx_dci_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("cellDTRX-RNTI-r18", cell_dtrx_rnti_r18);
  j.write_int("sizeDCI-2-9-r18", size_dci_2_9_r18);
  j.end_obj();
}

// T-StatusProhibit-v1610 ::= ENUMERATED
const char* t_status_prohibit_v1610_opts::to_string() const
{
  static const char* names[] = {"ms1", "ms2", "ms3", "ms4", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "t_status_prohibit_v1610_e");
}
uint8_t t_status_prohibit_v1610_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3, 4};
  return map_enum_number(numbers, 4, value, "t_status_prohibit_v1610_e");
}

// DL-AM-RLC-v1610 ::= SEQUENCE
OCUDUASN_CODE dl_am_rlc_v1610_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, t_status_prohibit_v1610_present}));

  if (t_status_prohibit_v1610_present) {
    HANDLE_CODE(t_status_prohibit_v1610.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_am_rlc_v1610_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &t_status_prohibit_v1610_present}));

  if (t_status_prohibit_v1610_present) {
    HANDLE_CODE(t_status_prohibit_v1610.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_am_rlc_v1610_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (t_status_prohibit_v1610_present) {
    j.write_str("t-StatusProhibit-v1610", t_status_prohibit_v1610.to_string());
  }
  j.end_obj();
}

// RLC-Config-v1610 ::= SEQUENCE
OCUDUASN_CODE rlc_cfg_v1610_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(dl_am_rlc_v1610.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rlc_cfg_v1610_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(dl_am_rlc_v1610.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void rlc_cfg_v1610_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("dl-AM-RLC-v1610");
  dl_am_rlc_v1610.to_json(j);
  j.end_obj();
}

// T-ReassemblyExt-r17 ::= ENUMERATED
const char* t_reassembly_ext_r17_opts::to_string() const
{
  static const char* names[] = {"ms210", "ms220", "ms340", "ms350", "ms550", "ms1100", "ms1650", "ms2200"};
  return convert_enum_idx(names, 8, value, "t_reassembly_ext_r17_e");
}
uint16_t t_reassembly_ext_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {210, 220, 340, 350, 550, 1100, 1650, 2200};
  return map_enum_number(numbers, 8, value, "t_reassembly_ext_r17_e");
}

// DL-AM-RLC-v1700 ::= SEQUENCE
OCUDUASN_CODE dl_am_rlc_v1700_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(t_reassembly_ext_r17_present, 1));

  if (t_reassembly_ext_r17_present) {
    HANDLE_CODE(t_reassembly_ext_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_am_rlc_v1700_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(t_reassembly_ext_r17_present, 1));

  if (t_reassembly_ext_r17_present) {
    HANDLE_CODE(t_reassembly_ext_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void dl_am_rlc_v1700_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (t_reassembly_ext_r17_present) {
    j.write_str("t-ReassemblyExt-r17", t_reassembly_ext_r17.to_string());
  }
  j.end_obj();
}

// DL-UM-RLC-v1700 ::= SEQUENCE
OCUDUASN_CODE dl_um_rlc_v1700_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(t_reassembly_ext_r17_present, 1));

  if (t_reassembly_ext_r17_present) {
    HANDLE_CODE(t_reassembly_ext_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_um_rlc_v1700_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(t_reassembly_ext_r17_present, 1));

  if (t_reassembly_ext_r17_present) {
    HANDLE_CODE(t_reassembly_ext_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void dl_um_rlc_v1700_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (t_reassembly_ext_r17_present) {
    j.write_str("t-ReassemblyExt-r17", t_reassembly_ext_r17.to_string());
  }
  j.end_obj();
}

// RLC-Config-v1700 ::= SEQUENCE
OCUDUASN_CODE rlc_cfg_v1700_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(dl_am_rlc_v1700.pack(bref));
  HANDLE_CODE(dl_um_rlc_v1700.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rlc_cfg_v1700_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(dl_am_rlc_v1700.unpack(bref));
  HANDLE_CODE(dl_um_rlc_v1700.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void rlc_cfg_v1700_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("dl-AM-RLC-v1700");
  dl_am_rlc_v1700.to_json(j);
  j.write_fieldname("dl-UM-RLC-v1700");
  dl_um_rlc_v1700.to_json(j);
  j.end_obj();
}

// MulticastRLC-BearerConfig-r17 ::= SEQUENCE
OCUDUASN_CODE multicast_rlc_bearer_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(is_ptm_entity_r17_present, 1));

  HANDLE_CODE(pack_integer(bref, served_mbs_radio_bearer_r17, (uint16_t)1u, (uint16_t)512u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_rlc_bearer_cfg_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(is_ptm_entity_r17_present, 1));

  HANDLE_CODE(unpack_integer(served_mbs_radio_bearer_r17, bref, (uint16_t)1u, (uint16_t)512u));

  return OCUDUASN_SUCCESS;
}
void multicast_rlc_bearer_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("servedMBS-RadioBearer-r17", served_mbs_radio_bearer_r17);
  if (is_ptm_entity_r17_present) {
    j.write_str("isPTM-Entity-r17", "true");
  }
  j.end_obj();
}

// RLC-BearerConfig ::= SEQUENCE
OCUDUASN_CODE rlc_bearer_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(3);
  group_flags[0] |= rlc_cfg_v1610.is_present();
  group_flags[1] |= rlc_cfg_v1700.is_present();
  group_flags[1] |= lc_ch_id_ext_r17_present;
  group_flags[1] |= multicast_rlc_bearer_cfg_r17.is_present();
  group_flags[1] |= served_radio_bearer_srb4_r17_present;
  group_flags[2] |= served_radio_bearer_srb5_r18_present;
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         served_radio_bearer_present,
                         reestablish_rlc_present,
                         rlc_cfg_present,
                         mac_lc_ch_cfg_present}));

  HANDLE_CODE(pack_integer(bref, lc_ch_id, (uint8_t)1u, (uint8_t)32u));
  if (served_radio_bearer_present) {
    HANDLE_CODE(served_radio_bearer.pack(bref));
  }
  if (rlc_cfg_present) {
    HANDLE_CODE(rlc_cfg.pack(bref));
  }
  if (mac_lc_ch_cfg_present) {
    HANDLE_CODE(mac_lc_ch_cfg.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(rlc_cfg_v1610.is_present(), 1));
      if (rlc_cfg_v1610.is_present()) {
        HANDLE_CODE(rlc_cfg_v1610->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {rlc_cfg_v1700.is_present(),
                             lc_ch_id_ext_r17_present,
                             multicast_rlc_bearer_cfg_r17.is_present(),
                             served_radio_bearer_srb4_r17_present}));
      if (rlc_cfg_v1700.is_present()) {
        HANDLE_CODE(rlc_cfg_v1700->pack(bref));
      }
      if (lc_ch_id_ext_r17_present) {
        HANDLE_CODE(pack_integer(bref, lc_ch_id_ext_r17, (uint32_t)320u, (uint32_t)65855u));
      }
      if (multicast_rlc_bearer_cfg_r17.is_present()) {
        HANDLE_CODE(multicast_rlc_bearer_cfg_r17->pack(bref));
      }
      if (served_radio_bearer_srb4_r17_present) {
        HANDLE_CODE(pack_integer(bref, served_radio_bearer_srb4_r17, (uint8_t)4u, (uint8_t)4u));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(served_radio_bearer_srb5_r18_present, 1));
      if (served_radio_bearer_srb5_r18_present) {
        HANDLE_CODE(pack_integer(bref, served_radio_bearer_srb5_r18, (uint8_t)5u, (uint8_t)5u));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rlc_bearer_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(
      bref, {&ext, &served_radio_bearer_present, &reestablish_rlc_present, &rlc_cfg_present, &mac_lc_ch_cfg_present}));

  HANDLE_CODE(unpack_integer(lc_ch_id, bref, (uint8_t)1u, (uint8_t)32u));
  if (served_radio_bearer_present) {
    HANDLE_CODE(served_radio_bearer.unpack(bref));
  }
  if (rlc_cfg_present) {
    HANDLE_CODE(rlc_cfg.unpack(bref));
  }
  if (mac_lc_ch_cfg_present) {
    HANDLE_CODE(mac_lc_ch_cfg.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(rlc_cfg_v1610, bref);
      if (rlc_cfg_v1610.is_present()) {
        HANDLE_CODE(rlc_cfg_v1610->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(rlc_cfg_v1700, bref);
      HANDLE_CODE(bref.unpack(lc_ch_id_ext_r17_present, 1));
      unpack_presence_flag(multicast_rlc_bearer_cfg_r17, bref);
      HANDLE_CODE(bref.unpack(served_radio_bearer_srb4_r17_present, 1));
      if (rlc_cfg_v1700.is_present()) {
        HANDLE_CODE(rlc_cfg_v1700->unpack(bref));
      }
      if (lc_ch_id_ext_r17_present) {
        HANDLE_CODE(unpack_integer(lc_ch_id_ext_r17, bref, (uint32_t)320u, (uint32_t)65855u));
      }
      if (multicast_rlc_bearer_cfg_r17.is_present()) {
        HANDLE_CODE(multicast_rlc_bearer_cfg_r17->unpack(bref));
      }
      if (served_radio_bearer_srb4_r17_present) {
        HANDLE_CODE(unpack_integer(served_radio_bearer_srb4_r17, bref, (uint8_t)4u, (uint8_t)4u));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(served_radio_bearer_srb5_r18_present, 1));
      if (served_radio_bearer_srb5_r18_present) {
        HANDLE_CODE(unpack_integer(served_radio_bearer_srb5_r18, bref, (uint8_t)5u, (uint8_t)5u));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void rlc_bearer_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("logicalChannelIdentity", lc_ch_id);
  if (served_radio_bearer_present) {
    j.write_fieldname("servedRadioBearer");
    served_radio_bearer.to_json(j);
  }
  if (reestablish_rlc_present) {
    j.write_str("reestablishRLC", "true");
  }
  if (rlc_cfg_present) {
    j.write_fieldname("rlc-Config");
    rlc_cfg.to_json(j);
  }
  if (mac_lc_ch_cfg_present) {
    j.write_fieldname("mac-LogicalChannelConfig");
    mac_lc_ch_cfg.to_json(j);
  }
  if (rlc_cfg_v1610.is_present()) {
    j.write_fieldname("rlc-Config-v1610");
    rlc_cfg_v1610->to_json(j);
  }
  if (rlc_cfg_v1700.is_present()) {
    j.write_fieldname("rlc-Config-v1700");
    rlc_cfg_v1700->to_json(j);
  }
  if (lc_ch_id_ext_r17_present) {
    j.write_int("logicalChannelIdentityExt-r17", lc_ch_id_ext_r17);
  }
  if (multicast_rlc_bearer_cfg_r17.is_present()) {
    j.write_fieldname("multicastRLC-BearerConfig-r17");
    multicast_rlc_bearer_cfg_r17->to_json(j);
  }
  if (served_radio_bearer_srb4_r17_present) {
    j.write_int("servedRadioBearerSRB4-r17", served_radio_bearer_srb4_r17);
  }
  if (served_radio_bearer_srb5_r18_present) {
    j.write_int("servedRadioBearerSRB5-r18", served_radio_bearer_srb5_r18);
  }
  j.end_obj();
}

void rlc_bearer_cfg_s::served_radio_bearer_c_::destroy_() {}
void rlc_bearer_cfg_s::served_radio_bearer_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
rlc_bearer_cfg_s::served_radio_bearer_c_::served_radio_bearer_c_(const rlc_bearer_cfg_s::served_radio_bearer_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::srb_id:
      c.init(other.c.get<uint8_t>());
      break;
    case types::drb_id:
      c.init(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rlc_bearer_cfg_s::served_radio_bearer_c_");
  }
}
rlc_bearer_cfg_s::served_radio_bearer_c_&
rlc_bearer_cfg_s::served_radio_bearer_c_::operator=(const rlc_bearer_cfg_s::served_radio_bearer_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::srb_id:
      c.set(other.c.get<uint8_t>());
      break;
    case types::drb_id:
      c.set(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rlc_bearer_cfg_s::served_radio_bearer_c_");
  }

  return *this;
}
uint8_t& rlc_bearer_cfg_s::served_radio_bearer_c_::set_srb_id()
{
  set(types::srb_id);
  return c.get<uint8_t>();
}
uint8_t& rlc_bearer_cfg_s::served_radio_bearer_c_::set_drb_id()
{
  set(types::drb_id);
  return c.get<uint8_t>();
}
void rlc_bearer_cfg_s::served_radio_bearer_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srb_id:
      j.write_int("srb-Identity", c.get<uint8_t>());
      break;
    case types::drb_id:
      j.write_int("drb-Identity", c.get<uint8_t>());
      break;
    default:
      log_invalid_choice_id(type_, "rlc_bearer_cfg_s::served_radio_bearer_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE rlc_bearer_cfg_s::served_radio_bearer_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::srb_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)3u));
      break;
    case types::drb_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)32u));
      break;
    default:
      log_invalid_choice_id(type_, "rlc_bearer_cfg_s::served_radio_bearer_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rlc_bearer_cfg_s::served_radio_bearer_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::srb_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)3u));
      break;
    case types::drb_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)32u));
      break;
    default:
      log_invalid_choice_id(type_, "rlc_bearer_cfg_s::served_radio_bearer_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* rlc_bearer_cfg_s::served_radio_bearer_c_::types_opts::to_string() const
{
  static const char* names[] = {"srb-Identity", "drb-Identity"};
  return convert_enum_idx(names, 2, value, "rlc_bearer_cfg_s::served_radio_bearer_c_::types");
}

// SchedulingRequestToAddMod ::= SEQUENCE
OCUDUASN_CODE sched_request_to_add_mod_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sr_prohibit_timer_present, 1));

  HANDLE_CODE(pack_integer(bref, sched_request_id, (uint8_t)0u, (uint8_t)7u));
  if (sr_prohibit_timer_present) {
    HANDLE_CODE(sr_prohibit_timer.pack(bref));
  }
  HANDLE_CODE(sr_trans_max.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sched_request_to_add_mod_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sr_prohibit_timer_present, 1));

  HANDLE_CODE(unpack_integer(sched_request_id, bref, (uint8_t)0u, (uint8_t)7u));
  if (sr_prohibit_timer_present) {
    HANDLE_CODE(sr_prohibit_timer.unpack(bref));
  }
  HANDLE_CODE(sr_trans_max.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void sched_request_to_add_mod_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("schedulingRequestId", sched_request_id);
  if (sr_prohibit_timer_present) {
    j.write_str("sr-ProhibitTimer", sr_prohibit_timer.to_string());
  }
  j.write_str("sr-TransMax", sr_trans_max.to_string());
  j.end_obj();
}

const char* sched_request_to_add_mod_s::sr_prohibit_timer_opts::to_string() const
{
  static const char* names[] = {"ms1", "ms2", "ms4", "ms8", "ms16", "ms32", "ms64", "ms128"};
  return convert_enum_idx(names, 8, value, "sched_request_to_add_mod_s::sr_prohibit_timer_e_");
}
uint8_t sched_request_to_add_mod_s::sr_prohibit_timer_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4, 8, 16, 32, 64, 128};
  return map_enum_number(numbers, 8, value, "sched_request_to_add_mod_s::sr_prohibit_timer_e_");
}

const char* sched_request_to_add_mod_s::sr_trans_max_opts::to_string() const
{
  static const char* names[] = {"n4", "n8", "n16", "n32", "n64", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "sched_request_to_add_mod_s::sr_trans_max_e_");
}
uint8_t sched_request_to_add_mod_s::sr_trans_max_opts::to_number() const
{
  static const uint8_t numbers[] = {4, 8, 16, 32, 64};
  return map_enum_number(numbers, 5, value, "sched_request_to_add_mod_s::sr_trans_max_e_");
}

// SchedulingRequestConfig ::= SEQUENCE
OCUDUASN_CODE sched_request_cfg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {sched_request_to_add_mod_list.size() > 0, sched_request_to_release_list.size() > 0}));

  if (sched_request_to_add_mod_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sched_request_to_add_mod_list, 1, 8));
  }
  if (sched_request_to_release_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sched_request_to_release_list, 1, 8, integer_packer<uint8_t>(0, 7)));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sched_request_cfg_s::unpack(cbit_ref& bref)
{
  bool sched_request_to_add_mod_list_present;
  bool sched_request_to_release_list_present;
  HANDLE_CODE(unpack_bits(bref, {&sched_request_to_add_mod_list_present, &sched_request_to_release_list_present}));

  if (sched_request_to_add_mod_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sched_request_to_add_mod_list, bref, 1, 8));
  }
  if (sched_request_to_release_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sched_request_to_release_list, bref, 1, 8, integer_packer<uint8_t>(0, 7)));
  }

  return OCUDUASN_SUCCESS;
}
void sched_request_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sched_request_to_add_mod_list.size() > 0) {
    j.start_array("schedulingRequestToAddModList");
    for (const auto& e1 : sched_request_to_add_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (sched_request_to_release_list.size() > 0) {
    j.start_array("schedulingRequestToReleaseList");
    for (const auto& e1 : sched_request_to_release_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  j.end_obj();
}

// TAG ::= SEQUENCE
OCUDUASN_CODE tag_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pack_integer(bref, tag_id, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(time_align_timer.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tag_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_integer(tag_id, bref, (uint8_t)0u, (uint8_t)3u));
  HANDLE_CODE(time_align_timer.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void tag_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("tag-Id", tag_id);
  j.write_str("timeAlignmentTimer", time_align_timer.to_string());
  j.end_obj();
}

// TAG-Config ::= SEQUENCE
OCUDUASN_CODE tag_cfg_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {tag_to_release_list.size() > 0, tag_to_add_mod_list.size() > 0}));

  if (tag_to_release_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, tag_to_release_list, 1, 4, integer_packer<uint8_t>(0, 3)));
  }
  if (tag_to_add_mod_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, tag_to_add_mod_list, 1, 4));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tag_cfg_s::unpack(cbit_ref& bref)
{
  bool tag_to_release_list_present;
  bool tag_to_add_mod_list_present;
  HANDLE_CODE(unpack_bits(bref, {&tag_to_release_list_present, &tag_to_add_mod_list_present}));

  if (tag_to_release_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(tag_to_release_list, bref, 1, 4, integer_packer<uint8_t>(0, 3)));
  }
  if (tag_to_add_mod_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(tag_to_add_mod_list, bref, 1, 4));
  }

  return OCUDUASN_SUCCESS;
}
void tag_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (tag_to_release_list.size() > 0) {
    j.start_array("tag-ToReleaseList");
    for (const auto& e1 : tag_to_release_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (tag_to_add_mod_list.size() > 0) {
    j.start_array("tag-ToAddModList");
    for (const auto& e1 : tag_to_add_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// MPE-Config-FR2-r16 ::= SEQUENCE
OCUDUASN_CODE mpe_cfg_fr2_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(mpe_prohibit_timer_r16.pack(bref));
  HANDLE_CODE(mpe_thres_r16.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mpe_cfg_fr2_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(mpe_prohibit_timer_r16.unpack(bref));
  HANDLE_CODE(mpe_thres_r16.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void mpe_cfg_fr2_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mpe-ProhibitTimer-r16", mpe_prohibit_timer_r16.to_string());
  j.write_str("mpe-Threshold-r16", mpe_thres_r16.to_string());
  j.end_obj();
}

const char* mpe_cfg_fr2_r16_s::mpe_prohibit_timer_r16_opts::to_string() const
{
  static const char* names[] = {"sf0", "sf10", "sf20", "sf50", "sf100", "sf200", "sf500", "sf1000"};
  return convert_enum_idx(names, 8, value, "mpe_cfg_fr2_r16_s::mpe_prohibit_timer_r16_e_");
}
uint16_t mpe_cfg_fr2_r16_s::mpe_prohibit_timer_r16_opts::to_number() const
{
  static const uint16_t numbers[] = {0, 10, 20, 50, 100, 200, 500, 1000};
  return map_enum_number(numbers, 8, value, "mpe_cfg_fr2_r16_s::mpe_prohibit_timer_r16_e_");
}

const char* mpe_cfg_fr2_r16_s::mpe_thres_r16_opts::to_string() const
{
  static const char* names[] = {"dB3", "dB6", "dB9", "dB12"};
  return convert_enum_idx(names, 4, value, "mpe_cfg_fr2_r16_s::mpe_thres_r16_e_");
}
uint8_t mpe_cfg_fr2_r16_s::mpe_thres_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {3, 6, 9, 12};
  return map_enum_number(numbers, 4, value, "mpe_cfg_fr2_r16_s::mpe_thres_r16_e_");
}

// MPE-Config-FR2-r17 ::= SEQUENCE
OCUDUASN_CODE mpe_cfg_fr2_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(mpe_prohibit_timer_r17.pack(bref));
  HANDLE_CODE(mpe_thres_r17.pack(bref));
  HANDLE_CODE(pack_integer(bref, nof_n_r17, (uint8_t)1u, (uint8_t)4u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mpe_cfg_fr2_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(mpe_prohibit_timer_r17.unpack(bref));
  HANDLE_CODE(mpe_thres_r17.unpack(bref));
  HANDLE_CODE(unpack_integer(nof_n_r17, bref, (uint8_t)1u, (uint8_t)4u));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mpe_cfg_fr2_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("mpe-ProhibitTimer-r17", mpe_prohibit_timer_r17.to_string());
  j.write_str("mpe-Threshold-r17", mpe_thres_r17.to_string());
  j.write_int("numberOfN-r17", nof_n_r17);
  j.end_obj();
}

const char* mpe_cfg_fr2_r17_s::mpe_prohibit_timer_r17_opts::to_string() const
{
  static const char* names[] = {"sf0", "sf10", "sf20", "sf50", "sf100", "sf200", "sf500", "sf1000"};
  return convert_enum_idx(names, 8, value, "mpe_cfg_fr2_r17_s::mpe_prohibit_timer_r17_e_");
}
uint16_t mpe_cfg_fr2_r17_s::mpe_prohibit_timer_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {0, 10, 20, 50, 100, 200, 500, 1000};
  return map_enum_number(numbers, 8, value, "mpe_cfg_fr2_r17_s::mpe_prohibit_timer_r17_e_");
}

const char* mpe_cfg_fr2_r17_s::mpe_thres_r17_opts::to_string() const
{
  static const char* names[] = {"dB3", "dB6", "dB9", "dB12"};
  return convert_enum_idx(names, 4, value, "mpe_cfg_fr2_r17_s::mpe_thres_r17_e_");
}
uint8_t mpe_cfg_fr2_r17_s::mpe_thres_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {3, 6, 9, 12};
  return map_enum_number(numbers, 4, value, "mpe_cfg_fr2_r17_s::mpe_thres_r17_e_");
}

// PHR-Config ::= SEQUENCE
OCUDUASN_CODE phr_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(3);
  group_flags[0] |= mpe_report_fr2_r16.is_present();
  group_flags[1] |= mpe_report_fr2_r17.is_present();
  group_flags[1] |= two_phr_mode_r17_present;
  group_flags[2] |= phr_assumed_pusch_report_r18_present;
  group_flags[2] |= dpc_report_fr1_r18_present;
  HANDLE_CODE(bref.pack(group_flags.any(), 1));

  HANDLE_CODE(phr_periodic_timer.pack(bref));
  HANDLE_CODE(phr_prohibit_timer.pack(bref));
  HANDLE_CODE(phr_tx_pwr_factor_change.pack(bref));
  HANDLE_CODE(bref.pack(multiple_phr, 1));
  HANDLE_CODE(bref.pack(dummy, 1));
  HANDLE_CODE(bref.pack(phr_type2_other_cell, 1));
  HANDLE_CODE(phr_mode_other_cg.pack(bref));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(mpe_report_fr2_r16.is_present(), 1));
      if (mpe_report_fr2_r16.is_present()) {
        HANDLE_CODE(mpe_report_fr2_r16->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {mpe_report_fr2_r17.is_present(), two_phr_mode_r17_present}));
      if (mpe_report_fr2_r17.is_present()) {
        HANDLE_CODE(mpe_report_fr2_r17->pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {phr_assumed_pusch_report_r18_present, dpc_report_fr1_r18_present}));
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE phr_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(phr_periodic_timer.unpack(bref));
  HANDLE_CODE(phr_prohibit_timer.unpack(bref));
  HANDLE_CODE(phr_tx_pwr_factor_change.unpack(bref));
  HANDLE_CODE(bref.unpack(multiple_phr, 1));
  HANDLE_CODE(bref.unpack(dummy, 1));
  HANDLE_CODE(bref.unpack(phr_type2_other_cell, 1));
  HANDLE_CODE(phr_mode_other_cg.unpack(bref));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(mpe_report_fr2_r16, bref);
      if (mpe_report_fr2_r16.is_present()) {
        HANDLE_CODE(mpe_report_fr2_r16->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(mpe_report_fr2_r17, bref);
      HANDLE_CODE(bref.unpack(two_phr_mode_r17_present, 1));
      if (mpe_report_fr2_r17.is_present()) {
        HANDLE_CODE(mpe_report_fr2_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref, {&phr_assumed_pusch_report_r18_present, &dpc_report_fr1_r18_present}));
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void phr_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("phr-PeriodicTimer", phr_periodic_timer.to_string());
  j.write_str("phr-ProhibitTimer", phr_prohibit_timer.to_string());
  j.write_str("phr-Tx-PowerFactorChange", phr_tx_pwr_factor_change.to_string());
  j.write_bool("multiplePHR", multiple_phr);
  j.write_bool("dummy", dummy);
  j.write_bool("phr-Type2OtherCell", phr_type2_other_cell);
  j.write_str("phr-ModeOtherCG", phr_mode_other_cg.to_string());
  if (mpe_report_fr2_r16.is_present()) {
    j.write_fieldname("mpe-Reporting-FR2-r16");
    mpe_report_fr2_r16->to_json(j);
  }
  if (mpe_report_fr2_r17.is_present()) {
    j.write_fieldname("mpe-Reporting-FR2-r17");
    mpe_report_fr2_r17->to_json(j);
  }
  if (two_phr_mode_r17_present) {
    j.write_str("twoPHRMode-r17", "enabled");
  }
  if (phr_assumed_pusch_report_r18_present) {
    j.write_str("phr-AssumedPUSCH-Reporting-r18", "enabled");
  }
  if (dpc_report_fr1_r18_present) {
    j.write_str("dpc-Reporting-FR1-r18", "enabled");
  }
  j.end_obj();
}

const char* phr_cfg_s::phr_periodic_timer_opts::to_string() const
{
  static const char* names[] = {"sf10", "sf20", "sf50", "sf100", "sf200", "sf500", "sf1000", "infinity"};
  return convert_enum_idx(names, 8, value, "phr_cfg_s::phr_periodic_timer_e_");
}
int16_t phr_cfg_s::phr_periodic_timer_opts::to_number() const
{
  static const int16_t numbers[] = {10, 20, 50, 100, 200, 500, 1000, -1};
  return map_enum_number(numbers, 8, value, "phr_cfg_s::phr_periodic_timer_e_");
}

const char* phr_cfg_s::phr_prohibit_timer_opts::to_string() const
{
  static const char* names[] = {"sf0", "sf10", "sf20", "sf50", "sf100", "sf200", "sf500", "sf1000"};
  return convert_enum_idx(names, 8, value, "phr_cfg_s::phr_prohibit_timer_e_");
}
uint16_t phr_cfg_s::phr_prohibit_timer_opts::to_number() const
{
  static const uint16_t numbers[] = {0, 10, 20, 50, 100, 200, 500, 1000};
  return map_enum_number(numbers, 8, value, "phr_cfg_s::phr_prohibit_timer_e_");
}

const char* phr_cfg_s::phr_tx_pwr_factor_change_opts::to_string() const
{
  static const char* names[] = {"dB1", "dB3", "dB6", "infinity"};
  return convert_enum_idx(names, 4, value, "phr_cfg_s::phr_tx_pwr_factor_change_e_");
}
int8_t phr_cfg_s::phr_tx_pwr_factor_change_opts::to_number() const
{
  static const int8_t numbers[] = {1, 3, 6, -1};
  return map_enum_number(numbers, 4, value, "phr_cfg_s::phr_tx_pwr_factor_change_e_");
}

const char* phr_cfg_s::phr_mode_other_cg_opts::to_string() const
{
  static const char* names[] = {"real", "virtual"};
  return convert_enum_idx(names, 2, value, "phr_cfg_s::phr_mode_other_cg_e_");
}

// DataInactivityTimer ::= ENUMERATED
const char* data_inactivity_timer_opts::to_string() const
{
  static const char* names[] = {
      "s1", "s2", "s3", "s5", "s7", "s10", "s15", "s20", "s40", "s50", "s60", "s80", "s100", "s120", "s150", "s180"};
  return convert_enum_idx(names, 16, value, "data_inactivity_timer_e");
}
uint8_t data_inactivity_timer_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3, 5, 7, 10, 15, 20, 40, 50, 60, 80, 100, 120, 150, 180};
  return map_enum_number(numbers, 16, value, "data_inactivity_timer_e");
}

// DRX-ConfigSecondaryGroup-r16 ::= SEQUENCE
OCUDUASN_CODE drx_cfg_secondary_group_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(drx_on_dur_timer_r16.pack(bref));
  HANDLE_CODE(drx_inactivity_timer_r16.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_secondary_group_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(drx_on_dur_timer_r16.unpack(bref));
  HANDLE_CODE(drx_inactivity_timer_r16.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void drx_cfg_secondary_group_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("drx-onDurationTimer-r16");
  drx_on_dur_timer_r16.to_json(j);
  j.write_str("drx-InactivityTimer-r16", drx_inactivity_timer_r16.to_string());
  j.end_obj();
}

void drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::destroy_() {}
void drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::drx_on_dur_timer_r16_c_(
    const drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::sub_milli_seconds:
      c.init(other.c.get<uint8_t>());
      break;
    case types::milli_seconds:
      c.init(other.c.get<milli_seconds_e_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_");
  }
}
drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_&
drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::operator=(
    const drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::sub_milli_seconds:
      c.set(other.c.get<uint8_t>());
      break;
    case types::milli_seconds:
      c.set(other.c.get<milli_seconds_e_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_");
  }

  return *this;
}
uint8_t& drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::set_sub_milli_seconds()
{
  set(types::sub_milli_seconds);
  return c.get<uint8_t>();
}
drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::milli_seconds_e_&
drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::set_milli_seconds()
{
  set(types::milli_seconds);
  return c.get<milli_seconds_e_>();
}
void drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::sub_milli_seconds:
      j.write_int("subMilliSeconds", c.get<uint8_t>());
      break;
    case types::milli_seconds:
      j.write_str("milliSeconds", c.get<milli_seconds_e_>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::sub_milli_seconds:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)31u));
      break;
    case types::milli_seconds:
      HANDLE_CODE(c.get<milli_seconds_e_>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::sub_milli_seconds:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)31u));
      break;
    case types::milli_seconds:
      HANDLE_CODE(c.get<milli_seconds_e_>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::milli_seconds_opts::to_string() const
{
  static const char* names[] = {"ms1",    "ms2",    "ms3",    "ms4",    "ms5",    "ms6",    "ms8",    "ms10",
                                "ms20",   "ms30",   "ms40",   "ms50",   "ms60",   "ms80",   "ms100",  "ms200",
                                "ms300",  "ms400",  "ms500",  "ms600",  "ms800",  "ms1000", "ms1200", "ms1600",
                                "spare8", "spare7", "spare6", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 32, value, "drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::milli_seconds_e_");
}
uint16_t drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::milli_seconds_opts::to_number() const
{
  static const uint16_t numbers[] = {1,  2,  3,   4,   5,   6,   8,   10,  20,  30,   40,   50,
                                     60, 80, 100, 200, 300, 400, 500, 600, 800, 1000, 1200, 1600};
  return map_enum_number(
      numbers, 24, value, "drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::milli_seconds_e_");
}

const char* drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::types_opts::to_string() const
{
  static const char* names[] = {"subMilliSeconds", "milliSeconds"};
  return convert_enum_idx(names, 2, value, "drx_cfg_secondary_group_r16_s::drx_on_dur_timer_r16_c_::types");
}

const char* drx_cfg_secondary_group_r16_s::drx_inactivity_timer_r16_opts::to_string() const
{
  static const char* names[] = {"ms0",    "ms1",    "ms2",    "ms3",    "ms4",    "ms5",    "ms6",    "ms8",
                                "ms10",   "ms20",   "ms30",   "ms40",   "ms50",   "ms60",   "ms80",   "ms100",
                                "ms200",  "ms300",  "ms500",  "ms750",  "ms1280", "ms1920", "ms2560", "spare9",
                                "spare8", "spare7", "spare6", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 32, value, "drx_cfg_secondary_group_r16_s::drx_inactivity_timer_r16_e_");
}
uint16_t drx_cfg_secondary_group_r16_s::drx_inactivity_timer_r16_opts::to_number() const
{
  static const uint16_t numbers[] = {0,  1,  2,  3,   4,   5,   6,   8,   10,   20,   30,  40,
                                     50, 60, 80, 100, 200, 300, 500, 750, 1280, 1920, 2560};
  return map_enum_number(numbers, 23, value, "drx_cfg_secondary_group_r16_s::drx_inactivity_timer_r16_e_");
}

// DRX-ConfigSL-r17 ::= SEQUENCE
OCUDUASN_CODE drx_cfg_sl_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, drx_harq_rtt_timer_sl_r17, (uint8_t)0u, (uint8_t)56u));
  HANDLE_CODE(drx_retx_timer_sl_r17.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_sl_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(drx_harq_rtt_timer_sl_r17, bref, (uint8_t)0u, (uint8_t)56u));
  HANDLE_CODE(drx_retx_timer_sl_r17.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void drx_cfg_sl_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("drx-HARQ-RTT-TimerSL-r17", drx_harq_rtt_timer_sl_r17);
  j.write_str("drx-RetransmissionTimerSL-r17", drx_retx_timer_sl_r17.to_string());
  j.end_obj();
}

const char* drx_cfg_sl_r17_s::drx_retx_timer_sl_r17_opts::to_string() const
{
  static const char* names[] = {"sl0",    "sl1",     "sl2",     "sl4",     "sl6",     "sl8",     "sl16",    "sl24",
                                "sl33",   "sl40",    "sl64",    "sl80",    "sl96",    "sl112",   "sl128",   "sl160",
                                "sl320",  "spare15", "spare14", "spare13", "spare12", "spare11", "spare10", "spare9",
                                "spare8", "spare7",  "spare6",  "spare5",  "spare4",  "spare3",  "spare2",  "spare1"};
  return convert_enum_idx(names, 32, value, "drx_cfg_sl_r17_s::drx_retx_timer_sl_r17_e_");
}
uint16_t drx_cfg_sl_r17_s::drx_retx_timer_sl_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {0, 1, 2, 4, 6, 8, 16, 24, 33, 40, 64, 80, 96, 112, 128, 160, 320};
  return map_enum_number(numbers, 17, value, "drx_cfg_sl_r17_s::drx_retx_timer_sl_r17_e_");
}

// DRX-ConfigExt-v1700 ::= SEQUENCE
OCUDUASN_CODE drx_cfg_ext_v1700_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, drx_harq_rtt_timer_dl_r17, (uint16_t)0u, (uint16_t)448u));
  HANDLE_CODE(pack_integer(bref, drx_harq_rtt_timer_ul_r17, (uint16_t)0u, (uint16_t)448u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_ext_v1700_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(drx_harq_rtt_timer_dl_r17, bref, (uint16_t)0u, (uint16_t)448u));
  HANDLE_CODE(unpack_integer(drx_harq_rtt_timer_ul_r17, bref, (uint16_t)0u, (uint16_t)448u));

  return OCUDUASN_SUCCESS;
}
void drx_cfg_ext_v1700_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("drx-HARQ-RTT-TimerDL-r17", drx_harq_rtt_timer_dl_r17);
  j.write_int("drx-HARQ-RTT-TimerUL-r17", drx_harq_rtt_timer_ul_r17);
  j.end_obj();
}

// SchedulingRequestToAddModExt-v1700 ::= SEQUENCE
OCUDUASN_CODE sched_request_to_add_mod_ext_v1700_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sr_prohibit_timer_v1700_present, 1));

  if (sr_prohibit_timer_v1700_present) {
    HANDLE_CODE(sr_prohibit_timer_v1700.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sched_request_to_add_mod_ext_v1700_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(sr_prohibit_timer_v1700_present, 1));

  if (sr_prohibit_timer_v1700_present) {
    HANDLE_CODE(sr_prohibit_timer_v1700.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void sched_request_to_add_mod_ext_v1700_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sr_prohibit_timer_v1700_present) {
    j.write_str("sr-ProhibitTimer-v1700", sr_prohibit_timer_v1700.to_string());
  }
  j.end_obj();
}

const char* sched_request_to_add_mod_ext_v1700_s::sr_prohibit_timer_v1700_opts::to_string() const
{
  static const char* names[] = {"ms192",
                                "ms256",
                                "ms320",
                                "ms384",
                                "ms448",
                                "ms512",
                                "ms576",
                                "ms640",
                                "ms1082",
                                "spare7",
                                "spare6",
                                "spare5",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(names, 16, value, "sched_request_to_add_mod_ext_v1700_s::sr_prohibit_timer_v1700_e_");
}
uint16_t sched_request_to_add_mod_ext_v1700_s::sr_prohibit_timer_v1700_opts::to_number() const
{
  static const uint16_t numbers[] = {192, 256, 320, 384, 448, 512, 576, 640, 1082};
  return map_enum_number(numbers, 9, value, "sched_request_to_add_mod_ext_v1700_s::sr_prohibit_timer_v1700_e_");
}

// SchedulingRequestConfig-v1700 ::= SEQUENCE
OCUDUASN_CODE sched_request_cfg_v1700_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(sched_request_to_add_mod_list_ext_v1700.size() > 0, 1));

  if (sched_request_to_add_mod_list_ext_v1700.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, sched_request_to_add_mod_list_ext_v1700, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sched_request_cfg_v1700_s::unpack(cbit_ref& bref)
{
  bool sched_request_to_add_mod_list_ext_v1700_present;
  HANDLE_CODE(bref.unpack(sched_request_to_add_mod_list_ext_v1700_present, 1));

  if (sched_request_to_add_mod_list_ext_v1700_present) {
    HANDLE_CODE(unpack_dyn_seq_of(sched_request_to_add_mod_list_ext_v1700, bref, 1, 8));
  }

  return OCUDUASN_SUCCESS;
}
void sched_request_cfg_v1700_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sched_request_to_add_mod_list_ext_v1700.size() > 0) {
    j.start_array("schedulingRequestToAddModListExt-v1700");
    for (const auto& e1 : sched_request_to_add_mod_list_ext_v1700) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// TAR-Config-r17 ::= SEQUENCE
OCUDUASN_CODE tar_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, offset_thres_ta_r17_present, timing_advance_sr_r17_present}));

  if (offset_thres_ta_r17_present) {
    HANDLE_CODE(offset_thres_ta_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tar_cfg_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &offset_thres_ta_r17_present, &timing_advance_sr_r17_present}));

  if (offset_thres_ta_r17_present) {
    HANDLE_CODE(offset_thres_ta_r17.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void tar_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (offset_thres_ta_r17_present) {
    j.write_str("offsetThresholdTA-r17", offset_thres_ta_r17.to_string());
  }
  if (timing_advance_sr_r17_present) {
    j.write_str("timingAdvanceSR-r17", "enabled");
  }
  j.end_obj();
}

const char* tar_cfg_r17_s::offset_thres_ta_r17_opts::to_string() const
{
  static const char* names[] = {"ms0dot5", "ms1",     "ms2",     "ms3",     "ms4",    "ms5",    "ms6",    "ms7",
                                "ms8",     "ms9",     "ms10",    "ms11",    "ms12",   "ms13",   "ms14",   "ms15",
                                "spare13", "spare12", "spare11", "spare10", "spare9", "spare8", "spare7", "spare6",
                                "spare5",  "spare4",  "spare3",  "spare2",  "spare1"};
  return convert_enum_idx(names, 29, value, "tar_cfg_r17_s::offset_thres_ta_r17_e_");
}
float tar_cfg_r17_s::offset_thres_ta_r17_opts::to_number() const
{
  static const float numbers[] = {0.5, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0};
  return map_enum_number(numbers, 16, value, "tar_cfg_r17_s::offset_thres_ta_r17_e_");
}
const char* tar_cfg_r17_s::offset_thres_ta_r17_opts::to_number_string() const
{
  static const char* number_strs[] = {
      "0.5", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
  return convert_enum_idx(number_strs, 29, value, "tar_cfg_r17_s::offset_thres_ta_r17_e_");
}

// DRX-ConfigPTM-r17 ::= SEQUENCE
OCUDUASN_CODE drx_cfg_ptm_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {drx_harq_rtt_timer_dl_ptm_r17_present, drx_retx_timer_dl_ptm_r17_present}));

  HANDLE_CODE(drx_on_dur_timer_ptm_r17.pack(bref));
  HANDLE_CODE(drx_inactivity_timer_ptm_r17.pack(bref));
  if (drx_harq_rtt_timer_dl_ptm_r17_present) {
    HANDLE_CODE(pack_integer(bref, drx_harq_rtt_timer_dl_ptm_r17, (uint8_t)0u, (uint8_t)56u));
  }
  if (drx_retx_timer_dl_ptm_r17_present) {
    HANDLE_CODE(drx_retx_timer_dl_ptm_r17.pack(bref));
  }
  HANDLE_CODE(drx_long_cycle_start_offset_ptm_r17.pack(bref));
  HANDLE_CODE(pack_integer(bref, drx_slot_offset_ptm_r17, (uint8_t)0u, (uint8_t)31u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_ptm_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&drx_harq_rtt_timer_dl_ptm_r17_present, &drx_retx_timer_dl_ptm_r17_present}));

  HANDLE_CODE(drx_on_dur_timer_ptm_r17.unpack(bref));
  HANDLE_CODE(drx_inactivity_timer_ptm_r17.unpack(bref));
  if (drx_harq_rtt_timer_dl_ptm_r17_present) {
    HANDLE_CODE(unpack_integer(drx_harq_rtt_timer_dl_ptm_r17, bref, (uint8_t)0u, (uint8_t)56u));
  }
  if (drx_retx_timer_dl_ptm_r17_present) {
    HANDLE_CODE(drx_retx_timer_dl_ptm_r17.unpack(bref));
  }
  HANDLE_CODE(drx_long_cycle_start_offset_ptm_r17.unpack(bref));
  HANDLE_CODE(unpack_integer(drx_slot_offset_ptm_r17, bref, (uint8_t)0u, (uint8_t)31u));

  return OCUDUASN_SUCCESS;
}
void drx_cfg_ptm_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("drx-onDurationTimerPTM-r17");
  drx_on_dur_timer_ptm_r17.to_json(j);
  j.write_str("drx-InactivityTimerPTM-r17", drx_inactivity_timer_ptm_r17.to_string());
  if (drx_harq_rtt_timer_dl_ptm_r17_present) {
    j.write_int("drx-HARQ-RTT-TimerDL-PTM-r17", drx_harq_rtt_timer_dl_ptm_r17);
  }
  if (drx_retx_timer_dl_ptm_r17_present) {
    j.write_str("drx-RetransmissionTimerDL-PTM-r17", drx_retx_timer_dl_ptm_r17.to_string());
  }
  j.write_fieldname("drx-LongCycleStartOffsetPTM-r17");
  drx_long_cycle_start_offset_ptm_r17.to_json(j);
  j.write_int("drx-SlotOffsetPTM-r17", drx_slot_offset_ptm_r17);
  j.end_obj();
}

void drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::destroy_() {}
void drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::drx_on_dur_timer_ptm_r17_c_(
    const drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::sub_milli_seconds:
      c.init(other.c.get<uint8_t>());
      break;
    case types::milli_seconds:
      c.init(other.c.get<milli_seconds_e_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_");
  }
}
drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_&
drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::operator=(const drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::sub_milli_seconds:
      c.set(other.c.get<uint8_t>());
      break;
    case types::milli_seconds:
      c.set(other.c.get<milli_seconds_e_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_");
  }

  return *this;
}
uint8_t& drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::set_sub_milli_seconds()
{
  set(types::sub_milli_seconds);
  return c.get<uint8_t>();
}
drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::milli_seconds_e_&
drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::set_milli_seconds()
{
  set(types::milli_seconds);
  return c.get<milli_seconds_e_>();
}
void drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::sub_milli_seconds:
      j.write_int("subMilliSeconds", c.get<uint8_t>());
      break;
    case types::milli_seconds:
      j.write_str("milliSeconds", c.get<milli_seconds_e_>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::sub_milli_seconds:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)31u));
      break;
    case types::milli_seconds:
      HANDLE_CODE(c.get<milli_seconds_e_>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::sub_milli_seconds:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)31u));
      break;
    case types::milli_seconds:
      HANDLE_CODE(c.get<milli_seconds_e_>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::milli_seconds_opts::to_string() const
{
  static const char* names[] = {"ms1",    "ms2",    "ms3",    "ms4",    "ms5",    "ms6",    "ms8",    "ms10",
                                "ms20",   "ms30",   "ms40",   "ms50",   "ms60",   "ms80",   "ms100",  "ms200",
                                "ms300",  "ms400",  "ms500",  "ms600",  "ms800",  "ms1000", "ms1200", "ms1600",
                                "spare8", "spare7", "spare6", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 32, value, "drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::milli_seconds_e_");
}
uint16_t drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::milli_seconds_opts::to_number() const
{
  static const uint16_t numbers[] = {1,  2,  3,   4,   5,   6,   8,   10,  20,  30,   40,   50,
                                     60, 80, 100, 200, 300, 400, 500, 600, 800, 1000, 1200, 1600};
  return map_enum_number(numbers, 24, value, "drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::milli_seconds_e_");
}

const char* drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::types_opts::to_string() const
{
  static const char* names[] = {"subMilliSeconds", "milliSeconds"};
  return convert_enum_idx(names, 2, value, "drx_cfg_ptm_r17_s::drx_on_dur_timer_ptm_r17_c_::types");
}

const char* drx_cfg_ptm_r17_s::drx_inactivity_timer_ptm_r17_opts::to_string() const
{
  static const char* names[] = {"ms0",    "ms1",    "ms2",    "ms3",    "ms4",    "ms5",    "ms6",    "ms8",
                                "ms10",   "ms20",   "ms30",   "ms40",   "ms50",   "ms60",   "ms80",   "ms100",
                                "ms200",  "ms300",  "ms500",  "ms750",  "ms1280", "ms1920", "ms2560", "spare9",
                                "spare8", "spare7", "spare6", "spare5", "spare4", "spare3", "spare2", "spare1"};
  return convert_enum_idx(names, 32, value, "drx_cfg_ptm_r17_s::drx_inactivity_timer_ptm_r17_e_");
}
uint16_t drx_cfg_ptm_r17_s::drx_inactivity_timer_ptm_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {0,  1,  2,  3,   4,   5,   6,   8,   10,   20,   30,  40,
                                     50, 60, 80, 100, 200, 300, 500, 750, 1280, 1920, 2560};
  return map_enum_number(numbers, 23, value, "drx_cfg_ptm_r17_s::drx_inactivity_timer_ptm_r17_e_");
}

const char* drx_cfg_ptm_r17_s::drx_retx_timer_dl_ptm_r17_opts::to_string() const
{
  static const char* names[] = {"sl0",    "sl1",     "sl2",     "sl4",     "sl6",     "sl8",     "sl16",    "sl24",
                                "sl33",   "sl40",    "sl64",    "sl80",    "sl96",    "sl112",   "sl128",   "sl160",
                                "sl320",  "spare15", "spare14", "spare13", "spare12", "spare11", "spare10", "spare9",
                                "spare8", "spare7",  "spare6",  "spare5",  "spare4",  "spare3",  "spare2",  "spare1"};
  return convert_enum_idx(names, 32, value, "drx_cfg_ptm_r17_s::drx_retx_timer_dl_ptm_r17_e_");
}
uint16_t drx_cfg_ptm_r17_s::drx_retx_timer_dl_ptm_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {0, 1, 2, 4, 6, 8, 16, 24, 33, 40, 64, 80, 96, 112, 128, 160, 320};
  return map_enum_number(numbers, 17, value, "drx_cfg_ptm_r17_s::drx_retx_timer_dl_ptm_r17_e_");
}

void drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::destroy_() {}
void drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::drx_long_cycle_start_offset_ptm_r17_c_(
    const drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ms10:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms20:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms32:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms40:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms60:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms64:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms70:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms80:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms128:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms160:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms256:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms320:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms512:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms640:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms1024:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms1280:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms2048:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms2560:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms5120:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms10240:
      c.init(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_");
  }
}
drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_&
drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::operator=(
    const drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ms10:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms20:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms32:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms40:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms60:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms64:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms70:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms80:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms128:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms160:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms256:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms320:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms512:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms640:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms1024:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms1280:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms2048:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms2560:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms5120:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms10240:
      c.set(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_");
  }

  return *this;
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms10()
{
  set(types::ms10);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms20()
{
  set(types::ms20);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms32()
{
  set(types::ms32);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms40()
{
  set(types::ms40);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms60()
{
  set(types::ms60);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms64()
{
  set(types::ms64);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms70()
{
  set(types::ms70);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms80()
{
  set(types::ms80);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms128()
{
  set(types::ms128);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms160()
{
  set(types::ms160);
  return c.get<uint8_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms256()
{
  set(types::ms256);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms320()
{
  set(types::ms320);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms512()
{
  set(types::ms512);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms640()
{
  set(types::ms640);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms1024()
{
  set(types::ms1024);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms1280()
{
  set(types::ms1280);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms2048()
{
  set(types::ms2048);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms2560()
{
  set(types::ms2560);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms5120()
{
  set(types::ms5120);
  return c.get<uint16_t>();
}
uint16_t& drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::set_ms10240()
{
  set(types::ms10240);
  return c.get<uint16_t>();
}
void drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ms10:
      j.write_int("ms10", c.get<uint8_t>());
      break;
    case types::ms20:
      j.write_int("ms20", c.get<uint8_t>());
      break;
    case types::ms32:
      j.write_int("ms32", c.get<uint8_t>());
      break;
    case types::ms40:
      j.write_int("ms40", c.get<uint8_t>());
      break;
    case types::ms60:
      j.write_int("ms60", c.get<uint8_t>());
      break;
    case types::ms64:
      j.write_int("ms64", c.get<uint8_t>());
      break;
    case types::ms70:
      j.write_int("ms70", c.get<uint8_t>());
      break;
    case types::ms80:
      j.write_int("ms80", c.get<uint8_t>());
      break;
    case types::ms128:
      j.write_int("ms128", c.get<uint8_t>());
      break;
    case types::ms160:
      j.write_int("ms160", c.get<uint8_t>());
      break;
    case types::ms256:
      j.write_int("ms256", c.get<uint16_t>());
      break;
    case types::ms320:
      j.write_int("ms320", c.get<uint16_t>());
      break;
    case types::ms512:
      j.write_int("ms512", c.get<uint16_t>());
      break;
    case types::ms640:
      j.write_int("ms640", c.get<uint16_t>());
      break;
    case types::ms1024:
      j.write_int("ms1024", c.get<uint16_t>());
      break;
    case types::ms1280:
      j.write_int("ms1280", c.get<uint16_t>());
      break;
    case types::ms2048:
      j.write_int("ms2048", c.get<uint16_t>());
      break;
    case types::ms2560:
      j.write_int("ms2560", c.get<uint16_t>());
      break;
    case types::ms5120:
      j.write_int("ms5120", c.get<uint16_t>());
      break;
    case types::ms10240:
      j.write_int("ms10240", c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ms10:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)9u));
      break;
    case types::ms20:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)19u));
      break;
    case types::ms32:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)31u));
      break;
    case types::ms40:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)39u));
      break;
    case types::ms60:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)59u));
      break;
    case types::ms64:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)63u));
      break;
    case types::ms70:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)69u));
      break;
    case types::ms80:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)79u));
      break;
    case types::ms128:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)127u));
      break;
    case types::ms160:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)159u));
      break;
    case types::ms256:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u));
      break;
    case types::ms320:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)319u));
      break;
    case types::ms512:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)511u));
      break;
    case types::ms640:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)639u));
      break;
    case types::ms1024:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::ms1280:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1279u));
      break;
    case types::ms2048:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)2047u));
      break;
    case types::ms2560:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)2559u));
      break;
    case types::ms5120:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)5119u));
      break;
    case types::ms10240:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)10239u));
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ms10:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)9u));
      break;
    case types::ms20:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)19u));
      break;
    case types::ms32:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)31u));
      break;
    case types::ms40:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)39u));
      break;
    case types::ms60:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)59u));
      break;
    case types::ms64:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)63u));
      break;
    case types::ms70:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)69u));
      break;
    case types::ms80:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)79u));
      break;
    case types::ms128:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)127u));
      break;
    case types::ms160:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)159u));
      break;
    case types::ms256:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u));
      break;
    case types::ms320:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)319u));
      break;
    case types::ms512:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)511u));
      break;
    case types::ms640:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)639u));
      break;
    case types::ms1024:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::ms1280:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1279u));
      break;
    case types::ms2048:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)2047u));
      break;
    case types::ms2560:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)2559u));
      break;
    case types::ms5120:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)5119u));
      break;
    case types::ms10240:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)10239u));
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::types_opts::to_string() const
{
  static const char* names[] = {"ms10",   "ms20",   "ms32",   "ms40",   "ms60",   "ms64",   "ms70",
                                "ms80",   "ms128",  "ms160",  "ms256",  "ms320",  "ms512",  "ms640",
                                "ms1024", "ms1280", "ms2048", "ms2560", "ms5120", "ms10240"};
  return convert_enum_idx(names, 20, value, "drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::types");
}
uint16_t drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::types_opts::to_number() const
{
  static const uint16_t numbers[] = {10,  20,  32,  40,  60,   64,   70,   80,   128,  160,
                                     256, 320, 512, 640, 1024, 1280, 2048, 2560, 5120, 10240};
  return map_enum_number(numbers, 20, value, "drx_cfg_ptm_r17_s::drx_long_cycle_start_offset_ptm_r17_c_::types");
}

// MBS-RNTI-SpecificConfig-r17 ::= SEQUENCE
OCUDUASN_CODE mbs_rnti_specific_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {drx_cfg_ptm_r17_present,
                         harq_feedback_enabler_multicast_r17_present,
                         harq_feedback_option_multicast_r17_present,
                         pdsch_aggregation_factor_r17_present}));

  HANDLE_CODE(pack_integer(bref, mbs_rnti_specific_cfg_id_r17, (uint8_t)0u, (uint8_t)15u));
  HANDLE_CODE(group_common_rnti_r17.pack(bref));
  if (drx_cfg_ptm_r17_present) {
    HANDLE_CODE(drx_cfg_ptm_r17.pack(bref));
  }
  if (harq_feedback_enabler_multicast_r17_present) {
    HANDLE_CODE(harq_feedback_enabler_multicast_r17.pack(bref));
  }
  if (harq_feedback_option_multicast_r17_present) {
    HANDLE_CODE(harq_feedback_option_multicast_r17.pack(bref));
  }
  if (pdsch_aggregation_factor_r17_present) {
    HANDLE_CODE(pdsch_aggregation_factor_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_rnti_specific_cfg_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref,
                          {&drx_cfg_ptm_r17_present,
                           &harq_feedback_enabler_multicast_r17_present,
                           &harq_feedback_option_multicast_r17_present,
                           &pdsch_aggregation_factor_r17_present}));

  HANDLE_CODE(unpack_integer(mbs_rnti_specific_cfg_id_r17, bref, (uint8_t)0u, (uint8_t)15u));
  HANDLE_CODE(group_common_rnti_r17.unpack(bref));
  if (drx_cfg_ptm_r17_present) {
    HANDLE_CODE(drx_cfg_ptm_r17.unpack(bref));
  }
  if (harq_feedback_enabler_multicast_r17_present) {
    HANDLE_CODE(harq_feedback_enabler_multicast_r17.unpack(bref));
  }
  if (harq_feedback_option_multicast_r17_present) {
    HANDLE_CODE(harq_feedback_option_multicast_r17.unpack(bref));
  }
  if (pdsch_aggregation_factor_r17_present) {
    HANDLE_CODE(pdsch_aggregation_factor_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void mbs_rnti_specific_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("mbs-RNTI-SpecificConfigId-r17", mbs_rnti_specific_cfg_id_r17);
  j.write_fieldname("groupCommon-RNTI-r17");
  group_common_rnti_r17.to_json(j);
  if (drx_cfg_ptm_r17_present) {
    j.write_fieldname("drx-ConfigPTM-r17");
    drx_cfg_ptm_r17.to_json(j);
  }
  if (harq_feedback_enabler_multicast_r17_present) {
    j.write_str("harq-FeedbackEnablerMulticast-r17", harq_feedback_enabler_multicast_r17.to_string());
  }
  if (harq_feedback_option_multicast_r17_present) {
    j.write_str("harq-FeedbackOptionMulticast-r17", harq_feedback_option_multicast_r17.to_string());
  }
  if (pdsch_aggregation_factor_r17_present) {
    j.write_str("pdsch-AggregationFactor-r17", pdsch_aggregation_factor_r17.to_string());
  }
  j.end_obj();
}

void mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::destroy_() {}
void mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::group_common_rnti_r17_c_(
    const mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::g_rnti:
      c.init(other.c.get<uint32_t>());
      break;
    case types::g_cs_rnti:
      c.init(other.c.get<uint32_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_");
  }
}
mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_& mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::operator=(
    const mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::g_rnti:
      c.set(other.c.get<uint32_t>());
      break;
    case types::g_cs_rnti:
      c.set(other.c.get<uint32_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_");
  }

  return *this;
}
uint32_t& mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::set_g_rnti()
{
  set(types::g_rnti);
  return c.get<uint32_t>();
}
uint32_t& mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::set_g_cs_rnti()
{
  set(types::g_cs_rnti);
  return c.get<uint32_t>();
}
void mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::g_rnti:
      j.write_int("g-RNTI", c.get<uint32_t>());
      break;
    case types::g_cs_rnti:
      j.write_int("g-CS-RNTI", c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::g_rnti:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)65535u));
      break;
    case types::g_cs_rnti:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)65535u));
      break;
    default:
      log_invalid_choice_id(type_, "mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::g_rnti:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)65535u));
      break;
    case types::g_cs_rnti:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)65535u));
      break;
    default:
      log_invalid_choice_id(type_, "mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::types_opts::to_string() const
{
  static const char* names[] = {"g-RNTI", "g-CS-RNTI"};
  return convert_enum_idx(names, 2, value, "mbs_rnti_specific_cfg_r17_s::group_common_rnti_r17_c_::types");
}

const char* mbs_rnti_specific_cfg_r17_s::harq_feedback_enabler_multicast_r17_opts::to_string() const
{
  static const char* names[] = {"dci-enabler", "enabled"};
  return convert_enum_idx(names, 2, value, "mbs_rnti_specific_cfg_r17_s::harq_feedback_enabler_multicast_r17_e_");
}

const char* mbs_rnti_specific_cfg_r17_s::harq_feedback_option_multicast_r17_opts::to_string() const
{
  static const char* names[] = {"ack-nack", "nack-only"};
  return convert_enum_idx(names, 2, value, "mbs_rnti_specific_cfg_r17_s::harq_feedback_option_multicast_r17_e_");
}

const char* mbs_rnti_specific_cfg_r17_s::pdsch_aggregation_factor_r17_opts::to_string() const
{
  static const char* names[] = {"n2", "n4", "n8"};
  return convert_enum_idx(names, 3, value, "mbs_rnti_specific_cfg_r17_s::pdsch_aggregation_factor_r17_e_");
}
uint8_t mbs_rnti_specific_cfg_r17_s::pdsch_aggregation_factor_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 8};
  return map_enum_number(numbers, 3, value, "mbs_rnti_specific_cfg_r17_s::pdsch_aggregation_factor_r17_e_");
}

// DRX-ConfigExt2-v1800 ::= SEQUENCE
OCUDUASN_CODE drx_cfg_ext2_v1800_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {short_drx_r18_present, drx_time_ref_sfn_r18_present}));

  HANDLE_CODE(drx_non_integer_long_cycle_start_offset_r18.pack(bref));
  if (short_drx_r18_present) {
    HANDLE_CODE(short_drx_r18.drx_non_integer_short_cycle_r18.pack(bref));
    HANDLE_CODE(pack_integer(bref, short_drx_r18.drx_short_cycle_timer_r18, (uint8_t)1u, (uint8_t)16u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_ext2_v1800_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&short_drx_r18_present, &drx_time_ref_sfn_r18_present}));

  HANDLE_CODE(drx_non_integer_long_cycle_start_offset_r18.unpack(bref));
  if (short_drx_r18_present) {
    HANDLE_CODE(short_drx_r18.drx_non_integer_short_cycle_r18.unpack(bref));
    HANDLE_CODE(unpack_integer(short_drx_r18.drx_short_cycle_timer_r18, bref, (uint8_t)1u, (uint8_t)16u));
  }

  return OCUDUASN_SUCCESS;
}
void drx_cfg_ext2_v1800_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("drx-NonIntegerLongCycleStartOffset-r18");
  drx_non_integer_long_cycle_start_offset_r18.to_json(j);
  if (short_drx_r18_present) {
    j.write_fieldname("shortDRX-r18");
    j.start_obj();
    j.write_str("drx-NonIntegerShortCycle-r18", short_drx_r18.drx_non_integer_short_cycle_r18.to_string());
    j.write_int("drx-ShortCycleTimer-r18", short_drx_r18.drx_short_cycle_timer_r18);
    j.end_obj();
  }
  if (drx_time_ref_sfn_r18_present) {
    j.write_str("drx-TimeReferenceSFN-r18", "sfn512");
  }
  j.end_obj();
}

void drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::destroy_() {}
void drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::drx_non_integer_long_cycle_start_offset_r18_c_(
    const drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ms1001over240:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms25over6:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms25over3:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms1001over120:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms100over9:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms25over2:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms40over3:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms125over9:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms50over3:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms1001over60:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms125over6:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms200over9:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms250over9:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms100over3:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms1001over30:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms75over2:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms125over3:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms1001over24:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms200over3:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms1001over15:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms250over3:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms1001over12:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms400over3:
      c.init(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_");
  }
}
drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_&
drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::operator=(
    const drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ms1001over240:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms25over6:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms25over3:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms1001over120:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms100over9:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms25over2:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms40over3:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms125over9:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms50over3:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms1001over60:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms125over6:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms200over9:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms250over9:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms100over3:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms1001over30:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms75over2:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms125over3:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms1001over24:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms200over3:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms1001over15:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms250over3:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms1001over12:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms400over3:
      c.set(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_");
  }

  return *this;
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms1001over240()
{
  set(types::ms1001over240);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms25over6()
{
  set(types::ms25over6);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms25over3()
{
  set(types::ms25over3);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms1001over120()
{
  set(types::ms1001over120);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms100over9()
{
  set(types::ms100over9);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms25over2()
{
  set(types::ms25over2);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms40over3()
{
  set(types::ms40over3);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms125over9()
{
  set(types::ms125over9);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms50over3()
{
  set(types::ms50over3);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms1001over60()
{
  set(types::ms1001over60);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms125over6()
{
  set(types::ms125over6);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms200over9()
{
  set(types::ms200over9);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms250over9()
{
  set(types::ms250over9);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms100over3()
{
  set(types::ms100over3);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms1001over30()
{
  set(types::ms1001over30);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms75over2()
{
  set(types::ms75over2);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms125over3()
{
  set(types::ms125over3);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms1001over24()
{
  set(types::ms1001over24);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms200over3()
{
  set(types::ms200over3);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms1001over15()
{
  set(types::ms1001over15);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms250over3()
{
  set(types::ms250over3);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms1001over12()
{
  set(types::ms1001over12);
  return c.get<uint8_t>();
}
uint8_t& drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::set_ms400over3()
{
  set(types::ms400over3);
  return c.get<uint8_t>();
}
void drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ms1001over240:
      j.write_int("ms1001over240", c.get<uint8_t>());
      break;
    case types::ms25over6:
      j.write_int("ms25over6", c.get<uint8_t>());
      break;
    case types::ms25over3:
      j.write_int("ms25over3", c.get<uint8_t>());
      break;
    case types::ms1001over120:
      j.write_int("ms1001over120", c.get<uint8_t>());
      break;
    case types::ms100over9:
      j.write_int("ms100over9", c.get<uint8_t>());
      break;
    case types::ms25over2:
      j.write_int("ms25over2", c.get<uint8_t>());
      break;
    case types::ms40over3:
      j.write_int("ms40over3", c.get<uint8_t>());
      break;
    case types::ms125over9:
      j.write_int("ms125over9", c.get<uint8_t>());
      break;
    case types::ms50over3:
      j.write_int("ms50over3", c.get<uint8_t>());
      break;
    case types::ms1001over60:
      j.write_int("ms1001over60", c.get<uint8_t>());
      break;
    case types::ms125over6:
      j.write_int("ms125over6", c.get<uint8_t>());
      break;
    case types::ms200over9:
      j.write_int("ms200over9", c.get<uint8_t>());
      break;
    case types::ms250over9:
      j.write_int("ms250over9", c.get<uint8_t>());
      break;
    case types::ms100over3:
      j.write_int("ms100over3", c.get<uint8_t>());
      break;
    case types::ms1001over30:
      j.write_int("ms1001over30", c.get<uint8_t>());
      break;
    case types::ms75over2:
      j.write_int("ms75over2", c.get<uint8_t>());
      break;
    case types::ms125over3:
      j.write_int("ms125over3", c.get<uint8_t>());
      break;
    case types::ms1001over24:
      j.write_int("ms1001over24", c.get<uint8_t>());
      break;
    case types::ms200over3:
      j.write_int("ms200over3", c.get<uint8_t>());
      break;
    case types::ms1001over15:
      j.write_int("ms1001over15", c.get<uint8_t>());
      break;
    case types::ms250over3:
      j.write_int("ms250over3", c.get<uint8_t>());
      break;
    case types::ms1001over12:
      j.write_int("ms1001over12", c.get<uint8_t>());
      break;
    case types::ms400over3:
      j.write_int("ms400over3", c.get<uint8_t>());
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ms1001over240:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)3u));
      break;
    case types::ms25over6:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)3u));
      break;
    case types::ms25over3:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)7u));
      break;
    case types::ms1001over120:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)7u));
      break;
    case types::ms100over9:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)10u));
      break;
    case types::ms25over2:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)11u));
      break;
    case types::ms40over3:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)12u));
      break;
    case types::ms125over9:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)12u));
      break;
    case types::ms50over3:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)15u));
      break;
    case types::ms1001over60:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)15u));
      break;
    case types::ms125over6:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)19u));
      break;
    case types::ms200over9:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)21u));
      break;
    case types::ms250over9:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)26u));
      break;
    case types::ms100over3:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)32u));
      break;
    case types::ms1001over30:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)32u));
      break;
    case types::ms75over2:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)36u));
      break;
    case types::ms125over3:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)40u));
      break;
    case types::ms1001over24:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)40u));
      break;
    case types::ms200over3:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)65u));
      break;
    case types::ms1001over15:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)65u));
      break;
    case types::ms250over3:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)82u));
      break;
    case types::ms1001over12:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)82u));
      break;
    case types::ms400over3:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)132u));
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ms1001over240:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)3u));
      break;
    case types::ms25over6:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)3u));
      break;
    case types::ms25over3:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)7u));
      break;
    case types::ms1001over120:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)7u));
      break;
    case types::ms100over9:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)10u));
      break;
    case types::ms25over2:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)11u));
      break;
    case types::ms40over3:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)12u));
      break;
    case types::ms125over9:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)12u));
      break;
    case types::ms50over3:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)15u));
      break;
    case types::ms1001over60:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)15u));
      break;
    case types::ms125over6:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)19u));
      break;
    case types::ms200over9:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)21u));
      break;
    case types::ms250over9:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)26u));
      break;
    case types::ms100over3:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)32u));
      break;
    case types::ms1001over30:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)32u));
      break;
    case types::ms75over2:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)36u));
      break;
    case types::ms125over3:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)40u));
      break;
    case types::ms1001over24:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)40u));
      break;
    case types::ms200over3:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)65u));
      break;
    case types::ms1001over15:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)65u));
      break;
    case types::ms250over3:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)82u));
      break;
    case types::ms1001over12:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)82u));
      break;
    case types::ms400over3:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)132u));
      break;
    default:
      log_invalid_choice_id(type_, "drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"ms1001over240", "ms25over6",    "ms25over3",    "ms1001over120", "ms100over9",
                                "ms25over2",     "ms40over3",    "ms125over9",   "ms50over3",     "ms1001over60",
                                "ms125over6",    "ms200over9",   "ms250over9",   "ms100over3",    "ms1001over30",
                                "ms75over2",     "ms125over3",   "ms1001over24", "ms200over3",    "ms1001over15",
                                "ms250over3",    "ms1001over12", "ms400over3"};
  return convert_enum_idx(
      names, 23, value, "drx_cfg_ext2_v1800_s::drx_non_integer_long_cycle_start_offset_r18_c_::types");
}

const char* drx_cfg_ext2_v1800_s::short_drx_r18_s_::drx_non_integer_short_cycle_r18_opts::to_string() const
{
  static const char* names[] = {
      "ms1001over240", "ms25over6",    "ms25over3",    "ms1001over120", "ms100over9", "ms25over2",  "ms40over3",
      "ms125over9",    "ms50over3",    "ms1001over60", "ms125over6",    "ms200over9", "ms100over3", "ms1001over30",
      "ms125over3",    "ms1001over24", "ms200over3",   "spare15",       "spare14",    "spare13",    "spare12",
      "spare11",       "spare10",      "spare9",       "spare8",        "spare7",     "spare6",     "spare5",
      "spare4",        "spare3",       "spare2",       "spare1"};
  return convert_enum_idx(
      names, 32, value, "drx_cfg_ext2_v1800_s::short_drx_r18_s_::drx_non_integer_short_cycle_r18_e_");
}

// LCG-DSR-Config-r18 ::= SEQUENCE
OCUDUASN_CODE lcg_dsr_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pack_integer(bref, lcg_id_r18, (uint8_t)0u, (uint8_t)7u));
  HANDLE_CODE(pack_integer(bref, remaining_time_thres_r18, (uint8_t)1u, (uint8_t)64u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE lcg_dsr_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_integer(lcg_id_r18, bref, (uint8_t)0u, (uint8_t)7u));
  HANDLE_CODE(unpack_integer(remaining_time_thres_r18, bref, (uint8_t)1u, (uint8_t)64u));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void lcg_dsr_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("lcg-Id-r18", lcg_id_r18);
  j.write_int("remainingTimeThreshold-r18", remaining_time_thres_r18);
  j.end_obj();
}

// TAR-Config-r18 ::= SEQUENCE
OCUDUASN_CODE tar_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, offset_thres_ta_r18_present, timing_advance_sr_r18_present}));

  if (offset_thres_ta_r18_present) {
    HANDLE_CODE(pack_integer(bref, offset_thres_ta_r18, (uint8_t)1u, (uint8_t)56u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE tar_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &offset_thres_ta_r18_present, &timing_advance_sr_r18_present}));

  if (offset_thres_ta_r18_present) {
    HANDLE_CODE(unpack_integer(offset_thres_ta_r18, bref, (uint8_t)1u, (uint8_t)56u));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void tar_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (offset_thres_ta_r18_present) {
    j.write_int("offsetThresholdTA-r18", offset_thres_ta_r18);
  }
  if (timing_advance_sr_r18_present) {
    j.write_str("timingAdvanceSR-r18", "enabled");
  }
  j.end_obj();
}

// MAC-CellGroupConfig ::= SEQUENCE
OCUDUASN_CODE mac_cell_group_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(7);
  group_flags[0] |= csi_mask_present;
  group_flags[0] |= data_inactivity_timer.is_present();
  group_flags[1] |= use_pre_bsr_r16_present;
  group_flags[1] |= sched_request_id_lbt_scell_r16_present;
  group_flags[1] |= lch_based_prioritization_r16_present;
  group_flags[1] |= sched_request_id_bfr_scell_r16_present;
  group_flags[1] |= drx_cfg_secondary_group_r16.is_present();
  group_flags[2] |= enhanced_skip_ul_tx_dyn_r16_present;
  group_flags[2] |= enhanced_skip_ul_tx_cfg_r16_present;
  group_flags[3] |= intra_cg_prioritization_r17_present;
  group_flags[3] |= drx_cfg_sl_r17.is_present();
  group_flags[3] |= drx_cfg_ext_v1700.is_present();
  group_flags[3] |= sched_request_id_bfr_r17_present;
  group_flags[3] |= sched_request_id_bfr2_r17_present;
  group_flags[3] |= sched_request_cfg_v1700.is_present();
  group_flags[3] |= tar_cfg_r17.is_present();
  group_flags[3] |= g_rnti_cfg_to_add_mod_list_r17.is_present();
  group_flags[3] |= g_rnti_cfg_to_release_list_r17.is_present();
  group_flags[3] |= g_cs_rnti_cfg_to_add_mod_list_r17.is_present();
  group_flags[3] |= g_cs_rnti_cfg_to_release_list_r17.is_present();
  group_flags[3] |= allow_csi_srs_tx_multicast_drx_active_r17_present;
  group_flags[4] |= sched_request_id_pos_mg_request_r17_present;
  group_flags[4] |= drx_last_tx_ul_r17_present;
  group_flags[5] |= pos_mg_request_r17_present;
  group_flags[6] |= drx_cfg_ext2_v1800.is_present();
  group_flags[6] |= add_bs_table_allowed_r18_present;
  group_flags[6] |= dsr_cfg_to_add_mod_list_r18.is_present();
  group_flags[6] |= dsr_cfg_to_release_list_r18.is_present();
  group_flags[6] |= tar_cfg_r18.is_present();
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         drx_cfg_present,
                         sched_request_cfg_present,
                         bsr_cfg_present,
                         tag_cfg_present,
                         phr_cfg_present}));

  if (drx_cfg_present) {
    HANDLE_CODE(drx_cfg.pack(bref));
  }
  if (sched_request_cfg_present) {
    HANDLE_CODE(sched_request_cfg.pack(bref));
  }
  if (bsr_cfg_present) {
    HANDLE_CODE(bsr_cfg.pack(bref));
  }
  if (tag_cfg_present) {
    HANDLE_CODE(tag_cfg.pack(bref));
  }
  if (phr_cfg_present) {
    HANDLE_CODE(phr_cfg.pack(bref));
  }
  HANDLE_CODE(bref.pack(skip_ul_tx_dyn, 1));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {csi_mask_present, data_inactivity_timer.is_present()}));
      if (csi_mask_present) {
        HANDLE_CODE(bref.pack(csi_mask, 1));
      }
      if (data_inactivity_timer.is_present()) {
        HANDLE_CODE(data_inactivity_timer->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {use_pre_bsr_r16_present,
                             sched_request_id_lbt_scell_r16_present,
                             lch_based_prioritization_r16_present,
                             sched_request_id_bfr_scell_r16_present,
                             drx_cfg_secondary_group_r16.is_present()}));
      if (sched_request_id_lbt_scell_r16_present) {
        HANDLE_CODE(pack_integer(bref, sched_request_id_lbt_scell_r16, (uint8_t)0u, (uint8_t)7u));
      }
      if (sched_request_id_bfr_scell_r16_present) {
        HANDLE_CODE(pack_integer(bref, sched_request_id_bfr_scell_r16, (uint8_t)0u, (uint8_t)7u));
      }
      if (drx_cfg_secondary_group_r16.is_present()) {
        HANDLE_CODE(drx_cfg_secondary_group_r16->pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {enhanced_skip_ul_tx_dyn_r16_present, enhanced_skip_ul_tx_cfg_r16_present}));
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {intra_cg_prioritization_r17_present,
                             drx_cfg_sl_r17.is_present(),
                             drx_cfg_ext_v1700.is_present(),
                             sched_request_id_bfr_r17_present,
                             sched_request_id_bfr2_r17_present,
                             sched_request_cfg_v1700.is_present(),
                             tar_cfg_r17.is_present(),
                             g_rnti_cfg_to_add_mod_list_r17.is_present(),
                             g_rnti_cfg_to_release_list_r17.is_present(),
                             g_cs_rnti_cfg_to_add_mod_list_r17.is_present(),
                             g_cs_rnti_cfg_to_release_list_r17.is_present(),
                             allow_csi_srs_tx_multicast_drx_active_r17_present}));
      if (drx_cfg_sl_r17.is_present()) {
        HANDLE_CODE(drx_cfg_sl_r17->pack(bref));
      }
      if (drx_cfg_ext_v1700.is_present()) {
        HANDLE_CODE(drx_cfg_ext_v1700->pack(bref));
      }
      if (sched_request_id_bfr_r17_present) {
        HANDLE_CODE(pack_integer(bref, sched_request_id_bfr_r17, (uint8_t)0u, (uint8_t)7u));
      }
      if (sched_request_id_bfr2_r17_present) {
        HANDLE_CODE(pack_integer(bref, sched_request_id_bfr2_r17, (uint8_t)0u, (uint8_t)7u));
      }
      if (sched_request_cfg_v1700.is_present()) {
        HANDLE_CODE(sched_request_cfg_v1700->pack(bref));
      }
      if (tar_cfg_r17.is_present()) {
        HANDLE_CODE(tar_cfg_r17->pack(bref));
      }
      if (g_rnti_cfg_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *g_rnti_cfg_to_add_mod_list_r17, 1, 16));
      }
      if (g_rnti_cfg_to_release_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *g_rnti_cfg_to_release_list_r17, 1, 16, integer_packer<uint8_t>(0, 15)));
      }
      if (g_cs_rnti_cfg_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *g_cs_rnti_cfg_to_add_mod_list_r17, 1, 8));
      }
      if (g_cs_rnti_cfg_to_release_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *g_cs_rnti_cfg_to_release_list_r17, 1, 8, integer_packer<uint8_t>(0, 15)));
      }
      if (allow_csi_srs_tx_multicast_drx_active_r17_present) {
        HANDLE_CODE(bref.pack(allow_csi_srs_tx_multicast_drx_active_r17, 1));
      }
    }
    if (group_flags[4]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {sched_request_id_pos_mg_request_r17_present, drx_last_tx_ul_r17_present}));
      if (sched_request_id_pos_mg_request_r17_present) {
        HANDLE_CODE(pack_integer(bref, sched_request_id_pos_mg_request_r17, (uint8_t)0u, (uint8_t)7u));
      }
    }
    if (group_flags[5]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(pos_mg_request_r17_present, 1));
    }
    if (group_flags[6]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {drx_cfg_ext2_v1800.is_present(),
                             add_bs_table_allowed_r18_present,
                             dsr_cfg_to_add_mod_list_r18.is_present(),
                             dsr_cfg_to_release_list_r18.is_present(),
                             tar_cfg_r18.is_present()}));
      if (drx_cfg_ext2_v1800.is_present()) {
        HANDLE_CODE(drx_cfg_ext2_v1800->pack(bref));
      }
      if (add_bs_table_allowed_r18_present) {
        HANDLE_CODE(add_bs_table_allowed_r18.pack(bref));
      }
      if (dsr_cfg_to_add_mod_list_r18.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *dsr_cfg_to_add_mod_list_r18, 1, 8));
      }
      if (dsr_cfg_to_release_list_r18.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *dsr_cfg_to_release_list_r18, 1, 8, integer_packer<uint8_t>(0, 7)));
      }
      if (tar_cfg_r18.is_present()) {
        HANDLE_CODE(tar_cfg_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mac_cell_group_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(
      bref,
      {&ext, &drx_cfg_present, &sched_request_cfg_present, &bsr_cfg_present, &tag_cfg_present, &phr_cfg_present}));

  if (drx_cfg_present) {
    HANDLE_CODE(drx_cfg.unpack(bref));
  }
  if (sched_request_cfg_present) {
    HANDLE_CODE(sched_request_cfg.unpack(bref));
  }
  if (bsr_cfg_present) {
    HANDLE_CODE(bsr_cfg.unpack(bref));
  }
  if (tag_cfg_present) {
    HANDLE_CODE(tag_cfg.unpack(bref));
  }
  if (phr_cfg_present) {
    HANDLE_CODE(phr_cfg.unpack(bref));
  }
  HANDLE_CODE(bref.unpack(skip_ul_tx_dyn, 1));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(csi_mask_present, 1));
      unpack_presence_flag(data_inactivity_timer, bref);
      if (csi_mask_present) {
        HANDLE_CODE(bref.unpack(csi_mask, 1));
      }
      if (data_inactivity_timer.is_present()) {
        HANDLE_CODE(data_inactivity_timer->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref,
                              {&use_pre_bsr_r16_present,
                               &sched_request_id_lbt_scell_r16_present,
                               &lch_based_prioritization_r16_present,
                               &sched_request_id_bfr_scell_r16_present}));
      unpack_presence_flag(drx_cfg_secondary_group_r16, bref);
      if (sched_request_id_lbt_scell_r16_present) {
        HANDLE_CODE(unpack_integer(sched_request_id_lbt_scell_r16, bref, (uint8_t)0u, (uint8_t)7u));
      }
      if (sched_request_id_bfr_scell_r16_present) {
        HANDLE_CODE(unpack_integer(sched_request_id_bfr_scell_r16, bref, (uint8_t)0u, (uint8_t)7u));
      }
      if (drx_cfg_secondary_group_r16.is_present()) {
        HANDLE_CODE(drx_cfg_secondary_group_r16->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref, {&enhanced_skip_ul_tx_dyn_r16_present, &enhanced_skip_ul_tx_cfg_r16_present}));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(intra_cg_prioritization_r17_present, 1));
      unpack_presence_flag(drx_cfg_sl_r17, bref);
      unpack_presence_flag(drx_cfg_ext_v1700, bref);
      HANDLE_CODE(unpack_bits(bref, {&sched_request_id_bfr_r17_present, &sched_request_id_bfr2_r17_present}));
      unpack_presence_flag(sched_request_cfg_v1700, bref);
      unpack_presence_flag(tar_cfg_r17, bref);
      unpack_presence_flag(g_rnti_cfg_to_add_mod_list_r17, bref);
      unpack_presence_flag(g_rnti_cfg_to_release_list_r17, bref);
      unpack_presence_flag(g_cs_rnti_cfg_to_add_mod_list_r17, bref);
      unpack_presence_flag(g_cs_rnti_cfg_to_release_list_r17, bref);
      HANDLE_CODE(bref.unpack(allow_csi_srs_tx_multicast_drx_active_r17_present, 1));
      if (drx_cfg_sl_r17.is_present()) {
        HANDLE_CODE(drx_cfg_sl_r17->unpack(bref));
      }
      if (drx_cfg_ext_v1700.is_present()) {
        HANDLE_CODE(drx_cfg_ext_v1700->unpack(bref));
      }
      if (sched_request_id_bfr_r17_present) {
        HANDLE_CODE(unpack_integer(sched_request_id_bfr_r17, bref, (uint8_t)0u, (uint8_t)7u));
      }
      if (sched_request_id_bfr2_r17_present) {
        HANDLE_CODE(unpack_integer(sched_request_id_bfr2_r17, bref, (uint8_t)0u, (uint8_t)7u));
      }
      if (sched_request_cfg_v1700.is_present()) {
        HANDLE_CODE(sched_request_cfg_v1700->unpack(bref));
      }
      if (tar_cfg_r17.is_present()) {
        HANDLE_CODE(tar_cfg_r17->unpack(bref));
      }
      if (g_rnti_cfg_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*g_rnti_cfg_to_add_mod_list_r17, bref, 1, 16));
      }
      if (g_rnti_cfg_to_release_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*g_rnti_cfg_to_release_list_r17, bref, 1, 16, integer_packer<uint8_t>(0, 15)));
      }
      if (g_cs_rnti_cfg_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*g_cs_rnti_cfg_to_add_mod_list_r17, bref, 1, 8));
      }
      if (g_cs_rnti_cfg_to_release_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*g_cs_rnti_cfg_to_release_list_r17, bref, 1, 8, integer_packer<uint8_t>(0, 15)));
      }
      if (allow_csi_srs_tx_multicast_drx_active_r17_present) {
        HANDLE_CODE(bref.unpack(allow_csi_srs_tx_multicast_drx_active_r17, 1));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref, {&sched_request_id_pos_mg_request_r17_present, &drx_last_tx_ul_r17_present}));
      if (sched_request_id_pos_mg_request_r17_present) {
        HANDLE_CODE(unpack_integer(sched_request_id_pos_mg_request_r17, bref, (uint8_t)0u, (uint8_t)7u));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(pos_mg_request_r17_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(drx_cfg_ext2_v1800, bref);
      HANDLE_CODE(bref.unpack(add_bs_table_allowed_r18_present, 1));
      unpack_presence_flag(dsr_cfg_to_add_mod_list_r18, bref);
      unpack_presence_flag(dsr_cfg_to_release_list_r18, bref);
      unpack_presence_flag(tar_cfg_r18, bref);
      if (drx_cfg_ext2_v1800.is_present()) {
        HANDLE_CODE(drx_cfg_ext2_v1800->unpack(bref));
      }
      if (add_bs_table_allowed_r18_present) {
        HANDLE_CODE(add_bs_table_allowed_r18.unpack(bref));
      }
      if (dsr_cfg_to_add_mod_list_r18.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*dsr_cfg_to_add_mod_list_r18, bref, 1, 8));
      }
      if (dsr_cfg_to_release_list_r18.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*dsr_cfg_to_release_list_r18, bref, 1, 8, integer_packer<uint8_t>(0, 7)));
      }
      if (tar_cfg_r18.is_present()) {
        HANDLE_CODE(tar_cfg_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void mac_cell_group_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (drx_cfg_present) {
    j.write_fieldname("drx-Config");
    drx_cfg.to_json(j);
  }
  if (sched_request_cfg_present) {
    j.write_fieldname("schedulingRequestConfig");
    sched_request_cfg.to_json(j);
  }
  if (bsr_cfg_present) {
    j.write_fieldname("bsr-Config");
    bsr_cfg.to_json(j);
  }
  if (tag_cfg_present) {
    j.write_fieldname("tag-Config");
    tag_cfg.to_json(j);
  }
  if (phr_cfg_present) {
    j.write_fieldname("phr-Config");
    phr_cfg.to_json(j);
  }
  j.write_bool("skipUplinkTxDynamic", skip_ul_tx_dyn);
  if (csi_mask_present) {
    j.write_bool("csi-Mask", csi_mask);
  }
  if (data_inactivity_timer.is_present()) {
    j.write_fieldname("dataInactivityTimer");
    data_inactivity_timer->to_json(j);
  }
  if (use_pre_bsr_r16_present) {
    j.write_str("usePreBSR-r16", "true");
  }
  if (sched_request_id_lbt_scell_r16_present) {
    j.write_int("schedulingRequestID-LBT-SCell-r16", sched_request_id_lbt_scell_r16);
  }
  if (lch_based_prioritization_r16_present) {
    j.write_str("lch-BasedPrioritization-r16", "enabled");
  }
  if (sched_request_id_bfr_scell_r16_present) {
    j.write_int("schedulingRequestID-BFR-SCell-r16", sched_request_id_bfr_scell_r16);
  }
  if (drx_cfg_secondary_group_r16.is_present()) {
    j.write_fieldname("drx-ConfigSecondaryGroup-r16");
    drx_cfg_secondary_group_r16->to_json(j);
  }
  if (enhanced_skip_ul_tx_dyn_r16_present) {
    j.write_str("enhancedSkipUplinkTxDynamic-r16", "true");
  }
  if (enhanced_skip_ul_tx_cfg_r16_present) {
    j.write_str("enhancedSkipUplinkTxConfigured-r16", "true");
  }
  if (intra_cg_prioritization_r17_present) {
    j.write_str("intraCG-Prioritization-r17", "enabled");
  }
  if (drx_cfg_sl_r17.is_present()) {
    j.write_fieldname("drx-ConfigSL-r17");
    drx_cfg_sl_r17->to_json(j);
  }
  if (drx_cfg_ext_v1700.is_present()) {
    j.write_fieldname("drx-ConfigExt-v1700");
    drx_cfg_ext_v1700->to_json(j);
  }
  if (sched_request_id_bfr_r17_present) {
    j.write_int("schedulingRequestID-BFR-r17", sched_request_id_bfr_r17);
  }
  if (sched_request_id_bfr2_r17_present) {
    j.write_int("schedulingRequestID-BFR2-r17", sched_request_id_bfr2_r17);
  }
  if (sched_request_cfg_v1700.is_present()) {
    j.write_fieldname("schedulingRequestConfig-v1700");
    sched_request_cfg_v1700->to_json(j);
  }
  if (tar_cfg_r17.is_present()) {
    j.write_fieldname("tar-Config-r17");
    tar_cfg_r17->to_json(j);
  }
  if (g_rnti_cfg_to_add_mod_list_r17.is_present()) {
    j.start_array("g-RNTI-ConfigToAddModList-r17");
    for (const auto& e1 : *g_rnti_cfg_to_add_mod_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (g_rnti_cfg_to_release_list_r17.is_present()) {
    j.start_array("g-RNTI-ConfigToReleaseList-r17");
    for (const auto& e1 : *g_rnti_cfg_to_release_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (g_cs_rnti_cfg_to_add_mod_list_r17.is_present()) {
    j.start_array("g-CS-RNTI-ConfigToAddModList-r17");
    for (const auto& e1 : *g_cs_rnti_cfg_to_add_mod_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (g_cs_rnti_cfg_to_release_list_r17.is_present()) {
    j.start_array("g-CS-RNTI-ConfigToReleaseList-r17");
    for (const auto& e1 : *g_cs_rnti_cfg_to_release_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (allow_csi_srs_tx_multicast_drx_active_r17_present) {
    j.write_bool("allowCSI-SRS-Tx-MulticastDRX-Active-r17", allow_csi_srs_tx_multicast_drx_active_r17);
  }
  if (sched_request_id_pos_mg_request_r17_present) {
    j.write_int("schedulingRequestID-PosMG-Request-r17", sched_request_id_pos_mg_request_r17);
  }
  if (drx_last_tx_ul_r17_present) {
    j.write_str("drx-LastTransmissionUL-r17", "enabled");
  }
  if (pos_mg_request_r17_present) {
    j.write_str("posMG-Request-r17", "enabled");
  }
  if (drx_cfg_ext2_v1800.is_present()) {
    j.write_fieldname("drx-ConfigExt2-v1800");
    drx_cfg_ext2_v1800->to_json(j);
  }
  if (add_bs_table_allowed_r18_present) {
    j.write_fieldname("additionalBS-TableAllowed-r18");
    add_bs_table_allowed_r18.to_json(j);
  }
  if (dsr_cfg_to_add_mod_list_r18.is_present()) {
    j.start_array("dsr-ConfigToAddModList-r18");
    for (const auto& e1 : *dsr_cfg_to_add_mod_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (dsr_cfg_to_release_list_r18.is_present()) {
    j.start_array("dsr-ConfigToReleaseList-r18");
    for (const auto& e1 : *dsr_cfg_to_release_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (tar_cfg_r18.is_present()) {
    j.write_fieldname("tar-Config-r18");
    tar_cfg_r18->to_json(j);
  }
  j.end_obj();
}

// DCP-Config-r16 ::= SEQUENCE
OCUDUASN_CODE d_cp_cfg_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref, {ps_wake_up_r16_present, ps_tx_periodic_l1_rsrp_r16_present, ps_tx_other_periodic_csi_r16_present}));

  HANDLE_CODE(pack_integer(bref, ps_rnti_r16, (uint32_t)0u, (uint32_t)65535u));
  HANDLE_CODE(pack_integer(bref, ps_offset_r16, (uint8_t)1u, (uint8_t)120u));
  HANDLE_CODE(pack_integer(bref, size_dci_2_6_r16, (uint8_t)1u, (uint8_t)140u));
  HANDLE_CODE(pack_integer(bref, ps_position_dci_2_6_r16, (uint8_t)0u, (uint8_t)139u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE d_cp_cfg_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(
      bref, {&ps_wake_up_r16_present, &ps_tx_periodic_l1_rsrp_r16_present, &ps_tx_other_periodic_csi_r16_present}));

  HANDLE_CODE(unpack_integer(ps_rnti_r16, bref, (uint32_t)0u, (uint32_t)65535u));
  HANDLE_CODE(unpack_integer(ps_offset_r16, bref, (uint8_t)1u, (uint8_t)120u));
  HANDLE_CODE(unpack_integer(size_dci_2_6_r16, bref, (uint8_t)1u, (uint8_t)140u));
  HANDLE_CODE(unpack_integer(ps_position_dci_2_6_r16, bref, (uint8_t)0u, (uint8_t)139u));

  return OCUDUASN_SUCCESS;
}
void d_cp_cfg_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("ps-RNTI-r16", ps_rnti_r16);
  j.write_int("ps-Offset-r16", ps_offset_r16);
  j.write_int("sizeDCI-2-6-r16", size_dci_2_6_r16);
  j.write_int("ps-PositionDCI-2-6-r16", ps_position_dci_2_6_r16);
  if (ps_wake_up_r16_present) {
    j.write_str("ps-WakeUp-r16", "true");
  }
  if (ps_tx_periodic_l1_rsrp_r16_present) {
    j.write_str("ps-TransmitPeriodicL1-RSRP-r16", "true");
  }
  if (ps_tx_other_periodic_csi_r16_present) {
    j.write_str("ps-TransmitOtherPeriodicCSI-r16", "true");
  }
  j.end_obj();
}

const char* pdsch_harq_ack_codebook_list_r16_item_opts::to_string() const
{
  static const char* names[] = {"semiStatic", "dynamic"};
  return convert_enum_idx(names, 2, value, "pdsch_harq_ack_codebook_list_r16_item_e_");
}

// PDCCH-BlindDetectionCA-CombIndicator-r16 ::= SEQUENCE
OCUDUASN_CODE pdcch_blind_detection_ca_comb_ind_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, pdcch_blind_detection_ca1_r16, (uint8_t)1u, (uint8_t)15u));
  HANDLE_CODE(pack_integer(bref, pdcch_blind_detection_ca2_r16, (uint8_t)1u, (uint8_t)15u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcch_blind_detection_ca_comb_ind_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(pdcch_blind_detection_ca1_r16, bref, (uint8_t)1u, (uint8_t)15u));
  HANDLE_CODE(unpack_integer(pdcch_blind_detection_ca2_r16, bref, (uint8_t)1u, (uint8_t)15u));

  return OCUDUASN_SUCCESS;
}
void pdcch_blind_detection_ca_comb_ind_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pdcch-BlindDetectionCA1-r16", pdcch_blind_detection_ca1_r16);
  j.write_int("pdcch-BlindDetectionCA2-r16", pdcch_blind_detection_ca2_r16);
  j.end_obj();
}

// PDSCH-HARQ-ACK-EnhType3-r17 ::= SEQUENCE
OCUDUASN_CODE pdsch_harq_ack_enh_type3_r17_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= per_harq_ext_r17.is_present();
  HANDLE_CODE(pack_bits(
      bref, {group_flags.any(), pdsch_harq_ack_enh_type3_ndi_r17_present, pdsch_harq_ack_enh_type3_cbg_r17_present}));

  HANDLE_CODE(pack_integer(bref, pdsch_harq_ack_enh_type3_idx_r17, (uint8_t)0u, (uint8_t)7u));
  HANDLE_CODE(applicable_r17.pack(bref));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(per_harq_ext_r17.is_present(), 1));
      if (per_harq_ext_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *per_harq_ext_r17, 1, 32));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdsch_harq_ack_enh_type3_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(
      unpack_bits(bref, {&ext, &pdsch_harq_ack_enh_type3_ndi_r17_present, &pdsch_harq_ack_enh_type3_cbg_r17_present}));

  HANDLE_CODE(unpack_integer(pdsch_harq_ack_enh_type3_idx_r17, bref, (uint8_t)0u, (uint8_t)7u));
  HANDLE_CODE(applicable_r17.unpack(bref));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(per_harq_ext_r17, bref);
      if (per_harq_ext_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*per_harq_ext_r17, bref, 1, 32));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void pdsch_harq_ack_enh_type3_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("pdsch-HARQ-ACK-EnhType3Index-r17", pdsch_harq_ack_enh_type3_idx_r17);
  j.write_fieldname("applicable-r17");
  applicable_r17.to_json(j);
  if (pdsch_harq_ack_enh_type3_ndi_r17_present) {
    j.write_str("pdsch-HARQ-ACK-EnhType3NDI-r17", "true");
  }
  if (pdsch_harq_ack_enh_type3_cbg_r17_present) {
    j.write_str("pdsch-HARQ-ACK-EnhType3CBG-r17", "true");
  }
  if (per_harq_ext_r17.is_present()) {
    j.start_array("perHARQ-Ext-r17");
    for (const auto& e1 : *per_harq_ext_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

void pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::destroy_()
{
  switch (type_) {
    case types::per_cc:
      c.destroy<per_cc_l_>();
      break;
    case types::per_harq:
      c.destroy<per_harq_l_>();
      break;
    default:
      break;
  }
}
void pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::per_cc:
      c.init<per_cc_l_>();
      break;
    case types::per_harq:
      c.init<per_harq_l_>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_");
  }
}
pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::applicable_r17_c_(
    const pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::per_cc:
      c.init(other.c.get<per_cc_l_>());
      break;
    case types::per_harq:
      c.init(other.c.get<per_harq_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_");
  }
}
pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_& pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::operator=(
    const pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::per_cc:
      c.set(other.c.get<per_cc_l_>());
      break;
    case types::per_harq:
      c.set(other.c.get<per_harq_l_>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_");
  }

  return *this;
}
pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::per_cc_l_&
pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::set_per_cc()
{
  set(types::per_cc);
  return c.get<per_cc_l_>();
}
pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::per_harq_l_&
pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::set_per_harq()
{
  set(types::per_harq);
  return c.get<per_harq_l_>();
}
void pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::per_cc:
      j.start_array("perCC");
      for (const auto& e1 : c.get<per_cc_l_>()) {
        j.write_int(e1);
      }
      j.end_array();
      break;
    case types::per_harq:
      j.start_array("perHARQ");
      for (const auto& e1 : c.get<per_harq_l_>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::per_cc:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<per_cc_l_>(), 1, 32, integer_packer<uint8_t>(0, 1)));
      break;
    case types::per_harq:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<per_harq_l_>(), 1, 32));
      break;
    default:
      log_invalid_choice_id(type_, "pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::per_cc:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<per_cc_l_>(), bref, 1, 32, integer_packer<uint8_t>(0, 1)));
      break;
    case types::per_harq:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<per_harq_l_>(), bref, 1, 32));
      break;
    default:
      log_invalid_choice_id(type_, "pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::types_opts::to_string() const
{
  static const char* names[] = {"perCC", "perHARQ"};
  return convert_enum_idx(names, 2, value, "pdsch_harq_ack_enh_type3_r17_s::applicable_r17_c_::types");
}

// MulticastConfig-r17 ::= SEQUENCE
OCUDUASN_CODE multicast_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref, {pdsch_harq_ack_codebook_list_multicast_r17_present, type1_codebook_generation_mode_r17_present}));

  if (pdsch_harq_ack_codebook_list_multicast_r17_present) {
    HANDLE_CODE(pdsch_harq_ack_codebook_list_multicast_r17.pack(bref));
  }
  if (type1_codebook_generation_mode_r17_present) {
    HANDLE_CODE(type1_codebook_generation_mode_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_cfg_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(
      bref, {&pdsch_harq_ack_codebook_list_multicast_r17_present, &type1_codebook_generation_mode_r17_present}));

  if (pdsch_harq_ack_codebook_list_multicast_r17_present) {
    HANDLE_CODE(pdsch_harq_ack_codebook_list_multicast_r17.unpack(bref));
  }
  if (type1_codebook_generation_mode_r17_present) {
    HANDLE_CODE(type1_codebook_generation_mode_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void multicast_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdsch_harq_ack_codebook_list_multicast_r17_present) {
    j.write_fieldname("pdsch-HARQ-ACK-CodebookListMulticast-r17");
    pdsch_harq_ack_codebook_list_multicast_r17.to_json(j);
  }
  if (type1_codebook_generation_mode_r17_present) {
    j.write_str("type1CodebookGenerationMode-r17", type1_codebook_generation_mode_r17.to_string());
  }
  j.end_obj();
}

const char* multicast_cfg_r17_s::type1_codebook_generation_mode_r17_opts::to_string() const
{
  static const char* names[] = {"mode1", "mode2"};
  return convert_enum_idx(names, 2, value, "multicast_cfg_r17_s::type1_codebook_generation_mode_r17_e_");
}
uint8_t multicast_cfg_r17_s::type1_codebook_generation_mode_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "multicast_cfg_r17_s::type1_codebook_generation_mode_r17_e_");
}

// PDCCH-BlindDetectionCA-CombIndicator-r17 ::= SEQUENCE
OCUDUASN_CODE pdcch_blind_detection_ca_comb_ind_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {pdcch_blind_detection_ca1_r17_present, pdcch_blind_detection_ca2_r17_present}));

  if (pdcch_blind_detection_ca1_r17_present) {
    HANDLE_CODE(pack_integer(bref, pdcch_blind_detection_ca1_r17, (uint8_t)1u, (uint8_t)15u));
  }
  if (pdcch_blind_detection_ca2_r17_present) {
    HANDLE_CODE(pack_integer(bref, pdcch_blind_detection_ca2_r17, (uint8_t)1u, (uint8_t)15u));
  }
  HANDLE_CODE(pack_integer(bref, pdcch_blind_detection_ca3_r17, (uint8_t)1u, (uint8_t)15u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdcch_blind_detection_ca_comb_ind_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&pdcch_blind_detection_ca1_r17_present, &pdcch_blind_detection_ca2_r17_present}));

  if (pdcch_blind_detection_ca1_r17_present) {
    HANDLE_CODE(unpack_integer(pdcch_blind_detection_ca1_r17, bref, (uint8_t)1u, (uint8_t)15u));
  }
  if (pdcch_blind_detection_ca2_r17_present) {
    HANDLE_CODE(unpack_integer(pdcch_blind_detection_ca2_r17, bref, (uint8_t)1u, (uint8_t)15u));
  }
  HANDLE_CODE(unpack_integer(pdcch_blind_detection_ca3_r17, bref, (uint8_t)1u, (uint8_t)15u));

  return OCUDUASN_SUCCESS;
}
void pdcch_blind_detection_ca_comb_ind_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pdcch_blind_detection_ca1_r17_present) {
    j.write_int("pdcch-BlindDetectionCA1-r17", pdcch_blind_detection_ca1_r17);
  }
  if (pdcch_blind_detection_ca2_r17_present) {
    j.write_int("pdcch-BlindDetectionCA2-r17", pdcch_blind_detection_ca2_r17);
  }
  j.write_int("pdcch-BlindDetectionCA3-r17", pdcch_blind_detection_ca3_r17);
  j.end_obj();
}

// PhysicalCellGroupConfig ::= SEQUENCE
OCUDUASN_CODE phys_cell_group_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(10);
  group_flags[0] |= mcs_c_rnti_present;
  group_flags[0] |= p_ue_fr1_present;
  group_flags[1] |= xscale_present;
  group_flags[2] |= pdcch_blind_detection.is_present();
  group_flags[3] |= dcp_cfg_r16.is_present();
  group_flags[3] |= harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16_present;
  group_flags[3] |= harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16_present;
  group_flags[3] |= pdsch_harq_ack_codebook_secondary_pucch_group_r16_present;
  group_flags[3] |= p_nr_fr2_r16_present;
  group_flags[3] |= p_ue_fr2_r16_present;
  group_flags[3] |= nrdc_p_cmode_fr1_r16_present;
  group_flags[3] |= nrdc_p_cmode_fr2_r16_present;
  group_flags[3] |= pdsch_harq_ack_codebook_r16_present;
  group_flags[3] |= nfi_total_dai_included_r16_present;
  group_flags[3] |= ul_total_dai_included_r16_present;
  group_flags[3] |= pdsch_harq_ack_one_shot_feedback_r16_present;
  group_flags[3] |= pdsch_harq_ack_one_shot_feedback_ndi_r16_present;
  group_flags[3] |= pdsch_harq_ack_one_shot_feedback_cbg_r16_present;
  group_flags[3] |= dl_assign_idx_dci_0_2_r16_present;
  group_flags[3] |= dl_assign_idx_dci_1_2_r16_present;
  group_flags[3] |= pdsch_harq_ack_codebook_list_r16.is_present();
  group_flags[3] |= ack_nack_feedback_mode_r16_present;
  group_flags[3] |= pdcch_blind_detection_ca_comb_ind_r16.is_present();
  group_flags[3] |= pdcch_blind_detection2_r16.is_present();
  group_flags[3] |= pdcch_blind_detection3_r16.is_present();
  group_flags[3] |= bd_factor_r_r16_present;
  group_flags[4] |= pdsch_harq_ack_enh_type3_to_add_mod_list_r17.is_present();
  group_flags[4] |= pdsch_harq_ack_enh_type3_to_release_list_r17.is_present();
  group_flags[4] |= pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17.is_present();
  group_flags[4] |= pdsch_harq_ack_enh_type3_secondary_to_release_list_r17.is_present();
  group_flags[4] |= pdsch_harq_ack_enh_type3_dci_field_secondary_pucch_group_r17_present;
  group_flags[4] |= pdsch_harq_ack_enh_type3_dci_field_r17_present;
  group_flags[4] |= pdsch_harq_ack_retx_r17_present;
  group_flags[4] |= pdsch_harq_ack_retx_secondary_pucch_group_r17_present;
  group_flags[4] |= pucch_s_scell_r17_present;
  group_flags[4] |= pucch_s_scell_secondary_pucch_group_r17_present;
  group_flags[4] |= pucch_s_scell_dyn_r17_present;
  group_flags[4] |= pucch_s_scell_dyn_secondary_pucch_group_r17_present;
  group_flags[4] |= pucch_s_scell_pattern_r17.is_present();
  group_flags[4] |= pucch_s_scell_pattern_secondary_pucch_group_r17.is_present();
  group_flags[4] |= uci_mux_with_diff_prio_r17_present;
  group_flags[4] |= uci_mux_with_diff_prio_secondary_pucch_group_r17_present;
  group_flags[4] |= simul_pucch_pusch_r17_present;
  group_flags[4] |= simul_pucch_pusch_secondary_pucch_group_r17_present;
  group_flags[4] |= prio_low_dg_high_cg_r17_present;
  group_flags[4] |= prio_high_dg_low_cg_r17_present;
  group_flags[4] |= two_qcl_type_d_for_pdcch_repeat_r17_present;
  group_flags[4] |= multicast_cfg_r17.is_present();
  group_flags[4] |= pdcch_blind_detection_ca_comb_ind_r17.is_present();
  group_flags[5] |= simul_sr_pusch_diff_pucch_groups_r17_present;
  group_flags[6] |= intra_band_nc_prach_simul_tx_r17_present;
  group_flags[7] |= pdcch_blind_detection4_r17.is_present();
  group_flags[8] |= simul_pucch_pusch_same_prio_r17_present;
  group_flags[8] |= simul_pucch_pusch_same_prio_secondary_pucch_group_r17_present;
  group_flags[9] |= ncr_rnti_r18_present;
  group_flags[9] |= cell_dtrx_dci_cfg_r18.is_present();
  group_flags[9] |= two_qcl_type_d_for_multi_dci_r18_present;
  group_flags[9] |= enable_type1_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present;
  group_flags[9] |= enable_type2_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present;
  group_flags[9] |= enable_type3_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present;
  group_flags[9] |= enable_diff_pucch_res_r18_present;
  group_flags[9] |= enable_diff_cb_size_r18_present;
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         harq_ack_spatial_bundling_pucch_present,
                         harq_ack_spatial_bundling_pusch_present,
                         p_nr_fr1_present,
                         tpc_srs_rnti_present,
                         tpc_pucch_rnti_present,
                         tpc_pusch_rnti_present,
                         sp_csi_rnti_present,
                         cs_rnti_present}));

  if (p_nr_fr1_present) {
    HANDLE_CODE(pack_integer(bref, p_nr_fr1, (int8_t)-30, (int8_t)33));
  }
  HANDLE_CODE(pdsch_harq_ack_codebook.pack(bref));
  if (tpc_srs_rnti_present) {
    HANDLE_CODE(pack_integer(bref, tpc_srs_rnti, (uint32_t)0u, (uint32_t)65535u));
  }
  if (tpc_pucch_rnti_present) {
    HANDLE_CODE(pack_integer(bref, tpc_pucch_rnti, (uint32_t)0u, (uint32_t)65535u));
  }
  if (tpc_pusch_rnti_present) {
    HANDLE_CODE(pack_integer(bref, tpc_pusch_rnti, (uint32_t)0u, (uint32_t)65535u));
  }
  if (sp_csi_rnti_present) {
    HANDLE_CODE(pack_integer(bref, sp_csi_rnti, (uint32_t)0u, (uint32_t)65535u));
  }
  if (cs_rnti_present) {
    HANDLE_CODE(cs_rnti.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {mcs_c_rnti_present, p_ue_fr1_present}));
      if (mcs_c_rnti_present) {
        HANDLE_CODE(pack_integer(bref, mcs_c_rnti, (uint32_t)0u, (uint32_t)65535u));
      }
      if (p_ue_fr1_present) {
        HANDLE_CODE(pack_integer(bref, p_ue_fr1, (int8_t)-30, (int8_t)33));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(xscale_present, 1));
      if (xscale_present) {
        HANDLE_CODE(xscale.pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(pdcch_blind_detection.is_present(), 1));
      if (pdcch_blind_detection.is_present()) {
        HANDLE_CODE(pdcch_blind_detection->pack(bref));
      }
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {dcp_cfg_r16.is_present(),
                             harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16_present,
                             harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16_present,
                             pdsch_harq_ack_codebook_secondary_pucch_group_r16_present,
                             p_nr_fr2_r16_present,
                             p_ue_fr2_r16_present,
                             nrdc_p_cmode_fr1_r16_present,
                             nrdc_p_cmode_fr2_r16_present,
                             pdsch_harq_ack_codebook_r16_present,
                             nfi_total_dai_included_r16_present,
                             ul_total_dai_included_r16_present,
                             pdsch_harq_ack_one_shot_feedback_r16_present,
                             pdsch_harq_ack_one_shot_feedback_ndi_r16_present,
                             pdsch_harq_ack_one_shot_feedback_cbg_r16_present,
                             dl_assign_idx_dci_0_2_r16_present,
                             dl_assign_idx_dci_1_2_r16_present,
                             pdsch_harq_ack_codebook_list_r16.is_present(),
                             ack_nack_feedback_mode_r16_present,
                             pdcch_blind_detection_ca_comb_ind_r16.is_present(),
                             pdcch_blind_detection2_r16.is_present(),
                             pdcch_blind_detection3_r16.is_present(),
                             bd_factor_r_r16_present}));
      if (dcp_cfg_r16.is_present()) {
        HANDLE_CODE(dcp_cfg_r16->pack(bref));
      }
      if (harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16_present) {
        HANDLE_CODE(harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16.pack(bref));
      }
      if (harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16_present) {
        HANDLE_CODE(harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16.pack(bref));
      }
      if (pdsch_harq_ack_codebook_secondary_pucch_group_r16_present) {
        HANDLE_CODE(pdsch_harq_ack_codebook_secondary_pucch_group_r16.pack(bref));
      }
      if (p_nr_fr2_r16_present) {
        HANDLE_CODE(pack_integer(bref, p_nr_fr2_r16, (int8_t)-30, (int8_t)33));
      }
      if (p_ue_fr2_r16_present) {
        HANDLE_CODE(pack_integer(bref, p_ue_fr2_r16, (int8_t)-30, (int8_t)33));
      }
      if (nrdc_p_cmode_fr1_r16_present) {
        HANDLE_CODE(nrdc_p_cmode_fr1_r16.pack(bref));
      }
      if (nrdc_p_cmode_fr2_r16_present) {
        HANDLE_CODE(nrdc_p_cmode_fr2_r16.pack(bref));
      }
      if (dl_assign_idx_dci_1_2_r16_present) {
        HANDLE_CODE(dl_assign_idx_dci_1_2_r16.pack(bref));
      }
      if (pdsch_harq_ack_codebook_list_r16.is_present()) {
        HANDLE_CODE(pdsch_harq_ack_codebook_list_r16->pack(bref));
      }
      if (ack_nack_feedback_mode_r16_present) {
        HANDLE_CODE(ack_nack_feedback_mode_r16.pack(bref));
      }
      if (pdcch_blind_detection_ca_comb_ind_r16.is_present()) {
        HANDLE_CODE(pdcch_blind_detection_ca_comb_ind_r16->pack(bref));
      }
      if (pdcch_blind_detection2_r16.is_present()) {
        HANDLE_CODE(pdcch_blind_detection2_r16->pack(bref));
      }
      if (pdcch_blind_detection3_r16.is_present()) {
        HANDLE_CODE(pdcch_blind_detection3_r16->pack(bref));
      }
    }
    if (group_flags[4]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {pdsch_harq_ack_enh_type3_to_add_mod_list_r17.is_present(),
                             pdsch_harq_ack_enh_type3_to_release_list_r17.is_present(),
                             pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17.is_present(),
                             pdsch_harq_ack_enh_type3_secondary_to_release_list_r17.is_present(),
                             pdsch_harq_ack_enh_type3_dci_field_secondary_pucch_group_r17_present,
                             pdsch_harq_ack_enh_type3_dci_field_r17_present,
                             pdsch_harq_ack_retx_r17_present,
                             pdsch_harq_ack_retx_secondary_pucch_group_r17_present,
                             pucch_s_scell_r17_present,
                             pucch_s_scell_secondary_pucch_group_r17_present,
                             pucch_s_scell_dyn_r17_present,
                             pucch_s_scell_dyn_secondary_pucch_group_r17_present,
                             pucch_s_scell_pattern_r17.is_present(),
                             pucch_s_scell_pattern_secondary_pucch_group_r17.is_present(),
                             uci_mux_with_diff_prio_r17_present,
                             uci_mux_with_diff_prio_secondary_pucch_group_r17_present,
                             simul_pucch_pusch_r17_present,
                             simul_pucch_pusch_secondary_pucch_group_r17_present,
                             prio_low_dg_high_cg_r17_present,
                             prio_high_dg_low_cg_r17_present,
                             two_qcl_type_d_for_pdcch_repeat_r17_present,
                             multicast_cfg_r17.is_present(),
                             pdcch_blind_detection_ca_comb_ind_r17.is_present()}));
      if (pdsch_harq_ack_enh_type3_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *pdsch_harq_ack_enh_type3_to_add_mod_list_r17, 1, 8));
      }
      if (pdsch_harq_ack_enh_type3_to_release_list_r17.is_present()) {
        HANDLE_CODE(
            pack_dyn_seq_of(bref, *pdsch_harq_ack_enh_type3_to_release_list_r17, 1, 8, integer_packer<uint8_t>(0, 7)));
      }
      if (pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17, 1, 8));
      }
      if (pdsch_harq_ack_enh_type3_secondary_to_release_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(
            bref, *pdsch_harq_ack_enh_type3_secondary_to_release_list_r17, 1, 8, integer_packer<uint8_t>(0, 7)));
      }
      if (pucch_s_scell_r17_present) {
        HANDLE_CODE(pack_integer(bref, pucch_s_scell_r17, (uint8_t)1u, (uint8_t)31u));
      }
      if (pucch_s_scell_secondary_pucch_group_r17_present) {
        HANDLE_CODE(pack_integer(bref, pucch_s_scell_secondary_pucch_group_r17, (uint8_t)1u, (uint8_t)31u));
      }
      if (pucch_s_scell_pattern_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *pucch_s_scell_pattern_r17, 1, 320, integer_packer<uint8_t>(0, 1)));
      }
      if (pucch_s_scell_pattern_secondary_pucch_group_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(
            bref, *pucch_s_scell_pattern_secondary_pucch_group_r17, 1, 320, integer_packer<uint8_t>(0, 1)));
      }
      if (multicast_cfg_r17.is_present()) {
        HANDLE_CODE(multicast_cfg_r17->pack(bref));
      }
      if (pdcch_blind_detection_ca_comb_ind_r17.is_present()) {
        HANDLE_CODE(pdcch_blind_detection_ca_comb_ind_r17->pack(bref));
      }
    }
    if (group_flags[5]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(simul_sr_pusch_diff_pucch_groups_r17_present, 1));
    }
    if (group_flags[6]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(intra_band_nc_prach_simul_tx_r17_present, 1));
    }
    if (group_flags[7]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(pdcch_blind_detection4_r17.is_present(), 1));
      if (pdcch_blind_detection4_r17.is_present()) {
        HANDLE_CODE(pdcch_blind_detection4_r17->pack(bref));
      }
    }
    if (group_flags[8]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(
          bref,
          {simul_pucch_pusch_same_prio_r17_present, simul_pucch_pusch_same_prio_secondary_pucch_group_r17_present}));
    }
    if (group_flags[9]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {ncr_rnti_r18_present,
                             cell_dtrx_dci_cfg_r18.is_present(),
                             two_qcl_type_d_for_multi_dci_r18_present,
                             enable_type1_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present,
                             enable_type2_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present,
                             enable_type3_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present,
                             enable_diff_pucch_res_r18_present,
                             enable_diff_cb_size_r18_present}));
      if (ncr_rnti_r18_present) {
        HANDLE_CODE(pack_integer(bref, ncr_rnti_r18, (uint32_t)0u, (uint32_t)65535u));
      }
      if (cell_dtrx_dci_cfg_r18.is_present()) {
        HANDLE_CODE(cell_dtrx_dci_cfg_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE phys_cell_group_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &harq_ack_spatial_bundling_pucch_present,
                           &harq_ack_spatial_bundling_pusch_present,
                           &p_nr_fr1_present,
                           &tpc_srs_rnti_present,
                           &tpc_pucch_rnti_present,
                           &tpc_pusch_rnti_present,
                           &sp_csi_rnti_present,
                           &cs_rnti_present}));

  if (p_nr_fr1_present) {
    HANDLE_CODE(unpack_integer(p_nr_fr1, bref, (int8_t)-30, (int8_t)33));
  }
  HANDLE_CODE(pdsch_harq_ack_codebook.unpack(bref));
  if (tpc_srs_rnti_present) {
    HANDLE_CODE(unpack_integer(tpc_srs_rnti, bref, (uint32_t)0u, (uint32_t)65535u));
  }
  if (tpc_pucch_rnti_present) {
    HANDLE_CODE(unpack_integer(tpc_pucch_rnti, bref, (uint32_t)0u, (uint32_t)65535u));
  }
  if (tpc_pusch_rnti_present) {
    HANDLE_CODE(unpack_integer(tpc_pusch_rnti, bref, (uint32_t)0u, (uint32_t)65535u));
  }
  if (sp_csi_rnti_present) {
    HANDLE_CODE(unpack_integer(sp_csi_rnti, bref, (uint32_t)0u, (uint32_t)65535u));
  }
  if (cs_rnti_present) {
    HANDLE_CODE(cs_rnti.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref, {&mcs_c_rnti_present, &p_ue_fr1_present}));
      if (mcs_c_rnti_present) {
        HANDLE_CODE(unpack_integer(mcs_c_rnti, bref, (uint32_t)0u, (uint32_t)65535u));
      }
      if (p_ue_fr1_present) {
        HANDLE_CODE(unpack_integer(p_ue_fr1, bref, (int8_t)-30, (int8_t)33));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(xscale_present, 1));
      if (xscale_present) {
        HANDLE_CODE(xscale.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(pdcch_blind_detection, bref);
      if (pdcch_blind_detection.is_present()) {
        HANDLE_CODE(pdcch_blind_detection->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(dcp_cfg_r16, bref);
      HANDLE_CODE(unpack_bits(bref,
                              {&harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16_present,
                               &harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16_present,
                               &pdsch_harq_ack_codebook_secondary_pucch_group_r16_present,
                               &p_nr_fr2_r16_present,
                               &p_ue_fr2_r16_present,
                               &nrdc_p_cmode_fr1_r16_present,
                               &nrdc_p_cmode_fr2_r16_present,
                               &pdsch_harq_ack_codebook_r16_present,
                               &nfi_total_dai_included_r16_present,
                               &ul_total_dai_included_r16_present,
                               &pdsch_harq_ack_one_shot_feedback_r16_present,
                               &pdsch_harq_ack_one_shot_feedback_ndi_r16_present,
                               &pdsch_harq_ack_one_shot_feedback_cbg_r16_present,
                               &dl_assign_idx_dci_0_2_r16_present,
                               &dl_assign_idx_dci_1_2_r16_present}));
      unpack_presence_flag(pdsch_harq_ack_codebook_list_r16, bref);
      HANDLE_CODE(bref.unpack(ack_nack_feedback_mode_r16_present, 1));
      unpack_presence_flag(pdcch_blind_detection_ca_comb_ind_r16, bref);
      unpack_presence_flag(pdcch_blind_detection2_r16, bref);
      unpack_presence_flag(pdcch_blind_detection3_r16, bref);
      HANDLE_CODE(bref.unpack(bd_factor_r_r16_present, 1));
      if (dcp_cfg_r16.is_present()) {
        HANDLE_CODE(dcp_cfg_r16->unpack(bref));
      }
      if (harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16_present) {
        HANDLE_CODE(harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16.unpack(bref));
      }
      if (harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16_present) {
        HANDLE_CODE(harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16.unpack(bref));
      }
      if (pdsch_harq_ack_codebook_secondary_pucch_group_r16_present) {
        HANDLE_CODE(pdsch_harq_ack_codebook_secondary_pucch_group_r16.unpack(bref));
      }
      if (p_nr_fr2_r16_present) {
        HANDLE_CODE(unpack_integer(p_nr_fr2_r16, bref, (int8_t)-30, (int8_t)33));
      }
      if (p_ue_fr2_r16_present) {
        HANDLE_CODE(unpack_integer(p_ue_fr2_r16, bref, (int8_t)-30, (int8_t)33));
      }
      if (nrdc_p_cmode_fr1_r16_present) {
        HANDLE_CODE(nrdc_p_cmode_fr1_r16.unpack(bref));
      }
      if (nrdc_p_cmode_fr2_r16_present) {
        HANDLE_CODE(nrdc_p_cmode_fr2_r16.unpack(bref));
      }
      if (dl_assign_idx_dci_1_2_r16_present) {
        HANDLE_CODE(dl_assign_idx_dci_1_2_r16.unpack(bref));
      }
      if (pdsch_harq_ack_codebook_list_r16.is_present()) {
        HANDLE_CODE(pdsch_harq_ack_codebook_list_r16->unpack(bref));
      }
      if (ack_nack_feedback_mode_r16_present) {
        HANDLE_CODE(ack_nack_feedback_mode_r16.unpack(bref));
      }
      if (pdcch_blind_detection_ca_comb_ind_r16.is_present()) {
        HANDLE_CODE(pdcch_blind_detection_ca_comb_ind_r16->unpack(bref));
      }
      if (pdcch_blind_detection2_r16.is_present()) {
        HANDLE_CODE(pdcch_blind_detection2_r16->unpack(bref));
      }
      if (pdcch_blind_detection3_r16.is_present()) {
        HANDLE_CODE(pdcch_blind_detection3_r16->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(pdsch_harq_ack_enh_type3_to_add_mod_list_r17, bref);
      unpack_presence_flag(pdsch_harq_ack_enh_type3_to_release_list_r17, bref);
      unpack_presence_flag(pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17, bref);
      unpack_presence_flag(pdsch_harq_ack_enh_type3_secondary_to_release_list_r17, bref);
      HANDLE_CODE(unpack_bits(bref,
                              {&pdsch_harq_ack_enh_type3_dci_field_secondary_pucch_group_r17_present,
                               &pdsch_harq_ack_enh_type3_dci_field_r17_present,
                               &pdsch_harq_ack_retx_r17_present,
                               &pdsch_harq_ack_retx_secondary_pucch_group_r17_present,
                               &pucch_s_scell_r17_present,
                               &pucch_s_scell_secondary_pucch_group_r17_present,
                               &pucch_s_scell_dyn_r17_present,
                               &pucch_s_scell_dyn_secondary_pucch_group_r17_present}));
      unpack_presence_flag(pucch_s_scell_pattern_r17, bref);
      unpack_presence_flag(pucch_s_scell_pattern_secondary_pucch_group_r17, bref);
      HANDLE_CODE(unpack_bits(bref,
                              {&uci_mux_with_diff_prio_r17_present,
                               &uci_mux_with_diff_prio_secondary_pucch_group_r17_present,
                               &simul_pucch_pusch_r17_present,
                               &simul_pucch_pusch_secondary_pucch_group_r17_present,
                               &prio_low_dg_high_cg_r17_present,
                               &prio_high_dg_low_cg_r17_present,
                               &two_qcl_type_d_for_pdcch_repeat_r17_present}));
      unpack_presence_flag(multicast_cfg_r17, bref);
      unpack_presence_flag(pdcch_blind_detection_ca_comb_ind_r17, bref);
      if (pdsch_harq_ack_enh_type3_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*pdsch_harq_ack_enh_type3_to_add_mod_list_r17, bref, 1, 8));
      }
      if (pdsch_harq_ack_enh_type3_to_release_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(
            *pdsch_harq_ack_enh_type3_to_release_list_r17, bref, 1, 8, integer_packer<uint8_t>(0, 7)));
      }
      if (pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17, bref, 1, 8));
      }
      if (pdsch_harq_ack_enh_type3_secondary_to_release_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(
            *pdsch_harq_ack_enh_type3_secondary_to_release_list_r17, bref, 1, 8, integer_packer<uint8_t>(0, 7)));
      }
      if (pucch_s_scell_r17_present) {
        HANDLE_CODE(unpack_integer(pucch_s_scell_r17, bref, (uint8_t)1u, (uint8_t)31u));
      }
      if (pucch_s_scell_secondary_pucch_group_r17_present) {
        HANDLE_CODE(unpack_integer(pucch_s_scell_secondary_pucch_group_r17, bref, (uint8_t)1u, (uint8_t)31u));
      }
      if (pucch_s_scell_pattern_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*pucch_s_scell_pattern_r17, bref, 1, 320, integer_packer<uint8_t>(0, 1)));
      }
      if (pucch_s_scell_pattern_secondary_pucch_group_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(
            *pucch_s_scell_pattern_secondary_pucch_group_r17, bref, 1, 320, integer_packer<uint8_t>(0, 1)));
      }
      if (multicast_cfg_r17.is_present()) {
        HANDLE_CODE(multicast_cfg_r17->unpack(bref));
      }
      if (pdcch_blind_detection_ca_comb_ind_r17.is_present()) {
        HANDLE_CODE(pdcch_blind_detection_ca_comb_ind_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(simul_sr_pusch_diff_pucch_groups_r17_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(intra_band_nc_prach_simul_tx_r17_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(pdcch_blind_detection4_r17, bref);
      if (pdcch_blind_detection4_r17.is_present()) {
        HANDLE_CODE(pdcch_blind_detection4_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(
          bref,
          {&simul_pucch_pusch_same_prio_r17_present, &simul_pucch_pusch_same_prio_secondary_pucch_group_r17_present}));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(ncr_rnti_r18_present, 1));
      unpack_presence_flag(cell_dtrx_dci_cfg_r18, bref);
      HANDLE_CODE(unpack_bits(bref,
                              {&two_qcl_type_d_for_multi_dci_r18_present,
                               &enable_type1_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present,
                               &enable_type2_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present,
                               &enable_type3_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present,
                               &enable_diff_pucch_res_r18_present,
                               &enable_diff_cb_size_r18_present}));
      if (ncr_rnti_r18_present) {
        HANDLE_CODE(unpack_integer(ncr_rnti_r18, bref, (uint32_t)0u, (uint32_t)65535u));
      }
      if (cell_dtrx_dci_cfg_r18.is_present()) {
        HANDLE_CODE(cell_dtrx_dci_cfg_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void phys_cell_group_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (harq_ack_spatial_bundling_pucch_present) {
    j.write_str("harq-ACK-SpatialBundlingPUCCH", "true");
  }
  if (harq_ack_spatial_bundling_pusch_present) {
    j.write_str("harq-ACK-SpatialBundlingPUSCH", "true");
  }
  if (p_nr_fr1_present) {
    j.write_int("p-NR-FR1", p_nr_fr1);
  }
  j.write_str("pdsch-HARQ-ACK-Codebook", pdsch_harq_ack_codebook.to_string());
  if (tpc_srs_rnti_present) {
    j.write_int("tpc-SRS-RNTI", tpc_srs_rnti);
  }
  if (tpc_pucch_rnti_present) {
    j.write_int("tpc-PUCCH-RNTI", tpc_pucch_rnti);
  }
  if (tpc_pusch_rnti_present) {
    j.write_int("tpc-PUSCH-RNTI", tpc_pusch_rnti);
  }
  if (sp_csi_rnti_present) {
    j.write_int("sp-CSI-RNTI", sp_csi_rnti);
  }
  if (cs_rnti_present) {
    j.write_fieldname("cs-RNTI");
    cs_rnti.to_json(j);
  }
  if (mcs_c_rnti_present) {
    j.write_int("mcs-C-RNTI", mcs_c_rnti);
  }
  if (p_ue_fr1_present) {
    j.write_int("p-UE-FR1", p_ue_fr1);
  }
  if (xscale_present) {
    j.write_str("xScale", xscale.to_string());
  }
  if (pdcch_blind_detection.is_present()) {
    j.write_fieldname("pdcch-BlindDetection");
    pdcch_blind_detection->to_json(j);
  }
  if (dcp_cfg_r16.is_present()) {
    j.write_fieldname("dcp-Config-r16");
    dcp_cfg_r16->to_json(j);
  }
  if (harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16_present) {
    j.write_str("harq-ACK-SpatialBundlingPUCCH-secondaryPUCCHgroup-r16",
                harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16.to_string());
  }
  if (harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16_present) {
    j.write_str("harq-ACK-SpatialBundlingPUSCH-secondaryPUCCHgroup-r16",
                harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16.to_string());
  }
  if (pdsch_harq_ack_codebook_secondary_pucch_group_r16_present) {
    j.write_str("pdsch-HARQ-ACK-Codebook-secondaryPUCCHgroup-r16",
                pdsch_harq_ack_codebook_secondary_pucch_group_r16.to_string());
  }
  if (p_nr_fr2_r16_present) {
    j.write_int("p-NR-FR2-r16", p_nr_fr2_r16);
  }
  if (p_ue_fr2_r16_present) {
    j.write_int("p-UE-FR2-r16", p_ue_fr2_r16);
  }
  if (nrdc_p_cmode_fr1_r16_present) {
    j.write_str("nrdc-PCmode-FR1-r16", nrdc_p_cmode_fr1_r16.to_string());
  }
  if (nrdc_p_cmode_fr2_r16_present) {
    j.write_str("nrdc-PCmode-FR2-r16", nrdc_p_cmode_fr2_r16.to_string());
  }
  if (pdsch_harq_ack_codebook_r16_present) {
    j.write_str("pdsch-HARQ-ACK-Codebook-r16", "enhancedDynamic");
  }
  if (nfi_total_dai_included_r16_present) {
    j.write_str("nfi-TotalDAI-Included-r16", "true");
  }
  if (ul_total_dai_included_r16_present) {
    j.write_str("ul-TotalDAI-Included-r16", "true");
  }
  if (pdsch_harq_ack_one_shot_feedback_r16_present) {
    j.write_str("pdsch-HARQ-ACK-OneShotFeedback-r16", "true");
  }
  if (pdsch_harq_ack_one_shot_feedback_ndi_r16_present) {
    j.write_str("pdsch-HARQ-ACK-OneShotFeedbackNDI-r16", "true");
  }
  if (pdsch_harq_ack_one_shot_feedback_cbg_r16_present) {
    j.write_str("pdsch-HARQ-ACK-OneShotFeedbackCBG-r16", "true");
  }
  if (dl_assign_idx_dci_0_2_r16_present) {
    j.write_str("downlinkAssignmentIndexDCI-0-2-r16", "enabled");
  }
  if (dl_assign_idx_dci_1_2_r16_present) {
    j.write_str("downlinkAssignmentIndexDCI-1-2-r16", dl_assign_idx_dci_1_2_r16.to_string());
  }
  if (pdsch_harq_ack_codebook_list_r16.is_present()) {
    j.write_fieldname("pdsch-HARQ-ACK-CodebookList-r16");
    pdsch_harq_ack_codebook_list_r16->to_json(j);
  }
  if (ack_nack_feedback_mode_r16_present) {
    j.write_str("ackNackFeedbackMode-r16", ack_nack_feedback_mode_r16.to_string());
  }
  if (pdcch_blind_detection_ca_comb_ind_r16.is_present()) {
    j.write_fieldname("pdcch-BlindDetectionCA-CombIndicator-r16");
    pdcch_blind_detection_ca_comb_ind_r16->to_json(j);
  }
  if (pdcch_blind_detection2_r16.is_present()) {
    j.write_fieldname("pdcch-BlindDetection2-r16");
    pdcch_blind_detection2_r16->to_json(j);
  }
  if (pdcch_blind_detection3_r16.is_present()) {
    j.write_fieldname("pdcch-BlindDetection3-r16");
    pdcch_blind_detection3_r16->to_json(j);
  }
  if (bd_factor_r_r16_present) {
    j.write_str("bdFactorR-r16", "n1");
  }
  if (pdsch_harq_ack_enh_type3_to_add_mod_list_r17.is_present()) {
    j.start_array("pdsch-HARQ-ACK-EnhType3ToAddModList-r17");
    for (const auto& e1 : *pdsch_harq_ack_enh_type3_to_add_mod_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdsch_harq_ack_enh_type3_to_release_list_r17.is_present()) {
    j.start_array("pdsch-HARQ-ACK-EnhType3ToReleaseList-r17");
    for (const auto& e1 : *pdsch_harq_ack_enh_type3_to_release_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17.is_present()) {
    j.start_array("pdsch-HARQ-ACK-EnhType3SecondaryToAddModList-r17");
    for (const auto& e1 : *pdsch_harq_ack_enh_type3_secondary_to_add_mod_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (pdsch_harq_ack_enh_type3_secondary_to_release_list_r17.is_present()) {
    j.start_array("pdsch-HARQ-ACK-EnhType3SecondaryToReleaseList-r17");
    for (const auto& e1 : *pdsch_harq_ack_enh_type3_secondary_to_release_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (pdsch_harq_ack_enh_type3_dci_field_secondary_pucch_group_r17_present) {
    j.write_str("pdsch-HARQ-ACK-EnhType3DCI-FieldSecondaryPUCCHgroup-r17", "enabled");
  }
  if (pdsch_harq_ack_enh_type3_dci_field_r17_present) {
    j.write_str("pdsch-HARQ-ACK-EnhType3DCI-Field-r17", "enabled");
  }
  if (pdsch_harq_ack_retx_r17_present) {
    j.write_str("pdsch-HARQ-ACK-Retx-r17", "enabled");
  }
  if (pdsch_harq_ack_retx_secondary_pucch_group_r17_present) {
    j.write_str("pdsch-HARQ-ACK-RetxSecondaryPUCCHgroup-r17", "enabled");
  }
  if (pucch_s_scell_r17_present) {
    j.write_int("pucch-sSCell-r17", pucch_s_scell_r17);
  }
  if (pucch_s_scell_secondary_pucch_group_r17_present) {
    j.write_int("pucch-sSCellSecondaryPUCCHgroup-r17", pucch_s_scell_secondary_pucch_group_r17);
  }
  if (pucch_s_scell_dyn_r17_present) {
    j.write_str("pucch-sSCellDyn-r17", "enabled");
  }
  if (pucch_s_scell_dyn_secondary_pucch_group_r17_present) {
    j.write_str("pucch-sSCellDynSecondaryPUCCHgroup-r17", "enabled");
  }
  if (pucch_s_scell_pattern_r17.is_present()) {
    j.start_array("pucch-sSCellPattern-r17");
    for (const auto& e1 : *pucch_s_scell_pattern_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (pucch_s_scell_pattern_secondary_pucch_group_r17.is_present()) {
    j.start_array("pucch-sSCellPatternSecondaryPUCCHgroup-r17");
    for (const auto& e1 : *pucch_s_scell_pattern_secondary_pucch_group_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (uci_mux_with_diff_prio_r17_present) {
    j.write_str("uci-MuxWithDiffPrio-r17", "enabled");
  }
  if (uci_mux_with_diff_prio_secondary_pucch_group_r17_present) {
    j.write_str("uci-MuxWithDiffPrioSecondaryPUCCHgroup-r17", "enabled");
  }
  if (simul_pucch_pusch_r17_present) {
    j.write_str("simultaneousPUCCH-PUSCH-r17", "enabled");
  }
  if (simul_pucch_pusch_secondary_pucch_group_r17_present) {
    j.write_str("simultaneousPUCCH-PUSCH-SecondaryPUCCHgroup-r17", "enabled");
  }
  if (prio_low_dg_high_cg_r17_present) {
    j.write_str("prioLowDG-HighCG-r17", "enabled");
  }
  if (prio_high_dg_low_cg_r17_present) {
    j.write_str("prioHighDG-LowCG-r17", "enabled");
  }
  if (two_qcl_type_d_for_pdcch_repeat_r17_present) {
    j.write_str("twoQCLTypeDforPDCCHRepetition-r17", "enabled");
  }
  if (multicast_cfg_r17.is_present()) {
    j.write_fieldname("multicastConfig-r17");
    multicast_cfg_r17->to_json(j);
  }
  if (pdcch_blind_detection_ca_comb_ind_r17.is_present()) {
    j.write_fieldname("pdcch-BlindDetectionCA-CombIndicator-r17");
    pdcch_blind_detection_ca_comb_ind_r17->to_json(j);
  }
  if (simul_sr_pusch_diff_pucch_groups_r17_present) {
    j.write_str("simultaneousSR-PUSCH-diffPUCCH-Groups-r17", "enabled");
  }
  if (intra_band_nc_prach_simul_tx_r17_present) {
    j.write_str("intraBandNC-PRACH-simulTx-r17", "enabled");
  }
  if (pdcch_blind_detection4_r17.is_present()) {
    j.write_fieldname("pdcch-BlindDetection4-r17");
    pdcch_blind_detection4_r17->to_json(j);
  }
  if (simul_pucch_pusch_same_prio_r17_present) {
    j.write_str("simultaneousPUCCH-PUSCH-SamePriority-r17", "enabled");
  }
  if (simul_pucch_pusch_same_prio_secondary_pucch_group_r17_present) {
    j.write_str("simultaneousPUCCH-PUSCH-SamePriority-SecondaryPUCCHgroup-r17", "enabled");
  }
  if (ncr_rnti_r18_present) {
    j.write_int("ncr-RNTI-r18", ncr_rnti_r18);
  }
  if (cell_dtrx_dci_cfg_r18.is_present()) {
    j.write_fieldname("cellDTRX-DCI-config-r18");
    cell_dtrx_dci_cfg_r18->to_json(j);
  }
  if (two_qcl_type_d_for_multi_dci_r18_present) {
    j.write_str("twoQCL-TypeD-ForMultiDCI-r18", "enabled");
  }
  if (enable_type1_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present) {
    j.write_str("enableType1HARQ-ACK-MuxForDL-AssignmentAfterUL-Grant-r18", "enabled");
  }
  if (enable_type2_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present) {
    j.write_str("enableType2HARQ-ACK-MuxForDL-AssignmentAfterUL-Grant-r18", "enabled");
  }
  if (enable_type3_harq_ack_mux_for_dl_assign_after_ul_grant_r18_present) {
    j.write_str("enableType3HARQ-ACK-MuxForDL-AssignmentAfterUL-Grant-r18", "enabled");
  }
  if (enable_diff_pucch_res_r18_present) {
    j.write_str("enableDiffPUCCH-Resource-r18", "enabled");
  }
  if (enable_diff_cb_size_r18_present) {
    j.write_str("enableDiffCB-Size-r18", "enabled");
  }
  j.end_obj();
}

const char* phys_cell_group_cfg_s::pdsch_harq_ack_codebook_opts::to_string() const
{
  static const char* names[] = {"semiStatic", "dynamic"};
  return convert_enum_idx(names, 2, value, "phys_cell_group_cfg_s::pdsch_harq_ack_codebook_e_");
}

const char* phys_cell_group_cfg_s::xscale_opts::to_string() const
{
  static const char* names[] = {"dB0", "dB6", "spare2", "spare1"};
  return convert_enum_idx(names, 4, value, "phys_cell_group_cfg_s::xscale_e_");
}
uint8_t phys_cell_group_cfg_s::xscale_opts::to_number() const
{
  static const uint8_t numbers[] = {0, 6};
  return map_enum_number(numbers, 2, value, "phys_cell_group_cfg_s::xscale_e_");
}

const char* phys_cell_group_cfg_s::harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16_opts::to_string() const
{
  static const char* names[] = {"enabled", "disabled"};
  return convert_enum_idx(
      names, 2, value, "phys_cell_group_cfg_s::harq_ack_spatial_bundling_pucch_secondary_pucch_group_r16_e_");
}

const char* phys_cell_group_cfg_s::harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16_opts::to_string() const
{
  static const char* names[] = {"enabled", "disabled"};
  return convert_enum_idx(
      names, 2, value, "phys_cell_group_cfg_s::harq_ack_spatial_bundling_pusch_secondary_pucch_group_r16_e_");
}

const char* phys_cell_group_cfg_s::pdsch_harq_ack_codebook_secondary_pucch_group_r16_opts::to_string() const
{
  static const char* names[] = {"semiStatic", "dynamic"};
  return convert_enum_idx(
      names, 2, value, "phys_cell_group_cfg_s::pdsch_harq_ack_codebook_secondary_pucch_group_r16_e_");
}

const char* phys_cell_group_cfg_s::nrdc_p_cmode_fr1_r16_opts::to_string() const
{
  static const char* names[] = {"semi-static-mode1", "semi-static-mode2", "dynamic"};
  return convert_enum_idx(names, 3, value, "phys_cell_group_cfg_s::nrdc_p_cmode_fr1_r16_e_");
}
uint8_t phys_cell_group_cfg_s::nrdc_p_cmode_fr1_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "phys_cell_group_cfg_s::nrdc_p_cmode_fr1_r16_e_");
}

const char* phys_cell_group_cfg_s::nrdc_p_cmode_fr2_r16_opts::to_string() const
{
  static const char* names[] = {"semi-static-mode1", "semi-static-mode2", "dynamic"};
  return convert_enum_idx(names, 3, value, "phys_cell_group_cfg_s::nrdc_p_cmode_fr2_r16_e_");
}
uint8_t phys_cell_group_cfg_s::nrdc_p_cmode_fr2_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "phys_cell_group_cfg_s::nrdc_p_cmode_fr2_r16_e_");
}

const char* phys_cell_group_cfg_s::dl_assign_idx_dci_1_2_r16_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n4"};
  return convert_enum_idx(names, 3, value, "phys_cell_group_cfg_s::dl_assign_idx_dci_1_2_r16_e_");
}
uint8_t phys_cell_group_cfg_s::dl_assign_idx_dci_1_2_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 4};
  return map_enum_number(numbers, 3, value, "phys_cell_group_cfg_s::dl_assign_idx_dci_1_2_r16_e_");
}

const char* phys_cell_group_cfg_s::ack_nack_feedback_mode_r16_opts::to_string() const
{
  static const char* names[] = {"joint", "separate"};
  return convert_enum_idx(names, 2, value, "phys_cell_group_cfg_s::ack_nack_feedback_mode_r16_e_");
}

// FrequencyInfoDL ::= SEQUENCE
OCUDUASN_CODE freq_info_dl_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= ref_cell_r18_present;
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), absolute_freq_ssb_present}));

  if (absolute_freq_ssb_present) {
    HANDLE_CODE(pack_integer(bref, absolute_freq_ssb, (uint32_t)0u, (uint32_t)3279165u));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, freq_band_list, 1, 8, integer_packer<uint16_t>(1, 1024)));
  HANDLE_CODE(pack_integer(bref, absolute_freq_point_a, (uint32_t)0u, (uint32_t)3279165u));
  HANDLE_CODE(pack_dyn_seq_of(bref, scs_specific_carrier_list, 1, 5));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(ref_cell_r18_present, 1));
      if (ref_cell_r18_present) {
        HANDLE_CODE(pack_integer(bref, ref_cell_r18, (uint8_t)0u, (uint8_t)31u));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE freq_info_dl_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &absolute_freq_ssb_present}));

  if (absolute_freq_ssb_present) {
    HANDLE_CODE(unpack_integer(absolute_freq_ssb, bref, (uint32_t)0u, (uint32_t)3279165u));
  }
  HANDLE_CODE(unpack_dyn_seq_of(freq_band_list, bref, 1, 8, integer_packer<uint16_t>(1, 1024)));
  HANDLE_CODE(unpack_integer(absolute_freq_point_a, bref, (uint32_t)0u, (uint32_t)3279165u));
  HANDLE_CODE(unpack_dyn_seq_of(scs_specific_carrier_list, bref, 1, 5));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(ref_cell_r18_present, 1));
      if (ref_cell_r18_present) {
        HANDLE_CODE(unpack_integer(ref_cell_r18, bref, (uint8_t)0u, (uint8_t)31u));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void freq_info_dl_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (absolute_freq_ssb_present) {
    j.write_int("absoluteFrequencySSB", absolute_freq_ssb);
  }
  j.start_array("frequencyBandList");
  for (const auto& e1 : freq_band_list) {
    j.write_int(e1);
  }
  j.end_array();
  j.write_int("absoluteFrequencyPointA", absolute_freq_point_a);
  j.start_array("scs-SpecificCarrierList");
  for (const auto& e1 : scs_specific_carrier_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (ref_cell_r18_present) {
    j.write_int("referenceCell-r18", ref_cell_r18);
  }
  j.end_obj();
}

// DownlinkConfigCommon ::= SEQUENCE
OCUDUASN_CODE dl_cfg_common_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= init_dl_bwp_red_cap_r17.is_present();
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), freq_info_dl_present, init_dl_bwp_present}));

  if (freq_info_dl_present) {
    HANDLE_CODE(freq_info_dl.pack(bref));
  }
  if (init_dl_bwp_present) {
    HANDLE_CODE(init_dl_bwp.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(init_dl_bwp_red_cap_r17.is_present(), 1));
      if (init_dl_bwp_red_cap_r17.is_present()) {
        HANDLE_CODE(init_dl_bwp_red_cap_r17->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_cfg_common_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &freq_info_dl_present, &init_dl_bwp_present}));

  if (freq_info_dl_present) {
    HANDLE_CODE(freq_info_dl.unpack(bref));
  }
  if (init_dl_bwp_present) {
    HANDLE_CODE(init_dl_bwp.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(init_dl_bwp_red_cap_r17, bref);
      if (init_dl_bwp_red_cap_r17.is_present()) {
        HANDLE_CODE(init_dl_bwp_red_cap_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void dl_cfg_common_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (freq_info_dl_present) {
    j.write_fieldname("frequencyInfoDL");
    freq_info_dl.to_json(j);
  }
  if (init_dl_bwp_present) {
    j.write_fieldname("initialDownlinkBWP");
    init_dl_bwp.to_json(j);
  }
  if (init_dl_bwp_red_cap_r17.is_present()) {
    j.write_fieldname("initialDownlinkBWP-RedCap-r17");
    init_dl_bwp_red_cap_r17->to_json(j);
  }
  j.end_obj();
}

// FrequencyInfoUL ::= SEQUENCE
OCUDUASN_CODE freq_info_ul_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(2);
  group_flags[0] |= add_spec_emission_v1760_present;
  group_flags[1] |= add_spec_emission_aerial_r18_present;
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         freq_band_list.size() > 0,
                         absolute_freq_point_a_present,
                         add_spec_emission_present,
                         p_max_present,
                         freq_shift7p5khz_present}));

  if (freq_band_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, freq_band_list, 1, 8, integer_packer<uint16_t>(1, 1024)));
  }
  if (absolute_freq_point_a_present) {
    HANDLE_CODE(pack_integer(bref, absolute_freq_point_a, (uint32_t)0u, (uint32_t)3279165u));
  }
  HANDLE_CODE(pack_dyn_seq_of(bref, scs_specific_carrier_list, 1, 5));
  if (add_spec_emission_present) {
    HANDLE_CODE(pack_integer(bref, add_spec_emission, (uint8_t)0u, (uint8_t)7u));
  }
  if (p_max_present) {
    HANDLE_CODE(pack_integer(bref, p_max, (int8_t)-30, (int8_t)33));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(add_spec_emission_v1760_present, 1));
      if (add_spec_emission_v1760_present) {
        HANDLE_CODE(pack_integer(bref, add_spec_emission_v1760, (uint8_t)8u, (uint8_t)39u));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(add_spec_emission_aerial_r18_present, 1));
      if (add_spec_emission_aerial_r18_present) {
        HANDLE_CODE(pack_integer(bref, add_spec_emission_aerial_r18, (uint8_t)0u, (uint8_t)39u));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE freq_info_ul_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool freq_band_list_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &freq_band_list_present,
                           &absolute_freq_point_a_present,
                           &add_spec_emission_present,
                           &p_max_present,
                           &freq_shift7p5khz_present}));

  if (freq_band_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(freq_band_list, bref, 1, 8, integer_packer<uint16_t>(1, 1024)));
  }
  if (absolute_freq_point_a_present) {
    HANDLE_CODE(unpack_integer(absolute_freq_point_a, bref, (uint32_t)0u, (uint32_t)3279165u));
  }
  HANDLE_CODE(unpack_dyn_seq_of(scs_specific_carrier_list, bref, 1, 5));
  if (add_spec_emission_present) {
    HANDLE_CODE(unpack_integer(add_spec_emission, bref, (uint8_t)0u, (uint8_t)7u));
  }
  if (p_max_present) {
    HANDLE_CODE(unpack_integer(p_max, bref, (int8_t)-30, (int8_t)33));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(add_spec_emission_v1760_present, 1));
      if (add_spec_emission_v1760_present) {
        HANDLE_CODE(unpack_integer(add_spec_emission_v1760, bref, (uint8_t)8u, (uint8_t)39u));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(add_spec_emission_aerial_r18_present, 1));
      if (add_spec_emission_aerial_r18_present) {
        HANDLE_CODE(unpack_integer(add_spec_emission_aerial_r18, bref, (uint8_t)0u, (uint8_t)39u));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void freq_info_ul_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (freq_band_list.size() > 0) {
    j.start_array("frequencyBandList");
    for (const auto& e1 : freq_band_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (absolute_freq_point_a_present) {
    j.write_int("absoluteFrequencyPointA", absolute_freq_point_a);
  }
  j.start_array("scs-SpecificCarrierList");
  for (const auto& e1 : scs_specific_carrier_list) {
    e1.to_json(j);
  }
  j.end_array();
  if (add_spec_emission_present) {
    j.write_int("additionalSpectrumEmission", add_spec_emission);
  }
  if (p_max_present) {
    j.write_int("p-Max", p_max);
  }
  if (freq_shift7p5khz_present) {
    j.write_str("frequencyShift7p5khz", "true");
  }
  if (add_spec_emission_v1760_present) {
    j.write_int("additionalSpectrumEmission-v1760", add_spec_emission_v1760);
  }
  if (add_spec_emission_aerial_r18_present) {
    j.write_int("additionalSpectrumEmissionAerial-r18", add_spec_emission_aerial_r18);
  }
  j.end_obj();
}

// UplinkConfigCommon ::= SEQUENCE
OCUDUASN_CODE ul_cfg_common_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {freq_info_ul_present, init_ul_bwp_present}));

  if (freq_info_ul_present) {
    HANDLE_CODE(freq_info_ul.pack(bref));
  }
  if (init_ul_bwp_present) {
    HANDLE_CODE(init_ul_bwp.pack(bref));
  }
  HANDLE_CODE(dummy.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_cfg_common_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_bits(bref, {&freq_info_ul_present, &init_ul_bwp_present}));

  if (freq_info_ul_present) {
    HANDLE_CODE(freq_info_ul.unpack(bref));
  }
  if (init_ul_bwp_present) {
    HANDLE_CODE(init_ul_bwp.unpack(bref));
  }
  HANDLE_CODE(dummy.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void ul_cfg_common_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (freq_info_ul_present) {
    j.write_fieldname("frequencyInfoUL");
    freq_info_ul.to_json(j);
  }
  if (init_ul_bwp_present) {
    j.write_fieldname("initialUplinkBWP");
    init_ul_bwp.to_json(j);
  }
  j.write_str("dummy", dummy.to_string());
  j.end_obj();
}

// HighSpeedConfig-v1700 ::= SEQUENCE
OCUDUASN_CODE high_speed_cfg_v1700_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {false,
                         high_speed_meas_ca_scell_r17_present,
                         high_speed_meas_inter_freq_r17_present,
                         high_speed_demod_ca_scell_r17_present}));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE high_speed_cfg_v1700_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &high_speed_meas_ca_scell_r17_present,
                           &high_speed_meas_inter_freq_r17_present,
                           &high_speed_demod_ca_scell_r17_present}));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void high_speed_cfg_v1700_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (high_speed_meas_ca_scell_r17_present) {
    j.write_str("highSpeedMeasCA-Scell-r17", "true");
  }
  if (high_speed_meas_inter_freq_r17_present) {
    j.write_str("highSpeedMeasInterFreq-r17", "true");
  }
  if (high_speed_demod_ca_scell_r17_present) {
    j.write_str("highSpeedDemodCA-Scell-r17", "true");
  }
  j.end_obj();
}

// UplinkConfigCommon-v1700 ::= SEQUENCE
OCUDUASN_CODE ul_cfg_common_v1700_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(init_ul_bwp_red_cap_r17_present, 1));

  if (init_ul_bwp_red_cap_r17_present) {
    HANDLE_CODE(init_ul_bwp_red_cap_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_cfg_common_v1700_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(init_ul_bwp_red_cap_r17_present, 1));

  if (init_ul_bwp_red_cap_r17_present) {
    HANDLE_CODE(init_ul_bwp_red_cap_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void ul_cfg_common_v1700_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (init_ul_bwp_red_cap_r17_present) {
    j.write_fieldname("initialUplinkBWP-RedCap-r17");
    init_ul_bwp_red_cap_r17.to_json(j);
  }
  j.end_obj();
}

// ServingCellConfigCommon ::= SEQUENCE
OCUDUASN_CODE serving_cell_cfg_common_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(5);
  group_flags[0] |= ch_access_mode_r16.is_present();
  group_flags[0] |= discovery_burst_win_len_r16_present;
  group_flags[0] |= ssb_position_qcl_r16_present;
  group_flags[0] |= high_speed_cfg_r16.is_present();
  group_flags[1] |= high_speed_cfg_v1700.is_present();
  group_flags[1] |= ch_access_mode2_r17_present;
  group_flags[1] |= discovery_burst_win_len_r17_present;
  group_flags[1] |= ssb_position_qcl_r17_present;
  group_flags[1] |= high_speed_cfg_fr2_r17.is_present();
  group_flags[1] |= ul_cfg_common_v1700.is_present();
  group_flags[1] |= ntn_cfg_r17.is_present();
  group_flags[2] |= feature_priorities_r17.is_present();
  group_flags[3] |= ra_ch_access_r17_present;
  group_flags[4] |= feature_priorities_v1800.is_present();
  group_flags[4] |= atg_cfg_r18.is_present();
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         pci_present,
                         dl_cfg_common_present,
                         ul_cfg_common_present,
                         supplementary_ul_cfg_present,
                         n_timing_advance_offset_present,
                         ssb_positions_in_burst_present,
                         ssb_periodicity_serving_cell_present,
                         lte_crs_to_match_around_present,
                         rate_match_pattern_to_add_mod_list.size() > 0,
                         rate_match_pattern_to_release_list.size() > 0,
                         ssb_subcarrier_spacing_present,
                         tdd_ul_dl_cfg_common_present}));

  if (pci_present) {
    HANDLE_CODE(pack_integer(bref, pci, (uint16_t)0u, (uint16_t)1007u));
  }
  if (dl_cfg_common_present) {
    HANDLE_CODE(dl_cfg_common.pack(bref));
  }
  if (ul_cfg_common_present) {
    HANDLE_CODE(ul_cfg_common.pack(bref));
  }
  if (supplementary_ul_cfg_present) {
    HANDLE_CODE(supplementary_ul_cfg.pack(bref));
  }
  if (n_timing_advance_offset_present) {
    HANDLE_CODE(n_timing_advance_offset.pack(bref));
  }
  if (ssb_positions_in_burst_present) {
    HANDLE_CODE(ssb_positions_in_burst.pack(bref));
  }
  if (ssb_periodicity_serving_cell_present) {
    HANDLE_CODE(ssb_periodicity_serving_cell.pack(bref));
  }
  HANDLE_CODE(dmrs_type_a_position.pack(bref));
  if (lte_crs_to_match_around_present) {
    HANDLE_CODE(lte_crs_to_match_around.pack(bref));
  }
  if (rate_match_pattern_to_add_mod_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, rate_match_pattern_to_add_mod_list, 1, 4));
  }
  if (rate_match_pattern_to_release_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, rate_match_pattern_to_release_list, 1, 4, integer_packer<uint8_t>(0, 3)));
  }
  if (ssb_subcarrier_spacing_present) {
    HANDLE_CODE(ssb_subcarrier_spacing.pack(bref));
  }
  if (tdd_ul_dl_cfg_common_present) {
    HANDLE_CODE(tdd_ul_dl_cfg_common.pack(bref));
  }
  HANDLE_CODE(pack_integer(bref, ss_pbch_block_pwr, (int8_t)-60, (int8_t)50));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {ch_access_mode_r16.is_present(),
                             discovery_burst_win_len_r16_present,
                             ssb_position_qcl_r16_present,
                             high_speed_cfg_r16.is_present()}));
      if (ch_access_mode_r16.is_present()) {
        HANDLE_CODE(ch_access_mode_r16->pack(bref));
      }
      if (discovery_burst_win_len_r16_present) {
        HANDLE_CODE(discovery_burst_win_len_r16.pack(bref));
      }
      if (ssb_position_qcl_r16_present) {
        HANDLE_CODE(ssb_position_qcl_r16.pack(bref));
      }
      if (high_speed_cfg_r16.is_present()) {
        HANDLE_CODE(high_speed_cfg_r16->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {high_speed_cfg_v1700.is_present(),
                             ch_access_mode2_r17_present,
                             discovery_burst_win_len_r17_present,
                             ssb_position_qcl_r17_present,
                             high_speed_cfg_fr2_r17.is_present(),
                             ul_cfg_common_v1700.is_present(),
                             ntn_cfg_r17.is_present()}));
      if (high_speed_cfg_v1700.is_present()) {
        HANDLE_CODE(high_speed_cfg_v1700->pack(bref));
      }
      if (discovery_burst_win_len_r17_present) {
        HANDLE_CODE(discovery_burst_win_len_r17.pack(bref));
      }
      if (ssb_position_qcl_r17_present) {
        HANDLE_CODE(ssb_position_qcl_r17.pack(bref));
      }
      if (high_speed_cfg_fr2_r17.is_present()) {
        HANDLE_CODE(high_speed_cfg_fr2_r17->pack(bref));
      }
      if (ul_cfg_common_v1700.is_present()) {
        HANDLE_CODE(ul_cfg_common_v1700->pack(bref));
      }
      if (ntn_cfg_r17.is_present()) {
        HANDLE_CODE(ntn_cfg_r17->pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(feature_priorities_r17.is_present(), 1));
      if (feature_priorities_r17.is_present()) {
        HANDLE_CODE(pack_bits(bref,
                              {feature_priorities_r17->red_cap_prio_r17_present,
                               feature_priorities_r17->slicing_prio_r17_present,
                               feature_priorities_r17->msg3_repeats_prio_r17_present,
                               feature_priorities_r17->sdt_prio_r17_present}));
        if (feature_priorities_r17->red_cap_prio_r17_present) {
          HANDLE_CODE(pack_integer(bref, feature_priorities_r17->red_cap_prio_r17, (uint8_t)0u, (uint8_t)7u));
        }
        if (feature_priorities_r17->slicing_prio_r17_present) {
          HANDLE_CODE(pack_integer(bref, feature_priorities_r17->slicing_prio_r17, (uint8_t)0u, (uint8_t)7u));
        }
        if (feature_priorities_r17->msg3_repeats_prio_r17_present) {
          HANDLE_CODE(pack_integer(bref, feature_priorities_r17->msg3_repeats_prio_r17, (uint8_t)0u, (uint8_t)7u));
        }
        if (feature_priorities_r17->sdt_prio_r17_present) {
          HANDLE_CODE(pack_integer(bref, feature_priorities_r17->sdt_prio_r17, (uint8_t)0u, (uint8_t)7u));
        }
      }
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(ra_ch_access_r17_present, 1));
    }
    if (group_flags[4]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {feature_priorities_v1800.is_present(), atg_cfg_r18.is_present()}));
      if (feature_priorities_v1800.is_present()) {
        HANDLE_CODE(pack_bits(bref,
                              {feature_priorities_v1800->msg1_repeats_prio_r18_present,
                               feature_priorities_v1800->ered_cap_prio_r18_present}));
        if (feature_priorities_v1800->msg1_repeats_prio_r18_present) {
          HANDLE_CODE(pack_integer(bref, feature_priorities_v1800->msg1_repeats_prio_r18, (uint8_t)0u, (uint8_t)7u));
        }
        if (feature_priorities_v1800->ered_cap_prio_r18_present) {
          HANDLE_CODE(pack_integer(bref, feature_priorities_v1800->ered_cap_prio_r18, (uint8_t)0u, (uint8_t)7u));
        }
      }
      if (atg_cfg_r18.is_present()) {
        HANDLE_CODE(atg_cfg_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE serving_cell_cfg_common_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool rate_match_pattern_to_add_mod_list_present;
  bool rate_match_pattern_to_release_list_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &pci_present,
                           &dl_cfg_common_present,
                           &ul_cfg_common_present,
                           &supplementary_ul_cfg_present,
                           &n_timing_advance_offset_present,
                           &ssb_positions_in_burst_present,
                           &ssb_periodicity_serving_cell_present,
                           &lte_crs_to_match_around_present,
                           &rate_match_pattern_to_add_mod_list_present,
                           &rate_match_pattern_to_release_list_present,
                           &ssb_subcarrier_spacing_present,
                           &tdd_ul_dl_cfg_common_present}));

  if (pci_present) {
    HANDLE_CODE(unpack_integer(pci, bref, (uint16_t)0u, (uint16_t)1007u));
  }
  if (dl_cfg_common_present) {
    HANDLE_CODE(dl_cfg_common.unpack(bref));
  }
  if (ul_cfg_common_present) {
    HANDLE_CODE(ul_cfg_common.unpack(bref));
  }
  if (supplementary_ul_cfg_present) {
    HANDLE_CODE(supplementary_ul_cfg.unpack(bref));
  }
  if (n_timing_advance_offset_present) {
    HANDLE_CODE(n_timing_advance_offset.unpack(bref));
  }
  if (ssb_positions_in_burst_present) {
    HANDLE_CODE(ssb_positions_in_burst.unpack(bref));
  }
  if (ssb_periodicity_serving_cell_present) {
    HANDLE_CODE(ssb_periodicity_serving_cell.unpack(bref));
  }
  HANDLE_CODE(dmrs_type_a_position.unpack(bref));
  if (lte_crs_to_match_around_present) {
    HANDLE_CODE(lte_crs_to_match_around.unpack(bref));
  }
  if (rate_match_pattern_to_add_mod_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(rate_match_pattern_to_add_mod_list, bref, 1, 4));
  }
  if (rate_match_pattern_to_release_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(rate_match_pattern_to_release_list, bref, 1, 4, integer_packer<uint8_t>(0, 3)));
  }
  if (ssb_subcarrier_spacing_present) {
    HANDLE_CODE(ssb_subcarrier_spacing.unpack(bref));
  }
  if (tdd_ul_dl_cfg_common_present) {
    HANDLE_CODE(tdd_ul_dl_cfg_common.unpack(bref));
  }
  HANDLE_CODE(unpack_integer(ss_pbch_block_pwr, bref, (int8_t)-60, (int8_t)50));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(ch_access_mode_r16, bref);
      HANDLE_CODE(unpack_bits(bref, {&discovery_burst_win_len_r16_present, &ssb_position_qcl_r16_present}));
      unpack_presence_flag(high_speed_cfg_r16, bref);
      if (ch_access_mode_r16.is_present()) {
        HANDLE_CODE(ch_access_mode_r16->unpack(bref));
      }
      if (discovery_burst_win_len_r16_present) {
        HANDLE_CODE(discovery_burst_win_len_r16.unpack(bref));
      }
      if (ssb_position_qcl_r16_present) {
        HANDLE_CODE(ssb_position_qcl_r16.unpack(bref));
      }
      if (high_speed_cfg_r16.is_present()) {
        HANDLE_CODE(high_speed_cfg_r16->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(high_speed_cfg_v1700, bref);
      HANDLE_CODE(unpack_bits(
          bref, {&ch_access_mode2_r17_present, &discovery_burst_win_len_r17_present, &ssb_position_qcl_r17_present}));
      unpack_presence_flag(high_speed_cfg_fr2_r17, bref);
      unpack_presence_flag(ul_cfg_common_v1700, bref);
      unpack_presence_flag(ntn_cfg_r17, bref);
      if (high_speed_cfg_v1700.is_present()) {
        HANDLE_CODE(high_speed_cfg_v1700->unpack(bref));
      }
      if (discovery_burst_win_len_r17_present) {
        HANDLE_CODE(discovery_burst_win_len_r17.unpack(bref));
      }
      if (ssb_position_qcl_r17_present) {
        HANDLE_CODE(ssb_position_qcl_r17.unpack(bref));
      }
      if (high_speed_cfg_fr2_r17.is_present()) {
        HANDLE_CODE(high_speed_cfg_fr2_r17->unpack(bref));
      }
      if (ul_cfg_common_v1700.is_present()) {
        HANDLE_CODE(ul_cfg_common_v1700->unpack(bref));
      }
      if (ntn_cfg_r17.is_present()) {
        HANDLE_CODE(ntn_cfg_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(feature_priorities_r17, bref);
      if (feature_priorities_r17.is_present()) {
        HANDLE_CODE(unpack_bits(bref,
                                {&feature_priorities_r17->red_cap_prio_r17_present,
                                 &feature_priorities_r17->slicing_prio_r17_present,
                                 &feature_priorities_r17->msg3_repeats_prio_r17_present,
                                 &feature_priorities_r17->sdt_prio_r17_present}));
        if (feature_priorities_r17->red_cap_prio_r17_present) {
          HANDLE_CODE(unpack_integer(feature_priorities_r17->red_cap_prio_r17, bref, (uint8_t)0u, (uint8_t)7u));
        }
        if (feature_priorities_r17->slicing_prio_r17_present) {
          HANDLE_CODE(unpack_integer(feature_priorities_r17->slicing_prio_r17, bref, (uint8_t)0u, (uint8_t)7u));
        }
        if (feature_priorities_r17->msg3_repeats_prio_r17_present) {
          HANDLE_CODE(unpack_integer(feature_priorities_r17->msg3_repeats_prio_r17, bref, (uint8_t)0u, (uint8_t)7u));
        }
        if (feature_priorities_r17->sdt_prio_r17_present) {
          HANDLE_CODE(unpack_integer(feature_priorities_r17->sdt_prio_r17, bref, (uint8_t)0u, (uint8_t)7u));
        }
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(ra_ch_access_r17_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(feature_priorities_v1800, bref);
      unpack_presence_flag(atg_cfg_r18, bref);
      if (feature_priorities_v1800.is_present()) {
        HANDLE_CODE(unpack_bits(bref,
                                {&feature_priorities_v1800->msg1_repeats_prio_r18_present,
                                 &feature_priorities_v1800->ered_cap_prio_r18_present}));
        if (feature_priorities_v1800->msg1_repeats_prio_r18_present) {
          HANDLE_CODE(unpack_integer(feature_priorities_v1800->msg1_repeats_prio_r18, bref, (uint8_t)0u, (uint8_t)7u));
        }
        if (feature_priorities_v1800->ered_cap_prio_r18_present) {
          HANDLE_CODE(unpack_integer(feature_priorities_v1800->ered_cap_prio_r18, bref, (uint8_t)0u, (uint8_t)7u));
        }
      }
      if (atg_cfg_r18.is_present()) {
        HANDLE_CODE(atg_cfg_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void serving_cell_cfg_common_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (pci_present) {
    j.write_int("physCellId", pci);
  }
  if (dl_cfg_common_present) {
    j.write_fieldname("downlinkConfigCommon");
    dl_cfg_common.to_json(j);
  }
  if (ul_cfg_common_present) {
    j.write_fieldname("uplinkConfigCommon");
    ul_cfg_common.to_json(j);
  }
  if (supplementary_ul_cfg_present) {
    j.write_fieldname("supplementaryUplinkConfig");
    supplementary_ul_cfg.to_json(j);
  }
  if (n_timing_advance_offset_present) {
    j.write_str("n-TimingAdvanceOffset", n_timing_advance_offset.to_string());
  }
  if (ssb_positions_in_burst_present) {
    j.write_fieldname("ssb-PositionsInBurst");
    ssb_positions_in_burst.to_json(j);
  }
  if (ssb_periodicity_serving_cell_present) {
    j.write_str("ssb-periodicityServingCell", ssb_periodicity_serving_cell.to_string());
  }
  j.write_str("dmrs-TypeA-Position", dmrs_type_a_position.to_string());
  if (lte_crs_to_match_around_present) {
    j.write_fieldname("lte-CRS-ToMatchAround");
    lte_crs_to_match_around.to_json(j);
  }
  if (rate_match_pattern_to_add_mod_list.size() > 0) {
    j.start_array("rateMatchPatternToAddModList");
    for (const auto& e1 : rate_match_pattern_to_add_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (rate_match_pattern_to_release_list.size() > 0) {
    j.start_array("rateMatchPatternToReleaseList");
    for (const auto& e1 : rate_match_pattern_to_release_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ssb_subcarrier_spacing_present) {
    j.write_str("ssbSubcarrierSpacing", ssb_subcarrier_spacing.to_string());
  }
  if (tdd_ul_dl_cfg_common_present) {
    j.write_fieldname("tdd-UL-DL-ConfigurationCommon");
    tdd_ul_dl_cfg_common.to_json(j);
  }
  j.write_int("ss-PBCH-BlockPower", ss_pbch_block_pwr);
  if (ch_access_mode_r16.is_present()) {
    j.write_fieldname("channelAccessMode-r16");
    ch_access_mode_r16->to_json(j);
  }
  if (discovery_burst_win_len_r16_present) {
    j.write_str("discoveryBurstWindowLength-r16", discovery_burst_win_len_r16.to_string());
  }
  if (ssb_position_qcl_r16_present) {
    j.write_str("ssb-PositionQCL-r16", ssb_position_qcl_r16.to_string());
  }
  if (high_speed_cfg_r16.is_present()) {
    j.write_fieldname("highSpeedConfig-r16");
    high_speed_cfg_r16->to_json(j);
  }
  if (high_speed_cfg_v1700.is_present()) {
    j.write_fieldname("highSpeedConfig-v1700");
    high_speed_cfg_v1700->to_json(j);
  }
  if (ch_access_mode2_r17_present) {
    j.write_str("channelAccessMode2-r17", "enabled");
  }
  if (discovery_burst_win_len_r17_present) {
    j.write_str("discoveryBurstWindowLength-r17", discovery_burst_win_len_r17.to_string());
  }
  if (ssb_position_qcl_r17_present) {
    j.write_str("ssb-PositionQCL-r17", ssb_position_qcl_r17.to_string());
  }
  if (high_speed_cfg_fr2_r17.is_present()) {
    j.write_fieldname("highSpeedConfigFR2-r17");
    high_speed_cfg_fr2_r17->to_json(j);
  }
  if (ul_cfg_common_v1700.is_present()) {
    j.write_fieldname("uplinkConfigCommon-v1700");
    ul_cfg_common_v1700->to_json(j);
  }
  if (ntn_cfg_r17.is_present()) {
    j.write_fieldname("ntn-Config-r17");
    ntn_cfg_r17->to_json(j);
  }
  if (feature_priorities_r17.is_present()) {
    j.write_fieldname("featurePriorities-r17");
    j.start_obj();
    if (feature_priorities_r17->red_cap_prio_r17_present) {
      j.write_int("redCapPriority-r17", feature_priorities_r17->red_cap_prio_r17);
    }
    if (feature_priorities_r17->slicing_prio_r17_present) {
      j.write_int("slicingPriority-r17", feature_priorities_r17->slicing_prio_r17);
    }
    if (feature_priorities_r17->msg3_repeats_prio_r17_present) {
      j.write_int("msg3-Repetitions-Priority-r17", feature_priorities_r17->msg3_repeats_prio_r17);
    }
    if (feature_priorities_r17->sdt_prio_r17_present) {
      j.write_int("sdt-Priority-r17", feature_priorities_r17->sdt_prio_r17);
    }
    j.end_obj();
  }
  if (ra_ch_access_r17_present) {
    j.write_str("ra-ChannelAccess-r17", "enabled");
  }
  if (feature_priorities_v1800.is_present()) {
    j.write_fieldname("featurePriorities-v1800");
    j.start_obj();
    if (feature_priorities_v1800->msg1_repeats_prio_r18_present) {
      j.write_int("msg1-Repetitions-Priority-r18", feature_priorities_v1800->msg1_repeats_prio_r18);
    }
    if (feature_priorities_v1800->ered_cap_prio_r18_present) {
      j.write_int("eRedCapPriority-r18", feature_priorities_v1800->ered_cap_prio_r18);
    }
    j.end_obj();
  }
  if (atg_cfg_r18.is_present()) {
    j.write_fieldname("atg-Config-r18");
    atg_cfg_r18->to_json(j);
  }
  j.end_obj();
}

const char* serving_cell_cfg_common_s::n_timing_advance_offset_opts::to_string() const
{
  static const char* names[] = {"n0", "n25600", "n39936"};
  return convert_enum_idx(names, 3, value, "serving_cell_cfg_common_s::n_timing_advance_offset_e_");
}
uint16_t serving_cell_cfg_common_s::n_timing_advance_offset_opts::to_number() const
{
  static const uint16_t numbers[] = {0, 25600, 39936};
  return map_enum_number(numbers, 3, value, "serving_cell_cfg_common_s::n_timing_advance_offset_e_");
}

void serving_cell_cfg_common_s::ssb_positions_in_burst_c_::destroy_()
{
  switch (type_) {
    case types::short_bitmap:
      c.destroy<fixed_bitstring<4>>();
      break;
    case types::medium_bitmap:
      c.destroy<fixed_bitstring<8>>();
      break;
    case types::long_bitmap:
      c.destroy<fixed_bitstring<64>>();
      break;
    default:
      break;
  }
}
void serving_cell_cfg_common_s::ssb_positions_in_burst_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::short_bitmap:
      c.init<fixed_bitstring<4>>();
      break;
    case types::medium_bitmap:
      c.init<fixed_bitstring<8>>();
      break;
    case types::long_bitmap:
      c.init<fixed_bitstring<64>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ssb_positions_in_burst_c_");
  }
}
serving_cell_cfg_common_s::ssb_positions_in_burst_c_::ssb_positions_in_burst_c_(
    const serving_cell_cfg_common_s::ssb_positions_in_burst_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::short_bitmap:
      c.init(other.c.get<fixed_bitstring<4>>());
      break;
    case types::medium_bitmap:
      c.init(other.c.get<fixed_bitstring<8>>());
      break;
    case types::long_bitmap:
      c.init(other.c.get<fixed_bitstring<64>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ssb_positions_in_burst_c_");
  }
}
serving_cell_cfg_common_s::ssb_positions_in_burst_c_& serving_cell_cfg_common_s::ssb_positions_in_burst_c_::operator=(
    const serving_cell_cfg_common_s::ssb_positions_in_burst_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::short_bitmap:
      c.set(other.c.get<fixed_bitstring<4>>());
      break;
    case types::medium_bitmap:
      c.set(other.c.get<fixed_bitstring<8>>());
      break;
    case types::long_bitmap:
      c.set(other.c.get<fixed_bitstring<64>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ssb_positions_in_burst_c_");
  }

  return *this;
}
fixed_bitstring<4>& serving_cell_cfg_common_s::ssb_positions_in_burst_c_::set_short_bitmap()
{
  set(types::short_bitmap);
  return c.get<fixed_bitstring<4>>();
}
fixed_bitstring<8>& serving_cell_cfg_common_s::ssb_positions_in_burst_c_::set_medium_bitmap()
{
  set(types::medium_bitmap);
  return c.get<fixed_bitstring<8>>();
}
fixed_bitstring<64>& serving_cell_cfg_common_s::ssb_positions_in_burst_c_::set_long_bitmap()
{
  set(types::long_bitmap);
  return c.get<fixed_bitstring<64>>();
}
void serving_cell_cfg_common_s::ssb_positions_in_burst_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::short_bitmap:
      j.write_fieldname("shortBitmap");
      c.get<fixed_bitstring<4>>().to_json(j);
      break;
    case types::medium_bitmap:
      j.write_fieldname("mediumBitmap");
      c.get<fixed_bitstring<8>>().to_json(j);
      break;
    case types::long_bitmap:
      j.write_fieldname("longBitmap");
      c.get<fixed_bitstring<64>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ssb_positions_in_burst_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE serving_cell_cfg_common_s::ssb_positions_in_burst_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::short_bitmap:
      HANDLE_CODE(c.get<fixed_bitstring<4>>().pack(bref));
      break;
    case types::medium_bitmap:
      HANDLE_CODE(c.get<fixed_bitstring<8>>().pack(bref));
      break;
    case types::long_bitmap:
      HANDLE_CODE(c.get<fixed_bitstring<64>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ssb_positions_in_burst_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE serving_cell_cfg_common_s::ssb_positions_in_burst_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::short_bitmap:
      HANDLE_CODE(c.get<fixed_bitstring<4>>().unpack(bref));
      break;
    case types::medium_bitmap:
      HANDLE_CODE(c.get<fixed_bitstring<8>>().unpack(bref));
      break;
    case types::long_bitmap:
      HANDLE_CODE(c.get<fixed_bitstring<64>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ssb_positions_in_burst_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* serving_cell_cfg_common_s::ssb_positions_in_burst_c_::types_opts::to_string() const
{
  static const char* names[] = {"shortBitmap", "mediumBitmap", "longBitmap"};
  return convert_enum_idx(names, 3, value, "serving_cell_cfg_common_s::ssb_positions_in_burst_c_::types");
}

const char* serving_cell_cfg_common_s::ssb_periodicity_serving_cell_opts::to_string() const
{
  static const char* names[] = {"ms5", "ms10", "ms20", "ms40", "ms80", "ms160", "spare2", "spare1"};
  return convert_enum_idx(names, 8, value, "serving_cell_cfg_common_s::ssb_periodicity_serving_cell_e_");
}
uint8_t serving_cell_cfg_common_s::ssb_periodicity_serving_cell_opts::to_number() const
{
  static const uint8_t numbers[] = {5, 10, 20, 40, 80, 160};
  return map_enum_number(numbers, 6, value, "serving_cell_cfg_common_s::ssb_periodicity_serving_cell_e_");
}

const char* serving_cell_cfg_common_s::dmrs_type_a_position_opts::to_string() const
{
  static const char* names[] = {"pos2", "pos3"};
  return convert_enum_idx(names, 2, value, "serving_cell_cfg_common_s::dmrs_type_a_position_e_");
}
uint8_t serving_cell_cfg_common_s::dmrs_type_a_position_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 3};
  return map_enum_number(numbers, 2, value, "serving_cell_cfg_common_s::dmrs_type_a_position_e_");
}

void serving_cell_cfg_common_s::ch_access_mode_r16_c_::set(types::options e)
{
  type_ = e;
}
void serving_cell_cfg_common_s::ch_access_mode_r16_c_::set_dyn()
{
  set(types::dyn);
}
semi_static_ch_access_cfg_r16_s& serving_cell_cfg_common_s::ch_access_mode_r16_c_::set_semi_static()
{
  set(types::semi_static);
  return c;
}
void serving_cell_cfg_common_s::ch_access_mode_r16_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::dyn:
      j.write_null("dynamic");
      break;
    case types::semi_static:
      j.write_fieldname("semiStatic");
      c.to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ch_access_mode_r16_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE serving_cell_cfg_common_s::ch_access_mode_r16_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::dyn:
      break;
    case types::semi_static:
      HANDLE_CODE(c.pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ch_access_mode_r16_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE serving_cell_cfg_common_s::ch_access_mode_r16_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::dyn:
      break;
    case types::semi_static:
      HANDLE_CODE(c.unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "serving_cell_cfg_common_s::ch_access_mode_r16_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* serving_cell_cfg_common_s::ch_access_mode_r16_c_::types_opts::to_string() const
{
  static const char* names[] = {"dynamic", "semiStatic"};
  return convert_enum_idx(names, 2, value, "serving_cell_cfg_common_s::ch_access_mode_r16_c_::types");
}

const char* serving_cell_cfg_common_s::discovery_burst_win_len_r16_opts::to_string() const
{
  static const char* names[] = {"ms0dot5", "ms1", "ms2", "ms3", "ms4", "ms5"};
  return convert_enum_idx(names, 6, value, "serving_cell_cfg_common_s::discovery_burst_win_len_r16_e_");
}
float serving_cell_cfg_common_s::discovery_burst_win_len_r16_opts::to_number() const
{
  static const float numbers[] = {0.5, 1.0, 2.0, 3.0, 4.0, 5.0};
  return map_enum_number(numbers, 6, value, "serving_cell_cfg_common_s::discovery_burst_win_len_r16_e_");
}
const char* serving_cell_cfg_common_s::discovery_burst_win_len_r16_opts::to_number_string() const
{
  static const char* number_strs[] = {"0.5", "1", "2", "3", "4", "5"};
  return convert_enum_idx(number_strs, 6, value, "serving_cell_cfg_common_s::discovery_burst_win_len_r16_e_");
}

const char* serving_cell_cfg_common_s::discovery_burst_win_len_r17_opts::to_string() const
{
  static const char* names[] = {"ms0dot125", "ms0dot25", "ms0dot5", "ms0dot75", "ms1", "ms1dot25"};
  return convert_enum_idx(names, 6, value, "serving_cell_cfg_common_s::discovery_burst_win_len_r17_e_");
}
float serving_cell_cfg_common_s::discovery_burst_win_len_r17_opts::to_number() const
{
  static const float numbers[] = {0.125, 0.25, 0.5, 0.75, 1.0, 1.25};
  return map_enum_number(numbers, 6, value, "serving_cell_cfg_common_s::discovery_burst_win_len_r17_e_");
}
const char* serving_cell_cfg_common_s::discovery_burst_win_len_r17_opts::to_number_string() const
{
  static const char* number_strs[] = {"0.125", "0.25", "0.5", "0.75", "1", "1.25"};
  return convert_enum_idx(number_strs, 6, value, "serving_cell_cfg_common_s::discovery_burst_win_len_r17_e_");
}

// RACH-ConfigDedicated ::= SEQUENCE
OCUDUASN_CODE rach_cfg_ded_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= ra_prioritization_two_step_r16.is_present();
  group_flags[0] |= cfra_two_step_r16.is_present();
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), cfra_present, ra_prioritization_present}));

  if (cfra_present) {
    HANDLE_CODE(cfra.pack(bref));
  }
  if (ra_prioritization_present) {
    HANDLE_CODE(ra_prioritization.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {ra_prioritization_two_step_r16.is_present(), cfra_two_step_r16.is_present()}));
      if (ra_prioritization_two_step_r16.is_present()) {
        HANDLE_CODE(ra_prioritization_two_step_r16->pack(bref));
      }
      if (cfra_two_step_r16.is_present()) {
        HANDLE_CODE(cfra_two_step_r16->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rach_cfg_ded_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &cfra_present, &ra_prioritization_present}));

  if (cfra_present) {
    HANDLE_CODE(cfra.unpack(bref));
  }
  if (ra_prioritization_present) {
    HANDLE_CODE(ra_prioritization.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(ra_prioritization_two_step_r16, bref);
      unpack_presence_flag(cfra_two_step_r16, bref);
      if (ra_prioritization_two_step_r16.is_present()) {
        HANDLE_CODE(ra_prioritization_two_step_r16->unpack(bref));
      }
      if (cfra_two_step_r16.is_present()) {
        HANDLE_CODE(cfra_two_step_r16->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void rach_cfg_ded_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (cfra_present) {
    j.write_fieldname("cfra");
    cfra.to_json(j);
  }
  if (ra_prioritization_present) {
    j.write_fieldname("ra-Prioritization");
    ra_prioritization.to_json(j);
  }
  if (ra_prioritization_two_step_r16.is_present()) {
    j.write_fieldname("ra-PrioritizationTwoStep-r16");
    ra_prioritization_two_step_r16->to_json(j);
  }
  if (cfra_two_step_r16.is_present()) {
    j.write_fieldname("cfra-TwoStep-r16");
    cfra_two_step_r16->to_json(j);
  }
  j.end_obj();
}

// DAPS-UplinkPowerConfig-r16 ::= SEQUENCE
OCUDUASN_CODE daps_ul_pwr_cfg_r16_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, p_daps_source_r16, (int8_t)-30, (int8_t)33));
  HANDLE_CODE(pack_integer(bref, p_daps_target_r16, (int8_t)-30, (int8_t)33));
  HANDLE_CODE(ul_pwr_sharing_daps_mode_r16.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE daps_ul_pwr_cfg_r16_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(p_daps_source_r16, bref, (int8_t)-30, (int8_t)33));
  HANDLE_CODE(unpack_integer(p_daps_target_r16, bref, (int8_t)-30, (int8_t)33));
  HANDLE_CODE(ul_pwr_sharing_daps_mode_r16.unpack(bref));

  return OCUDUASN_SUCCESS;
}
void daps_ul_pwr_cfg_r16_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("p-DAPS-Source-r16", p_daps_source_r16);
  j.write_int("p-DAPS-Target-r16", p_daps_target_r16);
  j.write_str("uplinkPowerSharingDAPS-Mode-r16", ul_pwr_sharing_daps_mode_r16.to_string());
  j.end_obj();
}

const char* daps_ul_pwr_cfg_r16_s::ul_pwr_sharing_daps_mode_r16_opts::to_string() const
{
  static const char* names[] = {"semi-static-mode1", "semi-static-mode2", "dynamic"};
  return convert_enum_idx(names, 3, value, "daps_ul_pwr_cfg_r16_s::ul_pwr_sharing_daps_mode_r16_e_");
}
uint8_t daps_ul_pwr_cfg_r16_s::ul_pwr_sharing_daps_mode_r16_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "daps_ul_pwr_cfg_r16_s::ul_pwr_sharing_daps_mode_r16_e_");
}

// SL-PathSwitchConfig-r17 ::= SEQUENCE
OCUDUASN_CODE sl_path_switch_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(target_relay_ue_id_r17.pack(bref));
  HANDLE_CODE(t420_r17.pack(bref));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sl_path_switch_cfg_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(target_relay_ue_id_r17.unpack(bref));
  HANDLE_CODE(t420_r17.unpack(bref));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sl_path_switch_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("targetRelayUE-Identity-r17");
  target_relay_ue_id_r17.to_json(j);
  j.write_str("t420-r17", t420_r17.to_string());
  j.end_obj();
}

const char* sl_path_switch_cfg_r17_s::t420_r17_opts::to_string() const
{
  static const char* names[] = {"ms50", "ms100", "ms150", "ms200", "ms500", "ms1000", "ms2000", "ms10000"};
  return convert_enum_idx(names, 8, value, "sl_path_switch_cfg_r17_s::t420_r17_e_");
}
uint16_t sl_path_switch_cfg_r17_s::t420_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {50, 100, 150, 200, 500, 1000, 2000, 10000};
  return map_enum_number(numbers, 8, value, "sl_path_switch_cfg_r17_s::t420_r17_e_");
}

// RACH-LessHO-r18 ::= SEQUENCE
OCUDUASN_CODE rach_less_ho_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, target_nta_r18_present, beam_ind_r18_present}));

  if (target_nta_r18_present) {
    HANDLE_CODE(target_nta_r18.pack(bref));
  }
  if (beam_ind_r18_present) {
    HANDLE_CODE(beam_ind_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rach_less_ho_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &target_nta_r18_present, &beam_ind_r18_present}));

  if (target_nta_r18_present) {
    HANDLE_CODE(target_nta_r18.unpack(bref));
  }
  if (beam_ind_r18_present) {
    HANDLE_CODE(beam_ind_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void rach_less_ho_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (target_nta_r18_present) {
    j.write_str("targetNTA-r18", target_nta_r18.to_string());
  }
  if (beam_ind_r18_present) {
    j.write_fieldname("beamIndication-r18");
    beam_ind_r18.to_json(j);
  }
  j.end_obj();
}

const char* rach_less_ho_r18_s::target_nta_r18_opts::to_string() const
{
  static const char* names[] = {"zero", "source"};
  return convert_enum_idx(names, 2, value, "rach_less_ho_r18_s::target_nta_r18_e_");
}
uint8_t rach_less_ho_r18_s::target_nta_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {0};
  return map_enum_number(numbers, 1, value, "rach_less_ho_r18_s::target_nta_r18_e_");
}

void rach_less_ho_r18_s::beam_ind_r18_c_::destroy_() {}
void rach_less_ho_r18_s::beam_ind_r18_c_::set(types::options e)
{
  destroy_();
  type_ = e;
}
rach_less_ho_r18_s::beam_ind_r18_c_::beam_ind_r18_c_(const rach_less_ho_r18_s::beam_ind_r18_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::tci_state_id_r18:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ssb_idx_r18:
      c.init(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rach_less_ho_r18_s::beam_ind_r18_c_");
  }
}
rach_less_ho_r18_s::beam_ind_r18_c_&
rach_less_ho_r18_s::beam_ind_r18_c_::operator=(const rach_less_ho_r18_s::beam_ind_r18_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::tci_state_id_r18:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ssb_idx_r18:
      c.set(other.c.get<uint8_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "rach_less_ho_r18_s::beam_ind_r18_c_");
  }

  return *this;
}
uint8_t& rach_less_ho_r18_s::beam_ind_r18_c_::set_tci_state_id_r18()
{
  set(types::tci_state_id_r18);
  return c.get<uint8_t>();
}
uint8_t& rach_less_ho_r18_s::beam_ind_r18_c_::set_ssb_idx_r18()
{
  set(types::ssb_idx_r18);
  return c.get<uint8_t>();
}
void rach_less_ho_r18_s::beam_ind_r18_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::tci_state_id_r18:
      j.write_int("tci-StateID-r18", c.get<uint8_t>());
      break;
    case types::ssb_idx_r18:
      j.write_int("ssb-Index-r18", c.get<uint8_t>());
      break;
    default:
      log_invalid_choice_id(type_, "rach_less_ho_r18_s::beam_ind_r18_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE rach_less_ho_r18_s::beam_ind_r18_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::tci_state_id_r18:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)127u));
      break;
    case types::ssb_idx_r18:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)63u));
      break;
    default:
      log_invalid_choice_id(type_, "rach_less_ho_r18_s::beam_ind_r18_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rach_less_ho_r18_s::beam_ind_r18_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::tci_state_id_r18:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)127u));
      break;
    case types::ssb_idx_r18:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)63u));
      break;
    default:
      log_invalid_choice_id(type_, "rach_less_ho_r18_s::beam_ind_r18_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* rach_less_ho_r18_s::beam_ind_r18_c_::types_opts::to_string() const
{
  static const char* names[] = {"tci-StateID-r18", "ssb-Index-r18"};
  return convert_enum_idx(names, 2, value, "rach_less_ho_r18_s::beam_ind_r18_c_::types");
}

// ReconfigurationWithSync ::= SEQUENCE
OCUDUASN_CODE recfg_with_sync_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(4);
  group_flags[0] |= smtc.is_present();
  group_flags[1] |= daps_ul_pwr_cfg_r16.is_present();
  group_flags[2] |= sl_path_switch_cfg_r17.is_present();
  group_flags[3] |= rach_less_ho_r18.is_present();
  group_flags[3] |= sl_indirect_path_maintain_r18_present;
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), sp_cell_cfg_common_present, rach_cfg_ded_present}));

  if (sp_cell_cfg_common_present) {
    HANDLE_CODE(sp_cell_cfg_common.pack(bref));
  }
  HANDLE_CODE(pack_integer(bref, new_ue_id, (uint32_t)0u, (uint32_t)65535u));
  HANDLE_CODE(t304.pack(bref));
  if (rach_cfg_ded_present) {
    HANDLE_CODE(rach_cfg_ded.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(smtc.is_present(), 1));
      if (smtc.is_present()) {
        HANDLE_CODE(smtc->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(daps_ul_pwr_cfg_r16.is_present(), 1));
      if (daps_ul_pwr_cfg_r16.is_present()) {
        HANDLE_CODE(daps_ul_pwr_cfg_r16->pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(sl_path_switch_cfg_r17.is_present(), 1));
      if (sl_path_switch_cfg_r17.is_present()) {
        HANDLE_CODE(sl_path_switch_cfg_r17->pack(bref));
      }
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {rach_less_ho_r18.is_present(), sl_indirect_path_maintain_r18_present}));
      if (rach_less_ho_r18.is_present()) {
        HANDLE_CODE(rach_less_ho_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE recfg_with_sync_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &sp_cell_cfg_common_present, &rach_cfg_ded_present}));

  if (sp_cell_cfg_common_present) {
    HANDLE_CODE(sp_cell_cfg_common.unpack(bref));
  }
  HANDLE_CODE(unpack_integer(new_ue_id, bref, (uint32_t)0u, (uint32_t)65535u));
  HANDLE_CODE(t304.unpack(bref));
  if (rach_cfg_ded_present) {
    HANDLE_CODE(rach_cfg_ded.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(smtc, bref);
      if (smtc.is_present()) {
        HANDLE_CODE(smtc->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(daps_ul_pwr_cfg_r16, bref);
      if (daps_ul_pwr_cfg_r16.is_present()) {
        HANDLE_CODE(daps_ul_pwr_cfg_r16->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(sl_path_switch_cfg_r17, bref);
      if (sl_path_switch_cfg_r17.is_present()) {
        HANDLE_CODE(sl_path_switch_cfg_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(rach_less_ho_r18, bref);
      HANDLE_CODE(bref.unpack(sl_indirect_path_maintain_r18_present, 1));
      if (rach_less_ho_r18.is_present()) {
        HANDLE_CODE(rach_less_ho_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void recfg_with_sync_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (sp_cell_cfg_common_present) {
    j.write_fieldname("spCellConfigCommon");
    sp_cell_cfg_common.to_json(j);
  }
  j.write_int("newUE-Identity", new_ue_id);
  j.write_str("t304", t304.to_string());
  if (rach_cfg_ded_present) {
    j.write_fieldname("rach-ConfigDedicated");
    rach_cfg_ded.to_json(j);
  }
  if (smtc.is_present()) {
    j.write_fieldname("smtc");
    smtc->to_json(j);
  }
  if (daps_ul_pwr_cfg_r16.is_present()) {
    j.write_fieldname("daps-UplinkPowerConfig-r16");
    daps_ul_pwr_cfg_r16->to_json(j);
  }
  if (sl_path_switch_cfg_r17.is_present()) {
    j.write_fieldname("sl-PathSwitchConfig-r17");
    sl_path_switch_cfg_r17->to_json(j);
  }
  if (rach_less_ho_r18.is_present()) {
    j.write_fieldname("rach-LessHO-r18");
    rach_less_ho_r18->to_json(j);
  }
  if (sl_indirect_path_maintain_r18_present) {
    j.write_str("sl-IndirectPathMaintain-r18", "true");
  }
  j.end_obj();
}

const char* recfg_with_sync_s::t304_opts::to_string() const
{
  static const char* names[] = {"ms50", "ms100", "ms150", "ms200", "ms500", "ms1000", "ms2000", "ms10000"};
  return convert_enum_idx(names, 8, value, "recfg_with_sync_s::t304_e_");
}
uint16_t recfg_with_sync_s::t304_opts::to_number() const
{
  static const uint16_t numbers[] = {50, 100, 150, 200, 500, 1000, 2000, 10000};
  return map_enum_number(numbers, 8, value, "recfg_with_sync_s::t304_e_");
}

void recfg_with_sync_s::rach_cfg_ded_c_::destroy_()
{
  switch (type_) {
    case types::ul:
      c.destroy<rach_cfg_ded_s>();
      break;
    case types::supplementary_ul:
      c.destroy<rach_cfg_ded_s>();
      break;
    default:
      break;
  }
}
void recfg_with_sync_s::rach_cfg_ded_c_::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::ul:
      c.init<rach_cfg_ded_s>();
      break;
    case types::supplementary_ul:
      c.init<rach_cfg_ded_s>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "recfg_with_sync_s::rach_cfg_ded_c_");
  }
}
recfg_with_sync_s::rach_cfg_ded_c_::rach_cfg_ded_c_(const recfg_with_sync_s::rach_cfg_ded_c_& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ul:
      c.init(other.c.get<rach_cfg_ded_s>());
      break;
    case types::supplementary_ul:
      c.init(other.c.get<rach_cfg_ded_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "recfg_with_sync_s::rach_cfg_ded_c_");
  }
}
recfg_with_sync_s::rach_cfg_ded_c_&
recfg_with_sync_s::rach_cfg_ded_c_::operator=(const recfg_with_sync_s::rach_cfg_ded_c_& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ul:
      c.set(other.c.get<rach_cfg_ded_s>());
      break;
    case types::supplementary_ul:
      c.set(other.c.get<rach_cfg_ded_s>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "recfg_with_sync_s::rach_cfg_ded_c_");
  }

  return *this;
}
rach_cfg_ded_s& recfg_with_sync_s::rach_cfg_ded_c_::set_ul()
{
  set(types::ul);
  return c.get<rach_cfg_ded_s>();
}
rach_cfg_ded_s& recfg_with_sync_s::rach_cfg_ded_c_::set_supplementary_ul()
{
  set(types::supplementary_ul);
  return c.get<rach_cfg_ded_s>();
}
void recfg_with_sync_s::rach_cfg_ded_c_::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ul:
      j.write_fieldname("uplink");
      c.get<rach_cfg_ded_s>().to_json(j);
      break;
    case types::supplementary_ul:
      j.write_fieldname("supplementaryUplink");
      c.get<rach_cfg_ded_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "recfg_with_sync_s::rach_cfg_ded_c_");
  }
  j.end_obj();
}
OCUDUASN_CODE recfg_with_sync_s::rach_cfg_ded_c_::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ul:
      HANDLE_CODE(c.get<rach_cfg_ded_s>().pack(bref));
      break;
    case types::supplementary_ul:
      HANDLE_CODE(c.get<rach_cfg_ded_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "recfg_with_sync_s::rach_cfg_ded_c_");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE recfg_with_sync_s::rach_cfg_ded_c_::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ul:
      HANDLE_CODE(c.get<rach_cfg_ded_s>().unpack(bref));
      break;
    case types::supplementary_ul:
      HANDLE_CODE(c.get<rach_cfg_ded_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "recfg_with_sync_s::rach_cfg_ded_c_");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* recfg_with_sync_s::rach_cfg_ded_c_::types_opts::to_string() const
{
  static const char* names[] = {"uplink", "supplementaryUplink"};
  return convert_enum_idx(names, 2, value, "recfg_with_sync_s::rach_cfg_ded_c_::types");
}

// RLF-TimersAndConstants ::= SEQUENCE
OCUDUASN_CODE rlf_timers_and_consts_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= t311.is_present();
  HANDLE_CODE(bref.pack(group_flags.any(), 1));

  HANDLE_CODE(t310.pack(bref));
  HANDLE_CODE(n310.pack(bref));
  HANDLE_CODE(n311.pack(bref));

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      if (not t311.is_present()) {
        asn1::log_error("Mandatory field t311 is missing");
        return OCUDUASN_ERROR_ENCODE_FAIL;
      }
      HANDLE_CODE(t311->pack(bref));
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rlf_timers_and_consts_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(t310.unpack(bref));
  HANDLE_CODE(n310.unpack(bref));
  HANDLE_CODE(n311.unpack(bref));

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      t311.set_present();
      HANDLE_CODE(t311->unpack(bref));
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void rlf_timers_and_consts_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("t310", t310.to_string());
  j.write_str("n310", n310.to_string());
  j.write_str("n311", n311.to_string());
  if (t311.is_present()) {
    j.write_str("t311", t311->to_string());
  }
  j.end_obj();
}

const char* rlf_timers_and_consts_s::t310_opts::to_string() const
{
  static const char* names[] = {"ms0", "ms50", "ms100", "ms200", "ms500", "ms1000", "ms2000", "ms4000", "ms6000"};
  return convert_enum_idx(names, 9, value, "rlf_timers_and_consts_s::t310_e_");
}
uint16_t rlf_timers_and_consts_s::t310_opts::to_number() const
{
  static const uint16_t numbers[] = {0, 50, 100, 200, 500, 1000, 2000, 4000, 6000};
  return map_enum_number(numbers, 9, value, "rlf_timers_and_consts_s::t310_e_");
}

const char* rlf_timers_and_consts_s::n310_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n3", "n4", "n6", "n8", "n10", "n20"};
  return convert_enum_idx(names, 8, value, "rlf_timers_and_consts_s::n310_e_");
}
uint8_t rlf_timers_and_consts_s::n310_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3, 4, 6, 8, 10, 20};
  return map_enum_number(numbers, 8, value, "rlf_timers_and_consts_s::n310_e_");
}

const char* rlf_timers_and_consts_s::n311_opts::to_string() const
{
  static const char* names[] = {"n1", "n2", "n3", "n4", "n5", "n6", "n8", "n10"};
  return convert_enum_idx(names, 8, value, "rlf_timers_and_consts_s::n311_e_");
}
uint8_t rlf_timers_and_consts_s::n311_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2, 3, 4, 5, 6, 8, 10};
  return map_enum_number(numbers, 8, value, "rlf_timers_and_consts_s::n311_e_");
}

const char* rlf_timers_and_consts_s::t311_opts::to_string() const
{
  static const char* names[] = {"ms1000", "ms3000", "ms5000", "ms10000", "ms15000", "ms20000", "ms30000"};
  return convert_enum_idx(names, 7, value, "rlf_timers_and_consts_s::t311_e_");
}
uint16_t rlf_timers_and_consts_s::t311_opts::to_number() const
{
  static const uint16_t numbers[] = {1000, 3000, 5000, 10000, 15000, 20000, 30000};
  return map_enum_number(numbers, 7, value, "rlf_timers_and_consts_s::t311_e_");
}

// GoodServingCellEvaluation-r17 ::= SEQUENCE
OCUDUASN_CODE good_serving_cell_eval_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(offset_r17_present, 1));

  if (offset_r17_present) {
    HANDLE_CODE(offset_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE good_serving_cell_eval_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(bref.unpack(offset_r17_present, 1));

  if (offset_r17_present) {
    HANDLE_CODE(offset_r17.unpack(bref));
  }

  return OCUDUASN_SUCCESS;
}
void good_serving_cell_eval_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (offset_r17_present) {
    j.write_str("offset-r17", offset_r17.to_string());
  }
  j.end_obj();
}

const char* good_serving_cell_eval_r17_s::offset_r17_opts::to_string() const
{
  static const char* names[] = {"db2", "db4", "db6", "db8"};
  return convert_enum_idx(names, 4, value, "good_serving_cell_eval_r17_s::offset_r17_e_");
}
uint8_t good_serving_cell_eval_r17_s::offset_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {2, 4, 6, 8};
  return map_enum_number(numbers, 4, value, "good_serving_cell_eval_r17_s::offset_r17_e_");
}

// DeactivatedSCG-Config-r17 ::= SEQUENCE
OCUDUASN_CODE deactiv_scg_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(bref.pack(bfd_and_rlm_r17, 1));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE deactiv_scg_cfg_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(bref.unpack(bfd_and_rlm_r17, 1));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void deactiv_scg_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_bool("bfd-and-RLM-r17", bfd_and_rlm_r17);
  j.end_obj();
}

// SpCellConfig ::= SEQUENCE
OCUDUASN_CODE sp_cell_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(1);
  group_flags[0] |= low_mob_eval_connected_r17.is_present();
  group_flags[0] |= good_serving_cell_eval_rlm_r17.is_present();
  group_flags[0] |= good_serving_cell_eval_bfd_r17.is_present();
  group_flags[0] |= deactiv_scg_cfg_r17.is_present();
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         serv_cell_idx_present,
                         recfg_with_sync_present,
                         rlf_timers_and_consts_present,
                         rlm_in_sync_out_of_sync_thres_present,
                         sp_cell_cfg_ded_present}));

  if (serv_cell_idx_present) {
    HANDLE_CODE(pack_integer(bref, serv_cell_idx, (uint8_t)0u, (uint8_t)31u));
  }
  if (recfg_with_sync_present) {
    HANDLE_CODE(recfg_with_sync.pack(bref));
  }
  if (rlf_timers_and_consts_present) {
    HANDLE_CODE(rlf_timers_and_consts.pack(bref));
  }
  if (sp_cell_cfg_ded_present) {
    HANDLE_CODE(sp_cell_cfg_ded.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {low_mob_eval_connected_r17.is_present(),
                             good_serving_cell_eval_rlm_r17.is_present(),
                             good_serving_cell_eval_bfd_r17.is_present(),
                             deactiv_scg_cfg_r17.is_present()}));
      if (low_mob_eval_connected_r17.is_present()) {
        HANDLE_CODE(low_mob_eval_connected_r17->s_search_delta_p_connected_r17.pack(bref));
        HANDLE_CODE(low_mob_eval_connected_r17->t_search_delta_p_connected_r17.pack(bref));
      }
      if (good_serving_cell_eval_rlm_r17.is_present()) {
        HANDLE_CODE(good_serving_cell_eval_rlm_r17->pack(bref));
      }
      if (good_serving_cell_eval_bfd_r17.is_present()) {
        HANDLE_CODE(good_serving_cell_eval_bfd_r17->pack(bref));
      }
      if (deactiv_scg_cfg_r17.is_present()) {
        HANDLE_CODE(deactiv_scg_cfg_r17->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sp_cell_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &serv_cell_idx_present,
                           &recfg_with_sync_present,
                           &rlf_timers_and_consts_present,
                           &rlm_in_sync_out_of_sync_thres_present,
                           &sp_cell_cfg_ded_present}));

  if (serv_cell_idx_present) {
    HANDLE_CODE(unpack_integer(serv_cell_idx, bref, (uint8_t)0u, (uint8_t)31u));
  }
  if (recfg_with_sync_present) {
    HANDLE_CODE(recfg_with_sync.unpack(bref));
  }
  if (rlf_timers_and_consts_present) {
    HANDLE_CODE(rlf_timers_and_consts.unpack(bref));
  }
  if (sp_cell_cfg_ded_present) {
    HANDLE_CODE(sp_cell_cfg_ded.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(low_mob_eval_connected_r17, bref);
      unpack_presence_flag(good_serving_cell_eval_rlm_r17, bref);
      unpack_presence_flag(good_serving_cell_eval_bfd_r17, bref);
      unpack_presence_flag(deactiv_scg_cfg_r17, bref);
      if (low_mob_eval_connected_r17.is_present()) {
        HANDLE_CODE(low_mob_eval_connected_r17->s_search_delta_p_connected_r17.unpack(bref));
        HANDLE_CODE(low_mob_eval_connected_r17->t_search_delta_p_connected_r17.unpack(bref));
      }
      if (good_serving_cell_eval_rlm_r17.is_present()) {
        HANDLE_CODE(good_serving_cell_eval_rlm_r17->unpack(bref));
      }
      if (good_serving_cell_eval_bfd_r17.is_present()) {
        HANDLE_CODE(good_serving_cell_eval_bfd_r17->unpack(bref));
      }
      if (deactiv_scg_cfg_r17.is_present()) {
        HANDLE_CODE(deactiv_scg_cfg_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void sp_cell_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (serv_cell_idx_present) {
    j.write_int("servCellIndex", serv_cell_idx);
  }
  if (recfg_with_sync_present) {
    j.write_fieldname("reconfigurationWithSync");
    recfg_with_sync.to_json(j);
  }
  if (rlf_timers_and_consts_present) {
    j.write_fieldname("rlf-TimersAndConstants");
    rlf_timers_and_consts.to_json(j);
  }
  if (rlm_in_sync_out_of_sync_thres_present) {
    j.write_str("rlmInSyncOutOfSyncThreshold", "n1");
  }
  if (sp_cell_cfg_ded_present) {
    j.write_fieldname("spCellConfigDedicated");
    sp_cell_cfg_ded.to_json(j);
  }
  if (low_mob_eval_connected_r17.is_present()) {
    j.write_fieldname("lowMobilityEvaluationConnected-r17");
    j.start_obj();
    j.write_str("s-SearchDeltaP-Connected-r17", low_mob_eval_connected_r17->s_search_delta_p_connected_r17.to_string());
    j.write_str("t-SearchDeltaP-Connected-r17", low_mob_eval_connected_r17->t_search_delta_p_connected_r17.to_string());
    j.end_obj();
  }
  if (good_serving_cell_eval_rlm_r17.is_present()) {
    j.write_fieldname("goodServingCellEvaluationRLM-r17");
    good_serving_cell_eval_rlm_r17->to_json(j);
  }
  if (good_serving_cell_eval_bfd_r17.is_present()) {
    j.write_fieldname("goodServingCellEvaluationBFD-r17");
    good_serving_cell_eval_bfd_r17->to_json(j);
  }
  if (deactiv_scg_cfg_r17.is_present()) {
    j.write_fieldname("deactivatedSCG-Config-r17");
    deactiv_scg_cfg_r17->to_json(j);
  }
  j.end_obj();
}

const char* sp_cell_cfg_s::low_mob_eval_connected_r17_s_::s_search_delta_p_connected_r17_opts::to_string() const
{
  static const char* names[] = {"dB3", "dB6", "dB9", "dB12", "dB15", "spare3", "spare2", "spare1"};
  return convert_enum_idx(
      names, 8, value, "sp_cell_cfg_s::low_mob_eval_connected_r17_s_::s_search_delta_p_connected_r17_e_");
}
uint8_t sp_cell_cfg_s::low_mob_eval_connected_r17_s_::s_search_delta_p_connected_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {3, 6, 9, 12, 15};
  return map_enum_number(
      numbers, 5, value, "sp_cell_cfg_s::low_mob_eval_connected_r17_s_::s_search_delta_p_connected_r17_e_");
}

const char* sp_cell_cfg_s::low_mob_eval_connected_r17_s_::t_search_delta_p_connected_r17_opts::to_string() const
{
  static const char* names[] = {"s5",
                                "s10",
                                "s20",
                                "s30",
                                "s60",
                                "s120",
                                "s180",
                                "s240",
                                "s300",
                                "spare7",
                                "spare6",
                                "spare5",
                                "spare4",
                                "spare3",
                                "spare2",
                                "spare1"};
  return convert_enum_idx(
      names, 16, value, "sp_cell_cfg_s::low_mob_eval_connected_r17_s_::t_search_delta_p_connected_r17_e_");
}
uint16_t sp_cell_cfg_s::low_mob_eval_connected_r17_s_::t_search_delta_p_connected_r17_opts::to_number() const
{
  static const uint16_t numbers[] = {5, 10, 20, 30, 60, 120, 180, 240, 300};
  return map_enum_number(
      numbers, 9, value, "sp_cell_cfg_s::low_mob_eval_connected_r17_s_::t_search_delta_p_connected_r17_e_");
}

// SCellConfig ::= SEQUENCE
OCUDUASN_CODE scell_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(4);
  group_flags[0] |= smtc.is_present();
  group_flags[1] |= scell_state_r16_present;
  group_flags[1] |= secondary_drx_group_cfg_r16_present;
  group_flags[2] |= pre_conf_gap_status_r17_present;
  group_flags[2] |= good_serving_cell_eval_bfd_r17.is_present();
  group_flags[2] |= scell_sib20_r17.is_present();
  group_flags[3] |= plmn_id_info_list_r17.is_present();
  group_flags[3] |= npn_id_info_list_r17.is_present();
  HANDLE_CODE(pack_bits(bref, {group_flags.any(), scell_cfg_common_present, scell_cfg_ded_present}));

  HANDLE_CODE(pack_integer(bref, scell_idx, (uint8_t)1u, (uint8_t)31u));
  if (scell_cfg_common_present) {
    HANDLE_CODE(scell_cfg_common.pack(bref));
  }
  if (scell_cfg_ded_present) {
    HANDLE_CODE(scell_cfg_ded.pack(bref));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(smtc.is_present(), 1));
      if (smtc.is_present()) {
        HANDLE_CODE(smtc->pack(bref));
      }
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {scell_state_r16_present, secondary_drx_group_cfg_r16_present}));
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {pre_conf_gap_status_r17_present,
                             good_serving_cell_eval_bfd_r17.is_present(),
                             scell_sib20_r17.is_present()}));
      if (pre_conf_gap_status_r17_present) {
        HANDLE_CODE(pre_conf_gap_status_r17.pack(bref));
      }
      if (good_serving_cell_eval_bfd_r17.is_present()) {
        HANDLE_CODE(good_serving_cell_eval_bfd_r17->pack(bref));
      }
      if (scell_sib20_r17.is_present()) {
        HANDLE_CODE(scell_sib20_r17->pack(bref));
      }
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref, {plmn_id_info_list_r17.is_present(), npn_id_info_list_r17.is_present()}));
      if (plmn_id_info_list_r17.is_present()) {
        HANDLE_CODE(plmn_id_info_list_r17->pack(bref));
      }
      if (npn_id_info_list_r17.is_present()) {
        HANDLE_CODE(npn_id_info_list_r17->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE scell_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref, {&ext, &scell_cfg_common_present, &scell_cfg_ded_present}));

  HANDLE_CODE(unpack_integer(scell_idx, bref, (uint8_t)1u, (uint8_t)31u));
  if (scell_cfg_common_present) {
    HANDLE_CODE(scell_cfg_common.unpack(bref));
  }
  if (scell_cfg_ded_present) {
    HANDLE_CODE(scell_cfg_ded.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(smtc, bref);
      if (smtc.is_present()) {
        HANDLE_CODE(smtc->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref, {&scell_state_r16_present, &secondary_drx_group_cfg_r16_present}));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(pre_conf_gap_status_r17_present, 1));
      unpack_presence_flag(good_serving_cell_eval_bfd_r17, bref);
      unpack_presence_flag(scell_sib20_r17, bref);
      if (pre_conf_gap_status_r17_present) {
        HANDLE_CODE(pre_conf_gap_status_r17.unpack(bref));
      }
      if (good_serving_cell_eval_bfd_r17.is_present()) {
        HANDLE_CODE(good_serving_cell_eval_bfd_r17->unpack(bref));
      }
      if (scell_sib20_r17.is_present()) {
        HANDLE_CODE(scell_sib20_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(plmn_id_info_list_r17, bref);
      unpack_presence_flag(npn_id_info_list_r17, bref);
      if (plmn_id_info_list_r17.is_present()) {
        HANDLE_CODE(plmn_id_info_list_r17->unpack(bref));
      }
      if (npn_id_info_list_r17.is_present()) {
        HANDLE_CODE(npn_id_info_list_r17->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void scell_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sCellIndex", scell_idx);
  if (scell_cfg_common_present) {
    j.write_fieldname("sCellConfigCommon");
    scell_cfg_common.to_json(j);
  }
  if (scell_cfg_ded_present) {
    j.write_fieldname("sCellConfigDedicated");
    scell_cfg_ded.to_json(j);
  }
  if (smtc.is_present()) {
    j.write_fieldname("smtc");
    smtc->to_json(j);
  }
  if (scell_state_r16_present) {
    j.write_str("sCellState-r16", "activated");
  }
  if (secondary_drx_group_cfg_r16_present) {
    j.write_str("secondaryDRX-GroupConfig-r16", "true");
  }
  if (pre_conf_gap_status_r17_present) {
    j.write_fieldname("preConfGapStatus-r17");
    pre_conf_gap_status_r17.to_json(j);
  }
  if (good_serving_cell_eval_bfd_r17.is_present()) {
    j.write_fieldname("goodServingCellEvaluationBFD-r17");
    good_serving_cell_eval_bfd_r17->to_json(j);
  }
  if (scell_sib20_r17.is_present()) {
    j.write_fieldname("sCellSIB20-r17");
    scell_sib20_r17->to_json(j);
  }
  if (plmn_id_info_list_r17.is_present()) {
    j.write_fieldname("plmn-IdentityInfoList-r17");
    plmn_id_info_list_r17->to_json(j);
  }
  if (npn_id_info_list_r17.is_present()) {
    j.write_fieldname("npn-IdentityInfoList-r17");
    npn_id_info_list_r17->to_json(j);
  }
  j.end_obj();
}

// Uu-RelayRLC-ChannelConfig-r17 ::= SEQUENCE
OCUDUASN_CODE uu_relay_rlc_ch_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref,
      {false, uu_lc_ch_id_r17_present, reestablish_rlc_r17_present, rlc_cfg_r17_present, mac_lc_ch_cfg_r17_present}));

  if (uu_lc_ch_id_r17_present) {
    HANDLE_CODE(pack_integer(bref, uu_lc_ch_id_r17, (uint8_t)1u, (uint8_t)32u));
  }
  HANDLE_CODE(pack_integer(bref, uu_relay_rlc_ch_id_r17, (uint8_t)1u, (uint8_t)32u));
  if (rlc_cfg_r17_present) {
    HANDLE_CODE(rlc_cfg_r17.pack(bref));
  }
  if (mac_lc_ch_cfg_r17_present) {
    HANDLE_CODE(mac_lc_ch_cfg_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE uu_relay_rlc_ch_cfg_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &uu_lc_ch_id_r17_present,
                           &reestablish_rlc_r17_present,
                           &rlc_cfg_r17_present,
                           &mac_lc_ch_cfg_r17_present}));

  if (uu_lc_ch_id_r17_present) {
    HANDLE_CODE(unpack_integer(uu_lc_ch_id_r17, bref, (uint8_t)1u, (uint8_t)32u));
  }
  HANDLE_CODE(unpack_integer(uu_relay_rlc_ch_id_r17, bref, (uint8_t)1u, (uint8_t)32u));
  if (rlc_cfg_r17_present) {
    HANDLE_CODE(rlc_cfg_r17.unpack(bref));
  }
  if (mac_lc_ch_cfg_r17_present) {
    HANDLE_CODE(mac_lc_ch_cfg_r17.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void uu_relay_rlc_ch_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (uu_lc_ch_id_r17_present) {
    j.write_int("uu-LogicalChannelIdentity-r17", uu_lc_ch_id_r17);
  }
  j.write_int("uu-RelayRLC-ChannelID-r17", uu_relay_rlc_ch_id_r17);
  if (reestablish_rlc_r17_present) {
    j.write_str("reestablishRLC-r17", "true");
  }
  if (rlc_cfg_r17_present) {
    j.write_fieldname("rlc-Config-r17");
    rlc_cfg_r17.to_json(j);
  }
  if (mac_lc_ch_cfg_r17_present) {
    j.write_fieldname("mac-LogicalChannelConfig-r17");
    mac_lc_ch_cfg_r17.to_json(j);
  }
  j.end_obj();
}

// IAB-ResourceConfig-r17 ::= SEQUENCE
OCUDUASN_CODE iab_res_cfg_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(
      bref,
      {false, slot_list_r17.size() > 0, periodicity_slot_list_r17_present, slot_list_subcarrier_spacing_r17_present}));

  HANDLE_CODE(pack_integer(bref, iab_res_cfg_id_r17, (uint32_t)0u, (uint32_t)65535u));
  if (slot_list_r17.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, slot_list_r17, 1, 5120, integer_packer<uint16_t>(0, 5119)));
  }
  if (periodicity_slot_list_r17_present) {
    HANDLE_CODE(periodicity_slot_list_r17.pack(bref));
  }
  if (slot_list_subcarrier_spacing_r17_present) {
    HANDLE_CODE(slot_list_subcarrier_spacing_r17.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_res_cfg_r17_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool slot_list_r17_present;
  HANDLE_CODE(unpack_bits(
      bref,
      {&ext, &slot_list_r17_present, &periodicity_slot_list_r17_present, &slot_list_subcarrier_spacing_r17_present}));

  HANDLE_CODE(unpack_integer(iab_res_cfg_id_r17, bref, (uint32_t)0u, (uint32_t)65535u));
  if (slot_list_r17_present) {
    HANDLE_CODE(unpack_dyn_seq_of(slot_list_r17, bref, 1, 5120, integer_packer<uint16_t>(0, 5119)));
  }
  if (periodicity_slot_list_r17_present) {
    HANDLE_CODE(periodicity_slot_list_r17.unpack(bref));
  }
  if (slot_list_subcarrier_spacing_r17_present) {
    HANDLE_CODE(slot_list_subcarrier_spacing_r17.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void iab_res_cfg_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("iab-ResourceConfigID-r17", iab_res_cfg_id_r17);
  if (slot_list_r17.size() > 0) {
    j.start_array("slotList-r17");
    for (const auto& e1 : slot_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (periodicity_slot_list_r17_present) {
    j.write_str("periodicitySlotList-r17", periodicity_slot_list_r17.to_string());
  }
  if (slot_list_subcarrier_spacing_r17_present) {
    j.write_str("slotListSubcarrierSpacing-r17", slot_list_subcarrier_spacing_r17.to_string());
  }
  j.end_obj();
}

const char* iab_res_cfg_r17_s::periodicity_slot_list_r17_opts::to_string() const
{
  static const char* names[] = {
      "ms0p5", "ms0p625", "ms1", "ms1p25", "ms2", "ms2p5", "ms5", "ms10", "ms20", "ms40", "ms80", "ms160"};
  return convert_enum_idx(names, 12, value, "iab_res_cfg_r17_s::periodicity_slot_list_r17_e_");
}

// IntraBandCC-CombinationReqList-r17 ::= SEQUENCE
OCUDUASN_CODE intra_band_cc_combination_req_list_r17_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_dyn_seq_of(bref, serv_cell_idx_list_r17, 1, 32, integer_packer<uint8_t>(0, 31)));
  HANDLE_CODE(pack_dyn_seq_of(bref, cc_combination_list_r17, 1, 128, SeqOfPacker<Packer>(1, 32, Packer())));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE intra_band_cc_combination_req_list_r17_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_dyn_seq_of(serv_cell_idx_list_r17, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
  HANDLE_CODE(unpack_dyn_seq_of(cc_combination_list_r17, bref, 1, 128, SeqOfPacker<Packer>(1, 32, Packer())));

  return OCUDUASN_SUCCESS;
}
void intra_band_cc_combination_req_list_r17_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("servCellIndexList-r17");
  for (const auto& e1 : serv_cell_idx_list_r17) {
    j.write_int(e1);
  }
  j.end_array();
  j.start_array("cc-CombinationList-r17");
  for (const auto& e1 : cc_combination_list_r17) {
    j.start_array();
    for (const auto& e2 : e1) {
      e2.to_json(j);
    }
    j.end_array();
  }
  j.end_array();
  j.end_obj();
}

// NCR-SlotPeriodicityAndSlotOffset-r18 ::= CHOICE
void ncr_slot_periodicity_and_slot_offset_r18_c::destroy_() {}
void ncr_slot_periodicity_and_slot_offset_r18_c::set(types::options e)
{
  destroy_();
  type_ = e;
}
ncr_slot_periodicity_and_slot_offset_r18_c::ncr_slot_periodicity_and_slot_offset_r18_c(
    const ncr_slot_periodicity_and_slot_offset_r18_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::sl1:
      break;
    case types::sl2:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl4:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl5:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl8:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl10:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl16:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl20:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl32:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl40:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl64:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl80:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl128:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl160:
      c.init(other.c.get<uint8_t>());
      break;
    case types::sl256:
      c.init(other.c.get<uint16_t>());
      break;
    case types::sl320:
      c.init(other.c.get<uint16_t>());
      break;
    case types::sl512:
      c.init(other.c.get<uint16_t>());
      break;
    case types::sl640:
      c.init(other.c.get<uint16_t>());
      break;
    case types::sl1024:
      c.init(other.c.get<uint16_t>());
      break;
    case types::sl1280:
      c.init(other.c.get<uint16_t>());
      break;
    case types::sl2560:
      c.init(other.c.get<uint16_t>());
      break;
    case types::sl5120:
      c.init(other.c.get<uint16_t>());
      break;
    case types::sl10240:
      c.init(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ncr_slot_periodicity_and_slot_offset_r18_c");
  }
}
ncr_slot_periodicity_and_slot_offset_r18_c&
ncr_slot_periodicity_and_slot_offset_r18_c::operator=(const ncr_slot_periodicity_and_slot_offset_r18_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::sl1:
      break;
    case types::sl2:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl4:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl5:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl8:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl10:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl16:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl20:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl32:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl40:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl64:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl80:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl128:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl160:
      c.set(other.c.get<uint8_t>());
      break;
    case types::sl256:
      c.set(other.c.get<uint16_t>());
      break;
    case types::sl320:
      c.set(other.c.get<uint16_t>());
      break;
    case types::sl512:
      c.set(other.c.get<uint16_t>());
      break;
    case types::sl640:
      c.set(other.c.get<uint16_t>());
      break;
    case types::sl1024:
      c.set(other.c.get<uint16_t>());
      break;
    case types::sl1280:
      c.set(other.c.get<uint16_t>());
      break;
    case types::sl2560:
      c.set(other.c.get<uint16_t>());
      break;
    case types::sl5120:
      c.set(other.c.get<uint16_t>());
      break;
    case types::sl10240:
      c.set(other.c.get<uint16_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ncr_slot_periodicity_and_slot_offset_r18_c");
  }

  return *this;
}
void ncr_slot_periodicity_and_slot_offset_r18_c::set_sl1()
{
  set(types::sl1);
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl2()
{
  set(types::sl2);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl4()
{
  set(types::sl4);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl5()
{
  set(types::sl5);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl8()
{
  set(types::sl8);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl10()
{
  set(types::sl10);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl16()
{
  set(types::sl16);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl20()
{
  set(types::sl20);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl32()
{
  set(types::sl32);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl40()
{
  set(types::sl40);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl64()
{
  set(types::sl64);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl80()
{
  set(types::sl80);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl128()
{
  set(types::sl128);
  return c.get<uint8_t>();
}
uint8_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl160()
{
  set(types::sl160);
  return c.get<uint8_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl256()
{
  set(types::sl256);
  return c.get<uint16_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl320()
{
  set(types::sl320);
  return c.get<uint16_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl512()
{
  set(types::sl512);
  return c.get<uint16_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl640()
{
  set(types::sl640);
  return c.get<uint16_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl1024()
{
  set(types::sl1024);
  return c.get<uint16_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl1280()
{
  set(types::sl1280);
  return c.get<uint16_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl2560()
{
  set(types::sl2560);
  return c.get<uint16_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl5120()
{
  set(types::sl5120);
  return c.get<uint16_t>();
}
uint16_t& ncr_slot_periodicity_and_slot_offset_r18_c::set_sl10240()
{
  set(types::sl10240);
  return c.get<uint16_t>();
}
void ncr_slot_periodicity_and_slot_offset_r18_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::sl1:
      j.write_null("sl1");
      break;
    case types::sl2:
      j.write_int("sl2", c.get<uint8_t>());
      break;
    case types::sl4:
      j.write_int("sl4", c.get<uint8_t>());
      break;
    case types::sl5:
      j.write_int("sl5", c.get<uint8_t>());
      break;
    case types::sl8:
      j.write_int("sl8", c.get<uint8_t>());
      break;
    case types::sl10:
      j.write_int("sl10", c.get<uint8_t>());
      break;
    case types::sl16:
      j.write_int("sl16", c.get<uint8_t>());
      break;
    case types::sl20:
      j.write_int("sl20", c.get<uint8_t>());
      break;
    case types::sl32:
      j.write_int("sl32", c.get<uint8_t>());
      break;
    case types::sl40:
      j.write_int("sl40", c.get<uint8_t>());
      break;
    case types::sl64:
      j.write_int("sl64", c.get<uint8_t>());
      break;
    case types::sl80:
      j.write_int("sl80", c.get<uint8_t>());
      break;
    case types::sl128:
      j.write_int("sl128", c.get<uint8_t>());
      break;
    case types::sl160:
      j.write_int("sl160", c.get<uint8_t>());
      break;
    case types::sl256:
      j.write_int("sl256", c.get<uint16_t>());
      break;
    case types::sl320:
      j.write_int("sl320", c.get<uint16_t>());
      break;
    case types::sl512:
      j.write_int("sl512", c.get<uint16_t>());
      break;
    case types::sl640:
      j.write_int("sl640", c.get<uint16_t>());
      break;
    case types::sl1024:
      j.write_int("sl1024", c.get<uint16_t>());
      break;
    case types::sl1280:
      j.write_int("sl1280", c.get<uint16_t>());
      break;
    case types::sl2560:
      j.write_int("sl2560", c.get<uint16_t>());
      break;
    case types::sl5120:
      j.write_int("sl5120", c.get<uint16_t>());
      break;
    case types::sl10240:
      j.write_int("sl10240", c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "ncr_slot_periodicity_and_slot_offset_r18_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ncr_slot_periodicity_and_slot_offset_r18_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::sl1:
      break;
    case types::sl2:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)1u));
      break;
    case types::sl4:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)3u));
      break;
    case types::sl5:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)4u));
      break;
    case types::sl8:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)7u));
      break;
    case types::sl10:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)9u));
      break;
    case types::sl16:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)15u));
      break;
    case types::sl20:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)19u));
      break;
    case types::sl32:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)31u));
      break;
    case types::sl40:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)39u));
      break;
    case types::sl64:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)63u));
      break;
    case types::sl80:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)79u));
      break;
    case types::sl128:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)127u));
      break;
    case types::sl160:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)159u));
      break;
    case types::sl256:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u));
      break;
    case types::sl320:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)319u));
      break;
    case types::sl512:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)511u));
      break;
    case types::sl640:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)639u));
      break;
    case types::sl1024:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::sl1280:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1279u));
      break;
    case types::sl2560:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)2559u));
      break;
    case types::sl5120:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)5119u));
      break;
    case types::sl10240:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)10239u));
      break;
    default:
      log_invalid_choice_id(type_, "ncr_slot_periodicity_and_slot_offset_r18_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_slot_periodicity_and_slot_offset_r18_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::sl1:
      break;
    case types::sl2:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)1u));
      break;
    case types::sl4:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)3u));
      break;
    case types::sl5:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)4u));
      break;
    case types::sl8:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)7u));
      break;
    case types::sl10:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)9u));
      break;
    case types::sl16:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)15u));
      break;
    case types::sl20:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)19u));
      break;
    case types::sl32:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)31u));
      break;
    case types::sl40:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)39u));
      break;
    case types::sl64:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)63u));
      break;
    case types::sl80:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)79u));
      break;
    case types::sl128:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)127u));
      break;
    case types::sl160:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)159u));
      break;
    case types::sl256:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u));
      break;
    case types::sl320:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)319u));
      break;
    case types::sl512:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)511u));
      break;
    case types::sl640:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)639u));
      break;
    case types::sl1024:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::sl1280:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1279u));
      break;
    case types::sl2560:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)2559u));
      break;
    case types::sl5120:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)5119u));
      break;
    case types::sl10240:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)10239u));
      break;
    default:
      log_invalid_choice_id(type_, "ncr_slot_periodicity_and_slot_offset_r18_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ncr_slot_periodicity_and_slot_offset_r18_c::types_opts::to_string() const
{
  static const char* names[] = {"sl1",   "sl2",   "sl4",    "sl5",    "sl8",    "sl10",   "sl16",   "sl20",
                                "sl32",  "sl40",  "sl64",   "sl80",   "sl128",  "sl160",  "sl256",  "sl320",
                                "sl512", "sl640", "sl1024", "sl1280", "sl2560", "sl5120", "sl10240"};
  return convert_enum_idx(names, 23, value, "ncr_slot_periodicity_and_slot_offset_r18_c::types");
}
uint16_t ncr_slot_periodicity_and_slot_offset_r18_c::types_opts::to_number() const
{
  static const uint16_t numbers[] = {1,   2,   4,   5,   8,   10,  16,   20,   32,   40,   64,   80,
                                     128, 160, 256, 320, 512, 640, 1024, 1280, 2560, 5120, 10240};
  return map_enum_number(numbers, 23, value, "ncr_slot_periodicity_and_slot_offset_r18_c::types");
}

// NCR-MsPeriodicityAndSlotOffset-r18 ::= CHOICE
void ncr_ms_periodicity_and_slot_offset_r18_c::destroy_() {}
void ncr_ms_periodicity_and_slot_offset_r18_c::set(types::options e)
{
  destroy_();
  type_ = e;
}
ncr_ms_periodicity_and_slot_offset_r18_c::ncr_ms_periodicity_and_slot_offset_r18_c(
    const ncr_ms_periodicity_and_slot_offset_r18_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::ms1:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms2:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms4:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms5:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms8:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms10:
      c.init(other.c.get<uint8_t>());
      break;
    case types::ms16:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms20:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms32:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms40:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms64:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms80:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms128:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms160:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms256:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms320:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms512:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms640:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms1024:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms1280:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms2560:
      c.init(other.c.get<uint16_t>());
      break;
    case types::ms5120:
      c.init(other.c.get<uint32_t>());
      break;
    case types::ms10240:
      c.init(other.c.get<uint32_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ncr_ms_periodicity_and_slot_offset_r18_c");
  }
}
ncr_ms_periodicity_and_slot_offset_r18_c&
ncr_ms_periodicity_and_slot_offset_r18_c::operator=(const ncr_ms_periodicity_and_slot_offset_r18_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::ms1:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms2:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms4:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms5:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms8:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms10:
      c.set(other.c.get<uint8_t>());
      break;
    case types::ms16:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms20:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms32:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms40:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms64:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms80:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms128:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms160:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms256:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms320:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms512:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms640:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms1024:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms1280:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms2560:
      c.set(other.c.get<uint16_t>());
      break;
    case types::ms5120:
      c.set(other.c.get<uint32_t>());
      break;
    case types::ms10240:
      c.set(other.c.get<uint32_t>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ncr_ms_periodicity_and_slot_offset_r18_c");
  }

  return *this;
}
uint8_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms1()
{
  set(types::ms1);
  return c.get<uint8_t>();
}
uint8_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms2()
{
  set(types::ms2);
  return c.get<uint8_t>();
}
uint8_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms4()
{
  set(types::ms4);
  return c.get<uint8_t>();
}
uint8_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms5()
{
  set(types::ms5);
  return c.get<uint8_t>();
}
uint8_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms8()
{
  set(types::ms8);
  return c.get<uint8_t>();
}
uint8_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms10()
{
  set(types::ms10);
  return c.get<uint8_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms16()
{
  set(types::ms16);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms20()
{
  set(types::ms20);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms32()
{
  set(types::ms32);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms40()
{
  set(types::ms40);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms64()
{
  set(types::ms64);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms80()
{
  set(types::ms80);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms128()
{
  set(types::ms128);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms160()
{
  set(types::ms160);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms256()
{
  set(types::ms256);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms320()
{
  set(types::ms320);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms512()
{
  set(types::ms512);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms640()
{
  set(types::ms640);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms1024()
{
  set(types::ms1024);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms1280()
{
  set(types::ms1280);
  return c.get<uint16_t>();
}
uint16_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms2560()
{
  set(types::ms2560);
  return c.get<uint16_t>();
}
uint32_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms5120()
{
  set(types::ms5120);
  return c.get<uint32_t>();
}
uint32_t& ncr_ms_periodicity_and_slot_offset_r18_c::set_ms10240()
{
  set(types::ms10240);
  return c.get<uint32_t>();
}
void ncr_ms_periodicity_and_slot_offset_r18_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::ms1:
      j.write_int("ms1", c.get<uint8_t>());
      break;
    case types::ms2:
      j.write_int("ms2", c.get<uint8_t>());
      break;
    case types::ms4:
      j.write_int("ms4", c.get<uint8_t>());
      break;
    case types::ms5:
      j.write_int("ms5", c.get<uint8_t>());
      break;
    case types::ms8:
      j.write_int("ms8", c.get<uint8_t>());
      break;
    case types::ms10:
      j.write_int("ms10", c.get<uint8_t>());
      break;
    case types::ms16:
      j.write_int("ms16", c.get<uint16_t>());
      break;
    case types::ms20:
      j.write_int("ms20", c.get<uint16_t>());
      break;
    case types::ms32:
      j.write_int("ms32", c.get<uint16_t>());
      break;
    case types::ms40:
      j.write_int("ms40", c.get<uint16_t>());
      break;
    case types::ms64:
      j.write_int("ms64", c.get<uint16_t>());
      break;
    case types::ms80:
      j.write_int("ms80", c.get<uint16_t>());
      break;
    case types::ms128:
      j.write_int("ms128", c.get<uint16_t>());
      break;
    case types::ms160:
      j.write_int("ms160", c.get<uint16_t>());
      break;
    case types::ms256:
      j.write_int("ms256", c.get<uint16_t>());
      break;
    case types::ms320:
      j.write_int("ms320", c.get<uint16_t>());
      break;
    case types::ms512:
      j.write_int("ms512", c.get<uint16_t>());
      break;
    case types::ms640:
      j.write_int("ms640", c.get<uint16_t>());
      break;
    case types::ms1024:
      j.write_int("ms1024", c.get<uint16_t>());
      break;
    case types::ms1280:
      j.write_int("ms1280", c.get<uint16_t>());
      break;
    case types::ms2560:
      j.write_int("ms2560", c.get<uint16_t>());
      break;
    case types::ms5120:
      j.write_int("ms5120", c.get<uint32_t>());
      break;
    case types::ms10240:
      j.write_int("ms10240", c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "ncr_ms_periodicity_and_slot_offset_r18_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ncr_ms_periodicity_and_slot_offset_r18_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::ms1:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)15u));
      break;
    case types::ms2:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)31u));
      break;
    case types::ms4:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)63u));
      break;
    case types::ms5:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)79u));
      break;
    case types::ms8:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)127u));
      break;
    case types::ms10:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)159u));
      break;
    case types::ms16:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)255u));
      break;
    case types::ms20:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)319u));
      break;
    case types::ms32:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)511u));
      break;
    case types::ms40:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)639u));
      break;
    case types::ms64:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::ms80:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1279u));
      break;
    case types::ms128:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)2047u));
      break;
    case types::ms160:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)2559u));
      break;
    case types::ms256:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u));
      break;
    case types::ms320:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)5119u));
      break;
    case types::ms512:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)8191u));
      break;
    case types::ms640:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)10239u));
      break;
    case types::ms1024:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)16383u));
      break;
    case types::ms1280:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)20479u));
      break;
    case types::ms2560:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)40959u));
      break;
    case types::ms5120:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)81919u));
      break;
    case types::ms10240:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)0u, (uint32_t)163839u));
      break;
    default:
      log_invalid_choice_id(type_, "ncr_ms_periodicity_and_slot_offset_r18_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_ms_periodicity_and_slot_offset_r18_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::ms1:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)15u));
      break;
    case types::ms2:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)31u));
      break;
    case types::ms4:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)63u));
      break;
    case types::ms5:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)79u));
      break;
    case types::ms8:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)127u));
      break;
    case types::ms10:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)159u));
      break;
    case types::ms16:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)255u));
      break;
    case types::ms20:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)319u));
      break;
    case types::ms32:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)511u));
      break;
    case types::ms40:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)639u));
      break;
    case types::ms64:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1023u));
      break;
    case types::ms80:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1279u));
      break;
    case types::ms128:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)2047u));
      break;
    case types::ms160:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)2559u));
      break;
    case types::ms256:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u));
      break;
    case types::ms320:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)5119u));
      break;
    case types::ms512:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)8191u));
      break;
    case types::ms640:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)10239u));
      break;
    case types::ms1024:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)16383u));
      break;
    case types::ms1280:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)20479u));
      break;
    case types::ms2560:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)40959u));
      break;
    case types::ms5120:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)81919u));
      break;
    case types::ms10240:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)0u, (uint32_t)163839u));
      break;
    default:
      log_invalid_choice_id(type_, "ncr_ms_periodicity_and_slot_offset_r18_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ncr_ms_periodicity_and_slot_offset_r18_c::types_opts::to_string() const
{
  static const char* names[] = {"ms1",   "ms2",   "ms4",    "ms5",    "ms8",    "ms10",   "ms16",   "ms20",
                                "ms32",  "ms40",  "ms64",   "ms80",   "ms128",  "ms160",  "ms256",  "ms320",
                                "ms512", "ms640", "ms1024", "ms1280", "ms2560", "ms5120", "ms10240"};
  return convert_enum_idx(names, 23, value, "ncr_ms_periodicity_and_slot_offset_r18_c::types");
}
uint16_t ncr_ms_periodicity_and_slot_offset_r18_c::types_opts::to_number() const
{
  static const uint16_t numbers[] = {1,   2,   4,   5,   8,   10,  16,   20,   32,   40,   64,   80,
                                     128, 160, 256, 320, 512, 640, 1024, 1280, 2560, 5120, 10240};
  return map_enum_number(numbers, 23, value, "ncr_ms_periodicity_and_slot_offset_r18_c::types");
}

// NCR-PeriodicityAndOffset-r18 ::= CHOICE
void ncr_periodicity_and_offset_r18_c::destroy_()
{
  switch (type_) {
    case types::slot:
      c.destroy<ncr_slot_periodicity_and_slot_offset_r18_c>();
      break;
    case types::ms:
      c.destroy<ncr_ms_periodicity_and_slot_offset_r18_c>();
      break;
    default:
      break;
  }
}
void ncr_periodicity_and_offset_r18_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::slot:
      c.init<ncr_slot_periodicity_and_slot_offset_r18_c>();
      break;
    case types::ms:
      c.init<ncr_ms_periodicity_and_slot_offset_r18_c>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ncr_periodicity_and_offset_r18_c");
  }
}
ncr_periodicity_and_offset_r18_c::ncr_periodicity_and_offset_r18_c(const ncr_periodicity_and_offset_r18_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::slot:
      c.init(other.c.get<ncr_slot_periodicity_and_slot_offset_r18_c>());
      break;
    case types::ms:
      c.init(other.c.get<ncr_ms_periodicity_and_slot_offset_r18_c>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ncr_periodicity_and_offset_r18_c");
  }
}
ncr_periodicity_and_offset_r18_c&
ncr_periodicity_and_offset_r18_c::operator=(const ncr_periodicity_and_offset_r18_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::slot:
      c.set(other.c.get<ncr_slot_periodicity_and_slot_offset_r18_c>());
      break;
    case types::ms:
      c.set(other.c.get<ncr_ms_periodicity_and_slot_offset_r18_c>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "ncr_periodicity_and_offset_r18_c");
  }

  return *this;
}
ncr_slot_periodicity_and_slot_offset_r18_c& ncr_periodicity_and_offset_r18_c::set_slot()
{
  set(types::slot);
  return c.get<ncr_slot_periodicity_and_slot_offset_r18_c>();
}
ncr_ms_periodicity_and_slot_offset_r18_c& ncr_periodicity_and_offset_r18_c::set_ms()
{
  set(types::ms);
  return c.get<ncr_ms_periodicity_and_slot_offset_r18_c>();
}
void ncr_periodicity_and_offset_r18_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::slot:
      j.write_fieldname("slot");
      c.get<ncr_slot_periodicity_and_slot_offset_r18_c>().to_json(j);
      break;
    case types::ms:
      j.write_fieldname("ms");
      c.get<ncr_ms_periodicity_and_slot_offset_r18_c>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "ncr_periodicity_and_offset_r18_c");
  }
  j.end_obj();
}
OCUDUASN_CODE ncr_periodicity_and_offset_r18_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::slot:
      HANDLE_CODE(c.get<ncr_slot_periodicity_and_slot_offset_r18_c>().pack(bref));
      break;
    case types::ms:
      HANDLE_CODE(c.get<ncr_ms_periodicity_and_slot_offset_r18_c>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ncr_periodicity_and_offset_r18_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_periodicity_and_offset_r18_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::slot:
      HANDLE_CODE(c.get<ncr_slot_periodicity_and_slot_offset_r18_c>().unpack(bref));
      break;
    case types::ms:
      HANDLE_CODE(c.get<ncr_ms_periodicity_and_slot_offset_r18_c>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "ncr_periodicity_and_offset_r18_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* ncr_periodicity_and_offset_r18_c::types_opts::to_string() const
{
  static const char* names[] = {"slot", "ms"};
  return convert_enum_idx(names, 2, value, "ncr_periodicity_and_offset_r18_c::types");
}

// NCR-PeriodicFwdResource-r18 ::= SEQUENCE
OCUDUASN_CODE ncr_periodic_fwd_res_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pack_integer(bref, periodic_fwd_rsrc_id_r18, (uint16_t)0u, (uint16_t)1023u));
  HANDLE_CODE(pack_integer(bref, beam_idx_r18, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(periodic_time_rsrc_r18.periodicity_and_offset_r18.pack(bref));
  HANDLE_CODE(pack_integer(bref, periodic_time_rsrc_r18.symbol_offset_r18, (uint8_t)0u, (uint8_t)13u));
  HANDLE_CODE(pack_integer(bref, periodic_time_rsrc_r18.dur_in_symbols_r18, (uint8_t)1u, (uint8_t)112u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_periodic_fwd_res_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_integer(periodic_fwd_rsrc_id_r18, bref, (uint16_t)0u, (uint16_t)1023u));
  HANDLE_CODE(unpack_integer(beam_idx_r18, bref, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(periodic_time_rsrc_r18.periodicity_and_offset_r18.unpack(bref));
  HANDLE_CODE(unpack_integer(periodic_time_rsrc_r18.symbol_offset_r18, bref, (uint8_t)0u, (uint8_t)13u));
  HANDLE_CODE(unpack_integer(periodic_time_rsrc_r18.dur_in_symbols_r18, bref, (uint8_t)1u, (uint8_t)112u));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ncr_periodic_fwd_res_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("periodicFwdRsrcId-r18", periodic_fwd_rsrc_id_r18);
  j.write_int("beamIndex-r18", beam_idx_r18);
  j.write_fieldname("periodicTimeRsrc-r18");
  j.start_obj();
  j.write_fieldname("periodicityAndOffset-r18");
  periodic_time_rsrc_r18.periodicity_and_offset_r18.to_json(j);
  j.write_int("symbolOffset-r18", periodic_time_rsrc_r18.symbol_offset_r18);
  j.write_int("durationInSymbols-r18", periodic_time_rsrc_r18.dur_in_symbols_r18);
  j.end_obj();
  j.end_obj();
}

// NCR-PeriodicFwdResourceSet-r18 ::= SEQUENCE
OCUDUASN_CODE ncr_periodic_fwd_res_set_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {false,
                         periodic_fwd_rsrc_to_add_mod_list_r18.size() > 0,
                         periodic_fwd_rsrc_to_release_list_r18.size() > 0,
                         ref_scs_r18_present,
                         prio_flag_r18_present}));

  HANDLE_CODE(pack_integer(bref, periodic_fwd_rsrc_set_id_r18, (uint8_t)0u, (uint8_t)31u));
  if (periodic_fwd_rsrc_to_add_mod_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, periodic_fwd_rsrc_to_add_mod_list_r18, 1, 1024));
  }
  if (periodic_fwd_rsrc_to_release_list_r18.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, periodic_fwd_rsrc_to_release_list_r18, 1, 1024, integer_packer<uint16_t>(0, 1023)));
  }
  if (ref_scs_r18_present) {
    HANDLE_CODE(ref_scs_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_periodic_fwd_res_set_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool periodic_fwd_rsrc_to_add_mod_list_r18_present;
  bool periodic_fwd_rsrc_to_release_list_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &periodic_fwd_rsrc_to_add_mod_list_r18_present,
                           &periodic_fwd_rsrc_to_release_list_r18_present,
                           &ref_scs_r18_present,
                           &prio_flag_r18_present}));

  HANDLE_CODE(unpack_integer(periodic_fwd_rsrc_set_id_r18, bref, (uint8_t)0u, (uint8_t)31u));
  if (periodic_fwd_rsrc_to_add_mod_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(periodic_fwd_rsrc_to_add_mod_list_r18, bref, 1, 1024));
  }
  if (periodic_fwd_rsrc_to_release_list_r18_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(periodic_fwd_rsrc_to_release_list_r18, bref, 1, 1024, integer_packer<uint16_t>(0, 1023)));
  }
  if (ref_scs_r18_present) {
    HANDLE_CODE(ref_scs_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ncr_periodic_fwd_res_set_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("periodicFwdRsrcSetId-r18", periodic_fwd_rsrc_set_id_r18);
  if (periodic_fwd_rsrc_to_add_mod_list_r18.size() > 0) {
    j.start_array("periodicFwdRsrcToAddModList-r18");
    for (const auto& e1 : periodic_fwd_rsrc_to_add_mod_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (periodic_fwd_rsrc_to_release_list_r18.size() > 0) {
    j.start_array("periodicFwdRsrcToReleaseList-r18");
    for (const auto& e1 : periodic_fwd_rsrc_to_release_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ref_scs_r18_present) {
    j.write_str("referenceSCS-r18", ref_scs_r18.to_string());
  }
  if (prio_flag_r18_present) {
    j.write_str("priorityFlag-r18", "true");
  }
  j.end_obj();
}

// NCR-AperiodicFwdTimeResource-r18 ::= SEQUENCE
OCUDUASN_CODE ncr_aperiodic_fwd_time_res_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pack_integer(bref, aperiodic_fwd_time_rsrc_id_r18, (uint8_t)0u, (uint8_t)111u));
  HANDLE_CODE(pack_integer(bref, slot_offset_aperiodic_r18, (uint8_t)0u, (uint8_t)14u));
  HANDLE_CODE(pack_integer(bref, symbol_offset_r18, (uint8_t)0u, (uint8_t)13u));
  HANDLE_CODE(pack_integer(bref, dur_in_symbols_r18, (uint8_t)1u, (uint8_t)28u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_aperiodic_fwd_time_res_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_integer(aperiodic_fwd_time_rsrc_id_r18, bref, (uint8_t)0u, (uint8_t)111u));
  HANDLE_CODE(unpack_integer(slot_offset_aperiodic_r18, bref, (uint8_t)0u, (uint8_t)14u));
  HANDLE_CODE(unpack_integer(symbol_offset_r18, bref, (uint8_t)0u, (uint8_t)13u));
  HANDLE_CODE(unpack_integer(dur_in_symbols_r18, bref, (uint8_t)1u, (uint8_t)28u));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ncr_aperiodic_fwd_time_res_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("aperiodicFwdTimeRsrcId-r18", aperiodic_fwd_time_rsrc_id_r18);
  j.write_int("slotOffsetAperiodic-r18", slot_offset_aperiodic_r18);
  j.write_int("symbolOffset-r18", symbol_offset_r18);
  j.write_int("durationInSymbols-r18", dur_in_symbols_r18);
  j.end_obj();
}

// NCR-AperiodicFwdConfig-r18 ::= SEQUENCE
OCUDUASN_CODE ncr_aperiodic_fwd_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {false,
                         aperiodic_fwd_time_rsrc_to_add_mod_list_r18.size() > 0,
                         aperiodic_fwd_time_rsrc_to_release_list_r18.size() > 0,
                         ref_scs_r18_present,
                         aperiodic_beam_field_width_r18_present,
                         nof_fields_r18_present}));

  if (aperiodic_fwd_time_rsrc_to_add_mod_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, aperiodic_fwd_time_rsrc_to_add_mod_list_r18, 1, 112));
  }
  if (aperiodic_fwd_time_rsrc_to_release_list_r18.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, aperiodic_fwd_time_rsrc_to_release_list_r18, 1, 112, integer_packer<uint8_t>(0, 111)));
  }
  if (ref_scs_r18_present) {
    HANDLE_CODE(ref_scs_r18.pack(bref));
  }
  if (aperiodic_beam_field_width_r18_present) {
    HANDLE_CODE(pack_integer(bref, aperiodic_beam_field_width_r18, (uint8_t)1u, (uint8_t)6u));
  }
  if (nof_fields_r18_present) {
    HANDLE_CODE(pack_integer(bref, nof_fields_r18, (uint8_t)1u, (uint8_t)32u));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_aperiodic_fwd_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool aperiodic_fwd_time_rsrc_to_add_mod_list_r18_present;
  bool aperiodic_fwd_time_rsrc_to_release_list_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &aperiodic_fwd_time_rsrc_to_add_mod_list_r18_present,
                           &aperiodic_fwd_time_rsrc_to_release_list_r18_present,
                           &ref_scs_r18_present,
                           &aperiodic_beam_field_width_r18_present,
                           &nof_fields_r18_present}));

  if (aperiodic_fwd_time_rsrc_to_add_mod_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(aperiodic_fwd_time_rsrc_to_add_mod_list_r18, bref, 1, 112));
  }
  if (aperiodic_fwd_time_rsrc_to_release_list_r18_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(aperiodic_fwd_time_rsrc_to_release_list_r18, bref, 1, 112, integer_packer<uint8_t>(0, 111)));
  }
  if (ref_scs_r18_present) {
    HANDLE_CODE(ref_scs_r18.unpack(bref));
  }
  if (aperiodic_beam_field_width_r18_present) {
    HANDLE_CODE(unpack_integer(aperiodic_beam_field_width_r18, bref, (uint8_t)1u, (uint8_t)6u));
  }
  if (nof_fields_r18_present) {
    HANDLE_CODE(unpack_integer(nof_fields_r18, bref, (uint8_t)1u, (uint8_t)32u));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ncr_aperiodic_fwd_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (aperiodic_fwd_time_rsrc_to_add_mod_list_r18.size() > 0) {
    j.start_array("aperiodicFwdTimeRsrcToAddModList-r18");
    for (const auto& e1 : aperiodic_fwd_time_rsrc_to_add_mod_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (aperiodic_fwd_time_rsrc_to_release_list_r18.size() > 0) {
    j.start_array("aperiodicFwdTimeRsrcToReleaseList-r18");
    for (const auto& e1 : aperiodic_fwd_time_rsrc_to_release_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ref_scs_r18_present) {
    j.write_str("referenceSCS-r18", ref_scs_r18.to_string());
  }
  if (aperiodic_beam_field_width_r18_present) {
    j.write_int("aperiodicBeamFieldWidth-r18", aperiodic_beam_field_width_r18);
  }
  if (nof_fields_r18_present) {
    j.write_int("numberOfFields-r18", nof_fields_r18);
  }
  j.end_obj();
}

// NCR-SemiPersistentFwdResource-r18 ::= SEQUENCE
OCUDUASN_CODE ncr_semi_persistent_fwd_res_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(bref.pack(false, 1));

  HANDLE_CODE(pack_integer(bref, semi_persistent_fwd_rsrc_id_r18, (uint8_t)0u, (uint8_t)127u));
  HANDLE_CODE(pack_integer(bref, beam_idx_r18, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(semi_persistent_time_rsrc_r18.periodicity_and_offset_r18.pack(bref));
  HANDLE_CODE(pack_integer(bref, semi_persistent_time_rsrc_r18.symbol_offset_r18, (uint8_t)0u, (uint8_t)13u));
  HANDLE_CODE(pack_integer(bref, semi_persistent_time_rsrc_r18.dur_in_symbols_r18, (uint8_t)1u, (uint8_t)112u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_semi_persistent_fwd_res_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));

  HANDLE_CODE(unpack_integer(semi_persistent_fwd_rsrc_id_r18, bref, (uint8_t)0u, (uint8_t)127u));
  HANDLE_CODE(unpack_integer(beam_idx_r18, bref, (uint8_t)0u, (uint8_t)63u));
  HANDLE_CODE(semi_persistent_time_rsrc_r18.periodicity_and_offset_r18.unpack(bref));
  HANDLE_CODE(unpack_integer(semi_persistent_time_rsrc_r18.symbol_offset_r18, bref, (uint8_t)0u, (uint8_t)13u));
  HANDLE_CODE(unpack_integer(semi_persistent_time_rsrc_r18.dur_in_symbols_r18, bref, (uint8_t)1u, (uint8_t)112u));

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ncr_semi_persistent_fwd_res_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("semiPersistentFwdRsrcId-r18", semi_persistent_fwd_rsrc_id_r18);
  j.write_int("beamIndex-r18", beam_idx_r18);
  j.write_fieldname("semiPersistentTimeRsrc-r18");
  j.start_obj();
  j.write_fieldname("periodicityAndOffset-r18");
  semi_persistent_time_rsrc_r18.periodicity_and_offset_r18.to_json(j);
  j.write_int("symbolOffset-r18", semi_persistent_time_rsrc_r18.symbol_offset_r18);
  j.write_int("durationInSymbols-r18", semi_persistent_time_rsrc_r18.dur_in_symbols_r18);
  j.end_obj();
  j.end_obj();
}

// NCR-SemiPersistentFwdResourceSet-r18 ::= SEQUENCE
OCUDUASN_CODE ncr_semi_persistent_fwd_res_set_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {false,
                         semi_persistent_fwd_rsrc_to_add_mod_list_r18.size() > 0,
                         semi_persistent_fwd_rsrc_to_release_list_r18.size() > 0,
                         ref_scs_r18_present,
                         prio_flag_r18_present}));

  HANDLE_CODE(pack_integer(bref, semi_persistent_fwd_rsrc_set_id_r18, (uint8_t)0u, (uint8_t)31u));
  if (semi_persistent_fwd_rsrc_to_add_mod_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, semi_persistent_fwd_rsrc_to_add_mod_list_r18, 1, 128));
  }
  if (semi_persistent_fwd_rsrc_to_release_list_r18.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, semi_persistent_fwd_rsrc_to_release_list_r18, 1, 128, integer_packer<uint8_t>(0, 127)));
  }
  if (ref_scs_r18_present) {
    HANDLE_CODE(ref_scs_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_semi_persistent_fwd_res_set_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool semi_persistent_fwd_rsrc_to_add_mod_list_r18_present;
  bool semi_persistent_fwd_rsrc_to_release_list_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &semi_persistent_fwd_rsrc_to_add_mod_list_r18_present,
                           &semi_persistent_fwd_rsrc_to_release_list_r18_present,
                           &ref_scs_r18_present,
                           &prio_flag_r18_present}));

  HANDLE_CODE(unpack_integer(semi_persistent_fwd_rsrc_set_id_r18, bref, (uint8_t)0u, (uint8_t)31u));
  if (semi_persistent_fwd_rsrc_to_add_mod_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(semi_persistent_fwd_rsrc_to_add_mod_list_r18, bref, 1, 128));
  }
  if (semi_persistent_fwd_rsrc_to_release_list_r18_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(semi_persistent_fwd_rsrc_to_release_list_r18, bref, 1, 128, integer_packer<uint8_t>(0, 127)));
  }
  if (ref_scs_r18_present) {
    HANDLE_CODE(ref_scs_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ncr_semi_persistent_fwd_res_set_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("semiPersistentFwdRsrcSetId-r18", semi_persistent_fwd_rsrc_set_id_r18);
  if (semi_persistent_fwd_rsrc_to_add_mod_list_r18.size() > 0) {
    j.start_array("semiPersistentFwdRsrcToAddModList-r18");
    for (const auto& e1 : semi_persistent_fwd_rsrc_to_add_mod_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (semi_persistent_fwd_rsrc_to_release_list_r18.size() > 0) {
    j.start_array("semiPersistentFwdRsrcToReleaseList-r18");
    for (const auto& e1 : semi_persistent_fwd_rsrc_to_release_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ref_scs_r18_present) {
    j.write_str("referenceSCS-r18", ref_scs_r18.to_string());
  }
  if (prio_flag_r18_present) {
    j.write_str("priorityFlag-r18", "true");
  }
  j.end_obj();
}

// NCR-FwdConfig-r18 ::= SEQUENCE
OCUDUASN_CODE ncr_fwd_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {false,
                         periodic_fwd_rsrc_set_to_add_mod_list_r18.size() > 0,
                         periodic_fwd_rsrc_set_to_release_list_r18.size() > 0,
                         aperiodic_fwd_cfg_r18_present,
                         semi_persistent_fwd_rsrc_set_to_add_mod_list_r18.size() > 0,
                         semi_persistent_fwd_rsrc_set_to_release_list_r18.size() > 0}));

  if (periodic_fwd_rsrc_set_to_add_mod_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, periodic_fwd_rsrc_set_to_add_mod_list_r18, 1, 32));
  }
  if (periodic_fwd_rsrc_set_to_release_list_r18.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, periodic_fwd_rsrc_set_to_release_list_r18, 1, 32, integer_packer<uint8_t>(0, 31)));
  }
  if (aperiodic_fwd_cfg_r18_present) {
    HANDLE_CODE(aperiodic_fwd_cfg_r18.pack(bref));
  }
  if (semi_persistent_fwd_rsrc_set_to_add_mod_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, semi_persistent_fwd_rsrc_set_to_add_mod_list_r18, 1, 32));
  }
  if (semi_persistent_fwd_rsrc_set_to_release_list_r18.size() > 0) {
    HANDLE_CODE(
        pack_dyn_seq_of(bref, semi_persistent_fwd_rsrc_set_to_release_list_r18, 1, 32, integer_packer<uint8_t>(0, 31)));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ncr_fwd_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool periodic_fwd_rsrc_set_to_add_mod_list_r18_present;
  bool periodic_fwd_rsrc_set_to_release_list_r18_present;
  bool semi_persistent_fwd_rsrc_set_to_add_mod_list_r18_present;
  bool semi_persistent_fwd_rsrc_set_to_release_list_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &periodic_fwd_rsrc_set_to_add_mod_list_r18_present,
                           &periodic_fwd_rsrc_set_to_release_list_r18_present,
                           &aperiodic_fwd_cfg_r18_present,
                           &semi_persistent_fwd_rsrc_set_to_add_mod_list_r18_present,
                           &semi_persistent_fwd_rsrc_set_to_release_list_r18_present}));

  if (periodic_fwd_rsrc_set_to_add_mod_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(periodic_fwd_rsrc_set_to_add_mod_list_r18, bref, 1, 32));
  }
  if (periodic_fwd_rsrc_set_to_release_list_r18_present) {
    HANDLE_CODE(
        unpack_dyn_seq_of(periodic_fwd_rsrc_set_to_release_list_r18, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
  }
  if (aperiodic_fwd_cfg_r18_present) {
    HANDLE_CODE(aperiodic_fwd_cfg_r18.unpack(bref));
  }
  if (semi_persistent_fwd_rsrc_set_to_add_mod_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(semi_persistent_fwd_rsrc_set_to_add_mod_list_r18, bref, 1, 32));
  }
  if (semi_persistent_fwd_rsrc_set_to_release_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(
        semi_persistent_fwd_rsrc_set_to_release_list_r18, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ncr_fwd_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (periodic_fwd_rsrc_set_to_add_mod_list_r18.size() > 0) {
    j.start_array("periodicFwdRsrcSetToAddModList-r18");
    for (const auto& e1 : periodic_fwd_rsrc_set_to_add_mod_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (periodic_fwd_rsrc_set_to_release_list_r18.size() > 0) {
    j.start_array("periodicFwdRsrcSetToReleaseList-r18");
    for (const auto& e1 : periodic_fwd_rsrc_set_to_release_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (aperiodic_fwd_cfg_r18_present) {
    j.write_fieldname("aperiodicFwdConfig-r18");
    aperiodic_fwd_cfg_r18.to_json(j);
  }
  if (semi_persistent_fwd_rsrc_set_to_add_mod_list_r18.size() > 0) {
    j.start_array("semiPersistentFwdRsrcSetToAddModList-r18");
    for (const auto& e1 : semi_persistent_fwd_rsrc_set_to_add_mod_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (semi_persistent_fwd_rsrc_set_to_release_list_r18.size() > 0) {
    j.start_array("semiPersistentFwdRsrcSetToReleaseList-r18");
    for (const auto& e1 : semi_persistent_fwd_rsrc_set_to_release_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  j.end_obj();
}

// UplinkTxSwitchingBandPairConfig-r18 ::= SEQUENCE
OCUDUASN_CODE ul_tx_switching_band_pair_cfg_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref, {false, switching2_t_mode_r18_present, switching_period_cfg_for_band_pair_r18_present}));

  HANDLE_CODE(pack_integer(bref, band_info_ul1_r18, (uint8_t)1u, (uint8_t)32u));
  HANDLE_CODE(pack_integer(bref, band_info_ul2_r18, (uint8_t)1u, (uint8_t)32u));
  HANDLE_CODE(switching_option_cfg_for_band_pair_r18.pack(bref));
  if (switching_period_cfg_for_band_pair_r18_present) {
    HANDLE_CODE(switching_period_cfg_for_band_pair_r18.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_tx_switching_band_pair_cfg_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(
      unpack_bits(bref, {&ext, &switching2_t_mode_r18_present, &switching_period_cfg_for_band_pair_r18_present}));

  HANDLE_CODE(unpack_integer(band_info_ul1_r18, bref, (uint8_t)1u, (uint8_t)32u));
  HANDLE_CODE(unpack_integer(band_info_ul2_r18, bref, (uint8_t)1u, (uint8_t)32u));
  HANDLE_CODE(switching_option_cfg_for_band_pair_r18.unpack(bref));
  if (switching_period_cfg_for_band_pair_r18_present) {
    HANDLE_CODE(switching_period_cfg_for_band_pair_r18.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ul_tx_switching_band_pair_cfg_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("bandInfoUL1-r18", band_info_ul1_r18);
  j.write_int("bandInfoUL2-r18", band_info_ul2_r18);
  j.write_str("switchingOptionConfigForBandPair-r18", switching_option_cfg_for_band_pair_r18.to_string());
  if (switching2_t_mode_r18_present) {
    j.write_str("switching2T-Mode-r18", "enabled");
  }
  if (switching_period_cfg_for_band_pair_r18_present) {
    j.write_str("switchingPeriodConfigForBandPair-r18", switching_period_cfg_for_band_pair_r18.to_string());
  }
  j.end_obj();
}

const char* ul_tx_switching_band_pair_cfg_r18_s::switching_option_cfg_for_band_pair_r18_opts::to_string() const
{
  static const char* names[] = {"switchedUL", "dualUL"};
  return convert_enum_idx(
      names, 2, value, "ul_tx_switching_band_pair_cfg_r18_s::switching_option_cfg_for_band_pair_r18_e_");
}

const char* ul_tx_switching_band_pair_cfg_r18_s::switching_period_cfg_for_band_pair_r18_opts::to_string() const
{
  static const char* names[] = {"n35us", "n140us"};
  return convert_enum_idx(
      names, 2, value, "ul_tx_switching_band_pair_cfg_r18_s::switching_period_cfg_for_band_pair_r18_e_");
}
uint8_t ul_tx_switching_band_pair_cfg_r18_s::switching_period_cfg_for_band_pair_r18_opts::to_number() const
{
  static const uint8_t numbers[] = {35, 140};
  return map_enum_number(
      numbers, 2, value, "ul_tx_switching_band_pair_cfg_r18_s::switching_period_cfg_for_band_pair_r18_e_");
}

// UplinkTxSwitchingAssociatedBandDualUL-r18 ::= SEQUENCE
OCUDUASN_CODE ul_tx_switching_associated_band_dual_ul_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_integer(bref, tx_band_r18, (uint8_t)1u, (uint8_t)32u));
  HANDLE_CODE(pack_integer(bref, associated_band_r18, (uint8_t)1u, (uint8_t)32u));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_tx_switching_associated_band_dual_ul_r18_s::unpack(cbit_ref& bref)
{
  HANDLE_CODE(unpack_integer(tx_band_r18, bref, (uint8_t)1u, (uint8_t)32u));
  HANDLE_CODE(unpack_integer(associated_band_r18, bref, (uint8_t)1u, (uint8_t)32u));

  return OCUDUASN_SUCCESS;
}
void ul_tx_switching_associated_band_dual_ul_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("transmitBand-r18", tx_band_r18);
  j.write_int("associatedBand-r18", associated_band_r18);
  j.end_obj();
}

// UplinkTxSwitchingMoreBands-r18 ::= SEQUENCE
OCUDUASN_CODE ul_tx_switching_more_bands_r18_s::pack(bit_ref& bref) const
{
  HANDLE_CODE(pack_bits(bref,
                        {false,
                         ul_tx_switching_band_list_r18.size() > 0,
                         ul_tx_switching_band_pair_list_r18.size() > 0,
                         ul_tx_switching_associated_band_dual_ul_list_r18_present}));

  if (ul_tx_switching_band_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ul_tx_switching_band_list_r18, 1, 32, integer_packer<uint16_t>(1, 1024)));
  }
  if (ul_tx_switching_band_pair_list_r18.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ul_tx_switching_band_pair_list_r18, 1, 32));
  }
  if (ul_tx_switching_associated_band_dual_ul_list_r18_present) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ul_tx_switching_associated_band_dual_ul_list_r18, 0, 32));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_tx_switching_more_bands_r18_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool ul_tx_switching_band_list_r18_present;
  bool ul_tx_switching_band_pair_list_r18_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &ul_tx_switching_band_list_r18_present,
                           &ul_tx_switching_band_pair_list_r18_present,
                           &ul_tx_switching_associated_band_dual_ul_list_r18_present}));

  if (ul_tx_switching_band_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ul_tx_switching_band_list_r18, bref, 1, 32, integer_packer<uint16_t>(1, 1024)));
  }
  if (ul_tx_switching_band_pair_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ul_tx_switching_band_pair_list_r18, bref, 1, 32));
  }
  if (ul_tx_switching_associated_band_dual_ul_list_r18_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ul_tx_switching_associated_band_dual_ul_list_r18, bref, 0, 32));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void ul_tx_switching_more_bands_r18_s::to_json(json_writer& j) const
{
  j.start_obj();
  if (ul_tx_switching_band_list_r18.size() > 0) {
    j.start_array("uplinkTxSwitchingBandList-r18");
    for (const auto& e1 : ul_tx_switching_band_list_r18) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ul_tx_switching_band_pair_list_r18.size() > 0) {
    j.start_array("uplinkTxSwitchingBandPairList-r18");
    for (const auto& e1 : ul_tx_switching_band_pair_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (ul_tx_switching_associated_band_dual_ul_list_r18_present) {
    j.start_array("uplinkTxSwitchingAssociatedBandDualUL-List-r18");
    for (const auto& e1 : ul_tx_switching_associated_band_dual_ul_list_r18) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// CellGroupConfig ::= SEQUENCE
OCUDUASN_CODE cell_group_cfg_s::pack(bit_ref& bref) const
{
  ext_groups_packer_guard group_flags(7);
  group_flags[0] |= report_ul_tx_direct_current_present;
  group_flags[1] |= bap_address_r16_present;
  group_flags[1] |= bh_rlc_ch_to_add_mod_list_r16.is_present();
  group_flags[1] |= bh_rlc_ch_to_release_list_r16.is_present();
  group_flags[1] |= f1c_transfer_path_r16_present;
  group_flags[1] |= simul_tci_upd_list1_r16.is_present();
  group_flags[1] |= simul_tci_upd_list2_r16.is_present();
  group_flags[1] |= simul_spatial_upd_list1_r16.is_present();
  group_flags[1] |= simul_spatial_upd_list2_r16.is_present();
  group_flags[1] |= ul_tx_switching_option_r16_present;
  group_flags[1] |= ul_tx_switching_pwr_boosting_r16_present;
  group_flags[2] |= report_ul_tx_direct_current_two_carrier_r16_present;
  group_flags[3] |= f1c_transfer_path_nrdc_r17_present;
  group_flags[3] |= ul_tx_switching_2_t_mode_r17_present;
  group_flags[3] |= ul_tx_switching_dual_ul_tx_state_r17_present;
  group_flags[3] |= uu_relay_rlc_ch_to_add_mod_list_r17.is_present();
  group_flags[3] |= uu_relay_rlc_ch_to_release_list_r17.is_present();
  group_flags[3] |= simul_u_tci_upd_list1_r17.is_present();
  group_flags[3] |= simul_u_tci_upd_list2_r17.is_present();
  group_flags[3] |= simul_u_tci_upd_list3_r17.is_present();
  group_flags[3] |= simul_u_tci_upd_list4_r17.is_present();
  group_flags[3] |= rlc_bearer_to_release_list_ext_r17.is_present();
  group_flags[3] |= iab_res_cfg_to_add_mod_list_r17.is_present();
  group_flags[3] |= iab_res_cfg_to_release_list_r17.is_present();
  group_flags[4] |= report_ul_tx_direct_current_more_carrier_r17.is_present();
  group_flags[5] |= prio_scell_prach_over_sp_periodic_srs_r17_present;
  group_flags[6] |= ncr_fwd_cfg_r18.is_present();
  group_flags[6] |= autonomous_denial_params_r18.is_present();
  group_flags[6] |= non_collocated_type_mrdc_r18_present;
  group_flags[6] |= non_collocated_type_nr_ca_r18_present;
  group_flags[6] |= ul_tx_switching_more_bands_r18.is_present();
  HANDLE_CODE(pack_bits(bref,
                        {group_flags.any(),
                         rlc_bearer_to_add_mod_list.size() > 0,
                         rlc_bearer_to_release_list.size() > 0,
                         mac_cell_group_cfg_present,
                         phys_cell_group_cfg_present,
                         sp_cell_cfg_present,
                         scell_to_add_mod_list.size() > 0,
                         scell_to_release_list.size() > 0}));

  HANDLE_CODE(pack_integer(bref, cell_group_id, (uint8_t)0u, (uint8_t)3u));
  if (rlc_bearer_to_add_mod_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, rlc_bearer_to_add_mod_list, 1, 32));
  }
  if (rlc_bearer_to_release_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, rlc_bearer_to_release_list, 1, 32, integer_packer<uint8_t>(1, 32)));
  }
  if (mac_cell_group_cfg_present) {
    HANDLE_CODE(mac_cell_group_cfg.pack(bref));
  }
  if (phys_cell_group_cfg_present) {
    HANDLE_CODE(phys_cell_group_cfg.pack(bref));
  }
  if (sp_cell_cfg_present) {
    HANDLE_CODE(sp_cell_cfg.pack(bref));
  }
  if (scell_to_add_mod_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, scell_to_add_mod_list, 1, 31));
  }
  if (scell_to_release_list.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, scell_to_release_list, 1, 31, integer_packer<uint8_t>(1, 31)));
  }

  if (group_flags.any()) {
    HANDLE_CODE(group_flags.pack(bref));

    if (group_flags[0]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(report_ul_tx_direct_current_present, 1));
    }
    if (group_flags[1]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {bap_address_r16_present,
                             bh_rlc_ch_to_add_mod_list_r16.is_present(),
                             bh_rlc_ch_to_release_list_r16.is_present(),
                             f1c_transfer_path_r16_present,
                             simul_tci_upd_list1_r16.is_present(),
                             simul_tci_upd_list2_r16.is_present(),
                             simul_spatial_upd_list1_r16.is_present(),
                             simul_spatial_upd_list2_r16.is_present(),
                             ul_tx_switching_option_r16_present,
                             ul_tx_switching_pwr_boosting_r16_present}));
      if (bap_address_r16_present) {
        HANDLE_CODE(bap_address_r16.pack(bref));
      }
      if (bh_rlc_ch_to_add_mod_list_r16.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *bh_rlc_ch_to_add_mod_list_r16, 1, 65536));
      }
      if (bh_rlc_ch_to_release_list_r16.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *bh_rlc_ch_to_release_list_r16, 1, 65536));
      }
      if (f1c_transfer_path_r16_present) {
        HANDLE_CODE(f1c_transfer_path_r16.pack(bref));
      }
      if (simul_tci_upd_list1_r16.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *simul_tci_upd_list1_r16, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_tci_upd_list2_r16.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *simul_tci_upd_list2_r16, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_spatial_upd_list1_r16.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *simul_spatial_upd_list1_r16, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_spatial_upd_list2_r16.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *simul_spatial_upd_list2_r16, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (ul_tx_switching_option_r16_present) {
        HANDLE_CODE(ul_tx_switching_option_r16.pack(bref));
      }
    }
    if (group_flags[2]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(report_ul_tx_direct_current_two_carrier_r16_present, 1));
    }
    if (group_flags[3]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {f1c_transfer_path_nrdc_r17_present,
                             ul_tx_switching_2_t_mode_r17_present,
                             ul_tx_switching_dual_ul_tx_state_r17_present,
                             uu_relay_rlc_ch_to_add_mod_list_r17.is_present(),
                             uu_relay_rlc_ch_to_release_list_r17.is_present(),
                             simul_u_tci_upd_list1_r17.is_present(),
                             simul_u_tci_upd_list2_r17.is_present(),
                             simul_u_tci_upd_list3_r17.is_present(),
                             simul_u_tci_upd_list4_r17.is_present(),
                             rlc_bearer_to_release_list_ext_r17.is_present(),
                             iab_res_cfg_to_add_mod_list_r17.is_present(),
                             iab_res_cfg_to_release_list_r17.is_present()}));
      if (f1c_transfer_path_nrdc_r17_present) {
        HANDLE_CODE(f1c_transfer_path_nrdc_r17.pack(bref));
      }
      if (ul_tx_switching_dual_ul_tx_state_r17_present) {
        HANDLE_CODE(ul_tx_switching_dual_ul_tx_state_r17.pack(bref));
      }
      if (uu_relay_rlc_ch_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *uu_relay_rlc_ch_to_add_mod_list_r17, 1, 32));
      }
      if (uu_relay_rlc_ch_to_release_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *uu_relay_rlc_ch_to_release_list_r17, 1, 32, integer_packer<uint8_t>(1, 32)));
      }
      if (simul_u_tci_upd_list1_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *simul_u_tci_upd_list1_r17, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_u_tci_upd_list2_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *simul_u_tci_upd_list2_r17, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_u_tci_upd_list3_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *simul_u_tci_upd_list3_r17, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_u_tci_upd_list4_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *simul_u_tci_upd_list4_r17, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (rlc_bearer_to_release_list_ext_r17.is_present()) {
        HANDLE_CODE(
            pack_dyn_seq_of(bref, *rlc_bearer_to_release_list_ext_r17, 1, 32, integer_packer<uint32_t>(320, 65855)));
      }
      if (iab_res_cfg_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *iab_res_cfg_to_add_mod_list_r17, 1, 65536));
      }
      if (iab_res_cfg_to_release_list_r17.is_present()) {
        HANDLE_CODE(
            pack_dyn_seq_of(bref, *iab_res_cfg_to_release_list_r17, 1, 65536, integer_packer<uint32_t>(0, 65535)));
      }
    }
    if (group_flags[4]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(report_ul_tx_direct_current_more_carrier_r17.is_present(), 1));
      if (report_ul_tx_direct_current_more_carrier_r17.is_present()) {
        HANDLE_CODE(pack_dyn_seq_of(bref, *report_ul_tx_direct_current_more_carrier_r17, 1, 32));
      }
    }
    if (group_flags[5]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(bref.pack(prio_scell_prach_over_sp_periodic_srs_r17_present, 1));
    }
    if (group_flags[6]) {
      varlength_field_pack_guard varlen_scope(bref, false);

      HANDLE_CODE(pack_bits(bref,
                            {ncr_fwd_cfg_r18.is_present(),
                             autonomous_denial_params_r18.is_present(),
                             non_collocated_type_mrdc_r18_present,
                             non_collocated_type_nr_ca_r18_present,
                             ul_tx_switching_more_bands_r18.is_present()}));
      if (ncr_fwd_cfg_r18.is_present()) {
        HANDLE_CODE(ncr_fwd_cfg_r18->pack(bref));
      }
      if (autonomous_denial_params_r18.is_present()) {
        HANDLE_CODE(autonomous_denial_params_r18->pack(bref));
      }
      if (ul_tx_switching_more_bands_r18.is_present()) {
        HANDLE_CODE(ul_tx_switching_more_bands_r18->pack(bref));
      }
    }
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_group_cfg_s::unpack(cbit_ref& bref)
{
  bool ext;
  bool rlc_bearer_to_add_mod_list_present;
  bool rlc_bearer_to_release_list_present;
  bool scell_to_add_mod_list_present;
  bool scell_to_release_list_present;
  HANDLE_CODE(unpack_bits(bref,
                          {&ext,
                           &rlc_bearer_to_add_mod_list_present,
                           &rlc_bearer_to_release_list_present,
                           &mac_cell_group_cfg_present,
                           &phys_cell_group_cfg_present,
                           &sp_cell_cfg_present,
                           &scell_to_add_mod_list_present,
                           &scell_to_release_list_present}));

  HANDLE_CODE(unpack_integer(cell_group_id, bref, (uint8_t)0u, (uint8_t)3u));
  if (rlc_bearer_to_add_mod_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(rlc_bearer_to_add_mod_list, bref, 1, 32));
  }
  if (rlc_bearer_to_release_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(rlc_bearer_to_release_list, bref, 1, 32, integer_packer<uint8_t>(1, 32)));
  }
  if (mac_cell_group_cfg_present) {
    HANDLE_CODE(mac_cell_group_cfg.unpack(bref));
  }
  if (phys_cell_group_cfg_present) {
    HANDLE_CODE(phys_cell_group_cfg.unpack(bref));
  }
  if (sp_cell_cfg_present) {
    HANDLE_CODE(sp_cell_cfg.unpack(bref));
  }
  if (scell_to_add_mod_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(scell_to_add_mod_list, bref, 1, 31));
  }
  if (scell_to_release_list_present) {
    HANDLE_CODE(unpack_dyn_seq_of(scell_to_release_list, bref, 1, 31, integer_packer<uint8_t>(1, 31)));
  }

  if (ext) {
    ext_groups_unpacker group_unpacker(bref);

    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(report_ul_tx_direct_current_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(bap_address_r16_present, 1));
      unpack_presence_flag(bh_rlc_ch_to_add_mod_list_r16, bref);
      unpack_presence_flag(bh_rlc_ch_to_release_list_r16, bref);
      HANDLE_CODE(bref.unpack(f1c_transfer_path_r16_present, 1));
      unpack_presence_flag(simul_tci_upd_list1_r16, bref);
      unpack_presence_flag(simul_tci_upd_list2_r16, bref);
      unpack_presence_flag(simul_spatial_upd_list1_r16, bref);
      unpack_presence_flag(simul_spatial_upd_list2_r16, bref);
      HANDLE_CODE(unpack_bits(bref, {&ul_tx_switching_option_r16_present, &ul_tx_switching_pwr_boosting_r16_present}));
      if (bap_address_r16_present) {
        HANDLE_CODE(bap_address_r16.unpack(bref));
      }
      if (bh_rlc_ch_to_add_mod_list_r16.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*bh_rlc_ch_to_add_mod_list_r16, bref, 1, 65536));
      }
      if (bh_rlc_ch_to_release_list_r16.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*bh_rlc_ch_to_release_list_r16, bref, 1, 65536));
      }
      if (f1c_transfer_path_r16_present) {
        HANDLE_CODE(f1c_transfer_path_r16.unpack(bref));
      }
      if (simul_tci_upd_list1_r16.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*simul_tci_upd_list1_r16, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_tci_upd_list2_r16.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*simul_tci_upd_list2_r16, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_spatial_upd_list1_r16.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*simul_spatial_upd_list1_r16, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_spatial_upd_list2_r16.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*simul_spatial_upd_list2_r16, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (ul_tx_switching_option_r16_present) {
        HANDLE_CODE(ul_tx_switching_option_r16.unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(report_ul_tx_direct_current_two_carrier_r16_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(unpack_bits(bref,
                              {&f1c_transfer_path_nrdc_r17_present,
                               &ul_tx_switching_2_t_mode_r17_present,
                               &ul_tx_switching_dual_ul_tx_state_r17_present}));
      unpack_presence_flag(uu_relay_rlc_ch_to_add_mod_list_r17, bref);
      unpack_presence_flag(uu_relay_rlc_ch_to_release_list_r17, bref);
      unpack_presence_flag(simul_u_tci_upd_list1_r17, bref);
      unpack_presence_flag(simul_u_tci_upd_list2_r17, bref);
      unpack_presence_flag(simul_u_tci_upd_list3_r17, bref);
      unpack_presence_flag(simul_u_tci_upd_list4_r17, bref);
      unpack_presence_flag(rlc_bearer_to_release_list_ext_r17, bref);
      unpack_presence_flag(iab_res_cfg_to_add_mod_list_r17, bref);
      unpack_presence_flag(iab_res_cfg_to_release_list_r17, bref);
      if (f1c_transfer_path_nrdc_r17_present) {
        HANDLE_CODE(f1c_transfer_path_nrdc_r17.unpack(bref));
      }
      if (ul_tx_switching_dual_ul_tx_state_r17_present) {
        HANDLE_CODE(ul_tx_switching_dual_ul_tx_state_r17.unpack(bref));
      }
      if (uu_relay_rlc_ch_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*uu_relay_rlc_ch_to_add_mod_list_r17, bref, 1, 32));
      }
      if (uu_relay_rlc_ch_to_release_list_r17.is_present()) {
        HANDLE_CODE(
            unpack_dyn_seq_of(*uu_relay_rlc_ch_to_release_list_r17, bref, 1, 32, integer_packer<uint8_t>(1, 32)));
      }
      if (simul_u_tci_upd_list1_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*simul_u_tci_upd_list1_r17, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_u_tci_upd_list2_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*simul_u_tci_upd_list2_r17, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_u_tci_upd_list3_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*simul_u_tci_upd_list3_r17, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (simul_u_tci_upd_list4_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*simul_u_tci_upd_list4_r17, bref, 1, 32, integer_packer<uint8_t>(0, 31)));
      }
      if (rlc_bearer_to_release_list_ext_r17.is_present()) {
        HANDLE_CODE(
            unpack_dyn_seq_of(*rlc_bearer_to_release_list_ext_r17, bref, 1, 32, integer_packer<uint32_t>(320, 65855)));
      }
      if (iab_res_cfg_to_add_mod_list_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*iab_res_cfg_to_add_mod_list_r17, bref, 1, 65536));
      }
      if (iab_res_cfg_to_release_list_r17.is_present()) {
        HANDLE_CODE(
            unpack_dyn_seq_of(*iab_res_cfg_to_release_list_r17, bref, 1, 65536, integer_packer<uint32_t>(0, 65535)));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(report_ul_tx_direct_current_more_carrier_r17, bref);
      if (report_ul_tx_direct_current_more_carrier_r17.is_present()) {
        HANDLE_CODE(unpack_dyn_seq_of(*report_ul_tx_direct_current_more_carrier_r17, bref, 1, 32));
      }
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      HANDLE_CODE(bref.unpack(prio_scell_prach_over_sp_periodic_srs_r17_present, 1));
    }
    HANDLE_CODE(group_unpacker.unpack_next_group());
    if (group_unpacker.get_last_group_range(bref)) {
      unpack_presence_flag(ncr_fwd_cfg_r18, bref);
      unpack_presence_flag(autonomous_denial_params_r18, bref);
      HANDLE_CODE(unpack_bits(bref, {&non_collocated_type_mrdc_r18_present, &non_collocated_type_nr_ca_r18_present}));
      unpack_presence_flag(ul_tx_switching_more_bands_r18, bref);
      if (ncr_fwd_cfg_r18.is_present()) {
        HANDLE_CODE(ncr_fwd_cfg_r18->unpack(bref));
      }
      if (autonomous_denial_params_r18.is_present()) {
        HANDLE_CODE(autonomous_denial_params_r18->unpack(bref));
      }
      if (ul_tx_switching_more_bands_r18.is_present()) {
        HANDLE_CODE(ul_tx_switching_more_bands_r18->unpack(bref));
      }
    }
    HANDLE_CODE(group_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void cell_group_cfg_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("cellGroupId", cell_group_id);
  if (rlc_bearer_to_add_mod_list.size() > 0) {
    j.start_array("rlc-BearerToAddModList");
    for (const auto& e1 : rlc_bearer_to_add_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (rlc_bearer_to_release_list.size() > 0) {
    j.start_array("rlc-BearerToReleaseList");
    for (const auto& e1 : rlc_bearer_to_release_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (mac_cell_group_cfg_present) {
    j.write_fieldname("mac-CellGroupConfig");
    mac_cell_group_cfg.to_json(j);
  }
  if (phys_cell_group_cfg_present) {
    j.write_fieldname("physicalCellGroupConfig");
    phys_cell_group_cfg.to_json(j);
  }
  if (sp_cell_cfg_present) {
    j.write_fieldname("spCellConfig");
    sp_cell_cfg.to_json(j);
  }
  if (scell_to_add_mod_list.size() > 0) {
    j.start_array("sCellToAddModList");
    for (const auto& e1 : scell_to_add_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (scell_to_release_list.size() > 0) {
    j.start_array("sCellToReleaseList");
    for (const auto& e1 : scell_to_release_list) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (report_ul_tx_direct_current_present) {
    j.write_str("reportUplinkTxDirectCurrent", "true");
  }
  if (bap_address_r16_present) {
    j.write_fieldname("bap-Address-r16");
    bap_address_r16.to_json(j);
  }
  if (bh_rlc_ch_to_add_mod_list_r16.is_present()) {
    j.start_array("bh-RLC-ChannelToAddModList-r16");
    for (const auto& e1 : *bh_rlc_ch_to_add_mod_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (bh_rlc_ch_to_release_list_r16.is_present()) {
    j.start_array("bh-RLC-ChannelToReleaseList-r16");
    for (const auto& e1 : *bh_rlc_ch_to_release_list_r16) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (f1c_transfer_path_r16_present) {
    j.write_str("f1c-TransferPath-r16", f1c_transfer_path_r16.to_string());
  }
  if (simul_tci_upd_list1_r16.is_present()) {
    j.start_array("simultaneousTCI-UpdateList1-r16");
    for (const auto& e1 : *simul_tci_upd_list1_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (simul_tci_upd_list2_r16.is_present()) {
    j.start_array("simultaneousTCI-UpdateList2-r16");
    for (const auto& e1 : *simul_tci_upd_list2_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (simul_spatial_upd_list1_r16.is_present()) {
    j.start_array("simultaneousSpatial-UpdatedList1-r16");
    for (const auto& e1 : *simul_spatial_upd_list1_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (simul_spatial_upd_list2_r16.is_present()) {
    j.start_array("simultaneousSpatial-UpdatedList2-r16");
    for (const auto& e1 : *simul_spatial_upd_list2_r16) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (ul_tx_switching_option_r16_present) {
    j.write_str("uplinkTxSwitchingOption-r16", ul_tx_switching_option_r16.to_string());
  }
  if (ul_tx_switching_pwr_boosting_r16_present) {
    j.write_str("uplinkTxSwitchingPowerBoosting-r16", "enabled");
  }
  if (report_ul_tx_direct_current_two_carrier_r16_present) {
    j.write_str("reportUplinkTxDirectCurrentTwoCarrier-r16", "true");
  }
  if (f1c_transfer_path_nrdc_r17_present) {
    j.write_str("f1c-TransferPathNRDC-r17", f1c_transfer_path_nrdc_r17.to_string());
  }
  if (ul_tx_switching_2_t_mode_r17_present) {
    j.write_str("uplinkTxSwitching-2T-Mode-r17", "enabled");
  }
  if (ul_tx_switching_dual_ul_tx_state_r17_present) {
    j.write_str("uplinkTxSwitching-DualUL-TxState-r17", ul_tx_switching_dual_ul_tx_state_r17.to_string());
  }
  if (uu_relay_rlc_ch_to_add_mod_list_r17.is_present()) {
    j.start_array("uu-RelayRLC-ChannelToAddModList-r17");
    for (const auto& e1 : *uu_relay_rlc_ch_to_add_mod_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (uu_relay_rlc_ch_to_release_list_r17.is_present()) {
    j.start_array("uu-RelayRLC-ChannelToReleaseList-r17");
    for (const auto& e1 : *uu_relay_rlc_ch_to_release_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (simul_u_tci_upd_list1_r17.is_present()) {
    j.start_array("simultaneousU-TCI-UpdateList1-r17");
    for (const auto& e1 : *simul_u_tci_upd_list1_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (simul_u_tci_upd_list2_r17.is_present()) {
    j.start_array("simultaneousU-TCI-UpdateList2-r17");
    for (const auto& e1 : *simul_u_tci_upd_list2_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (simul_u_tci_upd_list3_r17.is_present()) {
    j.start_array("simultaneousU-TCI-UpdateList3-r17");
    for (const auto& e1 : *simul_u_tci_upd_list3_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (simul_u_tci_upd_list4_r17.is_present()) {
    j.start_array("simultaneousU-TCI-UpdateList4-r17");
    for (const auto& e1 : *simul_u_tci_upd_list4_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (rlc_bearer_to_release_list_ext_r17.is_present()) {
    j.start_array("rlc-BearerToReleaseListExt-r17");
    for (const auto& e1 : *rlc_bearer_to_release_list_ext_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (iab_res_cfg_to_add_mod_list_r17.is_present()) {
    j.start_array("iab-ResourceConfigToAddModList-r17");
    for (const auto& e1 : *iab_res_cfg_to_add_mod_list_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (iab_res_cfg_to_release_list_r17.is_present()) {
    j.start_array("iab-ResourceConfigToReleaseList-r17");
    for (const auto& e1 : *iab_res_cfg_to_release_list_r17) {
      j.write_int(e1);
    }
    j.end_array();
  }
  if (report_ul_tx_direct_current_more_carrier_r17.is_present()) {
    j.start_array("reportUplinkTxDirectCurrentMoreCarrier-r17");
    for (const auto& e1 : *report_ul_tx_direct_current_more_carrier_r17) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (prio_scell_prach_over_sp_periodic_srs_r17_present) {
    j.write_str("prioSCellPRACH-OverSP-PeriodicSRS-r17", "enabled");
  }
  if (ncr_fwd_cfg_r18.is_present()) {
    j.write_fieldname("ncr-FwdConfig-r18");
    ncr_fwd_cfg_r18->to_json(j);
  }
  if (autonomous_denial_params_r18.is_present()) {
    j.write_fieldname("autonomousDenialParameters-r18");
    autonomous_denial_params_r18->to_json(j);
  }
  if (non_collocated_type_mrdc_r18_present) {
    j.write_str("nonCollocatedTypeMRDC-r18", "true");
  }
  if (non_collocated_type_nr_ca_r18_present) {
    j.write_str("nonCollocatedTypeNR-CA-r18", "true");
  }
  if (ul_tx_switching_more_bands_r18.is_present()) {
    j.write_fieldname("uplinkTxSwitchingMoreBands-r18");
    ul_tx_switching_more_bands_r18->to_json(j);
  }
  j.end_obj();
}

const char* cell_group_cfg_s::f1c_transfer_path_r16_opts::to_string() const
{
  static const char* names[] = {"lte", "nr", "both"};
  return convert_enum_idx(names, 3, value, "cell_group_cfg_s::f1c_transfer_path_r16_e_");
}

const char* cell_group_cfg_s::ul_tx_switching_option_r16_opts::to_string() const
{
  static const char* names[] = {"switchedUL", "dualUL"};
  return convert_enum_idx(names, 2, value, "cell_group_cfg_s::ul_tx_switching_option_r16_e_");
}

const char* cell_group_cfg_s::f1c_transfer_path_nrdc_r17_opts::to_string() const
{
  static const char* names[] = {"mcg", "scg", "both"};
  return convert_enum_idx(names, 3, value, "cell_group_cfg_s::f1c_transfer_path_nrdc_r17_e_");
}

const char* cell_group_cfg_s::ul_tx_switching_dual_ul_tx_state_r17_opts::to_string() const
{
  static const char* names[] = {"oneT", "twoT"};
  return convert_enum_idx(names, 2, value, "cell_group_cfg_s::ul_tx_switching_dual_ul_tx_state_r17_e_");
}
uint8_t cell_group_cfg_s::ul_tx_switching_dual_ul_tx_state_r17_opts::to_number() const
{
  static const uint8_t numbers[] = {1, 2};
  return map_enum_number(numbers, 2, value, "cell_group_cfg_s::ul_tx_switching_dual_ul_tx_state_r17_e_");
}
