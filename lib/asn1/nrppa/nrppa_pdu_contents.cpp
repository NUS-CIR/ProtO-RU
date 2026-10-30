// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/nrppa/nrppa_pdu_contents.h"

using namespace asn1;
using namespace asn1::nrppa;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// AperiodicSRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t aperiodic_srs_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {122};
  return map_enum_number(names, 1, idx, "id");
}
bool aperiodic_srs_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 122 == id;
}
crit_e aperiodic_srs_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 122) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
aperiodic_srs_ext_ies_o::ext_c aperiodic_srs_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  if (id != 122) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e aperiodic_srs_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 122) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Extension ::= OPEN TYPE
void aperiodic_srs_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("PosSRSResourceSet-Aggregation-List");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}
OCUDUASN_CODE aperiodic_srs_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 32, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aperiodic_srs_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 32, true));
  return OCUDUASN_SUCCESS;
}

const char* aperiodic_srs_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {"PosSRSResourceSet-Aggregation-List"};
  return convert_enum_idx(names, 1, value, "aperiodic_srs_ext_ies_o::ext_c::types");
}

// AperiodicSRS ::= SEQUENCE
OCUDUASN_CODE aperiodic_srs_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(srs_res_trigger_present, 1));
  HANDLE_CODE(bref.pack(ie_exts.size() > 0, 1));

  HANDLE_CODE(aperiodic.pack(bref));
  if (srs_res_trigger_present) {
    HANDLE_CODE(srs_res_trigger.pack(bref));
  }
  if (ie_exts.size() > 0) {
    HANDLE_CODE(pack_dyn_seq_of(bref, ie_exts, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aperiodic_srs_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(srs_res_trigger_present, 1));
  bool ie_exts_present;
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(aperiodic.unpack(bref));
  if (srs_res_trigger_present) {
    HANDLE_CODE(srs_res_trigger.unpack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(unpack_dyn_seq_of(ie_exts, bref, 1, 65535, true));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void aperiodic_srs_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("aperiodic", "true");
  if (srs_res_trigger_present) {
    j.write_fieldname("sRSResourceTrigger");
    srs_res_trigger.to_json(j);
  }
  if (ie_exts.size() > 0) {
    j.write_fieldname("iE-Extensions");
  }
  j.end_obj();
}

const char* aperiodic_srs_s::aperiodic_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "aperiodic_srs_s::aperiodic_e_");
}

// AssistanceInformationControl-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t assist_info_ctrl_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {23, 24, 38};
  return map_enum_number(names, 3, idx, "id");
}
bool assist_info_ctrl_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {23, 24, 38};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e assist_info_ctrl_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 23:
      return crit_e::reject;
    case 24:
      return crit_e::reject;
    case 38:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
assist_info_ctrl_ies_o::value_c assist_info_ctrl_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 23:
      ret.set(value_c::types::assist_info);
      break;
    case 24:
      ret.set(value_c::types::broadcast);
      break;
    case 38:
      ret.set(value_c::types::positioning_broadcast_cells);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e assist_info_ctrl_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 23:
      return presence_e::optional;
    case 24:
      return presence_e::optional;
    case 38:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void assist_info_ctrl_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::assist_info:
      c = assist_info_s{};
      break;
    case types::broadcast:
      c = broadcast_e{};
      break;
    case types::positioning_broadcast_cells:
      c = positioning_broadcast_cells_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "assist_info_ctrl_ies_o::value_c");
  }
}
assist_info_s& assist_info_ctrl_ies_o::value_c::assist_info()
{
  assert_choice_type(types::assist_info, type_, "Value");
  return c.get<assist_info_s>();
}
broadcast_e& assist_info_ctrl_ies_o::value_c::broadcast()
{
  assert_choice_type(types::broadcast, type_, "Value");
  return c.get<broadcast_e>();
}
positioning_broadcast_cells_l& assist_info_ctrl_ies_o::value_c::positioning_broadcast_cells()
{
  assert_choice_type(types::positioning_broadcast_cells, type_, "Value");
  return c.get<positioning_broadcast_cells_l>();
}
const assist_info_s& assist_info_ctrl_ies_o::value_c::assist_info() const
{
  assert_choice_type(types::assist_info, type_, "Value");
  return c.get<assist_info_s>();
}
const broadcast_e& assist_info_ctrl_ies_o::value_c::broadcast() const
{
  assert_choice_type(types::broadcast, type_, "Value");
  return c.get<broadcast_e>();
}
const positioning_broadcast_cells_l& assist_info_ctrl_ies_o::value_c::positioning_broadcast_cells() const
{
  assert_choice_type(types::positioning_broadcast_cells, type_, "Value");
  return c.get<positioning_broadcast_cells_l>();
}
void assist_info_ctrl_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::assist_info:
      j.write_fieldname("Assistance-Information");
      c.get<assist_info_s>().to_json(j);
      break;
    case types::broadcast:
      j.write_str("Broadcast", c.get<broadcast_e>().to_string());
      break;
    case types::positioning_broadcast_cells:
      j.start_array("PositioningBroadcastCells");
      for (const auto& e1 : c.get<positioning_broadcast_cells_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "assist_info_ctrl_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE assist_info_ctrl_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::assist_info:
      HANDLE_CODE(c.get<assist_info_s>().pack(bref));
      break;
    case types::broadcast:
      HANDLE_CODE(c.get<broadcast_e>().pack(bref));
      break;
    case types::positioning_broadcast_cells:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<positioning_broadcast_cells_l>(), 1, 16384, true));
      break;
    default:
      log_invalid_choice_id(type_, "assist_info_ctrl_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_ctrl_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::assist_info:
      HANDLE_CODE(c.get<assist_info_s>().unpack(bref));
      break;
    case types::broadcast:
      HANDLE_CODE(c.get<broadcast_e>().unpack(bref));
      break;
    case types::positioning_broadcast_cells:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<positioning_broadcast_cells_l>(), bref, 1, 16384, true));
      break;
    default:
      log_invalid_choice_id(type_, "assist_info_ctrl_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* assist_info_ctrl_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"Assistance-Information", "Broadcast", "PositioningBroadcastCells"};
  return convert_enum_idx(names, 3, value, "assist_info_ctrl_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<assist_info_ctrl_ies_o>;

OCUDUASN_CODE assist_info_ctrl_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += assist_info_present ? 1 : 0;
  nof_ies += broadcast_present ? 1 : 0;
  nof_ies += positioning_broadcast_cells_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (assist_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)23, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(assist_info.pack(bref));
  }
  if (broadcast_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)24, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(broadcast.pack(bref));
  }
  if (positioning_broadcast_cells_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)38, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, positioning_broadcast_cells, 1, 16384, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_ctrl_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != assist_info_ctrl_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 23: {
        assist_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(assist_info.unpack(bref));
        break;
      }
      case 24: {
        broadcast_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(broadcast.unpack(bref));
        break;
      }
      case 38: {
        positioning_broadcast_cells_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(positioning_broadcast_cells, bref, 1, 16384, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void assist_info_ctrl_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (assist_info_present) {
    j.write_int("id", 23);
    j.write_str("criticality", "reject");
    assist_info.to_json(j);
  }
  if (broadcast_present) {
    j.write_int("id", 24);
    j.write_str("criticality", "reject");
    j.write_str("Value", broadcast.to_string());
  }
  if (positioning_broadcast_cells_present) {
    j.write_int("id", 38);
    j.write_str("criticality", "reject");
    j.start_array("Value");
    for (const auto& e1 : positioning_broadcast_cells) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// AssistanceInformationFeedback-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t assist_info_feedback_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {25, 38, 1};
  return map_enum_number(names, 3, idx, "id");
}
bool assist_info_feedback_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {25, 38, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e assist_info_feedback_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 25:
      return crit_e::reject;
    case 38:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
assist_info_feedback_ies_o::value_c assist_info_feedback_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 25:
      ret.set(value_c::types::assist_info_fail_list);
      break;
    case 38:
      ret.set(value_c::types::positioning_broadcast_cells);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e assist_info_feedback_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 25:
      return presence_e::optional;
    case 38:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void assist_info_feedback_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::assist_info_fail_list:
      c = assist_info_fail_list_l{};
      break;
    case types::positioning_broadcast_cells:
      c = positioning_broadcast_cells_l{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "assist_info_feedback_ies_o::value_c");
  }
}
assist_info_fail_list_l& assist_info_feedback_ies_o::value_c::assist_info_fail_list()
{
  assert_choice_type(types::assist_info_fail_list, type_, "Value");
  return c.get<assist_info_fail_list_l>();
}
positioning_broadcast_cells_l& assist_info_feedback_ies_o::value_c::positioning_broadcast_cells()
{
  assert_choice_type(types::positioning_broadcast_cells, type_, "Value");
  return c.get<positioning_broadcast_cells_l>();
}
crit_diagnostics_s& assist_info_feedback_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const assist_info_fail_list_l& assist_info_feedback_ies_o::value_c::assist_info_fail_list() const
{
  assert_choice_type(types::assist_info_fail_list, type_, "Value");
  return c.get<assist_info_fail_list_l>();
}
const positioning_broadcast_cells_l& assist_info_feedback_ies_o::value_c::positioning_broadcast_cells() const
{
  assert_choice_type(types::positioning_broadcast_cells, type_, "Value");
  return c.get<positioning_broadcast_cells_l>();
}
const crit_diagnostics_s& assist_info_feedback_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void assist_info_feedback_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::assist_info_fail_list:
      j.start_array("AssistanceInformationFailureList");
      for (const auto& e1 : c.get<assist_info_fail_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::positioning_broadcast_cells:
      j.start_array("PositioningBroadcastCells");
      for (const auto& e1 : c.get<positioning_broadcast_cells_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "assist_info_feedback_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE assist_info_feedback_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::assist_info_fail_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<assist_info_fail_list_l>(), 1, 32, true));
      break;
    case types::positioning_broadcast_cells:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<positioning_broadcast_cells_l>(), 1, 16384, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "assist_info_feedback_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_feedback_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::assist_info_fail_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<assist_info_fail_list_l>(), bref, 1, 32, true));
      break;
    case types::positioning_broadcast_cells:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<positioning_broadcast_cells_l>(), bref, 1, 16384, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "assist_info_feedback_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* assist_info_feedback_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "AssistanceInformationFailureList", "PositioningBroadcastCells", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 3, value, "assist_info_feedback_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<assist_info_feedback_ies_o>;

OCUDUASN_CODE assist_info_feedback_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += assist_info_fail_list_present ? 1 : 0;
  nof_ies += positioning_broadcast_cells_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (assist_info_fail_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)25, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, assist_info_fail_list, 1, 32, true));
  }
  if (positioning_broadcast_cells_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)38, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, positioning_broadcast_cells, 1, 16384, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_feedback_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != assist_info_feedback_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 25: {
        assist_info_fail_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(assist_info_fail_list, bref, 1, 32, true));
        break;
      }
      case 38: {
        positioning_broadcast_cells_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(positioning_broadcast_cells, bref, 1, 16384, true));
        break;
      }
      case 1: {
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
void assist_info_feedback_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (assist_info_fail_list_present) {
    j.write_int("id", 25);
    j.write_str("criticality", "reject");
    j.start_array("Value");
    for (const auto& e1 : assist_info_fail_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (positioning_broadcast_cells_present) {
    j.write_int("id", 38);
    j.write_str("criticality", "reject");
    j.start_array("Value");
    for (const auto& e1 : positioning_broadcast_cells) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// E-CIDMeasurementFailureIndication-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t e_c_id_meas_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 6, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool e_c_id_meas_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 6, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_c_id_meas_fail_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 6:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e_c_id_meas_fail_ind_ies_o::value_c e_c_id_meas_fail_ind_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 2:
      ret.set(value_c::types::lmf_ue_meas_id);
      break;
    case 6:
      ret.set(value_c::types::ran_ue_meas_id);
      break;
    case 0:
      ret.set(value_c::types::cause);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e_c_id_meas_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 6:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e_c_id_meas_fail_ind_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_ue_meas_id:
      c = uint8_t{};
      break;
    case types::ran_ue_meas_id:
      c = uint8_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_fail_ind_ies_o::value_c");
  }
}
uint8_t& e_c_id_meas_fail_ind_ies_o::value_c::lmf_ue_meas_id()
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
uint8_t& e_c_id_meas_fail_ind_ies_o::value_c::ran_ue_meas_id()
{
  assert_choice_type(types::ran_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
cause_c& e_c_id_meas_fail_ind_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const uint8_t& e_c_id_meas_fail_ind_ies_o::value_c::lmf_ue_meas_id() const
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const uint8_t& e_c_id_meas_fail_ind_ies_o::value_c::ran_ue_meas_id() const
{
  assert_choice_type(types::ran_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const cause_c& e_c_id_meas_fail_ind_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
void e_c_id_meas_fail_ind_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::ran_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_fail_ind_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE e_c_id_meas_fail_ind_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::ran_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_fail_ind_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_fail_ind_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::ran_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_fail_ind_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e_c_id_meas_fail_ind_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..15,...)", "INTEGER (1..15,...)", "Cause"};
  return convert_enum_idx(names, 3, value, "e_c_id_meas_fail_ind_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e_c_id_meas_fail_ind_ies_o>;

OCUDUASN_CODE e_c_id_meas_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)6, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_fail_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 0: {
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
void e_c_id_meas_fail_ind_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_ue_meas_id);
  j.write_int("id", 6);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_ue_meas_id);
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  j.end_obj();
}

// E-CIDMeasurementInitiationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t e_c_id_meas_initiation_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 0, 1};
  return map_enum_number(names, 3, idx, "id");
}
bool e_c_id_meas_initiation_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_c_id_meas_initiation_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e_c_id_meas_initiation_fail_ies_o::value_c e_c_id_meas_initiation_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 2:
      ret.set(value_c::types::lmf_ue_meas_id);
      break;
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e_c_id_meas_initiation_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e_c_id_meas_initiation_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_ue_meas_id:
      c = uint8_t{};
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
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_fail_ies_o::value_c");
  }
}
uint8_t& e_c_id_meas_initiation_fail_ies_o::value_c::lmf_ue_meas_id()
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
cause_c& e_c_id_meas_initiation_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& e_c_id_meas_initiation_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint8_t& e_c_id_meas_initiation_fail_ies_o::value_c::lmf_ue_meas_id() const
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const cause_c& e_c_id_meas_initiation_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& e_c_id_meas_initiation_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void e_c_id_meas_initiation_fail_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_fail_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE e_c_id_meas_initiation_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_initiation_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e_c_id_meas_initiation_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..15,...)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 3, value, "e_c_id_meas_initiation_fail_ies_o::value_c::types");
}
uint8_t e_c_id_meas_initiation_fail_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "e_c_id_meas_initiation_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e_c_id_meas_initiation_fail_ies_o>;

OCUDUASN_CODE e_c_id_meas_initiation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_initiation_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_initiation_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
void e_c_id_meas_initiation_fail_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_ue_meas_id);
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// E-CIDMeasurementInitiationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t e_c_id_meas_initiation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 3, 4, 5, 15, 19, 105};
  return map_enum_number(names, 7, idx, "id");
}
bool e_c_id_meas_initiation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 3, 4, 5, 15, 19, 105};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_c_id_meas_initiation_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 4:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    case 105:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e_c_id_meas_initiation_request_ies_o::value_c e_c_id_meas_initiation_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 2:
      ret.set(value_c::types::lmf_ue_meas_id);
      break;
    case 3:
      ret.set(value_c::types::report_characteristics);
      break;
    case 4:
      ret.set(value_c::types::meas_periodicity);
      break;
    case 5:
      ret.set(value_c::types::meas_quantities);
      break;
    case 15:
      ret.set(value_c::types::other_rat_meas_quantities);
      break;
    case 19:
      ret.set(value_c::types::wlan_meas_quantities);
      break;
    case 105:
      ret.set(value_c::types::meas_periodicity_nr_ao_a);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e_c_id_meas_initiation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 4:
      return presence_e::conditional;
    case 5:
      return presence_e::mandatory;
    case 15:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    case 105:
      return presence_e::conditional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e_c_id_meas_initiation_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_ue_meas_id:
      c = uint8_t{};
      break;
    case types::report_characteristics:
      c = report_characteristics_e{};
      break;
    case types::meas_periodicity:
      c = meas_periodicity_e{};
      break;
    case types::meas_quantities:
      c = meas_quantities_l{};
      break;
    case types::other_rat_meas_quantities:
      c = other_rat_meas_quantities_l{};
      break;
    case types::wlan_meas_quantities:
      c = wlan_meas_quantities_l{};
      break;
    case types::meas_periodicity_nr_ao_a:
      c = meas_periodicity_nr_ao_a_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_request_ies_o::value_c");
  }
}
uint8_t& e_c_id_meas_initiation_request_ies_o::value_c::lmf_ue_meas_id()
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
report_characteristics_e& e_c_id_meas_initiation_request_ies_o::value_c::report_characteristics()
{
  assert_choice_type(types::report_characteristics, type_, "Value");
  return c.get<report_characteristics_e>();
}
meas_periodicity_e& e_c_id_meas_initiation_request_ies_o::value_c::meas_periodicity()
{
  assert_choice_type(types::meas_periodicity, type_, "Value");
  return c.get<meas_periodicity_e>();
}
meas_quantities_l& e_c_id_meas_initiation_request_ies_o::value_c::meas_quantities()
{
  assert_choice_type(types::meas_quantities, type_, "Value");
  return c.get<meas_quantities_l>();
}
other_rat_meas_quantities_l& e_c_id_meas_initiation_request_ies_o::value_c::other_rat_meas_quantities()
{
  assert_choice_type(types::other_rat_meas_quantities, type_, "Value");
  return c.get<other_rat_meas_quantities_l>();
}
wlan_meas_quantities_l& e_c_id_meas_initiation_request_ies_o::value_c::wlan_meas_quantities()
{
  assert_choice_type(types::wlan_meas_quantities, type_, "Value");
  return c.get<wlan_meas_quantities_l>();
}
meas_periodicity_nr_ao_a_e& e_c_id_meas_initiation_request_ies_o::value_c::meas_periodicity_nr_ao_a()
{
  assert_choice_type(types::meas_periodicity_nr_ao_a, type_, "Value");
  return c.get<meas_periodicity_nr_ao_a_e>();
}
const uint8_t& e_c_id_meas_initiation_request_ies_o::value_c::lmf_ue_meas_id() const
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const report_characteristics_e& e_c_id_meas_initiation_request_ies_o::value_c::report_characteristics() const
{
  assert_choice_type(types::report_characteristics, type_, "Value");
  return c.get<report_characteristics_e>();
}
const meas_periodicity_e& e_c_id_meas_initiation_request_ies_o::value_c::meas_periodicity() const
{
  assert_choice_type(types::meas_periodicity, type_, "Value");
  return c.get<meas_periodicity_e>();
}
const meas_quantities_l& e_c_id_meas_initiation_request_ies_o::value_c::meas_quantities() const
{
  assert_choice_type(types::meas_quantities, type_, "Value");
  return c.get<meas_quantities_l>();
}
const other_rat_meas_quantities_l& e_c_id_meas_initiation_request_ies_o::value_c::other_rat_meas_quantities() const
{
  assert_choice_type(types::other_rat_meas_quantities, type_, "Value");
  return c.get<other_rat_meas_quantities_l>();
}
const wlan_meas_quantities_l& e_c_id_meas_initiation_request_ies_o::value_c::wlan_meas_quantities() const
{
  assert_choice_type(types::wlan_meas_quantities, type_, "Value");
  return c.get<wlan_meas_quantities_l>();
}
const meas_periodicity_nr_ao_a_e& e_c_id_meas_initiation_request_ies_o::value_c::meas_periodicity_nr_ao_a() const
{
  assert_choice_type(types::meas_periodicity_nr_ao_a, type_, "Value");
  return c.get<meas_periodicity_nr_ao_a_e>();
}
void e_c_id_meas_initiation_request_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::report_characteristics:
      j.write_str("ReportCharacteristics", c.get<report_characteristics_e>().to_string());
      break;
    case types::meas_periodicity:
      j.write_str("MeasurementPeriodicity", c.get<meas_periodicity_e>().to_string());
      break;
    case types::meas_quantities:
      j.start_array("MeasurementQuantities");
      for (const auto& e1 : c.get<meas_quantities_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::other_rat_meas_quantities:
      j.start_array("OtherRATMeasurementQuantities");
      for (const auto& e1 : c.get<other_rat_meas_quantities_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::wlan_meas_quantities:
      j.start_array("WLANMeasurementQuantities");
      for (const auto& e1 : c.get<wlan_meas_quantities_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::meas_periodicity_nr_ao_a:
      j.write_str("MeasurementPeriodicityNR-AoA", c.get<meas_periodicity_nr_ao_a_e>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_request_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE e_c_id_meas_initiation_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::report_characteristics:
      HANDLE_CODE(c.get<report_characteristics_e>().pack(bref));
      break;
    case types::meas_periodicity:
      HANDLE_CODE(c.get<meas_periodicity_e>().pack(bref));
      break;
    case types::meas_quantities:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<meas_quantities_l>(), 1, 64, true));
      break;
    case types::other_rat_meas_quantities:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<other_rat_meas_quantities_l>(), 0, 64, true));
      break;
    case types::wlan_meas_quantities:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<wlan_meas_quantities_l>(), 0, 64, true));
      break;
    case types::meas_periodicity_nr_ao_a:
      HANDLE_CODE(c.get<meas_periodicity_nr_ao_a_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_initiation_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::report_characteristics:
      HANDLE_CODE(c.get<report_characteristics_e>().unpack(bref));
      break;
    case types::meas_periodicity:
      HANDLE_CODE(c.get<meas_periodicity_e>().unpack(bref));
      break;
    case types::meas_quantities:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<meas_quantities_l>(), bref, 1, 64, true));
      break;
    case types::other_rat_meas_quantities:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<other_rat_meas_quantities_l>(), bref, 0, 64, true));
      break;
    case types::wlan_meas_quantities:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<wlan_meas_quantities_l>(), bref, 0, 64, true));
      break;
    case types::meas_periodicity_nr_ao_a:
      HANDLE_CODE(c.get<meas_periodicity_nr_ao_a_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e_c_id_meas_initiation_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..15,...)",
                                "ReportCharacteristics",
                                "MeasurementPeriodicity",
                                "MeasurementQuantities",
                                "OtherRATMeasurementQuantities",
                                "WLANMeasurementQuantities",
                                "MeasurementPeriodicityNR-AoA"};
  return convert_enum_idx(names, 7, value, "e_c_id_meas_initiation_request_ies_o::value_c::types");
}
uint8_t e_c_id_meas_initiation_request_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "e_c_id_meas_initiation_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e_c_id_meas_initiation_request_ies_o>;

