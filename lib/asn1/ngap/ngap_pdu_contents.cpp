// SPDX-FileCopyrightText: Copyright (C) 2021-2026 Software Radio Systems Limited
// SPDX-FileCopyrightText: Copyright (C) 2026 OCUDU contributors
// SPDX-License-Identifier: BSD-3-Clause-Open-MPI
// Portions of this file may implement 3GPP specifications, which may be subject to additional licensing requirements.

#include "ocudu/asn1/ngap/ngap_pdu_contents.h"

using namespace asn1;
using namespace asn1::ngap;

/*******************************************************************************
 *                                Struct Methods
 ******************************************************************************/

// AMFCPRelocationIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t amf_cp_relocation_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 148, 0, 414};
  return map_enum_number(names, 5, idx, "id");
}
bool amf_cp_relocation_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 148, 0, 414};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e amf_cp_relocation_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 148:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 414:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e amf_cp_relocation_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 148:
      return presence_e::optional;
    case 0:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE amf_cp_relocation_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += s_nssai_present ? 1 : 0;
  nof_ies += allowed_nssai_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (s_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 148, crit_e{crit_e::ignore}, [this, &bref]() { return s_nssai.pack(bref); }));
  }
  if (allowed_nssai_present) {
    HANDLE_CODE(pack_ie(
        bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE amf_cp_relocation_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &amf_cp_relocation_ind_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 148: {
        s_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_nssai.unpack(bref); }));
        break;
      }
      case 0: {
        allowed_nssai_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void amf_cp_relocation_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (s_nssai_present) {
    j.start_obj();
    j.write_int("id", 148);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    s_nssai.to_json(j);
    j.end_obj();
  }
  if (allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 0);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// AMFConfigurationUpdateIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t amf_cfg_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {1, 96, 86, 80, 6, 7, 8, 274};
  return map_enum_number(names, 8, idx, "id");
}
bool amf_cfg_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {1, 96, 86, 80, 6, 7, 8, 274};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e amf_cfg_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 1:
      return crit_e::reject;
    case 96:
      return crit_e::reject;
    case 86:
      return crit_e::ignore;
    case 80:
      return crit_e::reject;
    case 6:
      return crit_e::ignore;
    case 7:
      return crit_e::ignore;
    case 8:
      return crit_e::ignore;
    case 274:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e amf_cfg_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 1:
      return presence_e::optional;
    case 96:
      return presence_e::optional;
    case 86:
      return presence_e::optional;
    case 80:
      return presence_e::optional;
    case 6:
      return presence_e::optional;
    case 7:
      return presence_e::optional;
    case 8:
      return presence_e::optional;
    case 274:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE amf_cfg_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += amf_name_present ? 1 : 0;
  nof_ies += served_guami_list_present ? 1 : 0;
  nof_ies += relative_amf_capacity_present ? 1 : 0;
  nof_ies += plmn_support_list_present ? 1 : 0;
  nof_ies += amf_tnl_assoc_to_add_list_present ? 1 : 0;
  nof_ies += amf_tnl_assoc_to_rem_list_present ? 1 : 0;
  nof_ies += amf_tnl_assoc_to_upd_list_present ? 1 : 0;
  nof_ies += extended_amf_name_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (amf_name_present) {
    HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::reject}, [this, &bref]() { return amf_name.pack(bref); }));
  }
  if (served_guami_list_present) {
    HANDLE_CODE(pack_ie(bref, 96, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, served_guami_list, 1, 256, true);
    }));
  }
  if (relative_amf_capacity_present) {
    HANDLE_CODE(pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, relative_amf_capacity, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (plmn_support_list_present) {
    HANDLE_CODE(pack_ie(bref, 80, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, plmn_support_list, 1, 12, true);
    }));
  }
  if (amf_tnl_assoc_to_add_list_present) {
    HANDLE_CODE(pack_ie(bref, 6, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, amf_tnl_assoc_to_add_list, 1, 32, true);
    }));
  }
  if (amf_tnl_assoc_to_rem_list_present) {
    HANDLE_CODE(pack_ie(bref, 7, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, amf_tnl_assoc_to_rem_list, 1, 32, true);
    }));
  }
  if (amf_tnl_assoc_to_upd_list_present) {
    HANDLE_CODE(pack_ie(bref, 8, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, amf_tnl_assoc_to_upd_list, 1, 32, true);
    }));
  }
  if (extended_amf_name_present) {
    HANDLE_CODE(pack_ie(bref, 274, crit_e{crit_e::ignore}, [this, &bref]() { return extended_amf_name.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE amf_cfg_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &amf_cfg_upd_ies_o::get_crit));

    switch (id) {
      case 1: {
        amf_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return amf_name.unpack(bref); }));
        break;
      }
      case 96: {
        served_guami_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(served_guami_list, bref, 1, 256, true); }));
        break;
      }
      case 86: {
        relative_amf_capacity_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(relative_amf_capacity, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 80: {
        plmn_support_list_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(plmn_support_list, bref, 1, 12, true); }));
        break;
      }
      case 6: {
        amf_tnl_assoc_to_add_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(amf_tnl_assoc_to_add_list, bref, 1, 32, true); }));
        break;
      }
      case 7: {
        amf_tnl_assoc_to_rem_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(amf_tnl_assoc_to_rem_list, bref, 1, 32, true); }));
        break;
      }
      case 8: {
        amf_tnl_assoc_to_upd_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(amf_tnl_assoc_to_upd_list, bref, 1, 32, true); }));
        break;
      }
      case 274: {
        extended_amf_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_amf_name.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void amf_cfg_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (amf_name_present) {
    j.start_obj();
    j.write_int("id", 1);
    j.write_str("criticality", "reject");
    j.write_str("value", amf_name.to_string());
    j.end_obj();
  }
  if (served_guami_list_present) {
    j.start_obj();
    j.write_int("id", 96);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : served_guami_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (relative_amf_capacity_present) {
    j.start_obj();
    j.write_int("id", 86);
    j.write_str("criticality", "ignore");
    j.write_int("value", relative_amf_capacity);
    j.end_obj();
  }
  if (plmn_support_list_present) {
    j.start_obj();
    j.write_int("id", 80);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : plmn_support_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (amf_tnl_assoc_to_add_list_present) {
    j.start_obj();
    j.write_int("id", 6);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : amf_tnl_assoc_to_add_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (amf_tnl_assoc_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 7);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : amf_tnl_assoc_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (amf_tnl_assoc_to_upd_list_present) {
    j.start_obj();
    j.write_int("id", 8);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : amf_tnl_assoc_to_upd_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (extended_amf_name_present) {
    j.start_obj();
    j.write_int("id", 274);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_amf_name.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// AMFConfigurationUpdateAcknowledgeIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t amf_cfg_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {5, 4, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool amf_cfg_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {5, 4, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e amf_cfg_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 5:
      return crit_e::ignore;
    case 4:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e amf_cfg_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 5:
      return presence_e::optional;
    case 4:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE amf_cfg_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += amf_tnl_assoc_setup_list_present ? 1 : 0;
  nof_ies += amf_tnl_assoc_failed_to_setup_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (amf_tnl_assoc_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 5, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, amf_tnl_assoc_setup_list, 1, 32, true);
    }));
  }
  if (amf_tnl_assoc_failed_to_setup_list_present) {
    HANDLE_CODE(pack_ie(bref, 4, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, amf_tnl_assoc_failed_to_setup_list, 1, 32, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE amf_cfg_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &amf_cfg_upd_ack_ies_o::get_crit));

    switch (id) {
      case 5: {
        amf_tnl_assoc_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(amf_tnl_assoc_setup_list, bref, 1, 32, true); }));
        break;
      }
      case 4: {
        amf_tnl_assoc_failed_to_setup_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(amf_tnl_assoc_failed_to_setup_list, bref, 1, 32, true);
        }));
        break;
      }
      case 19: {
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
void amf_cfg_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (amf_tnl_assoc_setup_list_present) {
    j.start_obj();
    j.write_int("id", 5);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : amf_tnl_assoc_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (amf_tnl_assoc_failed_to_setup_list_present) {
    j.start_obj();
    j.write_int("id", 4);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : amf_tnl_assoc_failed_to_setup_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// AMFConfigurationUpdateFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t amf_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {15, 107, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool amf_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {15, 107, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e amf_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 15:
      return crit_e::ignore;
    case 107:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e amf_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 15:
      return presence_e::mandatory;
    case 107:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE amf_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 107, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE amf_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &amf_cfg_upd_fail_ies_o::get_crit));

    switch (id) {
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 107: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
        break;
      }
      case 19: {
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
void amf_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 107);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// AMFStatusIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t amf_status_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {120};
  return map_enum_number(names, 1, idx, "id");
}
bool amf_status_ind_ies_o::is_id_valid(const uint32_t& id)
{
  return 120 == id;
}
crit_e amf_status_ind_ies_o::get_crit(const uint32_t& id)
{
  if (id == 120) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e amf_status_ind_ies_o::get_presence(const uint32_t& id)
{
  if (id == 120) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE amf_status_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 120, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, unavailable_guami_list, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE amf_status_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &amf_status_ind_ies_o::get_crit));

    if (id == 120) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(unavailable_guami_list, bref, 1, 256, true); }));
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
void amf_status_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 120);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : unavailable_guami_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// BroadcastSessionModificationFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 348, 15, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool broadcast_session_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 348, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_mod_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 348:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_mod_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 348:
      return presence_e::optional;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_mod_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += mbs_session_mod_fail_transfer_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_session_mod_fail_transfer_present) {
    HANDLE_CODE(pack_ie(
        bref, 348, crit_e{crit_e::ignore}, [this, &bref]() { return mbs_session_mod_fail_transfer.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_mod_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_mod_fail_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 348: {
        mbs_session_mod_fail_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_mod_fail_transfer.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void broadcast_session_mod_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_session_mod_fail_transfer_present) {
    j.start_obj();
    j.write_int("id", 348);
    j.write_str("criticality", "ignore");
    j.write_str("value", mbs_session_mod_fail_transfer.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastSessionModificationRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 298, 349};
  return map_enum_number(names, 3, idx, "id");
}
bool broadcast_session_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 298, 349};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 298:
      return crit_e::reject;
    case 349:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 298:
      return presence_e::optional;
    case 349:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += mbs_service_area_present ? 1 : 0;
  nof_ies += mbs_session_mod_request_transfer_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_service_area_present) {
    HANDLE_CODE(pack_ie(bref, 298, crit_e{crit_e::reject}, [this, &bref]() { return mbs_service_area.pack(bref); }));
  }
  if (mbs_session_mod_request_transfer_present) {
    HANDLE_CODE(pack_ie(
        bref, 349, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_mod_request_transfer.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_mod_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 298: {
        mbs_service_area_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_service_area.unpack(bref); }));
        break;
      }
      case 349: {
        mbs_session_mod_request_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_mod_request_transfer.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void broadcast_session_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_service_area_present) {
    j.start_obj();
    j.write_int("id", 298);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    mbs_service_area.to_json(j);
    j.end_obj();
  }
  if (mbs_session_mod_request_transfer_present) {
    j.start_obj();
    j.write_int("id", 349);
    j.write_str("criticality", "reject");
    j.write_str("value", mbs_session_mod_request_transfer.to_string());
    j.end_obj();
  }
  j.end_array();
}

// BroadcastSessionModificationResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 350, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool broadcast_session_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 350, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 350:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 350:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += mbs_session_mod_resp_transfer_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_session_mod_resp_transfer_present) {
    HANDLE_CODE(pack_ie(
        bref, 350, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_mod_resp_transfer.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_mod_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 350: {
        mbs_session_mod_resp_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_mod_resp_transfer.unpack(bref); }));
        break;
      }
      case 19: {
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
void broadcast_session_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_session_mod_resp_transfer_present) {
    j.start_obj();
    j.write_int("id", 350);
    j.write_str("criticality", "reject");
    j.write_str("value", mbs_session_mod_resp_transfer.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastSessionReleaseRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 15};
  return map_enum_number(names, 2, idx, "id");
}
bool broadcast_session_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 15};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_release_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 15: {
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
void broadcast_session_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// BroadcastSessionReleaseRequiredIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_release_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 15};
  return map_enum_number(names, 2, idx, "id");
}
bool broadcast_session_release_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 15};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_release_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_release_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_release_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_release_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_release_required_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 15: {
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
void broadcast_session_release_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// BroadcastSessionReleaseResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_release_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 358, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool broadcast_session_release_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 358, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_release_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 358:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_release_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 358:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_release_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += mbs_session_release_resp_transfer_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_session_release_resp_transfer_present) {
    HANDLE_CODE(pack_ie(
        bref, 358, crit_e{crit_e::ignore}, [this, &bref]() { return mbs_session_release_resp_transfer.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_release_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_release_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 358: {
        mbs_session_release_resp_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_release_resp_transfer.unpack(bref); }));
        break;
      }
      case 19: {
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
void broadcast_session_release_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_session_release_resp_transfer_present) {
    j.start_obj();
    j.write_int("id", 358);
    j.write_str("criticality", "ignore");
    j.write_str("value", mbs_session_release_resp_transfer.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastSessionSetupFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 314, 15, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool broadcast_session_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 314, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 314:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 314:
      return presence_e::optional;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += mbs_session_setup_fail_transfer_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_session_setup_fail_transfer_present) {
    HANDLE_CODE(pack_ie(
        bref, 314, crit_e{crit_e::ignore}, [this, &bref]() { return mbs_session_setup_fail_transfer.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_setup_fail_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 314: {
        mbs_session_setup_fail_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_setup_fail_transfer.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void broadcast_session_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_session_setup_fail_transfer_present) {
    j.start_obj();
    j.write_int("id", 314);
    j.write_str("criticality", "ignore");
    j.write_str("value", mbs_session_setup_fail_transfer.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastSessionSetupRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 148, 298, 315, 415};
  return map_enum_number(names, 5, idx, "id");
}
bool broadcast_session_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 148, 298, 315, 415};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 148:
      return crit_e::reject;
    case 298:
      return crit_e::reject;
    case 315:
      return crit_e::reject;
    case 415:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 148:
      return presence_e::mandatory;
    case 298:
      return presence_e::mandatory;
    case 315:
      return presence_e::mandatory;
    case 415:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += associated_session_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 148, crit_e{crit_e::reject}, [this, &bref]() { return s_nssai.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 298, crit_e{crit_e::reject}, [this, &bref]() { return mbs_service_area.pack(bref); }));
  HANDLE_CODE(pack_ie(
      bref, 315, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_setup_request_transfer.pack(bref); }));
  if (associated_session_id_present) {
    HANDLE_CODE(
        pack_ie(bref, 415, crit_e{crit_e::ignore}, [this, &bref]() { return associated_session_id.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_setup_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 148: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_nssai.unpack(bref); }));
        break;
      }
      case 298: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_service_area.unpack(bref); }));
        break;
      }
      case 315: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_setup_request_transfer.unpack(bref); }));
        break;
      }
      case 415: {
        associated_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return associated_session_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void broadcast_session_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 148);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  s_nssai.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 298);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_service_area.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 315);
  j.write_str("criticality", "reject");
  j.write_str("value", mbs_session_setup_request_transfer.to_string());
  j.end_obj();
  if (associated_session_id_present) {
    j.start_obj();
    j.write_int("id", 415);
    j.write_str("criticality", "ignore");
    j.write_str("value", associated_session_id.to_string());
    j.end_obj();
  }
  j.end_array();
}

// BroadcastSessionSetupResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 316, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool broadcast_session_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 316, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 316:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 316:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += mbs_session_setup_resp_transfer_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_session_setup_resp_transfer_present) {
    HANDLE_CODE(pack_ie(
        bref, 316, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_setup_resp_transfer.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_setup_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 316: {
        mbs_session_setup_resp_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_setup_resp_transfer.unpack(bref); }));
        break;
      }
      case 19: {
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
void broadcast_session_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_session_setup_resp_transfer_present) {
    j.start_obj();
    j.write_int("id", 316);
    j.write_str("criticality", "reject");
    j.write_str("value", mbs_session_setup_resp_transfer.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastSessionTransportFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_transport_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 417, 15, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool broadcast_session_transport_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 417, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_transport_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 417:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_transport_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 417:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_transport_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(
      bref, 417, crit_e{crit_e::ignore}, [this, &bref]() { return broadcast_transport_fail_transfer.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_transport_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_transport_fail_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 417: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return broadcast_transport_fail_transfer.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void broadcast_session_transport_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 417);
  j.write_str("criticality", "ignore");
  j.write_str("value", broadcast_transport_fail_transfer.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// BroadcastSessionTransportRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_transport_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 418};
  return map_enum_number(names, 2, idx, "id");
}
bool broadcast_session_transport_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 418};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_transport_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 418:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_transport_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 418:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_transport_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(
      bref, 418, crit_e{crit_e::reject}, [this, &bref]() { return broadcast_transport_request_transfer.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_transport_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_transport_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 418: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return broadcast_transport_request_transfer.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void broadcast_session_transport_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 418);
  j.write_str("criticality", "reject");
  j.write_str("value", broadcast_transport_request_transfer.to_string());
  j.end_obj();
  j.end_array();
}

// BroadcastSessionTransportResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t broadcast_session_transport_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 419, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool broadcast_session_transport_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 419, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e broadcast_session_transport_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 419:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e broadcast_session_transport_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 419:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE broadcast_session_transport_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(
      bref, 419, crit_e{crit_e::reject}, [this, &bref]() { return broadcast_transport_resp_transfer.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE broadcast_session_transport_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &broadcast_session_transport_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 419: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return broadcast_transport_resp_transfer.unpack(bref); }));
        break;
      }
      case 19: {
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
void broadcast_session_transport_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 419);
  j.write_str("criticality", "reject");
  j.write_str("value", broadcast_transport_resp_transfer.to_string());
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// CellTrafficTraceIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t cell_traffic_trace_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 44, 43, 109, 256, 257};
  return map_enum_number(names, 7, idx, "id");
}
bool cell_traffic_trace_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 44, 43, 109, 256, 257};
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
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 44:
      return crit_e::ignore;
    case 43:
      return crit_e::ignore;
    case 109:
      return crit_e::ignore;
    case 256:
      return crit_e::ignore;
    case 257:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e cell_traffic_trace_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 44:
      return presence_e::mandatory;
    case 43:
      return presence_e::mandatory;
    case 109:
      return presence_e::mandatory;
    case 256:
      return presence_e::optional;
    case 257:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE cell_traffic_trace_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += privacy_ind_present ? 1 : 0;
  nof_ies += trace_collection_entity_uri_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 44, crit_e{crit_e::ignore}, [this, &bref]() { return ngran_trace_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 43, crit_e{crit_e::ignore}, [this, &bref]() { return ngran_cgi.pack(bref); }));
  HANDLE_CODE(pack_ie(
      bref, 109, crit_e{crit_e::ignore}, [this, &bref]() { return trace_collection_entity_ip_address.pack(bref); }));
  if (privacy_ind_present) {
    HANDLE_CODE(pack_ie(bref, 256, crit_e{crit_e::ignore}, [this, &bref]() { return privacy_ind.pack(bref); }));
  }
  if (trace_collection_entity_uri_present) {
    HANDLE_CODE(
        pack_ie(bref, 257, crit_e{crit_e::ignore}, [this, &bref]() { return trace_collection_entity_uri.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE cell_traffic_trace_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &cell_traffic_trace_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 44: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ngran_trace_id.unpack(bref); }));
        break;
      }
      case 43: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ngran_cgi.unpack(bref); }));
        break;
      }
      case 109: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_collection_entity_ip_address.unpack(bref); }));
        break;
      }
      case 256: {
        privacy_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return privacy_ind.unpack(bref); }));
        break;
      }
      case 257: {
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
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 44);
  j.write_str("criticality", "ignore");
  j.write_str("value", ngran_trace_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 43);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  ngran_cgi.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 109);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  trace_collection_entity_ip_address.to_json(j);
  j.end_obj();
  if (privacy_ind_present) {
    j.start_obj();
    j.write_int("id", 256);
    j.write_str("criticality", "ignore");
    j.write_str("value", privacy_ind.to_string());
    j.end_obj();
  }
  if (trace_collection_entity_uri_present) {
    j.start_obj();
    j.write_int("id", 257);
    j.write_str("criticality", "ignore");
    j.write_str("value", trace_collection_entity_uri.to_string());
    j.end_obj();
  }
  j.end_array();
}

// ConnectionEstablishmentIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t conn_establishment_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 117, 226, 148, 0, 209, 212, 210, 205, 222, 264, 34, 48, 414, 443};
  return map_enum_number(names, 16, idx, "id");
}
bool conn_establishment_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 117, 226, 148, 0, 209, 212, 210, 205, 222, 264, 34, 48, 414, 443};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e conn_establishment_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 117:
      return crit_e::ignore;
    case 226:
      return crit_e::ignore;
    case 148:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 209:
      return crit_e::ignore;
    case 212:
      return crit_e::ignore;
    case 210:
      return crit_e::ignore;
    case 205:
      return crit_e::ignore;
    case 222:
      return crit_e::ignore;
    case 264:
      return crit_e::reject;
    case 34:
      return crit_e::ignore;
    case 48:
      return crit_e::reject;
    case 414:
      return crit_e::ignore;
    case 443:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e conn_establishment_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 117:
      return presence_e::optional;
    case 226:
      return presence_e::optional;
    case 148:
      return presence_e::optional;
    case 0:
      return presence_e::optional;
    case 209:
      return presence_e::optional;
    case 212:
      return presence_e::optional;
    case 210:
      return presence_e::optional;
    case 205:
      return presence_e::optional;
    case 222:
      return presence_e::optional;
    case 264:
      return presence_e::optional;
    case 34:
      return presence_e::optional;
    case 48:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    case 443:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE conn_establishment_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ue_radio_cap_present ? 1 : 0;
  nof_ies += end_ind_present ? 1 : 0;
  nof_ies += s_nssai_present ? 1 : 0;
  nof_ies += allowed_nssai_present ? 1 : 0;
  nof_ies += ue_diff_info_present ? 1 : 0;
  nof_ies += dl_cp_security_info_present ? 1 : 0;
  nof_ies += nb_iot_ue_prio_present ? 1 : 0;
  nof_ies += enhanced_coverage_restrict_present ? 1 : 0;
  nof_ies += ce_mode_brestricted_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += old_amf_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  nof_ies += extended_old_amf_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ue_radio_cap_present) {
    HANDLE_CODE(pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap.pack(bref); }));
  }
  if (end_ind_present) {
    HANDLE_CODE(pack_ie(bref, 226, crit_e{crit_e::ignore}, [this, &bref]() { return end_ind.pack(bref); }));
  }
  if (s_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 148, crit_e{crit_e::ignore}, [this, &bref]() { return s_nssai.pack(bref); }));
  }
  if (allowed_nssai_present) {
    HANDLE_CODE(pack_ie(
        bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  }
  if (ue_diff_info_present) {
    HANDLE_CODE(pack_ie(bref, 209, crit_e{crit_e::ignore}, [this, &bref]() { return ue_diff_info.pack(bref); }));
  }
  if (dl_cp_security_info_present) {
    HANDLE_CODE(pack_ie(bref, 212, crit_e{crit_e::ignore}, [this, &bref]() { return dl_cp_security_info.pack(bref); }));
  }
  if (nb_iot_ue_prio_present) {
    HANDLE_CODE(pack_ie(bref, 210, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, nb_iot_ue_prio, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }
  if (enhanced_coverage_restrict_present) {
    HANDLE_CODE(
        pack_ie(bref, 205, crit_e{crit_e::ignore}, [this, &bref]() { return enhanced_coverage_restrict.pack(bref); }));
  }
  if (ce_mode_brestricted_present) {
    HANDLE_CODE(pack_ie(bref, 222, crit_e{crit_e::ignore}, [this, &bref]() { return ce_mode_brestricted.pack(bref); }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 34, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  if (old_amf_present) {
    HANDLE_CODE(pack_ie(bref, 48, crit_e{crit_e::reject}, [this, &bref]() { return old_amf.pack(bref); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }
  if (extended_old_amf_present) {
    HANDLE_CODE(pack_ie(bref, 443, crit_e{crit_e::ignore}, [this, &bref]() { return extended_old_amf.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE conn_establishment_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &conn_establishment_ind_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 117: {
        ue_radio_cap_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap.unpack(bref); }));
        break;
      }
      case 226: {
        end_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return end_ind.unpack(bref); }));
        break;
      }
      case 148: {
        s_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_nssai.unpack(bref); }));
        break;
      }
      case 0: {
        allowed_nssai_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 209: {
        ue_diff_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_diff_info.unpack(bref); }));
        break;
      }
      case 212: {
        dl_cp_security_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return dl_cp_security_info.unpack(bref); }));
        break;
      }
      case 210: {
        nb_iot_ue_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(nb_iot_ue_prio, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 205: {
        enhanced_coverage_restrict_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return enhanced_coverage_restrict.unpack(bref); }));
        break;
      }
      case 222: {
        ce_mode_brestricted_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ce_mode_brestricted.unpack(bref); }));
        break;
      }
      case 264: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 34: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 48: {
        old_amf_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return old_amf.unpack(bref); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 443: {
        extended_old_amf_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_old_amf.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void conn_establishment_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (ue_radio_cap_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_str("value", ue_radio_cap.to_string());
    j.end_obj();
  }
  if (end_ind_present) {
    j.start_obj();
    j.write_int("id", 226);
    j.write_str("criticality", "ignore");
    j.write_str("value", end_ind.to_string());
    j.end_obj();
  }
  if (s_nssai_present) {
    j.start_obj();
    j.write_int("id", 148);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    s_nssai.to_json(j);
    j.end_obj();
  }
  if (allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 0);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_diff_info_present) {
    j.start_obj();
    j.write_int("id", 209);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_diff_info.to_json(j);
    j.end_obj();
  }
  if (dl_cp_security_info_present) {
    j.start_obj();
    j.write_int("id", 212);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    dl_cp_security_info.to_json(j);
    j.end_obj();
  }
  if (nb_iot_ue_prio_present) {
    j.start_obj();
    j.write_int("id", 210);
    j.write_str("criticality", "ignore");
    j.write_int("value", nb_iot_ue_prio);
    j.end_obj();
  }
  if (enhanced_coverage_restrict_present) {
    j.start_obj();
    j.write_int("id", 205);
    j.write_str("criticality", "ignore");
    j.write_str("value", "restricted");
    j.end_obj();
  }
  if (ce_mode_brestricted_present) {
    j.start_obj();
    j.write_int("id", 222);
    j.write_str("criticality", "ignore");
    j.write_str("value", ce_mode_brestricted.to_string());
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 264);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 34);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  if (old_amf_present) {
    j.start_obj();
    j.write_int("id", 48);
    j.write_str("criticality", "reject");
    j.write_str("value", old_amf.to_string());
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (extended_old_amf_present) {
    j.start_obj();
    j.write_int("id", 443);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_old_amf.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DeactivateTraceIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t deactiv_trace_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 44};
  return map_enum_number(names, 3, idx, "id");
}
bool deactiv_trace_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 44};
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
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 44:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e deactiv_trace_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 44:
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

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 44, crit_e{crit_e::ignore}, [this, &bref]() { return ngran_trace_id.pack(bref); }));

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
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 44: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ngran_trace_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 44);
  j.write_str("criticality", "ignore");
  j.write_str("value", ngran_trace_id.to_string());
  j.end_obj();
  j.end_array();
}

// DistributionReleaseRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t distribution_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 295, 300, 15};
  return map_enum_number(names, 4, idx, "id");
}
bool distribution_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 295, 300, 15};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e distribution_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 295:
      return crit_e::reject;
    case 300:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e distribution_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 295:
      return presence_e::optional;
    case 300:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE distribution_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_ie(bref, 295, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 300, crit_e{crit_e::reject}, [this, &bref]() {
    return mbs_distribution_release_request_transfer.pack(bref);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE distribution_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &distribution_release_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 295: {
        mbs_area_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 300: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return mbs_distribution_release_request_transfer.unpack(bref); }));
        break;
      }
      case 15: {
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
void distribution_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 295);
    j.write_str("criticality", "reject");
    j.write_int("value", mbs_area_session_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 300);
  j.write_str("criticality", "reject");
  j.write_str("value", mbs_distribution_release_request_transfer.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// DistributionReleaseResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t distribution_release_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 295, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool distribution_release_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 295, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e distribution_release_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 295:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e distribution_release_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 295:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE distribution_release_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_ie(bref, 295, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE distribution_release_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &distribution_release_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 295: {
        mbs_area_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 19: {
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
void distribution_release_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 295);
    j.write_str("criticality", "reject");
    j.write_int("value", mbs_area_session_id);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DistributionSetupFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t distribution_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 295, 303, 15, 19};
  return map_enum_number(names, 5, idx, "id");
}
bool distribution_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 295, 303, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e distribution_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 295:
      return crit_e::reject;
    case 303:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e distribution_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 295:
      return presence_e::optional;
    case 303:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE distribution_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_ie(bref, 295, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 303, crit_e{crit_e::ignore}, [this, &bref]() {
    return mbs_distribution_setup_unsuccessful_transfer.pack(bref);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE distribution_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &distribution_setup_fail_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 295: {
        mbs_area_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 303: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return mbs_distribution_setup_unsuccessful_transfer.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void distribution_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 295);
    j.write_str("criticality", "reject");
    j.write_int("value", mbs_area_session_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 303);
  j.write_str("criticality", "ignore");
  j.write_str("value", mbs_distribution_setup_unsuccessful_transfer.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DistributionSetupRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t distribution_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 295, 301};
  return map_enum_number(names, 3, idx, "id");
}
bool distribution_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 295, 301};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e distribution_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 295:
      return crit_e::reject;
    case 301:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e distribution_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 295:
      return presence_e::optional;
    case 301:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE distribution_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_ie(bref, 295, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 301, crit_e{crit_e::reject}, [this, &bref]() {
    return mbs_distribution_setup_request_transfer.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE distribution_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &distribution_setup_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 295: {
        mbs_area_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 301: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return mbs_distribution_setup_request_transfer.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void distribution_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 295);
    j.write_str("criticality", "reject");
    j.write_int("value", mbs_area_session_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 301);
  j.write_str("criticality", "reject");
  j.write_str("value", mbs_distribution_setup_request_transfer.to_string());
  j.end_obj();
  j.end_array();
}

// DistributionSetupResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t distribution_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 295, 302, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool distribution_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 295, 302, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e distribution_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 295:
      return crit_e::reject;
    case 302:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e distribution_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 295:
      return presence_e::optional;
    case 302:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE distribution_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_ie(bref, 295, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(
      bref, 302, crit_e{crit_e::reject}, [this, &bref]() { return mbs_distribution_setup_resp_transfer.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE distribution_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &distribution_setup_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 295: {
        mbs_area_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 302: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return mbs_distribution_setup_resp_transfer.unpack(bref); }));
        break;
      }
      case 19: {
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
void distribution_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 295);
    j.write_str("criticality", "reject");
    j.write_int("value", mbs_area_session_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 302);
  j.write_str("criticality", "reject");
  j.write_str("value", mbs_distribution_setup_resp_transfer.to_string());
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DownlinkNASTransport-IEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t dl_nas_transport_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10,  85,  48,  83,  38,  36,  31,  110, 0,   177, 205, 206,
                                   209, 222, 117, 228, 226, 264, 334, 34,  414, 400, 443};
  return map_enum_number(names, 23, idx, "id");
}
bool dl_nas_transport_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10,  85,  48,  83,  38,  36,  31,  110, 0,   177, 205, 206,
                                   209, 222, 117, 228, 226, 264, 334, 34,  414, 400, 443};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dl_nas_transport_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 48:
      return crit_e::reject;
    case 83:
      return crit_e::ignore;
    case 38:
      return crit_e::reject;
    case 36:
      return crit_e::ignore;
    case 31:
      return crit_e::ignore;
    case 110:
      return crit_e::ignore;
    case 0:
      return crit_e::reject;
    case 177:
      return crit_e::ignore;
    case 205:
      return crit_e::ignore;
    case 206:
      return crit_e::ignore;
    case 209:
      return crit_e::ignore;
    case 222:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 228:
      return crit_e::ignore;
    case 226:
      return crit_e::ignore;
    case 264:
      return crit_e::reject;
    case 334:
      return crit_e::ignore;
    case 34:
      return crit_e::ignore;
    case 414:
      return crit_e::ignore;
    case 400:
      return crit_e::ignore;
    case 443:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dl_nas_transport_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 48:
      return presence_e::optional;
    case 83:
      return presence_e::optional;
    case 38:
      return presence_e::mandatory;
    case 36:
      return presence_e::optional;
    case 31:
      return presence_e::optional;
    case 110:
      return presence_e::optional;
    case 0:
      return presence_e::optional;
    case 177:
      return presence_e::optional;
    case 205:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 209:
      return presence_e::optional;
    case 222:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 228:
      return presence_e::optional;
    case 226:
      return presence_e::optional;
    case 264:
      return presence_e::optional;
    case 334:
      return presence_e::optional;
    case 34:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    case 400:
      return presence_e::optional;
    case 443:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dl_nas_transport_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += old_amf_present ? 1 : 0;
  nof_ies += ran_paging_prio_present ? 1 : 0;
  nof_ies += mob_restrict_list_present ? 1 : 0;
  nof_ies += idx_to_rfsp_present ? 1 : 0;
  nof_ies += ue_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += allowed_nssai_present ? 1 : 0;
  nof_ies += srvcc_operation_possible_present ? 1 : 0;
  nof_ies += enhanced_coverage_restrict_present ? 1 : 0;
  nof_ies += extended_connected_time_present ? 1 : 0;
  nof_ies += ue_diff_info_present ? 1 : 0;
  nof_ies += ce_mode_brestricted_present ? 1 : 0;
  nof_ies += ue_radio_cap_present ? 1 : 0;
  nof_ies += ue_cap_info_request_present ? 1 : 0;
  nof_ies += end_ind_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  nof_ies += target_nssai_info_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  nof_ies += mobile_iab_authorized_present ? 1 : 0;
  nof_ies += extended_old_amf_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (old_amf_present) {
    HANDLE_CODE(pack_ie(bref, 48, crit_e{crit_e::reject}, [this, &bref]() { return old_amf.pack(bref); }));
  }
  if (ran_paging_prio_present) {
    HANDLE_CODE(pack_ie(bref, 83, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ran_paging_prio, (uint16_t)1u, (uint16_t)256u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::reject}, [this, &bref]() { return nas_pdu.pack(bref); }));
  if (mob_restrict_list_present) {
    HANDLE_CODE(pack_ie(bref, 36, crit_e{crit_e::ignore}, [this, &bref]() { return mob_restrict_list.pack(bref); }));
  }
  if (idx_to_rfsp_present) {
    HANDLE_CODE(pack_ie(bref, 31, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, idx_to_rfsp, (uint16_t)1u, (uint16_t)256u, true, true);
    }));
  }
  if (ue_aggr_max_bit_rate_present) {
    HANDLE_CODE(
        pack_ie(bref, 110, crit_e{crit_e::ignore}, [this, &bref]() { return ue_aggr_max_bit_rate.pack(bref); }));
  }
  if (allowed_nssai_present) {
    HANDLE_CODE(pack_ie(
        bref, 0, crit_e{crit_e::reject}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  }
  if (srvcc_operation_possible_present) {
    HANDLE_CODE(
        pack_ie(bref, 177, crit_e{crit_e::ignore}, [this, &bref]() { return srvcc_operation_possible.pack(bref); }));
  }
  if (enhanced_coverage_restrict_present) {
    HANDLE_CODE(
        pack_ie(bref, 205, crit_e{crit_e::ignore}, [this, &bref]() { return enhanced_coverage_restrict.pack(bref); }));
  }
  if (extended_connected_time_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, extended_connected_time, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (ue_diff_info_present) {
    HANDLE_CODE(pack_ie(bref, 209, crit_e{crit_e::ignore}, [this, &bref]() { return ue_diff_info.pack(bref); }));
  }
  if (ce_mode_brestricted_present) {
    HANDLE_CODE(pack_ie(bref, 222, crit_e{crit_e::ignore}, [this, &bref]() { return ce_mode_brestricted.pack(bref); }));
  }
  if (ue_radio_cap_present) {
    HANDLE_CODE(pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap.pack(bref); }));
  }
  if (ue_cap_info_request_present) {
    HANDLE_CODE(pack_ie(bref, 228, crit_e{crit_e::ignore}, [this, &bref]() { return ue_cap_info_request.pack(bref); }));
  }
  if (end_ind_present) {
    HANDLE_CODE(pack_ie(bref, 226, crit_e{crit_e::ignore}, [this, &bref]() { return end_ind.pack(bref); }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }
  if (target_nssai_info_present) {
    HANDLE_CODE(pack_ie(bref, 334, crit_e{crit_e::ignore}, [this, &bref]() { return target_nssai_info.pack(bref); }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 34, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }
  if (mobile_iab_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 400, crit_e{crit_e::ignore}, [this, &bref]() { return mobile_iab_authorized.pack(bref); }));
  }
  if (extended_old_amf_present) {
    HANDLE_CODE(pack_ie(bref, 443, crit_e{crit_e::ignore}, [this, &bref]() { return extended_old_amf.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_nas_transport_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_nas_transport_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 48: {
        old_amf_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return old_amf.unpack(bref); }));
        break;
      }
      case 83: {
        ran_paging_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_paging_prio, bref, (uint16_t)1u, (uint16_t)256u, false, true);
        }));
        break;
      }
      case 38: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nas_pdu.unpack(bref); }));
        break;
      }
      case 36: {
        mob_restrict_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_restrict_list.unpack(bref); }));
        break;
      }
      case 31: {
        idx_to_rfsp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(idx_to_rfsp, bref, (uint16_t)1u, (uint16_t)256u, true, true);
        }));
        break;
      }
      case 110: {
        ue_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 0: {
        allowed_nssai_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 177: {
        srvcc_operation_possible_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srvcc_operation_possible.unpack(bref); }));
        break;
      }
      case 205: {
        enhanced_coverage_restrict_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return enhanced_coverage_restrict.unpack(bref); }));
        break;
      }
      case 206: {
        extended_connected_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(extended_connected_time, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 209: {
        ue_diff_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_diff_info.unpack(bref); }));
        break;
      }
      case 222: {
        ce_mode_brestricted_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ce_mode_brestricted.unpack(bref); }));
        break;
      }
      case 117: {
        ue_radio_cap_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap.unpack(bref); }));
        break;
      }
      case 228: {
        ue_cap_info_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_cap_info_request.unpack(bref); }));
        break;
      }
      case 226: {
        end_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return end_ind.unpack(bref); }));
        break;
      }
      case 264: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 334: {
        target_nssai_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_nssai_info.unpack(bref); }));
        break;
      }
      case 34: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 400: {
        mobile_iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_authorized.unpack(bref); }));
        break;
      }
      case 443: {
        extended_old_amf_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_old_amf.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void dl_nas_transport_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (old_amf_present) {
    j.start_obj();
    j.write_int("id", 48);
    j.write_str("criticality", "reject");
    j.write_str("value", old_amf.to_string());
    j.end_obj();
  }
  if (ran_paging_prio_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_int("value", ran_paging_prio);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 38);
  j.write_str("criticality", "reject");
  j.write_str("value", nas_pdu.to_string());
  j.end_obj();
  if (mob_restrict_list_present) {
    j.start_obj();
    j.write_int("id", 36);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_restrict_list.to_json(j);
    j.end_obj();
  }
  if (idx_to_rfsp_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_int("value", idx_to_rfsp);
    j.end_obj();
  }
  if (ue_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 110);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 0);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (srvcc_operation_possible_present) {
    j.start_obj();
    j.write_int("id", 177);
    j.write_str("criticality", "ignore");
    j.write_str("value", srvcc_operation_possible.to_string());
    j.end_obj();
  }
  if (enhanced_coverage_restrict_present) {
    j.start_obj();
    j.write_int("id", 205);
    j.write_str("criticality", "ignore");
    j.write_str("value", "restricted");
    j.end_obj();
  }
  if (extended_connected_time_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "ignore");
    j.write_int("value", extended_connected_time);
    j.end_obj();
  }
  if (ue_diff_info_present) {
    j.start_obj();
    j.write_int("id", 209);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_diff_info.to_json(j);
    j.end_obj();
  }
  if (ce_mode_brestricted_present) {
    j.start_obj();
    j.write_int("id", 222);
    j.write_str("criticality", "ignore");
    j.write_str("value", ce_mode_brestricted.to_string());
    j.end_obj();
  }
  if (ue_radio_cap_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_str("value", ue_radio_cap.to_string());
    j.end_obj();
  }
  if (ue_cap_info_request_present) {
    j.start_obj();
    j.write_int("id", 228);
    j.write_str("criticality", "ignore");
    j.write_str("value", "requested");
    j.end_obj();
  }
  if (end_ind_present) {
    j.start_obj();
    j.write_int("id", 226);
    j.write_str("criticality", "ignore");
    j.write_str("value", end_ind.to_string());
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 264);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  if (target_nssai_info_present) {
    j.start_obj();
    j.write_int("id", 334);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    target_nssai_info.to_json(j);
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 34);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (mobile_iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 400);
    j.write_str("criticality", "ignore");
    j.write_str("value", mobile_iab_authorized.to_string());
    j.end_obj();
  }
  if (extended_old_amf_present) {
    j.start_obj();
    j.write_int("id", 443);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_old_amf.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DownlinkNonUEAssociatedNRPPaTransportIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t dl_non_ue_associated_nrppa_transport_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {89, 46};
  return map_enum_number(names, 2, idx, "id");
}
bool dl_non_ue_associated_nrppa_transport_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {89, 46};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dl_non_ue_associated_nrppa_transport_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 89:
      return crit_e::reject;
    case 46:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dl_non_ue_associated_nrppa_transport_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 89:
      return presence_e::mandatory;
    case 46:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dl_non_ue_associated_nrppa_transport_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() { return routing_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 46, crit_e{crit_e::reject}, [this, &bref]() { return nrppa_pdu.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_non_ue_associated_nrppa_transport_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_non_ue_associated_nrppa_transport_ies_o::get_crit));

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return routing_id.unpack(bref); }));
        break;
      }
      case 46: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nrppa_pdu.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void dl_non_ue_associated_nrppa_transport_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_str("value", routing_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 46);
  j.write_str("criticality", "reject");
  j.write_str("value", nrppa_pdu.to_string());
  j.end_obj();
  j.end_array();
}

// DownlinkRANConfigurationTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t dl_ran_cfg_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {98, 157, 250};
  return map_enum_number(names, 3, idx, "id");
}
bool dl_ran_cfg_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {98, 157, 250};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dl_ran_cfg_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 98:
      return crit_e::ignore;
    case 157:
      return crit_e::ignore;
    case 250:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dl_ran_cfg_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 98:
      return presence_e::optional;
    case 157:
      return presence_e::optional;
    case 250:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dl_ran_cfg_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += son_cfg_transfer_dl_present ? 1 : 0;
  nof_ies += endc_son_cfg_transfer_dl_present ? 1 : 0;
  nof_ies += intersys_son_cfg_transfer_dl_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (son_cfg_transfer_dl_present) {
    HANDLE_CODE(pack_ie(bref, 98, crit_e{crit_e::ignore}, [this, &bref]() { return son_cfg_transfer_dl.pack(bref); }));
  }
  if (endc_son_cfg_transfer_dl_present) {
    HANDLE_CODE(
        pack_ie(bref, 157, crit_e{crit_e::ignore}, [this, &bref]() { return endc_son_cfg_transfer_dl.pack(bref); }));
  }
  if (intersys_son_cfg_transfer_dl_present) {
    HANDLE_CODE(pack_ie(
        bref, 250, crit_e{crit_e::ignore}, [this, &bref]() { return intersys_son_cfg_transfer_dl.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_ran_cfg_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_ran_cfg_transfer_ies_o::get_crit));

    switch (id) {
      case 98: {
        son_cfg_transfer_dl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return son_cfg_transfer_dl.unpack(bref); }));
        break;
      }
      case 157: {
        endc_son_cfg_transfer_dl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return endc_son_cfg_transfer_dl.unpack(bref); }));
        break;
      }
      case 250: {
        intersys_son_cfg_transfer_dl_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return intersys_son_cfg_transfer_dl.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void dl_ran_cfg_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (son_cfg_transfer_dl_present) {
    j.start_obj();
    j.write_int("id", 98);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    son_cfg_transfer_dl.to_json(j);
    j.end_obj();
  }
  if (endc_son_cfg_transfer_dl_present) {
    j.start_obj();
    j.write_int("id", 157);
    j.write_str("criticality", "ignore");
    j.write_str("value", endc_son_cfg_transfer_dl.to_string());
    j.end_obj();
  }
  if (intersys_son_cfg_transfer_dl_present) {
    j.start_obj();
    j.write_int("id", 250);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    intersys_son_cfg_transfer_dl.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DownlinkRANEarlyStatusTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t dl_ran_early_status_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 268};
  return map_enum_number(names, 3, idx, "id");
}
bool dl_ran_early_status_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 268};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dl_ran_early_status_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 268:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dl_ran_early_status_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 268:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dl_ran_early_status_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 268, crit_e{crit_e::reject}, [this, &bref]() {
    return early_status_transfer_transparent_container.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_ran_early_status_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_ran_early_status_transfer_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 268: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return early_status_transfer_transparent_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void dl_ran_early_status_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 268);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  early_status_transfer_transparent_container.to_json(j);
  j.end_obj();
  j.end_array();
}

// DownlinkRANStatusTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t dl_ran_status_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 84};
  return map_enum_number(names, 3, idx, "id");
}
bool dl_ran_status_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 84};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dl_ran_status_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 84:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dl_ran_status_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 84:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dl_ran_status_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 84, crit_e{crit_e::reject}, [this, &bref]() {
    return ran_status_transfer_transparent_container.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_ran_status_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_ran_status_transfer_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 84: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return ran_status_transfer_transparent_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void dl_ran_status_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 84);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ran_status_transfer_transparent_container.to_json(j);
  j.end_obj();
  j.end_array();
}

// DownlinkRIMInformationTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t dl_rim_info_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {175};
  return map_enum_number(names, 1, idx, "id");
}
bool dl_rim_info_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  return 175 == id;
}
crit_e dl_rim_info_transfer_ies_o::get_crit(const uint32_t& id)
{
  if (id == 175) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e dl_rim_info_transfer_ies_o::get_presence(const uint32_t& id)
{
  if (id == 175) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE dl_rim_info_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += rim_info_transfer_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (rim_info_transfer_present) {
    HANDLE_CODE(pack_ie(bref, 175, crit_e{crit_e::ignore}, [this, &bref]() { return rim_info_transfer.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_rim_info_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_rim_info_transfer_ies_o::get_crit));

    if (id == 175) {
      {
        rim_info_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rim_info_transfer.unpack(bref); }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void dl_rim_info_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (rim_info_transfer_present) {
    j.start_obj();
    j.write_int("id", 175);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    rim_info_transfer.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// DownlinkUEAssociatedNRPPaTransportIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t dl_ue_associated_nrppa_transport_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 89, 46};
  return map_enum_number(names, 4, idx, "id");
}
bool dl_ue_associated_nrppa_transport_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 89, 46};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e dl_ue_associated_nrppa_transport_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 89:
      return crit_e::reject;
    case 46:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e dl_ue_associated_nrppa_transport_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 89:
      return presence_e::mandatory;
    case 46:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE dl_ue_associated_nrppa_transport_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() { return routing_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 46, crit_e{crit_e::reject}, [this, &bref]() { return nrppa_pdu.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE dl_ue_associated_nrppa_transport_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &dl_ue_associated_nrppa_transport_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return routing_id.unpack(bref); }));
        break;
      }
      case 46: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nrppa_pdu.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void dl_ue_associated_nrppa_transport_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_str("value", routing_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 46);
  j.write_str("criticality", "reject");
  j.write_str("value", nrppa_pdu.to_string());
  j.end_obj();
  j.end_array();
}

// ErrorIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t error_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 15, 19, 26};
  return map_enum_number(names, 5, idx, "id");
}
bool error_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 15, 19, 26};
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
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    case 26:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e error_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::optional;
    case 85:
      return presence_e::optional;
    case 15:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    case 26:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE error_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += amf_ue_ngap_id_present ? 1 : 0;
  nof_ies += ran_ue_ngap_id_present ? 1 : 0;
  nof_ies += cause_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += five_g_s_tmsi_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (amf_ue_ngap_id_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
    }));
  }
  if (ran_ue_ngap_id_present) {
    HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
    }));
  }
  if (cause_present) {
    HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (five_g_s_tmsi_present) {
    HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_s_tmsi.pack(bref); }));
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
      case 10: {
        amf_ue_ngap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        ran_ue_ngap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
        cause_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 26: {
        five_g_s_tmsi_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_s_tmsi.unpack(bref); }));
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
  if (amf_ue_ngap_id_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_uint("value", amf_ue_ngap_id);
    j.end_obj();
  }
  if (ran_ue_ngap_id_present) {
    j.start_obj();
    j.write_int("id", 85);
    j.write_str("criticality", "ignore");
    j.write_uint("value", ran_ue_ngap_id);
    j.end_obj();
  }
  if (cause_present) {
    j.start_obj();
    j.write_int("id", 15);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cause.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (five_g_s_tmsi_present) {
    j.start_obj();
    j.write_int("id", 26);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_s_tmsi.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// HandoverCancelIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_cancel_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 15};
  return map_enum_number(names, 3, idx, "id");
}
bool ho_cancel_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 15};
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
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_cancel_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_cancel_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cancel_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_cancel_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
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
void ho_cancel_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// HandoverCancelAcknowledgeIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_cancel_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool ho_cancel_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_cancel_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_cancel_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_cancel_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cancel_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_cancel_ack_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 19: {
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
void ho_cancel_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// HandoverCommandIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 29, 39, 59, 78, 106, 19};
  return map_enum_number(names, 8, idx, "id");
}
bool ho_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 29, 39, 59, 78, 106, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 29:
      return crit_e::reject;
    case 39:
      return crit_e::reject;
    case 59:
      return crit_e::ignore;
    case 78:
      return crit_e::ignore;
    case 106:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 29:
      return presence_e::mandatory;
    case 39:
      return presence_e::conditional;
    case 59:
      return presence_e::optional;
    case 78:
      return presence_e::optional;
    case 106:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += nas_security_params_from_ngran_present ? 1 : 0;
  nof_ies += pdu_session_res_ho_list_present ? 1 : 0;
  nof_ies += pdu_session_res_to_release_list_ho_cmd_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::reject}, [this, &bref]() { return handov_type.pack(bref); }));
  if (nas_security_params_from_ngran_present) {
    HANDLE_CODE(pack_ie(
        bref, 39, crit_e{crit_e::reject}, [this, &bref]() { return nas_security_params_from_ngran.pack(bref); }));
  }
  if (pdu_session_res_ho_list_present) {
    HANDLE_CODE(pack_ie(bref, 59, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_ho_list, 1, 256, true);
    }));
  }
  if (pdu_session_res_to_release_list_ho_cmd_present) {
    HANDLE_CODE(pack_ie(bref, 78, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_to_release_list_ho_cmd, 1, 256, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 106, crit_e{crit_e::reject}, [this, &bref]() {
    return target_to_source_transparent_container.pack(bref);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_cmd_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return handov_type.unpack(bref); }));
        break;
      }
      case 39: {
        nas_security_params_from_ngran_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nas_security_params_from_ngran.unpack(bref); }));
        break;
      }
      case 59: {
        pdu_session_res_ho_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_ho_list, bref, 1, 256, true); }));
        break;
      }
      case 78: {
        pdu_session_res_to_release_list_ho_cmd_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_to_release_list_ho_cmd, bref, 1, 256, true);
        }));
        break;
      }
      case 106: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return target_to_source_transparent_container.unpack(bref); }));
        break;
      }
      case 19: {
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
void ho_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_str("value", handov_type.to_string());
  j.end_obj();
  if (nas_security_params_from_ngran_present) {
    j.start_obj();
    j.write_int("id", 39);
    j.write_str("criticality", "reject");
    j.write_str("value", nas_security_params_from_ngran.to_string());
    j.end_obj();
  }
  if (pdu_session_res_ho_list_present) {
    j.start_obj();
    j.write_int("id", 59);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_ho_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_to_release_list_ho_cmd_present) {
    j.start_obj();
    j.write_int("id", 78);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_to_release_list_ho_cmd) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 106);
  j.write_str("criticality", "reject");
  j.write_str("value", target_to_source_transparent_container.to_string());
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// HandoverFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 15, 19, 262};
  return map_enum_number(names, 4, idx, "id");
}
bool ho_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 15, 19, 262};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    case 262:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    case 262:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += targetto_source_fail_transparent_container_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (targetto_source_fail_transparent_container_present) {
    HANDLE_CODE(pack_ie(bref, 262, crit_e{crit_e::ignore}, [this, &bref]() {
      return targetto_source_fail_transparent_container.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_fail_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 262: {
        targetto_source_fail_transparent_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return targetto_source_fail_transparent_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (targetto_source_fail_transparent_container_present) {
    j.start_obj();
    j.write_int("id", 262);
    j.write_str("criticality", "ignore");
    j.write_str("value", targetto_source_fail_transparent_container.to_string());
    j.end_obj();
  }
  j.end_array();
}

// HandoverNotifyIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_notify_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 121, 269};
  return map_enum_number(names, 4, idx, "id");
}
bool ho_notify_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 121, 269};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_notify_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 121:
      return crit_e::ignore;
    case 269:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_notify_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 121:
      return presence_e::mandatory;
    case 269:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_notify_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += notify_source_ngran_node_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  if (notify_source_ngran_node_present) {
    HANDLE_CODE(
        pack_ie(bref, 269, crit_e{crit_e::ignore}, [this, &bref]() { return notify_source_ngran_node.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_notify_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_notify_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 121: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 269: {
        notify_source_ngran_node_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return notify_source_ngran_node.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_notify_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 121);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  user_location_info.to_json(j);
  j.end_obj();
  if (notify_source_ngran_node_present) {
    j.start_obj();
    j.write_int("id", 269);
    j.write_str("criticality", "ignore");
    j.write_str("value", "notifySource");
    j.end_obj();
  }
  j.end_array();
}

// HandoverPreparationFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_prep_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 15, 19, 262};
  return map_enum_number(names, 5, idx, "id");
}
bool ho_prep_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 15, 19, 262};
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
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    case 262:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_prep_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    case 262:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_prep_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += targetto_source_fail_transparent_container_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (targetto_source_fail_transparent_container_present) {
    HANDLE_CODE(pack_ie(bref, 262, crit_e{crit_e::ignore}, [this, &bref]() {
      return targetto_source_fail_transparent_container.pack(bref);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_prep_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_prep_fail_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 262: {
        targetto_source_fail_transparent_container_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return targetto_source_fail_transparent_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (targetto_source_fail_transparent_container_present) {
    j.start_obj();
    j.write_int("id", 262);
    j.write_str("criticality", "ignore");
    j.write_str("value", targetto_source_fail_transparent_container.to_string());
    j.end_obj();
  }
  j.end_array();
}

// HandoverRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10,  29,  15,  110, 18,  119, 93,  41,  37,  73,  0,   108, 34,  101, 36,  33,  91,
                                   28,  146, 165, 177, 199, 205, 209, 216, 215, 218, 217, 219, 222, 234, 254, 264, 206,
                                   326, 335, 345, 346, 347, 373, 374, 375, 376, 377, 378, 400, 403, 414, 430};
  return map_enum_number(names, 49, idx, "id");
}
bool ho_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10,  29,  15,  110, 18,  119, 93,  41,  37,  73,  0,   108, 34,  101, 36,  33,  91,
                                   28,  146, 165, 177, 199, 205, 209, 216, 215, 218, 217, 219, 222, 234, 254, 264, 206,
                                   326, 335, 345, 346, 347, 373, 374, 375, 376, 377, 378, 400, 403, 414, 430};
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
    case 10:
      return crit_e::reject;
    case 29:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    case 110:
      return crit_e::reject;
    case 18:
      return crit_e::ignore;
    case 119:
      return crit_e::reject;
    case 93:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 37:
      return crit_e::reject;
    case 73:
      return crit_e::reject;
    case 0:
      return crit_e::reject;
    case 108:
      return crit_e::ignore;
    case 34:
      return crit_e::ignore;
    case 101:
      return crit_e::reject;
    case 36:
      return crit_e::ignore;
    case 33:
      return crit_e::ignore;
    case 91:
      return crit_e::ignore;
    case 28:
      return crit_e::reject;
    case 146:
      return crit_e::ignore;
    case 165:
      return crit_e::ignore;
    case 177:
      return crit_e::ignore;
    case 199:
      return crit_e::reject;
    case 205:
      return crit_e::ignore;
    case 209:
      return crit_e::ignore;
    case 216:
      return crit_e::ignore;
    case 215:
      return crit_e::ignore;
    case 218:
      return crit_e::ignore;
    case 217:
      return crit_e::ignore;
    case 219:
      return crit_e::ignore;
    case 222:
      return crit_e::ignore;
    case 234:
      return crit_e::ignore;
    case 254:
      return crit_e::ignore;
    case 264:
      return crit_e::reject;
    case 206:
      return crit_e::ignore;
    case 326:
      return crit_e::ignore;
    case 335:
      return crit_e::ignore;
    case 345:
      return crit_e::ignore;
    case 346:
      return crit_e::ignore;
    case 347:
      return crit_e::ignore;
    case 373:
      return crit_e::ignore;
    case 374:
      return crit_e::ignore;
    case 375:
      return crit_e::ignore;
    case 376:
      return crit_e::ignore;
    case 377:
      return crit_e::ignore;
    case 378:
      return crit_e::ignore;
    case 400:
      return crit_e::ignore;
    case 403:
      return crit_e::ignore;
    case 414:
      return crit_e::ignore;
    case 430:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 29:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 110:
      return presence_e::mandatory;
    case 18:
      return presence_e::optional;
    case 119:
      return presence_e::mandatory;
    case 93:
      return presence_e::mandatory;
    case 41:
      return presence_e::optional;
    case 37:
      return presence_e::optional;
    case 73:
      return presence_e::mandatory;
    case 0:
      return presence_e::mandatory;
    case 108:
      return presence_e::optional;
    case 34:
      return presence_e::optional;
    case 101:
      return presence_e::mandatory;
    case 36:
      return presence_e::optional;
    case 33:
      return presence_e::optional;
    case 91:
      return presence_e::optional;
    case 28:
      return presence_e::mandatory;
    case 146:
      return presence_e::optional;
    case 165:
      return presence_e::optional;
    case 177:
      return presence_e::optional;
    case 199:
      return presence_e::optional;
    case 205:
      return presence_e::optional;
    case 209:
      return presence_e::optional;
    case 216:
      return presence_e::optional;
    case 215:
      return presence_e::optional;
    case 218:
      return presence_e::optional;
    case 217:
      return presence_e::optional;
    case 219:
      return presence_e::optional;
    case 222:
      return presence_e::optional;
    case 234:
      return presence_e::optional;
    case 254:
      return presence_e::optional;
    case 264:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 326:
      return presence_e::optional;
    case 335:
      return presence_e::optional;
    case 345:
      return presence_e::optional;
    case 346:
      return presence_e::optional;
    case 347:
      return presence_e::optional;
    case 373:
      return presence_e::optional;
    case 374:
      return presence_e::optional;
    case 375:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 377:
      return presence_e::optional;
    case 378:
      return presence_e::optional;
    case 400:
      return presence_e::optional;
    case 403:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    case 430:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 10;
  nof_ies += core_network_assist_info_for_inactive_present ? 1 : 0;
  nof_ies += new_security_context_ind_present ? 1 : 0;
  nof_ies += nasc_present ? 1 : 0;
  nof_ies += trace_activation_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += mob_restrict_list_present ? 1 : 0;
  nof_ies += location_report_request_type_present ? 1 : 0;
  nof_ies += rrc_inactive_transition_report_request_present ? 1 : 0;
  nof_ies += redirection_voice_fallback_present ? 1 : 0;
  nof_ies += cn_assisted_ran_tuning_present ? 1 : 0;
  nof_ies += srvcc_operation_possible_present ? 1 : 0;
  nof_ies += iab_authorized_present ? 1 : 0;
  nof_ies += enhanced_coverage_restrict_present ? 1 : 0;
  nof_ies += ue_diff_info_present ? 1 : 0;
  nof_ies += nr_v2x_services_authorized_present ? 1 : 0;
  nof_ies += ltev2x_services_authorized_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += pc5_qos_params_present ? 1 : 0;
  nof_ies += ce_mode_brestricted_present ? 1 : 0;
  nof_ies += ue_up_c_iot_support_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_list_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  nof_ies += extended_connected_time_present ? 1 : 0;
  nof_ies += time_sync_assist_info_present ? 1 : 0;
  nof_ies += ue_slice_max_bit_rate_list_present ? 1 : 0;
  nof_ies += five_g_pro_se_authorized_present ? 1 : 0;
  nof_ies += five_g_pro_se_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += five_g_pro_se_pc5_qos_params_present ? 1 : 0;
  nof_ies += aerial_ue_sub_info_present ? 1 : 0;
  nof_ies += nr_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += lte_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += nr_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += lte_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += a2_x_pc5_qos_params_present ? 1 : 0;
  nof_ies += mobile_iab_authorized_present ? 1 : 0;
  nof_ies += no_pdu_session_ind_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  nof_ies += sl_positioning_ranging_service_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::reject}, [this, &bref]() { return handov_type.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 110, crit_e{crit_e::reject}, [this, &bref]() { return ue_aggr_max_bit_rate.pack(bref); }));
  if (core_network_assist_info_for_inactive_present) {
    HANDLE_CODE(pack_ie(bref, 18, crit_e{crit_e::ignore}, [this, &bref]() {
      return core_network_assist_info_for_inactive.pack(bref);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 119, crit_e{crit_e::reject}, [this, &bref]() { return ue_security_cap.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 93, crit_e{crit_e::reject}, [this, &bref]() { return security_context.pack(bref); }));
  if (new_security_context_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() { return new_security_context_ind.pack(bref); }));
  }
  if (nasc_present) {
    HANDLE_CODE(pack_ie(bref, 37, crit_e{crit_e::reject}, [this, &bref]() { return nasc.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 73, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_setup_list_ho_req, 1, 256, true);
  }));
  HANDLE_CODE(pack_ie(
      bref, 0, crit_e{crit_e::reject}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  if (trace_activation_present) {
    HANDLE_CODE(pack_ie(bref, 108, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 34, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 101, crit_e{crit_e::reject}, [this, &bref]() {
    return source_to_target_transparent_container.pack(bref);
  }));
  if (mob_restrict_list_present) {
    HANDLE_CODE(pack_ie(bref, 36, crit_e{crit_e::ignore}, [this, &bref]() { return mob_restrict_list.pack(bref); }));
  }
  if (location_report_request_type_present) {
    HANDLE_CODE(
        pack_ie(bref, 33, crit_e{crit_e::ignore}, [this, &bref]() { return location_report_request_type.pack(bref); }));
  }
  if (rrc_inactive_transition_report_request_present) {
    HANDLE_CODE(pack_ie(bref, 91, crit_e{crit_e::ignore}, [this, &bref]() {
      return rrc_inactive_transition_report_request.pack(bref);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 28, crit_e{crit_e::reject}, [this, &bref]() { return guami.pack(bref); }));
  if (redirection_voice_fallback_present) {
    HANDLE_CODE(
        pack_ie(bref, 146, crit_e{crit_e::ignore}, [this, &bref]() { return redirection_voice_fallback.pack(bref); }));
  }
  if (cn_assisted_ran_tuning_present) {
    HANDLE_CODE(
        pack_ie(bref, 165, crit_e{crit_e::ignore}, [this, &bref]() { return cn_assisted_ran_tuning.pack(bref); }));
  }
  if (srvcc_operation_possible_present) {
    HANDLE_CODE(
        pack_ie(bref, 177, crit_e{crit_e::ignore}, [this, &bref]() { return srvcc_operation_possible.pack(bref); }));
  }
  if (iab_authorized_present) {
    HANDLE_CODE(pack_ie(bref, 199, crit_e{crit_e::reject}, [this, &bref]() { return iab_authorized.pack(bref); }));
  }
  if (enhanced_coverage_restrict_present) {
    HANDLE_CODE(
        pack_ie(bref, 205, crit_e{crit_e::ignore}, [this, &bref]() { return enhanced_coverage_restrict.pack(bref); }));
  }
  if (ue_diff_info_present) {
    HANDLE_CODE(pack_ie(bref, 209, crit_e{crit_e::ignore}, [this, &bref]() { return ue_diff_info.pack(bref); }));
  }
  if (nr_v2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 216, crit_e{crit_e::ignore}, [this, &bref]() { return nr_v2x_services_authorized.pack(bref); }));
  }
  if (ltev2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 215, crit_e{crit_e::ignore}, [this, &bref]() { return ltev2x_services_authorized.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 218, crit_e{crit_e::ignore}, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 217, crit_e{crit_e::ignore}, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 219, crit_e{crit_e::ignore}, [this, &bref]() { return pc5_qos_params.pack(bref); }));
  }
  if (ce_mode_brestricted_present) {
    HANDLE_CODE(pack_ie(bref, 222, crit_e{crit_e::ignore}, [this, &bref]() { return ce_mode_brestricted.pack(bref); }));
  }
  if (ue_up_c_iot_support_present) {
    HANDLE_CODE(pack_ie(bref, 234, crit_e{crit_e::ignore}, [this, &bref]() { return ue_up_c_iot_support.pack(bref); }));
  }
  if (management_based_mdt_plmn_list_present) {
    HANDLE_CODE(pack_ie(bref, 254, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_list, 1, 16, true);
    }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }
  if (extended_connected_time_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, extended_connected_time, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (time_sync_assist_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 326, crit_e{crit_e::ignore}, [this, &bref]() { return time_sync_assist_info.pack(bref); }));
  }
  if (ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 335, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }
  if (five_g_pro_se_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 345, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_authorized.pack(bref); }));
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(bref, 346, crit_e{crit_e::ignore}, [this, &bref]() {
      return five_g_pro_se_ue_pc5_aggr_max_bit_rate.pack(bref);
    }));
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(
        bref, 347, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_pc5_qos_params.pack(bref); }));
  }
  if (aerial_ue_sub_info_present) {
    HANDLE_CODE(pack_ie(bref, 373, crit_e{crit_e::ignore}, [this, &bref]() { return aerial_ue_sub_info.pack(bref); }));
  }
  if (nr_a2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 374, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_services_authorized.pack(bref); }));
  }
  if (lte_a2_x_services_authorized_present) {
    HANDLE_CODE(pack_ie(
        bref, 375, crit_e{crit_e::ignore}, [this, &bref]() { return lte_a2_x_services_authorized.pack(bref); }));
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 376, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (lte_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 377, crit_e{crit_e::ignore}, [this, &bref]() { return lte_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (a2_x_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 378, crit_e{crit_e::ignore}, [this, &bref]() { return a2_x_pc5_qos_params.pack(bref); }));
  }
  if (mobile_iab_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 400, crit_e{crit_e::ignore}, [this, &bref]() { return mobile_iab_authorized.pack(bref); }));
  }
  if (no_pdu_session_ind_present) {
    HANDLE_CODE(pack_ie(bref, 403, crit_e{crit_e::ignore}, [this, &bref]() { return no_pdu_session_ind.pack(bref); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }
  if (sl_positioning_ranging_service_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 430, crit_e{crit_e::ignore}, [this, &bref]() { return sl_positioning_ranging_service_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 10;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return handov_type.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 110: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 18: {
        core_network_assist_info_for_inactive_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return core_network_assist_info_for_inactive.unpack(bref); }));
        break;
      }
      case 119: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_security_cap.unpack(bref); }));
        break;
      }
      case 93: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return security_context.unpack(bref); }));
        break;
      }
      case 41: {
        new_security_context_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return new_security_context_ind.unpack(bref); }));
        break;
      }
      case 37: {
        nasc_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nasc.unpack(bref); }));
        break;
      }
      case 73: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_setup_list_ho_req, bref, 1, 256, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 108: {
        trace_activation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_activation.unpack(bref); }));
        break;
      }
      case 34: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 101: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return source_to_target_transparent_container.unpack(bref); }));
        break;
      }
      case 36: {
        mob_restrict_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_restrict_list.unpack(bref); }));
        break;
      }
      case 33: {
        location_report_request_type_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_report_request_type.unpack(bref); }));
        break;
      }
      case 91: {
        rrc_inactive_transition_report_request_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return rrc_inactive_transition_report_request.unpack(bref); }));
        break;
      }
      case 28: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return guami.unpack(bref); }));
        break;
      }
      case 146: {
        redirection_voice_fallback_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return redirection_voice_fallback.unpack(bref); }));
        break;
      }
      case 165: {
        cn_assisted_ran_tuning_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cn_assisted_ran_tuning.unpack(bref); }));
        break;
      }
      case 177: {
        srvcc_operation_possible_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srvcc_operation_possible.unpack(bref); }));
        break;
      }
      case 199: {
        iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_authorized.unpack(bref); }));
        break;
      }
      case 205: {
        enhanced_coverage_restrict_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return enhanced_coverage_restrict.unpack(bref); }));
        break;
      }
      case 209: {
        ue_diff_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_diff_info.unpack(bref); }));
        break;
      }
      case 216: {
        nr_v2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_v2x_services_authorized.unpack(bref); }));
        break;
      }
      case 215: {
        ltev2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltev2x_services_authorized.unpack(bref); }));
        break;
      }
      case 218: {
        nr_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 217: {
        lte_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 219: {
        pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pc5_qos_params.unpack(bref); }));
        break;
      }
      case 222: {
        ce_mode_brestricted_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ce_mode_brestricted.unpack(bref); }));
        break;
      }
      case 234: {
        ue_up_c_iot_support_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_up_c_iot_support.unpack(bref); }));
        break;
      }
      case 254: {
        management_based_mdt_plmn_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(management_based_mdt_plmn_list, bref, 1, 16, true); }));
        break;
      }
      case 264: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 206: {
        extended_connected_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(extended_connected_time, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 326: {
        time_sync_assist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_sync_assist_info.unpack(bref); }));
        break;
      }
      case 335: {
        ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      case 345: {
        five_g_pro_se_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_authorized.unpack(bref); }));
        break;
      }
      case 346: {
        five_g_pro_se_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 347: {
        five_g_pro_se_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 373: {
        aerial_ue_sub_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return aerial_ue_sub_info.unpack(bref); }));
        break;
      }
      case 374: {
        nr_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 375: {
        lte_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 376: {
        nr_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 377: {
        lte_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 378: {
        a2_x_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return a2_x_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 400: {
        mobile_iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_authorized.unpack(bref); }));
        break;
      }
      case 403: {
        no_pdu_session_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return no_pdu_session_ind.unpack(bref); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 430: {
        sl_positioning_ranging_service_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sl_positioning_ranging_service_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_str("value", handov_type.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 110);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_aggr_max_bit_rate.to_json(j);
  j.end_obj();
  if (core_network_assist_info_for_inactive_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    core_network_assist_info_for_inactive.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 119);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_security_cap.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 93);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  security_context.to_json(j);
  j.end_obj();
  if (new_security_context_ind_present) {
    j.start_obj();
    j.write_int("id", 41);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (nasc_present) {
    j.start_obj();
    j.write_int("id", 37);
    j.write_str("criticality", "reject");
    j.write_str("value", nasc.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 73);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_setup_list_ho_req) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : allowed_nssai) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (trace_activation_present) {
    j.start_obj();
    j.write_int("id", 108);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    trace_activation.to_json(j);
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 34);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 101);
  j.write_str("criticality", "reject");
  j.write_str("value", source_to_target_transparent_container.to_string());
  j.end_obj();
  if (mob_restrict_list_present) {
    j.start_obj();
    j.write_int("id", 36);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_restrict_list.to_json(j);
    j.end_obj();
  }
  if (location_report_request_type_present) {
    j.start_obj();
    j.write_int("id", 33);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    location_report_request_type.to_json(j);
    j.end_obj();
  }
  if (rrc_inactive_transition_report_request_present) {
    j.start_obj();
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_inactive_transition_report_request.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 28);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  guami.to_json(j);
  j.end_obj();
  if (redirection_voice_fallback_present) {
    j.start_obj();
    j.write_int("id", 146);
    j.write_str("criticality", "ignore");
    j.write_str("value", redirection_voice_fallback.to_string());
    j.end_obj();
  }
  if (cn_assisted_ran_tuning_present) {
    j.start_obj();
    j.write_int("id", 165);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cn_assisted_ran_tuning.to_json(j);
    j.end_obj();
  }
  if (srvcc_operation_possible_present) {
    j.start_obj();
    j.write_int("id", 177);
    j.write_str("criticality", "ignore");
    j.write_str("value", srvcc_operation_possible.to_string());
    j.end_obj();
  }
  if (iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 199);
    j.write_str("criticality", "reject");
    j.write_str("value", iab_authorized.to_string());
    j.end_obj();
  }
  if (enhanced_coverage_restrict_present) {
    j.start_obj();
    j.write_int("id", 205);
    j.write_str("criticality", "ignore");
    j.write_str("value", "restricted");
    j.end_obj();
  }
  if (ue_diff_info_present) {
    j.start_obj();
    j.write_int("id", 209);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_diff_info.to_json(j);
    j.end_obj();
  }
  if (nr_v2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 216);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_v2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltev2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 215);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltev2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 218);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 217);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 219);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (ce_mode_brestricted_present) {
    j.start_obj();
    j.write_int("id", 222);
    j.write_str("criticality", "ignore");
    j.write_str("value", ce_mode_brestricted.to_string());
    j.end_obj();
  }
  if (ue_up_c_iot_support_present) {
    j.start_obj();
    j.write_int("id", 234);
    j.write_str("criticality", "ignore");
    j.write_str("value", "supported");
    j.end_obj();
  }
  if (management_based_mdt_plmn_list_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 264);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  if (extended_connected_time_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "ignore");
    j.write_int("value", extended_connected_time);
    j.end_obj();
  }
  if (time_sync_assist_info_present) {
    j.start_obj();
    j.write_int("id", 326);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    time_sync_assist_info.to_json(j);
    j.end_obj();
  }
  if (ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 335);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (five_g_pro_se_authorized_present) {
    j.start_obj();
    j.write_int("id", 345);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_authorized.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 346);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 347);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (aerial_ue_sub_info_present) {
    j.start_obj();
    j.write_int("id", 373);
    j.write_str("criticality", "ignore");
    j.write_str("value", aerial_ue_sub_info.to_string());
    j.end_obj();
  }
  if (nr_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 374);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (lte_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 375);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (lte_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 377);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (a2_x_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 378);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    a2_x_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 400);
    j.write_str("criticality", "ignore");
    j.write_str("value", mobile_iab_authorized.to_string());
    j.end_obj();
  }
  if (no_pdu_session_ind_present) {
    j.start_obj();
    j.write_int("id", 403);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_positioning_ranging_service_info_present) {
    j.start_obj();
    j.write_int("id", 430);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sl_positioning_ranging_service_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// HandoverRequestAcknowledgeIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_request_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 53, 56, 106, 19, 259, 333, 427};
  return map_enum_number(names, 9, idx, "id");
}
bool ho_request_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 53, 56, 106, 19, 259, 333, 427};
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
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 53:
      return crit_e::ignore;
    case 56:
      return crit_e::ignore;
    case 106:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    case 259:
      return crit_e::reject;
    case 333:
      return crit_e::ignore;
    case 427:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_request_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 53:
      return presence_e::mandatory;
    case 56:
      return presence_e::optional;
    case 106:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    case 259:
      return presence_e::optional;
    case 333:
      return presence_e::optional;
    case 427:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_request_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += pdu_session_res_failed_to_setup_list_ho_ack_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += npn_access_info_present ? 1 : 0;
  nof_ies += red_cap_ind_present ? 1 : 0;
  nof_ies += e_red_cap_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 53, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_admitted_list, 1, 256, true);
  }));
  if (pdu_session_res_failed_to_setup_list_ho_ack_present) {
    HANDLE_CODE(pack_ie(bref, 56, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_setup_list_ho_ack, 1, 256, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 106, crit_e{crit_e::reject}, [this, &bref]() {
    return target_to_source_transparent_container.pack(bref);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (npn_access_info_present) {
    HANDLE_CODE(pack_ie(bref, 259, crit_e{crit_e::reject}, [this, &bref]() { return npn_access_info.pack(bref); }));
  }
  if (red_cap_ind_present) {
    HANDLE_CODE(pack_ie(bref, 333, crit_e{crit_e::ignore}, [this, &bref]() { return red_cap_ind.pack(bref); }));
  }
  if (e_red_cap_ind_present) {
    HANDLE_CODE(pack_ie(bref, 427, crit_e{crit_e::ignore}, [this, &bref]() { return e_red_cap_ind.pack(bref); }));
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
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 53: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_admitted_list, bref, 1, 256, true); }));
        break;
      }
      case 56: {
        pdu_session_res_failed_to_setup_list_ho_ack_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_setup_list_ho_ack, bref, 1, 256, true);
        }));
        break;
      }
      case 106: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return target_to_source_transparent_container.unpack(bref); }));
        break;
      }
      case 19: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 259: {
        npn_access_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return npn_access_info.unpack(bref); }));
        break;
      }
      case 333: {
        red_cap_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return red_cap_ind.unpack(bref); }));
        break;
      }
      case 427: {
        e_red_cap_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return e_red_cap_ind.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 53);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_admitted_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (pdu_session_res_failed_to_setup_list_ho_ack_present) {
    j.start_obj();
    j.write_int("id", 56);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_setup_list_ho_ack) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 106);
  j.write_str("criticality", "reject");
  j.write_str("value", target_to_source_transparent_container.to_string());
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (npn_access_info_present) {
    j.start_obj();
    j.write_int("id", 259);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    npn_access_info.to_json(j);
    j.end_obj();
  }
  if (red_cap_ind_present) {
    j.start_obj();
    j.write_int("id", 333);
    j.write_str("criticality", "ignore");
    j.write_str("value", "redcap");
    j.end_obj();
  }
  if (e_red_cap_ind_present) {
    j.start_obj();
    j.write_int("id", 427);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// HandoverRequiredIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_required_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 29, 15, 105, 22, 61, 101};
  return map_enum_number(names, 8, idx, "id");
}
bool ho_required_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 29, 15, 105, 22, 61, 101};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ho_required_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 29:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    case 105:
      return crit_e::reject;
    case 22:
      return crit_e::ignore;
    case 61:
      return crit_e::reject;
    case 101:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_required_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 29:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 105:
      return presence_e::mandatory;
    case 22:
      return presence_e::optional;
    case 61:
      return presence_e::mandatory;
    case 101:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_required_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 7;
  nof_ies += direct_forwarding_path_availability_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 29, crit_e{crit_e::reject}, [this, &bref]() { return handov_type.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 105, crit_e{crit_e::reject}, [this, &bref]() { return target_id.pack(bref); }));
  if (direct_forwarding_path_availability_present) {
    HANDLE_CODE(pack_ie(
        bref, 22, crit_e{crit_e::ignore}, [this, &bref]() { return direct_forwarding_path_availability.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 61, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_list_ho_rqd, 1, 256, true);
  }));
  HANDLE_CODE(pack_ie(bref, 101, crit_e{crit_e::reject}, [this, &bref]() {
    return source_to_target_transparent_container.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_required_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 7;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_required_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 29: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return handov_type.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 105: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_id.unpack(bref); }));
        break;
      }
      case 22: {
        direct_forwarding_path_availability_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return direct_forwarding_path_availability.unpack(bref); }));
        break;
      }
      case 61: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_list_ho_rqd, bref, 1, 256, true); }));
        break;
      }
      case 101: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return source_to_target_transparent_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ho_required_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 29);
  j.write_str("criticality", "reject");
  j.write_str("value", handov_type.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 105);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  target_id.to_json(j);
  j.end_obj();
  if (direct_forwarding_path_availability_present) {
    j.start_obj();
    j.write_int("id", 22);
    j.write_str("criticality", "ignore");
    j.write_str("value", "direct-path-available");
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 61);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_list_ho_rqd) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 101);
  j.write_str("criticality", "reject");
  j.write_str("value", source_to_target_transparent_container.to_string());
  j.end_obj();
  j.end_array();
}

// HandoverSuccessIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ho_success_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85};
  return map_enum_number(names, 2, idx, "id");
}
bool ho_success_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85};
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
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ho_success_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ho_success_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ho_success_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ho_success_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
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
void ho_success_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.end_array();
}

// InitialContextSetupFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t init_context_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 132, 15, 19};
  return map_enum_number(names, 5, idx, "id");
}
bool init_context_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 132, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e init_context_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 132:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e init_context_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 132:
      return presence_e::optional;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE init_context_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pdu_session_res_failed_to_setup_list_cxt_fail_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_failed_to_setup_list_cxt_fail_present) {
    HANDLE_CODE(pack_ie(bref, 132, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_setup_list_cxt_fail, 1, 256, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE init_context_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &init_context_setup_fail_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 132: {
        pdu_session_res_failed_to_setup_list_cxt_fail_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_setup_list_cxt_fail, bref, 1, 256, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void init_context_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (pdu_session_res_failed_to_setup_list_cxt_fail_present) {
    j.start_obj();
    j.write_int("id", 132);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_setup_list_cxt_fail) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// InitialContextSetupRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t init_context_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10,  85,  48,  110, 18,  28,  71,  0,   119, 94,  108, 36,  117, 31,
                                   34,  38,  24,  91,  118, 146, 33,  165, 177, 199, 205, 206, 209, 216,
                                   215, 218, 217, 219, 222, 234, 238, 254, 264, 326, 328, 334, 335, 345,
                                   346, 347, 367, 373, 374, 375, 376, 377, 378, 400, 414, 430, 443};
  return map_enum_number(names, 55, idx, "id");
}
bool init_context_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10,  85,  48,  110, 18,  28,  71,  0,   119, 94,  108, 36,  117, 31,
                                   34,  38,  24,  91,  118, 146, 33,  165, 177, 199, 205, 206, 209, 216,
                                   215, 218, 217, 219, 222, 234, 238, 254, 264, 326, 328, 334, 335, 345,
                                   346, 347, 367, 373, 374, 375, 376, 377, 378, 400, 414, 430, 443};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e init_context_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 48:
      return crit_e::reject;
    case 110:
      return crit_e::reject;
    case 18:
      return crit_e::ignore;
    case 28:
      return crit_e::reject;
    case 71:
      return crit_e::reject;
    case 0:
      return crit_e::reject;
    case 119:
      return crit_e::reject;
    case 94:
      return crit_e::reject;
    case 108:
      return crit_e::ignore;
    case 36:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 31:
      return crit_e::ignore;
    case 34:
      return crit_e::ignore;
    case 38:
      return crit_e::ignore;
    case 24:
      return crit_e::reject;
    case 91:
      return crit_e::ignore;
    case 118:
      return crit_e::ignore;
    case 146:
      return crit_e::ignore;
    case 33:
      return crit_e::ignore;
    case 165:
      return crit_e::ignore;
    case 177:
      return crit_e::ignore;
    case 199:
      return crit_e::ignore;
    case 205:
      return crit_e::ignore;
    case 206:
      return crit_e::ignore;
    case 209:
      return crit_e::ignore;
    case 216:
      return crit_e::ignore;
    case 215:
      return crit_e::ignore;
    case 218:
      return crit_e::ignore;
    case 217:
      return crit_e::ignore;
    case 219:
      return crit_e::ignore;
    case 222:
      return crit_e::ignore;
    case 234:
      return crit_e::ignore;
    case 238:
      return crit_e::ignore;
    case 254:
      return crit_e::ignore;
    case 264:
      return crit_e::reject;
    case 326:
      return crit_e::ignore;
    case 328:
      return crit_e::ignore;
    case 334:
      return crit_e::ignore;
    case 335:
      return crit_e::ignore;
    case 345:
      return crit_e::ignore;
    case 346:
      return crit_e::ignore;
    case 347:
      return crit_e::ignore;
    case 367:
      return crit_e::ignore;
    case 373:
      return crit_e::ignore;
    case 374:
      return crit_e::ignore;
    case 375:
      return crit_e::ignore;
    case 376:
      return crit_e::ignore;
    case 377:
      return crit_e::ignore;
    case 378:
      return crit_e::ignore;
    case 400:
      return crit_e::ignore;
    case 414:
      return crit_e::ignore;
    case 430:
      return crit_e::ignore;
    case 443:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e init_context_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 48:
      return presence_e::optional;
    case 110:
      return presence_e::conditional;
    case 18:
      return presence_e::optional;
    case 28:
      return presence_e::mandatory;
    case 71:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 119:
      return presence_e::mandatory;
    case 94:
      return presence_e::mandatory;
    case 108:
      return presence_e::optional;
    case 36:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 31:
      return presence_e::optional;
    case 34:
      return presence_e::optional;
    case 38:
      return presence_e::optional;
    case 24:
      return presence_e::optional;
    case 91:
      return presence_e::optional;
    case 118:
      return presence_e::optional;
    case 146:
      return presence_e::optional;
    case 33:
      return presence_e::optional;
    case 165:
      return presence_e::optional;
    case 177:
      return presence_e::optional;
    case 199:
      return presence_e::optional;
    case 205:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 209:
      return presence_e::optional;
    case 216:
      return presence_e::optional;
    case 215:
      return presence_e::optional;
    case 218:
      return presence_e::optional;
    case 217:
      return presence_e::optional;
    case 219:
      return presence_e::optional;
    case 222:
      return presence_e::optional;
    case 234:
      return presence_e::optional;
    case 238:
      return presence_e::optional;
    case 254:
      return presence_e::optional;
    case 264:
      return presence_e::optional;
    case 326:
      return presence_e::optional;
    case 328:
      return presence_e::optional;
    case 334:
      return presence_e::optional;
    case 335:
      return presence_e::optional;
    case 345:
      return presence_e::optional;
    case 346:
      return presence_e::optional;
    case 347:
      return presence_e::optional;
    case 367:
      return presence_e::optional;
    case 373:
      return presence_e::optional;
    case 374:
      return presence_e::optional;
    case 375:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 377:
      return presence_e::optional;
    case 378:
      return presence_e::optional;
    case 400:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    case 430:
      return presence_e::optional;
    case 443:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE init_context_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 6;
  nof_ies += old_amf_present ? 1 : 0;
  nof_ies += ue_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += core_network_assist_info_for_inactive_present ? 1 : 0;
  nof_ies += pdu_session_res_setup_list_cxt_req_present ? 1 : 0;
  nof_ies += trace_activation_present ? 1 : 0;
  nof_ies += mob_restrict_list_present ? 1 : 0;
  nof_ies += ue_radio_cap_present ? 1 : 0;
  nof_ies += idx_to_rfsp_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += nas_pdu_present ? 1 : 0;
  nof_ies += emergency_fallback_ind_present ? 1 : 0;
  nof_ies += rrc_inactive_transition_report_request_present ? 1 : 0;
  nof_ies += ue_radio_cap_for_paging_present ? 1 : 0;
  nof_ies += redirection_voice_fallback_present ? 1 : 0;
  nof_ies += location_report_request_type_present ? 1 : 0;
  nof_ies += cn_assisted_ran_tuning_present ? 1 : 0;
  nof_ies += srvcc_operation_possible_present ? 1 : 0;
  nof_ies += iab_authorized_present ? 1 : 0;
  nof_ies += enhanced_coverage_restrict_present ? 1 : 0;
  nof_ies += extended_connected_time_present ? 1 : 0;
  nof_ies += ue_diff_info_present ? 1 : 0;
  nof_ies += nr_v2x_services_authorized_present ? 1 : 0;
  nof_ies += ltev2x_services_authorized_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += pc5_qos_params_present ? 1 : 0;
  nof_ies += ce_mode_brestricted_present ? 1 : 0;
  nof_ies += ue_up_c_iot_support_present ? 1 : 0;
  nof_ies += rg_level_wireline_access_characteristics_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_list_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  nof_ies += time_sync_assist_info_present ? 1 : 0;
  nof_ies += q_mcc_onfig_info_present ? 1 : 0;
  nof_ies += target_nssai_info_present ? 1 : 0;
  nof_ies += ue_slice_max_bit_rate_list_present ? 1 : 0;
  nof_ies += five_g_pro_se_authorized_present ? 1 : 0;
  nof_ies += five_g_pro_se_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += five_g_pro_se_pc5_qos_params_present ? 1 : 0;
  nof_ies += network_ctrl_repeater_authorized_present ? 1 : 0;
  nof_ies += aerial_ue_sub_info_present ? 1 : 0;
  nof_ies += nr_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += lte_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += nr_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += lte_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += a2_x_pc5_qos_params_present ? 1 : 0;
  nof_ies += mobile_iab_authorized_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  nof_ies += sl_positioning_ranging_service_info_present ? 1 : 0;
  nof_ies += extended_old_amf_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (old_amf_present) {
    HANDLE_CODE(pack_ie(bref, 48, crit_e{crit_e::reject}, [this, &bref]() { return old_amf.pack(bref); }));
  }
  if (ue_aggr_max_bit_rate_present) {
    HANDLE_CODE(
        pack_ie(bref, 110, crit_e{crit_e::reject}, [this, &bref]() { return ue_aggr_max_bit_rate.pack(bref); }));
  }
  if (core_network_assist_info_for_inactive_present) {
    HANDLE_CODE(pack_ie(bref, 18, crit_e{crit_e::ignore}, [this, &bref]() {
      return core_network_assist_info_for_inactive.pack(bref);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 28, crit_e{crit_e::reject}, [this, &bref]() { return guami.pack(bref); }));
  if (pdu_session_res_setup_list_cxt_req_present) {
    HANDLE_CODE(pack_ie(bref, 71, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_setup_list_cxt_req, 1, 256, true);
    }));
  }
  HANDLE_CODE(pack_ie(
      bref, 0, crit_e{crit_e::reject}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  HANDLE_CODE(pack_ie(bref, 119, crit_e{crit_e::reject}, [this, &bref]() { return ue_security_cap.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 94, crit_e{crit_e::reject}, [this, &bref]() { return security_key.pack(bref); }));
  if (trace_activation_present) {
    HANDLE_CODE(pack_ie(bref, 108, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));
  }
  if (mob_restrict_list_present) {
    HANDLE_CODE(pack_ie(bref, 36, crit_e{crit_e::ignore}, [this, &bref]() { return mob_restrict_list.pack(bref); }));
  }
  if (ue_radio_cap_present) {
    HANDLE_CODE(pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap.pack(bref); }));
  }
  if (idx_to_rfsp_present) {
    HANDLE_CODE(pack_ie(bref, 31, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, idx_to_rfsp, (uint16_t)1u, (uint16_t)256u, true, true);
    }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 34, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  if (nas_pdu_present) {
    HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::ignore}, [this, &bref]() { return nas_pdu.pack(bref); }));
  }
  if (emergency_fallback_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 24, crit_e{crit_e::reject}, [this, &bref]() { return emergency_fallback_ind.pack(bref); }));
  }
  if (rrc_inactive_transition_report_request_present) {
    HANDLE_CODE(pack_ie(bref, 91, crit_e{crit_e::ignore}, [this, &bref]() {
      return rrc_inactive_transition_report_request.pack(bref);
    }));
  }
  if (ue_radio_cap_for_paging_present) {
    HANDLE_CODE(
        pack_ie(bref, 118, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap_for_paging.pack(bref); }));
  }
  if (redirection_voice_fallback_present) {
    HANDLE_CODE(
        pack_ie(bref, 146, crit_e{crit_e::ignore}, [this, &bref]() { return redirection_voice_fallback.pack(bref); }));
  }
  if (location_report_request_type_present) {
    HANDLE_CODE(
        pack_ie(bref, 33, crit_e{crit_e::ignore}, [this, &bref]() { return location_report_request_type.pack(bref); }));
  }
  if (cn_assisted_ran_tuning_present) {
    HANDLE_CODE(
        pack_ie(bref, 165, crit_e{crit_e::ignore}, [this, &bref]() { return cn_assisted_ran_tuning.pack(bref); }));
  }
  if (srvcc_operation_possible_present) {
    HANDLE_CODE(
        pack_ie(bref, 177, crit_e{crit_e::ignore}, [this, &bref]() { return srvcc_operation_possible.pack(bref); }));
  }
  if (iab_authorized_present) {
    HANDLE_CODE(pack_ie(bref, 199, crit_e{crit_e::ignore}, [this, &bref]() { return iab_authorized.pack(bref); }));
  }
  if (enhanced_coverage_restrict_present) {
    HANDLE_CODE(
        pack_ie(bref, 205, crit_e{crit_e::ignore}, [this, &bref]() { return enhanced_coverage_restrict.pack(bref); }));
  }
  if (extended_connected_time_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, extended_connected_time, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (ue_diff_info_present) {
    HANDLE_CODE(pack_ie(bref, 209, crit_e{crit_e::ignore}, [this, &bref]() { return ue_diff_info.pack(bref); }));
  }
  if (nr_v2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 216, crit_e{crit_e::ignore}, [this, &bref]() { return nr_v2x_services_authorized.pack(bref); }));
  }
  if (ltev2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 215, crit_e{crit_e::ignore}, [this, &bref]() { return ltev2x_services_authorized.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 218, crit_e{crit_e::ignore}, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 217, crit_e{crit_e::ignore}, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 219, crit_e{crit_e::ignore}, [this, &bref]() { return pc5_qos_params.pack(bref); }));
  }
  if (ce_mode_brestricted_present) {
    HANDLE_CODE(pack_ie(bref, 222, crit_e{crit_e::ignore}, [this, &bref]() { return ce_mode_brestricted.pack(bref); }));
  }
  if (ue_up_c_iot_support_present) {
    HANDLE_CODE(pack_ie(bref, 234, crit_e{crit_e::ignore}, [this, &bref]() { return ue_up_c_iot_support.pack(bref); }));
  }
  if (rg_level_wireline_access_characteristics_present) {
    HANDLE_CODE(pack_ie(bref, 238, crit_e{crit_e::ignore}, [this, &bref]() {
      return rg_level_wireline_access_characteristics.pack(bref);
    }));
  }
  if (management_based_mdt_plmn_list_present) {
    HANDLE_CODE(pack_ie(bref, 254, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_list, 1, 16, true);
    }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }
  if (time_sync_assist_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 326, crit_e{crit_e::ignore}, [this, &bref]() { return time_sync_assist_info.pack(bref); }));
  }
  if (q_mcc_onfig_info_present) {
    HANDLE_CODE(pack_ie(bref, 328, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_onfig_info.pack(bref); }));
  }
  if (target_nssai_info_present) {
    HANDLE_CODE(pack_ie(bref, 334, crit_e{crit_e::ignore}, [this, &bref]() { return target_nssai_info.pack(bref); }));
  }
  if (ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 335, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }
  if (five_g_pro_se_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 345, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_authorized.pack(bref); }));
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(bref, 346, crit_e{crit_e::ignore}, [this, &bref]() {
      return five_g_pro_se_ue_pc5_aggr_max_bit_rate.pack(bref);
    }));
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(
        bref, 347, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_pc5_qos_params.pack(bref); }));
  }
  if (network_ctrl_repeater_authorized_present) {
    HANDLE_CODE(pack_ie(
        bref, 367, crit_e{crit_e::ignore}, [this, &bref]() { return network_ctrl_repeater_authorized.pack(bref); }));
  }
  if (aerial_ue_sub_info_present) {
    HANDLE_CODE(pack_ie(bref, 373, crit_e{crit_e::ignore}, [this, &bref]() { return aerial_ue_sub_info.pack(bref); }));
  }
  if (nr_a2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 374, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_services_authorized.pack(bref); }));
  }
  if (lte_a2_x_services_authorized_present) {
    HANDLE_CODE(pack_ie(
        bref, 375, crit_e{crit_e::ignore}, [this, &bref]() { return lte_a2_x_services_authorized.pack(bref); }));
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 376, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (lte_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 377, crit_e{crit_e::ignore}, [this, &bref]() { return lte_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (a2_x_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 378, crit_e{crit_e::ignore}, [this, &bref]() { return a2_x_pc5_qos_params.pack(bref); }));
  }
  if (mobile_iab_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 400, crit_e{crit_e::ignore}, [this, &bref]() { return mobile_iab_authorized.pack(bref); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }
  if (sl_positioning_ranging_service_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 430, crit_e{crit_e::ignore}, [this, &bref]() { return sl_positioning_ranging_service_info.pack(bref); }));
  }
  if (extended_old_amf_present) {
    HANDLE_CODE(pack_ie(bref, 443, crit_e{crit_e::ignore}, [this, &bref]() { return extended_old_amf.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE init_context_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 6;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &init_context_setup_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 48: {
        old_amf_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return old_amf.unpack(bref); }));
        break;
      }
      case 110: {
        ue_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 18: {
        core_network_assist_info_for_inactive_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return core_network_assist_info_for_inactive.unpack(bref); }));
        break;
      }
      case 28: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return guami.unpack(bref); }));
        break;
      }
      case 71: {
        pdu_session_res_setup_list_cxt_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_setup_list_cxt_req, bref, 1, 256, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 119: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_security_cap.unpack(bref); }));
        break;
      }
      case 94: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return security_key.unpack(bref); }));
        break;
      }
      case 108: {
        trace_activation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return trace_activation.unpack(bref); }));
        break;
      }
      case 36: {
        mob_restrict_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mob_restrict_list.unpack(bref); }));
        break;
      }
      case 117: {
        ue_radio_cap_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap.unpack(bref); }));
        break;
      }
      case 31: {
        idx_to_rfsp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(idx_to_rfsp, bref, (uint16_t)1u, (uint16_t)256u, true, true);
        }));
        break;
      }
      case 34: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 38: {
        nas_pdu_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nas_pdu.unpack(bref); }));
        break;
      }
      case 24: {
        emergency_fallback_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return emergency_fallback_ind.unpack(bref); }));
        break;
      }
      case 91: {
        rrc_inactive_transition_report_request_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return rrc_inactive_transition_report_request.unpack(bref); }));
        break;
      }
      case 118: {
        ue_radio_cap_for_paging_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_for_paging.unpack(bref); }));
        break;
      }
      case 146: {
        redirection_voice_fallback_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return redirection_voice_fallback.unpack(bref); }));
        break;
      }
      case 33: {
        location_report_request_type_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_report_request_type.unpack(bref); }));
        break;
      }
      case 165: {
        cn_assisted_ran_tuning_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cn_assisted_ran_tuning.unpack(bref); }));
        break;
      }
      case 177: {
        srvcc_operation_possible_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srvcc_operation_possible.unpack(bref); }));
        break;
      }
      case 199: {
        iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_authorized.unpack(bref); }));
        break;
      }
      case 205: {
        enhanced_coverage_restrict_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return enhanced_coverage_restrict.unpack(bref); }));
        break;
      }
      case 206: {
        extended_connected_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(extended_connected_time, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 209: {
        ue_diff_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_diff_info.unpack(bref); }));
        break;
      }
      case 216: {
        nr_v2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_v2x_services_authorized.unpack(bref); }));
        break;
      }
      case 215: {
        ltev2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltev2x_services_authorized.unpack(bref); }));
        break;
      }
      case 218: {
        nr_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 217: {
        lte_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 219: {
        pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pc5_qos_params.unpack(bref); }));
        break;
      }
      case 222: {
        ce_mode_brestricted_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ce_mode_brestricted.unpack(bref); }));
        break;
      }
      case 234: {
        ue_up_c_iot_support_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_up_c_iot_support.unpack(bref); }));
        break;
      }
      case 238: {
        rg_level_wireline_access_characteristics_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return rg_level_wireline_access_characteristics.unpack(bref); }));
        break;
      }
      case 254: {
        management_based_mdt_plmn_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(management_based_mdt_plmn_list, bref, 1, 16, true); }));
        break;
      }
      case 264: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 326: {
        time_sync_assist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_sync_assist_info.unpack(bref); }));
        break;
      }
      case 328: {
        q_mcc_onfig_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_onfig_info.unpack(bref); }));
        break;
      }
      case 334: {
        target_nssai_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return target_nssai_info.unpack(bref); }));
        break;
      }
      case 335: {
        ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      case 345: {
        five_g_pro_se_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_authorized.unpack(bref); }));
        break;
      }
      case 346: {
        five_g_pro_se_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 347: {
        five_g_pro_se_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 367: {
        network_ctrl_repeater_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return network_ctrl_repeater_authorized.unpack(bref); }));
        break;
      }
      case 373: {
        aerial_ue_sub_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return aerial_ue_sub_info.unpack(bref); }));
        break;
      }
      case 374: {
        nr_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 375: {
        lte_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 376: {
        nr_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 377: {
        lte_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 378: {
        a2_x_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return a2_x_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 400: {
        mobile_iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_authorized.unpack(bref); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 430: {
        sl_positioning_ranging_service_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sl_positioning_ranging_service_info.unpack(bref); }));
        break;
      }
      case 443: {
        extended_old_amf_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_old_amf.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void init_context_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (old_amf_present) {
    j.start_obj();
    j.write_int("id", 48);
    j.write_str("criticality", "reject");
    j.write_str("value", old_amf.to_string());
    j.end_obj();
  }
  if (ue_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 110);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ue_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (core_network_assist_info_for_inactive_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    core_network_assist_info_for_inactive.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 28);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  guami.to_json(j);
  j.end_obj();
  if (pdu_session_res_setup_list_cxt_req_present) {
    j.start_obj();
    j.write_int("id", 71);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_setup_list_cxt_req) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : allowed_nssai) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 119);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_security_cap.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 94);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  security_key.to_json(j);
  j.end_obj();
  if (trace_activation_present) {
    j.start_obj();
    j.write_int("id", 108);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    trace_activation.to_json(j);
    j.end_obj();
  }
  if (mob_restrict_list_present) {
    j.start_obj();
    j.write_int("id", 36);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mob_restrict_list.to_json(j);
    j.end_obj();
  }
  if (ue_radio_cap_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_str("value", ue_radio_cap.to_string());
    j.end_obj();
  }
  if (idx_to_rfsp_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_int("value", idx_to_rfsp);
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 34);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  if (nas_pdu_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "ignore");
    j.write_str("value", nas_pdu.to_string());
    j.end_obj();
  }
  if (emergency_fallback_ind_present) {
    j.start_obj();
    j.write_int("id", 24);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    emergency_fallback_ind.to_json(j);
    j.end_obj();
  }
  if (rrc_inactive_transition_report_request_present) {
    j.start_obj();
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_inactive_transition_report_request.to_string());
    j.end_obj();
  }
  if (ue_radio_cap_for_paging_present) {
    j.start_obj();
    j.write_int("id", 118);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_radio_cap_for_paging.to_json(j);
    j.end_obj();
  }
  if (redirection_voice_fallback_present) {
    j.start_obj();
    j.write_int("id", 146);
    j.write_str("criticality", "ignore");
    j.write_str("value", redirection_voice_fallback.to_string());
    j.end_obj();
  }
  if (location_report_request_type_present) {
    j.start_obj();
    j.write_int("id", 33);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    location_report_request_type.to_json(j);
    j.end_obj();
  }
  if (cn_assisted_ran_tuning_present) {
    j.start_obj();
    j.write_int("id", 165);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cn_assisted_ran_tuning.to_json(j);
    j.end_obj();
  }
  if (srvcc_operation_possible_present) {
    j.start_obj();
    j.write_int("id", 177);
    j.write_str("criticality", "ignore");
    j.write_str("value", srvcc_operation_possible.to_string());
    j.end_obj();
  }
  if (iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 199);
    j.write_str("criticality", "ignore");
    j.write_str("value", iab_authorized.to_string());
    j.end_obj();
  }
  if (enhanced_coverage_restrict_present) {
    j.start_obj();
    j.write_int("id", 205);
    j.write_str("criticality", "ignore");
    j.write_str("value", "restricted");
    j.end_obj();
  }
  if (extended_connected_time_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "ignore");
    j.write_int("value", extended_connected_time);
    j.end_obj();
  }
  if (ue_diff_info_present) {
    j.start_obj();
    j.write_int("id", 209);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_diff_info.to_json(j);
    j.end_obj();
  }
  if (nr_v2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 216);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_v2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltev2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 215);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltev2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 218);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 217);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 219);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (ce_mode_brestricted_present) {
    j.start_obj();
    j.write_int("id", 222);
    j.write_str("criticality", "ignore");
    j.write_str("value", ce_mode_brestricted.to_string());
    j.end_obj();
  }
  if (ue_up_c_iot_support_present) {
    j.start_obj();
    j.write_int("id", 234);
    j.write_str("criticality", "ignore");
    j.write_str("value", "supported");
    j.end_obj();
  }
  if (rg_level_wireline_access_characteristics_present) {
    j.start_obj();
    j.write_int("id", 238);
    j.write_str("criticality", "ignore");
    j.write_str("value", rg_level_wireline_access_characteristics.to_string());
    j.end_obj();
  }
  if (management_based_mdt_plmn_list_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 264);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  if (time_sync_assist_info_present) {
    j.start_obj();
    j.write_int("id", 326);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    time_sync_assist_info.to_json(j);
    j.end_obj();
  }
  if (q_mcc_onfig_info_present) {
    j.start_obj();
    j.write_int("id", 328);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_onfig_info.to_json(j);
    j.end_obj();
  }
  if (target_nssai_info_present) {
    j.start_obj();
    j.write_int("id", 334);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    target_nssai_info.to_json(j);
    j.end_obj();
  }
  if (ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 335);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (five_g_pro_se_authorized_present) {
    j.start_obj();
    j.write_int("id", 345);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_authorized.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 346);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 347);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (network_ctrl_repeater_authorized_present) {
    j.start_obj();
    j.write_int("id", 367);
    j.write_str("criticality", "ignore");
    j.write_str("value", network_ctrl_repeater_authorized.to_string());
    j.end_obj();
  }
  if (aerial_ue_sub_info_present) {
    j.start_obj();
    j.write_int("id", 373);
    j.write_str("criticality", "ignore");
    j.write_str("value", aerial_ue_sub_info.to_string());
    j.end_obj();
  }
  if (nr_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 374);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (lte_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 375);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (lte_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 377);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (a2_x_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 378);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    a2_x_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 400);
    j.write_str("criticality", "ignore");
    j.write_str("value", mobile_iab_authorized.to_string());
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_positioning_ranging_service_info_present) {
    j.start_obj();
    j.write_int("id", 430);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sl_positioning_ranging_service_info.to_json(j);
    j.end_obj();
  }
  if (extended_old_amf_present) {
    j.start_obj();
    j.write_int("id", 443);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_old_amf.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// InitialContextSetupResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t init_context_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 72, 55, 19};
  return map_enum_number(names, 5, idx, "id");
}
bool init_context_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 72, 55, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e init_context_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 72:
      return crit_e::ignore;
    case 55:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e init_context_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 72:
      return presence_e::optional;
    case 55:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE init_context_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_res_setup_list_cxt_res_present ? 1 : 0;
  nof_ies += pdu_session_res_failed_to_setup_list_cxt_res_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_setup_list_cxt_res_present) {
    HANDLE_CODE(pack_ie(bref, 72, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_setup_list_cxt_res, 1, 256, true);
    }));
  }
  if (pdu_session_res_failed_to_setup_list_cxt_res_present) {
    HANDLE_CODE(pack_ie(bref, 55, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_setup_list_cxt_res, 1, 256, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE init_context_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &init_context_setup_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 72: {
        pdu_session_res_setup_list_cxt_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_setup_list_cxt_res, bref, 1, 256, true);
        }));
        break;
      }
      case 55: {
        pdu_session_res_failed_to_setup_list_cxt_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_setup_list_cxt_res, bref, 1, 256, true);
        }));
        break;
      }
      case 19: {
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
void init_context_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (pdu_session_res_setup_list_cxt_res_present) {
    j.start_obj();
    j.write_int("id", 72);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_setup_list_cxt_res) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_failed_to_setup_list_cxt_res_present) {
    j.start_obj();
    j.write_int("id", 55);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_setup_list_cxt_res) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// InitialUEMessage-IEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t init_ue_msg_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {85,  38,  121, 90,  26,  3,   112, 0,   171, 174, 201,
                                   224, 225, 227, 245, 259, 333, 371, 402, 414, 427, 440};
  return map_enum_number(names, 22, idx, "id");
}
bool init_ue_msg_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {85,  38,  121, 90,  26,  3,   112, 0,   171, 174, 201,
                                   224, 225, 227, 245, 259, 333, 371, 402, 414, 427, 440};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e init_ue_msg_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 85:
      return crit_e::reject;
    case 38:
      return crit_e::reject;
    case 121:
      return crit_e::reject;
    case 90:
      return crit_e::ignore;
    case 26:
      return crit_e::reject;
    case 3:
      return crit_e::ignore;
    case 112:
      return crit_e::ignore;
    case 0:
      return crit_e::reject;
    case 171:
      return crit_e::ignore;
    case 174:
      return crit_e::ignore;
    case 201:
      return crit_e::reject;
    case 224:
      return crit_e::reject;
    case 225:
      return crit_e::ignore;
    case 227:
      return crit_e::ignore;
    case 245:
      return crit_e::ignore;
    case 259:
      return crit_e::reject;
    case 333:
      return crit_e::ignore;
    case 371:
      return crit_e::ignore;
    case 402:
      return crit_e::reject;
    case 414:
      return crit_e::ignore;
    case 427:
      return crit_e::ignore;
    case 440:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e init_ue_msg_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 85:
      return presence_e::mandatory;
    case 38:
      return presence_e::mandatory;
    case 121:
      return presence_e::mandatory;
    case 90:
      return presence_e::mandatory;
    case 26:
      return presence_e::optional;
    case 3:
      return presence_e::optional;
    case 112:
      return presence_e::optional;
    case 0:
      return presence_e::optional;
    case 171:
      return presence_e::optional;
    case 174:
      return presence_e::optional;
    case 201:
      return presence_e::optional;
    case 224:
      return presence_e::optional;
    case 225:
      return presence_e::optional;
    case 227:
      return presence_e::optional;
    case 245:
      return presence_e::optional;
    case 259:
      return presence_e::optional;
    case 333:
      return presence_e::optional;
    case 371:
      return presence_e::optional;
    case 402:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    case 427:
      return presence_e::optional;
    case 440:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE init_ue_msg_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += five_g_s_tmsi_present ? 1 : 0;
  nof_ies += amf_set_id_present ? 1 : 0;
  nof_ies += ue_context_request_present ? 1 : 0;
  nof_ies += allowed_nssai_present ? 1 : 0;
  nof_ies += source_to_target_amf_info_reroute_present ? 1 : 0;
  nof_ies += sel_plmn_id_present ? 1 : 0;
  nof_ies += iab_node_ind_present ? 1 : 0;
  nof_ies += ce_mode_b_support_ind_present ? 1 : 0;
  nof_ies += ltem_ind_present ? 1 : 0;
  nof_ies += edt_session_present ? 1 : 0;
  nof_ies += authenticated_ind_present ? 1 : 0;
  nof_ies += npn_access_info_present ? 1 : 0;
  nof_ies += red_cap_ind_present ? 1 : 0;
  nof_ies += sel_n_id_present ? 1 : 0;
  nof_ies += mobile_iab_node_ind_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  nof_ies += e_red_cap_ind_present ? 1 : 0;
  nof_ies += aun3_dev_access_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::reject}, [this, &bref]() { return nas_pdu.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::reject}, [this, &bref]() { return user_location_info.pack(bref); }));
  HANDLE_CODE(
      pack_ie(bref, 90, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_establishment_cause.pack(bref); }));
  if (five_g_s_tmsi_present) {
    HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::reject}, [this, &bref]() { return five_g_s_tmsi.pack(bref); }));
  }
  if (amf_set_id_present) {
    HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::ignore}, [this, &bref]() { return amf_set_id.pack(bref); }));
  }
  if (ue_context_request_present) {
    HANDLE_CODE(pack_ie(bref, 112, crit_e{crit_e::ignore}, [this, &bref]() { return ue_context_request.pack(bref); }));
  }
  if (allowed_nssai_present) {
    HANDLE_CODE(pack_ie(
        bref, 0, crit_e{crit_e::reject}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  }
  if (source_to_target_amf_info_reroute_present) {
    HANDLE_CODE(pack_ie(
        bref, 171, crit_e{crit_e::ignore}, [this, &bref]() { return source_to_target_amf_info_reroute.pack(bref); }));
  }
  if (sel_plmn_id_present) {
    HANDLE_CODE(pack_ie(bref, 174, crit_e{crit_e::ignore}, [this, &bref]() { return sel_plmn_id.pack(bref); }));
  }
  if (iab_node_ind_present) {
    HANDLE_CODE(pack_ie(bref, 201, crit_e{crit_e::reject}, [this, &bref]() { return iab_node_ind.pack(bref); }));
  }
  if (ce_mode_b_support_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 224, crit_e{crit_e::reject}, [this, &bref]() { return ce_mode_b_support_ind.pack(bref); }));
  }
  if (ltem_ind_present) {
    HANDLE_CODE(pack_ie(bref, 225, crit_e{crit_e::ignore}, [this, &bref]() { return ltem_ind.pack(bref); }));
  }
  if (edt_session_present) {
    HANDLE_CODE(pack_ie(bref, 227, crit_e{crit_e::ignore}, [this, &bref]() { return edt_session.pack(bref); }));
  }
  if (authenticated_ind_present) {
    HANDLE_CODE(pack_ie(bref, 245, crit_e{crit_e::ignore}, [this, &bref]() { return authenticated_ind.pack(bref); }));
  }
  if (npn_access_info_present) {
    HANDLE_CODE(pack_ie(bref, 259, crit_e{crit_e::reject}, [this, &bref]() { return npn_access_info.pack(bref); }));
  }
  if (red_cap_ind_present) {
    HANDLE_CODE(pack_ie(bref, 333, crit_e{crit_e::ignore}, [this, &bref]() { return red_cap_ind.pack(bref); }));
  }
  if (sel_n_id_present) {
    HANDLE_CODE(pack_ie(bref, 371, crit_e{crit_e::ignore}, [this, &bref]() { return sel_n_id.pack(bref); }));
  }
  if (mobile_iab_node_ind_present) {
    HANDLE_CODE(pack_ie(bref, 402, crit_e{crit_e::reject}, [this, &bref]() { return mobile_iab_node_ind.pack(bref); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }
  if (e_red_cap_ind_present) {
    HANDLE_CODE(pack_ie(bref, 427, crit_e{crit_e::ignore}, [this, &bref]() { return e_red_cap_ind.pack(bref); }));
  }
  if (aun3_dev_access_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 440, crit_e{crit_e::ignore}, [this, &bref]() { return aun3_dev_access_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE init_ue_msg_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &init_ue_msg_ies_o::get_crit));

    switch (id) {
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 38: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nas_pdu.unpack(bref); }));
        break;
      }
      case 121: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 90: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_establishment_cause.unpack(bref); }));
        break;
      }
      case 26: {
        five_g_s_tmsi_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_s_tmsi.unpack(bref); }));
        break;
      }
      case 3: {
        amf_set_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return amf_set_id.unpack(bref); }));
        break;
      }
      case 112: {
        ue_context_request_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_context_request.unpack(bref); }));
        break;
      }
      case 0: {
        allowed_nssai_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 171: {
        source_to_target_amf_info_reroute_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_to_target_amf_info_reroute.unpack(bref); }));
        break;
      }
      case 174: {
        sel_plmn_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sel_plmn_id.unpack(bref); }));
        break;
      }
      case 201: {
        iab_node_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_node_ind.unpack(bref); }));
        break;
      }
      case 224: {
        ce_mode_b_support_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ce_mode_b_support_ind.unpack(bref); }));
        break;
      }
      case 225: {
        ltem_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltem_ind.unpack(bref); }));
        break;
      }
      case 227: {
        edt_session_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return edt_session.unpack(bref); }));
        break;
      }
      case 245: {
        authenticated_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return authenticated_ind.unpack(bref); }));
        break;
      }
      case 259: {
        npn_access_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return npn_access_info.unpack(bref); }));
        break;
      }
      case 333: {
        red_cap_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return red_cap_ind.unpack(bref); }));
        break;
      }
      case 371: {
        sel_n_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return sel_n_id.unpack(bref); }));
        break;
      }
      case 402: {
        mobile_iab_node_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_node_ind.unpack(bref); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 427: {
        e_red_cap_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return e_red_cap_ind.unpack(bref); }));
        break;
      }
      case 440: {
        aun3_dev_access_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return aun3_dev_access_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void init_ue_msg_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 38);
  j.write_str("criticality", "reject");
  j.write_str("value", nas_pdu.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 121);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  user_location_info.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 90);
  j.write_str("criticality", "ignore");
  j.write_str("value", rrc_establishment_cause.to_string());
  j.end_obj();
  if (five_g_s_tmsi_present) {
    j.start_obj();
    j.write_int("id", 26);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    five_g_s_tmsi.to_json(j);
    j.end_obj();
  }
  if (amf_set_id_present) {
    j.start_obj();
    j.write_int("id", 3);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    amf_set_id.to_json(j);
    j.end_obj();
  }
  if (ue_context_request_present) {
    j.start_obj();
    j.write_int("id", 112);
    j.write_str("criticality", "ignore");
    j.write_str("value", "requested");
    j.end_obj();
  }
  if (allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 0);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (source_to_target_amf_info_reroute_present) {
    j.start_obj();
    j.write_int("id", 171);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_to_target_amf_info_reroute.to_json(j);
    j.end_obj();
  }
  if (sel_plmn_id_present) {
    j.start_obj();
    j.write_int("id", 174);
    j.write_str("criticality", "ignore");
    j.write_str("value", sel_plmn_id.to_string());
    j.end_obj();
  }
  if (iab_node_ind_present) {
    j.start_obj();
    j.write_int("id", 201);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (ce_mode_b_support_ind_present) {
    j.start_obj();
    j.write_int("id", 224);
    j.write_str("criticality", "reject");
    j.write_str("value", "supported");
    j.end_obj();
  }
  if (ltem_ind_present) {
    j.start_obj();
    j.write_int("id", 225);
    j.write_str("criticality", "ignore");
    j.write_str("value", "lte-m");
    j.end_obj();
  }
  if (edt_session_present) {
    j.start_obj();
    j.write_int("id", 227);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (authenticated_ind_present) {
    j.start_obj();
    j.write_int("id", 245);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (npn_access_info_present) {
    j.start_obj();
    j.write_int("id", 259);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    npn_access_info.to_json(j);
    j.end_obj();
  }
  if (red_cap_ind_present) {
    j.start_obj();
    j.write_int("id", 333);
    j.write_str("criticality", "ignore");
    j.write_str("value", "redcap");
    j.end_obj();
  }
  if (sel_n_id_present) {
    j.start_obj();
    j.write_int("id", 371);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sel_n_id.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_node_ind_present) {
    j.start_obj();
    j.write_int("id", 402);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (e_red_cap_ind_present) {
    j.start_obj();
    j.write_int("id", 427);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (aun3_dev_access_info_present) {
    j.start_obj();
    j.write_int("id", 440);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    aun3_dev_access_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// LocationReportIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t location_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 121, 116, 33};
  return map_enum_number(names, 5, idx, "id");
}
bool location_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 121, 116, 33};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e location_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 121:
      return crit_e::ignore;
    case 116:
      return crit_e::ignore;
    case 33:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e location_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 121:
      return presence_e::mandatory;
    case 116:
      return presence_e::optional;
    case 33:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE location_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += ue_presence_in_area_of_interest_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  if (ue_presence_in_area_of_interest_list_present) {
    HANDLE_CODE(pack_ie(bref, 116, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_presence_in_area_of_interest_list, 1, 64, true);
    }));
  }
  HANDLE_CODE(
      pack_ie(bref, 33, crit_e{crit_e::ignore}, [this, &bref]() { return location_report_request_type.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &location_report_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 121: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 116: {
        ue_presence_in_area_of_interest_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(ue_presence_in_area_of_interest_list, bref, 1, 64, true);
        }));
        break;
      }
      case 33: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_report_request_type.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void location_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 121);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  user_location_info.to_json(j);
  j.end_obj();
  if (ue_presence_in_area_of_interest_list_present) {
    j.start_obj();
    j.write_int("id", 116);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_presence_in_area_of_interest_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 33);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  location_report_request_type.to_json(j);
  j.end_obj();
  j.end_array();
}

// LocationReportingControlIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t location_report_ctrl_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 33};
  return map_enum_number(names, 3, idx, "id");
}
bool location_report_ctrl_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 33};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e location_report_ctrl_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 33:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e location_report_ctrl_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 33:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE location_report_ctrl_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(
      pack_ie(bref, 33, crit_e{crit_e::ignore}, [this, &bref]() { return location_report_request_type.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_report_ctrl_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &location_report_ctrl_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 33: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return location_report_request_type.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void location_report_ctrl_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 33);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  location_report_request_type.to_json(j);
  j.end_obj();
  j.end_array();
}

// LocationReportingFailureIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t location_report_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 15};
  return map_enum_number(names, 3, idx, "id");
}
bool location_report_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 15};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e location_report_fail_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e location_report_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE location_report_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE location_report_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &location_report_fail_ind_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
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
void location_report_fail_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// MTCommunicationHandlingFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t mt_communication_handling_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 15, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool mt_communication_handling_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mt_communication_handling_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mt_communication_handling_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mt_communication_handling_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mt_communication_handling_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mt_communication_handling_fail_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void mt_communication_handling_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MTCommunicationHandlingRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t mt_communication_handling_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 406, 431};
  return map_enum_number(names, 4, idx, "id");
}
bool mt_communication_handling_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 406, 431};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mt_communication_handling_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 406:
      return crit_e::reject;
    case 431:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mt_communication_handling_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 406:
      return presence_e::mandatory;
    case 431:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mt_communication_handling_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 406, crit_e{crit_e::reject}, [this, &bref]() { return five_gc_action.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 431, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_list_mt_comm_h_req, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mt_communication_handling_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mt_communication_handling_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 406: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_gc_action.unpack(bref); }));
        break;
      }
      case 431: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_list_mt_comm_h_req, bref, 1, 256, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void mt_communication_handling_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 406);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  five_gc_action.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 431);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_list_mt_comm_h_req) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// MTCommunicationHandlingResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t mt_communication_handling_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool mt_communication_handling_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e mt_communication_handling_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e mt_communication_handling_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE mt_communication_handling_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE mt_communication_handling_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &mt_communication_handling_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 19: {
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
void mt_communication_handling_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastGroupPagingIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_group_paging_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 298, 307};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_group_paging_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 298, 307};
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
    case 299:
      return crit_e::ignore;
    case 298:
      return crit_e::ignore;
    case 307:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_group_paging_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 298:
      return presence_e::optional;
    case 307:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_group_paging_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += mbs_service_area_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::ignore}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_service_area_present) {
    HANDLE_CODE(pack_ie(bref, 298, crit_e{crit_e::ignore}, [this, &bref]() { return mbs_service_area.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 307, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, multicast_group_paging_area_list, 1, 64, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_group_paging_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_group_paging_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 298: {
        mbs_service_area_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_service_area.unpack(bref); }));
        break;
      }
      case 307: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(multicast_group_paging_area_list, bref, 1, 64, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 299);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_service_area_present) {
    j.start_obj();
    j.write_int("id", 298);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    mbs_service_area.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 307);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : multicast_group_paging_area_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// MulticastSessionActivationFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_session_activation_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 15, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_session_activation_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_session_activation_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_session_activation_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_session_activation_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_session_activation_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_session_activation_fail_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void multicast_session_activation_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastSessionActivationRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_session_activation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 304};
  return map_enum_number(names, 2, idx, "id");
}
bool multicast_session_activation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 304};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_session_activation_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 304:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_session_activation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 304:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_session_activation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 304, crit_e{crit_e::reject}, [this, &bref]() {
    return multicast_session_activation_request_transfer.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_session_activation_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_session_activation_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 304: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return multicast_session_activation_request_transfer.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void multicast_session_activation_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 304);
  j.write_str("criticality", "reject");
  j.write_str("value", multicast_session_activation_request_transfer.to_string());
  j.end_obj();
  j.end_array();
}

// MulticastSessionActivationResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_session_activation_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 19};
  return map_enum_number(names, 2, idx, "id");
}
bool multicast_session_activation_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_session_activation_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_session_activation_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_session_activation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_session_activation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_session_activation_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 19: {
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
void multicast_session_activation_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastSessionDeactivationRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_session_deactivation_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 305};
  return map_enum_number(names, 2, idx, "id");
}
bool multicast_session_deactivation_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 305};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_session_deactivation_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 305:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_session_deactivation_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 305:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_session_deactivation_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 305, crit_e{crit_e::reject}, [this, &bref]() {
    return multicast_session_deactivation_request_transfer.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_session_deactivation_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_session_deactivation_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 305: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return multicast_session_deactivation_request_transfer.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void multicast_session_deactivation_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 305);
  j.write_str("criticality", "reject");
  j.write_str("value", multicast_session_deactivation_request_transfer.to_string());
  j.end_obj();
  j.end_array();
}

// MulticastSessionDeactivationResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_session_deactivation_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 19};
  return map_enum_number(names, 2, idx, "id");
}
bool multicast_session_deactivation_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_session_deactivation_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_session_deactivation_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_session_deactivation_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_session_deactivation_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_session_deactivation_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 19: {
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
void multicast_session_deactivation_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastSessionUpdateFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_session_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 295, 15, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool multicast_session_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 295, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_session_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 295:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_session_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 295:
      return presence_e::optional;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_session_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_ie(bref, 295, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_session_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_session_upd_fail_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 295: {
        mbs_area_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void multicast_session_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 295);
    j.write_str("criticality", "reject");
    j.write_int("value", mbs_area_session_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// MulticastSessionUpdateRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_session_upd_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 295, 306};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_session_upd_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 295, 306};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_session_upd_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 295:
      return crit_e::reject;
    case 306:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_session_upd_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 295:
      return presence_e::optional;
    case 306:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_session_upd_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_ie(bref, 295, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 306, crit_e{crit_e::reject}, [this, &bref]() {
    return multicast_session_upd_request_transfer.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_session_upd_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_session_upd_request_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 295: {
        mbs_area_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 306: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return multicast_session_upd_request_transfer.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void multicast_session_upd_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 295);
    j.write_str("criticality", "reject");
    j.write_int("value", mbs_area_session_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 306);
  j.write_str("criticality", "reject");
  j.write_str("value", multicast_session_upd_request_transfer.to_string());
  j.end_obj();
  j.end_array();
}

// MulticastSessionUpdateResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t multicast_session_upd_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {299, 295, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool multicast_session_upd_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {299, 295, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e multicast_session_upd_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 299:
      return crit_e::reject;
    case 295:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e multicast_session_upd_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 299:
      return presence_e::mandatory;
    case 295:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE multicast_session_upd_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += mbs_area_session_id_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 299, crit_e{crit_e::reject}, [this, &bref]() { return mbs_session_id.pack(bref); }));
  if (mbs_area_session_id_present) {
    HANDLE_CODE(pack_ie(bref, 295, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, mbs_area_session_id, (uint32_t)0u, (uint32_t)65535u, true, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE multicast_session_upd_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &multicast_session_upd_resp_ies_o::get_crit));

    switch (id) {
      case 299: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mbs_session_id.unpack(bref); }));
        break;
      }
      case 295: {
        mbs_area_session_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(mbs_area_session_id, bref, (uint32_t)0u, (uint32_t)65535u, true, true);
        }));
        break;
      }
      case 19: {
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
void multicast_session_upd_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 299);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  mbs_session_id.to_json(j);
  j.end_obj();
  if (mbs_area_session_id_present) {
    j.start_obj();
    j.write_int("id", 295);
    j.write_str("criticality", "reject");
    j.write_int("value", mbs_area_session_id);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// NASNonDeliveryIndication-IEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t nas_non_delivery_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 38, 15};
  return map_enum_number(names, 4, idx, "id");
}
bool nas_non_delivery_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 38, 15};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e nas_non_delivery_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 38:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e nas_non_delivery_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 38:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE nas_non_delivery_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::ignore}, [this, &bref]() { return nas_pdu.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE nas_non_delivery_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &nas_non_delivery_ind_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 38: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nas_pdu.unpack(bref); }));
        break;
      }
      case 15: {
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
void nas_non_delivery_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 38);
  j.write_str("criticality", "ignore");
  j.write_str("value", nas_pdu.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// NGResetIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ng_reset_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {15, 88};
  return map_enum_number(names, 2, idx, "id");
}
bool ng_reset_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {15, 88};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_reset_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 15:
      return crit_e::ignore;
    case 88:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_reset_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 15:
      return presence_e::mandatory;
    case 88:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_reset_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 88, crit_e{crit_e::reject}, [this, &bref]() { return reset_type.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_reset_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_reset_ies_o::get_crit));

    switch (id) {
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 88: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return reset_type.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ng_reset_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 88);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  reset_type.to_json(j);
  j.end_obj();
  j.end_array();
}

// NGResetAcknowledgeIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ng_reset_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {111, 19};
  return map_enum_number(names, 2, idx, "id");
}
bool ng_reset_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {111, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_reset_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 111:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_reset_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 111:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_reset_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += ue_associated_lc_ng_conn_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (ue_associated_lc_ng_conn_list_present) {
    HANDLE_CODE(pack_ie(bref, 111, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_associated_lc_ng_conn_list, 1, 65536, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_reset_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_reset_ack_ies_o::get_crit));

    switch (id) {
      case 111: {
        ue_associated_lc_ng_conn_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_associated_lc_ng_conn_list, bref, 1, 65536, true); }));
        break;
      }
      case 19: {
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
void ng_reset_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (ue_associated_lc_ng_conn_list_present) {
    j.start_obj();
    j.write_int("id", 111);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_associated_lc_ng_conn_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// NGSetupFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ng_setup_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {15, 107, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool ng_setup_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {15, 107, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_setup_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 15:
      return crit_e::ignore;
    case 107:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_setup_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 15:
      return presence_e::mandatory;
    case 107:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_setup_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 107, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_setup_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_setup_fail_ies_o::get_crit));

    switch (id) {
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 107: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
        break;
      }
      case 19: {
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
void ng_setup_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 107);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// NGSetupRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ng_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {27, 82, 102, 21, 147, 204, 273};
  return map_enum_number(names, 7, idx, "id");
}
bool ng_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {27, 82, 102, 21, 147, 204, 273};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 27:
      return crit_e::reject;
    case 82:
      return crit_e::ignore;
    case 102:
      return crit_e::reject;
    case 21:
      return crit_e::ignore;
    case 147:
      return crit_e::ignore;
    case 204:
      return crit_e::ignore;
    case 273:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 27:
      return presence_e::mandatory;
    case 82:
      return presence_e::optional;
    case 102:
      return presence_e::mandatory;
    case 21:
      return presence_e::mandatory;
    case 147:
      return presence_e::optional;
    case 204:
      return presence_e::optional;
    case 273:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ran_node_name_present ? 1 : 0;
  nof_ies += ue_retention_info_present ? 1 : 0;
  nof_ies += nb_iot_default_paging_drx_present ? 1 : 0;
  nof_ies += extended_ran_node_name_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::reject}, [this, &bref]() { return global_ran_node_id.pack(bref); }));
  if (ran_node_name_present) {
    HANDLE_CODE(pack_ie(bref, 82, crit_e{crit_e::ignore}, [this, &bref]() { return ran_node_name.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 102, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, supported_ta_list, 1, 256, true);
  }));
  HANDLE_CODE(pack_ie(bref, 21, crit_e{crit_e::ignore}, [this, &bref]() { return default_paging_drx.pack(bref); }));
  if (ue_retention_info_present) {
    HANDLE_CODE(pack_ie(bref, 147, crit_e{crit_e::ignore}, [this, &bref]() { return ue_retention_info.pack(bref); }));
  }
  if (nb_iot_default_paging_drx_present) {
    HANDLE_CODE(
        pack_ie(bref, 204, crit_e{crit_e::ignore}, [this, &bref]() { return nb_iot_default_paging_drx.pack(bref); }));
  }
  if (extended_ran_node_name_present) {
    HANDLE_CODE(
        pack_ie(bref, 273, crit_e{crit_e::ignore}, [this, &bref]() { return extended_ran_node_name.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_setup_request_ies_o::get_crit));

    switch (id) {
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ran_node_id.unpack(bref); }));
        break;
      }
      case 82: {
        ran_node_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_node_name.unpack(bref); }));
        break;
      }
      case 102: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(supported_ta_list, bref, 1, 256, true); }));
        break;
      }
      case 21: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return default_paging_drx.unpack(bref); }));
        break;
      }
      case 147: {
        ue_retention_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_retention_info.unpack(bref); }));
        break;
      }
      case 204: {
        nb_iot_default_paging_drx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nb_iot_default_paging_drx.unpack(bref); }));
        break;
      }
      case 273: {
        extended_ran_node_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_ran_node_name.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ng_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_ran_node_id.to_json(j);
  j.end_obj();
  if (ran_node_name_present) {
    j.start_obj();
    j.write_int("id", 82);
    j.write_str("criticality", "ignore");
    j.write_str("value", ran_node_name.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 102);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : supported_ta_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 21);
  j.write_str("criticality", "ignore");
  j.write_str("value", default_paging_drx.to_string());
  j.end_obj();
  if (ue_retention_info_present) {
    j.start_obj();
    j.write_int("id", 147);
    j.write_str("criticality", "ignore");
    j.write_str("value", "ues-retained");
    j.end_obj();
  }
  if (nb_iot_default_paging_drx_present) {
    j.start_obj();
    j.write_int("id", 204);
    j.write_str("criticality", "ignore");
    j.write_str("value", nb_iot_default_paging_drx.to_string());
    j.end_obj();
  }
  if (extended_ran_node_name_present) {
    j.start_obj();
    j.write_int("id", 273);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_ran_node_name.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// NGSetupResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ng_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {1, 96, 86, 80, 19, 147, 200, 274, 404};
  return map_enum_number(names, 9, idx, "id");
}
bool ng_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {1, 96, 86, 80, 19, 147, 200, 274, 404};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ng_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 1:
      return crit_e::reject;
    case 96:
      return crit_e::reject;
    case 86:
      return crit_e::ignore;
    case 80:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    case 147:
      return crit_e::ignore;
    case 200:
      return crit_e::ignore;
    case 274:
      return crit_e::ignore;
    case 404:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ng_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 1:
      return presence_e::mandatory;
    case 96:
      return presence_e::mandatory;
    case 86:
      return presence_e::mandatory;
    case 80:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    case 147:
      return presence_e::optional;
    case 200:
      return presence_e::optional;
    case 274:
      return presence_e::optional;
    case 404:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ng_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += ue_retention_info_present ? 1 : 0;
  nof_ies += iab_supported_present ? 1 : 0;
  nof_ies += extended_amf_name_present ? 1 : 0;
  nof_ies += mobile_iab_supported_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 1, crit_e{crit_e::reject}, [this, &bref]() { return amf_name.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 96, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, served_guami_list, 1, 256, true);
  }));
  HANDLE_CODE(pack_ie(bref, 86, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, relative_amf_capacity, (uint16_t)0u, (uint16_t)255u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 80, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, plmn_support_list, 1, 12, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (ue_retention_info_present) {
    HANDLE_CODE(pack_ie(bref, 147, crit_e{crit_e::ignore}, [this, &bref]() { return ue_retention_info.pack(bref); }));
  }
  if (iab_supported_present) {
    HANDLE_CODE(pack_ie(bref, 200, crit_e{crit_e::ignore}, [this, &bref]() { return iab_supported.pack(bref); }));
  }
  if (extended_amf_name_present) {
    HANDLE_CODE(pack_ie(bref, 274, crit_e{crit_e::ignore}, [this, &bref]() { return extended_amf_name.pack(bref); }));
  }
  if (mobile_iab_supported_present) {
    HANDLE_CODE(
        pack_ie(bref, 404, crit_e{crit_e::ignore}, [this, &bref]() { return mobile_iab_supported.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ng_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ng_setup_resp_ies_o::get_crit));

    switch (id) {
      case 1: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return amf_name.unpack(bref); }));
        break;
      }
      case 96: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(served_guami_list, bref, 1, 256, true); }));
        break;
      }
      case 86: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(relative_amf_capacity, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 80: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(plmn_support_list, bref, 1, 12, true); }));
        break;
      }
      case 19: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 147: {
        ue_retention_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_retention_info.unpack(bref); }));
        break;
      }
      case 200: {
        iab_supported_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_supported.unpack(bref); }));
        break;
      }
      case 274: {
        extended_amf_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_amf_name.unpack(bref); }));
        break;
      }
      case 404: {
        mobile_iab_supported_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_supported.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ng_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 1);
  j.write_str("criticality", "reject");
  j.write_str("value", amf_name.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 96);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : served_guami_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.start_obj();
  j.write_int("id", 86);
  j.write_str("criticality", "ignore");
  j.write_int("value", relative_amf_capacity);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 80);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : plmn_support_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (ue_retention_info_present) {
    j.start_obj();
    j.write_int("id", 147);
    j.write_str("criticality", "ignore");
    j.write_str("value", "ues-retained");
    j.end_obj();
  }
  if (iab_supported_present) {
    j.start_obj();
    j.write_int("id", 200);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (extended_amf_name_present) {
    j.start_obj();
    j.write_int("id", 274);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_amf_name.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_supported_present) {
    j.start_obj();
    j.write_int("id", 404);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// OverloadStartIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t overload_start_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {2, 9, 49};
  return map_enum_number(names, 3, idx, "id");
}
bool overload_start_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {2, 9, 49};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e overload_start_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 2:
      return crit_e::reject;
    case 9:
      return crit_e::ignore;
    case 49:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e overload_start_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 2:
      return presence_e::optional;
    case 9:
      return presence_e::optional;
    case 49:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE overload_start_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += amf_overload_resp_present ? 1 : 0;
  nof_ies += amf_traffic_load_reduction_ind_present ? 1 : 0;
  nof_ies += overload_start_nssai_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (amf_overload_resp_present) {
    HANDLE_CODE(pack_ie(bref, 2, crit_e{crit_e::reject}, [this, &bref]() { return amf_overload_resp.pack(bref); }));
  }
  if (amf_traffic_load_reduction_ind_present) {
    HANDLE_CODE(pack_ie(bref, 9, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, amf_traffic_load_reduction_ind, (uint8_t)1u, (uint8_t)99u, false, true);
    }));
  }
  if (overload_start_nssai_list_present) {
    HANDLE_CODE(pack_ie(bref, 49, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, overload_start_nssai_list, 1, 1024, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE overload_start_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &overload_start_ies_o::get_crit));

    switch (id) {
      case 2: {
        amf_overload_resp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return amf_overload_resp.unpack(bref); }));
        break;
      }
      case 9: {
        amf_traffic_load_reduction_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_traffic_load_reduction_ind, bref, (uint8_t)1u, (uint8_t)99u, false, true);
        }));
        break;
      }
      case 49: {
        overload_start_nssai_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(overload_start_nssai_list, bref, 1, 1024, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void overload_start_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (amf_overload_resp_present) {
    j.start_obj();
    j.write_int("id", 2);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    amf_overload_resp.to_json(j);
    j.end_obj();
  }
  if (amf_traffic_load_reduction_ind_present) {
    j.start_obj();
    j.write_int("id", 9);
    j.write_str("criticality", "ignore");
    j.write_int("value", amf_traffic_load_reduction_ind);
    j.end_obj();
  }
  if (overload_start_nssai_list_present) {
    j.start_obj();
    j.write_int("id", 49);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : overload_start_nssai_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionResourceModifyConfirmIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_modify_confirm_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 62, 131, 19};
  return map_enum_number(names, 5, idx, "id");
}
bool pdu_session_res_modify_confirm_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 62, 131, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_modify_confirm_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 62:
      return crit_e::ignore;
    case 131:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_modify_confirm_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 62:
      return presence_e::optional;
    case 131:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_modify_confirm_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_res_modify_list_mod_cfm_present ? 1 : 0;
  nof_ies += pdu_session_res_failed_to_modify_list_mod_cfm_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_modify_list_mod_cfm_present) {
    HANDLE_CODE(pack_ie(bref, 62, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_modify_list_mod_cfm, 1, 256, true);
    }));
  }
  if (pdu_session_res_failed_to_modify_list_mod_cfm_present) {
    HANDLE_CODE(pack_ie(bref, 131, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_modify_list_mod_cfm, 1, 256, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_modify_confirm_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_modify_confirm_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 62: {
        pdu_session_res_modify_list_mod_cfm_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_modify_list_mod_cfm, bref, 1, 256, true);
        }));
        break;
      }
      case 131: {
        pdu_session_res_failed_to_modify_list_mod_cfm_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_modify_list_mod_cfm, bref, 1, 256, true);
        }));
        break;
      }
      case 19: {
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
void pdu_session_res_modify_confirm_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (pdu_session_res_modify_list_mod_cfm_present) {
    j.start_obj();
    j.write_int("id", 62);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_modify_list_mod_cfm) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_failed_to_modify_list_mod_cfm_present) {
    j.start_obj();
    j.write_int("id", 131);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_modify_list_mod_cfm) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionResourceModifyIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_modify_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 63, 121};
  return map_enum_number(names, 4, idx, "id");
}
bool pdu_session_res_modify_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 63, 121};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_modify_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 63:
      return crit_e::reject;
    case 121:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_modify_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 63:
      return presence_e::mandatory;
    case 121:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_modify_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += user_location_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 63, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_modify_list_mod_ind, 1, 256, true);
  }));
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_modify_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_modify_ind_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 63: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_modify_list_mod_ind, bref, 1, 256, true);
        }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_modify_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 63);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_modify_list_mod_ind) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionResourceModifyRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_modify_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 83, 64};
  return map_enum_number(names, 4, idx, "id");
}
bool pdu_session_res_modify_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 83, 64};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_modify_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 83:
      return crit_e::ignore;
    case 64:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_modify_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 83:
      return presence_e::optional;
    case 64:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_modify_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ran_paging_prio_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ran_paging_prio_present) {
    HANDLE_CODE(pack_ie(bref, 83, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ran_paging_prio, (uint16_t)1u, (uint16_t)256u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 64, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_modify_list_mod_req, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_modify_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_modify_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 83: {
        ran_paging_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_paging_prio, bref, (uint16_t)1u, (uint16_t)256u, false, true);
        }));
        break;
      }
      case 64: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_modify_list_mod_req, bref, 1, 256, true);
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
void pdu_session_res_modify_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (ran_paging_prio_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_int("value", ran_paging_prio);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 64);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_modify_list_mod_req) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// PDUSessionResourceModifyResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_modify_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 65, 54, 121, 19};
  return map_enum_number(names, 6, idx, "id");
}
bool pdu_session_res_modify_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 65, 54, 121, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_modify_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 65:
      return crit_e::ignore;
    case 54:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_modify_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 65:
      return presence_e::optional;
    case 54:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_modify_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_res_modify_list_mod_res_present ? 1 : 0;
  nof_ies += pdu_session_res_failed_to_modify_list_mod_res_present ? 1 : 0;
  nof_ies += user_location_info_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_modify_list_mod_res_present) {
    HANDLE_CODE(pack_ie(bref, 65, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_modify_list_mod_res, 1, 256, true);
    }));
  }
  if (pdu_session_res_failed_to_modify_list_mod_res_present) {
    HANDLE_CODE(pack_ie(bref, 54, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_modify_list_mod_res, 1, 256, true);
    }));
  }
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_modify_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_modify_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 65: {
        pdu_session_res_modify_list_mod_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_modify_list_mod_res, bref, 1, 256, true);
        }));
        break;
      }
      case 54: {
        pdu_session_res_failed_to_modify_list_mod_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_modify_list_mod_res, bref, 1, 256, true);
        }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 19: {
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
void pdu_session_res_modify_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (pdu_session_res_modify_list_mod_res_present) {
    j.start_obj();
    j.write_int("id", 65);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_modify_list_mod_res) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_failed_to_modify_list_mod_res_present) {
    j.start_obj();
    j.write_int("id", 54);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_modify_list_mod_res) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionResourceNotifyIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_notify_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 66, 67, 121};
  return map_enum_number(names, 5, idx, "id");
}
bool pdu_session_res_notify_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 66, 67, 121};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_notify_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 66:
      return crit_e::reject;
    case 67:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_notify_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 66:
      return presence_e::optional;
    case 67:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_notify_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_res_notify_list_present ? 1 : 0;
  nof_ies += pdu_session_res_released_list_not_present ? 1 : 0;
  nof_ies += user_location_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_notify_list_present) {
    HANDLE_CODE(pack_ie(bref, 66, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_notify_list, 1, 256, true);
    }));
  }
  if (pdu_session_res_released_list_not_present) {
    HANDLE_CODE(pack_ie(bref, 67, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_released_list_not, 1, 256, true);
    }));
  }
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_notify_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_notify_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 66: {
        pdu_session_res_notify_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_notify_list, bref, 1, 256, true); }));
        break;
      }
      case 67: {
        pdu_session_res_released_list_not_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_released_list_not, bref, 1, 256, true);
        }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_notify_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (pdu_session_res_notify_list_present) {
    j.start_obj();
    j.write_int("id", 66);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_notify_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_released_list_not_present) {
    j.start_obj();
    j.write_int("id", 67);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_released_list_not) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionResourceReleaseCommandIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 83, 38, 79};
  return map_enum_number(names, 5, idx, "id");
}
bool pdu_session_res_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 83, 38, 79};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_release_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 83:
      return crit_e::ignore;
    case 38:
      return crit_e::ignore;
    case 79:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 83:
      return presence_e::optional;
    case 38:
      return presence_e::optional;
    case 79:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ran_paging_prio_present ? 1 : 0;
  nof_ies += nas_pdu_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ran_paging_prio_present) {
    HANDLE_CODE(pack_ie(bref, 83, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ran_paging_prio, (uint16_t)1u, (uint16_t)256u, false, true);
    }));
  }
  if (nas_pdu_present) {
    HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::ignore}, [this, &bref]() { return nas_pdu.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 79, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_to_release_list_rel_cmd, 1, 256, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_release_cmd_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 83: {
        ran_paging_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_paging_prio, bref, (uint16_t)1u, (uint16_t)256u, false, true);
        }));
        break;
      }
      case 38: {
        nas_pdu_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nas_pdu.unpack(bref); }));
        break;
      }
      case 79: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_to_release_list_rel_cmd, bref, 1, 256, true);
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
void pdu_session_res_release_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (ran_paging_prio_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_int("value", ran_paging_prio);
    j.end_obj();
  }
  if (nas_pdu_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "ignore");
    j.write_str("value", nas_pdu.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 79);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_to_release_list_rel_cmd) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  j.end_array();
}

// PDUSessionResourceReleaseResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_release_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 70, 121, 19};
  return map_enum_number(names, 5, idx, "id");
}
bool pdu_session_res_release_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 70, 121, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_release_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 70:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_release_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 70:
      return presence_e::mandatory;
    case 121:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_release_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += user_location_info_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 70, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_released_list_rel_res, 1, 256, true);
  }));
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_release_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_release_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 70: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_released_list_rel_res, bref, 1, 256, true);
        }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 19: {
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
void pdu_session_res_release_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 70);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_released_list_rel_res) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionResourceSetupRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_setup_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 83, 38, 74, 110, 335};
  return map_enum_number(names, 7, idx, "id");
}
bool pdu_session_res_setup_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 83, 38, 74, 110, 335};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_setup_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 83:
      return crit_e::ignore;
    case 38:
      return crit_e::reject;
    case 74:
      return crit_e::reject;
    case 110:
      return crit_e::ignore;
    case 335:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_setup_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 83:
      return presence_e::optional;
    case 38:
      return presence_e::optional;
    case 74:
      return presence_e::mandatory;
    case 110:
      return presence_e::optional;
    case 335:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_setup_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ran_paging_prio_present ? 1 : 0;
  nof_ies += nas_pdu_present ? 1 : 0;
  nof_ies += ue_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += ue_slice_max_bit_rate_list_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ran_paging_prio_present) {
    HANDLE_CODE(pack_ie(bref, 83, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ran_paging_prio, (uint16_t)1u, (uint16_t)256u, false, true);
    }));
  }
  if (nas_pdu_present) {
    HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::reject}, [this, &bref]() { return nas_pdu.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 74, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_setup_list_su_req, 1, 256, true);
  }));
  if (ue_aggr_max_bit_rate_present) {
    HANDLE_CODE(
        pack_ie(bref, 110, crit_e{crit_e::ignore}, [this, &bref]() { return ue_aggr_max_bit_rate.pack(bref); }));
  }
  if (ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 335, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_setup_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_setup_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 83: {
        ran_paging_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_paging_prio, bref, (uint16_t)1u, (uint16_t)256u, false, true);
        }));
        break;
      }
      case 38: {
        nas_pdu_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nas_pdu.unpack(bref); }));
        break;
      }
      case 74: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_setup_list_su_req, bref, 1, 256, true);
        }));
        break;
      }
      case 110: {
        ue_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 335: {
        ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_setup_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (ran_paging_prio_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_int("value", ran_paging_prio);
    j.end_obj();
  }
  if (nas_pdu_present) {
    j.start_obj();
    j.write_int("id", 38);
    j.write_str("criticality", "reject");
    j.write_str("value", nas_pdu.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 74);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_setup_list_su_req) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (ue_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 110);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 335);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PDUSessionResourceSetupResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pdu_session_res_setup_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 75, 58, 19, 121};
  return map_enum_number(names, 6, idx, "id");
}
bool pdu_session_res_setup_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 75, 58, 19, 121};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e pdu_session_res_setup_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 75:
      return crit_e::ignore;
    case 58:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pdu_session_res_setup_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 75:
      return presence_e::optional;
    case 58:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pdu_session_res_setup_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_res_setup_list_su_res_present ? 1 : 0;
  nof_ies += pdu_session_res_failed_to_setup_list_su_res_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += user_location_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_setup_list_su_res_present) {
    HANDLE_CODE(pack_ie(bref, 75, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_setup_list_su_res, 1, 256, true);
    }));
  }
  if (pdu_session_res_failed_to_setup_list_su_res_present) {
    HANDLE_CODE(pack_ie(bref, 58, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_setup_list_su_res, 1, 256, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pdu_session_res_setup_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pdu_session_res_setup_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 75: {
        pdu_session_res_setup_list_su_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_setup_list_su_res, bref, 1, 256, true);
        }));
        break;
      }
      case 58: {
        pdu_session_res_failed_to_setup_list_su_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_setup_list_su_res, bref, 1, 256, true);
        }));
        break;
      }
      case 19: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void pdu_session_res_setup_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (pdu_session_res_setup_list_su_res_present) {
    j.start_obj();
    j.write_int("id", 75);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_setup_list_su_res) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_failed_to_setup_list_su_res_present) {
    j.start_obj();
    j.write_int("id", 58);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_setup_list_su_res) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PWSCancelRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pws_cancel_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {35, 95, 122, 14};
  return map_enum_number(names, 4, idx, "id");
}
bool pws_cancel_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {35, 95, 122, 14};
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
    case 35:
      return crit_e::reject;
    case 95:
      return crit_e::reject;
    case 122:
      return crit_e::ignore;
    case 14:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pws_cancel_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 35:
      return presence_e::mandatory;
    case 95:
      return presence_e::mandatory;
    case 122:
      return presence_e::optional;
    case 14:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pws_cancel_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += warning_area_list_present ? 1 : 0;
  nof_ies += cancel_all_warning_msgs_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 35, crit_e{crit_e::reject}, [this, &bref]() { return msg_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::reject}, [this, &bref]() { return serial_num.pack(bref); }));
  if (warning_area_list_present) {
    HANDLE_CODE(pack_ie(bref, 122, crit_e{crit_e::ignore}, [this, &bref]() { return warning_area_list.pack(bref); }));
  }
  if (cancel_all_warning_msgs_present) {
    HANDLE_CODE(
        pack_ie(bref, 14, crit_e{crit_e::reject}, [this, &bref]() { return cancel_all_warning_msgs.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pws_cancel_request_ies_o::get_crit));

    switch (id) {
      case 35: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return msg_id.unpack(bref); }));
        break;
      }
      case 95: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return serial_num.unpack(bref); }));
        break;
      }
      case 122: {
        warning_area_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return warning_area_list.unpack(bref); }));
        break;
      }
      case 14: {
        cancel_all_warning_msgs_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cancel_all_warning_msgs.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 35);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  msg_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 95);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  serial_num.to_json(j);
  j.end_obj();
  if (warning_area_list_present) {
    j.start_obj();
    j.write_int("id", 122);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    warning_area_list.to_json(j);
    j.end_obj();
  }
  if (cancel_all_warning_msgs_present) {
    j.start_obj();
    j.write_int("id", 14);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// PWSCancelResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pws_cancel_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {35, 95, 12, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool pws_cancel_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {35, 95, 12, 19};
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
    case 35:
      return crit_e::reject;
    case 95:
      return crit_e::reject;
    case 12:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pws_cancel_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 35:
      return presence_e::mandatory;
    case 95:
      return presence_e::mandatory;
    case 12:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pws_cancel_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += broadcast_cancelled_area_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 35, crit_e{crit_e::reject}, [this, &bref]() { return msg_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::reject}, [this, &bref]() { return serial_num.pack(bref); }));
  if (broadcast_cancelled_area_list_present) {
    HANDLE_CODE(pack_ie(
        bref, 12, crit_e{crit_e::ignore}, [this, &bref]() { return broadcast_cancelled_area_list.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pws_cancel_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pws_cancel_resp_ies_o::get_crit));

    switch (id) {
      case 35: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return msg_id.unpack(bref); }));
        break;
      }
      case 95: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return serial_num.unpack(bref); }));
        break;
      }
      case 12: {
        broadcast_cancelled_area_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return broadcast_cancelled_area_list.unpack(bref); }));
        break;
      }
      case 19: {
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
void pws_cancel_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 35);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  msg_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 95);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  serial_num.to_json(j);
  j.end_obj();
  if (broadcast_cancelled_area_list_present) {
    j.start_obj();
    j.write_int("id", 12);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    broadcast_cancelled_area_list.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PWSFailureIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pws_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {81, 27};
  return map_enum_number(names, 2, idx, "id");
}
bool pws_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {81, 27};
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
    case 81:
      return crit_e::reject;
    case 27:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pws_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 81:
      return presence_e::mandatory;
    case 27:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pws_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(
      pack_ie(bref, 81, crit_e{crit_e::reject}, [this, &bref]() { return pws_failed_cell_id_list.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::reject}, [this, &bref]() { return global_ran_node_id.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pws_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pws_fail_ind_ies_o::get_crit));

    switch (id) {
      case 81: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pws_failed_cell_id_list.unpack(bref); }));
        break;
      }
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ran_node_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 81);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  pws_failed_cell_id_list.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_ran_node_id.to_json(j);
  j.end_obj();
  j.end_array();
}

// PWSRestartIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t pws_restart_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {16, 27, 104, 23};
  return map_enum_number(names, 4, idx, "id");
}
bool pws_restart_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {16, 27, 104, 23};
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
    case 16:
      return crit_e::reject;
    case 27:
      return crit_e::reject;
    case 104:
      return crit_e::reject;
    case 23:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e pws_restart_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 16:
      return presence_e::mandatory;
    case 27:
      return presence_e::mandatory;
    case 104:
      return presence_e::mandatory;
    case 23:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE pws_restart_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += emergency_area_id_list_for_restart_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(
      pack_ie(bref, 16, crit_e{crit_e::reject}, [this, &bref]() { return cell_id_list_for_restart.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::reject}, [this, &bref]() { return global_ran_node_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 104, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, tai_list_for_restart, 1, 2048, true);
  }));
  if (emergency_area_id_list_for_restart_present) {
    HANDLE_CODE(pack_ie(bref, 23, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, emergency_area_id_list_for_restart, 1, 256, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE pws_restart_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &pws_restart_ind_ies_o::get_crit));

    switch (id) {
      case 16: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cell_id_list_for_restart.unpack(bref); }));
        break;
      }
      case 27: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ran_node_id.unpack(bref); }));
        break;
      }
      case 104: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(tai_list_for_restart, bref, 1, 2048, true); }));
        break;
      }
      case 23: {
        emergency_area_id_list_for_restart_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(emergency_area_id_list_for_restart, bref, 1, 256, true);
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
void pws_restart_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 16);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  cell_id_list_for_restart.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 27);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  global_ran_node_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 104);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : tai_list_for_restart) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (emergency_area_id_list_for_restart_present) {
    j.start_obj();
    j.write_int("id", 23);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : emergency_area_id_list_for_restart) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// PagingIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t paging_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {115, 50, 103, 52, 118, 51, 11, 203, 202, 205, 208, 223, 222, 332, 342, 344};
  return map_enum_number(names, 16, idx, "id");
}
bool paging_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {115, 50, 103, 52, 118, 51, 11, 203, 202, 205, 208, 223, 222, 332, 342, 344};
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
    case 115:
      return crit_e::ignore;
    case 50:
      return crit_e::ignore;
    case 103:
      return crit_e::ignore;
    case 52:
      return crit_e::ignore;
    case 118:
      return crit_e::ignore;
    case 51:
      return crit_e::ignore;
    case 11:
      return crit_e::ignore;
    case 203:
      return crit_e::ignore;
    case 202:
      return crit_e::ignore;
    case 205:
      return crit_e::ignore;
    case 208:
      return crit_e::ignore;
    case 223:
      return crit_e::ignore;
    case 222:
      return crit_e::ignore;
    case 332:
      return crit_e::ignore;
    case 342:
      return crit_e::ignore;
    case 344:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e paging_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 115:
      return presence_e::mandatory;
    case 50:
      return presence_e::optional;
    case 103:
      return presence_e::mandatory;
    case 52:
      return presence_e::optional;
    case 118:
      return presence_e::optional;
    case 51:
      return presence_e::optional;
    case 11:
      return presence_e::optional;
    case 203:
      return presence_e::optional;
    case 202:
      return presence_e::optional;
    case 205:
      return presence_e::optional;
    case 208:
      return presence_e::optional;
    case 223:
      return presence_e::optional;
    case 222:
      return presence_e::optional;
    case 332:
      return presence_e::optional;
    case 342:
      return presence_e::optional;
    case 344:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE paging_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += paging_drx_present ? 1 : 0;
  nof_ies += paging_prio_present ? 1 : 0;
  nof_ies += ue_radio_cap_for_paging_present ? 1 : 0;
  nof_ies += paging_origin_present ? 1 : 0;
  nof_ies += assist_data_for_paging_present ? 1 : 0;
  nof_ies += nb_iot_paging_e_drx_info_present ? 1 : 0;
  nof_ies += nb_iot_paging_drx_present ? 1 : 0;
  nof_ies += enhanced_coverage_restrict_present ? 1 : 0;
  nof_ies += wus_assist_info_present ? 1 : 0;
  nof_ies += eutra_paginge_drx_info_present ? 1 : 0;
  nof_ies += ce_mode_brestricted_present ? 1 : 0;
  nof_ies += nr_paginge_drx_info_present ? 1 : 0;
  nof_ies += paging_cause_present ? 1 : 0;
  nof_ies += pe_ip_sassist_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 115, crit_e{crit_e::ignore}, [this, &bref]() { return ue_paging_id.pack(bref); }));
  if (paging_drx_present) {
    HANDLE_CODE(pack_ie(bref, 50, crit_e{crit_e::ignore}, [this, &bref]() { return paging_drx.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 103, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, tai_list_for_paging, 1, 16, true);
  }));
  if (paging_prio_present) {
    HANDLE_CODE(pack_ie(bref, 52, crit_e{crit_e::ignore}, [this, &bref]() { return paging_prio.pack(bref); }));
  }
  if (ue_radio_cap_for_paging_present) {
    HANDLE_CODE(
        pack_ie(bref, 118, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap_for_paging.pack(bref); }));
  }
  if (paging_origin_present) {
    HANDLE_CODE(pack_ie(bref, 51, crit_e{crit_e::ignore}, [this, &bref]() { return paging_origin.pack(bref); }));
  }
  if (assist_data_for_paging_present) {
    HANDLE_CODE(
        pack_ie(bref, 11, crit_e{crit_e::ignore}, [this, &bref]() { return assist_data_for_paging.pack(bref); }));
  }
  if (nb_iot_paging_e_drx_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 203, crit_e{crit_e::ignore}, [this, &bref]() { return nb_iot_paging_e_drx_info.pack(bref); }));
  }
  if (nb_iot_paging_drx_present) {
    HANDLE_CODE(pack_ie(bref, 202, crit_e{crit_e::ignore}, [this, &bref]() { return nb_iot_paging_drx.pack(bref); }));
  }
  if (enhanced_coverage_restrict_present) {
    HANDLE_CODE(
        pack_ie(bref, 205, crit_e{crit_e::ignore}, [this, &bref]() { return enhanced_coverage_restrict.pack(bref); }));
  }
  if (wus_assist_info_present) {
    HANDLE_CODE(pack_ie(bref, 208, crit_e{crit_e::ignore}, [this, &bref]() { return wus_assist_info.pack(bref); }));
  }
  if (eutra_paginge_drx_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 223, crit_e{crit_e::ignore}, [this, &bref]() { return eutra_paginge_drx_info.pack(bref); }));
  }
  if (ce_mode_brestricted_present) {
    HANDLE_CODE(pack_ie(bref, 222, crit_e{crit_e::ignore}, [this, &bref]() { return ce_mode_brestricted.pack(bref); }));
  }
  if (nr_paginge_drx_info_present) {
    HANDLE_CODE(pack_ie(bref, 332, crit_e{crit_e::ignore}, [this, &bref]() { return nr_paginge_drx_info.pack(bref); }));
  }
  if (paging_cause_present) {
    HANDLE_CODE(pack_ie(bref, 342, crit_e{crit_e::ignore}, [this, &bref]() { return paging_cause.pack(bref); }));
  }
  if (pe_ip_sassist_info_present) {
    HANDLE_CODE(pack_ie(bref, 344, crit_e{crit_e::ignore}, [this, &bref]() { return pe_ip_sassist_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE paging_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &paging_ies_o::get_crit));

    switch (id) {
      case 115: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_paging_id.unpack(bref); }));
        break;
      }
      case 50: {
        paging_drx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_drx.unpack(bref); }));
        break;
      }
      case 103: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(tai_list_for_paging, bref, 1, 16, true); }));
        break;
      }
      case 52: {
        paging_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_prio.unpack(bref); }));
        break;
      }
      case 118: {
        ue_radio_cap_for_paging_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_for_paging.unpack(bref); }));
        break;
      }
      case 51: {
        paging_origin_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_origin.unpack(bref); }));
        break;
      }
      case 11: {
        assist_data_for_paging_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return assist_data_for_paging.unpack(bref); }));
        break;
      }
      case 203: {
        nb_iot_paging_e_drx_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nb_iot_paging_e_drx_info.unpack(bref); }));
        break;
      }
      case 202: {
        nb_iot_paging_drx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nb_iot_paging_drx.unpack(bref); }));
        break;
      }
      case 205: {
        enhanced_coverage_restrict_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return enhanced_coverage_restrict.unpack(bref); }));
        break;
      }
      case 208: {
        wus_assist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return wus_assist_info.unpack(bref); }));
        break;
      }
      case 223: {
        eutra_paginge_drx_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return eutra_paginge_drx_info.unpack(bref); }));
        break;
      }
      case 222: {
        ce_mode_brestricted_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ce_mode_brestricted.unpack(bref); }));
        break;
      }
      case 332: {
        nr_paginge_drx_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_paginge_drx_info.unpack(bref); }));
        break;
      }
      case 342: {
        paging_cause_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_cause.unpack(bref); }));
        break;
      }
      case 344: {
        pe_ip_sassist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pe_ip_sassist_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 115);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  ue_paging_id.to_json(j);
  j.end_obj();
  if (paging_drx_present) {
    j.start_obj();
    j.write_int("id", 50);
    j.write_str("criticality", "ignore");
    j.write_str("value", paging_drx.to_string());
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 103);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : tai_list_for_paging) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (paging_prio_present) {
    j.start_obj();
    j.write_int("id", 52);
    j.write_str("criticality", "ignore");
    j.write_str("value", paging_prio.to_string());
    j.end_obj();
  }
  if (ue_radio_cap_for_paging_present) {
    j.start_obj();
    j.write_int("id", 118);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_radio_cap_for_paging.to_json(j);
    j.end_obj();
  }
  if (paging_origin_present) {
    j.start_obj();
    j.write_int("id", 51);
    j.write_str("criticality", "ignore");
    j.write_str("value", "non-3gpp");
    j.end_obj();
  }
  if (assist_data_for_paging_present) {
    j.start_obj();
    j.write_int("id", 11);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    assist_data_for_paging.to_json(j);
    j.end_obj();
  }
  if (nb_iot_paging_e_drx_info_present) {
    j.start_obj();
    j.write_int("id", 203);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nb_iot_paging_e_drx_info.to_json(j);
    j.end_obj();
  }
  if (nb_iot_paging_drx_present) {
    j.start_obj();
    j.write_int("id", 202);
    j.write_str("criticality", "ignore");
    j.write_str("value", nb_iot_paging_drx.to_string());
    j.end_obj();
  }
  if (enhanced_coverage_restrict_present) {
    j.start_obj();
    j.write_int("id", 205);
    j.write_str("criticality", "ignore");
    j.write_str("value", "restricted");
    j.end_obj();
  }
  if (wus_assist_info_present) {
    j.start_obj();
    j.write_int("id", 208);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    wus_assist_info.to_json(j);
    j.end_obj();
  }
  if (eutra_paginge_drx_info_present) {
    j.start_obj();
    j.write_int("id", 223);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    eutra_paginge_drx_info.to_json(j);
    j.end_obj();
  }
  if (ce_mode_brestricted_present) {
    j.start_obj();
    j.write_int("id", 222);
    j.write_str("criticality", "ignore");
    j.write_str("value", ce_mode_brestricted.to_string());
    j.end_obj();
  }
  if (nr_paginge_drx_info_present) {
    j.start_obj();
    j.write_int("id", 332);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_paginge_drx_info.to_json(j);
    j.end_obj();
  }
  if (paging_cause_present) {
    j.start_obj();
    j.write_int("id", 342);
    j.write_str("criticality", "ignore");
    j.write_str("value", "voice");
    j.end_obj();
  }
  if (pe_ip_sassist_info_present) {
    j.start_obj();
    j.write_int("id", 344);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pe_ip_sassist_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PathSwitchRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t path_switch_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {85, 100, 121, 119, 76, 57, 237, 333, 427};
  return map_enum_number(names, 9, idx, "id");
}
bool path_switch_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {85, 100, 121, 119, 76, 57, 237, 333, 427};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e path_switch_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 85:
      return crit_e::reject;
    case 100:
      return crit_e::reject;
    case 121:
      return crit_e::ignore;
    case 119:
      return crit_e::ignore;
    case 76:
      return crit_e::reject;
    case 57:
      return crit_e::ignore;
    case 237:
      return crit_e::ignore;
    case 333:
      return crit_e::ignore;
    case 427:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e path_switch_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 85:
      return presence_e::mandatory;
    case 100:
      return presence_e::mandatory;
    case 121:
      return presence_e::mandatory;
    case 119:
      return presence_e::mandatory;
    case 76:
      return presence_e::mandatory;
    case 57:
      return presence_e::optional;
    case 237:
      return presence_e::optional;
    case 333:
      return presence_e::optional;
    case 427:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE path_switch_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += pdu_session_res_failed_to_setup_list_ps_req_present ? 1 : 0;
  nof_ies += rrc_resume_cause_present ? 1 : 0;
  nof_ies += red_cap_ind_present ? 1 : 0;
  nof_ies += e_red_cap_ind_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 100, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, source_amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 119, crit_e{crit_e::ignore}, [this, &bref]() { return ue_security_cap.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 76, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_to_be_switched_dl_list, 1, 256, true);
  }));
  if (pdu_session_res_failed_to_setup_list_ps_req_present) {
    HANDLE_CODE(pack_ie(bref, 57, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_setup_list_ps_req, 1, 256, true);
    }));
  }
  if (rrc_resume_cause_present) {
    HANDLE_CODE(pack_ie(bref, 237, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_resume_cause.pack(bref); }));
  }
  if (red_cap_ind_present) {
    HANDLE_CODE(pack_ie(bref, 333, crit_e{crit_e::ignore}, [this, &bref]() { return red_cap_ind.pack(bref); }));
  }
  if (e_red_cap_ind_present) {
    HANDLE_CODE(pack_ie(bref, 427, crit_e{crit_e::ignore}, [this, &bref]() { return e_red_cap_ind.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE path_switch_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &path_switch_request_ies_o::get_crit));

    switch (id) {
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 100: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(source_amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 121: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 119: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_security_cap.unpack(bref); }));
        break;
      }
      case 76: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_to_be_switched_dl_list, bref, 1, 256, true);
        }));
        break;
      }
      case 57: {
        pdu_session_res_failed_to_setup_list_ps_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_setup_list_ps_req, bref, 1, 256, true);
        }));
        break;
      }
      case 237: {
        rrc_resume_cause_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_resume_cause.unpack(bref); }));
        break;
      }
      case 333: {
        red_cap_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return red_cap_ind.unpack(bref); }));
        break;
      }
      case 427: {
        e_red_cap_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return e_red_cap_ind.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void path_switch_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 100);
  j.write_str("criticality", "reject");
  j.write_uint("value", source_amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 121);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  user_location_info.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 119);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  ue_security_cap.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 76);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_to_be_switched_dl_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (pdu_session_res_failed_to_setup_list_ps_req_present) {
    j.start_obj();
    j.write_int("id", 57);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_setup_list_ps_req) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (rrc_resume_cause_present) {
    j.start_obj();
    j.write_int("id", 237);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_resume_cause.to_string());
    j.end_obj();
  }
  if (red_cap_ind_present) {
    j.start_obj();
    j.write_int("id", 333);
    j.write_str("criticality", "ignore");
    j.write_str("value", "redcap");
    j.end_obj();
  }
  if (e_red_cap_ind_present) {
    j.start_obj();
    j.write_int("id", 427);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// PathSwitchRequestAcknowledgeIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t path_switch_request_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10,  85,  119, 93,  41,  77,  68,  0,   18,  91,  19,  146, 165, 177,
                                   205, 206, 209, 216, 215, 218, 217, 219, 222, 234, 264, 254, 326, 345,
                                   346, 347, 359, 199, 373, 374, 375, 376, 377, 378, 400, 414, 430};
  return map_enum_number(names, 41, idx, "id");
}
bool path_switch_request_ack_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10,  85,  119, 93,  41,  77,  68,  0,   18,  91,  19,  146, 165, 177,
                                   205, 206, 209, 216, 215, 218, 217, 219, 222, 234, 264, 254, 326, 345,
                                   346, 347, 359, 199, 373, 374, 375, 376, 377, 378, 400, 414, 430};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e path_switch_request_ack_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 119:
      return crit_e::reject;
    case 93:
      return crit_e::reject;
    case 41:
      return crit_e::reject;
    case 77:
      return crit_e::ignore;
    case 68:
      return crit_e::ignore;
    case 0:
      return crit_e::reject;
    case 18:
      return crit_e::ignore;
    case 91:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    case 146:
      return crit_e::ignore;
    case 165:
      return crit_e::ignore;
    case 177:
      return crit_e::ignore;
    case 205:
      return crit_e::ignore;
    case 206:
      return crit_e::ignore;
    case 209:
      return crit_e::ignore;
    case 216:
      return crit_e::ignore;
    case 215:
      return crit_e::ignore;
    case 218:
      return crit_e::ignore;
    case 217:
      return crit_e::ignore;
    case 219:
      return crit_e::ignore;
    case 222:
      return crit_e::ignore;
    case 234:
      return crit_e::ignore;
    case 264:
      return crit_e::reject;
    case 254:
      return crit_e::ignore;
    case 326:
      return crit_e::ignore;
    case 345:
      return crit_e::ignore;
    case 346:
      return crit_e::ignore;
    case 347:
      return crit_e::ignore;
    case 359:
      return crit_e::ignore;
    case 199:
      return crit_e::ignore;
    case 373:
      return crit_e::ignore;
    case 374:
      return crit_e::ignore;
    case 375:
      return crit_e::ignore;
    case 376:
      return crit_e::ignore;
    case 377:
      return crit_e::ignore;
    case 378:
      return crit_e::ignore;
    case 400:
      return crit_e::ignore;
    case 414:
      return crit_e::ignore;
    case 430:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e path_switch_request_ack_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 119:
      return presence_e::optional;
    case 93:
      return presence_e::mandatory;
    case 41:
      return presence_e::optional;
    case 77:
      return presence_e::mandatory;
    case 68:
      return presence_e::optional;
    case 0:
      return presence_e::mandatory;
    case 18:
      return presence_e::optional;
    case 91:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    case 146:
      return presence_e::optional;
    case 165:
      return presence_e::optional;
    case 177:
      return presence_e::optional;
    case 205:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 209:
      return presence_e::optional;
    case 216:
      return presence_e::optional;
    case 215:
      return presence_e::optional;
    case 218:
      return presence_e::optional;
    case 217:
      return presence_e::optional;
    case 219:
      return presence_e::optional;
    case 222:
      return presence_e::optional;
    case 234:
      return presence_e::optional;
    case 264:
      return presence_e::optional;
    case 254:
      return presence_e::optional;
    case 326:
      return presence_e::optional;
    case 345:
      return presence_e::optional;
    case 346:
      return presence_e::optional;
    case 347:
      return presence_e::optional;
    case 359:
      return presence_e::optional;
    case 199:
      return presence_e::optional;
    case 373:
      return presence_e::optional;
    case 374:
      return presence_e::optional;
    case 375:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 377:
      return presence_e::optional;
    case 378:
      return presence_e::optional;
    case 400:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    case 430:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE path_switch_request_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  nof_ies += ue_security_cap_present ? 1 : 0;
  nof_ies += new_security_context_ind_present ? 1 : 0;
  nof_ies += pdu_session_res_released_list_ps_ack_present ? 1 : 0;
  nof_ies += core_network_assist_info_for_inactive_present ? 1 : 0;
  nof_ies += rrc_inactive_transition_report_request_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += redirection_voice_fallback_present ? 1 : 0;
  nof_ies += cn_assisted_ran_tuning_present ? 1 : 0;
  nof_ies += srvcc_operation_possible_present ? 1 : 0;
  nof_ies += enhanced_coverage_restrict_present ? 1 : 0;
  nof_ies += extended_connected_time_present ? 1 : 0;
  nof_ies += ue_diff_info_present ? 1 : 0;
  nof_ies += nr_v2x_services_authorized_present ? 1 : 0;
  nof_ies += ltev2x_services_authorized_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += pc5_qos_params_present ? 1 : 0;
  nof_ies += ce_mode_brestricted_present ? 1 : 0;
  nof_ies += ue_up_c_iot_support_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_list_present ? 1 : 0;
  nof_ies += time_sync_assist_info_present ? 1 : 0;
  nof_ies += five_g_pro_se_authorized_present ? 1 : 0;
  nof_ies += five_g_pro_se_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += five_g_pro_se_pc5_qos_params_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_mod_list_present ? 1 : 0;
  nof_ies += iab_authorized_present ? 1 : 0;
  nof_ies += aerial_ue_sub_info_present ? 1 : 0;
  nof_ies += nr_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += lte_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += nr_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += lte_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += a2_x_pc5_qos_params_present ? 1 : 0;
  nof_ies += mobile_iab_authorized_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  nof_ies += sl_positioning_ranging_service_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ue_security_cap_present) {
    HANDLE_CODE(pack_ie(bref, 119, crit_e{crit_e::reject}, [this, &bref]() { return ue_security_cap.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 93, crit_e{crit_e::reject}, [this, &bref]() { return security_context.pack(bref); }));
  if (new_security_context_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 41, crit_e{crit_e::reject}, [this, &bref]() { return new_security_context_ind.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 77, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_switched_list, 1, 256, true);
  }));
  if (pdu_session_res_released_list_ps_ack_present) {
    HANDLE_CODE(pack_ie(bref, 68, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_released_list_ps_ack, 1, 256, true);
    }));
  }
  HANDLE_CODE(pack_ie(
      bref, 0, crit_e{crit_e::reject}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  if (core_network_assist_info_for_inactive_present) {
    HANDLE_CODE(pack_ie(bref, 18, crit_e{crit_e::ignore}, [this, &bref]() {
      return core_network_assist_info_for_inactive.pack(bref);
    }));
  }
  if (rrc_inactive_transition_report_request_present) {
    HANDLE_CODE(pack_ie(bref, 91, crit_e{crit_e::ignore}, [this, &bref]() {
      return rrc_inactive_transition_report_request.pack(bref);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (redirection_voice_fallback_present) {
    HANDLE_CODE(
        pack_ie(bref, 146, crit_e{crit_e::ignore}, [this, &bref]() { return redirection_voice_fallback.pack(bref); }));
  }
  if (cn_assisted_ran_tuning_present) {
    HANDLE_CODE(
        pack_ie(bref, 165, crit_e{crit_e::ignore}, [this, &bref]() { return cn_assisted_ran_tuning.pack(bref); }));
  }
  if (srvcc_operation_possible_present) {
    HANDLE_CODE(
        pack_ie(bref, 177, crit_e{crit_e::ignore}, [this, &bref]() { return srvcc_operation_possible.pack(bref); }));
  }
  if (enhanced_coverage_restrict_present) {
    HANDLE_CODE(
        pack_ie(bref, 205, crit_e{crit_e::ignore}, [this, &bref]() { return enhanced_coverage_restrict.pack(bref); }));
  }
  if (extended_connected_time_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, extended_connected_time, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (ue_diff_info_present) {
    HANDLE_CODE(pack_ie(bref, 209, crit_e{crit_e::ignore}, [this, &bref]() { return ue_diff_info.pack(bref); }));
  }
  if (nr_v2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 216, crit_e{crit_e::ignore}, [this, &bref]() { return nr_v2x_services_authorized.pack(bref); }));
  }
  if (ltev2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 215, crit_e{crit_e::ignore}, [this, &bref]() { return ltev2x_services_authorized.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 218, crit_e{crit_e::ignore}, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 217, crit_e{crit_e::ignore}, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 219, crit_e{crit_e::ignore}, [this, &bref]() { return pc5_qos_params.pack(bref); }));
  }
  if (ce_mode_brestricted_present) {
    HANDLE_CODE(pack_ie(bref, 222, crit_e{crit_e::ignore}, [this, &bref]() { return ce_mode_brestricted.pack(bref); }));
  }
  if (ue_up_c_iot_support_present) {
    HANDLE_CODE(pack_ie(bref, 234, crit_e{crit_e::ignore}, [this, &bref]() { return ue_up_c_iot_support.pack(bref); }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }
  if (management_based_mdt_plmn_list_present) {
    HANDLE_CODE(pack_ie(bref, 254, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_list, 1, 16, true);
    }));
  }
  if (time_sync_assist_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 326, crit_e{crit_e::ignore}, [this, &bref]() { return time_sync_assist_info.pack(bref); }));
  }
  if (five_g_pro_se_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 345, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_authorized.pack(bref); }));
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(bref, 346, crit_e{crit_e::ignore}, [this, &bref]() {
      return five_g_pro_se_ue_pc5_aggr_max_bit_rate.pack(bref);
    }));
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(
        bref, 347, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_pc5_qos_params.pack(bref); }));
  }
  if (management_based_mdt_plmn_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 359, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_mod_list, 0, 16, true);
    }));
  }
  if (iab_authorized_present) {
    HANDLE_CODE(pack_ie(bref, 199, crit_e{crit_e::ignore}, [this, &bref]() { return iab_authorized.pack(bref); }));
  }
  if (aerial_ue_sub_info_present) {
    HANDLE_CODE(pack_ie(bref, 373, crit_e{crit_e::ignore}, [this, &bref]() { return aerial_ue_sub_info.pack(bref); }));
  }
  if (nr_a2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 374, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_services_authorized.pack(bref); }));
  }
  if (lte_a2_x_services_authorized_present) {
    HANDLE_CODE(pack_ie(
        bref, 375, crit_e{crit_e::ignore}, [this, &bref]() { return lte_a2_x_services_authorized.pack(bref); }));
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 376, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (lte_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 377, crit_e{crit_e::ignore}, [this, &bref]() { return lte_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (a2_x_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 378, crit_e{crit_e::ignore}, [this, &bref]() { return a2_x_pc5_qos_params.pack(bref); }));
  }
  if (mobile_iab_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 400, crit_e{crit_e::ignore}, [this, &bref]() { return mobile_iab_authorized.pack(bref); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }
  if (sl_positioning_ranging_service_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 430, crit_e{crit_e::ignore}, [this, &bref]() { return sl_positioning_ranging_service_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE path_switch_request_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &path_switch_request_ack_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 119: {
        ue_security_cap_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_security_cap.unpack(bref); }));
        break;
      }
      case 93: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return security_context.unpack(bref); }));
        break;
      }
      case 41: {
        new_security_context_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return new_security_context_ind.unpack(bref); }));
        break;
      }
      case 77: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_switched_list, bref, 1, 256, true); }));
        break;
      }
      case 68: {
        pdu_session_res_released_list_ps_ack_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_released_list_ps_ack, bref, 1, 256, true);
        }));
        break;
      }
      case 0: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 18: {
        core_network_assist_info_for_inactive_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return core_network_assist_info_for_inactive.unpack(bref); }));
        break;
      }
      case 91: {
        rrc_inactive_transition_report_request_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return rrc_inactive_transition_report_request.unpack(bref); }));
        break;
      }
      case 19: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 146: {
        redirection_voice_fallback_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return redirection_voice_fallback.unpack(bref); }));
        break;
      }
      case 165: {
        cn_assisted_ran_tuning_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cn_assisted_ran_tuning.unpack(bref); }));
        break;
      }
      case 177: {
        srvcc_operation_possible_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srvcc_operation_possible.unpack(bref); }));
        break;
      }
      case 205: {
        enhanced_coverage_restrict_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return enhanced_coverage_restrict.unpack(bref); }));
        break;
      }
      case 206: {
        extended_connected_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(extended_connected_time, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 209: {
        ue_diff_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_diff_info.unpack(bref); }));
        break;
      }
      case 216: {
        nr_v2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_v2x_services_authorized.unpack(bref); }));
        break;
      }
      case 215: {
        ltev2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltev2x_services_authorized.unpack(bref); }));
        break;
      }
      case 218: {
        nr_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 217: {
        lte_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 219: {
        pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pc5_qos_params.unpack(bref); }));
        break;
      }
      case 222: {
        ce_mode_brestricted_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ce_mode_brestricted.unpack(bref); }));
        break;
      }
      case 234: {
        ue_up_c_iot_support_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_up_c_iot_support.unpack(bref); }));
        break;
      }
      case 264: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 254: {
        management_based_mdt_plmn_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(management_based_mdt_plmn_list, bref, 1, 16, true); }));
        break;
      }
      case 326: {
        time_sync_assist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_sync_assist_info.unpack(bref); }));
        break;
      }
      case 345: {
        five_g_pro_se_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_authorized.unpack(bref); }));
        break;
      }
      case 346: {
        five_g_pro_se_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 347: {
        five_g_pro_se_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 359: {
        management_based_mdt_plmn_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(management_based_mdt_plmn_mod_list, bref, 0, 16, true);
        }));
        break;
      }
      case 199: {
        iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_authorized.unpack(bref); }));
        break;
      }
      case 373: {
        aerial_ue_sub_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return aerial_ue_sub_info.unpack(bref); }));
        break;
      }
      case 374: {
        nr_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 375: {
        lte_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 376: {
        nr_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 377: {
        lte_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 378: {
        a2_x_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return a2_x_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 400: {
        mobile_iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_authorized.unpack(bref); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 430: {
        sl_positioning_ranging_service_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sl_positioning_ranging_service_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void path_switch_request_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (ue_security_cap_present) {
    j.start_obj();
    j.write_int("id", 119);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ue_security_cap.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 93);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  security_context.to_json(j);
  j.end_obj();
  if (new_security_context_ind_present) {
    j.start_obj();
    j.write_int("id", 41);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 77);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_switched_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (pdu_session_res_released_list_ps_ack_present) {
    j.start_obj();
    j.write_int("id", 68);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_released_list_ps_ack) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 0);
  j.write_str("criticality", "reject");
  j.start_array("value");
  for (const auto& e1 : allowed_nssai) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (core_network_assist_info_for_inactive_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    core_network_assist_info_for_inactive.to_json(j);
    j.end_obj();
  }
  if (rrc_inactive_transition_report_request_present) {
    j.start_obj();
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_inactive_transition_report_request.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (redirection_voice_fallback_present) {
    j.start_obj();
    j.write_int("id", 146);
    j.write_str("criticality", "ignore");
    j.write_str("value", redirection_voice_fallback.to_string());
    j.end_obj();
  }
  if (cn_assisted_ran_tuning_present) {
    j.start_obj();
    j.write_int("id", 165);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cn_assisted_ran_tuning.to_json(j);
    j.end_obj();
  }
  if (srvcc_operation_possible_present) {
    j.start_obj();
    j.write_int("id", 177);
    j.write_str("criticality", "ignore");
    j.write_str("value", srvcc_operation_possible.to_string());
    j.end_obj();
  }
  if (enhanced_coverage_restrict_present) {
    j.start_obj();
    j.write_int("id", 205);
    j.write_str("criticality", "ignore");
    j.write_str("value", "restricted");
    j.end_obj();
  }
  if (extended_connected_time_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "ignore");
    j.write_int("value", extended_connected_time);
    j.end_obj();
  }
  if (ue_diff_info_present) {
    j.start_obj();
    j.write_int("id", 209);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_diff_info.to_json(j);
    j.end_obj();
  }
  if (nr_v2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 216);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_v2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltev2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 215);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltev2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 218);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 217);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 219);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (ce_mode_brestricted_present) {
    j.start_obj();
    j.write_int("id", 222);
    j.write_str("criticality", "ignore");
    j.write_str("value", ce_mode_brestricted.to_string());
    j.end_obj();
  }
  if (ue_up_c_iot_support_present) {
    j.start_obj();
    j.write_int("id", 234);
    j.write_str("criticality", "ignore");
    j.write_str("value", "supported");
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 264);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  if (management_based_mdt_plmn_list_present) {
    j.start_obj();
    j.write_int("id", 254);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (time_sync_assist_info_present) {
    j.start_obj();
    j.write_int("id", 326);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    time_sync_assist_info.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_authorized_present) {
    j.start_obj();
    j.write_int("id", 345);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_authorized.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 346);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 347);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (management_based_mdt_plmn_mod_list_present) {
    j.start_obj();
    j.write_int("id", 359);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_mod_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 199);
    j.write_str("criticality", "ignore");
    j.write_str("value", iab_authorized.to_string());
    j.end_obj();
  }
  if (aerial_ue_sub_info_present) {
    j.start_obj();
    j.write_int("id", 373);
    j.write_str("criticality", "ignore");
    j.write_str("value", aerial_ue_sub_info.to_string());
    j.end_obj();
  }
  if (nr_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 374);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (lte_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 375);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (lte_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 377);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (a2_x_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 378);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    a2_x_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 400);
    j.write_str("criticality", "ignore");
    j.write_str("value", mobile_iab_authorized.to_string());
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (sl_positioning_ranging_service_info_present) {
    j.start_obj();
    j.write_int("id", 430);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sl_positioning_ranging_service_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// PathSwitchRequestFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t path_switch_request_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 69, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool path_switch_request_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 69, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e path_switch_request_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 69:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e path_switch_request_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 69:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE path_switch_request_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 69, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_released_list_ps_fail, 1, 256, true);
  }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE path_switch_request_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &path_switch_request_fail_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 69: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_released_list_ps_fail, bref, 1, 256, true);
        }));
        break;
      }
      case 19: {
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
void path_switch_request_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 69);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_released_list_ps_fail) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// Value ::= OPEN TYPE
void ngap_private_ies_empty_o::value_c::to_json(json_writer& j) const
{
  j.start_obj();
  j.end_obj();
}
OCUDUASN_CODE ngap_private_ies_empty_o::value_c::pack(bit_ref& bref) const
{
  varlength_field_pack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ngap_private_ies_empty_o::value_c::unpack(cbit_ref& bref)
{
  varlength_field_unpack_guard varlen_scope(bref, true);
  return OCUDUASN_SUCCESS;
}

const char* ngap_private_ies_empty_o::value_c::types_opts::to_string() const
{
  static const char* names[] = {};
  return convert_enum_idx(names, 0, value, "ngap_private_ies_empty_o::value_c::types");
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
  HANDLE_CODE(bref.pack(false, 1));

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

// RANCPRelocationIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ran_cp_relocation_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {85, 26, 25, 213, 211};
  return map_enum_number(names, 5, idx, "id");
}
bool ran_cp_relocation_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {85, 26, 25, 213, 211};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ran_cp_relocation_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 85:
      return crit_e::reject;
    case 26:
      return crit_e::reject;
    case 25:
      return crit_e::ignore;
    case 213:
      return crit_e::ignore;
    case 211:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ran_cp_relocation_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 85:
      return presence_e::mandatory;
    case 26:
      return presence_e::mandatory;
    case 25:
      return presence_e::mandatory;
    case 213:
      return presence_e::mandatory;
    case 211:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ran_cp_relocation_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 5;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::reject}, [this, &bref]() { return five_g_s_tmsi.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 25, crit_e{crit_e::ignore}, [this, &bref]() { return eutra_cgi.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 213, crit_e{crit_e::ignore}, [this, &bref]() { return tai.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 211, crit_e{crit_e::reject}, [this, &bref]() { return ul_cp_security_info.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_cp_relocation_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 5;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ran_cp_relocation_ind_ies_o::get_crit));

    switch (id) {
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 26: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_s_tmsi.unpack(bref); }));
        break;
      }
      case 25: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return eutra_cgi.unpack(bref); }));
        break;
      }
      case 213: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return tai.unpack(bref); }));
        break;
      }
      case 211: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ul_cp_security_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ran_cp_relocation_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 26);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  five_g_s_tmsi.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 25);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  eutra_cgi.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 213);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  tai.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 211);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ul_cp_security_info.to_json(j);
  j.end_obj();
  j.end_array();
}

// RANConfigurationUpdateIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ran_cfg_upd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {82, 102, 21, 27, 167, 204, 273};
  return map_enum_number(names, 7, idx, "id");
}
bool ran_cfg_upd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {82, 102, 21, 27, 167, 204, 273};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ran_cfg_upd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 82:
      return crit_e::ignore;
    case 102:
      return crit_e::reject;
    case 21:
      return crit_e::ignore;
    case 27:
      return crit_e::ignore;
    case 167:
      return crit_e::reject;
    case 204:
      return crit_e::ignore;
    case 273:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ran_cfg_upd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 82:
      return presence_e::optional;
    case 102:
      return presence_e::optional;
    case 21:
      return presence_e::optional;
    case 27:
      return presence_e::optional;
    case 167:
      return presence_e::optional;
    case 204:
      return presence_e::optional;
    case 273:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ran_cfg_upd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += ran_node_name_present ? 1 : 0;
  nof_ies += supported_ta_list_present ? 1 : 0;
  nof_ies += default_paging_drx_present ? 1 : 0;
  nof_ies += global_ran_node_id_present ? 1 : 0;
  nof_ies += ngran_tnl_assoc_to_rem_list_present ? 1 : 0;
  nof_ies += nb_iot_default_paging_drx_present ? 1 : 0;
  nof_ies += extended_ran_node_name_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (ran_node_name_present) {
    HANDLE_CODE(pack_ie(bref, 82, crit_e{crit_e::ignore}, [this, &bref]() { return ran_node_name.pack(bref); }));
  }
  if (supported_ta_list_present) {
    HANDLE_CODE(pack_ie(bref, 102, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, supported_ta_list, 1, 256, true);
    }));
  }
  if (default_paging_drx_present) {
    HANDLE_CODE(pack_ie(bref, 21, crit_e{crit_e::ignore}, [this, &bref]() { return default_paging_drx.pack(bref); }));
  }
  if (global_ran_node_id_present) {
    HANDLE_CODE(pack_ie(bref, 27, crit_e{crit_e::ignore}, [this, &bref]() { return global_ran_node_id.pack(bref); }));
  }
  if (ngran_tnl_assoc_to_rem_list_present) {
    HANDLE_CODE(pack_ie(bref, 167, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ngran_tnl_assoc_to_rem_list, 1, 32, true);
    }));
  }
  if (nb_iot_default_paging_drx_present) {
    HANDLE_CODE(
        pack_ie(bref, 204, crit_e{crit_e::ignore}, [this, &bref]() { return nb_iot_default_paging_drx.pack(bref); }));
  }
  if (extended_ran_node_name_present) {
    HANDLE_CODE(
        pack_ie(bref, 273, crit_e{crit_e::ignore}, [this, &bref]() { return extended_ran_node_name.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_cfg_upd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ran_cfg_upd_ies_o::get_crit));

    switch (id) {
      case 82: {
        ran_node_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_node_name.unpack(bref); }));
        break;
      }
      case 102: {
        supported_ta_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(supported_ta_list, bref, 1, 256, true); }));
        break;
      }
      case 21: {
        default_paging_drx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return default_paging_drx.unpack(bref); }));
        break;
      }
      case 27: {
        global_ran_node_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return global_ran_node_id.unpack(bref); }));
        break;
      }
      case 167: {
        ngran_tnl_assoc_to_rem_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ngran_tnl_assoc_to_rem_list, bref, 1, 32, true); }));
        break;
      }
      case 204: {
        nb_iot_default_paging_drx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nb_iot_default_paging_drx.unpack(bref); }));
        break;
      }
      case 273: {
        extended_ran_node_name_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return extended_ran_node_name.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void ran_cfg_upd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (ran_node_name_present) {
    j.start_obj();
    j.write_int("id", 82);
    j.write_str("criticality", "ignore");
    j.write_str("value", ran_node_name.to_string());
    j.end_obj();
  }
  if (supported_ta_list_present) {
    j.start_obj();
    j.write_int("id", 102);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : supported_ta_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (default_paging_drx_present) {
    j.start_obj();
    j.write_int("id", 21);
    j.write_str("criticality", "ignore");
    j.write_str("value", default_paging_drx.to_string());
    j.end_obj();
  }
  if (global_ran_node_id_present) {
    j.start_obj();
    j.write_int("id", 27);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    global_ran_node_id.to_json(j);
    j.end_obj();
  }
  if (ngran_tnl_assoc_to_rem_list_present) {
    j.start_obj();
    j.write_int("id", 167);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : ngran_tnl_assoc_to_rem_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (nb_iot_default_paging_drx_present) {
    j.start_obj();
    j.write_int("id", 204);
    j.write_str("criticality", "ignore");
    j.write_str("value", nb_iot_default_paging_drx.to_string());
    j.end_obj();
  }
  if (extended_ran_node_name_present) {
    j.start_obj();
    j.write_int("id", 273);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    extended_ran_node_name.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RANConfigurationUpdateAcknowledgeIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ran_cfg_upd_ack_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {19};
  return map_enum_number(names, 1, idx, "id");
}
bool ran_cfg_upd_ack_ies_o::is_id_valid(const uint32_t& id)
{
  return 19 == id;
}
crit_e ran_cfg_upd_ack_ies_o::get_crit(const uint32_t& id)
{
  if (id == 19) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e ran_cfg_upd_ack_ies_o::get_presence(const uint32_t& id)
{
  if (id == 19) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE ran_cfg_upd_ack_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_cfg_upd_ack_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ran_cfg_upd_ack_ies_o::get_crit));

    if (id == 19) {
      {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void ran_cfg_upd_ack_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RANConfigurationUpdateFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ran_cfg_upd_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {15, 107, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool ran_cfg_upd_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {15, 107, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ran_cfg_upd_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 15:
      return crit_e::ignore;
    case 107:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ran_cfg_upd_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 15:
      return presence_e::mandatory;
    case 107:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ran_cfg_upd_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += time_to_wait_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (time_to_wait_present) {
    HANDLE_CODE(pack_ie(bref, 107, crit_e{crit_e::ignore}, [this, &bref]() { return time_to_wait.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_cfg_upd_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ran_cfg_upd_fail_ies_o::get_crit));

    switch (id) {
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 107: {
        time_to_wait_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_to_wait.unpack(bref); }));
        break;
      }
      case 19: {
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
void ran_cfg_upd_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (time_to_wait_present) {
    j.start_obj();
    j.write_int("id", 107);
    j.write_str("criticality", "ignore");
    j.write_str("value", time_to_wait.to_string());
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// RANPagingRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ran_paging_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 407, 408};
  return map_enum_number(names, 4, idx, "id");
}
bool ran_paging_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 407, 408};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ran_paging_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 407:
      return crit_e::ignore;
    case 408:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ran_paging_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 407:
      return presence_e::optional;
    case 408:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ran_paging_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += paging_policy_diff_present ? 1 : 0;
  nof_ies += dl_sig_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (paging_policy_diff_present) {
    HANDLE_CODE(pack_ie(bref, 407, crit_e{crit_e::ignore}, [this, &bref]() { return paging_policy_diff.pack(bref); }));
  }
  if (dl_sig_present) {
    HANDLE_CODE(pack_ie(bref, 408, crit_e{crit_e::ignore}, [this, &bref]() { return dl_sig.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ran_paging_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ran_paging_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 407: {
        paging_policy_diff_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_policy_diff.unpack(bref); }));
        break;
      }
      case 408: {
        dl_sig_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return dl_sig.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ran_paging_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (paging_policy_diff_present) {
    j.start_obj();
    j.write_int("id", 407);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    paging_policy_diff.to_json(j);
    j.end_obj();
  }
  if (dl_sig_present) {
    j.start_obj();
    j.write_int("id", 408);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// RRCInactiveTransitionReportIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t rrc_inactive_transition_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 92, 121};
  return map_enum_number(names, 4, idx, "id");
}
bool rrc_inactive_transition_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 92, 121};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e rrc_inactive_transition_report_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 92:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e rrc_inactive_transition_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 92:
      return presence_e::mandatory;
    case 121:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE rrc_inactive_transition_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 92, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_state.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE rrc_inactive_transition_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &rrc_inactive_transition_report_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 92: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_state.unpack(bref); }));
        break;
      }
      case 121: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void rrc_inactive_transition_report_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 92);
  j.write_str("criticality", "ignore");
  j.write_str("value", rrc_state.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 121);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  user_location_info.to_json(j);
  j.end_obj();
  j.end_array();
}

// RerouteNASRequest-IEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t reroute_nas_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {85, 10, 42, 3, 0, 171, 414};
  return map_enum_number(names, 7, idx, "id");
}
bool reroute_nas_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {85, 10, 42, 3, 0, 171, 414};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e reroute_nas_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 85:
      return crit_e::reject;
    case 10:
      return crit_e::ignore;
    case 42:
      return crit_e::reject;
    case 3:
      return crit_e::reject;
    case 0:
      return crit_e::reject;
    case 171:
      return crit_e::ignore;
    case 414:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e reroute_nas_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 85:
      return presence_e::mandatory;
    case 10:
      return presence_e::optional;
    case 42:
      return presence_e::mandatory;
    case 3:
      return presence_e::mandatory;
    case 0:
      return presence_e::optional;
    case 171:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE reroute_nas_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += amf_ue_ngap_id_present ? 1 : 0;
  nof_ies += allowed_nssai_present ? 1 : 0;
  nof_ies += source_to_target_amf_info_reroute_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (amf_ue_ngap_id_present) {
    HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 42, crit_e{crit_e::reject}, [this, &bref]() { return ngap_msg.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 3, crit_e{crit_e::reject}, [this, &bref]() { return amf_set_id.pack(bref); }));
  if (allowed_nssai_present) {
    HANDLE_CODE(pack_ie(
        bref, 0, crit_e{crit_e::reject}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  }
  if (source_to_target_amf_info_reroute_present) {
    HANDLE_CODE(pack_ie(
        bref, 171, crit_e{crit_e::ignore}, [this, &bref]() { return source_to_target_amf_info_reroute.pack(bref); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE reroute_nas_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &reroute_nas_request_ies_o::get_crit));

    switch (id) {
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 10: {
        amf_ue_ngap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 42: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ngap_msg.unpack(bref); }));
        break;
      }
      case 3: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return amf_set_id.unpack(bref); }));
        break;
      }
      case 0: {
        allowed_nssai_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 171: {
        source_to_target_amf_info_reroute_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return source_to_target_amf_info_reroute.unpack(bref); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void reroute_nas_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (amf_ue_ngap_id_present) {
    j.start_obj();
    j.write_int("id", 10);
    j.write_str("criticality", "ignore");
    j.write_uint("value", amf_ue_ngap_id);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 42);
  j.write_str("criticality", "reject");
  j.write_str("value", ngap_msg.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 3);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  amf_set_id.to_json(j);
  j.end_obj();
  if (allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 0);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (source_to_target_amf_info_reroute_present) {
    j.start_obj();
    j.write_int("id", 171);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    source_to_target_amf_info_reroute.to_json(j);
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// RetrieveUEInformationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t retrieve_ue_info_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {26};
  return map_enum_number(names, 1, idx, "id");
}
bool retrieve_ue_info_ies_o::is_id_valid(const uint32_t& id)
{
  return 26 == id;
}
crit_e retrieve_ue_info_ies_o::get_crit(const uint32_t& id)
{
  if (id == 26) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e retrieve_ue_info_ies_o::get_presence(const uint32_t& id)
{
  if (id == 26) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE retrieve_ue_info_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::reject}, [this, &bref]() { return five_g_s_tmsi.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE retrieve_ue_info_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &retrieve_ue_info_ies_o::get_crit));

    if (id == 26) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_s_tmsi.unpack(bref); }));
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
void retrieve_ue_info_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 26);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  five_g_s_tmsi.to_json(j);
  j.end_obj();
  j.end_array();
}

// SecondaryRATDataUsageReportIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t secondary_rat_data_usage_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 142, 143, 121};
  return map_enum_number(names, 5, idx, "id");
}
bool secondary_rat_data_usage_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 142, 143, 121};
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
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 142:
      return crit_e::ignore;
    case 143:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e secondary_rat_data_usage_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 142:
      return presence_e::mandatory;
    case 143:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE secondary_rat_data_usage_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ho_flag_present ? 1 : 0;
  nof_ies += user_location_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 142, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_dyn_seq_of(bref, pdu_session_res_secondary_rat_usage_list, 1, 256, true);
  }));
  if (ho_flag_present) {
    HANDLE_CODE(pack_ie(bref, 143, crit_e{crit_e::ignore}, [this, &bref]() { return ho_flag.pack(bref); }));
  }
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }

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
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 142: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_secondary_rat_usage_list, bref, 1, 256, true);
        }));
        break;
      }
      case 143: {
        ho_flag_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ho_flag.unpack(bref); }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 142);
  j.write_str("criticality", "ignore");
  j.start_array("value");
  for (const auto& e1 : pdu_session_res_secondary_rat_usage_list) {
    e1.to_json(j);
  }
  j.end_array();
  j.end_obj();
  if (ho_flag_present) {
    j.start_obj();
    j.write_int("id", 143);
    j.write_str("criticality", "ignore");
    j.write_str("value", "handover-preparation");
    j.end_obj();
  }
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TimingSynchronisationStatusFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t timing_synchronisation_status_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {89, 15, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool timing_synchronisation_status_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {89, 15, 19};
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
    case 89:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e timing_synchronisation_status_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 89:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
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

  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::ignore}, [this, &bref]() { return routing_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &timing_synchronisation_status_fail_ies_o::get_crit));

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return routing_id.unpack(bref); }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void timing_synchronisation_status_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "ignore");
  j.write_str("value", routing_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TimingSynchronisationStatusReportIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t timing_synchronisation_status_report_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {89, 387, 389};
  return map_enum_number(names, 3, idx, "id");
}
bool timing_synchronisation_status_report_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {89, 387, 389};
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
    case 89:
      return crit_e::ignore;
    case 387:
      return crit_e::ignore;
    case 389:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e timing_synchronisation_status_report_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 89:
      return presence_e::mandatory;
    case 387:
      return presence_e::mandatory;
    case 389:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE timing_synchronisation_status_report_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::ignore}, [this, &bref]() { return routing_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 387, crit_e{crit_e::ignore}, [this, &bref]() {
    return ran_timing_synchronisation_status_info.pack(bref);
  }));
  HANDLE_CODE(pack_ie(bref, 389, crit_e{crit_e::ignore}, [this, &bref]() { return ran_tss_scope.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE timing_synchronisation_status_report_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &timing_synchronisation_status_report_ies_o::get_crit));

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return routing_id.unpack(bref); }));
        break;
      }
      case 387: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return ran_timing_synchronisation_status_info.unpack(bref); }));
        break;
      }
      case 389: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_tss_scope.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 89);
  j.write_str("criticality", "ignore");
  j.write_str("value", routing_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 387);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  ran_timing_synchronisation_status_info.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 389);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  ran_tss_scope.to_json(j);
  j.end_obj();
  j.end_array();
}

// TimingSynchronisationStatusRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t timing_synchronisation_status_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {89, 388};
  return map_enum_number(names, 2, idx, "id");
}
bool timing_synchronisation_status_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {89, 388};
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
    case 89:
      return crit_e::reject;
    case 388:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e timing_synchronisation_status_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 89:
      return presence_e::mandatory;
    case 388:
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

  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() { return routing_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 388, crit_e{crit_e::reject}, [this, &bref]() { return ran_tss_request_type.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE timing_synchronisation_status_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &timing_synchronisation_status_request_ies_o::get_crit));

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return routing_id.unpack(bref); }));
        break;
      }
      case 388: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ran_tss_request_type.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_str("value", routing_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 388);
  j.write_str("criticality", "reject");
  j.write_str("value", ran_tss_request_type.to_string());
  j.end_obj();
  j.end_array();
}

// TimingSynchronisationStatusResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t timing_synchronisation_status_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {89, 19};
  return map_enum_number(names, 2, idx, "id");
}
bool timing_synchronisation_status_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {89, 19};
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
    case 89:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e timing_synchronisation_status_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 89:
      return presence_e::mandatory;
    case 19:
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

  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::ignore}, [this, &bref]() { return routing_id.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &timing_synchronisation_status_resp_ies_o::get_crit));

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return routing_id.unpack(bref); }));
        break;
      }
      case 19: {
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
void timing_synchronisation_status_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "ignore");
  j.write_str("value", routing_id.to_string());
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// TraceFailureIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t trace_fail_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 44, 15};
  return map_enum_number(names, 4, idx, "id");
}
bool trace_fail_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 44, 15};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e trace_fail_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 44:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trace_fail_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 44:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE trace_fail_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 44, crit_e{crit_e::ignore}, [this, &bref]() { return ngran_trace_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trace_fail_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &trace_fail_ind_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 44: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ngran_trace_id.unpack(bref); }));
        break;
      }
      case 15: {
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
void trace_fail_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 44);
  j.write_str("criticality", "ignore");
  j.write_str("value", ngran_trace_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// TraceStartIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t trace_start_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 108};
  return map_enum_number(names, 3, idx, "id");
}
bool trace_start_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 108};
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
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 108:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e trace_start_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 108:
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

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 108, crit_e{crit_e::ignore}, [this, &bref]() { return trace_activation.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE trace_start_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &trace_start_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 108: {
        nof_mandatory_ies--;
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
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 108);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  trace_activation.to_json(j);
  j.end_obj();
  j.end_array();
}

// UEContextModificationFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_mod_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 15, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_context_mod_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_mod_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_mod_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_mod_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_mod_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_mod_fail_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void ue_context_mod_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextModificationRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_mod_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10,  85,  83,  94,  31,  110, 119, 18,  24,  40,  91,  162, 165,
                                   177, 199, 216, 215, 218, 217, 219, 264, 238, 326, 328, 329, 335,
                                   359, 345, 346, 347, 367, 373, 374, 375, 376, 377, 378, 400, 430};
  return map_enum_number(names, 39, idx, "id");
}
bool ue_context_mod_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10,  85,  83,  94,  31,  110, 119, 18,  24,  40,  91,  162, 165,
                                   177, 199, 216, 215, 218, 217, 219, 264, 238, 326, 328, 329, 335,
                                   359, 345, 346, 347, 367, 373, 374, 375, 376, 377, 378, 400, 430};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_mod_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 83:
      return crit_e::ignore;
    case 94:
      return crit_e::reject;
    case 31:
      return crit_e::ignore;
    case 110:
      return crit_e::ignore;
    case 119:
      return crit_e::reject;
    case 18:
      return crit_e::ignore;
    case 24:
      return crit_e::reject;
    case 40:
      return crit_e::reject;
    case 91:
      return crit_e::ignore;
    case 162:
      return crit_e::reject;
    case 165:
      return crit_e::ignore;
    case 177:
      return crit_e::ignore;
    case 199:
      return crit_e::ignore;
    case 216:
      return crit_e::ignore;
    case 215:
      return crit_e::ignore;
    case 218:
      return crit_e::ignore;
    case 217:
      return crit_e::ignore;
    case 219:
      return crit_e::ignore;
    case 264:
      return crit_e::reject;
    case 238:
      return crit_e::ignore;
    case 326:
      return crit_e::ignore;
    case 328:
      return crit_e::ignore;
    case 329:
      return crit_e::ignore;
    case 335:
      return crit_e::ignore;
    case 359:
      return crit_e::ignore;
    case 345:
      return crit_e::ignore;
    case 346:
      return crit_e::ignore;
    case 347:
      return crit_e::ignore;
    case 367:
      return crit_e::ignore;
    case 373:
      return crit_e::ignore;
    case 374:
      return crit_e::ignore;
    case 375:
      return crit_e::ignore;
    case 376:
      return crit_e::ignore;
    case 377:
      return crit_e::ignore;
    case 378:
      return crit_e::ignore;
    case 400:
      return crit_e::ignore;
    case 430:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_mod_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 83:
      return presence_e::optional;
    case 94:
      return presence_e::optional;
    case 31:
      return presence_e::optional;
    case 110:
      return presence_e::optional;
    case 119:
      return presence_e::optional;
    case 18:
      return presence_e::optional;
    case 24:
      return presence_e::optional;
    case 40:
      return presence_e::optional;
    case 91:
      return presence_e::optional;
    case 162:
      return presence_e::optional;
    case 165:
      return presence_e::optional;
    case 177:
      return presence_e::optional;
    case 199:
      return presence_e::optional;
    case 216:
      return presence_e::optional;
    case 215:
      return presence_e::optional;
    case 218:
      return presence_e::optional;
    case 217:
      return presence_e::optional;
    case 219:
      return presence_e::optional;
    case 264:
      return presence_e::optional;
    case 238:
      return presence_e::optional;
    case 326:
      return presence_e::optional;
    case 328:
      return presence_e::optional;
    case 329:
      return presence_e::optional;
    case 335:
      return presence_e::optional;
    case 359:
      return presence_e::optional;
    case 345:
      return presence_e::optional;
    case 346:
      return presence_e::optional;
    case 347:
      return presence_e::optional;
    case 367:
      return presence_e::optional;
    case 373:
      return presence_e::optional;
    case 374:
      return presence_e::optional;
    case 375:
      return presence_e::optional;
    case 376:
      return presence_e::optional;
    case 377:
      return presence_e::optional;
    case 378:
      return presence_e::optional;
    case 400:
      return presence_e::optional;
    case 430:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_mod_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ran_paging_prio_present ? 1 : 0;
  nof_ies += security_key_present ? 1 : 0;
  nof_ies += idx_to_rfsp_present ? 1 : 0;
  nof_ies += ue_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += ue_security_cap_present ? 1 : 0;
  nof_ies += core_network_assist_info_for_inactive_present ? 1 : 0;
  nof_ies += emergency_fallback_ind_present ? 1 : 0;
  nof_ies += new_amf_ue_ngap_id_present ? 1 : 0;
  nof_ies += rrc_inactive_transition_report_request_present ? 1 : 0;
  nof_ies += new_guami_present ? 1 : 0;
  nof_ies += cn_assisted_ran_tuning_present ? 1 : 0;
  nof_ies += srvcc_operation_possible_present ? 1 : 0;
  nof_ies += iab_authorized_present ? 1 : 0;
  nof_ies += nr_v2x_services_authorized_present ? 1 : 0;
  nof_ies += ltev2x_services_authorized_present ? 1 : 0;
  nof_ies += nr_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += lte_ue_sidelink_aggr_max_bitrate_present ? 1 : 0;
  nof_ies += pc5_qos_params_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  nof_ies += rg_level_wireline_access_characteristics_present ? 1 : 0;
  nof_ies += time_sync_assist_info_present ? 1 : 0;
  nof_ies += q_mcc_onfig_info_present ? 1 : 0;
  nof_ies += qmc_deactivation_present ? 1 : 0;
  nof_ies += ue_slice_max_bit_rate_list_present ? 1 : 0;
  nof_ies += management_based_mdt_plmn_mod_list_present ? 1 : 0;
  nof_ies += five_g_pro_se_authorized_present ? 1 : 0;
  nof_ies += five_g_pro_se_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += five_g_pro_se_pc5_qos_params_present ? 1 : 0;
  nof_ies += network_ctrl_repeater_authorized_present ? 1 : 0;
  nof_ies += aerial_ue_sub_info_present ? 1 : 0;
  nof_ies += nr_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += lte_a2_x_services_authorized_present ? 1 : 0;
  nof_ies += nr_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += lte_a2_x_ue_pc5_aggr_max_bit_rate_present ? 1 : 0;
  nof_ies += a2_x_pc5_qos_params_present ? 1 : 0;
  nof_ies += mobile_iab_authorized_present ? 1 : 0;
  nof_ies += sl_positioning_ranging_service_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ran_paging_prio_present) {
    HANDLE_CODE(pack_ie(bref, 83, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, ran_paging_prio, (uint16_t)1u, (uint16_t)256u, false, true);
    }));
  }
  if (security_key_present) {
    HANDLE_CODE(pack_ie(bref, 94, crit_e{crit_e::reject}, [this, &bref]() { return security_key.pack(bref); }));
  }
  if (idx_to_rfsp_present) {
    HANDLE_CODE(pack_ie(bref, 31, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, idx_to_rfsp, (uint16_t)1u, (uint16_t)256u, true, true);
    }));
  }
  if (ue_aggr_max_bit_rate_present) {
    HANDLE_CODE(
        pack_ie(bref, 110, crit_e{crit_e::ignore}, [this, &bref]() { return ue_aggr_max_bit_rate.pack(bref); }));
  }
  if (ue_security_cap_present) {
    HANDLE_CODE(pack_ie(bref, 119, crit_e{crit_e::reject}, [this, &bref]() { return ue_security_cap.pack(bref); }));
  }
  if (core_network_assist_info_for_inactive_present) {
    HANDLE_CODE(pack_ie(bref, 18, crit_e{crit_e::ignore}, [this, &bref]() {
      return core_network_assist_info_for_inactive.pack(bref);
    }));
  }
  if (emergency_fallback_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 24, crit_e{crit_e::reject}, [this, &bref]() { return emergency_fallback_ind.pack(bref); }));
  }
  if (new_amf_ue_ngap_id_present) {
    HANDLE_CODE(pack_ie(bref, 40, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_integer(bref, new_amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
    }));
  }
  if (rrc_inactive_transition_report_request_present) {
    HANDLE_CODE(pack_ie(bref, 91, crit_e{crit_e::ignore}, [this, &bref]() {
      return rrc_inactive_transition_report_request.pack(bref);
    }));
  }
  if (new_guami_present) {
    HANDLE_CODE(pack_ie(bref, 162, crit_e{crit_e::reject}, [this, &bref]() { return new_guami.pack(bref); }));
  }
  if (cn_assisted_ran_tuning_present) {
    HANDLE_CODE(
        pack_ie(bref, 165, crit_e{crit_e::ignore}, [this, &bref]() { return cn_assisted_ran_tuning.pack(bref); }));
  }
  if (srvcc_operation_possible_present) {
    HANDLE_CODE(
        pack_ie(bref, 177, crit_e{crit_e::ignore}, [this, &bref]() { return srvcc_operation_possible.pack(bref); }));
  }
  if (iab_authorized_present) {
    HANDLE_CODE(pack_ie(bref, 199, crit_e{crit_e::ignore}, [this, &bref]() { return iab_authorized.pack(bref); }));
  }
  if (nr_v2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 216, crit_e{crit_e::ignore}, [this, &bref]() { return nr_v2x_services_authorized.pack(bref); }));
  }
  if (ltev2x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 215, crit_e{crit_e::ignore}, [this, &bref]() { return ltev2x_services_authorized.pack(bref); }));
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 218, crit_e{crit_e::ignore}, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    HANDLE_CODE(pack_ie(
        bref, 217, crit_e{crit_e::ignore}, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.pack(bref); }));
  }
  if (pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 219, crit_e{crit_e::ignore}, [this, &bref]() { return pc5_qos_params.pack(bref); }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }
  if (rg_level_wireline_access_characteristics_present) {
    HANDLE_CODE(pack_ie(bref, 238, crit_e{crit_e::ignore}, [this, &bref]() {
      return rg_level_wireline_access_characteristics.pack(bref);
    }));
  }
  if (time_sync_assist_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 326, crit_e{crit_e::ignore}, [this, &bref]() { return time_sync_assist_info.pack(bref); }));
  }
  if (q_mcc_onfig_info_present) {
    HANDLE_CODE(pack_ie(bref, 328, crit_e{crit_e::ignore}, [this, &bref]() { return q_mcc_onfig_info.pack(bref); }));
  }
  if (qmc_deactivation_present) {
    HANDLE_CODE(pack_ie(bref, 329, crit_e{crit_e::ignore}, [this, &bref]() { return qmc_deactivation.pack(bref); }));
  }
  if (ue_slice_max_bit_rate_list_present) {
    HANDLE_CODE(pack_ie(bref, 335, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, ue_slice_max_bit_rate_list, 1, 8, true);
    }));
  }
  if (management_based_mdt_plmn_mod_list_present) {
    HANDLE_CODE(pack_ie(bref, 359, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, management_based_mdt_plmn_mod_list, 0, 16, true);
    }));
  }
  if (five_g_pro_se_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 345, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_authorized.pack(bref); }));
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(bref, 346, crit_e{crit_e::ignore}, [this, &bref]() {
      return five_g_pro_se_ue_pc5_aggr_max_bit_rate.pack(bref);
    }));
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(
        bref, 347, crit_e{crit_e::ignore}, [this, &bref]() { return five_g_pro_se_pc5_qos_params.pack(bref); }));
  }
  if (network_ctrl_repeater_authorized_present) {
    HANDLE_CODE(pack_ie(
        bref, 367, crit_e{crit_e::ignore}, [this, &bref]() { return network_ctrl_repeater_authorized.pack(bref); }));
  }
  if (aerial_ue_sub_info_present) {
    HANDLE_CODE(pack_ie(bref, 373, crit_e{crit_e::ignore}, [this, &bref]() { return aerial_ue_sub_info.pack(bref); }));
  }
  if (nr_a2_x_services_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 374, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_services_authorized.pack(bref); }));
  }
  if (lte_a2_x_services_authorized_present) {
    HANDLE_CODE(pack_ie(
        bref, 375, crit_e{crit_e::ignore}, [this, &bref]() { return lte_a2_x_services_authorized.pack(bref); }));
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 376, crit_e{crit_e::ignore}, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (lte_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    HANDLE_CODE(pack_ie(
        bref, 377, crit_e{crit_e::ignore}, [this, &bref]() { return lte_a2_x_ue_pc5_aggr_max_bit_rate.pack(bref); }));
  }
  if (a2_x_pc5_qos_params_present) {
    HANDLE_CODE(pack_ie(bref, 378, crit_e{crit_e::ignore}, [this, &bref]() { return a2_x_pc5_qos_params.pack(bref); }));
  }
  if (mobile_iab_authorized_present) {
    HANDLE_CODE(
        pack_ie(bref, 400, crit_e{crit_e::ignore}, [this, &bref]() { return mobile_iab_authorized.pack(bref); }));
  }
  if (sl_positioning_ranging_service_info_present) {
    HANDLE_CODE(pack_ie(
        bref, 430, crit_e{crit_e::ignore}, [this, &bref]() { return sl_positioning_ranging_service_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_mod_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_mod_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 83: {
        ran_paging_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_paging_prio, bref, (uint16_t)1u, (uint16_t)256u, false, true);
        }));
        break;
      }
      case 94: {
        security_key_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return security_key.unpack(bref); }));
        break;
      }
      case 31: {
        idx_to_rfsp_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(idx_to_rfsp, bref, (uint16_t)1u, (uint16_t)256u, true, true);
        }));
        break;
      }
      case 110: {
        ue_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 119: {
        ue_security_cap_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_security_cap.unpack(bref); }));
        break;
      }
      case 18: {
        core_network_assist_info_for_inactive_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return core_network_assist_info_for_inactive.unpack(bref); }));
        break;
      }
      case 24: {
        emergency_fallback_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return emergency_fallback_ind.unpack(bref); }));
        break;
      }
      case 40: {
        new_amf_ue_ngap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(new_amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 91: {
        rrc_inactive_transition_report_request_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return rrc_inactive_transition_report_request.unpack(bref); }));
        break;
      }
      case 162: {
        new_guami_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return new_guami.unpack(bref); }));
        break;
      }
      case 165: {
        cn_assisted_ran_tuning_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cn_assisted_ran_tuning.unpack(bref); }));
        break;
      }
      case 177: {
        srvcc_operation_possible_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return srvcc_operation_possible.unpack(bref); }));
        break;
      }
      case 199: {
        iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return iab_authorized.unpack(bref); }));
        break;
      }
      case 216: {
        nr_v2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_v2x_services_authorized.unpack(bref); }));
        break;
      }
      case 215: {
        ltev2x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ltev2x_services_authorized.unpack(bref); }));
        break;
      }
      case 218: {
        nr_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 217: {
        lte_ue_sidelink_aggr_max_bitrate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_ue_sidelink_aggr_max_bitrate.unpack(bref); }));
        break;
      }
      case 219: {
        pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return pc5_qos_params.unpack(bref); }));
        break;
      }
      case 264: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 238: {
        rg_level_wireline_access_characteristics_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return rg_level_wireline_access_characteristics.unpack(bref); }));
        break;
      }
      case 326: {
        time_sync_assist_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return time_sync_assist_info.unpack(bref); }));
        break;
      }
      case 328: {
        q_mcc_onfig_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return q_mcc_onfig_info.unpack(bref); }));
        break;
      }
      case 329: {
        qmc_deactivation_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return qmc_deactivation.unpack(bref); }));
        break;
      }
      case 335: {
        ue_slice_max_bit_rate_list_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(ue_slice_max_bit_rate_list, bref, 1, 8, true); }));
        break;
      }
      case 359: {
        management_based_mdt_plmn_mod_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(management_based_mdt_plmn_mod_list, bref, 0, 16, true);
        }));
        break;
      }
      case 345: {
        five_g_pro_se_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_authorized.unpack(bref); }));
        break;
      }
      case 346: {
        five_g_pro_se_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 347: {
        five_g_pro_se_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_pro_se_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 367: {
        network_ctrl_repeater_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return network_ctrl_repeater_authorized.unpack(bref); }));
        break;
      }
      case 373: {
        aerial_ue_sub_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return aerial_ue_sub_info.unpack(bref); }));
        break;
      }
      case 374: {
        nr_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 375: {
        lte_a2_x_services_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_a2_x_services_authorized.unpack(bref); }));
        break;
      }
      case 376: {
        nr_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nr_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 377: {
        lte_a2_x_ue_pc5_aggr_max_bit_rate_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return lte_a2_x_ue_pc5_aggr_max_bit_rate.unpack(bref); }));
        break;
      }
      case 378: {
        a2_x_pc5_qos_params_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return a2_x_pc5_qos_params.unpack(bref); }));
        break;
      }
      case 400: {
        mobile_iab_authorized_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return mobile_iab_authorized.unpack(bref); }));
        break;
      }
      case 430: {
        sl_positioning_ranging_service_info_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return sl_positioning_ranging_service_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_mod_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (ran_paging_prio_present) {
    j.start_obj();
    j.write_int("id", 83);
    j.write_str("criticality", "ignore");
    j.write_int("value", ran_paging_prio);
    j.end_obj();
  }
  if (security_key_present) {
    j.start_obj();
    j.write_int("id", 94);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    security_key.to_json(j);
    j.end_obj();
  }
  if (idx_to_rfsp_present) {
    j.start_obj();
    j.write_int("id", 31);
    j.write_str("criticality", "ignore");
    j.write_int("value", idx_to_rfsp);
    j.end_obj();
  }
  if (ue_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 110);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (ue_security_cap_present) {
    j.start_obj();
    j.write_int("id", 119);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    ue_security_cap.to_json(j);
    j.end_obj();
  }
  if (core_network_assist_info_for_inactive_present) {
    j.start_obj();
    j.write_int("id", 18);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    core_network_assist_info_for_inactive.to_json(j);
    j.end_obj();
  }
  if (emergency_fallback_ind_present) {
    j.start_obj();
    j.write_int("id", 24);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    emergency_fallback_ind.to_json(j);
    j.end_obj();
  }
  if (new_amf_ue_ngap_id_present) {
    j.start_obj();
    j.write_int("id", 40);
    j.write_str("criticality", "reject");
    j.write_uint("value", new_amf_ue_ngap_id);
    j.end_obj();
  }
  if (rrc_inactive_transition_report_request_present) {
    j.start_obj();
    j.write_int("id", 91);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_inactive_transition_report_request.to_string());
    j.end_obj();
  }
  if (new_guami_present) {
    j.start_obj();
    j.write_int("id", 162);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    new_guami.to_json(j);
    j.end_obj();
  }
  if (cn_assisted_ran_tuning_present) {
    j.start_obj();
    j.write_int("id", 165);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    cn_assisted_ran_tuning.to_json(j);
    j.end_obj();
  }
  if (srvcc_operation_possible_present) {
    j.start_obj();
    j.write_int("id", 177);
    j.write_str("criticality", "ignore");
    j.write_str("value", srvcc_operation_possible.to_string());
    j.end_obj();
  }
  if (iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 199);
    j.write_str("criticality", "ignore");
    j.write_str("value", iab_authorized.to_string());
    j.end_obj();
  }
  if (nr_v2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 216);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_v2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (ltev2x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 215);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ltev2x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 218);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (lte_ue_sidelink_aggr_max_bitrate_present) {
    j.start_obj();
    j.write_int("id", 217);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_ue_sidelink_aggr_max_bitrate.to_json(j);
    j.end_obj();
  }
  if (pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 219);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 264);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  if (rg_level_wireline_access_characteristics_present) {
    j.start_obj();
    j.write_int("id", 238);
    j.write_str("criticality", "ignore");
    j.write_str("value", rg_level_wireline_access_characteristics.to_string());
    j.end_obj();
  }
  if (time_sync_assist_info_present) {
    j.start_obj();
    j.write_int("id", 326);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    time_sync_assist_info.to_json(j);
    j.end_obj();
  }
  if (q_mcc_onfig_info_present) {
    j.start_obj();
    j.write_int("id", 328);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    q_mcc_onfig_info.to_json(j);
    j.end_obj();
  }
  if (qmc_deactivation_present) {
    j.start_obj();
    j.write_int("id", 329);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    qmc_deactivation.to_json(j);
    j.end_obj();
  }
  if (ue_slice_max_bit_rate_list_present) {
    j.start_obj();
    j.write_int("id", 335);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : ue_slice_max_bit_rate_list) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (management_based_mdt_plmn_mod_list_present) {
    j.start_obj();
    j.write_int("id", 359);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : management_based_mdt_plmn_mod_list) {
      j.write_str(e1.to_string());
    }
    j.end_array();
    j.end_obj();
  }
  if (five_g_pro_se_authorized_present) {
    j.start_obj();
    j.write_int("id", 345);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_authorized.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 346);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (five_g_pro_se_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 347);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    five_g_pro_se_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (network_ctrl_repeater_authorized_present) {
    j.start_obj();
    j.write_int("id", 367);
    j.write_str("criticality", "ignore");
    j.write_str("value", network_ctrl_repeater_authorized.to_string());
    j.end_obj();
  }
  if (aerial_ue_sub_info_present) {
    j.start_obj();
    j.write_int("id", 373);
    j.write_str("criticality", "ignore");
    j.write_str("value", aerial_ue_sub_info.to_string());
    j.end_obj();
  }
  if (nr_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 374);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (lte_a2_x_services_authorized_present) {
    j.start_obj();
    j.write_int("id", 375);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_a2_x_services_authorized.to_json(j);
    j.end_obj();
  }
  if (nr_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 376);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    nr_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (lte_a2_x_ue_pc5_aggr_max_bit_rate_present) {
    j.start_obj();
    j.write_int("id", 377);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    lte_a2_x_ue_pc5_aggr_max_bit_rate.to_json(j);
    j.end_obj();
  }
  if (a2_x_pc5_qos_params_present) {
    j.start_obj();
    j.write_int("id", 378);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    a2_x_pc5_qos_params.to_json(j);
    j.end_obj();
  }
  if (mobile_iab_authorized_present) {
    j.start_obj();
    j.write_int("id", 400);
    j.write_str("criticality", "ignore");
    j.write_str("value", mobile_iab_authorized.to_string());
    j.end_obj();
  }
  if (sl_positioning_ranging_service_info_present) {
    j.start_obj();
    j.write_int("id", 430);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    sl_positioning_ranging_service_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextModificationResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_mod_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 92, 121, 19};
  return map_enum_number(names, 5, idx, "id");
}
bool ue_context_mod_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 92, 121, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_mod_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 92:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_mod_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 92:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_mod_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += rrc_state_present ? 1 : 0;
  nof_ies += user_location_info_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (rrc_state_present) {
    HANDLE_CODE(pack_ie(bref, 92, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_state.pack(bref); }));
  }
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_mod_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_mod_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 92: {
        rrc_state_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_state.unpack(bref); }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 19: {
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
void ue_context_mod_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (rrc_state_present) {
    j.start_obj();
    j.write_int("id", 92);
    j.write_str("criticality", "ignore");
    j.write_str("value", rrc_state.to_string());
    j.end_obj();
  }
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextReleaseCommand-IEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_release_cmd_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {114, 15};
  return map_enum_number(names, 2, idx, "id");
}
bool ue_context_release_cmd_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {114, 15};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_release_cmd_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 114:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_release_cmd_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 114:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_release_cmd_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 114, crit_e{crit_e::reject}, [this, &bref]() { return ue_ngap_ids.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_release_cmd_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_release_cmd_ies_o::get_crit));

    switch (id) {
      case 114: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_ngap_ids.unpack(bref); }));
        break;
      }
      case 15: {
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
void ue_context_release_cmd_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 114);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ue_ngap_ids.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// UEContextReleaseComplete-IEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_release_complete_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 121, 32, 60, 19, 207};
  return map_enum_number(names, 7, idx, "id");
}
bool ue_context_release_complete_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 121, 32, 60, 19, 207};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_release_complete_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    case 32:
      return crit_e::ignore;
    case 60:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    case 207:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_release_complete_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 121:
      return presence_e::optional;
    case 32:
      return presence_e::optional;
    case 60:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    case 207:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_release_complete_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += user_location_info_present ? 1 : 0;
  nof_ies += info_on_recommended_cells_and_ran_nodes_for_paging_present ? 1 : 0;
  nof_ies += pdu_session_res_list_cxt_rel_cpl_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  nof_ies += paging_assis_datafor_c_ecapab_ue_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }
  if (info_on_recommended_cells_and_ran_nodes_for_paging_present) {
    HANDLE_CODE(pack_ie(bref, 32, crit_e{crit_e::ignore}, [this, &bref]() {
      return info_on_recommended_cells_and_ran_nodes_for_paging.pack(bref);
    }));
  }
  if (pdu_session_res_list_cxt_rel_cpl_present) {
    HANDLE_CODE(pack_ie(bref, 60, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_list_cxt_rel_cpl, 1, 256, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }
  if (paging_assis_datafor_c_ecapab_ue_present) {
    HANDLE_CODE(pack_ie(
        bref, 207, crit_e{crit_e::ignore}, [this, &bref]() { return paging_assis_datafor_c_ecapab_ue.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_release_complete_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_release_complete_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 32: {
        info_on_recommended_cells_and_ran_nodes_for_paging_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return info_on_recommended_cells_and_ran_nodes_for_paging.unpack(bref); }));
        break;
      }
      case 60: {
        pdu_session_res_list_cxt_rel_cpl_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_list_cxt_rel_cpl, bref, 1, 256, true); }));
        break;
      }
      case 19: {
        crit_diagnostics_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return crit_diagnostics.unpack(bref); }));
        break;
      }
      case 207: {
        paging_assis_datafor_c_ecapab_ue_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_assis_datafor_c_ecapab_ue.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_release_complete_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  if (info_on_recommended_cells_and_ran_nodes_for_paging_present) {
    j.start_obj();
    j.write_int("id", 32);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    info_on_recommended_cells_and_ran_nodes_for_paging.to_json(j);
    j.end_obj();
  }
  if (pdu_session_res_list_cxt_rel_cpl_present) {
    j.start_obj();
    j.write_int("id", 60);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_list_cxt_rel_cpl) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  if (paging_assis_datafor_c_ecapab_ue_present) {
    j.start_obj();
    j.write_int("id", 207);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    paging_assis_datafor_c_ecapab_ue.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextReleaseRequest-IEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 133, 15};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_context_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 133, 15};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 133:
      return crit_e::reject;
    case 15:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 133:
      return presence_e::optional;
    case 15:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pdu_session_res_list_cxt_rel_req_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_list_cxt_rel_req_present) {
    HANDLE_CODE(pack_ie(bref, 133, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_list_cxt_rel_req, 1, 256, true);
    }));
  }
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_release_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 133: {
        pdu_session_res_list_cxt_rel_req_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(pdu_session_res_list_cxt_rel_req, bref, 1, 256, true); }));
        break;
      }
      case 15: {
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
void ue_context_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (pdu_session_res_list_cxt_rel_req_present) {
    j.start_obj();
    j.write_int("id", 133);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_list_cxt_rel_req) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  j.end_array();
}

// UEContextResumeFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_resume_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 15, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_context_resume_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_resume_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_resume_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_resume_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_resume_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_resume_fail_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void ue_context_resume_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextResumeRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_resume_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 237, 232, 229, 235, 32, 207, 121};
  return map_enum_number(names, 9, idx, "id");
}
bool ue_context_resume_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 237, 232, 229, 235, 32, 207, 121};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_resume_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 237:
      return crit_e::ignore;
    case 232:
      return crit_e::reject;
    case 229:
      return crit_e::reject;
    case 235:
      return crit_e::ignore;
    case 32:
      return crit_e::ignore;
    case 207:
      return crit_e::ignore;
    case 121:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_resume_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 237:
      return presence_e::mandatory;
    case 232:
      return presence_e::optional;
    case 229:
      return presence_e::optional;
    case 235:
      return presence_e::optional;
    case 32:
      return presence_e::optional;
    case 207:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_resume_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += pdu_session_res_resume_list_res_req_present ? 1 : 0;
  nof_ies += pdu_session_res_failed_to_resume_list_res_req_present ? 1 : 0;
  nof_ies += suspend_request_ind_present ? 1 : 0;
  nof_ies += info_on_recommended_cells_and_ran_nodes_for_paging_present ? 1 : 0;
  nof_ies += paging_assis_datafor_c_ecapab_ue_present ? 1 : 0;
  nof_ies += user_location_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 237, crit_e{crit_e::ignore}, [this, &bref]() { return rrc_resume_cause.pack(bref); }));
  if (pdu_session_res_resume_list_res_req_present) {
    HANDLE_CODE(pack_ie(bref, 232, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_resume_list_res_req, 1, 256, true);
    }));
  }
  if (pdu_session_res_failed_to_resume_list_res_req_present) {
    HANDLE_CODE(pack_ie(bref, 229, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_resume_list_res_req, 1, 256, true);
    }));
  }
  if (suspend_request_ind_present) {
    HANDLE_CODE(pack_ie(bref, 235, crit_e{crit_e::ignore}, [this, &bref]() { return suspend_request_ind.pack(bref); }));
  }
  if (info_on_recommended_cells_and_ran_nodes_for_paging_present) {
    HANDLE_CODE(pack_ie(bref, 32, crit_e{crit_e::ignore}, [this, &bref]() {
      return info_on_recommended_cells_and_ran_nodes_for_paging.pack(bref);
    }));
  }
  if (paging_assis_datafor_c_ecapab_ue_present) {
    HANDLE_CODE(pack_ie(
        bref, 207, crit_e{crit_e::ignore}, [this, &bref]() { return paging_assis_datafor_c_ecapab_ue.pack(bref); }));
  }
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_resume_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_resume_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 237: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rrc_resume_cause.unpack(bref); }));
        break;
      }
      case 232: {
        pdu_session_res_resume_list_res_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_resume_list_res_req, bref, 1, 256, true);
        }));
        break;
      }
      case 229: {
        pdu_session_res_failed_to_resume_list_res_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_resume_list_res_req, bref, 1, 256, true);
        }));
        break;
      }
      case 235: {
        suspend_request_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return suspend_request_ind.unpack(bref); }));
        break;
      }
      case 32: {
        info_on_recommended_cells_and_ran_nodes_for_paging_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return info_on_recommended_cells_and_ran_nodes_for_paging.unpack(bref); }));
        break;
      }
      case 207: {
        paging_assis_datafor_c_ecapab_ue_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_assis_datafor_c_ecapab_ue.unpack(bref); }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_resume_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 237);
  j.write_str("criticality", "ignore");
  j.write_str("value", rrc_resume_cause.to_string());
  j.end_obj();
  if (pdu_session_res_resume_list_res_req_present) {
    j.start_obj();
    j.write_int("id", 232);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_resume_list_res_req) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_failed_to_resume_list_res_req_present) {
    j.start_obj();
    j.write_int("id", 229);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_resume_list_res_req) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (suspend_request_ind_present) {
    j.start_obj();
    j.write_int("id", 235);
    j.write_str("criticality", "ignore");
    j.write_str("value", "suspend-requested");
    j.end_obj();
  }
  if (info_on_recommended_cells_and_ran_nodes_for_paging_present) {
    j.start_obj();
    j.write_int("id", 32);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    info_on_recommended_cells_and_ran_nodes_for_paging.to_json(j);
    j.end_obj();
  }
  if (paging_assis_datafor_c_ecapab_ue_present) {
    j.start_obj();
    j.write_int("id", 207);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    paging_assis_datafor_c_ecapab_ue.to_json(j);
    j.end_obj();
  }
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextResumeResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_resume_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 233, 230, 93, 236, 206, 19};
  return map_enum_number(names, 8, idx, "id");
}
bool ue_context_resume_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 233, 230, 93, 236, 206, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_resume_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 233:
      return crit_e::reject;
    case 230:
      return crit_e::reject;
    case 93:
      return crit_e::reject;
    case 236:
      return crit_e::ignore;
    case 206:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_resume_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 233:
      return presence_e::optional;
    case 230:
      return presence_e::optional;
    case 93:
      return presence_e::optional;
    case 236:
      return presence_e::optional;
    case 206:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_resume_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += pdu_session_res_resume_list_res_res_present ? 1 : 0;
  nof_ies += pdu_session_res_failed_to_resume_list_res_res_present ? 1 : 0;
  nof_ies += security_context_present ? 1 : 0;
  nof_ies += suspend_resp_ind_present ? 1 : 0;
  nof_ies += extended_connected_time_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (pdu_session_res_resume_list_res_res_present) {
    HANDLE_CODE(pack_ie(bref, 233, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_resume_list_res_res, 1, 256, true);
    }));
  }
  if (pdu_session_res_failed_to_resume_list_res_res_present) {
    HANDLE_CODE(pack_ie(bref, 230, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_failed_to_resume_list_res_res, 1, 256, true);
    }));
  }
  if (security_context_present) {
    HANDLE_CODE(pack_ie(bref, 93, crit_e{crit_e::reject}, [this, &bref]() { return security_context.pack(bref); }));
  }
  if (suspend_resp_ind_present) {
    HANDLE_CODE(pack_ie(bref, 236, crit_e{crit_e::ignore}, [this, &bref]() { return suspend_resp_ind.pack(bref); }));
  }
  if (extended_connected_time_present) {
    HANDLE_CODE(pack_ie(bref, 206, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, extended_connected_time, (uint16_t)0u, (uint16_t)255u, false, true);
    }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_resume_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_resume_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 233: {
        pdu_session_res_resume_list_res_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_resume_list_res_res, bref, 1, 256, true);
        }));
        break;
      }
      case 230: {
        pdu_session_res_failed_to_resume_list_res_res_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_failed_to_resume_list_res_res, bref, 1, 256, true);
        }));
        break;
      }
      case 93: {
        security_context_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return security_context.unpack(bref); }));
        break;
      }
      case 236: {
        suspend_resp_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return suspend_resp_ind.unpack(bref); }));
        break;
      }
      case 206: {
        extended_connected_time_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(extended_connected_time, bref, (uint16_t)0u, (uint16_t)255u, false, true);
        }));
        break;
      }
      case 19: {
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
void ue_context_resume_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (pdu_session_res_resume_list_res_res_present) {
    j.start_obj();
    j.write_int("id", 233);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_resume_list_res_res) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (pdu_session_res_failed_to_resume_list_res_res_present) {
    j.start_obj();
    j.write_int("id", 230);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_failed_to_resume_list_res_res) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (security_context_present) {
    j.start_obj();
    j.write_int("id", 93);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    security_context.to_json(j);
    j.end_obj();
  }
  if (suspend_resp_ind_present) {
    j.start_obj();
    j.write_int("id", 236);
    j.write_str("criticality", "ignore");
    j.write_str("value", "suspend-indicated");
    j.end_obj();
  }
  if (extended_connected_time_present) {
    j.start_obj();
    j.write_int("id", 206);
    j.write_str("criticality", "ignore");
    j.write_int("value", extended_connected_time);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextSuspendFailureIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_suspend_fail_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 15, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_context_suspend_fail_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 15, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_suspend_fail_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 15:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_suspend_fail_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 15:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_suspend_fail_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 15, crit_e{crit_e::ignore}, [this, &bref]() { return cause.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_suspend_fail_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_suspend_fail_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 15: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return cause.unpack(bref); }));
        break;
      }
      case 19: {
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
void ue_context_suspend_fail_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 15);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  cause.to_json(j);
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextSuspendRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_suspend_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 32, 207, 231, 121};
  return map_enum_number(names, 6, idx, "id");
}
bool ue_context_suspend_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 32, 207, 231, 121};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_suspend_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 32:
      return crit_e::ignore;
    case 207:
      return crit_e::ignore;
    case 231:
      return crit_e::reject;
    case 121:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_suspend_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 32:
      return presence_e::optional;
    case 207:
      return presence_e::optional;
    case 231:
      return presence_e::optional;
    case 121:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_suspend_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += info_on_recommended_cells_and_ran_nodes_for_paging_present ? 1 : 0;
  nof_ies += paging_assis_datafor_c_ecapab_ue_present ? 1 : 0;
  nof_ies += pdu_session_res_suspend_list_sus_req_present ? 1 : 0;
  nof_ies += user_location_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (info_on_recommended_cells_and_ran_nodes_for_paging_present) {
    HANDLE_CODE(pack_ie(bref, 32, crit_e{crit_e::ignore}, [this, &bref]() {
      return info_on_recommended_cells_and_ran_nodes_for_paging.pack(bref);
    }));
  }
  if (paging_assis_datafor_c_ecapab_ue_present) {
    HANDLE_CODE(pack_ie(
        bref, 207, crit_e{crit_e::ignore}, [this, &bref]() { return paging_assis_datafor_c_ecapab_ue.pack(bref); }));
  }
  if (pdu_session_res_suspend_list_sus_req_present) {
    HANDLE_CODE(pack_ie(bref, 231, crit_e{crit_e::reject}, [this, &bref]() {
      return pack_dyn_seq_of(bref, pdu_session_res_suspend_list_sus_req, 1, 256, true);
    }));
  }
  if (user_location_info_present) {
    HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_suspend_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_suspend_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 32: {
        info_on_recommended_cells_and_ran_nodes_for_paging_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return info_on_recommended_cells_and_ran_nodes_for_paging.unpack(bref); }));
        break;
      }
      case 207: {
        paging_assis_datafor_c_ecapab_ue_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return paging_assis_datafor_c_ecapab_ue.unpack(bref); }));
        break;
      }
      case 231: {
        pdu_session_res_suspend_list_sus_req_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_dyn_seq_of(pdu_session_res_suspend_list_sus_req, bref, 1, 256, true);
        }));
        break;
      }
      case 121: {
        user_location_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_context_suspend_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (info_on_recommended_cells_and_ran_nodes_for_paging_present) {
    j.start_obj();
    j.write_int("id", 32);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    info_on_recommended_cells_and_ran_nodes_for_paging.to_json(j);
    j.end_obj();
  }
  if (paging_assis_datafor_c_ecapab_ue_present) {
    j.start_obj();
    j.write_int("id", 207);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    paging_assis_datafor_c_ecapab_ue.to_json(j);
    j.end_obj();
  }
  if (pdu_session_res_suspend_list_sus_req_present) {
    j.start_obj();
    j.write_int("id", 231);
    j.write_str("criticality", "reject");
    j.start_array("value");
    for (const auto& e1 : pdu_session_res_suspend_list_sus_req) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (user_location_info_present) {
    j.start_obj();
    j.write_int("id", 121);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    user_location_info.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEContextSuspendResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_context_suspend_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 93, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_context_suspend_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 93, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_context_suspend_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 93:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_context_suspend_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 93:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_context_suspend_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += security_context_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (security_context_present) {
    HANDLE_CODE(pack_ie(bref, 93, crit_e{crit_e::reject}, [this, &bref]() { return security_context.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_context_suspend_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_context_suspend_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 93: {
        security_context_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return security_context.unpack(bref); }));
        break;
      }
      case 19: {
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
void ue_context_suspend_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (security_context_present) {
    j.start_obj();
    j.write_int("id", 93);
    j.write_str("criticality", "reject");
    j.write_fieldname("value");
    security_context.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UEInformationTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_info_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {26, 210, 117, 148, 0, 209, 34, 414};
  return map_enum_number(names, 8, idx, "id");
}
bool ue_info_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {26, 210, 117, 148, 0, 209, 34, 414};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_info_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 26:
      return crit_e::reject;
    case 210:
      return crit_e::ignore;
    case 117:
      return crit_e::ignore;
    case 148:
      return crit_e::ignore;
    case 0:
      return crit_e::ignore;
    case 209:
      return crit_e::ignore;
    case 34:
      return crit_e::ignore;
    case 414:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_info_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 26:
      return presence_e::mandatory;
    case 210:
      return presence_e::optional;
    case 117:
      return presence_e::optional;
    case 148:
      return presence_e::optional;
    case 0:
      return presence_e::optional;
    case 209:
      return presence_e::optional;
    case 34:
      return presence_e::optional;
    case 414:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_info_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  nof_ies += nb_iot_ue_prio_present ? 1 : 0;
  nof_ies += ue_radio_cap_present ? 1 : 0;
  nof_ies += s_nssai_present ? 1 : 0;
  nof_ies += allowed_nssai_present ? 1 : 0;
  nof_ies += ue_diff_info_present ? 1 : 0;
  nof_ies += masked_imeisv_present ? 1 : 0;
  nof_ies += partially_allowed_nssai_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 26, crit_e{crit_e::reject}, [this, &bref]() { return five_g_s_tmsi.pack(bref); }));
  if (nb_iot_ue_prio_present) {
    HANDLE_CODE(pack_ie(bref, 210, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_integer(bref, nb_iot_ue_prio, (uint16_t)0u, (uint16_t)255u, true, true);
    }));
  }
  if (ue_radio_cap_present) {
    HANDLE_CODE(pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap.pack(bref); }));
  }
  if (s_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 148, crit_e{crit_e::ignore}, [this, &bref]() { return s_nssai.pack(bref); }));
  }
  if (allowed_nssai_present) {
    HANDLE_CODE(pack_ie(
        bref, 0, crit_e{crit_e::ignore}, [this, &bref]() { return pack_dyn_seq_of(bref, allowed_nssai, 1, 8, true); }));
  }
  if (ue_diff_info_present) {
    HANDLE_CODE(pack_ie(bref, 209, crit_e{crit_e::ignore}, [this, &bref]() { return ue_diff_info.pack(bref); }));
  }
  if (masked_imeisv_present) {
    HANDLE_CODE(pack_ie(bref, 34, crit_e{crit_e::ignore}, [this, &bref]() { return masked_imeisv.pack(bref); }));
  }
  if (partially_allowed_nssai_present) {
    HANDLE_CODE(pack_ie(bref, 414, crit_e{crit_e::ignore}, [this, &bref]() {
      return pack_dyn_seq_of(bref, partially_allowed_nssai, 1, 8, true);
    }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_info_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_info_transfer_ies_o::get_crit));

    switch (id) {
      case 26: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return five_g_s_tmsi.unpack(bref); }));
        break;
      }
      case 210: {
        nb_iot_ue_prio_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(nb_iot_ue_prio, bref, (uint16_t)0u, (uint16_t)255u, true, true);
        }));
        break;
      }
      case 117: {
        ue_radio_cap_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap.unpack(bref); }));
        break;
      }
      case 148: {
        s_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return s_nssai.unpack(bref); }));
        break;
      }
      case 0: {
        allowed_nssai_present = true;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return unpack_dyn_seq_of(allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      case 209: {
        ue_diff_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_diff_info.unpack(bref); }));
        break;
      }
      case 34: {
        masked_imeisv_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return masked_imeisv.unpack(bref); }));
        break;
      }
      case 414: {
        partially_allowed_nssai_present = true;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return unpack_dyn_seq_of(partially_allowed_nssai, bref, 1, 8, true); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_info_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 26);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  five_g_s_tmsi.to_json(j);
  j.end_obj();
  if (nb_iot_ue_prio_present) {
    j.start_obj();
    j.write_int("id", 210);
    j.write_str("criticality", "ignore");
    j.write_int("value", nb_iot_ue_prio);
    j.end_obj();
  }
  if (ue_radio_cap_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_str("value", ue_radio_cap.to_string());
    j.end_obj();
  }
  if (s_nssai_present) {
    j.start_obj();
    j.write_int("id", 148);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    s_nssai.to_json(j);
    j.end_obj();
  }
  if (allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 0);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  if (ue_diff_info_present) {
    j.start_obj();
    j.write_int("id", 209);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_diff_info.to_json(j);
    j.end_obj();
  }
  if (masked_imeisv_present) {
    j.start_obj();
    j.write_int("id", 34);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    masked_imeisv.to_json(j);
    j.end_obj();
  }
  if (partially_allowed_nssai_present) {
    j.start_obj();
    j.write_int("id", 414);
    j.write_str("criticality", "ignore");
    j.start_array("value");
    for (const auto& e1 : partially_allowed_nssai) {
      e1.to_json(j);
    }
    j.end_array();
    j.end_obj();
  }
  j.end_array();
}

// UERadioCapabilityCheckRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_radio_cap_check_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 117, 264};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_radio_cap_check_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 117, 264};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_radio_cap_check_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 117:
      return crit_e::ignore;
    case 264:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_radio_cap_check_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 117:
      return presence_e::optional;
    case 264:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_radio_cap_check_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += ue_radio_cap_present ? 1 : 0;
  nof_ies += ue_radio_cap_id_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  if (ue_radio_cap_present) {
    HANDLE_CODE(pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap.pack(bref); }));
  }
  if (ue_radio_cap_id_present) {
    HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_cap_check_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_radio_cap_check_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 117: {
        ue_radio_cap_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap.unpack(bref); }));
        break;
      }
      case 264: {
        ue_radio_cap_id_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_radio_cap_check_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  if (ue_radio_cap_present) {
    j.start_obj();
    j.write_int("id", 117);
    j.write_str("criticality", "ignore");
    j.write_str("value", ue_radio_cap.to_string());
    j.end_obj();
  }
  if (ue_radio_cap_id_present) {
    j.start_obj();
    j.write_int("id", 264);
    j.write_str("criticality", "reject");
    j.write_str("value", ue_radio_cap_id.to_string());
    j.end_obj();
  }
  j.end_array();
}

// UERadioCapabilityCheckResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_radio_cap_check_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 30, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool ue_radio_cap_check_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 30, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_radio_cap_check_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::ignore;
    case 85:
      return crit_e::ignore;
    case 30:
      return crit_e::reject;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_radio_cap_check_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 30:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_radio_cap_check_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::ignore}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 30, crit_e{crit_e::reject}, [this, &bref]() { return ims_voice_support_ind.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_cap_check_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_radio_cap_check_resp_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 30: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ims_voice_support_ind.unpack(bref); }));
        break;
      }
      case 19: {
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
void ue_radio_cap_check_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "ignore");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "ignore");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 30);
  j.write_str("criticality", "reject");
  j.write_str("value", ims_voice_support_ind.to_string());
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UERadioCapabilityIDMappingRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_radio_cap_id_map_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {264};
  return map_enum_number(names, 1, idx, "id");
}
bool ue_radio_cap_id_map_request_ies_o::is_id_valid(const uint32_t& id)
{
  return 264 == id;
}
crit_e ue_radio_cap_id_map_request_ies_o::get_crit(const uint32_t& id)
{
  if (id == 264) {
    return crit_e::reject;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e ue_radio_cap_id_map_request_ies_o::get_presence(const uint32_t& id)
{
  if (id == 264) {
    return presence_e::mandatory;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE ue_radio_cap_id_map_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 1;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_cap_id_map_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 1;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_radio_cap_id_map_request_ies_o::get_crit));

    if (id == 264) {
      {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
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
void ue_radio_cap_id_map_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 264);
  j.write_str("criticality", "reject");
  j.write_str("value", ue_radio_cap_id.to_string());
  j.end_obj();
  j.end_array();
}

// UERadioCapabilityIDMappingResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_radio_cap_id_map_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {264, 117, 19};
  return map_enum_number(names, 3, idx, "id");
}
bool ue_radio_cap_id_map_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {264, 117, 19};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_radio_cap_id_map_resp_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 264:
      return crit_e::reject;
    case 117:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_radio_cap_id_map_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 264:
      return presence_e::mandatory;
    case 117:
      return presence_e::mandatory;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_radio_cap_id_map_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 264, crit_e{crit_e::reject}, [this, &bref]() { return ue_radio_cap_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap.pack(bref); }));
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_cap_id_map_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_radio_cap_id_map_resp_ies_o::get_crit));

    switch (id) {
      case 264: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_id.unpack(bref); }));
        break;
      }
      case 117: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap.unpack(bref); }));
        break;
      }
      case 19: {
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
void ue_radio_cap_id_map_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 264);
  j.write_str("criticality", "reject");
  j.write_str("value", ue_radio_cap_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "ignore");
  j.write_str("value", ue_radio_cap.to_string());
  j.end_obj();
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UERadioCapabilityInfoIndicationIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_radio_cap_info_ind_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 117, 118, 265, 428};
  return map_enum_number(names, 6, idx, "id");
}
bool ue_radio_cap_info_ind_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 117, 118, 265, 428};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_radio_cap_info_ind_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 117:
      return crit_e::ignore;
    case 118:
      return crit_e::ignore;
    case 265:
      return crit_e::ignore;
    case 428:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_radio_cap_info_ind_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 117:
      return presence_e::mandatory;
    case 118:
      return presence_e::optional;
    case 265:
      return presence_e::optional;
    case 428:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_radio_cap_info_ind_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  nof_ies += ue_radio_cap_for_paging_present ? 1 : 0;
  nof_ies += ue_radio_cap_eutra_format_present ? 1 : 0;
  nof_ies += xr_dev_with2_rx_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 117, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap.pack(bref); }));
  if (ue_radio_cap_for_paging_present) {
    HANDLE_CODE(
        pack_ie(bref, 118, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap_for_paging.pack(bref); }));
  }
  if (ue_radio_cap_eutra_format_present) {
    HANDLE_CODE(
        pack_ie(bref, 265, crit_e{crit_e::ignore}, [this, &bref]() { return ue_radio_cap_eutra_format.pack(bref); }));
  }
  if (xr_dev_with2_rx_present) {
    HANDLE_CODE(pack_ie(bref, 428, crit_e{crit_e::ignore}, [this, &bref]() { return xr_dev_with2_rx.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_radio_cap_info_ind_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_radio_cap_info_ind_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 117: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap.unpack(bref); }));
        break;
      }
      case 118: {
        ue_radio_cap_for_paging_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_for_paging.unpack(bref); }));
        break;
      }
      case 265: {
        ue_radio_cap_eutra_format_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return ue_radio_cap_eutra_format.unpack(bref); }));
        break;
      }
      case 428: {
        xr_dev_with2_rx_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return xr_dev_with2_rx.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ue_radio_cap_info_ind_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 117);
  j.write_str("criticality", "ignore");
  j.write_str("value", ue_radio_cap.to_string());
  j.end_obj();
  if (ue_radio_cap_for_paging_present) {
    j.start_obj();
    j.write_int("id", 118);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    ue_radio_cap_for_paging.to_json(j);
    j.end_obj();
  }
  if (ue_radio_cap_eutra_format_present) {
    j.start_obj();
    j.write_int("id", 265);
    j.write_str("criticality", "ignore");
    j.write_str("value", ue_radio_cap_eutra_format.to_string());
    j.end_obj();
  }
  if (xr_dev_with2_rx_present) {
    j.start_obj();
    j.write_int("id", 428);
    j.write_str("criticality", "ignore");
    j.write_str("value", "true");
    j.end_obj();
  }
  j.end_array();
}

// UETNLABindingReleaseRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ue_tnla_binding_release_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85};
  return map_enum_number(names, 2, idx, "id");
}
bool ue_tnla_binding_release_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ue_tnla_binding_release_request_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ue_tnla_binding_release_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ue_tnla_binding_release_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ue_tnla_binding_release_request_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ue_tnla_binding_release_request_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
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
void ue_tnla_binding_release_request_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.end_array();
}

// UplinkNASTransport-IEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ul_nas_transport_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 38, 121, 239, 246, 247};
  return map_enum_number(names, 7, idx, "id");
}
bool ul_nas_transport_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 38, 121, 239, 246, 247};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_nas_transport_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 38:
      return crit_e::reject;
    case 121:
      return crit_e::ignore;
    case 239:
      return crit_e::reject;
    case 246:
      return crit_e::reject;
    case 247:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ul_nas_transport_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 38:
      return presence_e::mandatory;
    case 121:
      return presence_e::mandatory;
    case 239:
      return presence_e::optional;
    case 246:
      return presence_e::optional;
    case 247:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ul_nas_transport_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += w_agf_id_info_present ? 1 : 0;
  nof_ies += tngf_id_info_present ? 1 : 0;
  nof_ies += twif_id_info_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 38, crit_e{crit_e::reject}, [this, &bref]() { return nas_pdu.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 121, crit_e{crit_e::ignore}, [this, &bref]() { return user_location_info.pack(bref); }));
  if (w_agf_id_info_present) {
    HANDLE_CODE(pack_ie(bref, 239, crit_e{crit_e::reject}, [this, &bref]() { return w_agf_id_info.pack(bref); }));
  }
  if (tngf_id_info_present) {
    HANDLE_CODE(pack_ie(bref, 246, crit_e{crit_e::reject}, [this, &bref]() { return tngf_id_info.pack(bref); }));
  }
  if (twif_id_info_present) {
    HANDLE_CODE(pack_ie(bref, 247, crit_e{crit_e::reject}, [this, &bref]() { return twif_id_info.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_nas_transport_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_nas_transport_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 38: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nas_pdu.unpack(bref); }));
        break;
      }
      case 121: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return user_location_info.unpack(bref); }));
        break;
      }
      case 239: {
        w_agf_id_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return w_agf_id_info.unpack(bref); }));
        break;
      }
      case 246: {
        tngf_id_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return tngf_id_info.unpack(bref); }));
        break;
      }
      case 247: {
        twif_id_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return twif_id_info.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ul_nas_transport_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 38);
  j.write_str("criticality", "reject");
  j.write_str("value", nas_pdu.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 121);
  j.write_str("criticality", "ignore");
  j.write_fieldname("value");
  user_location_info.to_json(j);
  j.end_obj();
  if (w_agf_id_info_present) {
    j.start_obj();
    j.write_int("id", 239);
    j.write_str("criticality", "reject");
    j.write_str("value", w_agf_id_info.to_string());
    j.end_obj();
  }
  if (tngf_id_info_present) {
    j.start_obj();
    j.write_int("id", 246);
    j.write_str("criticality", "reject");
    j.write_str("value", tngf_id_info.to_string());
    j.end_obj();
  }
  if (twif_id_info_present) {
    j.start_obj();
    j.write_int("id", 247);
    j.write_str("criticality", "reject");
    j.write_str("value", twif_id_info.to_string());
    j.end_obj();
  }
  j.end_array();
}

// UplinkNonUEAssociatedNRPPaTransportIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ul_non_ue_associated_nrppa_transport_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {89, 46};
  return map_enum_number(names, 2, idx, "id");
}
bool ul_non_ue_associated_nrppa_transport_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {89, 46};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_non_ue_associated_nrppa_transport_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 89:
      return crit_e::reject;
    case 46:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ul_non_ue_associated_nrppa_transport_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 89:
      return presence_e::mandatory;
    case 46:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ul_non_ue_associated_nrppa_transport_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() { return routing_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 46, crit_e{crit_e::reject}, [this, &bref]() { return nrppa_pdu.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_non_ue_associated_nrppa_transport_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_non_ue_associated_nrppa_transport_ies_o::get_crit));

    switch (id) {
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return routing_id.unpack(bref); }));
        break;
      }
      case 46: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nrppa_pdu.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ul_non_ue_associated_nrppa_transport_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_str("value", routing_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 46);
  j.write_str("criticality", "reject");
  j.write_str("value", nrppa_pdu.to_string());
  j.end_obj();
  j.end_array();
}

// UplinkRANConfigurationTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ul_ran_cfg_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {99, 158, 251};
  return map_enum_number(names, 3, idx, "id");
}
bool ul_ran_cfg_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {99, 158, 251};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_ran_cfg_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 99:
      return crit_e::ignore;
    case 158:
      return crit_e::ignore;
    case 251:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ul_ran_cfg_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 99:
      return presence_e::optional;
    case 158:
      return presence_e::optional;
    case 251:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ul_ran_cfg_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += son_cfg_transfer_ul_present ? 1 : 0;
  nof_ies += endc_son_cfg_transfer_ul_present ? 1 : 0;
  nof_ies += intersys_son_cfg_transfer_ul_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (son_cfg_transfer_ul_present) {
    HANDLE_CODE(pack_ie(bref, 99, crit_e{crit_e::ignore}, [this, &bref]() { return son_cfg_transfer_ul.pack(bref); }));
  }
  if (endc_son_cfg_transfer_ul_present) {
    HANDLE_CODE(
        pack_ie(bref, 158, crit_e{crit_e::ignore}, [this, &bref]() { return endc_son_cfg_transfer_ul.pack(bref); }));
  }
  if (intersys_son_cfg_transfer_ul_present) {
    HANDLE_CODE(pack_ie(
        bref, 251, crit_e{crit_e::ignore}, [this, &bref]() { return intersys_son_cfg_transfer_ul.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ran_cfg_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_ran_cfg_transfer_ies_o::get_crit));

    switch (id) {
      case 99: {
        son_cfg_transfer_ul_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return son_cfg_transfer_ul.unpack(bref); }));
        break;
      }
      case 158: {
        endc_son_cfg_transfer_ul_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return endc_son_cfg_transfer_ul.unpack(bref); }));
        break;
      }
      case 251: {
        intersys_son_cfg_transfer_ul_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return intersys_son_cfg_transfer_ul.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }

  return OCUDUASN_SUCCESS;
}
void ul_ran_cfg_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (son_cfg_transfer_ul_present) {
    j.start_obj();
    j.write_int("id", 99);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    son_cfg_transfer_ul.to_json(j);
    j.end_obj();
  }
  if (endc_son_cfg_transfer_ul_present) {
    j.start_obj();
    j.write_int("id", 158);
    j.write_str("criticality", "ignore");
    j.write_str("value", endc_son_cfg_transfer_ul.to_string());
    j.end_obj();
  }
  if (intersys_son_cfg_transfer_ul_present) {
    j.start_obj();
    j.write_int("id", 251);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    intersys_son_cfg_transfer_ul.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UplinkRANEarlyStatusTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ul_ran_early_status_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 268};
  return map_enum_number(names, 3, idx, "id");
}
bool ul_ran_early_status_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 268};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_ran_early_status_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 268:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ul_ran_early_status_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 268:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ul_ran_early_status_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 268, crit_e{crit_e::reject}, [this, &bref]() {
    return early_status_transfer_transparent_container.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ran_early_status_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_ran_early_status_transfer_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 268: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(
            bref, [this, &bref]() { return early_status_transfer_transparent_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ul_ran_early_status_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 268);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  early_status_transfer_transparent_container.to_json(j);
  j.end_obj();
  j.end_array();
}

// UplinkRANStatusTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ul_ran_status_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 84};
  return map_enum_number(names, 3, idx, "id");
}
bool ul_ran_status_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 84};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_ran_status_transfer_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 84:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ul_ran_status_transfer_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 84:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ul_ran_status_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 3;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 84, crit_e{crit_e::reject}, [this, &bref]() {
    return ran_status_transfer_transparent_container.pack(bref);
  }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ran_status_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 3;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_ran_status_transfer_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 84: {
        nof_mandatory_ies--;
        HANDLE_CODE(
            unpack_ie_value(bref, [this, &bref]() { return ran_status_transfer_transparent_container.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ul_ran_status_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 84);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  ran_status_transfer_transparent_container.to_json(j);
  j.end_obj();
  j.end_array();
}

// UplinkRIMInformationTransferIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ul_rim_info_transfer_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {175};
  return map_enum_number(names, 1, idx, "id");
}
bool ul_rim_info_transfer_ies_o::is_id_valid(const uint32_t& id)
{
  return 175 == id;
}
crit_e ul_rim_info_transfer_ies_o::get_crit(const uint32_t& id)
{
  if (id == 175) {
    return crit_e::ignore;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}
presence_e ul_rim_info_transfer_ies_o::get_presence(const uint32_t& id)
{
  if (id == 175) {
    return presence_e::optional;
  }
  asn1::log_error("The id={} is not recognized", id);
  return {};
}

OCUDUASN_CODE ul_rim_info_transfer_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 0;
  nof_ies += rim_info_transfer_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  if (rim_info_transfer_present) {
    HANDLE_CODE(pack_ie(bref, 175, crit_e{crit_e::ignore}, [this, &bref]() { return rim_info_transfer.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_rim_info_transfer_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_rim_info_transfer_ies_o::get_crit));

    if (id == 175) {
      {
        rim_info_transfer_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return rim_info_transfer.unpack(bref); }));
      }
    } else {
      HANDLE_CODE(skip_ignored_ie(bref, crit, id));
    }
  }

  return OCUDUASN_SUCCESS;
}
void ul_rim_info_transfer_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  if (rim_info_transfer_present) {
    j.start_obj();
    j.write_int("id", 175);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    rim_info_transfer.to_json(j);
    j.end_obj();
  }
  j.end_array();
}

// UplinkUEAssociatedNRPPaTransportIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t ul_ue_associated_nrppa_transport_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {10, 85, 89, 46};
  return map_enum_number(names, 4, idx, "id");
}
bool ul_ue_associated_nrppa_transport_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {10, 85, 89, 46};
  for (const auto& o : names) {
    if (o == id) {
      return true;
    }
  }
  return false;
}
crit_e ul_ue_associated_nrppa_transport_ies_o::get_crit(const uint32_t& id)
{
  switch (id) {
    case 10:
      return crit_e::reject;
    case 85:
      return crit_e::reject;
    case 89:
      return crit_e::reject;
    case 46:
      return crit_e::reject;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e ul_ue_associated_nrppa_transport_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 10:
      return presence_e::mandatory;
    case 85:
      return presence_e::mandatory;
    case 89:
      return presence_e::mandatory;
    case 46:
      return presence_e::mandatory;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE ul_ue_associated_nrppa_transport_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 10, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, amf_ue_ngap_id, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 85, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, ran_ue_ngap_id, (uint64_t)0u, (uint64_t)4294967295u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 89, crit_e{crit_e::reject}, [this, &bref]() { return routing_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 46, crit_e{crit_e::reject}, [this, &bref]() { return nrppa_pdu.pack(bref); }));

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE ul_ue_associated_nrppa_transport_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 4;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &ul_ue_associated_nrppa_transport_ies_o::get_crit));

    switch (id) {
      case 10: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(amf_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)1099511627775u, false, true);
        }));
        break;
      }
      case 85: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(ran_ue_ngap_id, bref, (uint64_t)0u, (uint64_t)4294967295u, false, true);
        }));
        break;
      }
      case 89: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return routing_id.unpack(bref); }));
        break;
      }
      case 46: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return nrppa_pdu.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
        break;
    }
  }
  if (nof_mandatory_ies > 0) {
    asn1::log_error("Mandatory fields are missing\n");

    return OCUDUASN_ERROR_DECODE_FAIL;
  }
  return OCUDUASN_SUCCESS;
}
void ul_ue_associated_nrppa_transport_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 10);
  j.write_str("criticality", "reject");
  j.write_uint("value", amf_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 85);
  j.write_str("criticality", "reject");
  j.write_uint("value", ran_ue_ngap_id);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 89);
  j.write_str("criticality", "reject");
  j.write_str("value", routing_id.to_string());
  j.end_obj();
  j.start_obj();
  j.write_int("id", 46);
  j.write_str("criticality", "reject");
  j.write_str("value", nrppa_pdu.to_string());
  j.end_obj();
  j.end_array();
}

// WriteReplaceWarningRequestIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t write_replace_warning_request_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {35, 95, 122, 87, 47, 125, 124, 20, 123, 17, 141};
  return map_enum_number(names, 11, idx, "id");
}
bool write_replace_warning_request_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {35, 95, 122, 87, 47, 125, 124, 20, 123, 17, 141};
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
    case 35:
      return crit_e::reject;
    case 95:
      return crit_e::reject;
    case 122:
      return crit_e::ignore;
    case 87:
      return crit_e::reject;
    case 47:
      return crit_e::reject;
    case 125:
      return crit_e::ignore;
    case 124:
      return crit_e::ignore;
    case 20:
      return crit_e::ignore;
    case 123:
      return crit_e::ignore;
    case 17:
      return crit_e::reject;
    case 141:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e write_replace_warning_request_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 35:
      return presence_e::mandatory;
    case 95:
      return presence_e::mandatory;
    case 122:
      return presence_e::optional;
    case 87:
      return presence_e::mandatory;
    case 47:
      return presence_e::mandatory;
    case 125:
      return presence_e::optional;
    case 124:
      return presence_e::optional;
    case 20:
      return presence_e::optional;
    case 123:
      return presence_e::optional;
    case 17:
      return presence_e::optional;
    case 141:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE write_replace_warning_request_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 4;
  nof_ies += warning_area_list_present ? 1 : 0;
  nof_ies += warning_type_present ? 1 : 0;
  nof_ies += warning_security_info_present ? 1 : 0;
  nof_ies += data_coding_scheme_present ? 1 : 0;
  nof_ies += warning_msg_contents_present ? 1 : 0;
  nof_ies += concurrent_warning_msg_ind_present ? 1 : 0;
  nof_ies += warning_area_coordinates_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 35, crit_e{crit_e::reject}, [this, &bref]() { return msg_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::reject}, [this, &bref]() { return serial_num.pack(bref); }));
  if (warning_area_list_present) {
    HANDLE_CODE(pack_ie(bref, 122, crit_e{crit_e::ignore}, [this, &bref]() { return warning_area_list.pack(bref); }));
  }
  HANDLE_CODE(pack_ie(bref, 87, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, repeat_period, (uint32_t)0u, (uint32_t)131071u, false, true);
  }));
  HANDLE_CODE(pack_ie(bref, 47, crit_e{crit_e::reject}, [this, &bref]() {
    return pack_integer(bref, nof_broadcasts_requested, (uint32_t)0u, (uint32_t)65535u, false, true);
  }));
  if (warning_type_present) {
    HANDLE_CODE(pack_ie(bref, 125, crit_e{crit_e::ignore}, [this, &bref]() { return warning_type.pack(bref); }));
  }
  if (warning_security_info_present) {
    HANDLE_CODE(
        pack_ie(bref, 124, crit_e{crit_e::ignore}, [this, &bref]() { return warning_security_info.pack(bref); }));
  }
  if (data_coding_scheme_present) {
    HANDLE_CODE(pack_ie(bref, 20, crit_e{crit_e::ignore}, [this, &bref]() { return data_coding_scheme.pack(bref); }));
  }
  if (warning_msg_contents_present) {
    HANDLE_CODE(
        pack_ie(bref, 123, crit_e{crit_e::ignore}, [this, &bref]() { return warning_msg_contents.pack(bref); }));
  }
  if (concurrent_warning_msg_ind_present) {
    HANDLE_CODE(
        pack_ie(bref, 17, crit_e{crit_e::reject}, [this, &bref]() { return concurrent_warning_msg_ind.pack(bref); }));
  }
  if (warning_area_coordinates_present) {
    HANDLE_CODE(
        pack_ie(bref, 141, crit_e{crit_e::ignore}, [this, &bref]() { return warning_area_coordinates.pack(bref); }));
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
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &write_replace_warning_request_ies_o::get_crit));

    switch (id) {
      case 35: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return msg_id.unpack(bref); }));
        break;
      }
      case 95: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return serial_num.unpack(bref); }));
        break;
      }
      case 122: {
        warning_area_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return warning_area_list.unpack(bref); }));
        break;
      }
      case 87: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(repeat_period, bref, (uint32_t)0u, (uint32_t)131071u, false, true);
        }));
        break;
      }
      case 47: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() {
          return unpack_integer(nof_broadcasts_requested, bref, (uint32_t)0u, (uint32_t)65535u, false, true);
        }));
        break;
      }
      case 125: {
        warning_type_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return warning_type.unpack(bref); }));
        break;
      }
      case 124: {
        warning_security_info_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return warning_security_info.unpack(bref); }));
        break;
      }
      case 20: {
        data_coding_scheme_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return data_coding_scheme.unpack(bref); }));
        break;
      }
      case 123: {
        warning_msg_contents_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return warning_msg_contents.unpack(bref); }));
        break;
      }
      case 17: {
        concurrent_warning_msg_ind_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return concurrent_warning_msg_ind.unpack(bref); }));
        break;
      }
      case 141: {
        warning_area_coordinates_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return warning_area_coordinates.unpack(bref); }));
        break;
      }
      default:
        HANDLE_CODE(skip_ignored_ie(bref, crit, id));
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
  j.write_int("id", 35);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  msg_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 95);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  serial_num.to_json(j);
  j.end_obj();
  if (warning_area_list_present) {
    j.start_obj();
    j.write_int("id", 122);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    warning_area_list.to_json(j);
    j.end_obj();
  }
  j.start_obj();
  j.write_int("id", 87);
  j.write_str("criticality", "reject");
  j.write_int("value", repeat_period);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 47);
  j.write_str("criticality", "reject");
  j.write_int("value", nof_broadcasts_requested);
  j.end_obj();
  if (warning_type_present) {
    j.start_obj();
    j.write_int("id", 125);
    j.write_str("criticality", "ignore");
    j.write_str("value", warning_type.to_string());
    j.end_obj();
  }
  if (warning_security_info_present) {
    j.start_obj();
    j.write_int("id", 124);
    j.write_str("criticality", "ignore");
    j.write_str("value", warning_security_info.to_string());
    j.end_obj();
  }
  if (data_coding_scheme_present) {
    j.start_obj();
    j.write_int("id", 20);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    data_coding_scheme.to_json(j);
    j.end_obj();
  }
  if (warning_msg_contents_present) {
    j.start_obj();
    j.write_int("id", 123);
    j.write_str("criticality", "ignore");
    j.write_str("value", warning_msg_contents.to_string());
    j.end_obj();
  }
  if (concurrent_warning_msg_ind_present) {
    j.start_obj();
    j.write_int("id", 17);
    j.write_str("criticality", "reject");
    j.write_str("value", "true");
    j.end_obj();
  }
  if (warning_area_coordinates_present) {
    j.start_obj();
    j.write_int("id", 141);
    j.write_str("criticality", "ignore");
    j.write_str("value", warning_area_coordinates.to_string());
    j.end_obj();
  }
  j.end_array();
}

// WriteReplaceWarningResponseIEs ::= OBJECT SET OF NGAP-PROTOCOL-IES
uint32_t write_replace_warning_resp_ies_o::idx_to_id(uint32_t idx)
{
  static const uint32_t names[] = {35, 95, 13, 19};
  return map_enum_number(names, 4, idx, "id");
}
bool write_replace_warning_resp_ies_o::is_id_valid(const uint32_t& id)
{
  static const uint32_t names[] = {35, 95, 13, 19};
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
    case 35:
      return crit_e::reject;
    case 95:
      return crit_e::reject;
    case 13:
      return crit_e::ignore;
    case 19:
      return crit_e::ignore;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}
presence_e write_replace_warning_resp_ies_o::get_presence(const uint32_t& id)
{
  switch (id) {
    case 35:
      return presence_e::mandatory;
    case 95:
      return presence_e::mandatory;
    case 13:
      return presence_e::optional;
    case 19:
      return presence_e::optional;
    default:
      asn1::log_error("The id={} is not recognized", id);
  }
  return {};
}

OCUDUASN_CODE write_replace_warning_resp_ies_container::pack(bit_ref& bref) const
{
  uint32_t nof_ies = 2;
  nof_ies += broadcast_completed_area_list_present ? 1 : 0;
  nof_ies += crit_diagnostics_present ? 1 : 0;
  HANDLE_CODE(pack_length(bref, nof_ies, 0u, 65535u, true));

  HANDLE_CODE(pack_ie(bref, 35, crit_e{crit_e::reject}, [this, &bref]() { return msg_id.pack(bref); }));
  HANDLE_CODE(pack_ie(bref, 95, crit_e{crit_e::reject}, [this, &bref]() { return serial_num.pack(bref); }));
  if (broadcast_completed_area_list_present) {
    HANDLE_CODE(pack_ie(
        bref, 13, crit_e{crit_e::ignore}, [this, &bref]() { return broadcast_completed_area_list.pack(bref); }));
  }
  if (crit_diagnostics_present) {
    HANDLE_CODE(pack_ie(bref, 19, crit_e{crit_e::ignore}, [this, &bref]() { return crit_diagnostics.pack(bref); }));
  }

  return OCUDUASN_SUCCESS;
}
OCUDUASN_CODE write_replace_warning_resp_ies_container::unpack(cbit_ref& bref)
{
  uint32_t nof_ies = 0;
  HANDLE_CODE(unpack_length(nof_ies, bref, 0u, 65535u, true));

  uint32_t nof_mandatory_ies = 2;

  for (; nof_ies > 0; --nof_ies) {
    uint32_t id;
    crit_e   crit;
    HANDLE_CODE(unpack_ie_header(bref, id, crit, &write_replace_warning_resp_ies_o::get_crit));

    switch (id) {
      case 35: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return msg_id.unpack(bref); }));
        break;
      }
      case 95: {
        nof_mandatory_ies--;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return serial_num.unpack(bref); }));
        break;
      }
      case 13: {
        broadcast_completed_area_list_present = true;
        HANDLE_CODE(unpack_ie_value(bref, [this, &bref]() { return broadcast_completed_area_list.unpack(bref); }));
        break;
      }
      case 19: {
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
void write_replace_warning_resp_ies_container::to_json(json_writer& j) const
{
  j.start_array();
  j.start_obj();
  j.write_int("id", 35);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  msg_id.to_json(j);
  j.end_obj();
  j.start_obj();
  j.write_int("id", 95);
  j.write_str("criticality", "reject");
  j.write_fieldname("value");
  serial_num.to_json(j);
  j.end_obj();
  if (broadcast_completed_area_list_present) {
    j.start_obj();
    j.write_int("id", 13);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    broadcast_completed_area_list.to_json(j);
    j.end_obj();
  }
  if (crit_diagnostics_present) {
    j.start_obj();
    j.write_int("id", 19);
    j.write_str("criticality", "ignore");
    j.write_fieldname("value");
    crit_diagnostics.to_json(j);
    j.end_obj();
  }
  j.end_array();
}
