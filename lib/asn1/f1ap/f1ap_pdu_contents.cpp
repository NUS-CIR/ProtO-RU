// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/f1ap/f1ap_pdu_contents.h"

using namespace asn1;
using namespace asn1::f1ap;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// AccessAndMobilityIndicationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t access_and_mob_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 359, 360, 443, 736};
  return map_enum_number(names, 5, idx, "id");
}
bool access_and_mob_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 359, 360, 443, 736};
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
    case 78:
      return crit_e::reject;
    case 359:
      return crit_e::ignore;
    case 360:
      return crit_e::ignore;
    case 443:
      return crit_e::ignore;
    case 736:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e access_and_mob_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 359:
      return presence_e::optional;
    case 360:
      return presence_e::optional;
    case 443:
      return presence_e::optional;
    case 736:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE access_and_mob_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ra_report_list_present ? 1 : 0;
  nof_ies += rlf_report_info_list_present ? 1 : 0;
  nof_ies += successful_ho_report_info_list_present ? 1 : 0;
  nof_ies += successful_pscell_change_report_info_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ra_report_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)359, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ra_report_list, 1, 64, true));
  }
  if (rlf_report_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)360, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, rlf_report_info_list, 1, 64, true));
  }
  if (successful_ho_report_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)443, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, successful_ho_report_info_list, 1, 64, true));
  }
  if (successful_pscell_change_report_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)736, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, successful_pscell_change_report_info_list, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE access_and_mob_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != access_and_mob_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 359: {
        ra_report_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ra_report_list, bref, 1, 64, true));
        break;
      }
      case 360: {
        rlf_report_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(rlf_report_info_list, bref, 1, 64, true));
        break;
      }
      case 443: {
        successful_ho_report_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(successful_ho_report_info_list, bref, 1, 64, true));
        break;
      }
      case 736: {
        successful_pscell_change_report_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(successful_pscell_change_report_info_list, bref, 1, 64, true));
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
void access_and_mob_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ra_report_list_present) {
    j.start_obj();
    j.write_int("id", 359);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ra_report_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (rlf_report_info_list_present) {
    j.start_obj();
    j.write_int("id", 360);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : rlf_report_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (successful_ho_report_info_list_present) {
    j.start_obj();
    j.write_int("id", 443);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : successful_ho_report_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (successful_pscell_change_report_info_list_present) {
    j.start_obj();
    j.write_int("id", 736);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : successful_pscell_change_report_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// AccessSuccessIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t access_success_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 111};
  return map_enum_number(names, 3, idx, "id");
}
bool access_success_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 111};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e access_success_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 111:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e access_success_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 111:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE access_success_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)111, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nr_cgi.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE access_success_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != access_success_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 111: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nr_cgi.unpack(bref));
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
void access_success_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 111);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  nr_cgi.to_json(j);
  j.end_obj();
  j.end_array();
}

// AperiodicSRS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
uint32_t aperiodic_srs_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {829};
  return map_enum_number(names, 1, idx, "id");
}
bool aperiodic_srs_ext_ies_o::is_id_valid(const uint32_t& id)
{
  return 829 == id;
}
crit_e aperiodic_srs_ext_ies_o::get_crit(const uint32_t& id)
{
  if (id == 829) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e aperiodic_srs_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 829) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE aperiodic_srs_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += aggr_pos_srs_res_set_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (aggr_pos_srs_res_set_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)829, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, aggr_pos_srs_res_set_list, 1, 32, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aperiodic_srs_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != aperiodic_srs_ext_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 829) {
      {
        aggr_pos_srs_res_set_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(aggr_pos_srs_res_set_list, bref, 1, 32, true));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void aperiodic_srs_ext_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (aggr_pos_srs_res_set_list_present) {
    j.start_obj();
    j.write_int("id", 829);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : aggr_pos_srs_res_set_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// AperiodicSRS ::= SEQUENCE
OCUDUASN_CODE aperiodic_srs_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(srs_res_trigger_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(aperiodic.pack(bref));
  if (srs_res_trigger_present) {
    HANDLE_CODE(srs_res_trigger.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aperiodic_srs_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(srs_res_trigger_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(aperiodic.unpack(bref));
  if (srs_res_trigger_present) {
    HANDLE_CODE(srs_res_trigger.unpack(bref));
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
void aperiodic_srs_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_str("aperiodic", "true");
  if (srs_res_trigger_present) {
    j.write_fieldname("sRSResourceTrigger");
    srs_res_trigger.to_json(j);
  }
  if (ie_exts_present) {
    j.write_fieldname("iE-Extensions");
    ie_exts.to_json(j);
  }
  j.end_obj();
}

const char* aperiodic_srs_s::aperiodic_opts::to_string() const
{
  static const char* names[] = {"true"};
  return convert_enum_idx(names, 1, value, "aperiodic_srs_s::aperiodic_e_");
}

// BAPMappingConfiguration-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t bap_map_cfg_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 283, 285, 299, 512, 514, 516, 641};
  return map_enum_number(names, 8, idx, "id");
}
bool bap_map_cfg_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 283, 285, 299, 512, 514, 516, 641};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bap_map_cfg_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 283:
      return crit_e::ignore;
    case 285:
      return crit_e::ignore;
    case 299:
      return crit_e::ignore;
    case 512:
      return crit_e::ignore;
    case 514:
      return crit_e::ignore;
    case 516:
      return crit_e::ignore;
    case 641:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bap_map_cfg_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 283:
      return presence_e::optional;
    case 285:
      return presence_e::optional;
    case 299:
      return presence_e::optional;
    case 512:
      return presence_e::optional;
    case 514:
      return presence_e::optional;
    case 516:
      return presence_e::optional;
    case 641:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bap_map_cfg_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += bh_routing_info_added_list_present ? 1 : 0;
  nof_ies += bh_routing_info_remd_list_present ? 1 : 0;
  nof_ies += traffic_map_info_present ? 1 : 0;
  nof_ies += buffer_size_thresh_present ? 1 : 0;
  nof_ies += bap_hdr_rewriting_added_list_present ? 1 : 0;
  nof_ies += re_routing_enable_ind_present ? 1 : 0;
  nof_ies += bap_hdr_rewriting_remd_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (bh_routing_info_added_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)283, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, bh_routing_info_added_list, 1, 1024, true));
  }
  if (bh_routing_info_remd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)285, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, bh_routing_info_remd_list, 1, 1024, true));
  }
  if (traffic_map_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)299, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(traffic_map_info.pack(bref));
  }
  if (buffer_size_thresh_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)512, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, buffer_size_thresh, (uint32_t)0u, (uint32_t)16777215u, false, true));
  }
  if (bap_hdr_rewriting_added_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)514, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, bap_hdr_rewriting_added_list, 1, 1024, true));
  }
  if (re_routing_enable_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)516, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(re_routing_enable_ind.pack(bref));
  }
  if (bap_hdr_rewriting_remd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)641, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, bap_hdr_rewriting_remd_list, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bap_map_cfg_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != bap_map_cfg_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 283: {
        bh_routing_info_added_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(bh_routing_info_added_list, bref, 1, 1024, true));
        break;
      }
      case 285: {
        bh_routing_info_remd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(bh_routing_info_remd_list, bref, 1, 1024, true));
        break;
      }
      case 299: {
        traffic_map_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(traffic_map_info.unpack(bref));
        break;
      }
      case 512: {
        buffer_size_thresh_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(buffer_size_thresh, bref, (uint32_t)0u, (uint32_t)16777215u, false, true));
        break;
      }
      case 514: {
        bap_hdr_rewriting_added_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(bap_hdr_rewriting_added_list, bref, 1, 1024, true));
        break;
      }
      case 516: {
        re_routing_enable_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(re_routing_enable_ind.unpack(bref));
        break;
      }
      case 641: {
        bap_hdr_rewriting_remd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(bap_hdr_rewriting_remd_list, bref, 1, 1024, true));
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
void bap_map_cfg_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (bh_routing_info_added_list_present) {
    j.start_obj();
    j.write_int("id", 283);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bh_routing_info_added_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (bh_routing_info_remd_list_present) {
    j.start_obj();
    j.write_int("id", 285);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bh_routing_info_remd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (traffic_map_info_present) {
    j.start_obj();
    j.write_int("id", 299);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    traffic_map_info.to_json(j);
    j.end_obj();
  }
  if (buffer_size_thresh_present) {
    j.start_obj();
    j.write_int("id", 512);
    j.write_str("criticality", "ignore");
    j.write_int("value", buffer_size_thresh);
    j.end_obj();
  }
  if (bap_hdr_rewriting_added_list_present) {
    j.start_obj();
    j.write_int("id", 514);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bap_hdr_rewriting_added_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (re_routing_enable_ind_present) {
    j.start_obj();
    j.write_int("id", 516);
    j.write_str("criticality", "ignore");
    j.write_str("value", re_routing_enable_ind.to_string());
    j.end_obj();
  }
  if (bap_hdr_rewriting_remd_list_present) {
    j.start_obj();
    j.write_int("id", 641);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : bap_hdr_rewriting_remd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// BAPMappingConfigurationAcknowledge-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t bap_map_cfg_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 7};
  return map_enum_number(names, 2, idx, "id");
}
bool bap_map_cfg_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bap_map_cfg_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bap_map_cfg_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bap_map_cfg_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bap_map_cfg_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != bap_map_cfg_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 7: {
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
void bap_map_cfg_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BAPMappingConfigurationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t bap_map_cfg_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool bap_map_cfg_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e bap_map_cfg_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e bap_map_cfg_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 77:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE bap_map_cfg_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE bap_map_cfg_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != bap_map_cfg_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 77: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 7: {
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
void bap_map_cfg_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastContextModificationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 0, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool broadcast_context_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_mod_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_mod_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_mod_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_mod_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_mod_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void broadcast_context_mod_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastContextModificationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 481, 454, 476, 470, 472, 717};
  return map_enum_number(names, 8, idx, "id");
}
bool broadcast_context_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 481, 454, 476, 470, 472, 717};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 481:
      return crit_e::reject;
    case 454:
      return crit_e::reject;
    case 476:
      return crit_e::reject;
    case 470:
      return crit_e::reject;
    case 472:
      return crit_e::reject;
    case 717:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 481:
      return presence_e::optional;
    case 454:
      return presence_e::mandatory;
    case 476:
      return presence_e::optional;
    case 470:
      return presence_e::optional;
    case 472:
      return presence_e::optional;
    case 717:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += mbs_service_area_present ? 1 : 0;
  nof_ies += broadcast_m_rbs_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += broadcast_m_rbs_to_be_modified_list_present ? 1 : 0;
  nof_ies += broadcast_m_rbs_to_be_released_list_present ? 1 : 0;
  nof_ies += supported_ue_type_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (mbs_service_area_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)481, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_service_area.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)454, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_cu_to_du_rrc_info.pack(bref));
  }
  if (broadcast_m_rbs_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)476, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_to_be_setup_mod_list, 1, 32, true));
  }
  if (broadcast_m_rbs_to_be_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)470, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_to_be_modified_list, 1, 32, true));
  }
  if (broadcast_m_rbs_to_be_released_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)472, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_to_be_released_list, 1, 32, true));
  }
  if (supported_ue_type_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)717, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_ue_type_list, 1, 8, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_mod_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 481: {
        mbs_service_area_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_service_area.unpack(bref));
        break;
      }
      case 454: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_cu_to_du_rrc_info.unpack(bref));
        break;
      }
      case 476: {
        broadcast_m_rbs_to_be_setup_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_to_be_setup_mod_list, bref, 1, 32, true));
        break;
      }
      case 470: {
        broadcast_m_rbs_to_be_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_to_be_modified_list, bref, 1, 32, true));
        break;
      }
      case 472: {
        broadcast_m_rbs_to_be_released_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_to_be_released_list, bref, 1, 32, true));
        break;
      }
      case 717: {
        supported_ue_type_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(supported_ue_type_list, bref, 1, 8, true));
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
void broadcast_context_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (mbs_service_area_present) {
    j.start_obj();
    j.write_int("id", 481);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    mbs_service_area.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 454);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_cu_to_du_rrc_info.to_json(j);
  j.end_obj();
  if (broadcast_m_rbs_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 476);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (broadcast_m_rbs_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 470);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (broadcast_m_rbs_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 472);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (supported_ue_type_list_present) {
    j.start_obj();
    j.write_int("id", 717);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : supported_ue_type_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// BroadcastContextModificationResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 468, 462, 464, 458, 7, 646};
  return map_enum_number(names, 8, idx, "id");
}
bool broadcast_context_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 468, 462, 464, 458, 7, 646};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 468:
      return crit_e::reject;
    case 462:
      return crit_e::ignore;
    case 464:
      return crit_e::reject;
    case 458:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 646:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 468:
      return presence_e::optional;
    case 462:
      return presence_e::optional;
    case 464:
      return presence_e::optional;
    case 458:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 646:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += broadcast_m_rbs_setup_mod_list_present ? 1 : 0;
  nof_ies += broadcast_m_rbs_failed_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += broadcast_m_rbs_modified_list_present ? 1 : 0;
  nof_ies += broadcast_m_rbs_failed_to_be_modified_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += broadcast_area_scope_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (broadcast_m_rbs_setup_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)468, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_setup_mod_list, 1, 32, true));
  }
  if (broadcast_m_rbs_failed_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)462, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_failed_to_be_setup_mod_list, 1, 32, true));
  }
  if (broadcast_m_rbs_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)464, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_modified_list, 1, 32, true));
  }
  if (broadcast_m_rbs_failed_to_be_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)458, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_failed_to_be_modified_list, 1, 32, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (broadcast_area_scope_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)646, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(broadcast_area_scope.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_mod_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 468: {
        broadcast_m_rbs_setup_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_setup_mod_list, bref, 1, 32, true));
        break;
      }
      case 462: {
        broadcast_m_rbs_failed_to_be_setup_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_failed_to_be_setup_mod_list, bref, 1, 32, true));
        break;
      }
      case 464: {
        broadcast_m_rbs_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_modified_list, bref, 1, 32, true));
        break;
      }
      case 458: {
        broadcast_m_rbs_failed_to_be_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_failed_to_be_modified_list, bref, 1, 32, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 646: {
        broadcast_area_scope_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(broadcast_area_scope.unpack(bref));
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
void broadcast_context_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (broadcast_m_rbs_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 468);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (broadcast_m_rbs_failed_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 462);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_failed_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (broadcast_m_rbs_modified_list_present) {
    j.start_obj();
    j.write_int("id", 464);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (broadcast_m_rbs_failed_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 458);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_failed_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (broadcast_area_scope_present) {
    j.start_obj();
    j.write_int("id", 646);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    broadcast_area_scope.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastContextReleaseCommandIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool broadcast_context_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_release_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_release_cmd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
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
void broadcast_context_release_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// BroadcastContextReleaseCompleteIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_release_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool broadcast_context_release_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_release_complete_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_release_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_release_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_release_complete_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_release_complete_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 7: {
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
void broadcast_context_release_complete_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastContextReleaseRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool broadcast_context_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_release_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
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
void broadcast_context_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// BroadcastContextSetupFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 0, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool broadcast_context_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += gnb_du_mbs_f1ap_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (gnb_du_mbs_f1ap_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_setup_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        gnb_du_mbs_f1ap_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void broadcast_context_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  if (gnb_du_mbs_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 452);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_mbs_f1ap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastContextSetupRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 455, 481, 454, 456, 474, 717, 767, 844};
  return map_enum_number(names, 9, idx, "id");
}
bool broadcast_context_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 455, 481, 454, 456, 474, 717, 767, 844};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 455:
      return crit_e::reject;
    case 481:
      return crit_e::reject;
    case 454:
      return crit_e::reject;
    case 456:
      return crit_e::reject;
    case 474:
      return crit_e::reject;
    case 717:
      return crit_e::ignore;
    case 767:
      return crit_e::ignore;
    case 844:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 455:
      return presence_e::mandatory;
    case 481:
      return presence_e::optional;
    case 454:
      return presence_e::mandatory;
    case 456:
      return presence_e::mandatory;
    case 474:
      return presence_e::mandatory;
    case 717:
      return presence_e::optional;
    case 767:
      return presence_e::optional;
    case 844:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += mbs_service_area_present ? 1 : 0;
  nof_ies += supported_ue_type_list_present ? 1 : 0;
  nof_ies += associated_session_id_present ? 1 : 0;
  nof_ies += ran_sharing_assist_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)455, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_session_id.pack(bref));
  }
  if (mbs_service_area_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)481, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_service_area.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)454, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_cu_to_du_rrc_info.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)456, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(snssai.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)474, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_to_be_setup_list, 1, 32, true));
  }
  if (supported_ue_type_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)717, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, supported_ue_type_list, 1, 8, true));
  }
  if (associated_session_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)767, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(associated_session_id.pack(bref));
  }
  if (ran_sharing_assist_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)844, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ran_sharing_assist_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_setup_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 455: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_session_id.unpack(bref));
        break;
      }
      case 481: {
        mbs_service_area_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_service_area.unpack(bref));
        break;
      }
      case 454: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_cu_to_du_rrc_info.unpack(bref));
        break;
      }
      case 456: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(snssai.unpack(bref));
        break;
      }
      case 474: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_to_be_setup_list, bref, 1, 32, true));
        break;
      }
      case 717: {
        supported_ue_type_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(supported_ue_type_list, bref, 1, 8, true));
        break;
      }
      case 767: {
        associated_session_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(associated_session_id.unpack(bref));
        break;
      }
      case 844: {
        ran_sharing_assist_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ran_sharing_assist_info.unpack(bref));
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
void broadcast_context_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 455);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_service_area_present) {
    j.start_obj();
    j.write_int("id", 481);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    mbs_service_area.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 454);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_cu_to_du_rrc_info.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 456);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  snssai.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 474);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : broadcast_m_rbs_to_be_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (supported_ue_type_list_present) {
    j.start_obj();
    j.write_int("id", 717);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : supported_ue_type_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (associated_session_id_present) {
    j.start_obj();
    j.write_int("id", 767);
    j.write_str("criticality", "ignore");
    j.write_str("value", associated_session_id.to_string());
    j.end_obj();
  }
  if (ran_sharing_assist_info_present) {
    j.start_obj();
    j.write_int("id", 844);
    j.write_str("criticality", "ignore");
    j.write_str("value", "mbs-session-in-non-shared-cell-resources");
    j.end_obj();
  }
  j.end_array();
}

// BroadcastContextSetupResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_context_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 466, 460, 646, 7};
  return map_enum_number(names, 6, idx, "id");
}
bool broadcast_context_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 466, 460, 646, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_context_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 466:
      return crit_e::reject;
    case 460:
      return crit_e::ignore;
    case 646:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_context_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 466:
      return presence_e::mandatory;
    case 460:
      return presence_e::optional;
    case 646:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_context_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += broadcast_m_rbs_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += broadcast_area_scope_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)466, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_setup_list, 1, 32, true));
  }
  if (broadcast_m_rbs_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)460, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_failed_to_be_setup_list, 1, 32, true));
  }
  if (broadcast_area_scope_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)646, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(broadcast_area_scope.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_context_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_context_setup_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 466: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_setup_list, bref, 1, 32, true));
        break;
      }
      case 460: {
        broadcast_m_rbs_failed_to_be_setup_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_failed_to_be_setup_list, bref, 1, 32, true));
        break;
      }
      case 646: {
        broadcast_area_scope_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(broadcast_area_scope.unpack(bref));
        break;
      }
      case 7: {
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
void broadcast_context_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 466);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : broadcast_m_rbs_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (broadcast_m_rbs_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 460);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (broadcast_area_scope_present) {
    j.start_obj();
    j.write_int("id", 646);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    broadcast_area_scope.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastTransportResourceRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t broadcast_transport_res_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 789, 846};
  return map_enum_number(names, 4, idx, "id");
}
bool broadcast_transport_res_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 789, 846};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_transport_res_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 789:
      return crit_e::reject;
    case 846:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_transport_res_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 789:
      return presence_e::optional;
    case 846:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_transport_res_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += broadcast_m_rbs_transport_request_list_present ? 1 : 0;
  nof_ies += f1_u_path_fail_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (broadcast_m_rbs_transport_request_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)789, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_m_rbs_transport_request_list, 1, 32, true));
  }
  if (f1_u_path_fail_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)846, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(f1_u_path_fail.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_transport_res_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != broadcast_transport_res_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 789: {
        broadcast_m_rbs_transport_request_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_m_rbs_transport_request_list, bref, 1, 32, true));
        break;
      }
      case 846: {
        f1_u_path_fail_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(f1_u_path_fail.unpack(bref));
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
void broadcast_transport_res_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (broadcast_m_rbs_transport_request_list_present) {
    j.start_obj();
    j.write_int("id", 789);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : broadcast_m_rbs_transport_request_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (f1_u_path_fail_present) {
    j.start_obj();
    j.write_int("id", 846);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// CUDUCellSwitchNotificationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t cu_du_cell_switch_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 111, 729, 837};
  return map_enum_number(names, 5, idx, "id");
}
bool cu_du_cell_switch_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 111, 729, 837};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cu_du_cell_switch_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 111:
      return crit_e::reject;
    case 729:
      return crit_e::ignore;
    case 837:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cu_du_cell_switch_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 111:
      return presence_e::mandatory;
    case 729:
      return presence_e::optional;
    case 837:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cu_du_cell_switch_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ltm_cell_switch_info_present ? 1 : 0;
  nof_ies += tai_nformation_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)111, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nr_cgi.pack(bref));
  }
  if (ltm_cell_switch_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)729, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ltm_cell_switch_info.pack(bref));
  }
  if (tai_nformation_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)837, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, tai_nformation_list, 1, 8, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cu_du_cell_switch_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != cu_du_cell_switch_notif_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 111: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nr_cgi.unpack(bref));
        break;
      }
      case 729: {
        ltm_cell_switch_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ltm_cell_switch_info.unpack(bref));
        break;
      }
      case 837: {
        tai_nformation_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(tai_nformation_list, bref, 1, 8, true));
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
void cu_du_cell_switch_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 111);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  nr_cgi.to_json(j);
  j.end_obj();
  if (ltm_cell_switch_info_present) {
    j.start_obj();
    j.write_int("id", 729);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltm_cell_switch_info.to_json(j);
    j.end_obj();
  }
  if (tai_nformation_list_present) {
    j.start_obj();
    j.write_int("id", 837);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : tai_nformation_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// CUDUMobilityInitiationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t cu_du_mob_initiation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 859};
  return map_enum_number(names, 3, idx, "id");
}
bool cu_du_mob_initiation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 859};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cu_du_mob_initiation_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 859:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cu_du_mob_initiation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 859:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cu_du_mob_initiation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)859, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mob_initiation.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cu_du_mob_initiation_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != cu_du_mob_initiation_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 859: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mob_initiation.unpack(bref));
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
void cu_du_mob_initiation_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 859);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mob_initiation.to_json(j);
  j.end_obj();
  j.end_array();
}

// CUDURadioInformationTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t cu_du_radio_info_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 250};
  return map_enum_number(names, 2, idx, "id");
}
bool cu_du_radio_info_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 250};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cu_du_radio_info_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 250:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cu_du_radio_info_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 250:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cu_du_radio_info_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)250, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cu_du_radio_info_type.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cu_du_radio_info_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != cu_du_radio_info_transfer_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 250: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cu_du_radio_info_type.unpack(bref));
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
void cu_du_radio_info_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 250);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cu_du_radio_info_type.to_json(j);
  j.end_obj();
  j.end_array();
}

// CUDUTAInformationTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t cu_du_tai_nformation_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 798};
  return map_enum_number(names, 2, idx, "id");
}
bool cu_du_tai_nformation_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 798};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e cu_du_tai_nformation_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 798:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cu_du_tai_nformation_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 798:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cu_du_tai_nformation_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)798, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cu_to_du_tai_nformation_list, 1, 8, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cu_du_tai_nformation_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != cu_du_tai_nformation_transfer_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 798: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cu_to_du_tai_nformation_list, bref, 1, 8, true));
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
void cu_du_tai_nformation_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 798);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : cu_to_du_tai_nformation_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// CellTrafficTraceIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t cell_traffic_trace_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 243, 378, 379, 380};
  return map_enum_number(names, 6, idx, "id");
}
bool cell_traffic_trace_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 243, 378, 379, 380};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 243:
      return crit_e::ignore;
    case 378:
      return crit_e::ignore;
    case 379:
      return crit_e::ignore;
    case 380:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cell_traffic_trace_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 243:
      return presence_e::mandatory;
    case 378:
      return presence_e::mandatory;
    case 379:
      return presence_e::optional;
    case 380:
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

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)243, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(trace_id.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)378, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(trace_collection_entity_ip_address.pack(bref));
  }
  if (privacy_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)379, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(privacy_ind.pack(bref));
  }
  if (trace_collection_entity_uri_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)380, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(trace_collection_entity_uri.pack(bref));
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
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != cell_traffic_trace_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 243: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(trace_id.unpack(bref));
        break;
      }
      case 378: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(trace_collection_entity_ip_address.unpack(bref));
        break;
      }
      case 379: {
        privacy_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(privacy_ind.unpack(bref));
        break;
      }
      case 380: {
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
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 243);
  j.write_str("criticality", "ignore");
  j.write_str("value", trace_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 378);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  trace_collection_entity_ip_address.to_json(j);
  j.end_obj();
  if (privacy_ind_present) {
    j.start_obj();
    j.write_int("id", 379);
    j.write_str("criticality", "ignore");
    j.write_str("value", privacy_ind.to_string());
    j.end_obj();
  }
  if (trace_collection_entity_uri_present) {
    j.start_obj();
    j.write_int("id", 380);
    j.write_str("criticality", "ignore");
    j.write_str("value", trace_collection_entity_uri.to_string());
    j.end_obj();
  }
  j.end_array();
}

// DUCUAccessAndMobilityIndicationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t du_cu_access_and_mob_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 795};
  return map_enum_number(names, 2, idx, "id");
}
bool du_cu_access_and_mob_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 795};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e du_cu_access_and_mob_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 795:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e du_cu_access_and_mob_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 795:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE du_cu_access_and_mob_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += dl_lbt_fail_info_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (dl_lbt_fail_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)795, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, dl_lbt_fail_info_list, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE du_cu_access_and_mob_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != du_cu_access_and_mob_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 795: {
        dl_lbt_fail_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(dl_lbt_fail_info_list, bref, 1, 64, true));
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
void du_cu_access_and_mob_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (dl_lbt_fail_info_list_present) {
    j.start_obj();
    j.write_int("id", 795);
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

// DUCUCellSwitchNotificationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t du_cu_cell_switch_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 111, 729, 837};
  return map_enum_number(names, 5, idx, "id");
}
bool du_cu_cell_switch_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 111, 729, 837};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e du_cu_cell_switch_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 111:
      return crit_e::reject;
    case 729:
      return crit_e::ignore;
    case 837:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e du_cu_cell_switch_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 111:
      return presence_e::mandatory;
    case 729:
      return presence_e::optional;
    case 837:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE du_cu_cell_switch_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ltm_cell_switch_info_present ? 1 : 0;
  nof_ies += tai_nformation_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)111, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nr_cgi.pack(bref));
  }
  if (ltm_cell_switch_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)729, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ltm_cell_switch_info.pack(bref));
  }
  if (tai_nformation_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)837, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, tai_nformation_list, 1, 8, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE du_cu_cell_switch_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != du_cu_cell_switch_notif_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 111: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nr_cgi.unpack(bref));
        break;
      }
      case 729: {
        ltm_cell_switch_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ltm_cell_switch_info.unpack(bref));
        break;
      }
      case 837: {
        tai_nformation_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(tai_nformation_list, bref, 1, 8, true));
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
void du_cu_cell_switch_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 111);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  nr_cgi.to_json(j);
  j.end_obj();
  if (ltm_cell_switch_info_present) {
    j.start_obj();
    j.write_int("id", 729);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltm_cell_switch_info.to_json(j);
    j.end_obj();
  }
  if (tai_nformation_list_present) {
    j.start_obj();
    j.write_int("id", 837);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : tai_nformation_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// DUCURadioInformationTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t du_cu_radio_info_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 249};
  return map_enum_number(names, 2, idx, "id");
}
bool du_cu_radio_info_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 249};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e du_cu_radio_info_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 249:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e du_cu_radio_info_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 249:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE du_cu_radio_info_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)249, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(du_cu_radio_info_type.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE du_cu_radio_info_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != du_cu_radio_info_transfer_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 249: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(du_cu_radio_info_type.unpack(bref));
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
void du_cu_radio_info_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 249);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  du_cu_radio_info_type.to_json(j);
  j.end_obj();
  j.end_array();
}

// DUCUTAInformationTransferIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t du_cu_tai_nformation_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 730};
  return map_enum_number(names, 2, idx, "id");
}
bool du_cu_tai_nformation_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 730};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e du_cu_tai_nformation_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 730:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e du_cu_tai_nformation_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 730:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE du_cu_tai_nformation_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)730, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, du_to_cu_tai_nformation_list, 1, 8, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE du_cu_tai_nformation_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != du_cu_tai_nformation_transfer_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 730: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(du_to_cu_tai_nformation_list, bref, 1, 8, true));
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
void du_cu_tai_nformation_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 730);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : du_to_cu_tai_nformation_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// DeactivateTraceIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t deactiv_trace_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 243};
  return map_enum_number(names, 3, idx, "id");
}
bool deactiv_trace_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 243};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 243:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e deactiv_trace_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 243:
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

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)243, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(trace_id.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE deactiv_trace_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != deactiv_trace_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 243: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(trace_id.unpack(bref));
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
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 243);
  j.write_str("criticality", "ignore");
  j.write_str("value", trace_id.to_string());
  j.end_obj();
  j.end_array();
}

// E-CIDMeasurementFailureIndication-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t e_c_id_meas_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 412, 413, 0};
  return map_enum_number(names, 5, idx, "id");
}
bool e_c_id_meas_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 412, 413, 0};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 412:
      return crit_e::reject;
    case 413:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e_c_id_meas_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 412:
      return presence_e::mandatory;
    case 413:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e_c_id_meas_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)412, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)413, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
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

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_fail_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 412: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 413: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 412);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 413);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// E-CIDMeasurementInitiationFailure-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t e_c_id_meas_initiation_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 412, 413, 0, 7};
  return map_enum_number(names, 6, idx, "id");
}
bool e_c_id_meas_initiation_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 412, 413, 0, 7};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 412:
      return crit_e::reject;
    case 413:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e_c_id_meas_initiation_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 412:
      return presence_e::mandatory;
    case 413:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e_c_id_meas_initiation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)412, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)413, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_initiation_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 412: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 413: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 412);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 413);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// E-CIDMeasurementInitiationRequest-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t e_c_id_meas_initiation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 412, 413, 424, 416, 414, 672};
  return map_enum_number(names, 8, idx, "id");
}
bool e_c_id_meas_initiation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 412, 413, 424, 416, 414, 672};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 412:
      return crit_e::reject;
    case 413:
      return crit_e::reject;
    case 424:
      return crit_e::reject;
    case 416:
      return crit_e::reject;
    case 414:
      return crit_e::reject;
    case 672:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e_c_id_meas_initiation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 412:
      return presence_e::mandatory;
    case 413:
      return presence_e::mandatory;
    case 424:
      return presence_e::mandatory;
    case 416:
      return presence_e::conditional;
    case 414:
      return presence_e::mandatory;
    case 672:
      return presence_e::conditional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e_c_id_meas_initiation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 6;
  nof_ies += e_c_id_meas_periodicity_present ? 1 : 0;
  nof_ies += pos_meas_periodicity_nr_ao_a_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)412, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)413, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)424, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(e_c_id_report_characteristics.pack(bref));
  }
  if (e_c_id_meas_periodicity_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)416, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(e_c_id_meas_periodicity.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)414, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, e_c_id_meas_quantities, 1, 64, true));
  }
  if (pos_meas_periodicity_nr_ao_a_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)672, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_meas_periodicity_nr_ao_a.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_initiation_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 6;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_initiation_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 412: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 413: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 424: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(e_c_id_report_characteristics.unpack(bref));
        break;
      }
      case 416: {
        e_c_id_meas_periodicity_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(e_c_id_meas_periodicity.unpack(bref));
        break;
      }
      case 414: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(e_c_id_meas_quantities, bref, 1, 64, true));
        break;
      }
      case 672: {
        pos_meas_periodicity_nr_ao_a_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_meas_periodicity_nr_ao_a.unpack(bref));
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 412);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 413);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 424);
  j.write_str("criticality", "reject");
  j.write_str("value", e_c_id_report_characteristics.to_string());
  j.end_obj();
  if (e_c_id_meas_periodicity_present) {
    j.start_obj();
    j.write_int("id", 416);
    j.write_str("criticality", "reject");
    j.write_str("value", e_c_id_meas_periodicity.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 414);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : e_c_id_meas_quantities) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (pos_meas_periodicity_nr_ao_a_present) {
    j.start_obj();
    j.write_int("id", 672);
    j.write_str("criticality", "reject");
    j.write_str("value", pos_meas_periodicity_nr_ao_a.to_string());
    j.end_obj();
  }
  j.end_array();
}

// E-CIDMeasurementInitiationResponse-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t e_c_id_meas_initiation_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 412, 413, 417, 418, 7};
  return map_enum_number(names, 7, idx, "id");
}
bool e_c_id_meas_initiation_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 412, 413, 417, 418, 7};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 412:
      return crit_e::reject;
    case 413:
      return crit_e::reject;
    case 417:
      return crit_e::ignore;
    case 418:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e_c_id_meas_initiation_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 412:
      return presence_e::mandatory;
    case 413:
      return presence_e::mandatory;
    case 417:
      return presence_e::optional;
    case 418:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e_c_id_meas_initiation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += e_c_id_meas_result_present ? 1 : 0;
  nof_ies += cell_portion_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)412, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)413, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  if (e_c_id_meas_result_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)417, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(e_c_id_meas_result.pack(bref));
  }
  if (cell_portion_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)418, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, cell_portion_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_initiation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_initiation_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 412: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 413: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 417: {
        e_c_id_meas_result_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(e_c_id_meas_result.unpack(bref));
        break;
      }
      case 418: {
        cell_portion_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(cell_portion_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 7: {
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
void e_c_id_meas_initiation_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 412);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 413);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  if (e_c_id_meas_result_present) {
    j.start_obj();
    j.write_int("id", 417);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    e_c_id_meas_result.to_json(j);
    j.end_obj();
  }
  if (cell_portion_id_present) {
    j.start_obj();
    j.write_int("id", 418);
    j.write_str("criticality", "ignore");
    j.write_int("value", cell_portion_id);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// E-CIDMeasurementReport-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t e_c_id_meas_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 412, 413, 417, 418};
  return map_enum_number(names, 6, idx, "id");
}
bool e_c_id_meas_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 412, 413, 417, 418};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 412:
      return crit_e::reject;
    case 413:
      return crit_e::reject;
    case 417:
      return crit_e::ignore;
    case 418:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e_c_id_meas_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 412:
      return presence_e::mandatory;
    case 413:
      return presence_e::mandatory;
    case 417:
      return presence_e::mandatory;
    case 418:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e_c_id_meas_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += cell_portion_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)412, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)413, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)417, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(e_c_id_meas_result.pack(bref));
  }
  if (cell_portion_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)418, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_report_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 412: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 413: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 417: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(e_c_id_meas_result.unpack(bref));
        break;
      }
      case 418: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 412);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 413);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 417);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  e_c_id_meas_result.to_json(j);
  j.end_obj();
  if (cell_portion_id_present) {
    j.start_obj();
    j.write_int("id", 418);
    j.write_str("criticality", "ignore");
    j.write_int("value", cell_portion_id);
    j.end_obj();
  }
  j.end_array();
}

// E-CIDMeasurementTerminationCommand-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t e_c_id_meas_termination_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 412, 413};
  return map_enum_number(names, 4, idx, "id");
}
bool e_c_id_meas_termination_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 412, 413};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 412:
      return crit_e::reject;
    case 413:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e e_c_id_meas_termination_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 412:
      return presence_e::mandatory;
    case 413:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE e_c_id_meas_termination_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)412, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)413, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_meas_id, (uint16_t)1u, (uint16_t)256u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_termination_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != e_c_id_meas_termination_cmd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 412: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
        break;
      }
      case 413: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_meas_id, bref, (uint16_t)1u, (uint16_t)256u, true, true));
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 412);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 413);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  j.end_array();
}

// ErrorIndicationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t error_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 40, 41, 0, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool error_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 40, 41, 0, 7};
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
    case 78:
      return crit_e::reject;
    case 40:
      return crit_e::ignore;
    case 41:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e error_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 40:
      return presence_e::optional;
    case 41:
      return presence_e::optional;
    case 0:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE error_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += gnb_cu_ue_f1ap_id_present ? 1 : 0;
  nof_ies += gnb_du_ue_f1ap_id_present ? 1 : 0;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (gnb_cu_ue_f1ap_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (gnb_du_ue_f1ap_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (cause_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != error_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 40: {
        gnb_cu_ue_f1ap_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        gnb_du_ue_f1ap_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 0: {
        cause_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void error_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (gnb_cu_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 40);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_cu_ue_f1ap_id);
    j.end_obj();
  }
  if (gnb_du_ue_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 41);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_ue_f1ap_id);
    j.end_obj();
  }
  if (cause_present) {
    j.start_obj();
    j.write_int("id", 0);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cause.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// F1RemovalFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t f1_removal_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool f1_removal_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e f1_removal_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e f1_removal_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE f1_removal_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_removal_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != f1_removal_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void f1_removal_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// F1RemovalRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t f1_removal_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78};
  return map_enum_number(names, 1, idx, "id");
}
bool f1_removal_request_ies_o::is_id_valid(const uint32_t& id)
{
  return 78 == id;
}
crit_e f1_removal_request_ies_o::get_crit(const uint32_t& id)
{
  if (id == 78) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e f1_removal_request_ies_o::get_presence(const uint32_t& id)
{
  if (id == 78) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE f1_removal_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_removal_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != f1_removal_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    if (id == 78) {
      {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
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
void f1_removal_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.end_array();
}

// F1RemovalResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t f1_removal_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 7};
  return map_enum_number(names, 2, idx, "id");
}
bool f1_removal_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e f1_removal_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e f1_removal_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE f1_removal_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_removal_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != f1_removal_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 7: {
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
void f1_removal_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// F1SetupFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t f1_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool f1_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e f1_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e f1_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 77:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE f1_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != f1_setup_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 77: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 7: {
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
void f1_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// F1SetupOutcome ::= ENUMERATED
const char* f1_setup_outcome_opts::to_string() const
{
  static const char* names[] = {"success", "failure"};
  return convert_enum_idx(names, 2, value, "f1_setup_outcome_e");
}

// F1SetupRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t f1_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 42, 45, 44, 171, 254, 281, 427, 762, 765};
  return map_enum_number(names, 10, idx, "id");
}
bool f1_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 42, 45, 44, 171, 254, 281, 427, 762, 765};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e f1_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 42:
      return crit_e::reject;
    case 45:
      return crit_e::ignore;
    case 44:
      return crit_e::reject;
    case 171:
      return crit_e::reject;
    case 254:
      return crit_e::ignore;
    case 281:
      return crit_e::ignore;
    case 427:
      return crit_e::ignore;
    case 762:
      return crit_e::reject;
    case 765:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e f1_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 42:
      return presence_e::mandatory;
    case 45:
      return presence_e::optional;
    case 44:
      return presence_e::optional;
    case 171:
      return presence_e::mandatory;
    case 254:
      return presence_e::optional;
    case 281:
      return presence_e::optional;
    case 427:
      return presence_e::optional;
    case 762:
      return presence_e::optional;
    case 765:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE f1_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += gnb_du_name_present ? 1 : 0;
  nof_ies += gnb_du_served_cells_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += bap_address_present ? 1 : 0;
  nof_ies += extended_gnb_du_name_present ? 1 : 0;
  nof_ies += rrc_terminating_iab_donor_gnb_id_present ? 1 : 0;
  nof_ies += mobile_iab_mt_user_location_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)42, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }
  if (gnb_du_name_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)45, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(gnb_du_name.pack(bref));
  }
  if (gnb_du_served_cells_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)44, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_du_served_cells_list, 1, 512, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)171, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(gnb_du_rrc_version.pack(bref));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)254, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(transport_layer_address_info.pack(bref));
  }
  if (bap_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)281, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(bap_address.pack(bref));
  }
  if (extended_gnb_du_name_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)427, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(extended_gnb_du_name.pack(bref));
  }
  if (rrc_terminating_iab_donor_gnb_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)762, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(rrc_terminating_iab_donor_gnb_id.pack(bref));
  }
  if (mobile_iab_mt_user_location_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)765, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mobile_iab_mt_user_location_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != f1_setup_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 42: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));
        break;
      }
      case 45: {
        gnb_du_name_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(gnb_du_name.unpack(bref));
        break;
      }
      case 44: {
        gnb_du_served_cells_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(gnb_du_served_cells_list, bref, 1, 512, true));
        break;
      }
      case 171: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(gnb_du_rrc_version.unpack(bref));
        break;
      }
      case 254: {
        transport_layer_address_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(transport_layer_address_info.unpack(bref));
        break;
      }
      case 281: {
        bap_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(bap_address.unpack(bref));
        break;
      }
      case 427: {
        extended_gnb_du_name_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(extended_gnb_du_name.unpack(bref));
        break;
      }
      case 762: {
        rrc_terminating_iab_donor_gnb_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(rrc_terminating_iab_donor_gnb_id.unpack(bref));
        break;
      }
      case 765: {
        mobile_iab_mt_user_location_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mobile_iab_mt_user_location_info.unpack(bref));
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
void f1_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 42);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_id);
  j.end_obj();
  if (gnb_du_name_present) {
    j.start_obj();
    j.write_int("id", 45);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_du_name.to_string());
    j.end_obj();
  }
  if (gnb_du_served_cells_list_present) {
    j.start_obj();
    j.write_int("id", 44);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : gnb_du_served_cells_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 171);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  gnb_du_rrc_version.to_json(j);
  j.end_obj();
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (bap_address_present) {
    j.start_obj();
    j.write_int("id", 281);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    bap_address.to_json(j);
    j.end_obj();
  }
  if (extended_gnb_du_name_present) {
    j.start_obj();
    j.write_int("id", 427);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_du_name.to_json(j);
    j.end_obj();
  }
  if (rrc_terminating_iab_donor_gnb_id_present) {
    j.start_obj();
    j.write_int("id", 762);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    rrc_terminating_iab_donor_gnb_id.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_mt_user_location_info_present) {
    j.start_obj();
    j.write_int("id", 765);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mobile_iab_mt_user_location_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// F1SetupResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t f1_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 82, 3, 170, 254, 287, 281, 426, 763};
  return map_enum_number(names, 9, idx, "id");
}
bool f1_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 82, 3, 170, 254, 287, 281, 426, 763};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e f1_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 82:
      return crit_e::ignore;
    case 3:
      return crit_e::reject;
    case 170:
      return crit_e::reject;
    case 254:
      return crit_e::ignore;
    case 287:
      return crit_e::reject;
    case 281:
      return crit_e::ignore;
    case 426:
      return crit_e::ignore;
    case 763:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e f1_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 82:
      return presence_e::optional;
    case 3:
      return presence_e::optional;
    case 170:
      return presence_e::mandatory;
    case 254:
      return presence_e::optional;
    case 287:
      return presence_e::optional;
    case 281:
      return presence_e::optional;
    case 426:
      return presence_e::optional;
    case 763:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE f1_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += gnb_cu_name_present ? 1 : 0;
  nof_ies += cells_to_be_activ_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += ul_bh_non_up_traffic_map_present ? 1 : 0;
  nof_ies += bap_address_present ? 1 : 0;
  nof_ies += extended_gnb_cu_name_present ? 1 : 0;
  nof_ies += ncgi_to_be_upd_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (gnb_cu_name_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)82, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(gnb_cu_name.pack(bref));
  }
  if (cells_to_be_activ_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)3, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_to_be_activ_list, 1, 512, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)170, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(gnb_cu_rrc_version.pack(bref));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)254, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(transport_layer_address_info.pack(bref));
  }
  if (ul_bh_non_up_traffic_map_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)287, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ul_bh_non_up_traffic_map.pack(bref));
  }
  if (bap_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)281, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(bap_address.pack(bref));
  }
  if (extended_gnb_cu_name_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)426, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(extended_gnb_cu_name.pack(bref));
  }
  if (ncgi_to_be_upd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)763, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ncgi_to_be_upd_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != f1_setup_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 82: {
        gnb_cu_name_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(gnb_cu_name.unpack(bref));
        break;
      }
      case 3: {
        cells_to_be_activ_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_to_be_activ_list, bref, 1, 512, true));
        break;
      }
      case 170: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(gnb_cu_rrc_version.unpack(bref));
        break;
      }
      case 254: {
        transport_layer_address_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(transport_layer_address_info.unpack(bref));
        break;
      }
      case 287: {
        ul_bh_non_up_traffic_map_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ul_bh_non_up_traffic_map.unpack(bref));
        break;
      }
      case 281: {
        bap_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(bap_address.unpack(bref));
        break;
      }
      case 426: {
        extended_gnb_cu_name_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(extended_gnb_cu_name.unpack(bref));
        break;
      }
      case 763: {
        ncgi_to_be_upd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ncgi_to_be_upd_list, bref, 1, 512, true));
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
void f1_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (gnb_cu_name_present) {
    j.start_obj();
    j.write_int("id", 82);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_cu_name.to_string());
    j.end_obj();
  }
  if (cells_to_be_activ_list_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_to_be_activ_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 170);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  gnb_cu_rrc_version.to_json(j);
  j.end_obj();
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (ul_bh_non_up_traffic_map_present) {
    j.start_obj();
    j.write_int("id", 287);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ul_bh_non_up_traffic_map.to_json(j);
    j.end_obj();
  }
  if (bap_address_present) {
    j.start_obj();
    j.write_int("id", 281);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    bap_address.to_json(j);
    j.end_obj();
  }
  if (extended_gnb_cu_name_present) {
    j.start_obj();
    j.write_int("id", 426);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_cu_name.to_json(j);
    j.end_obj();
  }
  if (ncgi_to_be_upd_list_present) {
    j.start_obj();
    j.write_int("id", 763);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ncgi_to_be_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// GNBCUConfigurationUpdateIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_cu_cfg_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 3, 5, 121, 123, 125, 129, 105, 244, 254, 287, 281, 447, 449, 82, 426, 746};
  return map_enum_number(names, 17, idx, "id");
}
bool gnb_cu_cfg_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 3, 5, 121, 123, 125, 129, 105, 244, 254, 287, 281, 447, 449, 82, 426, 746};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cfg_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 5:
      return crit_e::reject;
    case 121:
      return crit_e::ignore;
    case 123:
      return crit_e::ignore;
    case 125:
      return crit_e::ignore;
    case 129:
      return crit_e::ignore;
    case 105:
      return crit_e::reject;
    case 244:
      return crit_e::ignore;
    case 254:
      return crit_e::ignore;
    case 287:
      return crit_e::reject;
    case 281:
      return crit_e::ignore;
    case 447:
      return crit_e::ignore;
    case 449:
      return crit_e::ignore;
    case 82:
      return crit_e::ignore;
    case 426:
      return crit_e::ignore;
    case 746:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cfg_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 3:
      return presence_e::optional;
    case 5:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    case 123:
      return presence_e::optional;
    case 125:
      return presence_e::optional;
    case 129:
      return presence_e::optional;
    case 105:
      return presence_e::optional;
    case 244:
      return presence_e::optional;
    case 254:
      return presence_e::optional;
    case 287:
      return presence_e::optional;
    case 281:
      return presence_e::optional;
    case 447:
      return presence_e::optional;
    case 449:
      return presence_e::optional;
    case 82:
      return presence_e::optional;
    case 426:
      return presence_e::optional;
    case 746:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cfg_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += cells_to_be_activ_list_present ? 1 : 0;
  nof_ies += cells_to_be_deactiv_list_present ? 1 : 0;
  nof_ies += gnb_cu_tnl_assoc_to_add_list_present ? 1 : 0;
  nof_ies += gnb_cu_tnl_assoc_to_rem_list_present ? 1 : 0;
  nof_ies += gnb_cu_tnl_assoc_to_upd_list_present ? 1 : 0;
  nof_ies += cells_to_be_barred_list_present ? 1 : 0;
  nof_ies += protected_eutra_res_list_present ? 1 : 0;
  nof_ies += neighbour_cell_info_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += ul_bh_non_up_traffic_map_present ? 1 : 0;
  nof_ies += bap_address_present ? 1 : 0;
  nof_ies += cco_assist_info_present ? 1 : 0;
  nof_ies += cells_for_son_list_present ? 1 : 0;
  nof_ies += gnb_cu_name_present ? 1 : 0;
  nof_ies += extended_gnb_cu_name_present ? 1 : 0;
  nof_ies += cells_allowed_to_be_deactiv_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (cells_to_be_activ_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)3, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_to_be_activ_list, 1, 512, true));
  }
  if (cells_to_be_deactiv_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_to_be_deactiv_list, 1, 512, true));
  }
  if (gnb_cu_tnl_assoc_to_add_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)121, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_cu_tnl_assoc_to_add_list, 1, 32, true));
  }
  if (gnb_cu_tnl_assoc_to_rem_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)123, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_cu_tnl_assoc_to_rem_list, 1, 32, true));
  }
  if (gnb_cu_tnl_assoc_to_upd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)125, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_cu_tnl_assoc_to_upd_list, 1, 32, true));
  }
  if (cells_to_be_barred_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)129, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_to_be_barred_list, 1, 512, true));
  }
  if (protected_eutra_res_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)105, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, protected_eutra_res_list, 1, 256, true));
  }
  if (neighbour_cell_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)244, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, neighbour_cell_info_list, 1, 512, true));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)254, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(transport_layer_address_info.pack(bref));
  }
  if (ul_bh_non_up_traffic_map_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)287, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ul_bh_non_up_traffic_map.pack(bref));
  }
  if (bap_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)281, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(bap_address.pack(bref));
  }
  if (cco_assist_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)447, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cco_assist_info.pack(bref));
  }
  if (cells_for_son_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)449, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_for_son_list, 1, 256, true));
  }
  if (gnb_cu_name_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)82, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(gnb_cu_name.pack(bref));
  }
  if (extended_gnb_cu_name_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)426, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(extended_gnb_cu_name.pack(bref));
  }
  if (cells_allowed_to_be_deactiv_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)746, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_allowed_to_be_deactiv_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cfg_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_cu_cfg_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 3: {
        cells_to_be_activ_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_to_be_activ_list, bref, 1, 512, true));
        break;
      }
      case 5: {
        cells_to_be_deactiv_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_to_be_deactiv_list, bref, 1, 512, true));
        break;
      }
      case 121: {
        gnb_cu_tnl_assoc_to_add_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(gnb_cu_tnl_assoc_to_add_list, bref, 1, 32, true));
        break;
      }
      case 123: {
        gnb_cu_tnl_assoc_to_rem_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(gnb_cu_tnl_assoc_to_rem_list, bref, 1, 32, true));
        break;
      }
      case 125: {
        gnb_cu_tnl_assoc_to_upd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(gnb_cu_tnl_assoc_to_upd_list, bref, 1, 32, true));
        break;
      }
      case 129: {
        cells_to_be_barred_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_to_be_barred_list, bref, 1, 512, true));
        break;
      }
      case 105: {
        protected_eutra_res_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(protected_eutra_res_list, bref, 1, 256, true));
        break;
      }
      case 244: {
        neighbour_cell_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(neighbour_cell_info_list, bref, 1, 512, true));
        break;
      }
      case 254: {
        transport_layer_address_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(transport_layer_address_info.unpack(bref));
        break;
      }
      case 287: {
        ul_bh_non_up_traffic_map_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ul_bh_non_up_traffic_map.unpack(bref));
        break;
      }
      case 281: {
        bap_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(bap_address.unpack(bref));
        break;
      }
      case 447: {
        cco_assist_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cco_assist_info.unpack(bref));
        break;
      }
      case 449: {
        cells_for_son_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_for_son_list, bref, 1, 256, true));
        break;
      }
      case 82: {
        gnb_cu_name_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(gnb_cu_name.unpack(bref));
        break;
      }
      case 426: {
        extended_gnb_cu_name_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(extended_gnb_cu_name.unpack(bref));
        break;
      }
      case 746: {
        cells_allowed_to_be_deactiv_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_allowed_to_be_deactiv_list, bref, 1, 512, true));
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
void gnb_cu_cfg_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (cells_to_be_activ_list_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_to_be_activ_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (cells_to_be_deactiv_list_present) {
    j.start_obj();
    j.write_int("id", 5);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_to_be_deactiv_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_tnl_assoc_to_add_list_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_tnl_assoc_to_add_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_tnl_assoc_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 123);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_tnl_assoc_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_tnl_assoc_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 125);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_tnl_assoc_to_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (cells_to_be_barred_list_present) {
    j.start_obj();
    j.write_int("id", 129);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cells_to_be_barred_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (protected_eutra_res_list_present) {
    j.start_obj();
    j.write_int("id", 105);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : protected_eutra_res_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (neighbour_cell_info_list_present) {
    j.start_obj();
    j.write_int("id", 244);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : neighbour_cell_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (ul_bh_non_up_traffic_map_present) {
    j.start_obj();
    j.write_int("id", 287);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ul_bh_non_up_traffic_map.to_json(j);
    j.end_obj();
  }
  if (bap_address_present) {
    j.start_obj();
    j.write_int("id", 281);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    bap_address.to_json(j);
    j.end_obj();
  }
  if (cco_assist_info_present) {
    j.start_obj();
    j.write_int("id", 447);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cco_assist_info.to_json(j);
    j.end_obj();
  }
  if (cells_for_son_list_present) {
    j.start_obj();
    j.write_int("id", 449);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cells_for_son_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_name_present) {
    j.start_obj();
    j.write_int("id", 82);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_cu_name.to_string());
    j.end_obj();
  }
  if (extended_gnb_cu_name_present) {
    j.start_obj();
    j.write_int("id", 426);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_cu_name.to_json(j);
    j.end_obj();
  }
  if (cells_allowed_to_be_deactiv_list_present) {
    j.start_obj();
    j.write_int("id", 746);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cells_allowed_to_be_deactiv_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// GNBCUConfigurationUpdateAcknowledgeIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_cu_cfg_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 1, 7, 132, 134, 189, 254, 745};
  return map_enum_number(names, 8, idx, "id");
}
bool gnb_cu_cfg_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 1, 7, 132, 134, 189, 254, 745};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cfg_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 1:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 132:
      return crit_e::ignore;
    case 134:
      return crit_e::ignore;
    case 189:
      return crit_e::ignore;
    case 254:
      return crit_e::ignore;
    case 745:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cfg_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 1:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 132:
      return presence_e::optional;
    case 134:
      return presence_e::optional;
    case 189:
      return presence_e::optional;
    case 254:
      return presence_e::optional;
    case 745:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cfg_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += cells_failed_to_be_activ_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += gnb_cu_tnl_assoc_setup_list_present ? 1 : 0;
  nof_ies += gnb_cu_tnl_assoc_failed_to_setup_list_present ? 1 : 0;
  nof_ies += ded_si_delivery_needed_ue_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += cells_with_ss_bs_activ_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (cells_failed_to_be_activ_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)1, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_failed_to_be_activ_list, 1, 512, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (gnb_cu_tnl_assoc_setup_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)132, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_cu_tnl_assoc_setup_list, 1, 32, true));
  }
  if (gnb_cu_tnl_assoc_failed_to_setup_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)134, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_cu_tnl_assoc_failed_to_setup_list, 1, 32, true));
  }
  if (ded_si_delivery_needed_ue_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)189, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ded_si_delivery_needed_ue_list, 1, 65536, true));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)254, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(transport_layer_address_info.pack(bref));
  }
  if (cells_with_ss_bs_activ_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)745, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_with_ss_bs_activ_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cfg_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_cu_cfg_upd_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 1: {
        cells_failed_to_be_activ_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_failed_to_be_activ_list, bref, 1, 512, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 132: {
        gnb_cu_tnl_assoc_setup_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(gnb_cu_tnl_assoc_setup_list, bref, 1, 32, true));
        break;
      }
      case 134: {
        gnb_cu_tnl_assoc_failed_to_setup_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(gnb_cu_tnl_assoc_failed_to_setup_list, bref, 1, 32, true));
        break;
      }
      case 189: {
        ded_si_delivery_needed_ue_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ded_si_delivery_needed_ue_list, bref, 1, 65536, true));
        break;
      }
      case 254: {
        transport_layer_address_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(transport_layer_address_info.unpack(bref));
        break;
      }
      case 745: {
        cells_with_ss_bs_activ_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_with_ss_bs_activ_list, bref, 1, 512, true));
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
void gnb_cu_cfg_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (cells_failed_to_be_activ_list_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_failed_to_be_activ_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (gnb_cu_tnl_assoc_setup_list_present) {
    j.start_obj();
    j.write_int("id", 132);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_tnl_assoc_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_cu_tnl_assoc_failed_to_setup_list_present) {
    j.start_obj();
    j.write_int("id", 134);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : gnb_cu_tnl_assoc_failed_to_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ded_si_delivery_needed_ue_list_present) {
    j.start_obj();
    j.write_int("id", 189);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ded_si_delivery_needed_ue_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (cells_with_ss_bs_activ_list_present) {
    j.start_obj();
    j.write_int("id", 745);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cells_with_ss_bs_activ_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// GNBCUConfigurationUpdateFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_cu_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_cu_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_cu_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_cu_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 77:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_cu_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_cu_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_cu_cfg_upd_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 77: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 7: {
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
void gnb_cu_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNBDUConfigurationUpdateIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_cfg_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 58, 62, 60, 89, 189, 42, 228, 254, 446, 45, 427, 761, 765};
  return map_enum_number(names, 14, idx, "id");
}
bool gnb_du_cfg_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 58, 62, 60, 89, 189, 42, 228, 254, 446, 45, 427, 761, 765};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_cfg_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 58:
      return crit_e::reject;
    case 62:
      return crit_e::reject;
    case 60:
      return crit_e::reject;
    case 89:
      return crit_e::reject;
    case 189:
      return crit_e::ignore;
    case 42:
      return crit_e::reject;
    case 228:
      return crit_e::reject;
    case 254:
      return crit_e::ignore;
    case 446:
      return crit_e::ignore;
    case 45:
      return crit_e::ignore;
    case 427:
      return crit_e::ignore;
    case 761:
      return crit_e::reject;
    case 765:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_cfg_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 58:
      return presence_e::optional;
    case 62:
      return presence_e::optional;
    case 60:
      return presence_e::optional;
    case 89:
      return presence_e::optional;
    case 189:
      return presence_e::optional;
    case 42:
      return presence_e::optional;
    case 228:
      return presence_e::optional;
    case 254:
      return presence_e::optional;
    case 446:
      return presence_e::optional;
    case 45:
      return presence_e::optional;
    case 427:
      return presence_e::optional;
    case 761:
      return presence_e::optional;
    case 765:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_cfg_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += served_cells_to_add_list_present ? 1 : 0;
  nof_ies += served_cells_to_modify_list_present ? 1 : 0;
  nof_ies += served_cells_to_delete_list_present ? 1 : 0;
  nof_ies += cells_status_list_present ? 1 : 0;
  nof_ies += ded_si_delivery_needed_ue_list_present ? 1 : 0;
  nof_ies += gnb_du_id_present ? 1 : 0;
  nof_ies += gnb_du_tnl_assoc_to_rem_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += coverage_mod_notif_present ? 1 : 0;
  nof_ies += gnb_du_name_present ? 1 : 0;
  nof_ies += extended_gnb_du_name_present ? 1 : 0;
  nof_ies += rrc_terminating_iab_donor_related_info_present ? 1 : 0;
  nof_ies += mobile_iab_mt_user_location_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (served_cells_to_add_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)58, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, served_cells_to_add_list, 1, 512, true));
  }
  if (served_cells_to_modify_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)62, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, served_cells_to_modify_list, 1, 512, true));
  }
  if (served_cells_to_delete_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)60, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, served_cells_to_delete_list, 1, 512, true));
  }
  if (cells_status_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)89, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_status_list, 0, 512, true));
  }
  if (ded_si_delivery_needed_ue_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)189, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ded_si_delivery_needed_ue_list, 1, 65536, true));
  }
  if (gnb_du_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)42, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_id, (uint64_t)0u, (uint64_t)68719476735u, false, true));
  }
  if (gnb_du_tnl_assoc_to_rem_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)228, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, gnb_du_tnl_assoc_to_rem_list, 1, 32, true));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)254, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(transport_layer_address_info.pack(bref));
  }
  if (coverage_mod_notif_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)446, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(coverage_mod_notif.pack(bref));
  }
  if (gnb_du_name_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)45, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(gnb_du_name.pack(bref));
  }
  if (extended_gnb_du_name_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)427, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(extended_gnb_du_name.pack(bref));
  }
  if (rrc_terminating_iab_donor_related_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)761, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(rrc_terminating_iab_donor_related_info.pack(bref));
  }
  if (mobile_iab_mt_user_location_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)765, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mobile_iab_mt_user_location_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_cfg_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_cfg_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 58: {
        served_cells_to_add_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(served_cells_to_add_list, bref, 1, 512, true));
        break;
      }
      case 62: {
        served_cells_to_modify_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(served_cells_to_modify_list, bref, 1, 512, true));
        break;
      }
      case 60: {
        served_cells_to_delete_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(served_cells_to_delete_list, bref, 1, 512, true));
        break;
      }
      case 89: {
        cells_status_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_status_list, bref, 0, 512, true));
        break;
      }
      case 189: {
        ded_si_delivery_needed_ue_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ded_si_delivery_needed_ue_list, bref, 1, 65536, true));
        break;
      }
      case 42: {
        gnb_du_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_id, bref, (uint64_t)0u, (uint64_t)68719476735u, false, true));
        break;
      }
      case 228: {
        gnb_du_tnl_assoc_to_rem_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(gnb_du_tnl_assoc_to_rem_list, bref, 1, 32, true));
        break;
      }
      case 254: {
        transport_layer_address_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(transport_layer_address_info.unpack(bref));
        break;
      }
      case 446: {
        coverage_mod_notif_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(coverage_mod_notif.unpack(bref));
        break;
      }
      case 45: {
        gnb_du_name_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(gnb_du_name.unpack(bref));
        break;
      }
      case 427: {
        extended_gnb_du_name_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(extended_gnb_du_name.unpack(bref));
        break;
      }
      case 761: {
        rrc_terminating_iab_donor_related_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(rrc_terminating_iab_donor_related_info.unpack(bref));
        break;
      }
      case 765: {
        mobile_iab_mt_user_location_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mobile_iab_mt_user_location_info.unpack(bref));
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
void gnb_du_cfg_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (served_cells_to_add_list_present) {
    j.start_obj();
    j.write_int("id", 58);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : served_cells_to_add_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (served_cells_to_modify_list_present) {
    j.start_obj();
    j.write_int("id", 62);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : served_cells_to_modify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (served_cells_to_delete_list_present) {
    j.start_obj();
    j.write_int("id", 60);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : served_cells_to_delete_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (cells_status_list_present) {
    j.start_obj();
    j.write_int("id", 89);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_status_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ded_si_delivery_needed_ue_list_present) {
    j.start_obj();
    j.write_int("id", 189);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ded_si_delivery_needed_ue_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (gnb_du_id_present) {
    j.start_obj();
    j.write_int("id", 42);
    j.write_str("criticality", "reject");
    j.write_uint("value", gnb_du_id);
    j.end_obj();
  }
  if (gnb_du_tnl_assoc_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 228);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : gnb_du_tnl_assoc_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (coverage_mod_notif_present) {
    j.start_obj();
    j.write_int("id", 446);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    coverage_mod_notif.to_json(j);
    j.end_obj();
  }
  if (gnb_du_name_present) {
    j.start_obj();
    j.write_int("id", 45);
    j.write_str("criticality", "ignore");
    j.write_str("value", gnb_du_name.to_string());
    j.end_obj();
  }
  if (extended_gnb_du_name_present) {
    j.start_obj();
    j.write_int("id", 427);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_gnb_du_name.to_json(j);
    j.end_obj();
  }
  if (rrc_terminating_iab_donor_related_info_present) {
    j.start_obj();
    j.write_int("id", 761);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    rrc_terminating_iab_donor_related_info.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_mt_user_location_info_present) {
    j.start_obj();
    j.write_int("id", 765);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mobile_iab_mt_user_location_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNBDUConfigurationUpdateAcknowledgeIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_cfg_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 3, 7, 5, 254, 287, 281, 449};
  return map_enum_number(names, 8, idx, "id");
}
bool gnb_du_cfg_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 3, 7, 5, 254, 287, 281, 449};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_cfg_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 5:
      return crit_e::reject;
    case 254:
      return crit_e::ignore;
    case 287:
      return crit_e::reject;
    case 281:
      return crit_e::ignore;
    case 449:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_cfg_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 3:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 5:
      return presence_e::optional;
    case 254:
      return presence_e::optional;
    case 287:
      return presence_e::optional;
    case 281:
      return presence_e::optional;
    case 449:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_cfg_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += cells_to_be_activ_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += cells_to_be_deactiv_list_present ? 1 : 0;
  nof_ies += transport_layer_address_info_present ? 1 : 0;
  nof_ies += ul_bh_non_up_traffic_map_present ? 1 : 0;
  nof_ies += bap_address_present ? 1 : 0;
  nof_ies += cells_for_son_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (cells_to_be_activ_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)3, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_to_be_activ_list, 1, 512, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (cells_to_be_deactiv_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)5, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_to_be_deactiv_list, 1, 512, true));
  }
  if (transport_layer_address_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)254, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(transport_layer_address_info.pack(bref));
  }
  if (ul_bh_non_up_traffic_map_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)287, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ul_bh_non_up_traffic_map.pack(bref));
  }
  if (bap_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)281, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(bap_address.pack(bref));
  }
  if (cells_for_son_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)449, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_for_son_list, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_cfg_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_cfg_upd_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 3: {
        cells_to_be_activ_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_to_be_activ_list, bref, 1, 512, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 5: {
        cells_to_be_deactiv_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_to_be_deactiv_list, bref, 1, 512, true));
        break;
      }
      case 254: {
        transport_layer_address_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(transport_layer_address_info.unpack(bref));
        break;
      }
      case 287: {
        ul_bh_non_up_traffic_map_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ul_bh_non_up_traffic_map.unpack(bref));
        break;
      }
      case 281: {
        bap_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(bap_address.unpack(bref));
        break;
      }
      case 449: {
        cells_for_son_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_for_son_list, bref, 1, 256, true));
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
void gnb_du_cfg_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (cells_to_be_activ_list_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_to_be_activ_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (cells_to_be_deactiv_list_present) {
    j.start_obj();
    j.write_int("id", 5);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_to_be_deactiv_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (transport_layer_address_info_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    transport_layer_address_info.to_json(j);
    j.end_obj();
  }
  if (ul_bh_non_up_traffic_map_present) {
    j.start_obj();
    j.write_int("id", 287);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ul_bh_non_up_traffic_map.to_json(j);
    j.end_obj();
  }
  if (bap_address_present) {
    j.start_obj();
    j.write_int("id", 281);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    bap_address.to_json(j);
    j.end_obj();
  }
  if (cells_for_son_list_present) {
    j.start_obj();
    j.write_int("id", 449);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cells_for_son_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// GNBDUConfigurationUpdateFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_du_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 77:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_cfg_upd_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 77: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 7: {
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
void gnb_du_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNBDUResourceConfigurationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_res_cfg_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 288, 289, 533, 534};
  return map_enum_number(names, 5, idx, "id");
}
bool gnb_du_res_cfg_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 288, 289, 533, 534};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_res_cfg_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 288:
      return crit_e::reject;
    case 289:
      return crit_e::reject;
    case 533:
      return crit_e::reject;
    case 534:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_res_cfg_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 288:
      return presence_e::optional;
    case 289:
      return presence_e::optional;
    case 533:
      return presence_e::optional;
    case 534:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_res_cfg_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += activ_cells_to_be_upd_list_present ? 1 : 0;
  nof_ies += child_nodes_list_present ? 1 : 0;
  nof_ies += neighbour_node_cells_list_present ? 1 : 0;
  nof_ies += serving_cells_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (activ_cells_to_be_upd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)288, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, activ_cells_to_be_upd_list, 1, 512, true));
  }
  if (child_nodes_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)289, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, child_nodes_list, 1, 1024, true));
  }
  if (neighbour_node_cells_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)533, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, neighbour_node_cells_list, 1, 1024, true));
  }
  if (serving_cells_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)534, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, serving_cells_list, 1, 32, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_res_cfg_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_res_cfg_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 288: {
        activ_cells_to_be_upd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(activ_cells_to_be_upd_list, bref, 1, 512, true));
        break;
      }
      case 289: {
        child_nodes_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(child_nodes_list, bref, 1, 1024, true));
        break;
      }
      case 533: {
        neighbour_node_cells_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(neighbour_node_cells_list, bref, 1, 1024, true));
        break;
      }
      case 534: {
        serving_cells_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(serving_cells_list, bref, 1, 32, true));
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
void gnb_du_res_cfg_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (activ_cells_to_be_upd_list_present) {
    j.start_obj();
    j.write_int("id", 288);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : activ_cells_to_be_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (child_nodes_list_present) {
    j.start_obj();
    j.write_int("id", 289);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : child_nodes_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (neighbour_node_cells_list_present) {
    j.start_obj();
    j.write_int("id", 533);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : neighbour_node_cells_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (serving_cells_list_present) {
    j.start_obj();
    j.write_int("id", 534);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : serving_cells_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// GNBDUResourceConfigurationAcknowledgeIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_res_cfg_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 7};
  return map_enum_number(names, 2, idx, "id");
}
bool gnb_du_res_cfg_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_res_cfg_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_res_cfg_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_res_cfg_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_res_cfg_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_res_cfg_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 7: {
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
void gnb_du_res_cfg_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNBDUResourceConfigurationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_res_cfg_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_du_res_cfg_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_res_cfg_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_res_cfg_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 77:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_res_cfg_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_res_cfg_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_res_cfg_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 77: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 7: {
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
void gnb_du_res_cfg_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// GNBDUResourceCoordinationRequest-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_res_coordination_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 106, 101, 213};
  return map_enum_number(names, 4, idx, "id");
}
bool gnb_du_res_coordination_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 106, 101, 213};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_res_coordination_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 106:
      return crit_e::reject;
    case 101:
      return crit_e::reject;
    case 213:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_res_coordination_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 106:
      return presence_e::mandatory;
    case 101:
      return presence_e::mandatory;
    case 213:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_res_coordination_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ignore_res_coordination_container_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)106, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(request_type.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)101, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(eutra_nr_cell_res_coordination_req_container.pack(bref));
  }
  if (ignore_res_coordination_container_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)213, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ignore_res_coordination_container.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_res_coordination_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_res_coordination_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 106: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(request_type.unpack(bref));
        break;
      }
      case 101: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(eutra_nr_cell_res_coordination_req_container.unpack(bref));
        break;
      }
      case 213: {
        ignore_res_coordination_container_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ignore_res_coordination_container.unpack(bref));
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
void gnb_du_res_coordination_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 106);
  j.write_str("criticality", "reject");
  j.write_str("value", request_type.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 101);
  j.write_str("criticality", "reject");
  j.write_str("value", eutra_nr_cell_res_coordination_req_container.to_string());
  j.end_obj();
  if (ignore_res_coordination_container_present) {
    j.start_obj();
    j.write_int("id", 213);
    j.write_str("criticality", "reject");
    j.write_str("value", "yes");
    j.end_obj();
  }
  j.end_array();
}