OCUDUASN_CODE e_c_id_meas_initiation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += meas_periodicity_present ? 1 : 0;
  nof_ies += other_rat_meas_quantities_present ? 1 : 0;
  nof_ies += wlan_meas_quantities_present ? 1 : 0;
  nof_ies += meas_periodicity_nr_ao_a_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)3, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(report_characteristics.pack(bref));
  }
  if (meas_periodicity_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)4, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_periodicity.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, meas_quantities, 1, 64, true));
  }
  if (other_rat_meas_quantities_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)15, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, other_rat_meas_quantities, 0, 64, true));
  }
  if (wlan_meas_quantities_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)19, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, wlan_meas_quantities, 0, 64, true));
  }
  if (meas_periodicity_nr_ao_a_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_periodicity_nr_ao_a.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_initiation_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_initiation_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(report_characteristics.unpack(bref));
        break;
      }
      case 4: {
        meas_periodicity_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_periodicity.unpack(bref));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(meas_quantities, bref, 1, 64, true));
        break;
      }
      case 15: {
        other_rat_meas_quantities_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(other_rat_meas_quantities, bref, 0, 64, true));
        break;
      }
      case 19: {
        wlan_meas_quantities_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(wlan_meas_quantities, bref, 0, 64, true));
        break;
      }
      case 105: {
        meas_periodicity_nr_ao_a_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_periodicity_nr_ao_a.unpack(bref));
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
void e_c_id_meas_initiation_request_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_ue_meas_id);
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_str("Value", report_characteristics.to_string());
  if (meas_periodicity_present) {
    j.write_int("id", 4);
    j.write_str("criticality", "reject");
    j.write_str("Value", meas_periodicity.to_string());
  }
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.start_array("Value");
  for (const auto& e1 : meas_quantities) {
    e1.to_json(j);
  }
  j.end_array();
  if (other_rat_meas_quantities_present) {
    j.write_int("id", 15);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : other_rat_meas_quantities) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (wlan_meas_quantities_present) {
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : wlan_meas_quantities) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (meas_periodicity_nr_ao_a_present) {
    j.write_int("id", 105);
    j.write_str("criticality", "reject");
    j.write_str("Value", meas_periodicity_nr_ao_a.to_string());
  }
  j.end_obj();
}

// E-CIDMeasurementInitiationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t e_c_id_meas_initiation_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 6, 7, 1, 14, 17, 21};
  return map_enum_number(names, 7, idx, "id");
}
bool e_c_id_meas_initiation_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 6, 7, 1, 14, 17, 21};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_c_id_meas_initiation_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 6:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    case 14:
      return crit_e::ignore;
    case 17:
      return crit_e::ignore;
    case 21:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e_c_id_meas_initiation_resp_ies_o::value_c e_c_id_meas_initiation_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 2:
      ret.set(value_c::types::lmf_ue_meas_id);
      break;
    case 6:
      ret.set(value_c::types::ran_ue_meas_id);
      break;
    case 7:
      ret.set(value_c::types::e_c_id_meas_result);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    case 14:
      ret.set(value_c::types::cell_portion_id);
      break;
    case 17:
      ret.set(value_c::types::other_rat_meas_result);
      break;
    case 21:
      ret.set(value_c::types::wlan_meas_result);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e_c_id_meas_initiation_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 6:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    case 14:
      return presence_e::optional;
    case 17:
      return presence_e::optional;
    case 21:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e_c_id_meas_initiation_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_ue_meas_id:
      c = uint8_t{};
      break;
    case types::ran_ue_meas_id:
      c = uint8_t{};
      break;
    case types::e_c_id_meas_result:
      c = e_c_id_meas_result_s{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::cell_portion_id:
      c = uint16_t{};
      break;
    case types::other_rat_meas_result:
      c = other_rat_meas_result_l{};
      break;
    case types::wlan_meas_result:
      c = wlan_meas_result_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_resp_ies_o::value_c");
  }
}
uint8_t& e_c_id_meas_initiation_resp_ies_o::value_c::lmf_ue_meas_id()
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
uint8_t& e_c_id_meas_initiation_resp_ies_o::value_c::ran_ue_meas_id()
{
  assert_choice_type(types::ran_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
e_c_id_meas_result_s& e_c_id_meas_initiation_resp_ies_o::value_c::e_c_id_meas_result()
{
  assert_choice_type(types::e_c_id_meas_result, type_, "Value");
  return c.get<e_c_id_meas_result_s>();
}
crit_diagnostics_s& e_c_id_meas_initiation_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
uint16_t& e_c_id_meas_initiation_resp_ies_o::value_c::cell_portion_id()
{
  assert_choice_type(types::cell_portion_id, type_, "Value");
  return c.get<uint16_t>();
}
other_rat_meas_result_l& e_c_id_meas_initiation_resp_ies_o::value_c::other_rat_meas_result()
{
  assert_choice_type(types::other_rat_meas_result, type_, "Value");
  return c.get<other_rat_meas_result_l>();
}
wlan_meas_result_l& e_c_id_meas_initiation_resp_ies_o::value_c::wlan_meas_result()
{
  assert_choice_type(types::wlan_meas_result, type_, "Value");
  return c.get<wlan_meas_result_l>();
}
const uint8_t& e_c_id_meas_initiation_resp_ies_o::value_c::lmf_ue_meas_id() const
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const uint8_t& e_c_id_meas_initiation_resp_ies_o::value_c::ran_ue_meas_id() const
{
  assert_choice_type(types::ran_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const e_c_id_meas_result_s& e_c_id_meas_initiation_resp_ies_o::value_c::e_c_id_meas_result() const
{
  assert_choice_type(types::e_c_id_meas_result, type_, "Value");
  return c.get<e_c_id_meas_result_s>();
}
const crit_diagnostics_s& e_c_id_meas_initiation_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& e_c_id_meas_initiation_resp_ies_o::value_c::cell_portion_id() const
{
  assert_choice_type(types::cell_portion_id, type_, "Value");
  return c.get<uint16_t>();
}
const other_rat_meas_result_l& e_c_id_meas_initiation_resp_ies_o::value_c::other_rat_meas_result() const
{
  assert_choice_type(types::other_rat_meas_result, type_, "Value");
  return c.get<other_rat_meas_result_l>();
}
const wlan_meas_result_l& e_c_id_meas_initiation_resp_ies_o::value_c::wlan_meas_result() const
{
  assert_choice_type(types::wlan_meas_result, type_, "Value");
  return c.get<wlan_meas_result_l>();
}
void e_c_id_meas_initiation_resp_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::ran_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::e_c_id_meas_result:
      j.write_fieldname("E-CID-MeasurementResult");
      c.get<e_c_id_meas_result_s>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    case types::cell_portion_id:
      j.write_int("INTEGER (0..4095,...)", c.get<uint16_t>());
      break;
    case types::other_rat_meas_result:
      j.start_array("OtherRATMeasurementResult");
      for (const auto& e1 : c.get<other_rat_meas_result_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::wlan_meas_result:
      j.start_array("WLANMeasurementResult");
      for (const auto& e1 : c.get<wlan_meas_result_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_resp_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE e_c_id_meas_initiation_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::ran_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::e_c_id_meas_result:
      HANDLE_CODE(c.get<e_c_id_meas_result_s>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    case types::cell_portion_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    case types::other_rat_meas_result:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<other_rat_meas_result_l>(), 1, 64, true));
      break;
    case types::wlan_meas_result:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<wlan_meas_result_l>(), 1, 64, true));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_initiation_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::ran_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::e_c_id_meas_result:
      HANDLE_CODE(c.get<e_c_id_meas_result_s>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    case types::cell_portion_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    case types::other_rat_meas_result:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<other_rat_meas_result_l>(), bref, 1, 64, true));
      break;
    case types::wlan_meas_result:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<wlan_meas_result_l>(), bref, 1, 64, true));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_initiation_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e_c_id_meas_initiation_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..15,...)",
                                "INTEGER (1..15,...)",
                                "E-CID-MeasurementResult",
                                "CriticalityDiagnostics",
                                "INTEGER (0..4095,...)",
                                "OtherRATMeasurementResult",
                                "WLANMeasurementResult"};
  return convert_enum_idx(names, 7, value, "e_c_id_meas_initiation_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e_c_id_meas_initiation_resp_ies_o>;

OCUDUASN_CODE e_c_id_meas_initiation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += e_c_id_meas_result_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += cell_portion_id_present ? 1 : 0;
  nof_ies += other_rat_meas_result_present ? 1 : 0;
  nof_ies += wlan_meas_result_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)6, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  if (e_c_id_meas_result_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(e_c_id_meas_result.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (cell_portion_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)14, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, cell_portion_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (other_rat_meas_result_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)17, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, other_rat_meas_result, 1, 64, true));
  }
  if (wlan_meas_result_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)21, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, wlan_meas_result, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_initiation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_initiation_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 7: {
        e_c_id_meas_result_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(e_c_id_meas_result.unpack(bref));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 14: {
        cell_portion_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(cell_portion_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 17: {
        other_rat_meas_result_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(other_rat_meas_result, bref, 1, 64, true));
        break;
      }
      case 21: {
        wlan_meas_result_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(wlan_meas_result, bref, 1, 64, true));
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
void e_c_id_meas_initiation_resp_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_ue_meas_id);
  j.write_int("id", 6);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_ue_meas_id);
  if (e_c_id_meas_result_present) {
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    e_c_id_meas_result.to_json(j);
  }
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  if (cell_portion_id_present) {
    j.write_int("id", 14);
    j.write_str("criticality", "ignore");
    j.write_int("Value", cell_portion_id);
  }
  if (other_rat_meas_result_present) {
    j.write_int("id", 17);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : other_rat_meas_result) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (wlan_meas_result_present) {
    j.write_int("id", 21);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : wlan_meas_result) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// E-CIDMeasurementReport-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t e_c_id_meas_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 6, 7, 14};
  return map_enum_number(names, 4, idx, "id");
}
bool e_c_id_meas_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 6, 7, 14};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_c_id_meas_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 6:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 14:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e_c_id_meas_report_ies_o::value_c e_c_id_meas_report_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 2:
      ret.set(value_c::types::lmf_ue_meas_id);
      break;
    case 6:
      ret.set(value_c::types::ran_ue_meas_id);
      break;
    case 7:
      ret.set(value_c::types::e_c_id_meas_result);
      break;
    case 14:
      ret.set(value_c::types::cell_portion_id);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e_c_id_meas_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 6:
      return presence_e::mandatory;
    case 7:
      return presence_e::mandatory;
    case 14:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e_c_id_meas_report_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_ue_meas_id:
      c = uint8_t{};
      break;
    case types::ran_ue_meas_id:
      c = uint8_t{};
      break;
    case types::e_c_id_meas_result:
      c = e_c_id_meas_result_s{};
      break;
    case types::cell_portion_id:
      c = uint16_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_report_ies_o::value_c");
  }
}
uint8_t& e_c_id_meas_report_ies_o::value_c::lmf_ue_meas_id()
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
uint8_t& e_c_id_meas_report_ies_o::value_c::ran_ue_meas_id()
{
  assert_choice_type(types::ran_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
e_c_id_meas_result_s& e_c_id_meas_report_ies_o::value_c::e_c_id_meas_result()
{
  assert_choice_type(types::e_c_id_meas_result, type_, "Value");
  return c.get<e_c_id_meas_result_s>();
}
uint16_t& e_c_id_meas_report_ies_o::value_c::cell_portion_id()
{
  assert_choice_type(types::cell_portion_id, type_, "Value");
  return c.get<uint16_t>();
}
const uint8_t& e_c_id_meas_report_ies_o::value_c::lmf_ue_meas_id() const
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const uint8_t& e_c_id_meas_report_ies_o::value_c::ran_ue_meas_id() const
{
  assert_choice_type(types::ran_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const e_c_id_meas_result_s& e_c_id_meas_report_ies_o::value_c::e_c_id_meas_result() const
{
  assert_choice_type(types::e_c_id_meas_result, type_, "Value");
  return c.get<e_c_id_meas_result_s>();
}
const uint16_t& e_c_id_meas_report_ies_o::value_c::cell_portion_id() const
{
  assert_choice_type(types::cell_portion_id, type_, "Value");
  return c.get<uint16_t>();
}
void e_c_id_meas_report_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::ran_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::e_c_id_meas_result:
      j.write_fieldname("E-CID-MeasurementResult");
      c.get<e_c_id_meas_result_s>().to_json(j);
      break;
    case types::cell_portion_id:
      j.write_int("INTEGER (0..4095,...)", c.get<uint16_t>());
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_report_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE e_c_id_meas_report_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::ran_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::e_c_id_meas_result:
      HANDLE_CODE(c.get<e_c_id_meas_result_s>().pack(bref));
      break;
    case types::cell_portion_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_report_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_report_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::ran_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::e_c_id_meas_result:
      HANDLE_CODE(c.get<e_c_id_meas_result_s>().unpack(bref));
      break;
    case types::cell_portion_id:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)4095u, true, true));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_report_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e_c_id_meas_report_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (1..15,...)", "INTEGER (1..15,...)", "E-CID-MeasurementResult", "INTEGER (0..4095,...)"};
  return convert_enum_idx(names, 4, value, "e_c_id_meas_report_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e_c_id_meas_report_ies_o>;

OCUDUASN_CODE e_c_id_meas_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += cell_portion_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)6, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(e_c_id_meas_result.pack(bref));
  }
  if (cell_portion_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)14, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, cell_portion_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_report_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(e_c_id_meas_result.unpack(bref));
        break;
      }
      case 14: {
        cell_portion_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(cell_portion_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
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
void e_c_id_meas_report_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_ue_meas_id);
  j.write_int("id", 6);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_ue_meas_id);
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  e_c_id_meas_result.to_json(j);
  if (cell_portion_id_present) {
    j.write_int("id", 14);
    j.write_str("criticality", "ignore");
    j.write_int("Value", cell_portion_id);
  }
  j.end_obj();
}

// E-CIDMeasurementTerminationCommand-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t e_c_id_meas_termination_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 6};
  return map_enum_number(names, 2, idx, "id");
}
bool e_c_id_meas_termination_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 6};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e e_c_id_meas_termination_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 6:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
e_c_id_meas_termination_cmd_ies_o::value_c e_c_id_meas_termination_cmd_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 2:
      ret.set(value_c::types::lmf_ue_meas_id);
      break;
    case 6:
      ret.set(value_c::types::ran_ue_meas_id);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e e_c_id_meas_termination_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::mandatory;
    case 6:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void e_c_id_meas_termination_cmd_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_ue_meas_id:
      c = uint8_t{};
      break;
    case types::ran_ue_meas_id:
      c = uint8_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_termination_cmd_ies_o::value_c");
  }
}
uint8_t& e_c_id_meas_termination_cmd_ies_o::value_c::lmf_ue_meas_id()
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
uint8_t& e_c_id_meas_termination_cmd_ies_o::value_c::ran_ue_meas_id()
{
  assert_choice_type(types::ran_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const uint8_t& e_c_id_meas_termination_cmd_ies_o::value_c::lmf_ue_meas_id() const
{
  assert_choice_type(types::lmf_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
const uint8_t& e_c_id_meas_termination_cmd_ies_o::value_c::ran_ue_meas_id() const
{
  assert_choice_type(types::ran_ue_meas_id, type_, "Value");
  return c.get<uint8_t>();
}
void e_c_id_meas_termination_cmd_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    case types::ran_ue_meas_id:
      j.write_int("INTEGER (1..15,...)", c.get<uint8_t>());
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_termination_cmd_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE e_c_id_meas_termination_cmd_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::ran_ue_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_termination_cmd_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_termination_cmd_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    case types::ran_ue_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)1u, (uint8_t)15u, true, true));
      break;
    default:
      log_invalid_choice_id(type_, "e_c_id_meas_termination_cmd_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* e_c_id_meas_termination_cmd_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..15,...)", "INTEGER (1..15,...)"};
  return convert_enum_idx(names, 2, value, "e_c_id_meas_termination_cmd_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<e_c_id_meas_termination_cmd_ies_o>;

OCUDUASN_CODE e_c_id_meas_termination_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)2, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)6, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_termination_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_termination_cmd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true));
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
void e_c_id_meas_termination_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_ue_meas_id);
  j.write_int("id", 6);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_ue_meas_id);
  j.end_obj();
}

