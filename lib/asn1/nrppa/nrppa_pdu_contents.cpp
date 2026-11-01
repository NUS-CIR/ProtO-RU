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
presence_e aperiodic_srs_ext_ies_o::get_presence(const uint32_t& id)
{
  if (id == 122) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE aperiodic_srs_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += pos_srs_res_set_aggregation_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (pos_srs_res_set_aggregation_list_present) {
    HANDLE_CODE(pack_ie(bref, 122, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pos_srs_res_set_aggregation_list, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE aperiodic_srs_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &aperiodic_srs_ext_ies_o::get_crit));

    if (id == 122) {
      {
        pos_srs_res_set_aggregation_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pos_srs_res_set_aggregation_list, bref, 1, 32, true); }));
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
  if (pos_srs_res_set_aggregation_list_present) {
    j.start_obj();
    j.write_int("id", 122);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : pos_srs_res_set_aggregation_list) {
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

OCUDUASN_CODE assist_info_ctrl_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += assist_info_present ? 1 : 0;
  nof_ies += broadcast_present ? 1 : 0;
  nof_ies += positioning_broadcast_cells_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (assist_info_present) {
    HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() { return assist_info.pack(bref); }));
  }
  if (broadcast_present) {
    HANDLE_CODE(pack_ie(bref, 24, crit_e{crit_e::reject}, [this, &bref]() { return broadcast.pack(bref); }));
  }
  if (positioning_broadcast_cells_present) {
    HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, positioning_broadcast_cells, 1, 16384, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_ctrl_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &assist_info_ctrl_ies_o::get_crit));

    switch (id) {
      case 23: {
        assist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return assist_info.unpack(bref); }));
        break;
      }
      case 24: {
        broadcast_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return broadcast.unpack(bref); }));
        break;
      }
      case 38: {
        positioning_broadcast_cells_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(positioning_broadcast_cells, bref, 1, 16384, true); }));
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
  j.start_array();
  if (assist_info_present) {
    j.start_obj();
    j.write_int("id", 23);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    assist_info.to_json(j);
    j.end_obj();
  }
  if (broadcast_present) {
    j.start_obj();
    j.write_int("id", 24);
    j.write_str("criticality", "reject");
    j.write_str("value", broadcast.to_string());
    j.end_obj();
  }
  if (positioning_broadcast_cells_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : positioning_broadcast_cells) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE assist_info_feedback_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += assist_info_fail_list_present ? 1 : 0;
  nof_ies += positioning_broadcast_cells_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (assist_info_fail_list_present) {
    HANDLE_CODE(pack_ie(bref, 25, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, assist_info_fail_list, 1, 32, true);
    }));
  }
  if (positioning_broadcast_cells_present) {
    HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, positioning_broadcast_cells, 1, 16384, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE assist_info_feedback_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &assist_info_feedback_ies_o::get_crit));

    switch (id) {
      case 25: {
        assist_info_fail_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(assist_info_fail_list, bref, 1, 32, true); }));
        break;
      }
      case 38: {
        positioning_broadcast_cells_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(positioning_broadcast_cells, bref, 1, 16384, true); }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
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
  j.start_array();
  if (assist_info_fail_list_present) {
    j.start_obj();
    j.write_int("id", 25);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : assist_info_fail_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (positioning_broadcast_cells_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : positioning_broadcast_cells) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE e_c_id_meas_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 6, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e_c_id_meas_fail_ind_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 6);
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