// GNBDUResourceCoordinationResponse-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_res_coordination_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 102};
  return map_enum_number(names, 2, idx, "id");
}
bool gnb_du_res_coordination_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 102};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_res_coordination_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 102:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_res_coordination_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 102:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_res_coordination_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)102, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(eutra_nr_cell_res_coordination_req_ack_container.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_res_coordination_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_res_coordination_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 102: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(eutra_nr_cell_res_coordination_req_ack_container.unpack(bref));
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
void gnb_du_res_coordination_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 102);
  j.write_str("criticality", "reject");
  j.write_str("value", eutra_nr_cell_res_coordination_req_ack_container.to_string());
  j.end_obj();
  j.end_array();
}

// GNBDUStatusIndicationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t gnb_du_status_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 172, 509};
  return map_enum_number(names, 3, idx, "id");
}
bool gnb_du_status_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 172, 509};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e gnb_du_status_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 172:
      return crit_e::reject;
    case 509:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e gnb_du_status_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 172:
      return presence_e::mandatory;
    case 509:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE gnb_du_status_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += iab_congestion_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)172, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(gnb_du_overload_info.pack(bref));
  }
  if (iab_congestion_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)509, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(iab_congestion_ind.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE gnb_du_status_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != gnb_du_status_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 172: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(gnb_du_overload_info.unpack(bref));
        break;
      }
      case 509: {
        iab_congestion_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(iab_congestion_ind.unpack(bref));
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
void gnb_du_status_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 172);
  j.write_str("criticality", "reject");
  j.write_str("value", gnb_du_overload_info.to_string());
  j.end_obj();
  if (iab_congestion_ind_present) {
    j.start_obj();
    j.write_int("id", 509);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    iab_congestion_ind.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// IABTNLAddressFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t iab_tnl_address_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool iab_tnl_address_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_tnl_address_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_tnl_address_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 77:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_tnl_address_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_tnl_address_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != iab_tnl_address_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 77: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 7: {
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
void iab_tnl_address_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// IABTNLAddressRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t iab_tnl_address_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 297, 296, 292, 513};
  return map_enum_number(names, 5, idx, "id");
}
bool iab_tnl_address_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 297, 296, 292, 513};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_tnl_address_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 297:
      return crit_e::reject;
    case 296:
      return crit_e::reject;
    case 292:
      return crit_e::reject;
    case 513:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_tnl_address_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 297:
      return presence_e::optional;
    case 296:
      return presence_e::optional;
    case 292:
      return presence_e::optional;
    case 513:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_tnl_address_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ia_bv4_addresses_requested_present ? 1 : 0;
  nof_ies += iab_ip_v6_request_type_present ? 1 : 0;
  nof_ies += iab_tnl_addresses_to_rem_list_present ? 1 : 0;
  nof_ies += iab_tnl_addresses_exception_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ia_bv4_addresses_requested_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)297, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ia_bv4_addresses_requested.pack(bref));
  }
  if (iab_ip_v6_request_type_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)296, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(iab_ip_v6_request_type.pack(bref));
  }
  if (iab_tnl_addresses_to_rem_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)292, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, iab_tnl_addresses_to_rem_list, 1, 1024, true));
  }
  if (iab_tnl_addresses_exception_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)513, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(iab_tnl_addresses_exception.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_tnl_address_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != iab_tnl_address_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 297: {
        ia_bv4_addresses_requested_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ia_bv4_addresses_requested.unpack(bref));
        break;
      }
      case 296: {
        iab_ip_v6_request_type_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(iab_ip_v6_request_type.unpack(bref));
        break;
      }
      case 292: {
        iab_tnl_addresses_to_rem_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(iab_tnl_addresses_to_rem_list, bref, 1, 1024, true));
        break;
      }
      case 513: {
        iab_tnl_addresses_exception_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(iab_tnl_addresses_exception.unpack(bref));
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
void iab_tnl_address_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ia_bv4_addresses_requested_present) {
    j.start_obj();
    j.write_int("id", 297);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ia_bv4_addresses_requested.to_json(j);
    j.end_obj();
  }
  if (iab_ip_v6_request_type_present) {
    j.start_obj();
    j.write_int("id", 296);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    iab_ip_v6_request_type.to_json(j);
    j.end_obj();
  }
  if (iab_tnl_addresses_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 292);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : iab_tnl_addresses_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (iab_tnl_addresses_exception_present) {
    j.start_obj();
    j.write_int("id", 513);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    iab_tnl_addresses_exception.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// IABTNLAddressResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t iab_tnl_address_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 294};
  return map_enum_number(names, 2, idx, "id");
}
bool iab_tnl_address_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 294};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_tnl_address_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 294:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_tnl_address_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 294:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_tnl_address_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)294, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, iab_allocated_tnl_address_list, 1, 1024, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_tnl_address_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != iab_tnl_address_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 294: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(iab_allocated_tnl_address_list, bref, 1, 1024, true));
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
void iab_tnl_address_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 294);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : iab_allocated_tnl_address_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// IABUPConfigurationUpdateFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t iab_up_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool iab_up_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 77, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_up_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 77:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_up_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 77:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_up_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (time_to_wait_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)77, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_to_wait.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_up_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != iab_up_cfg_upd_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 77: {
        time_to_wait_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_to_wait.unpack(bref));
        break;
      }
      case 7: {
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
void iab_up_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 77);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// IABUPConfigurationUpdateRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t iab_up_cfg_upd_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 300, 302};
  return map_enum_number(names, 3, idx, "id");
}
bool iab_up_cfg_upd_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 300, 302};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_up_cfg_upd_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 300:
      return crit_e::ignore;
    case 302:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_up_cfg_upd_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 300:
      return presence_e::optional;
    case 302:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_up_cfg_upd_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ul_up_tnl_info_to_upd_list_present ? 1 : 0;
  nof_ies += ul_up_tnl_address_to_upd_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ul_up_tnl_info_to_upd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)300, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ul_up_tnl_info_to_upd_list, 1, 32678, true));
  }
  if (ul_up_tnl_address_to_upd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)302, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ul_up_tnl_address_to_upd_list, 1, 8, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_up_cfg_upd_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != iab_up_cfg_upd_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 300: {
        ul_up_tnl_info_to_upd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ul_up_tnl_info_to_upd_list, bref, 1, 32678, true));
        break;
      }
      case 302: {
        ul_up_tnl_address_to_upd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ul_up_tnl_address_to_upd_list, bref, 1, 8, true));
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
void iab_up_cfg_upd_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ul_up_tnl_info_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 300);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ul_up_tnl_info_to_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ul_up_tnl_address_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 302);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ul_up_tnl_address_to_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// IABUPConfigurationUpdateResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t iab_up_cfg_upd_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 7, 304};
  return map_enum_number(names, 3, idx, "id");
}
bool iab_up_cfg_upd_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 7, 304};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e iab_up_cfg_upd_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 304:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e iab_up_cfg_upd_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    case 304:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE iab_up_cfg_upd_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += dl_up_tnl_address_to_upd_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (dl_up_tnl_address_to_upd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)304, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, dl_up_tnl_address_to_upd_list, 1, 8, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE iab_up_cfg_upd_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != iab_up_cfg_upd_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 304: {
        dl_up_tnl_address_to_upd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(dl_up_tnl_address_to_upd_list, bref, 1, 8, true));
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
void iab_up_cfg_upd_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (dl_up_tnl_address_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 304);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : dl_up_tnl_address_to_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// MIABF1SetupOutcomeNotificationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t miabf1_setup_outcome_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 760, 758, 787};
  return map_enum_number(names, 4, idx, "id");
}
bool miabf1_setup_outcome_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 760, 758, 787};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e miabf1_setup_outcome_notif_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 760:
      return crit_e::reject;
    case 758:
      return crit_e::ignore;
    case 787:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e miabf1_setup_outcome_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 760:
      return presence_e::mandatory;
    case 758:
      return presence_e::optional;
    case 787:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE miabf1_setup_outcome_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += activ_cells_map_list_present ? 1 : 0;
  nof_ies += target_f1_terminating_donor_gnb_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)760, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(f1_setup_outcome.pack(bref));
  }
  if (activ_cells_map_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)758, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, activ_cells_map_list, 1, 512, true));
  }
  if (target_f1_terminating_donor_gnb_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)787, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(target_f1_terminating_donor_gnb_id.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE miabf1_setup_outcome_notif_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != miabf1_setup_outcome_notif_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 760: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(f1_setup_outcome.unpack(bref));
        break;
      }
      case 758: {
        activ_cells_map_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(activ_cells_map_list, bref, 1, 512, true));
        break;
      }
      case 787: {
        target_f1_terminating_donor_gnb_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(target_f1_terminating_donor_gnb_id.unpack(bref));
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
void miabf1_setup_outcome_notif_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 760);
  j.write_str("criticality", "reject");
  j.write_str("value", f1_setup_outcome.to_string());
  j.end_obj();
  if (activ_cells_map_list_present) {
    j.start_obj();
    j.write_int("id", 758);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : activ_cells_map_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (target_f1_terminating_donor_gnb_id_present) {
    j.start_obj();
    j.write_int("id", 787);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    target_f1_terminating_donor_gnb_id.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MIABF1SetupTriggeringIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t miabf1_setup_trigger_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 755, 756, 757};
  return map_enum_number(names, 4, idx, "id");
}
bool miabf1_setup_trigger_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 755, 756, 757};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e miabf1_setup_trigger_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 755:
      return crit_e::reject;
    case 756:
      return crit_e::ignore;
    case 757:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e miabf1_setup_trigger_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 755:
      return presence_e::mandatory;
    case 756:
      return presence_e::optional;
    case 757:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE miabf1_setup_trigger_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += target_gnb_ip_address_present ? 1 : 0;
  nof_ies += target_se_gw_ip_address_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)755, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(target_gnb_id.pack(bref));
  }
  if (target_gnb_ip_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)756, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(target_gnb_ip_address.pack(bref));
  }
  if (target_se_gw_ip_address_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)757, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(target_se_gw_ip_address.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE miabf1_setup_trigger_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != miabf1_setup_trigger_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 755: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(target_gnb_id.unpack(bref));
        break;
      }
      case 756: {
        target_gnb_ip_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(target_gnb_ip_address.unpack(bref));
        break;
      }
      case 757: {
        target_se_gw_ip_address_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(target_se_gw_ip_address.unpack(bref));
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
void miabf1_setup_trigger_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 755);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  target_gnb_id.to_json(j);
  j.end_obj();
  if (target_gnb_ip_address_present) {
    j.start_obj();
    j.write_int("id", 756);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    target_gnb_ip_address.to_json(j);
    j.end_obj();
  }
  if (target_se_gw_ip_address_present) {
    j.start_obj();
    j.write_int("id", 757);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    target_se_gw_ip_address.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MeasurementActivation-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t meas_activation_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 649, 570};
  return map_enum_number(names, 4, idx, "id");
}
bool meas_activation_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 649, 570};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 649:
      return crit_e::reject;
    case 570:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e meas_activation_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 649:
      return presence_e::mandatory;
    case 570:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE meas_activation_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += prs_meas_info_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)649, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(activation_request_type.pack(bref));
  }
  if (prs_meas_info_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)570, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, prs_meas_info_list, 1, 4, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_activation_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_activation_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 649: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(activation_request_type.unpack(bref));
        break;
      }
      case 570: {
        prs_meas_info_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(prs_meas_info_list, bref, 1, 4, true));
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 649);
  j.write_str("criticality", "reject");
  j.write_str("value", activation_request_type.to_string());
  j.end_obj();
  if (prs_meas_info_list_present) {
    j.start_obj();
    j.write_int("id", 570);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : prs_meas_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// MeasurementPreconfigurationConfirm-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t meas_precfg_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 650, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool meas_precfg_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 650, 7};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 650:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e meas_precfg_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 650:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE meas_precfg_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pos_meas_gap_pre_cfg_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (pos_meas_gap_pre_cfg_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)650, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_meas_gap_pre_cfg_list.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_precfg_confirm_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 650: {
        pos_meas_gap_pre_cfg_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_meas_gap_pre_cfg_list.unpack(bref));
        break;
      }
      case 7: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  if (pos_meas_gap_pre_cfg_list_present) {
    j.start_obj();
    j.write_int("id", 650);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pos_meas_gap_pre_cfg_list.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MeasurementPreconfigurationRefuse-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t meas_precfg_refuse_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 0, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool meas_precfg_refuse_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 0, 7};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e meas_precfg_refuse_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE meas_precfg_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_precfg_refuse_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MeasurementPreconfigurationRequired-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t meas_precfg_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 569};
  return map_enum_number(names, 3, idx, "id");
}
bool meas_precfg_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 569};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e meas_precfg_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 569:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e meas_precfg_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 569:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE meas_precfg_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)569, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_prs_info_list, 1, 256, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_precfg_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != meas_precfg_required_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 569: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_prs_info_list, bref, 1, 256, true));
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
void meas_precfg_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 569);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : trp_prs_info_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// MulticastCommonConfigurationRefuseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_common_cfg_refuse_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_common_cfg_refuse_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_common_cfg_refuse_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_common_cfg_refuse_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_common_cfg_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_common_cfg_refuse_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_common_cfg_refuse_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void multicast_common_cfg_refuse_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastCommonConfigurationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_common_cfg_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 774};
  return map_enum_number(names, 2, idx, "id");
}
bool multicast_common_cfg_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 774};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_common_cfg_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 774:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_common_cfg_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 774:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_common_cfg_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += multicast_cu2_du_common_rrc_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (multicast_cu2_du_common_rrc_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)774, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(multicast_cu2_du_common_rrc_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_common_cfg_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_common_cfg_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 774: {
        multicast_cu2_du_common_rrc_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(multicast_cu2_du_common_rrc_info.unpack(bref));
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
void multicast_common_cfg_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (multicast_cu2_du_common_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 774);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    multicast_cu2_du_common_rrc_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastCommonConfigurationResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_common_cfg_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 7};
  return map_enum_number(names, 2, idx, "id");
}
bool multicast_common_cfg_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_common_cfg_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_common_cfg_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_common_cfg_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_common_cfg_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_common_cfg_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 7: {
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
void multicast_common_cfg_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextModificationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 0, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool multicast_context_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_mod_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_mod_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_mod_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_mod_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_mod_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void multicast_context_mod_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextModificationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 481, 500, 494, 496, 769, 770};
  return map_enum_number(names, 8, idx, "id");
}
bool multicast_context_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 481, 500, 494, 496, 769, 770};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 481:
      return crit_e::reject;
    case 500:
      return crit_e::reject;
    case 494:
      return crit_e::reject;
    case 496:
      return crit_e::reject;
    case 769:
      return crit_e::reject;
    case 770:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 481:
      return presence_e::optional;
    case 500:
      return presence_e::optional;
    case 494:
      return presence_e::optional;
    case 496:
      return presence_e::optional;
    case 769:
      return presence_e::optional;
    case 770:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += mbs_service_area_present ? 1 : 0;
  nof_ies += multicast_m_rbs_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += multicast_m_rbs_to_be_modified_list_present ? 1 : 0;
  nof_ies += multicast_m_rbs_to_be_released_list_present ? 1 : 0;
  nof_ies += multicast_cu2_du_rrc_info_present ? 1 : 0;
  nof_ies += mbs_multicast_session_reception_state_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (mbs_service_area_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)481, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_service_area.pack(bref));
  }
  if (multicast_m_rbs_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)500, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_to_be_setup_mod_list, 1, 32, true));
  }
  if (multicast_m_rbs_to_be_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)494, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_to_be_modified_list, 1, 32, true));
  }
  if (multicast_m_rbs_to_be_released_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)496, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_to_be_released_list, 1, 32, true));
  }
  if (multicast_cu2_du_rrc_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)769, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(multicast_cu2_du_rrc_info.pack(bref));
  }
  if (mbs_multicast_session_reception_state_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)770, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_multicast_session_reception_state.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_mod_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 481: {
        mbs_service_area_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_service_area.unpack(bref));
        break;
      }
      case 500: {
        multicast_m_rbs_to_be_setup_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_to_be_setup_mod_list, bref, 1, 32, true));
        break;
      }
      case 494: {
        multicast_m_rbs_to_be_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_to_be_modified_list, bref, 1, 32, true));
        break;
      }
      case 496: {
        multicast_m_rbs_to_be_released_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_to_be_released_list, bref, 1, 32, true));
        break;
      }
      case 769: {
        multicast_cu2_du_rrc_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(multicast_cu2_du_rrc_info.unpack(bref));
        break;
      }
      case 770: {
        mbs_multicast_session_reception_state_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_multicast_session_reception_state.unpack(bref));
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
void multicast_context_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (mbs_service_area_present) {
    j.start_obj();
    j.write_int("id", 481);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    mbs_service_area.to_json(j);
    j.end_obj();
  }
  if (multicast_m_rbs_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 500);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : multicast_m_rbs_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_m_rbs_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 494);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : multicast_m_rbs_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_m_rbs_to_be_released_list_present) {
    j.start_obj();
    j.write_int("id", 496);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : multicast_m_rbs_to_be_released_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_cu2_du_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 769);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    multicast_cu2_du_rrc_info.to_json(j);
    j.end_obj();
  }
  if (mbs_multicast_session_reception_state_present) {
    j.start_obj();
    j.write_int("id", 770);
    j.write_str("criticality", "reject");
    j.write_str("value", mbs_multicast_session_reception_state.to_string());
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextModificationResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 492, 486, 488, 482, 7, 772};
  return map_enum_number(names, 8, idx, "id");
}
bool multicast_context_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 492, 486, 488, 482, 7, 772};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 492:
      return crit_e::reject;
    case 486:
      return crit_e::ignore;
    case 488:
      return crit_e::reject;
    case 482:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 772:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 492:
      return presence_e::optional;
    case 486:
      return presence_e::optional;
    case 488:
      return presence_e::optional;
    case 482:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 772:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += multicast_m_rbs_setup_mod_list_present ? 1 : 0;
  nof_ies += multicast_m_rbs_failed_to_be_setup_mod_list_present ? 1 : 0;
  nof_ies += multicast_m_rbs_modified_list_present ? 1 : 0;
  nof_ies += multicast_m_rbs_failed_to_be_modified_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += multicast_du2_cu_rrc_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (multicast_m_rbs_setup_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)492, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_setup_mod_list, 1, 32, true));
  }
  if (multicast_m_rbs_failed_to_be_setup_mod_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)486, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_failed_to_be_setup_mod_list, 1, 32, true));
  }
  if (multicast_m_rbs_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)488, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_modified_list, 1, 32, true));
  }
  if (multicast_m_rbs_failed_to_be_modified_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)482, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_failed_to_be_modified_list, 1, 32, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (multicast_du2_cu_rrc_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)772, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(multicast_du2_cu_rrc_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_mod_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 492: {
        multicast_m_rbs_setup_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_setup_mod_list, bref, 1, 32, true));
        break;
      }
      case 486: {
        multicast_m_rbs_failed_to_be_setup_mod_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_failed_to_be_setup_mod_list, bref, 1, 32, true));
        break;
      }
      case 488: {
        multicast_m_rbs_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_modified_list, bref, 1, 32, true));
        break;
      }
      case 482: {
        multicast_m_rbs_failed_to_be_modified_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_failed_to_be_modified_list, bref, 1, 32, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 772: {
        multicast_du2_cu_rrc_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(multicast_du2_cu_rrc_info.unpack(bref));
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
void multicast_context_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (multicast_m_rbs_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 492);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : multicast_m_rbs_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_m_rbs_failed_to_be_setup_mod_list_present) {
    j.start_obj();
    j.write_int("id", 486);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : multicast_m_rbs_failed_to_be_setup_mod_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_m_rbs_modified_list_present) {
    j.start_obj();
    j.write_int("id", 488);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : multicast_m_rbs_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (multicast_m_rbs_failed_to_be_modified_list_present) {
    j.start_obj();
    j.write_int("id", 482);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : multicast_m_rbs_failed_to_be_modified_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (multicast_du2_cu_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 772);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    multicast_du2_cu_rrc_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextNotificationConfirmIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_notif_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_context_notif_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_notif_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_notif_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_notif_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_notif_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_notif_confirm_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 7: {
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
void multicast_context_notif_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextNotificationIndicationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_notif_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 772};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_context_notif_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 772};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_notif_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 772:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_notif_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 772:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_notif_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += multicast_du2_cu_rrc_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (multicast_du2_cu_rrc_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)772, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(multicast_du2_cu_rrc_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_notif_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_notif_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 772: {
        multicast_du2_cu_rrc_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(multicast_du2_cu_rrc_info.unpack(bref));
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
void multicast_context_notif_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (multicast_du2_cu_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 772);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    multicast_du2_cu_rrc_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextNotificationRefuseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_notif_refuse_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 7, 0};
  return map_enum_number(names, 4, idx, "id");
}
bool multicast_context_notif_refuse_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 7, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_notif_refuse_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_notif_refuse_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_notif_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_notif_refuse_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_notif_refuse_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
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
void multicast_context_notif_refuse_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// MulticastContextReleaseCommandIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_context_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_release_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_release_cmd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
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
void multicast_context_release_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// MulticastContextReleaseCompleteIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_release_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_context_release_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_release_complete_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_release_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_release_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_release_complete_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_release_complete_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 7: {
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
void multicast_context_release_complete_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextReleaseRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 0};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_context_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_release_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
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
void multicast_context_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// MulticastContextSetupFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 0, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool multicast_context_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += gnb_du_mbs_f1ap_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (gnb_du_mbs_f1ap_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_setup_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        gnb_du_mbs_f1ap_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void multicast_context_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  if (gnb_du_mbs_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 452);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_mbs_f1ap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextSetupRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 455, 481, 456, 498, 769, 770};
  return map_enum_number(names, 7, idx, "id");
}
bool multicast_context_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 455, 481, 456, 498, 769, 770};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 455:
      return crit_e::reject;
    case 481:
      return crit_e::reject;
    case 456:
      return crit_e::reject;
    case 498:
      return crit_e::reject;
    case 769:
      return crit_e::reject;
    case 770:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 455:
      return presence_e::mandatory;
    case 481:
      return presence_e::optional;
    case 456:
      return presence_e::mandatory;
    case 498:
      return presence_e::mandatory;
    case 769:
      return presence_e::optional;
    case 770:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += mbs_service_area_present ? 1 : 0;
  nof_ies += multicast_cu2_du_rrc_info_present ? 1 : 0;
  nof_ies += mbs_multicast_session_reception_state_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)455, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_session_id.pack(bref));
  }
  if (mbs_service_area_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)481, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_service_area.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)456, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(snssai.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)498, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_to_be_setup_list, 1, 32, true));
  }
  if (multicast_cu2_du_rrc_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)769, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(multicast_cu2_du_rrc_info.pack(bref));
  }
  if (mbs_multicast_session_reception_state_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)770, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_multicast_session_reception_state.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_setup_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 455: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_session_id.unpack(bref));
        break;
      }
      case 481: {
        mbs_service_area_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_service_area.unpack(bref));
        break;
      }
      case 456: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(snssai.unpack(bref));
        break;
      }
      case 498: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_to_be_setup_list, bref, 1, 32, true));
        break;
      }
      case 769: {
        multicast_cu2_du_rrc_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(multicast_cu2_du_rrc_info.unpack(bref));
        break;
      }
      case 770: {
        mbs_multicast_session_reception_state_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_multicast_session_reception_state.unpack(bref));
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
void multicast_context_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 455);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_service_area_present) {
    j.start_obj();
    j.write_int("id", 481);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    mbs_service_area.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 456);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  snssai.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 498);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : multicast_m_rbs_to_be_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (multicast_cu2_du_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 769);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    multicast_cu2_du_rrc_info.to_json(j);
    j.end_obj();
  }
  if (mbs_multicast_session_reception_state_present) {
    j.start_obj();
    j.write_int("id", 770);
    j.write_str("criticality", "reject");
    j.write_str("value", mbs_multicast_session_reception_state.to_string());
    j.end_obj();
  }
  j.end_array();
}

// MulticastContextSetupResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_context_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 490, 484, 7, 772};
  return map_enum_number(names, 6, idx, "id");
}
bool multicast_context_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 490, 484, 7, 772};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_context_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 490:
      return crit_e::reject;
    case 484:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 772:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_context_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 490:
      return presence_e::mandatory;
    case 484:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 772:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_context_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += multicast_m_rbs_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += multicast_du2_cu_rrc_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)490, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_setup_list, 1, 32, true));
  }
  if (multicast_m_rbs_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)484, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_m_rbs_failed_to_be_setup_list, 1, 32, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (multicast_du2_cu_rrc_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)772, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(multicast_du2_cu_rrc_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_context_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_context_setup_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 490: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_setup_list, bref, 1, 32, true));
        break;
      }
      case 484: {
        multicast_m_rbs_failed_to_be_setup_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_m_rbs_failed_to_be_setup_list, bref, 1, 32, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 772: {
        multicast_du2_cu_rrc_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(multicast_du2_cu_rrc_info.unpack(bref));
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
void multicast_context_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 490);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : multicast_m_rbs_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (multicast_m_rbs_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 484);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : multicast_m_rbs_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (multicast_du2_cu_rrc_info_present) {
    j.start_obj();
    j.write_int("id", 772);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    multicast_du2_cu_rrc_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastDistributionReleaseCommandIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_distribution_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 502, 0};
  return map_enum_number(names, 4, idx, "id");
}
bool multicast_distribution_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 502, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_distribution_release_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 502:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_distribution_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 502:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_distribution_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)502, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_distribution_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_distribution_release_cmd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 502: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
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
void multicast_distribution_release_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 502);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_multicast_f1_u_context_descriptor.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// MulticastDistributionReleaseCompleteIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_distribution_release_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 502, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool multicast_distribution_release_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 502, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_distribution_release_complete_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 502:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_distribution_release_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 502:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_distribution_release_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)502, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_distribution_release_complete_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_distribution_release_complete_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 502: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
        break;
      }
      case 7: {
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
void multicast_distribution_release_complete_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 502);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_multicast_f1_u_context_descriptor.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastDistributionSetupFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_distribution_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 502, 0, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool multicast_distribution_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 502, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_distribution_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::ignore;
    case 502:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_distribution_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::optional;
    case 502:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_distribution_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += gnb_du_mbs_f1ap_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (gnb_du_mbs_f1ap_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)502, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_distribution_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_distribution_setup_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        gnb_du_mbs_f1ap_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 502: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void multicast_distribution_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  if (gnb_du_mbs_f1ap_id_present) {
    j.start_obj();
    j.write_int("id", 452);
    j.write_str("criticality", "ignore");
    j.write_uint("value", gnb_du_mbs_f1ap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 502);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_multicast_f1_u_context_descriptor.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastDistributionSetupRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_distribution_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 502, 503};
  return map_enum_number(names, 4, idx, "id");
}
bool multicast_distribution_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 502, 503};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_distribution_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 502:
      return crit_e::reject;
    case 503:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_distribution_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 502:
      return presence_e::mandatory;
    case 503:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_distribution_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)502, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)503, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_f1_u_context_to_be_setup_list, 1, 32, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_distribution_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_distribution_setup_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 502: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
        break;
      }
      case 503: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_f1_u_context_to_be_setup_list, bref, 1, 32, true));
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
void multicast_distribution_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 502);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_multicast_f1_u_context_descriptor.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 503);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : multicast_f1_u_context_to_be_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// MulticastDistributionSetupResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_distribution_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {451, 452, 502, 505, 507, 7, 681};
  return map_enum_number(names, 7, idx, "id");
}
bool multicast_distribution_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {451, 452, 502, 505, 507, 7, 681};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_distribution_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 451:
      return crit_e::reject;
    case 452:
      return crit_e::reject;
    case 502:
      return crit_e::reject;
    case 505:
      return crit_e::reject;
    case 507:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 681:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_distribution_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 451:
      return presence_e::mandatory;
    case 452:
      return presence_e::mandatory;
    case 502:
      return presence_e::mandatory;
    case 505:
      return presence_e::mandatory;
    case 507:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 681:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_distribution_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += multicast_f1_u_context_failed_to_be_setup_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)451, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)452, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_mbs_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)502, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)505, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_f1_u_context_setup_list, 1, 32, true));
  }
  if (multicast_f1_u_context_failed_to_be_setup_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)507, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, multicast_f1_u_context_failed_to_be_setup_list, 1, 32, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)681, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(multicast_f1_u_context_ref_cu.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_distribution_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_distribution_setup_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 451: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 452: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_mbs_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 502: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_multicast_f1_u_context_descriptor.unpack(bref));
        break;
      }
      case 505: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_f1_u_context_setup_list, bref, 1, 32, true));
        break;
      }
      case 507: {
        multicast_f1_u_context_failed_to_be_setup_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(multicast_f1_u_context_failed_to_be_setup_list, bref, 1, 32, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 681: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(multicast_f1_u_context_ref_cu.unpack(bref));
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
void multicast_distribution_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 451);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 452);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_mbs_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 502);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_multicast_f1_u_context_descriptor.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 505);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : multicast_f1_u_context_setup_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (multicast_f1_u_context_failed_to_be_setup_list_present) {
    j.start_obj();
    j.write_int("id", 507);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : multicast_f1_u_context_failed_to_be_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 681);
  j.write_str("criticality", "reject");
  j.write_str("value", multicast_f1_u_context_ref_cu.to_string());
  j.end_obj();
  j.end_array();
}

// MulticastGroupPagingIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t multicast_group_paging_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {455, 479, 687, 768};
  return map_enum_number(names, 4, idx, "id");
}
bool multicast_group_paging_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {455, 479, 687, 768};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_group_paging_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 455:
      return crit_e::reject;
    case 479:
      return crit_e::ignore;
    case 687:
      return crit_e::ignore;
    case 768:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_group_paging_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 455:
      return presence_e::mandatory;
    case 479:
      return presence_e::optional;
    case 687:
      return presence_e::optional;
    case 768:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_group_paging_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ue_id_list_for_paging_list_present ? 1 : 0;
  nof_ies += mc_paging_cell_list_present ? 1 : 0;
  nof_ies += ind_mc_inactive_reception_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)455, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mbs_session_id.pack(bref));
  }
  if (ue_id_list_for_paging_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)479, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ue_id_list_for_paging_list, 1, 4096, true));
  }
  if (mc_paging_cell_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)687, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, mc_paging_cell_list, 1, 512, true));
  }
  if (ind_mc_inactive_reception_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)768, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ind_mc_inactive_reception.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_group_paging_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != multicast_group_paging_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 455: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mbs_session_id.unpack(bref));
        break;
      }
      case 479: {
        ue_id_list_for_paging_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ue_id_list_for_paging_list, bref, 1, 4096, true));
        break;
      }
      case 687: {
        mc_paging_cell_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(mc_paging_cell_list, bref, 1, 512, true));
        break;
      }
      case 768: {
        ind_mc_inactive_reception_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ind_mc_inactive_reception.unpack(bref));
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
void multicast_group_paging_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 455);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (ue_id_list_for_paging_list_present) {
    j.start_obj();
    j.write_int("id", 479);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_id_list_for_paging_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (mc_paging_cell_list_present) {
    j.start_obj();
    j.write_int("id", 687);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : mc_paging_cell_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ind_mc_inactive_reception_present) {
    j.start_obj();
    j.write_int("id", 768);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// NetworkAccessRateReductionIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t network_access_rate_reduction_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 225};
  return map_enum_number(names, 2, idx, "id");
}
bool network_access_rate_reduction_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 225};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e network_access_rate_reduction_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 225:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e network_access_rate_reduction_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 225:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE network_access_rate_reduction_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)225, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(uac_assist_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE network_access_rate_reduction_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != network_access_rate_reduction_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 225: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(uac_assist_info.unpack(bref));
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
void network_access_rate_reduction_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 225);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  uac_assist_info.to_json(j);
  j.end_obj();
  j.end_array();
}

// NotifyIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t notify_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 137};
  return map_enum_number(names, 3, idx, "id");
}
bool notify_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 137};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e notify_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 137:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e notify_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 137:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE notify_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)137, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, drb_notify_list, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE notify_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != notify_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 137: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(drb_notify_list, bref, 1, 64, true));
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
void notify_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 137);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : drb_notify_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// PDCMeasurementFailureIndication-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pdc_meas_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 546, 0};
  return map_enum_number(names, 4, idx, "id");
}
bool pdc_meas_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 546, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdc_meas_fail_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 546:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdc_meas_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 546:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdc_meas_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)546, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_pdc_meas_id, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdc_meas_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdc_meas_fail_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 546: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_pdc_meas_id, bref, (uint8_t)1u, (uint8_t)16u, true, true));
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
void pdc_meas_fail_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 546);
  j.write_str("criticality", "ignore");
  j.write_int("value", ran_ue_pdc_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// PDCMeasurementInitiationFailure-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pdc_meas_initiation_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 546, 0, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool pdc_meas_initiation_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 546, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdc_meas_initiation_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 546:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdc_meas_initiation_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 546:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdc_meas_initiation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)546, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_pdc_meas_id, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdc_meas_initiation_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdc_meas_initiation_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 546: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_pdc_meas_id, bref, (uint8_t)1u, (uint8_t)16u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void pdc_meas_initiation_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 546);
  j.write_str("criticality", "ignore");
  j.write_int("value", ran_ue_pdc_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDCMeasurementInitiationRequest-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pdc_meas_initiation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 546, 545, 541, 542};
  return map_enum_number(names, 6, idx, "id");
}
bool pdc_meas_initiation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 546, 545, 541, 542};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdc_meas_initiation_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 546:
      return crit_e::reject;
    case 545:
      return crit_e::reject;
    case 541:
      return crit_e::reject;
    case 542:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdc_meas_initiation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 546:
      return presence_e::mandatory;
    case 545:
      return presence_e::mandatory;
    case 541:
      return presence_e::conditional;
    case 542:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdc_meas_initiation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += pdc_meas_periodicity_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)546, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_pdc_meas_id, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)545, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdc_report_type.pack(bref));
  }
  if (pdc_meas_periodicity_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)541, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdc_meas_periodicity.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)542, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pdc_meas_quantities, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdc_meas_initiation_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdc_meas_initiation_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 546: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_pdc_meas_id, bref, (uint8_t)1u, (uint8_t)16u, true, true));
        break;
      }
      case 545: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdc_report_type.unpack(bref));
        break;
      }
      case 541: {
        pdc_meas_periodicity_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdc_meas_periodicity.unpack(bref));
        break;
      }
      case 542: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pdc_meas_quantities, bref, 1, 16, true));
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
void pdc_meas_initiation_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 546);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_pdc_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 545);
  j.write_str("criticality", "reject");
  j.write_str("value", pdc_report_type.to_string());
  j.end_obj();
  if (pdc_meas_periodicity_present) {
    j.start_obj();
    j.write_int("id", 541);
    j.write_str("criticality", "reject");
    j.write_str("value", pdc_meas_periodicity.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 542);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdc_meas_quantities) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// PDCMeasurementInitiationResponse-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pdc_meas_initiation_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 546, 544, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool pdc_meas_initiation_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 546, 544, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdc_meas_initiation_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 546:
      return crit_e::reject;
    case 544:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdc_meas_initiation_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 546:
      return presence_e::mandatory;
    case 544:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdc_meas_initiation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pdc_meas_result_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)546, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_pdc_meas_id, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  if (pdc_meas_result_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)544, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdc_meas_result.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdc_meas_initiation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdc_meas_initiation_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 546: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_pdc_meas_id, bref, (uint8_t)1u, (uint8_t)16u, true, true));
        break;
      }
      case 544: {
        pdc_meas_result_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdc_meas_result.unpack(bref));
        break;
      }
      case 7: {
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
void pdc_meas_initiation_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 546);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_pdc_meas_id);
  j.end_obj();
  if (pdc_meas_result_present) {
    j.start_obj();
    j.write_int("id", 544);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pdc_meas_result.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDCMeasurementReport-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pdc_meas_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 546, 544};
  return map_enum_number(names, 4, idx, "id");
}
bool pdc_meas_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 546, 544};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdc_meas_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 546:
      return crit_e::reject;
    case 544:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdc_meas_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 546:
      return presence_e::mandatory;
    case 544:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdc_meas_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)546, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_pdc_meas_id, (uint8_t)1u, (uint8_t)16u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)544, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pdc_meas_result.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdc_meas_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdc_meas_report_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 546: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_pdc_meas_id, bref, (uint8_t)1u, (uint8_t)16u, true, true));
        break;
      }
      case 544: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pdc_meas_result.unpack(bref));
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
void pdc_meas_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 546);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_pdc_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 544);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  pdc_meas_result.to_json(j);
  j.end_obj();
  j.end_array();
}

// PDCMeasurementTerminationCommand-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pdc_meas_termination_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 546};
  return map_enum_number(names, 3, idx, "id");
}
bool pdc_meas_termination_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 546};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdc_meas_termination_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 546:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdc_meas_termination_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 546:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdc_meas_termination_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)546, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_ue_pdc_meas_id, (uint8_t)1u, (uint8_t)16u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdc_meas_termination_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pdc_meas_termination_cmd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 546: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_ue_pdc_meas_id, bref, (uint8_t)1u, (uint8_t)16u, true, true));
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
void pdc_meas_termination_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 546);
  j.write_str("criticality", "ignore");
  j.write_int("value", ran_ue_pdc_meas_id);
  j.end_obj();
  j.end_array();
}