// ErrorIndication-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t error_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool error_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 1};
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
    case 0:
      return crit_e::ignore;
    case 1:
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
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
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
    case 0:
      return presence_e::optional;
    case 1:
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
  j.start_obj();
  switch (type_) {
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "error_ind_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE error_ind_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
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
  static const char* names[] = {"Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "error_ind_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<error_ind_ies_o>;

OCUDUASN_CODE error_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (cause_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
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
      case 0: {
        cause_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
  j.start_obj();
  if (cause_present) {
    j.write_int("id", 0);
    j.write_str("criticality", "ignore");
    cause.to_json(j);
  }
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// MeasurementAbort-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_abort_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {39, 40};
  return map_enum_number(names, 2, idx, "id");
}
bool meas_abort_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {39, 40};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_abort_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 39:
      return crit_e::reject;
    case 40:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_abort_ies_o::value_c meas_abort_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 39:
      ret.set(value_c::types::lmf_meas_id);
      break;
    case 40:
      ret.set(value_c::types::ran_meas_id);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_abort_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 39:
      return presence_e::mandatory;
    case 40:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_abort_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_meas_id:
      c = uint32_t{};
      break;
    case types::ran_meas_id:
      c = uint32_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_abort_ies_o::value_c");
  }
}
uint32_t& meas_abort_ies_o::value_c::lmf_meas_id()
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& meas_abort_ies_o::value_c::ran_meas_id()
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& meas_abort_ies_o::value_c::lmf_meas_id() const
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& meas_abort_ies_o::value_c::ran_meas_id() const
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
void meas_abort_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::ran_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    default:
      log_invalid_choice_id(type_, "meas_abort_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_abort_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    default:
      log_invalid_choice_id(type_, "meas_abort_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_abort_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    default:
      log_invalid_choice_id(type_, "meas_abort_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_abort_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..65536,...)", "INTEGER (1..65536,...)"};
  return convert_enum_idx(names, 2, value, "meas_abort_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_abort_ies_o>;

OCUDUASN_CODE meas_abort_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)39, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_abort_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_abort_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
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
void meas_abort_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_meas_id);
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_meas_id);
  j.end_obj();
}

// MeasurementActivation-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_activation_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {98, 88};
  return map_enum_number(names, 2, idx, "id");
}
bool meas_activation_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {98, 88};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_activation_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 98:
      return crit_e::reject;
    case 88:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_activation_ies_o::value_c meas_activation_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 98:
      ret.set(value_c::types::request_type);
      break;
    case 88:
      ret.set(value_c::types::prs_meass_info_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_activation_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 98:
      return presence_e::mandatory;
    case 88:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_activation_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::request_type:
      c = request_type_e{};
      break;
    case types::prs_meass_info_list:
      c = prs_meass_info_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_activation_ies_o::value_c");
  }
}
request_type_e& meas_activation_ies_o::value_c::request_type()
{
  assert_choice_type(types::request_type, type_, "Value");
  return c.get<request_type_e>();
}
prs_meass_info_list_l& meas_activation_ies_o::value_c::prs_meass_info_list()
{
  assert_choice_type(types::prs_meass_info_list, type_, "Value");
  return c.get<prs_meass_info_list_l>();
}
const request_type_e& meas_activation_ies_o::value_c::request_type() const
{
  assert_choice_type(types::request_type, type_, "Value");
  return c.get<request_type_e>();
}
const prs_meass_info_list_l& meas_activation_ies_o::value_c::prs_meass_info_list() const
{
  assert_choice_type(types::prs_meass_info_list, type_, "Value");
  return c.get<prs_meass_info_list_l>();
}
void meas_activation_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::request_type:
      j.write_str("RequestType", c.get<request_type_e>().to_string());
      break;
    case types::prs_meass_info_list:
      j.start_array("PRS-Measurements-Info-List");
      for (const auto& e1 : c.get<prs_meass_info_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "meas_activation_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_activation_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::request_type:
      HANDLE_CODE(c.get<request_type_e>().pack(bref));
      break;
    case types::prs_meass_info_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<prs_meass_info_list_l>(), 1, 4, true));
      break;
    default:
      log_invalid_choice_id(type_, "meas_activation_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_activation_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::request_type:
      HANDLE_CODE(c.get<request_type_e>().unpack(bref));
      break;
    case types::prs_meass_info_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<prs_meass_info_list_l>(), bref, 1, 4, true));
      break;
    default:
      log_invalid_choice_id(type_, "meas_activation_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_activation_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RequestType", "PRS-Measurements-Info-List"};
  return convert_enum_idx(names, 2, value, "meas_activation_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_activation_ies_o>;

OCUDUASN_CODE meas_activation_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += prs_meass_info_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)98, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(request_type.pack(bref));
  }
  if (prs_meass_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)88, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, prs_meass_info_list, 1, 4, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_activation_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_activation_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 98: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(request_type.unpack(bref));
        break;
      }
      case 88: {
        prs_meass_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(prs_meass_info_list, bref, 1, 4, true));
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
void meas_activation_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 98);
  j.write_str("criticality", "reject");
  j.write_str("Value", request_type.to_string());
  if (prs_meass_info_list_present) {
    j.write_int("id", 88);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : prs_meass_info_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// MeasurementFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {39, 0, 1};
  return map_enum_number(names, 3, idx, "id");
}
bool meas_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {39, 0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 39:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_fail_ies_o::value_c meas_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 39:
      ret.set(value_c::types::lmf_meas_id);
      break;
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 39:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_meas_id:
      c = uint32_t{};
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
      log_invalid_choice_id(type_, "meas_fail_ies_o::value_c");
  }
}
uint32_t& meas_fail_ies_o::value_c::lmf_meas_id()
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
cause_c& meas_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& meas_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint32_t& meas_fail_ies_o::value_c::lmf_meas_id() const
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const cause_c& meas_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& meas_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void meas_fail_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "meas_fail_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..65536,...)", "Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 3, value, "meas_fail_ies_o::value_c::types");
}
uint8_t meas_fail_ies_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "meas_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_fail_ies_o>;

OCUDUASN_CODE meas_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)39, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
void meas_fail_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_meas_id);
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// MeasurementFailureIndication-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {39, 40, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool meas_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {39, 40, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_fail_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 39:
      return crit_e::reject;
    case 40:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_fail_ind_ies_o::value_c meas_fail_ind_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 39:
      ret.set(value_c::types::lmf_meas_id);
      break;
    case 40:
      ret.set(value_c::types::ran_meas_id);
      break;
    case 0:
      ret.set(value_c::types::cause);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 39:
      return presence_e::mandatory;
    case 40:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_fail_ind_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_meas_id:
      c = uint32_t{};
      break;
    case types::ran_meas_id:
      c = uint32_t{};
      break;
    case types::cause:
      c = cause_c{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_fail_ind_ies_o::value_c");
  }
}
uint32_t& meas_fail_ind_ies_o::value_c::lmf_meas_id()
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& meas_fail_ind_ies_o::value_c::ran_meas_id()
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
cause_c& meas_fail_ind_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const uint32_t& meas_fail_ind_ies_o::value_c::lmf_meas_id() const
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& meas_fail_ind_ies_o::value_c::ran_meas_id() const
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const cause_c& meas_fail_ind_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
void meas_fail_ind_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::ran_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "meas_fail_ind_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_fail_ind_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_fail_ind_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_fail_ind_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_fail_ind_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_fail_ind_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..65536,...)", "INTEGER (1..65536,...)", "Cause"};
  return convert_enum_idx(names, 3, value, "meas_fail_ind_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_fail_ind_ies_o>;

OCUDUASN_CODE meas_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)39, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_fail_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 0: {
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
void meas_fail_ind_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_meas_id);
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_meas_id);
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  j.end_obj();
}

// MeasurementPreconfigurationConfirm-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_precfg_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {97, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool meas_precfg_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {97, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_precfg_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 97:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_precfg_confirm_ies_o::value_c meas_precfg_confirm_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 97:
      ret.set(value_c::types::precfg_result);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_precfg_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 97:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_precfg_confirm_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::precfg_result:
      c = fixed_bitstring<8, false, true>{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_precfg_confirm_ies_o::value_c");
  }
}
fixed_bitstring<8, false, true>& meas_precfg_confirm_ies_o::value_c::precfg_result()
{
  assert_choice_type(types::precfg_result, type_, "Value");
  return c.get<fixed_bitstring<8, false, true>>();
}
crit_diagnostics_s& meas_precfg_confirm_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const fixed_bitstring<8, false, true>& meas_precfg_confirm_ies_o::value_c::precfg_result() const
{
  assert_choice_type(types::precfg_result, type_, "Value");
  return c.get<fixed_bitstring<8, false, true>>();
}
const crit_diagnostics_s& meas_precfg_confirm_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void meas_precfg_confirm_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::precfg_result:
      j.write_str("BIT STRING", c.get<fixed_bitstring<8, false, true>>().to_string());
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "meas_precfg_confirm_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_precfg_confirm_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::precfg_result:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().pack(bref)));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_precfg_confirm_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_precfg_confirm_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::precfg_result:
      HANDLE_CODE((c.get<fixed_bitstring<8, false, true>>().unpack(bref)));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_precfg_confirm_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_precfg_confirm_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"BIT STRING", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "meas_precfg_confirm_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_precfg_confirm_ies_o>;

OCUDUASN_CODE meas_precfg_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)97, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(precfg_result.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_precfg_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_precfg_confirm_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 97: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(precfg_result.unpack(bref));
        break;
      }
      case 1: {
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
void meas_precfg_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 97);
  j.write_str("criticality", "ignore");
  j.write_str("Value", precfg_result.to_string());
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// MeasurementPreconfigurationRefuse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_precfg_refuse_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool meas_precfg_refuse_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_precfg_refuse_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_precfg_refuse_ies_o::value_c meas_precfg_refuse_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_precfg_refuse_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_precfg_refuse_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_precfg_refuse_ies_o::value_c");
  }
}
cause_c& meas_precfg_refuse_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& meas_precfg_refuse_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const cause_c& meas_precfg_refuse_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& meas_precfg_refuse_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void meas_precfg_refuse_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "meas_precfg_refuse_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_precfg_refuse_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_precfg_refuse_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_precfg_refuse_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_precfg_refuse_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_precfg_refuse_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "meas_precfg_refuse_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_precfg_refuse_ies_o>;

OCUDUASN_CODE meas_precfg_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_precfg_refuse_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_precfg_refuse_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
void meas_precfg_refuse_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// MeasurementPreconfigurationRequired-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_precfg_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {87};
  return map_enum_number(names, 1, idx, "id");
}
bool meas_precfg_required_ies_o::is_id_valid(const uint32_t& id)
{
  return 87 == id;
}
crit_e meas_precfg_required_ies_o::get_crit(const uint32_t& id)
{
  if (id == 87) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
meas_precfg_required_ies_o::value_c meas_precfg_required_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 87) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_precfg_required_ies_o::get_presence(const uint32_t& id)
{
  if (id == 87) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void meas_precfg_required_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("TRP-PRS-Information-List");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}
OCUDUASN_CODE meas_precfg_required_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 256, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_precfg_required_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 256, true));
  return OCUDUASN_SUCCESS;
}

const char* meas_precfg_required_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TRP-PRS-Information-List"};
  return convert_enum_idx(names, 1, value, "meas_precfg_required_ies_o::value_c::types");
}