OCUDUASN_CODE e_c_id_meas_initiation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e_c_id_meas_initiation_fail_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
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
void e_c_id_meas_initiation_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE e_c_id_meas_initiation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += meas_periodicity_present ? 1 : 0;
  nof_ies += other_rat_meas_quantities_present ? 1 : 0;
  nof_ies += wlan_meas_quantities_present ? 1 : 0;
  nof_ies += meas_periodicity_nr_ao_a_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() { return report_characteristics.pack(bref); }));
  if (meas_periodicity_present) {
    HANDLE_CODE(pack_ie(bref, 4, crit_e{crit_e::reject}, [this, &bref]() { return meas_periodicity.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 5, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, meas_quantities, 1, 64, true);
  }));
  if (other_rat_meas_quantities_present) {
    HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, other_rat_meas_quantities, 0, 64, true);
    }));
  }
  if (wlan_meas_quantities_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, wlan_meas_quantities, 0, 64, true);
    }));
  }
  if (meas_periodicity_nr_ao_a_present) {
    HANDLE_CODE(
        pack_ie(bref, 105, crit_e{crit_e::reject}, [this, &bref]() { return meas_periodicity_nr_ao_a.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e_c_id_meas_initiation_request_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return report_characteristics.unpack(bref); }));
        break;
      }
      case 4: {
        meas_periodicity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_periodicity.unpack(bref); }));
        break;
      }
      case 5: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(meas_quantities, bref, 1, 64, true); }));
        break;
      }
      case 15: {
        other_rat_meas_quantities_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(other_rat_meas_quantities, bref, 0, 64, true); }));
        break;
      }
      case 19: {
        wlan_meas_quantities_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(wlan_meas_quantities, bref, 0, 64, true); }));
        break;
      }
      case 105: {
        meas_periodicity_nr_ao_a_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_periodicity_nr_ao_a.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_str("value", report_characteristics.to_string());
  j.end_obj();
  if (meas_periodicity_present) {
    j.start_obj();
    j.write_int("id", 4);
    j.write_str("criticality", "reject");
    j.write_str("value", meas_periodicity.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 5);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : meas_quantities) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (other_rat_meas_quantities_present) {
    j.start_obj();
    j.write_int("id", 15);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : other_rat_meas_quantities) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (wlan_meas_quantities_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : wlan_meas_quantities) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (meas_periodicity_nr_ao_a_present) {
    j.start_obj();
    j.write_int("id", 105);
    j.write_str("criticality", "reject");
    j.write_str("value", meas_periodicity_nr_ao_a.to_string());
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE e_c_id_meas_initiation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += e_c_id_meas_result_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += cell_portion_id_present ? 1 : 0;
  nof_ies += other_rat_meas_result_present ? 1 : 0;
  nof_ies += wlan_meas_result_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 6, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  if (e_c_id_meas_result_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return e_c_id_meas_result.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (cell_portion_id_present) {
    HANDLE_CODE(pack_ie(bref, 14, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, cell_portion_id, (uint16_t)0u, (uint16_t)4095u, true, true);
    }));
  }
  if (other_rat_meas_result_present) {
    HANDLE_CODE(pack_ie(bref, 17, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, other_rat_meas_result, 1, 64, true);
    }));
  }
  if (wlan_meas_result_present) {
    HANDLE_CODE(pack_ie(bref, 21, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, wlan_meas_result, 1, 64, true);
    }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e_c_id_meas_initiation_resp_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 7: {
        e_c_id_meas_result_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return e_c_id_meas_result.unpack(bref); }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 14: {
        cell_portion_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(cell_portion_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true);
        }));
        break;
      }
      case 17: {
        other_rat_meas_result_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(other_rat_meas_result, bref, 1, 64, true); }));
        break;
      }
      case 21: {
        wlan_meas_result_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(wlan_meas_result, bref, 1, 64, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 6);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  if (e_c_id_meas_result_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    e_c_id_meas_result.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (cell_portion_id_present) {
    j.start_obj();
    j.write_int("id", 14);
    j.write_str("criticality", "ignore");
    j.write_int("value", cell_portion_id);
    j.end_obj();
  }
  if (other_rat_meas_result_present) {
    j.start_obj();
    j.write_int("id", 17);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : other_rat_meas_result) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (wlan_meas_result_present) {
    j.start_obj();
    j.write_int("id", 21);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : wlan_meas_result) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE e_c_id_meas_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += cell_portion_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 6, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() { return e_c_id_meas_result.pack(bref); }));
  if (cell_portion_id_present) {
    HANDLE_CODE(pack_ie(bref, 14, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, cell_portion_id, (uint16_t)0u, (uint16_t)4095u, true, true);
    }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e_c_id_meas_report_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 7: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return e_c_id_meas_result.unpack(bref); }));
        break;
      }
      case 14: {
        cell_portion_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(cell_portion_id, bref, (uint16_t)0u, (uint16_t)4095u, true, true);
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
void e_c_id_meas_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 6);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 7);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  e_c_id_meas_result.to_json(j);
  j.end_obj();
  if (cell_portion_id_present) {
    j.start_obj();
    j.write_int("id", 14);
    j.write_str("criticality", "ignore");
    j.write_int("value", cell_portion_id);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE e_c_id_meas_termination_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 6, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_meas_id, (uint8_t)1u, (uint8_t)15u, true, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE e_c_id_meas_termination_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &e_c_id_meas_termination_cmd_ies_o::get_crit));

    switch (id) {
      case 2: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
        }));
        break;
      }
      case 6: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_meas_id, bref, (uint8_t)1u, (uint8_t)15u, true, true);
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
void e_c_id_meas_termination_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 2);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_ue_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 6);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_ue_meas_id);
  j.end_obj();
  j.end_array();
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

OCUDUASN_CODE error_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (cause_present) {
    HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
      case 0: {
        cause_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
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
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE meas_abort_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_abort_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_abort_ies_o::get_crit));

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
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
void meas_abort_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  j.end_array();
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