// PRSConfigurationFailure-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t prs_cfg_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool prs_cfg_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 7};
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
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e prs_cfg_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE prs_cfg_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != prs_cfg_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PRSConfigurationRequest-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t prs_cfg_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 571, 549};
  return map_enum_number(names, 3, idx, "id");
}
bool prs_cfg_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 571, 549};
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
    case 78:
      return crit_e::reject;
    case 571:
      return crit_e::reject;
    case 549:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e prs_cfg_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 571:
      return presence_e::mandatory;
    case 549:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE prs_cfg_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)571, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(prs_cfg_request_type.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)549, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != prs_cfg_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 571: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(prs_cfg_request_type.unpack(bref));
        break;
      }
      case 549: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 571);
  j.write_str("criticality", "reject");
  j.write_str("value", prs_cfg_request_type.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 549);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : prstrp_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// PRSConfigurationResponse-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t prs_cfg_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 550, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool prs_cfg_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 550, 7};
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
    case 78:
      return crit_e::reject;
    case 550:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e prs_cfg_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 550:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE prs_cfg_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += prs_tx_trp_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (prs_tx_trp_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)550, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, prs_tx_trp_list, 1, 65535, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != prs_cfg_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 550: {
        prs_tx_trp_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(prs_tx_trp_list, bref, 1, 65535, true));
        break;
      }
      case 7: {
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
void prs_cfg_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (prs_tx_trp_list_present) {
    j.start_obj();
    j.write_int("id", 550);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : prs_tx_trp_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PWSCancelRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pws_cancel_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 142, 148, 157, 220};
  return map_enum_number(names, 5, idx, "id");
}
bool pws_cancel_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 142, 148, 157, 220};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pws_cancel_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 142:
      return crit_e::reject;
    case 148:
      return crit_e::reject;
    case 157:
      return crit_e::reject;
    case 220:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pws_cancel_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 142:
      return presence_e::mandatory;
    case 148:
      return presence_e::optional;
    case 157:
      return presence_e::optional;
    case 220:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pws_cancel_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += broadcast_to_be_cancelled_list_present ? 1 : 0;
  nof_ies += cancel_all_warning_msgs_ind_present ? 1 : 0;
  nof_ies += notif_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)142, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, numof_broadcast_request, (uint32_t)0u, (uint32_t)65535u, false, true));
  }
  if (broadcast_to_be_cancelled_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)148, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, broadcast_to_be_cancelled_list, 1, 512, true));
  }
  if (cancel_all_warning_msgs_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)157, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cancel_all_warning_msgs_ind.pack(bref));
  }
  if (notif_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)220, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(notif_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pws_cancel_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pws_cancel_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 142: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(numof_broadcast_request, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
        break;
      }
      case 148: {
        broadcast_to_be_cancelled_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(broadcast_to_be_cancelled_list, bref, 1, 512, true));
        break;
      }
      case 157: {
        cancel_all_warning_msgs_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cancel_all_warning_msgs_ind.unpack(bref));
        break;
      }
      case 220: {
        notif_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(notif_info.unpack(bref));
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
void pws_cancel_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 142);
  j.write_str("criticality", "reject");
  j.write_int("value", numof_broadcast_request);
  j.end_obj();
  if (broadcast_to_be_cancelled_list_present) {
    j.start_obj();
    j.write_int("id", 148);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : broadcast_to_be_cancelled_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (cancel_all_warning_msgs_ind_present) {
    j.start_obj();
    j.write_int("id", 157);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (notif_info_present) {
    j.start_obj();
    j.write_int("id", 220);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    notif_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PWSCancelResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pws_cancel_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 150, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool pws_cancel_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 150, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pws_cancel_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 150:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pws_cancel_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 150:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pws_cancel_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += cells_broadcast_cancelled_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (cells_broadcast_cancelled_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)150, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_broadcast_cancelled_list, 1, 512, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pws_cancel_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pws_cancel_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 150: {
        cells_broadcast_cancelled_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_broadcast_cancelled_list, bref, 1, 512, true));
        break;
      }
      case 7: {
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
void pws_cancel_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (cells_broadcast_cancelled_list_present) {
    j.start_obj();
    j.write_int("id", 150);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_broadcast_cancelled_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PWSFailureIndicationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pws_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 154};
  return map_enum_number(names, 2, idx, "id");
}
bool pws_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 154};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pws_fail_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 154:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pws_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 154:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pws_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += pws_failed_nr_cgi_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (pws_failed_nr_cgi_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)154, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pws_failed_nr_cgi_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pws_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pws_fail_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 154: {
        pws_failed_nr_cgi_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pws_failed_nr_cgi_list, bref, 1, 512, true));
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
void pws_fail_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (pws_failed_nr_cgi_list_present) {
    j.start_obj();
    j.write_int("id", 154);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pws_failed_nr_cgi_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PWSRestartIndicationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pws_restart_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 152};
  return map_enum_number(names, 2, idx, "id");
}
bool pws_restart_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 152};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pws_restart_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 152:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pws_restart_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 152:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pws_restart_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)152, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, nr_cgi_list_for_restart_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pws_restart_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pws_restart_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 152: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(nr_cgi_list_for_restart_list, bref, 1, 512, true));
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
void pws_restart_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 152);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : nr_cgi_list_for_restart_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// PagingIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t paging_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {117, 127, 114, 115, 113, 216, 580, 581, 582, 583, 620, 622, 623, 694, 698, 713, 785};
  return map_enum_number(names, 17, idx, "id");
}
bool paging_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {117, 127, 114, 115, 113, 216, 580, 581, 582, 583, 620, 622, 623, 694, 698, 713, 785};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e paging_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 117:
      return crit_e::reject;
    case 127:
      return crit_e::reject;
    case 114:
      return crit_e::ignore;
    case 115:
      return crit_e::ignore;
    case 113:
      return crit_e::ignore;
    case 216:
      return crit_e::ignore;
    case 580:
      return crit_e::ignore;
    case 581:
      return crit_e::ignore;
    case 582:
      return crit_e::ignore;
    case 583:
      return crit_e::ignore;
    case 620:
      return crit_e::ignore;
    case 622:
      return crit_e::ignore;
    case 623:
      return crit_e::ignore;
    case 694:
      return crit_e::ignore;
    case 698:
      return crit_e::ignore;
    case 713:
      return crit_e::ignore;
    case 785:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e paging_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 117:
      return presence_e::mandatory;
    case 127:
      return presence_e::mandatory;
    case 114:
      return presence_e::optional;
    case 115:
      return presence_e::optional;
    case 113:
      return presence_e::mandatory;
    case 216:
      return presence_e::optional;
    case 580:
      return presence_e::optional;
    case 581:
      return presence_e::optional;
    case 582:
      return presence_e::optional;
    case 583:
      return presence_e::optional;
    case 620:
      return presence_e::optional;
    case 622:
      return presence_e::optional;
    case 623:
      return presence_e::optional;
    case 694:
      return presence_e::optional;
    case 698:
      return presence_e::optional;
    case 713:
      return presence_e::optional;
    case 785:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE paging_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += paging_drx_present ? 1 : 0;
  nof_ies += paging_prio_present ? 1 : 0;
  nof_ies += paging_origin_present ? 1 : 0;
  nof_ies += ran_ue_paging_drx_present ? 1 : 0;
  nof_ies += cn_ue_paging_drx_present ? 1 : 0;
  nof_ies += nr_paginge_drx_info_present ? 1 : 0;
  nof_ies += nr_paginge_drx_infofor_rrc_inactive_present ? 1 : 0;
  nof_ies += paging_cause_present ? 1 : 0;
  nof_ies += pe_ip_s_assist_info_present ? 1 : 0;
  nof_ies += ue_paging_cap_present ? 1 : 0;
  nof_ies += extended_ue_id_idx_value_present ? 1 : 0;
  nof_ies += hashed_ue_id_idx_value_present ? 1 : 0;
  nof_ies += mt_sdt_info_present ? 1 : 0;
  nof_ies += nr_paginglonge_drx_infofor_rrc_inactive_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)117, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ue_id_idx_value.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)127, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(paging_id.pack(bref));
  }
  if (paging_drx_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)114, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(paging_drx.pack(bref));
  }
  if (paging_prio_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)115, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(paging_prio.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)113, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, paging_cell_list, 1, 512, true));
  }
  if (paging_origin_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)216, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(paging_origin.pack(bref));
  }
  if (ran_ue_paging_drx_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)580, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ran_ue_paging_drx.pack(bref));
  }
  if (cn_ue_paging_drx_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)581, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cn_ue_paging_drx.pack(bref));
  }
  if (nr_paginge_drx_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)582, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nr_paginge_drx_info.pack(bref));
  }
  if (nr_paginge_drx_infofor_rrc_inactive_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)583, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nr_paginge_drx_infofor_rrc_inactive.pack(bref));
  }
  if (paging_cause_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)620, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(paging_cause.pack(bref));
  }
  if (pe_ip_s_assist_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)622, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pe_ip_s_assist_info.pack(bref));
  }
  if (ue_paging_cap_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)623, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ue_paging_cap.pack(bref));
  }
  if (extended_ue_id_idx_value_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)694, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(extended_ue_id_idx_value.pack(bref));
  }
  if (hashed_ue_id_idx_value_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)698, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(hashed_ue_id_idx_value.pack(bref));
  }
  if (mt_sdt_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)713, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(mt_sdt_info.pack(bref));
  }
  if (nr_paginglonge_drx_infofor_rrc_inactive_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)785, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nr_paginglonge_drx_infofor_rrc_inactive.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE paging_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != paging_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 117: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ue_id_idx_value.unpack(bref));
        break;
      }
      case 127: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(paging_id.unpack(bref));
        break;
      }
      case 114: {
        paging_drx_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(paging_drx.unpack(bref));
        break;
      }
      case 115: {
        paging_prio_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(paging_prio.unpack(bref));
        break;
      }
      case 113: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(paging_cell_list, bref, 1, 512, true));
        break;
      }
      case 216: {
        paging_origin_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(paging_origin.unpack(bref));
        break;
      }
      case 580: {
        ran_ue_paging_drx_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ran_ue_paging_drx.unpack(bref));
        break;
      }
      case 581: {
        cn_ue_paging_drx_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cn_ue_paging_drx.unpack(bref));
        break;
      }
      case 582: {
        nr_paginge_drx_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nr_paginge_drx_info.unpack(bref));
        break;
      }
      case 583: {
        nr_paginge_drx_infofor_rrc_inactive_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nr_paginge_drx_infofor_rrc_inactive.unpack(bref));
        break;
      }
      case 620: {
        paging_cause_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(paging_cause.unpack(bref));
        break;
      }
      case 622: {
        pe_ip_s_assist_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pe_ip_s_assist_info.unpack(bref));
        break;
      }
      case 623: {
        ue_paging_cap_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ue_paging_cap.unpack(bref));
        break;
      }
      case 694: {
        extended_ue_id_idx_value_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(extended_ue_id_idx_value.unpack(bref));
        break;
      }
      case 698: {
        hashed_ue_id_idx_value_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(hashed_ue_id_idx_value.unpack(bref));
        break;
      }
      case 713: {
        mt_sdt_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(mt_sdt_info.unpack(bref));
        break;
      }
      case 785: {
        nr_paginglonge_drx_infofor_rrc_inactive_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nr_paginglonge_drx_infofor_rrc_inactive.unpack(bref));
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
void paging_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_id_idx_value.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 127);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  paging_id.to_json(j);
  j.end_obj();
  if (paging_drx_present) {
    j.start_obj();
    j.write_int("id", 114);
    j.write_str("criticality", "ignore");
    j.write_str("value", paging_drx.to_string());
    j.end_obj();
  }
  if (paging_prio_present) {
    j.start_obj();
    j.write_int("id", 115);
    j.write_str("criticality", "ignore");
    j.write_str("value", paging_prio.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 113);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : paging_cell_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (paging_origin_present) {
    j.start_obj();
    j.write_int("id", 216);
    j.write_str("criticality", "ignore");
    j.write_str("value", "non-3gpp");
    j.end_obj();
  }
  if (ran_ue_paging_drx_present) {
    j.start_obj();
    j.write_int("id", 580);
    j.write_str("criticality", "ignore");
    j.write_str("value", ran_ue_paging_drx.to_string());
    j.end_obj();
  }
  if (cn_ue_paging_drx_present) {
    j.start_obj();
    j.write_int("id", 581);
    j.write_str("criticality", "ignore");
    j.write_str("value", cn_ue_paging_drx.to_string());
    j.end_obj();
  }
  if (nr_paginge_drx_info_present) {
    j.start_obj();
    j.write_int("id", 582);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_paginge_drx_info.to_json(j);
    j.end_obj();
  }
  if (nr_paginge_drx_infofor_rrc_inactive_present) {
    j.start_obj();
    j.write_int("id", 583);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_paginge_drx_infofor_rrc_inactive.to_json(j);
    j.end_obj();
  }
  if (paging_cause_present) {
    j.start_obj();
    j.write_int("id", 620);
    j.write_str("criticality", "ignore");
    j.write_str("value", "voice");
    j.end_obj();
  }
  if (pe_ip_s_assist_info_present) {
    j.start_obj();
    j.write_int("id", 622);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pe_ip_s_assist_info.to_json(j);
    j.end_obj();
  }
  if (ue_paging_cap_present) {
    j.start_obj();
    j.write_int("id", 623);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_paging_cap.to_json(j);
    j.end_obj();
  }
  if (extended_ue_id_idx_value_present) {
    j.start_obj();
    j.write_int("id", 694);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_ue_id_idx_value.to_json(j);
    j.end_obj();
  }
  if (hashed_ue_id_idx_value_present) {
    j.start_obj();
    j.write_int("id", 698);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    hashed_ue_id_idx_value.to_json(j);
    j.end_obj();
  }
  if (mt_sdt_info_present) {
    j.start_obj();
    j.write_int("id", 713);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mt_sdt_info.to_json(j);
    j.end_obj();
  }
  if (nr_paginglonge_drx_infofor_rrc_inactive_present) {
    j.start_obj();
    j.write_int("id", 785);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_paginglonge_drx_infofor_rrc_inactive.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PosSystemInformationDeliveryCommandIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t pos_sys_info_delivery_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 111, 682, 156};
  return map_enum_number(names, 4, idx, "id");
}
bool pos_sys_info_delivery_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 111, 682, 156};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pos_sys_info_delivery_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 111:
      return crit_e::reject;
    case 682:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pos_sys_info_delivery_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 111:
      return presence_e::mandatory;
    case 682:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pos_sys_info_delivery_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)111, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nr_cgi.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)682, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_sitype_list, 1, 32, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)156, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, confirmed_ue_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pos_sys_info_delivery_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != pos_sys_info_delivery_cmd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 111: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nr_cgi.unpack(bref));
        break;
      }
      case 682: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pos_sitype_list, bref, 1, 32, true));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(confirmed_ue_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
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
void pos_sys_info_delivery_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 111);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  nr_cgi.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 682);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pos_sitype_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_uint("value", confirmed_ue_id);
  j.end_obj();
  j.end_array();
}

// PositioningActivationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_activation_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 0, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool positioning_activation_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 0, 7};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_activation_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_activation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_activation_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// SemipersistentSRS-ExtIEs ::= OBJECT SET OF F1AP-PROTOCOL-EXTENSION
uint32_t semipersistent_srs_ext_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {435, 829};
  return map_enum_number(names, 2, idx, "id");
}
bool semipersistent_srs_ext_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {435, 829};
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
    case 435:
      return crit_e::ignore;
    case 829:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e semipersistent_srs_ext_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 435:
      return presence_e::optional;
    case 829:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE semipersistent_srs_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += srs_spatial_relation_per_srs_res_present ? 1 : 0;
  nof_ies += aggr_pos_srs_res_set_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (srs_spatial_relation_per_srs_res_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)435, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_spatial_relation_per_srs_res.pack(bref));
  }
  if (aggr_pos_srs_res_set_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)829, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, aggr_pos_srs_res_set_list, 1, 32, true));
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
      case 435: {
        srs_spatial_relation_per_srs_res_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_spatial_relation_per_srs_res.unpack(bref));
        break;
      }
      case 829: {
        aggr_pos_srs_res_set_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(aggr_pos_srs_res_set_list, bref, 1, 32, true));
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
  j.start_array();
  if (srs_spatial_relation_per_srs_res_present) {
    j.start_obj();
    j.write_int("id", 435);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    srs_spatial_relation_per_srs_res.to_json(j);
    j.end_obj();
  }
  if (aggr_pos_srs_res_set_list_present) {
    j.start_obj();
    j.write_int("id", 829);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : aggr_pos_srs_res_set_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// SemipersistentSRS ::= SEQUENCE
OCUDUASN_CODE semipersistent_srs_s::pack(bit_ref& bref) const
{
  bref.pack(false, 1);
  HANDLE_CODE(bref.pack(srs_spatial_relation_present, 1));
  HANDLE_CODE(bref.pack(ie_exts_present, 1));

  HANDLE_CODE(pack_integer(bref, srs_res_set_id, (uint8_t)0u, (uint8_t)15u, true, true));
  if (srs_spatial_relation_present) {
    HANDLE_CODE(srs_spatial_relation.pack(bref));
  }
  if (ie_exts_present) {
    HANDLE_CODE(ie_exts.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE semipersistent_srs_s::unpack(cbit_ref& bref)
{
  bool ext;
  HANDLE_CODE(bref.unpack(ext, 1));
  HANDLE_CODE(bref.unpack(srs_spatial_relation_present, 1));
  HANDLE_CODE(bref.unpack(ie_exts_present, 1));

  HANDLE_CODE(unpack_integer(srs_res_set_id, bref, (uint8_t)0u, (uint8_t)15u, true, true));
  if (srs_spatial_relation_present) {
    HANDLE_CODE(srs_spatial_relation.unpack(bref));
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
void semipersistent_srs_s::to_json(json_writer& j) const
{
  j.start_obj();
  j.write_int("sRSResourceSetID", srs_res_set_id);
  if (srs_spatial_relation_present) {
    j.write_fieldname("sRSSpatialRelation");
    srs_spatial_relation.to_json(j);
  }
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
      j.write_fieldname("choice-extension");
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
  static const char* names[] = {"semipersistentSRS", "aperiodicSRS", "choice-extension"};
  return convert_enum_idx(names, 3, value, "srs_type_c::types");
}

// PositioningActivationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_activation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 403, 404};
  return map_enum_number(names, 4, idx, "id");
}
bool positioning_activation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 403, 404};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 403:
      return crit_e::reject;
    case 404:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_activation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 403:
      return presence_e::mandatory;
    case 404:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_activation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += activation_time_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)403, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_type.pack(bref));
  }
  if (activation_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)404, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_activation_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 403: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_type.unpack(bref));
        break;
      }
      case 404: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 403);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  srs_type.to_json(j);
  j.end_obj();
  if (activation_time_present) {
    j.start_obj();
    j.write_int("id", 404);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    activation_time.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PositioningActivationResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_activation_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 420, 421, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool positioning_activation_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 420, 421, 7};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 420:
      return crit_e::ignore;
    case 421:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_activation_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 420:
      return presence_e::optional;
    case 421:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_activation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += sys_frame_num_present ? 1 : 0;
  nof_ies += slot_num_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (sys_frame_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)420, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true));
  }
  if (slot_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)421, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, slot_num, (uint8_t)0u, (uint8_t)79u, false, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_activation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_activation_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 420: {
        sys_frame_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true));
        break;
      }
      case 421: {
        slot_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(slot_num, bref, (uint8_t)0u, (uint8_t)79u, false, true));
        break;
      }
      case 7: {
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
void positioning_activation_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  if (sys_frame_num_present) {
    j.start_obj();
    j.write_int("id", 420);
    j.write_str("criticality", "ignore");
    j.write_int("value", sys_frame_num);
    j.end_obj();
  }
  if (slot_num_present) {
    j.start_obj();
    j.write_int("id", 421);
    j.write_str("criticality", "ignore");
    j.write_int("value", slot_num);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PositioningAssistanceInformationControlIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_assist_info_ctrl_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 392, 393, 406, 394};
  return map_enum_number(names, 5, idx, "id");
}
bool positioning_assist_info_ctrl_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 392, 393, 406, 394};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_assist_info_ctrl_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 392:
      return crit_e::reject;
    case 393:
      return crit_e::reject;
    case 406:
      return crit_e::reject;
    case 394:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_assist_info_ctrl_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 392:
      return presence_e::optional;
    case 393:
      return presence_e::optional;
    case 406:
      return presence_e::optional;
    case 394:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_assist_info_ctrl_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += pos_assist_info_present ? 1 : 0;
  nof_ies += pos_broadcast_present ? 1 : 0;
  nof_ies += positioning_broadcast_cells_present ? 1 : 0;
  nof_ies += routing_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (pos_assist_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)392, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_assist_info.pack(bref));
  }
  if (pos_broadcast_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)393, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_broadcast.pack(bref));
  }
  if (positioning_broadcast_cells_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)406, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, positioning_broadcast_cells, 1, 16384, true));
  }
  if (routing_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)394, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(routing_id.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_assist_info_ctrl_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_assist_info_ctrl_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 392: {
        pos_assist_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_assist_info.unpack(bref));
        break;
      }
      case 393: {
        pos_broadcast_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_broadcast.unpack(bref));
        break;
      }
      case 406: {
        positioning_broadcast_cells_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(positioning_broadcast_cells, bref, 1, 16384, true));
        break;
      }
      case 394: {
        routing_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(routing_id.unpack(bref));
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
void positioning_assist_info_ctrl_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (pos_assist_info_present) {
    j.start_obj();
    j.write_int("id", 392);
    j.write_str("criticality", "reject");
    j.write_str("value", pos_assist_info.to_string());
    j.end_obj();
  }
  if (pos_broadcast_present) {
    j.start_obj();
    j.write_int("id", 393);
    j.write_str("criticality", "reject");
    j.write_str("value", pos_broadcast.to_string());
    j.end_obj();
  }
  if (positioning_broadcast_cells_present) {
    j.start_obj();
    j.write_int("id", 406);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : positioning_broadcast_cells) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (routing_id_present) {
    j.start_obj();
    j.write_int("id", 394);
    j.write_str("criticality", "reject");
    j.write_str("value", routing_id.to_string());
    j.end_obj();
  }
  j.end_array();
}

// PositioningAssistanceInformationFeedbackIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_assist_info_feedback_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 395, 406, 394, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool positioning_assist_info_feedback_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 395, 406, 394, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_assist_info_feedback_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 395:
      return crit_e::reject;
    case 406:
      return crit_e::reject;
    case 394:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_assist_info_feedback_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 395:
      return presence_e::optional;
    case 406:
      return presence_e::optional;
    case 394:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_assist_info_feedback_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += pos_assist_info_fail_list_present ? 1 : 0;
  nof_ies += positioning_broadcast_cells_present ? 1 : 0;
  nof_ies += routing_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (pos_assist_info_fail_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)395, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_assist_info_fail_list.pack(bref));
  }
  if (positioning_broadcast_cells_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)406, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, positioning_broadcast_cells, 1, 16384, true));
  }
  if (routing_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)394, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(routing_id.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_assist_info_feedback_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_assist_info_feedback_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 395: {
        pos_assist_info_fail_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_assist_info_fail_list.unpack(bref));
        break;
      }
      case 406: {
        positioning_broadcast_cells_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(positioning_broadcast_cells, bref, 1, 16384, true));
        break;
      }
      case 394: {
        routing_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(routing_id.unpack(bref));
        break;
      }
      case 7: {
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
void positioning_assist_info_feedback_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (pos_assist_info_fail_list_present) {
    j.start_obj();
    j.write_int("id", 395);
    j.write_str("criticality", "reject");
    j.write_str("value", pos_assist_info_fail_list.to_string());
    j.end_obj();
  }
  if (positioning_broadcast_cells_present) {
    j.start_obj();
    j.write_int("id", 406);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : positioning_broadcast_cells) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (routing_id_present) {
    j.start_obj();
    j.write_int("id", 394);
    j.write_str("criticality", "reject");
    j.write_str("value", routing_id.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PositioningDeactivationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_deactivation_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 405};
  return map_enum_number(names, 3, idx, "id");
}
bool positioning_deactivation_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 405};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_deactivation_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 405:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_deactivation_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 405:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_deactivation_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)405, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(abort_tx.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_deactivation_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_deactivation_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 405: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(abort_tx.unpack(bref));
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
void positioning_deactivation_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 405);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  abort_tx.to_json(j);
  j.end_obj();
  j.end_array();
}

// PositioningInformationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_info_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 0, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool positioning_info_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 0, 7};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_info_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_info_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_info_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PositioningInformationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_info_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 391, 575, 689, 802, 830};
  return map_enum_number(names, 7, idx, "id");
}
bool positioning_info_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 391, 575, 689, 802, 830};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 391:
      return crit_e::ignore;
    case 575:
      return crit_e::ignore;
    case 689:
      return crit_e::ignore;
    case 802:
      return crit_e::ignore;
    case 830:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_info_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 391:
      return presence_e::optional;
    case 575:
      return presence_e::optional;
    case 689:
      return presence_e::optional;
    case 802:
      return presence_e::optional;
    case 830:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_info_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += requested_srs_tx_characteristics_present ? 1 : 0;
  nof_ies += ue_report_info_present ? 1 : 0;
  nof_ies += srs_pos_rrc_inactive_query_ind_present ? 1 : 0;
  nof_ies += time_win_info_srs_list_present ? 1 : 0;
  nof_ies += requested_srs_precfg_characteristics_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (requested_srs_tx_characteristics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)391, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(requested_srs_tx_characteristics.pack(bref));
  }
  if (ue_report_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)575, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ue_report_info.pack(bref));
  }
  if (srs_pos_rrc_inactive_query_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)689, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_pos_rrc_inactive_query_ind.pack(bref));
  }
  if (time_win_info_srs_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)802, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, time_win_info_srs_list, 1, 16, true));
  }
  if (requested_srs_precfg_characteristics_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)830, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, requested_srs_precfg_characteristics_list, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_info_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 391: {
        requested_srs_tx_characteristics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(requested_srs_tx_characteristics.unpack(bref));
        break;
      }
      case 575: {
        ue_report_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ue_report_info.unpack(bref));
        break;
      }
      case 689: {
        srs_pos_rrc_inactive_query_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_pos_rrc_inactive_query_ind.unpack(bref));
        break;
      }
      case 802: {
        time_win_info_srs_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(time_win_info_srs_list, bref, 1, 16, true));
        break;
      }
      case 830: {
        requested_srs_precfg_characteristics_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(requested_srs_precfg_characteristics_list, bref, 1, 16, true));
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
void positioning_info_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  if (requested_srs_tx_characteristics_present) {
    j.start_obj();
    j.write_int("id", 391);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    requested_srs_tx_characteristics.to_json(j);
    j.end_obj();
  }
  if (ue_report_info_present) {
    j.start_obj();
    j.write_int("id", 575);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_report_info.to_json(j);
    j.end_obj();
  }
  if (srs_pos_rrc_inactive_query_ind_present) {
    j.start_obj();
    j.write_int("id", 689);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (time_win_info_srs_list_present) {
    j.start_obj();
    j.write_int("id", 802);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : time_win_info_srs_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (requested_srs_precfg_characteristics_list_present) {
    j.start_obj();
    j.write_int("id", 830);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : requested_srs_precfg_characteristics_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PositioningInformationResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_info_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 407, 419, 7, 674, 811, 831};
  return map_enum_number(names, 8, idx, "id");
}
bool positioning_info_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 407, 419, 7, 674, 811, 831};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 407:
      return crit_e::ignore;
    case 419:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 674:
      return crit_e::ignore;
    case 811:
      return crit_e::ignore;
    case 831:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_info_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 407:
      return presence_e::optional;
    case 419:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 674:
      return presence_e::optional;
    case 811:
      return presence_e::optional;
    case 831:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_info_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += sfn_initisation_time_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += srs_pos_rrc_inactive_cfg_present ? 1 : 0;
  nof_ies += srs_pos_rrc_inactive_validity_area_cfg_present ? 1 : 0;
  nof_ies += srs_precfg_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (srs_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)407, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_cfg.pack(bref));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)419, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sfn_initisation_time.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (srs_pos_rrc_inactive_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)674, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_pos_rrc_inactive_cfg.pack(bref));
  }
  if (srs_pos_rrc_inactive_validity_area_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)811, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_pos_rrc_inactive_validity_area_cfg.pack(bref));
  }
  if (srs_precfg_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)831, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, srs_precfg_list, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_info_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 407: {
        srs_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_cfg.unpack(bref));
        break;
      }
      case 419: {
        sfn_initisation_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sfn_initisation_time.unpack(bref));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 674: {
        srs_pos_rrc_inactive_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_pos_rrc_inactive_cfg.unpack(bref));
        break;
      }
      case 811: {
        srs_pos_rrc_inactive_validity_area_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_pos_rrc_inactive_validity_area_cfg.unpack(bref));
        break;
      }
      case 831: {
        srs_precfg_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(srs_precfg_list, bref, 1, 16, true));
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
void positioning_info_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 407);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_cfg.to_json(j);
    j.end_obj();
  }
  if (sfn_initisation_time_present) {
    j.start_obj();
    j.write_int("id", 419);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sfn_initisation_time.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (srs_pos_rrc_inactive_cfg_present) {
    j.start_obj();
    j.write_int("id", 674);
    j.write_str("criticality", "ignore");
    j.write_str("value", srs_pos_rrc_inactive_cfg.to_string());
    j.end_obj();
  }
  if (srs_pos_rrc_inactive_validity_area_cfg_present) {
    j.start_obj();
    j.write_int("id", 811);
    j.write_str("criticality", "ignore");
    j.write_str("value", srs_pos_rrc_inactive_validity_area_cfg.to_string());
    j.end_obj();
  }
  if (srs_precfg_list_present) {
    j.start_obj();
    j.write_int("id", 831);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : srs_precfg_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PositioningInformationUpdateIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_info_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 407, 419};
  return map_enum_number(names, 4, idx, "id");
}
bool positioning_info_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 407, 419};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 407:
      return crit_e::ignore;
    case 419:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_info_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 407:
      return presence_e::optional;
    case 419:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_info_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += sfn_initisation_time_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (srs_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)407, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_cfg.pack(bref));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)419, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sfn_initisation_time.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_info_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 407: {
        srs_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_cfg.unpack(bref));
        break;
      }
      case 419: {
        sfn_initisation_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sfn_initisation_time.unpack(bref));
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
void positioning_info_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 407);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_cfg.to_json(j);
    j.end_obj();
  }
  if (sfn_initisation_time_present) {
    j.start_obj();
    j.write_int("id", 419);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sfn_initisation_time.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PositioningMeasurementAbortIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_meas_abort_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 402, 411};
  return map_enum_number(names, 3, idx, "id");
}
bool positioning_meas_abort_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 402, 411};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_meas_abort_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 402:
      return crit_e::reject;
    case 411:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_meas_abort_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 402:
      return presence_e::mandatory;
    case 411:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_meas_abort_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)402, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)411, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_meas_abort_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_meas_abort_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 402: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 411: {
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
void positioning_meas_abort_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 402);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 411);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  j.end_array();
}

// PositioningMeasurementFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_meas_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 402, 411, 0, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool positioning_meas_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 402, 411, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_meas_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 402:
      return crit_e::reject;
    case 411:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_meas_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 402:
      return presence_e::mandatory;
    case 411:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_meas_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)402, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)411, (uint32_t)0u, (uint32_t)65535u, false, true));
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
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_meas_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_meas_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 402: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 411: {
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
      case 7: {
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
void positioning_meas_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 402);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 411);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PositioningMeasurementFailureIndicationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_meas_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 402, 411, 0};
  return map_enum_number(names, 4, idx, "id");
}
bool positioning_meas_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 402, 411, 0};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_meas_fail_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 402:
      return crit_e::reject;
    case 411:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_meas_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 402:
      return presence_e::mandatory;
    case 411:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_meas_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)402, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)411, (uint32_t)0u, (uint32_t)65535u, false, true));
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
OCUDUASN_CODE positioning_meas_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_meas_fail_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 402: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 411: {
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
void positioning_meas_fail_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 402);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 411);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// PositioningMeasurementReportIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_meas_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 402, 411, 397};
  return map_enum_number(names, 4, idx, "id");
}
bool positioning_meas_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 402, 411, 397};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_meas_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 402:
      return crit_e::reject;
    case 411:
      return crit_e::reject;
    case 397:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_meas_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 402:
      return presence_e::mandatory;
    case 411:
      return presence_e::mandatory;
    case 397:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_meas_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)402, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)411, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)397, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_meas_result_list, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_meas_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_meas_report_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 402: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 411: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 397: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pos_meas_result_list, bref, 1, 64, true));
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
void positioning_meas_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 402);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 411);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 397);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pos_meas_result_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// PositioningMeasurementRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_meas_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {
      78, 402, 411, 422, 408, 409, 396, 419, 407, 423, 420, 421, 438, 555, 574, 573, 634, 803};
  return map_enum_number(names, 18, idx, "id");
}
bool positioning_meas_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {
      78, 402, 411, 422, 408, 409, 396, 419, 407, 423, 420, 421, 438, 555, 574, 573, 634, 803};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_meas_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 402:
      return crit_e::reject;
    case 411:
      return crit_e::reject;
    case 422:
      return crit_e::reject;
    case 408:
      return crit_e::reject;
    case 409:
      return crit_e::reject;
    case 396:
      return crit_e::reject;
    case 419:
      return crit_e::ignore;
    case 407:
      return crit_e::ignore;
    case 423:
      return crit_e::ignore;
    case 420:
      return crit_e::ignore;
    case 421:
      return crit_e::ignore;
    case 438:
      return crit_e::reject;
    case 555:
      return crit_e::ignore;
    case 574:
      return crit_e::ignore;
    case 573:
      return crit_e::ignore;
    case 634:
      return crit_e::ignore;
    case 803:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_meas_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 402:
      return presence_e::mandatory;
    case 411:
      return presence_e::mandatory;
    case 422:
      return presence_e::mandatory;
    case 408:
      return presence_e::mandatory;
    case 409:
      return presence_e::conditional;
    case 396:
      return presence_e::mandatory;
    case 419:
      return presence_e::optional;
    case 407:
      return presence_e::optional;
    case 423:
      return presence_e::optional;
    case 420:
      return presence_e::optional;
    case 421:
      return presence_e::optional;
    case 438:
      return presence_e::conditional;
    case 555:
      return presence_e::optional;
    case 574:
      return presence_e::optional;
    case 573:
      return presence_e::optional;
    case 634:
      return presence_e::optional;
    case 803:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_meas_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 6;
  nof_ies += pos_meas_periodicity_present ? 1 : 0;
  nof_ies += sfn_initisation_time_present ? 1 : 0;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += meas_beam_info_request_present ? 1 : 0;
  nof_ies += sys_frame_num_present ? 1 : 0;
  nof_ies += slot_num_present ? 1 : 0;
  nof_ies += pos_meas_periodicity_extended_present ? 1 : 0;
  nof_ies += resp_time_present ? 1 : 0;
  nof_ies += meas_characteristics_request_ind_present ? 1 : 0;
  nof_ies += meas_time_occasion_present ? 1 : 0;
  nof_ies += pos_meas_amount_present ? 1 : 0;
  nof_ies += time_win_info_meas_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)402, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)411, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)422, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_meas_request_list, 1, 64, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)408, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_report_characteristics.pack(bref));
  }
  if (pos_meas_periodicity_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)409, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_meas_periodicity.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)396, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_meas_quantities, 1, 16384, true));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)419, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(sfn_initisation_time.pack(bref));
  }
  if (srs_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)407, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_cfg.pack(bref));
  }
  if (meas_beam_info_request_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)423, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_beam_info_request.pack(bref));
  }
  if (sys_frame_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)420, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true));
  }
  if (slot_num_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)421, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, slot_num, (uint8_t)0u, (uint8_t)79u, false, true));
  }
  if (pos_meas_periodicity_extended_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)438, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_meas_periodicity_extended.pack(bref));
  }
  if (resp_time_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)555, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(resp_time.pack(bref));
  }
  if (meas_characteristics_request_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)574, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_characteristics_request_ind.pack(bref));
  }
  if (meas_time_occasion_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)573, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_time_occasion.pack(bref));
  }
  if (pos_meas_amount_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)634, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pos_meas_amount.pack(bref));
  }
  if (time_win_info_meas_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)803, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, time_win_info_meas_list, 1, 16, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_meas_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 6;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_meas_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 402: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 411: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 422: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_meas_request_list, bref, 1, 64, true));
        break;
      }
      case 408: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_report_characteristics.unpack(bref));
        break;
      }
      case 409: {
        pos_meas_periodicity_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_meas_periodicity.unpack(bref));
        break;
      }
      case 396: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pos_meas_quantities, bref, 1, 16384, true));
        break;
      }
      case 419: {
        sfn_initisation_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(sfn_initisation_time.unpack(bref));
        break;
      }
      case 407: {
        srs_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_cfg.unpack(bref));
        break;
      }
      case 423: {
        meas_beam_info_request_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_beam_info_request.unpack(bref));
        break;
      }
      case 420: {
        sys_frame_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true));
        break;
      }
      case 421: {
        slot_num_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(slot_num, bref, (uint8_t)0u, (uint8_t)79u, false, true));
        break;
      }
      case 438: {
        pos_meas_periodicity_extended_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_meas_periodicity_extended.unpack(bref));
        break;
      }
      case 555: {
        resp_time_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(resp_time.unpack(bref));
        break;
      }
      case 574: {
        meas_characteristics_request_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_characteristics_request_ind.unpack(bref));
        break;
      }
      case 573: {
        meas_time_occasion_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_time_occasion.unpack(bref));
        break;
      }
      case 634: {
        pos_meas_amount_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pos_meas_amount.unpack(bref));
        break;
      }
      case 803: {
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
void positioning_meas_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 402);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 411);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 422);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : trp_meas_request_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 408);
  j.write_str("criticality", "reject");
  j.write_str("value", pos_report_characteristics.to_string());
  j.end_obj();
  if (pos_meas_periodicity_present) {
    j.start_obj();
    j.write_int("id", 409);
    j.write_str("criticality", "reject");
    j.write_str("value", pos_meas_periodicity.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 396);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pos_meas_quantities) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (sfn_initisation_time_present) {
    j.start_obj();
    j.write_int("id", 419);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sfn_initisation_time.to_json(j);
    j.end_obj();
  }
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 407);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_cfg.to_json(j);
    j.end_obj();
  }
  if (meas_beam_info_request_present) {
    j.start_obj();
    j.write_int("id", 423);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (sys_frame_num_present) {
    j.start_obj();
    j.write_int("id", 420);
    j.write_str("criticality", "ignore");
    j.write_int("value", sys_frame_num);
    j.end_obj();
  }
  if (slot_num_present) {
    j.start_obj();
    j.write_int("id", 421);
    j.write_str("criticality", "ignore");
    j.write_int("value", slot_num);
    j.end_obj();
  }
  if (pos_meas_periodicity_extended_present) {
    j.start_obj();
    j.write_int("id", 438);
    j.write_str("criticality", "reject");
    j.write_str("value", pos_meas_periodicity_extended.to_string());
    j.end_obj();
  }
  if (resp_time_present) {
    j.start_obj();
    j.write_int("id", 555);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    resp_time.to_json(j);
    j.end_obj();
  }
  if (meas_characteristics_request_ind_present) {
    j.start_obj();
    j.write_int("id", 574);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    meas_characteristics_request_ind.to_json(j);
    j.end_obj();
  }
  if (meas_time_occasion_present) {
    j.start_obj();
    j.write_int("id", 573);
    j.write_str("criticality", "ignore");
    j.write_str("value", meas_time_occasion.to_string());
    j.end_obj();
  }
  if (pos_meas_amount_present) {
    j.start_obj();
    j.write_int("id", 634);
    j.write_str("criticality", "ignore");
    j.write_str("value", pos_meas_amount.to_string());
    j.end_obj();
  }
  if (time_win_info_meas_list_present) {
    j.start_obj();
    j.write_int("id", 803);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : time_win_info_meas_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PositioningMeasurementResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_meas_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 402, 411, 397, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool positioning_meas_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 402, 411, 397, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_meas_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 402:
      return crit_e::reject;
    case 411:
      return crit_e::reject;
    case 397:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_meas_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 402:
      return presence_e::mandatory;
    case 411:
      return presence_e::mandatory;
    case 397:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_meas_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pos_meas_result_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)402, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)411, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  if (pos_meas_result_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)397, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, pos_meas_result_list, 1, 64, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_meas_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_meas_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 402: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 411: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 397: {
        pos_meas_result_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(pos_meas_result_list, bref, 1, 64, true));
        break;
      }
      case 7: {
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
void positioning_meas_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 402);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 411);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  if (pos_meas_result_list_present) {
    j.start_obj();
    j.write_int("id", 397);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pos_meas_result_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PositioningMeasurementUpdateIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t positioning_meas_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 402, 411, 407, 553, 574, 573};
  return map_enum_number(names, 7, idx, "id");
}
bool positioning_meas_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 402, 411, 407, 553, 574, 573};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e positioning_meas_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 402:
      return crit_e::reject;
    case 411:
      return crit_e::reject;
    case 407:
      return crit_e::ignore;
    case 553:
      return crit_e::reject;
    case 574:
      return crit_e::ignore;
    case 573:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e positioning_meas_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 402:
      return presence_e::mandatory;
    case 411:
      return presence_e::mandatory;
    case 407:
      return presence_e::optional;
    case 553:
      return presence_e::optional;
    case 574:
      return presence_e::optional;
    case 573:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE positioning_meas_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += trp_meas_upd_list_present ? 1 : 0;
  nof_ies += meas_characteristics_request_ind_present ? 1 : 0;
  nof_ies += meas_time_occasion_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)402, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)411, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true));
  }
  if (srs_cfg_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)407, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_cfg.pack(bref));
  }
  if (trp_meas_upd_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)553, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_meas_upd_list, 1, 64, true));
  }
  if (meas_characteristics_request_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)574, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_characteristics_request_ind.pack(bref));
  }
  if (meas_time_occasion_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)573, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(meas_time_occasion.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_meas_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != positioning_meas_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 402: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 411: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true));
        break;
      }
      case 407: {
        srs_cfg_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_cfg.unpack(bref));
        break;
      }
      case 553: {
        trp_meas_upd_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_meas_upd_list, bref, 1, 64, true));
        break;
      }
      case 574: {
        meas_characteristics_request_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(meas_characteristics_request_ind.unpack(bref));
        break;
      }
      case 573: {
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
void positioning_meas_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 402);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 411);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 407);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_cfg.to_json(j);
    j.end_obj();
  }
  if (trp_meas_upd_list_present) {
    j.start_obj();
    j.write_int("id", 553);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : trp_meas_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (meas_characteristics_request_ind_present) {
    j.start_obj();
    j.write_int("id", 574);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    meas_characteristics_request_ind.to_json(j);
    j.end_obj();
  }
  if (meas_time_occasion_present) {
    j.start_obj();
    j.write_int("id", 573);
    j.write_str("criticality", "ignore");
    j.write_str("value", meas_time_occasion.to_string());
    j.end_obj();
  }
  j.end_array();
}

// Value ::= OPEN TYPE
void f1ap_private_ies_empty_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.end_obj();
}
OCUDUASN_CODE f1ap_private_ies_empty_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE f1ap_private_ies_empty_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}

const char* f1ap_private_ies_empty_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {};
  return convert_enum_idx(names, 0, value, "f1ap_private_ies_empty_o::value_c::types");
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

// QoEInformationTransfer-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t qo_e_info_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 585};
  return map_enum_number(names, 3, idx, "id");
}
bool qo_e_info_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 585};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e qo_e_info_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 585:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e qo_e_info_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 585:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE qo_e_info_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += qo_e_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  if (qo_e_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)585, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(qo_e_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qo_e_info_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != qo_e_info_transfer_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 585: {
        qo_e_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(qo_e_info.unpack(bref));
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
void qo_e_info_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  if (qo_e_info_present) {
    j.start_obj();
    j.write_int("id", 585);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    qo_e_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// QoEInformationTransferControl-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t qo_e_info_transfer_ctrl_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 733};
  return map_enum_number(names, 2, idx, "id");
}
bool qo_e_info_transfer_ctrl_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 733};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e qo_e_info_transfer_ctrl_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 733:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e qo_e_info_transfer_ctrl_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 733:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE qo_e_info_transfer_ctrl_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += deactivation_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (deactivation_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)733, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(deactivation_ind.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE qo_e_info_transfer_ctrl_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != qo_e_info_transfer_ctrl_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 733: {
        deactivation_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(deactivation_ind.unpack(bref));
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
void qo_e_info_transfer_ctrl_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (deactivation_ind_present) {
    j.start_obj();
    j.write_int("id", 733);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    deactivation_ind.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RRCDeliveryReportIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t rrc_delivery_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 185, 64};
  return map_enum_number(names, 4, idx, "id");
}
bool rrc_delivery_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 185, 64};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e rrc_delivery_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 185:
      return crit_e::ignore;
    case 64:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e rrc_delivery_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 185:
      return presence_e::mandatory;
    case 64:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE rrc_delivery_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)185, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(rrc_delivery_status.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)64, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, srb_id, (uint8_t)0u, (uint8_t)3u, true, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_delivery_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != rrc_delivery_report_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 185: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(rrc_delivery_status.unpack(bref));
        break;
      }
      case 64: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(srb_id, bref, (uint8_t)0u, (uint8_t)3u, true, true));
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
void rrc_delivery_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 185);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  rrc_delivery_status.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 64);
  j.write_str("criticality", "ignore");
  j.write_int("value", srb_id);
  j.end_obj();
  j.end_array();
}

// RachIndication-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t rach_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 734};
  return map_enum_number(names, 2, idx, "id");
}
bool rach_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 734};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e rach_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 734:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e rach_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 734:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE rach_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)734, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ra_report_ind_list, 1, 64, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rach_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != rach_ind_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 734: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ra_report_ind_list, bref, 1, 64, true));
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
void rach_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 734);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : ra_report_ind_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// ReferenceTimeInformationReportIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ref_time_info_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 366};
  return map_enum_number(names, 2, idx, "id");
}
bool ref_time_info_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 366};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ref_time_info_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::ignore;
    case 366:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ref_time_info_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 366:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ref_time_info_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)366, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(time_ref_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ref_time_info_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ref_time_info_report_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 366: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(time_ref_info.unpack(bref));
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
void ref_time_info_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "ignore");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 366);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  time_ref_info.to_json(j);
  j.end_obj();
  j.end_array();
}

// ReferenceTimeInformationReportingControlIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ref_time_info_report_ctrl_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 365};
  return map_enum_number(names, 2, idx, "id");
}
bool ref_time_info_report_ctrl_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 365};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ref_time_info_report_ctrl_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 365:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ref_time_info_report_ctrl_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 365:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ref_time_info_report_ctrl_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)365, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(report_request_type.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ref_time_info_report_ctrl_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != ref_time_info_report_ctrl_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 365: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(report_request_type.unpack(bref));
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
void ref_time_info_report_ctrl_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 365);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  report_request_type.to_json(j);
  j.end_obj();
  j.end_array();
}

// ResetAll ::= ENUMERATED
const char* reset_all_opts::to_string() const
{
  static const char* names[] = {"reset-all"};
  return convert_enum_idx(names, 1, value, "reset_all_e");
}

// UE-associatedLogicalF1-ConnectionItemRes ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_associated_lc_f1_conn_item_res_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {80};
  return map_enum_number(names, 1, idx, "id");
}
bool ue_associated_lc_f1_conn_item_res_o::is_id_valid(const uint32_t& id)
{
  return 80 == id;
}
crit_e ue_associated_lc_f1_conn_item_res_o::get_crit(const uint32_t& id)
{
  if (id == 80) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ue_associated_lc_f1_conn_item_res_o::value_c ue_associated_lc_f1_conn_item_res_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 80) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ue_associated_lc_f1_conn_item_res_o::get_presence(const uint32_t& id)
{
  if (id == 80) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ue_associated_lc_f1_conn_item_res_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ue_associated_lc_f1_conn_item_res_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_associated_lc_f1_conn_item_res_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ue_associated_lc_f1_conn_item_res_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"UE-associatedLogicalF1-ConnectionItem"};
  return convert_enum_idx(names, 1, value, "ue_associated_lc_f1_conn_item_res_o::value_c::types");
}
uint8_t ue_associated_lc_f1_conn_item_res_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ue_associated_lc_f1_conn_item_res_o::value_c::types");
}

// ResetType ::= CHOICE
void reset_type_c::destroy_()
{
  switch (type_) {
    case types::part_of_f1_interface:
      c.destroy<ue_associated_lc_f1_conn_list_res_l>();
      break;
    case types::choice_ext:
      c.destroy<protocol_ie_single_container_s<reset_type_ext_ies_o>>();
      break;
    default:
      break;
  }
}
void reset_type_c::set(types::options e)
{
  destroy_();
  type_ = e;
  switch (type_) {
    case types::f1_interface:
      break;
    case types::part_of_f1_interface:
      c.init<ue_associated_lc_f1_conn_list_res_l>();
      break;
    case types::choice_ext:
      c.init<protocol_ie_single_container_s<reset_type_ext_ies_o>>();
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "reset_type_c");
  }
}
reset_type_c::reset_type_c(const reset_type_c& other)
{
  type_ = other.type();
  switch (type_) {
    case types::f1_interface:
      c.init(other.c.get<reset_all_e>());
      break;
    case types::part_of_f1_interface:
      c.init(other.c.get<ue_associated_lc_f1_conn_list_res_l>());
      break;
    case types::choice_ext:
      c.init(other.c.get<protocol_ie_single_container_s<reset_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "reset_type_c");
  }
}
reset_type_c& reset_type_c::operator=(const reset_type_c& other)
{
  if (this == &other) {
    return *this;
  }
  set(other.type());
  switch (type_) {
    case types::f1_interface:
      c.set(other.c.get<reset_all_e>());
      break;
    case types::part_of_f1_interface:
      c.set(other.c.get<ue_associated_lc_f1_conn_list_res_l>());
      break;
    case types::choice_ext:
      c.set(other.c.get<protocol_ie_single_container_s<reset_type_ext_ies_o>>());
      break;
    case types::nulltype:
      break;
    default:
      log_invalid_choice_id(type_, "reset_type_c");
  }

  return *this;
}
reset_all_e& reset_type_c::set_f1_interface()
{
  set(types::f1_interface);
  return c.get<reset_all_e>();
}
ue_associated_lc_f1_conn_list_res_l& reset_type_c::set_part_of_f1_interface()
{
  set(types::part_of_f1_interface);
  return c.get<ue_associated_lc_f1_conn_list_res_l>();
}
protocol_ie_single_container_s<reset_type_ext_ies_o>& reset_type_c::set_choice_ext()
{
  set(types::choice_ext);
  return c.get<protocol_ie_single_container_s<reset_type_ext_ies_o>>();
}
void reset_type_c::to_json(json_writer& j) const
{
  j.start_obj();
  switch (type_) {
    case types::f1_interface:
      j.write_str("f1-Interface", "reset-all");
      break;
    case types::part_of_f1_interface:
      j.start_array("partOfF1-Interface");
      for (const auto& e1 : c.get<ue_associated_lc_f1_conn_list_res_l>()) {
        e1.to_json(j);
      }
      j.end_array();
      break;
    case types::choice_ext:
      j.write_fieldname("choice-extension");
      c.get<protocol_ie_single_container_s<reset_type_ext_ies_o>>().to_json(j);
      break;
    default:
      log_invalid_choice_id(type_, "reset_type_c");
  }
  j.end_obj();
}
OCUDUASN_CODE reset_type_c::pack(bit_ref& bref) const
{
  HANDLE_CODE(type_.pack(bref));
  switch (type_) {
    case types::f1_interface:
      HANDLE_CODE(c.get<reset_all_e>().pack(bref));
      break;
    case types::part_of_f1_interface:
      HANDLE_CODE(pack_dyn_seq_of(bref, c.get<ue_associated_lc_f1_conn_list_res_l>(), 1, 65536, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<reset_type_ext_ies_o>>().pack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "reset_type_c");
      return OCUDUASN_ERROR_ENCODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reset_type_c::unpack(cbit_ref& bref)
{
  types e;
  HANDLE_CODE(e.unpack(bref));
  set(e);
  switch (type_) {
    case types::f1_interface:
      HANDLE_CODE(c.get<reset_all_e>().unpack(bref));
      break;
    case types::part_of_f1_interface:
      HANDLE_CODE(unpack_dyn_seq_of(c.get<ue_associated_lc_f1_conn_list_res_l>(), bref, 1, 65536, true));
      break;
    case types::choice_ext:
      HANDLE_CODE(c.get<protocol_ie_single_container_s<reset_type_ext_ies_o>>().unpack(bref));
      break;
    default:
      log_invalid_choice_id(type_, "reset_type_c");
      return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}

const char* reset_type_c::types_opts::to_string() const
{
  static const char* names[] = {"f1-Interface", "partOfF1-Interface", "choice-extension"};
  return convert_enum_idx(names, 3, value, "reset_type_c::types");
}

// ResetIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t reset_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 48};
  return map_enum_number(names, 3, idx, "id");
}
bool reset_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 48};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e reset_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 48:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e reset_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 48:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE reset_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)48, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(reset_type.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reset_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != reset_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 48: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(reset_type.unpack(bref));
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
void reset_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 48);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  reset_type.to_json(j);
  j.end_obj();
  j.end_array();
}