// MeasurementReport-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {39, 40, 43};
  return map_enum_number(names, 3, idx, "id");
}
bool meas_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {39, 40, 43};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 39:
      return crit_e::reject;
    case 40:
      return crit_e::reject;
    case 43:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_report_ies_o::value_c meas_report_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 39:
      ret.set(value_c::types::lmf_meas_id);
      break;
    case 40:
      ret.set(value_c::types::ran_meas_id);
      break;
    case 43:
      ret.set(value_c::types::trp_meas_report_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 39:
      return presence_e::mandatory;
    case 40:
      return presence_e::mandatory;
    case 43:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_report_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_meas_id:
      c = uint32_t{};
      break;
    case types::ran_meas_id:
      c = uint32_t{};
      break;
    case types::trp_meas_report_list:
      c = trp_meas_resp_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_report_ies_o::value_c");
  }
}
uint32_t& meas_report_ies_o::value_c::lmf_meas_id()
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& meas_report_ies_o::value_c::ran_meas_id()
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
trp_meas_resp_list_l& meas_report_ies_o::value_c::trp_meas_report_list()
{
  assert_choice_type(types::trp_meas_report_list, type_, "Value");
  return c.get<trp_meas_resp_list_l>();
}
const uint32_t& meas_report_ies_o::value_c::lmf_meas_id() const
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& meas_report_ies_o::value_c::ran_meas_id() const
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const trp_meas_resp_list_l& meas_report_ies_o::value_c::trp_meas_report_list() const
{
  assert_choice_type(types::trp_meas_report_list, type_, "Value");
  return c.get<trp_meas_resp_list_l>();
}
void meas_report_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::ran_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::trp_meas_report_list:
      j.start_array("TRP-MeasurementResponseList");
      for (const auto& e1 : c.get<trp_meas_resp_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "meas_report_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_report_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::trp_meas_report_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_meas_resp_list_l>(), 1, 64, true));
      break;
    default:
      log_invalid_choice_id(type_, "meas_report_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_report_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::trp_meas_report_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_meas_resp_list_l>(), bref, 1, 64, true));
      break;
    default:
      log_invalid_choice_id(type_, "meas_report_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_report_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..65536,...)", "INTEGER (1..65536,...)", "TRP-MeasurementResponseList"};
  return convert_enum_idx(names, 3, value, "meas_report_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_report_ies_o>;

OCUDUASN_CODE meas_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)39, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)43, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_meas_report_list, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_report_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 43: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_meas_report_list, bref, 1, 64, true));
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
void meas_report_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_meas_id);
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_meas_id);
  j.write_int("id", 43);
  j.write_str("criticality", "reject");
  j.start_array("Value");
  for (const auto& e1 : trp_meas_report_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// MeasurementRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {39, 41, 3, 4, 52, 54, 26, 31, 49, 50, 64, 72, 92, 91, 95, 125};
  return map_enum_number(names, 16, idx, "id");
}
bool meas_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {39, 41, 3, 4, 52, 54, 26, 31, 49, 50, 64, 72, 92, 91, 95, 125};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 39:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 4:
      return crit_e::reject;
    case 52:
      return crit_e::reject;
    case 54:
      return crit_e::ignore;
    case 26:
      return crit_e::ignore;
    case 31:
      return crit_e::ignore;
    case 49:
      return crit_e::ignore;
    case 50:
      return crit_e::ignore;
    case 64:
      return crit_e::reject;
    case 72:
      return crit_e::ignore;
    case 92:
      return crit_e::ignore;
    case 91:
      return crit_e::ignore;
    case 95:
      return crit_e::ignore;
    case 125:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_request_ies_o::value_c meas_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 39:
      ret.set(value_c::types::lmf_meas_id);
      break;
    case 41:
      ret.set(value_c::types::trp_meas_request_list);
      break;
    case 3:
      ret.set(value_c::types::report_characteristics);
      break;
    case 4:
      ret.set(value_c::types::meas_periodicity);
      break;
    case 52:
      ret.set(value_c::types::trp_meas_quantities);
      break;
    case 54:
      ret.set(value_c::types::sfn_initisation_time);
      break;
    case 26:
      ret.set(value_c::types::srs_cfg);
      break;
    case 31:
      ret.set(value_c::types::meas_beam_info_request);
      break;
    case 49:
      ret.set(value_c::types::sys_frame_num);
      break;
    case 50:
      ret.set(value_c::types::slot_num);
      break;
    case 64:
      ret.set(value_c::types::meas_periodicity_extended);
      break;
    case 72:
      ret.set(value_c::types::resp_time);
      break;
    case 92:
      ret.set(value_c::types::meas_characteristics_request_ind);
      break;
    case 91:
      ret.set(value_c::types::meas_time_occasion);
      break;
    case 95:
      ret.set(value_c::types::meas_amount);
      break;
    case 125:
      ret.set(value_c::types::time_win_info_meas_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 39:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 4:
      return presence_e::conditional;
    case 52:
      return presence_e::mandatory;
    case 54:
      return presence_e::optional;
    case 26:
      return presence_e::optional;
    case 31:
      return presence_e::optional;
    case 49:
      return presence_e::optional;
    case 50:
      return presence_e::optional;
    case 64:
      return presence_e::conditional;
    case 72:
      return presence_e::optional;
    case 92:
      return presence_e::optional;
    case 91:
      return presence_e::optional;
    case 95:
      return presence_e::optional;
    case 125:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_meas_id:
      c = uint32_t{};
      break;
    case types::trp_meas_request_list:
      c = trp_meas_request_list_l{};
      break;
    case types::report_characteristics:
      c = report_characteristics_e{};
      break;
    case types::meas_periodicity:
      c = meas_periodicity_e{};
      break;
    case types::trp_meas_quantities:
      c = trp_meas_quantities_l{};
      break;
    case types::sfn_initisation_time:
      c = fixed_bitstring<64, false, true>{};
      break;
    case types::srs_cfg:
      c = srs_configuration_s{};
      break;
    case types::meas_beam_info_request:
      c = meas_beam_info_request_e{};
      break;
    case types::sys_frame_num:
      c = uint16_t{};
      break;
    case types::slot_num:
      c = uint8_t{};
      break;
    case types::meas_periodicity_extended:
      c = meas_periodicity_extended_e{};
      break;
    case types::resp_time:
      c = resp_time_s{};
      break;
    case types::meas_characteristics_request_ind:
      c = fixed_bitstring<16, false, true>{};
      break;
    case types::meas_time_occasion:
      c = meas_time_occasion_e{};
      break;
    case types::meas_amount:
      c = meas_amount_e{};
      break;
    case types::time_win_info_meas_list:
      c = time_win_info_meas_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_request_ies_o::value_c");
  }
}
uint32_t& meas_request_ies_o::value_c::lmf_meas_id()
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
trp_meas_request_list_l& meas_request_ies_o::value_c::trp_meas_request_list()
{
  assert_choice_type(types::trp_meas_request_list, type_, "Value");
  return c.get<trp_meas_request_list_l>();
}
report_characteristics_e& meas_request_ies_o::value_c::report_characteristics()
{
  assert_choice_type(types::report_characteristics, type_, "Value");
  return c.get<report_characteristics_e>();
}
meas_periodicity_e& meas_request_ies_o::value_c::meas_periodicity()
{
  assert_choice_type(types::meas_periodicity, type_, "Value");
  return c.get<meas_periodicity_e>();
}
trp_meas_quantities_l& meas_request_ies_o::value_c::trp_meas_quantities()
{
  assert_choice_type(types::trp_meas_quantities, type_, "Value");
  return c.get<trp_meas_quantities_l>();
}
fixed_bitstring<64, false, true>& meas_request_ies_o::value_c::sfn_initisation_time()
{
  assert_choice_type(types::sfn_initisation_time, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
srs_configuration_s& meas_request_ies_o::value_c::srs_cfg()
{
  assert_choice_type(types::srs_cfg, type_, "Value");
  return c.get<srs_configuration_s>();
}
meas_beam_info_request_e& meas_request_ies_o::value_c::meas_beam_info_request()
{
  assert_choice_type(types::meas_beam_info_request, type_, "Value");
  return c.get<meas_beam_info_request_e>();
}
uint16_t& meas_request_ies_o::value_c::sys_frame_num()
{
  assert_choice_type(types::sys_frame_num, type_, "Value");
  return c.get<uint16_t>();
}
uint8_t& meas_request_ies_o::value_c::slot_num()
{
  assert_choice_type(types::slot_num, type_, "Value");
  return c.get<uint8_t>();
}
meas_periodicity_extended_e& meas_request_ies_o::value_c::meas_periodicity_extended()
{
  assert_choice_type(types::meas_periodicity_extended, type_, "Value");
  return c.get<meas_periodicity_extended_e>();
}
resp_time_s& meas_request_ies_o::value_c::resp_time()
{
  assert_choice_type(types::resp_time, type_, "Value");
  return c.get<resp_time_s>();
}
fixed_bitstring<16, false, true>& meas_request_ies_o::value_c::meas_characteristics_request_ind()
{
  assert_choice_type(types::meas_characteristics_request_ind, type_, "Value");
  return c.get<fixed_bitstring<16, false, true>>();
}
meas_time_occasion_e& meas_request_ies_o::value_c::meas_time_occasion()
{
  assert_choice_type(types::meas_time_occasion, type_, "Value");
  return c.get<meas_time_occasion_e>();
}
meas_amount_e& meas_request_ies_o::value_c::meas_amount()
{
  assert_choice_type(types::meas_amount, type_, "Value");
  return c.get<meas_amount_e>();
}
time_win_info_meas_list_l& meas_request_ies_o::value_c::time_win_info_meas_list()
{
  assert_choice_type(types::time_win_info_meas_list, type_, "Value");
  return c.get<time_win_info_meas_list_l>();
}
const uint32_t& meas_request_ies_o::value_c::lmf_meas_id() const
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const trp_meas_request_list_l& meas_request_ies_o::value_c::trp_meas_request_list() const
{
  assert_choice_type(types::trp_meas_request_list, type_, "Value");
  return c.get<trp_meas_request_list_l>();
}
const report_characteristics_e& meas_request_ies_o::value_c::report_characteristics() const
{
  assert_choice_type(types::report_characteristics, type_, "Value");
  return c.get<report_characteristics_e>();
}
const meas_periodicity_e& meas_request_ies_o::value_c::meas_periodicity() const
{
  assert_choice_type(types::meas_periodicity, type_, "Value");
  return c.get<meas_periodicity_e>();
}
const trp_meas_quantities_l& meas_request_ies_o::value_c::trp_meas_quantities() const
{
  assert_choice_type(types::trp_meas_quantities, type_, "Value");
  return c.get<trp_meas_quantities_l>();
}
const fixed_bitstring<64, false, true>& meas_request_ies_o::value_c::sfn_initisation_time() const
{
  assert_choice_type(types::sfn_initisation_time, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
const srs_configuration_s& meas_request_ies_o::value_c::srs_cfg() const
{
  assert_choice_type(types::srs_cfg, type_, "Value");
  return c.get<srs_configuration_s>();
}
const meas_beam_info_request_e& meas_request_ies_o::value_c::meas_beam_info_request() const
{
  assert_choice_type(types::meas_beam_info_request, type_, "Value");
  return c.get<meas_beam_info_request_e>();
}
const uint16_t& meas_request_ies_o::value_c::sys_frame_num() const
{
  assert_choice_type(types::sys_frame_num, type_, "Value");
  return c.get<uint16_t>();
}
const uint8_t& meas_request_ies_o::value_c::slot_num() const
{
  assert_choice_type(types::slot_num, type_, "Value");
  return c.get<uint8_t>();
}
const meas_periodicity_extended_e& meas_request_ies_o::value_c::meas_periodicity_extended() const
{
  assert_choice_type(types::meas_periodicity_extended, type_, "Value");
  return c.get<meas_periodicity_extended_e>();
}
const resp_time_s& meas_request_ies_o::value_c::resp_time() const
{
  assert_choice_type(types::resp_time, type_, "Value");
  return c.get<resp_time_s>();
}
const fixed_bitstring<16, false, true>& meas_request_ies_o::value_c::meas_characteristics_request_ind() const
{
  assert_choice_type(types::meas_characteristics_request_ind, type_, "Value");
  return c.get<fixed_bitstring<16, false, true>>();
}
const meas_time_occasion_e& meas_request_ies_o::value_c::meas_time_occasion() const
{
  assert_choice_type(types::meas_time_occasion, type_, "Value");
  return c.get<meas_time_occasion_e>();
}
const meas_amount_e& meas_request_ies_o::value_c::meas_amount() const
{
  assert_choice_type(types::meas_amount, type_, "Value");
  return c.get<meas_amount_e>();
}
const time_win_info_meas_list_l& meas_request_ies_o::value_c::time_win_info_meas_list() const
{
  assert_choice_type(types::time_win_info_meas_list, type_, "Value");
  return c.get<time_win_info_meas_list_l>();
}
void meas_request_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::trp_meas_request_list:
      j.start_array("TRP-MeasurementRequestList");
      for (const auto& e1 : c.get<trp_meas_request_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::report_characteristics:
      j.write_str("ReportCharacteristics", c.get<report_characteristics_e>().to_string());
      break;
    case types::meas_periodicity:
      j.write_str("MeasurementPeriodicity", c.get<meas_periodicity_e>().to_string());
      break;
    case types::trp_meas_quantities:
      j.start_array("TRPMeasurementQuantities");
      for (const auto& e1 : c.get<trp_meas_quantities_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::sfn_initisation_time:
      j.write_str("BIT STRING", c.get<fixed_bitstring<64, false, true>>().to_string());
      break;
    case types::srs_cfg:
      j.write_fieldname("SRSConfiguration");
      c.get<srs_configuration_s>().to_json(j);
      break;
    case types::meas_beam_info_request:
      j.write_str("MeasurementBeamInfoRequest", "true");
      break;
    case types::sys_frame_num:
      j.write_int("INTEGER (0..1023)", c.get<uint16_t>());
      break;
    case types::slot_num:
      j.write_int("INTEGER (0..79)", c.get<uint8_t>());
      break;
    case types::meas_periodicity_extended:
      j.write_str("MeasurementPeriodicityExtended", c.get<meas_periodicity_extended_e>().to_string());
      break;
    case types::resp_time:
      j.write_fieldname("ResponseTime");
      c.get<resp_time_s>().to_json(j);
      break;
    case types::meas_characteristics_request_ind:
      j.write_str("BIT STRING", c.get<fixed_bitstring<16, false, true>>().to_string());
      break;
    case types::meas_time_occasion:
      j.write_str("MeasurementTimeOccasion", c.get<meas_time_occasion_e>().to_string());
      break;
    case types::meas_amount:
      j.write_str("MeasurementAmount", c.get<meas_amount_e>().to_string());
      break;
    case types::time_win_info_meas_list:
      j.start_array("TimeWindowInformation-Measurement-List");
      for (const auto& e1 : c.get<time_win_info_meas_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "meas_request_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::trp_meas_request_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_meas_request_list_l>(), 1, 64, true));
      break;
    case types::report_characteristics:
      HANDLE_CODE(c.get<report_characteristics_e>().pack(bref));
      break;
    case types::meas_periodicity:
      HANDLE_CODE(c.get<meas_periodicity_e>().pack(bref));
      break;
    case types::trp_meas_quantities:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_meas_quantities_l>(), 1, 16384, true));
      break;
    case types::sfn_initisation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    case types::srs_cfg:
      HANDLE_CODE(c.get<srs_configuration_s>().pack(bref));
      break;
    case types::meas_beam_info_request:
      HANDLE_CODE(c.get<meas_beam_info_request_e>().pack(bref));
      break;
    case types::sys_frame_num:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1023u, false, true));
      break;
    case types::slot_num:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)79u, false, true));
      break;
    case types::meas_periodicity_extended:
      HANDLE_CODE(c.get<meas_periodicity_extended_e>().pack(bref));
      break;
    case types::resp_time:
      HANDLE_CODE(c.get<resp_time_s>().pack(bref));
      break;
    case types::meas_characteristics_request_ind:
      HANDLE_CODE((c.get<fixed_bitstring<16, false, true>>().pack(bref)));
      break;
    case types::meas_time_occasion:
      HANDLE_CODE(c.get<meas_time_occasion_e>().pack(bref));
      break;
    case types::meas_amount:
      HANDLE_CODE(c.get<meas_amount_e>().pack(bref));
      break;
    case types::time_win_info_meas_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<time_win_info_meas_list_l>(), 1, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "meas_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::trp_meas_request_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_meas_request_list_l>(), bref, 1, 64, true));
      break;
    case types::report_characteristics:
      HANDLE_CODE(c.get<report_characteristics_e>().unpack(bref));
      break;
    case types::meas_periodicity:
      HANDLE_CODE(c.get<meas_periodicity_e>().unpack(bref));
      break;
    case types::trp_meas_quantities:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_meas_quantities_l>(), bref, 1, 16384, true));
      break;
    case types::sfn_initisation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    case types::srs_cfg:
      HANDLE_CODE(c.get<srs_configuration_s>().unpack(bref));
      break;
    case types::meas_beam_info_request:
      HANDLE_CODE(c.get<meas_beam_info_request_e>().unpack(bref));
      break;
    case types::sys_frame_num:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1023u, false, true));
      break;
    case types::slot_num:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)79u, false, true));
      break;
    case types::meas_periodicity_extended:
      HANDLE_CODE(c.get<meas_periodicity_extended_e>().unpack(bref));
      break;
    case types::resp_time:
      HANDLE_CODE(c.get<resp_time_s>().unpack(bref));
      break;
    case types::meas_characteristics_request_ind:
      HANDLE_CODE((c.get<fixed_bitstring<16, false, true>>().unpack(bref)));
      break;
    case types::meas_time_occasion:
      HANDLE_CODE(c.get<meas_time_occasion_e>().unpack(bref));
      break;
    case types::meas_amount:
      HANDLE_CODE(c.get<meas_amount_e>().unpack(bref));
      break;
    case types::time_win_info_meas_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<time_win_info_meas_list_l>(), bref, 1, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "meas_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..65536,...)",
                                "TRP-MeasurementRequestList",
                                "ReportCharacteristics",
                                "MeasurementPeriodicity",
                                "TRPMeasurementQuantities",
                                "BIT STRING",
                                "SRSConfiguration",
                                "MeasurementBeamInfoRequest",
                                "INTEGER (0..1023)",
                                "INTEGER (0..79)",
                                "MeasurementPeriodicityExtended",
                                "ResponseTime",
                                "BIT STRING",
                                "MeasurementTimeOccasion",
                                "MeasurementAmount",
                                "TimeWindowInformation-Measurement-List"};
  return convert_enum_idx(names, 16, value, "meas_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_request_ies_o>;

OCUDUASN_CODE meas_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += meas_periodicity_present ? 1 : 0;
  nof_ies += sfn_initisation_time_present ? 1 : 0;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += meas_beam_info_request_present ? 1 : 0;
  nof_ies += sys_frame_num_present ? 1 : 0;
  nof_ies += slot_num_present ? 1 : 0;
  nof_ies += meas_periodicity_extended_present ? 1 : 0;
  nof_ies += resp_time_present ? 1 : 0;
  nof_ies += meas_characteristics_request_ind_present ? 1 : 0;
  nof_ies += meas_time_occasion_present ? 1 : 0;
  nof_ies += meas_amount_present ? 1 : 0;
  nof_ies += time_win_info_meas_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)39, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_meas_request_list, 1, 64, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)3, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(report_characteristics.pack(bref));
  }
  if (meas_periodicity_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)4, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_periodicity.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)52, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_meas_quantities, 1, 16384, true));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)54, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sfn_initisation_time.pack(bref));
  }
  if (srs_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)26, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_cfg.pack(bref));
  }
  if (meas_beam_info_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)31, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_beam_info_request.pack(bref));
  }
  if (sys_frame_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true));
  }
  if (slot_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)50, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, slot_num, (uint8_t)0u, (uint8_t)79u, false, true));
  }
  if (meas_periodicity_extended_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)64, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_periodicity_extended.pack(bref));
  }
  if (resp_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)72, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(resp_time.pack(bref));
  }
  if (meas_characteristics_request_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)92, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_characteristics_request_ind.pack(bref));
  }
  if (meas_time_occasion_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)91, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_time_occasion.pack(bref));
  }
  if (meas_amount_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)95, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_amount.pack(bref));
  }
  if (time_win_info_meas_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)125, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, time_win_info_meas_list, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_meas_request_list, bref, 1, 64, true));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(report_characteristics.unpack(bref));
        break;
      }
      case 4: {
        meas_periodicity_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_periodicity.unpack(bref));
        break;
      }
      case 52: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_meas_quantities, bref, 1, 16384, true));
        break;
      }
      case 54: {
        sfn_initisation_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sfn_initisation_time.unpack(bref));
        break;
      }
      case 26: {
        srs_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_cfg.unpack(bref));
        break;
      }
      case 31: {
        meas_beam_info_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_beam_info_request.unpack(bref));
        break;
      }
      case 49: {
        sys_frame_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true));
        break;
      }
      case 50: {
        slot_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(slot_num, bref, (uint8_t)0u, (uint8_t)79u, false, true));
        break;
      }
      case 64: {
        meas_periodicity_extended_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_periodicity_extended.unpack(bref));
        break;
      }
      case 72: {
        resp_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(resp_time.unpack(bref));
        break;
      }
      case 92: {
        meas_characteristics_request_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_characteristics_request_ind.unpack(bref));
        break;
      }
      case 91: {
        meas_time_occasion_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_time_occasion.unpack(bref));
        break;
      }
      case 95: {
        meas_amount_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_amount.unpack(bref));
        break;
      }
      case 125: {
        time_win_info_meas_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(time_win_info_meas_list, bref, 1, 16, true));
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
void meas_request_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_meas_id);
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.start_array("Value");
  for (const auto& e1 : trp_meas_request_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_str("Value", report_characteristics.to_string());
  if (meas_periodicity_present) {
    j.write_int("id", 4);
    j.write_str("criticality", "reject");
    j.write_str("Value", meas_periodicity.to_string());
  }
  j.write_int("id", 52);
  j.write_str("criticality", "reject");
  j.start_array("Value");
  for (const auto& e1 : trp_meas_quantities) {
    e1.to_json(j);
  }
  j.end_array();
  if (sfn_initisation_time_present) {
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.write_str("Value", sfn_initisation_time.to_string());
  }
  if (srs_cfg_present) {
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    srs_cfg.to_json(j);
  }
  if (meas_beam_info_request_present) {
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_str("Value", "true");
  }
  if (sys_frame_num_present) {
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_int("Value", sys_frame_num);
  }
  if (slot_num_present) {
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.write_int("Value", slot_num);
  }
  if (meas_periodicity_extended_present) {
    j.write_int("id", 64);
    j.write_str("criticality", "reject");
    j.write_str("Value", meas_periodicity_extended.to_string());
  }
  if (resp_time_present) {
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    resp_time.to_json(j);
  }
  if (meas_characteristics_request_ind_present) {
    j.write_int("id", 92);
    j.write_str("criticality", "ignore");
    j.write_str("Value", meas_characteristics_request_ind.to_string());
  }
  if (meas_time_occasion_present) {
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.write_str("Value", meas_time_occasion.to_string());
  }
  if (meas_amount_present) {
    j.write_int("id", 95);
    j.write_str("criticality", "ignore");
    j.write_str("Value", meas_amount.to_string());
  }
  if (time_win_info_meas_list_present) {
    j.write_int("id", 125);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : time_win_info_meas_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// MeasurementResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {39, 40, 42, 1};
  return map_enum_number(names, 4, idx, "id");
}
bool meas_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {39, 40, 42, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 39:
      return crit_e::reject;
    case 40:
      return crit_e::reject;
    case 42:
      return crit_e::reject;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_resp_ies_o::value_c meas_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 39:
      ret.set(value_c::types::lmf_meas_id);
      break;
    case 40:
      ret.set(value_c::types::ran_meas_id);
      break;
    case 42:
      ret.set(value_c::types::trp_meas_resp_list);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 39:
      return presence_e::mandatory;
    case 40:
      return presence_e::mandatory;
    case 42:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_meas_id:
      c = uint32_t{};
      break;
    case types::ran_meas_id:
      c = uint32_t{};
      break;
    case types::trp_meas_resp_list:
      c = trp_meas_resp_list_l{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_resp_ies_o::value_c");
  }
}
uint32_t& meas_resp_ies_o::value_c::lmf_meas_id()
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& meas_resp_ies_o::value_c::ran_meas_id()
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
trp_meas_resp_list_l& meas_resp_ies_o::value_c::trp_meas_resp_list()
{
  assert_choice_type(types::trp_meas_resp_list, type_, "Value");
  return c.get<trp_meas_resp_list_l>();
}
crit_diagnostics_s& meas_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint32_t& meas_resp_ies_o::value_c::lmf_meas_id() const
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& meas_resp_ies_o::value_c::ran_meas_id() const
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const trp_meas_resp_list_l& meas_resp_ies_o::value_c::trp_meas_resp_list() const
{
  assert_choice_type(types::trp_meas_resp_list, type_, "Value");
  return c.get<trp_meas_resp_list_l>();
}
const crit_diagnostics_s& meas_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void meas_resp_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::ran_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::trp_meas_resp_list:
      j.start_array("TRP-MeasurementResponseList");
      for (const auto& e1 : c.get<trp_meas_resp_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "meas_resp_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::trp_meas_resp_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_meas_resp_list_l>(), 1, 64, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::trp_meas_resp_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_meas_resp_list_l>(), bref, 1, 64, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {
      "INTEGER (1..65536,...)", "INTEGER (1..65536,...)", "TRP-MeasurementResponseList", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 4, value, "meas_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_resp_ies_o>;

OCUDUASN_CODE meas_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += trp_meas_resp_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)39, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  if (trp_meas_resp_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)42, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_meas_resp_list, 1, 64, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 42: {
        trp_meas_resp_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_meas_resp_list, bref, 1, 64, true));
        break;
      }
      case 1: {
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
void meas_resp_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_meas_id);
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_meas_id);
  if (trp_meas_resp_list_present) {
    j.write_int("id", 42);
    j.write_str("criticality", "reject");
    j.start_array("Value");
    for (const auto& e1 : trp_meas_resp_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// MeasurementUpdate-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t meas_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {39, 40, 26, 70, 92, 91};
  return map_enum_number(names, 6, idx, "id");
}
bool meas_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {39, 40, 26, 70, 92, 91};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 39:
      return crit_e::reject;
    case 40:
      return crit_e::reject;
    case 26:
      return crit_e::ignore;
    case 70:
      return crit_e::reject;
    case 92:
      return crit_e::ignore;
    case 91:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
meas_upd_ies_o::value_c meas_upd_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 39:
      ret.set(value_c::types::lmf_meas_id);
      break;
    case 40:
      ret.set(value_c::types::ran_meas_id);
      break;
    case 26:
      ret.set(value_c::types::srs_cfg);
      break;
    case 70:
      ret.set(value_c::types::trp_meas_upd_list);
      break;
    case 92:
      ret.set(value_c::types::meas_characteristics_request_ind);
      break;
    case 91:
      ret.set(value_c::types::meas_time_occasion);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e meas_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 39:
      return presence_e::mandatory;
    case 40:
      return presence_e::mandatory;
    case 26:
      return presence_e::optional;
    case 70:
      return presence_e::optional;
    case 92:
      return presence_e::optional;
    case 91:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void meas_upd_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::lmf_meas_id:
      c = uint32_t{};
      break;
    case types::ran_meas_id:
      c = uint32_t{};
      break;
    case types::srs_cfg:
      c = srs_configuration_s{};
      break;
    case types::trp_meas_upd_list:
      c = trp_meas_upd_list_l{};
      break;
    case types::meas_characteristics_request_ind:
      c = fixed_bitstring<16, false, true>{};
      break;
    case types::meas_time_occasion:
      c = meas_time_occasion_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "meas_upd_ies_o::value_c");
  }
}
uint32_t& meas_upd_ies_o::value_c::lmf_meas_id()
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
uint32_t& meas_upd_ies_o::value_c::ran_meas_id()
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
srs_configuration_s& meas_upd_ies_o::value_c::srs_cfg()
{
  assert_choice_type(types::srs_cfg, type_, "Value");
  return c.get<srs_configuration_s>();
}
trp_meas_upd_list_l& meas_upd_ies_o::value_c::trp_meas_upd_list()
{
  assert_choice_type(types::trp_meas_upd_list, type_, "Value");
  return c.get<trp_meas_upd_list_l>();
}
fixed_bitstring<16, false, true>& meas_upd_ies_o::value_c::meas_characteristics_request_ind()
{
  assert_choice_type(types::meas_characteristics_request_ind, type_, "Value");
  return c.get<fixed_bitstring<16, false, true>>();
}
meas_time_occasion_e& meas_upd_ies_o::value_c::meas_time_occasion()
{
  assert_choice_type(types::meas_time_occasion, type_, "Value");
  return c.get<meas_time_occasion_e>();
}
const uint32_t& meas_upd_ies_o::value_c::lmf_meas_id() const
{
  assert_choice_type(types::lmf_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const uint32_t& meas_upd_ies_o::value_c::ran_meas_id() const
{
  assert_choice_type(types::ran_meas_id, type_, "Value");
  return c.get<uint32_t>();
}
const srs_configuration_s& meas_upd_ies_o::value_c::srs_cfg() const
{
  assert_choice_type(types::srs_cfg, type_, "Value");
  return c.get<srs_configuration_s>();
}
const trp_meas_upd_list_l& meas_upd_ies_o::value_c::trp_meas_upd_list() const
{
  assert_choice_type(types::trp_meas_upd_list, type_, "Value");
  return c.get<trp_meas_upd_list_l>();
}
const fixed_bitstring<16, false, true>& meas_upd_ies_o::value_c::meas_characteristics_request_ind() const
{
  assert_choice_type(types::meas_characteristics_request_ind, type_, "Value");
  return c.get<fixed_bitstring<16, false, true>>();
}
const meas_time_occasion_e& meas_upd_ies_o::value_c::meas_time_occasion() const
{
  assert_choice_type(types::meas_time_occasion, type_, "Value");
  return c.get<meas_time_occasion_e>();
}
void meas_upd_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::lmf_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::ran_meas_id:
      j.write_int("INTEGER (1..65536,...)", c.get<uint32_t>());
      break;
    case types::srs_cfg:
      j.write_fieldname("SRSConfiguration");
      c.get<srs_configuration_s>().to_json(j);
      break;
    case types::trp_meas_upd_list:
      j.start_array("TRP-MeasurementUpdateList");
      for (const auto& e1 : c.get<trp_meas_upd_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::meas_characteristics_request_ind:
      j.write_str("BIT STRING", c.get<fixed_bitstring<16, false, true>>().to_string());
      break;
    case types::meas_time_occasion:
      j.write_str("MeasurementTimeOccasion", c.get<meas_time_occasion_e>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "meas_upd_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE meas_upd_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(pack_integer(bref, c.get<uint32_t>(), (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::srs_cfg:
      HANDLE_CODE(c.get<srs_configuration_s>().pack(bref));
      break;
    case types::trp_meas_upd_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_meas_upd_list_l>(), 1, 64, true));
      break;
    case types::meas_characteristics_request_ind:
      HANDLE_CODE((c.get<fixed_bitstring<16, false, true>>().pack(bref)));
      break;
    case types::meas_time_occasion:
      HANDLE_CODE(c.get<meas_time_occasion_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_upd_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_upd_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::lmf_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::ran_meas_id:
      HANDLE_CODE(unpack_integer(c.get<uint32_t>(), bref, (uint32_t)1u, (uint32_t)65536u, true, true));
      break;
    case types::srs_cfg:
      HANDLE_CODE(c.get<srs_configuration_s>().unpack(bref));
      break;
    case types::trp_meas_upd_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_meas_upd_list_l>(), bref, 1, 64, true));
      break;
    case types::meas_characteristics_request_ind:
      HANDLE_CODE((c.get<fixed_bitstring<16, false, true>>().unpack(bref)));
      break;
    case types::meas_time_occasion:
      HANDLE_CODE(c.get<meas_time_occasion_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "meas_upd_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* meas_upd_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"INTEGER (1..65536,...)",
                                "INTEGER (1..65536,...)",
                                "SRSConfiguration",
                                "TRP-MeasurementUpdateList",
                                "BIT STRING",
                                "MeasurementTimeOccasion"};
  return convert_enum_idx(names, 6, value, "meas_upd_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<meas_upd_ies_o>;

OCUDUASN_CODE meas_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += trp_meas_upd_list_present ? 1 : 0;
  nof_ies += meas_characteristics_request_ind_present ? 1 : 0;
  nof_ies += meas_time_occasion_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)39, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  if (srs_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)26, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_cfg.pack(bref));
  }
  if (trp_meas_upd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)70, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_meas_upd_list, 1, 64, true));
  }
  if (meas_characteristics_request_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)92, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_characteristics_request_ind.pack(bref));
  }
  if (meas_time_occasion_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)91, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_time_occasion.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 26: {
        srs_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_cfg.unpack(bref));
        break;
      }
      case 70: {
        trp_meas_upd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_meas_upd_list, bref, 1, 64, true));
        break;
      }
      case 92: {
        meas_characteristics_request_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_characteristics_request_ind.unpack(bref));
        break;
      }
      case 91: {
        meas_time_occasion_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_time_occasion.unpack(bref));
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
void meas_upd_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("Value", lmf_meas_id);
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("Value", ran_meas_id);
  if (srs_cfg_present) {
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    srs_cfg.to_json(j);
  }
  if (trp_meas_upd_list_present) {
    j.write_int("id", 70);
    j.write_str("criticality", "reject");
    j.start_array("Value");
    for (const auto& e1 : trp_meas_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (meas_characteristics_request_ind_present) {
    j.write_int("id", 92);
    j.write_str("criticality", "ignore");
    j.write_str("Value", meas_characteristics_request_ind.to_string());
  }
  if (meas_time_occasion_present) {
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.write_str("Value", meas_time_occasion.to_string());
  }
  j.end_obj();
}

// OTDOA-Information-Type-Item ::= SEQUENCE
OCUDUASN_CODE otdoa_info_type_item_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(otdoa_info_item.pack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_info_type_item_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(otdoa_info_item.unpack(bref));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void otdoa_info_type_item_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("oTDOA-Information-Item", otdoa_info_item.to_string());
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// OTDOA-Information-Type-ItemIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t otdoa_info_type_item_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10};
  return map_enum_number(names, 1, idx, "id");
}
bool otdoa_info_type_item_ies_o::is_id_valid(const uint32_t& id)
{
  return 10 == id;
}
crit_e otdoa_info_type_item_ies_o::get_crit(const uint32_t& id)
{
  if (id == 10) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
otdoa_info_type_item_ies_o::value_c otdoa_info_type_item_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 10) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e otdoa_info_type_item_ies_o::get_presence(const uint32_t& id)
{
  if (id == 10) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void otdoa_info_type_item_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("OTDOA-Information-Type-Item");
  c.to_json(j);
  j.end_obj();
}
OCUDUASN_CODE otdoa_info_type_item_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_info_type_item_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* otdoa_info_type_item_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"OTDOA-Information-Type-Item"};
  return convert_enum_idx(names, 1, value, "otdoa_info_type_item_ies_o::value_c::types");
}

// OTDOAInformationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t otdoa_info_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool otdoa_info_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e otdoa_info_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
otdoa_info_fail_ies_o::value_c otdoa_info_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e otdoa_info_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void otdoa_info_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_info_fail_ies_o::value_c");
  }
}
cause_c& otdoa_info_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& otdoa_info_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const cause_c& otdoa_info_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& otdoa_info_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void otdoa_info_fail_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_info_fail_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE otdoa_info_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_info_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_info_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_info_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* otdoa_info_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "otdoa_info_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<otdoa_info_fail_ies_o>;

OCUDUASN_CODE otdoa_info_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_info_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != otdoa_info_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
void otdoa_info_fail_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// OTDOAInformationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t otdoa_info_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {9};
  return map_enum_number(names, 1, idx, "id");
}
bool otdoa_info_request_ies_o::is_id_valid(const uint32_t& id)
{
  return 9 == id;
}
crit_e otdoa_info_request_ies_o::get_crit(const uint32_t& id)
{
  if (id == 9) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
otdoa_info_request_ies_o::value_c otdoa_info_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 9) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e otdoa_info_request_ies_o::get_presence(const uint32_t& id)
{
  if (id == 9) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void otdoa_info_request_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.start_array("OTDOA-Information-Type");
  for (const auto& e1 : c) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}
OCUDUASN_CODE otdoa_info_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(pack_dyn_seq_of(bref, c, 1, 63, true));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_info_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(unpack_dyn_seq_of(c, bref, 1, 63, true));
  return OCUDUASN_SUCCESS;
}

const char* otdoa_info_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"OTDOA-Information-Type"};
  return convert_enum_idx(names, 1, value, "otdoa_info_request_ies_o::value_c::types");
}

// OTDOAInformationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t otdoa_info_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {8, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool otdoa_info_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {8, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e otdoa_info_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 8:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
otdoa_info_resp_ies_o::value_c otdoa_info_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 8:
      ret.set(value_c::types::otdoa_cells);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e otdoa_info_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 8:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void otdoa_info_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::otdoa_cells:
      c = otdoa_cells_l{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_info_resp_ies_o::value_c");
  }
}
otdoa_cells_l& otdoa_info_resp_ies_o::value_c::otdoa_cells()
{
  assert_choice_type(types::otdoa_cells, type_, "Value");
  return c.get<otdoa_cells_l>();
}
crit_diagnostics_s& otdoa_info_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const otdoa_cells_l& otdoa_info_resp_ies_o::value_c::otdoa_cells() const
{
  assert_choice_type(types::otdoa_cells, type_, "Value");
  return c.get<otdoa_cells_l>();
}
const crit_diagnostics_s& otdoa_info_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void otdoa_info_resp_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::otdoa_cells:
      j.start_array("OTDOACells");
      for (const auto& e1 : c.get<otdoa_cells_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_info_resp_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE otdoa_info_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::otdoa_cells:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<otdoa_cells_l>(), 1, 3840, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_info_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_info_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::otdoa_cells:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<otdoa_cells_l>(), bref, 1, 3840, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "otdoa_info_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* otdoa_info_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"OTDOACells", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "otdoa_info_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<otdoa_info_resp_ies_o>;

OCUDUASN_CODE otdoa_info_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)8, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, otdoa_cells, 1, 3840, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_info_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != otdoa_info_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 8: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(otdoa_cells, bref, 1, 3840, true));
        break;
      }
      case 1: {
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
void otdoa_info_resp_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 8);
  j.write_str("criticality", "ignore");
  j.start_array("Value");
  for (const auto& e1 : otdoa_cells) {
    e1.to_json(j);
  }
  j.end_array();
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// PRSConfigurationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t prs_cfg_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool prs_cfg_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e prs_cfg_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
prs_cfg_fail_ies_o::value_c prs_cfg_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e prs_cfg_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void prs_cfg_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_fail_ies_o::value_c");
  }
}
cause_c& prs_cfg_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& prs_cfg_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const cause_c& prs_cfg_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& prs_cfg_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void prs_cfg_fail_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_fail_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE prs_cfg_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* prs_cfg_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "prs_cfg_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<prs_cfg_fail_ies_o>;

OCUDUASN_CODE prs_cfg_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != prs_cfg_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
void prs_cfg_fail_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// PRSConfigurationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t prs_cfg_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {89, 66};
  return map_enum_number(names, 2, idx, "id");
}
bool prs_cfg_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {89, 66};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e prs_cfg_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 89:
      return crit_e::reject;
    case 66:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
prs_cfg_request_ies_o::value_c prs_cfg_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 89:
      ret.set(value_c::types::prs_cfg_request_type);
      break;
    case 66:
      ret.set(value_c::types::prstrp_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e prs_cfg_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 89:
      return presence_e::mandatory;
    case 66:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void prs_cfg_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::prs_cfg_request_type:
      c = prs_cfg_request_type_e{};
      break;
    case types::prstrp_list:
      c = prstrp_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_request_ies_o::value_c");
  }
}
prs_cfg_request_type_e& prs_cfg_request_ies_o::value_c::prs_cfg_request_type()
{
  assert_choice_type(types::prs_cfg_request_type, type_, "Value");
  return c.get<prs_cfg_request_type_e>();
}
prstrp_list_l& prs_cfg_request_ies_o::value_c::prstrp_list()
{
  assert_choice_type(types::prstrp_list, type_, "Value");
  return c.get<prstrp_list_l>();
}
const prs_cfg_request_type_e& prs_cfg_request_ies_o::value_c::prs_cfg_request_type() const
{
  assert_choice_type(types::prs_cfg_request_type, type_, "Value");
  return c.get<prs_cfg_request_type_e>();
}
const prstrp_list_l& prs_cfg_request_ies_o::value_c::prstrp_list() const
{
  assert_choice_type(types::prstrp_list, type_, "Value");
  return c.get<prstrp_list_l>();
}
void prs_cfg_request_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::prs_cfg_request_type:
      j.write_str("PRSConfigRequestType", c.get<prs_cfg_request_type_e>().to_string());
      break;
    case types::prstrp_list:
      j.start_array("PRSTRPList");
      for (const auto& e1 : c.get<prstrp_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_request_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE prs_cfg_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::prs_cfg_request_type:
      HANDLE_CODE(c.get<prs_cfg_request_type_e>().pack(bref));
      break;
    case types::prstrp_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<prstrp_list_l>(), 1, 65535, true));
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::prs_cfg_request_type:
      HANDLE_CODE(c.get<prs_cfg_request_type_e>().unpack(bref));
      break;
    case types::prstrp_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<prstrp_list_l>(), bref, 1, 65535, true));
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* prs_cfg_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"PRSConfigRequestType", "PRSTRPList"};
  return convert_enum_idx(names, 2, value, "prs_cfg_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<prs_cfg_request_ies_o>;

OCUDUASN_CODE prs_cfg_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)89, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(prs_cfg_request_type.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)66, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, prstrp_list, 1, 65535, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != prs_cfg_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(prs_cfg_request_type.unpack(bref));
        break;
      }
      case 66: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(prstrp_list, bref, 1, 65535, true));
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
void prs_cfg_request_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_str("Value", prs_cfg_request_type.to_string());
  j.write_int("id", 66);
  j.write_str("criticality", "ignore");
  j.start_array("Value");
  for (const auto& e1 : prstrp_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// PRSConfigurationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t prs_cfg_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {67, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool prs_cfg_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {67, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e prs_cfg_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 67:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
prs_cfg_resp_ies_o::value_c prs_cfg_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 67:
      ret.set(value_c::types::prs_tx_trp_list);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e prs_cfg_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 67:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void prs_cfg_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::prs_tx_trp_list:
      c = prs_tx_trp_list_l{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_resp_ies_o::value_c");
  }
}
prs_tx_trp_list_l& prs_cfg_resp_ies_o::value_c::prs_tx_trp_list()
{
  assert_choice_type(types::prs_tx_trp_list, type_, "Value");
  return c.get<prs_tx_trp_list_l>();
}
crit_diagnostics_s& prs_cfg_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const prs_tx_trp_list_l& prs_cfg_resp_ies_o::value_c::prs_tx_trp_list() const
{
  assert_choice_type(types::prs_tx_trp_list, type_, "Value");
  return c.get<prs_tx_trp_list_l>();
}
const crit_diagnostics_s& prs_cfg_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void prs_cfg_resp_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::prs_tx_trp_list:
      j.start_array("PRSTransmissionTRPList");
      for (const auto& e1 : c.get<prs_tx_trp_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_resp_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE prs_cfg_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::prs_tx_trp_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<prs_tx_trp_list_l>(), 1, 65535, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::prs_tx_trp_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<prs_tx_trp_list_l>(), bref, 1, 65535, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "prs_cfg_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* prs_cfg_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"PRSTransmissionTRPList", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "prs_cfg_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<prs_cfg_resp_ies_o>;

OCUDUASN_CODE prs_cfg_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += prs_tx_trp_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (prs_tx_trp_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)67, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, prs_tx_trp_list, 1, 65535, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != prs_cfg_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 67: {
        prs_tx_trp_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(prs_tx_trp_list, bref, 1, 65535, true));
        break;
      }
      case 1: {
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
void prs_cfg_resp_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (prs_tx_trp_list_present) {
    j.write_int("id", 67);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : prs_tx_trp_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// PositioningActivationFailureIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t positioning_activation_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool positioning_activation_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_activation_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
positioning_activation_fail_ies_o::value_c positioning_activation_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e positioning_activation_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void positioning_activation_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_fail_ies_o::value_c");
  }
}
cause_c& positioning_activation_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& positioning_activation_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const cause_c& positioning_activation_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& positioning_activation_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void positioning_activation_fail_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_fail_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE positioning_activation_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_activation_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* positioning_activation_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "positioning_activation_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<positioning_activation_fail_ies_o>;

OCUDUASN_CODE positioning_activation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_activation_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_activation_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
void positioning_activation_fail_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// SemipersistentSRS-ExtIEs ::= OBJECT SET OF NRPPA-PROTOCOL-EXTENSION
uint32_t semipersistent_srs_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {48, 63, 122};
  return map_enum_number(names, 3, idx, "id");
}
bool semipersistent_srs_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {48, 63, 122};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e semipersistent_srs_ext_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 48:
      return crit_e::ignore;
    case 63:
      return crit_e::ignore;
    case 122:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