OCUDUASN_CODE meas_activation_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += prs_meass_info_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 98, crit_e{crit_e::reject}, [this, &bref]() { return request_type.pack(bref); }));
  if (prs_meass_info_list_present) {
    HANDLE_CODE(pack_ie(bref, 88, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, prs_meass_info_list, 1, 4, true);
    }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_activation_ies_o::get_crit));

    switch (id) {
      case 98: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return request_type.unpack(bref); }));
        break;
      }
      case 88: {
        prs_meass_info_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(prs_meass_info_list, bref, 1, 4, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 98);
  j.write_str("criticality", "reject");
  j.write_str("value", request_type.to_string());
  j.end_obj();
  if (prs_meass_info_list_present) {
    j.start_obj();
    j.write_int("id", 88);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : prs_meass_info_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE meas_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_fail_ies_o::get_crit));

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
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
void meas_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE meas_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_fail_ind_ies_o::get_crit));

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 40);
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

OCUDUASN_CODE meas_precfg_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 97, crit_e{crit_e::ignore}, [this, &bref]() { return precfg_result.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_precfg_confirm_ies_o::get_crit));

    switch (id) {
      case 97: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return precfg_result.unpack(bref); }));
        break;
      }
      case 1: {
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
void meas_precfg_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 97);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  precfg_result.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE meas_precfg_refuse_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_precfg_refuse_ies_o::get_crit));

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
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
void meas_precfg_refuse_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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
presence_e meas_precfg_required_ies_o::get_presence(const uint32_t& id)
{
  if (id == 87) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE meas_precfg_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 87, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, trp_prs_info_list, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_precfg_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_precfg_required_ies_o::get_crit));

    if (id == 87) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(trp_prs_info_list, bref, 1, 256, true); }));
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
void meas_precfg_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 87);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : trp_prs_info_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
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