// UE-associatedLogicalF1-ConnectionItemResAck ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t ue_associated_lc_f1_conn_item_res_ack_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {80};
  return map_enum_number(names, 1, idx, "id");
}
bool ue_associated_lc_f1_conn_item_res_ack_o::is_id_valid(const uint32_t& id)
{
  return 80 == id;
}
crit_e ue_associated_lc_f1_conn_item_res_ack_o::get_crit(const uint32_t& id)
{
  if (id == 80) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
ue_associated_lc_f1_conn_item_res_ack_o::value_c ue_associated_lc_f1_conn_item_res_ack_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 80) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e ue_associated_lc_f1_conn_item_res_ack_o::get_presence(const uint32_t& id)
{
  if (id == 80) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void ue_associated_lc_f1_conn_item_res_ack_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE ue_associated_lc_f1_conn_item_res_ack_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_associated_lc_f1_conn_item_res_ack_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* ue_associated_lc_f1_conn_item_res_ack_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"UE-associatedLogicalF1-ConnectionItem"};
  return convert_enum_idx(names, 1, value, "ue_associated_lc_f1_conn_item_res_ack_o::value_c::types");
}
uint8_t ue_associated_lc_f1_conn_item_res_ack_o::value_c::types_opts::to_number() const
{
  static const uint8_t numbers[] = {1};
  return map_enum_number(numbers, 1, value, "ue_associated_lc_f1_conn_item_res_ack_o::value_c::types");
}

// ResetAcknowledgeIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t reset_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 81, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool reset_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 81, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e reset_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 81:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e reset_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 81:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE reset_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += ue_associated_lc_f1_conn_list_res_ack_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (ue_associated_lc_f1_conn_list_res_ack_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)81, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ue_associated_lc_f1_conn_list_res_ack, 1, 65536, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reset_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != reset_ack_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 81: {
        ue_associated_lc_f1_conn_list_res_ack_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ue_associated_lc_f1_conn_list_res_ack, bref, 1, 65536, true));
        break;
      }
      case 7: {
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
void reset_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (ue_associated_lc_f1_conn_list_res_ack_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_associated_lc_f1_conn_list_res_ack) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t res_status_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 345, 346, 0, 7};
  return map_enum_number(names, 5, idx, "id");
}
bool res_status_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 345, 346, 0, 7};
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
    case 78:
      return crit_e::reject;
    case 345:
      return crit_e::reject;
    case 346:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 345:
      return presence_e::mandatory;
    case 346:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)345, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_meas_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)346, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_meas_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != res_status_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 345: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_meas_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 346: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_meas_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void res_status_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 345);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 346);
  j.write_str("criticality", "ignore");
  j.write_int("value", gnb_du_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t res_status_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 345, 346, 347, 348, 349, 352};
  return map_enum_number(names, 7, idx, "id");
}
bool res_status_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 345, 346, 347, 348, 349, 352};
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
    case 78:
      return crit_e::reject;
    case 345:
      return crit_e::reject;
    case 346:
      return crit_e::ignore;
    case 347:
      return crit_e::ignore;
    case 348:
      return crit_e::ignore;
    case 349:
      return crit_e::ignore;
    case 352:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 345:
      return presence_e::mandatory;
    case 346:
      return presence_e::conditional;
    case 347:
      return presence_e::mandatory;
    case 348:
      return presence_e::conditional;
    case 349:
      return presence_e::optional;
    case 352:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += gnb_du_meas_id_present ? 1 : 0;
  nof_ies += report_characteristics_present ? 1 : 0;
  nof_ies += cell_to_report_list_present ? 1 : 0;
  nof_ies += report_periodicity_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)345, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_meas_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (gnb_du_meas_id_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)346, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_meas_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)347, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(regist_request.pack(bref));
  }
  if (report_characteristics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)348, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(report_characteristics.pack(bref));
  }
  if (cell_to_report_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)349, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_to_report_list, 1, 512, true));
  }
  if (report_periodicity_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)352, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(report_periodicity.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != res_status_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 345: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_meas_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 346: {
        gnb_du_meas_id_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_meas_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 347: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(regist_request.unpack(bref));
        break;
      }
      case 348: {
        report_characteristics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(report_characteristics.unpack(bref));
        break;
      }
      case 349: {
        cell_to_report_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cell_to_report_list, bref, 1, 512, true));
        break;
      }
      case 352: {
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
void res_status_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 345);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_meas_id);
  j.end_obj();
  if (gnb_du_meas_id_present) {
    j.start_obj();
    j.write_int("id", 346);
    j.write_str("criticality", "ignore");
    j.write_int("value", gnb_du_meas_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 347);
  j.write_str("criticality", "ignore");
  j.write_str("value", regist_request.to_string());
  j.end_obj();
  if (report_characteristics_present) {
    j.start_obj();
    j.write_int("id", 348);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    report_characteristics.to_json(j);
    j.end_obj();
  }
  if (cell_to_report_list_present) {
    j.start_obj();
    j.write_int("id", 349);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cell_to_report_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (report_periodicity_present) {
    j.start_obj();
    j.write_int("id", 352);
    j.write_str("criticality", "ignore");
    j.write_str("value", report_periodicity.to_string());
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t res_status_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 345, 346, 7};
  return map_enum_number(names, 4, idx, "id");
}
bool res_status_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 345, 346, 7};
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
    case 78:
      return crit_e::reject;
    case 345:
      return crit_e::reject;
    case 346:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 345:
      return presence_e::mandatory;
    case 346:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)345, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_meas_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)346, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_meas_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != res_status_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 345: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_meas_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 346: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_meas_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 7: {
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
void res_status_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 345);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 346);
  j.write_str("criticality", "ignore");
  j.write_int("value", gnb_du_meas_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// ResourceStatusUpdateIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t res_status_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 345, 346, 351, 353, 350};
  return map_enum_number(names, 6, idx, "id");
}
bool res_status_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 345, 346, 351, 353, 350};
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
    case 78:
      return crit_e::reject;
    case 345:
      return crit_e::reject;
    case 346:
      return crit_e::ignore;
    case 351:
      return crit_e::ignore;
    case 353:
      return crit_e::ignore;
    case 350:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e res_status_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 345:
      return presence_e::mandatory;
    case 346:
      return presence_e::mandatory;
    case 351:
      return presence_e::optional;
    case 353:
      return presence_e::optional;
    case 350:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE res_status_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += hardware_load_ind_present ? 1 : 0;
  nof_ies += tnl_capacity_ind_present ? 1 : 0;
  nof_ies += cell_meas_result_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)345, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_meas_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)346, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_meas_id, (uint16_t)0u, (uint16_t)4095u, true, true));
  }
  if (hardware_load_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)351, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(hardware_load_ind.pack(bref));
  }
  if (tnl_capacity_ind_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)353, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(tnl_capacity_ind.pack(bref));
  }
  if (cell_meas_result_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)350, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cell_meas_result_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE res_status_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != res_status_upd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 345: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_meas_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 346: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_meas_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true));
        break;
      }
      case 351: {
        hardware_load_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(hardware_load_ind.unpack(bref));
        break;
      }
      case 353: {
        tnl_capacity_ind_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(tnl_capacity_ind.unpack(bref));
        break;
      }
      case 350: {
        cell_meas_result_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cell_meas_result_list, bref, 1, 512, true));
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
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 345);
  j.write_str("criticality", "reject");
  j.write_int("value", gnb_cu_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 346);
  j.write_str("criticality", "ignore");
  j.write_int("value", gnb_du_meas_id);
  j.end_obj();
  if (hardware_load_ind_present) {
    j.start_obj();
    j.write_int("id", 351);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    hardware_load_ind.to_json(j);
    j.end_obj();
  }
  if (tnl_capacity_ind_present) {
    j.start_obj();
    j.write_int("id", 353);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    tnl_capacity_ind.to_json(j);
    j.end_obj();
  }
  if (cell_meas_result_list_present) {
    j.start_obj();
    j.write_int("id", 350);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : cell_meas_result_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// SRSInformationReservationNotificationIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t srs_info_reserv_notif_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 813, 832, 857};
  return map_enum_number(names, 4, idx, "id");
}
bool srs_info_reserv_notif_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 813, 832, 857};
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
    case 78:
      return crit_e::reject;
    case 813:
      return crit_e::reject;
    case 832:
      return crit_e::ignore;
    case 857:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e srs_info_reserv_notif_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 813:
      return presence_e::mandatory;
    case 832:
      return presence_e::optional;
    case 857:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE srs_info_reserv_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += srs_info_present ? 1 : 0;
  nof_ies += precfg_srs_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)813, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_reserv_type.pack(bref));
  }
  if (srs_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)832, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(srs_info.pack(bref));
  }
  if (precfg_srs_info_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)857, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != srs_info_reserv_notif_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 813: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_reserv_type.unpack(bref));
        break;
      }
      case 832: {
        srs_info_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(srs_info.unpack(bref));
        break;
      }
      case 857: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 813);
  j.write_str("criticality", "reject");
  j.write_str("value", srs_reserv_type.to_string());
  j.end_obj();
  if (srs_info_present) {
    j.start_obj();
    j.write_int("id", 832);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_info.to_json(j);
    j.end_obj();
  }
  if (precfg_srs_info_present) {
    j.start_obj();
    j.write_int("id", 857);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : precfg_srs_info) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// SystemInformationDeliveryCommandIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t sys_info_delivery_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 111, 116, 156};
  return map_enum_number(names, 4, idx, "id");
}
bool sys_info_delivery_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 111, 116, 156};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e sys_info_delivery_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 111:
      return crit_e::reject;
    case 116:
      return crit_e::reject;
    case 156:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e sys_info_delivery_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 111:
      return presence_e::mandatory;
    case 116:
      return presence_e::mandatory;
    case 156:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE sys_info_delivery_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)111, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(nr_cgi.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)116, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, sitype_list, 1, 32, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)156, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, confirmed_ue_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE sys_info_delivery_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != sys_info_delivery_cmd_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 111: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(nr_cgi.unpack(bref));
        break;
      }
      case 116: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(sitype_list, bref, 1, 32, true));
        break;
      }
      case 156: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(confirmed_ue_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
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
void sys_info_delivery_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 111);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  nr_cgi.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 116);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : sitype_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 156);
  j.write_str("criticality", "reject");
  j.write_uint("value", confirmed_ue_id);
  j.end_obj();
  j.end_array();
}

// TRPInformationFailureIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t trp_info_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool trp_info_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 7};
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
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trp_info_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE trp_info_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_info_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TRPInformationItemTRPResp ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t trp_info_item_trp_resp_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {401};
  return map_enum_number(names, 1, idx, "id");
}
bool trp_info_item_trp_resp_o::is_id_valid(const uint32_t& id)
{
  return 401 == id;
}
crit_e trp_info_item_trp_resp_o::get_crit(const uint32_t& id)
{
  if (id == 401) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
trp_info_item_trp_resp_o::value_c trp_info_item_trp_resp_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 401) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_info_item_trp_resp_o::get_presence(const uint32_t& id)
{
  if (id == 401) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void trp_info_item_trp_resp_o::value_c::to_json(json_writer& j) const
{
  c.to_json(j);
}
OCUDUASN_CODE trp_info_item_trp_resp_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_item_trp_resp_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* trp_info_item_trp_resp_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TRPInformationItem"};
  return convert_enum_idx(names, 1, value, "trp_info_item_trp_resp_o::value_c::types");
}

// TRPInformationTypeItemTRPReq ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t trp_info_type_item_trp_req_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {399};
  return map_enum_number(names, 1, idx, "id");
}
bool trp_info_type_item_trp_req_o::is_id_valid(const uint32_t& id)
{
  return 399 == id;
}
crit_e trp_info_type_item_trp_req_o::get_crit(const uint32_t& id)
{
  if (id == 399) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
trp_info_type_item_trp_req_o::value_c trp_info_type_item_trp_req_o::get_value(const uint32_t& id)
{
  value_c ret{};
  if (id != 399) {
    asn1::log_error("The id={} is not recognized", id);
  }
  return ret;
}
presence_e trp_info_type_item_trp_req_o::get_presence(const uint32_t& id)
{
  if (id == 399) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

// Value ::= OPEN TYPE
void trp_info_type_item_trp_req_o::value_c::to_json(json_writer& j) const
{
  j.write_str(c.to_string());
}
OCUDUASN_CODE trp_info_type_item_trp_req_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.pack(bref));
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_type_item_trp_req_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  HANDLE_CODE(c.unpack(bref));
  return OCUDUASN_SUCCESS;
}

const char* trp_info_type_item_trp_req_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {"TRPInformationTypeItem"};
  return convert_enum_idx(names, 1, value, "trp_info_type_item_trp_req_o::value_c::types");
}

// TRPInformationRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t trp_info_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 410, 398};
  return map_enum_number(names, 3, idx, "id");
}
bool trp_info_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 410, 398};
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
    case 78:
      return crit_e::reject;
    case 410:
      return crit_e::ignore;
    case 398:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trp_info_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 410:
      return presence_e::optional;
    case 398:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE trp_info_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += trp_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (trp_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)410, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_list, 1, 65535, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)398, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_info_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 410: {
        trp_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_list, bref, 1, 65535, true));
        break;
      }
      case 398: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (trp_list_present) {
    j.start_obj();
    j.write_int("id", 410);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : trp_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 398);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : trp_info_type_list_trp_req) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// TRPInformationResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t trp_info_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 400, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool trp_info_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 400, 7};
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
    case 78:
      return crit_e::reject;
    case 400:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trp_info_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 400:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE trp_info_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)400, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, trp_info_list_trp_resp, 1, 65535, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
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

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trp_info_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 400: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(trp_info_list_trp_resp, bref, 1, 65535, true));
        break;
      }
      case 7: {
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 400);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : trp_info_list_trp_resp) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TimingSynchronisationStatusFailure-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t timing_synchronisation_status_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 0, 7};
  return map_enum_number(names, 3, idx, "id");
}
bool timing_synchronisation_status_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 0, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e timing_synchronisation_status_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 0:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e timing_synchronisation_status_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE timing_synchronisation_status_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)0, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(cause.pack(bref));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE timing_synchronisation_status_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != timing_synchronisation_status_fail_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(cause.unpack(bref));
        break;
      }
      case 7: {
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
void timing_synchronisation_status_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TimingSynchronisationStatusReport-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t timing_synchronisation_status_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 750};
  return map_enum_number(names, 2, idx, "id");
}
bool timing_synchronisation_status_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 750};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e timing_synchronisation_status_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 750:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e timing_synchronisation_status_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 750:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE timing_synchronisation_status_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)750, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(ran_timing_synchronisation_status_info.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE timing_synchronisation_status_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != timing_synchronisation_status_report_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 750: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(ran_timing_synchronisation_status_info.unpack(bref));
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
void timing_synchronisation_status_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 750);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  ran_timing_synchronisation_status_info.to_json(j);
  j.end_obj();
  j.end_array();
}

// TimingSynchronisationStatusRequest-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t timing_synchronisation_status_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 749};
  return map_enum_number(names, 2, idx, "id");
}
bool timing_synchronisation_status_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 749};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e timing_synchronisation_status_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 749:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e timing_synchronisation_status_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 749:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE timing_synchronisation_status_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)749, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(rantss_request_type.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE timing_synchronisation_status_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != timing_synchronisation_status_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 749: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(rantss_request_type.unpack(bref));
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
void timing_synchronisation_status_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 749);
  j.write_str("criticality", "reject");
  j.write_str("value", rantss_request_type.to_string());
  j.end_obj();
  j.end_array();
}

// TimingSynchronisationStatusResponse-IEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t timing_synchronisation_status_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 7};
  return map_enum_number(names, 2, idx, "id");
}
bool timing_synchronisation_status_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 7};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e timing_synchronisation_status_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e timing_synchronisation_status_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 7:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE timing_synchronisation_status_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE timing_synchronisation_status_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != timing_synchronisation_status_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 7: {
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
void timing_synchronisation_status_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TraceStartIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t trace_start_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {40, 41, 242};
  return map_enum_number(names, 3, idx, "id");
}
bool trace_start_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {40, 41, 242};
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
    case 40:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 242:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trace_start_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 40:
      return presence_e::mandatory;
    case 41:
      return presence_e::mandatory;
    case 242:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE trace_start_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)40, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_cu_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)41, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, gnb_du_ue_f1ap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)242, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(trace_activation.pack(bref));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trace_start_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != trace_start_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 40: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_cu_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(gnb_du_ue_f1ap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true));
        break;
      }
      case 242: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(trace_activation.unpack(bref));
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
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_cu_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.write_uint("value", gnb_du_ue_f1ap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 242);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  trace_activation.to_json(j);
  j.end_obj();
  j.end_array();
}

// WriteReplaceWarningRequestIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t write_replace_warning_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 140, 141, 142, 144};
  return map_enum_number(names, 5, idx, "id");
}
bool write_replace_warning_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 140, 141, 142, 144};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e write_replace_warning_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 140:
      return crit_e::reject;
    case 141:
      return crit_e::reject;
    case 142:
      return crit_e::reject;
    case 144:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e write_replace_warning_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 140:
      return presence_e::mandatory;
    case 141:
      return presence_e::mandatory;
    case 142:
      return presence_e::mandatory;
    case 144:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE write_replace_warning_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += cells_to_be_broadcast_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)140, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pws_sys_info.pack(bref));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)141, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, repeat_period, (uint32_t)0u, (uint32_t)131071u, true, true));
  }
  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)142, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, numof_broadcast_request, (uint32_t)0u, (uint32_t)65535u, false, true));
  }
  if (cells_to_be_broadcast_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)144, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_to_be_broadcast_list, 1, 512, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE write_replace_warning_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != write_replace_warning_request_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 140: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(pws_sys_info.unpack(bref));
        break;
      }
      case 141: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(repeat_period, bref, (uint32_t)0u, (uint32_t)131071u, true, true));
        break;
      }
      case 142: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(numof_broadcast_request, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
        break;
      }
      case 144: {
        cells_to_be_broadcast_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_to_be_broadcast_list, bref, 1, 512, true));
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
void write_replace_warning_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 140);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  pws_sys_info.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 141);
  j.write_str("criticality", "reject");
  j.write_int("value", repeat_period);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 142);
  j.write_str("criticality", "reject");
  j.write_int("value", numof_broadcast_request);
  j.end_obj();
  if (cells_to_be_broadcast_list_present) {
    j.start_obj();
    j.write_int("id", 144);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_to_be_broadcast_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// WriteReplaceWarningResponseIEs ::= OBJECT SET OF F1AP-PROTOCOL-IES
uint32_t write_replace_warning_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {78, 146, 7, 189};
  return map_enum_number(names, 4, idx, "id");
}
bool write_replace_warning_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {78, 146, 7, 189};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e write_replace_warning_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 78:
      return crit_e::reject;
    case 146:
      return crit_e::reject;
    case 7:
      return crit_e::ignore;
    case 189:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e write_replace_warning_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 78:
      return presence_e::mandatory;
    case 146:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 189:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE write_replace_warning_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += cells_broadcast_completed_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += ded_si_delivery_needed_ue_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  {
    HANDLE_CODE(pack_integer(bref, (uint32_t)78, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_integer(bref, transaction_id, (uint16_t)0u, (uint16_t)255u, true, true));
  }
  if (cells_broadcast_completed_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)146, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::reject}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, cells_broadcast_completed_list, 1, 512, true));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)7, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(crit_diagnostics.pack(bref));
  }
  if (ded_si_delivery_needed_ue_list_present) {
    HANDLE_CODE(pack_integer(bref, (uint32_t)189, (uint32_t)0u, (uint32_t)65535u, false, true));
    HANDLE_CODE(crit_e{crit_e::ignore}.pack(bref));
    varlength_field_pack_guard varlen_scope(bref, true);
    HANDLE_CODE(pack_dyn_seq_of(bref, ded_si_delivery_needed_ue_list, 1, 65536, true));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE write_replace_warning_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    HANDLE_CODE(unpack_integer(id, bref, (uint32_t)0u, (uint32_t)65535u, false, true));
    crit_e crit;
    HANDLE_CODE(crit.unpack(bref));
    if (crit != write_replace_warning_resp_ies_o::get_crit(id)) {
      asn1::log_warning("Unpacked crit does not match object set definition");
    }

    switch (id) {
      case 78: {
        nof_mandatory_ies--;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_integer(transaction_id, bref, (uint16_t)0u, (uint16_t)255u, true, true));
        break;
      }
      case 146: {
        cells_broadcast_completed_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(cells_broadcast_completed_list, bref, 1, 512, true));
        break;
      }
      case 7: {
        crit_diagnostics_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(crit_diagnostics.unpack(bref));
        break;
      }
      case 189: {
        ded_si_delivery_needed_ue_list_present = true;
        varlength_field_unpack_guard varlen_scope(bref, true);
        HANDLE_CODE(unpack_dyn_seq_of(ded_si_delivery_needed_ue_list, bref, 1, 65536, true));
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
void write_replace_warning_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 78);
  j.write_str("criticality", "reject");
  j.write_int("value", transaction_id);
  j.end_obj();
  if (cells_broadcast_completed_list_present) {
    j.start_obj();
    j.write_int("id", 146);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : cells_broadcast_completed_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (ded_si_delivery_needed_ue_list_present) {
    j.start_obj();
    j.write_int("id", 189);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ded_si_delivery_needed_ue_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}