semipersistent_srs_ext_ies_o::ext_c semipersistent_srs_ext_ies_o::get_ext(const uint32_t& id)
{
  ext_c ret{};
  switch (id) {
    case 48:
      ret.set(ext_c::types::srs_spatial_relation);
      break;
    case 63:
      ret.set(ext_c::types::srs_spatial_relation_per_srs_res);
      break;
    case 122:
      ret.set(ext_c::types::pos_srs_res_set_aggregation_list);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e semipersistent_srs_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 48:
      return presence_e::optional;
    case 63:
      return presence_e::optional;
    case 122:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Extension ::= OPEN TYPE
void semipersistent_srs_ext_ies_o::ext_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::srs_spatial_relation:
      c = spatial_relation_info_s{};
      break;
    case types::srs_spatial_relation_per_srs_res:
      c = spatial_relation_per_srs_res_s{};
      break;
    case types::pos_srs_res_set_aggregation_list:
      c = pos_srs_res_set_aggregation_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "semipersistent_srs_ext_ies_o::ext_c");
  }
}
spatial_relation_info_s& semipersistent_srs_ext_ies_o::ext_c::srs_spatial_relation()
{
  assert_choice_type(types::srs_spatial_relation, type_, "Extension");
  return c.get<spatial_relation_info_s>();
}
spatial_relation_per_srs_res_s& semipersistent_srs_ext_ies_o::ext_c::srs_spatial_relation_per_srs_res()
{
  assert_choice_type(types::srs_spatial_relation_per_srs_res, type_, "Extension");
  return c.get<spatial_relation_per_srs_res_s>();
}
pos_srs_res_set_aggregation_list_l& semipersistent_srs_ext_ies_o::ext_c::pos_srs_res_set_aggregation_list()
{
  assert_choice_type(types::pos_srs_res_set_aggregation_list, type_, "Extension");
  return c.get<pos_srs_res_set_aggregation_list_l>();
}
const spatial_relation_info_s& semipersistent_srs_ext_ies_o::ext_c::srs_spatial_relation() const
{
  assert_choice_type(types::srs_spatial_relation, type_, "Extension");
  return c.get<spatial_relation_info_s>();
}
const spatial_relation_per_srs_res_s& semipersistent_srs_ext_ies_o::ext_c::srs_spatial_relation_per_srs_res() const
{
  assert_choice_type(types::srs_spatial_relation_per_srs_res, type_, "Extension");
  return c.get<spatial_relation_per_srs_res_s>();
}
const pos_srs_res_set_aggregation_list_l& semipersistent_srs_ext_ies_o::ext_c::pos_srs_res_set_aggregation_list() const
{
  assert_choice_type(types::pos_srs_res_set_aggregation_list, type_, "Extension");
  return c.get<pos_srs_res_set_aggregation_list_l>();
}
void semipersistent_srs_ext_ies_o::ext_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srs_spatial_relation:
      j.write_fieldname("SpatialRelationInfo");
      c.get<spatial_relation_info_s>().to_json(j);
      break;
    case types::srs_spatial_relation_per_srs_res:
      j.write_fieldname("SpatialRelationPerSRSResource");
      c.get<spatial_relation_per_srs_res_s>().to_json(j);
      break;
    case types::pos_srs_res_set_aggregation_list:
      j.start_array("PosSRSResourceSet-Aggregation-List");
      for (const auto& e1 : c.get<pos_srs_res_set_aggregation_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "semipersistent_srs_ext_ies_o::ext_c");
  }
  j.end_obj();
}
OCUDUASN_CODE semipersistent_srs_ext_ies_o::ext_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_spatial_relation:
      HANDLE_CODE(c.get<spatial_relation_info_s>().pack(bref));
      break;
    case types::srs_spatial_relation_per_srs_res:
      HANDLE_CODE(c.get<spatial_relation_per_srs_res_s>().pack(bref));
      break;
    case types::pos_srs_res_set_aggregation_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<pos_srs_res_set_aggregation_list_l>(), 1, 32, true));
      break;
    default:
      log_invalid_choice_id(type_, "semipersistent_srs_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE semipersistent_srs_ext_ies_o::ext_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_spatial_relation:
      HANDLE_CODE(c.get<spatial_relation_info_s>().unpack(bref));
      break;
    case types::srs_spatial_relation_per_srs_res:
      HANDLE_CODE(c.get<spatial_relation_per_srs_res_s>().unpack(bref));
      break;
    case types::pos_srs_res_set_aggregation_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<pos_srs_res_set_aggregation_list_l>(), bref, 1, 32, true));
      break;
    default:
      log_invalid_choice_id(type_, "semipersistent_srs_ext_ies_o::ext_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* semipersistent_srs_ext_ies_o::ext_c::types_opts::to_string() const
{
  static const char* names[] = {
      "SpatialRelationInfo", "SpatialRelationPerSRSResource", "PosSRSResourceSet-Aggregation-List"};
  return convert_enum_idx(names, 3, value, "semipersistent_srs_ext_ies_o::ext_c::types");
}

template struct asn1::protocol_ext_field_s<semipersistent_srs_ext_ies_o>;

OCUDUASN_CODE semipersistent_srs_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += srs_spatial_relation_present ? 1 : 0;
  nof_ies += srs_spatial_relation_per_srs_res_present ? 1 : 0;
  nof_ies += pos_srs_res_set_aggregation_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (srs_spatial_relation_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)48, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_spatial_relation.pack(bref));
  }
  if (srs_spatial_relation_per_srs_res_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)63, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_spatial_relation_per_srs_res.pack(bref));
  }
  if (pos_srs_res_set_aggregation_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)122, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_srs_res_set_aggregation_list, 1, 32, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE semipersistent_srs_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != semipersistent_srs_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 48: {
        srs_spatial_relation_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_spatial_relation.unpack(bref));
        break;
      }
      case 63: {
        srs_spatial_relation_per_srs_res_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_spatial_relation_per_srs_res.unpack(bref));
        break;
      }
      case 122: {
        pos_srs_res_set_aggregation_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pos_srs_res_set_aggregation_list, bref, 1, 32, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void semipersistent_srs_ext_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_spatial_relation_present) {
    j.write_int("id", 48);
    j.write_str("criticality", "ignore");
    srs_spatial_relation.to_json(j);
  }
  if (srs_spatial_relation_per_srs_res_present) {
    j.write_int("id", 63);
    j.write_str("criticality", "ignore");
    srs_spatial_relation_per_srs_res.to_json(j);
  }
  if (pos_srs_res_set_aggregation_list_present) {
    j.write_int("id", 122);
    j.write_str("criticality", "ignore");
    j.start_array("Extension");
    for (const auto& e1 : pos_srs_res_set_aggregation_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// SemipersistentSRS ::= SEQUENCE
OCUDUASN_CODE semipersistent_srs_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, srs_res_set_id, (uint8_t)0u, (uint8_t)15u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE semipersistent_srs_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_res_set_id, bref, (uint8_t)0u, (uint8_t)15u, true, true));
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.unpack(bref));
  }

  if (ext) {
    ext_groups_unpacker unknown_ext_unpacker(bref, true);
    HANDLE_CODE(unknown_ext_unpacker.consume_remaining_groups(bref));
  }
  return OCUDUASN_SUCCESS;
}
void semipersistent_srs_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSResourceSetID", srs_res_set_id);
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

// SRSType ::= CHOICE
void srs_type_c::destroy_()
{
  switch (type_) {
    case types::semipersistent_srs:
      c.destroy<semipersistent_srs_s>();
      break;
    case types::aperiodic_srs:
      c.destroy<aperiodic_srs_s>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<srs_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void srs_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::semipersistent_srs:
      c.init<semipersistent_srs_s>();
      break;
    case types::aperiodic_srs:
      c.init<aperiodic_srs_s>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<srs_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "srs_type_c");
  }
}
srs_type_c::srs_type_c(const srs_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::semipersistent_srs:
      c.init(other.c.get<semipersistent_srs_s>());
      break;
    case types::aperiodic_srs:
      c.init(other.c.get<aperiodic_srs_s>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<srs_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "srs_type_c");
  }
}
srs_type_c& srs_type_c::operator=(const srs_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::semipersistent_srs:
      c.set(other.c.get<semipersistent_srs_s>());
      break;
    case types::aperiodic_srs:
      c.set(other.c.get<aperiodic_srs_s>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<srs_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "srs_type_c");
  }

  return *this;
}
semipersistent_srs_s& srs_type_c::set_semipersistent_srs()
{
  set(types::semipersistent_srs);
  return c.get<semipersistent_srs_s>();
}
aperiodic_srs_s& srs_type_c::set_aperiodic_srs()
{
  set(types::aperiodic_srs);
  return c.get<aperiodic_srs_s>();
}
protocol_ie_single_container_s<srs_type_ext_ies_o>& srs_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<srs_type_ext_ies_o>>();
}
void srs_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::semipersistent_srs:
      j.write_fieldname("semipersistentSRS");
      c.get<semipersistent_srs_s>().to_json(j);
      break;
    case types::aperiodic_srs:
      j.write_fieldname("aperiodicSRS");
      c.get<aperiodic_srs_s>().to_json(j);
      break;
    case types::choice_ext:
      j.write_fieldname("choice-Extension");
      c.get<protocol_ie_single_container_s<srs_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "srs_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE srs_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::semipersistent_srs:
      HANDLE_CODE(c.get<semipersistent_srs_s>().pack(bref));
      break;
    case types::aperiodic_srs:
      HANDLE_CODE(c.get<aperiodic_srs_s>().pack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<srs_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "srs_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::semipersistent_srs:
      HANDLE_CODE(c.get<semipersistent_srs_s>().unpack(bref));
      break;
    case types::aperiodic_srs:
      HANDLE_CODE(c.get<aperiodic_srs_s>().unpack(bref));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<srs_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "srs_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* srs_type_c::types_opts::to_string() const
{
  static const char* names[] = {"semipersistentSRS", "aperiodicSRS", "choice-Extension"};
  return convert_enum_idx(names, 3, value, "srs_type_c::types");
}

// PositioningActivationRequestIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t positioning_activation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {44, 45};
  return map_enum_number(names, 2, idx, "id");
}
bool positioning_activation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {44, 45};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_activation_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 44:
      return crit_e::reject;
    case 45:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
positioning_activation_request_ies_o::value_c positioning_activation_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 44:
      ret.set(value_c::types::srs_type);
      break;
    case 45:
      ret.set(value_c::types::activation_time);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e positioning_activation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 44:
      return presence_e::mandatory;
    case 45:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void positioning_activation_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::srs_type:
      c = srs_type_c{};
      break;
    case types::activation_time:
      c = fixed_bitstring<64, false, true>{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_request_ies_o::value_c");
  }
}
srs_type_c& positioning_activation_request_ies_o::value_c::srs_type()
{
  assert_choice_type(types::srs_type, type_, "Value");
  return c.get<srs_type_c>();
}
fixed_bitstring<64, false, true>& positioning_activation_request_ies_o::value_c::activation_time()
{
  assert_choice_type(types::activation_time, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
const srs_type_c& positioning_activation_request_ies_o::value_c::srs_type() const
{
  assert_choice_type(types::srs_type, type_, "Value");
  return c.get<srs_type_c>();
}
const fixed_bitstring<64, false, true>& positioning_activation_request_ies_o::value_c::activation_time() const
{
  assert_choice_type(types::activation_time, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
void positioning_activation_request_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srs_type:
      j.write_fieldname("SRSType");
      c.get<srs_type_c>().to_json(j);
      break;
    case types::activation_time:
      j.write_str("BIT STRING", c.get<fixed_bitstring<64, false, true>>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_request_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE positioning_activation_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_type:
      HANDLE_CODE(c.get<srs_type_c>().pack(bref));
      break;
    case types::activation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_activation_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_type:
      HANDLE_CODE(c.get<srs_type_c>().unpack(bref));
      break;
    case types::activation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* positioning_activation_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"SRSType", "BIT STRING"};
  return convert_enum_idx(names, 2, value, "positioning_activation_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<positioning_activation_request_ies_o>;

OCUDUASN_CODE positioning_activation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += activation_time_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)44, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_type.pack(bref));
  }
  if (activation_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)45, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(activation_time.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_activation_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_activation_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 44: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_type.unpack(bref));
        break;
      }
      case 45: {
        activation_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(activation_time.unpack(bref));
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
void positioning_activation_request_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 44);
  j.write_str("criticality", "reject");
  srs_type.to_json(j);
  if (activation_time_present) {
    j.write_int("id", 45);
    j.write_str("criticality", "ignore");
    j.write_str("Value", activation_time.to_string());
  }
  j.end_obj();
}

// PositioningActivationResponseIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t positioning_activation_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {1, 49, 50};
  return map_enum_number(names, 3, idx, "id");
}
bool positioning_activation_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {1, 49, 50};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_activation_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 1:
      return crit_e::ignore;
    case 49:
      return crit_e::ignore;
    case 50:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
positioning_activation_resp_ies_o::value_c positioning_activation_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    case 49:
      ret.set(value_c::types::sys_frame_num);
      break;
    case 50:
      ret.set(value_c::types::slot_num);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e positioning_activation_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 1:
      return presence_e::optional;
    case 49:
      return presence_e::optional;
    case 50:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void positioning_activation_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::sys_frame_num:
      c = uint16_t{};
      break;
    case types::slot_num:
      c = uint8_t{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_resp_ies_o::value_c");
  }
}
crit_diagnostics_s& positioning_activation_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
uint16_t& positioning_activation_resp_ies_o::value_c::sys_frame_num()
{
  assert_choice_type(types::sys_frame_num, type_, "Value");
  return c.get<uint16_t>();
}
uint8_t& positioning_activation_resp_ies_o::value_c::slot_num()
{
  assert_choice_type(types::slot_num, type_, "Value");
  return c.get<uint8_t>();
}
const crit_diagnostics_s& positioning_activation_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const uint16_t& positioning_activation_resp_ies_o::value_c::sys_frame_num() const
{
  assert_choice_type(types::sys_frame_num, type_, "Value");
  return c.get<uint16_t>();
}
const uint8_t& positioning_activation_resp_ies_o::value_c::slot_num() const
{
  assert_choice_type(types::slot_num, type_, "Value");
  return c.get<uint8_t>();
}
void positioning_activation_resp_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    case types::sys_frame_num:
      j.write_int("INTEGER (0..1023)", c.get<uint16_t>());
      break;
    case types::slot_num:
      j.write_int("INTEGER (0..79)", c.get<uint8_t>());
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_resp_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE positioning_activation_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    case types::sys_frame_num:
      HANDLE_CODE(pack_integer(bref, c.get<uint16_t>(), (uint16_t)0u, (uint16_t)1023u, false, true));
      break;
    case types::slot_num:
      HANDLE_CODE(pack_integer(bref, c.get<uint8_t>(), (uint8_t)0u, (uint8_t)79u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_activation_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    case types::sys_frame_num:
      HANDLE_CODE(unpack_integer(c.get<uint16_t>(), bref, (uint16_t)0u, (uint16_t)1023u, false, true));
      break;
    case types::slot_num:
      HANDLE_CODE(unpack_integer(c.get<uint8_t>(), bref, (uint8_t)0u, (uint8_t)79u, false, true));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_activation_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* positioning_activation_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"CriticalityDiagnostics", "INTEGER (0..1023)", "INTEGER (0..79)"};
  return convert_enum_idx(names, 3, value, "positioning_activation_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<positioning_activation_resp_ies_o>;

OCUDUASN_CODE positioning_activation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += sys_frame_num_present ? 1 : 0;
  nof_ies += slot_num_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (sys_frame_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)49, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true));
  }
  if (slot_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)50, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, slot_num, (uint8_t)0u, (uint8_t)79u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_activation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_activation_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 1: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 49: {
        sys_frame_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true));
        break;
      }
      case 50: {
        slot_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(slot_num, bref, (uint8_t)0u, (uint8_t)79u, false, true));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void positioning_activation_resp_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  if (sys_frame_num_present) {
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_int("Value", sys_frame_num);
  }
  if (slot_num_present) {
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.write_int("Value", slot_num);
  }
  j.end_obj();
}

// PositioningDeactivationIEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t positioning_deactivation_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {53};
  return map_enum_number(names, 1, idx, "id");
}
bool positioning_deactivation_ies_o::is_id_valid(const uint32_t& id)
{
  return 53 == id;
}
crit_e positioning_deactivation_ies_o::get_crit(const uint32_t& id)
{
  if (id == 53) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
positioning_deactivation_ies_o::value_c positioning_deactivation_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 53) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e positioning_deactivation_ies_o::get_presence(const uint32_t& id)
{
  if (id == 53) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void positioning_deactivation_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_fieldname("AbortTransmission");
  c.to_json(j);
  j.end_obj();
}
OCUDUASN_CODE positioning_deactivation_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_deactivation_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* positioning_deactivation_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"AbortTransmission"};
  return convert_enum_idx(names, 1, value, "positioning_deactivation_ies_o::value_c::types");
}

// PositioningInformationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t positioning_info_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool positioning_info_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_info_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
positioning_info_fail_ies_o::value_c positioning_info_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e positioning_info_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void positioning_info_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_fail_ies_o::value_c");
  }
}
cause_c& positioning_info_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& positioning_info_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const cause_c& positioning_info_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& positioning_info_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void positioning_info_fail_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_fail_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE positioning_info_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* positioning_info_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "positioning_info_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<positioning_info_fail_ies_o>;

OCUDUASN_CODE positioning_info_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_info_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
void positioning_info_fail_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// PositioningInformationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t positioning_info_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {12, 73, 90, 99, 124, 136, 159};
  return map_enum_number(names, 7, idx, "id");
}
bool positioning_info_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {12, 73, 90, 99, 124, 136, 159};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_info_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 12:
      return crit_e::ignore;
    case 73:
      return crit_e::ignore;
    case 90:
      return crit_e::ignore;
    case 99:
      return crit_e::reject;
    case 124:
      return crit_e::ignore;
    case 136:
      return crit_e::ignore;
    case 159:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