OCUDUASN_CODE meas_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 43, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, trp_meas_report_list, 1, 64, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE meas_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_report_ies_o::get_crit));

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 43: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(trp_meas_report_list, bref, 1, 64, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 43);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : trp_meas_report_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
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

  HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, trp_meas_request_list, 1, 64, true);
  }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() { return report_characteristics.pack(bref); }));
  if (meas_periodicity_present) {
    HANDLE_CODE(pack_ie(bref, 4, crit_e{crit_e::reject}, [this, &bref]() { return meas_periodicity.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 52, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, trp_meas_quantities, 1, 16384, true);
  }));
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_ie(bref, 54, crit_e{crit_e::ignore}, [this, &bref]() { return sfn_initisation_time.pack(bref); }));
  }
  if (srs_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::ignore}, [this, &bref]() { return srs_cfg.pack(bref); }));
  }
  if (meas_beam_info_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 31, crit_e{crit_e::ignore}, [this, &bref]() { return meas_beam_info_request.pack(bref); }));
  }
  if (sys_frame_num_present) {
    HANDLE_CODE(pack_ie(bref, 49, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true);
    }));
  }
  if (slot_num_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, slot_num, (uint8_t)0u, (uint8_t)79u, false, true);
    }));
  }
  if (meas_periodicity_extended_present) {
    HANDLE_CODE(
        pack_ie(bref, 64, crit_e{crit_e::reject}, [this, &bref]() { return meas_periodicity_extended.pack(bref); }));
  }
  if (resp_time_present) {
    HANDLE_CODE(pack_ie(bref, 72, crit_e{crit_e::ignore}, [this, &bref]() { return resp_time.pack(bref); }));
  }
  if (meas_characteristics_request_ind_present) {
    HANDLE_CODE(pack_ie(
        bref, 92, crit_e{crit_e::ignore}, [this, &bref]() { return meas_characteristics_request_ind.pack(bref); }));
  }
  if (meas_time_occasion_present) {
    HANDLE_CODE(pack_ie(bref, 91, crit_e{crit_e::ignore}, [this, &bref]() { return meas_time_occasion.pack(bref); }));
  }
  if (meas_amount_present) {
    HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::ignore}, [this, &bref]() { return meas_amount.pack(bref); }));
  }
  if (time_win_info_meas_list_present) {
    HANDLE_CODE(pack_ie(bref, 125, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, time_win_info_meas_list, 1, 16, true);
    }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_request_ies_o::get_crit));

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 41: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(trp_meas_request_list, bref, 1, 64, true); }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return report_characteristics.unpack(bref); }));
        break;
      }
      case 4: {
        meas_periodicity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_periodicity.unpack(bref); }));
        break;
      }
      case 52: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(trp_meas_quantities, bref, 1, 16384, true); }));
        break;
      }
      case 54: {
        sfn_initisation_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sfn_initisation_time.unpack(bref); }));
        break;
      }
      case 26: {
        srs_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_cfg.unpack(bref); }));
        break;
      }
      case 31: {
        meas_beam_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_beam_info_request.unpack(bref); }));
        break;
      }
      case 49: {
        sys_frame_num_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true);
        }));
        break;
      }
      case 50: {
        slot_num_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_integer(slot_num, bref, (uint8_t)0u, (uint8_t)79u, false, true); }));
        break;
      }
      case 64: {
        meas_periodicity_extended_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_periodicity_extended.unpack(bref); }));
        break;
      }
      case 72: {
        resp_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return resp_time.unpack(bref); }));
        break;
      }
      case 92: {
        meas_characteristics_request_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_characteristics_request_ind.unpack(bref); }));
        break;
      }
      case 91: {
        meas_time_occasion_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_time_occasion.unpack(bref); }));
        break;
      }
      case 95: {
        meas_amount_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_amount.unpack(bref); }));
        break;
      }
      case 125: {
        time_win_info_meas_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(time_win_info_meas_list, bref, 1, 16, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 41);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : trp_meas_request_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_str("value", report_characteristics.to_string());
  j.end_obj();
  if (meas_periodicity_present) {
    j.start_obj();
    j.write_int("id", 4);
    j.write_str("criticality", "reject");
    j.write_str("value", meas_periodicity.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 52);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : trp_meas_quantities) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (sfn_initisation_time_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sfn_initisation_time.to_json(j);
    j.end_obj();
  }
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_cfg.to_json(j);
    j.end_obj();
  }
  if (meas_beam_info_request_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (sys_frame_num_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_int("value", sys_frame_num);
    j.end_obj();
  }
  if (slot_num_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.write_int("value", slot_num);
    j.end_obj();
  }
  if (meas_periodicity_extended_present) {
    j.start_obj();
    j.write_int("id", 64);
    j.write_str("criticality", "reject");
    j.write_str("value", meas_periodicity_extended.to_string());
    j.end_obj();
  }
  if (resp_time_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    resp_time.to_json(j);
    j.end_obj();
  }
  if (meas_characteristics_request_ind_present) {
    j.start_obj();
    j.write_int("id", 92);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    meas_characteristics_request_ind.to_json(j);
    j.end_obj();
  }
  if (meas_time_occasion_present) {
    j.start_obj();
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.write_str("value", meas_time_occasion.to_string());
    j.end_obj();
  }
  if (meas_amount_present) {
    j.start_obj();
    j.write_int("id", 95);
    j.write_str("criticality", "ignore");
    j.write_str("value", meas_amount.to_string());
    j.end_obj();
  }
  if (time_win_info_meas_list_present) {
    j.start_obj();
    j.write_int("id", 125);
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

OCUDUASN_CODE meas_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += trp_meas_resp_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  if (trp_meas_resp_list_present) {
    HANDLE_CODE(pack_ie(bref, 42, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, trp_meas_resp_list, 1, 64, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_resp_ies_o::get_crit));

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 42: {
        trp_meas_resp_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(trp_meas_resp_list, bref, 1, 64, true); }));
        break;
      }
      case 1: {
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
void meas_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  if (trp_meas_resp_list_present) {
    j.start_obj();
    j.write_int("id", 42);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : trp_meas_resp_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE meas_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += srs_cfg_present ? 1 : 0;
  nof_ies += trp_meas_upd_list_present ? 1 : 0;
  nof_ies += meas_characteristics_request_ind_present ? 1 : 0;
  nof_ies += meas_time_occasion_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 39, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, lmf_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_meas_id, (uint32_t)1u, (uint32_t)65536u, true, true);
  }));
  if (srs_cfg_present) {
    HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::ignore}, [this, &bref]() { return srs_cfg.pack(bref); }));
  }
  if (trp_meas_upd_list_present) {
    HANDLE_CODE(pack_ie(bref, 70, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, trp_meas_upd_list, 1, 64, true);
    }));
  }
  if (meas_characteristics_request_ind_present) {
    HANDLE_CODE(pack_ie(
        bref, 92, crit_e{crit_e::ignore}, [this, &bref]() { return meas_characteristics_request_ind.pack(bref); }));
  }
  if (meas_time_occasion_present) {
    HANDLE_CODE(pack_ie(bref, 91, crit_e{crit_e::ignore}, [this, &bref]() { return meas_time_occasion.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &meas_upd_ies_o::get_crit));

    switch (id) {
      case 39: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(lmf_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 40: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_meas_id, bref, (uint32_t)1u, (uint32_t)65536u, true, true);
        }));
        break;
      }
      case 26: {
        srs_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_cfg.unpack(bref); }));
        break;
      }
      case 70: {
        trp_meas_upd_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(trp_meas_upd_list, bref, 1, 64, true); }));
        break;
      }
      case 92: {
        meas_characteristics_request_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_characteristics_request_ind.unpack(bref); }));
        break;
      }
      case 91: {
        meas_time_occasion_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return meas_time_occasion.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.start_array();
  j.start_obj();
  j.write_int("id", 39);
  j.write_str("criticality", "reject");
  j.write_int("value", lmf_meas_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 40);
  j.write_str("criticality", "reject");
  j.write_int("value", ran_meas_id);
  j.end_obj();
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_cfg.to_json(j);
    j.end_obj();
  }
  if (trp_meas_upd_list_present) {
    j.start_obj();
    j.write_int("id", 70);
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
    j.write_int("id", 92);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    meas_characteristics_request_ind.to_json(j);
    j.end_obj();
  }
  if (meas_time_occasion_present) {
    j.start_obj();
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.write_str("value", meas_time_occasion.to_string());
    j.end_obj();
  }
  j.end_array();
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
  c.to_json(j);
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