positioning_info_request_ies_o::value_c positioning_info_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 12:
      ret.set(value_c::types::requested_srs_tx_characteristics);
      break;
    case 73:
      ret.set(value_c::types::ue_report_info);
      break;
    case 90:
      ret.set(value_c::types::ue_teg_info_request);
      break;
    case 99:
      ret.set(value_c::types::ue_teg_report_periodicity);
      break;
    case 124:
      ret.set(value_c::types::time_win_info_srs_list);
      break;
    case 136:
      ret.set(value_c::types::requested_srs_precfg_characteristics_list);
      break;
    case 159:
      ret.set(value_c::types::remote_ue_ind_request);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e positioning_info_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 12:
      return presence_e::optional;
    case 73:
      return presence_e::optional;
    case 90:
      return presence_e::optional;
    case 99:
      return presence_e::conditional;
    case 124:
      return presence_e::optional;
    case 136:
      return presence_e::optional;
    case 159:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void positioning_info_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::requested_srs_tx_characteristics:
      c = requested_srs_tx_characteristics_s{};
      break;
    case types::ue_report_info:
      c = ue_report_info_s{};
      break;
    case types::ue_teg_info_request:
      c = ue_teg_info_request_e{};
      break;
    case types::ue_teg_report_periodicity:
      c = ue_teg_report_periodicity_e{};
      break;
    case types::time_win_info_srs_list:
      c = time_win_info_srs_list_l{};
      break;
    case types::requested_srs_precfg_characteristics_list:
      c = requested_srs_precfg_characteristics_list_l{};
      break;
    case types::remote_ue_ind_request:
      c = remote_ue_ind_request_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_request_ies_o::value_c");
  }
}
requested_srs_tx_characteristics_s& positioning_info_request_ies_o::value_c::requested_srs_tx_characteristics()
{
  assert_choice_type(types::requested_srs_tx_characteristics, type_, "Value");
  return c.get<requested_srs_tx_characteristics_s>();
}
ue_report_info_s& positioning_info_request_ies_o::value_c::ue_report_info()
{
  assert_choice_type(types::ue_report_info, type_, "Value");
  return c.get<ue_report_info_s>();
}
ue_teg_info_request_e& positioning_info_request_ies_o::value_c::ue_teg_info_request()
{
  assert_choice_type(types::ue_teg_info_request, type_, "Value");
  return c.get<ue_teg_info_request_e>();
}
ue_teg_report_periodicity_e& positioning_info_request_ies_o::value_c::ue_teg_report_periodicity()
{
  assert_choice_type(types::ue_teg_report_periodicity, type_, "Value");
  return c.get<ue_teg_report_periodicity_e>();
}
time_win_info_srs_list_l& positioning_info_request_ies_o::value_c::time_win_info_srs_list()
{
  assert_choice_type(types::time_win_info_srs_list, type_, "Value");
  return c.get<time_win_info_srs_list_l>();
}
requested_srs_precfg_characteristics_list_l&
positioning_info_request_ies_o::value_c::requested_srs_precfg_characteristics_list()
{
  assert_choice_type(types::requested_srs_precfg_characteristics_list, type_, "Value");
  return c.get<requested_srs_precfg_characteristics_list_l>();
}
remote_ue_ind_request_e& positioning_info_request_ies_o::value_c::remote_ue_ind_request()
{
  assert_choice_type(types::remote_ue_ind_request, type_, "Value");
  return c.get<remote_ue_ind_request_e>();
}
const requested_srs_tx_characteristics_s&
positioning_info_request_ies_o::value_c::requested_srs_tx_characteristics() const
{
  assert_choice_type(types::requested_srs_tx_characteristics, type_, "Value");
  return c.get<requested_srs_tx_characteristics_s>();
}
const ue_report_info_s& positioning_info_request_ies_o::value_c::ue_report_info() const
{
  assert_choice_type(types::ue_report_info, type_, "Value");
  return c.get<ue_report_info_s>();
}
const ue_teg_info_request_e& positioning_info_request_ies_o::value_c::ue_teg_info_request() const
{
  assert_choice_type(types::ue_teg_info_request, type_, "Value");
  return c.get<ue_teg_info_request_e>();
}
const ue_teg_report_periodicity_e& positioning_info_request_ies_o::value_c::ue_teg_report_periodicity() const
{
  assert_choice_type(types::ue_teg_report_periodicity, type_, "Value");
  return c.get<ue_teg_report_periodicity_e>();
}
const time_win_info_srs_list_l& positioning_info_request_ies_o::value_c::time_win_info_srs_list() const
{
  assert_choice_type(types::time_win_info_srs_list, type_, "Value");
  return c.get<time_win_info_srs_list_l>();
}
const requested_srs_precfg_characteristics_list_l&
positioning_info_request_ies_o::value_c::requested_srs_precfg_characteristics_list() const
{
  assert_choice_type(types::requested_srs_precfg_characteristics_list, type_, "Value");
  return c.get<requested_srs_precfg_characteristics_list_l>();
}
const remote_ue_ind_request_e& positioning_info_request_ies_o::value_c::remote_ue_ind_request() const
{
  assert_choice_type(types::remote_ue_ind_request, type_, "Value");
  return c.get<remote_ue_ind_request_e>();
}
void positioning_info_request_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::requested_srs_tx_characteristics:
      j.write_fieldname("RequestedSRSTransmissionCharacteristics");
      c.get<requested_srs_tx_characteristics_s>().to_json(j);
      break;
    case types::ue_report_info:
      j.write_fieldname("UEReportingInformation");
      c.get<ue_report_info_s>().to_json(j);
      break;
    case types::ue_teg_info_request:
      j.write_str("UE-TEG-Info-Request", c.get<ue_teg_info_request_e>().to_string());
      break;
    case types::ue_teg_report_periodicity:
      j.write_str("UE-TEG-ReportingPeriodicity", c.get<ue_teg_report_periodicity_e>().to_string());
      break;
    case types::time_win_info_srs_list:
      j.start_array("TimeWindowInformation-SRS-List");
      for (const auto& e1 : c.get<time_win_info_srs_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::requested_srs_precfg_characteristics_list:
      j.start_array("RequestedSRSPreconfigurationCharacteristics-List");
      for (const auto& e1 : c.get<requested_srs_precfg_characteristics_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::remote_ue_ind_request:
      j.write_str("Remote-UE-Indication-Request", "true");
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_request_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE positioning_info_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::requested_srs_tx_characteristics:
      HANDLE_CODE(c.get<requested_srs_tx_characteristics_s>().pack(bref));
      break;
    case types::ue_report_info:
      HANDLE_CODE(c.get<ue_report_info_s>().pack(bref));
      break;
    case types::ue_teg_info_request:
      HANDLE_CODE(c.get<ue_teg_info_request_e>().pack(bref));
      break;
    case types::ue_teg_report_periodicity:
      HANDLE_CODE(c.get<ue_teg_report_periodicity_e>().pack(bref));
      break;
    case types::time_win_info_srs_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<time_win_info_srs_list_l>(), 1, 16, true));
      break;
    case types::requested_srs_precfg_characteristics_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<requested_srs_precfg_characteristics_list_l>(), 1, 16, true));
      break;
    case types::remote_ue_ind_request:
      HANDLE_CODE(c.get<remote_ue_ind_request_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::requested_srs_tx_characteristics:
      HANDLE_CODE(c.get<requested_srs_tx_characteristics_s>().unpack(bref));
      break;
    case types::ue_report_info:
      HANDLE_CODE(c.get<ue_report_info_s>().unpack(bref));
      break;
    case types::ue_teg_info_request:
      HANDLE_CODE(c.get<ue_teg_info_request_e>().unpack(bref));
      break;
    case types::ue_teg_report_periodicity:
      HANDLE_CODE(c.get<ue_teg_report_periodicity_e>().unpack(bref));
      break;
    case types::time_win_info_srs_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<time_win_info_srs_list_l>(), bref, 1, 16, true));
      break;
    case types::requested_srs_precfg_characteristics_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<requested_srs_precfg_characteristics_list_l>(), bref, 1, 16, true));
      break;
    case types::remote_ue_ind_request:
      HANDLE_CODE(c.get<remote_ue_ind_request_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* positioning_info_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"RequestedSRSTransmissionCharacteristics",
                                "UEReportingInformation",
                                "UE-TEG-Info-Request",
                                "UE-TEG-ReportingPeriodicity",
                                "TimeWindowInformation-SRS-List",
                                "RequestedSRSPreconfigurationCharacteristics-List",
                                "Remote-UE-Indication-Request"};
  return convert_enum_idx(names, 7, value, "positioning_info_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<positioning_info_request_ies_o>;

OCUDUASN_CODE positioning_info_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += requested_srs_tx_characteristics_present ? 1 : 0;
  nof_ies += ue_report_info_present ? 1 : 0;
  nof_ies += ue_teg_info_request_present ? 1 : 0;
  nof_ies += ue_teg_report_periodicity_present ? 1 : 0;
  nof_ies += time_win_info_srs_list_present ? 1 : 0;
  nof_ies += requested_srs_precfg_characteristics_list_present ? 1 : 0;
  nof_ies += remote_ue_ind_request_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (requested_srs_tx_characteristics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)12, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(requested_srs_tx_characteristics.pack(bref));
  }
  if (ue_report_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)73, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ue_report_info.pack(bref));
  }
  if (ue_teg_info_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)90, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ue_teg_info_request.pack(bref));
  }
  if (ue_teg_report_periodicity_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)99, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ue_teg_report_periodicity.pack(bref));
  }
  if (time_win_info_srs_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)124, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, time_win_info_srs_list, 1, 16, true));
  }
  if (requested_srs_precfg_characteristics_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)136, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, requested_srs_precfg_characteristics_list, 1, 16, true));
  }
  if (remote_ue_ind_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)159, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(remote_ue_ind_request.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_info_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 12: {
        requested_srs_tx_characteristics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(requested_srs_tx_characteristics.unpack(bref));
        break;
      }
      case 73: {
        ue_report_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ue_report_info.unpack(bref));
        break;
      }
      case 90: {
        ue_teg_info_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ue_teg_info_request.unpack(bref));
        break;
      }
      case 99: {
        ue_teg_report_periodicity_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ue_teg_report_periodicity.unpack(bref));
        break;
      }
      case 124: {
        time_win_info_srs_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(time_win_info_srs_list, bref, 1, 16, true));
        break;
      }
      case 136: {
        requested_srs_precfg_characteristics_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(requested_srs_precfg_characteristics_list, bref, 1, 16, true));
        break;
      }
      case 159: {
        remote_ue_ind_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(remote_ue_ind_request.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void positioning_info_request_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (requested_srs_tx_characteristics_present) {
    j.write_int("id", 12);
    j.write_str("criticality", "ignore");
    requested_srs_tx_characteristics.to_json(j);
  }
  if (ue_report_info_present) {
    j.write_int("id", 73);
    j.write_str("criticality", "ignore");
    ue_report_info.to_json(j);
  }
  if (ue_teg_info_request_present) {
    j.write_int("id", 90);
    j.write_str("criticality", "ignore");
    j.write_str("Value", ue_teg_info_request.to_string());
  }
  if (ue_teg_report_periodicity_present) {
    j.write_int("id", 99);
    j.write_str("criticality", "reject");
    j.write_str("Value", ue_teg_report_periodicity.to_string());
  }
  if (time_win_info_srs_list_present) {
    j.write_int("id", 124);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : time_win_info_srs_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (requested_srs_precfg_characteristics_list_present) {
    j.write_int("id", 136);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : requested_srs_precfg_characteristics_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (remote_ue_ind_request_present) {
    j.write_int("id", 159);
    j.write_str("criticality", "ignore");
    j.write_str("Value", "true");
  }
  j.end_obj();
}

// PositioningInformationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t positioning_info_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {26, 54, 1, 81, 113, 128, 137, 160};
  return map_enum_number(names, 8, idx, "id");
}
bool positioning_info_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {26, 54, 1, 81, 113, 128, 137, 160};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_info_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 26:
      return crit_e::ignore;
    case 54:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    case 81:
      return crit_e::ignore;
    case 113:
      return crit_e::ignore;
    case 128:
      return crit_e::ignore;
    case 137:
      return crit_e::ignore;
    case 160:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
positioning_info_resp_ies_o::value_c positioning_info_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 26:
      ret.set(value_c::types::srs_cfg);
      break;
    case 54:
      ret.set(value_c::types::sfn_initisation_time);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    case 81:
      ret.set(value_c::types::ue_tx_teg_assoc_list);
      break;
    case 113:
      ret.set(value_c::types::new_nr_cgi);
      break;
    case 128:
      ret.set(value_c::types::pos_validity_area_cell_list);
      break;
    case 137:
      ret.set(value_c::types::srs_precfg_list);
      break;
    case 160:
      ret.set(value_c::types::remote_ue_ind);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e positioning_info_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 26:
      return presence_e::optional;
    case 54:
      return presence_e::optional;
    case 1:
      return presence_e::optional;
    case 81:
      return presence_e::optional;
    case 113:
      return presence_e::optional;
    case 128:
      return presence_e::optional;
    case 137:
      return presence_e::optional;
    case 160:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void positioning_info_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::srs_cfg:
      c = srs_configuration_s{};
      break;
    case types::sfn_initisation_time:
      c = fixed_bitstring<64, false, true>{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::ue_tx_teg_assoc_list:
      c = ue_tx_teg_assoc_list_l{};
      break;
    case types::new_nr_cgi:
      c = cgi_nr_s{};
      break;
    case types::pos_validity_area_cell_list:
      c = pos_validity_area_cell_list_l{};
      break;
    case types::srs_precfg_list:
      c = srs_precfg_list_l{};
      break;
    case types::remote_ue_ind:
      c = remote_ue_ind_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_resp_ies_o::value_c");
  }
}
srs_configuration_s& positioning_info_resp_ies_o::value_c::srs_cfg()
{
  assert_choice_type(types::srs_cfg, type_, "Value");
  return c.get<srs_configuration_s>();
}
fixed_bitstring<64, false, true>& positioning_info_resp_ies_o::value_c::sfn_initisation_time()
{
  assert_choice_type(types::sfn_initisation_time, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
crit_diagnostics_s& positioning_info_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
ue_tx_teg_assoc_list_l& positioning_info_resp_ies_o::value_c::ue_tx_teg_assoc_list()
{
  assert_choice_type(types::ue_tx_teg_assoc_list, type_, "Value");
  return c.get<ue_tx_teg_assoc_list_l>();
}
cgi_nr_s& positioning_info_resp_ies_o::value_c::new_nr_cgi()
{
  assert_choice_type(types::new_nr_cgi, type_, "Value");
  return c.get<cgi_nr_s>();
}
pos_validity_area_cell_list_l& positioning_info_resp_ies_o::value_c::pos_validity_area_cell_list()
{
  assert_choice_type(types::pos_validity_area_cell_list, type_, "Value");
  return c.get<pos_validity_area_cell_list_l>();
}
srs_precfg_list_l& positioning_info_resp_ies_o::value_c::srs_precfg_list()
{
  assert_choice_type(types::srs_precfg_list, type_, "Value");
  return c.get<srs_precfg_list_l>();
}
remote_ue_ind_e& positioning_info_resp_ies_o::value_c::remote_ue_ind()
{
  assert_choice_type(types::remote_ue_ind, type_, "Value");
  return c.get<remote_ue_ind_e>();
}
const srs_configuration_s& positioning_info_resp_ies_o::value_c::srs_cfg() const
{
  assert_choice_type(types::srs_cfg, type_, "Value");
  return c.get<srs_configuration_s>();
}
const fixed_bitstring<64, false, true>& positioning_info_resp_ies_o::value_c::sfn_initisation_time() const
{
  assert_choice_type(types::sfn_initisation_time, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
const crit_diagnostics_s& positioning_info_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const ue_tx_teg_assoc_list_l& positioning_info_resp_ies_o::value_c::ue_tx_teg_assoc_list() const
{
  assert_choice_type(types::ue_tx_teg_assoc_list, type_, "Value");
  return c.get<ue_tx_teg_assoc_list_l>();
}
const cgi_nr_s& positioning_info_resp_ies_o::value_c::new_nr_cgi() const
{
  assert_choice_type(types::new_nr_cgi, type_, "Value");
  return c.get<cgi_nr_s>();
}
const pos_validity_area_cell_list_l& positioning_info_resp_ies_o::value_c::pos_validity_area_cell_list() const
{
  assert_choice_type(types::pos_validity_area_cell_list, type_, "Value");
  return c.get<pos_validity_area_cell_list_l>();
}
const srs_precfg_list_l& positioning_info_resp_ies_o::value_c::srs_precfg_list() const
{
  assert_choice_type(types::srs_precfg_list, type_, "Value");
  return c.get<srs_precfg_list_l>();
}
const remote_ue_ind_e& positioning_info_resp_ies_o::value_c::remote_ue_ind() const
{
  assert_choice_type(types::remote_ue_ind, type_, "Value");
  return c.get<remote_ue_ind_e>();
}
void positioning_info_resp_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srs_cfg:
      j.write_fieldname("SRSConfiguration");
      c.get<srs_configuration_s>().to_json(j);
      break;
    case types::sfn_initisation_time:
      j.write_str("BIT STRING", c.get<fixed_bitstring<64, false, true>>().to_string());
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    case types::ue_tx_teg_assoc_list:
      j.start_array("UETxTEGAssociationList");
      for (const auto& e1 : c.get<ue_tx_teg_assoc_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::new_nr_cgi:
      j.write_fieldname("CGI-NR");
      c.get<cgi_nr_s>().to_json(j);
      break;
    case types::pos_validity_area_cell_list:
      j.start_array("PosValidityAreaCellList");
      for (const auto& e1 : c.get<pos_validity_area_cell_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::srs_precfg_list:
      j.start_array("SRSPreconfiguration-List");
      for (const auto& e1 : c.get<srs_precfg_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::remote_ue_ind:
      j.write_str("Remote-UE-Indication", "l2-u2n-remote-ue");
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_resp_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE positioning_info_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_cfg:
      HANDLE_CODE(c.get<srs_configuration_s>().pack(bref));
      break;
    case types::sfn_initisation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    case types::ue_tx_teg_assoc_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ue_tx_teg_assoc_list_l>(), 1, 256, true));
      break;
    case types::new_nr_cgi:
      HANDLE_CODE(c.get<cgi_nr_s>().pack(bref));
      break;
    case types::pos_validity_area_cell_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<pos_validity_area_cell_list_l>(), 1, 32, true));
      break;
    case types::srs_precfg_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<srs_precfg_list_l>(), 1, 16, true));
      break;
    case types::remote_ue_ind:
      HANDLE_CODE(c.get<remote_ue_ind_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_cfg:
      HANDLE_CODE(c.get<srs_configuration_s>().unpack(bref));
      break;
    case types::sfn_initisation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    case types::ue_tx_teg_assoc_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ue_tx_teg_assoc_list_l>(), bref, 1, 256, true));
      break;
    case types::new_nr_cgi:
      HANDLE_CODE(c.get<cgi_nr_s>().unpack(bref));
      break;
    case types::pos_validity_area_cell_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<pos_validity_area_cell_list_l>(), bref, 1, 32, true));
      break;
    case types::srs_precfg_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<srs_precfg_list_l>(), bref, 1, 16, true));
      break;
    case types::remote_ue_ind:
      HANDLE_CODE(c.get<remote_ue_ind_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* positioning_info_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"SRSConfiguration",
                                "BIT STRING",
                                "CriticalityDiagnostics",
                                "UETxTEGAssociationList",
                                "CGI-NR",
                                "PosValidityAreaCellList",
                                "SRSPreconfiguration-List",
                                "Remote-UE-Indication"};
  return convert_enum_idx(names, 8, value, "positioning_info_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<positioning_info_resp_ies_o>;

OCUDUASN_CODE positioning_info_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += sfn_initisation_time_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += ue_tx_teg_assoc_list_present ? 1 : 0;
  nof_ies += new_nr_cgi_present ? 1 : 0;
  nof_ies += pos_validity_area_cell_list_present ? 1 : 0;
  nof_ies += srs_precfg_list_present ? 1 : 0;
  nof_ies += remote_ue_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (srs_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)26, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_cfg.pack(bref));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)54, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sfn_initisation_time.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (ue_tx_teg_assoc_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)81, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ue_tx_teg_assoc_list, 1, 256, true));
  }
  if (new_nr_cgi_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)113, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(new_nr_cgi.pack(bref));
  }
  if (pos_validity_area_cell_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)128, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_validity_area_cell_list, 1, 32, true));
  }
  if (srs_precfg_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)137, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, srs_precfg_list, 1, 16, true));
  }
  if (remote_ue_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)160, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(remote_ue_ind.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_info_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 26: {
        srs_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_cfg.unpack(bref));
        break;
      }
      case 54: {
        sfn_initisation_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sfn_initisation_time.unpack(bref));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 81: {
        ue_tx_teg_assoc_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ue_tx_teg_assoc_list, bref, 1, 256, true));
        break;
      }
      case 113: {
        new_nr_cgi_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(new_nr_cgi.unpack(bref));
        break;
      }
      case 128: {
        pos_validity_area_cell_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pos_validity_area_cell_list, bref, 1, 32, true));
        break;
      }
      case 137: {
        srs_precfg_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(srs_precfg_list, bref, 1, 16, true));
        break;
      }
      case 160: {
        remote_ue_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(remote_ue_ind.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void positioning_info_resp_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_cfg_present) {
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    srs_cfg.to_json(j);
  }
  if (sfn_initisation_time_present) {
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.write_str("Value", sfn_initisation_time.to_string());
  }
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  if (ue_tx_teg_assoc_list_present) {
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : ue_tx_teg_assoc_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (new_nr_cgi_present) {
    j.write_int("id", 113);
    j.write_str("criticality", "ignore");
    new_nr_cgi.to_json(j);
  }
  if (pos_validity_area_cell_list_present) {
    j.write_int("id", 128);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : pos_validity_area_cell_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (srs_precfg_list_present) {
    j.write_int("id", 137);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : srs_precfg_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (remote_ue_ind_present) {
    j.write_int("id", 160);
    j.write_str("criticality", "ignore");
    j.write_str("Value", "l2-u2n-remote-ue");
  }
  j.end_obj();
}

// PositioningInformationUpdate-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t positioning_info_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {26, 54, 81, 106, 134, 161};
  return map_enum_number(names, 6, idx, "id");
}
bool positioning_info_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {26, 54, 81, 106, 134, 161};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_info_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 26:
      return crit_e::ignore;
    case 54:
      return crit_e::ignore;
    case 81:
      return crit_e::ignore;
    case 106:
      return crit_e::ignore;
    case 134:
      return crit_e::ignore;
    case 161:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
positioning_info_upd_ies_o::value_c positioning_info_upd_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 26:
      ret.set(value_c::types::srs_cfg);
      break;
    case 54:
      ret.set(value_c::types::sfn_initisation_time);
      break;
    case 81:
      ret.set(value_c::types::ue_tx_teg_assoc_list);
      break;
    case 106:
      ret.set(value_c::types::srs_tx_status);
      break;
    case 134:
      ret.set(value_c::types::new_cell_id);
      break;
    case 161:
      ret.set(value_c::types::remote_ue_status);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e positioning_info_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 26:
      return presence_e::optional;
    case 54:
      return presence_e::optional;
    case 81:
      return presence_e::optional;
    case 106:
      return presence_e::optional;
    case 134:
      return presence_e::optional;
    case 161:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void positioning_info_upd_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::srs_cfg:
      c = srs_configuration_s{};
      break;
    case types::sfn_initisation_time:
      c = fixed_bitstring<64, false, true>{};
      break;
    case types::ue_tx_teg_assoc_list:
      c = ue_tx_teg_assoc_list_l{};
      break;
    case types::srs_tx_status:
      c = srs_tx_status_e{};
      break;
    case types::new_cell_id:
      c = cgi_nr_s{};
      break;
    case types::remote_ue_status:
      c = remote_ue_status_e{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_upd_ies_o::value_c");
  }
}
srs_configuration_s& positioning_info_upd_ies_o::value_c::srs_cfg()
{
  assert_choice_type(types::srs_cfg, type_, "Value");
  return c.get<srs_configuration_s>();
}
fixed_bitstring<64, false, true>& positioning_info_upd_ies_o::value_c::sfn_initisation_time()
{
  assert_choice_type(types::sfn_initisation_time, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
ue_tx_teg_assoc_list_l& positioning_info_upd_ies_o::value_c::ue_tx_teg_assoc_list()
{
  assert_choice_type(types::ue_tx_teg_assoc_list, type_, "Value");
  return c.get<ue_tx_teg_assoc_list_l>();
}
srs_tx_status_e& positioning_info_upd_ies_o::value_c::srs_tx_status()
{
  assert_choice_type(types::srs_tx_status, type_, "Value");
  return c.get<srs_tx_status_e>();
}
cgi_nr_s& positioning_info_upd_ies_o::value_c::new_cell_id()
{
  assert_choice_type(types::new_cell_id, type_, "Value");
  return c.get<cgi_nr_s>();
}
remote_ue_status_e& positioning_info_upd_ies_o::value_c::remote_ue_status()
{
  assert_choice_type(types::remote_ue_status, type_, "Value");
  return c.get<remote_ue_status_e>();
}
const srs_configuration_s& positioning_info_upd_ies_o::value_c::srs_cfg() const
{
  assert_choice_type(types::srs_cfg, type_, "Value");
  return c.get<srs_configuration_s>();
}
const fixed_bitstring<64, false, true>& positioning_info_upd_ies_o::value_c::sfn_initisation_time() const
{
  assert_choice_type(types::sfn_initisation_time, type_, "Value");
  return c.get<fixed_bitstring<64, false, true>>();
}
const ue_tx_teg_assoc_list_l& positioning_info_upd_ies_o::value_c::ue_tx_teg_assoc_list() const
{
  assert_choice_type(types::ue_tx_teg_assoc_list, type_, "Value");
  return c.get<ue_tx_teg_assoc_list_l>();
}
const srs_tx_status_e& positioning_info_upd_ies_o::value_c::srs_tx_status() const
{
  assert_choice_type(types::srs_tx_status, type_, "Value");
  return c.get<srs_tx_status_e>();
}
const cgi_nr_s& positioning_info_upd_ies_o::value_c::new_cell_id() const
{
  assert_choice_type(types::new_cell_id, type_, "Value");
  return c.get<cgi_nr_s>();
}
const remote_ue_status_e& positioning_info_upd_ies_o::value_c::remote_ue_status() const
{
  assert_choice_type(types::remote_ue_status, type_, "Value");
  return c.get<remote_ue_status_e>();
}
void positioning_info_upd_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srs_cfg:
      j.write_fieldname("SRSConfiguration");
      c.get<srs_configuration_s>().to_json(j);
      break;
    case types::sfn_initisation_time:
      j.write_str("BIT STRING", c.get<fixed_bitstring<64, false, true>>().to_string());
      break;
    case types::ue_tx_teg_assoc_list:
      j.start_array("UETxTEGAssociationList");
      for (const auto& e1 : c.get<ue_tx_teg_assoc_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::srs_tx_status:
      j.write_str("SRSTransmissionStatus", c.get<srs_tx_status_e>().to_string());
      break;
    case types::new_cell_id:
      j.write_fieldname("CGI-NR");
      c.get<cgi_nr_s>().to_json(j);
      break;
    case types::remote_ue_status:
      j.write_str("Remote-UE-Status", c.get<remote_ue_status_e>().to_string());
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_upd_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE positioning_info_upd_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_cfg:
      HANDLE_CODE(c.get<srs_configuration_s>().pack(bref));
      break;
    case types::sfn_initisation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().pack(bref)));
      break;
    case types::ue_tx_teg_assoc_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ue_tx_teg_assoc_list_l>(), 1, 256, true));
      break;
    case types::srs_tx_status:
      HANDLE_CODE(c.get<srs_tx_status_e>().pack(bref));
      break;
    case types::new_cell_id:
      HANDLE_CODE(c.get<cgi_nr_s>().pack(bref));
      break;
    case types::remote_ue_status:
      HANDLE_CODE(c.get<remote_ue_status_e>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_upd_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_upd_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_cfg:
      HANDLE_CODE(c.get<srs_configuration_s>().unpack(bref));
      break;
    case types::sfn_initisation_time:
      HANDLE_CODE((c.get<fixed_bitstring<64, false, true>>().unpack(bref)));
      break;
    case types::ue_tx_teg_assoc_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ue_tx_teg_assoc_list_l>(), bref, 1, 256, true));
      break;
    case types::srs_tx_status:
      HANDLE_CODE(c.get<srs_tx_status_e>().unpack(bref));
      break;
    case types::new_cell_id:
      HANDLE_CODE(c.get<cgi_nr_s>().unpack(bref));
      break;
    case types::remote_ue_status:
      HANDLE_CODE(c.get<remote_ue_status_e>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "positioning_info_upd_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* positioning_info_upd_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"SRSConfiguration",
                                "BIT STRING",
                                "UETxTEGAssociationList",
                                "SRSTransmissionStatus",
                                "CGI-NR",
                                "Remote-UE-Status"};
  return convert_enum_idx(names, 6, value, "positioning_info_upd_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<positioning_info_upd_ies_o>;

OCUDUASN_CODE positioning_info_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += sfn_initisation_time_present ? 1 : 0;
  nof_ies += ue_tx_teg_assoc_list_present ? 1 : 0;
  nof_ies += srs_tx_status_present ? 1 : 0;
  nof_ies += new_cell_id_present ? 1 : 0;
  nof_ies += remote_ue_status_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (srs_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)26, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_cfg.pack(bref));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)54, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sfn_initisation_time.pack(bref));
  }
  if (ue_tx_teg_assoc_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)81, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ue_tx_teg_assoc_list, 1, 256, true));
  }
  if (srs_tx_status_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)106, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_tx_status.pack(bref));
  }
  if (new_cell_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)134, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(new_cell_id.pack(bref));
  }
  if (remote_ue_status_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)161, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(remote_ue_status.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_info_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 26: {
        srs_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_cfg.unpack(bref));
        break;
      }
      case 54: {
        sfn_initisation_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sfn_initisation_time.unpack(bref));
        break;
      }
      case 81: {
        ue_tx_teg_assoc_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ue_tx_teg_assoc_list, bref, 1, 256, true));
        break;
      }
      case 106: {
        srs_tx_status_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_tx_status.unpack(bref));
        break;
      }
      case 134: {
        new_cell_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(new_cell_id.unpack(bref));
        break;
      }
      case 161: {
        remote_ue_status_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(remote_ue_status.unpack(bref));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void positioning_info_upd_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (srs_cfg_present) {
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    srs_cfg.to_json(j);
  }
  if (sfn_initisation_time_present) {
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.write_str("Value", sfn_initisation_time.to_string());
  }
  if (ue_tx_teg_assoc_list_present) {
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : ue_tx_teg_assoc_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  if (srs_tx_status_present) {
    j.write_int("id", 106);
    j.write_str("criticality", "ignore");
    j.write_str("Value", srs_tx_status.to_string());
  }
  if (new_cell_id_present) {
    j.write_int("id", 134);
    j.write_str("criticality", "ignore");
    new_cell_id.to_json(j);
  }
  if (remote_ue_status_present) {
    j.write_int("id", 161);
    j.write_str("criticality", "ignore");
    j.write_str("Value", remote_ue_status.to_string());
  }
  j.end_obj();
}

// Value ::= OPEN TYPE
void nr_ppa_private_ies_empty_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.end_obj();
}
OCUDUASN_CODE nr_ppa_private_ies_empty_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE nr_ppa_private_ies_empty_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}