OCUDUASN_CODE otdoa_info_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &otdoa_info_fail_ies_o::get_crit));

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
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
void otdoa_info_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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
presence_e otdoa_info_request_ies_o::get_presence(const uint32_t& id)
{
  if (id == 9) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE otdoa_info_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 9, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, otdoa_info_type_group, 1, 63, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE otdoa_info_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &otdoa_info_request_ies_o::get_crit));

    if (id == 9) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(otdoa_info_type_group, bref, 1, 63, true); }));
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
void otdoa_info_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 9);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : otdoa_info_type_group) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
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

OCUDUASN_CODE otdoa_info_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(
      bref, 8, crit_e{crit_e::ignore}, [this, &bref]() { return pack_dyn_seq_of(bref, otdoa_cells, 1, 3840, true); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &otdoa_info_resp_ies_o::get_crit));

    switch (id) {
      case 8: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(otdoa_cells, bref, 1, 3840, true); }));
        break;
      }
      case 1: {
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
void otdoa_info_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 8);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : otdoa_cells) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE prs_cfg_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &prs_cfg_fail_ies_o::get_crit));

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
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
void prs_cfg_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE prs_cfg_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() { return prs_cfg_request_type.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 66, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, prstrp_list, 1, 65535, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &prs_cfg_request_ies_o::get_crit));

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return prs_cfg_request_type.unpack(bref); }));
        break;
      }
      case 66: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(prstrp_list, bref, 1, 65535, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_str("value", prs_cfg_request_type.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 66);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : prstrp_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
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

OCUDUASN_CODE prs_cfg_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += prs_tx_trp_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (prs_tx_trp_list_present) {
    HANDLE_CODE(pack_ie(bref, 67, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, prs_tx_trp_list, 1, 65535, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE prs_cfg_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &prs_cfg_resp_ies_o::get_crit));

    switch (id) {
      case 67: {
        prs_tx_trp_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(prs_tx_trp_list, bref, 1, 65535, true); }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
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
  j.start_array();
  if (prs_tx_trp_list_present) {
    j.start_obj();
    j.write_int("id", 67);
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
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE positioning_activation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &positioning_activation_fail_ies_o::get_crit));

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
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
void positioning_activation_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE semipersistent_srs_ext_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += srs_spatial_relation_present ? 1 : 0;
  nof_ies += srs_spatial_relation_per_srs_res_present ? 1 : 0;
  nof_ies += pos_srs_res_set_aggregation_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 1u, 65535u, true));

  if (srs_spatial_relation_present) {
    HANDLE_CODE(pack_ie(bref, 48, crit_e{crit_e::ignore}, [this, &bref]() { return srs_spatial_relation.pack(bref); }));
  }
  if (srs_spatial_relation_per_srs_res_present) {
    HANDLE_CODE(pack_ie(
        bref, 63, crit_e{crit_e::ignore}, [this, &bref]() { return srs_spatial_relation_per_srs_res.pack(bref); }));
  }
  if (pos_srs_res_set_aggregation_list_present) {
    HANDLE_CODE(pack_ie(bref, 122, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pos_srs_res_set_aggregation_list, 1, 32, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE semipersistent_srs_ext_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 1u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &semipersistent_srs_ext_ies_o::get_crit));

    switch (id) {
      case 48: {
        srs_spatial_relation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_spatial_relation.unpack(bref); }));
        break;
      }
      case 63: {
        srs_spatial_relation_per_srs_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_spatial_relation_per_srs_res.unpack(bref); }));
        break;
      }
      case 122: {
        pos_srs_res_set_aggregation_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pos_srs_res_set_aggregation_list, bref, 1, 32, true); }));
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
  if (srs_spatial_relation_present) {
    j.start_obj();
    j.write_int("id", 48);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    srs_spatial_relation.to_json(j);
    j.end_obj();
  }
  if (srs_spatial_relation_per_srs_res_present) {
    j.start_obj();
    j.write_int("id", 63);
    j.write_str("criticality", "ignore");
    j.write_fieldname("extensionValue");
    srs_spatial_relation_per_srs_res.to_json(j);
    j.end_obj();
  }
  if (pos_srs_res_set_aggregation_list_present) {
    j.start_obj();
    j.write_int("id", 122);
    j.write_str("criticality", "ignore");
    j.start_array("extensionValue");
    for (const auto& e1 : pos_srs_res_set_aggregation_list) {
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

OCUDUASN_CODE positioning_activation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += activation_time_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 44, crit_e{crit_e::reject}, [this, &bref]() { return srs_type.pack(bref); }));
  if (activation_time_present) {
    HANDLE_CODE(pack_ie(bref, 45, crit_e{crit_e::ignore}, [this, &bref]() { return activation_time.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &positioning_activation_request_ies_o::get_crit));

    switch (id) {
      case 44: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_type.unpack(bref); }));
        break;
      }
      case 45: {
        activation_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return activation_time.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 44);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  srs_type.to_json(j);
  j.end_obj();
  if (activation_time_present) {
    j.start_obj();
    j.write_int("id", 45);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    activation_time.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE positioning_activation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += sys_frame_num_present ? 1 : 0;
  nof_ies += slot_num_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (sys_frame_num_present) {
    HANDLE_CODE(pack_ie(bref, 49, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, sys_frame_num, (uint16_t)0u, (uint16_t)1023u, false, true);
    }));
  }
  if (slot_num_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, slot_num, (uint8_t)0u, (uint8_t)79u, false, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_activation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &positioning_activation_resp_ies_o::get_crit));

    switch (id) {
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 49: {
        sys_frame_num_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(sys_frame_num, bref, (uint16_t)0u, (uint16_t)1023u, false, true);
        }));
        break;
      }
      case 50: {
        slot_num_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_integer(slot_num, bref, (uint8_t)0u, (uint8_t)79u, false, true); }));
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
  j.start_array();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (sys_frame_num_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.write_int("value", sys_frame_num);
    j.end_obj();
  }
  if (slot_num_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.write_int("value", slot_num);
    j.end_obj();
  }
  j.end_array();
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
presence_e positioning_deactivation_ies_o::get_presence(const uint32_t& id)
{
  if (id == 53) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE positioning_deactivation_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 53, crit_e{crit_e::ignore}, [this, &bref]() { return abort_tx.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_deactivation_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &positioning_deactivation_ies_o::get_crit));

    if (id == 53) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return abort_tx.unpack(bref); }));
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
void positioning_deactivation_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 53);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  abort_tx.to_json(j);
  j.end_obj();
  j.end_array();
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

OCUDUASN_CODE positioning_info_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &positioning_info_fail_ies_o::get_crit));

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
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
void positioning_info_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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
    HANDLE_CODE(pack_ie(
        bref, 12, crit_e{crit_e::ignore}, [this, &bref]() { return requested_srs_tx_characteristics.pack(bref); }));
  }
  if (ue_report_info_present) {
    HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::ignore}, [this, &bref]() { return ue_report_info.pack(bref); }));
  }
  if (ue_teg_info_request_present) {
    HANDLE_CODE(pack_ie(bref, 90, crit_e{crit_e::ignore}, [this, &bref]() { return ue_teg_info_request.pack(bref); }));
  }
  if (ue_teg_report_periodicity_present) {
    HANDLE_CODE(
        pack_ie(bref, 99, crit_e{crit_e::reject}, [this, &bref]() { return ue_teg_report_periodicity.pack(bref); }));
  }
  if (time_win_info_srs_list_present) {
    HANDLE_CODE(pack_ie(bref, 124, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, time_win_info_srs_list, 1, 16, true);
    }));
  }
  if (requested_srs_precfg_characteristics_list_present) {
    HANDLE_CODE(pack_ie(bref, 136, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, requested_srs_precfg_characteristics_list, 1, 16, true);
    }));
  }
  if (remote_ue_ind_request_present) {
    HANDLE_CODE(
        pack_ie(bref, 159, crit_e{crit_e::ignore}, [this, &bref]() { return remote_ue_ind_request.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &positioning_info_request_ies_o::get_crit));

    switch (id) {
      case 12: {
        requested_srs_tx_characteristics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return requested_srs_tx_characteristics.unpack(bref); }));
        break;
      }
      case 73: {
        ue_report_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_report_info.unpack(bref); }));
        break;
      }
      case 90: {
        ue_teg_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_teg_info_request.unpack(bref); }));
        break;
      }
      case 99: {
        ue_teg_report_periodicity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_teg_report_periodicity.unpack(bref); }));
        break;
      }
      case 124: {
        time_win_info_srs_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(time_win_info_srs_list, bref, 1, 16, true); }));
        break;
      }
      case 136: {
        requested_srs_precfg_characteristics_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(requested_srs_precfg_characteristics_list, bref, 1, 16, true);
        }));
        break;
      }
      case 159: {
        remote_ue_ind_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return remote_ue_ind_request.unpack(bref); }));
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
  j.start_array();
  if (requested_srs_tx_characteristics_present) {
    j.start_obj();
    j.write_int("id", 12);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    requested_srs_tx_characteristics.to_json(j);
    j.end_obj();
  }
  if (ue_report_info_present) {
    j.start_obj();
    j.write_int("id", 73);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_report_info.to_json(j);
    j.end_obj();
  }
  if (ue_teg_info_request_present) {
    j.start_obj();
    j.write_int("id", 90);
    j.write_str("criticality", "ignore");
    j.write_str("value", ue_teg_info_request.to_string());
    j.end_obj();
  }
  if (ue_teg_report_periodicity_present) {
    j.start_obj();
    j.write_int("id", 99);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_teg_report_periodicity.to_string());
    j.end_obj();
  }
  if (time_win_info_srs_list_present) {
    j.start_obj();
    j.write_int("id", 124);
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
    j.write_int("id", 136);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : requested_srs_precfg_characteristics_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (remote_ue_ind_request_present) {
    j.start_obj();
    j.write_int("id", 159);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
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
    HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::ignore}, [this, &bref]() { return srs_cfg.pack(bref); }));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_ie(bref, 54, crit_e{crit_e::ignore}, [this, &bref]() { return sfn_initisation_time.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (ue_tx_teg_assoc_list_present) {
    HANDLE_CODE(pack_ie(bref, 81, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_tx_teg_assoc_list, 1, 256, true);
    }));
  }
  if (new_nr_cgi_present) {
    HANDLE_CODE(pack_ie(bref, 113, crit_e{crit_e::ignore}, [this, &bref]() { return new_nr_cgi.pack(bref); }));
  }
  if (pos_validity_area_cell_list_present) {
    HANDLE_CODE(pack_ie(bref, 128, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pos_validity_area_cell_list, 1, 32, true);
    }));
  }
  if (srs_precfg_list_present) {
    HANDLE_CODE(pack_ie(bref, 137, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, srs_precfg_list, 1, 16, true);
    }));
  }
  if (remote_ue_ind_present) {
    HANDLE_CODE(pack_ie(bref, 160, crit_e{crit_e::ignore}, [this, &bref]() { return remote_ue_ind.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &positioning_info_resp_ies_o::get_crit));

    switch (id) {
      case 26: {
        srs_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_cfg.unpack(bref); }));
        break;
      }
      case 54: {
        sfn_initisation_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sfn_initisation_time.unpack(bref); }));
        break;
      }
      case 1: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 81: {
        ue_tx_teg_assoc_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_tx_teg_assoc_list, bref, 1, 256, true); }));
        break;
      }
      case 113: {
        new_nr_cgi_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return new_nr_cgi.unpack(bref); }));
        break;
      }
      case 128: {
        pos_validity_area_cell_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pos_validity_area_cell_list, bref, 1, 32, true); }));
        break;
      }
      case 137: {
        srs_precfg_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(srs_precfg_list, bref, 1, 16, true); }));
        break;
      }
      case 160: {
        remote_ue_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return remote_ue_ind.unpack(bref); }));
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
  j.start_array();
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_cfg.to_json(j);
    j.end_obj();
  }
  if (sfn_initisation_time_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sfn_initisation_time.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (ue_tx_teg_assoc_list_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_tx_teg_assoc_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (new_nr_cgi_present) {
    j.start_obj();
    j.write_int("id", 113);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    new_nr_cgi.to_json(j);
    j.end_obj();
  }
  if (pos_validity_area_cell_list_present) {
    j.start_obj();
    j.write_int("id", 128);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pos_validity_area_cell_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srs_precfg_list_present) {
    j.start_obj();
    j.write_int("id", 137);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : srs_precfg_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (remote_ue_ind_present) {
    j.start_obj();
    j.write_int("id", 160);
    j.write_str("criticality", "ignore");
    j.write_str("value", "l2-u2n-remote-ue");
    j.end_obj();
  }
  j.end_array();
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
    HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::ignore}, [this, &bref]() { return srs_cfg.pack(bref); }));
  }
  if (sfn_initisation_time_present) {
    HANDLE_CODE(pack_ie(bref, 54, crit_e{crit_e::ignore}, [this, &bref]() { return sfn_initisation_time.pack(bref); }));
  }
  if (ue_tx_teg_assoc_list_present) {
    HANDLE_CODE(pack_ie(bref, 81, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_tx_teg_assoc_list, 1, 256, true);
    }));
  }
  if (srs_tx_status_present) {
    HANDLE_CODE(pack_ie(bref, 106, crit_e{crit_e::ignore}, [this, &bref]() { return srs_tx_status.pack(bref); }));
  }
  if (new_cell_id_present) {
    HANDLE_CODE(pack_ie(bref, 134, crit_e{crit_e::ignore}, [this, &bref]() { return new_cell_id.pack(bref); }));
  }
  if (remote_ue_status_present) {
    HANDLE_CODE(pack_ie(bref, 161, crit_e{crit_e::ignore}, [this, &bref]() { return remote_ue_status.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE positioning_info_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &positioning_info_upd_ies_o::get_crit));

    switch (id) {
      case 26: {
        srs_cfg_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_cfg.unpack(bref); }));
        break;
      }
      case 54: {
        sfn_initisation_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sfn_initisation_time.unpack(bref); }));
        break;
      }
      case 81: {
        ue_tx_teg_assoc_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_tx_teg_assoc_list, bref, 1, 256, true); }));
        break;
      }
      case 106: {
        srs_tx_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_tx_status.unpack(bref); }));
        break;
      }
      case 134: {
        new_cell_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return new_cell_id.unpack(bref); }));
        break;
      }
      case 161: {
        remote_ue_status_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return remote_ue_status.unpack(bref); }));
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
  j.start_array();
  if (srs_cfg_present) {
    j.start_obj();
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_cfg.to_json(j);
    j.end_obj();
  }
  if (sfn_initisation_time_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sfn_initisation_time.to_json(j);
    j.end_obj();
  }
  if (ue_tx_teg_assoc_list_present) {
    j.start_obj();
    j.write_int("id", 81);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_tx_teg_assoc_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srs_tx_status_present) {
    j.start_obj();
    j.write_int("id", 106);
    j.write_str("criticality", "ignore");
    j.write_str("value", srs_tx_status.to_string());
    j.end_obj();
  }
  if (new_cell_id_present) {
    j.start_obj();
    j.write_int("id", 134);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    new_cell_id.to_json(j);
    j.end_obj();
  }
  if (remote_ue_status_present) {
    j.start_obj();
    j.write_int("id", 161);
    j.write_str("criticality", "ignore");
    j.write_str("value", remote_ue_status.to_string());
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE srs_info_reserv_notif_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += srs_info_present ? 1 : 0;
  nof_ies += precfg_srs_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 129, crit_e{crit_e::reject}, [this, &bref]() { return srs_reserv_type.pack(bref); }));
  if (srs_info_present) {
    HANDLE_CODE(pack_ie(bref, 138, crit_e{crit_e::ignore}, [this, &bref]() { return srs_info.pack(bref); }));
  }
  if (precfg_srs_info_present) {
    HANDLE_CODE(pack_ie(bref, 163, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, precfg_srs_info, 1, 16, true);
    }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &srs_info_reserv_notif_ies_o::get_crit));

    switch (id) {
      case 129: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_reserv_type.unpack(bref); }));
        break;
      }
      case 138: {
        srs_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srs_info.unpack(bref); }));
        break;
      }
      case 163: {
        precfg_srs_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(precfg_srs_info, bref, 1, 16, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 129);
  j.write_str("criticality", "reject");
  j.write_str("value", srs_reserv_type.to_string());
  j.end_obj();
  if (srs_info_present) {
    j.start_obj();
    j.write_int("id", 138);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    srs_info.to_json(j);
    j.end_obj();
  }
  if (precfg_srs_info_present) {
    j.start_obj();
    j.write_int("id", 163);
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

OCUDUASN_CODE trp_info_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &trp_info_fail_ies_o::get_crit));

    switch (id) {
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 1: {
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
void trp_info_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
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

OCUDUASN_CODE trp_info_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += trp_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (trp_list_present) {
    HANDLE_CODE(pack_ie(
        bref, 47, crit_e{crit_e::ignore}, [this, &bref]() { return pack_dyn_seq_of(bref, trp_list, 1, 65535, true); }));
  }
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, trp_info_type_list_trp_req, 1, 64, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trp_info_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &trp_info_request_ies_o::get_crit));

    switch (id) {
      case 47: {
        trp_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(trp_list, bref, 1, 65535, true); }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(trp_info_type_list_trp_req, bref, 1, 64, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  if (trp_list_present) {
    j.start_obj();
    j.write_int("id", 47);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : trp_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : trp_info_type_list_trp_req) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
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

OCUDUASN_CODE trp_info_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 30, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, trp_info_list_trp_resp, 1, 65535, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &trp_info_resp_ies_o::get_crit));

    switch (id) {
      case 30: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(trp_info_list_trp_resp, bref, 1, 65535, true); }));
        break;
      }
      case 1: {
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
void trp_info_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 30);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : trp_info_list_trp_resp) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}