const char* nr_ppa_private_ies_empty_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {};
  return convert_enum_idx(names, 0, value, "nr_ppa_private_ies_empty_o::value_c::types");
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
  j.start_obj();
  j.end_obj();
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

// SRSInformationReservationNotification-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t srs_info_reserv_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {129, 138, 163};
  return map_enum_number(names, 3, idx, "id");
}
bool srs_info_reserv_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {129, 138, 163};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e srs_info_reserv_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 129:
      return crit_e::reject;
    case 138:
      return crit_e::ignore;
    case 163:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
srs_info_reserv_notif_ies_o::value_c srs_info_reserv_notif_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 129:
      ret.set(value_c::types::srs_reserv_type);
      break;
    case 138:
      ret.set(value_c::types::srs_info);
      break;
    case 163:
      ret.set(value_c::types::precfg_srs_info);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e srs_info_reserv_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 129:
      return presence_e::mandatory;
    case 138:
      return presence_e::optional;
    case 163:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void srs_info_reserv_notif_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::srs_reserv_type:
      c = srs_reserv_type_e{};
      break;
    case types::srs_info:
      c = requested_srs_tx_characteristics_s{};
      break;
    case types::precfg_srs_info:
      c = requested_srs_precfg_characteristics_list_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "srs_info_reserv_notif_ies_o::value_c");
  }
}
srs_reserv_type_e& srs_info_reserv_notif_ies_o::value_c::srs_reserv_type()
{
  assert_choice_type(types::srs_reserv_type, type_, "Value");
  return c.get<srs_reserv_type_e>();
}
requested_srs_tx_characteristics_s& srs_info_reserv_notif_ies_o::value_c::srs_info()
{
  assert_choice_type(types::srs_info, type_, "Value");
  return c.get<requested_srs_tx_characteristics_s>();
}
requested_srs_precfg_characteristics_list_l& srs_info_reserv_notif_ies_o::value_c::precfg_srs_info()
{
  assert_choice_type(types::precfg_srs_info, type_, "Value");
  return c.get<requested_srs_precfg_characteristics_list_l>();
}
const srs_reserv_type_e& srs_info_reserv_notif_ies_o::value_c::srs_reserv_type() const
{
  assert_choice_type(types::srs_reserv_type, type_, "Value");
  return c.get<srs_reserv_type_e>();
}
const requested_srs_tx_characteristics_s& srs_info_reserv_notif_ies_o::value_c::srs_info() const
{
  assert_choice_type(types::srs_info, type_, "Value");
  return c.get<requested_srs_tx_characteristics_s>();
}
const requested_srs_precfg_characteristics_list_l& srs_info_reserv_notif_ies_o::value_c::precfg_srs_info() const
{
  assert_choice_type(types::precfg_srs_info, type_, "Value");
  return c.get<requested_srs_precfg_characteristics_list_l>();
}
void srs_info_reserv_notif_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::srs_reserv_type:
      j.write_str("SRSReservationType", c.get<srs_reserv_type_e>().to_string());
      break;
    case types::srs_info:
      j.write_fieldname("RequestedSRSTransmissionCharacteristics");
      c.get<requested_srs_tx_characteristics_s>().to_json(j);
      break;
    case types::precfg_srs_info:
      j.start_array("RequestedSRSPreconfigurationCharacteristics-List");
      for (const auto& e1 : c.get<requested_srs_precfg_characteristics_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "srs_info_reserv_notif_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE srs_info_reserv_notif_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_reserv_type:
      HANDLE_CODE(c.get<srs_reserv_type_e>().pack(bref));
      break;
    case types::srs_info:
      HANDLE_CODE(c.get<requested_srs_tx_characteristics_s>().pack(bref));
      break;
    case types::precfg_srs_info:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<requested_srs_precfg_characteristics_list_l>(), 1, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "srs_info_reserv_notif_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_info_reserv_notif_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::srs_reserv_type:
      HANDLE_CODE(c.get<srs_reserv_type_e>().unpack(bref));
      break;
    case types::srs_info:
      HANDLE_CODE(c.get<requested_srs_tx_characteristics_s>().unpack(bref));
      break;
    case types::precfg_srs_info:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<requested_srs_precfg_characteristics_list_l>(), bref, 1, 16, true));
      break;
    default:
      log_invalid_choice_id(type_, "srs_info_reserv_notif_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* srs_info_reserv_notif_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"SRSReservationType",
                                "RequestedSRSTransmissionCharacteristics",
                                "RequestedSRSPreconfigurationCharacteristics-List"};
  return convert_enum_idx(names, 3, value, "srs_info_reserv_notif_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<srs_info_reserv_notif_ies_o>;

OCUDUASN_CODE srs_info_reserv_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += srs_info_present ? 1 : 0;
  nof_ies += precfg_srs_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)129, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_reserv_type.pack(bref));
  }
  if (srs_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)138, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_info.pack(bref));
  }
  if (precfg_srs_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)163, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, precfg_srs_info, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE srs_info_reserv_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != srs_info_reserv_notif_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 129: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_reserv_type.unpack(bref));
        break;
      }
      case 138: {
        srs_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_info.unpack(bref));
        break;
      }
      case 163: {
        precfg_srs_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(precfg_srs_info, bref, 1, 16, true));
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
void srs_info_reserv_notif_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 129);
  j.write_str("criticality", "reject");
  j.write_str("Value", srs_reserv_type.to_string());
  if (srs_info_present) {
    j.write_int("id", 138);
    j.write_str("criticality", "ignore");
    srs_info.to_json(j);
  }
  if (precfg_srs_info_present) {
    j.write_int("id", 163);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : precfg_srs_info) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.end_obj();
}

// TRPInformationFailure-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t trp_info_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {0, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool trp_info_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {0, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trp_info_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 0:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
trp_info_fail_ies_o::value_c trp_info_fail_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 0:
      ret.set(value_c::types::cause);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_info_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 0:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void trp_info_fail_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::cause:
      c = cause_c{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_fail_ies_o::value_c");
  }
}
cause_c& trp_info_fail_ies_o::value_c::cause()
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
crit_diagnostics_s& trp_info_fail_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const cause_c& trp_info_fail_ies_o::value_c::cause() const
{
  assert_choice_type(types::cause, type_, "Value");
  return c.get<cause_c>();
}
const crit_diagnostics_s& trp_info_fail_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void trp_info_fail_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::cause:
      j.write_fieldname("Cause");
      c.get<cause_c>().to_json(j);
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_fail_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_info_fail_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().pack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_fail_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_fail_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::cause:
      HANDLE_CODE(c.get<cause_c>().unpack(bref));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_fail_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_info_fail_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"Cause", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "trp_info_fail_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<trp_info_fail_ies_o>;

OCUDUASN_CODE trp_info_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_info_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 1: {
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
void trp_info_fail_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  cause.to_json(j);
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}

// TRPInformationRequest-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t trp_info_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {47, 29};
  return map_enum_number(names, 2, idx, "id");
}
bool trp_info_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {47, 29};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trp_info_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 47:
      return crit_e::ignore;
    case 29:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
trp_info_request_ies_o::value_c trp_info_request_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 47:
      ret.set(value_c::types::trp_list);
      break;
    case 29:
      ret.set(value_c::types::trp_info_type_list_trp_req);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_info_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 47:
      return presence_e::optional;
    case 29:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void trp_info_request_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::trp_list:
      c = trp_list_l{};
      break;
    case types::trp_info_type_list_trp_req:
      c = trp_info_type_list_trp_req_l{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_request_ies_o::value_c");
  }
}
trp_list_l& trp_info_request_ies_o::value_c::trp_list()
{
  assert_choice_type(types::trp_list, type_, "Value");
  return c.get<trp_list_l>();
}
trp_info_type_list_trp_req_l& trp_info_request_ies_o::value_c::trp_info_type_list_trp_req()
{
  assert_choice_type(types::trp_info_type_list_trp_req, type_, "Value");
  return c.get<trp_info_type_list_trp_req_l>();
}
const trp_list_l& trp_info_request_ies_o::value_c::trp_list() const
{
  assert_choice_type(types::trp_list, type_, "Value");
  return c.get<trp_list_l>();
}
const trp_info_type_list_trp_req_l& trp_info_request_ies_o::value_c::trp_info_type_list_trp_req() const
{
  assert_choice_type(types::trp_info_type_list_trp_req, type_, "Value");
  return c.get<trp_info_type_list_trp_req_l>();
}
void trp_info_request_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::trp_list:
      j.start_array("TRPList");
      for (const auto& e1 : c.get<trp_list_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::trp_info_type_list_trp_req:
      j.start_array("TRPInformationTypeListTRPReq");
      for (const auto& e1 : c.get<trp_info_type_list_trp_req_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_request_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_info_request_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::trp_list:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_list_l>(), 1, 65535, true));
      break;
    case types::trp_info_type_list_trp_req:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_info_type_list_trp_req_l>(), 1, 64, true));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_request_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_request_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::trp_list:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_list_l>(), bref, 1, 65535, true));
      break;
    case types::trp_info_type_list_trp_req:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_info_type_list_trp_req_l>(), bref, 1, 64, true));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_request_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_info_request_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TRPList", "TRPInformationTypeListTRPReq"};
  return convert_enum_idx(names, 2, value, "trp_info_request_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<trp_info_request_ies_o>;

OCUDUASN_CODE trp_info_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += trp_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (trp_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)47, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_list, 1, 65535, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)29, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_info_type_list_trp_req, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_info_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 47: {
        trp_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_list, bref, 1, 65535, true));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_info_type_list_trp_req, bref, 1, 64, true));
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
void trp_info_request_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  if (trp_list_present) {
    j.write_int("id", 47);
    j.write_str("criticality", "ignore");
    j.start_array("Value");
    for (const auto& e1 : trp_list) {
      e1.to_json(j);
    }
    j.end_array();
  }
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.start_array("Value");
  for (const auto& e1 : trp_info_type_list_trp_req) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
}

// TRPInformationResponse-IEs ::= OBJECT SET OF NRPPA-PROTOCOL-IES
uint32_t trp_info_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {30, 1};
  return map_enum_number(names, 2, idx, "id");
}
bool trp_info_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {30, 1};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trp_info_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 30:
      return crit_e::ignore;
    case 1:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
trp_info_resp_ies_o::value_c trp_info_resp_ies_o::get_value(const uint32_t& id)
{
  value_c ret{};
  switch (id) {
    case 30:
      ret.set(value_c::types::trp_info_list_trp_resp);
      break;
    case 1:
      ret.set(value_c::types::crit_diagnostics);
      break;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_info_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 30:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

// Value ::= OPEN TYPE
void trp_info_resp_ies_o::value_c::set(types::options e)
{
  type_ = e;
  switch (type_) {
    case types::trp_info_list_trp_resp:
      c = trp_info_list_trp_resp_l{};
      break;
    case types::crit_diagnostics:
      c = crit_diagnostics_s{};
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_resp_ies_o::value_c");
  }
}
trp_info_list_trp_resp_l& trp_info_resp_ies_o::value_c::trp_info_list_trp_resp()
{
  assert_choice_type(types::trp_info_list_trp_resp, type_, "Value");
  return c.get<trp_info_list_trp_resp_l>();
}
crit_diagnostics_s& trp_info_resp_ies_o::value_c::crit_diagnostics()
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
const trp_info_list_trp_resp_l& trp_info_resp_ies_o::value_c::trp_info_list_trp_resp() const
{
  assert_choice_type(types::trp_info_list_trp_resp, type_, "Value");
  return c.get<trp_info_list_trp_resp_l>();
}
const crit_diagnostics_s& trp_info_resp_ies_o::value_c::crit_diagnostics() const
{
  assert_choice_type(types::crit_diagnostics, type_, "Value");
  return c.get<crit_diagnostics_s>();
}
void trp_info_resp_ies_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::trp_info_list_trp_resp:
      j.start_array("TRPInformationListTRPResp");
      for (const auto& e1 : c.get<trp_info_list_trp_resp_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::crit_diagnostics:
      j.write_fieldname("CriticalityDiagnostics");
      c.get<crit_diagnostics_s>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_resp_ies_o::value_c");
  }
  j.end_obj();
}
OCUDUASN_CODE trp_info_resp_ies_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::trp_info_list_trp_resp:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<trp_info_list_trp_resp_l>(), 1, 65535, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_resp_ies_o::value_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_resp_ies_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  switch (type_) {
    case types::trp_info_list_trp_resp:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<trp_info_list_trp_resp_l>(), bref, 1, 65535, true));
      break;
    case types::crit_diagnostics:
      HANDLE_CODE(c.get<crit_diagnostics_s>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "trp_info_resp_ies_o::value_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* trp_info_resp_ies_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TRPInformationListTRPResp", "CriticalityDiagnostics"};
  return convert_enum_idx(names, 2, value, "trp_info_resp_ies_o::value_c::types");
}

template struct asn1::protocol_ie_field_s<trp_info_resp_ies_o>;

OCUDUASN_CODE trp_info_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)30, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_info_list_trp_resp, 1, 65535, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_info_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 30: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_info_list_trp_resp, bref, 1, 65535, true));
        break;
      }
      case 1: {
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
void trp_info_resp_ies_container::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("id", 30);
  j.write_str("criticality", "ignore");
  j.start_array("Value");
  for (const auto& e1 : trp_info_list_trp_resp) {
    e1.to_json(j);
  }
  j.end_array();
  if (crit_diagnostics_present) {
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    crit_diagnostics.to_json(j);
  }
  j.end_obj();
}
